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
#include "qwadro/sim/modeling/awxMesh.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/sim/modeling/awxMeshTopology.h"
#include "qwadro/sim/modeling/awxVertexData.h"

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
_AFX void _AfxMesh113131(awxMesh msh)
{
    afxNat* OriginalIndices = msh->topology->vtxIdx;
    
    afxNat const NumMeshTris = AwxCountMeshTriangles(msh->topology);
    struct TriWeightData* TriWeights = AfxAllocate(NIL, sizeof(TriWeights[0]), NumMeshTris, 0, AfxHint());
    
    for (afxNat triIdx = 0; triIdx < NumMeshTris; ++triIdx)
    {
        struct TriWeightData* TriData = &TriWeights[triIdx];
        TriData->jntCnt = 0;
        
        for (afxNat vtxIdx = 0; vtxIdx < 3; ++vtxIdx)
        {
            TriData->vtxIdx[vtxIdx] = OriginalIndices[triIdx * 3 + vtxIdx];
            
            awxVertexBias* bias = &msh->vtd->biases[OriginalIndices[triIdx * 3 + vtxIdx]];
            
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

_AFX awxMeshVertebra* AwxGetMeshVertebra(awxMesh msh, afxNat artIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->vertebraCnt, artIdx, 1);
    awxMeshVertebra *mshv = &msh->vertebras[artIdx];
    AfxAssertType(mshv, afxFcc_MSHV);
    return mshv;
}

_AFX afxNat AwxCountMeshVertebras(awxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->vertebraCnt;
}

_AFX afxBool AwxMeshIsDeformable(awxMesh msh)
{
    // The vertices in a Qwadro mesh are either deforming or rigid. 
    // Deforming meshes are ones that are bound to multiple bones, whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return (1 < msh->vertebraCnt);
}

_AFX afxBool AwxGetMeshTopology(awxMesh msh, awxMeshTopology* msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    awxMeshTopology msht2 = msh->topology;

    if (msht)
    {
        AfxAssertObjects(1, &msht2, afxFcc_MSHT);
        *msht = msht2;
    }

    return !!msht2;
}

_AFX void AwxRelinkMeshTopology(awxMesh msh, awxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    if (msht)
    {
        AfxAssertObjects(1, &msht, afxFcc_MSHT);
        AfxReacquireObjects(1, (void*[]) { msht });
    }

    awxMeshTopology msht2 = msh->topology;
    msh->topology = msht;

    if (msht2)
    {
        AfxAssertObjects(1, &msht2, afxFcc_MSHT);
        AfxReleaseObjects(1, (void*[]) { msht2 });
    }    
}

_AFX awxMeshMorph* AwxGetMeshMorph(awxMesh msh, afxNat morphIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, morphIdx, 1);
    awxMeshMorph *mshm = &msh->morphs[morphIdx];
    AfxAssertType(mshm, afxFcc_MSHM);
    return mshm;
}

_AFX afxNat AwxCountMeshMorphes(awxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->morphCnt;
}

_AFX afxNat AwxCountMeshVertices(awxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->vtd->vtxCnt;
}

_AFX awxVertexData AwxGetMeshVertices(awxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    awxVertexData vtd = msh->vtd;
    AfxTryAssertObjects(1, &vtd, afxFcc_VTD);
    return vtd;
}

_AFX afxString const* AwxGetMeshId(awxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return &msh->id.str;
}

_AFX afxError _AfxMshDtor(awxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxEntry("msh=%p", msh);

    afxSimulation sim = AfxGetObjectProvider(msh);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMmu mmu = AfxSimulationGetMemory(sim);

    for (afxNat i = 0; i < msh->vertebraCnt; i++)
    {
        awxMeshVertebra *msha = &msh->vertebras[i];

        //if (msha->boneName)
            //AfxDeallocateString(&msha->id);
    }

    if (msh->vertebras)
        AfxDeallocate(mmu, msh->vertebras);

    if (msh->topology)
    {
        AfxAssertObjects(1, &msh->topology, afxFcc_MSHT);
        AwxRelinkMeshTopology(msh, NIL);
    }

    if (msh->morphs)
        AfxDeallocate(mmu, msh->morphs);

    if (msh->vtd)
        AfxReleaseObjects(1, (void*[]) { msh->vtd });

    //if (msh->uri)
    AfxDeallocateString(&msh->id);

    return err;
}

_AFX afxError _AfxMshCtor(awxMesh msh, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxEntry("msh=%p", msh);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    awxMeshBuilder const* mshb = ((awxMeshBuilder const*)cookie->udd[1]) + cookie->no;
    AfxAssert(mshb);
    AfxAssertType(mshb, afxFcc_MSHB);

    afxMmu mmu = AfxSimulationGetMemory(sim);

    AfxResetBufferedString(&msh->id);
    AfxCloneString(&msh->id, &mshb->id);

    msh->vtd = AwxBuildVertexData(sim, mshb);
    afxNat vtxCnt = msh->vtd->vtxCnt;

    msh->topology = AfxBuildMeshTopology(sim, mshb, 0, mshb->surfCnt);
    afxNat idxCnt = AwxCountMeshIndices(msh->topology);
    afxIndexedTriangle *tris = msh->topology->tris;
    afxNat triCnt = AwxCountMeshTriangles(msh->topology);

    for (afxNat i = 0; i < triCnt; i++)
    {
        AfxAssertRange(vtxCnt, tris[i][0], 1);
        AfxAssertRange(vtxCnt, tris[i][1], 1);
        AfxAssertRange(vtxCnt, tris[i][2], 1);
    }

    afxNat mtlCnt = mshb->surfCnt;

    afxNat pivotCnt = mshb->artCnt;
    msh->vertebraCnt = pivotCnt;
    msh->vertebras = NIL;

    if (pivotCnt && !(msh->vertebras = AfxAllocate(mmu, sizeof(msh->vertebras[0]), pivotCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < pivotCnt; i++)
        {
            awxMeshVertebra *mshv = &msh->vertebras[i];
            AfxAssignTypeFcc(mshv, afxFcc_MSHV);

            mshv->triCnt = 0;
            mshv->tris = NIL; // AfxAllocate(mmu, mshv->triCnt * sizeof(mshv->triIdx[0]), NIL, AfxHint());

            AfxMakeFixedString32(&mshv->id, 0);
            afxString const *str = &mshb->pivots[i];
            AfxCopyString(&mshv->id.str, str);

            AfxResetAabb(mshv->aabb);
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
                        awxVertexBias* bias = &msh->vtd->biases[v->baseBiasIdx + k];

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
            awxMeshVertebra *mshv = &msh->vertebras[i];
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
            awxMeshVertebra *mshv = &msh->vertebras[i];
            AfxAssertType(mshv, afxFcc_MSHV);

            if (mshv->tris)
                AfxDeallocate(mmu, mshv->tris);
        }

        if (msh->vertebras) // what if tris is set?
            AfxDeallocate(mmu, msh->vertebras);
    }

    if (!err)
    {
        afxChar const* echStr = "%.*s Mesh %p assembled. Id: \"%.*s\"\n    %u vertices with %u attributes.\n    %u triangles (%u bytes per index) arranged in %u surfaces.\n    %u vertebras\n";
        AfxEcho(echStr, AfxPushString(msh->vertebraCnt > 1 ? &AfxStaticString("Skinned") : &AfxStaticString("Rigid")), msh, AfxPushString(&msh->id.str), msh->vtd->vtxCnt, msh->vtd->attrCnt, msh->topology->triCnt, AfxDetermineMeshIndexSize(msh->topology), msh->topology->surfCnt, msh->vertebraCnt);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AwxBuildMeshes(afxSimulation sim, afxNat cnt, awxMeshBuilder const mshb[], awxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(cnt);
    AfxAssert(mshb);
    AfxAssert(msh);

    if (AfxAcquireObjects(AwxGetMeshClass(sim), cnt, (afxObject*)msh, (void const*[]) { sim, (void*)mshb }))
        AfxThrowError();

    return err;
}

_AFX void AwxTransformMeshes(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, awxMesh msh[])
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
        awxMesh msh2 = msh[i];

        if (msh2)
        {
            AfxAssertObjects(1, &msh2, afxFcc_MSH);

            awxVertexData vtd = AwxGetMeshVertices(msh2);
            AfxTryAssertObjects(1, &vtd, afxFcc_VTD);

            AwxTransformVertexDatas(ltm, iltm, atv, flags & 1, 1, &vtd); // sinalizado para renormalizar normals

            afxNat morphCnt = AwxCountMeshMorphes(msh2);

            for (afxNat j = 0; j < morphCnt; j++)
            {
                awxMeshMorph* mshm = AwxGetMeshMorph(msh2, j);
                vtd = mshm->vtd;
                AfxTryAssertObjects(1, &vtd, afxFcc_VTD);
                AfxAssert(!mshm->delta || AwxGetVertexAttributeFlags(vtd, 0) & awxVertexFlag_DELTA);

                if (mshm->delta)
                {
                    AwxTransformVertexDatas(ltm, iltm, atv, FALSE, 1, &vtd);
                }
                else
                {
                    AwxTransformVertexDatas(ltm, iltm, atv, flags & 1, 1, &vtd);
                }
            }

            awxMeshTopology msht;

            if ((flags & 2) && (AfxDetM3d(ltm) < 0.0) && AwxGetMeshTopology(msh2, &msht)) // se sinalizado para reordenar índices de faces
                AfxInvertMeshWinding(msht);

            afxNat artCnt = AwxCountMeshVertebras(msh2);

            for (afxNat j = 0; j < artCnt; j++)
            {
                awxMeshVertebra* mshv = AwxGetMeshVertebra(msh2, j);
                AfxTransformAabbs(ltm, atv, 1, &mshv->aabb, &mshv->aabb);
            }
        }
    }
}

_AFX void AwxRenormalizeMeshes(afxNat cnt, awxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(msh);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        awxMesh msh2 = msh[i];
        AfxAssertObjects(1, &msh2, afxFcc_MSH);
        awxVertexData vtd = msh2->vtd;
        awxMeshTopology msht = msh2->topology;

        afxString const attrs[] =
        {
            AfxString("posn"),
            AfxString("nrm")
        };
        afxNat attrIdx[2];
        AwxFindVertexDataAttributes(vtd, 2, attrs, attrIdx);
        afxV4d* posn = AwxExposeVertexData(vtd, attrIdx[0], 0);
        afxV3d* nrm = AwxExposeVertexData(vtd, attrIdx[1], 0);

        afxIndexedTriangle const* tris = AwxGetMeshTriangles(msht, 0);
        afxNat triCnt = AwxCountMeshTriangles(msht);
        afxNat vtxCnt = AwxCountMeshVertices(msh2);

        AwxZeroVertexData(vtd, attrIdx[1], 0, vtxCnt);

        for (afxNat j = 0; j < triCnt; j++)
        {
            afxNat const ia = tris[j][0];
            afxNat const ib = tris[j][1];
            afxNat const ic = tris[j][2];

            afxV4d e1, e2, no;
            AfxSubV4d(e1, posn[ia], posn[ib]);
            AfxSubV4d(e2, posn[ic], posn[ib]);
            AfxCrossV3d(no, e1, e2);

            AfxAddV3d(nrm[ia], nrm[ia], no);
            AfxAddV3d(nrm[ib], nrm[ib], no);
            AfxAddV3d(nrm[ic], nrm[ic], no);
        }

        AwxNormalizeVertexData(vtd, attrIdx[1], 0, vtxCnt);
    }
}

_AFX afxClassConfig _AfxMshClsConfig =
{
    .fcc = afxFcc_MSH,
    .name = "Mesh",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(awxMesh)),
    .mmu = NIL,
    .ctor = (void*)_AfxMshCtor,
    .dtor = (void*)_AfxMshDtor
};
