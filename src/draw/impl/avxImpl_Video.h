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

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_IMPL___VIDEO_H
#define AVX_IMPL___VIDEO_H

#include "qwadro/inc/draw/afxDrawSystem.h"

#ifdef _AVX_BUFFER_C
#ifdef _AVX_BUFFER_IMPL
AFX_OBJECT(_avxBuffer)
#else
AFX_OBJECT(avxBuffer)
#endif
{
    avxBufferFlags  flags;
    avxBufferUsage  usage;
    afxUnit         cap; // capacity
    avxFormat       fmt; // for tensor buffer
    void*           udd;



    void*           data; // used by host-side allocation
    avxBuffer       src; // handled used by subacquision of another buffer
    afxSize         start; // offset used by subacquision of another buffer

    afxMask         sharingMask;

    afxByte*        bytemap; // used by mapping
    afxSize         mappedOffset;
    afxUnit         mappedRange;
    afxFlags        mappedFlags;
    afxAtom32       pendingRemap;

    struct
    {
        // required size of this storage block.
        afxUnit size;
        // required alignment of this storage block.
        afxUnit alignment;
        // required memory conditions for this storage block.
        afxFlags memType;

        // binding
        afxUnit mmu; // memory holder
        afxSize offset; // offset into memory holder bound to this storage block.
        union
        {
            afxSize addr;
            afxByte*s8;
            afxUnit32* rgba8;
            afxReal* r32f;
            afxV2d* rg32f;
            afxV3d* rgb32f;
            afxV4d* rgba32f;
        } hostedData;
    } storage[1]; // non-sparse
};
#endif//_AVX_BUFFER_C

#ifdef _AVX_RASTER_C
#ifdef _AVX_RASTER_IMPL
AFX_OBJECT(_avxRaster)
#else
AFX_OBJECT(afxRaster)
#endif
{
    afxRaster       base; // sub
    afxRasterFlags  flags;
    afxRasterUsage  usage;
    void*           udd;
    avxFormat       fmt;
    afxUnit         baseLayer; // sub
    avxRange       extent; // extent of image
    afxUnit         baseLod; // sub
    afxUnit         lodCnt; // mip level cnt
    afxUnit         sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.
    avxSwizzling    swizzling; // sub
    
    struct
    {
        // required size of this storage block.
        afxUnit size;
        // required alignment of this storage block.
        afxUnit alignment;
        // required memory conditions for this storage block.
        afxFlags memType;

        // binding
        afxUnit mmu; // memory holder
        afxSize offset; // offset into memory holder bound to this storage block.
        union
        {
            afxSize addr;
            afxByte*s8;
            afxUnit32* rgba8;
            afxReal* r32f;
            afxV2d* rg32f;
            afxV3d* rgb32f;
            afxV4d* rgba32f;
        }hostedData;
    } storage[1]; // non-sparse

    afxChar const*const*label;

    afxBool _dout; // DOUT buffer
    afxBool _doutBlitSrc; // it will never be scanout directly to display but will be copied to a dma-buf that can be scanout.
};
#endif

AVX afxClassConfig const _AVX_BUF_CLASS_CONFIG;
AVX afxClassConfig const _AVX_RAS_CLASS_CONFIG;

AVXINL void _AvxSanitizeRasterRegion(afxRaster ras, afxRasterRegion const* raw, afxRasterRegion* san);
AVXINL void _AvxSanitizeRasterIo(afxRaster ras, afxSize bufCap, afxRasterIo const* raw, afxRasterIo* san);
AVXINL void _AvxSanitizeRasterCopy(afxRaster src, afxRaster dst, afxRasterCopy const* raw, afxRasterCopy* san);

AVXINL void _AvxSanitizeBufferCopy(avxBuffer src, avxBuffer dst, avxBufferCopy const* raw, avxBufferCopy* san);
AVXINL void _AvxSanitizeBufferIo(avxBuffer src, avxBuffer dst, avxBufferIo const* raw, avxBufferIo* san);

#endif//AVX_IMPL___VIDEO_H
