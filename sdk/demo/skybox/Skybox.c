#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include "afx/afxQwadro.h"
#include "afx/afxApplication.h"
#include "afx/core/diag/afxDebug.h"
#include "afx/math/afxMathDefs.h"

#define ENABLE_DRAW 1

#ifdef ENABLE_DRAW
#include "afx/draw/afxDrawSystem.h"
#include "afx/sim/rendering/afxRenderer.h"
#endif

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

void UpdateFrameMovement(const afxReal DeltaTime)
{
    afxError err = AFX_ERR_NONE;

    const afxReal MovementThisFrame = DeltaTime * CameraSpeed;

    // Note: because the NegZ axis is forward, we have to invert the way you'd normally
    // think about the 'W' or 'S' key's action.  Also, we don't have a key for moving the
    // camera up and down, but it should be clear how to add one.
    const afxReal ForwardSpeed = (AfxKeyIsPressed(kbd, AFX_KEY_W) ? -1 : 0.0f) + (AfxKeyIsPressed(kbd, AFX_KEY_S) ? 1 : 0.0f);
    const afxReal RightSpeed = (AfxKeyIsPressed(kbd, AFX_KEY_A) ? -1 : 0.0f) + (AfxKeyIsPressed(kbd, AFX_KEY_D) ? 1 : 0.0f);

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

    afxArchive arc = AfxAcquireArchive(AFX_FILE_FLAG_R, &uri128.uri);
    AfxAssertObject(arc, AFX_FCC_ARC);
    afxUri itemNam;
    AfxUriWrapLiteral(&itemNam, "art/worldtest.tga", 0);
    afxNat itemIdx;
    AfxFindArchiveItem(arc, &itemNam, &itemIdx);
    afxStream item;
    AfxOpenArchiveItem(arc, itemIdx, &item);


    AfxUriWrapLiteral(&itemNam, "tmp/worldtest.tga", 0);
    AfxDownloadArchiveItem(arc, itemIdx, &itemNam);

    AfxUriWrapLiteral(&uriMap, "e2newton.icd", 0);
    afxSimulationSpecification simSpec = { 0 };
    simSpec.bounding = NIL;
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    sim = AfxApplicationAcquireSimulation(app, &simSpec);
    AfxAssertObject(sim, AFX_FCC_SIM);

    AfxUriWrapLiteral(&uriMap, "window", 0);
    afxDrawOutputConfig doutConfig = {0};
    doutConfig.pixelFmt = AFX_PFD_RGB8_SRGB;
    doutConfig.endpoint = &uriMap;
    AfxAcquireDrawOutputs(&doutConfig, 1, &dout);
    AfxAssert(dout);
    AfxReconnectDrawOutput(dout, dctx, NIL);

    AfxSimulationAcquireRenderers(sim, 1, NIL, &rnd);
    
    AfxRendererBindOutput(rnd, dout);

    
    AfxWrapStringLiteral(&str, "viewer", 0);
    cam = AfxSimulationAcquireCamera(sim, &str, NIL, TRUE);
    AfxAssert(cam);

    rnd->activeCamera = cam;

    kbd = AfxFindKeyboard(0);
    mse = AfxFindMouse(0);
    AfxObjectInstallEventFilter(&mseD->hid.obj, &cam->nod.obj);
    
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

        afxDrawSystem dsys;
        afxDrawSystemSpecification dsysSpec = { 0 };
        AfxEstablishDrawSystem(&dsysSpec, &dsys);
        AfxAssertType(dsys, AFX_FCC_DSYS);

        afxDrawContextConfig dctxConfig = { 0 };
        AfxAcquireDrawContexts(&dctxConfig, 1, &dctx);
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
        AfxAcquireApplications(&appConfig, 1, &TheApp);
        AfxAssertObject(TheApp, AFX_FCC_APP);
        AfxRunApplication(TheApp);

        while (AFX_OPCODE_CONTINUE == AfxDoSystemThreading(0));

        AfxReleaseApplications(1, &TheApp);

        AfxReleaseObject(&dctxD->obj);

        AfxAbolishDrawSystem();
        AfxShutdownBasicIoSystem();
    }
    Sleep(3000);
    return 0;
}
