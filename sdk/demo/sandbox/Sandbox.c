#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "afx/afxQwadro.h"
#include "afx/afxApplication.h"
#include "afx/core/diag/afxDebug.h"
#include "afx/math/afxQuat.h"

#define ENABLE_DRAW 1

#ifdef ENABLE_DRAW
#include "afx/draw/afxDrawSystem.h"
#include "afx/simul/render/afxRenderer.h"
#endif

afxSystem sys = NIL;
afxDrawSystem dsys = NIL;
afxApplication TheApp = NIL;
afxSimulation sim = NIL;

afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
afxDrawInput din = NIL;

afxCamera cam = NIL;
afxBody body = NIL;
afxEntity ent = NIL;

afxNode controledNod = NIL;
afxUri4096 uri;
afxRenderer TheRenderer = { 0 };
afxRenderer *renderer = &TheRenderer;
//afxSoundSystem ssys = NIL;


 afxSize frameTime = 0, swapTime = 0, frameNum = 0;
 afxReal dt = 0;
 afxNat fps = 0;

 afxBool WorldRender = TRUE;
#if 0
 void
     ClumpsRotate(afxReal x, afxReal y)
 {
     if (WorldRender)
     {
         afxNode frame;
         const afxV4d *right;
         afxV4d pos, delta;
         afxReal angleX, angleY;

         angleX = (-x / 10);
         angleY = (y / 20);

         frame = AfxCamera.GetNode(cam);

         /*
          * Remember where the camera is...
          */
         AfxM4dGetTranslation(frame->m, pos);

         /*
          * Remove the translation component...
          */
         AfxV3dGetScaled(pos, -1.0f, delta);
         AfxNodeAddTranslation(frame, delta); // post concat matrix

         /*
          * Rotate to the new direction...
          */
         right = frame->m[0];
         AfxNodeAddRotation(frame, right, angleY); // post concat matrix
         AfxNodeAddRotation(frame, &Yaxis, angleX); // post concat matrix

         /*
          * Put the camera back to where it was...
          */
         RwFrameTranslate(frame, &pos, rwCOMBINEPOSTCONCAT); // post concat matrix
     }
     else
     {
         RwRaster *cameraRaster;
         RwFrame *clumpFrame;
         RwReal angleX, angleY;

         angleX = (x / 10);
         angleY = (-y / 10);

         cameraRaster = RwCameraGetRaster(Camera);

         clumpFrame = RpClumpGetFrame(Clump);

         RwFrameRotate(clumpFrame, &Xaxis, angleY, rwCOMBINEPOSTCONCAT);
         RwFrameRotate(clumpFrame, &Yaxis, angleX, rwCOMBINEPOSTCONCAT);
     }
 }
#endif

_AFXEXPORT afxResult AfxUpdateApplication(afxApplication app)
{
    afxError err = NIL;
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

#ifdef ENABLE_DRAW
    //afxDrawScript dscr = AfxDrawInput_PrepareBuffer(din1);

    //AfxDrawInput_ConcludeBuffer(din1);
    afxKeyboard kbd = AfxSystemFindKeyboard(sys, 0);
    AfxAssertObject(kbd, AFX_FCC_KBD);

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_1))
        controledNod = &cam->nod;
    else if (AfxKeyboardIsPressed(kbd, AFX_KEY_2))
        controledNod = &body->nod;

    afxM4d m;
    AfxNodeCopyTransform(controledNod, m);
    afxV4d axis;
    AfxM4dGetLookAt(m, axis);

    static afxReal factor = 1.f;

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_P))
        factor += 100.0;
    else
        factor -= 10.0;

    if (factor < 1.0)
        factor = 1.0;

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_A))
        AfxNodeAddTranslation(controledNod, AfxSpawnV4d(-10 * factor * dt, 0, 0, 0));

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_D))
        AfxNodeAddTranslation(controledNod, AfxSpawnV4d(10 * factor *dt, 0, 0, 0));

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_W))
        AfxNodeAddTranslation(controledNod, AfxSpawnV4d(0, 0, -10 * factor *dt, 0));

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_S))
        AfxNodeAddTranslation(controledNod, AfxSpawnV4d(0, 0, 10 * factor *dt, 0));

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_UP))
        AfxNodeAddTranslation(controledNod, AfxSpawnV4d(0, 10 * factor *dt, 0, 0));

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_DOWN))
        AfxNodeAddTranslation(controledNod, AfxSpawnV4d(0, -10 * factor *dt, 0, 0));

    afxQuat q;
    AfxQuatMakeAxialRotation(q, AfxSpawnV3d(1 * dt, 0, 0), 1);

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_R))
        AfxNodeAddRotation(controledNod, q);

    AfxQuatMakeAxialRotation(q, AfxSpawnV3d(1 * dt, 0, 0), -1);

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_T))
        AfxNodeAddRotation(controledNod, q);

    AfxQuatMakeAxialRotation(q, AfxSpawnV3d(0, 1 * dt, 0), 1);

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_F))
        AfxNodeAddRotation(controledNod, q);

    AfxQuatMakeAxialRotation(q, AfxSpawnV3d(0, 1 * dt, 0), -1);

    if (AfxKeyboardIsPressed(kbd, AFX_KEY_G))
        AfxNodeAddRotation(controledNod, q);

    //if (AfxKeyboardIsPressed(kbd, AFX_KEY_V))
        //AfxNodeAddScale(controledNod, AfxSpawnV3d(0, 0, 1 * dt));

    //if (AfxKeyboardIsPressed(kbd, AFX_KEY_B))
        //AfxNodeAddScale(controledNod, AfxSpawnV3d(0, 0, -1 * dt));
    
    //afxQuat orient;
    //AfxQuatMakeAxialRotation(orient, AfxSpawnV3d(1, 1, 1), 1.f);
    //AfxNode_Rotate(AfxGetAttachedNode(&cam->nodal), orient);
    //AfxNode_Translate(AfxGetAttachedNode(&cam->nodal), AfxSpawnV4d(0, 0, 1 * sim->stepDeltaTime, 1));
    //AfxSimulationStep(sim, 1);
    //AfxDrawSystemProcess(dsys);
#endif
    return AFX_SUCCESS;
}

// URI128
//32323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232323232
//3232323232323232323232323232323232323232323232323232323232323232

_AFXEXPORT afxResult AfxEnterApplication(afxApplication app)
{
    afxError err = NIL;
    AfxEntry("app=%p", app);

    afxUri uriMap;
    AfxUriReflectData(&uriMap, "e2newton.icd", 0);
    afxSimulationSpecification simSpec = { 0 };
    simSpec.bounding = NIL;
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    sim = AfxApplicationAcquireSimulation(TheApp, &simSpec);
    AfxAssertObject(sim, AFX_FCC_SIM);

    AfxUriReflectData(&uriMap, "window", 0);
    afxDrawOutputSpecification doutSpec = {0};
    doutSpec.endpoint = &uriMap;
    doutSpec.bufCnt = 2;
    doutSpec.clipped = TRUE;
    doutSpec.colorSpc = NIL;
    doutSpec.presentAlpha = FALSE;
    doutSpec.pixelFmt = AFX_PIXEL_FMT_RGBA8;
    doutSpec.presentMode = AFX_PRESENT_MODE_LIFO;
    doutSpec.presentTransform = NIL;
    doutSpec.bufUsage = AFX_TEX_USAGE_SURFACE_RASTER;
    afxWhd extent = { 1280, 720, 1 };

    dout = AfxDrawContextAcquireOutput(dctx, extent, &doutSpec);
    AfxAssert(dout);

    //AfxAcquireSky();

    //AfxUriFormat(&uri, "%.*s/scenario/zero/zero.obj", AfxPushString(AfxApplication_GetArtDirectory(app)));
    //afxArray assets;
    //AfxDeployArray(&assets, NIL, 0, sizeof(void*));
    //AfxLoadAssets(sim, &uri);

#if 0
    AfxUriReflectData(&uriMap, "art/actor/hellknight/hellknight.md5mesh", 0);
    AfxSimulationLoadMD5Assets(sim, &uri, NIL);

    afxString32 str;
    AfxStringDeployRaw32(&str, "/hellknight.md5mesh", 0);
#else
    //AfxUriReflectData(&uriMap, "art/scenario/TV-Stand-5/TV-Stand-5.obj", 0);
    //AfxUriReflectData(&uriMap, "art/scenario/gtabr/gtabr.obj", 0);
    //AfxUriReflectData(&uriMap, "art/f16/f16.obj", 0);
    //AfxUriReflectData(&uriMap, "art/scenario/bibliotheca/bibliotheca.obj", 0);
    //AfxUriReflectData(&uriMap, "art/scenario/zero/zero.obj", 0);
    //AfxUriReflectData(&uriMap, "art/scenario/SpaceStation/SpaceStation.obj", 0);
    AfxUriReflectData(&uriMap, "art/object/container/container.obj", 0);

    AfxSimulationLoadObjAssets(sim, &uriMap, NIL);
#if 0
    
    AfxSimulationLoadObjAssets(sim, &uriMap, NIL);
#endif
    afxString str;
    //AfxStringMap(&str, "bibliotheca", 0);
    //AfxString32DeployRaw(&str, "TV-Stand-5", 0);
    //AfxString32DeployRaw(&str, "SpaceStation", 0);
    //AfxString32DeployRaw(&str, "f16", 0);
    //AfxStringMap(&str, "zero", 0);
    AfxStringMap(&str, "container", 0);
#endif

    afxModel mdl = AfxSimulationFindModel(sim, &str);
    // TODO FetchModel(/dir/to/file)

    body = AfxSimulationAcquireBody(sim, &str, NIL, mdl);
    AfxAssert(body);
#if 0
    mdl = AfxSimulationFindModel(sim, &str);
    body = AfxSimulationAcquireBody(sim, &str, NIL, mdl);
    AfxAssert(body);
#endif
    //AfxStringUpdate(&str, 0, "zero");
    //afxBody body2;
    //AfxAcquireBody(&body2, sim, &str, AfxFindModel(sim, &str));
    //AfxAssert(body2);

    AfxStringMap(&str, "viewer", 0);
    cam = AfxSimulationAcquireCamera(sim, &str, NIL, TRUE);
    AfxAssert(cam);
    //cam->farClip = -100000.0;
    controledNod = &body->nod;
    //AfxAttachViewpoint(vpnt, cam);
    
    afxResult rslt = _AfxBuildRenderer(&TheRenderer, dout, sim);
    AfxAssert(!rslt);

    //AfxDrawInputAffinePrefetchThreads(din, 0, 1, (afxNat[]) { 1 });
    return AFX_SUCCESS; 
}

_AFXEXPORT afxResult AfxLeaveApplication(afxApplication app)
{
    AfxEntry("app=%p", app);
    //AfxObjectRelease(&din->obj);
    AfxEcho("aaaaaaaaaaaa");

    return AFX_SUCCESS;
}

int AfxMain(afxApplication app, int argc, char const* argv[])
{

}

int main(int argc, char const* argv[])
{
    afxError err = NIL;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxUri4096 romUri;
    AfxUri4096(&romUri, NIL);
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
