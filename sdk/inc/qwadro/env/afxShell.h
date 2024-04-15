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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_SHELL_H
#define AFX_SHELL_H

#include "qwadro/core/afxThread.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/core/afxFixedString.h"
#include "qwadro/math/afxVector.h"
#include "qwadro/core/afxManager.h"

AFX_DEFINE_HANDLE(afxHandle);

AFX afxError    AfxLoadScript(afxString const* domain, afxUri const* uri);

AFX afxError    AfxAcquireScript(afxString const* domain, afxString const* code);

AFX afxError    AfxInjectScript(afxString const* domain, afxString const* code);

AFX void        XssUnhandle(afxShell sh, afxNat cnt, afxHandle han[]);
AFX void        XssHandleEachCall(afxShell sh, afxNat cnt, afxString const signatures[], afxHandle handles[]);
AFX afxNat      XssHandleAnyCall(afxShell sh, afxNat cnt, afxString const signatures[], afxHandle handles[]);
AFX afxNat      XssHandleAnyVar(afxShell sh, afxString const* domain, afxNat baseSlot, afxNat cnt, afxString const name[], afxHandle handles[]);

AFX afxError    XssCall(afxShell sh, afxHandle method);

AFX void        XssPushError(afxShell sh, afxNat slot, afxError err);

AFX void        XssPushBool(afxShell sh, afxNat slot, afxBool val);
AFX afxBool     XssPullBool(afxShell sh, afxNat slot);

AFX void        XssPushReal(afxShell sh, afxNat slot, afxReal val);
AFX afxReal     XssPullReal(afxShell sh, afxNat slot);

AFX void        XssPushReal64(afxShell sh, afxNat slot, afxReal64 val);
AFX afxReal64   XssPullReal64(afxShell sh, afxNat slot);

AFX void        XssPullReal3(afxShell sh, afxNat slot, afxV3d v);

AFX void        XssPushNat(afxShell sh, afxNat slot, afxNat val);
AFX afxNat      XssPullNat(afxShell sh, afxNat slot);

AFX void        XssPushInt(afxShell sh, afxNat slot, afxInt val);
AFX afxInt      XssPullInt(afxShell sh, afxNat slot);

AFX void        XssPushSize(afxShell sh, afxNat slot, afxSize val);
AFX afxSize     XssPullSize(afxShell sh, afxNat slot);

AFX void        XssPushHandle(afxShell sh, afxNat slot, afxHandle han);
AFX afxHandle   XssPullHandle(afxShell sh, afxNat slot);

AFX void        XssPushInstance(afxShell sh, afxNat slot, void* obj);
AFX void*       XssPullInstance(afxShell sh, afxNat slot);

AFX void        XssPullUri(afxShell sh, afxNat slot, afxUri* uri);
AFX void        XssPullString(afxShell sh, afxNat slot, afxString* s);

AFX void GenAcqObj(afxShell sh);
AFX void GenRelObj(void* data);

AFX afxError        AfxAcquireShell(afxShell* shell);

AFX afxManager*     AfxGetControllerManager(void);
AFX afxManager*     AfxGetKeyboardManager(void);
AFX afxManager*     AfxGetMouseManager(void);

AFX afxNat          AfxCountControllers(void);
AFX afxNat          AfxCountKeyboards(void);
AFX afxNat          AfxCountMouses(void);

AFX afxNat          AfxEnumerateControllers(afxNat first, afxNat cnt, afxController controllers[]);
AFX afxNat          AfxEnumerateKeyboards(afxNat first, afxNat cnt, afxKeyboard keyboards[]);
AFX afxNat          AfxEnumerateMouses(afxNat first, afxNat cnt, afxMouse mouses[]);

AFX afxNat          AfxInvokeControllers(afxNat first, afxNat cnt, afxBool(*f)(afxController, void*), void *udd);
AFX afxNat          AfxInvokeKeyboards(afxNat first, afxNat cnt, afxBool(*f)(afxKeyboard, void*), void *udd);
AFX afxNat          AfxInvokeMouses(afxNat first, afxNat cnt, afxBool(*f)(afxMouse, void*), void *udd);

#endif//AFX_SHELL_H
