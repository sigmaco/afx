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
#include "afx/sim/render/afxRenderer.h"
#endif

const afxReal CameraSpeed = 30.0f;
afxSystem sys = NIL;
afxKeyboard kbd = NIL;
afxMouse mse = NIL;
afxDrawSystem dsys = NIL;
afxApplication TheApp = NIL;
afxSimulation sim = NIL;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
afxRenderer rnd = NIL;

afxCamera cam = NIL;

afxUri2048 uri;

 afxSize frameTime = 0, swapTime = 0, frameNum = 0;
 afxReal dt = 0;
 afxNat fps = 0;

void UpdateFrameMovement(const afxReal DeltaTime)
{
    afxError err = AFX_ERR_NONE;

    const afxReal MovementThisFrame = DeltaTime * CameraSpeed;

    // Note: because the NegZ axis is forward, we have to invert the way you'd normally
    // think about the 'W' or 'S' key's action.  Also, we don't have a key for moving the
    // camera up and down, but it should be clear how to add one.
    const afxReal ForwardSpeed = (AfxKeyboardIsPressed(kbd, AFX_KEY_W) ? -1 : 0.0f) + (AfxKeyboardIsPressed(kbd, AFX_KEY_S) ? 1 : 0.0f);
    const afxReal RightSpeed = (AfxKeyboardIsPressed(kbd, AFX_KEY_A) ? -1 : 0.0f) + (AfxKeyboardIsPressed(kbd, AFX_KEY_D) ? 1 : 0.0f);

    AfxCameraMoveRelative(cam, AfxSpawnV3d(MovementThisFrame * RightSpeed, 0.0f, MovementThisFrame * ForwardSpeed));
}

_AFXEXPORT afxResult AfxUpdateApplication(afxApplication app)
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

    AfxRendererBeginScene(rnd, cam);
    AfxRendererDrawSky(rnd, TRUE);


    AfxRendererEndScene(rnd);
    
    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxEnterApplication(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    afxString str, str2;
    afxUri uriMap;

    afxFileSystem fsys = AfxDrawSystemGetFileSystem(dsys);
    AfxAssertObject(fsys, AFX_FCC_FSYS);

    afxUri128 uri128;
    AfxUri128(&uri128);
    afxStoragePointSpecification mpSpec = { 0 };
    AfxUriFormat(&uri128.uri, "art/mnt.zip");
    mpSpec.hostPath = &uri128.uri;
    AfxUriWrapLiteral(&uriMap, "art", 0);
    mpSpec.namespace = &uriMap;
    mpSpec.perm = AFX_IO_FLAG_R;
    afxResult rslt = AfxFileSystemMountStoragePoints(fsys, 1, &mpSpec);
    AfxAssert(rslt == 1);

    afxArchive arc = AfxFileSystemAcquireArchive(fsys, AFX_IO_FLAG_R, &uri128.uri);
    AfxAssertObject(arc, AFX_FCC_ARC);
    afxUri itemNam;
    AfxUriWrapLiteral(&itemNam, "art/worldtest.tga", 0);
    afxNat itemIdx;
    AfxArchiveFindItem(arc, &itemNam, &itemIdx);
    afxStream item = AfxArchiveOpenItem(arc, itemIdx);


    AfxUriWrapLiteral(&itemNam, "tmp/worldtest.tga", 0);
    AfxArchiveDownloadItem(arc, itemIdx, &itemNam);

    AfxUriWrapLiteral(&uriMap, "e2newton.icd", 0);
    afxSimulationSpecification simSpec = { 0 };
    simSpec.bounding = NIL;
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    sim = AfxApplicationAcquireSimulation(TheApp, &simSpec);
    AfxAssertObject(sim, AFX_FCC_SIM);

    AfxUriWrapLiteral(&uriMap, "window", 0);
    afxDrawOutputSpecification doutSpec = {0};
    doutSpec.endpoint = &uriMap;
    doutSpec.whd[0] = 1280;
    doutSpec.whd[1] = 720;
    doutSpec.whd[2] = 1;
    doutSpec.bufCnt = 2;
    doutSpec.clipped = TRUE;
    doutSpec.colorSpc = NIL;
    doutSpec.presentAlpha = FALSE;
    doutSpec.pixelFmt = AFX_PIXEL_FMT_RGBA8;
    doutSpec.presentMode = AFX_PRESENT_MODE_LIFO;
    doutSpec.presentTransform = NIL;
    doutSpec.bufUsage = AFX_TEX_USAGE_SURFACE_RASTER;

    dout = AfxDrawContextAcquireOutput(dctx, &doutSpec);
    AfxAssert(dout);

    AfxSimulationAcquireRenderers(sim, 1, NIL, &rnd);
    
    AfxRendererBindOutput(rnd, dout);

    
    AfxStringWrapLiteral(&str, "viewer", 0);
    cam = AfxSimulationAcquireCamera(sim, &str, NIL, TRUE);
    AfxAssert(cam);

    kbd = AfxSystemFindKeyboard(sys, 0);
    mse = AfxSystemFindMouse(sys, 0);
    AfxObjectInstallEventFilter(&mse->hid.obj, &cam->nod.obj);
    
    AfxDrawInputAffinePrefetchThreads(rnd->din, 0, 1, (afxNat[]) { 1 }); // bug: sem isso não desenha
    
    return AFX_SUCCESS; 
}

_AFXEXPORT afxResult AfxLeaveApplication(afxApplication app)
{
    AfxEntry("app=%p", app);

    AfxEcho("aaaaaaaaaaaa");

    return AFX_SUCCESS;
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
    AfxUriFormat(&romUri.uri, "%s", argv[0]); // hardcoded name

    afxBool reboot = 1;
    while (reboot)
    {
        sys = AfxSystemBootUp(NIL);
        afxDrawSystemSpecification dsysSpec = { 0 };
        dsys = AfxSystemAcquireDrawSystem(sys, &dsysSpec);
        AfxAssertObject(dsys, AFX_FCC_DSYS);

        afxDrawContextSpecification dctxSpec = { 0 };
        dctxSpec.driverId = 0;
        dctxSpec.queueCnt = 1;
        dctxSpec.autonomousQueue = FALSE;

        dctx = AfxDrawSystemAcquireContext(dsys, &dctxSpec);
        AfxAssert(dctx);

        afxApplicationSpecification appSpec = { 0 };
        appSpec.argc = argc;
        appSpec.argv = argv;
        appSpec.dctx = dctx;
        appSpec.enter = AfxEnterApplication;
        appSpec.exit = AfxLeaveApplication;
        appSpec.update = AfxUpdateApplication;
        TheApp = AfxSystemAcquireApplication(sys, &appSpec);
        AfxAssertObject(TheApp, AFX_FCC_APP);

        if (AFX_OPCODE_BREAK == AfxApplicationExecute(TheApp))
            reboot = 0;

        AfxObjectRelease(&TheApp->obj);

        AfxObjectRelease(&dctx->obj);

        AfxObjectRelease(&dsys->obj);

        AfxObjectRelease(&sys->obj);
    }
    Sleep(3000);
    return 0;
}
