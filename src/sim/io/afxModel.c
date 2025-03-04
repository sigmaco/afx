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

//#define _ASX_DRAW_C
#define _ASX_MODEL_C
#define _ASX_SKELETON_C
//#define _ASX_PLACEMENT_C
//#define _ASX_POSE_C
#include "../../draw/impl/avxImplementation.h"
#include "../impl/asxImplementation.h"

// IMPORTANT:
// Some models are not built from a mesh in the classic sense of the word. 
// Models structured out of Non-uniform rational basis spline (NURBS) also exit. 
// Technically you could build a model in many ways that do not involve a mesh in the classic sense of the word, such as atoms and Voxels. 
// When people speak of a mesh they normally refer to a polygon mesh.

_ASX afxChain* _AsxGetMeshRigs(afxMesh msh);

_ASX afxTransform* _AsxMdlGetLtArray(afxModel mdl, afxUnit base)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    return &mdl->jntLt[base];
}

_ASX afxM4d* _AsxMdlGetIwArray(afxModel mdl, afxUnit base)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    return &mdl->jntIw[base];
}

_ASX afxUnit* _AsxMdlGetPiArray(afxModel mdl, afxUnit base)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    return &mdl->jntPi[base];
}

_ASX afxReal* _AsxMdlGetLeArray(afxModel mdl, afxUnit base)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    return &mdl->jntLe[base];
}

_ASXINL afxBool AfxGetModelUrn(afxModel mdl, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(id);
    *id = mdl->urn;
    return mdl->urn.len;
}

_ASXINL void AfxDescribeModel(afxModel mdl, afxModelInfo* info)
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

_ASXINL void AfxDisplaceModel(afxModel mdl, afxTransform const* t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    if (!t)
        AfxResetTransform(&mdl->displacement);
    else
        AfxCopyTransform(&mdl->displacement, t);
}

_ASXINL void AfxGetModelDisplacement(afxModel mdl, afxTransform* t)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(t);
    AfxCopyTransform(&mdl->displacement, t);
}

_ASXINL void AfxComputeModelDisplacement(afxModel mdl, afxM4d m)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT(m);
    AfxComputeCompositeTransformM4d(&mdl->displacement, m);
}

// MESH RIGGING

_ASX void AfxDescribeMeshRig(afxModel mdl, afxUnit rigIdx, afxMeshRigInfo* info)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
    AFX_ASSERT(info);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    asxMeshRig* rig = mdl->rigs[rigIdx];
    if (!rig) *info = (afxMeshRigInfo) { 0 };
    else
    {
        info->flags = rig->flags;
        info->msh = rig->msh;
        info->biasToJnt = rig->biasToJntMap;
        info->biasFromJnt = rig->biasFromJntMap;
        info->origSkl = rig->skl;
        info->txd = rig->txd;
    }
}

_ASXINL afxBool AfxIsRiggedMeshTransplanted(afxModel mdl, afxUnit rigIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);
    
    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    asxMeshRig* rig = mdl->rigs[rigIdx];
    return (rig && (rig->flags & afxMeshRigFlag_TRANSPLANTED));
}

_ASXINL afxUnit AfxGetRiggedMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit rigCnt, afxMesh meshes[])
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

        asxMeshRig* rig = mdl->rigs[rigIdx];
        if (!rig) continue;
        afxMesh msh = rig->msh;

        if (msh)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);
            ++rslt;
        }
        meshes[i] = msh;
    }
    return rslt;
}

_ASX afxError _AsxPopMeshRig(asxMeshRig **pRig)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(pRig);

    asxMeshRig* rig = *pRig;

    afxMesh msh = rig->msh;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    if (rig->flags & afxMeshRigFlag_TRANSPLANTED)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &rig->skl);
        AfxDisposeObjects(1, &rig->skl);
        rig->flags &= ~afxMeshRigFlag_TRANSPLANTED;
    }

    if (rig->biasFromJntMap && (rig->biasFromJntMap != rig->biasToJntMap))
        AfxDeallocate((void**)&rig->biasFromJntMap, AfxHere());

    if (rig->biasToJntMap)
        AfxDeallocate((void**)&rig->biasToJntMap, AfxHere());

    if (rig->mtlToTxdMap)
        AfxDeallocate((void**)&rig->mtlToTxdMap, AfxHere());

    AfxPopPoolUnit(_AsxGetMeshRigPool(rig->msh), rig);
    AfxDisposeObjects(1, &msh);
    *pRig = NIL;

    return err;
}

_ASX afxError AfxRigMeshes(afxModel mdl, afxModel skl, afxUnit baseRigIdx, afxUnit mshCnt, afxMesh const meshes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, baseRigIdx, mshCnt);

    AFX_TRY_ASSERT_OBJECTS(afxFcc_MDL, 1, &skl);
    afxBool transplanted = skl && (skl != mdl);

    for (afxUnit i = 0; i < mshCnt; i++)
    {
        afxMesh msh = meshes ? meshes[i] : NIL;
        afxUnit rigIdx = baseRigIdx + i;
        asxMeshRig** pRig = &mdl->rigs[rigIdx];
        asxMeshRig* rig = *pRig;
        afxMesh mshCurr = NIL;

        if (rig)
        {
            mshCurr = rig->msh;

            if (msh == mshCurr)
                continue;
        }
        else if (!msh)
            continue;

        if (mshCurr) // detach operation
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &mshCurr);
            _AsxPopMeshRig(pRig);
            AFX_ASSERT(!mdl->rigs[rigIdx]);
            --mdl->usedRigCnt;
        }

        if (msh) // attach operation
        {
            AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

            afxMeshInfo mshi;
            AfxDescribeMesh(msh, &mshi);

            afxUnit biasCnt = mshi.biasCnt;
            afxUnit* biasFromJntMap = NIL;
            afxUnit* biasToJntMap = NIL;
            afxUnit* mtlToTxdMap = NIL;

            if (biasCnt)
            {
                if (AfxAllocate(biasCnt * sizeof(biasToJntMap[0]), 0, AfxHere(), (void**)&biasToJntMap)) AfxThrowError();
                else
                {
                    afxString* biasTags = AfxGetMeshBiasTags(msh, 0);
                    afxUnit biasMappedCnt = 0;

                    if (biasCnt != (biasMappedCnt = AsxFindJoints(mdl, biasCnt, biasTags, biasToJntMap)))
                    {
                        //AfxLogAdvertence("%u biases mapped to joints from %u", biasMappedCnt, biasCnt);
                    }

                    if (!transplanted) biasFromJntMap = biasToJntMap;
                    else if (AfxAllocate(biasCnt * sizeof(biasFromJntMap[0]), 0, AfxHere(), (void**)&biasFromJntMap)) AfxThrowError();
                    else
                    {
                        if (biasCnt != (biasMappedCnt = AsxFindJoints(skl, biasCnt, biasTags, biasFromJntMap)))
                        {
                            //AfxLogAdvertence("%u joints to biases mapped from %u", biasMappedCnt, biasCnt);
                        }
                    }
                }

                // on error, alloc's will be released at end
            }

            if (!err && mshi.mtlCnt)
            {
                if (AfxAllocate(mshi.mtlCnt * sizeof(mtlToTxdMap[0]), 0, AfxHere(), (void**)&mtlToTxdMap)) AfxThrowError();
                else
                {
                    afxString* materials = AfxGetMeshMaterials(msh, 0);
                    afxUnit mappedMtlCnt = 0;

                    if (mshi.mtlCnt != (mappedMtlCnt = AfxFindMaterialIndices(AfxGetProvider(msh), mshi.mtlCnt, materials, mtlToTxdMap)))
                    {

                    }
                }
            }

            if (err)
            {
                // on error condition, dealloc maps

                if (biasFromJntMap && (biasFromJntMap != biasToJntMap))
                    AfxDeallocate((void**)&biasFromJntMap, AfxHere());

                if (biasToJntMap)
                    AfxDeallocate((void**)&biasToJntMap, AfxHere());

                if (mtlToTxdMap)
                    AfxDeallocate((void**)&mtlToTxdMap, AfxHere());
            }
            else
            {
                afxUnit idx;
                asxMeshRig* rig = AfxPushPoolUnit(_AsxGetMeshRigPool(msh), &idx);
                if (!rig) AfxThrowError();
                else
                {
                    AfxReacquireObjects(1, &msh);
                    rig->msh = msh;
                    rig->rigId = idx;

                    rig->mdl = mdl;
                    rig->biasToJntMap = biasToJntMap;
                    rig->skl = skl;
                    rig->biasFromJntMap = biasFromJntMap;
                    rig->mtlToTxdMap = mtlToTxdMap;
                    rig->flags = NIL;

                    if (transplanted)
                    {
                        AfxReacquireObjects(1, &skl);
                        rig->flags |= afxMeshRigFlag_TRANSPLANTED;
                    }

                    mdl->rigs[rigIdx] = rig;
                    ++mdl->usedRigCnt;

                    // SOME DEBUG LOGGING

                    afxString s;
                    AfxGetMeshUrn(msh, &s);
                    AfxLogEcho("+-- rig #%u", rigIdx);
                    AfxLogEcho("|   +-- msh <%.*s> %p", AfxPushString(&s), msh);
                    AfxLogEcho("|   +-- biases-from/to-joints");
                    for (afxUnit i = 0; i < mshi.biasCnt; i++)
                    {
                        s = *AfxGetMeshBiasTags(msh, i);

                        if (transplanted)
                            AfxLogEcho("|   |   +-- <%.*s> @ <%u#%.*s>(%u#'%.*s')", AfxPushString(&s), rig->biasFromJntMap[i], AfxPushString(&skl->jntId[rig->biasFromJntMap[i]]), rig->biasToJntMap[i], AfxPushString(&mdl->jntId[rig->biasToJntMap[i]]));
                        else
                            AfxLogEcho("|   |   +-- <%.*s> @ %u#'%.*s'", AfxPushString(&s), rig->biasToJntMap[i], AfxPushString(&mdl->jntId[rig->biasToJntMap[i]]));
                    }
                }
            }
        }

        ++rigIdx;

        if (err)
        {
            // Pop previous successful mesh linkages

            for (afxUnit j = i; j-- > 0;)
            {
                if (_AsxPopMeshRig(&mdl->rigs[j]))
                    AfxThrowError();
                else
                    --mdl->usedRigCnt;
            }
            break;
        }
    }
    return err;
}

_ASXINL void AfxComputeRiggedMeshMatrices(afxModel mdl, afxUnit rigIdx, afxPlacement plce, afxUnit baseBiasIdx, afxUnit biasCnt, afxM4d matrices[])
{
    // Should be compatible with BuildMeshBinding4x4Array(rig, wp, baseJnt, jntCnt, matrix4x4Array)

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    asxMeshRig* rig = mdl->rigs[rigIdx];
    if (!rig) return;

    afxMesh msh = rig->msh;

    if (!msh) return;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxMeshInfo mshi;
    AfxDescribeMesh(msh, &mshi);

    AFX_ASSERT_RANGE(mshi.biasCnt, baseBiasIdx, biasCnt);
    // sanitize arguments
    baseBiasIdx = AfxMin(baseBiasIdx, mshi.biasCnt - 1);
    biasCnt = AfxMin(biasCnt, mshi.biasCnt - baseBiasIdx);

    afxM4d* w = _AsxPlceGetWorldArray(plce, 0);
    afxM4d* iw = rig->skl->jntIw;
    afxUnit const* biasToJntMap = rig->biasToJntMap;
    afxUnit const* biasFromJntMap = rig->biasFromJntMap;

    for (afxUnit i = 0; i < biasCnt; i++)
    {
        afxUnit biasIdx = baseBiasIdx + i;
        AfxM4dMultiplyAffine(matrices[i], iw[biasFromJntMap[i]], w[biasToJntMap[biasIdx]]);
    }
}

_ASXINL void AfxSetMeshRigTxd(afxModel mdl, afxUnit rigIdx, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    asxMeshRig* rig = mdl->rigs[rigIdx];
    if (!rig) return;

    afxMesh msh = rig->msh;

    if (!msh) return;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxMaterial curr;

    if (mtl != (curr = rig->txd))
    {
        if (mtl)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
            AfxReacquireObjects(1, &mtl);
        }

        rig->txd = mtl;

        if (curr)
        {
            AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &curr);
            AfxDisposeObjects(1, &curr);
        }
    }
}

_ASXINL afxBool AfxFindMeshRigMaterial(afxModel mdl, afxUnit rigIdx, afxString const* id, afxMaterial* material)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    // sanitize arguments
    rigIdx = AfxMin(rigIdx, mdl->rigCnt - 1);

    asxMeshRig* rig = mdl->rigs[rigIdx];
    if (!rig) return FALSE;

    afxMesh msh = rig->msh;

    if (!msh) return FALSE;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxMaterial txd = rig->txd;
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

_ASX afxUnit AsxGetJointParents(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void* indices, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);
    AFX_ASSERT(indices);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    switch (stride)
    {
        case sizeof(afxUnit8) :
        {
            afxUnit8* dst = indices;
            for (afxUnit i = 0; i < cnt; i++) dst[i] = mdl->jntPi[baseJntIdx + i];
            break;
        }
        case sizeof(afxUnit16) :
        {
            afxUnit16* dst = indices;
            for (afxUnit i = 0; i < cnt; i++) dst[i] = mdl->jntPi[baseJntIdx + i];
            break;
        }
        case sizeof(afxUnit32) :
        {
            afxUnit32* dst = indices;
            for (afxUnit i = 0; i < cnt; i++) dst[i] = mdl->jntPi[baseJntIdx + i];
            break;
        }
        default: AfxThrowError(); break;
    }
    return cnt;
}

_ASX afxError AsxReparentJoints(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* indices, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);
    //AFX_ASSERT(indices);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    if (!indices) for (afxUnit i = 0; i < cnt; i++) mdl->jntPi[baseJntIdx + i] = AFX_INVALID_INDEX;
    else switch (stride)
    {
        case sizeof(afxUnit8) :
        {
            afxUnit8 const* src = indices;

            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit parentIdx = src[i];
                AfxAssertOR(parentIdx == AFX_INVALID_INDEX, parentIdx < mdl->jntCnt);
                mdl->jntPi[baseJntIdx + i] = parentIdx;
            }
            break;
        }
        case sizeof(afxUnit16) :
        {
            afxUnit16 const* src = indices;

            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit parentIdx = src[i];
                AfxAssertOR(parentIdx == AFX_INVALID_INDEX, parentIdx < mdl->jntCnt);
                mdl->jntPi[baseJntIdx + i] = parentIdx;
            }
            break;
        }
        case sizeof(afxUnit32) :
        {
            afxUnit32 const* src = indices;

            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit parentIdx = src[i];
                AfxAssertOR(parentIdx == AFX_INVALID_INDEX, parentIdx < mdl->jntCnt);
                mdl->jntPi[baseJntIdx + i] = parentIdx;
            }
            break;
        }
        default: AfxThrowError(); break;
    }
    return err;
}

_ASX afxUnit AsxGetJointInversors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void* matrices, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);
    AFX_ASSERT(matrices);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    switch (stride)
    {
    case 0:
    case sizeof(afxM4d):
    {
        afxM4d* dst = matrices;

        for (afxUnit i = 0; i < cnt; i++)
            AfxM4dCopy(dst[i], mdl->jntIw[baseJntIdx + i]);

        break;
    }
    case sizeof(afxAtm3d) :
    {
        afxAtm3d* dst = (afxAtm3d*)matrices;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxV3dCopy(dst[i][0], mdl->jntIw[baseJntIdx + i][0]);
            AfxV3dCopy(dst[i][1], mdl->jntIw[baseJntIdx + i][1]);
            AfxV3dCopy(dst[i][2], mdl->jntIw[baseJntIdx + i][2]);
            AfxV3dCopy(dst[i][3], mdl->jntIw[baseJntIdx + i][3]);
        }
        break;
    }
    case sizeof(afxM3d) :
    {
        afxM3d* dst = (afxM3d*)matrices;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxM3dCopyM4d(dst[i], mdl->jntIw[baseJntIdx + i]);
        }
        break;
    }
    default:
    {
        AfxThrowError();
        break;
    }
    }
    return cnt;
}

_ASX afxError AsxResetJointInversors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* matrices, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    if (!matrices) for (afxUnit i = 0; i < cnt; i++) AfxM4dReset(mdl->jntIw[baseJntIdx + i]);
    else switch (stride)
    {
    case 0:
    case sizeof(afxM4d) :
    {
        afxM4d const* src = matrices;

        for (afxUnit i = 0; i < cnt; i++)
            AfxM4dCopy(mdl->jntIw[baseJntIdx + i], src[i]);

        break;
    }
    case sizeof(afxAtm3d) :
    {
        afxAtm3d const* src = (afxAtm3d const*)matrices;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxV4dCopyAtv3d(mdl->jntIw[baseJntIdx + i][0], src[i][0]);
            AfxV4dCopyAtv3d(mdl->jntIw[baseJntIdx + i][1], src[i][1]);
            AfxV4dCopyAtv3d(mdl->jntIw[baseJntIdx + i][2], src[i][2]);
            AfxV4dCopyAtv3d(mdl->jntIw[baseJntIdx + i][3], src[i][3]);
        }
        break;
    }
    case sizeof(afxM3d) :
    {
        afxM3d const* src = (afxM3d const*)matrices;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxM4dCopyM3d(mdl->jntIw[baseJntIdx + i], src[i], AFX_V4D_IDENTITY);
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

_ASX afxUnit AsxGetJointTransforms(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxTransform transforms[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);
    AFX_ASSERT(transforms);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    for (afxUnit i = 0; i < cnt; i++)
        AfxCopyTransform(&transforms[i], &mdl->jntLt[baseJntIdx + i]);

    return cnt;
}

_ASX afxError AsxResetJointTransforms(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxTransform const transforms[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    if (!transforms) for (afxUnit i = 0; i < cnt; i++) AfxResetTransform(&mdl->jntLt[baseJntIdx + i]);
    else for (afxUnit i = 0; i < cnt; i++)
        AfxCopyTransform(&mdl->jntLt[baseJntIdx + i], &transforms[i]);

    return err;
}

_ASX afxUnit AsxGetJointLodErrors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxReal lodErrors[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);
    AFX_ASSERT(lodErrors);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    for (afxUnit i = 0; i < cnt; i++)
        lodErrors[i] = mdl->jntLe[baseJntIdx + i];

    return cnt;
}

_ASX afxError AsxResetJointLodErrors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxReal const lodErrors[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->jntCnt, baseJntIdx, cnt);

    cnt = AfxMin(cnt, mdl->jntCnt - baseJntIdx);

    if (!lodErrors) for (afxUnit i = 0; i < cnt; i++) mdl->jntLe[baseJntIdx + i] = -1.0;
    else for (afxUnit i = 0; i < cnt; i++)
        mdl->jntLe[baseJntIdx + i] = lodErrors[i];

    return err;
}

_ASXINL void AsxQueryModelErrorTolerance(afxModel mdl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler)
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

_ASX afxUnit AsxCountJoints(afxModel mdl, afxReal allowedErr)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    afxUnit rslt = 0;

    if (allowedErr == 0.0 || !mdl->lodType) rslt = mdl->jntCnt;
    else
    {
        afxReal errEnd, errScaler;
        AsxQueryModelErrorTolerance(mdl, allowedErr, &errEnd, &errScaler);

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

_ASX afxUnit AsxFindJoints(afxModel mdl, afxUnit cnt, afxString const ids[], afxUnit indices[])
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

_ASX void AfxBuildRiggedMeshCompositeMatrices(afxModel mdl, afxUnit rigIdx, afxPlacement plce, afxUnit cnt, afxM4d matrices[])
{
    // parametrized wrapper
    // void AfxSklBuildIndexedCompositeBuffer(afxSkeleton skl, afxPlacement const wp, afxUnit const* indices, afxUnit idxCnt, afxM4d buffer[])
    // AfxSklBuildIndexedCompositeBuffer(skl, pose, mdl->rigs[rigIdx].biasToJointMap, cnt, buffer);

    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);
    AFX_ASSERT_RANGE(mdl->rigCnt, rigIdx, 1);

    AFX_ASSERT(mdl->jntCnt >= cnt);
    AFX_ASSERT(plce);
    afxUnit plceCap = AfxGetPlacementCapacity(plce);
    AFX_ASSERT(plceCap >= cnt);
    AFX_ASSERT(matrices);

    asxMeshRig* rig = mdl->rigs[rigIdx];
    if (!rig) return;

    afxMesh msh = rig->msh;

    if (!msh) return;
    AFX_ASSERT_OBJECTS(afxFcc_MSH, 1, &msh);

    afxUnit const* biasToJntMap = rig->biasToJntMap;
#if 0
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit jntIdx = biasToJntMap[i];
        AFX_ASSERT_RANGE(mdl->jntCnt, jntIdx, 1);
        AFX_ASSERT_RANGE(plce->matCnt, jntIdx, 1);
        BuildSingleCompositeFromWorldPose_Generic(mdl->jntIw[jntIdx], plce->world[jntIdx], matrices[i]);
    }
#else
    AfxBuildIndexedCompositeMatrices(plce, mdl, cnt, biasToJntMap, FALSE, matrices);
#endif
}






_ASX afxError _AsxMdlDtorCb(afxModel mdl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    if (mdl->rigCnt)
        AfxRigMeshes(mdl, NIL, 0, mdl->rigCnt, NIL);

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

_ASX afxError _AsxMdlCtorCb(afxModel mdl, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxModelBlueprint const* mdlb = AFX_CAST(afxModelBlueprint const*, args[1]) + invokeNo;

    if (!AfxCatalogStrings(_AsxGetModelUrnStringBase(sim), 1, &mdlb->urn.str, &mdl->urn))
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

    mdl->allowedLodErrFadingFactor = /*_AsxGetAllowedLodErrorFadingFactor(sim);*/ 0.80000001;

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
        AsxReparentJoints(mdl, 0, jntCnt, NIL, 0);

        // reset other joint attributes

        AsxResetJointInversors(mdl, 0, jntCnt, NIL, 0);
        AsxResetJointTransforms(mdl, 0, jntCnt, NIL);
        AsxResetJointLodErrors(mdl, 0, jntCnt, NIL);
        mdl->jntIdd = NIL;
        mdl->jntUdd = NIL;

        // reset or catalog joint tags' strings.

        if (!mdlb->joints) AfxResetStrings(jntCnt, mdl->jntId);
        else if (!AfxCatalogStrings(_AsxGetPivotTagStringBase(sim), jntCnt, mdlb->joints, mdl->jntId))
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
        afxUnit jntCnt = AsxCountJoints(mdl, 0);
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

_ASX afxClassConfig const _ASX_MDL_CLASS_CONFIG =
{
    .fcc = afxFcc_MDL,
    .name = "Model",
    .desc = "Skeletal Animable Model",
    .fixedSiz = sizeof(AFX_OBJECT(afxModel)),
    .ctor = (void*)_AsxMdlCtorCb,
    .dtor = (void*)_AsxMdlDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX void AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltTol, afxV3d const atv, afxReal atTol, afxFlags flags, afxUnit cnt, afxModel models[])
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

_ASX afxUnit AfxEnumerateModels(afxSimulation sim, afxUnit first, afxUnit cnt, afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxClass const* cls = _AsxGetModelClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)models);
}

_ASX afxError AfxAssembleModels(afxSimulation sim, afxUnit cnt, afxModelBlueprint const blueprints[], afxModel models[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(blueprints);
    AFX_ASSERT(models);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AsxGetModelClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)models, (void const*[]) { sim, blueprints }))
        AfxThrowError();

    return err;
}
