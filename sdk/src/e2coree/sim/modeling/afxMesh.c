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
#define _AFX_MESH_C
#define _AFX_VERTEX_DATA_C
#define _AFX_MESH_TOPOLOGY_C
#include "qwadro/sim/modeling/afxMesh.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxMatrix.h"
#include "qwadro/sim/modeling/afxMeshTopology.h"
#include "qwadro/sim/modeling/awxVertexData.h"
#include "qwadro/core/afxIndexedString.h"

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

_AFX afxMeshPivot* AfxGetMeshPivot(afxMesh msh, afxNat pvtIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->pivotCnt, pvtIdx, 1);
    afxMeshPivot *mshv = &msh->pivots[pvtIdx];
    AfxAssertType(mshv, afxFcc_MSHV);
    return mshv;
}

_AFX afxBool AfxGetMeshPivotId(afxMesh msh, afxNat pvtIdx, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->pivotCnt, pvtIdx, 1);
    AfxAssert(id);
    afxBool rslt = TRUE;

    afxMeshPivot *mshv = &msh->pivots[pvtIdx];
    AfxAssertType(mshv, afxFcc_MSHV);

    if (mshv->idStrIdx != AFX_INVALID_INDEX)
        rslt = !!AfxResolveStrings(msh->strc, 1, &mshv->idStrIdx, id);
    else
        AfxResetString(id);

    return rslt;
}

_AFX afxNat AfxCountMeshVertebras(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->pivotCnt;
}

_AFX afxBool AfxMeshIsDeformable(afxMesh msh)
{
    // The vertices in a Qwadro mesh are either deforming or rigid. 
    // Deforming meshes are ones that are bound to multiple bones, whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return (1 < msh->pivotCnt);
}

_AFX afxMeshTopology AfxGetMeshTopology(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxMeshTopology msht = msh->topology;
    AfxTryAssertObjects(1, &msht, afxFcc_MSHT);        
    return msht;
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

_AFX awxVertexData AfxGetMeshVertices(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    awxVertexData vtd = msh->vtd;
    AfxTryAssertObjects(1, &vtd, afxFcc_VTD);
    return vtd;
}

_AFX afxBool AfxGetMeshId(afxMesh msh, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssert(id);
    afxBool rslt = TRUE;

    if (msh->idStrIdx != AFX_INVALID_INDEX)
        rslt = !!AfxResolveStrings(msh->strc, 1, &msh->idStrIdx, id);
    else
        AfxResetString(id);

    return rslt;
}

_AFX afxError _AfxMshDtor(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxEntry("msh=%p", msh);

    afxSimulation sim = AfxGetObjectProvider(msh);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    for (afxNat i = 0; i < msh->pivotCnt; i++)
    {
        afxMeshPivot *msha = &msh->pivots[i];

        //if (msha->boneName)
            //AfxDeallocateString(&msha->id);
    }

    if (msh->pivots)
        AfxDeallocate(mmu, msh->pivots);

    if (msh->topology)
    {
        AfxAssertObjects(1, &msh->topology, afxFcc_MSHT);
        AfxRelinkMeshTopology(msh, NIL);
    }

    if (msh->morphs)
        AfxDeallocate(mmu, msh->morphs);

    if (msh->vtd)
        AfxReleaseObjects(1, (void*[]) { msh->vtd });

    //if (msh->uri)

    if (msh->strc)
        AfxReleaseObjects(1, (void*[]) { msh->strc });

    return err;
}

_AFX afxError _AfxMshCtor(afxMesh msh, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxEntry("msh=%p", msh);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxStringCatalog strc = cookie->udd[1];
    awxVertexData const* vtdDatas = cookie->udd[2];
    awxVertexData vtd = vtdDatas[cookie->no];
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    afxMeshTopology const* topologies = cookie->udd[3];
    afxMeshTopology msht = topologies[cookie->no];
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    afxMeshBlueprint const* mshb = cookie->udd[4];

    if (!(msh->strc = strc))
        msh->idStrIdx = AFX_INVALID_INDEX;
    else if (AfxCatalogStrings(strc, 1, &mshb->id.str.str, &msh->idStrIdx, 0))
        AfxThrowError();
    else
        AfxReacquireObjects(1, (void*[]) { strc });

    AfxMakeFixedString32(&msh->id, &mshb->id.str.str);


    afxMmu mmu = AfxGetSimulationMmu(sim);

    msh->vtd = vtd;
    AfxReacquireObjects(1, (void*[]) { vtd });
    afxNat vtxCnt = vtd->vtxCnt;

    msh->topology = msht;
    AfxReacquireObjects(1, (void*[]) { msht });
    afxNat triCnt = AfxCountMeshTriangles(msht);
    afxIndexedTriangle *tris = AfxGetMeshTriangles(msht, 0);

    for (afxNat i = 0; i < triCnt; i++)
    {
        if ((vtxCnt < tris[i][0]) || (vtxCnt < tris[i][1]) || (vtxCnt < tris[i][2]))
        {
            AfxThrowError();
            AfxAssertRange(vtxCnt, tris[i][0], 1);
            AfxAssertRange(vtxCnt, tris[i][1], 1);
            AfxAssertRange(vtxCnt, tris[i][2], 1);
        }
    }

    if (!err)
    {
        afxNat mtlCnt = mshb->mtlCnt;
        //msh->mtlCnt = mtlCnt;

        afxNat pivotCnt = mshb->pivotCnt;
        afxNat biasCnt = AwxCountVertexBiases(vtd);
        afxVertexBias const* biases = AwxGetVertexBiases(vtd, 0);

        for (afxNat i = 0; i < biasCnt; i++)
        {
            AfxAssert(1.0 >= biases[i].weight);

            if (pivotCnt < biases[i].pivotIdx)
            {
                AfxThrowError();
                AfxAssertRange(pivotCnt, biases[i].pivotIdx, 1);
            }
        }

        if (!err)
        {
            msh->pivotCnt = pivotCnt;
            msh->pivots = NIL;

            if (pivotCnt && !(msh->pivots = AfxAllocate(mmu, pivotCnt, sizeof(msh->pivots[0]), 0, AfxHint()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < pivotCnt; i++)
                {
                    afxMeshPivot *mshv = &msh->pivots[i];
                    AfxAssignTypeFcc(mshv, afxFcc_MSHV);

                    mshv->triCnt = 0;
                    mshv->tris = NIL; // AfxAllocate(mmu, mshv->triCnt * sizeof(mshv->triIdx[0]), NIL, AfxHint());

                    if (!strc)
                        mshv->idStrIdx = AFX_INVALID_INDEX;
                    else if (AfxCatalogStrings(strc, 1, &mshb->pivots[i], &mshv->idStrIdx, 0))
                        AfxThrowError();

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
                                afxVertexBias* bias = &msh->vtd->biases[v->baseBiasIdx + k];

                                if (bias->pivotIdx == h)
                                {
                                    afxNat idx;
                                    AfxInsertArrayUnits(&skinnedTris, 1, &idx, &tris[i][j]);
                                    ++msh->pivots[bias->pivotIdx].triCnt;
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
                    afxMeshPivot *mshv = &msh->pivots[i];
                    AfxAssignTypeFcc(mshv, afxFcc_MSHV);

                    mshv->tris = AfxGetArrayUnit(&skinnedTris, baseSkinnedTriIdx);
                    baseSkinnedTriIdx += mshv->triCnt;
                }
#endif
            }
            msh->extData = NIL;

            if (err)
            {
                for (afxNat i = msh->pivotCnt; i-- > 0;)
                {
                    afxMeshPivot *mshv = &msh->pivots[i];
                    AfxAssertType(mshv, afxFcc_MSHV);

                    if (mshv->tris)
                        AfxDeallocate(mmu, mshv->tris);
                }

                if (msh->pivots) // what if tris is set?
                    AfxDeallocate(mmu, msh->pivots);
            }
        }
    }

    if (err)
    {
        AfxReleaseObjects(1, (void*[]) { vtd });
        AfxReleaseObjects(1, (void*[]) { msht });
    }

    if (!err)
    {
        afxChar const* echStr = "%.*s Mesh %p assembled. Id: \"%.*s\"\n    %u vertices with %u attributes.\n    %u triangles (%u bytes per index) arranged in %u surfaces.\n    %u vertebras\n";
        AfxEcho(echStr, AfxPushString(msh->pivotCnt > 1 ? &AfxStaticString("Skinned") : &AfxStaticString("Rigid")), msh, AfxPushString(&msh->id.str.str), vtd->vtxCnt, vtd->attrCnt, msht->triCnt, AfxDetermineMeshIndexSize(msht), msht->surfCnt, msh->pivotCnt);
    }
    return err;
}

_AFX afxClassConfig _AfxMshClsConfig =
{
    .fcc = afxFcc_MSH,
    .name = "Mesh",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxMesh)),
    .mmu = NIL,
    .ctor = (void*)_AfxMshCtor,
    .dtor = (void*)_AfxMshDtor
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX void AfxTransformMeshes(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal linearTol, afxReal const atv[4], afxReal affineTol, afxFlags flags, afxNat cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(meshes);
    //AfxThrowError();
    (void)affineTol;
    (void)linearTol;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMesh msh2 = meshes[i];

        if (msh2)
        {
            AfxAssertObjects(1, &msh2, afxFcc_MSH);

            awxVertexData vtd = AfxGetMeshVertices(msh2);
            AfxTryAssertObjects(1, &vtd, afxFcc_VTD);

            AwxTransformVertexDatas(ltm, iltm, atv, flags & 1, 1, &vtd); // sinalizado para renormalizar normals

            afxNat morphCnt = AfxCountMeshMorphes(msh2);

            for (afxNat j = 0; j < morphCnt; j++)
            {
                afxMeshMorph* mshm = AfxGetMeshMorph(msh2, j);
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

            afxMeshTopology msht;

            if ((flags & 2) && (AfxDetM3d(ltm) < 0.0) && (msht = AfxGetMeshTopology(msh2))) // se sinalizado para reordenar índices de faces
                AfxInvertMeshWinding(msht);

            afxNat artCnt = AfxCountMeshVertebras(msh2);

            for (afxNat j = 0; j < artCnt; j++)
            {
                afxMeshPivot* mshv = AfxGetMeshPivot(msh2, j);
                AfxTransformAabbs(ltm, atv, 1, &mshv->aabb, &mshv->aabb);
            }
        }
    }
}

_AFX void AfxRenormalizeMeshes(afxNat cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(meshes);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMesh msh2 = meshes[i];
        AfxAssertObjects(1, &msh2, afxFcc_MSH);
        awxVertexData vtd = msh2->vtd;
        afxMeshTopology msht = msh2->topology;

        afxString const attrs[] =
        {
            AfxString("posn"),
            AfxString("nrm")
        };
        afxNat attrIdx[2];
        AwxFindVertexDataAttributes(vtd, 2, attrs, attrIdx);
        afxV4d* posn = AwxExposeVertexData(vtd, attrIdx[0], 0);
        afxV3d* nrm = AwxExposeVertexData(vtd, attrIdx[1], 0);

        afxIndexedTriangle const* tris = AfxGetMeshTriangles(msht, 0);
        afxNat triCnt = AfxCountMeshTriangles(msht);
        afxNat vtxCnt = AfxCountMeshVertices(msh2);

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

_AFX afxError AfxAssembleMeshes(afxSimulation sim, afxStringCatalog strc, awxVertexData const datas[], afxMeshTopology const topologies[], afxNat cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(blueprints);
    AfxAssert(meshes);
    AfxAssert(cnt);

    if (AfxAcquireObjects(AwxGetMeshClass(sim), cnt, (afxObject*)meshes, (void const*[]) { sim, strc, datas, topologies, blueprints }))
        AfxThrowError();

    return err;
}

_AFX afxError AfxBuildMeshes(afxSimulation sim, afxStringCatalog strc, afxNat cnt, afxMeshBuilder const builders[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(cnt);
    AfxAssert(builders);
    AfxAssert(meshes);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMeshBuilder const* mshb = &builders[i];
        awxVertexData vtd = AwxBuildVertexData(sim, strc, mshb);
        afxMeshTopology msht = AfxBuildMeshTopology(sim, mshb, 0, mshb->surfCnt);

        afxMeshBlueprint blueprint = { 0 };
        blueprint.vtxDataIdx = 0;
        blueprint.topologyIdx = 0;
        blueprint.pivotCnt = mshb->artCnt;
        blueprint.pivots = mshb->pivots;
        AfxMakeFixedString32(&blueprint.id, &mshb->id.str.str);

        if (AfxAssembleMeshes(sim, strc, &vtd, &msht, 1, &blueprint, &meshes[i]))
        {
            AfxThrowError();
            AfxReleaseObjects(i, (void**)meshes);
            break;
        }
        AfxReleaseObjects(1, (void*[]) { msht });
        AfxReleaseObjects(1, (void*[]) { vtd });
    }

    return err;
}
