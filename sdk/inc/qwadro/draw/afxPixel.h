/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_VERTEX1_H
#define AFX_VERTEX1_H

#include "qwadro/draw/afxDrawDefs.h"

typedef enum afxPixelElementType
{
    afxPixelElementType_UNKNOWN,
    afxPixelElementType_UNUSED,
    afxPixelElementType_UNORM,
    afxPixelElementType_SNORM,
    afxPixelElementType_UINT,
    afxPixelElementType_SINT,
    afxPixelElementType_FLOAT,
    afxPixelElementType_SSCALED,
    afxPixelElementType_USCALED,
    afxPixelElementType_SFIXED,
} afxPixelElementType;

AFX_DEFINE_STRUCT(afxPfd)
/// Pixel format descriptor
{
    afxNat32            bpp; /// bits per pixel
    afxNat32            bypp; /// bytes per pixel
    afxNat32            compCnt; /// number of components
    afxPixelElementType type[4];
    afxNat32            defaults[4]; /// Default values for missing components.
    afxNat32            swizzle[4]; /// swizzle per component
    afxNat32            bpc[4]; /// bits per component
    afxBool             isNormalized[4];
    afxReal             tof[4]; /// To float scale factor
    afxNat32            bcWh[2];
    afxBool             isSrgb;
    afxBool             isBc;
    afxBool             isSubsampled;
    afxBool             isLuminance;    
};

#endif//AFX_VERTEX1_H
