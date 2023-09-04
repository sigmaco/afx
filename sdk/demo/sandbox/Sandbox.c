#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "afx/afxQwadro.h"
#include "afx/core/afxApplication.h"
#include "afx/core/diag/afxDebug.h"
#include "afx/math/afxMathDefs.h"
#include "afx/core/io/afxMouse.h"
#include "../src/e2idtech/afxMD5Model.h"
#include "../src/e2cad/afxWavefrontObject.h"

#define ENABLE_DRAW 1

#ifdef ENABLE_DRAW
#include "afx/draw/afxDrawSystem.h"
#include "afx/sim/rendering/afxRenderer.h"
#endif

afxSystem sys;
afxDrawSystem dsys;

const afxReal CameraSpeed = 30.0f;
afxKeyboard kbd = NIL;
afxMouse mse = NIL;
afxSimulation sim = NIL;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
afxRenderer rnd = NIL;

afxCamera cam = NIL;
afxBody bod = NIL;

afxSize frameTime = 0, swapTime = 0, frameNum = 0;
afxReal dt = 0;
afxNat fps = 0;

afxError DinFetcherFn(afxDrawInput din, afxDrawThread dthr) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    afxRenderer rnd = AfxGetDrawInputUdd(din);
    afxDrawContext dctx;
    AfxGetConnectedDrawInputContext(din, &dctx);
    afxDrawScript dscr;
    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);

    afxNat outBufIdx = 0;
    AfxRequestDrawOutputBuffer(dout, 0, &outBufIdx);
    rnd->activeOutputBufIdx = outBufIdx;
    afxTexture surf;
    AfxGetDrawOutputBuffer(dout, outBufIdx, &surf);
    AfxAssertObjects(1, &surf, AFX_FCC_TEX);

    AfxRendererBeginScene(rnd, rnd->activeCamera, NIL, surf);
    //AfxRendererDrawSky(rnd, TRUE);

    if (bod)
        AfxRendererDrawBody(rnd, bod);

    AfxRendererEndScene(rnd);

    if (AfxSubmitPresentations(rnd->din, 1, &dout, &rnd->activeOutputBufIdx)) // draw output count hardcoded to 1.
        AfxThrowError();

    return err;
}

void UpdateFrameMovement(const afxReal DeltaTime)
{
    afxError err = AFX_ERR_NONE;

    const afxReal MovementThisFrame = DeltaTime * CameraSpeed;

    // Note: because the NegZ axis is forward, we have to invert the way you'd normally
    // think about the 'W' or 'S' key's action.  Also, we don't have a key for moving the
    // camera up and down, but it should be clear how to add one.
    const afxReal ForwardSpeed = (AfxKeyIsPressed(AFX_KEY_W, 0) ? -1 : 0.0f) + (AfxKeyIsPressed(AFX_KEY_S, 0) ? 1 : 0.0f);
    const afxReal RightSpeed = (AfxKeyIsPressed(AFX_KEY_A, 0) ? -1 : 0.0f) + (AfxKeyIsPressed(AFX_KEY_D, 0) ? 1 : 0.0f);

    AfxCameraMoveRelative(cam, AfxSpawnV3d(MovementThisFrame * RightSpeed, 0.0f, MovementThisFrame * ForwardSpeed));
}

_AFXEXPORT afxResult AfxUpdateApplication(afxThread thr, afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    afxSize time = AfxGetTimer();

    dt = ((afxReal)time - (afxReal)frameTime) * 0.001;

    if (1000 <= (time - swapTime))
    {
        swapTime = time;
        fps = frameNum;
        frameNum = 0;
    }

    // deveriam ser movidos para afxDrawInput para separação e exibição correta para cada saída?

    ++frameNum;
    frameTime = time;

    UpdateFrameMovement(dt);

    //AfxRendererBeginScene(rnd, cam);
    //AfxRendererDrawSky(rnd, TRUE);

    //AfxRendererDrawBody(rnd, bod);

    //AfxRendererEndScene(rnd);

    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxEnterApplication(afxThread thr, afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    afxUri uriMap;
    AfxUriWrapLiteral(&uriMap, "e2newton.icd", 0);
    afxSimulationConfig simSpec = { 0 };
    simSpec.bounding = NIL;
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    AfxAcquireSimulations(app, 1, &sim, &simSpec);
    AfxAssertObjects(1, &sim, AFX_FCC_SIM);
    
    AfxUriWrapLiteral(&uriMap, "window", 0);
    afxDrawOutputConfig doutConfig = {0};
    AfxAcquireDrawOutputs(dsys, 1, &dout, &doutConfig);
    AfxAssert(dout);
    AfxReconnectDrawOutput(dout, dctx, NIL);

    afxRendererConfig rndConf = { 0 };
    rndConf.dinProc = DinFetcherFn;
    AfxAcquireRenderers(sim, 1, &rnd, &rndConf);

    AfxRendererBindOutput(rnd, dout);

    afxUri uriMap2;
    afxString str;
    afxModel mdl;
    //AfxAcquireSky();

    //AfxFormatUri(&uri, "%.*s/scenario/zero/zero.obj", AfxPushString(AfxApplication_GetArtDirectory(app)));
    //afxArray assets;
    //AfxDeployArray(&assets, NIL, 0, sizeof(void*));
    //AfxLoadAssets(sim, &uri);

    AfxUriWrapLiteral(&uriMap, "art/actor/hellknight/hellknight.md5mesh", 0);
    AfxSimulationLoadMD5Assets(sim, &uriMap, NIL);

    AfxExcerptUriObject(&uriMap2, &uriMap);
    AfxAcquireModels(sim, 1, &uriMap2, &mdl);

    //AfxUriWrapLiteral(&uriMap, "art/scenario/TV-Stand-5/TV-Stand-5.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/scenario/gtabr/gtabr.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/f16/f16.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/scenario/bibliotheca/bibliotheca.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/scenario/zero/zero.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/scenario/SpaceStation/SpaceStation.obj", 0);
    AfxUriWrapLiteral(&uriMap, "art/object/container/container.obj", 0);
    AfxSimulationLoadObjAssets(sim, &uriMap, NIL);

    //AfxWrapStringLiteral(&str, "bibliotheca", 0);
    //AfxString32DeployRaw(&str, "TV-Stand-5", 0);
    //AfxString32DeployRaw(&str, "SpaceStation", 0);
    //AfxString32DeployRaw(&str, "f16", 0);
    //AfxWrapStringLiteral(&str, "zero", 0);
    //AfxWrapStringLiteral(&str, "container", 0);


    AfxExcerptUriObject(&uriMap2, &uriMap);
    AfxAcquireModels(sim, 1, &uriMap2, &mdl);
    // TODO FetchModel(/dir/to/file)

    AfxAcquireBodies(sim, 1, &bod, &mdl);
    AfxAssert(bod);
#if 0
    mdl = AfxSimulationFindModel(sim, &str);
    body = AfxSimulationAcquireBody(sim, &str, NIL, mdl);
    AfxAssert(body);
#endif
    //AfxCopyStringLiteral(&str, 0, "zero");
    //afxBody body2;
    //AfxAcquireBody(&body2, sim, &str, AfxFindModel(sim, &str));
    //AfxAssert(body2);

    AfxWrapStringLiteral(&str, "viewer", 0);
    AfxAcquireCameras(sim, 1, &cam);
    AfxAssert(cam);
    //cam->farClip = -100000.0;
    //AfxCameraAddOffset(cam, AfxSpawnV3d(0, 1.1, 0));

    //AfxAttachViewpoint(vpnt, cam);

    rnd->activeCamera = cam;

    AfxGetKeyboard(0, &kbd);
    AfxGetMouse(0, &mse);
    AfxObjectInstallEventFilter(mse, cam);

    AfxEnableDrawInputPrefetching(rnd->din, 1); // bug: sem isso não desenha

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

}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxUri2048 romUri;
    AfxUri2048(&romUri);
    AfxFormatUri(&romUri.uri, "%s", argv[0]); // hardcoded name

    afxBool reboot = 1;
    while (reboot)
    {
        afxSystem sys;
        AfxBootUpBasicIoSystem(NIL, &sys);

        afxDrawSystemConfig dsysSpec = { 0 };
        AfxChooseDrawSystemConfiguration(&dsysSpec, 0);
        AfxAcquireDrawSystems(1, &dsys, &dsysSpec);
        AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);

        afxDrawContextConfig dctxConfig = { 0 };
        AfxAcquireDrawContexts(dsys, 1, &dctx, &dctxConfig);
        AfxAssert(dctx);

        afxApplication TheApp;
        afxApplicationConfig appConfig = { 0 };
        appConfig.argc = argc;
        appConfig.argv = argv;
        appConfig.dctx = dctx;
        appConfig.proc = SandboxThrProc;
        AfxAcquireApplications(1, &TheApp, &appConfig);
        AfxAssertObjects(1, &TheApp, AFX_FCC_APP);
        AfxRunApplication(TheApp);

        while (AfxSystemIsOperating())
            AfxDoSystemThreading(0);

        AfxReleaseObjects(1, (void*[]) { TheApp });

        AfxReleaseObjects(1, (void*[]) { dctx });

        AfxReleaseObjects(1, (void*[]) { dsys });
        AfxShutdownBasicIoSystem(0);
    }
    Sleep(3000);
    return 0;
}
