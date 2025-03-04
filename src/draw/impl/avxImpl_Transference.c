/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_RASTER_C
#define _AVX_BUFFER_C
#include "avxImplementation.h"

AFX_DEFINE_STRUCT(avxMemoryReq)
{
    afxSize     size;
    afxSize     align;
    uint32_t    memType;
};

AFX_DEFINE_STRUCT(avxBufferStorage)
{
    afxSize     size;
    afxSize     align;
    uint32_t    memType;
};

AFX_DEFINE_STRUCT(avxRasterStorage)
{
    afxSize     size;
    afxSize     align;
    uint32_t    memType;
};

_AVX void _AvxGetRasterMemoryRequirements(afxDrawSystem dsys, afxRaster ras, avxMemoryReq* req)
{
    avxMemoryReq req2;
    req2.align = ras->storage[0].alignment;
    req2.size = ras->storage[0].size;
    req2.memType = ras->storage[0].memType;
    *req = req2;
}

_AVX void _AvxGetBufferMemoryRequirements(afxDrawSystem dsys, avxBuffer buf, avxMemoryReq* req)
{
    avxMemoryReq req2 = { 0 };
    req2.align = buf->storage[0].alignment;
    req2.size = buf->storage[0].size;
    req2.memType = buf->storage[0].memType;
}

_AVX void _GetImageSubresourceLayout(afxDrawSystem dsys, afxRaster ras, afxUnit lodIdx, afxUnit layerIdx, afxRasterLayout* layout)
{
    avxFormat fmt = ras->fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);

    afxRasterLayout lay2 = { 0 };
    afxWhd whd = { ras->extent.w, ras->extent.h, ras->extent.d };
    afxBool is3d = !!AfxTestRasterFlags(ras, afxRasterFlag_3D);

    if (AfxTestRasterFlags(ras, afxRasterFlag_RESAMPLED))
    {
        while (lodIdx--)
        {
            afxUnit rowStride = AFX_ALIGNED_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
            afxUnit imgStride = AFX_ALIGNED_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
            lay2.offset += whd.d * imgStride;

            whd.w = AfxMax(1, whd.w * 2);
            whd.h = AfxMax(1, whd.h * 2);
            if (is3d) whd.d = AfxMax(1, whd.d * 2);
        }

        afxUnit rowStride = AFX_ALIGNED_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
        afxUnit imgStride = AFX_ALIGNED_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
        
        lay2.rowStride = rowStride;
        lay2.imgStride = imgStride;
    }
    else
    {
        while (lodIdx--)
        {
            afxUnit rowStride = AFX_ALIGNED_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
            afxUnit imgStride = AFX_ALIGNED_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
            lay2.offset += whd.d * imgStride;

            whd.w = AfxMax(1, whd.w >> 1);
            whd.h = AfxMax(1, whd.h >> 1);
            if (is3d) whd.d = AfxMax(1, whd.d >> 1);
        }

        afxUnit rowStride = AFX_ALIGNED_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
        afxUnit imgStride = AFX_ALIGNED_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
        
        lay2.rowStride = rowStride;
        lay2.imgStride = imgStride;
    }
}

_AVX afxError _AvxCommitBuffers(afxDrawSystem dsys, afxUnit exuIdx, afxUnit cnt, avxBuffer buffers[], avxFence wait, avxFence signal)
{
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i];

        if (!buf->storage[0].hostedData.addr)
        {
            AfxAllocate(buf->storage[0].size, buf->storage[0].alignment, AfxHere(), (void**)&buf->storage[0].hostedData.addr);
        }
    }
}

_AVX afxError _AvxCommitRasters(afxDrawSystem dsys, afxUnit exuIdx, afxUnit cnt, afxRaster rasters[], avxFence wait, avxFence signal)
{
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxRaster ras = rasters[i];

        if (!ras->storage[0].hostedData.addr)
        {
            AfxAllocate(ras->storage[0].size, ras->storage[0].alignment, AfxHere(), (void**)&ras->storage[0].hostedData.addr);
        }
    }
}

_AVX afxCmdId AvxCmdPageInRasterSIGMA(afxDrawContext dctx, afxRaster ras, afxUnit rgnCnt, afxRasterRegion const rgns[]);
_AVX afxCmdId AvxCmdPageOutRasterSIGMA(afxDrawContext dctx, afxRaster ras, afxUnit rgnCnt, afxRasterRegion const rgns[]);
