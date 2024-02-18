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

#include "qwadro/core/afxExecutable.h"

typedef enum afxIcdType
{
    afxIcdType_DRAW,
    afxIcdType_SOUND,
    afxIcdType_TELECOM,
    afxIcdType_SIM
} afxIcdType;

#if 0
AFX_DEFINE_STRUCT(afxIcdInfo)
{
    afxExecutable               mdle;
    afxString const*        name; // driver name: SIGMA GL/2
    afxString const*        vendor; // author: SIGMA Technology Group (The technology arm of Federação SIGMA)
    afxString const*        website; // website: sigmaco.org
    afxString const*        note; // The standard QWADRO draw system implementation.
    afxNat                  verMajor; // 1
    afxNat                  verMinor; // 0
    afxNat                  verPatch; // 0
    afxClassConfig const*   devExt;
    afxNat                  devCnt;
};
#endif

#ifdef _AFX_CORE_C
#ifdef _AFX_ICD_C
AFX_OBJECT(afxIcd)
{
    AFX_OBJECT(afxExecutable)exe;
    afxChain                classes;
    afxClass                devices;

    afxString               name;
    afxString               vendor;
    afxString               website;
    afxString               note;
    afxNat                  verMajor; // 1
    afxNat                  verMinor; // 0
    afxNat                  verPatch; // 0
};
#endif//_AFX_ICD_C
#endif//_AFX_CORE_C

#endif//AFX_ICD_H
