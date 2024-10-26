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
#define _AMX_MODEL_C
#define _AMX_BODY_C
#define _AMX_MATERIAL_C
#define _AVX_VERTEX_BUFFER_C
#include "../../dev/AmxImplKit.h"
#include "../../dev/AvxImplKit.h"

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
        afxUnit arrel;

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

_AMX afxError AkxDrawBodies(akxRenderer scn, avxCmdb cmdb, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &scn, afxFcc_RND);

    for (afxUnit bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        afxBody bod = bodies[bodIdx];
        AfxAssertObjects(1, &bod, afxFcc_BOD);
        afxModel mdl;

        if (AfxGetBodyModel(bod, &mdl))
        {
            AfxAssertObjects(1, &mdl, afxFcc_MDL);
            afxSkeleton skl;
            AfxGetModelSkeleton(mdl, &skl);
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            afxUnit jntCnt = AfxCountSkeletonJoints(skl, 0);
            afxReal sampleLodErr = 0.0;
            afxUnit sampledJntCnt = AfxCountSkeletonJoints(skl, sampleLodErr);

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

            for (afxUnit mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
            {
                afxMesh msh = mdl->rigs[mshIdx].msh;

                if (msh)
                {
                    AfxAssertObjects(1, &msh, afxFcc_MSH);
                    
                    afxMeshInfo mshi;
                    AfxDescribeMesh(msh, &mshi);

                    afxMeshMorph mshm;
                    AfxGetMeshMorphes(msh, 0, 1, &mshm);
                    AfxAssertObjects(1, &mshm.geo, afxFcc_GEO);

                    avxVertexCache vtxCache;
                    AfxBufferizeGeometry(mshm.geo, &vtxCache);

                    AvxCmdBindVertexSources(cmdb, 0, 2,
                        (afxBuffer[]) { vtxCache.buf, vtxCache.buf },
                        (afxUnit32[]) { vtxCache.streams[0].base, vtxCache.streams[1].base },
                        (afxUnit32[]) { vtxCache.streams[0].range, vtxCache.streams[1].range },
                        (afxUnit32[]) { vtxCache.streams[0].stride, vtxCache.streams[1].stride });
                    
                    AvxCmdApplyDrawTechnique(cmdb, scn->lightingDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->blinnTestRazrDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->testDtec, 0, scn->testVin, NIL);

#if 0
#if 0
                    if (AfxIsMeshDeformable(msh))
                        AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 0, scn->rigidVin, NIL);
#else
                    if (AfxIsMeshDeformable(msh))
                        AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
#endif
#endif

                    avxIndexCache idxCache;
                    AfxBufferizeMesh(msh, 0, &vtxCache, &idxCache);
                    AvxCmdBindIndexSource(cmdb, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);

                    //AvxCmdSetPrimitiveTopology(cmdb, avxTopology_TRI_LIST);
                    //AvxCmdSetCullMode(cmdb, NIL);
                    //AvxCmdSwitchFrontFace(cmdb, AfxRandom2(0, 1));

                    //AvxCmdEnableDepthTest(cmdb, TRUE);
                    //AvxCmdEnableDepthWrite(cmdb, TRUE);

                    AvxCmdBindBuffers(cmdb, 3, 0, 1, &scn->framesets[scn->frameIdx].objConstantsBuffer, NIL, NIL);

                    afxUnit const *ToBoneIndices = AfxGetMeshRigBiasToJointMapping(mdl, mshIdx);
                    AfxBuildIndexedCompositeBuffer(skl, scn->wp, ToBoneIndices, 1, &m);
                    AfxBuildIndexedCompositeBuffer(skl, scn->wp, ToBoneIndices, mshi.biasCnt, scn->framesets[scn->frameIdx].objConstants.w);

                    //AfxM4dCopyAtm(m, m);
                    //AfxM4dReset(m);
                    AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 0, sizeof(m), m);
                    AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 64, mshi.biasCnt * sizeof(m), scn->framesets[scn->frameIdx].objConstants.w);
                    //AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

                    afxUnit surfCnt = mshi.secCnt;

                    for (afxUnit j = 0; j < surfCnt; j++)
                    {
                        afxMeshSection sec;
                        AfxGetMeshSections(msh, j, 1, &sec);

                        if (sec.mtlIdx)
                        {
                            // Push textures
                        }

                        afxUnit idxCnt = (sec.triCnt * 3);
                        afxUnit firstIdx = (sec.baseTriIdx * 3);
                        AvxCmdDrawIndexed(cmdb, idxCnt, 1, firstIdx, mshm.baseVtx, 0);
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

    AkxDrawBodies(scn, cmdb, scn->capturedNodes.pop, scn->capturedNodes.data);

    return err;
#endif
    afxBody bod;
    afxUnit i = 0;
    while (AfxEnumerateBodies(sim, i++, 1, &bod))
    {
        AfxAssertObjects(1, &bod, afxFcc_BOD);
        afxModel mdl;

        if (AfxGetBodyModel(bod, &mdl))
        {
            AfxAssertObjects(1, &mdl, afxFcc_MDL);
            afxSkeleton skl;
            AfxGetModelSkeleton(mdl, &skl);
            AfxAssertObjects(1, &skl, afxFcc_SKL);
            afxUnit jntCnt = AfxCountSkeletonJoints(skl, 0);
            afxReal sampleLodErr = 0.0;
            afxUnit sampledJntCnt = AfxCountSkeletonJoints(skl, sampleLodErr);

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

            for (afxUnit mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
            {
                afxMesh msh = mdl->rigs[mshIdx].msh;

                if (msh)
                {
                    AfxAssertObjects(1, &msh, afxFcc_MSH);

                    afxMeshInfo mshi;
                    AfxDescribeMesh(msh, &mshi);

                    afxMeshMorph mshm;
                    AfxGetMeshMorphes(msh, 0, 1, &mshm);
                    AfxAssertObjects(1, &mshm.geo, afxFcc_GEO);

                    avxVertexCache vtxCache;
                    AfxBufferizeGeometry(mshm.geo, &vtxCache);
                    
                    AvxCmdBindVertexSources(cmdb, 0, 2,
                        (afxBuffer[]) { vtxCache.buf, vtxCache.buf },
                        (afxUnit32[]) { vtxCache.streams[0].base, vtxCache.streams[1].base },
                        (afxUnit32[]) { vtxCache.streams[0].range, vtxCache.streams[1].range },
                        (afxUnit32[]) { vtxCache.streams[0].stride, vtxCache.streams[1].stride });
                    

                    AvxCmdApplyDrawTechnique(cmdb, scn->lightingDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->blinnTestRazrDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(cmdb, scn->testDtec, 0, scn->testVin, NIL);

#if 0
#if 0
                    if (AfxIsMeshDeformable(msh))
                        AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(cmdb, scn->bodyDtec, 0, scn->rigidVin, NIL);
#else
                    if (AfxIsMeshDeformable(msh))
                        AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(cmdb, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
#endif
#endif
                    avxIndexCache idxCache;
                    AfxBufferizeMesh(msh, 0, &vtxCache, &idxCache);
                    AvxCmdBindIndexSource(cmdb, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);

                    //AvxCmdSetPrimitiveTopology(cmdb, avxTopology_TRI_LIST);
                    //AvxCmdSetCullMode(cmdb, NIL);
                    //AvxCmdSwitchFrontFace(cmdb, AfxRandom2(0, 1));

                    //AvxCmdEnableDepthTest(cmdb, TRUE);
                    //AvxCmdEnableDepthWrite(cmdb, TRUE);




                    AvxCmdBindBuffers(cmdb, 3, 0, 1, &scn->framesets[scn->frameIdx].objConstantsBuffer, NIL, NIL);

                    afxUnit const *ToBoneIndices = AfxGetMeshRigBiasToJointMapping(mdl, mshIdx);
                    AfxBuildIndexedCompositeBuffer(skl, scn->wp, ToBoneIndices, 1, &m);
                    AfxBuildIndexedCompositeBuffer(skl, scn->wp, ToBoneIndices, mshi.biasCnt, scn->framesets[scn->frameIdx].objConstants.w);

                    //AfxM4dCopyAtm(m, m);
                    //AfxM4dReset(m);
                    AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 0, sizeof(m), m);
                    AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 64, mshi.biasCnt * sizeof(m), scn->framesets[scn->frameIdx].objConstants.w);
                    //AvxCmdUpdateBuffer(cmdb, scn->framesets[scn->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

                    afxUnit surfCnt = mshi.secCnt;

                    for (afxUnit j = 0; j < surfCnt; j++)
                    {
                        afxMeshSection sec;
                        AfxGetMeshSections(msh, j, 1, &sec);

                        if (sec.mtlIdx)
                        {
                            // Push textures
                        }

                        afxUnit idxCnt = (sec.triCnt * 3);
                        afxUnit firstIdx = (sec.baseTriIdx * 3);
                        AvxCmdDrawIndexed(cmdb, idxCnt, 1, firstIdx, mshm.baseVtx, 0);
                        //AvxCmdDraw(cmdb, vtxCnt, 1, 0, 0);
                    }
                }
            }
        }
    }
    return err;
}

_AMX afxError AkxCmdDrawBodies(avxCmdb cmdb, akxRenderer rnd, afxReal dt, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &rnd, afxFcc_RND);
    AfxAssertObjects(cnt, bodies, afxFcc_BOD);

    for (afxUnit bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        afxBody bod = bodies[bodIdx];
        AfxAssertObjects(1, &bod, afxFcc_BOD);

        afxModel mdl;
        AfxGetBodyModel(bod, &mdl);
        AfxAssertObjects(1, &mdl, afxFcc_MDL);
        afxSkeleton skl;
        AfxGetModelSkeleton(mdl, &skl);
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


        for (afxUnit mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
        {
            
            afxMesh msh = mdl->rigs[mshIdx].msh;

            if (msh)
            {
                AfxAssertObjects(1, &msh, afxFcc_MSH);
                afxMeshInfo mshi;
                AfxDescribeMesh(msh, &mshi);
                afxMeshMorph mshm;
                AfxGetMeshMorphes(msh, 0, 1, &mshm);
                AfxAssertObjects(1, &mshm.geo, afxFcc_GEO);

                //AvxCmdApplyDrawTechnique(cmdb, rnd->lightingDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdApplyDrawTechnique(cmdb, rnd->tutCamUtilDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdApplyDrawTechnique(cmdb, rnd->blinnTestRazrDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdApplyDrawTechnique(cmdb, rnd->bodyDtec, 0, rnd->rigidVin, NIL);
                AvxCmdApplyDrawTechnique(cmdb, rnd->testDtec, 0, rnd->testVin, NIL);

                avxVertexCache vtxCache;
                AfxBufferizeGeometry(mshm.geo, &vtxCache);

                avxTopology top = AfxGetMeshTopology(msh);

                avxIndexCache idxCache;
                AfxBufferizeMesh(msh, 0, &vtxCache, &idxCache);
                AvxCmdBindIndexSource(cmdb, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);
                
                //AvxCmdSetPrimitiveTopology(cmdb, avxTopology_TRI_LIST);
                //AvxCmdSetCullMode(cmdb, NIL);
                //AvxCmdSwitchFrontFace(cmdb, AfxRandom2(0, 1));


                //AvxCmdEnableDepthTest(cmdb, TRUE);
                //AvxCmdEnableDepthWrite(cmdb, TRUE);

                AvxCmdBindBuffers(cmdb, 3, 0, 1, &rnd->framesets[rnd->frameIdx].objConstantsBuffer, NIL, NIL);

                afxUnit const *ToBoneIndices = AfxGetMeshRigBiasToJointMapping(mdl, mshIdx);
                AfxBuildIndexedCompositeBuffer(skl, rnd->wp, ToBoneIndices, 1, &m);
                //AfxM4dCopyAtm(m, m);
                //AfxM4dReset(m);
                AvxCmdUpdateBuffer(cmdb, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 0, sizeof(m), m);
                AvxCmdUpdateBuffer(cmdb, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 64, sizeof(m), m);
                //AvxCmdUpdateBuffer(cmdb, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

                afxUnit surfCnt = mshi.secCnt;

                for (afxUnit j = 0; j < surfCnt; j++)
                {
                    afxMeshSection sec;
                    AfxGetMeshSections(msh, j, 1, &sec);
                    //AFX_ASSERT(msh->topology->primType == avxTopology_TRI_LIST);

                    akxMaterialConstants mat;

                    if (sec.mtlIdx == AFX_INVALID_INDEX)
                    {
                        AfxColorSet(mat.Kd, 0.3f, 0.3f, 0.3f, 1.0f);
                        mat.hasDiffTex = FALSE;
                        //AfxUpdateBuffer(rnd->framesets[rnd->frameIdx].mtlConstantsBuffer, 0, sizeof(mat), &mat);
                    }
                    else
                    {
#if 0
                        AFX_ASSERT(sec->mtlIdx < msh->mtlSlotCnt);
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

                    afxUnit idxCnt = (sec.triCnt * 3);
                    afxUnit firstIdx = (sec.baseTriIdx * 3);
                    AvxCmdDrawIndexed(cmdb, idxCnt, 1, firstIdx, mshm.baseVtx, 0);
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

    AvxCmdBindVertexSources(cmdb, 0, 1, &rnd->testVbo, NIL, NIL, (afxUnit32[]) { sizeof(afxV3d) });
    //AvxCmdResetVertexStreams(cmdb, 1, NIL, (afxUnit32[]) { sizeof(afxV3d) }, NIL);
    //AvxCmdResetVertexAttributes(cmdb, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
    AvxCmdBindIndexSource(cmdb, rnd->testIbo, 0, sizeof(afxUnit32) * 6, sizeof(afxUnit32));
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

    afxUnit frameIdx = (rnd->frameIdx + 1) % rnd->frameCnt;
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
        afxWhd whd = AfxGetCanvasExtent(canv);
        rnd->drawArea.extent[0] = whd.w;
        rnd->drawArea.extent[1] = whd.h;

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

    for (afxUnit i = 0; i < rnd->frameCnt; i++)
    {
        AfxReleaseObjects(1, &rnd->framesets[i].viewConstantsBuffer);
        AfxReleaseObjects(1, &rnd->framesets[i].shdConstantsBuffer);
        AfxReleaseObjects(1, &rnd->framesets[i].mtlConstantsBuffer);
        AfxReleaseObjects(1, &rnd->framesets[i].objConstantsBuffer);

    }

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
    for (afxUnit i = 0; i < rnd->frameCnt; i++)
    {
        if (rnd->canv[i])
            AfxReleaseObject(&rnd->canv[i]->obj);
    }
#endif

    AfxCleanUpArray(&rnd->capturedNodes);

    return err;
}

_AMX afxError _AfxRndCtor(akxRenderer rnd, void** args, afxUnit invokeNo)
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
        AFX_ASSERT(din);
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
        afxUnit32 testIndices[] =
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

        vin = AfxAcquireVertexInput(dctx, ARRAY_SIZE(vinStreams), vinStreams, ARRAY_SIZE(vinAttrs), vinAttrs);
        AfxAssertObjects(1, &vin, afxFcc_VIN);
        rnd->testVin = vin;

        AfxMakeUri(&uri, 0, "../data/pipeline/test/test.xsh.xml", 0);
        AfxLoadDrawTechnique(din, &uri, &rnd->testDtec);
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
            .fmt = afxVertexFormat_U8x4
        },
        {
            .location = 2,
            .streamIdx = 0,
            .offset = 16,
            .fmt = afxVertexFormat_V4D
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

    avxVertexInput skinnedVin = AfxAcquireVertexInput(dctx, ARRAY_SIZE(vinStreams), vinStreams, ARRAY_SIZE(vinAttrs), vinAttrs);
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

    avxVertexInput rigidVin = AfxAcquireVertexInput(dctx, ARRAY_SIZE(vinStreams2), vinStreams2, ARRAY_SIZE(vinAttrs2), vinAttrs2);
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

        for (afxUnit i = 0; i < rnd->frameCnt; i++)
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
        AfxLoadDrawTechnique(din, &uri, &rnd->bodyDtec);
    }

    AfxRendererSetStar(rnd, AfxV4d(0, 0, 0, 1), AfxV3d(-0.8660f, 0.5f, 0), AfxV4d(0.8, 0.8, 0.8, 0.8));
    // ambient 0.2, 0.2, 0.2, 0.2

    AfxAcquirePoses(sim, 1, (afxUnit[]) { 255 }, &rnd->lp);
    AfxAcquirePoseBuffers(sim, 1, (afxUnit[]) { 255 }, (afxBool[]) {TRUE}, &rnd->wp);

    AfxMakeUri(&uri, 0, "../data/pipeline/testLighting/testLighting.xsh.xml", 0);
    AfxLoadDrawTechnique(din, &uri, &rnd->blinnTestRazrDtec);

    AfxMakeUri(&uri, 0, "../data/pipeline/tutCamUtil/tutCamUtil.xsh.xml", 0);
    AfxLoadDrawTechnique(din, &uri, &rnd->tutCamUtilDtec);

    AfxMakeUri(&uri, 0, "../data/pipeline/lighting/lighting.xsh.xml", 0);
    AfxLoadDrawTechnique(din, &uri, &rnd->lightingDtec);

    AfxMakeArray(&rnd->capturedNodes, 100, sizeof(afxBody), NIL);

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

_AMX afxError AkxAcquireRenderers(afxSimulation sim, afxUnit cnt, akxRenderer rnd[], akxRendererConfig const config[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    if (AfxAcquireObjects(AfxGetRendererClass(sim), cnt, (afxObject*)rnd, (void const*[]) { (void*)config }))
        AfxThrowError();

    return err;
}
