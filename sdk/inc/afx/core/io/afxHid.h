/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_HID_H
#define AFX_HID_H

#include "afx/core/afxObject.h"

typedef enum afxHidFlag
{
    AFX_HID_FLAG_DUMMY
} afxHidFlag;

AFX_DEFINE_HANDLE(afxHid);

AFX_OBJECT(afxHid)
{
    afxObject   obj;
#ifdef _AFX_HID_C
    afxHidFlag  flags;
    afxNat      port;
#endif
};

AFX void*       AfxHidGetSystem(afxHid hid);

AFX afxNat      AfxHidGetPort(afxHid hid);
AFX afxResult   AfxHidTestFlags(afxHid hid, afxHidFlag flags);

#endif//AFX_HID_H