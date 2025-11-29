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

#ifndef AVX_CANVAS_DDK_H
#define AVX_CANVAS_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxIddCanv);

AFX_DEFINE_STRUCT(_avxDdiCanv)
{
    afxError(*relink)(avxCanvas, afxBool, afxUnit, afxUnit, avxRaster[]);
    afxError(*readjust)(avxCanvas, afxWarp const);
};

#ifdef _AVX_CANVAS_C
AFX_DEFINE_STRUCT(_avxCanvasBin)
{
    avxRaster       buf; // the texture subresource that will be output to for this color attachment.
    avxRaster       resolve; // the texture subresource that will receive the resolved output for this color attachment if view is multisampled.
    afxBool         managed;
    avxFormat       fmt; // the format of the image that will be used for the attachment.
    avxRasterUsage  usage; // additional properties of the attachment.
    avxRasterFlags  flags;
    afxUnit         lodCnt; // the number of samples of the image.
};
#ifdef _AVX_CANVAS_IMPL
AFX_OBJECT(_avxCanvas)
#else
AFX_OBJECT(avxCanvas)
#endif
{
    _avxDdiCanv const*  ddi;
    _avxIddCanv*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    avxCanvasFlags      flags;
    avxRange            whdMin; // when a user-provided raster are attached
    avxRange            whdMax; // when a user-provided raster are attached
    avxRange            whd; // D is layered
    afxUnit             lodCnt; // multisampling level (2^lodCnt)
    afxUnit             linkedCnt;
    afxUnit             binCnt; // immutable
    _avxCanvasBin*      bins;
    afxFlags            ownershipMask; // one for each surface. Forcing it to be limited to max 32 surfaces.
    afxUnit             colorCnt;
    afxUnit             dsBinIdx[2];
};
#endif//_AVX_CANVAS_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_CANV;

#endif//AVX_CANVAS_DDK_H
