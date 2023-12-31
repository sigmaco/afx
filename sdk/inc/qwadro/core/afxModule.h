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

/// Um afxModule é uma API do QWADRO para Applicable Service Interface (.asi) e Installable Client Driver (.icd), 
/// estes que são os dois formatos executáveis e vinculáveis que expandem o ecossistema de execução QWADRO.

#ifndef AFX_MODULE_H
#define AFX_MODULE_H

#include "qwadro/core/afxObject.h"
#include "qwadro/io/afxUri.h"

typedef enum afxModuleType
{
    afxModuleType_ASI,
    afxModuleType_ICD,
    afxModuleType_SYS,
    afxModuleType_DLL
} afxModuleType;

#ifdef _AFX_CORE_C
#ifdef _AFX_MODULE_C
AFX_OBJECT(afxModule)
{
    afxModuleType       type;
    afxFixedUri128      path;
    void                *osHandle;
    afxBool             hasBeenLoaded;
    
    afxString           description;
    afxString           version;
    afxString           product;
    afxString           vendor;
    afxString           legal;
};
#endif//_AFX_MODULE_C
#endif//_AFX_CORE_C

AFX afxUri const*       AfxGetModulePath(afxModule mdle);

AFX void*               AfxFindModuleSymbol(afxModule mdle, afxChar const *name);
AFX afxResult           AfxFindModuleSymbols(afxModule mdle, afxNat cnt, afxChar const *name[], void *sym[]);

AFX void                AfxGetModuleVendor(afxModule mdle, afxString const* name);
AFX void                AfxGetModuleVersion(afxModule mdle, afxNat *major, afxNat *minor, afxNat *patch);

AFX afxError            AfxAcquireModules(afxNat cnt, afxUri const uri[], afxModule mdle[]);

#endif//AFX_MODULE_H
