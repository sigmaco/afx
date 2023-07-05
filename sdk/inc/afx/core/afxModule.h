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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MODULE_H
#define AFX_MODULE_H

#include "afx/core/afxObject.h"

AFX_DEFINE_HANDLE(afxModule);

AFX_OBJECT(afxModule)
{
    afxObject           obj;
#ifdef _AFX_MODULE_C
    afxUri*             uri; // 128
    void                *osHandle;
    afxBool             hasBeenLoaded;
#endif
};

AFX void*               AfxModuleGetSystem(afxModule mdle);
AFX afxUri const*       AfxModuleGetUri(afxModule mdle);

AFX void*               AfxModuleFindSymbol(afxModule mdle, afxChar const *name);
AFX afxResult           AfxModuleFindSymbols(afxModule mdle, afxNat cnt, afxChar const *name[], void *sym[]);

#endif//AFX_MODULE_H