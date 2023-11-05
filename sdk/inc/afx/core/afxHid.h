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

#include "afx/core/afxInstance.h"

typedef enum afxHidFlag
{
    AFX_HID_FLAG_DUMMY
} afxHidFlag;

AFX_DEFINE_STRUCT(afxHid)
{
    _AFX_DBG_FCC
    afxHandle   hidObj;
    afxNat      port;
    afxHidFlag  flags;
};

AFX afxNat      AfxGetHidPort(afxHid* hid);
AFX afxResult   AfxTestHidFlags(afxHid* hid, afxHidFlag flags);

#endif//AFX_HID_H