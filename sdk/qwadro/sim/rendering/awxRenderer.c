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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_SIM_C
#define _AFX_SIMULATION_C
#define _AFX_MESH_C
#define _AFX_MODEL_C
#define _AFX_MATERIAL_C
#define _AFX_VERTEX_BUFFER_C
#define _AFX_VERTEX_DATA_C
#define _AFX_MESH_TOPOLOGY_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/sim/rendering/awxRenderer.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/math/afxQuaternion.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/math/afxOpticalMatrix.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/sim/modeling/afxMeshTopology.h"
#include "qwadro/sim/modeling/awxVertexData.h"

_AKX afxError AwxCmdDrawBodies(afxDrawStream diob, awxRenderer rnd, afxReal dt, afxNat cnt, awxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);
    AfxAssertObjects(cnt, bodies, afxFcc_BOD);

    for (afxNat bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        awxBody bod = bodies[bodIdx];
        AfxAssertObjects(1, &bod, afxFcc_BOD);

        afxModel mdl = AwxGetBodyModel(bod);
        AfxAssertObjects(1, &mdl, afxFcc_MDL);
        afxSkeleton skl = AfxGetModelSkeleton(mdl);
        AfxTryAssertObjects(1, &skl, afxFcc_SKL);

        afxM4d m, m2;
        //AfxComputeModelDisplacement(bod->mdl, m);
        //afxReal64 ct, dt;
        //AfxGetThreadTime(&ct, &dt);
        //AwxUpdateBodyMatrix(bod, dt, FALSE, bod->placement, bod->placement);
        AwxSampleBodyAnimationsAcceleratedLOD(bod, bod->cachedBoneCnt, bod->placement, rnd->lp, rnd->wp, 0.0);
        //AwxSampleBodyAnimationsLODSparse(bod, 0, bod->cachedBoneCnt, rnd->lp, 0.0, NIL);
        //AfxComputePoseBuffer(skl, 0, bod->cachedBoneCnt, rnd->lp, m, rnd->wp);
        //AfxComputeRestPoseBuffer(skl, 0, bod->cachedBoneCnt, m, rnd->wp);


        for (afxNat mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
        {
            
            afxMesh msh = mdl->rigs[mshIdx].msh;

            if (msh)
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);

                //AfxCmdBindPipeline(diob, 0, rnd->lighting);
                //AfxCmdBindPipeline(diob, 0, rnd->tutCamUtil);
                AfxCmdBindPipeline(diob, 0, rnd->blinnTestPip);
                //AfxCmdBindPipeline(diob, 0, rnd->rigidBodyPip);                
                //AfxCmdBindPipeline(diob, 0, rnd->testPip);

                afxNat baseVtxIdx = 0, vtxCnt = 0;
                awxVertexData vtd = AfxGetMeshVertices(msh);

                vtxCnt = AwxCountVertices(vtd);

                AwxBufferizeVertexData(rnd->din, vtd);
                AwxCmdBindVertexDataCache(diob, 0, vtd);

                afxMeshTopology msht = AfxGetMeshTopology(msh);

                AfxBufferizeMeshTopology(msht);
                AfxCmdBindIndexSource(diob, msht->cache.buf, msht->cache.base, msht->cache.range, msht->cache.stride);
                
                //AfxCmdSetPrimitiveTopology(diob, afxPrimTopology_TRI_LIST);
                //AfxCmdSetCullMode(diob, NIL);
                //AfxCmdSwitchFrontFace(diob, AfxRandom2(0, 1));


                //AfxCmdEnableDepthTest(diob, TRUE);
                //AfxCmdEnableDepthWrite(diob, TRUE);

                afxNat zeros[] = { 0, 0, 0 };
                AfxCmdBindBuffers(diob, 3, 0, 1, &rnd->framesets[rnd->frameIdx].objConstantsBuffer, zeros, zeros);

                afxNat const *ToBoneIndices = AfxGetRiggedMeshBiasToJointMapping(mdl, mshIdx);
                AfxBuildIndexedCompositeBuffer(bod->cachedSkl, rnd->wp, ToBoneIndices, 1, &m);
                
                AfxCmdUpdateBuffer(diob, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 0, sizeof(m), m);
                AfxCmdUpdateBuffer(diob, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 64, sizeof(m), m);
                AfxCmdUpdateBuffer(diob, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

                afxNat surfCnt = AfxCountMeshSurfaces(msht);

                for (afxNat j = 0; j < surfCnt; j++)
                {
                    afxMeshSurface *sec = AfxGetMeshSurface(msht, j);
                    //AfxAssert(msh->topology->primType == afxPrimTopology_TRI_LIST);

                    afxMaterialConstants mat;

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
                        afxMaterial mtl = msh->mtlSlots[sec->mtlIdx].mtl;

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

                    afxNat idxCnt = (sec->triCnt * 3);
                    afxNat firstIdx = (sec->baseTriIdx * 3);
                    AfxCmdDrawIndexed(diob, baseVtxIdx, 0, 1, firstIdx, idxCnt);
                    //AfxCmdDraw(diob, 0, 1, 0, vtxCnt);
                }
                //AfxCmdDrawIndexed(diob, 0, 0, 1, 0, msht->triCnt * 3);

                //AfxCmdDrawIndexed(diob, 0, 0, 1, 0, msht->triCnt * 3);
            }
        }
    }
    return err;
}

_AKX afxError AwxCmdDrawTestIndexed(afxDrawStream diob, awxRenderer rnd)
{
    AfxCmdBindPipeline(diob, 0, rnd->testPip);

    AfxCmdBindVertexSources(diob, 0, 1, (afxBuffer[]) { rnd->testVbo }, NIL, NIL, (afxNat32[]) { sizeof(afxV3d) });
    //AfxCmdResetVertexStreams(diob, 1, NIL, (afxNat32[]) { sizeof(afxV3d) }, NIL);
    //AfxCmdResetVertexAttributes(diob, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
    AfxCmdBindIndexSource(diob, rnd->testIbo, 0, sizeof(afxNat32) * 6, sizeof(afxNat32));
    AfxCmdDrawIndexed(diob, 0, 0, 1, 0, 6);
    //AfxCmdDraw(diob, 6, 1, 0, 0);
    return 0;
}

_AKX afxError AfxRendererSetStar(awxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    awxViewConstants *viewConstants = &rnd->framesets[rnd->frameIdx].viewConstants;

    AfxCopyV4d(viewConstants->starPos, pos);
    AfxCopyV4d(viewConstants->starDir, dir);
    AfxCopyV4d(viewConstants->starKd, Kd);

    return err;
}

_AKX afxError AwxCmdEndSceneRendering(afxDrawStream diob, awxRenderer rnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    AfxCmdFinishSynthesis(diob);

    return err;
}

_AKX afxError AwxCmdBeginSceneRendering(afxDrawStream diob, awxRenderer rnd, afxCamera cam, afxRect const* drawArea, afxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    afxNat frameIdx = (rnd->frameIdx + 1) % rnd->frameCnt;
    rnd->frameIdx = frameIdx;

    //afxCanvas canv;
    //AfxEnumerateDrawOutputCanvases(rnd->dout, outBufIdx, &canv);
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
        AfxAcquireTextureBlueprint(&depthSurfB, whd, afxPixelFormat_D24, afxRasterUsage_DRAW);
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
    AfxCmdBeginSynthesis(diob, &dps);

    //afxWhd extent;
    //AfxGetCanvasExtent(canv, extent);
    afxViewport vp = { 0 };
    vp.extent[0] = rnd->drawArea.extent[0];
    vp.extent[1] = rnd->drawArea.extent[1];
    vp.depth[0] = (afxReal)0;
    vp.depth[1] = (afxReal)1;
    AfxCmdResetViewports(diob, 1, &vp);

    awxViewConstants *viewConstants = &rnd->framesets[frameIdx].viewConstants;

    if ((rnd->activeCam = cam))
    {
        //cam = rnd->activeCamera;

        afxV2d extent = { rnd->drawArea.extent[0], rnd->drawArea.extent[1] };
        AfxSetCameraAspectRatios(cam, AfxDeterminePhysicalAspectRatio(rnd->drawArea.extent[0], rnd->drawArea.extent[1]), extent, extent);

        viewConstants->viewExtent[0] = rnd->drawArea.extent[0];
        viewConstants->viewExtent[1] = rnd->drawArea.extent[1];

        afxV4d viewPos;
        AfxGetCameraPosition(cam, viewPos);
        AfxCopyAtv4d(viewConstants->viewPos, viewPos);

        afxM4d v, iv, p, ip;
        AfxRecomputeCameraMatrices(cam);
        AfxGetCameraMatrices(cam, v, iv);
        AfxGetCameraProjectiveMatrices(cam, p, ip);

        AfxCopyM4d(viewConstants->p, p);
        AfxCopyM4d(viewConstants->ip, ip);
        AfxPickAtm4d(v, viewConstants->v);
        AfxCopyM4d(viewConstants->iv, iv);
        AfxMultiplyM4d(viewConstants->pv, p, v);
        AfxInvertM4d(viewConstants->pv, viewConstants->ipv);

    }

    AfxUpdateBuffer(rnd->framesets[frameIdx].viewConstantsBuffer, 0, sizeof(*viewConstants), viewConstants);
    afxNat zeros[] = { 0 };
    AfxCmdBindBuffers(diob, 0, 0, 1, &rnd->framesets[frameIdx].viewConstantsBuffer, zeros, zeros);
    return err;
}

_AKX afxError _AfxRndDtor(awxRenderer rnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

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

    afxDrawInput din = rnd->din;

    AfxReleaseObjects(1, (void*[]) { din });
#if 0
    for (afxNat i = 0; i < rnd->frameCnt; i++)
    {
        if (rnd->canv[i])
            AfxReleaseObject(&rnd->canv[i]->obj);
    }
#endif
    return err;
}

_AKX afxError _AfxRndCtor(awxRenderer rnd, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    awxRendererConfig const* config = ((awxRendererConfig const*)cookie->udd[0]) + cookie->no;

    afxSimulation sim = AfxGetObjectProvider(rnd);
    rnd->cachedSim = sim;
    afxDrawContext dctx = sim->dctx;
    rnd->cachedDctx = dctx;
    
    //rnd->dout = NIL;

    rnd->frameCnt = 2;
    rnd->frameIdx = 0;

    //AfxZero2(rnd->canv, sizeof(rnd->canv));
    AfxZero2(1, sizeof(rnd->framesets), rnd->framesets);

    // acquire and set up our dedicated draw input device.
    {
        afxDrawInputConfig dinConfig = { 0 };
        dinConfig.proc = config->dinProc;
        dinConfig.udd = rnd;
        dinConfig.cmdPoolMemStock = 4096;
        dinConfig.estimatedSubmissionCnt = 3;

        AfxAcquireDrawInput(0, &dinConfig, &rnd->din);
        AfxAssert(rnd->din);
        AfxReconnectDrawInput(rnd->din, dctx);
    }

    afxUri uri;
    afxVertexInput vin;

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
        vboSpec.access = afxBufferAccess_W;
        vboSpec.usage = afxBufferUsage_VERTEX;
        AfxAcquireBuffers(dctx, 1, &vboSpec, &rnd->testVbo);
        
        afxBufferSpecification iboSpec = { 0 };
        iboSpec.siz = sizeof(testIndices);
        iboSpec.src = testIndices;
        iboSpec.access = afxBufferAccess_W;
        iboSpec.usage = afxBufferUsage_INDEX;
        AfxAcquireBuffers(dctx, 1, &iboSpec, &rnd->testIbo);
        
        afxVertexInputStream const vinStreams[] =
        {
            {
                .instanceRate = 0,
                .slotIdx = 0
            }
        };
        afxVertexInputAttr const vinAttrs[] =
        {
            {
                .location = 0,
                .streamIdx = 0,
                .offset = 0,
                .fmt = afxVertexFormat_V3D
            }
        };

        vin = AfxAcquireVertexInput(dctx, AFX_COUNTOF(vinStreams), vinStreams, AFX_COUNTOF(vinAttrs), vinAttrs);
        AfxAssertObjects(1, &vin, afxFcc_VIN);

        AfxMakeUri(&uri, "data/pipeline/test/test.xsh.xml", 0);
        rnd->testPip = AfxAssemblePipelineFromXsh(dctx, vin, &uri);
    }

    // acquire needed resources and set up our sky.
    {
        rnd->skyEnabled = 1;

        if (rnd->skyEnabled)
        {
            AfxBuildSkybox(&rnd->sky, sim);
        }
    }

    afxVertexInputStream const vinStreams[] =
    {
        {
            .instanceRate = 0,
            .slotIdx = 0
        },
        {
            .instanceRate = 0,
            .slotIdx = 1
        }
    };
    afxVertexInputAttr const vinAttrs[] =
    {
        {
            .location = 0,
            .streamIdx = 0,
            .offset = 0,
            .fmt = afxVertexFormat_V3D
        },
        {
            .location = 1,
            .streamIdx = 0,
            .offset = 12,
            .fmt = afxVertexFormat_V4B
        },
        {
            .location = 2,
            .streamIdx = 0,
            .offset = 16,
            .fmt = afxVertexFormat_V4B
        },
        {
            .location = 3,
            .streamIdx = 1,
            .offset = 0,
            .fmt = afxVertexFormat_V3D
        },
        {
            .location = 6,
            .streamIdx = 1,
            .offset = 12,
            .fmt = afxVertexFormat_V2D
        }
    };

    afxVertexInput skinnedVin = AfxAcquireVertexInput(dctx, AFX_COUNTOF(vinStreams), vinStreams, AFX_COUNTOF(vinAttrs), vinAttrs);
    AfxAssertObjects(1, &skinnedVin, afxFcc_VIN);
    afxVertexInputStream const vinStreams2[] =
    {
        {
            .instanceRate = 0,
            .slotIdx = 0
        },
        {
            .instanceRate = 0,
            .slotIdx = 1
        }
    };
    afxVertexInputAttr const vinAttrs2[] =
    {
        {
            .location = 0,
            .streamIdx = 0,
            .offset = 0,
            .fmt = afxVertexFormat_V3D
        },
        {
            .location = 3,
            .streamIdx = 1,
            .offset = 0,
            .fmt = afxVertexFormat_V3D
        },
        {
            .location = 6,
            .streamIdx = 1,
            .offset = 12,
            .fmt = afxVertexFormat_V2D
        }
    };

    afxVertexInput rigidVin = AfxAcquireVertexInput(dctx, AFX_COUNTOF(vinStreams2), vinStreams2, AFX_COUNTOF(vinAttrs2), vinAttrs2);
    AfxAssertObjects(1, &rigidVin, afxFcc_VIN);


    {
        afxBufferSpecification bufSpec[] =
        {
            { .siz = sizeof(awxViewConstants), .access = afxBufferAccess_W, .usage = afxBufferUsage_UNIFORM, .src = NIL },
            { .siz = sizeof(awxShaderConstants), .access = afxBufferAccess_W, .usage = afxBufferUsage_UNIFORM, .src = NIL },
            { .siz = sizeof(afxMaterialConstants), .access = afxBufferAccess_W, .usage = afxBufferUsage_UNIFORM, .src = NIL },
            { .siz = sizeof(awxInstanceConstants), .access = afxBufferAccess_W, .usage = afxBufferUsage_UNIFORM, .src = NIL }
        };
#if 0
        afxRasterInfo texiDepthSurfB = { 0 };
        texiDepthSurfB.e
        afxTextureBlueprint depthSurfB;
        AfxAcquireTextureBlueprint(&depthSurfB, (afxWhd) {1024, 1024, 1 }, afxPixelFormat_D24, afxRasterUsage_DRAW);
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
        AfxMakeUri(&uri, "data/pipeline/body/body.xsh.xml?rigid", 0);
        rnd->rigidBodyPip = AfxAssemblePipelineFromXsh(dctx, rigidVin, &uri);
        AfxMakeUri(&uri, "data/pipeline/body/body.xsh.xml?skinned", 0);
        rnd->skinnedBodyPip = AfxAssemblePipelineFromXsh(dctx, skinnedVin, &uri);
    }

    AfxRendererSetStar(rnd, AfxSpawnV4d(0, 0, 0, 1), AfxSpawnV3d(-0.8660f, 0.5f, 0), AfxSpawnV4d(0.8, 0.8, 0.8, 0.8));
    // ambient 0.2, 0.2, 0.2, 0.2

    AfxAcquirePoses(sim, 1, (afxNat[]) { 255 }, &rnd->lp);
    AfxAcquirePoseBuffers(sim, 1, (afxNat[]) { 255 }, (afxBool[]) {TRUE}, &rnd->wp);

    AfxMakeUri(&uri, "data/pipeline/testLighting/testLighting.xsh.xml?blinn", 0);
    rnd->blinnTestPip = AfxAssemblePipelineFromXsh(dctx, rigidVin, &uri);

    AfxMakeUri(&uri, "data/pipeline/tutCamUtil/tutCamUtil.xsh.xml?tutCamUtil", 0);
    rnd->tutCamUtil = AfxAssemblePipelineFromXsh(dctx, rigidVin, &uri);

    AfxMakeUri(&uri, "data/pipeline/lighting/lighting.xsh.xml?lighting", 0);
    rnd->lighting = AfxAssemblePipelineFromXsh(dctx, rigidVin, &uri);

    return err;
}

_AKX afxClassConfig _AfxRndMgrCfg =
{
    .fcc = afxFcc_RND,
    .name = "Renderer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(awxRenderer)),
    .mmu = NIL,
    .ctor = (void*)_AfxRndCtor,
    .dtor = (void*)_AfxRndDtor
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AKX afxError AwxAcquireRenderers(afxSimulation sim, afxNat cnt, awxRenderer rnd[], awxRendererConfig const config[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (AfxAcquireObjects(AfxGetRendererClass(sim), cnt, (afxObject*)rnd, (void const*[]) { (void*)config }))
        AfxThrowError();

    return err;
}
