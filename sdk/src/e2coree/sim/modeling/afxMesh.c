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
#define _AFX_MESH_C
#define _AFX_VERTEX_DATA_C
#define _AFX_MESH_TOPOLOGY_C
#include "afx/sim/modeling/afxMesh.h"
#include "afx/sim/afxSimulation.h"
#include "afx/math/afxMatrix.h"
#include "afx/sim/modeling/afxMeshTopology.h"
#include "afx/sim/modeling/afxVertexData.h"

#define MaxNumWeights 8

struct WeightVertex
{
    afxNat8 BoneWeights[MaxNumWeights];
    afxNat8 BoneIndices[MaxNumWeights];
    afxNat BonesUsed;
};

struct TriWeightData
{
    afxNat jntCnt;
    afxNat8 jntIdx[MaxNumWeights * 3];
    afxIndexedTriangle vtxIdx;
};

////////////////////////////////////////////////////////////////////////////////
// MESH                                                                       //
////////////////////////////////////////////////////////////////////////////////

#if 0
_AFX void _AfxMesh113131(afxMesh msh)
{
    afxNat* OriginalIndices = msh->topology->vtxIdx;
    
    afxNat const NumMeshTris = AfxCountMeshTriangles(msh->topology);
    struct TriWeightData* TriWeights = AfxAllocate(NIL, sizeof(TriWeights[0]), NumMeshTris, 0, AfxHint());
    
    for (afxNat triIdx = 0; triIdx < NumMeshTris; ++triIdx)
    {
        struct TriWeightData* TriData = &TriWeights[triIdx];
        TriData->jntCnt = 0;
        
        for (afxNat vtxIdx = 0; vtxIdx < 3; ++vtxIdx)
        {
            TriData->vtxIdx[vtxIdx] = OriginalIndices[triIdx * 3 + vtxIdx];
            
            afxVertexBias* bias = &msh->vtd->biases[OriginalIndices[triIdx * 3 + vtxIdx]];
            
            for (afxNat vtxWgtIdx = 0; vtxWgtIdx < bias->weightCnt; ++vtxWgtIdx)
            {
                afxVertexWeight* w = &msh->vtd->weights[bias->baseWeightIdx + vtxWgtIdx];
                
                if (AfxFind(TriData->jntIdx, TriData->jntIdx + TriData->jntCnt, w->pivotIdx) == TriData->jntIdx + TriData->jntCnt)
                {
                    TriData->jntIdx[TriData->jntCnt++] = w->pivotIdx;
                }
            }
        }
    }
}
#endif

_AFX afxMeshVertebra* AfxGetMeshVertebra(afxMesh msh, afxNat artIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->vertebraCnt, artIdx, 1);
    afxMeshVertebra *mshv = &msh->vertebras[artIdx];
    AfxAssertType(mshv, afxFcc_MSHV);
    return mshv;
}

_AFX afxNat AfxCountMeshVertebras(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->vertebraCnt;
}

_AFX afxBool AfxMeshIsDeformable(afxMesh msh)
{
    // The vertices in a Qwadro mesh are either deforming or rigid. 
    // Deforming meshes are ones that are bound to multiple bones, whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return (1 < msh->vertebraCnt);
}

_AFX void AfxSetMeshMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->mtlSlotCnt, mtlIdx, 1);

    if (mtl)
    {
        AfxAssertObjects(1, &mtl, afxFcc_MTL);
        AfxReacquireObjects(1, (void*[]) { mtl });
    }

    afxMaterial mtl2 = msh->mtlSlots[mtlIdx].mtl;
    msh->mtlSlots[mtlIdx].mtl = mtl;
    
    if (mtl2)
    {
        AfxAssertObjects(1, &mtl2, afxFcc_MTL);
        AfxReleaseObjects(1, (void*[]) { mtl2 });
    }
}

_AFX afxBool AfxFindMeshMaterial(afxMesh msh, afxUri const* id, afxMaterial* mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertType(id, afxFcc_URI);

    for (afxNat i = 0; i < msh->mtlSlotCnt; i++)
    {
        afxMaterial mtl2 = msh->mtlSlots[i].mtl;

        if (mtl2)
        {
            AfxAssertObjects(1, &mtl2, afxFcc_MTL);

            if (0 == AfxCompareUri(AfxGetMaterialUri(mtl2), id))
            {
                if (mtl)
                    *mtl = mtl2;

                return TRUE;
            }
        }
    }
    return FALSE;
}

_AFX afxBool AfxGetMeshMaterial(afxMesh msh, afxNat mtlIdx, afxMaterial* mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->mtlSlotCnt, mtlIdx, 1);
    afxMaterial mtl2 = msh->mtlSlots[mtlIdx].mtl;
    
    if (mtl)
    {
        AfxTryAssertObjects(1, &mtl2, afxFcc_MTL);
        *mtl = mtl2;
    }

    return !!mtl2;
}

_AFX afxNat AfxCountMeshMaterials(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->mtlSlotCnt;
}

_AFX afxBool AfxGetMeshTopology(afxMesh msh, afxMeshTopology* msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxMeshTopology msht2 = msh->topology;

    if (msht)
    {
        AfxAssertObjects(1, &msht2, afxFcc_MSHT);
        *msht = msht2;
    }

    return !!msht2;
}

_AFX void AfxRelinkMeshTopology(afxMesh msh, afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    if (msht)
    {
        AfxAssertObjects(1, &msht, afxFcc_MSHT);
        AfxReacquireObjects(1, (void*[]) { msht });
    }

    afxMeshTopology msht2 = msh->topology;
    msh->topology = msht;

    if (msht2)
    {
        AfxAssertObjects(1, &msht2, afxFcc_MSHT);
        AfxReleaseObjects(1, (void*[]) { msht2 });
    }    
}

_AFX afxMeshMorph* AfxGetMeshMorph(afxMesh msh, afxNat morphIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, morphIdx, 1);
    afxMeshMorph *mshm = &msh->morphs[morphIdx];
    AfxAssertType(mshm, afxFcc_MSHM);
    return mshm;
}

_AFX afxNat AfxCountMeshMorphes(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->morphCnt;
}

_AFX afxNat AfxCountMeshVertices(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->vtd->vtxCnt;
}

_AFX afxVertexData AfxGetMeshVertices(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxVertexData vtd = msh->vtd;
    AfxTryAssertObjects(1, &vtd, afxFcc_VTD);
    return vtd;
}

_AFX afxString const* AfxGetMeshId(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return &msh->id;
}

_AFX afxError _AfxMshDtor(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxEntry("msh=%p", msh);

    afxSimulation sim = AfxGetObjectProvider(msh);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxContext mem = AfxSimulationGetMemory(sim);

    for (afxNat i = 0; i < msh->vertebraCnt; i++)
    {
        afxMeshVertebra *msha = &msh->vertebras[i];

        //if (msha->boneName)
            //AfxDeallocateString(&msha->id);
    }

    if (msh->vertebras)
        AfxDeallocate(mem, msh->vertebras);

    for (afxNat i = 0; i < msh->mtlSlotCnt; i++)
        AfxSetMeshMaterial(msh, i, NIL);

    if (msh->mtlSlots)
        AfxDeallocate(mem, msh->mtlSlots);

    if (msh->topology)
    {
        AfxAssertObjects(1, &msh->topology, afxFcc_MSHT);
        AfxRelinkMeshTopology(msh, NIL);
    }

    if (msh->morphs)
        AfxDeallocate(mem, msh->morphs);

    if (msh->vtd)
        AfxReleaseObjects(1, (void*[]) { msh->vtd });

    //if (msh->uri)
    AfxDeallocateString(&msh->id);

    return err;
}

_AFX afxError _AfxMshCtor(afxMesh msh, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxEntry("msh=%p", msh);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMeshBuilder const* mshb = ((afxMeshBuilder const*)cookie->udd[1]) + cookie->no;
    AfxAssert(mshb);
    AfxAssertType(mshb, afxFcc_MSHB);

    afxContext mem = AfxSimulationGetMemory(sim);

    AfxResetString(&msh->id);
    AfxCloneString(&msh->id, &mshb->id);

    msh->vtd = AfxBuildVertexData(sim, mshb);
    afxNat vtxCnt = msh->vtd->vtxCnt;

    msh->topology = AfxBuildMeshTopology(sim, mshb, 0, mshb->surfCnt);
    afxNat idxCnt = AfxCountMeshIndices(msh->topology);
    afxIndexedTriangle *tris = msh->topology->tris;
    afxNat triCnt = AfxCountMeshTriangles(msh->topology);

    for (afxNat i = 0; i < triCnt; i++)
    {
        AfxAssertRange(vtxCnt, tris[i][0], 1);
        AfxAssertRange(vtxCnt, tris[i][1], 1);
        AfxAssertRange(vtxCnt, tris[i][2], 1);
    }

    afxNat mtlCnt = mshb->surfCnt;
    msh->mtlSlotCnt = mtlCnt;
    msh->mtlSlots = NIL;

    if (mtlCnt && !(msh->mtlSlots = AfxAllocate(mem, sizeof(msh->mtlSlots[0]), mtlCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < mtlCnt; i++)
        {
            afxMaterialSlot *slot = &msh->mtlSlots[i];
            AfxAssert(slot);
            slot->mtl = NIL;
        }
        AfxAssert(mtlCnt == msh->mtlSlotCnt);

        afxNat pivotCnt = mshb->artCnt;
        msh->vertebraCnt = pivotCnt;
        msh->vertebras = NIL;

        if (pivotCnt && !(msh->vertebras = AfxAllocate(mem, sizeof(msh->vertebras[0]), pivotCnt, 0, AfxHint()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < pivotCnt; i++)
            {
                afxMeshVertebra *mshv = &msh->vertebras[i];
                AfxAssignTypeFcc(mshv, afxFcc_MSHV);

                mshv->triCnt = 0;
                mshv->tris = NIL; // AfxAllocate(mem, mshv->triCnt * sizeof(mshv->triIdx[0]), NIL, AfxHint());

                AfxString16(&mshv->id);
                afxString const *str = &mshb->pivots[i];
                AfxCopyString(&mshv->id.str, str);

                AfxResetAabb(&mshv->aabb);
            }
#if 0
            afxArray skinnedTris;
            AfxAllocateArray(&skinnedTris, triCnt, sizeof(afxIndexedTriangle), NIL);
            AfxReserveArraySpace(&skinnedTris, triCnt);

            for (afxNat h = 0; h < pivotCnt; h++)
            {
                for (afxNat i = 0; i < triCnt; i++)
                {
                    for (afxNat j = 0; j < 3; j++)
                    {
                        afxVertex const* v = &msh->vtd->vtx[tris[i][j]];

                        for (afxNat k = 0; k < v->biasCnt; k++)
                        {
                            afxVertexBias* bias = &msh->vtd->biases[v->baseBiasIdx + k];

                            if (bias->pivotIdx == h)
                            {
                                afxNat idx;
                                AfxInsertArrayUnits(&skinnedTris, 1, &idx, &tris[i][j]);
                                ++msh->vertebras[bias->pivotIdx].triCnt;
                                j = 3;
                                break;
                            }
                        }
                    }
                }
            }

            afxNat baseSkinnedTriIdx = 0;

            for (afxNat i = 0; i < pivotCnt; i++)
            {
                afxMeshVertebra *mshv = &msh->vertebras[i];
                AfxAssignTypeFcc(mshv, afxFcc_MSHV);

                mshv->tris = AfxGetArrayUnit(&skinnedTris, baseSkinnedTriIdx);
                baseSkinnedTriIdx += mshv->triCnt;
            }
#endif
        }
        msh->extData = NIL;

        if (err)
        {
            for (afxNat i = msh->vertebraCnt; i-- > 0;)
            {
                afxMeshVertebra *mshv = &msh->vertebras[i];
                AfxAssertType(mshv, afxFcc_MSHV);

                if (mshv->tris)
                    AfxDeallocate(mem, mshv->tris);
            }

            if (msh->vertebras) // what if tris is set?
                AfxDeallocate(mem, msh->vertebras);
        }

        if (err)
        {
            for (afxNat i = msh->mtlSlotCnt; i-- > 0;)
            {
                afxMaterialSlot *slot = &msh->mtlSlots[i];

                if (slot->mtl)
                {
                    AfxAssertObjects(1, &slot->mtl, afxFcc_MTL);
                    AfxReleaseObjects(1, (void*[]) { slot->mtl });
                }
            }

            if (msh->mtlSlots)
                AfxDeallocate(mem, msh->mtlSlots);
        }
    }

    if (!err)
    {
        afxChar const* echStr = "%.*s Mesh %p assembled. Id: \"%.*s\"\n    %u vertices with %u attributes.\n    %u triangles (%u bytes per index) arranged in %u surfaces.\n    %u material slots, %u vertebras\n";
        AfxEcho(echStr, AfxPushString(msh->vertebraCnt > 1 ? &AfxStaticString("Skinned") : &AfxStaticString("Rigid")), msh, AfxPushString(&msh->id), msh->vtd->vtxCnt, msh->vtd->attrCnt, msh->topology->triCnt, AfxDetermineMeshIndexSize(msh->topology), msh->topology->surfCnt, msh->mtlSlotCnt, msh->vertebraCnt);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxBuildMeshes(afxSimulation sim, afxNat cnt, afxMeshBuilder const mshb[], afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(cnt);
    AfxAssert(mshb);
    AfxAssert(msh);

    if (AfxAcquireObjects(AfxGetMeshClass(sim), cnt, (afxHandle*)msh, (void*[]) { sim, (void*)mshb }))
        AfxThrowError();

    return err;
}

_AFX void AfxTransformMeshes(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(msh);
    //AfxThrowError();
    (void)affineTol;
    (void)linearTol;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMesh msh2 = msh[i];

        if (msh2)
        {
            AfxAssertObjects(1, &msh2, afxFcc_MSH);

            afxVertexData vtd = AfxGetMeshVertices(msh2);
            AfxTryAssertObjects(1, &vtd, afxFcc_VTD);

            AfxTransformVertexDatas(ltm, iltm, atv, flags & 1, 1, &vtd); // sinalizado para renormalizar normals

            afxNat morphCnt = AfxCountMeshMorphes(msh2);

            for (afxNat j = 0; j < morphCnt; j++)
            {
                afxMeshMorph* mshm = AfxGetMeshMorph(msh2, j);
                vtd = mshm->vtd;
                AfxTryAssertObjects(1, &vtd, afxFcc_VTD);
                AfxAssert(!mshm->delta || AfxGetVertexAttributeFlags(vtd, 0) & afxVertexFlag_DELTA);

                if (mshm->delta)
                {
                    AfxTransformVertexDatas(ltm, iltm, atv, FALSE, 1, &vtd);
                }
                else
                {
                    AfxTransformVertexDatas(ltm, iltm, atv, flags & 1, 1, &vtd);
                }
            }

            afxMeshTopology msht;

            if ((flags & 2) && (AfxDetM3d(ltm) < 0.0) && AfxGetMeshTopology(msh2, &msht)) // se sinalizado para reordenar índices de faces
                AfxInvertMeshWinding(msht);

            afxNat artCnt = AfxCountMeshVertebras(msh2);

            for (afxNat j = 0; j < artCnt; j++)
            {
                afxMeshVertebra* mshv = AfxGetMeshVertebra(msh2, j);
                AfxTransformAabbs(ltm, atv, 1, &mshv->aabb, &mshv->aabb);
            }
        }
    }
}

_AFX void AfxRenormalizeMeshes(afxNat cnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(msh);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMesh msh2 = msh[i];
        AfxAssertObjects(1, &msh2, afxFcc_MSH);
        afxVertexData vtd = msh2->vtd;
        afxMeshTopology msht = msh2->topology;

        afxString const attrs[] =
        {
            AfxString("posn"),
            AfxString("nrm")
        };
        afxNat attrIdx[2];
        AfxFindVertexDataAttributes(vtd, 2, attrs, attrIdx);
        afxV4d* posn = AfxExposeVertexData(vtd, attrIdx[0], 0);
        afxV3d* nrm = AfxExposeVertexData(vtd, attrIdx[1], 0);

        afxIndexedTriangle const* tris = AfxGetMeshTriangles(msht, 0);
        afxNat triCnt = AfxCountMeshTriangles(msht);
        afxNat vtxCnt = AfxCountMeshVertices(msh2);

        AfxZeroVertexData(vtd, attrIdx[1], 0, vtxCnt);

        for (afxNat j = 0; j < triCnt; j++)
        {
            afxNat const ia = tris[j][0];
            afxNat const ib = tris[j][1];
            afxNat const ic = tris[j][2];

            afxV4d const e1, e2, no;
            AfxSubV4d(e1, posn[ia], posn[ib]);
            AfxSubV4d(e2, posn[ic], posn[ib]);
            AfxCrossV3d(no, e1, e2);

            AfxAddV3d(nrm[ia], nrm[ia], no);
            AfxAddV3d(nrm[ib], nrm[ib], no);
            AfxAddV3d(nrm[ic], nrm[ic], no);
        }

        AfxNormalizeVertexData(vtd, attrIdx[1], 0, vtxCnt);
    }
}

_AFX afxClassConfig _AfxMshClsConfig =
{
    .fcc = afxFcc_MSH,
    .name = "Mesh",
    .unitsPerPage = 8,
    .size = sizeof(AFX_OBJECT(afxMesh)),
    .ctx = NIL,
    .ctor = (void*)_AfxMshCtor,
    .dtor = (void*)_AfxMshDtor
};
