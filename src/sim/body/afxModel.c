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

#define _AMX_DRAW_C
#define _AMX_MODEL_C
#define _AMX_SKELETON_C
#define _AMX_PLACEMENT_C
#define _AMX_POSE_C
#include "../../draw/impl/avxImplementation.h"
#include "../impl/amxImplementation.h"

// IMPORTANT:
// Some models are not built from a mesh in the classic sense of the word. 
// Models structured out of Non-uniform rational basis spline (NURBS) also exit. 
// Technically you could build a model in many ways that do not involve a mesh in the classic sense of the word, such as atoms and Voxels. 
// When people speak of a mesh they normally refer to a polygon mesh.

_AMXINL afxBool AfxGetModelUrn(afxModel mdl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(id);
    *id = mdl->urn;
    return mdl->urn.len;
}

_AMXINL void AfxDescribeModel(afxModel mdl, afxModelInfo* info)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    info->jntCnt = mdl->jntCnt;
    info->joints = mdl->jntId;
    info->jntPi = mdl->jntPi;
    info->jntLt = mdl->jntLt;
    info->jntIw = mdl->jntIw;
    info->jntLe = mdl->jntLe;
    info->lodType = mdl->lodType;
    info->allowedLodErrFadingFactor = mdl->allowedLodErrFadingFactor;
    info->displacement = mdl->displacement;
    info->rigCnt = mdl->rigCnt;
    info->riggedMeshCnt = mdl->usedRigCnt;
}

// DISPLACEMENT

_AMXINL void AfxDisplaceModel(afxModel mdl, afxTransform const* t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    if (!t)
        AfxResetTransform(&mdl->displacement);
    else
        AfxCopyTransform(&mdl->displacement, t);
}

_AMXINL void AfxGetModelDisplacement(afxModel mdl, afxTransform* t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(t);
    AfxCopyTransform(&mdl->displacement, t);
}

_AMXINL void AfxComputeModelDisplacement(afxModel mdl, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(m);
    AfxComputeCompositeTransformM4d(&mdl->displacement, m);
}

// MESH RIGGING

_AMX void AfxDescribeMeshRig(afxModel mdl, afxUnit rigIdx, afxMeshRigInfo* info)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    afxMeshRig* rig = &mdl->rigs[rigIdx];

    AFX_ASSERT(info);
    info->flags = rig->flags;
    info->msh = rig->msh;
    info->biasToJnt = rig->biasToJointMap;
    info->biasFromJnt = rig->biasFromJointMap;
    info->origSkl = rig->sklOrig;
    info->txd = rig->txd;
}

_AMXINL afxBool AfxIsMeshRigTransplanted(afxModel mdl, afxUnit rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
    
    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    return mdl->rigs[rigIdx].flags & afxMeshRigFlag_TRANSPLANTED;
}

_AMXINL afxUnit AfxGetRiggedMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit rigCnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, baseRigIdx, rigCnt);
    
    // sanitize arguments
    baseRigIdx = AfxMin(baseRigIdx, mdl->rigCnt - 1);
    rigCnt = AfxMin(rigCnt, mdl->rigCnt - baseRigIdx);

    afxUnit rslt = 0;
    for (afxUnit i = 0; i < rigCnt; i++)
    {
        afxUnit rigIdx = baseRigIdx + i;
        AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
        afxMesh msh = mdl->rigs[rigIdx].msh;

        if (msh)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
            ++rslt;
        }
        meshes[i] = msh;
    }
    return rslt;
}

_AMX afxError AfxRigMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit rigCnt, afxMesh const meshes[], afxModel sklOrig)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, baseRigIdx, rigCnt);

    // sanitize arguments
    baseRigIdx = AfxMin(baseRigIdx, mdl->rigCnt - 1);
    rigCnt = AfxMin(rigCnt, mdl->rigCnt - baseRigIdx);

    AFX_TRY_ASSERT_OBJECTS(afxFcc_MDL, 1, &sklOrig);
    afxBool transplanted = sklOrig && (sklOrig != mdl);

    for (afxUnit rigIter = 0; rigIter < rigCnt; rigIter++)
    {
        afxUnit rigIdx = baseRigIdx + rigIter;
        AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
        afxMeshRig* rig = &mdl->rigs[rigIdx];
        afxMesh mshCurr = rig->msh;
        afxMesh msh = meshes ? meshes[rigIter] : NIL;

        if (mshCurr == msh)
            continue;

        afxUnit* biasFromJointMap = NIL;
        afxUnit* biasToJointMap = NIL;

        if (msh) // attach operation
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
            afxMeshInfo mshi;
            AfxDescribeMesh(msh, &mshi);

            afxUnit biasCnt = mshi.biasCnt;

            if (biasCnt && AfxAllocate(biasCnt * sizeof(biasToJointMap[0]), 0, AfxHere(), (void**)&biasToJointMap))
            {
                AfxThrowError();
                continue;
            }
            
            if (!transplanted) biasFromJointMap = biasToJointMap;
            else if (biasCnt && AfxAllocate(biasCnt * sizeof(biasFromJointMap[0]), 0, AfxHere(), (void**)&biasFromJointMap))
                AfxThrowError();

            if (!err)
            {
                afxString* biasTags = AfxGetMeshBiasTags(msh, 0);
                afxUnit biasMappedCnt = 0;

                if (biasCnt != (biasMappedCnt = AfxFindJoints(mdl, biasCnt, biasTags, biasToJointMap)))
                {
                    //AfxLogAdvertence("%u biases mapped to joints from %u", biasMappedCnt, biasCnt);
                }

                if (transplanted)
                {
                    if (biasCnt != (biasMappedCnt = AfxFindJoints(mdl, biasCnt, biasTags, biasFromJointMap)))
                    {
                        //AfxLogAdvertence("%u joints to biases mapped from %u", biasMappedCnt, biasCnt);
                    }
                }

                afxString s;
                AfxGetMeshUrn(msh, &s);
                AfxLogEcho("+-- rig #%u", rigIdx);
                AfxLogEcho("|   +-- msh <%.*s> %p", AfxPushString(&s), msh);
                AfxLogEcho("|   +-- biases-from/to-joints");
                for (afxUnit i = 0; i < biasCnt; i++)
                {
                    s = *AfxGetMeshBiasTags(msh, i);
                    AfxLogEcho("|   |   +-- <%.*s> @ <%u#%.*s>(%u#'%.*s')", AfxPushString(&s), biasFromJointMap[i], AfxPushString(&mdl->jntId[biasFromJointMap[i]]), biasToJointMap[i], AfxPushString(&mdl->jntId[biasToJointMap[i]]));
                }

                AfxReacquireObjects(1, &msh);

                if (sklOrig && (sklOrig != mdl))
                    AfxReacquireObjects(1, &sklOrig);
            }

            if (err && biasFromJointMap && biasFromJointMap != biasToJointMap)
                AfxDeallocate((void**)&biasFromJointMap, AfxHere());

            if (err && biasToJointMap)
                AfxDeallocate((void**)&biasToJointMap, AfxHere());
        }

        if (!err)
        {
            if (mshCurr) // detach operation
            {
                AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &mshCurr);

                if (rig->biasFromJointMap && (rig->biasFromJointMap != rig->biasToJointMap))
                {
                    AfxDeallocate((void**)&rig->biasFromJointMap, AfxHere());
                    rig->biasFromJointMap = NIL;
                }

                if (rig->biasToJointMap)
                {
                    AfxDeallocate((void**)&rig->biasToJointMap, AfxHere());
                    rig->biasToJointMap = NIL;
                }

                if (AfxIsMeshRigTransplanted(mdl, rigIdx))
                {
                    afxModel sklOrig = rig->sklOrig;
                    AFX_TRY_ASSERT_OBJECTS(afxFcc_MDL, 1, &sklOrig);
                    AfxDisposeObjects(1, &sklOrig);
                    rig->sklOrig = NIL;
                    rig->flags &= ~afxMeshRigFlag_TRANSPLANTED;
                }

                AfxDisposeObjects(1, &mshCurr);
                rig->msh = NIL;
                --mdl->usedRigCnt;
            }

            if (msh)
            {
                rig->msh = msh;
                rig->sklOrig = sklOrig;
                rig->biasFromJointMap = biasFromJointMap;
                rig->biasToJointMap = biasToJointMap;

                if (transplanted)
                    rig->flags |= afxMeshRigFlag_TRANSPLANTED;

                ++mdl->usedRigCnt;
            }
        }
    }
    return err;
}

_AMXINL void AfxComputeMeshRigMatrices(afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit baseBiasIdx, afxUnit biasCnt, afxM4d matrices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    afxMesh msh = mdl->rigs[rigIdx].msh;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
    afxMeshInfo mshi;
    AfxDescribeMesh(msh, &mshi);

    AFX_ASSERT_RANGE(mshi.biasCnt, baseBiasIdx, biasCnt);
    // sanitize arguments
    baseBiasIdx = AfxMin(baseBiasIdx, mshi.biasCnt - 1);
    biasCnt = AfxMin(biasCnt, mshi.biasCnt - baseBiasIdx);

    afxM4d* w = AfxGetPlacementMatrices(plce, 0);
    afxM4d* iw = mdl->rigs[rigIdx].sklOrig->jntIw;
    afxUnit const* mapping = mdl->rigs[rigIdx].biasToJointMap;
    afxUnit const* mappingOrig = mdl->rigs[rigIdx].biasFromJointMap;

    for (afxUnit i = 0, artIdx = baseBiasIdx; artIdx < biasCnt; artIdx++)
    {
        AfxM4dMultiplyAffine(matrices[i], iw[mappingOrig[i]], w[mapping[i]]);
    }
}

_AMXINL void AfxSetMeshRigTxd(afxModel mdl, afxUnit rigIdx, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    afxMaterial curr;

    if (mtl != (curr = mdl->rigs[rigIdx].txd))
    {
        if (mtl)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
            AfxReacquireObjects(1, &mtl);
        }

        mdl->rigs[rigIdx].txd = mtl;

        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &curr);
            AfxDisposeObjects(1, &curr);
        }
    }
}

_AMXINL afxBool AfxFindMeshRigMaterial(afxModel mdl, afxUnit rigIdx, afxString const* id, afxMaterial* material)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    afxMaterial txd = mdl->rigs[rigIdx].txd;
    afxMaterial mtl = NIL;

    if (txd)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &txd);
        AFX_ASSERT(id);

        if ((mtl = AfxFindSubmaterial(txd, id)))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
        }
    }
    AFX_ASSERT(material);
    *material = mtl;
    return !!mtl;
}

// SKELETON

_AMX afxError AfxReparentJoints(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* indices, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);
    //AFX_ASSERT(indices);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    if (!indices) for (afxUnit i = 0; i < cnt; i++) mdl->jntPi[i] = AFX_INVALID_INDEX;
    else switch (stride)
    {
        case sizeof(afxUnit8) :
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8 const* src = indices;
                afxUnit parentIdx = src[i];
                AfxAssertOR(parentIdx == AFX_INVALID_INDEX, parentIdx < mdl->jntCnt);
                mdl->jntPi[i] = parentIdx;
            }
            break;
        }
        case sizeof(afxUnit16) :
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16 const* src = indices;
                afxUnit parentIdx = src[i];
                AfxAssertOR(parentIdx == AFX_INVALID_INDEX, parentIdx < mdl->jntCnt);
                mdl->jntPi[i] = parentIdx;
            }
            break;
        }
        case sizeof(afxUnit32) :
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32 const* src = indices;
                afxUnit parentIdx = src[i];
                AfxAssertOR(parentIdx == AFX_INVALID_INDEX, parentIdx < mdl->jntCnt);
                mdl->jntPi[i] = parentIdx;
            }
            break;
        }
        default: AfxThrowError(); break;
    }
    return err;
}

_AMX afxError AfxResetJointMatrices(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* matrices, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    if (!matrices) for (afxUnit i = 0; i < cnt; i++) AfxM4dReset(mdl->jntIw[i]);
    else switch (stride)
    {
    case 0:
    {
        afxM4d const* src = matrices;

        for (afxUnit i = 0; i < cnt; i++)
            AfxM4dCopyAtm(mdl->jntIw[i], src[0]);

        break;
    }
    case sizeof(afxM4d) :
    {
        afxM4d const* src = matrices;

        for (afxUnit i = 0; i < cnt; i++)
            AfxM4dCopyAtm(mdl->jntIw[i], src[i]);

        break;
    }
    case sizeof(afxAtm3d) :
    {
        afxAtm3d const* src = (afxAtm3d const*)matrices;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxM4dSetAffine(mdl->jntIw[i], src[i][0], src[i][1], src[i][2], src[i][3]);
        }
        break;
    }
    case sizeof(afxM3d) :
    {
        afxM3d const* src = (afxM3d const*)matrices;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxM4dCopyM3d(mdl->jntIw[i], src[i], AFX_V4D_IDENTITY);
        }
        break;
    }
    default:
    {
        AfxThrowError();
        break;
    }
    }
    return err;
}

_AMX afxError AfxResetJointTransforms(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxTransform const transforms[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    if (!transforms) for (afxUnit i = 0; i < cnt; i++) AfxResetTransform(&mdl->jntLt[i]);
    else for (afxUnit i = 0; i < cnt; i++)
        AfxCopyTransform(&mdl->jntLt[i], &transforms[i]);

    return err;
}

_AMX afxError AfxResetJointLodErrors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxReal const lodErrors[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    if (!lodErrors) for (afxUnit i = 0; i < cnt; i++) mdl->jntLe[i] = -1.0;
    else for (afxUnit i = 0; i < cnt; i++)
        mdl->jntLe[i] = lodErrors[i];

    return err;
}

_AMXINL void AfxQueryModelErrorTolerance(afxModel mdl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    
    if (allowedErr > 0.0)
    {
        afxReal s = mdl->allowedLodErrFadingFactor * allowedErr;
        *allowedErrEnd = s;
        *allowedErrScaler = 1.0 / (allowedErr - s);
    }
    else
    {
        *allowedErrEnd = 0.0;
        *allowedErrScaler = 0.0;
    }
}

_AMX afxUnit AfxCountJoints(afxModel mdl, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    afxUnit rslt = 0;

    if (allowedErr == 0.0 || !mdl->lodType) rslt = mdl->jntCnt;
    else
    {
        afxReal errEnd, errScaler;
        AfxQueryModelErrorTolerance(mdl, allowedErr, &errEnd, &errScaler);

        afxUnit jointCnt = mdl->jntCnt;
        afxReal* lodError = mdl->jntLe;

        if (jointCnt >= 4)
        {
            while (lodError[rslt + 0] >= errEnd)
            {
                if (lodError[rslt + 1] < errEnd)
                {
                    ++rslt;
                    break;
                }

                if (lodError[rslt + 2] < errEnd)
                {
                    rslt += 2;
                    break;
                }

                if (lodError[rslt + 3] < errEnd)
                {
                    rslt += 3;
                    break;
                }

                rslt += 4;

                if (rslt >= jointCnt - 3)
                    break;
            }
        }

        for (; rslt < jointCnt; rslt++)
            if (lodError[rslt] < errEnd)
                break;
    }
    return rslt;
}

_AMX afxUnit AfxFindJoints(afxModel mdl, afxUnit cnt, afxString const ids[], afxUnit indices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(indices);
    AFX_ASSERT(cnt);
    AFX_ASSERT(ids);
    afxUnit rslt = 0;
    afxUnit totalJntCnt = mdl->jntCnt;

    for (afxUnit j = 0; j < cnt; j++)
    {
        afxUnit jointIdx = AFX_INVALID_INDEX;

        for (afxUnit i = 0; i < totalJntCnt; i++)
        {
            afxString jointTag = mdl->jntId[i];

            if ((!jointTag.len && !ids[j].len) || (jointTag.len && (0 == AfxCompareStrings(&ids[j], 0, TRUE, 1, &jointTag))))
            {
                jointIdx = i;
                ++rslt;
                break;

            }
        }
        indices[j] = jointIdx;
    }
    return rslt;
}

_AMX void AfxComputeAttachmentWorldMatrix(afxModel mdl, afxUnit jntIdx, afxPose const pose, afxM4d const offset, afxUnit const* sparseJntMap, afxUnit const* sparseJntMapRev, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    // Should be compatible with void GetWorldMatrixFromLocalAttitude(const skeleton *Skeleton, int BoneIndex, const local_pose *LocalAttitude, const float *Offset4x4, float *Result4x4, const int *SparseBoneArray, const int *SparseBoneArrayReverse)
    // void AfxSklGetWorldMatrixFromLocalAttitude(afxSkeleton skl, afxUnit jointIdx, afxPose const lp, afxM4d const offset, afxM4d m, afxUnit const* sparseBoneArray, afxUnit const* sparseBoneArrayReverse)
    // AfxSklGetWorldMatrixFromLocalAttitude(skl, jntIdx, atti, offset, m, sparseJntMap, sparseJntMapRev);

    AFX_ASSERT_RANGE(pose->artCnt, jntIdx, 1);

    AfxM4dReset(m);

    for (afxUnit i = jntIdx; i != AFX_INVALID_INDEX; i = mdl->jntPi[i])
    {
        AFX_ASSERT_RANGE(mdl->jntCnt, i, 1);
        afxTransform* t = AfxGetPoseTransform(pose, sparseJntMapRev ? sparseJntMapRev[i] : i);
        afxM4d tmp, tmp2;
        AfxComputeCompositeTransformM4d(t, tmp);
        AfxM4dMultiplyAffine(tmp2, m, tmp);
        AfxM4dCopy(m, tmp2);
    }

    if (offset)
    {
        afxM4d tmp2;
        AfxM4dMultiplyAffine(tmp2, m, offset);
        AfxM4dCopy(m, tmp2);
    }
}

_AMX void AfxComputeAttachmentOffset(afxModel mdl, afxUnit jntIdx, afxPose const pose, afxM4d const offset, afxUnit const* sparseJntMap, afxUnit const* sparseJntMapRev, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    // void AfxSklGetSkeletonAttachmentOffset(afxSkeleton skl, afxUnit jntIdx, afxPose const pos, afxM4d const offset, afxM4d m, afxUnit const* sparseArtArray, afxUnit const* sparseArtArrayReverse)
    // AfxSklGetSkeletonAttachmentOffset(skl, jntIdx, atti, offset, m, sparseJntMap, sparseJntMapRev);

    afxM4d tmp;
    AfxComputeAttachmentWorldMatrix(mdl, jntIdx, pose, offset, sparseJntMap, sparseJntMapRev, tmp);
    AfxM4dInvertAffine(m, tmp); // invert it
}

_AMX void AfxBuildIndexedCompositeBufferTransposed(afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit cnt, afxReal buffer[][3][4])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
    
    // void AfxSklBuildIndexedCompositeBufferTransposed(afxSkeleton skl, afxPlacement const posb, afxUnit const* indices, afxUnit idxCnt, afxReal buffer[][3][4])
    // AfxSklBuildIndexedCompositeBufferTransposed(skl, pose, mdl->rigs[rigIdx].biasToJointMap, cnt, buffer);

    AFX_ASSERT(mdl->jntCnt >= cnt);
    AFX_ASSERT(plce);
    AFX_ASSERT(plce->matCnt >= cnt);
    AFX_ASSERT(buffer);

    afxUnit const* biasToJointMap = mdl->rigs[rigIdx].biasToJointMap;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AFX_ASSERT_RANGE(mdl->jntCnt, biasToJointMap[i], 1);
        AFX_ASSERT_RANGE(plce->matCnt, biasToJointMap[i], 1);
        BuildSingleCompositeFromWorldPoseTranspose_Generic(mdl->jntIw[biasToJointMap[i]], plce->world[biasToJointMap[i]], buffer[i]);
    }
}

_AMX void AfxBuildIndexedCompositeBuffer(afxModel mdl, afxUnit rigIdx, afxPlacement const plce, afxUnit cnt, afxM4d buffer[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // void AfxSklBuildIndexedCompositeBuffer(afxSkeleton skl, afxPlacement const wp, afxUnit const* indices, afxUnit idxCnt, afxM4d buffer[])
    // AfxSklBuildIndexedCompositeBuffer(skl, pose, mdl->rigs[rigIdx].biasToJointMap, cnt, buffer);

    AFX_ASSERT(mdl->jntCnt >= cnt);
    AFX_ASSERT(plce);
    AFX_ASSERT(plce->matCnt >= cnt);
    AFX_ASSERT(buffer);

    afxUnit const* biasToJointMap = mdl->rigs[rigIdx].biasToJointMap;

    for (afxUnit i = 0; i < cnt; i++)
    {
        AFX_ASSERT_RANGE(mdl->jntCnt, biasToJointMap[i], 1);
        AFX_ASSERT_RANGE(plce->matCnt, biasToJointMap[i], 1);
        BuildSingleCompositeFromWorldPose_Generic(mdl->jntIw[biasToJointMap[i]], plce->world[biasToJointMap[i]], buffer[i]);
    }
}

_AMX void AfxComputePlacement(afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxUnit baseReqJnt, afxUnit reqJntCnt, afxPose const pose, afxM4d const offset, afxBool noComposite, afxPlacement plce)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT_RANGE(plce->matCnt, baseJntIdx, jntCnt);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    afxTransform* lt = mdl->jntLt;
    afxM4d* iw = mdl->jntIw;
    afxM4d* w = plce->world;
    afxM4d* delta = plce->delta;

    if (!pose)
    {
        // compute the rest pose whether a attitude is not specified.
        // rest pose is computed for every joint ignoring baseReqJnt/reqJntCnt;
        // rest pose always compute composite delta matrices.
        // Should be compatible with void BuildRestPosture(const skeleton *Skeleton, int FirstBone, int BoneCount, const float *Offset4x4, world_pose *Result)
        // AfxSklComputeRestPosture(skl, baseJntIdx, jntCnt, offset, pose);

        for (afxUnit i = 0; i < jntCnt; i++)
        {
            afxUnit jointIdx = baseJntIdx + i;
            AFX_ASSERT_RANGE(mdl->jntCnt, jointIdx, 1);
            afxUnit parentIdx = mdl->jntPi[jointIdx];
            afxV4d const* parent;

            if (parentIdx == AFX_INVALID_INDEX) parent = offset;
            else
            {
                AFX_ASSERT_RANGE(mdl->jntCnt, parentIdx, 1);
                parent = w[parentIdx];
            }

            if (delta) // always computed if present for rest pose.
            {
                BWP_Dispatch(&lt[jointIdx], iw[jointIdx], delta[jointIdx], w[jointIdx], parent);
            }
            else
            {
                BWPNC_Dispatch(&lt[jointIdx], parent, w[jointIdx]);
            }
        }
    }
    else
    {
        AFX_ASSERT_RANGE(pose->artCnt, baseJntIdx, jntCnt);

        if (noComposite || !delta)
        {
            // Should be compatible with void BuildPostureNoCompositeLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const granny::local_pose *LocalAttitude, const float *Offset4x4, world_pose *Result)
            // AfxSklComputePostureNoCompositeLod(skl, baseJntIdx, jntCnt, baseReqJnt, reqJntCnt, atti, offset, pose);

            for (afxUnit i = 0; i < jntCnt; i++)
            {
                afxUnit jntIdx = baseJntIdx + i;
                AFX_ASSERT_RANGE(mdl->jntCnt, jntIdx, 1);
                afxUnit parentIdx = mdl->jntPi[jntIdx];
                afxV4d const* parent;

                if (parentIdx == AFX_INVALID_INDEX) parent = offset;
                else
                {
                    AFX_ASSERT_RANGE(mdl->jntCnt, parentIdx, 1);
                    parent = w[parentIdx];
                }

                afxTransform *t = &pose->arts[jntIdx].xform;

                if (reqJntCnt <= 0)
                    t = &lt[jntIdx];

                BWPNC_Dispatch(t, parent, w[jntIdx]);
                --reqJntCnt;
            }
        }
        else
        {
            // Should be compatible with void BuildPostureLOD(const skeleton *Skeleton, int FirstBone, int BoneCount, int FirstValidLocalBone, int ValidLocalBoneCount, const local_pose *LocalAttitude, const float *Offset4x4, world_pose *Result)
            // AfxSklComputePostureLod(skl, baseJntIdx, jntCnt, baseReqJnt, reqJntCnt, atti, offset, pose);

            for (afxUnit i = 0; i < jntCnt; i++)
            {
                afxUnit jntIdx = baseJntIdx + i;
                AFX_ASSERT_RANGE(mdl->jntCnt, jntIdx, 1);
                afxUnit parentIdx = mdl->jntPi[jntIdx];
                afxV4d const* parent;

                if (parentIdx == AFX_INVALID_INDEX) parent = offset;
                else
                {
                    AFX_ASSERT_RANGE(mdl->jntCnt, parentIdx, 1);
                    parent = w[parentIdx];
                }

                afxTransform *t = &pose->arts[jntIdx].xform;

                if (reqJntCnt <= 0)
                    t = &lt[jntIdx];

                BWP_Dispatch(t, iw[jntIdx], delta[jntIdx], w[jntIdx], parent);
                --reqJntCnt;
            }
        }
    }
}

_AMX void AfxComputeRestPose(afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxPose pose)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    // Should be compatible with void BuildRestLocalAttitude(const skeleton *Skeleton, int FirstBone, int BoneCount, local_pose *LocalAttitude)
    // AfxSklComputeRestLocalAttitude(skl, baseJntIdx, jntCnt, atti);
    AFX_ASSERT_RANGE(pose->artCnt, baseJntIdx, jntCnt);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, jntCnt);

    afxTransform* lt = mdl->jntLt;

    for (afxUnit i = jntCnt; i-- > 0;)
    {
        afxUnit jointIdx = baseJntIdx + i;
        AFX_ASSERT_RANGE(mdl->jntCnt, jointIdx, 1);
        akxArticulation* t = &pose->arts[jointIdx];
        t->cnt = 1;
        t->weight = 1.0;
        AfxCopyTransform(&t->xform, &lt[jointIdx]);
    }
}

_AMX void AfxRebuildPose(afxModel mdl, afxUnit baseJntIdx, afxUnit jntCnt, afxM4d const offset, afxPlacement const plce, afxBool rigid, afxPose pose)
{
    // Computes the local pose from the world pose.
    // If rigid is argumented as TRUE, the local pose will not include any scale/shear.

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, jntCnt);
    AFX_ASSERT_RANGE(pose->artCnt, baseJntIdx, jntCnt);
    AFX_ASSERT_RANGE(plce->matCnt, baseJntIdx, jntCnt);
    AFX_ASSERT(pose);
    AFX_ASSERT(plce);

    if (!offset)
        offset = AFX_M4D_IDENTITY;

    if (rigid)
    {
        //AfxSklLocalAttitudeFromPostureNoScale(skl, atti, pose, offset, baseJntIdx, jntCnt);
        // Should be compatible with void LocalAttitudeFromPostureNoScale(const skeleton *Skeleton, local_pose *Result, const world_pose *Posture, const float *Offset4x4, int FirstBone, int BoneCount)

        for (afxUnit i = jntCnt; i-- > 0;)
        {
            afxUnit jointIdx = baseJntIdx + i;
            afxUnit parentIdx = mdl->jntPi[jointIdx];
            afxM4d InverseParent;

            if (parentIdx == AFX_INVALID_INDEX) AfxM4dInvertAffine(InverseParent, offset);
            else
            {
                AFX_ASSERT_RANGE(mdl->jntCnt, parentIdx, 1);
                AfxM4dInvertAffine(InverseParent, plce->world[parentIdx]);
            }

            afxM4d Local;
            AfxM4dMultiplyAffine(Local, plce->world[jointIdx], InverseParent);
            afxTransform* t = AfxGetPoseTransform(pose, jointIdx);

            AfxV3dCopy(t->position, Local[3]);
            afxM3d Rotation;
            AfxM3dSet(Rotation, Local[0], Local[1], Local[2]);
            AfxQuatRotationFromM3d(t->orientation, Rotation);
            AfxQuatNormalize(t->orientation, t->orientation);
            AfxM3dReset(t->scaleShear);
            t->flags = afxTransformFlag_TRANSLATED | afxTransformFlag_ROTATED;
        }
    }
    else
    {
        // Should be compatible with void LocalAttitudeFromPosture(const skeleton *Skeleton, local_pose *Result, const world_pose *Posture, const float *Offset4x4, int FirstBone, int BoneCount)
        //AfxSklLocalAttitudeFromPosture(skl, atti, pose, offset, baseJntIdx, jntCnt);

        for (afxUnit i = jntCnt; i-- > 0;)
        {
            afxUnit jointIdx = baseJntIdx + i;
            AFX_ASSERT_RANGE(mdl->jntCnt, jointIdx, 1);
            afxUnit parentIdx = mdl->jntPi[jointIdx];
            afxM4d InverseParent;

            if (parentIdx == AFX_INVALID_INDEX) AfxM4dInvertAffine(InverseParent, offset);
            else
            {
                AFX_ASSERT_RANGE(mdl->jntCnt, parentIdx, 1);
                AfxM4dInvertAffine(InverseParent, plce->world[parentIdx]);
            }

            afxM4d Local;
            AfxM4dMultiplyAffine(Local, plce->world[jointIdx], InverseParent);

            afxM3d Linear;
            AfxM3dSet(Linear, Local[0], Local[1], Local[2]);

            afxM3d Q, Scale;

            if (!AfxPolarDecomposeM3d(Linear, 0.0000099999997, Q, Scale))
                AfxLogError("Can't accurately decompose MAX transform Q");

            afxV3d Position;
            AfxV3dCopy(Position, Local[3]);
            afxQuat Orientation;
            AfxQuatRotationFromM3d(Orientation, Q);
            AfxQuatNormalize(Orientation, Orientation);
            afxTransform* t = AfxGetPoseTransform(pose, jointIdx);
            AfxSetTransform(t, Position, Orientation, Scale, TRUE);
        }
    }
}






_AMX afxError _AmxMdlDtorCb(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    if (mdl->rigCnt)
        AfxRigMeshes(mdl, 0, mdl->rigCnt, NIL, NIL);

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntPi[0]),
            .var = (void**)&mdl->jntPi
        },
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntLt[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&mdl->jntLt
        },
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntIw[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&mdl->jntIw
        },
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntLe[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&mdl->jntLe
        },
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntId[0]),
            .var = (void**)&mdl->jntId
        },

        {
            .cnt = mdl->rigCnt,
            .siz = sizeof(mdl->rigs[0]),
            .var = (void**)&mdl->rigs
        }
    };

    if (AfxDeallocateInstanceData(mdl, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    AFX_ASSERT(!mdl->jntPi);
    AFX_ASSERT(!mdl->jntLe);
    AFX_ASSERT(!mdl->jntId);
    AFX_ASSERT(!mdl->jntLt);
    AFX_ASSERT(!mdl->jntIw);

    return err;
}

_AMX afxError _AmxMdlCtorCb(afxModel mdl, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxModelBlueprint const* mdlb = AFX_CAST(afxModelBlueprint const*, args[1]) + invokeNo;

    if (!AfxCatalogStrings(AfxGetModelUrnStringBase(sim), 1, &mdlb->urn.str, &mdl->urn))
    {
        AfxThrowError();
        return err;
    }

    afxTransform const* displacement = &mdlb->displacement;

    if (!displacement->flags)
        AfxResetTransform(&mdl->displacement);
    else
        AfxCopyTransform(&mdl->displacement, displacement);

    AfxBoxReset(&mdl->aabb);

    afxUnit rigCnt = AfxMax(1, mdlb->rigCnt);
    afxUnit baseMshIdx = 0;

    mdl->rigCnt = 0;
    mdl->rigs = NIL;

    afxUnit jntCnt = AfxMax(1, mdlb->jointCnt);
    afxUnit lodType = mdlb->lodType;
    mdl->jntCnt = jntCnt;
    mdl->lodType = lodType;

    mdl->allowedLodErrFadingFactor = /*_AmxGetAllowedLodErrorFadingFactor(sim);*/ 0.80000001;

    afxObjectStash stashes[] =
    {
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntPi[0]),
            .var = (void**)&mdl->jntPi
        },
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntLt[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&mdl->jntLt
        },
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntIw[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&mdl->jntIw
        },
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntLe[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&mdl->jntLe
        },
        {
            .cnt = mdl->jntCnt,
            .siz = sizeof(mdl->jntId[0]),
            .var = (void**)&mdl->jntId
        },

        {
            .cnt = rigCnt,
            .siz = sizeof(mdl->rigs[0]),
            .var = (void**)&mdl->rigs
        }
    };

    if (AfxAllocateInstanceData(mdl, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        AFX_ASSERT(mdl->jntPi);
        AFX_ASSERT(mdl->jntLt);
        AFX_ASSERT(mdl->jntIw);
        AFX_ASSERT(mdl->jntLe);
        AFX_ASSERT(mdl->jntId);
        AFX_ASSERT(mdl->rigs);

        mdl->udd = NIL;

        // reset joint parent indices to avoid crash
        AfxReparentJoints(mdl, 0, jntCnt, NIL, 0);

        // reset other joint attributes

        AfxResetJointMatrices(mdl, 0, jntCnt, NIL, 0);
        AfxResetJointTransforms(mdl, 0, jntCnt, NIL);
        AfxResetJointLodErrors(mdl, 0, jntCnt, NIL);
        mdl->jntIdd = NIL;
        mdl->jntUdd = NIL;

        // reset or catalog joint tags' strings.

        if (!mdlb->joints) AfxResetStrings(jntCnt, mdl->jntId);
        else if (!AfxCatalogStrings(AvxGetPivotTagStringBase(AfxGetSimulationDrawInput(sim)), jntCnt, mdlb->joints, mdl->jntId))
            AfxThrowError();

        AFX_ASSERT(mdl->jntCnt == jntCnt);

        AfxZero(mdl->rigs, rigCnt * sizeof(mdl->rigs[0]));
        mdl->rigCnt = rigCnt;

        AFX_ASSERT(mdl->rigCnt == rigCnt);

        if (err && AfxDeallocateInstanceData(mdl, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }

    if (!err)
    {
        afxString s;
        AfxGetModelUrn(mdl, &s);
        afxUnit jntCnt = AfxCountJoints(mdl, 0);
        AfxLogEcho("Skeletal Animable model <%.*s> assembled at %p. %u mesh rigs for %u joints.", AfxPushString(&s), mdl, mdl->rigCnt, jntCnt);
        AfxLogEcho("Listing %u joints..:", jntCnt);

        for (afxUnit i = 0; i < jntCnt; i++)
        {
            s = mdl->jntId[i];
            AfxLogEcho("    %3u <%.*s> %3u", i, AfxPushString(&s), mdl->jntPi[i]);
        }
    }
    return err;
}

_AMX afxClassConfig const _AMX_MDL_CLASS_CONFIG =
{
    .fcc = afxFcc_MDL,
    .name = "Model",
    .desc = "Skeletal Animable Model",
    .fixedSiz = sizeof(AFX_OBJECT(afxModel)),
    .ctor = (void*)_AmxMdlCtorCb,
    .dtor = (void*)_AmxMdlDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AMX void AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);
    AFX_ASSERT(models);

    for (afxUnit mdlIdx = 0; mdlIdx < cnt; mdlIdx++)
    {
        afxModel mdl = models[mdlIdx];

        if (!mdl)
            continue;
        
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        afxModelInfo mdli;
        AfxDescribeModel(mdl, &mdli);

        //AfxTransformSkeletons(ltm, iltm, ltTol, atv, atTol, 1, &skl);
        {
            afxUnit jntCnt = mdl->jntCnt;
            AfxAssimilateTransforms(ltm, iltm, atv, jntCnt, mdl->jntLt, mdl->jntLt);
            AfxAssimilateAtm4d(ltm, iltm, atv, jntCnt, mdl->jntIw, mdl->jntIw);
        }

        afxMesh meshes[64];
        afxUnit batchSiz = 64;
        afxUnit totalMshCnt = mdli.rigCnt;
        afxUnit batches = totalMshCnt / batchSiz;
        afxUnit remaining = totalMshCnt % batchSiz;
        afxUnit baseSlotIdx = 0;
        afxUnit mshCnt = 0;

        for (afxUnit j = 0; j < batches; j++)
        {
            mshCnt = AfxGetRiggedMeshes(mdl, baseSlotIdx, batchSiz, meshes);
            baseSlotIdx += mshCnt;

            // WARNING: What to do if mesh is shared among other models of strange asset?
            AfxTransformMeshes(ltm, iltm, ltTol, atv, atTol, flags, mshCnt, meshes);
        }

        if (remaining)
        {
            mshCnt = AfxGetRiggedMeshes(mdl, baseSlotIdx, remaining, meshes);
            // WARNING: What to do if mesh is shared among other models of strange asset?
            AfxTransformMeshes(ltm, iltm, ltTol, atv, atTol, flags, mshCnt, meshes);
        }

        AfxAssimilateTransforms(ltm, iltm, atv, 1, &mdl->displacement, &mdl->displacement);
    }
}

_AMX afxUnit AfxEnumerateModels(afxSimulation sim, afxUnit first, afxUnit cnt, afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = _AmxGetModelClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)models);
}

_AMX afxError AfxAssembleModels(afxSimulation sim, afxUnit cnt, afxModelBlueprint const blueprints[], afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(models);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AmxGetModelClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)models, (void const*[]) { sim, blueprints }))
        AfxThrowError();

    return err;
}
