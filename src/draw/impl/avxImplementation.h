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

#include "../../dev/afxExecImplKit.h"
#include "../../dev/afxIoImplKit.h"
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
        afxDrawPortCaps caps;
    }*ports;
    afxUnit             portCnt;

    afxBool             leftHandedSpace;
    avxClipSpaceDepth   clipSpaceDepth;

    afxChain            ineps;
    struct _afxDdevIdd* idd;
};
#endif//_AVX_DRAW_DEVICE_C

AVX afxClassConfig const _AVX_DDEV_CLASS_CONFIG;

AVX afxClass const* _AvxGetDrawDeviceClass(afxModule icd);
AVX afxClass const* _AvxGetDrawSystemClass(afxModule icd);

AVX afxError _AvxRegisterDrawDevices(afxModule icd, afxUnit cnt, afxDrawDeviceInfo const infos[], afxDrawDevice devices[]);
AVX afxError _AvxImplementDrawSystem(afxModule icd, afxClassConfig const* vduCls, afxClassConfig const* ddevCls, afxClassConfig const* dsysCls);

AVX afxBool AvxGetIcd(afxUnit icdIdx, afxModule* driver);

#endif//AVX_IMPLEMENTATION_H
