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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // Advanced Visual Experience on Qwadro                                     //
//////////////////////////////////////////////////////////////////////////////
// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_DRAW_SYSTEM_H
#define AVX_DRAW_SYSTEM_H

#include "qwadro/exec/afxDevice.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/math/avxColor.h"
#include "qwadro/draw/afxPixel.h"
#include "qwadro/draw/math/avxMatrix.h"
// provided classes.
#include "qwadro/draw/dev/afxDrawBridge.h"
#include "qwadro/draw/dev/afxDrawDevice.h"
#include "qwadro/draw/avxCmdb.h"
#include "qwadro/draw/pipe/avxDrawOps.h"
#include "qwadro/draw/dev/afxDrawContext.h"
#include "qwadro/draw/dev/afxDrawOutput.h"
#include "qwadro/draw/dev/afxDrawInput.h"
#include "qwadro/draw/io/afxTarga.h"
// provided classes.
#include "qwadro/draw/avxCamera.h"

AVX afxReal64       AfxFindAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam);

/// If you don't know what the physical aspect ratio is of the device you're using 
/// (for example, if you're using a standard PC, there is no way to determine for sure what kind of monitor is attached), 
/// you can either assume square pixels (pass the width of the screen divided by the height), or you can use Qwadro's "best guess": 

AVX afxReal64       AfxFindPhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight);

AVX afxClass const* AvxGetDrawDeviceClass(void);
AVX afxClass const* AvxGetDrawOutputClass(void);
AVX afxClass const* AvxGetDrawInputClass(void);

AVX afxNat          AfxChooseDrawDevices
(
    afxDrawDeviceCaps const*    caps,
    afxDrawDeviceLimits const*  limits,
    afxNat                      maxCnt,
    afxNat                      ddevId[]
); // return count of found devices

AVX afxBool         AfxGetDrawDevice
(
    afxNat          ddevId, 
    afxDrawDevice*  device
);

AVX afxNat          AfxEvokeDrawDevices
(
    afxBool         (*f)(afxDrawDevice, void*),
    void*           udd,
    afxNat          first,
    afxNat          cnt,
    afxDrawDevice   devices[]
);

/// Enumerates all draw devices by passing the handle to each device, in turn, to an application-defined callback function. 
/// AfxEvokeDrawDevices() continues until the last device is enumerated or the callback function returns FALSE.
AVX afxNat          AfxInvokeDrawDevices
(
    afxNat          first,
    afxNat          cnt,
    afxBool         (*f)(afxDrawDevice, void*),
    void*           udd
);

AVX afxNat          AfxInvokeDrawDevices2
(
    afxNat          first,
    afxNat          cnt,
    afxBool         (*f)(afxDrawDevice, void*),
    void*           udd,
    afxBool         (*f2)(afxDrawDevice, void*),
    void*           udd2
);

/// The AfxEnumerateDrawDevices() function lets you obtain the handle to each draw device in the current session.
AVX afxNat          AfxEnumerateDrawDevices
(
    afxNat          first,
    afxNat          cnt,
    afxDrawDevice   devices[]
);

#endif//AVX_DRAW_SYSTEM_H
