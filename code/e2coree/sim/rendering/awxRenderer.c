/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_SIM_C
#define _AFX_SIMULATION_C
#define _AFX_MESH_C
#define _AFX_MODEL_C
#define _AFX_MATERIAL_C
#define _AFX_VERTEX_BUFFER_C
#define _AFX_VERTEX_DATA_C
#define _AFX_MESH_TOPOLOGY_C
#include "qwadro/afxQwadro.h"
#include "qwadro/sim/rendering/awxRenderer.h"
#include "qwadro/draw/pipe/afxDrawScript.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxOpticalMatrix.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/sim/modeling/awxMeshTopology.h"
#include "qwadro/sim/modeling/awxVertexData.h"

_AFX afxError AwxCmdDrawBodies(afxDrawScript dscr, awxRenderer rnd, afxNat cnt, awxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);
    AfxAssertObjects(cnt, bodies, afxFcc_BOD);

    for (afxNat bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        awxBody bod = bodies[bodIdx];
        AfxAssertObjects(1, &bod, afxFcc_BOD);

        awxModel mdl = AwxGetBodyModel(bod);
        AfxAssertObjects(1, &mdl, afxFcc_MDL);
        awxSkeleton skl = AwxGetModelSkeleton(mdl);
        AfxTryAssertObjects(1, &skl, afxFcc_SKL);

        for (afxNat mshIdx = 0; mshIdx < mdl->slotCnt; mshIdx++)
        {
            awxMesh msh = mdl->slots[mshIdx].msh;

            if (msh)
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);

                afxNat baseVtxIdx = 0, vtxCnt = 0;
                awxVertexData vtd = AwxGetMeshVertices(msh);

                AwxBufferizeVertexData(vtd);
                AwxBindVertexData(vtd, dscr);

                awxMeshTopology msht;
                AwxGetMeshTopology(msh, &msht);

                AfxBufferizeMeshTopology(msht);
                AfxCmdBindIndexSource(dscr, msht->cache.buf, msht->cache.base, msht->cache.range, msht->cache.stride);
                
                AfxCmdBindPipeline(dscr, 0, rnd->lighting);
                //AfxCmdBindPipeline(dscr, 0, rnd->tutCamUtil);
                //AfxCmdBindPipeline(dscr, 0, rnd->blinnTestPip);
                //AfxCmdBindPipeline(dscr, 0, rnd->rigidBodyPip);                
                //AfxCmdBindPipeline(dscr, 0, rnd->testPip);

                //AfxCmdSetPrimitiveTopology(dscr, afxPrimTopology_TRI_LIST);
                //AfxCmdSetCullMode(dscr, afxCullMode_BACK);
                //AfxCmdSwitchFrontFace(dscr, FALSE);

                //AfxCmdEnableDepthTest(dscr, TRUE);
                //AfxCmdEnableDepthWrite(dscr, TRUE);

                afxNat surfCnt = AwxCountMeshSurfaces(msht);

                for (afxNat j = 0; j < surfCnt; j++)
                {
                    awxMeshSurface *sec = AwxGetMeshSurface(msht, j);
                    //AfxAssert(msh->topology->primType == afxPrimTopology_TRI_LIST);

                    awxMaterialConstants mat;

                    if (sec->mtlIdx == AFX_INVALID_INDEX)
                    {
                        AfxSetColor(mat.Kd, 0.3f, 0.3f, 0.3f, 1.0f);
                        mat.hasDiffTex = FALSE;
                        //AfxUpdateBuffer(rnd->framesets[rnd->frameIdx].mtlConstantsBuffer, 0, sizeof(mat), &mat);
                    }
                    else
                    {
#if 0
                        AfxAssert(sec->mtlIdx < msh->mtlSlotCnt);
                        awxMaterial mtl = msh->mtlSlots[sec->mtlIdx].mtl;

                        if (mtl)
                        {
                            AfxAssertObjects(1, &mtl, afxFcc_MTL);
                            afxRaster tex = mtl->tex;

                            if (tex)
                            {
                                AfxAssertObjects(1, &tex, afxFcc_RAS);
                                AfxSetColor(mat.Kd, 0.3f, 0.3f, 0.3f, 1.0f);
                                mat.hasDiffTex = TRUE;
                            }
                            else
                            {
                                AfxSetColor(mat.Kd, 0.3f, 0.3f, 0.3f, 1.0f);
                                mat.hasDiffTex = FALSE;
                            }

                            //AfxUpdateBuffer(rnd->framesets[rnd->frameIdx].mtlConstantsBuffer, 0, sizeof(mat), &mat);
                        }
#endif
                    }


                    afxM4d CompositeMatrix;
                    afxNat const *ToBoneIndices = AwxGetLinkedMeshBoneMap(mdl, mshIdx);
                    //AfxBuildIndexedCompositeBuffer(skl, rnd->wp, ToBoneIndices, 1, &CompositeMatrix);
                    
                    afxM4d m, m2;
                    //AwxUpdateBodyModelMatrix(bod, 1, AFX_M4D_IDENTITY, m, FALSE);
                    
                    //afxM4d w;
                    //AfxInverseM4d(w, m);
                    //AfxRotationM4dFromEuler(w, AfxSpawnV3d(0, AfxRandomReal2(0, 360), 0));
                    AwxGetModelInitialPlacement(mdl, m);
                    AfxInverseM4d(m2, m);
                    //AfxComposeTransformWorldM4d(&mdl->init, m2, m);

                    AfxUpdateBuffer(rnd->framesets[rnd->frameIdx].objConstantsBuffer, offsetof(awxInstanceConstants, w), sizeof(CompositeMatrix), CompositeMatrix);
                    AfxUpdateBuffer(rnd->framesets[rnd->frameIdx].objConstantsBuffer, offsetof(awxInstanceConstants, m), sizeof(m), AFX_M4D_IDENTITY);
                    
                    
                    awxInstanceConstants *objConstants = &rnd->framesets[rnd->frameIdx].objConstants;
                    
                    AfxCopyAtm4(objConstants->m, AFX_M4D_IDENTITY);
                    AfxCopyAtm4(objConstants->w[0], AFX_M4D_IDENTITY);

                    //AfxUpdateBuffer(rnd->framesets[rnd->frameIdx].objConstantsBuffer, 0, sizeof(*objConstants), objConstants);
                    afxNat zeros[] = { 0, 0, 0 };
                    AfxCmdBindBuffers(dscr, 3, 0, 1, &rnd->framesets[rnd->frameIdx].objConstantsBuffer, zeros, zeros);

                    afxNat idxCnt = (sec->triCnt * 3);
                    afxNat firstIdx = (sec->baseTriIdx * 3);
                    AfxCmdDrawIndexed(dscr, idxCnt, 0, firstIdx, baseVtxIdx, 0);
                    //AfxCmdDraw(dscr, msh->vtxCnt, 1, msh->baseVtx, 0);
                }
                //AfxCmdDrawIndexed(dscr, msht->triCnt *3, 1, 0, 0, 0);
            }
        }
    }
    return err;
}

_AFX afxError AwxCmdDrawTestIndexed(afxDrawScript dscr, awxRenderer rnd)
{
    AfxCmdBindPipeline(dscr, 0, rnd->testPip);

    AfxCmdBindVertexSources(dscr, 0, 1, (afxBuffer[]) { rnd->testVbo }, NIL, NIL);
    //AfxCmdResetVertexStreams(dscr, 1, NIL, (afxNat32[]) { sizeof(afxV3d) }, NIL);
    //AfxCmdResetVertexAttributes(dscr, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
    AfxCmdBindIndexSource(dscr, rnd->testIbo, 0, sizeof(afxNat32) * 6, sizeof(afxNat32));
    AfxCmdDrawIndexed(dscr, 6, 1, 0, 0, 0);
    //AfxCmdDraw(dscr, 6, 1, 0, 0);
    return 0;
}

_AFX afxError AfxRendererSetStar(awxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    awxViewConstants *viewConstants = &rnd->framesets[rnd->frameIdx].viewConstants;

    AfxCopyV4d(viewConstants->starPos, pos);
    AfxCopyV4d(viewConstants->starDir, dir);
    AfxCopyV4d(viewConstants->starKd, Kd);

    return err;
}

_AFX afxError AwxCmdEndSceneRendering(afxDrawScript dscr, awxRenderer rnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    AfxCmdFinishSynthesis(dscr);

    return err;
}

_AFX afxError AwxCmdBeginSceneRendering(afxDrawScript dscr, awxRenderer rnd, afxCamera cam, afxRect const* drawArea, afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    afxNat frameIdx = (rnd->frameIdx + 1) % rnd->frameCnt;
    rnd->frameIdx = frameIdx;

    //afxCanvas canv;
    //AfxGetDrawOutputCanvas(rnd->dout, outBufIdx, &canv);
    //rnd->canv = canv;

    if (drawArea)
    {
        rnd->drawArea = *drawArea;
    }
    else if (canv)
    {
        afxWhd whd;
        AfxGetCanvasExtent(canv, whd);
        rnd->drawArea.extent[0] = whd[0];
        rnd->drawArea.extent[1] = whd[1];

    }
    else
    {
        rnd->drawArea = (afxRect) { 0 };
    }

#if 0
    afxWhd depthWhd;
    AfxGetRasterExtent(rnd->framesets[frameIdx].depthSurf, 0, depthWhd);

    if (depthWhd[0] != whd[0] || depthWhd[1] != whd[1])
    {
        afxTextureBlueprint depthSurfB;
        AfxAcquireTextureBlueprint(&depthSurfB, whd, afxPixelFormat_D24, afxRasterFlag_DRAW);
        AfxTextureBlueprintAddImage(&depthSurfB, afxPixelFormat_D24, whd, NIL, 0);

        for (afxNat i = 0; i < rnd->frameCnt; i++)
        {
            AfxReleaseObjects(1, (void*[]) { rnd->framesets[i].depthSurf });
            AfxBuildRasters(rnd->cachedDctx, 1, &rnd->framesets[i].depthSurf, &depthSurfB);
            AfxAssert(rnd->framesets[i].depthSurf != surf);
        }

        AfxTextureBlueprintEnd(&depthSurfB, 0, NIL);
    }
#endif
            
    afxDrawTarget rdt = { 0 };
    //rdt.tex = surf;
    rdt.clearValue.color[0] = 0.3f;
    rdt.clearValue.color[1] = 0.1f;
    rdt.clearValue.color[2] = 0.3f;
    rdt.clearValue.color[3] = 1;
    rdt.loadOp = afxSurfaceLoadOp_CLEAR;
    rdt.storeOp = afxSurfaceStoreOp_STORE;

    afxDrawTarget ddt = { 0 };
    //ddt.tex = rnd->framesets[frameIdx].depthSurf;
    ddt.clearValue.depth = 1.0;
    ddt.clearValue.stencil = 0;
    ddt.loadOp = afxSurfaceLoadOp_CLEAR;
    ddt.storeOp = afxSurfaceStoreOp_STORE;

    //afxCanvas canv = rnd->canv;
    afxSynthesisConfig dps = { 0 };
    dps.canv = canv;
    dps.area = rnd->drawArea;
    dps.layerCnt = 1;
    dps.rasterCnt = 1;
    dps.rasters = &rdt;
    dps.depth = &ddt;
    dps.stencil = NIL;
    AfxCmdBeginSynthesis(dscr, &dps);

    //afxWhd extent;
    //AfxGetCanvasExtent(canv, extent);
    afxViewport vp = { 0 };
    vp.extent[0] = rnd->drawArea.extent[0];
    vp.extent[1] = rnd->drawArea.extent[1];
    vp.depth[0] = (afxReal)0;
    vp.depth[1] = (afxReal)1;
    AfxCmdResetViewports(dscr, 1, &vp);

    awxViewConstants *viewConstants = &rnd->framesets[frameIdx].viewConstants;

    if ((rnd->activeCam = cam))
    {
        //cam = rnd->activeCamera;

        afxV2d extent = { rnd->drawArea.extent[0], rnd->drawArea.extent[1] };
        AfxSetCameraAspectRatios(cam, AfxTryFindPhysicalAspectRatio(rnd->drawArea.extent[0], rnd->drawArea.extent[1]), extent, extent);

        viewConstants->viewExtent[0] = rnd->drawArea.extent[0];
        viewConstants->viewExtent[1] = rnd->drawArea.extent[1];

        afxV4d viewPos;
        AfxGetCameraPosition(cam, viewPos);
        viewPos[3] = 1.f;
        AfxCopyV4d(viewConstants->viewPos, viewPos);

        afxM4d v, iv, p, ip;
        AfxRecomputeCameraMatrices(cam);
        AfxGetCameraViewMatrices(cam, v, iv);
        AfxGetCameraProjectionMatrices(cam, p, ip);

        //AfxSetM4dPosition(viewpoint, m);
        //AfxComputeRenderWareViewM4d(v, v);
        //AfxSetM2dIdentity(v);
        //AfxTransposeM4d(m, m);
        //AfxTransposeM4d(m2, m2);

        //AfxComputeRenderWareProjectionM4d(p, AFX_V2D_ZERO, extent, 0.1, 1000.0, 1);
        //AfxComputePerspectiveMatrix_LH(m, extent, 0.1, 1000.0);
        //AfxTransposeM4d(m2, m);
        //AfxTransposeM4d(m2, m2);

        //AfxComputeRenderWareViewM4d(v, iv);
        AfxCopyAtm4(viewConstants->v, v);
        AfxMakeAffineM4d(viewConstants->v);
        //AfxCopyM4d(viewConstants->iv, iv);
        //AfxComputeRenderWareProjectionM4d(p, &vp, TRUE);
        //AfxComputeRenderWareProjectionM4d(p, &vp, TRUE);
        //AfxTransposeM4d(viewConstants->p, p);
        AfxCopyM4d(viewConstants->p, p);
        //AfxTransposeM4d(viewConstants->p, p);
        //AfxCopyM4d(viewConstants->ip, ip);
    }

    AfxUpdateBuffer(rnd->framesets[frameIdx].viewConstantsBuffer, 0, sizeof(*viewConstants), viewConstants);
    afxNat zeros[] = { 0 };
    AfxCmdBindBuffers(dscr, 0, 0, 1, &rnd->framesets[frameIdx].viewConstantsBuffer, zeros, zeros);
    return err;
}

_AFX afxError _AfxRndDtor(awxRenderer rnd)
{
    AfxEntry("rnd=%p", rnd);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    afxDrawInput din = rnd->din;

    AfxReleaseObjects(1, (void*[]) { din });
#if 0
    for (afxNat i = 0; i < rnd->frameCnt; i++)
    {
        if (rnd->canv[i])
            AfxReleaseObject(&rnd->canv[i]->obj);
    }
#endif
    for (afxNat i = 0; i < rnd->frameCnt; i++)
    {
        AfxReleaseObjects(1, (void*[]) { rnd->framesets[i].viewConstantsBuffer });
        AfxReleaseObjects(1, (void*[]) { rnd->framesets[i].shdConstantsBuffer });
        AfxReleaseObjects(1, (void*[]) { rnd->framesets[i].mtlConstantsBuffer });
        AfxReleaseObjects(1, (void*[]) { rnd->framesets[i].objConstantsBuffer });

        //AfxReleaseObjects(1, (void*[]) { rnd->framesets[i].depthSurf });
    }

    AfxReleaseObjects(1, (void*[]) { rnd->sky.cubemap });
    AfxReleaseObjects(1, (void*[]) { rnd->sky.cube });
    AfxReleaseObjects(1, (void*[]) { rnd->sky.skyPip });
    AfxReleaseObjects(1, (void*[]) { rnd->sky.smp });

    AfxReleaseObjects(1, (void*[]) { rnd->rigidBodyPip });
    AfxReleaseObjects(1, (void*[]) { rnd->skinnedBodyPip });

    return err;
}

_AFX afxError _AfxRndCtor(awxRenderer rnd, afxCookie const *cookie)
{
    AfxEntry("rnd=%p", rnd);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    awxRendererConfig const* config = ((awxRendererConfig const*)cookie->udd[0]) + cookie->no;

    awxSimulation sim = AfxGetObjectProvider(&rnd->obj);
    rnd->cachedSim = sim;
    afxDrawContext dctx = sim->dctx;
    rnd->cachedDctx = dctx;
    
    //rnd->dout = NIL;

    rnd->frameCnt = 2;
    rnd->frameIdx = 0;

    //AfxZero(rnd->canv, sizeof(rnd->canv));
    AfxZero(1, sizeof(rnd->framesets), rnd->framesets);

    // acquire and set up our dedicated draw input device.
    {
        afxDrawInputConfig dinConfig = { 0 };
        dinConfig.prefetch = (void*)config->dinProc;
        dinConfig.udd = rnd;
        dinConfig.cmdPoolMemStock = 4096;
        dinConfig.estimatedSubmissionCnt = 3;

        AfxOpenDrawInputs(0, 1, &dinConfig, &rnd->din);
        AfxAssert(rnd->din);
        AfxReconnectDrawInput(rnd->din, dctx);
    }

    afxUri uri;

    //afxUri name;

    {
        afxV3d testVertices[] =
        {
            { 0.5f,  0.5f, 0.0f },  // top right
            { 0.5f, -0.5f, 0.0f },  // bottom right
            {-0.5f, -0.5f, 0.0f },  // bottom left
            {-0.5f,  0.5f, 0.0f }  // top left 
        };
        afxNat32 testIndices[] =
        // note that we start from 0!
        {  
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        afxBufferSpecification vboSpec = { 0 };
        vboSpec.siz = sizeof(testVertices);
        vboSpec.src = testVertices;
        vboSpec.access = NIL;
        vboSpec.usage = afxBufferUsage_VERTEX;
        AfxAcquireBuffers(dctx, 1, &vboSpec, &rnd->testVbo);
        
        afxBufferSpecification iboSpec = { 0 };
        iboSpec.siz = sizeof(testIndices);
        iboSpec.src = testIndices;
        iboSpec.access = NIL;
        iboSpec.usage = afxBufferUsage_INDEX;
        AfxAcquireBuffers(dctx, 1, &iboSpec, &rnd->testIbo);
        
        AfxMakeUri(&uri, "data/pipeline/test.xsh.xml", 0);
        rnd->testPip = AfxLoadPipelineFromXsh(dctx, &uri);
    }

    // acquire needed resources and set up our sky.
    {
        rnd->skyEnabled = 1;

        if (rnd->skyEnabled)
        {
            AfxBuildSkybox(&rnd->sky, sim);
        }
    }

    {
        afxBufferSpecification bufSpec[] =
        {
            { .siz = sizeof(awxViewConstants), .access = afxBufferAccess_W, .usage = afxBufferUsage_UNIFORM, .src = NIL },
            { .siz = sizeof(awxShaderConstants), .access = afxBufferAccess_W, .usage = afxBufferUsage_UNIFORM, .src = NIL },
            { .siz = sizeof(awxMaterialConstants), .access = afxBufferAccess_W, .usage = afxBufferUsage_UNIFORM, .src = NIL },
            { .siz = sizeof(awxInstanceConstants), .access = afxBufferAccess_W, .usage = afxBufferUsage_UNIFORM, .src = NIL }
        };
#if 0
        afxRasterInfo texiDepthSurfB = { 0 };
        texiDepthSurfB.e
        afxTextureBlueprint depthSurfB;
        AfxAcquireTextureBlueprint(&depthSurfB, (afxWhd) {1024, 1024, 1 }, afxPixelFormat_D24, afxRasterFlag_DRAW);
        AfxTextureBlueprintAddImage(&depthSurfB, afxPixelFormat_D24, (afxWhd) { 1, 1, 1 }, NIL, 0);
#endif

        for (afxNat i = 0; i < rnd->frameCnt; i++)
        {
            //AfxBuildRasters(dctx, 1, &rnd->framesets[i].depthSurf, &depthSurfB);

            AfxAcquireBuffers(dctx, 1, &bufSpec[0], &rnd->framesets[i].viewConstantsBuffer);
            AfxAcquireBuffers(dctx, 1, &bufSpec[1], &rnd->framesets[i].shdConstantsBuffer);
            AfxAcquireBuffers(dctx, 1, &bufSpec[2], &rnd->framesets[i].mtlConstantsBuffer);
            AfxAcquireBuffers(dctx, 1, &bufSpec[3], &rnd->framesets[i].objConstantsBuffer);
        }

        //AfxTextureBlueprintEnd(&depthSurfB, 0, NIL);
    }

    {
        AfxMakeUri(&uri, "data/pipeline/body.xsh.xml?rigid", 0);
        rnd->rigidBodyPip = AfxLoadPipelineFromXsh(dctx, &uri);
        AfxMakeUri(&uri, "data/pipeline/body.xsh.xml?skinned", 0);
        rnd->skinnedBodyPip = AfxLoadPipelineFromXsh(dctx, &uri);
    }

    AfxRendererSetStar(rnd, AfxSpawnV4d(0, 0, 0, 1), AfxSpawnV3d(-0.8660f, 0.5f, 0), AfxSpawnV4d(0.8, 0.8, 0.8, 0.8));
    // ambient 0.2, 0.2, 0.2, 0.2

    AfxAcquirePoses(sim, 1, (afxNat[]) { 255 }, &rnd->lp);
    AfxAcquireWorldPoses(sim, 1, (afxNat[]) { 255 }, (afxBool[]) {FALSE}, &rnd->wp);

    AfxMakeUri(&uri, "data/pipeline/testLighting.xsh.xml?blinn", 0);
    rnd->blinnTestPip = AfxLoadPipelineFromXsh(dctx, &uri);

    AfxMakeUri(&uri, "data/pipeline/tutCamUtil.xsh.xml?tutCamUtil", 0);
    rnd->tutCamUtil = AfxLoadPipelineFromXsh(dctx, &uri);

    AfxMakeUri(&uri, "data/pipeline/lighting.xsh.xml?lighting", 0);
    rnd->lighting = AfxLoadPipelineFromXsh(dctx, &uri);

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AwxAcquireRenderers(awxSimulation sim, afxNat cnt, awxRenderer rnd[], awxRendererConfig const config[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (AfxAcquireObjects(AfxGetRendererClass(sim), cnt, (afxObject*)rnd, (void const*[]) { (void*)config }))
        AfxThrowError();

    return err;
}

_AFX afxClassConfig _AfxRndClsConfig =
{
    .fcc = afxFcc_RND,
    .name = "Renderer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(awxRenderer)),
    .mmu = NIL,
    .ctor = (void*)_AfxRndCtor,
    .dtor = (void*)_AfxRndDtor
};
