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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_DEVICE_DDK_H
#define AFX_DEVICE_DDK_H

#include "qwadro/exec/afxSystem.h"
#include "../exec/afxThreadingDDK.h"

#ifdef _AFX_CONTEXT_C
AFX_OBJECT(afxDevLink)
{
    afxChain            classes;
    afxClass            semaphores;
};
#endif//_AFX_CONTEXT_C

#ifdef _AFX_DEVICE_C
AFX_OBJECT(afxDevice)
{
    afxUri32            urn; // "//./dev/"
    afxDeviceType       type;
    afxDeviceStatus     status;
    afxAcceleration     acceleration;
    afxBool             serving;

    afxUnit32           hwVndId;
    afxUnit32           hwVndDevId;
    afxUnit32           swVndId;
    afxUnit32           swVndDrvId;
    afxUnit32           driverVer;
    afxUnit32           apiVer;

    afxLink             icd;
    afxString128        devName;

    afxChain            classes;

    afxUnit             manifestPagNo;

    afxError            (*procCb)(afxDevice,afxThread,void*);
    afxResult           (*ioctlCb)(afxDevice,afxUnit,va_list);
};
#endif//_AFX_DEVICE_C

AFX afxClassConfig const _AFX_DEV_CLASS_CONFIG;

#endif//AFX_DEVICE_DDK_H
