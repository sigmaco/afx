/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   U S E R   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced User Experiences Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced User Experience                                                 //
//////////////////////////////////////////////////////////////////////////////

#ifndef AUX_DISPLAY_DDK_H
#define AUX_DISPLAY_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/ux/afxUxDefs.h"
#include "qwadro/draw/avxViewport.h"
#include "qwadro/ux/afxShell.h"
//#include "qwadro/../../dep_/vgl1/vgl1.h"

AFX_DECLARE_STRUCT(_avxIddDisp);
AFX_DECLARE_STRUCT(_avxDdiDisp);

#ifdef _AVX_DISPLAY_C

AFX_DEFINE_STRUCT(_avxDisplayPort)
{
    afxBool         prime;
    afxRect         workArea;
    afxRect         fullArea;
    afxChar         name[32]; // the name of the display.
    afxChar         label[128]; // the friendly name of the display.
};

#ifdef _AVX_DISPLAY_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_avxDisplay)
#else
AFX_OBJECT(afxDisplay)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    _avxIddDisp*        idd;
    _avxDdiDisp const*  ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxUnit             portCnt;
    _avxDisplayPort     ports[2];

    afxChar             name[32]; // the name of the display.
    afxChar             label[128]; // the friendly name of the display.
    afxUnit             dimWh[2]; // the physical width and height of the visible portion of the display, in millimeters.
    afxUnit             resWh[2]; // the physical, native, or preferred resolution of the display.
    afxUnit             dpi[2];
    avxVideoTransform supportedXforms; // transforms are supported by this display.
    afxBool             planeReorder; // can re-arrange the planes on this display in any order relative to each other?
    afxBool             persistentContent; // can submit persistent present operations on swapchains created against this display?
    afxUnit             bpp;
    afxUnit             freq;
    afxUnit             planeCnt;
    afxUnit             paletteSiz;
    afxBool             clipCapable;
};
#endif//_AVX_DISPLAY_C


AVX afxClassConfig const _AVX_VDU_CLASS_CONFIG;

#endif//AUX_DISPLAY_DDK_H
