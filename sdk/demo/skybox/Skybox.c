#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include "afx/afxQwadro.h"
#include "afx/core/afxApplication.h"
#include "afx/core/diag/afxDebug.h"
#include "afx/math/afxMathDefs.h"

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

afxUri2048 uri;

 afxSize frameTime = 0, swapTime = 0, frameNum = 0;
 afxReal64 dt = 0;
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
    AfxRendererDrawSky(rnd, TRUE);

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

_AFXEXPORT void AfxUpdateApplication(afxThread thr, afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &app, AFX_FCC_APP);

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

    AfxGetExecutionTime(NIL, &dt);
    UpdateFrameMovement(dt);
 
}

_AFXEXPORT void AfxEnterApplication(afxThread thr, afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    afxString str, str2;
    afxUri uriMap;

    afxUri128 uri128;
    AfxUri128(&uri128);
    afxStoragePointSpecification mpSpec = { 0 };
    AfxFormatUri(&uri128.uri, "art/mnt.zip");
    mpSpec.hostPath = &uri128.uri;
    AfxUriWrapLiteral(&uriMap, "art", 0);
    mpSpec.namespace = &uriMap;
    mpSpec.perm = AFX_IO_FLAG_R;
    afxResult rslt = AfxMountStoragePoints(1, &mpSpec);
    AfxAssert(rslt == 1);

    afxArchive arc;
    AfxAcquireArchives(1, &arc, &uri128.uri, (afxFileFlags[]) { AFX_FILE_FLAG_R });
    AfxAssertObjects(1, &arc, AFX_FCC_ARC);
    afxUri itemNam;
    AfxUriWrapLiteral(&itemNam, "art/worldtest.tga", 0);
    afxNat itemIdx;
    AfxFindArchiveItem(arc, &itemNam, &itemIdx);
    afxStream item;
    AfxOpenArchiveItem(arc, itemIdx, &item);
    AfxReleaseStream(&item);
    AfxUriWrapLiteral(&itemNam, "tmp/worldtest.tga", 0);
    //AfxDownloadArchiveItem(arc, itemIdx, &itemNam);
    AfxReleaseObjects(1, (void*[]) { arc });

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
    doutConfig.pixelFmt = AFX_PFD_RGB8_SRGB;
    doutConfig.endpoint = &uriMap;
    AfxAcquireDrawOutputs(dsys, 1, &dout, &doutConfig);
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    AfxReconnectDrawOutput(dout, dctx, NIL);

    afxRendererConfig rndConf = { 0 };
    rndConf.dinProc = DinFetcherFn;
    AfxAcquireRenderers(sim, 1, &rnd, &rndConf);
    
    AfxRendererBindOutput(rnd, dout);

    AfxAcquireCameras(sim, 1, &cam);
    AfxAssert(cam);

    rnd->activeCamera = cam;

    AfxGetKeyboard(0, &kbd);
    AfxGetMouse(0, &mse);
    AfxObjectInstallEventFilter(mse, cam);
    
    AfxEnableDrawInputPrefetching(rnd->din, TRUE); // bug: sem isso não desenha
    
}

_AFXEXPORT afxError SkyboxThrProc(afxThread thr, afxApplication app, afxThreadOpcode opcode)
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
        afxSystem sys = NIL;
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
        appConfig.proc = SkyboxThrProc;
        appConfig.dctx = dctx;
        //appConfig.enter = AfxEnterApplication;
        //appConfig.exit = AfxLeaveApplication;
        //appConfig.update = AfxUpdateApplication;
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
