/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_DISPLAY_H
#define AVX_DISPLAY_H

#include "qwadro/inc/draw/op/avxPipeline.h"
#include "qwadro/inc/draw/io/avxRaster.h"
#include "qwadro/inc/draw/io/avxBuffer.h"
#include "qwadro/inc/draw/afxDrawOutput.h"

AFX_DEFINE_STRUCT(avxDisplayInfo)
{
    afxDeviceInfo       dev;
    afxDisplay          vdu;
    afxChar             name[32]; // the name of the display.
    afxChar             label[128]; // the friendly name of the display.
    afxUnit             dimWh[2]; // the physical width and height of the visible portion of the display, in millimeters.
    afxUnit             resWh[2]; // the physical, native, or preferred resolution of the display.
    avxVideoTransform   supportedXforms; // transforms are supported by this display.
    afxBool             planeReorder; // can re-arrange the planes on this display in any order relative to each other?
    afxBool             persistentContent; // can submit persistent present operations on swapchains created against this display?
};

AFX_DEFINE_STRUCT(avxDisplayCapabilities)
{
    avxRange           currExtent;
    avxRange           minBufExtent;
    avxRange           maxBufExtent;
    afxUnit             minBufCnt;
    afxUnit             maxBufCnt;
    avxVideoTransform   currTransform;
    avxVideoTransform   supportedTransforms;
    avxVideoAlpha       supportedCompositeAlpha;
    avxRasterUsage      supportedUsage;
};

typedef enum avxScanlining
// Flags indicating the method the raster uses to create an image on a surface.
{
    avxScanlining_IGNORE, // Scanline order is unspecified.
    avxScanlining_PROGRESSIVE, // The image is created from the first scanline to the last without skipping any.
    avxScanlining_INTERLACED_UFF, // upper field first; the even-numbered lines (0, 2, 4, 6, ...)
    avxScanlining_INTERLACED_LFF, // lower field first; the odd-numbered lines (1, 3, 5, 7, ...)
} avxScanlining;

AFX_DEFINE_STRUCT(avxDisplayMode)
// Describes a display mode.
{
    afxUnit         width;
    afxUnit         height;
    afxUnit         freq;
    afxUnit         bpp;

    avxFormat       fmt;
    avxVideoScaling scaling;
    avxScanlining   scanlineOrdering;
    afxBool         stereo;
};

////////////////////////////////////////////////////////////////////////////////

AVX afxUnit AfxEnumerateDisplays(afxDrawDevice ddev, afxUnit first, afxUnit cnt, afxDisplay displays[]);

// dwmgl
// dwmgdi
// dwmd3d9
// dwmd3d11
// dwmd3d12

#endif//AVX_DISPLAY_H
