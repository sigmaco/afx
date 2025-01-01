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
AFX_OBJECT(afxBuffer)
#endif
{
    afxBufferFlags  flags;
    afxBufferUsage  usage;
    avxFormat       fmt; // for tensor buffer
    void*           udd;

    void*           data; // used by system bufferization
    afxBuffer       src; // handled used by subacquision of another buffer
    afxSize         start; // offset used by subacquision of another buffer
    afxUnit         cap; // capacity

    afxMask         sharingMask;

    afxByte*        bytemap; // used by mapping
    afxSize         mappedOffset;
    afxUnit         mappedRange;
    afxFlags        mappedFlags;
    afxAtom32       pendingRemap;
    afxError        (*remap)(afxBuffer, afxUnit, afxUnit, afxFlags, void**);
};
#endif//_AVX_BUFFER_C

#ifdef _AVX_RASTER_C
AFX_DEFINE_STRUCT(afxSubraster)
{
    afxUnit              baseLod;
    afxUnit              lodCnt;
    afxUnit              baseLayer;
    afxUnit              layerCnt;
    avxFormat           fmt;
    avxColorSwizzling   swizzling;
    afxRasterFlags      flags;
};
#ifdef _AVX_RASTER_IMPL
AFX_OBJECT(_avxRaster)
#else
AFX_OBJECT(afxRaster)
#endif
{
    afxRasterFlags      flags;
    afxRasterUsage      usage;
    void*               udd;
    avxFormat           fmt;
    afxWhd              extent; // extent of image
    afxUnit             lodCnt; // mip level cnt
    afxUnit             sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.

    afxUnit             subCnt;
    union
    {
        afxSubraster    sub0;
        afxSubraster*   subs;
    };
};
#endif

AVX afxClassConfig const _AVX_BUF_CLASS_CONFIG;
AVX afxClassConfig const _AVX_RAS_CLASS_CONFIG;

AVXINL void _AvxSanitizeRasterRegion(afxRaster ras, afxRasterRegion const* raw, afxRasterRegion* san);
AVXINL void _AvxSanitizeRasterIo(afxRaster ras, afxSize bufCap, afxRasterIo const* raw, afxRasterIo* san);
AVXINL void _AvxSanitizeRasterCopy(afxRaster src, afxRaster dst, afxRasterCopy const* raw, afxRasterCopy* san);

AVXINL void _AvxSanitizeBufferCopy(afxBuffer src, afxBuffer dst, afxBufferCopy const* raw, afxBufferCopy* san);
AVXINL void _AvxSanitizeBufferIo(afxBuffer src, afxBuffer dst, afxBufferIo const* raw, afxBufferIo* san);

#endif//AVX_IMPL___VIDEO_H
