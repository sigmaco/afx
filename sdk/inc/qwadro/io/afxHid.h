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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_HID_H
#define AFX_HID_H

#include "qwadro/core/afxDevice.h"

typedef enum afxHidFlag
{
    AFX_HID_FLAG_DUMMY
} afxHidFlag;

#ifdef _AFX_CORE_C
#ifdef _AFX_HID_C
AFX_OBJECT(afxHid)
{
    AFX_OBJECT(afxDevice)   dev;
    afxNat                  port;
    afxHidFlag              flags;
};
#endif//_AFX_HID_C
#endif//_AFX_CORE_C

AFX afxNat      AfxGetHidPort(afxHid hid);
AFX afxResult   AfxTestHidFlags(afxHid hid, afxHidFlag flags);

#endif//AFX_HID_H
