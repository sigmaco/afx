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
#define _AVX_VERTEX_DATA_C
#define _AVX_VERTEX_BUFFER_C
#include "../dev/AvxImplKit.h"

_AVX afxNat AkxFindVertexAttributes(akxVertexData vtd, afxNat cnt, afxString const id[], afxNat attrIdx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssert(attrIdx);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
        attrIdx[i] = AFX_INVALID_INDEX;

    for (afxNat i = 0; i < vtd->attrCnt; i++)
    {
        afxString str = vtd->attrs[i].id;

        if (0 == AfxCompareString(&id[rslt], &str))
            attrIdx[rslt++] = i;

        if (cnt == rslt)
            break;
    }
    return rslt;
}

_AVX afxVertexFormat AkxGetVertexAttributeFormat(akxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].fmt;
}

_AVX akxVertexUsage AkxGetVertexAttributeUsage(akxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].usage;
}

_AVX akxVertexFlags AkxGetVertexAttributeFlags(akxVertexData vtd, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    return vtd->attrs[attrIdx].flags;
}

_AVX afxError AkxGetVertexAttributeInfo(akxVertexData vtd, afxNat attrIdx, afxVertexFormat* fmt, akxVertexUsage* usage, akxVertexFlags* flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);

    if (fmt)
        *fmt = vtd->attrs[attrIdx].fmt;

    if (usage)
        *usage = vtd->attrs[attrIdx].usage;

    if (flags)
        *flags = vtd->attrs[attrIdx].flags;

    return err;
}

_AVX afxNat AkxCountVertices(akxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    return vtd->vtxCnt;
}

_AVX afxNat AkxCountVertexAttributes(akxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    return vtd->attrCnt;
}

_AVX afxError AkxUpdateVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src, afxNat32 srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(src);
    //AfxAssert(srcStride);
#if 0
    afxByte const* src2 = src;
    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);
    afxNat srcIdx = vtd->attrs[attrIdx].srcIdx;
    afxSize combOff = vtd->srcs[srcIdx].offset + (baseVtx * unitSiz);
    afxBuffer buf = vtd->srcs[srcIdx].buf;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    if (AfxUpdateBuffer2(buf, combOff, vtxCnt * unitSiz, unitSiz, src, srcStride))
        AfxThrowError();
#endif

    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data && !(vtd->attrs[attrIdx].data = AfxAllocate(vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        AfxStream2(vtxCnt, src, srcStride, &(vtd->attrs[attrIdx].dataBytemap[unitSiz * baseVtx]), unitSiz);
        //AfxStream(vtxCnt, srcStride, unitSiz, src, vtd->attrs[attrIdx].data);
    }
    return err;
}

_AVX void* AkxExposeVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, 1);

    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data && !(vtd->attrs[attrIdx].data = AfxAllocate(vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
        AfxThrowError();

    afxByte *data = vtd->attrs[attrIdx].data;

    if (!data)
        AfxThrowError();
    else
        data += baseVtx * unitSiz;

    return data;
}

_AVX afxError AkxFillVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const* src)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(src);

    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
            AfxThrowError();

    if (!err)
    {
        AfxStream(vtxCnt, 0, unitSiz, src, vtd->attrs[attrIdx].data);
    }
    return err;
}

_AVX afxError AkxInvertNormalizedVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    AfxAssert(vtd->attrs[attrIdx].flags & akxVertexFlag_NORMALIZED);
    
    if (vtd->attrs[attrIdx].data)
    {
        afxV3d* nrm = AkxExposeVertexData(vtd, attrIdx, baseVtx);

        for (afxNat i = 3; i < vtxCnt; i++)
        {
            AfxV3dScale(nrm[i], nrm[i], -1.f);
        }
    }
    return err;
}

_AVX afxError AkxNormalizeVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    
    afxVertexFormat fmt = vtd->attrs[attrIdx].fmt;
    AfxAssert(fmt == afxVertexFormat_V2D || fmt == afxVertexFormat_V3D || fmt == afxVertexFormat_V4D);

    afxNat32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!vtd->attrs[attrIdx].data)
        if (!(vtd->attrs[attrIdx].data = AfxAllocate(vtd->vtxCnt, unitSiz, AFX_SIMD_ALIGN, AfxHere())))
            AfxThrowError();

    if (!err)
    {
        switch (fmt)
        {
        case afxVertexFormat_V2D:
        {
            AfxNormalizeArrayedV2d(vtxCnt, vtd->attrs[attrIdx].data, vtd->attrs[attrIdx].data);
            break;
        }
        case afxVertexFormat_V3D:
        {
            AfxNormalizeArrayedV3d(vtxCnt, vtd->attrs[attrIdx].data, vtd->attrs[attrIdx].data);
            break;
        }
        case afxVertexFormat_V4D:
        {
            AfxNormalizeArrayedV4d(vtxCnt, vtd->attrs[attrIdx].data, vtd->attrs[attrIdx].data);
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_AVX afxError AkxZeroVertexData(akxVertexData vtd, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    AfxAssertRange(vtd->attrCnt, attrIdx, 1);
    AfxAssertRange(vtd->vtxCnt, baseVtx, vtxCnt);
    
    if (AkxFillVertexData(vtd, attrIdx, baseVtx, vtxCnt, AFX_V4D_ZERO))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxVtdDtorCb(akxVertexData vtd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    
    if (vtd->attrs)
    {
        for (afxNat i = 0; i < vtd->attrCnt; i++)
        {
            if (vtd->attrs[i].data)
                AfxDeallocate(vtd->attrs[i].data);
        }

        AfxDeallocate(vtd->attrs);
    }
    return err;
}

_AVX afxError _AvxVtdCtorCb(akxVertexData vtd, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vtd, afxFcc_VTD);
    
    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    akxVertexAttrSpec const* attrSpecs = args[1];
    akxVertexDataSpec const* spec = args[2];
    spec += invokeNo;

    afxNat totalBiasCnt = spec->biasCnt;
    afxNat vtxCnt = spec->vtxCnt;

    AfxAssert(vtxCnt); // at least one
    vtd->vtxCnt = vtxCnt;
            
    afxNat attrCnt = spec->attrCnt;
    akxVertexDataAttr* attrs;

    if (!(attrs = AfxAllocate(attrCnt, sizeof(attrs[0]), NIL, AfxHere()))) AfxThrowError();
    else
    {
        vtd->attrCnt = attrCnt;
        vtd->attrs = attrs;

        for (afxNat i = 0; i < attrCnt; i++)
        {
            akxVertexAttrSpec const* attrSpec = &attrSpecs[i];

            afxBool hasData = TRUE;

            afxString tmp;
            AfxMakeString(&tmp, 0, attrSpec->id, 0);

            if (!AfxCatalogStrings(AfxGetMorphTagStringBase(din), 1, &tmp, &attrs[i].id))
                AfxThrowError();

            attrs[i].usage = attrSpec->usage;
            attrs[i].flags = NIL;

            if (attrs[i].usage & akxVertexUsage_POS)
                attrs[i].flags |= akxVertexFlag_AFFINE | akxVertexFlag_LINEAR;

            attrs[i].flags |= attrSpec->flags;
            AfxAssert(attrSpec->fmt < afxVertexFormat_TOTAL);
            attrs[i].fmt = attrSpec->fmt;
            attrs[i].data = NIL;

            afxSize vtxSiz = AfxVertexFormatGetSize(attrs[i].fmt);

            if (attrSpec->src)
                if (AkxUpdateVertexData(vtd, i, 0, vtxCnt, attrSpec->src, attrSpec->srcStride))
                    AfxThrowError();
        }
    }
    return err;
}

_AVX afxClassConfig _AvxVtdClsCfg =
{
    .fcc = afxFcc_VTD,
    .name = "Vertex Data",
    .fixedSiz = sizeof(AFX_OBJECT(akxVertexData)),
    .ctor = (void*)_AvxVtdCtorCb,
    .dtor = (void*)_AvxVtdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AkxAcquireVertexDatas(afxDrawInput din, akxVertexAttrSpec const attrSpec[], afxNat cnt, akxVertexDataSpec const specs[], akxVertexData datas[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(datas);
    AfxAssert(specs);
    AfxAssert(cnt);

    afxClass* cls = (afxClass*)AfxGetMeshDataClass(din);
    AfxAssertClass(cls, afxFcc_VTD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)datas, (void const*[]) { din, attrSpec, specs }))
        AfxThrowError();

    return err;
}

_AVX akxVertexData AkxBuildVertexData(afxDrawInput din, afxMeshBuilder const* mshb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(mshb);
    akxVertexData vtd = NIL;

    akxVertexDataSpec spec;
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
        akxVertexBias const* vb = AfxGetArrayUnit(&mshb->biases, i);
        AfxAssert(1.0 >= vb->weight);
        AfxAssertRange(mshb->artCnt, vb->pivotIdx, 1);
    }

    akxVertexAttrSpec attrSpecs[16];

    attrSpecs[0].id = "pos";
    attrSpecs[0].flags = akxVertexFlag_AFFINE | akxVertexFlag_LINEAR;
    attrSpecs[0].fmt = afxVertexFormat_V4D;
    attrSpecs[0].usage = akxVertexUsage_POS | akxVertexUsage_POSITIONAL;
    attrSpecs[0].src = mshb->posn;
    attrSpecs[0].srcStride = sizeof(mshb->posn[0]);

    attrSpecs[1].id = "nrm";
    attrSpecs[1].flags = akxVertexFlag_LINEAR;
    attrSpecs[1].fmt = afxVertexFormat_V3D;
    attrSpecs[1].usage = akxVertexUsage_NRM | akxVertexUsage_TANGENT;
    attrSpecs[1].src = mshb->nrm;
    attrSpecs[1].srcStride = sizeof(mshb->nrm[0]);

    attrSpecs[2].id = "uv";
    attrSpecs[2].flags = NIL;
    attrSpecs[2].fmt = afxVertexFormat_V2D;
    attrSpecs[2].usage = akxVertexUsage_UV;
    attrSpecs[2].src = mshb->uv;
    attrSpecs[2].srcStride = sizeof(mshb->uv[0]);

    if (AkxAcquireVertexDatas(din, attrSpecs, 1, &spec, &vtd))
        AfxThrowError();

    return vtd;
}

_AVX void AkxTransformVertexDatas(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxBool renormalize, afxNat cnt, akxVertexData vtd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(atv);
    AfxAssert(ltm);
    AfxAssert(iltm);
    AfxAssert(cnt);
    AfxAssert(vtd);

    for (afxNat i = 0; i < cnt; i++)
    {
        akxVertexData vtd2 = vtd[i];

        if (vtd2)
        {
            AfxAssertObjects(1, &vtd2, afxFcc_VTD);

            afxNat baseVtx = 0;
            afxNat vtxCnt = vtd2->vtxCnt;

            for (afxNat j = 0; j < vtd2->attrCnt; j++)
            {
                void* data = AkxExposeVertexData(vtd2, j, baseVtx);
                akxVertexFlags flags = AkxGetVertexAttributeFlags(vtd2, j);
                afxBool linearFlag = flags & akxVertexFlag_LINEAR;
                afxBool invLinearFlag = flags & akxVertexFlag_LINEAR_INV;
                afxBool affineFlag = flags & akxVertexFlag_AFFINE;
                afxBool deltaFlag = flags & akxVertexFlag_DELTA;
                afxBool normalizedFlag = flags & akxVertexFlag_NORMALIZED;
                akxVertexUsage usage = AkxGetVertexAttributeUsage(vtd2, j);
                afxVertexFormat fmt = AkxGetVertexAttributeFormat(vtd2, j);
                afxBool normalized;

                if (usage & akxVertexUsage_POS)
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
                else if (usage & akxVertexUsage_TAN)
                {
                    AfxM3dPostMultiplyV3d(ltm, vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }
                else if (usage & akxVertexUsage_BIT)
                {
                    AfxM3dPostMultiplyV3d(ltm, vtxCnt, data, data);

                    normalizedFlag = TRUE;
                    normalized = TRUE;
                }                
                else if (usage & akxVertexUsage_TBC)
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
                else if (usage & akxVertexUsage_NRM)
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
                if (data && AkxGetVertexAttributeFlags(vtd2, j) & akxVertexUsage_SPATIAL)
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
