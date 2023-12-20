#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/afxQwadro.h"
#include "qwadro/core/afxApplication.h"
#include "qwadro/core/afxDebug.h"
#include "qwadro/math/afxMathDefs.h"
#include "qwadro/core/afxMouse.h"
#include "../e2idtech/afxMD5Model.h"
#include "../e2cad/afxWavefrontObject.h"

#define ENABLE_DRAW 1

#ifdef ENABLE_DRAW
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sim/rendering/awxRenderer.h"
#endif

afxSystem sys;
afxDrawSystem dsys;

afxClock StartClock;
afxClock LastClock;

awxBody cubeBod = NIL;
awxModel cubeMdl = NIL;
awxMesh cube = NIL;
const afxReal CameraSpeed = 30.0f;
afxKeyboard kbd = NIL;
afxMouse mse = NIL;
awxSimulation sim = NIL;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
awxRenderer rnd = NIL;

afxCamera cam = NIL;
awxBody bod = NIL;

afxError DrawInputProc(afxDrawInput din, afxNat thrUnitIdx) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    awxRenderer rnd = AfxGetDrawInputUdd(din);
    afxDrawContext dctx = AfxGetDrawInputContext(din);
    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);

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
            AfxGetDrawOutputSurface(dout, outBufIdx, 1, &surf);
            afxCanvas canv;
            AfxGetDrawOutputCanvas(dout, outBufIdx, 1, &canv);
            AfxAssertObjects(1, &surf, afxFcc_RAS);

            AwxCmdBeginSceneRendering(dscr, rnd, rnd->activeCam, NIL, canv);

            
            //if (bod)
                //AwxCmdDrawBodies(dscr, rnd, 1, &bod);

            if (cubeBod)
                AwxCmdDrawBodies(dscr, rnd, 1, &cubeBod);

            AwxCmdDrawTestIndexed(dscr, rnd);

            AfxDrawSky(dscr, &rnd->sky);

            AwxCmdEndSceneRendering(dscr, rnd);

            if (AfxCompileDrawScript(dscr)) AfxThrowError();
            else if (AfxSubmitDrawScripts(din, 1, &dscr))
                AfxThrowError();

            if (AfxPresentDrawOutputBuffers(din, 1, &dout, &outBufIdx))
                AfxThrowError();
        }
    }

    return err;
}

void UpdateFrameMovement(afxReal64 DeltaTime)
{
    afxError err = AFX_ERR_NONE;

    afxReal64 MovementThisFrame = DeltaTime * CameraSpeed;

    // Note: because the NegZ axis is forward, we have to invert the way you'd normally
    // think about the 'W' or 'S' key's action.
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
}

_AFXEXPORT void AfxUpdateApplication(afxThread thr, afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    afxSize time = AfxGetTimer();

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
}

_AFXEXPORT afxResult AfxEnterApplication(afxThread thr, afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    afxUri uriMap;
    AfxMakeUri(&uriMap, "e2newton.icd", 0);
    awxSimulationConfig simSpec = { 0 };
    AfxRecomputeAabb(simSpec.extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    simSpec.unitsPerMeter = 1.f;
    AfxSetV3d(simSpec.right,    1, 0, 0);
    AfxSetV3d(simSpec.up,       0, 1, 0);
    AfxSetV3d(simSpec.back,     0, 0, 1);
    AfxZeroV3d(simSpec.origin);
    AfxAcquireSimulations(1, &sim, &simSpec);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    
    AfxMakeUri(&uriMap, "window", 0);
    afxDrawOutputConfig doutConfig = {0};
    doutConfig.auxDsFmt[0] = afxPixelFormat_D32F;
    AfxOpenDrawOutputs(dsys, 0, 1, &doutConfig, &dout);
    AfxAssert(dout);
    AfxReconnectDrawOutput(dout, dctx);

    awxRendererConfig rndConf = { 0 };
    rndConf.dinProc = DrawInputProc;
    AwxAcquireRenderers(sim, 1, &rnd, &rndConf);

    afxUri uriMap2;
    afxString str;
    awxModel mdl;
    //AfxAcquireSky();

    //AfxFormatUri(&uri, "%.*s/scenario/zero/zero.obj", AfxPushString(AfxApplication_GetArtDirectory(app)));
    //afxArray assets;
    //AfxDeployArray(&assets, NIL, 0, sizeof(void*));
    //AfxLoadAssets(sim, &uri);

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
    AfxMakeUri(&uriMap, "art/scenario/bibliotheca/bibliotheca.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/zero/zero.obj", 0);
    //AfxMakeUri(&uriMap, "art/scenario/SpaceStation/SpaceStation.obj", 0);
    //AfxMakeUri(&uriMap, "art/object/container/container.obj", 0);
    //AfxSimulationLoadObjAssets(sim, &uriMap, NIL);

    awxAsset cad;
    AfxLoadAssetsFromWavefrontObj(sim, NIL, 1, &uriMap, &cad);
    //AfxLoadAssetsFromMd5(sim, NIL, 1, &uriMap, &cad);

    afxV3d atv;
    afxM3d ltm, iltm;
    AfxComputeBasisConversion(sim, 1.0, AFX_V3D_X, AFX_V3D_Y, AFX_V3D_Z, AFX_V3D_ZERO, ltm, iltm, atv);
    //AfxTransformAssets(ltm, iltm, atv, 1e-5f, 1e-5f, 3, 1, &cad); // renormalize e reordene triângulos

    AfxGetUriName(&uriMap2, &uriMap);
    AfxFindResources(cad, afxFcc_MDL, 1, AfxGetUriString(&uriMap2), &mdl);
    //AfxAcquireModels(sim, 1, &uriMap2, &mdl);
    // TODO FetchModel(/dir/to/file)

    AwxEmbodyModel(mdl, 1, &bod);
    AfxAssert(bod);

    cube = AfxBuildCubeMesh(sim, -200.0);
    //cube = AfxBuildParallelepipedMesh(sim, 100, 100);
    //cube = AfxBuildParallelepipedMesh(sim, AfxSpawnV3d(100, 100, 10));
    //cube = AfxBuildDomeMesh(sim, 100.0, 4);
    cubeMdl = AwxAssembleModel(sim, &AfxStaticString("cube"), AwxGetModelSkeleton(mdl), NIL, 1, &cube);
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

    AfxAcquireCameras(dsys, 1, &cam);
    AfxAssert(cam);
    //AfxSetCameraFov(cam, AFX_PI / 4.0);
    //cam->farClip = -100000.0;
    //AfxApplyCameraOffset(cam, AfxSpawnV3d(0, 1.1, 0));

    //AfxAttachViewpoint(vpnt, cam);

    rnd->activeCam = cam;

    AfxGetKeyboard(0, &kbd);
    AfxGetMouse(0, &mse);
    AfxObjectInstallEventFilter(mse, cam);

    AfxEnableDrawInputPrefetching(rnd->din, 1); // bug: sem isso não desenha
    
    AfxGetClock(&StartClock);
    LastClock = StartClock;

    return AFX_SUCCESS; 
}

_AFXEXPORT afxResult AfxLeaveApplication(afxThread thr, afxApplication app)
{
    AfxEntry("app=%p", app);
    //AfxReleaseObject(&dinD->obj);
    AfxEcho("aaaaaaaaaaaa");

    return AFX_SUCCESS;
}

_AFXEXPORT afxError SandboxThrProc(afxThread thr, afxApplication app, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;

    switch (opcode)
    {
        //case 0: break;
    case AFX_THR_OPCODE_RUN: AfxEnterApplication(thr, app); break;
        //case AFX_THR_OPCODE_QUIT: AfxEndApplication(thr, app); break;
    default: AfxUpdateApplication(thr, app); break;
    }

    return err;
}

int AfxMain(afxApplication app, int argc, char const* argv[])
{
    return 0;
}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxFixedUri2048 romUri;
    AfxMakeFixedUri2048(&romUri);
    AfxFormatUri(&romUri.uri, "%s", argv[0]); // hardcoded name

    afxBool reboot = 1;
    while (reboot)
    {
        afxSystem sys;
        AfxBootUpBasicIoSystem(NIL, &sys);

        afxDrawSystemConfig dsysSpec = { 0 };
        AfxChooseDrawSystemConfiguration(&dsysSpec, 0);
        AfxAcquireDrawSystems(1, &dsysSpec, &dsys);
        AfxAssertObjects(1, &dsys, afxFcc_DSYS);

        afxDrawContextConfig dctxConfig = { 0 };
        AfxAcquireDrawContexts(dsys, 0, 1, &dctxConfig, &dctx);
        AfxAssert(dctx);

        afxApplication TheApp;
        afxApplicationConfig appConfig = { 0 };
        appConfig.argc = argc;
        appConfig.argv = argv;
        appConfig.dctx = dctx;
        appConfig.proc = SandboxThrProc;
        AfxAcquireApplications(1, &TheApp, &appConfig);
        AfxAssertObjects(1, &TheApp, afxFcc_APP);
        AfxRunApplication(TheApp);

        while (AfxSystemIsExecuting())
            AfxDoSystemThreading(0);

        AfxReleaseObjects(1, (void*[]) { TheApp });

        AfxReleaseObjects(1, (void*[]) { dctx });

        AfxReleaseObjects(1, (void*[]) { dsys });
        AfxShutdownBasicIoSystem(0);
    }
    Sleep(3000);
    return 0;
}
