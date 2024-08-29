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

#define _AVX_DRAW_C
#define _AVX_MESH_C
#include "../dev/AvxImplKit.h"

////////////////////////////////////////////////////////////////////////////////

_AVX afxBool AfxGetMeshUrn(afxMesh msh, afxString* id)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssert(id);
    AfxReflectString(&msh->urn, id);
    return msh->urn.len;
}

_AVX afxNat AfxCountMeshMorphs(afxMesh msh, afxNat baseMorphIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, baseMorphIdx, 1);
    return msh->morphCnt - baseMorphIdx;
}

_AVX afxNat AfxCountMeshVertices(afxMesh msh, afxNat morphIdx, afxNat baseVtxIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, morphIdx, 1);
    AfxAssert(msh->morphs);
    akxMeshMorph* mshm = &msh->morphs[morphIdx];
    AfxAssertRange(mshm->vtxCnt, baseVtxIdx, 1);
    return mshm->vtxCnt - baseVtxIdx;
}

_AVX afxNat AfxCountMeshBiases(afxMesh msh, afxNat baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->biasCnt, baseBiasIdx, 1);
    return msh->biasCnt - baseBiasIdx;
}

_AVX afxBool AfxMeshIsDeformable(afxMesh msh)
{
    // The vertices in a Qwadro mesh are either deforming or rigid. 
    // Deforming meshes are ones that are bound to multiple bones, whereas rigid meshes are ones that are bound to a single bone (and thus move "rigidly" with that bone). 

    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    return (1 < AfxCountMeshBiases(msh, 0));
}

_AVX afxMeshTopology AfxGetMeshTopology(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxMeshTopology msht = msh->topology;
    AfxAssertObjects(1, &msht, afxFcc_MSHT);
    return msht;
}

_AVX akxMeshMorph* AfxGetMeshMorphs(afxMesh msh, afxNat baseMorphIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, baseMorphIdx, 1);
    akxMeshMorph* mshm = &msh->morphs[baseMorphIdx];
    //AfxAssertType(mshm, afxFcc_MSHM);
    return mshm;
}

_AVX akxMeshBias* AfxGetMeshBiases(afxMesh msh, afxNat baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->biasCnt, baseBiasIdx, 1);
    akxMeshBias* mshv = &msh->biases[baseBiasIdx];
    //AfxAssertType(mshv, afxFcc_MSHV);
    return mshv;
}

_AVX afxString* AfxGetMeshBiasTags(afxMesh msh, afxNat baseBiasIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->biasCnt, baseBiasIdx, 1);
    return &msh->biasTagMap[baseBiasIdx];
}

_AVX afxBool AfxGetMeshVertices(afxMesh msh, afxNat morphIdx, akxVertexData* data, afxNat* baseVtx, afxNat* vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    AfxAssertRange(msh->morphCnt, morphIdx, 1);
    akxVertexData vtd = NIL;

    if (morphIdx < msh->morphCnt)
    {
        akxMeshMorph const* mshm = &msh->morphs[morphIdx];
        vtd = mshm->vtd;
        AfxAssertObjects(1, &vtd, afxFcc_VTD);
        AfxAssert(data);
        *data = vtd;
        *baseVtx = mshm->baseVtx;
        *vtxCnt = mshm->vtxCnt;
    }
    return !!vtd;
}

_AVX afxError _AkxRelinkMeshTopology(afxMesh msh, afxMeshTopology msht)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    afxMeshTopology curr = msh->topology;

    if (msht != curr)
    {
        if (msht)
        {
            AfxAssertObjects(1, &msht, afxFcc_MSHT);
            AfxReacquireObjects(1, &msht);

            msh->topology = msht;
            AfxReacquireObjects(1, &msht);
            
            afxNat surCnt = AfxCountMeshSurfaces(msht, 0);
            afxNat triCnt = AfxCountMeshTriangles(msht, 0);
            akxIndexedTriangle *tris = AfxGetMeshTriangles(msht, 0);

            AfxResetBoxes(surCnt, msh->surfAabbMap);

            akxVertexData vtd;
            afxVertexFormat fmt;
            afxNat baseVtx, vtxCnt;
            AfxGetMeshVertices(msh, 0, &vtd, &baseVtx, &vtxCnt);
            AkxGetVertexAttributeInfo(vtd, 0, &fmt, NIL, NIL);
            void* data = AkxExposeVertexData(vtd, 0, baseVtx);

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

            if (fmt == afxVertexFormat_V3D)
            {
                for (afxNat j = 0; j < surCnt; j++)
                {
                    akxMeshSurface* mshs = AfxGetMeshSurface(msht, j);
                    afxV3d* data3 = data;

                    for (afxNat i = 0; i < mshs->triCnt; i++)
                    {
                        for (afxNat k = 0; k < 3; k++)
                            AfxAabbAbsorbAtv3d(msh->surfAabbMap[j], 1, &data3[tris[mshs->baseTriIdx + i][k]]);
                    }
                }
            }
            else if (fmt == afxVertexFormat_V4D)
            {
                for (afxNat j = 0; j < surCnt; j++)
                {
                    akxMeshSurface* mshs = AfxGetMeshSurface(msht, j);
                    afxV4d* data3 = data;

                    for (afxNat i = 0; i < mshs->triCnt; i++)
                    {
                        for (afxNat k = 0; k < 3; k++)
                            AfxAabbAbsorbAtv4d(msh->surfAabbMap[j], 1, &data3[tris[mshs->baseTriIdx + i][k]]);
                    }
                }
            }
            else AfxThrowError();
        }

        msh->topology = msht;

        if (curr)
        {
            AfxAssertObjects(1, &curr, afxFcc_MSHT);
            AfxReleaseObjects(1, &curr);
        }
    }
    return err;
}

_AVX afxError _AvxMshDtorCb(afxMesh msh)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);
    
    if (msh->morphs)
    {
        afxNat morphCnt = msh->morphCnt;

        for (afxNat i = 0; i < morphCnt; i++)
        {
            akxMeshMorph* morph = &msh->morphs[i];
            AfxAssertObjects(1, &morph->vtd, afxFcc_VTD);
            AfxReleaseObjects(1, &morph->vtd);
        }
    }

    afxObjectStash stashes[] =
    {
        {
            .cnt = msh->morphCnt,
            .siz = sizeof(msh->morphs[0]),
            .var = (void**)&msh->morphs
        },
        {
            .cnt = msh->morphCnt - 1,
            .siz = sizeof(msh->extraMorphTagMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->extraMorphTagMap
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biases[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->biases
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biasTagMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->biasTagMap
        },
        {
            .cnt = msh->biasCnt,
            .siz = sizeof(msh->biasObbMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->biasObbMap
        },
        {
            .cnt = AfxCountMeshSurfaces(msh->topology, 0),
            .siz = sizeof(msh->surfAabbMap[0]),
            .align = AFX_SIMD_ALIGN,
            .var = (void**)&msh->surfAabbMap
        }
    };

    if (AfxDeallocateInstanceData(msh, AFX_COUNTOF(stashes), stashes))
        AfxThrowError();

    AfxAssert(!msh->morphs);
    AfxAssert(!msh->extraMorphTagMap);
    AfxAssert(!msh->biases);
    AfxAssert(!msh->biasTagMap);
    AfxAssert(!msh->biasObbMap);

    if (msh->topology)
    {
        AfxAssertObjects(1, &msh->topology, afxFcc_MSHT);
        _AkxRelinkMeshTopology(msh, NIL);
    }

    return err;
}

_AVX afxError _AvxMshCtorCb(afxMesh msh, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &msh, afxFcc_MSH);

    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    akxMeshBlueprint const* mshb = args[1];

    if (!AfxCatalogStrings(AfxGetMorphTagStringBase(din), 1, &mshb->id.str, &msh->urn))
        AfxThrowError();

    afxMeshTopology msht = mshb->topology;

    if (!msht) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &msht, afxFcc_MSHT);

        afxNat extraMorphCnt = mshb->extraMorphCnt;
        afxNat morphCnt = 1 + extraMorphCnt;
        afxNat biasCnt = AfxMax(1, mshb->biasCnt);

        msh->morphs = NIL;
        msh->extraMorphTagMap = NIL;
        msh->biases = NIL;
        msh->biasTagMap = NIL;
        msh->biasObbMap = NIL;
        msh->surfAabbMap = NIL;
        msh->topology = NIL;

        afxNat surfCnt = AfxCountMeshSurfaces(msht, 0);

        afxObjectStash stashes[] =
        {
            {
                .cnt = morphCnt,
                .siz = sizeof(msh->morphs[0]),
                .var = (void**)&msh->morphs
            },
            {
                .cnt = extraMorphCnt,
                .siz = sizeof(msh->extraMorphTagMap[0]),
                .align = AFX_SIMD_ALIGN,
                .var = (void**)&msh->extraMorphTagMap
            },
            {
                .cnt = biasCnt,
                .siz = sizeof(msh->biases[0]),
                .align = AFX_SIMD_ALIGN,
                .var = (void**)&msh->biases
            },
            {
                .cnt = biasCnt,
                .siz = sizeof(msh->biasTagMap[0]),
                .align = AFX_SIMD_ALIGN,
                .var = (void**)&msh->biasTagMap
            },
            {
                .cnt = biasCnt,
                .siz = sizeof(msh->biasObbMap[0]),
                .align = AFX_SIMD_ALIGN,
                .var = (void**)&msh->biasObbMap
            },
            {
                .cnt = surfCnt,
                .siz = sizeof(msh->surfAabbMap[0]),
                .align = AFX_SIMD_ALIGN,
                .var = (void**)&msh->surfAabbMap
            }
        };

        if (AfxAllocateInstanceData(msh, AFX_COUNTOF(stashes), stashes)) AfxThrowError();
        else
        {
            AfxAssert(msh->morphs);
            AfxAssert(!extraMorphCnt || msh->extraMorphTagMap);
            AfxAssert(!biasCnt || msh->biases);
            AfxAssert(!biasCnt || msh->biasTagMap);
            AfxAssert(!biasCnt || msh->biasObbMap);

            for (afxNat i = 0; i < morphCnt; i++)
            {
                akxMeshMorph const* src;

                if (i == 0)
                {
                    if (!mshb->vtd)
                    {
                        AfxThrowError();
                        break;
                    }

                    akxMeshMorph const firstMorph =
                    {
                        .vtd = mshb->vtd,
                        .baseVtx = mshb->baseVtx,
                        .vtxCnt = mshb->vtxCnt,
                        .delta = FALSE
                    };
                    src = &firstMorph;
                }
                else if (mshb->extraMorphs) src = &mshb->extraMorphs[i];
                else src = (akxMeshMorph const[]) { 0 };

                akxVertexData vtd = src->vtd;
                AfxAssertObjects(1, &vtd, afxFcc_VTD);
                AfxReacquireObjects(1, &vtd);
                afxNat maxVtxCnt = vtd ? AkxCountVertices(vtd) : 0;
                AfxAssertRange(maxVtxCnt, src->baseVtx, src->vtxCnt);
                afxNat vtxCnt = src->vtxCnt ? src->vtxCnt : maxVtxCnt - src->baseVtx;

                akxMeshMorph* morph = &msh->morphs[i];
                morph->vtd = vtd;
                morph->baseVtx = src->baseVtx;
                morph->vtxCnt = vtxCnt;
                morph->delta = !!src->delta;
            }

            msh->morphCnt = morphCnt;

            if (extraMorphCnt)
            {
                if (!mshb->extraMorphTagMap) for (afxNat i = 0; i < extraMorphCnt; i++)
                    AfxResetString(&msh->extraMorphTagMap[i]);
                else if (extraMorphCnt != (afxNat)AfxCatalogStrings(AfxGetMorphTagStringBase(din), extraMorphCnt, mshb->extraMorphTagMap, msh->extraMorphTagMap))
                    AfxThrowError();
            }

            if (!err)
            {
                msh->biasCnt = biasCnt;

                AfxAssert(biasCnt < 2 || mshb->biases);

                if (biasCnt)
                {
                    if (!mshb->biases) for (afxNat i = 0; i < biasCnt; i++)
                        AfxResetString(&msh->biasTagMap[i]);
                    else if (biasCnt != (afxNat)AfxCatalogStrings(AfxGetPivotTagStringBase(din), biasCnt, mshb->biases, msh->biasTagMap))
                        AfxThrowError();

                    for (afxNat i = 0; i < biasCnt; i++)
                    {
                        akxMeshBias* bias = &msh->biases[i];
                        AfxBoxReset(bias->obb);
                        bias->triCnt = 0;
                        bias->tris = NIL;
                    }

                    AfxResetBoxes(biasCnt, msh->biasObbMap);
                }

                if (_AkxRelinkMeshTopology(msh, msht))
                    AfxThrowError();

                if (!err)
                {
                    msh->extData = NIL;
                }
            }

            if (err)
            {
                AfxAssert(msh->morphs);

                if (msh->morphs)
                {
                    for (afxNat i = 0; i < morphCnt; i++)
                    {
                        akxMeshMorph* morph = &msh->morphs[i];
                        AfxAssertObjects(1, &morph->vtd, afxFcc_VTD);
                        AfxReleaseObjects(1, &morph->vtd);
                    }
                }
            }

            if (err)
            {
                if (AfxDeallocateInstanceData(msh, AFX_COUNTOF(stashes), stashes))
                    AfxThrowError();
            }
        }

        // EXHIBIT STATISTICAS

        if (!err)
        {
            afxString s;
            AfxGetMeshUrn(msh, &s);

            AfxAssertObjects(1, &mshb->topology, afxFcc_MSHT);

            afxNat vtxAttrCnt = AkxCountVertexAttributes(msh->morphs[0].vtd);
            afxNat mshtSurCnt = AfxCountMeshSurfaces(msh->topology, 0);
            afxNat mshtTriCnt = AfxCountMeshSurfaces(msh->topology, 0);

            AfxLogEcho("%.*s Mesh <%.*s> assembled at %p.\n    %u vertices with %u attributes.\n    %u triangles (%u bytes per index) arranged in %u surfaces.\n    Listing %u biases..:",
                AfxPushString(msh->biasCnt > 1 ? &AfxStaticString("Skinned") : &AfxStaticString("Rigid")),
                AfxPushString(&s), msh, msh->morphs[0].vtxCnt, vtxAttrCnt, mshtTriCnt, AfxDetermineMeshIndexSize(mshb->topology), mshtSurCnt, msh->biasCnt
            );

            for (afxNat i = 0; i < msh->biasCnt; i++)
            {
                s = msh->biasTagMap[i];
                AfxLogEcho("    %3u <%.*s> %3u", i, AfxPushString(&s), msh->biases ? msh->biases[i].triCnt : 0);
            }
        }
    }
    return err;
}

_AVX afxClassConfig _AvxMshClsCfg =
{
    .fcc = afxFcc_MSH,
    .name = "Mesh",
    .desc = "Rigid/Deformable Mesh",
    .fixedSiz = sizeof(AFX_OBJECT(afxMesh)),
    .ctor = (void*)_AvxMshCtorCb,
    .dtor = (void*)_AvxMshDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAssembleMeshes(afxDrawInput din, afxNat cnt, akxMeshBlueprint const blueprints[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(blueprints);
    AfxAssert(meshes);
    AfxAssert(cnt);

    afxClass* cls = (afxClass*)AfxGetMeshClass(din);
    AfxAssertClass(cls, afxFcc_MSH);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)meshes, (void const*[]) { din, blueprints }))
        AfxThrowError();

    return err;
}

_AVX afxNat AfxEnumerateMeshes(afxDrawInput din, afxNat base, afxNat cnt, afxMesh msh[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = AfxGetMeshClass(din);
    AfxAssertClass(cls, afxFcc_MSH);
    return AfxEnumerateClassInstances(cls, base, cnt, (afxObject*)msh);
}

_AVX afxError AfxBuildMeshes(afxDrawInput din, afxNat cnt, afxMeshBuilder const builders[], afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(builders);
    AfxAssert(meshes);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMeshBuilder const* mshb = &builders[i];
        akxVertexData vtd = AkxBuildVertexData(din, mshb);
        afxMeshTopology msht = AfxBuildMeshTopology(din, mshb, 0, mshb->surfCnt);

        akxMeshBlueprint blueprint = { 0 };
        blueprint.vtd = vtd;
        blueprint.topology = msht;
        blueprint.biasCnt = mshb->artCnt;
        blueprint.biases = mshb->pivots;
        AfxMakeString32(&blueprint.id, &mshb->id.str);

        if (AfxAssembleMeshes(din, 1, &blueprint, &meshes[i]))
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

_AVX void AfxTransformMeshes(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxNat cnt, afxMesh meshes[])
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

            akxVertexData vtd;
            afxNat baseVtx, vtxCnt;
            AfxGetMeshVertices(msh2, 0, &vtd, &baseVtx, &vtxCnt);
            AfxTryAssertObjects(1, &vtd, afxFcc_VTD);
            
            AkxTransformVertexDatas(ltm, iltm, atv, flags & 1, 1, &vtd); // sinalizado para renormalizar normals

            afxNat morphCnt = AfxCountMeshMorphs(msh2, 0);

            for (afxNat j = 0; j < morphCnt; j++)
            {
                akxMeshMorph* mshm = AfxGetMeshMorphs(msh2, j);
                vtd = mshm->vtd;
                AfxTryAssertObjects(1, &vtd, afxFcc_VTD);
                AfxAssert(!mshm->delta || AkxGetVertexAttributeFlags(vtd, 0) & akxVertexFlag_DELTA);

                if (mshm->delta)
                {
                    AkxTransformVertexDatas(ltm, iltm, atv, FALSE, 1, &vtd);
                }
                else
                {
                    AkxTransformVertexDatas(ltm, iltm, atv, flags & 1, 1, &vtd);
                }
            }

            afxMeshTopology msht;

            if ((flags & 2) && (AfxM3dDet(ltm) < 0.0) && (msht = AfxGetMeshTopology(msh2))) // se sinalizado para reordenar índices de faces
                AfxInvertMeshWinding(msht);

            afxNat artCnt = AfxCountMeshBiases(msh2, 0);

#if 0
            for (afxNat j = 0; j < artCnt; j++)
            {
                akxMeshBias* mshv = AfxGetMeshBiases(msh2, j);
                AfxTransformObbs(ltm, atv, 1, &mshv->aabb, &mshv->aabb);
            }
#endif
        }
    }
}

_AVX void AfxRenormalizeMeshes(afxNat cnt, afxMesh meshes[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(meshes);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMesh msh2 = meshes[i];
        AfxAssertObjects(1, &msh2, afxFcc_MSH);
        akxVertexData vtd = msh2->morphs[0].vtd;
        afxMeshTopology msht = msh2->topology;

        afxString const attrs[] =
        {
            AfxString("posn"),
            AfxString("nrm")
        };
        afxNat attrIdx[2];
        AkxFindVertexAttributes(vtd, 2, attrs, attrIdx);
        afxV4d* posn = AkxExposeVertexData(vtd, attrIdx[0], 0);
        afxV3d* nrm = AkxExposeVertexData(vtd, attrIdx[1], 0);

        akxIndexedTriangle const* tris = AfxGetMeshTriangles(msht, 0);
        afxNat triCnt = AfxCountMeshTriangles(msht, 0);
        afxNat vtxCnt = AfxCountMeshVertices(msh2, 0, 0);

        AkxZeroVertexData(vtd, attrIdx[1], 0, vtxCnt);

        for (afxNat j = 0; j < triCnt; j++)
        {
            afxNat const ia = tris[j][0];
            afxNat const ib = tris[j][1];
            afxNat const ic = tris[j][2];

            afxV4d e1, e2, no;
            AfxV4dSub(e1, posn[ia], posn[ib]);
            AfxV4dSub(e2, posn[ic], posn[ib]);
            AfxV3dCross(no, e1, e2);

            AfxV3dAdd(nrm[ia], nrm[ia], no);
            AfxV3dAdd(nrm[ib], nrm[ib], no);
            AfxV3dAdd(nrm[ic], nrm[ic], no);
        }

        AkxNormalizeVertexData(vtd, attrIdx[1], 0, vtxCnt);
    }
}
