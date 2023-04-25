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

afxResult _AfxDrawMesh(afxModel mdl, afxMesh msh, void *data)
{
    return 0;
}

afxResult _AfxDrawNodals(afxDrawScript dscr, afxNat qid, afxRenderer *renderer, afxNode nod)
{
    afxError err = NIL;

    if (AfxObjectGetFcc(&nod->obj) == AFX_FCC_BOD)
    {
        afxBody bod = AfxRebase(nod, afxBody, nod);

        afxSkeleton skl = AfxBodyGetSkeleton(bod);
        afxModel mdl = AfxConnectionGetObject(&bod->mdl);

        if (mdl)
        {
            AfxAssertObject(mdl, AFX_FCC_MDL);

            for (afxNat i = 0; i < mdl->mshCnt; i++)
            {
                afxMesh msh = AfxModelGetMesh(mdl, i);

                afxVertexBuffer vbuf[] = { AfxConnectionGetObject(&msh->vbuf), AfxConnectionGetObject(&msh->vbuf), AfxConnectionGetObject(&msh->vbuf) };
                afxNat baseVtx[] = { 0, 0, 0 };
                afxNat vtxArr[] = { 0, 1, 2 };
                AfxDrawScriptCmdBindVertexBuffers(dscr, 0, 3, vbuf, NIL, vtxArr);

                afxObjectConstants obj;
                afxM4d w;
                AfxNodeCopyAbsoluteTransform(nod, w);
                AfxM4dCopy(obj.m, w);
                AfxBufferUpdate(renderer->sets[0].objConstants, 0, sizeof(obj), &obj);

                afxIndexBuffer ibuf = AfxMeshGetIndexBuffer(msh);
                AfxDrawScriptCmdBindIndexBuffer(dscr, ibuf, msh->topology->baseIdx);

                afxPipelineInputAssemblyState siasp;
                siasp.topology = msh->topology->primType;
                siasp.primRestartEnable = FALSE;
                AfxDrawScriptCmdSetInputAssemblyState(dscr, &siasp);

                for (afxNat j = 0; j < msh->topology->secCnt; j++)
                {
                    afxMeshSection *sec = AfxMeshGetSection(msh, j);
                    AfxAssert(msh->topology->primType == AFX_PRIM_TOPOLOGY_TRI_LIST);

                    afxMaterialConstants mat;

                    if (sec->mtlIdx == AFX_INVALID_INDEX)
                    {
                        AfxColorSet(mat.color, 0.3, 0.3, 0.3, 1.0);
                        mat.hasDiffTex = FALSE;
                        AfxBufferUpdate(renderer->sets[0].mtlConstants, 0, sizeof(mat), &mat);
                    }
                    else
                    {
                        AfxAssert(sec->mtlIdx < msh->mtlCnt);
                        afxMaterial mtl = AfxConnectionGetObject(&msh->materials[sec->mtlIdx]);
                        AfxAssertObject(mtl, AFX_FCC_MTL);
                        afxTexture tex = AfxConnectionGetObject(&mtl->tex);

                        if (tex)
                        {
                            AfxAssertObject(tex, AFX_FCC_TEX);
                            AfxColorSet(mat.color, 0.3, 0.3, 0.3, 1.0);
                            mat.hasDiffTex = TRUE;
                        }
                        else
                        {
                            AfxColorSet(mat.color, 0.3, 0.3, 0.3, 1.0);
                            mat.hasDiffTex = FALSE;
                        }

                        AfxBufferUpdate(renderer->sets[0].mtlConstants, 0, sizeof(mat), &mat);
                    }

                    AfxDrawScriptCmdDrawIndexed(dscr, sec->triCnt, 1, msh->topology->baseIdx + sec->baseTriIdx, 0, 0);
                }
            }
        }
    }
    return 0;
}

void _AfxRendererDoDrawForObjects(afxDrawScript dscr, afxNat qid, afxRenderer *renderer)
{
    afxError err = NIL;

    AfxDrawScriptCmdBindPipeline(dscr, renderer->basePip);

    AfxDrawScriptCmdBindLegos(dscr, 0, 1, &renderer->sets[0].viewLego);
    AfxDrawScriptCmdBindLegos(dscr, 1, 1, &renderer->sets[0].shdLego);
    AfxDrawScriptCmdBindLegos(dscr, 2, 1, &renderer->sets[0].mtlLego);
    AfxDrawScriptCmdBindLegos(dscr, 3, 1, &renderer->sets[0].objLego);

    if ((renderer->cam))
    {
        afxArray(afxNode) *capturedNodes = &renderer->capturedNodes;
         
        AfxArrayEmpty(capturedNodes);
        AfxCameraCaptureNodes(renderer->cam, renderer->p, NIL, capturedNodes);
        AfxCameraCopyFrustum(renderer->cam, &renderer->viewVolume);

        afxFrustum const* viewVolume = (afxFrustum const*)&(renderer->viewVolume);

        //AfxBuffer_Write(renderer->ubo[0], &(AfxSpawnSegment(offsetof(struct _xfxUbo0, sgl_p), sizeof(afxM4d), 0)), din->transforms[AFX_DIN_TRANSFORM_P]);
        //AfxBuffer_Write(renderer->ubo[0], &(AfxSpawnSegment(offsetof(struct _xfxUbo0, sgl_v), sizeof(afxM4d), 0)), din->transforms[AFX_DIN_TRANSFORM_V]);
        //AfxBuffer_Write(renderer->ubo[0], &(AfxSpawnSegment(offsetof(struct _xfxUbo0, sgl_vp), sizeof(afxM4d), 0)), din->transforms[AFX_DIN_TRANSFORM_VP]);
        //AfxPrescribeBindBuffer(dscr, renderer->ubo[0], 0, 0, 0, 0, 0);

        //AfxBuffer_Write(renderer->ubo[1], &(AfxSpawnSegment(offsetof(struct _xfxUbo1, m), sizeof(afxM4d), 0)), din->transforms[AFX_DIN_TRANSFORM_M]);
        //AfxBuffer_Write(renderer->ubo[1], &(AfxSpawnSegment(offsetof(struct _xfxUbo1, v), sizeof(afxM4d), 0)), din->transforms[AFX_DIN_TRANSFORM_V]);
        //AfxBuffer_Write(renderer->ubo[1], &(AfxSpawnSegment(offsetof(struct _xfxUbo1, vit), sizeof(afxM4d), 0)), din->transforms[AFX_DIN_TRANSFORM_VIT]);
        //AfxBuffer_Write(renderer->ubo[1], &(AfxSpawnSegment(offsetof(struct _xfxUbo1, p), sizeof(afxM4d), 0)), din->transforms[AFX_DIN_TRANSFORM_P]);
        //AfxBuffer_Write(renderer->ubo[1], &(AfxSpawnSegment(offsetof(struct _xfxUbo1, wirecolor), sizeof(afxV3d), 0)), AfxSpawnV3d(1,1,1));
        //AfxBuffer_Write(renderer->ubo[1], &(AfxSpawnSegment(offsetof(struct _xfxUbo1, vp), sizeof(afxV2d), 0)), AfxSpawnV2d(din->surfaceArea.extent[0] * dout->width, din->surfaceArea.extent[1] * dout->height));
        //AfxBuffer_Write(renderer->ubo[1], &(AfxSpawnSegment(offsetof(struct _xfxUbo1, color), sizeof(afxV3d), 0)), AfxSpawnV3d(1, 0, 1));
        //AfxBuffer_Write(renderer->ubo[1], &(AfxSpawnSegment(offsetof(struct _xfxUbo1, shininess), sizeof(afxReal), 0)), AfxSpawnV2d(35.f, 0));
        //AfxPrescribeBindBuffer(dscr, renderer->ubo[1], 0, 0, 0, 1, 0);

        for (afxNat k = 0; k < AfxArrayGetPop(capturedNodes); ++k)
        {
            afxNode nod = *(afxNode*)AfxArrayGetElement(capturedNodes, k);
            AfxAssertObject(nod, AFX_FCC_NOD);

            _AfxDrawNodals(dscr, qid, renderer, nod);
        }
    }
}

_AFXEXPORT afxError _AfxRendererDoDraw(afxDrawInput din, afxNat qid, afxRenderer *renderer) // called by draw context
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
            afxCanvas canv = renderer->sets[outBufIdx].canv;

            afxRect drawArea = { 0 };
            AfxAssertObject(canv, AFX_FCC_CANV);
            AfxCanvasGetExtent(canv, drawArea.extent);

            afxRenderPassAnnex const fbufAnnexes[] = { { TRUE, FALSE, {.color = { 0.3, 0.2, 0.1, 1 } } }, { TRUE, FALSE, {.depth = 1.f,.stencil = 0 } } };
            AfxDrawScriptCmdBeginRenderPass(dscr, canv, &drawArea, 2, fbufAnnexes);

            AfxDrawScriptCmdBindPipeline(dscr, renderer->skyPip);

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
            AfxM4dCopy(renderer->p, p);
            AfxBufferUpdate(renderer->sets[0].viewConstants, 0, sizeof(view), &view);

            AfxDrawScriptCmdBindLegos(dscr, 0, 1, &renderer->sets[0].viewLego);

            afxNat vtxArr[] = { 0 };
            AfxDrawScriptCmdBindVertexBuffers(dscr, 0, 1, &renderer->skyGeom, vtxArr, vtxArr);
            AfxDrawScriptCmdDraw(dscr, 36, 1, 0, 0);

            //_AfxRendererDoDrawForObjects(dscr, qid, renderer);

            AfxDrawScriptCmdEndRenderPass(dscr);


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

afxError _AfxSetUpRenderer(afxRenderer *renderer, afxSimulation sim)
{
    afxError err = NIL;

    renderer->sim = sim;
    renderer->dctx = sim->dctx;
    renderer->din = NIL;
    renderer->dout = NIL;

    renderer->cam = cam;
    //renderer->viewVolume
    AfxArrayDeploy(&renderer->capturedNodes, sizeof(afxNode), 10);

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
    AfxStringMap(&name, "blue", 0);
    afxWhd skyWhd = { 2048, 2048, 1 };
    AfxTextureBlueprintDeploy(&texb, &name, AFX_PIXEL_FMT_RGB8, skyWhd, 1, 6, AFX_TEX_CUBEMAP);
    afxUri cubeUri[6];
    AfxUriMapConstData(&cubeUri[0], "art/skybox/day/right.tga", 0);
    AfxUriMapConstData(&cubeUri[1], "art/skybox/day/left.tga", 0);
    AfxUriMapConstData(&cubeUri[2], "art/skybox/day/bottom.tga", 0);
    AfxUriMapConstData(&cubeUri[3], "art/skybox/day/top.tga", 0);
    AfxUriMapConstData(&cubeUri[4], "art/skybox/day/front.tga", 0);
    AfxUriMapConstData(&cubeUri[5], "art/skybox/day/back.tga", 0);
    AfxTextureBlueprintSetSourceUrl(&texb, 0, 6, cubeUri);

    AfxDrawContextBuildTextures(renderer->dctx, 1, &texb, &renderer->skyTex);
    AfxTextureBlueprintDrop(&texb);
    
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
    afxVertexDataSpecification skyGeomSpec;
    skyGeomSpec.fmt = (skyGeomSpec.srcFmt = AFX_VTX_FMT_XYZ32);
    skyGeomSpec.src = skyboxVertices;
    skyGeomSpec.usage = AFX_VTX_USAGE_POS;
    skyGeomSpec.semantic = AfxStringMapConst(&skyPosSem, "a_xyz", 0);
    renderer->skyGeom = AfxDrawContextBuildVertexBuffer(renderer->dctx, 36, 1, &skyGeomSpec);
    AfxAssertObject(renderer->skyGeom, AFX_FCC_VBUF);
    AfxUriMapConstData(&uri, "data/pipeline/skybox.pip.urd", 0);
    renderer->skyPip = AfxDrawContextFetchPipeline(renderer->dctx, &uri);
    renderer->skyType = 0;



    afxLegoSchema bindSchema[4];
    afxPipelineRig pipr = AfxPipelineGetRig(renderer->skyPip);
    AfxPipelineRigGetSockets(pipr, 0, 1, bindSchema);

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

    afxLegoPoint points[4];

    for (afxNat i = 0; i < 2; i++)
    {
        afxSurfaceSpecification const depthSurfSpec = { NIL, AFX_PIXEL_FMT_D24S8, AFX_TEX_USAGE_SURFACE_DEPTH };
        afxResult rslt = AfxDrawOutputBuildCanvases(dout, i, 1, 1, &depthSurfSpec, &(renderer->sets[i].canv));
        AfxAssert(rslt == 1);

        renderer->sets[i].viewConstants = AfxDrawContextAcquireBuffer(renderer->dctx, &bufSpec[0]);

        points[0].resource.data.buf = renderer->sets[i].viewConstants;
        points[0].resource.data.base = 0;
        points[0].resource.data.range = bufSpec->siz;
        points[1].resource.img.tex = renderer->skyTex;
        points[1].resource.img.smp = renderer->skySmp;

        renderer->sets[i].viewLego = AfxDrawContextAcquireLego(renderer->dctx, &bindSchema[0], points);

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

    // deveriam ser movidos para afxDrawInput para separa��o e exibi��o correta para cada sa�da?

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

    afxUri uriMap;
    AfxUriMapConstData(&uriMap, "e2newton.icd", 0);
    afxSimulationSpecification simSpec = { 0 };
    simSpec.bounding = NIL;
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    sim = AfxApplicationAcquireSimulation(TheApp, &simSpec);
    AfxAssertObject(sim, AFX_FCC_SIM);

    AfxUriMapConstData(&uriMap, "window", 0);
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
    dinSpec.prefetch = (void*)_AfxRendererDoDraw;
    dinSpec.udd = renderer;
    dinSpec.cmdPoolMemStock = 4096;
    dinSpec.estimatedSubmissionCnt = 3;
    dinSpec.enabledPresentationThreads = (afxNat[]) { 1, 0, 0, 0 };
    dinSpec.enabledStreamingThreads = (afxNat[]) { 1, 1, 1, 1 };
    dinSpec.enabledSubmissionThreads = (afxNat[]) { 1, 1, 1, 1 };
    din = AfxDrawContextAcquireInput(dctx, &dinSpec);
    AfxAssert(din);

    afxString str;
    AfxStringMapConst(&str, "viewer", 0);
    cam = AfxSimulationAcquireCamera(sim, &str, NIL, TRUE);
    AfxAssert(cam);
    //cam->farClip = -100000.0;
    controledNod = &cam->nod;
    //AfxAttachViewpoint(vpnt, cam);
    
    _AfxSetUpRenderer(&TheRenderer, sim);
    AfxDrawInputAffinePrefetchThreads(din, 0, 1, (afxNat[]) { 1 });
    afxResult rslt;
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

        dsys = AfxSystemAcquireDrawSystem(sys);
        AfxAssertObject(dsys, AFX_FCC_DSYS);

        afxDrawContextSpecification dctxSpec = { 0 };
        dctxSpec.driverId = 0;
        dctxSpec.queueCnt = 1;
        dctxSpec.autonomousQueue = FALSE;

        dctx = AfxDrawSystemAcquireContext(dsys, &dctxSpec);
        AfxAssert(dctx);

        afxApplicationSpecification appSpec;
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
