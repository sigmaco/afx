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

#ifndef AVX_IMPLEMENTATION_H
#define AVX_IMPLEMENTATION_H

#include "../../impl/afxExecImplKit.h"
#include "../../impl/afxIoImplKit.h"

#ifdef _AFX_DEBUG
// hardcoded validation controls
#define AVX_VALIDATION_ENABLED TRUE
#define AVX_LIMIT_VALIDATION_ENABLED TRUE
#define AVX_FEATURE_VALIDATION_ENABLED TRUE
#endif

#include "qwadro/inc/draw/afxDrawSystem.h"
#include "avxImpl_System.h"
#include "avxImpl_Context.h"
#include "avxImpl_Input.h"
#include "avxImpl_Executor.h"
#include "avxImpl_Pipeline.h"
#include "avxImpl_Surface.h"
#include "avxImpl_Storage.h"

AFX_DEFINE_STRUCT(_avxDrawSystemImplementation)
{
    afxClassConfig viddCls;
    afxClassConfig ddevCls;
    afxClassConfig dsysCls;
};

AFX_DEFINE_STRUCT(_avxDrawDeviceRegistration)
{
    afxDeviceInfo       dev;
    afxAcceleration     acceleration;
    afxDrawCaps         capabilities;
    afxDrawFeatures     features;
    afxDrawLimits       limits;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxBool             nonRhCoords;
    avxClipSpaceDepth   clipSpaceDepth;
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
    afxAcceleration     acceleration;
    afxDrawCaps         capabilities;
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
    struct _afxDdevIdd* idd;
};
#endif//_AVX_DRAW_DEVICE_C

#ifdef _AVX_DISPLAY_C
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

    afxChar             name[32]; // the name of the display.
    afxChar             label[128]; // the friendly name of the display.
    afxUnit             dimWh[2]; // the physical width and height of the visible portion of the display, in millimeters.
    afxUnit             resWh[2]; // the physical, native, or preferred resolution of the display.
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

AVX afxClassConfig const _AVX_DDEV_CLASS_CONFIG;
AVX afxClassConfig const _AVX_VDU_CLASS_CONFIG;

AVX void*           _AvxGetDrawDeviceIdd(afxDrawDevice ddev);

AVX afxClass const* _AvxIcdGetDdevClass(afxModule icd);
AVX afxClass const* _AvxIcdGetDsysClass(afxModule icd);
AVX afxClass const* _AvxGetDisplayClass(afxModule icd);

AVX afxError _AvxRegisterDisplays(afxModule icd, afxUnit cnt, avxDisplayInfo const infos[], afxDisplay displays[]);
AVX afxError _AvxRegisterDrawDevices(afxModule icd, afxUnit cnt, _avxDrawDeviceRegistration const infos[], afxDrawDevice devices[]);
AVX afxError _AvxImplementDrawSystem(afxModule icd, _avxDrawSystemImplementation const* cfg);

AVX afxBool _AvxGetIcd(afxUnit icdIdx, afxModule* driver);

AVX afxDrawLimits const* _AvxDdevGetLimits(afxDrawDevice ddev);
AVX avxFormatDescription const _AvxStdPfds[avxFormat_TOTAL];

#endif//AVX_IMPLEMENTATION_H
