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

#ifndef AFX_IO_SCRIPT_H
#define AFX_IO_SCRIPT_H

#include "afxStream.h"

AFX_DEFINE_STRUCT(afxVar)
{
    enum
    {
        AFX_VAR_SIZE_8,
        AFX_VAR_SIZE_16,
        AFX_VAR_SIZE_32,
        AFX_VAR_SIZE_64,
        AFX_VAR_SIZE_128,
    };
    enum
    {
        AFX_VAR_TYPE_IMM,
        AFX_VAR_TYPE_REF
    };
};

AFX_DEFINE_HANDLE(afxIoScript);

AFX_OBJECT(afxIoScript)
{
    afxObject   obj;
};

struct afxIoScriptInterface
{
    afxError    (*Begin)(afxIoScript scr, afxNat flags);
    afxError    (*End)(afxIoScript scr);
    afxError    (*Reset)(afxIoScript scr);

    void        (*PrescribePush)(afxIoScript scr, afxNat tlsSlotIdx);
    void        (*PrescribeZero)(afxIoScript scr, void *p, afxNat offset, afxNat range);
    void        (*PrescribeFill)(afxIoScript scr, void *p, afxNat offset, afxNat range, afxNat value);
    void        (*PrescribeCopy)(afxIoScript scr, void *p, afxNat offset, afxNat range, void *dst, afxNat dstOff);
    void        (*PrescribeInput)(afxIoScript scr, afxStream ios, afxNat offset, afxNat range, void *dst, afxNat dstOffset);
    void        (*PrescribeOutput)(afxIoScript scr, afxStream ios, afxNat offset, afxNat range, void *src, afxNat srcOffset);

}
AFX AfxIoScript;

#endif//AFX_IO_SCRIPT_H