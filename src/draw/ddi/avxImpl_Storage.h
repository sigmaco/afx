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

#ifndef AVX_IMPL___STORAGE_H
#define AVX_IMPL___STORAGE_H

#include "qwadro/inc/draw/afxDrawSystem.h"

typedef enum _avxMemFlag
{
    _avxMemFlag_MAPPEABLE = AFX_BITMASK(0), // visible to CPU
    _avxMemFlag_CACHED = AFX_BITMASK(1), // cached by CPU
    _avxMemFlag_ENCLAVED = AFX_BITMASK(2), // virtually separated portion
    _avxMemFlag_EMBEDDED = AFX_BITMASK(3) // on-chip memory
} _avxMemFlags;

AFX_DEFINE_STRUCT(_avxRasStorage)
{
    afxLink iommu;
    // binding
    afxUnit mmu; // memory holder
    afxSize offset; // offset into memory holder bound to this storage block.
    // Persistent mapping required at acquisition. Do not allow unmapping.
    afxBool     permanentlyMapped;
    afxAtom32   pendingRemap;
    afxSize     mapOffset;
    afxUnit     mapRange;
    afxFlags    mapFlags;
    afxByte*    mapPtr; // used by mapping
    union
    {
        afxSize     addr;
        afxByte*    bytemap;
        afxUnit8*   u8;
        afxUnit16*  u16;
        afxUnit32*  u32;
        afxInt8*    i8;
        afxInt16*   i16;
        afxInt32*   i32;
        afxReal32*  f32;
        afxReal64*  f64;
        afxV2d*     f32v2;
        afxV3d*     f32v3;
        afxV4d*     f32v4;
    } hostedAlloc;
};

AFX_DEFINE_STRUCT(_avxBufStorage)
{
    afxLink iommu;
    // binding
    afxUnit mmu; // memory holder
    afxSize offset; // offset into memory holder bound to this storage block.
    // Persistent mapping required at acquisition. Do not allow unmapping.
    afxBool     permanentlyMapped; // TODO: Eliminate it? Now we have ref-counted mapping.
    afxAtom32   pendingRemap;
    afxSize     mapOffset;
    afxUnit     mapRange;
    afxFlags    mapFlags;
    afxUnit     mapRefCnt;
    afxByte*    mapPtr; // used by mapping
    union
    {
        afxSize     addr;
        afxByte*    bytemap;
        afxUnit8*   u8;
        afxUnit16*  u16;
        afxUnit32*  u32;
        afxInt8*    i8;
        afxInt16*   i16;
        afxInt32*   i32;
        afxReal32*  f32;
        afxReal64*  f64;
        afxV2d*     f32v2;
        afxV3d*     f32v3;
        afxV4d*     f32v4;
    } hostedAlloc;
};

#ifdef _AVX_BUFFER_C
#ifdef _AVX_BUFFER_IMPL
AFX_OBJECT(_avxBuffer)
#else
AFX_OBJECT(avxBuffer)
#endif
{
    afxString       tag;
    void*           udd;
    avxBuffer       base; // handle used by subacquision of another buffer
    afxSize         from; // offset used by subacquision of another buffer
    //afxUnit         size; // capacity
    avxBufferUsage  usage;
    avxBufferFlags  flags;
    avxFormat       fmt; // for tensor buffer

    afxMask         sharingMask;

    // STORAGE
    // required size of this storage block.
    afxSize         reqSiz;
    afxUnit         reqAlign;
    // required memory conditions for this storage block.
    afxFlags        reqMemType;
    _avxBufStorage  storage[1]; // non-sparse
};
#endif//_AVX_BUFFER_C

#ifdef _AVX_RASTER_C
#ifdef _AVX_RASTER_IMPL
AFX_OBJECT(_avxRaster)
#else
AFX_OBJECT(avxRaster)
#endif
{
    afxString       tag;
    void*           udd;
    avxRaster       base; // sub
    avxRasterFlags  flags;
    avxRasterUsage  usage;
    afxUnit         baseLod; // sub
    afxUnit         lodCnt; // mip level cnt
    afxUnit         baseLayer; // sub
    avxRange        whd; // extent of image
    avxFormat       fmt;
    afxUnit         sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.
    avxSwizzling    swizzling; // sub
    
    // STORAGE
    // required size of this storage block.
    afxSize         reqSiz;
    afxUnit         reqAlign;
    // required memory conditions for this storage block.
    afxFlags        reqMemType;
    _avxRasStorage  storage[1]; // non-sparse

    // GAMBIARRA
    afxBool _dout; // DOUT buffer
    afxBool _doutBlitSrc; // it will never be scanout directly to display but will be copied to a dma-buf that can be scanout.
};
#endif

AVX afxClassConfig const _AVX_BUF_CLASS_CONFIG;
AVX afxClassConfig const _AVX_RAS_CLASS_CONFIG;

AVXINL void _AvxSanitizeRasterIo(avxRaster ras, afxSize bufCap, afxUnit cnt, avxRasterIo const raw[], avxRasterIo san[]);
AVXINL void _AvxSanitizeRasterCopy(avxRaster ras, avxRaster src, afxUnit cnt, avxRasterCopy const raw[], avxRasterCopy san[]);
AVXINL void _AvxSanitizeRasterRegion(avxRaster ras, afxUnit cnt, avxRasterRegion const raw[], avxRasterRegion san[]);

AVXINL void _AvxSanitizeBufferCopy(avxBuffer buf, avxBuffer src, afxUnit cnt, avxBufferCopy const raw[], avxBufferCopy san[]);
AVXINL void _AvxSanitizeBufferIo(avxBuffer buf, avxBuffer src, afxUnit cnt, avxBufferIo const raw[], avxBufferIo san[]);

AVXINL void _AvxAllocateRasters(afxUnit cnt, avxRaster rasters[]);
AVXINL void _AvxAllocateBuffers(afxDrawSystem dsys, afxUnit cnt, avxBufferInfo const infos[], avxBuffer buffers[]);

#endif//AVX_IMPL___STORAGE_H
