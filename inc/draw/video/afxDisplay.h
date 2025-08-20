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
//#include "qwadro/inc/draw/video/afxSurface.h"

#define AVX_MAX_GAMMA_CURVE_LENGTH (1025)

typedef enum avxVideoScaling
// mask specifying presentation scaling methods.
{
    // Do not scale.
    // Pixels are mapped to one and only one pixel in the surface.
    avxVideoScaling_ONE_TO_ONE = AFX_BITMASK(0),

    // Minify or magnify such that at least one of the resulting width or height is equal to the corresponding surface dimension, 
    // and the other resulting dimension is less than or equal to the corresponding surface dimension, 
    // with the aspect ratio of the resulting image being identical to that of the original swapchain image.
    avxVideoScaling_ASPECT_RATIO = AFX_BITMASK(1),

    // minify or magnify such that the resulting dimensions are equal to those of the surface.
    avxVideoScaling_STRETCH = AFX_BITMASK(2)
} avxVideoScaling;

typedef enum avxVideoTransform
{
    // When not specified, it is treated as IDENTITY, what is expected to has not rotation nor mirroring (0 degrees).
    // If no option is chosen, the Qwadro treats it as identity.
    avxVideoTransform_IDENTITY,

    // 90 degrees rotated
    avxVideoTransform_90 = AFX_BITMASK(0),

    // 180 degrees rotated
    avxVideoTransform_180 = AFX_BITMASK(1),

    // 270 degrees rotated
    avxVideoTransform_270 = AFX_BITMASK(2),

    // Invert pixel grid horizontally.
    avxVideoTransform_FLIP_H = AFX_BITMASK(3),

    // Invert pixel grid vertically.
    avxVideoTransform_FLIP_V = AFX_BITMASK(4),

    // 90 degress rotated + V mirror
    avxVideoTransform_90FV = (avxVideoTransform_90 | avxVideoTransform_FLIP_V),

    // 90 degrees rotated + H mirror
    avxVideoTransform_90FH = (avxVideoTransform_90 | avxVideoTransform_FLIP_H),
} avxVideoTransform;

typedef enum avxVideoAlpha
{
    // Ignore alpha component in the compositing process. 
    // The raster is treated as if it has a constant alpha of 1.0.
    avxVideoAlpha_OPAQUE = AFX_BITMASK(0),

    // The compositor will respect the alpha component.
    avxVideoAlpha_PREMUL = AFX_BITMASK(1),

    // The compositor will multiply the color components of the raster by the alpha component during compositing.
    avxVideoAlpha_POSTMUL = AFX_BITMASK(2)

    // If no option is chosen, the way in which the presentation engine treats the alpha component in the images is unknown to the SIGMA GL/2 API. 
    // Instead, the application is responsible for setting the composite alpha blending mode using native window system commands. 
    // If the application does not set the blending mode using native window system commands, then a platform-specific default will be used.
} avxVideoAlpha;

/*
    The gamma curve here is a piecewise linear function; not a spline. 
    It's essentially a lookup table (LUT) or function approximation over the [0,1] domain using a set of explicitly defined control points.
    
    The array avxGammaControlCapabilites.ctrlPntPos are the X-values (inputs in [0,1] range).
    Often uniformly spaced, but the driver can specify non-uniform spacing.
    Think of this as: x0,x1,x2,...,xn-1 e [0, 1].

    The array avxGammaControl.curve are the corresponding Y-values (R, G, B output intensities).
    So together, they define: (x0,y0),(x1,y1),...,(xn-1,yn-1).
    Each y_i is a afxV3d, meaning we have three gamma curves: one each for red, green, and blue.

    This is not a B-spline curve because the control points are explicit mappings, i.e., each (x, y) is a sample of the gamma function.
    Also there's no knot vector; the curve is not interpolated using basis functions. Also there's no degree (like cubic or quadratic).
    It’s meant to be rendering-efficient, hence uses simple linear interpolation between points.
*/

AFX_DEFINE_STRUCT(avxGammaControlCapabilites)
// The capabilities of the hardware for gamma correction, particularly how gamma control points can be used.
{
    // When TRUE, the hardware supports scaling and offsetting the gamma curve.
    // These are applied before gamma correction
    afxBool scaleAndOffsetSupported;
    // The maximum and minimum luminance values that the display can output after gamma conversion.
    // Useful for tone mapping and HDR scenarios.
    afxReal minConvertedValue;
    afxReal maxConvertedValue;
    // Number of gamma control points supported.
    // Can be up to 1025. This determines the resolution of the gamma curve.
    afxUnit ctrlPntCnt;
    // These are the normalized (0.0 to 1.0) input values for the gamma curve.
    // Corresponds to curve[] values in avxGammaControl.
    afxReal ctrlPntPos[AVX_MAX_GAMMA_CURVE_LENGTH];
};

AFX_DEFINE_STRUCT(avxGammaControl)
// Structure specifying/describing the gamma curve settings to apply to the display.
{
    // scale and offset are used if avxGammaControlCapabilites.scaleAndOffsetSupported is TRUE.
    // Each channel (Red, Green, Blue) has its own scaling and offset factor.
    // Formula for each output channel: output = scale * input + offset.
    afxV3d scale;
    afxV3d offset;
    // Defines the gamma curve for red, green, and blue channels.
    // The values correspond to the avxGammaControlCapabilites.ctrlPntPos.
    // You provide a afxV3d value (with R, G, B floats) for each control point position.
    afxV3d curve[AVX_MAX_GAMMA_CURVE_LENGTH];
};

AFX_DEFINE_STRUCT(avxDisplayPortInfo)
{
    afxDisplay          vdu;
    afxChar             name[32]; // the name of the display.
    afxChar             label[128]; // the friendly name of the display.
    afxUnit             dimWh[2]; // the physical width and height of the visible portion of the display, in millimeters.
    afxUnit             resWh[2]; // the physical, native, or preferred resolution of the display.
    avxVideoTransform   supportedXforms; // transforms are supported by this display.
    afxBool             planeReorder; // can re-arrange the planes on this display in any order relative to each other?
    afxBool             persistentContent; // can submit persistent present operations on swapchains created against this display?
    afxRect             workArea;
    afxRect             fullArea;
};

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
    //afxUnit         bpp;

    avxFormat       fmt;
    avxVideoScaling scaling;
    avxScanlining   scanlineOrdering;
    afxBool         stereo;
};

// Win32: only supported while in full-screen mode.
AVX afxError AvxQueryGammaControlCapabilites(afxDisplay dsp, avxGammaControlCapabilites* caps);

// Win32: only supported while in full-screen mode.
AVX afxError AvxDescribeGammaControl(afxDisplay dsp, avxGammaControl* desc);

// Win32: only supported while in full-screen mode.
AVX afxError AvxControlGamma(afxDisplay dsp, avxGammaControl const* ctrl);

////////////////////////////////////////////////////////////////////////////////

AVX afxUnit AfxEnumerateDisplays(afxModule icd, afxUnit first, afxUnit cnt, afxDisplay displays[]);

// dwmgl
// dwmgdi
// dwmd3d9
// dwmd3d11
// dwmd3d12

#endif//AVX_DISPLAY_H
