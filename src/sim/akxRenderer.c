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
#define _ASX_SIMULATION_C
#define _ASX_MODEL_C
#define _ASX_BODY_C
#define _ASX_MATERIAL_C
#define _AVX_VERTEX_BUFFER_C
#include "../draw/impl/avxImplementation.h"
#include "impl/asxImplementation.h"

#if 0
_ASX afxBool _AsxCaptureBodCb(afxBody bod, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxFrustum* frustum = udd[0];
    afxReal lodErr = *(afxReal*)udd[1];
    afxArray* pvs = udd[2];

    //AfxFrustumTestAabbs(&frustum, 1, &aabb);

    if (1) // do visibility culling
    {
        afxUnit arrel;

        if (!AfxPushArrayUnits(pvs, 1, &arrel, &bod)) AfxThrowError();
        else
        {

        }
    }

    return TRUE;
}

_ASX afxError AsxCaptureBodyPvs(afxSimulation sim, afxCamera cam, afxReal lodErr, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxFrustum frustum;
    AfxGetCameraFrustum(cam, &frustum);

    AfxInvokeBodies(sim, 0, AFX_N32_MAX, (void*)_AsxCaptureBodCb, (void*[]) { &frustum, &lodErr, pvs });

    return err;
}
#endif

_ASX afxError AsxDrawBodies(akxRenderer scn, afxContext mctx, afxDrawContext dctx, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &scn);

    for (afxUnit bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        afxBody bod = bodies[bodIdx];
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
        afxModel mdl;

        if (!AfxGetBodyModel(bod, &mdl))
            continue;

        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
        afxUnit jntCnt = AfxCountJoints(mdl, 0);
        afxReal sampleLodErr = 0.0;
        afxUnit sampledJntCnt = AfxCountJoints(mdl, sampleLodErr);

        afxM4d m, m2;
        //AfxComputeModelDisplacement(bod->mdl, m);
        //afxReal64 ct, dt;
        //AfxGetThreadTime(&ct, &dt);
        //AfxUpdateBodyMatrix(bod, dt, FALSE, bod->placement, bod->placement);
        //AfxSampleBodyAnimationsLODSparse(bod, 0, bod->cachedBoneCnt, scn->lp, 0.0, NIL);
        //AfxBuildPlacement(skl, 0, bod->cachedBoneCnt, scn->lp, m, scn->wp);
        //AfxComputeRestPosture(skl, 0, bod->cachedBoneCnt, m, scn->wp);

#if !0
        AsxCmdSampleBodyAnimationsAcceleratedLOD(mctx, bod, jntCnt, bod->placement, scn->lp, scn->wp, 0.0);
        //AfxSampleBodyAnimationsAcceleratedLOD(bod, jntCnt, bod->placement, scn->lp, scn->wp, 0.0);
#else
        afxM4d mdlDispl;
        AfxComputeModelDisplacement(mdl, mdlDispl);
        AfxSampleBodyAnimationsLODSparse(bod, 0, sampledJntCnt, bod->pose, sampleLodErr, NIL);
        AfxComputePostureLod(skl, 0, jntCnt, 0, sampledJntCnt, bod->pose, mdlDispl, scn->wp);
#endif

        for (afxUnit mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
        {
            afxMeshRigInfo rig;
            AfxDescribeMeshRig(mdl, mshIdx, &rig);
            
            afxMesh msh = rig.msh;
            if (!msh) continue;
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
            
            afxMeshInfo mshi;
            AfxDescribeMesh(msh, &mshi);

            afxMeshMorph mshm;
            AfxGetMeshMorphes(msh, 0, 1, &mshm);

            avxVertexCache vtxCache;
            avxIndexCache idxCache;
            AfxBufferizeMesh(msh, 0, &vtxCache, &idxCache);

            avxBufferedStream vtxSrcs[] =
            {
                {
                    .buf = vtxCache.buf,
                    .offset = vtxCache.streams[0].base,
                    .range = vtxCache.streams[0].range,
                    .stride = vtxCache.streams[0].stride
                },
                {
                    .buf = vtxCache.buf,
                    .offset = vtxCache.streams[1].base,
                    .range = vtxCache.streams[1].range,
                    .stride = vtxCache.streams[1].stride
                },
            };
            AvxCmdBindVertexBuffers(dctx, 0, ARRAY_SIZE(vtxSrcs), vtxSrcs);
                    
            AvxCmdApplyDrawTechnique(dctx, scn->lightingDtec, 0, scn->rigidVin, NIL);
            //AvxCmdApplyDrawTechnique(dctx, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
            //AvxCmdApplyDrawTechnique(dctx, scn->blinnTestRazrDtec, 0, scn->rigidVin, NIL);
            //AvxCmdApplyDrawTechnique(dctx, scn->bodyDtec, 0, scn->rigidVin, NIL);
            //AvxCmdApplyDrawTechnique(dctx, scn->testDtec, 0, scn->testVin, NIL);

#if 0
#if 0
            if (AfxIsMeshDeformable(msh))
                AvxCmdApplyDrawTechnique(dctx, scn->bodyDtec, 1, scn->skinnedVin, NIL);
            else
                AvxCmdApplyDrawTechnique(dctx, scn->bodyDtec, 0, scn->rigidVin, NIL);
#else
            if (AfxIsMeshDeformable(msh))
                AvxCmdApplyDrawTechnique(dctx, scn->tutCamUtilDtec, 1, scn->skinnedVin, NIL);
            else
                AvxCmdApplyDrawTechnique(dctx, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
#endif
#endif

            AvxCmdBindIndexBuffer(dctx, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);

            //AvxCmdSetPrimitiveTopology(dctx, avxTopology_TRI_LIST);
            //AvxCmdSetCullMode(dctx, NIL);
            //AvxCmdSwitchFrontFace(dctx, AfxRandom2(0, 1));

            //AvxCmdEnableDepthTest(dctx, TRUE);
            //AvxCmdEnableDepthWrite(dctx, TRUE);

            AvxCmdBindBuffers(dctx, 3, 0, 1, (afxBufferMap[]) { {.buf = scn->framesets[scn->frameIdx].objUbo} });

            //afxUnit const *ToBoneIndices = AfxGetMeshRigBiasToJointMapping(mdl, mshIdx);
            AfxBuildRiggedMeshCompositeMatrices(mdl, mshIdx, scn->wp, 1, &m);
            AfxBuildRiggedMeshCompositeMatrices(mdl, mshIdx, scn->wp, mshi.biasCnt, scn->framesets[scn->frameIdx].objConstants.m);

            //AfxM4dCopyAtm(m, m);
            //AfxM4dReset(m);
            AvxCmdUpdateBuffer(dctx, scn->framesets[scn->frameIdx].objUbo, 0, sizeof(m), m);
            AvxCmdUpdateBuffer(dctx, scn->framesets[scn->frameIdx].objUbo, 0, mshi.biasCnt * sizeof(m), scn->framesets[scn->frameIdx].objConstants.m);
            //AvxCmdUpdateBuffer(dctx, scn->framesets[scn->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

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
                AvxCmdDrawIndexed(dctx, idxCnt, 1, firstIdx, mshm.baseVtx, 0);
                //AvxCmdDraw(dctx, vtxCnt, 1, 0, 0);
            }
        }
    }
    return err;
}

#if 0
_ASX afxError AsxBeginSceneCapture(akxRenderer scn, afxCamera cam, afxSimulation sim, afxDrawContext dctx, afxContext mctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &scn);
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
#if !0
    AfxEmptyArray(&scn->capturedNodes);
    AsxCaptureBodyPvs(sim, cam, 0, &scn->capturedNodes);

    AsxDrawBodies(scn, mctx, dctx, scn->capturedNodes.pop, scn->capturedNodes.data);

    return err;
#endif
    afxBody bod;
    afxUnit i = 0;
    while (AfxEnumerateBodies(sim, i++, 1, &bod))
    {
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
        afxModel mdl;

        if (AfxGetBodyModel(bod, &mdl))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
            afxUnit jntCnt = AfxCountJoints(mdl, 0);
            afxReal sampleLodErr = 0.0;
            afxUnit sampledJntCnt = AfxCountJoints(mdl, sampleLodErr);

            afxM4d m, m2;
            //AfxComputeModelDisplacement(bod->mdl, m);
            //afxReal64 ct, dt;
            //AfxGetThreadTime(&ct, &dt);
            //AfxUpdateBodyMatrix(bod, dt, FALSE, bod->placement, bod->placement);
            //AfxSampleBodyAnimationsLODSparse(bod, 0, bod->cachedBoneCnt, scn->lp, 0.0, NIL);
            //AfxBuildPlacement(skl, 0, bod->cachedBoneCnt, scn->lp, m, scn->wp);
            //AfxComputeRestPosture(skl, 0, bod->cachedBoneCnt, m, scn->wp);

#if !0
            AsxCmdSampleBodyAnimationsAcceleratedLOD(NIL, bod, jntCnt, bod->placement, scn->lp, scn->wp, 0.0);
#else
            afxM4d mdlDispl;
            AfxComputeModelDisplacement(mdl, mdlDispl);
            AfxSampleBodyAnimationsLODSparse(bod, 0, sampledJntCnt, bod->pose, sampleLodErr, NIL);
            AfxComputePostureLod(skl, 0, jntCnt, 0, sampledJntCnt, bod->pose, mdlDispl, scn->wp);
#endif

            for (afxUnit mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
            {
                afxMesh msh = mdl->rigs[mshIdx].msh;

                if (msh)
                {
                    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

                    afxMeshInfo mshi;
                    AfxDescribeMesh(msh, &mshi);

                    afxMeshMorph mshm;
                    AfxGetMeshMorphes(msh, 0, 1, &mshm);

                    avxVertexCache vtxCache;
                    avxIndexCache idxCache;
                    AfxBufferizeMesh(msh, 0, &vtxCache, &idxCache);
                    
                    AvxCmdBindVertexBuffers(dctx, 0, 2,
                        (afxBuffer[]) { vtxCache.buf, vtxCache.buf },
                        (afxUnit32[]) { vtxCache.streams[0].base, vtxCache.streams[1].base },
                        (afxUnit32[]) { vtxCache.streams[0].range, vtxCache.streams[1].range },
                        (afxUnit32[]) { vtxCache.streams[0].stride, vtxCache.streams[1].stride });
                    

                    AvxCmdApplyDrawTechnique(dctx, scn->lightingDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(dctx, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(dctx, scn->blinnTestRazrDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(dctx, scn->bodyDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdApplyDrawTechnique(dctx, scn->testDtec, 0, scn->testVin, NIL);

#if 0
#if 0
                    if (AfxIsMeshDeformable(msh))
                        AvxCmdApplyDrawTechnique(dctx, scn->bodyDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(dctx, scn->bodyDtec, 0, scn->rigidVin, NIL);
#else
                    if (AfxIsMeshDeformable(msh))
                        AvxCmdApplyDrawTechnique(dctx, scn->tutCamUtilDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdApplyDrawTechnique(dctx, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
#endif
#endif
                    AvxCmdBindIndexBuffer(dctx, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);

                    //AvxCmdSetPrimitiveTopology(dctx, avxTopology_TRI_LIST);
                    //AvxCmdSetCullMode(dctx, NIL);
                    //AvxCmdSwitchFrontFace(dctx, AfxRandom2(0, 1));

                    //AvxCmdEnableDepthTest(dctx, TRUE);
                    //AvxCmdEnableDepthWrite(dctx, TRUE);




                    AvxCmdBindBuffers(dctx, 3, 0, 1, &scn->framesets[scn->frameIdx].objConstantsBuffer, NIL, NIL);

                    //afxUnit const *ToBoneIndices = AfxGetMeshRigBiasToJointMapping(mdl, mshIdx);
                    AfxBuildIndexedCompositeBuffer(mdl, mshIdx, scn->wp, 1, &m);
                    AfxBuildIndexedCompositeBuffer(mdl, mshIdx, scn->wp, mshi.biasCnt, scn->framesets[scn->frameIdx].objConstants.w);

                    //AfxM4dCopyAtm(m, m);
                    //AfxM4dReset(m);
                    AvxCmdUpdateBuffer(dctx, scn->framesets[scn->frameIdx].objConstantsBuffer, 0, sizeof(m), m);
                    AvxCmdUpdateBuffer(dctx, scn->framesets[scn->frameIdx].objConstantsBuffer, 64, mshi.biasCnt * sizeof(m), scn->framesets[scn->frameIdx].objConstants.w);
                    //AvxCmdUpdateBuffer(dctx, scn->framesets[scn->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

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
                        AvxCmdDrawIndexed(dctx, idxCnt, 1, firstIdx, mshm.baseVtx, 0);
                        //AvxCmdDraw(dctx, vtxCnt, 1, 0, 0);
                    }
                }
            }
        }
    }
    return err;
}
#endif

_ASX afxError AsxCmdDrawBodies(afxDrawContext dctx, akxRenderer rnd, afxReal dt, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);
    AFX_ASSERT_OBJECTS(afxFcc_BOD, cnt, bodies);

    for (afxUnit bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        afxBody bod = bodies[bodIdx];
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

        afxModel mdl;
        AfxGetBodyModel(bod, &mdl);
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        afxM4d m, m2;
        //AfxComputeModelDisplacement(bod->mdl, m);
        //afxReal64 ct, dt;
        //AfxGetThreadTime(&ct, &dt);
        //AfxUpdateBodyMatrix(bod, dt, FALSE, bod->placement, bod->placement);
        //AsxCmdSampleBodyAnimationsAcceleratedLOD(sctx, bod, AfxCountJoints(mdl, 0), bod->placement, rnd->lp, rnd->wp, 0.0);
        AsxCmdSampleBodyAnimationsAcceleratedLOD(NIL, bod, AfxCountJoints(mdl, 0), bod->placement, rnd->lp, rnd->wp, 0.0);
        //AfxSampleBodyAnimationsLODSparse(bod, 0, bod->cachedBoneCnt, rnd->lp, 0.0, NIL);
        //AfxBuildPlacement(skl, 0, bod->cachedBoneCnt, rnd->lp, m, rnd->wp);
        //AfxComputeRestPosture(skl, 0, bod->cachedBoneCnt, m, rnd->wp);


        for (afxUnit mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
        {
            afxMeshRigInfo rig;
            AfxDescribeMeshRig(mdl, mshIdx, &rig);

            afxMesh msh = rig.msh;
            if (!msh) continue;
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

            if (msh)
            {
                AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
                afxMeshInfo mshi;
                AfxDescribeMesh(msh, &mshi);

                afxMeshMorph mshm;
                AfxGetMeshMorphes(msh, 0, 1, &mshm);

                avxVertexCache vtxCache;
                avxIndexCache idxCache;
                AfxBufferizeMesh(msh, 0, &vtxCache, &idxCache);

                //AvxCmdApplyDrawTechnique(dctx, rnd->lightingDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdApplyDrawTechnique(dctx, rnd->tutCamUtilDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdApplyDrawTechnique(dctx, rnd->blinnTestRazrDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdApplyDrawTechnique(dctx, rnd->bodyDtec, 0, rnd->rigidVin, NIL);
                AvxCmdApplyDrawTechnique(dctx, rnd->testDtec, 0, rnd->testVin, NIL);

                AvxCmdBindIndexBuffer(dctx, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);
                
                //AvxCmdSetPrimitiveTopology(dctx, avxTopology_TRI_LIST);
                //AvxCmdSetCullMode(dctx, NIL);
                //AvxCmdSwitchFrontFace(dctx, AfxRandom2(0, 1));


                //AvxCmdEnableDepthTest(dctx, TRUE);
                //AvxCmdEnableDepthWrite(dctx, TRUE);

                AvxCmdBindBuffers(dctx, 3, 0, 1, (afxBufferMap[]) { {.buf = rnd->framesets[rnd->frameIdx].objUbo } });

                AfxBuildRiggedMeshCompositeMatrices(mdl, mshIdx, rnd->wp, 1, &m);
                //AfxM4dCopyAtm(m, m);
                //AfxM4dReset(m);
                AvxCmdUpdateBuffer(dctx, rnd->framesets[rnd->frameIdx].objUbo, 0, sizeof(m), m);
                AvxCmdUpdateBuffer(dctx, rnd->framesets[rnd->frameIdx].objUbo, 64, sizeof(m), m);
                //AvxCmdUpdateBuffer(dctx, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

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
                            AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
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
                    AvxCmdDrawIndexed(dctx, idxCnt, 1, firstIdx, mshm.baseVtx, 0);
                    //AvxCmdDraw(dctx, vtxCnt, 1, 0, 0);
                }
                //AvxCmdDrawIndexed(dctx, msht->triCnt * 3, 1, 0, 0, 0);

                //AvxCmdDrawIndexed(dctx, msht->triCnt * 3, 1, 0, 0, 0);
            }
        }
    }
    return err;
}

_ASX afxError AsxCmdDrawTestIndexed(afxDrawContext dctx, akxRenderer rnd)
{
    AvxCmdApplyDrawTechnique(dctx, rnd->testDtec, 0, rnd->testVin, NIL);

    avxBufferedStream vtxSrc;
    vtxSrc.buf = rnd->testVbo;
    vtxSrc.offset = 0;
    vtxSrc.range = 0;
    vtxSrc.stride = sizeof(afxV3d);
    AvxCmdBindVertexBuffers(dctx, 0, 1, &vtxSrc);
    //AvxCmdResetVertexStreams(dctx, 1, NIL, (afxUnit32[]) { sizeof(afxV3d) }, NIL);
    //AvxCmdResetVertexAttributes(dctx, 1, NIL, (afxVertexFormat[]) { afxVertexFormat_V3D }, NIL, NIL);
    AvxCmdBindIndexBuffer(dctx, rnd->testIbo, 0, sizeof(afxUnit32) * 6, sizeof(afxUnit32));
    AvxCmdDrawIndexed(dctx, 6, 1, 0, 0, 0);
    //AvxCmdDraw(dctx, 6, 1, 0, 0);
    return 0;
}

_ASX afxError AfxRendererSetStar(akxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);

    akxViewConstants *viewConstants = &rnd->framesets[rnd->frameIdx].viewConstants;

    AfxV4dCopy(viewConstants->starPos, pos);
    AfxV4dCopy(viewConstants->starDir, dir);
    AfxV4dCopy(viewConstants->starKd, Kd);

    return err;
}

_ASX afxError AsxEndSceneRendering(afxDrawContext dctx, akxRenderer rnd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);

    return err;
}

_ASX afxError AsxBeginSceneRendering(afxDrawContext dctx, akxRenderer rnd, afxCamera cam, afxRect const* drawArea, avxCanvas canv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);

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

    AvxCmdUpdateBuffer(dctx, rnd->framesets[frameIdx].viewUbo, 0, sizeof(*viewConstants), viewConstants);
    AvxCmdBindBuffers(dctx, 0, 0, 1, (afxBufferMap[]) { {.buf = rnd->framesets[frameIdx].viewUbo } });
    return err;
}

_ASX afxError _AfxRndDtor(akxRenderer rnd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);

    for (afxUnit i = 0; i < rnd->frameCnt; i++)
    {
        AfxDisposeObjects(1, &rnd->framesets[i].viewUbo);
        AfxDisposeObjects(1, &rnd->framesets[i].shdUbo);
        AfxDisposeObjects(1, &rnd->framesets[i].mtlUbo);
        AfxDisposeObjects(1, &rnd->framesets[i].objUbo);

    }

    AfxDisposeObjects(1, &rnd->testVin);
    AfxDisposeObjects(1, &rnd->rigidVin);
    AfxDisposeObjects(1, &rnd->skinnedVin);

    AfxDisposeObjects(1, &rnd->blinnTestRazrDtec);
    AfxDisposeObjects(1, &rnd->bodyDtec);
    AfxDisposeObjects(1, &rnd->lightingDtec);
    AfxDisposeObjects(1, &rnd->testDtec);
    AfxDisposeObjects(1, &rnd->tutCamUtilDtec);

    afxDrawInput din = rnd->din;

    AfxDisposeObjects(1, &din);
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

_ASX afxError _AfxRndCtor(akxRenderer rnd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);

    akxRendererConfig const* config = ((akxRendererConfig const*)args[0]) + invokeNo;

    afxSimulation sim = AfxGetProvider(rnd);
    rnd->cachedSim = sim;
    afxDrawSystem dsys = sim->dsys;
    rnd->cachedDsys = dsys;
    
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

        AfxOpenDrawInput(dsys, &dinConfig, &din);
        AFX_ASSERT(din);
    }
    else
    {
        AfxReacquireObjects(1, &din);
    }
    rnd->din = din;

    afxUri uri;
    avxVertexDecl vin;

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
        vboSpec.cap = sizeof(testVertices);
        vboSpec.flags = afxBufferFlag_W;
        vboSpec.usage = afxBufferUsage_VERTEX;
        AfxAcquireBuffers(dsys, 1, &vboSpec, &rnd->testVbo);
        
        afxBufferIo vboIop = { 0 };
        vboIop.dstStride = 1;
        vboIop.srcStride = 1;
        vboIop.rowCnt = vboSpec.cap;
        AfxUpdateBuffer(rnd->testVbo, 1, &vboIop, 0, testVertices);

        afxBufferInfo iboSpec = { 0 };
        iboSpec.cap = sizeof(testIndices);
        iboSpec.flags = afxBufferFlag_W;
        iboSpec.usage = afxBufferUsage_INDEX;
        AfxAcquireBuffers(dsys, 1, &iboSpec, &rnd->testIbo);

        afxBufferIo iboIop = { 0 };
        iboIop.dstStride = 1;
        iboIop.srcStride = 1;
        iboIop.rowCnt = iboSpec.cap;
        AfxUpdateBuffer(rnd->testIbo, 1, &iboIop, 0, testIndices);

        avxVertexFetch const vinStreams[] =
        {
            {
                .instanceRate = 0,
                .srcIdx = 0
            }
        };
        avxVertexInput const vinAttrs[] =
        {
            {
                .location = 0,
                .srcIdx = 0,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            }
        };

        AfxDeclareVertexLayout(dsys, ARRAY_SIZE(vinStreams), vinStreams, ARRAY_SIZE(vinAttrs), vinAttrs, &vin);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
        rnd->testVin = vin;

        AfxMakeUri(&uri, 0, "../data/pipeline/test/test.xsh.xml", 0);
        AfxLoadDrawTechnique(din, &uri, &rnd->testDtec);
    }

    avxVertexFetch const vinStreams[] =
    {
        {
            .instanceRate = 0,
            .srcIdx = 0
        },
        {
            .instanceRate = 0,
            .srcIdx = 1
        }
    };
    avxVertexInput const vinAttrs[] =
    {
        {
            .location = 0,
            .srcIdx = 0,
            .offset = 0,
            .fmt = avxFormat_RGB32f
        },
        {
            .location = 1,
            .srcIdx = 0,
            .offset = 12,
            .fmt = avxFormat_RGBA8un
        },
        {
            .location = 2,
            .srcIdx = 0,
            .offset = 16,
            .fmt = avxFormat_RGBA32f
        },
        {
            .location = 3,
            .srcIdx = 1,
            .offset = 0,
            .fmt = avxFormat_RGB32f
        },
        {
            .location = 6,
            .srcIdx = 1,
            .offset = 12,
            .fmt = avxFormat_RG32f
        }
    };

    avxVertexDecl skinnedVin;
    AfxDeclareVertexLayout(dsys, ARRAY_SIZE(vinStreams), vinStreams, ARRAY_SIZE(vinAttrs), vinAttrs, &skinnedVin);
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &skinnedVin);
    rnd->skinnedVin = skinnedVin;

    avxVertexFetch const vinStreams2[] =
    {
        {
            .instanceRate = 0,
            .srcIdx = 0
        },
        {
            .instanceRate = 0,
            .srcIdx = 1
        }
    };
    avxVertexInput const vinAttrs2[] =
    {
        {
            .location = 0,
            .srcIdx = 0,
            .offset = 0,
            .fmt = avxFormat_RGB32f
        },
        {
            .location = 3,
            .srcIdx = 1,
            .offset = 0,
            .fmt = avxFormat_RGB32f
        },
        {
            .location = 6,
            .srcIdx = 1,
            .offset = 12,
            .fmt = avxFormat_RG32f
        }
    };

    avxVertexDecl rigidVin;
    AfxDeclareVertexLayout(dsys, ARRAY_SIZE(vinStreams2), vinStreams2, ARRAY_SIZE(vinAttrs2), vinAttrs2, &rigidVin);
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &rigidVin);
    rnd->rigidVin = rigidVin;

    {
        avxVertexFetch const vinStreams3[] =
        {
            {
                .instanceRate = 0,
                .srcIdx = 0
            },
            {
                .instanceRate = 0,
                .srcIdx = 1
            },
            {
                .instanceRate = 1,
                .srcIdx = 2
            }
        };

        avxVertexInput const vinAttrs3[] =
        {
            // per-vertex fetch
            {
                .location = 0,
                .srcIdx = 0,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 3,
                .srcIdx = 1,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 6,
                .srcIdx = 1,
                .offset = 12,
                .fmt = avxFormat_RG32f
            },
            // per-instance fetch
            {
                .location = 10,
                .srcIdx = 2,
                .offset = 0,
                .fmt = avxFormat_R32u
            },
            {
                .location = 11,
                .srcIdx = 2,
                .offset = 4,
                .fmt = avxFormat_R32u
            },
            {
                .location = 12,
                .srcIdx = 2,
                .offset = 8,
                .fmt = avxFormat_R32u
            },
            {
                .location = 13,
                .srcIdx = 2,
                .offset = 12,
                .fmt = avxFormat_R32u
            }
        };

        avxVertexDecl vin_p3n3t2_bi_mtl_mtx_jnt;
        AfxDeclareVertexLayout(dsys, ARRAY_SIZE(vinStreams3), vinStreams3, ARRAY_SIZE(vinAttrs3), vinAttrs3, &vin_p3n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3n3t2_bi_mtl_mtx_jnt = vin_p3n3t2_bi_mtl_mtx_jnt;

        avxVertexInput const vinAttrs33[] =
        {
            // per-vertex fetch
            {
                .location = 0,
                .srcIdx = 0,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 1,
                .srcIdx = 0,
                .offset = 12,
                .fmt = avxFormat_R8u
            },
            {
                .location = 3,
                .srcIdx = 1,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 6,
                .srcIdx = 1,
                .offset = 12,
                .fmt = avxFormat_RG32f
            },
            // per-instance fetch
            {
                .location = 10,
                .srcIdx = 2,
                .offset = 0,
                .fmt = avxFormat_R32u
            },
            {
                .location = 11,
                .srcIdx = 2,
                .offset = 4,
                .fmt = avxFormat_R32u
            },
            {
                .location = 12,
                .srcIdx = 2,
                .offset = 8,
                .fmt = avxFormat_R32u
            },
            {
                .location = 13,
                .srcIdx = 2,
                .offset = 12,
                .fmt = avxFormat_R32u
            }
        };

        avxVertexDecl vin_p3j1n3t2_bi_mtl_mtx_jnt;
        AfxDeclareVertexLayout(dsys, ARRAY_SIZE(vinStreams3), vinStreams3, ARRAY_SIZE(vinAttrs33), vinAttrs33, &vin_p3j1n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3j1n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3j1n3t2_bi_mtl_mtx_jnt = vin_p3j1n3t2_bi_mtl_mtx_jnt;

        avxVertexInput const vinAttrs333[] =
        {
            // per-vertex fetch
            {
                .location = 0,
                .srcIdx = 0,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 1,
                .srcIdx = 0,
                .offset = 12,
                .fmt = avxFormat_RG32f
            },
            {
                .location = 2,
                .srcIdx = 0,
                .offset = 20,
                .fmt = avxFormat_RG8u
            },
            {
                .location = 3,
                .srcIdx = 1,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 6,
                .srcIdx = 1,
                .offset = 12,
                .fmt = avxFormat_RG32f
            },
            // per-instance fetch
            {
                .location = 10,
                .srcIdx = 2,
                .offset = 0,
                .fmt = avxFormat_R32u
            },
            {
                .location = 11,
                .srcIdx = 2,
                .offset = 4,
                .fmt = avxFormat_R32u
            },
            {
                .location = 12,
                .srcIdx = 2,
                .offset = 8,
                .fmt = avxFormat_R32u
            },
            {
                .location = 13,
                .srcIdx = 2,
                .offset = 12,
                .fmt = avxFormat_R32u
            }
        };

        avxVertexDecl vin_p3j2n3t2_bi_mtl_mtx_jnt;
        AfxDeclareVertexLayout(dsys, ARRAY_SIZE(vinStreams3), vinStreams3, ARRAY_SIZE(vinAttrs333), vinAttrs333, &vin_p3j2n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3j2n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3j2n3t2_bi_mtl_mtx_jnt = vin_p3j2n3t2_bi_mtl_mtx_jnt;

        avxVertexInput const vinAttrs3333[] =
        {
            // per-vertex fetch
            {
                .location = 0,
                .srcIdx = 0,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 1,
                .srcIdx = 0,
                .offset = 12,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 2,
                .srcIdx = 0,
                .offset = 24,
                .fmt = avxFormat_RGB8u
            },
            {
                .location = 3,
                .srcIdx = 1,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 6,
                .srcIdx = 1,
                .offset = 12,
                .fmt = avxFormat_RG32f
            },
            // per-instance fetch
            {
                .location = 10,
                .srcIdx = 2,
                .offset = 0,
                .fmt = avxFormat_R32u
            },
            {
                .location = 11,
                .srcIdx = 2,
                .offset = 4,
                .fmt = avxFormat_R32u
            },
            {
                .location = 12,
                .srcIdx = 2,
                .offset = 8,
                .fmt = avxFormat_R32u
            },
            {
                .location = 13,
                .srcIdx = 2,
                .offset = 12,
                .fmt = avxFormat_R32u
            }
        };

        avxVertexDecl vin_p3j3n3t2_bi_mtl_mtx_jnt;
        AfxDeclareVertexLayout(dsys, ARRAY_SIZE(vinStreams3), vinStreams3, ARRAY_SIZE(vinAttrs3333), vinAttrs3333, &vin_p3j3n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3j3n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3j3n3t2_bi_mtl_mtx_jnt = vin_p3j3n3t2_bi_mtl_mtx_jnt;

        avxVertexInput const vinAttrs33333[] =
        {
            // per-vertex fetch
            {
                .location = 0,
                .srcIdx = 0,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 1,
                .srcIdx = 0,
                .offset = 12,
                .fmt = avxFormat_RGBA32f
            },
            {
                .location = 2,
                .srcIdx = 0,
                .offset = 28,
                .fmt = avxFormat_RGBA8u
            },
            {
                .location = 3,
                .srcIdx = 1,
                .offset = 0,
                .fmt = avxFormat_RGB32f
            },
            {
                .location = 6,
                .srcIdx = 1,
                .offset = 12,
                .fmt = avxFormat_RG32f
            },
            // per-instance fetch
            {
                .location = 10,
                .srcIdx = 2,
                .offset = 0,
                .fmt = avxFormat_R32u
            },
            {
                .location = 11,
                .srcIdx = 2,
                .offset = 4,
                .fmt = avxFormat_R32u
            },
            {
                .location = 12,
                .srcIdx = 2,
                .offset = 8,
                .fmt = avxFormat_R32u
            },
            {
                .location = 13,
                .srcIdx = 2,
                .offset = 12,
                .fmt = avxFormat_R32u
            }
        };

        avxVertexDecl vin_p3j4n3t2_bi_mtl_mtx_jnt;
        AfxDeclareVertexLayout(dsys, ARRAY_SIZE(vinStreams3), vinStreams3, ARRAY_SIZE(vinAttrs33333), vinAttrs33333, &vin_p3j4n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3j4n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3j4n3t2_bi_mtl_mtx_jnt = vin_p3j4n3t2_bi_mtl_mtx_jnt;

    }
    {
        afxBufferInfo bufSpec[] =
        {
            { .cap = sizeof(akxViewConstants), .flags = afxBufferFlag_W | afxBufferFlag_X | afxBufferFlag_COHERENT, .usage = afxBufferUsage_UNIFORM },
            { .cap = sizeof(akxShaderConstants), .flags = afxBufferFlag_W | afxBufferFlag_X | afxBufferFlag_COHERENT, .usage = afxBufferUsage_UNIFORM },
            { .cap = sizeof(akxMaterialConstants), .flags = afxBufferFlag_W | afxBufferFlag_X | afxBufferFlag_COHERENT, .usage = afxBufferUsage_UNIFORM },
            { .cap = sizeof(akxInstanceConstants), .flags = afxBufferFlag_W | afxBufferFlag_X | afxBufferFlag_COHERENT, .usage = afxBufferUsage_UNIFORM }
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
            //AfxBuildRasters(dsys, 1, &rnd->framesets[i].depthSurf, &depthSurfB);

            AfxAcquireBuffers(dsys, 1, &bufSpec[0], &rnd->framesets[i].viewUbo);
            AfxAcquireBuffers(dsys, 1, &bufSpec[1], &rnd->framesets[i].shdUbo);
            AfxAcquireBuffers(dsys, 1, &bufSpec[2], &rnd->framesets[i].mtlUbo);
            AfxAcquireBuffers(dsys, 1, &bufSpec[3], &rnd->framesets[i].objUbo);

            afxBufferInfo bufi = { 0 };
            bufi.cap = AVX_BUF_UPDATE_CAPACITY;
            bufi.usage = afxBufferUsage_INDIRECT | afxBufferUsage_VERTEX;
            bufi.flags = afxBufferFlag_W | afxBufferFlag_X | afxBufferFlag_COHERENT;
            AfxAcquireBuffers(dsys, 1, &bufi, &rnd->framesets[i].icbo);

            bufi.cap = AVX_BUF_UPDATE_CAPACITY;
            bufi.usage = afxBufferUsage_FETCH;
            bufi.flags = afxBufferFlag_W | afxBufferFlag_X | afxBufferFlag_COHERENT;
            bufi.fmt = avxFormat_R32u;
            AfxAcquireBuffers(dsys, 1, &bufi, &rnd->framesets[i].biasMapUbo);


            afxByte* icboPtr = NIL;
            afxByte* mtboPtr = NIL;
            afxByte* bmboPtr = NIL;
            afxByte* mtlboPtr = NIL;
            afxBufferRemap maps[] =
            {
                {
                    .buf = rnd->framesets[i].objUbo,
                    .offset = 0,
                    .range = AfxGetBufferCapacity(rnd->framesets[i].objUbo, 0),
                    .placeholder = (void**)&mtboPtr
                },
                {
                    .buf = rnd->framesets[i].biasMapUbo,
                    .offset = 0,
                    .range = AfxGetBufferCapacity(rnd->framesets[i].biasMapUbo, 0),
                    .placeholder = (void**)&bmboPtr
                },
                {
                    .buf = rnd->framesets[i].mtlUbo,
                    .offset = 0,
                    .range = AfxGetBufferCapacity(rnd->framesets[i].mtlUbo, 0),
                    .placeholder = (void**)&mtlboPtr
                },
                {
                    .buf = rnd->framesets[i].icbo,
                    .offset = 0,
                    .range = AfxGetBufferCapacity(rnd->framesets[i].icbo, 0),
                    .placeholder = (void**)&icboPtr
                }
            };

            if (AfxMapBuffers(dsys, ARRAY_SIZE(maps), maps))
                AfxThrowError();

            rnd->framesets[i].mtboPtr = mtboPtr;
            rnd->framesets[i].bmboPtr = bmboPtr;
            rnd->framesets[i].mtlboPtr = mtlboPtr;
            rnd->framesets[i].icboPtr = icboPtr;

            AfxAcquireFences(dsys, TRUE, 1, &rnd->framesets[i].drawCompletedFence);
        }

        //AfxTextureBlueprintEnd(&depthSurfB, 0, NIL);
    }

    {
        AfxMakeUri(&uri, 0, "../data/pipeline/body/body.xsh.xml", 0);
        AfxLoadDrawTechnique(din, &uri, &rnd->bodyDtec);
    }

    AfxRendererSetStar(rnd, AFX_V4D(0, 0, 0, 1), AFX_V3D(-0.8660f, 0.5f, 0), AFX_V4D(0.8, 0.8, 0.8, 0.8));
    // ambient 0.2, 0.2, 0.2, 0.2

    AfxAcquirePoses(sim, 1, (afxUnit[]) { 255 }, &rnd->lp);
    AfxAcquirePlacements(sim, 1, (afxUnit[]) { 255 }, (afxBool[]) {TRUE}, &rnd->wp);

    AfxMakeUri(&uri, 0, "../data/pipeline/testLighting/testLighting.xsh.xml", 0);
    AfxLoadDrawTechnique(din, &uri, &rnd->blinnTestRazrDtec);

    AfxMakeUri(&uri, 0, "../data/pipeline/tutCamUtil/tutCamUtil.xsh.xml", 0);
    AfxLoadDrawTechnique(din, &uri, &rnd->tutCamUtilDtec);

    AfxMakeUri(&uri, 0, "../data/pipeline/lighting/lighting.xsh.xml", 0);
    AfxLoadDrawTechnique(din, &uri, &rnd->lightingDtec);

    AfxMakeArray(&rnd->capturedNodes, sizeof(afxBody), 100, NIL, 0);

    return err;
}

_ASX afxClassConfig const _ASX_RND_CLASS_CONFIG =
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

_ASX afxError AsxAcquireRenderers(afxSimulation sim, afxUnit cnt, akxRenderer rnd[], akxRendererConfig const config[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    if (AfxAcquireObjects(_AsxGetRendererClass(sim), cnt, (afxObject*)rnd, (void const*[]) { (void*)config }))
        AfxThrowError();

    return err;
}
