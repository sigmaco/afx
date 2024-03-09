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

/// Um afxIcd é uma API do QWADRO para Installable Client Driver (.icd), que é um formato executável e vinculável que registra afxDevice's no ecossistema de execução QWADRO.

#ifndef AFX_ICD_H
#define AFX_ICD_H

#include "qwadro/core/afxInitialization.h"

typedef enum afxIcdType
{
    afxIcdType_DRAW,
    afxIcdType_SOUND,
    afxIcdType_TELECOM,
    afxIcdType_SIM
} afxIcdType;

#ifdef _AFX_CORE_C
#ifdef _AFX_ICD_C
AFX_OBJECT(afxIcd)
{
    afxUri128           uri;
    afxIni              ini;
    afxString           product;
    afxString           description;
    afxString           vendor;
    afxUri              website;
    afxNat              verMajor; // 1
    afxNat              verMinor; // 0
    afxNat              verPatch; // 0

    afxChain            devices; // afxDevice
    afxExecutable       exe;
};
#endif//_AFX_ICD_C
#endif//_AFX_CORE_C

AFX afxError            AfxInstallClientDrivers(afxNat cnt, afxUri const manifest[], afxIcd drivers[]);

AFX afxUri const*       AfxGetIcdIdentifier(afxIcd icd);

AFX afxExecutable       AfxGetIcdExecutable(afxIcd icd);

AFX afxIni const*       AfxGetIcdInitializer(afxIcd icd);

AFX afxChain const*     AfxGetRegisteredDevices(afxIcd icd);

AFX void                AfxGetIcdVersion(afxIcd icd, afxNat* major, afxNat* minor, afxNat* patch);

#endif//AFX_ICD_H
