#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>


#define _AUX_UX_C
#define _AFX_WINDOW_C

#include "qwadro/afxQwadro.h"
#include "../src/techid/afxMD5Model.h"
#include "../src/cadio/afxWavefrontObject.h"
#include "../src/rad3d/afxGranny2Model.h"

#define ENABLE_DRAW 1

#ifdef ENABLE_DRAW
#include "qwadro/sim/rendering/akxRenderer.h"
#include "qwadro/sim/rendering/akxTerrain.h"
#endif
#include "../../dep_/vgl/vgl.h"

afxBool readyToRender = FALSE;
afxScript TheApp;
afxWindow window;
akxBody cubeBod = NIL;
afxModel cubeMdl = NIL;
afxMesh cube = NIL;
const afxReal CameraSpeed = 30.0f;
afxSimulation sim = NIL;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
akxRenderer rnd = NIL;
akxAnimation ani = NIL;
avxCamera cam = NIL;
akxBody bod = NIL;
akxTerrain ter = NIL;

void *vg = NIL;

akxPose sharedLocalPose;
akxPoseBuffer sharedPoseBuffer;

afxBool DrawInputProc(afxDrawInput din, avxEvent const* ev) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    switch (ev->id)
    {
    default:
    {
        akxRenderer rnd = AfxGetDrawInputUdd(din);
        afxDrawContext dctx;
        AfxGetDrawInputContext(din, &dctx);
        
        if (readyToRender)
        {
            afxNat outBufIdx = 0;

            if (!AfxLockDrawOutputBuffer(dout, 0, &outBufIdx))
            {
                avxCmdb cmdb;
                afxNat queIdx = AFX_INVALID_INDEX;
                afxNat portIdx = 0;

                if (AfxAcquireCmdBuffers(din, avxCmdbUsage_ONCE, 1, &cmdb)) AfxThrowError();
                else
                {
                    avxCanvas canv;
                    AfxEnumerateDrawOutputCanvases(dout, outBufIdx, 1, &canv);
                    AfxAssertObjects(1, &canv, afxFcc_CANV);

                    AkxCmdBeginSceneRendering(cmdb, rnd, rnd->activeCam, NIL, canv);

                    if (bod)
                    {
                        afxReal64 ct, dt;
                        //AfxGetApplicationTime(TheApp, &ct, &dt);
                        //AkxCmdDrawBodies(cmdb, rnd, dt, 1, &bod);
                    }

                    AkxDrawTerrain(ter, rnd, cmdb);

                    AkxBeginSceneCapture(rnd, cam, sim, cmdb);

                    AkxCmdDrawTestIndexed(cmdb, rnd);

                    //AfxDrawSky(cmdb, &rnd->sky);

                    afxWhd canvWhd;
                    AfxGetCanvasExtent(canv, canvWhd);
                    //TestSvg(vg, cmdb, canvWhd);

                    AkxCmdEndSceneRendering(cmdb, rnd);

                    afxSemaphore dscrCompleteSem = NIL;

                    if (AfxCompileCmdBuffers(1, &cmdb)) AfxThrowError();
                    else
                    {
                        afxExecutionRequest execReq = { 0 };
                        execReq.cmdb = cmdb;
                        execReq.signal = dscrCompleteSem;

                        if (AFX_INVALID_INDEX == (queIdx = AfxExecuteCmdBuffers(din, 1, &execReq, NIL)))
                            AfxThrowError();
                    }

                    //AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx);
                }

                afxSemaphore dscrCompleteSem = NIL;

                //AfxStampDrawOutputBuffers(1, &req, AfxV2d(200, 200), &AfxString("Test"), 738);

                if (AFX_INVALID_INDEX == AfxPresentDrawOutputBuffer(dout, outBufIdx, portIdx, dscrCompleteSem))
                    AfxThrowError();

            }
        }
        break;
    }
    }
    return FALSE;
}

afxBool CamEventFilter(afxObject *obj, afxObject *watched, auxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    avxCamera cam = (void*)obj;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    (void)watched;
    (void)ev;

    switch (ev->id)
    {
    case auxEventId_AXIS:
    {
        //afxMouse mse = (void*)watched;
        //AfxAssertObjects(1, &mse, afxFcc_MSE);

        // TODO Leva isso para o usuário

        if (AfxIsLmbPressed(0))
        {
            afxV2d delta;
            afxV3d deltaEar;
            AfxGetMouseMotion(0, delta);
            deltaEar[1] = -((afxReal)(delta[0] * AFX_PI / 180.f));
            deltaEar[0] = -((afxReal)(delta[1] * AFX_PI / 180.f));
            deltaEar[2] = 0.f;

            AfxApplyCameraOrientation(cam, deltaEar);
        }

        if (AfxIsRmbPressed(0))
        {
            afxV2d delta;
            afxV3d off;
            AfxGetMouseMotion(0, delta);
            off[0] = -((afxReal)(delta[0] * AFX_PI / 180.f));
            off[1] = -((afxReal)(delta[1] * AFX_PI / 180.f));
            off[2] = 0.f;

            AfxApplyCameraDisplacement(cam, off);
        }
        break;
    }
    case auxEventId_WHEEL:
    {
        afxReal w = AfxGetMouseWheelDelta(0);
        w = w / 120.f; // WHEEL_DELTA
        AfxApplyCameraDistance(cam, w);
        break;
    }
    case auxEventId_KEY:
    {
        //afxKeyboard kbd = (void*)watched;
        //AfxAssertObjects(1, &kbd, afxFcc_KBD);
        break;
    }
    default:break;
    }
    return FALSE;
}

void UpdateFrameMovement(afxReal64 DeltaTime)
{
    afxError err = AFX_ERR_NONE;
#if !0
    afxReal64 MovementThisFrame = DeltaTime * CameraSpeed;

    // Note: because the NegZ axis is forward, we have to invert the way you'd normally think about the 'W' or 'S' key's action.
    afxReal64 ForwardSpeed = (AfxGetKeyPressure(0, afxKey_W) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_S) ? 1 : 0.0f);
    afxReal64 RightSpeed = (AfxGetKeyPressure(0, afxKey_A) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_D) ? 1 : 0.0f);
    afxReal64 UpSpeed = (AfxGetKeyPressure(0, afxKey_Q) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_E) ? 1 : 0.0f);
    afxV3d v =
    {
        MovementThisFrame * RightSpeed,
        MovementThisFrame * UpSpeed,
        MovementThisFrame * ForwardSpeed
    };

    if (AfxV3dSum(v))
        AfxTranslateCamera(cam, v);
#else
    //AfxProcessCameraInteraction(cam, 0, CameraSpeed, DeltaTime);
#endif

    afxReal64 curTime, lastTime;
    AfxGetThreadTime(&curTime, &lastTime);
    AfxReclockMotives(bod, curTime);

#if 0
    afxM4d m;
    AfxComputeModelDisplacement(bod->mdl, m);
    AfxSampleBodyAnimationsAcceleratedLOD(bod, bod->cachedBoneCnt, m, sharedLocalPose, sharedPoseBuffer, 0.0);
#if 0
    afxM4d CompositeMatrix;
    AfxEnumerateRiggedMeshes(bod->mdl, i);
    AfxBuildIndexedCompositeBuffer(bod->cachedSkl, sharedPoseBuffer, AfxGetRiggedMeshBiasToJointMapping(bod->mdl, i), 1, &CompositeMatrix);
#endif
#endif
}

DLLEXPORT afxResult Once(afxScript app)
{
    afxError err = AFX_ERR_NONE;

    return AFX_SUCCESS; 
}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxUri2048 romUri;
    AfxMakeUri2048(&romUri, NIL);
    AfxFormatUri(&romUri.uri, "%s", argv[0]); // hardcoded name

    // Boot up the Qwadro

    afxSystemConfig sysCfg;
    AfxConfigureSystem(&sysCfg);
    AfxDoSystemBootUp(&sysCfg);

    afxUri pointUri, endpointUri;
    AfxMakeUri(&pointUri, 0, "art", 0);
    AfxMakeUri(&endpointUri, 0, "art", 0);
    AfxMountStorageUnit(&pointUri, &endpointUri, afxFileFlag_RX);

    // Acquire hardware device contexts

    afxDrawContextConfig dctxCfg = { 0 };
    AfxOpenDrawDevice(0, &dctxCfg, &dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    // Acquire a drawable surface
    
    afxWindowConfig wrc = { 0 };
    wrc.surface.pixelFmt = afxPixelFormat_ARGB8;
    wrc.surface.pixelFmtDs[0] = afxPixelFormat_D24;
    wrc.surface.bufCnt = 3;
    AfxAcquireWindow(dctx, &wrc, &window);
    AfxAdjustWindowFromNdc(window, NIL, AfxSpawnV3d(0.5, 0.5, 1));

#if 0
    afxDrawOutputConfig doutConfig = { 0 };
    doutConfig.pixelFmtDs[0] = afxPixelFormat_D24;
    AfxAcquireDrawOutput(0, &doutConfig, &dout);
#endif
    AfxGetWindowDrawOutput(window, NIL, &dout);
    AfxAssert(dout);
    AfxReconnectDrawOutput(dout, dctx);

    // Acquire a simulation

    akxSimulationConfig simSpec = { 0 };
    AfxRecomputeAabb(simSpec.extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.unitsPerMeter = 1.f;
    AfxV3dSet(simSpec.right, 1, 0, 0);
    AfxV3dSet(simSpec.up, 0, 1, 0);
    AfxV3dSet(simSpec.back, 0, 0, 1);
    AfxV3dZero(simSpec.origin);
    AfxAcquireSimulations(1, &simSpec, &sim);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    // Acquire a simulation renderer

    //AfxInstallWatcher();
    akxRendererConfig rndConf = { 0 };
    rndConf.dinProc = DrawInputProc;
    AkxAcquireRenderers(sim, 1, &rnd, &rndConf);

    AfxAcquireTerrain(sim, &ter);

    // Load assets

    afxUri uriMap2;
    afxString str;
    afxModel mdl;
    afxUri uriMap;

    //AfxMakeUri(&uriMap, 0, "//./art/scenario/cod-mw3/ny_manhattan/ny_manhattan.obj", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/scenario/control-statiopn/uploads_files_3580612_control+statiopn.obj", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/scenario/cs_rio/cs_rio_base.obj", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/scenario/resdogs/resdogs.obj", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/scenario/TV-Stand-5/TV-Stand-5.obj", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/scenario/gtabr/gtabr.obj", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/f16/f16.obj", 0);
    AfxMakeUri(&uriMap, 0, "//./art/scenario/bibliotheca/bibliotheca-uvless.obj", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/scenario/zero/zero.obj", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/scenario/SpaceStation/SpaceStation.obj", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/object/container/container.obj", 0);
    mdl = AfxLoadAssetsFromWavefrontObj(sim, &uriMap);

    AfxAcquireBodies(mdl, 1, &bod);
    AfxAssert(bod);

    //AfxMakeUri(&uriMap, "//./art/building/fort/SPC_Fort_Center.gr2", 0);
    AfxMakeUri(&uriMap, 0, "//./art/building/mill/w_mill_3age.gr2", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/building/mill/west_mill_age3_damaged.gr2", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/actor/p_explorer.gr2", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/actor/pixo_run.gr2", 0);
    mdl = AkxLoadModelsFromGrn3d2(sim, &uriMap, 0); // .m4d

    afxV3d atv;
    afxM3d ltm, iltm;
    AfxComputeBasisConversion(sim, 1.0, AFX_V3D_X, AFX_V3D_Y, AFX_V3D_Z, AFX_V3D_ZERO, ltm, iltm, atv);
    //AfxTransformModels(ltm, iltm, 1e-5f, atv, 1e-5f, 3, 1, &mdl);

    AfxAcquireBodies(mdl, 1, &bod);
    AfxAssert(bod);

    AfxMakeUri(&uriMap, 0, "//./art/building/mill/w_mill_3age_idle.gr2", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/building/mill/west_mill_age3_limping_idle.gr2", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/actor/knockout_recover.gr2", 0);
    //AfxMakeUri(&uriMap, 0, "//./art/actor/pixo_run.gr2", 0);
    ani = AkxLoadAnimationsFromGrn3d2(sim, &uriMap, 0); // .k4d



    akxMotor moto;
    AfxRunAnimation(ani, 0, 0, 1, &bod);
    //AfxRunAnimation(ani, 0, 0, 1, &bod);
    //akxMotor moto2 = AfxRunAnimation(bod, 0, ani);

    //AkxAdjustMotorIterations(moto2, 0);
    //AkxReleaseOnceUnusedMotors(1, &moto);

    //cube = AfxBuildCubeMesh(sim, 10.0, 0);
    //cube = AfxBuildDomeMesh2(sim, 100.0, 10, 20, TRUE);
    cube = AfxBuildSphereMesh(sim, 100.0, 20, 20, TRUE);
    //cube = AfxBuildCapsuleMesh(sim, 1, 50.0, 20, 20, 36, FALSE);
    //cube = AfxBuildPlaneMesh(sim, 10, 10, 1000, 1000);
    //cube = AfxBuildParallelepipedMesh(sim, AfxSpawnV3d(100, 10, 100), 0.5, 0.5);
    
    afxStringBase strb;
    AfxAcquireStringCatalogs(1, &strb);

    akxModelBlueprint mdlb = { 0 };
    mdlb.meshes = &cube;
    mdlb.rigCnt = 1;
    AfxMakeString32(&mdlb.id, &AfxStaticString("cube"));
    
    akxSkeletonBlueprint sklb = { 0 };
    AfxAssembleSkeletons(sim, 1, &sklb, &mdlb.skl);

    AfxAssembleModel(sim, 1, &mdlb, &cubeMdl);
    AfxAcquireBodies(cubeMdl, 1, &cubeBod);

#if 0
    mdl = AfxSimulationFindModel(sim, &str);
    body = AfxSimulationAcquireBody(sim, &str, NIL, mdl);
    AfxAssert(body);
#endif
    //AfxCopyStringLiteral(&str, 0, "zero");
    //akxBody body2;
    //AfxAcquireBody(&body2, sim, &str, AfxFindModel(sim, &str));
    //AfxAssert(body2);

    // Acquire a view point

    AfxAcquireCameras(rnd->din, 1, &cam);
    AfxAssert(cam);
    //AfxSetCameraFov(cam, AFX_PI / 4.0);
    //cam->farClip = -100000.0;
    //AfxApplyCameraDisplacement(cam, AfxSpawnV3d(0, 1.1, 0));

    //AfxAttachViewpoint(vpnt, cam);

    rnd->activeCam = cam;

    // Handle mouse-camera interaction

    afxHid hid;
    AfxGetHid(0, &hid);
    AfxResetEventFilter(cam, (void*)CamEventFilter);
    AfxInstallWatcher(window, cam);

    AfxAcquirePoses(sim, 1, (afxNat[]) { 256 }, &sharedLocalPose);
    AfxAcquirePoseBuffers(sim, 1, (afxNat[]) { 256 }, NIL, &sharedPoseBuffer);

    // Load startup scripts

    afxUri uri;
    AfxMakeUri(&uri, 0, "//./z/sandbox.xss", 0);
    //AfxLoadScript(NIL, &uri);

    // Run

    readyToRender = TRUE;

    vg = AvxAcquireGraphic(dctx, 1);

    while (AfxSystemIsExecuting())
    {
        DrawInputProc(rnd->din, NIL);

        afxReal64 ct, dt;
        AfxStepWindow(window, &ct, &dt);
        UpdateFrameMovement(dt);

        AfxDoThreading(0);
    }

    AvxReleaseGraphic(vg);

    AfxReleaseObjects(1, &window);

    AfxReleaseObjects(1, &dctx);

    AfxDoSystemShutdown(0);
    Sleep(3000);
    return 0;
}
