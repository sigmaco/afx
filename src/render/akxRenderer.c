/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_SIM_C
#define _ASX_SIMULATION_C
#define _ARX_MODEL_C
#define _ARX_BODY_C
#define _ARX_MATERIAL_C
#define _ARX_VERTEX_BUFFER_C
#include "../draw/ddi/avxImplementation.h"
#include "ddi/arxImpl_Input.h"
#include "qwadro/inc/sim/afxSimulation.h"

#if 0
_ARX afxBool _AsxCaptureBodCb(arxBody bod, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

    afxFrustum* frustum = udd[0];
    afxReal lodErr = *(afxReal*)udd[1];
    afxArray* pvs = udd[2];

    //AfxDoesFrustumCullAabbs(&frustum, 1, &aabb);

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

_ARX afxError AsxCaptureBodyPvs(afxSimulation sim, arxCamera cam, afxReal lodErr, afxArray* pvs)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxFrustum frustum;
    AvxGetCameraFrustum(cam, &frustum);

    AfxInvokeBodies(sim, 0, AFX_U32_MAX, (void*)_AsxCaptureBodCb, (void*[]) { &frustum, &lodErr, pvs });

    return err;
}
#endif

_ARX afxError ArxDrawBodies(akxRenderer scn, afxContext sctx, afxDrawContext dctx, afxUnit cnt, arxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &scn);

    for (afxUnit bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        arxBody bod = bodies[bodIdx];
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
        arxModel mdl;

        if (!ArxGetBodyModel(bod, &mdl))
            continue;

        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
        afxUnit jntCnt = ArxCountJoints(mdl, 0);
        afxReal sampleLodErr = 0.0;
        afxUnit sampledJntCnt = ArxCountJoints(mdl, sampleLodErr);

        afxM4d m, m2;
        //AfxComputeModelDisplacement(bod->mdl, m);
        //afxReal64 ct, dt;
        //AfxGetThreadTime(&ct, &dt);
        //ArxComputeBodyMotionMatrix(bod, dt, FALSE, bod->placement, bod->placement);
        //AfxSampleBodyAnimations(bod, 0, bod->cachedBoneCnt, scn->lp, 0.0, NIL);
        //ArxBuildPlacement(skl, 0, bod->cachedBoneCnt, scn->lp, m, scn->wp);
        //AfxComputeRestPosture(skl, 0, bod->cachedBoneCnt, m, scn->wp);

        arxPlacement bodPlce;
        ArxGetBodyPose(bod, NIL, &bodPlce);
#if !0
        AsxCmdSampleBodyAnimationsAcceleratedLOD(sctx, bod, jntCnt, AFX_M4D_IDENTITY, scn->lp, scn->wp[scn->frameIdx], 0.0);
        //AfxSampleBodyAnimationsAccelerated(bod, jntCnt, bod->placement, scn->lp, scn->wp, 0.0);
#else
        afxM4d mdlDispl;
        AfxComputeModelDisplacement(mdl, mdlDispl);
        AfxSampleBodyAnimations(bod, 0, sampledJntCnt, bod->pose, sampleLodErr, NIL);
        AfxComputePostureLod(skl, 0, jntCnt, 0, sampledJntCnt, bod->pose, mdlDispl, scn->wp);
#endif

        for (afxUnit mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
        {
            arxMeshRigInfo rig;
            ArxDescribeMeshRig(mdl, mshIdx, &rig);
            
            arxMesh msh = rig.msh;
            if (!msh) continue;
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
            
            arxMeshInfo mshi;
            ArxDescribeMesh(msh, &mshi);

            arxMeshMorph mshm;
            ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

            arxVertexCache vtxCache;
            arxIndexCache idxCache;
            ArxBufferizeMesh(msh, 0, &vtxCache, &idxCache);

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

            AvxCmdUseDrawTechniqueSIGMA(dctx, scn->lightingDtec, 0, scn->rigidVin, NIL);
            //AvxCmdUseDrawTechniqueSIGMA(dctx, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
            //AvxCmdUseDrawTechniqueSIGMA(dctx, scn->blinnTestRazrDtec, 0, scn->rigidVin, NIL);
            //AvxCmdUseDrawTechniqueSIGMA(dctx, scn->bodyDtec, 0, scn->rigidVin, NIL);
            //AvxCmdUseDrawTechniqueSIGMA(dctx, scn->testDtec, 0, scn->testVin, NIL);

#if 0
#if 0
            if (AfxIsMeshDeformable(msh))
                AvxCmdUseDrawTechniqueSIGMA(dctx, scn->bodyDtec, 1, scn->skinnedVin, NIL);
            else
                AvxCmdUseDrawTechniqueSIGMA(dctx, scn->bodyDtec, 0, scn->rigidVin, NIL);
#else
            if (AfxIsMeshDeformable(msh))
                AvxCmdUseDrawTechniqueSIGMA(dctx, scn->tutCamUtilDtec, 1, scn->skinnedVin, NIL);
            else
                AvxCmdUseDrawTechniqueSIGMA(dctx, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
#endif
#endif

            AvxCmdBindIndexBuffer(dctx, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);

            //AvxCmdSetPrimitiveTopology(dctx, avxTopology_TRI_LIST);
            //AvxCmdSetCullMode(dctx, NIL);
            //AvxCmdSwitchFrontFace(dctx, AfxRandom2(0, 1));

            //AvxCmdSwitchDepthTesting(dctx, TRUE);
            //AvxCmdEnableDepthWrite(dctx, TRUE);

            AvxCmdBindBuffers(dctx, 3, 30, 1, (avxBufferedMap[]) { {.buf = scn->framesets[scn->frameIdx].objUbo} });

            //afxUnit const *ToBoneIndices = AfxGetMeshRigBiasToJointMapping(mdl, mshIdx);
            ArxBuildRiggedMeshCompositeMatrices(mdl, mshIdx, scn->wp[scn->frameIdx], 1, &m);
            ArxBuildRiggedMeshCompositeMatrices(mdl, mshIdx, scn->wp[scn->frameIdx], mshi.biasCnt, scn->framesets[scn->frameIdx].objConstants.m);

            //AfxM4dCopyAtm(m, m);
            //AfxM4dReset(m);
            AvxCmdUpdateBuffer(dctx, scn->framesets[scn->frameIdx].objUbo, 0, sizeof(m), m);
            AvxCmdUpdateBuffer(dctx, scn->framesets[scn->frameIdx].objUbo, 0, mshi.biasCnt * sizeof(m), scn->framesets[scn->frameIdx].objConstants.m);
            //AvxCmdUpdateBuffer(dctx, scn->framesets[scn->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

            afxUnit surfCnt = mshi.secCnt;

            for (afxUnit j = 0; j < surfCnt; j++)
            {
                arxMeshSection sec;
                ArxDescribeMeshSections(msh, j, 1, &sec);

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
_ARX afxError ArxBeginSceneCapture(akxRenderer scn, arxCamera cam, afxSimulation sim, afxDrawContext dctx, afxContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &scn);
    AFX_ASSERT_OBJECTS(afxFcc_CAM, 1, &cam);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
#if !0
    AfxEmptyArray(&scn->capturedNodes);
    AsxCaptureBodyPvs(sim, cam, 0, &scn->capturedNodes);

    ArxDrawBodies(scn, sctx, dctx, scn->capturedNodes.pop, scn->capturedNodes.data);

    return err;
#endif
    arxBody bod;
    afxUnit i = 0;
    while (AfxEnumerateBodies(sim, i++, 1, &bod))
    {
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);
        arxModel mdl;

        if (ArxGetBodyModel(bod, &mdl))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
            afxUnit jntCnt = ArxCountJoints(mdl, 0);
            afxReal sampleLodErr = 0.0;
            afxUnit sampledJntCnt = ArxCountJoints(mdl, sampleLodErr);

            afxM4d m, m2;
            //AfxComputeModelDisplacement(bod->mdl, m);
            //afxReal64 ct, dt;
            //AfxGetThreadTime(&ct, &dt);
            //ArxComputeBodyMotionMatrix(bod, dt, FALSE, bod->placement, bod->placement);
            //AfxSampleBodyAnimations(bod, 0, bod->cachedBoneCnt, scn->lp, 0.0, NIL);
            //ArxBuildPlacement(skl, 0, bod->cachedBoneCnt, scn->lp, m, scn->wp);
            //AfxComputeRestPosture(skl, 0, bod->cachedBoneCnt, m, scn->wp);

#if !0
            AmxCmdSampleBodyAnimationsAcceleratedLOD(NIL, bod, jntCnt, bod->placement, scn->lp, scn->wp, 0.0);
#else
            afxM4d mdlDispl;
            AfxComputeModelDisplacement(mdl, mdlDispl);
            AfxSampleBodyAnimations(bod, 0, sampledJntCnt, bod->pose, sampleLodErr, NIL);
            AfxComputePostureLod(skl, 0, jntCnt, 0, sampledJntCnt, bod->pose, mdlDispl, scn->wp);
#endif

            for (afxUnit mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
            {
                arxMesh msh = mdl->rigs[mshIdx].msh;

                if (msh)
                {
                    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

                    arxMeshInfo mshi;
                    ArxDescribeMesh(msh, &mshi);

                    arxMeshMorph mshm;
                    ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

                    arxVertexCache vtxCache;
                    arxIndexCache idxCache;
                    ArxBufferizeMesh(msh, 0, &vtxCache, &idxCache);
                    
                    AvxCmdBindVertexBuffers(dctx, 0, 2,
                        (avxBuffer[]) { vtxCache.buf, vtxCache.buf },
                        (afxUnit32[]) { vtxCache.streams[0].base, vtxCache.streams[1].base },
                        (afxUnit32[]) { vtxCache.streams[0].range, vtxCache.streams[1].range },
                        (afxUnit32[]) { vtxCache.streams[0].stride, vtxCache.streams[1].stride });
                    

                    AvxCmdUseDrawTechniqueSIGMA(dctx, scn->lightingDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdUseDrawTechniqueSIGMA(dctx, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdUseDrawTechniqueSIGMA(dctx, scn->blinnTestRazrDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdUseDrawTechniqueSIGMA(dctx, scn->bodyDtec, 0, scn->rigidVin, NIL);
                    //AvxCmdUseDrawTechniqueSIGMA(dctx, scn->testDtec, 0, scn->testVin, NIL);

#if 0
#if 0
                    if (AfxIsMeshDeformable(msh))
                        AvxCmdUseDrawTechniqueSIGMA(dctx, scn->bodyDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdUseDrawTechniqueSIGMA(dctx, scn->bodyDtec, 0, scn->rigidVin, NIL);
#else
                    if (AfxIsMeshDeformable(msh))
                        AvxCmdUseDrawTechniqueSIGMA(dctx, scn->tutCamUtilDtec, 1, scn->skinnedVin, NIL);
                    else
                        AvxCmdUseDrawTechniqueSIGMA(dctx, scn->tutCamUtilDtec, 0, scn->rigidVin, NIL);
#endif
#endif
                    AvxCmdBindIndexBuffer(dctx, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);

                    //AvxCmdSetPrimitiveTopology(dctx, avxTopology_TRI_LIST);
                    //AvxCmdSetCullMode(dctx, NIL);
                    //AvxCmdSwitchFrontFace(dctx, AfxRandom2(0, 1));

                    //AvxCmdSwitchDepthTesting(dctx, TRUE);
                    //AvxCmdEnableDepthWrite(dctx, TRUE);




                    AvxCmdBindBuffers(dctx, 3, 30, 1, &scn->framesets[scn->frameIdx].objConstantsBuffer, NIL, NIL);

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
                        arxMeshSection sec;
                        ArxDescribeMeshSections(msh, j, 1, &sec);

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

_ARX afxError ArxCmdDrawBodies(afxDrawContext dctx, akxRenderer rnd, afxReal dt, afxUnit cnt, arxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);
    AFX_ASSERT_OBJECTS(afxFcc_BOD, cnt, bodies);

    for (afxUnit bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        arxBody bod = bodies[bodIdx];
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

        arxModel mdl;
        ArxGetBodyModel(bod, &mdl);
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        arxPlacement bodPlce;
        ArxGetBodyPose(bod, NIL, &bodPlce);

        afxM4d m, m2;
        //AfxComputeModelDisplacement(bod->mdl, m);
        //afxReal64 ct, dt;
        //AfxGetThreadTime(&ct, &dt);
        //ArxComputeBodyMotionMatrix(bod, dt, FALSE, bod->placement, bod->placement);
        //AmxCmdSampleBodyAnimationsAcceleratedLOD(sctx, bod, ArxCountJoints(mdl, 0), bod->placement, rnd->lp, rnd->wp, 0.0);
        AsxCmdSampleBodyAnimationsAcceleratedLOD(NIL, bod, ArxCountJoints(mdl, 0), AFX_M4D_IDENTITY, rnd->lp, rnd->wp[rnd->frameIdx], 0.0);
        //AfxSampleBodyAnimations(bod, 0, bod->cachedBoneCnt, rnd->lp, 0.0, NIL);
        //ArxBuildPlacement(skl, 0, bod->cachedBoneCnt, rnd->lp, m, rnd->wp);
        //AfxComputeRestPosture(skl, 0, bod->cachedBoneCnt, m, rnd->wp);


        for (afxUnit mshIdx = 0; mshIdx < mdl->rigCnt; mshIdx++)
        {
            arxMeshRigInfo rig;
            ArxDescribeMeshRig(mdl, mshIdx, &rig);

            arxMesh msh = rig.msh;
            if (!msh) continue;
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

            if (msh)
            {
                AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
                arxMeshInfo mshi;
                ArxDescribeMesh(msh, &mshi);

                arxMeshMorph mshm;
                ArxDescribeMeshMorphes(msh, 0, 1, &mshm);

                arxVertexCache vtxCache;
                arxIndexCache idxCache;
                ArxBufferizeMesh(msh, 0, &vtxCache, &idxCache);

                //AvxCmdUseDrawTechniqueSIGMA(dctx, rnd->lightingDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdUseDrawTechniqueSIGMA(dctx, rnd->tutCamUtilDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdUseDrawTechniqueSIGMA(dctx, rnd->blinnTestRazrDtec, 0, rnd->rigidVin, NIL);
                //AvxCmdUseDrawTechniqueSIGMA(dctx, rnd->bodyDtec, 0, rnd->rigidVin, NIL);
                AvxCmdUseDrawTechniqueSIGMA(dctx, rnd->testDtec, 0, rnd->testVin, NIL);

                AvxCmdBindIndexBuffer(dctx, idxCache.buf, idxCache.base, idxCache.range, idxCache.stride);
                
                //AvxCmdSetPrimitiveTopology(dctx, avxTopology_TRI_LIST);
                //AvxCmdSetCullMode(dctx, NIL);
                //AvxCmdSwitchFrontFace(dctx, AfxRandom2(0, 1));


                //AvxCmdSwitchDepthTesting(dctx, TRUE);
                //AvxCmdEnableDepthWrite(dctx, TRUE);

                AvxCmdBindBuffers(dctx, 3, 30, 1, (avxBufferedMap[]) { {.buf = rnd->framesets[rnd->frameIdx].objUbo } });

                ArxBuildRiggedMeshCompositeMatrices(mdl, mshIdx, rnd->wp[rnd->frameIdx], 1, &m);
                //AfxM4dCopyAtm(m, m);
                //AfxM4dReset(m);
                AvxCmdUpdateBuffer(dctx, rnd->framesets[rnd->frameIdx].objUbo, 0, sizeof(m), m);
                AvxCmdUpdateBuffer(dctx, rnd->framesets[rnd->frameIdx].objUbo, 64, sizeof(m), m);
                //AvxCmdUpdateBuffer(dctx, rnd->framesets[rnd->frameIdx].objConstantsBuffer, 128, sizeof(m), m);

                afxUnit surfCnt = mshi.secCnt;

                for (afxUnit j = 0; j < surfCnt; j++)
                {
                    arxMeshSection sec;
                    ArxDescribeMeshSections(msh, j, 1, &sec);
                    //AFX_ASSERT(msh->topology->primType == avxTopology_TRI_LIST);

                    akxMaterialConstants mat;

                    if (sec.mtlIdx == AFX_INVALID_INDEX)
                    {
                        AvxMakeColor(mat.Kd, 0.3f, 0.3f, 0.3f, 1.0f);
                        mat.hasDiffTex = FALSE;
                        //AvxUpdateBuffer(rnd->framesets[rnd->frameIdx].mtlConstantsBuffer, 0, sizeof(mat), &mat);
                    }
                    else
                    {
#if 0
                        AFX_ASSERT(sec->mtlIdx < msh->mtlSlotCnt);
                        arxMaterial mtl = msh->mtlSlots[sec->mtlIdx].mtl;

                        if (mtl)
                        {
                            AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
                            avxRaster tex = mtl->tex;

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

                            //AvxUpdateBuffer(rnd->framesets[rnd->frameIdx].mtlConstantsBuffer, 0, sizeof(mat), &mat);
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

_ARX afxError AkxTestDrawIndexed(akxRenderer rnd, afxDrawContext dctx)
{
    AvxCmdUseDrawTechniqueSIGMA(dctx, rnd->testDtec, 0, rnd->testVin, NIL);

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

_ARX afxError ArxRendererSetStar(akxRenderer rnd, afxV4d const pos, afxV3d const dir, afxV4d const Kd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);

    akxViewConstants *viewConstants = &rnd->framesets[rnd->frameIdx].viewConstants;

    AfxV4dCopy(viewConstants->starPos, pos);
    AfxV4dCopy(viewConstants->starDir, dir);
    AfxV4dCopy(viewConstants->starKd, Kd);

    return err;
}

_ARX afxError ArxEndSceneRendering(akxRenderer rnd, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);

    return err;
}

_ARX afxError ArxBeginSceneRendering(akxRenderer rnd, arxCamera cam, afxRect const* drawArea, avxCanvas canv, afxDrawContext dctx)
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
        AvxGetCanvasArea(canv, AVX_ORIGIN_ZERO, &rnd->drawArea);

    }
    else
    {
        rnd->drawArea = (afxRect) { 0 };
    }

    akxViewConstants *viewConstants = &rnd->framesets[frameIdx].viewConstants;

    if ((rnd->activeCam = cam))
    {
        //cam = rnd->activeCamera;

        afxV2d extent = { rnd->drawArea.w, rnd->drawArea.h };
        ArxSetCameraAspectRatios(cam, AvxFindPhysicalAspectRatio(rnd->drawArea.w, rnd->drawArea.h), extent, extent);

        viewConstants->viewExtent[0] = rnd->drawArea.w;
        viewConstants->viewExtent[1] = rnd->drawArea.h;

        afxV4d viewPos;
        ArxGetCameraPosition(cam, viewPos);
        AfxV4dCopyAtv3d(viewConstants->viewPos, viewPos);

        afxM4d v, iv, p, ip, pv, ipv;
        ArxRecomputeCameraMatrices(cam);
        ArxGetCameraMatrices(cam, p, pv, v, ip, ipv, iv);
        
        AfxM4dCopy(viewConstants->p, p);
        AfxM4dCopy(viewConstants->ip, ip);
        AfxM4dCopyAtm(viewConstants->v, v);
        AfxM4dCopy(viewConstants->iv, iv);
        //AfxM4dMultiply(viewConstants->pv, p, v);
        AfxM4dCopy(viewConstants->pv, pv);
        //AfxM4dInvert(viewConstants->ipv, viewConstants->pv);
        AfxM4dCopy(viewConstants->ipv, ipv);
    }

    AvxCmdUpdateBuffer(dctx, rnd->framesets[frameIdx].viewUbo, 0, sizeof(*viewConstants), viewConstants);
    AvxCmdBindBuffers(dctx, 0, 0, 1, (avxBufferedMap[]) { {.buf = rnd->framesets[frameIdx].viewUbo } });
    return err;
}

_ARX afxError _ArxRndDtor(akxRenderer rnd)
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

    arxRenderware din = rnd->din;

    AfxDisposeObjects(1, &din);
#if 0
    for (afxUnit i = 0; i < rnd->frameCnt; i++)
    {
        if (rnd->canv[i])
            AfxReleaseObject(&rnd->canv[i]->obj);
    }
#endif

    AfxEmptyArray(&rnd->capturedNodes, FALSE, FALSE);

    return err;
}

_ARX afxError _ArxRndCtor(akxRenderer rnd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RND, 1, &rnd);

    arxRenderware din = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    akxRendererConfig const* config = ((akxRendererConfig const*)args[1]) + invokeNo;

    rnd->cachedSim = config->sim;
    afxDrawSystem dsys = ArxGetDrawInputContext(din);
    rnd->cachedDsys = dsys;
    
    //rnd->dout = NIL;

    rnd->frameCnt = 2;
    rnd->frameIdx = 0;

    //AfxZero2(rnd->canv, sizeof(rnd->canv));
    AfxZero(rnd->framesets, sizeof(rnd->framesets));

    // acquire and set up our dedicated draw input device.

    if (!config->din)
    {
        arxRenderwareConfig dinConfig = { 0 };
        dinConfig.dsys = dsys;
        dinConfig.proc = config->dinProc;
        dinConfig.udd = rnd;
        dinConfig.cmdPoolMemStock = 4096;
        dinConfig.estimatedSubmissionCnt = 3;

        ArxOpenRenderware(0, &dinConfig, &din);
        AFX_ASSERT(din);
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

        avxBufferInfo vboSpec = { 0 };
        vboSpec.size = sizeof(testVertices);
        vboSpec.flags = avxBufferFlag_W;
        vboSpec.usage = avxBufferUsage_VERTEX;
        AvxAcquireBuffers(dsys, 1, &vboSpec, &rnd->testVbo);
        
        avxBufferIo vboIop = { 0 };
        vboIop.dstStride = 1;
        vboIop.srcStride = 1;
        vboIop.rowCnt = vboSpec.size;
        AvxUpdateBuffer(rnd->testVbo, 1, &vboIop, testVertices, 0);

        avxBufferInfo iboSpec = { 0 };
        iboSpec.size = sizeof(testIndices);
        iboSpec.flags = avxBufferFlag_W;
        iboSpec.usage = avxBufferUsage_INDEX;
        AvxAcquireBuffers(dsys, 1, &iboSpec, &rnd->testIbo);

        avxBufferIo iboIop = { 0 };
        iboIop.dstStride = 1;
        iboIop.srcStride = 1;
        iboIop.rowCnt = iboSpec.size;
        AvxUpdateBuffer(rnd->testIbo, 1, &iboIop, testIndices, 0);

        avxVertexLayout vtxl = 
        { 
            .srcCnt = 1,
            .srcs =
            {
                AVX_VERTEX_FETCH(0, 0, 0, 0, 1)
            },
            .attrs =
            {
                AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f)
            }
        };

        AvxDeclareVertexInputs(dsys, 1, &vtxl, &vin);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
        rnd->testVin = vin;

        AfxMakeUri(&uri, 0, "../gfx/test/test.xsh.xml", 0);
        ArxLoadDrawTechnique(din, &uri, &rnd->testDtec);
    }

    avxVertexLayout skinVtxl =
    {
        .srcCnt = 2,
        .srcs =        
        {
            AVX_VERTEX_FETCH(0, 0, 0, 0, 3),
            AVX_VERTEX_FETCH(1, 0, 0, 3, 2)
        },
        .attrs =
        {
            AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f),
            AVX_VERTEX_ATTR(1, 12, avxFormat_RGBA8un),
            AVX_VERTEX_ATTR(2, 16, avxFormat_RGBA32f),
            AVX_VERTEX_ATTR(3, 0, avxFormat_RGB32f),
            AVX_VERTEX_ATTR(6, 12, avxFormat_RG32f)
        }
    };

    avxVertexInput skinnedVin;
    AvxDeclareVertexInputs(dsys, 1, &skinVtxl, &skinnedVin);
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &skinnedVin);
    rnd->skinnedVin = skinnedVin;

    avxVertexLayout rigidVtxl =
    {
        .srcCnt = 2,
        .srcs =
        {
            AVX_VERTEX_FETCH(0, 0, 0, 0, 1),
            AVX_VERTEX_FETCH(1, 0, 0, 1, 2)
        },
        .attrs =
        {
            AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f),
            AVX_VERTEX_ATTR(3, 0, avxFormat_RGB32f),
            AVX_VERTEX_ATTR(6, 12, avxFormat_RG32f)
        }
    };

    avxVertexInput rigidVin;
    AvxDeclareVertexInputs(dsys, 1, &rigidVtxl, &rigidVin);
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &rigidVin);
    rnd->rigidVin = rigidVin;

    {
        avxVertexLayout vtxl_p3n3t2_bi_mtl_mtx_jnt =
        {
            .srcCnt = 3,
            .srcs =
            {
                AVX_VERTEX_FETCH(0, 0, 0, 0, 1),
                AVX_VERTEX_FETCH(1, 0, 0, 1, 2),
                AVX_VERTEX_FETCH(2, 0, 1, 3, 4)
            },
            .attrs =
            {
                // per-vertex fetch
                AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(3, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(6, 12, avxFormat_RG32f),
                // per-instance fetch
                AVX_VERTEX_ATTR(10, 0, avxFormat_R32u),
                AVX_VERTEX_ATTR(11, 4, avxFormat_R32u),
                AVX_VERTEX_ATTR(12, 8, avxFormat_R32u),
                AVX_VERTEX_ATTR(13, 12, avxFormat_R32u)
            }
        };

        avxVertexInput vin_p3n3t2_bi_mtl_mtx_jnt;
        AvxDeclareVertexInputs(dsys, 1, &vtxl_p3n3t2_bi_mtl_mtx_jnt, &vin_p3n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3n3t2_bi_mtl_mtx_jnt = vin_p3n3t2_bi_mtl_mtx_jnt;

        avxVertexLayout vtxl_p3j1n3t2_bi_mtl_mtx_jnt =
        {
            .srcCnt = 3,
            .srcs =
            {
                AVX_VERTEX_FETCH(0, 0, 0, 0, 2),
                AVX_VERTEX_FETCH(1, 0, 0, 2, 2),
                AVX_VERTEX_FETCH(2, 0, 1, 4, 4)
            },
            .attrs =
            {
                // per-vertex fetch
                AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(1, 12, avxFormat_R8u),
                AVX_VERTEX_ATTR(3, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(6, 12, avxFormat_RG32f),
                // per instance
                AVX_VERTEX_ATTR(10, 0, avxFormat_R32u),
                AVX_VERTEX_ATTR(11, 4, avxFormat_R32u),
                AVX_VERTEX_ATTR(12, 8, avxFormat_R32u),
                AVX_VERTEX_ATTR(13, 12, avxFormat_R32u),
            }
        };

        avxVertexInput vin_p3j1n3t2_bi_mtl_mtx_jnt;
        AvxDeclareVertexInputs(dsys, 1, &vtxl_p3j1n3t2_bi_mtl_mtx_jnt, &vin_p3j1n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3j1n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3j1n3t2_bi_mtl_mtx_jnt = vin_p3j1n3t2_bi_mtl_mtx_jnt;

        avxVertexLayout vtxl_p3j2n3t2_bi_mtl_mtx_jnt =
        {
            .srcCnt = 3,
            .srcs =
            {
                AVX_VERTEX_FETCH(0, 0, 0, 0, 3),
                AVX_VERTEX_FETCH(1, 0, 0, 3, 2),
                AVX_VERTEX_FETCH(2, 0, 1, 5, 4)
            },
            .attrs =
            {
                // per-vertex fetch
                AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(1, 12, avxFormat_RG32f),
                AVX_VERTEX_ATTR(2, 20, avxFormat_RG8u),
                AVX_VERTEX_ATTR(3, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(6, 12, avxFormat_RG32f),
                // per-instance fetch
                AVX_VERTEX_ATTR(10, 0, avxFormat_R32u),
                AVX_VERTEX_ATTR(11, 4, avxFormat_R32u),
                AVX_VERTEX_ATTR(12, 8, avxFormat_R32u),
                AVX_VERTEX_ATTR(13, 12, avxFormat_R32u)
            }
        };

        avxVertexInput vin_p3j2n3t2_bi_mtl_mtx_jnt;
        AvxDeclareVertexInputs(dsys, 1, &vtxl_p3j2n3t2_bi_mtl_mtx_jnt, &vin_p3j2n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3j2n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3j2n3t2_bi_mtl_mtx_jnt = vin_p3j2n3t2_bi_mtl_mtx_jnt;

        avxVertexLayout vtxl_p3j3n3t2_bi_mtl_mtx_jnt =
        {
            .srcCnt = 3,
            .srcs =
            {
                AVX_VERTEX_FETCH(0, 0, 0, 0, 3),
                AVX_VERTEX_FETCH(1, 0, 0, 3, 2),
                AVX_VERTEX_FETCH(2, 0, 1, 5, 4)
            },
            .attrs =
            {
                // per-vertex fetch
                AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(1, 12, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(2, 24, avxFormat_RGB8u),
                AVX_VERTEX_ATTR(3, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(6, 12, avxFormat_RG32f),
                // per-instance fetch
                AVX_VERTEX_ATTR(10, 0, avxFormat_R32u),
                AVX_VERTEX_ATTR(11, 4, avxFormat_R32u),
                AVX_VERTEX_ATTR(12, 8, avxFormat_R32u),
                AVX_VERTEX_ATTR(13, 12, avxFormat_R32u),
            }
        };

        avxVertexInput vin_p3j3n3t2_bi_mtl_mtx_jnt;
        AvxDeclareVertexInputs(dsys, 1, &vtxl_p3j3n3t2_bi_mtl_mtx_jnt, &vin_p3j3n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3j3n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3j3n3t2_bi_mtl_mtx_jnt = vin_p3j3n3t2_bi_mtl_mtx_jnt;

        avxVertexLayout vtxl_p3j4n3t2_bi_mtl_mtx_jnt =
        {
            .srcCnt = 3,
            .srcs =
            {
                AVX_VERTEX_FETCH(0, 0, 0, 0, 3),
                AVX_VERTEX_FETCH(1, 0, 0, 3, 2),
                AVX_VERTEX_FETCH(2, 0, 1, 5, 4)
            },
            //.srcs[0].attrCnt = 9,
            .attrs =
            {
                // per-vertex fetch
                AVX_VERTEX_ATTR(0, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(1, 12, avxFormat_RGBA32f),
                AVX_VERTEX_ATTR(2, 28, avxFormat_RGBA8u),
                AVX_VERTEX_ATTR(3, 0, avxFormat_RGB32f),
                AVX_VERTEX_ATTR(6, 12, avxFormat_RG32f),
                // per-instance fetch
                AVX_VERTEX_ATTR(10, 0, avxFormat_R32u),
                AVX_VERTEX_ATTR(11, 4, avxFormat_R32u),
                AVX_VERTEX_ATTR(12, 8, avxFormat_R32u),
                AVX_VERTEX_ATTR(13, 12, avxFormat_R32u),
            }
        };

        avxVertexInput vin_p3j4n3t2_bi_mtl_mtx_jnt;
        AvxDeclareVertexInputs(dsys, 1, &vtxl_p3j4n3t2_bi_mtl_mtx_jnt, &vin_p3j4n3t2_bi_mtl_mtx_jnt);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin_p3j4n3t2_bi_mtl_mtx_jnt);
        rnd->vin_p3j4n3t2_bi_mtl_mtx_jnt = vin_p3j4n3t2_bi_mtl_mtx_jnt;

    }
    {
        avxBufferInfo bufSpec[] =
        {
            { .size = sizeof(akxViewConstants), .flags = avxBufferFlag_WX | avxBufferFlag_C, .usage = avxBufferUsage_UNIFORM },
            { .size = sizeof(akxShaderConstants), .flags = avxBufferFlag_WX | avxBufferFlag_C, .usage = avxBufferUsage_UNIFORM },
            { .size = sizeof(akxMaterialConstants), .flags = avxBufferFlag_WX | avxBufferFlag_C, .usage = avxBufferUsage_UNIFORM },
            { .size = sizeof(akxInstanceConstants), .flags = avxBufferFlag_WX/* | avxBufferFlag_COHERENT*/, .usage = avxBufferUsage_UNIFORM }
        };
#if 0
        avxRasterInfo texiDepthSurfB = { 0 };
        texiDepthSurfB.e
        afxTextureBlueprint depthSurfB;
        AfxAcquireTextureBlueprint(&depthSurfB, (afxWhd) {1024, 1024, 1 }, avxFormat_D24, avxRasterUsage_DRAW);
        AfxTextureBlueprintAddImage(&depthSurfB, avxFormat_D24, (afxWhd) { 1, 1, 1 }, NIL, 0);
#endif

        for (afxUnit i = 0; i < rnd->frameCnt; i++)
        {
            //AfxBuildRasters(dsys, 1, &rnd->framesets[i].depthSurf, &depthSurfB);

            AvxAcquireBuffers(dsys, 1, &bufSpec[0], &rnd->framesets[i].viewUbo);
            AvxAcquireBuffers(dsys, 1, &bufSpec[1], &rnd->framesets[i].shdUbo);
            AvxAcquireBuffers(dsys, 1, &bufSpec[2], &rnd->framesets[i].mtlUbo);
            AvxAcquireBuffers(dsys, 1, &bufSpec[3], &rnd->framesets[i].objUbo);

            avxBufferInfo bufi = { 0 };
            bufi.size = AVX_BUFFER_UPDATE_CAPACITY;
            bufi.usage = avxBufferUsage_INDIRECT | avxBufferUsage_VERTEX;
            bufi.flags = avxBufferFlag_W | avxBufferFlag_X/* | avxBufferFlag_COHERENT*/;
            AvxAcquireBuffers(dsys, 1, &bufi, &rnd->framesets[i].icbo);

            bufi.size = AVX_BUFFER_UPDATE_CAPACITY;
            bufi.usage = avxBufferUsage_FETCH;
            bufi.flags = avxBufferFlag_W | avxBufferFlag_X/* | avxBufferFlag_COHERENT*/;
            bufi.fmt = avxFormat_R32u;
            AvxAcquireBuffers(dsys, 1, &bufi, &rnd->framesets[i].biasMapUbo);


            afxByte* icboPtr = NIL;
            afxByte* mtboPtr = NIL;
            afxByte* bmboPtr = NIL;
            afxByte* mtlboPtr = NIL;
            avxBufferedMap maps[] =
            {
                AVX_BUFFERED_MAP(rnd->framesets[i].objUbo, 0, AvxGetBufferCapacity(rnd->framesets[i].objUbo, 0), 0),
                AVX_BUFFERED_MAP(rnd->framesets[i].biasMapUbo, 0, AvxGetBufferCapacity(rnd->framesets[i].biasMapUbo, 0), 0),
                AVX_BUFFERED_MAP(rnd->framesets[i].mtlUbo, 0, AvxGetBufferCapacity(rnd->framesets[i].mtlUbo, 0), 0),
                AVX_BUFFERED_MAP(rnd->framesets[i].icbo, 0, AvxGetBufferCapacity(rnd->framesets[i].icbo, 0), 0)
            };

            if (AvxMapBuffers(dsys, ARRAY_SIZE(maps), maps, (void**[]) {&mtboPtr, &bmboPtr, &mtlboPtr, &icboPtr }))
                AfxThrowError();

            rnd->framesets[i].mtboPtr = mtboPtr;
            rnd->framesets[i].bmboPtr = bmboPtr;
            rnd->framesets[i].mtlboPtr = mtlboPtr;
            rnd->framesets[i].icboPtr = icboPtr;

            AvxAcquireFences(dsys, TRUE, 1, &rnd->framesets[i].drawCompletedFence);
        }

        //AfxTextureBlueprintEnd(&depthSurfB, 0, NIL);
    }

    {
        AfxMakeUri(&uri, 0, "../gfx/body/body.xsh.xml", 0);
        ArxLoadDrawTechnique(din, &uri, &rnd->bodyDtec);
    }

    ArxRendererSetStar(rnd, AFX_V4D(0, 0, 0, 1), AFX_V3D(-0.8660f, 0.5f, 0), AFX_V4D(0.8, 0.8, 0.8, 0.8));
    // ambient 0.2, 0.2, 0.2, 0.2
    
    arxPoseInfo posei = { 0 };
    posei.artCnt = 255;
    ArxAcquirePoses(rnd->din, 1, &posei, &rnd->lp);
    ArxAcquirePlacements(rnd->din, 1, (afxUnit[]) { 255 }, (afxBool[]) {TRUE}, &rnd->wp[0]);
    ArxAcquirePlacements(rnd->din, 1, (afxUnit[]) { 255 }, (afxBool[]) { TRUE }, &rnd->wp[1]);

    AfxMakeUri(&uri, 0, "../gfx/testLighting/testLighting.xsh.xml", 0);
    ArxLoadDrawTechnique(din, &uri, &rnd->blinnTestRazrDtec);

    AfxMakeUri(&uri, 0, "../gfx/tutCamUtil/tutCamUtil.xsh.xml", 0);
    ArxLoadDrawTechnique(din, &uri, &rnd->tutCamUtilDtec);

    AfxMakeUri(&uri, 0, "../gfx/lighting/lighting.xsh.xml", 0);
    ArxLoadDrawTechnique(din, &uri, &rnd->lightingDtec);

    AfxMakeArray(&rnd->capturedNodes, sizeof(arxBody), 100, NIL, 0);

    return err;
}

_ARX afxClassConfig const _ARX_RND_CLASS_CONFIG =
{
    .fcc = afxFcc_RND,
    .name = "Renderer",
    .fixedSiz = sizeof(AFX_OBJECT(akxRenderer)),
    .ctor = (void*)_ArxRndCtor,
    .dtor = (void*)_ArxRndDtor
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireRenderers(arxRenderware din, afxUnit cnt, akxRenderer rnd[], akxRendererConfig const config[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    if (AfxAcquireObjects((afxClass *)_ArxGetRendererClass(din), cnt, (afxObject*)rnd, (void const*[]) { din, (void*)config }))
        AfxThrowError();

    return err;
}
