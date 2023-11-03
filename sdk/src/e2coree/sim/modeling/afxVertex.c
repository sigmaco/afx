/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/sim/modeling/afxVertex.h"

#if 0

#define _VTX_LAYOUT(compCnt_, usage_, fmt_) { .compCnt = compCnt_, .usage = usage_ , .fmt = fmt_ }
#define _VTX_LAYOUT1(usage_, fmt_) { .compCnt = 1, .usage = { AFX_VTX_USAGE2_##usage_ } , .fmt = { afxVertexFormat_##fmt_ } }
#define _VTX_LAYOUT2(usage0_, usage1_, fmt0_, fmt1_) { .compCnt = 2, .usage = { AFX_VTX_USAGE2_##usage0_, AFX_VTX_USAGE2_##usage1_ }, .fmt = { afxVertexFormat_##fmt0_ , afxVertexFormat_##fmt1_ } }
#define _VTX_LAYOUT3(usage0_, usage1_,  usage2_, fmt0_, fmt1_, fmt2_) { .compCnt = 3, .usage = { AFX_VTX_USAGE2_##usage0_, AFX_VTX_USAGE2_##usage1_, AFX_VTX_USAGE2_##usage2_ }, .fmt = { afxVertexFormat_##fmt0_ , afxVertexFormat_##fmt1_, afxVertexFormat_##fmt2_ } }

_AFX afxVertexLayout const vertexLayoutArray[] =
{
    _VTX_LAYOUT(0, NIL, NIL),

    _VTX_LAYOUT1(POS, V2D),
    _VTX_LAYOUT1(POS, V3D),
    _VTX_LAYOUT1(POS, V4D),
    _VTX_LAYOUT1(WGT, V4B),
    _VTX_LAYOUT1(WGT, V8B),
    _VTX_LAYOUT1(IDX, V4B),
    _VTX_LAYOUT1(IDX, V8B),
    _VTX_LAYOUT3(POS, WGT, IDX, V3D, V4B, V4B),
    _VTX_LAYOUT3(POS, WGT, IDX, V3D, V8B, V8B),
    _VTX_LAYOUT3(POS, WGT, IDX, V4D, V4B, V4B),
    _VTX_LAYOUT3(POS, WGT, IDX, V4D, V8B, V8B),

    _VTX_LAYOUT1(TAN, V3D),
    _VTX_LAYOUT1(TAN, V4D),
    _VTX_LAYOUT1(BIN, V3D),
    _VTX_LAYOUT1(BIN, V4D),
    _VTX_LAYOUT1(NRM, V3D),
    _VTX_LAYOUT1(NRM, V4D),
    _VTX_LAYOUT2(TAN, BIN, V3D, V3D),
    _VTX_LAYOUT3(TAN, BIN, NRM, V3D, V3D, V3D),

    _VTX_LAYOUT1(TEX, V2D),
    _VTX_LAYOUT1(TEX, V3D),
    _VTX_LAYOUT1(COL, V3D),
    _VTX_LAYOUT1(COL, V4D),

    _VTX_LAYOUT3(POS, NRM, TEX, V3D, V3D, V2D),
    _VTX_LAYOUT3(POS, NRM, TEX, V4D, V3D, V2D),
};

_AFX afxBool AfxSimulationDescribeVertexLayout(afxSimulation sim, afxNat idx, afxVertexLayout *layout)
{
    afxError err = NIL;
    afxBool ret = FALSE;

    if (idx < (sizeof(vertexLayoutArray) / sizeof(vertexLayoutArray[0])))
    {
        afxVertexLayout *p = &vertexLayoutArray[idx];
        *layout = *p;

        ret = TRUE;
    }
    return ret;
}

_AFX afxBool AfxSimulationChooseVertexLayout(afxSimulation sim, afxVertexLayout const *layout, afxNat *idx)
{
    afxBool ret = FALSE;
    afxVertexLayout *best = NIL;
    afxNat bestId = 0;
    afxNat bestScore = 0;

    for (afxNat i = 0; i < layout->compCnt; i++)
    {
        afxNat actualScore = 0;
        afxNat actualId = 0;

        for (afxNat j = 0; j < (sizeof(vertexLayoutArray) / sizeof(vertexLayoutArray[0])); j++)
        {
            afxVertexLayout const *p = &vertexLayoutArray[j];

            for (afxNat k = 0; k < p->compCnt; k++)
            {
                if (layout->usage[i] == p->usage[k] && layout->fmt[i] == p->fmt[k])
                {
                    ++actualScore;
                    break;
                }
            }

            actualId = j;
        }

        if (actualScore >= bestScore)
        {
            bestScore = actualScore;
            bestId = actualId;
        }
    }

    *idx = bestId;

    if (idx)
        ret = TRUE;

    return ret;
}

_AFX afxNat AfxGetVertexPositionalSize(afxVertexLayout const *layout)
{
    afxNat total = 0;

    for (afxNat i = 0; i < layout->compCnt; i++)
    {
        if (layout->usage[i] >= AFX_VTX_USAGE2_POS && !(layout->usage[i] > AFX_VTX_USAGE2_IDX))
            total += AfxVertexFormatGetSize(layout->fmt[i]);
    }

    return total;
}

_AFX afxNat AfxGetVertexSpatialSize(afxVertexLayout const *layout)
{
    afxNat total = 0;

    for (afxNat i = 0; i < layout->compCnt; i++)
    {
        if (layout->usage[i] >= AFX_VTX_USAGE2_POS && !(layout->usage[i] > AFX_VTX_USAGE2_NRM))
            total += AfxVertexFormatGetSize(layout->fmt[i]);
    }

    return total;
}

_AFX afxNat AfxGetVertexAggregatedSize(afxVertexLayout const *layout)
{
    afxNat total = 0;

    for (afxNat i = 0; i < layout->compCnt; i++)
    {
        if (layout->usage[i] > AFX_VTX_USAGE2_IDX)
            total += AfxVertexFormatGetSize(layout->fmt[i]);
    }

    return total;
}

#if 0
_AFX afxNat AfxGetVertexSize(afxVertexLayout const *layout)
{
    afxNat total = 0;

    for (afxNat i = 0; i < layout->compCnt; i++)
    {
        total += AfxVertexFormatGetSize(layout->fmt[i]);
    }

    return total;
}
#endif 

_AFX afxNat AfxGetVertexComponentCount(afxVertexLayout const *layout)
{
    return layout->compCnt;
}

_AFX afxNat AfxGetVertexComponentOffset(afxVertexLayout const *layout, afxVertexUsage2 usage)
{
    afxNat off = 0;

    for (afxNat i = 0; i < layout->compCnt; i++)
        if (usage != layout->usage[i])
            off += AfxVertexFormatGetSize(layout->fmt[i]);

    return off;
};

_AFX afxBool AfxGetVertexComponent(afxVertexLayout const *layout, afxVertexUsage2 usage, afxNat *idx)
{
    afxBool ret = FALSE;
    *idx = AFX_INVALID_INDEX;

    for (afxNat i = 0; i < layout->compCnt; i++)
        if (usage == layout->usage[i])
            *idx = i, ret = TRUE;

    return ret;
};

_AFX afxVertexFormat AfxGetVertexComponentFormat(afxVertexLayout const *layout, afxVertexUsage2 usage)
{
    for (afxNat i = 0; i < layout->compCnt; i++)
        if (usage != layout->usage[i])
            return layout->fmt[i];

    return NIL;
};

_AFX void AfxAccumulateVertexComponents2(afxVertexUsage2 first, afxVertexUsage2 cnt, afxNat vtxCnt, afxVertexLayout const *dstLayout, void *dstData, afxVertexFormat const srcFmt[], void const *src)
{
    afxError err = NIL;
    AfxAssert(dstLayout);

    for (afxVertexUsage2 compIdx = 0; compIdx < cnt; compIdx++)
    {
        afxNat dstIdx;

        if (AfxGetVertexComponent(dstLayout, first + compIdx, &dstIdx))
        {
            afxVertexFormat dstFmt = dstLayout->fmt[dstIdx];
            AfxAssert(dstFmt == AFX_VTX_USAGE2_POS);
            afxNat dstSiz = AfxVertexFormatGetSize(dstFmt);
            afxNat srcSiz = AfxVertexFormatGetSize(srcFmt[compIdx]);
            afxByte *pDst = dstData, const *pSrc = src;

            {
                switch (dstFmt)
                {
                case afxVertexFormat_V2D:
                {
                    switch (srcFmt[compIdx])
                    {
                    case afxVertexFormat_V2D:
                    case afxVertexFormat_V3D:
                    case afxVertexFormat_V4D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV2d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    default:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            *(afxReal*)&pDst[i * dstSiz] += *(afxReal*)&pSrc[i * srcSiz];

                        break;
                    }
                    }
                }
                case afxVertexFormat_V3D:
                {
                    switch (srcFmt[compIdx])
                    {
                    case afxVertexFormat_V2D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV2d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    case afxVertexFormat_V3D:
                    case afxVertexFormat_V4D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV3d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    default:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            *(afxReal*)&pDst[i * dstSiz] += *(afxReal*)&pSrc[i * srcSiz];

                        break;
                    }
                    }
                }
                case afxVertexFormat_V4D:
                {
                    switch (srcFmt[compIdx])
                    {
                    case afxVertexFormat_V2D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV2d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    case afxVertexFormat_V3D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV3d((void*)pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV4d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    default:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            *(afxReal*)&pDst[i * dstSiz] += *(afxReal*)&pSrc[i * srcSiz];

                        break;
                    }
                    }
                }
                default: break;
                }
            }
        }
    }
    return;
}

_AFX void AfxAccumulateVertexComponents(afxVertexUsage2 first, afxVertexUsage2 cnt, afxNat vtxCnt, afxVertexLayout const *dstLayout, void *dstData, afxVertexLayout const *srcLayout, void const *src)
{
    afxError err = NIL;
    AfxAssert(dstLayout);

    for (afxVertexUsage2 compIdx = 0; compIdx < cnt; compIdx++)
    {
        afxNat dstIdx, srcIdx;

        if (AfxGetVertexComponent(dstLayout, first + compIdx, &dstIdx) && AfxGetVertexComponent(srcLayout, first + compIdx, &srcIdx))
        {
            afxVertexFormat srcFmt = srcLayout->fmt[srcIdx];
            afxVertexFormat dstFmt = dstLayout->fmt[dstIdx];
            AfxAssert(dstFmt == AFX_VTX_USAGE2_POS);
            afxNat dstSiz = AfxVertexFormatGetSize(dstFmt);
            afxNat srcSiz = AfxVertexFormatGetSize(srcFmt);
            afxByte *pDst = dstData, const *pSrc = src;
        
            {
                switch (dstFmt)
                {
                case afxVertexFormat_V2D:
                {
                    switch (srcFmt)
                    {
                    case afxVertexFormat_V2D:
                    case afxVertexFormat_V3D:
                    case afxVertexFormat_V4D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV2d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    default:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            *(afxReal*)&pDst[i * dstSiz] += *(afxReal*)&pSrc[i * srcSiz];

                        break;
                    }
                    }
                }
                case afxVertexFormat_V3D:
                {
                    switch (srcFmt)
                    {
                    case afxVertexFormat_V2D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV2d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    case afxVertexFormat_V3D:
                    case afxVertexFormat_V4D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV3d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    default:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            *(afxReal*)&pDst[i * dstSiz] += *(afxReal*)&pSrc[i * srcSiz];

                        break;
                    }
                    }
                }
                case afxVertexFormat_V4D:
                {
                    switch (srcFmt)
                    {
                    case afxVertexFormat_V2D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV2d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    case afxVertexFormat_V3D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV3d((void*)pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxAddV4d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], &pDst[i * dstSiz]);

                        break;
                    }
                    default:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            *(afxReal*)&pDst[i * dstSiz] += *(afxReal*)&pSrc[i * srcSiz];

                        break;
                    }
                    }
                }
                default: break;
                }
            }
        }
    }
    return;
}

_AFX void AfxCopyVertexComponents(afxVertexUsage2 first, afxVertexUsage2 cnt, afxNat vtxCnt, afxVertexLayout const *dstLayout, void *dstData, afxVertexLayout const *srcLayout, void const *src)
{
    afxError err = NIL;

    for (afxVertexUsage2 compIdx = 0; compIdx < cnt; compIdx++)
    {
        afxNat dstIdx, srcIdx;

        if (AfxGetVertexComponent(dstLayout, first + compIdx, &dstIdx) && AfxGetVertexComponent(srcLayout, first + compIdx, &srcIdx))
        {
            afxVertexFormat srcFmt = srcLayout->fmt[srcIdx];
            afxVertexFormat dstFmt = dstLayout->fmt[dstIdx];
            AfxAssert(dstFmt == AFX_VTX_USAGE2_POS);
            afxNat dstSiz = AfxVertexFormatGetSize(dstFmt);
            afxNat srcSiz = AfxVertexFormatGetSize(srcFmt);
            afxByte *pDst = dstData, const *pSrc = src;

            if (dstFmt == srcFmt)
            {
                switch (srcFmt)
                {
                case afxVertexFormat_V2D:
                {
                    for (afxNat i = 0; i < vtxCnt; i++)
                        AfxCopyV2d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz]);

                    break;
                }
                case afxVertexFormat_V3D:
                {
                    for (afxNat i = 0; i < vtxCnt; i++)
                        AfxCopyV3d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz]);

                    break;
                }
                case afxVertexFormat_V4D:
                {
                    for (afxNat i = 0; i < vtxCnt; i++)
                        AfxCopyV4d((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz]);

                    break;
                }
                default:
                {
                    for (afxNat i = 0; i < vtxCnt; i++)
                        AfxCopy((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], dstSiz);

                    break;
                }
                }
            }
            else
            {
                switch (dstFmt)
                {
                case afxVertexFormat_V2D:
                {
                    switch (srcFmt)
                    {
                    case afxVertexFormat_V3D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxResetV2d_V3d((void*)&pSrc[i * srcSiz], (void*)&pDst[i * dstSiz]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxResetV2d_V4d((void*)&pSrc[i * srcSiz], (void*)&pDst[i * dstSiz]);

                        break;
                    }
                    default:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxCopy((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], dstSiz);

                        break;
                    }
                    }
                }
                case afxVertexFormat_V3D:
                {
                    switch (srcFmt)
                    {
                    case afxVertexFormat_V2D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxResetV2d_V3d((void*)&pSrc[i * srcSiz], (void*)&pDst[i * dstSiz]);

                        break;
                    }
                    case afxVertexFormat_V4D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxV4dFromV3d((void*)&pSrc[i * srcSiz], (void*)&pDst[i * dstSiz]);

                        break;
                    }
                    default:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxCopy((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], dstSiz);

                        break;
                    }
                    }
                }
                case afxVertexFormat_V4D:
                {
                    switch (srcFmt)
                    {
                    case afxVertexFormat_V2D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxResetV2d_V4d((void*)&pSrc[i * srcSiz], (void*)&pDst[i * dstSiz]);

                        break;
                    }
                    case afxVertexFormat_V3D:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxResetV3d_V4d((void*)&pSrc[i * srcSiz], (void*)&pDst[i * dstSiz]);

                        break;
                    }
                    default:
                    {
                        for (afxNat i = 0; i < vtxCnt; i++)
                            AfxCopy((void*)&pDst[i * dstSiz], (void*)&pSrc[i * srcSiz], dstSiz);

                        break;
                    }
                    }
                }
                default: break;
                }
            }
        }
    }
    return;
}

_AFX void AfxMergeVertices(afxNat vtxCnt, afxVertexLayout const *layout, void *dst, afxVertexLayout const *srcLayout, void const *src)
{
    AfxAccumulateVertexComponents(0, layout->compCnt, vtxCnt, layout, dst, srcLayout, src);
}

_AFX void AfxCopyVertices(afxNat vtxCnt, afxVertexLayout const *layout, void *dst, afxVertexLayout const *srcLayout, void const *src)
{
    AfxCopyVertexComponents(0, layout->compCnt, vtxCnt, layout, dst, srcLayout, src);
}

#endif