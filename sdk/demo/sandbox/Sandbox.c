#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "afx/afxQwadro.h"
#include "afx/afxApplication.h"
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
afxBody bod = NIL;

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
    const afxReal ForwardSpeed = (AfxKeysIsPressed(kbd, AFX_KEY_W) ? -1 : 0.0f) + (AfxKeysIsPressed(kbd, AFX_KEY_S) ? 1 : 0.0f);
    const afxReal RightSpeed = (AfxKeysIsPressed(kbd, AFX_KEY_A) ? -1 : 0.0f) + (AfxKeysIsPressed(kbd, AFX_KEY_D) ? 1 : 0.0f);

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
    //AfxRendererDrawSky(rnd, TRUE);

    AfxRendererDrawBody(rnd, bod);

    AfxRendererEndScene(rnd);

    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxEnterApplication(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    afxUri uriMap;
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
    doutSpec.bufUsage = AFX_TEX_FLAG_SURFACE_RASTER;

    dout = AfxAcquireDrawOutputs(&doutSpec);
    AfxAssert(dout);
    AfxReconnectDrawOutput(dout, dctx);

    AfxSimulationAcquireRenderers(sim, 1, NIL, &rnd);

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
    AfxSimulationAcquireModels(sim, 1, &uriMap2, &mdl);

    //AfxUriWrapLiteral(&uriMap, "art/scenario/TV-Stand-5/TV-Stand-5.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/scenario/gtabr/gtabr.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/f16/f16.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/scenario/bibliotheca/bibliotheca.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/scenario/zero/zero.obj", 0);
    //AfxUriWrapLiteral(&uriMap, "art/scenario/SpaceStation/SpaceStation.obj", 0);
    AfxUriWrapLiteral(&uriMap, "art/object/container/container.obj", 0);
    //AfxSimulationLoadObjAssets(sim, &uriMap, NIL);

    //AfxWrapStringLiteral(&str, "bibliotheca", 0);
    //AfxString32DeployRaw(&str, "TV-Stand-5", 0);
    //AfxString32DeployRaw(&str, "SpaceStation", 0);
    //AfxString32DeployRaw(&str, "f16", 0);
    //AfxWrapStringLiteral(&str, "zero", 0);
    //AfxWrapStringLiteral(&str, "container", 0);


    AfxExcerptUriObject(&uriMap2, &uriMap);
    AfxSimulationAcquireModels(sim, 1, &uriMap2, &mdl);
    // TODO FetchModel(/dir/to/file)

    AfxSimulationAcquireBodies(sim, 1, &mdl, &bod);
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
    cam = AfxSimulationAcquireCamera(sim, &str, NIL, TRUE);
    AfxAssert(cam);
    //cam->farClip = -100000.0;
    //AfxCameraAddOffset(cam, AfxSpawnV3d(0, 1.1, 0));

    //AfxAttachViewpoint(vpnt, cam);

    kbd = AfxFindKeyboard(0);
    mse = AfxFindMouse(0);
    AfxObjectInstallEventFilter(&mse->hid.obj, &cam->nod.obj);

    AfxEnableDrawInputPrefetching(rnd->din, 1); // bug: sem isso não desenha

    return AFX_SUCCESS; 
}

_AFXEXPORT afxResult AfxLeaveApplication(afxApplication app)
{
    AfxEntry("app=%p", app);
    //AfxReleaseObject(&din->obj);
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
    AfxFormatUri(&romUri.uri, "%s", argv[0]); // hardcoded name

    afxBool reboot = 1;
    while (reboot)
    {
        sys = AfxBootUpSystem(NIL);

        afxDrawSystemSpecification dsysSpec = { 0 };
        dsys = AfxBootUpDrawSystem(&dsysSpec);
        AfxAssertObject(dsys, AFX_FCC_DSYS);

        afxDrawContextSpecification dctxSpec = { 0 };

        dctx = AfxAcquireDrawContexts(&dctxSpec);
        AfxAssert(dctx);

        afxApplicationSpecification appSpec;
        appSpec.argc = argc;
        appSpec.argv = argv;
        appSpec.dctx = dctx;
        appSpec.enter = AfxEnterApplication;
        appSpec.exit = AfxLeaveApplication;
        appSpec.update = AfxUpdateApplication;
        TheApp = AfxAcquireApplication(&appSpec);
        AfxAssertObject(TheApp, AFX_FCC_APP);

        if (AFX_OPCODE_BREAK == AfxRunApplication(TheApp))
            reboot = 0;

        AfxReleaseObject(&TheApp->obj);

        AfxReleaseObject(&dctx->obj);

        AfxShutdownDrawSystem();
        AfxShutdownSystem();
    }
    Sleep(3000);
    return 0;
}
