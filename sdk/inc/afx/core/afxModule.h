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

#ifndef AFX_MODULE_H
#define AFX_MODULE_H

#include "afx/core/base/afxObject.h"

AFX_DEFINE_HANDLE(afxModule);

AFX void*       AfxModuleGetSystem(afxModule mdle);

AFX void*       AfxModuleFindSymbol(afxModule mdle, afxChar const *name);
AFX afxResult   AfxModuleFindSymbols(afxModule mdle, afxNat cnt, afxChar const *name[], void *sym[]);

AFX_OBJECT(afxModule)
{
    afxObject       obj;
    afxString32     name;
    void            *osHandle;
    afxBool         hasBeenLoaded;
};

#endif//AFX_MODULE_H