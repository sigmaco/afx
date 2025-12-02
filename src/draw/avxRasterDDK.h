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
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_RASTER_DDK_H
#define AVX_RASTER_DDK_H

#include "avxStorageDDK.h"

AFX_DECLARE_STRUCT(_avxDdiRas);
AFX_DECLARE_STRUCT(_avxIddRas);

#ifdef _AVX_RASTER_C
#ifdef _AVX_RASTER_IMPL
AFX_OBJECT(_avxRaster)
#else
AFX_OBJECT(avxRaster)
#endif
{
    _avxDdiRas const*   ddi;
    _avxIddRas*         idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    avxRaster           base; // sub
    avxRasterFlags      flags;
    avxRasterUsage      usage;
    afxUnit             baseMip; // sub
    afxUnit             mipCnt; // mip level cnt
    afxUnit             baseLayer; // sub
    avxRange            whd; // extent of image
    afxUnit             spp; // samples per pixel --- 1, 2, 4, 8, 16, 32, or 64.
    avxFormat           fmt;
    afxUnit             fmtStride; // cached to avoid queries.
    avxSwizzling        swizzling; // sub
    
    afxBool             sub;
    afxBool             exo;

    // STORAGE
    // required size of this storage block.
    afxSize             reqSiz;
    afxUnit             reqAlign;
    // required memory conditions for this storage block.
    afxFlags            reqMemType;
    _avxRasStorage      storage[1]; // non-sparse
    afxSize             storageOffset;

    // GAMBIARRA
    afxBool             _dout; // DOUT buffer
    afxBool             _doutBlitSrc; // it will never be scanout directly to display but will be copied to a dma-buf that can be scanout.
};
#endif

AVX afxClassConfig const _AVX_CLASS_CONFIG_RAS;

AVXINL void _AvxSanitizeRasterIo(avxRaster ras, afxSize bufCap, afxUnit cnt, avxRasterIo const raw[], avxRasterIo san[]);
AVXINL void _AvxSanitizeRasterCopy(avxRaster ras, avxRaster src, afxUnit cnt, avxRasterCopy const raw[], avxRasterCopy san[]);
AVXINL void _AvxSanitizeRasterRegion(avxRaster ras, afxUnit cnt, avxRasterRegion const raw[], avxRasterRegion san[]);

#endif//AVX_RASTER_DDK_H
