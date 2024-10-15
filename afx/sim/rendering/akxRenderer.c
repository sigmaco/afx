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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_SIM_C
#define _AMX_SIMULATION_C
#define _AVX_MESH_C
#define _AMX_MODEL_C
#define _AMX_BODY_C
#define _AMX_MATERIAL_C
#define _AVX_VERTEX_BUFFER_C
#define _AVX_GEOMETRY_C
#define _AVX_MESH_TOPOLOGY_C
#include "../../dev/AmxImplKit.h"
#include "../../dev/AvxImplKit.h"

_AMX afxError AkxCmdBindVertexDataCache(avxCmdb cmdb, afxNat slotIdx, afxGeometry vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

    akxVertexCache* cache = &vtd->cache;
    
    AvxCmdBindVertexSources(cmdb, 0, 2, (afxBuffer[]) { cache->buf, cache->buf },
                                        (afxNat32[]) { cache->streams[0].base, cache->streams[1].base },
                                        (afxNat32[]) { cache->streams[0].range, cache->streams[1].range },
                                        (afxNat32[]) { cache->streams[0].stride, cache->streams[1].stride });
    return err;
}

_AMX afxError AkxBufferizeVertexData(afxDrawInput din, afxGeometry vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);

    if (vtd->cache.buf)
        return err;

    //afxSimulation sim = AfxGetProvider(vtd);
    //AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxString const positionals[] = { AfxString("pos"), AfxString("pvt"), AfxString("wgt") };

    afxNat cacheStride[2] = { 0 };
    afxNat cacheIdx[16];
    afxNat cachedAttrOffset[16];
    afxVertexFormat cachedAttrFmt[16];

    for (afxNat i = 0; i < vtd->attrCnt; i++)
    {
        avxGeometryAttr* attr = &vtd->attrs[i];
        afxVertexFormat fmt = attr->fmt;
        AfxAssert(fmt < afxVertexFormat_TOTAL);

        afxString str = attr->usage.str;

        if (attr->flags & afxVertexFlag_POSITIONAL)
        {
            if (0 == AfxCompareString(&str, &positionals[0]))
            {
                cachedAttrFmt[i] = afxVertexFormat_V3D;
            }
            else if (0 == AfxCompareString(&str, &positionals[1]))
            {
                cachedAttrFmt[i] = afxVertexFormat_V4B;
            }
            else if (0 == AfxCompareString(&str, &positionals[2]))
            {
                cachedAttrFmt[i] = afxVertexFormat_V4B;
            }
            else
            {
                // pointless
                AfxThrowError();
                cachedAttrFmt[i] = attr->fmt;
            }
            cacheIdx[i] = 0;
        }
        else
        {
            cacheIdx[i] = 1;
            cachedAttrFmt[i] = attr->fmt;
        }

        //attr->cacheIdx = cacheIdx[i];
        //attr->cachedOffset = cacheStride[cacheIdx[i]];
        cachedAttrOffset[i] = cacheStride[cacheIdx[i]];
        cacheStride[cacheIdx[i]] += AfxVertexFormatGetSize(cachedAttrFmt[i]);
        //attr->cachedFmt = cachedFmt[i];
    }

    akxVertexCache* cache = &vtd->cache;
    AfxPushLinkage(&cache->vbuf, NIL);
    cache->vin = NIL;

    afxBufferInfo vboSpec = { 0 };
    vboSpec.flags = afxBufferFlag_W;
    vboSpec.usage = afxBufferUsage_VERTEX;
    vboSpec.bufCap = 0;

    for (afxNat i = 0; i < 2; i++)
    {
        cache->streams[i].base = i ? vtd->cache.streams[i - 1].range : 0;
        cache->streams[i].range = AFX_ALIGNED_SIZEOF(cacheStride[i] * vtd->vtxCnt, 64);
        cache->streams[i].stride = cacheStride[i];

        vboSpec.bufCap += cache->streams[i].range;
    }

    afxBuffer buf;
    afxDrawContext dctx;
    AfxGetDrawInputContext(din, &dctx);
    AfxAcquireBuffers(/*din*/dctx, 1, &vboSpec, &buf);
    cache->buf = buf;

    for (afxNat srcIdx = 0; srcIdx < 2; srcIdx++)
    {
        AfxAssert(cacheStride[srcIdx]);
        AfxAssertRange(AfxGetBufferCapacity(cache->buf, 0), cache->streams[srcIdx].base, cache->streams[srcIdx].range);
        void* dst;

        if (AfxMapBuffer(cache->buf, cache->streams[srcIdx].base, cache->streams[srcIdx].range, NIL, &dst)) AfxThrowError();
        else
        {
            for (afxNat j = 0; j < vtd->attrCnt; j++)
            {
                if (cacheIdx[j] == srcIdx)
                {
                    avxGeometryAttr* attr = &vtd->attrs[j];
                    //AfxAssert(data);
                    afxVertexFormat fmt = attr->fmt;
                    AfxAssert(fmt < afxVertexFormat_TOTAL);

                    afxByte* data = AfxExposeGeometry(vtd, j, 0);
                    
                    if (data)
                    {
                        afxSize srcStride = AfxVertexFormatGetSize(fmt);
                        AfxAssert(srcStride);
                        AfxStream3(vtd->vtxCnt, data, 0, srcStride, dst, cachedAttrOffset[j], cache->streams[srcIdx].stride);
                    }
                }
            }
            AfxUnmapBuffer(cache->buf, TRUE);
        }
    }
    return err;
}

_AMX afxError AfxBufferizeMesh(afxDrawInput din, afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxVertexIndexCache* cache = &msh->cache;

    if (!cache->buf)
    {
        afxNat idxSiz = AfxDetermineMeshIndexSize(msh);
        afxNat idxCnt = AfxCountMeshIndices(msh, 0);

        afxBufferInfo spec;
        spec.bufCap = idxCnt * idxSiz;
        spec.flags = afxBufferFlag_W;
        spec.usage = afxBufferUsage_INDEX;

        afxDrawContext dctx;
        AfxGetDrawInputContext(din, &dctx);

        if (AfxAcquireBuffers(dctx, 1, &spec, &cache->buf)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &cache->buf, afxFcc_BUF);

            cache->base = 0;
            cache->range = spec.bufCap;
            cache->stride = cache->range / idxCnt;
            AfxAssert(cache->stride == idxSiz);
            //cache->idxSiz = idxSiz;
        }

        afxBuffer buf;

        if ((buf = cache->buf))
        {
            AfxAssertObjects(1, &buf, afxFcc_BUF);

            AfxAssert(cache->stride);
            AfxAssertRange(AfxGetBufferCapacity(buf, 0), cache->base, cache->range);
            //AfxUpdateBuffer2(cache->buf, cache->base, cache->range, cache->stride, msht->vtxIdx, sizeof(msht->vtxIdx[0]));

            afxBufferIo iop = { 0 };
            iop.dstOffset = cache->base;
            iop.dstStride = cache->stride;
            iop.srcStride = sizeof(msh->tris[0][0]);
            iop.rowCnt = msh->triCnt * 3;
            AfxUpdateBuffer(cache->buf, 1, &iop, msh->tris, 0);

#if _AFX_DEBUG
            void* p;
            AfxWaitForDrawBridge(dctx, 0, 0);
            AfxMapBuffer(cache->buf, cache->base, cache->range, NIL, &p);

            for (afxNat i = 0; i < msh->triCnt; i++)
            {
                if (idxSiz == 1)
                {
                    afxIndexedTriangle8 *tris = p;
                    AfxAssert(msh->tris[i][0] == tris[i][0]);
                    AfxAssert(msh->tris[i][1] == tris[i][1]);
                    AfxAssert(msh->tris[i][2] == tris[i][2]);
                }
                else if (idxSiz == 2)
                {
                    afxIndexedTriangle16 *tris = p;
                    AfxAssert(msh->tris[i][0] == tris[i][0]);
                    AfxAssert(msh->tris[i][1] == tris[i][1]);
                    AfxAssert(msh->tris[i][2] == tris[i][2]);

                    tris[i][0] = msh->tris[i][0];
                    tris[i][1] = msh->tris[i][1];
                    tris[i][2] = msh->tris[i][2];
                }
                else if (idxSiz == 4)
                {
                    afxIndexedTriangle *tris = p;
                    AfxAssert(msh->tris[i][0] == tris[i][0]);
                    AfxAssert(msh->tris[i][1] == tris[i][1]);
                    AfxAssert(msh->tris[i][2] == tris[i][2]);
                }

                if (err)
                {
                    int a = 1;
                }
            }

            AfxUnmapBuffer(cache->buf, FALSE);
#endif
        }
    }
    return err;
}

_AMX afxError AfxRequestVertexCache(afxDrawInput din, afxGeometry vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);



    return err;
};

_AMX afxBool _AkxCaptureBodCb(afxBody bod, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &bod, afxFcc_BOD);

    afxFrustum* frustum = udd[0];
    afxReal lodErr = *(afxReal*)udd[1];
    afxArray* pvs = udd[2];

    //AfxFrustumTestAabbs(&frustum, 1, &aabb);

    if (1) // do visibility culling
    {
        afxNat arrel;

        if (!AfxInsertArrayUnits(pvs, 1, &arrel, &bod)) AfxThrowError();
        else
        {

        }
    }

    return TRUE;
}

_AMX afxError AkxCaptureBodyPvs(afxSimulation sim, afxCamera cam, afxReal lodErr, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxFrustum frustum;
    AfxGetCameraFrustum(cam, &frustum);

    AfxInvokeBodies(sim, 0, AFX_N32_MAX, (void*)_AkxCaptureBodCb, (void*[]) { &frustum, &lodErr, pvs });

    return err;
}

_AMX afxError AkxDrawBodies(akxRenderer scn, avxCmdb cmdb, afxNat cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &scn, afxFcc_RND);

    for (afxNat bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        afxBody bod = bodies[bodIdx];
        AfxAssertObjects(1, &bod, afxFcc_BOD);
        afxModel mdl;

        if (AfxGetBodyModel(bod, &mdl))
        {
            AfxAssertObjects(1, &mdl, afxFcc_MDL);
            afxSkeleton skl = AfxGetModelSkeleton(mdl);
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            afxNat jntCnt = AfxCountSkeletonJoints(skl, 0);
            afxReal sampleLodErr = 0.0;
            afxNat sampledJntCnt = AfxCountSkeletonJoints(skl, sampleLodErr);

            afxM4d m, m2;
            //AfxComputeModelDisplacement(bod->mdl, m);
            //afxReal64 ct, dt;
            //AfxGetThreadTime(&ct, &dt);
            //AfxUpdateBodyMatrix(bod, dt, FALSE, bod->placement, bod->placement);
            //AfxSampleBodyAnimationsLODSparse(bod, 0, bod->cachedBoneCnt, scn->lp, 0.0, NIL);
            //AfxComputePoseBuffer(skl, 0, bod->cachedBoneCnt, scn->lp, m, scn->wp);
            //AfxComputeRestPoseBuffer(skl, 0, bod->cachedBoneCnt, m, scn->wp);

#if !0
            AfxSampleBodyAnimationsAcceleratedLOD(bod, jntCnt, bod->placement, scn->lp, scn->wp, 0.0);
#else
            afxM4d mdlDispl;
            AfxComputeModelDisplacement(mdl, mdlDispl);
            AfxSampleBodyAnimationsLODSparse(bod, 0, sampledJntCnt, bod->pose, sampleLodErr, NIL);
            AfxComputePoseBufferLod(skl, 0, jntCnt, 0, sampledJntCnt, bod->pose, mdlDispl, scn->wp);
#endif

            for (afxNat mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
            {
                afxMesh msh = mdl->rigs[mshIdx].msh;

                if (msh)
                {
                    AfxAssertObjects(1, &msh, afxFcc_MSH);

                    afxGeometry vtd;
                    afxNat baseVtx, vtxCnt;
                    AfxGetMeshVertices(msh, 0, &vtd, &baseVtx, &vtxCnt);
                    AfxAssertObjects(1, &vtd, afxFcc_VTD);

                    AkxBufferizeVertexData(scn->din, vtd);
                    AkxCmdBindVertexDataCache(cmdb, 0, vtd);

                    AvxCmdApplyDrawTechnique(cmdb, scn->lightingDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->blinnTestRazrDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->testDtec, 0, scn->testVin, NIL);

#if 0
#if 0
                    if (AfxMeshIsDeformable(msh))
                        AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 0, scn->rigidVin, NIL);
#else
                    if (AfxMeshIsDeformable(msh))
                        AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
#endif
#endif

                    AfxBufferizeMesh(scn->din, msh);
                    AvxCmdBindIndexSource(cmdb, msh->cache.buf, msh->cache.base, msh->cache.range, msh->cache.stride);

                    //AvxCmdSetPrimitiveTopology(cmdb, avxTopology_TRI_LIST);
                    //AvxCmdSetCullMode(cmdb, NIL);
                    //AvxCmdSwitchFrontFace(cmdb, AfxRandom2(0, 1));

                    //AvxCmdEnableDepthTest(cmdb, TRUE);
                    //AvxCmdEnableDepthWrite(cmdb, TRUE);




                    AvxCmdBindBuffers(cmdb, 3, 0, 1, &scn->framesets[scn->frameIdx].objConstantsBuffer, NIL, NIL);

                    afxNat const *ToBoneIndices = AfxGetRiggedMeshBiasToJointMapping(mdl, mshIdx);
                    AfxBuildIndexedCompositeBuffer(skl, scn->wp, ToBoneIndices, 1, &m);
                    AfxBuildIndexedCompositeBuffer(skl, scn->wp, ToBoneIndices, msh->biasCnt, scn->framesets[scn->frameIdx].objConstants.w);

                    //AfxM4dCopyAtm(m, m);
                    //AfxM4dReset(m);
                    AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 0, sizeof(m), m);
                    AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 64, msh->biasCnt * sizeof(m), scn->framesets[scn->frameIdx].objConstants.w);
                    //AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

                    afxNat surfCnt = AfxCountMeshSurfaces(msh, 0);

                    for (afxNat j = 0; j < surfCnt; j++)
                    {
                        afxMeshSurface *sec = AfxGetMeshSurface(msh, j);

                        if (sec->mtlIdx)
                        {
                            // Push textures
                        }

                        afxNat idxCnt = (sec->triCnt * 3);
                        afxNat firstIdx = (sec->baseTriIdx * 3);
                        AvxCmdDrawIndexed(cmdb, idxCnt, 1, firstIdx, baseVtx, 0);
                        //AvxCmdDraw(cmdb, vtxCnt, 1, 0, 0);
                    }
                }
            }
        }
    }
    return err;
}

_AMX afxError AkxBeginSceneCapture(akxRenderer scn, afxCamera cam, afxSimulation sim, avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &scn, afxFcc_RND);
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
#if !0
    AfxEmptyArray(&scn->capturedNodes);
    AkxCaptureBodyPvs(sim, cam, 0, &scn->capturedNodes);

    AkxDrawBodies(scn, cmdb, scn->capturedNodes.cnt, scn->capturedNodes.data);

    return err;
#endif
    afxBody bod;
    afxNat i = 0;
    while (AfxEnumerateBodies(sim, i++, 1, &bod))
    {
        AfxAssertObjects(1, &bod, afxFcc_BOD);
        afxModel mdl;

        if (AfxGetBodyModel(bod, &mdl))
        {
            AfxAssertObjects(1, &mdl, afxFcc_MDL);
            afxSkeleton skl = AfxGetModelSkeleton(mdl);
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            afxNat jntCnt = AfxCountSkeletonJoints(skl, 0);
            afxReal sampleLodErr = 0.0;
            afxNat sampledJntCnt = AfxCountSkeletonJoints(skl, sampleLodErr);

            afxM4d m, m2;
            //AfxComputeModelDisplacement(bod->mdl, m);
            //afxReal64 ct, dt;
            //AfxGetThreadTime(&ct, &dt);
            //AfxUpdateBodyMatrix(bod, dt, FALSE, bod->placement, bod->placement);
            //AfxSampleBodyAnimationsLODSparse(bod, 0, bod->cachedBoneCnt, scn->lp, 0.0, NIL);
            //AfxComputePoseBuffer(skl, 0, bod->cachedBoneCnt, scn->lp, m, scn->wp);
            //AfxComputeRestPoseBuffer(skl, 0, bod->cachedBoneCnt, m, scn->wp);

#if !0
            AfxSampleBodyAnimationsAcceleratedLOD(bod, jntCnt, bod->placement, scn->lp, scn->wp, 0.0);
#else
            afxM4d mdlDispl;
            AfxComputeModelDisplacement(mdl, mdlDispl);
            AfxSampleBodyAnimationsLODSparse(bod, 0, sampledJntCnt, bod->pose, sampleLodErr, NIL);
            AfxComputePoseBufferLod(skl, 0, jntCnt, 0, sampledJntCnt, bod->pose, mdlDispl, scn->wp);
#endif

            for (afxNat mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
            {
                afxMesh msh = mdl->rigs[mshIdx].msh;

                if (msh)
                {
                    AfxAssertObjects(1, &msh, afxFcc_MSH);

                    afxGeometry vtd;
                    afxNat baseVtx, vtxCnt;
                    AfxGetMeshVertices(msh, 0, &vtd, &baseVtx, &vtxCnt);
                    AfxAssertObjects(1, &vtd, afxFcc_VTD);

                    AkxBufferizeVertexData(scn->din, vtd);
                    AkxCmdBindVertexDataCache(cmdb, 0, vtd);
                    
                    AvxCmdApplyDrawTechnique(cmdb, scn->lightingDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->blinnTestRazrDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->testDtec, 0, scn->testVin, NIL);

#if 0
#if 0
                    if (AfxMeshIsDeformable(msh))
                        AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 0, scn->rigidVin, NIL);
#else
                    if (AfxMeshIsDeformable(msh))
                        AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
#endif
#endif
                    AfxBufferizeMesh(scn->din, msh);
                    AvxCmdBindIndexSource(cmdb, msh->cache.buf, msh->cache.base, msh->cache.range, msh->cache.stride);

                    //AvxCmdSetPrimitiveTopology(cmdb, avxTopology_TRI_LIST);
                    //AvxCmdSetCullMode(cmdb, NIL);
                    //AvxCmdSwitchFrontFace(cmdb, AfxRandom2(0, 1));

                    //AvxCmdEnableDepthTest(cmdb, TRUE);
                    //AvxCmdEnableDepthWrite(cmdb, TRUE);




                    AvxCmdBindBuffers(cmdb, 3, 0, 1, &scn->framesets[scn->frameIdx].objConstantsBuffer, NIL, NIL);

                    afxNat const *ToBoneIndices = AfxGetRiggedMeshBiasToJointMapping(mdl, mshIdx);
                    AfxBuildIndexedCompositeBuffer(skl, scn->wp, ToBoneIndices, 1, &m);
                    AfxBuildIndexedCompositeBuffer(skl, scn->wp, ToBoneIndices, msh->biasCnt, scn->framesets[scn->frameIdx].objConstants.w);

                    //AfxM4dCopyAtm(m, m);
                    //AfxM4dReset(m);
                    AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 0, sizeof(m), m);
                    AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 64, msh->biasCnt * sizeof(m), scn->framesets[scn->frameIdx].objConstants.w);
                    //AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

                    afxNat surfCnt = AfxCountMeshSurfaces(msh, 0);

                    for (afxNat j = 0; j < surfCnt; j++)
                    {
                        afxMeshSurface *sec = AfxGetMeshSurface(msh, j);

                        if (sec->mtlIdx)
                        {
                            // Push textures
                        }

                        afxNat idxCnt = (sec->triCnt * 3);
                        afxNat firstIdx = (sec->baseTriIdx * 3);
                        AvxCmdDrawIndexed(cmdb, idxCnt, 1, firstIdx, baseVtx, 0);
                        //AvxCmdDraw(cmdb, vtxCnt, 1, 0, 0);
                    }
                }
            }
        }
    }
    return err;
}

_AMX afxError AkxCmdDrawBodies(avxCmdb cmdb, akxRenderer rnd, afxReal dt, afxNat cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);
    AfxAssertObjects(cnt, bodies, afxFcc_BOD);

    for (afxNat bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        afxBody bod = bodies[bodIdx];
        AfxAssertObjects(1, &bod, afxFcc_BOD);

        afxModel mdl;
        AfxGetBodyModel(bod, &mdl);
        AfxAssertObjects(1, &mdl, afxFcc_MDL);
        afxSkeleton skl = AfxGetModelSkeleton(mdl);
        AfxTryAssertObjects(1, &skl, afxFcc_SKL);

        afxM4d m, m2;
        //AfxComputeModelDisplacement(bod->mdl, m);
        //afxReal64 ct, dt;
        //AfxGetThreadTime(&ct, &dt);
        //AfxUpdateBodyMatrix(bod, dt, FALSE, bod->placement, bod->placement);
        AfxSampleBodyAnimationsAcceleratedLOD(bod, AfxCountSkeletonJoints(skl, 0), bod->placement, rnd->lp, rnd->wp, 0.0);
        //AfxSampleBodyAnimationsLODSparse(bod, 0, bod->cachedBoneCnt, rnd->lp, 0.0, NIL);
        //AfxComputePoseBuffer(skl, 0, bod->cachedBoneCnt, rnd->lp, m, rnd->wp);
        //AfxComputeRestPoseBuffer(skl, 0, bod->cachedBoneCnt, m, rnd->wp);


        for (afxNat mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
        {
            
            afxMesh msh = mdl->rigs[mshIdx].msh;

            if (msh)
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);

                //AvxCmdApplyDrawTechnique(cmdb, rnd->lightingDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdApplyDrawTechnique(cmdb, rnd->tutCamUtilDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdApplyDrawTechnique(cmdb, rnd->blinnTestRazrDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdApplyDrawTechnique(cmdb, rnd->bodyDtec, 0, rnd->rigidVin, NIL);
                AvxCmdApplyDrawTechnique(cmdb, rnd->testDtec, 0, rnd->testVin, NIL);

                afxNat baseVtx = 0, vtxCnt = 0;
                afxGeometry vtd;
                AfxGetMeshVertices(msh, 0, &vtd, &baseVtx, &vtxCnt);

                vtxCnt = AfxCountVertices(vtd);

                AkxBufferizeVertexData(rnd->din, vtd);
                AkxCmdBindVertexDataCache(cmdb, 0, vtd);

                avxTopology top = AfxGetMeshTopology(msh);

                AfxBufferizeMesh(rnd->din, msh);
                AvxCmdBindIndexSource(cmdb, msh->cache.buf, msh->cache.base, msh->cache.range, msh->cache.stride);
                
                //AvxCmdSetPrimitiveTopology(cmdb, avxTopology_TRI_LIST);
                //AvxCmdSetCullMode(cmdb, NIL);
                //AvxCmdSwitchFrontFace(cmdb, AfxRandom2(0, 1));


                //AvxCmdEnableDepthTest(cmdb, TRUE);
                //AvxCmdEnableDepthWrite(cmdb, TRUE);

                AvxCmdBindBuffers(cmdb, 3, 0, 1, &rnd->framesets[rnd->frameIdx].objConstantsBuffer, NIL, NIL);

                afxNat const *ToBoneIndices = AfxGetRiggedMeshBiasToJointMapping(mdl, mshIdx);
                AfxBuildIndexedCompositeBuffer(skl, rnd->wp, ToBoneIndices, 1, &m);
                //AfxM4dCopyAtm(m, m);
                //AfxM4dReset(m);
                AvxCmdUpdateBuffer(cmdb, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 0, sizeof(m), m);
                AvxCmdUpdateBuffer(cmdb, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 64, sizeof(m), m);
                //AvxCmdUpdateBuffer(cmdb, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

                afxNat surfCnt = AfxCountMeshSurfaces(msh, 0);

                for (afxNat j = 0; j < surfCnt; j++)
                {
                    afxMeshSurface *sec = AfxGetMeshSurface(msh, j);
                    //AfxAssert(msh->topology->primType == avxTopology_TRI_LIST);

                    akxMaterialConstants mat;

                    if (sec->mtlIdx == AFX_INVALID_INDEX)
                    {
                        AfxColorSet(mat.Kd, 0.3f, 0.3f, 0.3f, 1.0f);
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
                    AvxCmdDrawIndexed(cmdb, idxCnt, 1, firstIdx, baseVtx, 0);
                    //AvxCmdDraw(cmdb, vtxCnt, 1, 0, 0);
                }
                //AvxCmdDrawIndexed(cmdb, msht->triCnt * 3, 1, 0, 0, 0);

                //AvxCmdDrawIndexed(cmdb, msht->triCnt * 3, 1, 0, 0, 0);
            }
        }
    }
    return err;
}

_AMX afxError AkxCmdDrawTestIndexed(avxCmdb cmdb, akxRenderer rnd)
{
    AvxCmdApplyDrawTechnique(cmdb, rnd->testDtec, 0, rnd->testVin, NIL);

    AvxCmdBindVertexSources(cmdb, 0, 1, &rnd->testVbo, NIL, NIL, (afxNat32[]) { sizeof(afxV3d) });
    //AvxCmdResetVertexStreams(cmdb, 1, NIL, (afxNat32[]) { sizeof(afxV3d) }, NIL);
    //AvxCmdResetVertexAttributes(cmdb, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
    AvxCmdBindIndexSource(cmdb, rnd->testIbo, 0, sizeof(afxNat32) * 6, sizeof(afxNat32));
    AvxCmdDrawIndexed(cmdb, 6, 1, 0, 0, 0);
    //AvxCmdDraw(cmdb, 6, 1, 0, 0);
    return 0;
}

_AMX afxError AfxRendererSetStar(akxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    akxViewConstants *viewConstants = &rnd->framesets[rnd->frameIdx].viewConstants;

    AfxV4dCopy(viewConstants->starPos, pos);
    AfxV4dCopy(viewConstants->starDir, dir);
    AfxV4dCopy(viewConstants->starKd, Kd);

    return err;
}

_AMX afxError AkxCmdEndSceneRendering(avxCmdb cmdb, akxRenderer rnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    return err;
}

_AMX afxError AkxCmdBeginSceneRendering(avxCmdb cmdb, akxRenderer rnd, afxCamera cam, afxRect const* drawArea, avxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    afxNat frameIdx = (rnd->frameIdx + 1) % rnd->frameCnt;
    rnd->frameIdx = frameIdx;

    //avxCanvas canv;
    //AfxGetDrawOutputCanvases(rnd->dout, outBufIdx, &canv);
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

    akxViewConstants *viewConstants = &rnd->framesets[frameIdx].viewConstants;

    if ((rnd->activeCam = cam))
    {
        //cam = rnd->activeCamera;

        afxV2d extent = { rnd->drawArea.extent[0], rnd->drawArea.extent[1] };
        AfxSetCameraAspectRatios(cam, AfxFindPhysicalAspectRatio(rnd->drawArea.extent[0], rnd->drawArea.extent[1]), extent, extent);

        viewConstants->viewExtent[0] = rnd->drawArea.extent[0];
        viewConstants->viewExtent[1] = rnd->drawArea.extent[1];

        afxV4d viewPos;
        AfxGetCameraPosition(cam, viewPos);
        AfxV4dCopyAtv3d(viewConstants->viewPos, viewPos);

        afxM4d v, iv, p, ip;
        AfxRecomputeCameraMatrices(cam);
        AfxGetCameraMatrices(cam, v, iv);
        AfxGetCameraProjectiveMatrices(cam, p, ip);

        AfxM4dCopy(viewConstants->p, p);
        AfxM4dCopy(viewConstants->ip, ip);
        AfxM4dCopyAtm(viewConstants->v, v);
        AfxM4dCopy(viewConstants->iv, iv);
        AfxM4dMultiply(viewConstants->pv, p, v);
        AfxM4dInvert(viewConstants->ipv, viewConstants->pv);

    }

    AvxCmdUpdateBuffer(cmdb, rnd->framesets[frameIdx].viewConstantsBuffer, 0, sizeof(*viewConstants), viewConstants);
    AvxCmdBindBuffers(cmdb, 0, 0, 1, &rnd->framesets[frameIdx].viewConstantsBuffer, NIL, NIL);
    return err;
}

_AMX afxError _AfxRndDtor(akxRenderer rnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    for (afxNat i = 0; i < rnd->frameCnt; i++)
    {
        AfxReleaseObjects(1, &rnd->framesets[i].viewConstantsBuffer);
        AfxReleaseObjects(1, &rnd->framesets[i].shdConstantsBuffer);
        AfxReleaseObjects(1, &rnd->framesets[i].mtlConstantsBuffer);
        AfxReleaseObjects(1, &rnd->framesets[i].objConstantsBuffer);

    }

    AfxReleaseObjects(1, &rnd->sky.cubemap);
    AfxReleaseObjects(1, &rnd->sky.cube);
    AfxReleaseObjects(1, &rnd->sky.skyDtec);
    AfxReleaseObjects(1, &rnd->sky.skyVin);
    AfxReleaseObjects(1, &rnd->sky.smp);

    AfxReleaseObjects(1, &rnd->testVin);
    AfxReleaseObjects(1, &rnd->rigidVin);
    AfxReleaseObjects(1, &rnd->skinnedVin);

    AfxReleaseObjects(1, &rnd->blinnTestRazrDtec);
    AfxReleaseObjects(1, &rnd->bodyDtec);
    AfxReleaseObjects(1, &rnd->lightingDtec);
    AfxReleaseObjects(1, &rnd->testDtec);
    AfxReleaseObjects(1, &rnd->tutCamUtilDtec);

    afxDrawInput din = rnd->din;

    AfxReleaseObjects(1, &din);
#if 0
    for (afxNat i = 0; i < rnd->frameCnt; i++)
    {
        if (rnd->canv[i])
            AfxReleaseObject(&rnd->canv[i]->obj);
    }
#endif

    AfxDeallocateArray(&rnd->capturedNodes);

    return err;
}

_AMX afxError _AfxRndCtor(akxRenderer rnd, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);

    akxRendererConfig const* config = ((akxRendererConfig const*)args[0]) + invokeNo;

    afxSimulation sim = AfxGetProvider(rnd);
    rnd->cachedSim = sim;
    afxDrawContext dctx = sim->dctx;
    rnd->cachedDctx = dctx;
    
    //rnd->dout = NIL;

    rnd->frameCnt = 2;
    rnd->frameIdx = 0;

    //AfxZero2(rnd->canv, sizeof(rnd->canv));
    AfxZero(rnd->framesets, sizeof(rnd->framesets));
    afxDrawInput din = config->din;

    // acquire and set up our dedicated draw input device.

    if (!config->din)
    {
        afxDrawInputConfig dinConfig = { 0 };
        dinConfig.proc = config->dinProc;
        dinConfig.udd = rnd;
        dinConfig.cmdPoolMemStock = 4096;
        dinConfig.estimatedSubmissionCnt = 3;

        AfxOpenDrawInput(0, &dinConfig, &din);
        AfxAssert(din);
        AfxReconnectDrawInput(din, dctx);
    }
    else
    {
        AfxReacquireObjects(1, &din);
    }
    rnd->din = din;

    afxUri uri;
    avxVertexInput vin;

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

        afxBufferInfo vboSpec = { 0 };
        vboSpec.bufCap = sizeof(testVertices);
        vboSpec.flags = afxBufferFlag_W;
        vboSpec.usage = afxBufferUsage_VERTEX;
        AfxAcquireBuffers(dctx, 1, &vboSpec, &rnd->testVbo);
        
        afxBufferIo vboIop = { 0 };
        vboIop.dstStride = 1;
        vboIop.srcStride = 1;
        vboIop.rowCnt = vboSpec.bufCap;
        AfxUpdateBuffer(rnd->testVbo, 1, &vboIop, testVertices, 0);

        afxBufferInfo iboSpec = { 0 };
        iboSpec.bufCap = sizeof(testIndices);
        iboSpec.flags = afxBufferFlag_W;
        iboSpec.usage = afxBufferUsage_INDEX;
        AfxAcquireBuffers(dctx, 1, &iboSpec, &rnd->testIbo);

        afxBufferIo iboIop = { 0 };
        iboIop.dstStride = 1;
        iboIop.srcStride = 1;
        iboIop.rowCnt = iboSpec.bufCap;
        AfxUpdateBuffer(rnd->testIbo, 1, &iboIop, testIndices, 0);

        avxVertexInputStream const vinStreams[] =
        {
            {
                .instanceRate = 0,
                .slotIdx = 0
            }
        };
        avxVertexInputAttr const vinAttrs[] =
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
        rnd->testVin = vin;

        AfxMakeUri(&uri, 0, "../data/pipeline/test/test.xsh.xml", 0);
        AfxLoadDrawTechniques(din, 1, &uri, &rnd->testDtec);
    }

    // acquire needed resources and set up our sky.
    {
        rnd->skyEnabled = 1;

        if (rnd->skyEnabled)
        {
            AfxBuildSkybox(&rnd->sky, sim, din);
        }
    }

    avxVertexInputStream const vinStreams[] =
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
    avxVertexInputAttr const vinAttrs[] =
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

    avxVertexInput skinnedVin = AfxAcquireVertexInput(dctx, AFX_COUNTOF(vinStreams), vinStreams, AFX_COUNTOF(vinAttrs), vinAttrs);
    AfxAssertObjects(1, &skinnedVin, afxFcc_VIN);
    rnd->skinnedVin = skinnedVin;

    avxVertexInputStream const vinStreams2[] =
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
    avxVertexInputAttr const vinAttrs2[] =
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

    avxVertexInput rigidVin = AfxAcquireVertexInput(dctx, AFX_COUNTOF(vinStreams2), vinStreams2, AFX_COUNTOF(vinAttrs2), vinAttrs2);
    AfxAssertObjects(1, &rigidVin, afxFcc_VIN);
    rnd->rigidVin = rigidVin;


    {
        afxBufferInfo bufSpec[] =
        {
            { .bufCap = sizeof(akxViewConstants), .flags = afxBufferFlag_W, .usage = afxBufferUsage_UNIFORM },
            { .bufCap = sizeof(akxShaderConstants), .flags = afxBufferFlag_W, .usage = afxBufferUsage_UNIFORM },
            { .bufCap = sizeof(akxMaterialConstants), .flags = afxBufferFlag_W, .usage = afxBufferUsage_UNIFORM },
            { .bufCap = sizeof(akxInstanceConstants), .flags = afxBufferFlag_W, .usage = afxBufferUsage_UNIFORM }
        };
#if 0
        afxRasterInfo texiDepthSurfB = { 0 };
        texiDepthSurfB.e
        afxTextureBlueprint depthSurfB;
        AfxAcquireTextureBlueprint(&depthSurfB, (afxWhd) {1024, 1024, 1 }, avxFormat_D24, afxRasterUsage_DRAW);
        AfxTextureBlueprintAddImage(&depthSurfB, avxFormat_D24, (afxWhd) { 1, 1, 1 }, NIL, 0);
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
        AfxMakeUri(&uri, 0, "../data/pipeline/body/body.xsh.xml", 0);
        AfxLoadDrawTechniques(din, 1, &uri, &rnd->bodyDtec);
    }

    AfxRendererSetStar(rnd, AfxSpawnV4d(0, 0, 0, 1), AfxSpawnV3d(-0.8660f, 0.5f, 0), AfxSpawnV4d(0.8, 0.8, 0.8, 0.8));
    // ambient 0.2, 0.2, 0.2, 0.2

    AfxAcquirePoses(sim, 1, (afxNat[]) { 255 }, &rnd->lp);
    AfxAcquirePoseBuffers(sim, 1, (afxNat[]) { 255 }, (afxBool[]) {TRUE}, &rnd->wp);

    AfxMakeUri(&uri, 0, "../data/pipeline/testLighting/testLighting.xsh.xml", 0);
    AfxLoadDrawTechniques(din, 1, &uri, &rnd->blinnTestRazrDtec);

    AfxMakeUri(&uri, 0, "../data/pipeline/tutCamUtil/tutCamUtil.xsh.xml", 0);
    AfxLoadDrawTechniques(din, 1, &uri, &rnd->tutCamUtilDtec);

    AfxMakeUri(&uri, 0, "../data/pipeline/lighting/lighting.xsh.xml", 0);
    AfxLoadDrawTechniques(din, 1, &uri, &rnd->lightingDtec);

    AfxAllocateArray(&rnd->capturedNodes, 100, sizeof(afxBody), NIL);

    return err;
}

_AMX afxClassConfig _AfxRndMgrCfg =
{
    .fcc = afxFcc_RND,
    .name = "Renderer",
    .fixedSiz = sizeof(AFX_OBJECT(akxRenderer)),
    .ctor = (void*)_AfxRndCtor,
    .dtor = (void*)_AfxRndDtor
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AMX afxError AkxAcquireRenderers(afxSimulation sim, afxNat cnt, akxRenderer rnd[], akxRendererConfig const config[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (AfxAcquireObjects(AfxGetRendererClass(sim), cnt, (afxObject*)rnd, (void const*[]) { (void*)config }))
        AfxThrowError();

    return err;
}
