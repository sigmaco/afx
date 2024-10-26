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

#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/base/afxManifest.h"

typedef enum afxModuleType
{
    afxModuleType_DLL, // has DllMain
    afxModuleType_ASI, // has 
    afxModuleType_ICD, // has DriverEntry
    afxModuleType_SYS, // has
} afxModuleType;

AFX void                AfxGetModulePath(afxModule mdle, afxUri* uri);

AFX void*               AfxGetSymbolAddress(afxModule mdle, afxString const* name);
AFX afxUnit              AfxGetSymbolAddresses(afxModule mdle, afxUnit cnt, afxString const names[], void* addresses[]);
AFX afxUnit              AfxGetSymbolAddresses2(afxModule mdle, afxBool demangle, afxUnit cnt, afxString const names[], void* addresses[]);


AFX void*               AfxFindModuleSymbol(afxModule mdle, afxChar const *name);
AFX afxResult           AfxFindModuleSymbols(afxModule mdle, afxUnit cnt, afxChar const *name[], void *sym[]);

AFX afxUnit              AfxFindSymbolAddresses(afxModule mdle, afxUnit cnt, afxString const names[], void* addresses[]);

AFX void                AfxGetModuleInfo(afxModule icd, afxString* product, afxString* vendor, afxString* description);
AFX void                AfxGetModuleVersion(afxModule icd, afxUnit* verMajor, afxUnit* verMinor, afxUnit* verPatch);

AFX void*               AfxGetModuleIdd(afxModule mdle);

AFX afxManifest const*  AfxGetModuleManifest(afxModule icd);

////////////////////////////////////////////////////////////////////////////////

AFX afxBool             AfxFindModule(afxUri const *uri, afxModule* mdle);

AFX afxError            AfxLoadModule(afxUri const* uri, afxFlags flags, afxModule* mdle);

#endif//AFX_MODULE_H
