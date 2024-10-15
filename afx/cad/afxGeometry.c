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
#define _AVX_GEOMETRY_C
#define _AVX_VERTEX_BUFFER_C
#include "../dev/AvxImplKit.h"

_AVX afxNat AkxFindVertexAttributes(afxGeometry geo, afxNat cnt, afxString const id[], afxNat attrIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssert(attrIdx);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
        attrIdx[i] = AFX_INVALID_INDEX;

    for (afxNat i = 0; i < geo->attrCnt; i++)
    {
        afxString str = geo->attrs[i].usage.str;

        if (0 == AfxCompareString(&id[rslt], &str))
            attrIdx[rslt++] = i;

        if (cnt == rslt)
            break;
    }
    return rslt;
}

_AVX afxVertexFormat AkxGetVertexAttributeFormat(afxGeometry geo, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);
    return geo->attrs[attrIdx].fmt;
}

_AVX afxString AkxGetVertexAttributeUsage(afxGeometry geo, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);
    return geo->attrs[attrIdx].usage.str;
}

_AVX afxVertexFlags AkxGetVertexAttributeFlags(afxGeometry geo, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);
    return geo->attrs[attrIdx].flags;
}

_AVX afxError AkxGetVertexAttributeInfo(afxGeometry geo, afxNat attrIdx, afxVertexFormat* fmt, afxString* usage, afxVertexFlags* flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);

    if (fmt)
        *fmt = geo->attrs[attrIdx].fmt;

    if (usage)
        *usage = geo->attrs[attrIdx].usage.str;

    if (flags)
        *flags = geo->attrs[attrIdx].flags;

    return err;
}

_AVX afxNat AfxCountVertices(afxGeometry geo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    return geo->vtxCnt;
}

_AVX afxNat AkxCountVertexAttributes(afxGeometry geo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    return geo->attrCnt;
}

_AVX afxError AfxUpdateGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src, afxNat32 srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);
    AfxAssertRange(geo->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(src);

    afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!geo->data[attrIdx] && !(geo->data[attrIdx] = AfxAllocate(geo->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        AfxStream2(vtxCnt, src, srcStride, &(geo->data[attrIdx][unitSiz * baseVtx]), unitSiz);
        //AfxStream(vtxCnt, srcStride, unitSiz, src, geo->attrs[attrIdx].data);
    }
    return err;
}

_AVX afxError AfxUploadGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, afxStream in, afxNat stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);
    AfxAssertRange(geo->vtxCnt, baseVtx, vtxCnt);
    AfxAssertObjects(1, &in, afxFcc_IOB);

    afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!geo->data[attrIdx] && !(geo->data[attrIdx] = AfxAllocate(geo->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        AfxReadStream2(in, vtxCnt * unitSiz, stride, &geo->data[attrIdx][baseVtx * unitSiz], unitSiz);
        //AfxStream2(vtxCnt, src, srcStride, &(geo->data[attrIdx][unitSiz * baseVtx]), unitSiz);
        //AfxStream(vtxCnt, srcStride, unitSiz, src, geo->attrs[attrIdx].data);
    }
    return err;
}

_AVX afxError AfxDownloadGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, afxStream out, afxNat stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);
    AfxAssertRange(geo->vtxCnt, baseVtx, vtxCnt);
    AfxAssertObjects(1, &out, afxFcc_IOB);

    afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!geo->data[attrIdx] && !(geo->data[attrIdx] = AfxAllocate(geo->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        AfxWriteStream2(out, vtxCnt * unitSiz, stride, &geo->data[attrIdx][baseVtx * unitSiz], unitSiz);
    }
    return err;
}

_AVX void* AfxExposeGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);
    AfxAssertRange(geo->vtxCnt, baseVtx, 1);

    afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!geo->data[attrIdx] && !(geo->data[attrIdx] = AfxAllocate(geo->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
        AfxThrowError();

    afxByte *data = geo->data[attrIdx];

    if (!data)
        AfxThrowError();
    else
        data += baseVtx * unitSiz;

    return data;
}

_AVX afxError AfxInvertNormalizedGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);
    AfxAssertRange(geo->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(geo->attrs[attrIdx].flags & afxVertexFlag_NORMALIZED);
    
    if (geo->data[attrIdx])
    {
        afxV3d* nrm = AfxExposeGeometry(geo, attrIdx, baseVtx);

        for (afxNat i = 3; i < vtxCnt; i++)
        {
            AfxV3dScale(nrm[i], nrm[i], -1.f);
        }
    }
    return err;
}

_AVX afxError AfxNormalizeGeometry(afxGeometry geo, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    AfxAssertRange(geo->attrCnt, attrIdx, 1);
    AfxAssertRange(geo->vtxCnt, baseVtx, vtxCnt);
    
    afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
    AfxAssert(fmt == afxVertexFormat_V2D || fmt == afxVertexFormat_V3D || fmt == afxVertexFormat_V4D);

    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!geo->data[attrIdx])
        if (!(geo->data[attrIdx] = AfxAllocate(geo->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
            AfxThrowError();

    if (!err)
    {
        switch (fmt)
        {
        case afxVertexFormat_V2D:
        {
            AfxNormalizeArrayedV2d(vtxCnt, (void*)geo->data[attrIdx], (void*)geo->data[attrIdx]);
            break;
        }
        case afxVertexFormat_V3D:
        {
            AfxNormalizeArrayedV3d(vtxCnt, (void*)geo->data[attrIdx], (void*)geo->data[attrIdx]);
            break;
        }
        case afxVertexFormat_V4D:
        {
            AfxNormalizeArrayedV4d(vtxCnt, (void*)geo->data[attrIdx], (void*)geo->data[attrIdx]);
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_AVX afxError _AvxGeomDtorCb(afxGeometry geo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = geo->attrCnt,
            .siz = sizeof(geo->attrs[0]),
            .var = (void**)&geo->attrs
        },
        {
            .cnt = geo->attrCnt,
            .siz = sizeof(geo->data[0]),
            .var = (void**)&geo->data
        }
    };

    if (AfxDeallocateInstanceData(geo, AFX_COUNTOF(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxGeomCtorCb(afxGeometry geo, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_VTD);
    
    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    akxVertexAttrSpec const* attrSpecs = args[1];
    afxGeometrySpec const* spec = args[2];
    spec += invokeNo;

    afxNat totalBiasCnt = spec->biasCnt;
    afxNat vtxCnt = spec->vtxCnt;

    AfxAssert(vtxCnt); // at least one
    geo->vtxCnt = vtxCnt;
            
    afxNat attrCnt = spec->attrCnt;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = attrCnt,
            .siz = sizeof(geo->attrs[0]),
            .var = (void**)&geo->attrs
        },
        {
            .cnt = attrCnt,
            .siz = sizeof(geo->data[0]),
            .var = (void**)&geo->data
        }
    };

    if (AfxAllocateInstanceData(geo, AFX_COUNTOF(stashes), stashes)) AfxThrowError();
    else
    {
        geo->attrCnt = attrCnt;
        
        for (afxNat i = 0; i < attrCnt; i++)
        {
            avxGeometryAttr* attrs = geo->attrs;
            akxVertexAttrSpec const* attrSpec = &attrSpecs[i];

            geo->data[i] = NIL;

            afxBool hasData = TRUE;

            afxString tmp;
            AfxMakeString(&tmp, 0, attrSpec->usage, 0);

            //if (!AfxCatalogStrings(AfxGetMorphTagStringBase(din), 1, &tmp, &attrs[i].id))
            //    AfxThrowError();
            AfxMakeString8(&attrs[i].usage, &tmp);

            attrs[i].flags = NIL;

            if (AfxCompareStringL(&attrs[i].usage.str, 0, "pos", 3))
                attrs[i].flags |= afxVertexFlag_AFFINE | afxVertexFlag_LINEAR;

            attrs[i].flags |= attrSpec->flags;
            AfxAssert(attrSpec->fmt < afxVertexFormat_TOTAL);
            attrs[i].fmt = attrSpec->fmt;
            
            afxSize vtxSiz = AfxVertexFormatGetSize(attrs[i].fmt);

            //if (attrSpec->src)
              //  if (AfxUpdateGeometry(geo, i, 0, vtxCnt, attrSpec->src, attrSpec->srcStride))
                //    AfxThrowError();
        }

        if (err && AfxDeallocateInstanceData(geo, AFX_COUNTOF(stashes), stashes))
            AfxThrowError();
    }
    return err;
}

_AVX afxClassConfig _AvxGeomClsCfg =
{
    .fcc = afxFcc_VTD,
    .name = "Geometry",
    .desc = "Geometry",
    .fixedSiz = sizeof(AFX_OBJECT(afxGeometry)),
    .ctor = (void*)_AvxGeomCtorCb,
    .dtor = (void*)_AvxGeomDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireGeometries(afxDrawInput din, akxVertexAttrSpec const attrSpec[], afxNat cnt, afxGeometrySpec const specs[], afxGeometry datas[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(datas);
    AfxAssert(specs);
    AfxAssert(cnt);

    afxClass* cls = (afxClass*)AfxGetGeometryClass(din);
    AfxAssertClass(cls, afxFcc_VTD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)datas, (void const*[]) { din, attrSpec, specs }))
        AfxThrowError();

    return err;
}

_AVX afxGeometry AfxBuildGeometry(afxDrawInput din, afxMeshBuilder const* mshb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(mshb);
    afxGeometry geo = NIL;

    afxGeometrySpec spec;
    spec.vtxCnt = mshb->vtxCnt;
    spec.vtxSrc = mshb->vtx;
    spec.vtxSrcStride = sizeof(mshb->vtx[0]);
    spec.biasCnt = mshb->biases.cnt;
    spec.biasSrcStride = mshb->biases.unitSiz;
    spec.biasSrc = mshb->biases.data;
    spec.baseAttrIdx = 0;
    spec.attrCnt = 3;

    for (afxNat i = 0; i < spec.biasCnt; i++)
    {
        afxVertexBias const* vb = AfxGetArrayUnit(&mshb->biases, i);
        AfxAssert(1.0 >= vb->weight);
        AfxAssertRange(mshb->artCnt, vb->pivotIdx, 1);
    }

    akxVertexAttrSpec attrSpecs[16];

    attrSpecs[0].fmt = afxVertexFormat_V4D;
    attrSpecs[0].flags = afxVertexFlag_AFFINE | afxVertexFlag_LINEAR;
    attrSpecs[0].usage = "pos";

    attrSpecs[1].fmt = afxVertexFormat_V3D;
    attrSpecs[1].flags = afxVertexFlag_LINEAR;
    attrSpecs[1].usage = "nrm";

    attrSpecs[2].fmt = afxVertexFormat_V2D;
    attrSpecs[2].flags = NIL;
    attrSpecs[2].usage = "uv";

    if (AfxAcquireGeometries(din, attrSpecs, 1, &spec, &geo))
        AfxThrowError();

    AfxUpdateGeometry(geo, 0, 0, spec.vtxCnt, mshb->posn, sizeof(mshb->posn[0]));
    AfxUpdateGeometry(geo, 1, 0, spec.vtxCnt, mshb->nrm, sizeof(mshb->nrm[0]));
    AfxUpdateGeometry(geo, 2, 0, spec.vtxCnt, mshb->uv, sizeof(mshb->uv[0]));

    return geo;
}

_AVX void AfxTransformGeometries(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxBool renormalize, afxNat cnt, afxGeometry geo[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(geo);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxGeometry vtd2 = geo[i];

        if (vtd2)
        {
            AfxAssertObjects(1, &vtd2, afxFcc_VTD);

            afxNat baseVtx = 0;
            afxNat vtxCnt = vtd2->vtxCnt;

            for (afxNat j = 0; j < vtd2->attrCnt; j++)
            {
                void* data = AfxExposeGeometry(vtd2, j, baseVtx);
                afxVertexFlags flags = AkxGetVertexAttributeFlags(vtd2, j);
                afxBool linearFlag = flags & afxVertexFlag_LINEAR;
                afxBool invLinearFlag = flags & afxVertexFlag_LINEAR_INV;
                afxBool affineFlag = flags & afxVertexFlag_AFFINE;
                afxBool deltaFlag = flags & afxVertexFlag_DELTA;
                afxBool normalizedFlag = flags & afxVertexFlag_NORMALIZED;
                afxString usage = AkxGetVertexAttributeUsage(vtd2, j);
                afxVertexFormat fmt = AkxGetVertexAttributeFormat(vtd2, j);
                afxBool normalized;

                if (AfxCompareStringL(&usage, 0, "pos", 3))
                {
                    if (fmt == afxVertexFormat_V4D)
                    {
                        AfxM3dPostMultiplyLtv4d(ltm, vtxCnt, data, data);

                        if (!deltaFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxV4dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], atv);
                    }
                    else if (fmt == afxVertexFormat_V3D)
                    {
                        AfxM3dPostMultiplyV3d(ltm, vtxCnt, data, data);

                        if (!deltaFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], atv);
                    }

                    normalized = FALSE;
                }
#if !0
                else if (AfxCompareStringL(&usage, 0, "tan", 3))
                {
                    AfxM3dPostMultiplyV3d(ltm, vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }
                else if (AfxCompareStringL(&usage, 0, "bit", 3))
                {
                    AfxM3dPostMultiplyV3d(ltm, vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }                
                else if (AfxCompareStringL(&usage, 0, "tbc", 3))
                {
                    if (deltaFlag)
                    {
                        AfxM3dPostMultiplyV3d(ltm, vtxCnt, data, data);
                    }
                    else
                    {
                        AfxM3dPostMultiplyV3d(iltm, vtxCnt, data, data);
                    }

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }
                else if (AfxCompareStringL(&usage, 0, "nrm", 3))
                {
                    if (deltaFlag)
                    {
                        AfxM3dPostMultiplyV3d(ltm, vtxCnt, data, data);
                    }
                    else
                    {
                        AfxM3dPostMultiplyV3d(iltm, vtxCnt, data, data);
                    }

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }

                if (renormalize && normalized)
                {
                    if (fmt == afxVertexFormat_V2D)
                        AfxZeroOrNormalizeArrayedV2d(vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V3D)
                        AfxZeroOrNormalizeArrayedV3d(vtxCnt, data, data);
                    else if (fmt == afxVertexFormat_V4D)
                        AfxZeroOrNormalizeArrayedV4d(vtxCnt, data, data);
                }
#endif
#if 0
                if (data && AkxGetVertexAttributeFlags(vtd2, j) & afxVertexUsage_SPATIAL)
                {
                    switch (AkxGetVertexAttributeFormat(vtd2, j))
                    {
                    case afxVertexFormat_V2D:
                    {
                        AfxThrowError();
                        break;
                    }
                    case afxVertexFormat_V3D:
                    {
                        AfxThrowError(); // inoperant

                        if (linearFlag)
                            AfxM3dPostMultiplyV3d(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxM3dPostMultiplyV3d(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], at);

                        if (renormalize)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxV3dNormalize(((afxV3d*)data)[k], ((afxV3d*)data)[k]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        AfxThrowError(); // inoperant

                        if (linearFlag)
                            AfxM3dPostMultiplyLtv4d(lt, vtxCnt, data, data);

                        if (invLinearFlag)
                            AfxM3dPostMultiplyLtv4d(ilt, vtxCnt, data, data);

                        if (affineFlag)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxV3dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], at);

                        if (renormalize)
                            for (afxNat k = 0; k < vtxCnt; k++)
                                AfxV4dNormalize(((afxV4d*)data)[k], ((afxV4d*)data)[k]);

                        break;
                    }
                    default: AfxThrowError(); break;
                    }
                }
#endif
            }
        }
    }
}

AFX_DEFINE_STRUCT(urdGeo)
{
    afxNat32 vtxCnt;
    afxNat32 attrCnt;
    afxV3d  aabbMin;
    afxV3d  aabbMax;
};

AFX_DEFINE_STRUCT(urdGeoVa)
{
    afxNat32 fmt;
    afxNat32 flags;
    afxChar8 usage[8];
};

_AVX afxError AfxStoreGeometries(afxStream out, afxNat cnt, afxGeometry geometries[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssertObjects(cnt, geometries, afxFcc_VTD);
    afxNat geoIdx = 0;

    do
    {
        afxGeometry geo = geometries[geoIdx];

        urdMark mark = { 0 };
        mark.fcc = afxChunkId_QW_SKL;

        urdGeo nfo =
        {
            .vtxCnt = geo->vtxCnt,
            .attrCnt = geo->attrCnt
        };

        mark.siz += sizeof(nfo);
        
        for (afxNat i = 0; i < geo->attrCnt; i++)
        {
            afxVertexFormat fmt = geo->attrs[i].fmt;
            afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);
            
            mark.siz += sizeof(urdGeoVa) + unitSiz * geo->vtxCnt;
        }

        if (AfxWriteStream(out, sizeof(mark), 0, &mark))
            AfxThrowError();

        AfxV3dCopy(nfo.aabbMax, geo->aabb[AFX_AABB_SUP]);
        AfxV3dCopy(nfo.aabbMin, geo->aabb[AFX_AABB_INF]);
        
        AfxWriteStream(out, sizeof(nfo), 0, &nfo);

        for (afxNat i = 0; i < geo->attrCnt; i++)
        {
            urdGeoVa attrNfo =
            {
                .fmt = geo->attrs[i].fmt,
                .flags = geo->attrs[i].flags
            };
            AfxDumpString(&geo->attrs[i].usage.str, 0, geo->attrs[i].usage.str.len, attrNfo.usage);

            AfxWriteStream(out, sizeof(attrNfo), 0, &attrNfo);
        }

        for (afxNat i = 0; i < geo->attrCnt; i++)
        {
            afxVertexFormat fmt = geo->attrs[i].fmt;
            afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

            AfxWriteStream(out, unitSiz * geo->vtxCnt, 0, geo->data[i]);
        }

    } while (!err && (++geoIdx < cnt));

    return err;
}

_AVX afxError AfxLoadGeometry(afxStream in, urdMark const* hdr, afxDrawInput din, afxGeometry* geometry)
{
    afxError err = NIL;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AfxAssert(geometry);

    urdGeo nfo;

    if (AfxReadStream(in, sizeof(nfo), 0, &nfo))
        AfxThrowError();

    afxString8 tags[16];
    akxVertexAttrSpec attrs[16];

    for (afxNat i = 0; i < nfo.attrCnt; i++)
    {
        urdGeoVa attrNfo;

        AfxReadStream(in, sizeof(attrNfo), 0, &attrNfo);

        attrs[i].flags = attrNfo.flags;
        attrs[i].fmt = attrNfo.fmt;
        attrs[i].usage = attrNfo.usage;

        afxString s;
        AfxMakeString(&s, 0, attrs[i].usage, 0);
        AfxMakeString8(&tags[i], &s);
    }

    afxGeometry geo;
    afxGeometrySpec geob = { 0 };
    geob.attrCnt = nfo.attrCnt;
    geob.vtxCnt = nfo.vtxCnt;
    AfxAcquireGeometries(din, attrs, 1, &geob, &geo);

    for (afxNat i = 0; i < nfo.attrCnt; i++)
    {
        afxVertexFormat fmt = geo->attrs[i].fmt;
        afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

        AfxUploadGeometry(geo, i, 0, nfo.vtxCnt, in, unitSiz);
    }

    *geometry = geo;

    return err;
}


