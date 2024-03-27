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

#ifndef AFX_ENVIRONMENT_H
#define AFX_ENVIRONMENT_H

#include "qwadro/core/afxThread.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/core/afxFixedString.h"
#include "qwadro/math/afxVector.h"

AFX_DEFINE_HANDLE(afxHandle);

AFX afxError    AfxLoadScript(afxString const* domain, afxUri const* uri);

AFX afxError    AfxAcquireScript(afxString const* domain, afxString const* code);

AFX afxError    AfxInjectScript(afxString const* domain, afxString const* code);

AFX void        XssUnhandle(afxEnvironment env, afxNat cnt, afxHandle han[]);
AFX void        XssHandleEachCall(afxEnvironment env, afxNat cnt, afxString const signatures[], afxHandle handles[]);
AFX afxNat      XssHandleAnyCall(afxEnvironment env, afxNat cnt, afxString const signatures[], afxHandle handles[]);
AFX afxNat      XssHandleAnyVar(afxEnvironment env, afxString const* domain, afxNat baseSlot, afxNat cnt, afxString const name[], afxHandle handles[]);

AFX afxError    XssCall(afxEnvironment env, afxHandle method);

AFX void        XssPushError(afxEnvironment env, afxNat slot, afxError err);

AFX void        XssPushBool(afxEnvironment env, afxNat slot, afxBool val);
AFX afxBool     XssPullBool(afxEnvironment env, afxNat slot);

AFX void        XssPushReal(afxEnvironment env, afxNat slot, afxReal val);
AFX afxReal     XssPullReal(afxEnvironment env, afxNat slot);

AFX void        XssPushReal64(afxEnvironment env, afxNat slot, afxReal64 val);
AFX afxReal64   XssPullReal64(afxEnvironment env, afxNat slot);

AFX void        XssPullReal3(afxEnvironment env, afxNat slot, afxV3d v);

AFX void        XssPushNat(afxEnvironment env, afxNat slot, afxNat val);
AFX afxNat      XssPullNat(afxEnvironment env, afxNat slot);

AFX void        XssPushInt(afxEnvironment env, afxNat slot, afxInt val);
AFX afxInt      XssPullInt(afxEnvironment env, afxNat slot);

AFX void        XssPushSize(afxEnvironment env, afxNat slot, afxSize val);
AFX afxSize     XssPullSize(afxEnvironment env, afxNat slot);

AFX void        XssPushHandle(afxEnvironment env, afxNat slot, afxHandle han);
AFX afxHandle   XssPullHandle(afxEnvironment env, afxNat slot);

AFX void        XssPushInstance(afxEnvironment env, afxNat slot, void* obj);
AFX void*       XssPullInstance(afxEnvironment env, afxNat slot);

AFX void        XssPullUri(afxEnvironment env, afxNat slot, afxUri* uri);
AFX void        XssPullString(afxEnvironment env, afxNat slot, afxString* s);

AFX void GenAcqObj(afxEnvironment env);
AFX void GenRelObj(void* data);

AFX afxEnvironment AfxAcquireEnvironment(void);

#endif//AFX_ENVIRONMENT_H
