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

// hardcoded validation controls
#define AVX_FEATURE_VALIDATION_ENABLED TRUE
#define AVX_LIMIT_VALIDATION_ENABLED TRUE
#define AVX_VALIDATION_ENABLED TRUE


#include "qwadro/inc/draw/afxDrawSystem.h"
#include "avxImpl_System.h"
#include "avxImpl_Context.h"
#include "avxImpl_Input.h"
#include "avxImpl_Executor.h"
#include "avxImpl_Pipeline.h"
#include "avxImpl_Output.h"
#include "avxImpl_Video.h"

AFX_DEFINE_STRUCT(afxDrawDeviceInfo)
{
    afxDeviceInfo       dev;
    afxDrawLimits       limits;
    afxDrawFeatures     features;
    afxUnit             portCnt;
    afxBool             leftHandedSpace;
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
    afxDrawLimits       limits;
    afxDrawFeatures     features;
    struct
    {
        afxString8      urn;
        afxString128    desc;
        afxDrawCapabilities caps;
    }*ports;
    afxUnit             portCnt;

    afxBool             leftHandedSpace;
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

AVX afxClass const* _AvxGetDrawDeviceClass(afxModule icd);
AVX afxClass const* _AvxGetDrawSystemClass(afxModule icd);
AVX afxClass const* _AvxGetDisplayClass(afxModule icd);

AVX afxError _AvxRegisterDisplays(afxModule icd, afxUnit cnt, avxDisplayInfo const infos[], afxDisplay displays[]);
AVX afxError _AvxRegisterDrawDevices(afxModule icd, afxUnit cnt, afxDrawDeviceInfo const infos[], afxDrawDevice devices[]);
AVX afxError _AvxImplementDrawSystem(afxModule icd, afxClassConfig const* viddCls, afxClassConfig const* ddevCls, afxClassConfig const* dsysCls);

AVX afxBool _AvxGetIcd(afxUnit icdIdx, afxModule* driver);

AVX afxDrawLimits const* _AvxDdevGetLimits(afxDrawDevice ddev);
AVX avxFormatDescription const _AvxStdPfds[avxFormat_TOTAL];

#endif//AVX_IMPLEMENTATION_H
