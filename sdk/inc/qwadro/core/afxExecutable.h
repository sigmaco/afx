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

/// Um afxExecutable é uma API do QWADRO para Applicable Service Interface (.asi) e Installable Client Driver (.icd), 
/// estes que são os dois formatos executáveis e vinculáveis que expandem o ecossistema de execução QWADRO.

#ifndef AFX_EXECUTABLE_H
#define AFX_EXECUTABLE_H

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
#ifdef _AFX_EXECUTABLE_C
AFX_OBJECT(afxExecutable)
{
    afxModuleType       type;
    afxFixedUri128      path;
    void                *osHandle;
    afxBool             hasBeenLoaded;
    afxBool             demangle;
    
    afxString           description;
    afxString           version;
    afxString           product;
    afxString           vendor;
    afxString           legal;
};
#endif//_AFX_EXECUTABLE_C
#endif//_AFX_CORE_C

AFX afxUri const*       AfxGetExecutablePath(afxExecutable exe);

AFX void*               AfxFindExecutableSymbol(afxExecutable exe, afxChar const *name);
AFX afxResult           AfxFindModuleSymbols(afxExecutable exe, afxNat cnt, afxChar const *name[], void *sym[]);

AFX afxNat              AfxFindSymbolAddresses(afxExecutable exe, afxNat cnt, afxString const names[], void* addresses[]);

AFX void                AfxGetModuleVendor(afxExecutable exe, afxString const* name);
AFX void                AfxGetModuleVersion(afxExecutable exe, afxNat *major, afxNat *minor, afxNat *patch);

AFX afxError            AfxLoadExecutables(afxNat cnt, afxUri const uri[], afxExecutable mdle[]);

#endif//AFX_EXECUTABLE_H
