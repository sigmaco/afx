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
#include "qwadro/mem/afxMappedString.h"

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
_AKX void _AfxMesh113131(afxMesh msh)
{
    afxNat* OriginalIndices = msh->topology->vtxIdx;
    
    afxNat const NumMeshTris = AfxCountMeshTriangles(msh->topology);
    struct TriWeightData* TriWeights = AfxAllocate(sizeof(TriWeights[0]), NumMeshTris, 0, AfxHere());
    
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

_AKX afxMeshBias* AfxGetMeshBiases(afxMesh msh, afxNat baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->biasCnt, baseBiasIdx, 1);
    afxMeshBias *mshv = &msh->biasData[baseBiasIdx];
    AfxAssertType(mshv, afxFcc_MSHV);
    return mshv;
}

_AKX afxBool AfxGetMeshBiasName(afxMesh msh, afxNat biasIdx, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->biasCnt, biasIdx, 1);
    AfxAssert(id);
    return AfxResolveStrings2(msh->strb, 1, &msh->biasName[biasIdx], id);
}

_AKX afxNat AfxCountMeshBiases(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->biasCnt;
}

_AKX afxBool AfxMeshIsDeformable(afxMesh msh)
{
    // The vertices in a Qwadro mesh are either deforming or rigid. 
    // Deforming meshes are ones that are bound to multiple bones, whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return (1 < msh->biasCnt);
}

_AKX afxMeshTopology AfxGetMeshTopology(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxMeshTopology msht = msh->topology;
    AfxTryAssertObjects(1, &msht, afxFcc_MSHT);        
    return msht;
}

_AKX void AfxRelinkMeshTopology(afxMesh msh, afxMeshTopology msht)
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

_AKX afxMeshMorph* AfxGetMeshMorphes(afxMesh msh, afxNat baseMorphIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, baseMorphIdx, 1);
    afxMeshMorph *mshm = &msh->morphs[baseMorphIdx];
    AfxAssertType(mshm, afxFcc_MSHM);
    return mshm;
}

_AKX afxNat AfxCountMeshMorphes(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->morphCnt;
}

_AKX afxNat AfxCountMeshVertices(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return msh->vtd->vtxCnt;
}

_AKX awxVertexData AfxGetMeshVertices(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    awxVertexData vtd = msh->vtd;
    AfxTryAssertObjects(1, &vtd, afxFcc_VTD);
    return vtd;
}

_AKX afxBool AfxGetMeshId(afxMesh msh, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssert(id);
    return AfxResolveStrings2(msh->strb, 1, &msh->id, id);
}

_AKX afxError _AfxMshDtor(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    
    afxSimulation sim = AfxGetObjectProvider(msh);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMmu mmu = AfxGetSimulationMmu(sim);

    if (msh->biasName)
        AfxDeallocate(msh->biasName);

    if (msh->biasData)
        AfxDeallocate(msh->biasData);

    if (msh->topology)
    {
        AfxAssertObjects(1, &msh->topology, afxFcc_MSHT);
        AfxRelinkMeshTopology(msh, NIL);
    }

    if (msh->morphs)
        AfxDeallocate(msh->morphs);

    if (msh->vtd)
        AfxReleaseObjects(1, (void*[]) { msh->vtd });

    //if (msh->uri)

    if (msh->strb)
        AfxReleaseObjects(1, (void*[]) { msh->strb });

    return err;
}

_AKX afxError _AfxMshCtor(afxMesh msh, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    afxSimulation sim = cookie->udd[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxMeshBlueprint const* mshb = cookie->udd[1];

    afxStringBase strb = mshb->strb;
    awxVertexData vtd = mshb->vertices;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    afxMeshTopology msht = mshb->topology;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);

    if (!(msh->strb = strb))
        AfxResetString(&msh->id);
    else if (!AfxCatalogStrings2(strb, 1, &mshb->id.str.str, &msh->id))
        AfxThrowError();
    else
        AfxReacquireObjects(1, (void*[]) { strb });

    afxMmu mmu = AfxGetSimulationMmu(sim);

    msh->vtd = vtd;
    AfxReacquireObjects(1, (void*[]) { vtd });
    afxNat vtxCnt = vtd->vtxCnt;
    msh->vtxCnt = vtxCnt;
    msh->baseVtx = 0;

    msh->topology = msht;
    AfxReacquireObjects(1, (void*[]) { msht });
    afxNat triCnt = AfxCountMeshTriangles(msht);
    afxIndexedTriangle *tris = AfxGetMeshTriangles(msht, 0);

    for (afxNat i = 0; i < triCnt; i++)
    {
        AfxAssertRange(vtxCnt, tris[i][0], 1);
        AfxAssertRange(vtxCnt, tris[i][1], 1);
        AfxAssertRange(vtxCnt, tris[i][2], 1);

        if ((vtxCnt < tris[i][0]) || (vtxCnt < tris[i][1]) || (vtxCnt < tris[i][2]))
        {
            AfxThrowError();
        }
    }

    if (!err)
    {
        afxNat mtlCnt = mshb->mtlCnt;
        //msh->mtlCnt = mtlCnt;

        afxNat biasCnt = mshb->biasCnt;

        if (!err)
        {
            msh->biasCnt = biasCnt;
            msh->biasName = NIL;
            msh->biasData = NIL;

            if (biasCnt && !(msh->biasName = AfxAllocate(biasCnt, sizeof(msh->biasName[0]), 0, AfxHere()))) AfxThrowError();
            else
            {
                if (!strb || !mshb->pivots)
                {
                    for (afxNat i = 0; i < biasCnt; i++)
                        AfxResetString(&msh->biasName[i]);
                }
                else if (!AfxCatalogStrings2(strb, biasCnt, mshb->pivots, msh->biasName))
                    AfxThrowError();
            }

            msh->extData = NIL;

            if (err && msh->biasName)
                AfxDeallocate(msh->biasName);
        }
    }

    if (err)
    {
        AfxReleaseObjects(1, (void*[]) { vtd });
        AfxReleaseObjects(1, (void*[]) { msht });
    }

    if (!err)
    {
        afxString s;

        if (!msh->strb)
            AfxResetString(&s);
        else
            AfxResolveStrings2(msh->strb, 1, &msh->id, &s);

        AfxLogEcho("%.*s Mesh <%.*s> assembled. %p\n    %u vertices with %u attributes.\n    %u triangles (%u bytes per index) arranged in %u surfaces.\n    Listing %u biases:",
            AfxPushString(msh->biasCnt > 1 ? &AfxStaticString("Skinned") : &AfxStaticString("Rigid")),
            AfxPushString(&s), msh, vtd->vtxCnt, vtd->attrCnt, msht->triCnt, AfxDetermineMeshIndexSize(msht), msht->surfCnt, msh->biasCnt
        );

        for (afxNat i = 0; i < msh->biasCnt; i++)
        {
            if (!msh->strb)
                AfxResetString(&s);
            else
                AfxResolveStrings2(msh->strb, 1, &(msh->biasName[i]), &s);

            AfxDbgLogf(0xFF, AfxHere(), "\n    %u <%.*s> %u", i, AfxPushString(&s), msh->biasData ? msh->biasData[i].triCnt : 0);
        }
    }
    return err;
}

_AKX afxClassConfig _AfxMshMgrCfg =
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

_AKX afxError AfxAssembleMeshes(afxSimulation sim, afxNat cnt, afxMeshBlueprint const blueprints[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(blueprints);
    AfxAssert(meshes);
    AfxAssert(cnt);

    if (AfxAcquireObjects(AwxGetMeshClass(sim), cnt, (afxObject*)meshes, (void const*[]) { sim, blueprints }))
        AfxThrowError();

    return err;
}

_AKX afxError AfxBuildMeshes(afxSimulation sim, afxStringBase strb, afxNat cnt, afxMeshBuilder const builders[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(cnt);
    AfxAssert(builders);
    AfxAssert(meshes);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMeshBuilder const* mshb = &builders[i];
        awxVertexData vtd = AwxBuildVertexData(sim, strb, mshb);
        afxMeshTopology msht = AfxBuildMeshTopology(sim, mshb, 0, mshb->surfCnt);

        afxMeshBlueprint blueprint = { 0 };
        blueprint.vertices = vtd;
        blueprint.topology = msht;
        blueprint.biasCnt = mshb->artCnt;
        blueprint.pivots = mshb->pivots;
        blueprint.strb = strb;
        AfxMakeString32(&blueprint.id, &mshb->id.str.str);

        if (AfxAssembleMeshes(sim, 1, &blueprint, &meshes[i]))
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

_AKX void AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxNat cnt, afxMesh meshes[])
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
                afxMeshMorph* mshm = AfxGetMeshMorphes(msh2, j);
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

            afxNat artCnt = AfxCountMeshBiases(msh2);

            for (afxNat j = 0; j < artCnt; j++)
            {
                afxMeshBias* mshv = AfxGetMeshBiases(msh2, j);
                AfxTransformObbs(ltm, atv, 1, &mshv->aabb, &mshv->aabb);
            }
        }
    }
}

_AKX void AfxRenormalizeMeshes(afxNat cnt, afxMesh meshes[])
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
        AwxFindVertexAttributes(vtd, 2, attrs, attrIdx);
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
