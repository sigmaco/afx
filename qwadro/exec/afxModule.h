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

/// Um afxModule é uma API do QWADRO para Applicable Service Interface (.asi) e Installable Client Driver (.icd), 
/// estes que são os dois formatos executáveis e vinculáveis que expandem o ecossistema de execução QWADRO.

#ifndef AFX_MODULE_H
#define AFX_MODULE_H

#include "qwadro/base/afxObject.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/base/afxManifest.h"

typedef enum afxModuleFlags
{
    afxModuleFlag_DLL   = AFX_BITMASK(0), // has DllMain
    afxModuleFlag_ASI   = AFX_BITMASK(1), // has 
    afxModuleFlag_ICD   = AFX_BITMASK(2), // has DriverEntry
    afxModuleFlag_SYS   = AFX_BITMASK(3), // has
    
    afxModuleFlag_AVX   = AFX_BITMASK(8),
    afxModuleFlag_AMX   = AFX_BITMASK(9),
    afxModuleFlag_ASX   = AFX_BITMASK(10),
    afxModuleFlag_AUX   = AFX_BITMASK(11),
} afxModuleFlags;

AFX afxModuleFlags      AfxTestModule(afxModule mdle, afxModuleFlags bitmask);

AFX void                AfxGetModulePath(afxModule mdle, afxUri* uri);

AFX void*               AfxGetSymbolAddress(afxModule mdle, afxString const* name);
AFX afxUnit             AfxGetSymbolAddresses(afxModule mdle, afxUnit cnt, afxString const names[], void* addresses[]);
AFX afxUnit             AfxGetSymbolAddresses2(afxModule mdle, afxBool demangle, afxUnit cnt, afxString const names[], void* addresses[]);


AFX void*               AfxFindModuleSymbol(afxModule mdle, afxChar const *name);
AFX afxResult           AfxFindModuleSymbols(afxModule mdle, afxUnit cnt, afxChar const *name[], void *sym[]);

AFX afxUnit             AfxFindSymbolAddresses(afxModule mdle, afxUnit cnt, afxString const names[], void* addresses[]);

AFX void                AfxGetModuleInfo(afxModule icd, afxString* product, afxString* vendor, afxString* description);
AFX void                AfxGetModuleVersion(afxModule icd, afxUnit* verMajor, afxUnit* verMinor, afxUnit* verPatch);

AFX void*               AfxGetModuleIdd(afxModule mdle);

AFX afxManifest const*  AfxGetModuleManifest(afxModule icd);

////////////////////////////////////////////////////////////////////////////////

AFX afxBool     AfxFindModule(afxUri const *uri, afxModule* mdle);

AFX afxError    AfxLoadModule(afxUri const* uri, afxFlags flags, afxModule* mdle);

AFX afxUnit     AfxEnumerateModules(afxUnit first, afxUnit cnt, afxModule executables[]);

AFX afxUnit     AfxInvokeModules(afxUnit first, afxUnit cnt, afxBool(*f)(afxModule, void*), void *udd);

#endif//AFX_MODULE_H
