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

#ifndef AFX_XSS_H
#define AFX_XSS_H

#include "qwadro/ux/afxUxDefs.h"

typedef struct LunaVM* xssVm;

AUX void        XssUnhandle(xssVm vm, afxNat cnt, afxHandle han[]);
AUX void        XssHandleEachCall(xssVm vm, afxNat cnt, afxString const signatures[], afxHandle handles[]);
AUX afxNat      XssHandleAnyCall(xssVm vm, afxNat cnt, afxString const signatures[], afxHandle handles[]);
AUX afxNat      XssHandleAnyVar(xssVm vm, afxString const* domain, afxNat baseSlot, afxNat cnt, afxString const name[], afxHandle handles[]);

AUX afxError    XssCall(xssVm vm, afxHandle method);

AUX void        XssPushError(xssVm vm, afxNat slot, afxError err);

AUX void        XssPushBool(xssVm vm, afxNat slot, afxBool val);
AUX afxBool     XssPullBool(xssVm vm, afxNat slot);

AUX void        XssPushReal(xssVm vm, afxNat slot, afxReal val);
AUX afxReal     XssPullReal(xssVm vm, afxNat slot);

AUX void        XssPushReal64(xssVm vm, afxNat slot, afxReal64 val);
AUX afxReal64   XssPullReal64(xssVm vm, afxNat slot);

AUX void        XssPullReal3(xssVm vm, afxNat slot, afxV3d v);

AUX void        XssPushNat(xssVm vm, afxNat slot, afxNat val);
AUX afxNat      XssPullNat(xssVm vm, afxNat slot);

AUX void        XssPushInt(xssVm vm, afxNat slot, afxInt val);
AUX afxInt      XssPullInt(xssVm vm, afxNat slot);

AUX void        XssPushSize(xssVm vm, afxNat slot, afxSize val);
AUX afxSize     XssPullSize(xssVm vm, afxNat slot);

AUX void        XssPushHandle(xssVm vm, afxNat slot, afxHandle han);
AUX afxHandle   XssPullHandle(xssVm vm, afxNat slot);

AUX void        XssPushInstance(xssVm vm, afxNat slot, void* obj);
AUX void*       XssPullInstance(xssVm vm, afxNat slot);

AUX void        XssPullUri(xssVm vm, afxNat slot, afxUri* uri);
AUX void        XssPullString(xssVm vm, afxNat slot, afxString* s);

AUX void GenAcqObj(xssVm vm);
AUX void GenRelObj(void* data);

#endif//AFX_XSS_H
