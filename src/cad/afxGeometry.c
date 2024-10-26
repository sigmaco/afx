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
#include "qwadro/inc/math/afxReal16.h"

_AVX afxDrawInput AfxGetGeometryDrawInput(afxGeometry geo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    afxDrawInput din = AfxGetProvider(geo);
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return din;
}

_AVX afxError AfxDescribeGeometry(afxGeometry geo, afxGeometryInfo* info)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT(info);
    info->vtxCnt = geo->vtxCnt;
    info->attrCnt = geo->attrCnt;
    return err;
}

_AVX afxError AfxDescribeGeometryRow(afxGeometry geo, afxUnit attrIdx, afxGeometryRow* info)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT_RANGE(geo->attrCnt, attrIdx, 1);
    AFX_ASSERT(info);

    // sanitize arguments
    attrIdx = AfxMin(attrIdx, geo->attrCnt - 1);

    info->fmt = geo->attrs[attrIdx].fmt;
    info->flags = geo->attrs[attrIdx].flags;
    AfxMakeString8(&info->usage, &geo->attrs[attrIdx].usage.str);
    return err;
}

_AVX afxUnit AfxFindGeometryRows(afxGeometry geo, afxUnit cnt, afxString const usages[], afxUnit indices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT(indices);
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
        indices[i] = AFX_INVALID_INDEX;

    for (afxUnit i = 0; i < geo->attrCnt; i++)
    {
        afxString str = geo->attrs[i].usage.str;

        if (0 == AfxCompareStrings(&usages[rslt], TRUE, 1, &str))
            indices[rslt++] = i;

        if (cnt == rslt)
            break;
    }
    return rslt;
}

_AVX afxError AfxResetGeometry(afxGeometry geo, afxUnit attrIdx, afxVertexFormat fmt, afxVertexFlags flags, afxString const* usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT_RANGE(geo->attrCnt, attrIdx, 1);

    // sanitize arguments
    attrIdx = AfxMin(attrIdx, geo->attrCnt - 1);

    avxGeometryAttr* attr = &geo->attrs[attrIdx];
    attr->fmt = fmt;
    attr->flags = flags;
    AfxCopyString(&attr->usage.str, usage);
    AfxResetBoxes(1, &attr->aabb);

    if (geo->data[attrIdx])
        AfxDeallocate(geo->data[attrIdx]), geo->data[attrIdx] = NIL;

    return err;
}

_AVX afxError AfxUpdateGeometry(afxGeometry geo, afxUnit attrIdx, afxUnit baseVtx, afxUnit vtxCnt, void const* src, afxUnit32 srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT_RANGE(geo->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(geo->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT(src);

    // sanitize arguments
    attrIdx = AfxMin(attrIdx, geo->attrCnt - 1);
    baseVtx = AfxMin(baseVtx, geo->vtxCnt - 1);
    vtxCnt = AfxMin(vtxCnt, geo->vtxCnt - baseVtx);

    afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
    afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!geo->data[attrIdx] && !(geo->data[attrIdx] = AfxAllocate(geo->vtxCnt, unitSiz, AFX_SIMD_ALIGNMENT, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        AfxStream2(vtxCnt, src, srcStride, &(geo->data[attrIdx][unitSiz * baseVtx]), unitSiz);
        //AfxStream(vtxCnt, srcStride, unitSiz, src, geo->attrs[attrIdx].data);
        geo->attrs[attrIdx].aabbUpdReq = TRUE;
    }
    return err;
}

_AVX afxError AfxUploadGeometry(afxGeometry geo, afxUnit attrIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream in, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT_RANGE(geo->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(geo->vtxCnt, baseVtx, vtxCnt);
    AfxAssertObjects(1, &in, afxFcc_IOB);

    // sanitize arguments
    attrIdx = AfxMin(attrIdx, geo->attrCnt - 1);
    baseVtx = AfxMin(baseVtx, geo->vtxCnt - 1);
    vtxCnt = AfxMin(vtxCnt, geo->vtxCnt - baseVtx);

    afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
    afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!geo->data[attrIdx] && !(geo->data[attrIdx] = AfxAllocate(geo->vtxCnt, unitSiz, AFX_SIMD_ALIGNMENT, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        AfxReadStream2(in, vtxCnt * unitSiz, stride, &geo->data[attrIdx][baseVtx * unitSiz], unitSiz);
        //AfxStream2(vtxCnt, src, srcStride, &(geo->data[attrIdx][unitSiz * baseVtx]), unitSiz);
        //AfxStream(vtxCnt, srcStride, unitSiz, src, geo->attrs[attrIdx].data);
        geo->attrs[attrIdx].aabbUpdReq = TRUE;
    }
    return err;
}

_AVX afxError AfxDownloadGeometry(afxGeometry geo, afxUnit attrIdx, afxUnit baseVtx, afxUnit vtxCnt, afxStream out, afxUnit stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT_RANGE(geo->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(geo->vtxCnt, baseVtx, vtxCnt);
    AfxAssertObjects(1, &out, afxFcc_IOB);

    // sanitize arguments
    attrIdx = AfxMin(attrIdx, geo->attrCnt - 1);
    baseVtx = AfxMin(baseVtx, geo->vtxCnt - 1);
    vtxCnt = AfxMin(vtxCnt, geo->vtxCnt - baseVtx);

    afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
    afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!geo->data[attrIdx] && !(geo->data[attrIdx] = AfxAllocate(geo->vtxCnt, unitSiz, AFX_SIMD_ALIGNMENT, AfxHere())))
        AfxThrowError();

    if (!err)
    {
        AfxDoStreamOutput(out, stride, vtxCnt, &geo->data[attrIdx][baseVtx * unitSiz], unitSiz);
    }
    return err;
}

_AVX void* AfxAccessGeometry(afxGeometry geo, afxUnit attrIdx, afxUnit baseVtx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT_RANGE(geo->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(geo->vtxCnt, baseVtx, 1);

    // sanitize arguments
    attrIdx = AfxMin(attrIdx, geo->attrCnt - 1);
    baseVtx = AfxMin(baseVtx, geo->vtxCnt - 1);

    afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
    afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);

    if (!geo->data[attrIdx] && !(geo->data[attrIdx] = AfxAllocate(geo->vtxCnt, unitSiz, AFX_SIMD_ALIGNMENT, AfxHere())))
        AfxThrowError();

    afxByte *data = geo->data[attrIdx];

    if (!data)
        AfxThrowError();
    else
        data += baseVtx * unitSiz;

    return data;
}

_AVX afxError AfxInvertGeometry(afxGeometry geo, afxUnit attrIdx, afxUnit baseVtx, afxUnit vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT_RANGE(geo->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(geo->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT(geo->attrs[attrIdx].flags & afxVertexFlag_NORMALIZED);

    // sanitize arguments
    attrIdx = AfxMin(attrIdx, geo->attrCnt - 1);
    baseVtx = AfxMin(baseVtx, geo->vtxCnt - 1);
    vtxCnt = AfxMin(vtxCnt, geo->vtxCnt - baseVtx);

    if (!(geo->attrs[attrIdx].flags & afxVertexFlag_NORMALIZED)) AfxThrowError();
    else if (geo->data[attrIdx])
    {
        afxV3d* nrm = AfxAccessGeometry(geo, attrIdx, baseVtx);

        for (afxUnit i = 3; i < vtxCnt; i++)
            AfxV3dScale(nrm[i], nrm[i], -1.f);

        geo->attrs[attrIdx].aabbUpdReq = TRUE;
    }
    return err;
}

_AVX afxError AfxNormalizeGeometry(afxGeometry geo, afxUnit attrIdx, afxUnit baseVtx, afxUnit vtxCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    AFX_ASSERT_RANGE(geo->attrCnt, attrIdx, 1);
    AFX_ASSERT_RANGE(geo->vtxCnt, baseVtx, vtxCnt);
    AFX_ASSERT(geo->attrs[attrIdx].flags & afxVertexFlag_NORMALIZED);

    // sanitize arguments
    attrIdx = AfxMin(attrIdx, geo->attrCnt - 1);
    baseVtx = AfxMin(baseVtx, geo->vtxCnt - 1);
    vtxCnt = AfxMin(vtxCnt, geo->vtxCnt - baseVtx);

    if (!(geo->attrs[attrIdx].flags & afxVertexFlag_NORMALIZED)) AfxThrowError();
    else if (geo->data[attrIdx])
    {
        afxVertexFormat fmt = geo->attrs[attrIdx].fmt;
        AFX_ASSERT(fmt == afxVertexFormat_V2D || fmt == afxVertexFormat_V3D || fmt == afxVertexFormat_V4D);
        //afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);

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

        if (!err)
        {
            geo->attrs[attrIdx].aabbUpdReq = TRUE;
        }
    }
    return err;
}

#define GET_ADDR(base_, stride_, idx_) (void*)((afxByte*)base_ + stride_ * idx_)
#define GET_CONST_ADDR(base_, stride_, idx_) (void const*)((afxByte const*)base_ + stride_ * idx_)

_AVX afxError AfxConvert2
(
    afxUnit cnt,
    void const* src,
    afxVertexFormat srcFmt,
    afxUnit srcStride,
    void* dst,
    afxVertexFormat dstFmt,
    afxUnit dstStride,
    afxBool normalized
)
{
    afxError err = AFX_ERR_NONE;

    afxUnit srcCompCnt = 0;
    afxUnit dstCompCnt = 0;

    switch (srcFmt)
    {
    case afxVertexFormat_R32:
    case afxVertexFormat_R16:
    case afxVertexFormat_U32:
    case afxVertexFormat_I32:
    case afxVertexFormat_U16:
    case afxVertexFormat_I16:
    case afxVertexFormat_U8:
    case afxVertexFormat_I8:
        srcCompCnt = 1; break;
    case afxVertexFormat_R32x2:
    case afxVertexFormat_R16x2:
    case afxVertexFormat_U32x2:
    case afxVertexFormat_I32x2:
    case afxVertexFormat_U16x2:
    case afxVertexFormat_I16x2:
    case afxVertexFormat_U8x2:
    case afxVertexFormat_I8x2:
        srcCompCnt = 2; break;
    case afxVertexFormat_R32x3:
    //case afxVertexFormat_U32x3:
    //case afxVertexFormat_I32x3:
        srcCompCnt = 3; break;
    case afxVertexFormat_R32x4:
    case afxVertexFormat_R16x4:
    case afxVertexFormat_U32x4:
    case afxVertexFormat_I32x4:
    case afxVertexFormat_U16x4:
    case afxVertexFormat_I16x4:
    case afxVertexFormat_U8x4:
    case afxVertexFormat_I8x4:
        srcCompCnt = 4; break;
    default:
        break;
    }

    switch (dstFmt)
    {
    case afxVertexFormat_R32:
    case afxVertexFormat_R16:
    case afxVertexFormat_U32:
    case afxVertexFormat_I32:
    case afxVertexFormat_U16:
    case afxVertexFormat_I16:
    case afxVertexFormat_U8:
    case afxVertexFormat_I8:
        dstCompCnt = 1; break;
    case afxVertexFormat_R32x2:
    case afxVertexFormat_R16x2:
    case afxVertexFormat_U32x2:
    case afxVertexFormat_I32x2:
    case afxVertexFormat_U16x2:
    case afxVertexFormat_I16x2:
    case afxVertexFormat_U8x2:
    case afxVertexFormat_I8x2:
        dstCompCnt = 2; break;
    case afxVertexFormat_R32x3:
        //case afxVertexFormat_U32x3:
        //case afxVertexFormat_I32x3:
        dstCompCnt = 3; break;
    case afxVertexFormat_R32x4:
    case afxVertexFormat_R16x4:
    case afxVertexFormat_U32x4:
    case afxVertexFormat_I32x4:
    case afxVertexFormat_U16x4:
    case afxVertexFormat_I16x4:
    case afxVertexFormat_U8x4:
    case afxVertexFormat_I8x4:
        dstCompCnt = 4; break;
    default:
        break;
    }

    switch (dstFmt)
    {
    case afxVertexFormat_R32:
    case afxVertexFormat_R32x2:
    case afxVertexFormat_R32x3:
    case afxVertexFormat_R32x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32:
        case afxVertexFormat_R32x2:
        case afxVertexFormat_R32x3:
        case afxVertexFormat_R32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_R16:
        case afxVertexFormat_R16x2:
        case afxVertexFormat_R16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxUnpackReal16(src4[j]);
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
        //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
        //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxVertexFormat_R16:
    case afxVertexFormat_R16x2:
    case afxVertexFormat_R16x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32:
        case afxVertexFormat_R32x2:
        case afxVertexFormat_R32x3:
        case afxVertexFormat_R32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16(src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_R16:
        case afxVertexFormat_R16x2:
        case afxVertexFormat_R16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
        //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
        //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxReal16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = AfxPackReal16((afxReal)src4[j]);
                    else
                        dst3[j] = AfxPackReal16(0.f);
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
        break;
    }
    case afxVertexFormat_U32:
    case afxVertexFormat_U32x2:
    //case afxVertexFormat_U32x3:
    case afxVertexFormat_U32x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32: // real32 to nat32
        case afxVertexFormat_R32x2: // real32 to nat32
        case afxVertexFormat_R32x3: // real32 to nat32
        case afxVertexFormat_R32x4: // real32 to nat32
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0.f;
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
        //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
        //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit32* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    case afxVertexFormat_U16:
    case afxVertexFormat_U16x2:
    case afxVertexFormat_U16x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x2: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x3: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x4: // real32 to nat8 (normalized or raw)
        {
            if (normalized)
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j] * 65535.f;
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            else
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j];
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
        //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
        //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit16* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    case afxVertexFormat_U8:
    case afxVertexFormat_U8x2:
    case afxVertexFormat_U8x4:
    {
        switch (srcFmt)
        {
        case afxVertexFormat_R32: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x2: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x3: // real32 to nat8 (normalized or raw)
        case afxVertexFormat_R32x4: // real32 to nat8 (normalized or raw)
        {
            if (normalized)
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j] * 255.f;
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            else
            {
                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                    afxReal32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                    for (afxUnit j = 0; j < dstCompCnt; j++)
                    {
                        if (j < srcCompCnt)
                            dst3[j] = src4[j];
                        else
                            dst3[j] = 0.f;
                    }
                }
            }
            break;
        }
        case afxVertexFormat_U32:
        case afxVertexFormat_U32x2:
        //case afxVertexFormat_U32x3:
        case afxVertexFormat_U32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I32:
        case afxVertexFormat_I32x2:
        //case afxVertexFormat_I32x3:
        case afxVertexFormat_I32x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt32 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U16:
        case afxVertexFormat_U16x2:
        case afxVertexFormat_U16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I16:
        case afxVertexFormat_I16x2:
        case afxVertexFormat_I16x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt16 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_U8:
        case afxVertexFormat_U8x2:
        case afxVertexFormat_U8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxUnit8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        case afxVertexFormat_I8:
        case afxVertexFormat_I8x2:
        case afxVertexFormat_I8x4:
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                afxUnit8* dst3 = GET_ADDR(dst, dstStride, i);
                afxInt8 const* src4 = GET_CONST_ADDR(src, srcStride, i);

                for (afxUnit j = 0; j < dstCompCnt; j++)
                {
                    if (j < srcCompCnt)
                        dst3[j] = src4[j];
                    else
                        dst3[j] = 0;
                }
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    default: AfxThrowError();  break;
    }

    return err;
}

_AVX afxError AfxBufferizeGeometry(afxGeometry geo, avxVertexCache* vtxCache)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);

    if (geo->cache.buf)
    {
        if (vtxCache)
            *vtxCache = geo->cache;

        return err;
    }

    afxString const positionals[] = { AfxString("pos"), AfxString("pvt"), AfxString("wgt") };

    afxUnit cacheStride[2] = { 0 };
    afxUnit cacheIdx[AVX_MAX_GEOMETRY_ATTRIBUTES];
    afxUnit cachedAttrOffset[AVX_MAX_GEOMETRY_ATTRIBUTES];
    afxVertexFormat cachedAttrFmt[AVX_MAX_GEOMETRY_ATTRIBUTES];

    for (afxUnit i = 0; i < geo->attrCnt; i++)
    {
        avxGeometryAttr* attr = &geo->attrs[i];
        afxVertexFormat fmt = attr->fmt;
        AFX_ASSERT(fmt < afxVertexFormat_TOTAL);

        switch (AfxCompareStrings(&attr->usage.str, FALSE, ARRAY_SIZE(positionals), positionals))
        {
        case 0: // pos
        {
            cachedAttrFmt[i] = afxVertexFormat_V3D;
            cacheIdx[i] = 0;
            break;
        }
        case 1: // pvt
        {
            switch (fmt)
            {
            case afxVertexFormat_U16:
            case afxVertexFormat_U32:
            case afxVertexFormat_I8:
            case afxVertexFormat_I16:
            case afxVertexFormat_I32:
                cachedAttrFmt[i] = afxVertexFormat_U8;
                break;
            default:
                cachedAttrFmt[i] = fmt;
                break;
            }
            cacheIdx[i] = 0;
            break;
        }
        case 2: // wgt
        {
            switch (fmt)
            {
            case afxVertexFormat_REAL:
                cachedAttrFmt[i] = afxVertexFormat_U8;
                break;
            case afxVertexFormat_V2D:
                cachedAttrFmt[i] = afxVertexFormat_U8x2;
                break;
            case afxVertexFormat_V3D:
                cachedAttrFmt[i] = afxVertexFormat_U8x4;
                break;
            case afxVertexFormat_V4D:
                cachedAttrFmt[i] = afxVertexFormat_U8x4;
                break;
            default:
                cachedAttrFmt[i] = fmt;
                break;
            }
            cacheIdx[i] = 0;
            break;
        }
        default:
        {
            cachedAttrFmt[i] = attr->fmt;
            cacheIdx[i] = 1;
            break;
        }
        }
        cachedAttrOffset[i] = cacheStride[cacheIdx[i]];
        cacheStride[cacheIdx[i]] += AfxVertexFormatGetSize(cachedAttrFmt[i]);
    }

    avxVertexCache* cache = &geo->cache;
    AfxPushLinkage(&cache->vbuf, NIL);
    cache->vin = NIL;

    afxBufferInfo vboSpec = { 0 };
    vboSpec.flags = afxBufferFlag_W;
    vboSpec.usage = afxBufferUsage_VERTEX;
    vboSpec.bufCap = 0;

    for (afxUnit i = 0; i < 2; i++)
    {
        cache->streams[i].base = i ? geo->cache.streams[i - 1].range : 0;
        cache->streams[i].range = AFX_ALIGNED_SIZE(cacheStride[i] * geo->vtxCnt, AFX_BUF_ALIGNMENT);
        cache->streams[i].stride = cacheStride[i];

        vboSpec.bufCap += cache->streams[i].range;
    }

    afxBuffer buf;
    afxDrawContext dctx;
    afxDrawInput din = AfxGetGeometryDrawInput(geo);
    AfxGetDrawInputContext(din, &dctx);
    AfxAcquireBuffers(/*din*/dctx, 1, &vboSpec, &buf);
    cache->buf = buf;

    for (afxUnit srcIdx = 0; srcIdx < 2; srcIdx++)
    {
        AFX_ASSERT(cacheStride[srcIdx]);
        AFX_ASSERT_RANGE(AfxGetBufferCapacity(cache->buf, 0), cache->streams[srcIdx].base, cache->streams[srcIdx].range);
        void* dst;

        if (AfxMapBuffer(cache->buf, cache->streams[srcIdx].base, cache->streams[srcIdx].range, NIL, &dst))
            AfxThrowError();
        else
        {
            for (afxUnit j = 0; j < geo->attrCnt; j++)
            {
                if (cacheIdx[j] == srcIdx)
                {
                    avxGeometryAttr* attr = &geo->attrs[j];
                    //AFX_ASSERT(data);
                    afxVertexFormat fmt = attr->fmt;
                    AFX_ASSERT(fmt < afxVertexFormat_TOTAL);

                    afxByte* data = AfxAccessGeometry(geo, j, 0);

                    if (data)
                    {
                        afxSize srcStride = AfxVertexFormatGetSize(fmt);
                        AFX_ASSERT(srcStride);
                        AfxStream3(geo->vtxCnt, data, 0, srcStride, dst, cachedAttrOffset[j], cache->streams[srcIdx].stride);
                    }
                }
            }
            AfxUnmapBuffer(cache->buf, TRUE);
        }
    }

    if (vtxCache)
        *vtxCache = *cache;

    return err;
}

_AVX afxError _AvxGeomDtorCb(afxGeometry geo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);

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
        },
        {
            .cnt = geo->vtxToVtxMap ? geo->vtxCnt : 0,
            .siz = sizeof(geo->vtxToVtxMap[0]),
            .var = (void**)&geo->vtxToVtxMap
        }
    };

    if (AfxDeallocateInstanceData(geo, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxGeomCtorCb(afxGeometry geo, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &geo, afxFcc_GEO);
    
    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxGeometryInfo const* info = AFX_CAST(afxGeometryInfo const*, args[1]) + invokeNo;
    
    afxUnit vtxCnt = info->vtxCnt;

    AFX_ASSERT(vtxCnt); // at least one
    geo->vtxCnt = vtxCnt;
            
    afxUnit attrCnt = info->attrCnt;

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
        },
        {
            .cnt = info->vtxToVtxMap ? vtxCnt : 0,
            .siz = sizeof(geo->vtxToVtxMap[0]),
            .align = AFX_SIMD_ALIGNMENT,
            .var = (void**)&geo->vtxToVtxMap
        }
    };

    if (AfxAllocateInstanceData(geo, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        geo->attrCnt = attrCnt;
        
        for (afxUnit i = 0; i < attrCnt; i++)
        {
            avxGeometryAttr* attr = &geo->attrs[i];

            attr->fmt = afxVertexFormat_V4D;
            attr->flags = NIL;
            AfxMakeString8(&attr->usage, NIL);

            geo->data[i] = NIL;
        }

        if (err && AfxDeallocateInstanceData(geo, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }
    return err;
}

_AVX afxClassConfig _AvxGeomClsCfg =
{
    .fcc = afxFcc_GEO,
    .name = "Geometry",
    .desc = "Geometry",
    .fixedSiz = sizeof(AFX_OBJECT(afxGeometry)),
    .ctor = (void*)_AvxGeomCtorCb,
    .dtor = (void*)_AvxGeomDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireGeometries(afxDrawInput din, afxUnit cnt, afxGeometryInfo const infos[], afxGeometry geometries[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AFX_ASSERT(geometries);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)AfxGetGeometryClass(din);
    AfxAssertClass(cls, afxFcc_GEO);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)geometries, (void const*[]) { din, infos }))
        AfxThrowError();

    return err;
}

_AVX afxGeometry AfxBuildGeometry(afxDrawInput din, afxTriangulation const* mshb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AFX_ASSERT(mshb);
    afxGeometry geo = NIL;

    afxGeometryInfo spec;
    spec.vtxCnt = mshb->vtxCnt;
    spec.attrCnt = 3;
#if 0
    spec.vtxSrc = mshb->vtx;
    spec.vtxSrcStride = sizeof(mshb->vtx[0]);
    spec.biasCnt = mshb->biases.cnt;
    spec.biasSrcStride = mshb->biases.unitSiz;
    spec.biasSrc = mshb->biases.data;
    spec.baseAttrIdx = 0;

    for (afxUnit i = 0; i < spec.biasCnt; i++)
    {
        afxVertexBias const* vb = AfxGetArrayUnit(&mshb->biases, i);
        AFX_ASSERT(1.0 >= vb->weight);
        AFX_ASSERT_RANGE(mshb->artCnt, vb->pivotIdx, 1);
    }
#endif

    if (AfxAcquireGeometries(din, 1, &spec, &geo))
        AfxThrowError();

    AfxResetGeometry(geo, 0, afxVertexFormat_V4D, afxVertexFlag_ATV | afxVertexFlag_LTM, &AfxString("pos"));
    AfxResetGeometry(geo, 1, afxVertexFormat_V3D, afxVertexFlag_LTM, &AfxString("nrm"));
    AfxResetGeometry(geo, 2, afxVertexFormat_V2D, NIL, &AfxString("uv"));

    AfxUpdateGeometry(geo, 0, 0, spec.vtxCnt, mshb->posn, sizeof(mshb->posn[0]));
    AfxUpdateGeometry(geo, 1, 0, spec.vtxCnt, mshb->nrm, sizeof(mshb->nrm[0]));
    AfxUpdateGeometry(geo, 2, 0, spec.vtxCnt, mshb->uv, sizeof(mshb->uv[0]));

    return geo;
}

_AVX void AfxTransformGeometries(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxBool renormalize, afxUnit cnt, afxGeometry geometries[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(geometries);
    AFX_ASSERT(atv);
    AFX_ASSERT(ltm);
    AFX_ASSERT(iltm);
    AFX_ASSERT(cnt);

    for (afxUnit geoIdx = 0; geoIdx < cnt; geoIdx++)
    {
        afxGeometry geo = geometries[geoIdx];

        if (!geo)
            continue;

        AfxAssertObjects(1, &geo, afxFcc_GEO);

        afxGeometryInfo geoi;
        AfxDescribeGeometry(geo, &geoi);

        for (afxUnit j = 0; j < geo->attrCnt; j++)
        {
            afxGeometryRow geor;
            AfxDescribeGeometryRow(geo, j, &geor);

            void* data = AfxAccessGeometry(geo, j, 0);
            afxBool linearFlag = geor.flags & afxVertexFlag_LTM;
            afxBool invLinearFlag = geor.flags & afxVertexFlag_ILTM;
            afxBool affineFlag = geor.flags & afxVertexFlag_ATV;
            afxBool deltaFlag = geor.flags & afxVertexFlag_DELTA;
            afxBool normalizedFlag = geor.flags & afxVertexFlag_NORMALIZED;
            afxVertexFormat fmt = geor.fmt;
            afxBool normalized;
#if 0
            if (geor.flags & afxVertexFlag_ATV)
            {
                if (fmt == afxVertexFormat_V4D)
                    AfxM3dPostMultiplyLtv4d(ltm, geoi.vtxCnt, data, data);
                else if (fmt == afxVertexFormat_V3D)
                    AfxM3dPostMultiplyV3d(ltm, geoi.vtxCnt, data, data);

                if (!(geor.flags & afxVertexFlag_DELTA))
                {
                    if (fmt == afxVertexFormat_V4D)
                        for (afxUnit k = 0; k < geoi.vtxCnt; k++)
                            AfxV4dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], atv);
                    else if (fmt == afxVertexFormat_V3D)
                        for (afxUnit k = 0; k < geoi.vtxCnt; k++)
                            AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], atv);
                }
                normalized = FALSE;
            }
#endif
            afxString const attrs[] = { AfxString("pos"), AfxString("tan"), AfxString("bit"), AfxString("tbc"), AfxString("nrm") };

            switch (AfxCompareStrings(&geor.usage.str, FALSE, ARRAY_SIZE(attrs), attrs))
            {
            case 0: // pos
            {
                if (fmt == afxVertexFormat_V4D)
                {
                    AfxM3dPostMultiplyLtv4d(ltm, geoi.vtxCnt, data, data);

                    if (!deltaFlag)
                        for (afxUnit k = 0; k < geoi.vtxCnt; k++)
                            AfxV4dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], atv);
                }
                else if (fmt == afxVertexFormat_V3D)
                {
                    AfxM3dPostMultiplyV3d(ltm, geoi.vtxCnt, data, data);

                    if (!deltaFlag)
                        for (afxUnit k = 0; k < geoi.vtxCnt; k++)
                            AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], atv);
                }

                normalized = FALSE;
                break;
            }
            case 1: // tan
            {
                AfxM3dPostMultiplyV3d(ltm, geoi.vtxCnt, data, data);

                normalizedFlag = TRUE;
                normalized = TRUE;
                break;
            }
            case 2: // bit
            {
                AfxM3dPostMultiplyV3d(ltm, geoi.vtxCnt, data, data);

                normalizedFlag = TRUE;
                normalized = TRUE;
                break;
            }
            case 3: // tbc
            {
                if (deltaFlag)
                {
                    AfxM3dPostMultiplyV3d(ltm, geoi.vtxCnt, data, data);
                }
                else
                {
                    AfxM3dPostMultiplyV3d(iltm, geoi.vtxCnt, data, data);
                }

                normalizedFlag = TRUE;
                normalized = TRUE;
                break;
            }
            case 4: // nrm
            {
                if (deltaFlag)
                {
                    AfxM3dPostMultiplyV3d(ltm, geoi.vtxCnt, data, data);
                }
                else
                {
                    AfxM3dPostMultiplyV3d(iltm, geoi.vtxCnt, data, data);
                }

                normalizedFlag = TRUE;
                normalized = TRUE;
                break;
            }
            default: break;
            }

            if (renormalize && normalized)
            {
                if (fmt == afxVertexFormat_V2D)
                    AfxZeroOrNormalizeArrayedV2d(geoi.vtxCnt, data, data);
                else if (fmt == afxVertexFormat_V3D)
                    AfxZeroOrNormalizeArrayedV3d(geoi.vtxCnt, data, data);
                else if (fmt == afxVertexFormat_V4D)
                    AfxZeroOrNormalizeArrayedV4d(geoi.vtxCnt, data, data);
            }

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
                        for (afxUnit k = 0; k < vtxCnt; k++)
                            AfxV3dAdd(((afxV3d*)data)[k], ((afxV3d*)data)[k], at);

                    if (renormalize)
                        for (afxUnit k = 0; k < vtxCnt; k++)
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
                        for (afxUnit k = 0; k < vtxCnt; k++)
                            AfxV3dAdd(((afxV4d*)data)[k], ((afxV4d*)data)[k], at);

                    if (renormalize)
                        for (afxUnit k = 0; k < vtxCnt; k++)
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

AFX_DEFINE_STRUCT(urdGeo)
{
    afxUnit32 vtxCnt;
    afxUnit32 attrCnt;
    afxV3d  aabbMin;
    afxV3d  aabbMax;
};

AFX_DEFINE_STRUCT(urdGeoVa)
{
    afxUnit32 fmt;
    afxUnit32 flags;
    afxChar8 usage[8];
};

_AVX afxError AfxStoreGeometries(afxStream out, afxUnit cnt, afxGeometry geometries[])
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOB);
    AfxAssertObjects(cnt, geometries, afxFcc_GEO);
    afxUnit geoIdx = 0;

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
        
        for (afxUnit i = 0; i < geo->attrCnt; i++)
        {
            afxVertexFormat fmt = geo->attrs[i].fmt;
            afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);
            
            mark.siz += sizeof(urdGeoVa) + unitSiz * geo->vtxCnt;
        }

        if (AfxWriteStream(out, sizeof(mark), 0, &mark))
            AfxThrowError();

        AfxV3dCopy(nfo.aabbMax, geo->aabb.sup);
        AfxV3dCopy(nfo.aabbMin, geo->aabb.inf);
        
        AfxWriteStream(out, sizeof(nfo), 0, &nfo);

        for (afxUnit i = 0; i < geo->attrCnt; i++)
        {
            urdGeoVa attrNfo =
            {
                .fmt = geo->attrs[i].fmt,
                .flags = geo->attrs[i].flags
            };
            AfxDumpString(&geo->attrs[i].usage.str, 0, geo->attrs[i].usage.str.len, attrNfo.usage);

            AfxWriteStream(out, sizeof(attrNfo), 0, &attrNfo);
        }

        for (afxUnit i = 0; i < geo->attrCnt; i++)
        {
            afxVertexFormat fmt = geo->attrs[i].fmt;
            afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);

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
    AFX_ASSERT(geometry);

    urdGeo nfo;

    if (AfxReadStream(in, sizeof(nfo), 0, &nfo))
        AfxThrowError();

    afxGeometry geo;
    afxGeometryInfo geob = { 0 };
    geob.attrCnt = nfo.attrCnt;
    geob.vtxCnt = nfo.vtxCnt;
    AfxAcquireGeometries(din, 1, &geob, &geo);

    afxString8 tags[16];

    for (afxUnit i = 0; i < nfo.attrCnt; i++)
    {
        urdGeoVa attrNfo;

        AfxReadStream(in, sizeof(attrNfo), 0, &attrNfo);

        afxString s;
        AfxMakeString(&s, 0, attrNfo.usage, 0);
        AfxResetGeometry(geo, i, attrNfo.fmt, attrNfo.flags, &s);
    }

    for (afxUnit i = 0; i < nfo.attrCnt; i++)
    {
        afxVertexFormat fmt = geo->attrs[i].fmt;
        afxUnit32 unitSiz = AfxVertexFormatGetSize(fmt);

        AfxUploadGeometry(geo, i, 0, nfo.vtxCnt, in, unitSiz);
    }

    *geometry = geo;

    return err;
}


