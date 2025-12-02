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

#ifndef AVX_DEVICE_DDK_H
#define AVX_DEVICE_DDK_H

#include "avxBridgeDDK.h"

AFX_DEFINE_STRUCT(_avxDdevReg)
{
    afxDeviceInfo       dev;
    afxAcceleration     acceleration;
    avxAptitude         capabilities;
    afxDrawFeatures     features;
    afxDrawLimits       limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxBool             nonRhCoords;
    avxClipSpaceDepth   clipSpaceDepth;
};

AFX_DECLARE_STRUCT(_avxIddDdev);
AFX_DEFINE_STRUCT(_avxDdiDdev)
{

};

#ifdef _AVX_DRAW_DEVICE_C
#ifdef _AVX_DRAW_DEVICE_IMPL
#ifndef _AFX_DEVICE_C
#   error "Require afxDevice implementation"
#endif
AFX_OBJECT(_avxDrawDevice)
#else
AFX_OBJECT(afxDrawDevice)
#endif
{
    AFX_OBJ(afxDevice)  dev;
    _avxDdiDdev const*  ddi;
    _avxIddDdev*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxAcceleration     acceleration;
    avxAptitude         capabilities;
    afxDrawFeatures     features;
    afxDrawLimits       limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxString128        desc; // before port extintion
    afxString8          urn; // before port extintion

    afxBool             nonRhCoords;
    avxClipSpaceDepth   clipSpaceDepth;

    avxFormatDescription const*pfds;

    afxChain            ineps;
};
#endif//_AVX_DRAW_DEVICE_C

AVX afxClassConfig const _AVX_CLASS_CONFIG_DDEV;

AVX void*           _AvxGetDrawDeviceIdd(afxDrawDevice ddev);

AVX afxDrawLimits const* _AvxDdevGetLimits(afxDrawDevice ddev);

#endif//AVX_DEVICE_DDK_H
