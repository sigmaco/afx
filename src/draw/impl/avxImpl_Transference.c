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

_AVX void _AvxGetRasterMemoryRequirements(afxDrawSystem dsys, avxRaster ras, avxMemoryReq* req)
{
    avxMemoryReq req2;
    req2.align = ras->alignment;
    req2.size = ras->size;
    req2.memType = ras->memType;
    *req = req2;
}

_AVX void _AvxGetBufferMemoryRequirements(afxDrawSystem dsys, avxBuffer buf, avxMemoryReq* req)
{
    avxMemoryReq req2 = { 0 };
    req2.align = AVX_BUFFER_ALIGNMENT;
    req2.size = buf->size;
    req2.memType = buf->memType;
}

_AVX void _GetImageSubresourceLayout(afxDrawSystem dsys, avxRaster ras, afxUnit lodIdx, afxUnit layerIdx, avxRasterLayout* layout)
{
    avxFormat fmt = ras->fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);

    avxRasterLayout lay2 = { 0 };
    afxWhd whd = { ras->extent.w, ras->extent.h, ras->extent.d };
    afxBool is3d = !!AvxTestRasterFlags(ras, avxRasterFlag_3D);

    if (AvxTestRasterFlags(ras, avxRasterFlag_UPSAMPLED))
    {
        while (lodIdx--)
        {
            afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
            afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
            lay2.offset += whd.d * imgStride;

            whd.w = AFX_MAX(1, whd.w * 2);
            whd.h = AFX_MAX(1, whd.h * 2);
            if (is3d) whd.d = AFX_MAX(1, whd.d * 2);
        }

        afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
        afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
        
        lay2.rowStride = rowStride;
        lay2.imgStride = imgStride;
    }
    else
    {
        while (lodIdx--)
        {
            afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
            afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
            lay2.offset += whd.d * imgStride;

            whd.w = AFX_MAX(1, whd.w >> 1);
            whd.h = AFX_MAX(1, whd.h >> 1);
            if (is3d) whd.d = AFX_MAX(1, whd.d >> 1);
        }

        afxUnit rowStride = AFX_ALIGN_SIZE(whd.w * pfd.stride, AFX_SIMD_ALIGNMENT);
        afxUnit imgStride = AFX_ALIGN_SIZE(whd.h * rowStride, AFX_SIMD_ALIGNMENT);
        
        lay2.rowStride = rowStride;
        lay2.imgStride = imgStride;
    }
}

_AVX afxError _AvxCommitBuffers(afxDrawSystem dsys, afxUnit exuIdx, afxUnit cnt, avxBuffer buffers[], avxFence wait, avxFence signal)
{
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBuffer buf = buffers[i];

        if (!buf->storage[0].hostedAlloc.addr)
        {
            AfxAllocate(buf->size, AVX_BUFFER_ALIGNMENT, AfxHere(), (void**)&buf->storage[0].hostedAlloc.addr);
        }
    }
}

_AVX afxError _AvxCommitRasters(afxDrawSystem dsys, afxUnit exuIdx, afxUnit cnt, avxRaster rasters[], avxFence wait, avxFence signal)
{
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxRaster ras = rasters[i];

        if (!ras->storage[0].hostedAlloc.addr)
        {
            AfxAllocate(ras->size, ras->alignment, AfxHere(), (void**)&ras->storage[0].hostedAlloc.addr);
        }
    }
}

_AVX afxCmdId AvxCmdPageInRasterSIGMA(afxDrawContext dctx, avxRaster ras, afxUnit rgnCnt, avxRasterRegion const rgns[]);
_AVX afxCmdId AvxCmdPageOutRasterSIGMA(afxDrawContext dctx, avxRaster ras, afxUnit rgnCnt, avxRasterRegion const rgns[]);
