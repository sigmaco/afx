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

afxNode controledNod = NIL;
afxUri4096 uri;
typedef struct
{
    afxSkyType          skyType;
    afxVertexBuffer     skyGeom;
    afxTexture          skyTex;
    afxColor            skyEmissiveColor;
    afxColor            skyAmbientColor;
    afxDrawOperation    skyDop;
    afxSampler          skySmp;

    afxCamera           cam;
    struct
    {
        afxCanvas       canv;
        afxBuffer       viewConstants; // p, v
        afxLego         viewLego;
    }                   frameSet[2];
    afxNat              frameCnt;
    afxNat              frameIdx;
} skyRenderer;

skyRenderer TheSkyRenderer =
{
    NIL,
    NIL,
    NIL,
    { 1.0,1.0,1.0,1.0 },
    { 0.1,0.1,0.1,1.0 },
    NIL,
    NIL
};
//afxSoundSystem ssys = NIL;

 afxSize frameTime = 0, swapTime = 0, frameNum = 0;
 afxReal dt = 0;
 afxNat fps = 0;

_AFXEXPORT afxError ExecuteRenderer(afxDrawInput din, afxNat qid, skyRenderer *renderer) // called by draw context
{
    afxError err = NIL;

    afxDrawScript dscr = AfxDrawInputAcquireScript(din, 1);

    if (!dscr) AfxThrowError();
    else
    {
        if (AfxDrawScriptBegin(dscr, NIL)) AfxThrowError();
        else
        {
            afxNat outBufIdx = 0;
            AfxDrawOutputRequestBuffer(dout, 0, &outBufIdx);
            afxCanvas canv = renderer->frameSet[outBufIdx].canv;

            afxRect drawArea = { 0 };
            AfxAssertObject(canv, AFX_FCC_CANV);
            AfxCanvasGetExtent(canv, drawArea.extent);

            afxRenderPassAnnex const fbufAnnexes[] = { { TRUE, FALSE, {.color = { 0.3, 0.2, 0.1, 1 } } }, { TRUE, FALSE, {.depth = 1.f,.stencil = 0 } } };
            AfxDrawScriptCmdBeginOperation(dscr, renderer->skyDop, 0, canv, &drawArea, 2, fbufAnnexes);
            
            afxWhd extent;
            AfxCanvasGetExtent(canv, extent);
            afxViewport vp = { 0 };
            vp.extent[0] = extent[0];
            vp.extent[1] = extent[1];
            vp.depth[0] = (afxReal)0;
            vp.depth[1] = (afxReal)1;
            AfxDrawScriptCmdSetViewport(dscr, 0, 1, &vp);
#if 0
            afxPipelineRasterizerState ras = { 0 };
            ras.cullMode = AFX_CULL_MODE_BACK;
            ras.fillMode = AFX_FILL_MODE_SOLID;
            ras.frontFace = AFX_FRONT_FACE_CCW;
            ras.lineWidth = 1.f;
            AfxDrawScriptCmdSetRasterizerState(dscr, &ras);

            afxPipelineDepthState depth = { 0 };
            depth.depthTestEnable = TRUE;
            depth.depthWriteEnable = TRUE;
            depth.depthCompareOp = AFX_COMPARE_OP_LESS;
            AfxDrawScriptCmdSetDepthState(dscr, &depth);
#endif
            afxM4d camMtx;
            AfxNodeCopyAbsoluteTransform(AfxCameraGetNode(renderer->cam), camMtx);

            afxReal fov = AfxCameraGetFov(renderer->cam);
            afxReal clipPlanes[2];
            AfxCameraGetClipPlanes(renderer->cam, clipPlanes);
            afxM4d v, ci;
            AfxM4dMakeInverse(ci, camMtx);
            //AfxM4dCopy(ci, camMtx);
            AfxM4dCopy(v, ci);
            //AfxCamera.CopyViewMatrix(renderer->cam, v);
            //AfxM4dMakeRwView(v, ci);
            //AfxM4dCopy(renderer->v, v);

            afxM4d p;
            AfxM4dMakePerspective(p, fov, AfxScalar(vp.extent[0]) / AfxScalar(vp.extent[1]), clipPlanes[0], clipPlanes[1]);
            //AfxM4dMakeRwProjection(p, vp.offset, vp.extent, clipPlanes[0], clipPlanes[1], renderer->cam->perspective);
            //AfxM4dCopy(renderer->p, p);

            afxViewConstants view;
            AfxM4dCopy(view.v, v);
            AfxM4dCopy(view.p, p);
            //AfxM4dCopy(renderer->p, p);
            AfxBufferUpdate(renderer->frameSet[0].viewConstants, 0, sizeof(view), &view);

            AfxDrawScriptCmdBindLegos(dscr, 0, 1, &renderer->frameSet[0].viewLego);

            afxNat vtxArr[] = { 0 };
            AfxDrawScriptCmdBindVertexBuffers(dscr, 0, 1, &renderer->skyGeom, vtxArr, vtxArr);
            AfxDrawScriptCmdDraw(dscr, 36, 1, 0, 0);

            AfxDrawScriptCmdEndOperation(dscr);


            if (AfxDrawScriptEnd(dscr)) AfxThrowError();
            else
            {
                if (AfxDrawInputEnqueueSubmission(din, 1, &dscr)) // draw output count hardcoded to 1.
                    AfxThrowError();

                //if (qid == 0)
                if (AfxDrawInputEnqueuePresentation(din, 1, &dout, &outBufIdx)) // draw output count hardcoded to 1.
                    AfxThrowError();
            }
        }
    }
    return err;
}

afxError SetUpRenderer(skyRenderer *renderer, afxSimulation sim)
{
    afxError err = NIL;

    renderer->cam = cam;
    //renderer->viewVolume

    afxTextureRegion rgn;
    rgn.lod = 0;
    rgn.layerCnt = 6;
    rgn.baseLayer = 0;
    rgn.offset[0] = 0;
    rgn.offset[1] = 0;
    rgn.offset[2] = 0;
    
    // sky
    afxTextureBlueprint texb;
    afxString name;
    AfxStringMapWriteable(&name, "blue", 0);
    afxWhd skyWhd = { 2048, 2048, 1 };
    AfxTextureBlueprintReset(&texb, &name, AFX_PIXEL_FMT_RGB8, skyWhd, AFX_TEX_CUBEMAP);
    afxUri cubeUri[6];
    AfxUriReflectData(&cubeUri[0], "art/skybox/day/right.tga", 0);
    AfxUriReflectData(&cubeUri[1], "art/skybox/day/left.tga", 0);
    AfxUriReflectData(&cubeUri[2], "art/skybox/day/bottom.tga", 0);
    AfxUriReflectData(&cubeUri[3], "art/skybox/day/top.tga", 0);
    AfxUriReflectData(&cubeUri[4], "art/skybox/day/front.tga", 0);
    AfxUriReflectData(&cubeUri[5], "art/skybox/day/back.tga", 0);
    AfxTextureBlueprintAddImagesFromResource(&texb, 6, cubeUri);

    AfxDrawContextBuildTextures(dctx, 1, &texb, &renderer->skyTex);
    
    afxUri uri;

    AfxColorSet(renderer->skyAmbientColor, 0.1, 0.1, 0.1, 1);
    AfxColorSet(renderer->skyEmissiveColor, 0.1, 0.1, 0.1, 1);
    
    afxReal skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    //for (afxNat i = 0; i < 36 * 3; i++)
    //    skyboxVertices[i] *= 100;

    afxString skyPosSem;
    afxVertexBufferBlueprint vbub;
    AfxVertexBufferBlueprintReset(&vbub, ((sizeof(skyboxVertices) / sizeof(afxReal)) / 3));
    AfxVertexBufferBlueprintAddRow(&vbub, AfxStringMap(&skyPosSem, "a_xyz", 0), AFX_VTX_FMT_XYZ32, AFX_VTX_USAGE_POS, skyboxVertices, AFX_VTX_FMT_XYZ32);
    AfxDrawContextBuildVertexBuffers(dctx, 1, &vbub, &renderer->skyGeom);
    AfxAssertObject(renderer->skyGeom, AFX_FCC_VBUF);
    AfxUriReflectData(&uri, "data/pipeline/skybox.urd", 0);
    AfxDrawContextAcquireOperations(dctx, 1, &uri, &renderer->skyDop);
    renderer->skyType = 0;

    afxPipeline pip = AfxDrawOperationGetPipeline(renderer->skyDop, 0, 0);
    afxPipelineRig pipr = AfxPipelineGetRig(pip);

    afxLegoTemplate bindSchema[4];
    AfxPipelineRigEnumerateTemplates(pipr, 0, 1, bindSchema);

    afxBufferSpecification const bufSpec[] =
    {
        { sizeof(afxViewConstants), AFX_BUF_USAGE_CONSTANT, NIL }
    };

    afxSamplerSpecification smpSpec = { 0 };
    smpSpec.magFilter = AFX_TEXEL_FLT_LINEAR;
    smpSpec.minFilter = AFX_TEXEL_FLT_LINEAR;
    smpSpec.mipmapFilter = AFX_TEXEL_FLT_LINEAR;
    smpSpec.uvw[0] = AFX_TEXEL_ADDR_CLAMP;
    smpSpec.uvw[1] = AFX_TEXEL_ADDR_CLAMP;
    smpSpec.uvw[2] = AFX_TEXEL_ADDR_CLAMP;
    
    renderer->skySmp = AfxDrawContextAcquireSampler(dctx, &smpSpec);
    AfxAssertObject(renderer->skySmp, AFX_FCC_SMP);

    afxLegoData points[4];

    for (afxNat i = 0; i < 2; i++)
    {
        afxResult rslt = AfxDrawOutputBuildCanvases(dout, 1, (afxNat[]) { i }, (afxPixelFormat[]) { AFX_PIXEL_FMT_D32 }, NIL, &renderer->frameSet[i].canv);
        AfxAssert(rslt == 1);

        renderer->frameSet[i].viewConstants = AfxDrawContextAcquireBuffer(dctx, &bufSpec[0]);

        points[0].buf = renderer->frameSet[i].viewConstants;
        points[0].base = 0;
        points[0].range = bufSpec->siz;
        points[1].tex = renderer->skyTex;
        points[1].smp = renderer->skySmp;

        AfxDrawContextAcquireLegos(dctx, 1, &bindSchema[0], points, &renderer->frameSet[i].viewLego);

    }

    return err;
}

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

    afxString str, str2;
    afxUri uriMap;

    afxFileSystem fsys = AfxDrawSystemGetFileSystem(dsys);
    AfxAssertObject(fsys, AFX_FCC_FSYS);
    afxStoragePointSpecification mpSpec = { 0 };
    afxUri256 uri256;
    AfxUri256(&uri256, NIL);
    mpSpec.hostPath = AfxUriFormat(&uri256.uri, "art/mnt.zip");
    mpSpec.namespace = AfxUriReflectData(&uriMap, "art", 0);
    mpSpec.perm = AFX_IO_FLAG_R;
    afxResult rslt = AfxFileSystemMountStoragePoints(fsys, 1, &mpSpec);
    AfxAssert(rslt == 1);

    afxArchive arc = AfxFileSystemAcquireArchive(fsys, AFX_IO_FLAG_R, &uri256.uri);
    AfxAssertObject(arc, AFX_FCC_ARC);
    afxUri itemNam;
    AfxUriReflectData(&itemNam, "art/worldtest.tga", 0);
    afxNat itemIdx = AfxArchiveFindItem(arc, &itemNam);
    afxStream item = AfxArchiveOpenItem(arc, itemIdx);


    AfxUriReflectData(&itemNam, "system/worldtest.tga", 0);
    AfxArchiveDownloadItem(arc, itemIdx, &itemNam);

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

    afxDrawInputSpecification dinSpec = { 0 };
    dinSpec.prefetch = (void*)ExecuteRenderer;
    dinSpec.udd = &TheSkyRenderer;
    dinSpec.cmdPoolMemStock = 4096;
    dinSpec.estimatedSubmissionCnt = 3;
    dinSpec.enabledPresentationThreads = (afxNat[]) { 1, 0, 0, 0 };
    dinSpec.enabledStreamingThreads = (afxNat[]) { 1, 1, 1, 1 };
    dinSpec.enabledSubmissionThreads = (afxNat[]) { 1, 1, 1, 1 };
    din = AfxDrawContextAcquireInput(dctx, &dinSpec);
    AfxAssert(din);

    AfxStringMap(&str, "viewer", 0);
    cam = AfxSimulationAcquireCamera(sim, &str, NIL, TRUE);
    AfxAssert(cam);
    //cam->farClip = -100000.0;
    controledNod = &cam->nod;
    //AfxAttachViewpoint(vpnt, cam);
    
    SetUpRenderer(&TheSkyRenderer, sim);
    AfxDrawInputAffinePrefetchThreads(din, 0, 1, (afxNat[]) { 1 });
    
    return AFX_SUCCESS; 
}

_AFXEXPORT afxResult AfxLeaveApplication(afxApplication app)
{
    AfxEntry("app=%p", app);
    AfxObjectRelease(&din->obj);
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
