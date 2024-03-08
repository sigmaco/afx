#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/afxQwadro.h"
#include "qwadro/ux/afxApplication.h"
#include "qwadro/core/afxDebug.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/io/afxMouse.h"
#include "../src/techid/afxMD5Model.h"
#include "../src/cadio/afxWavefrontObject.h"
#include "../src/rad3d/afxGranny2Model.h"


#define ENABLE_DRAW 1

#ifdef ENABLE_DRAW
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sim/rendering/awxRenderer.h"
#endif

afxClock StartClock;
afxClock LastClock;

awxBody cubeBod = NIL;
afxModel cubeMdl = NIL;
afxMesh cube = NIL;
const afxReal CameraSpeed = 30.0f;
afxSimulation sim = NIL;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
awxRenderer rnd = NIL;
awxAnimation ani = NIL;
afxCamera cam = NIL;
awxBody bod = NIL;

awxPose *sharedLocalPose;
awxWorldPose *sharedWorldPose;

afxBool DrawInputProc(afxDrawInput din, afxDrawEvent const* ev) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    switch (ev->id)
    {
    default:
    {
        awxRenderer rnd = AfxGetDrawInputUdd(din);
        afxDrawContext dctx = AfxGetDrawInputContext(din);
        
        afxDrawScript dscr;

        if (AfxAcquireDrawScripts(din, 0, 1, &dscr)) AfxThrowError();
        else
        {
            if (AfxRecordDrawScript(dscr, afxDrawScriptUsage_ONCE)) AfxThrowError();
            else
            {

                afxNat outBufIdx = 0;
                AfxRequestDrawOutputBuffer(dout, 0, &outBufIdx);
                afxRaster surf;
                AfxGetDrawOutputBuffer(dout, outBufIdx, 1, &surf);
                afxCanvas canv;
                AfxGetDrawOutputCanvas(dout, outBufIdx, 1, &canv);
                AfxAssertObjects(1, &surf, afxFcc_RAS);

                AwxCmdBeginSceneRendering(dscr, rnd, rnd->activeCam, NIL, canv);


                if (bod)
                    AwxCmdDrawBodies(dscr, rnd, 1, &bod);

                //if (cubeBod)
                    //AwxCmdDrawBodies(dscr, rnd, 1, &cubeBod);

                AwxCmdDrawTestIndexed(dscr, rnd);

                AfxDrawSky(dscr, &rnd->sky);

                AwxCmdEndSceneRendering(dscr, rnd);

                afxSemaphore dscrCompleteSem = NIL;

                if (AfxCompileDrawScript(dscr)) AfxThrowError();
                else if (AfxExecuteDrawScripts(din, 1, &dscr, NIL, NIL, &dscrCompleteSem))
                    AfxThrowError();

                if (AfxPresentDrawBuffers(din, 1, &dout, &outBufIdx, &dscrCompleteSem))
                    AfxThrowError();
            }
        }
        break;
    }
    }
    return FALSE;
}

void UpdateFrameMovement(afxReal64 DeltaTime)
{
    afxError err = AFX_ERR_NONE;

    afxReal64 MovementThisFrame = DeltaTime * CameraSpeed;

    // Note: because the NegZ axis is forward, we have to invert the way you'd normally think about the 'W' or 'S' key's action.
    afxReal64 ForwardSpeed = (AfxKeyIsPressed(0, AFX_KEY_W) ? -1 : 0.0f) + (AfxKeyIsPressed(0, AFX_KEY_S) ? 1 : 0.0f);
    afxReal64 RightSpeed = (AfxKeyIsPressed(0, AFX_KEY_A) ? -1 : 0.0f) + (AfxKeyIsPressed(0, AFX_KEY_D) ? 1 : 0.0f);
    afxReal64 UpSpeed = (AfxKeyIsPressed(0, AFX_KEY_Q) ? -1 : 0.0f) + (AfxKeyIsPressed(0, AFX_KEY_E) ? 1 : 0.0f);
    afxV3d v =
    {
        MovementThisFrame * RightSpeed,
        MovementThisFrame * UpSpeed,
        MovementThisFrame * ForwardSpeed
    };
    AfxApplyCameraMotion(cam, v);
    
    afxReal64 curTime;
    AfxGetExecutionTime(&curTime, NIL);
    AwxReclockLinkedMotors(bod, curTime);

#if 0
    afxM4d m;
    AfxGetModelInitialPlacement(bod->mdl, m);
    AwxSampleBodyAnimationsAcceleratedLOD(bod, bod->cachedBoneCnt, m, sharedLocalPose, sharedWorldPose, 0.0);
#if 0
    afxM4d CompositeMatrix;
    AfxEnumerateRiggedMeshes(bod->mdl, i);
    AfxBuildIndexedCompositeBuffer(bod->cachedSkl, sharedWorldPose, AfxGetRiggedMeshMapping(bod->mdl, i), 1, &CompositeMatrix);
#endif
#endif
}

_AFXEXPORT afxResult Once(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    awxSimulationConfig simSpec = { 0 };
    AfxRecomputeAabb(simSpec.extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.unitsPerMeter = 1.f;
    AfxSetV3d(simSpec.right,    1, 0, 0);
    AfxSetV3d(simSpec.up,       0, 1, 0);
    AfxSetV3d(simSpec.back,     0, 0, 1);
    AfxZeroV3d(simSpec.origin);
    AfxAcquireSimulations(1, &simSpec, &sim);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    
    afxDrawOutputConfig doutConfig = {0};
    doutConfig.auxDsFmt[0] = afxPixelFormat_D24;
    AfxOpenDrawOutputs(0, 1, &doutConfig, &dout);
    AfxAssert(dout);
    AfxReconnectDrawOutput(dout, dctx);

    awxRendererConfig rndConf = { 0 };
    rndConf.dinProc = DrawInputProc;
    AwxAcquireRenderers(sim, 1, &rnd, &rndConf);

    afxUri uriMap2;
    afxString str;
    afxModel mdl;
    //AfxAcquireSky();

    //AfxFormatUri(&uri, "%.*s/scenario/zero/zero.obj", AfxPushString(AfxApplication_GetArtDirectory(app)));
    //afxArray assets;
    //AfxDeployArray(&assets, NIL, 0, sizeof(void*));
    //AfxLoadAssets(sim, &uri);

    afxUri uriMap;
    AfxMakeUri(&uriMap, "art/actor/hellknight/hellknight.md5mesh", 0);
    //AfxSimulationLoadMD5Assets(sim, &uriMap, NIL);

    //AfxFindResources(cad2, afxFcc_MDL, 1, &uriMap2, &mdl);
    //AfxAcquireModels(sim, 1, &uriMap2, &mdl);

    //AfxMakeUri(&uriMap, "art/scenario/cod-mw3/ny_manhattan/ny_manhattan.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/control-statiopn/uploads_files_3580612_control+statiopn.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/cs_rio/cs_rio_base.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/resdogs/resdogs.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/TV-Stand-5/TV-Stand-5.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/gtabr/gtabr.obj", 0);
    //AfxMakeUri(&uriMap, "art/f16/f16.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/bibliotheca/bibliotheca-uvless.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/zero/zero.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/SpaceStation/SpaceStation.obj", 0);
    AfxMakeUri(&uriMap, "art/object/container/container.obj", 0);
    //AfxSimulationLoadObjAssets(sim, &uriMap, NIL);

    //AfxMakeUri(&uriMap, "art/building/fort/SPC_Fort_Center.gr2", 0);
    AfxMakeUri(&uriMap, "art/building/mill/w_mill_3age.gr2", 0);
    //AfxMakeUri(&uriMap, "art/actor/p_explorer.gr2", 0);
    mdl = AwxLoadModelsFromGrn3d2(sim, &uriMap, 0); // .m4d

    
    awxAsset cad;
    //AfxLoadAssetsFromWavefrontObj(sim, NIL, 1, &uriMap, &cad);
    //AfxLoadAssetsFromMd5(sim, NIL, 1, &uriMap, &cad);

    afxV3d atv;
    afxM3d ltm, iltm;
    AfxComputeBasisConversion(sim, 1.0, AFX_V3D_X, AFX_V3D_Y, AFX_V3D_Z, AFX_V3D_ZERO, ltm, iltm, atv);
    //AfxTransformAssets(ltm, iltm, atv, 1e-5f, 1e-5f, 3, 1, &cad); // renormalize e reordene triângulos
    //AfxTransformModels(ltm, iltm, 1e-5f, atv, 1e-5f, 3, 1, &mdl);

    AfxExcerptUriName(&uriMap, &uriMap2);
    //AfxFindResources(cad, afxFcc_MDL, 1, AfxGetUriString(&uriMap2), &mdl);
    //AfxAcquireModels(sim, 1, &uriMap2, &mdl);
    // TODO FetchModel(/dir/to/file)

    AwxEmbodyModel(mdl, 1, &bod);
    AfxAssert(bod);

    AfxMakeUri(&uriMap, "art/building/mill/w_mill_3age_idle.gr2", 0);
    //AfxMakeUri(&uriMap, "art/actor/knockout_recover.gr2", 0);
    ani = AwxLoadAnimationsFromGrn3d2(sim, &uriMap, 0); // .k4d

    

    awxMotor moto = AwxRunAnimation(bod, 0, ani);
    //awxMotor moto2 = AwxRunAnimation(bod, 0, ani);
    
    AwxAdjustMotorIterations(moto, 0);
    //AwxAdjustMotorIterations(moto2, 0);
    //AwxReleaseOnceUnusedMotors(1, &moto);

    cube = AfxBuildCubeMesh(sim, -200.0);
    //cube = AfxBuildParallelepipedMesh(sim, 100, 100);
    //cube = AfxBuildParallelepipedMesh(sim, AfxSpawnV3d(100, 100, 10));
    //cube = AfxBuildDomeMesh(sim, 100.0, 4);
    
    afxStringCatalog strc;
    AfxAcquireStringCatalogs(1, &strc);

    afxModelBlueprint mdlb = { 0 };
    mdlb.meshes = &cube;
    mdlb.mshCnt = 1;
    AfxMakeString32(&mdlb.id, &AfxStaticString("cube"));
    mdlb.skl = AfxGetModelSkeleton(mdl);
    mdlb.strc = strc;
    AfxAssembleModel(sim, 1, &mdlb, &cubeMdl);
    AwxEmbodyModel(cubeMdl, 1, &cubeBod);
#if 0
    mdl = AfxSimulationFindModel(sim, &str);
    body = AfxSimulationAcquireBody(sim, &str, NIL, mdl);
    AfxAssert(body);
#endif
    //AfxCopyStringLiteral(&str, 0, "zero");
    //awxBody body2;
    //AfxAcquireBody(&body2, sim, &str, AfxFindModel(sim, &str));
    //AfxAssert(body2);

    AfxMakeUri(&uriMap, "tmp/test.skl", 0);
    afxFile f;
    AfxOpenFiles(afxFileFlag_W, 1, &uriMap, &f);
    afxStream fs = AfxGetFileStream(f);
    _afxMddFileData hdr = { 0 };
#if 0
    afxSize firstSklOffset = AfxAskStreamPosn(fs);
    AfxSerializeSkeletons(fs, 1, (void*[]) { AfxGetModelSkeleton(mdl) });
    hdr.sklCnt = 1;
    hdr.sklDirBaseOffset = firstSklOffset;
#endif

#if !0
    afxSize firstMshtOffset = AfxAskStreamPosn(fs);
    AfxSerializeMeshTopologies(fs, 1, (void*[]) { AfxGetMeshTopology(cube) });
    hdr.mshtCnt = 1;
    hdr.mshtDirBaseOffset = firstMshtOffset;
#endif

    AfxWriteStream(fs, &hdr, sizeof(hdr), 0);
    AfxReleaseObjects(1, &f);

    AfxOpenFiles(afxFileFlag_R, 1, &uriMap, &f);
    fs = AfxGetFileStream(f);
    AfxGoToStreamEnd(fs, sizeof(hdr));
    AfxReadStream(fs, &hdr, sizeof(hdr), 1);
    
#if 0
    AfxAssert(hdr.sklCnt);
    AfxGoToStreamBegin(fs, hdr.sklDirBaseOffset);
    afxSkeleton skl;
    AfxDeserializeSkeletons(fs, sim, 1, &skl);
#endif

#if 0
    AfxGoToStreamBegin(fs, hdr.mshtDirBaseOffset);
    afxMeshTopology msht;
    AfxDeserializeMeshTopologies(fs, sim, 1, &msht);
#endif

    AfxReleaseObjects(1, &f);
    
    AfxAcquireCameras(rnd->din, 1, &cam);
    AfxAssert(cam);
    //AfxSetCameraFov(cam, AFX_PI / 4.0);
    //cam->farClip = -100000.0;
    //AfxApplyCameraOffset(cam, AfxSpawnV3d(0, 1.1, 0));

    //AfxAttachViewpoint(vpnt, cam);

    rnd->activeCam = cam;

    afxMouse mse = AfxGetMouse(0);
    AfxObjectInstallEventFilter(mse, cam);
    
    AfxGetClock(&StartClock);
    LastClock = StartClock;

    AfxAcquirePoses(sim, 1, (afxNat[]) { 256 }, &sharedLocalPose);
    AfxAcquireWorldPoses(sim, 1, (afxNat[]) { 256 }, NIL, &sharedWorldPose);

    return AFX_SUCCESS; 
}

_AFXEXPORT afxError SandboxProc(afxApplication app, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;

    switch (opcode)
    {
        //case 0: break;
    case AFX_THR_OPCODE_RUN:
    {
        Once(app);
        break;
    }
    case AFX_THR_OPCODE_QUIT:
    {
        AfxEntry("app=%p", app);
        //AfxReleaseObject(&dinD->obj);
        AfxEcho("aaaaaaaaaaaa");

        break;
    }
    default:
    {
        afxReal64 CurrentTime, DeltaTime;
        afxClock CurrClock;
        AfxGetClock(&CurrClock);

        // Ignore clock recentering issues for this example
        CurrentTime = AfxGetSecondsElapsed(&StartClock, &CurrClock);
        DeltaTime = AfxGetSecondsElapsed(&LastClock, &CurrClock);
        LastClock = CurrClock;

        afxReal64 dt;
        AfxGetExecutionTime(NIL, &dt);
        UpdateFrameMovement(DeltaTime);

        break;
    }
    }

    return err;
}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxUri2048 romUri;
    AfxMakeUri2048(&romUri, NIL);
    AfxFormatUri(&romUri.uri, "%s", argv[0]); // hardcoded name

    afxBool reboot = 1;
    while (reboot)
    {
        afxSystemConfig sysCfg;
        afxPlatformConfig winCfg;
        afxDrawSystemConfig dsysCfg;
        AfxChooseSystemConfiguration(&sysCfg, &winCfg);
        AfxChooseDrawSystemConfiguration(&dsysCfg);
        sysCfg.platform = &winCfg;
        sysCfg.draw = &dsysCfg;
        AfxDoSystemBootUp(&sysCfg);

        afxDrawContextConfig dctxCfg = { 0 };
        AfxAcquireDrawContexts(0, 1, &dctxCfg, &dctx);
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxApplication TheApp;
        afxApplicationConfig appConfig = { 0 };
        appConfig.argc = argc;
        appConfig.argv = argv;
        appConfig.dctx = dctx;
        appConfig.proc = SandboxProc;
        AfxAcquireApplications(1, &appConfig, &TheApp);
        AfxAssertObjects(1, &TheApp, afxFcc_APP);
        AfxRunApplication(TheApp);

        while (AfxSystemIsExecuting())
            AfxDoSystemExecution(0);

        AfxReleaseObjects(1, (void*[]) { TheApp });

        AfxReleaseObjects(1, (void*[]) { dctx });

        AfxDoSystemShutdown(0);
    }
    Sleep(3000);
    return 0;
}
