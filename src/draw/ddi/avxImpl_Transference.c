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
//#define _AFX_CONTEXT_C
//#define _AVX_DRAW_SYSTEM_C
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
    req2.align = ras->reqAlign;
    req2.size = ras->reqSiz;
    req2.memType = ras->reqMemType;
    *req = req2;
}

_AVX void _AvxGetBufferMemoryRequirements(afxDrawSystem dsys, avxBuffer buf, avxMemoryReq* req)
{
    avxMemoryReq req2 = { 0 };
    req2.align = buf->reqAlign;
    req2.size = buf->reqSiz;
    req2.memType = buf->reqMemType;
}

_AVX void _GetImageSubresourceLayout(afxDrawSystem dsys, avxRaster ras, afxUnit lodIdx, afxUnit layerIdx, avxRasterLayout* layout)
{
    avxFormat fmt = ras->fmt;
    avxFormatDescription pfd;
    AvxDescribeFormats(1, &fmt, &pfd);

    avxRasterLayout lay2 = { 0 };
    afxWarp whd = { ras->whd.w, ras->whd.h, ras->whd.d };
    afxBool is3d = !!AvxTestRasterFlags(ras, avxRasterFlag_3D);

    if (AvxTestRasterFlags(ras, avxRasterFlag_MULTISAMP))
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
            AfxAllocate(buf->reqSiz, AVX_BUFFER_ALIGNMENT, AfxHere(), (void**)&buf->storage[0].hostedAlloc.addr);
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
            AfxAllocate(ras->reqSiz, ras->reqAlign, AfxHere(), (void**)&ras->storage[0].hostedAlloc.addr);
        }
    }
}

_AVX afxCmdId AvxCmdPageInRasterSIGMA(afxDrawContext dctx, avxRaster ras, afxUnit rgnCnt, avxRasterRegion const rgns[]);
_AVX afxCmdId AvxCmdPageOutRasterSIGMA(afxDrawContext dctx, avxRaster ras, afxUnit rgnCnt, avxRasterRegion const rgns[]);

_AVX afxCmdId _AvxCmdRegenerateMipmapsSIGMA(afxDrawContext dctx, afxFlags flags, afxUnit rasCnt, avxRaster rasters[])
{
    for (afxUnit rasIt = 0; rasIt < rasCnt; rasIt++)
    {
        avxRaster ras = rasters[rasIt];
        afxUnit mipLevels = ras->lodCnt;
        afxUnit mipWidth = ras->whd.w;
        afxUnit mipHeight = ras->whd.h;

        for (afxUnit i = 1; i < mipLevels; i++)
        {
            AvxCmdPipelineBarrier(dctx, avxPipelineStage_TRANSFER, NIL);

            // Set up blit
            avxRasterBlit blit =
            {
                .src.lodIdx = i - 1,
                .src.whd = { mipWidth, mipHeight, 1 },
                .dst.lodIdx = i,
                .dst.whd = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 }
            };
            AvxCmdBlitRaster(dctx, ras, 1, &blit, ras, avxTexelFilter_LINEAR);

            // Update dimensions for next level
            mipWidth = mipWidth > 1 ? mipWidth / 2 : 1;
            mipHeight = mipHeight > 1 ? mipHeight / 2 : 1;
        }
    }
}

AFX_DEFINE_STRUCT(avxRasterAllocation)
{
    avxRaster   ras;
    afxUnit     iommu;
    afxSize     offset;
    afxFlags    flags;
    afxError    rslt;
};

AFX_DEFINE_STRUCT(avxBufferAllocation)
{
    avxBuffer   buf;
    afxUnit     iommu;
    afxSize     offset;
    afxFlags    flags;
    afxError    rslt;
};

_AVXINL void _AvxAllocateRasters(afxUnit cnt, avxRaster rasters[])
{
    avxRaster ras = rasters[0];
    afxDrawSystem dsys = AfxGetProvider(ras);
    avxMemoryReq req = { 0 };
    _AvxGetRasterMemoryRequirements(dsys, ras, &req);

    //AfxPushLink(&ras->storage[0].iommu, );
}

_AVXINL void _AvxAllocateBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[])
{
    afxError err;

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxBufferInfo const* info = &infos[i];
        avxBuffer buf = buffers[i];
        _avxBufStorage* bufs = &buf->storage[0];

        if (buf->flags & avxBufferFlag_F)
        {
            //bufs->offset = info->from;
            AFX_ASSERT((!info->dataSiz) || (info->dataSiz && buf->reqSiz));
            bufs->hostedAlloc.bytemap = info->data;
        }
        else
        {
            if (AfxAllocate(buf->reqSiz, buf->reqAlign, AfxHere(), (void**)&bufs->hostedAlloc.bytemap))
            {
                AfxThrowError();
            }
        }
    }
}

_AVXINL void AvxCommitRasterStorage()
{

}

AFX_DEFINE_STRUCT(avxBufferPaging)
{
    avxBuffer       buf;
    afxSize         base;
    afxSize         range;
    //afxUnit         iommu;
    //afxSize         offset;
    afxFlags        flags;
    afxError        rslt;
};

AFX_DEFINE_STRUCT(avxRasterPaging)
{
    avxRaster       ras;
    avxRasterRegion rgn;
    //afxUnit         iommu;
    //afxSize         offset;
    afxFlags        flags;
    afxError        rslt;
};

/*
    void NamedBufferPageCommitmentEXT(uint buffer,
                                          intptr offset,
                                          sizeiptr size,
                                          boolean commit);
    void TexturePageCommitmentEXT(uint texture,
                                      int level,
                                      int xoffset,
                                      int yoffset,
                                      int zoffset,
                                      sizei width,
                                      sizei height,
                                      sizei depth,
                                      boolean commit);
*/
