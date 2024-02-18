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

// The autonomous service interface for Qwadro.

#ifndef AFX_SERVICE_H
#define AFX_SERVICE_H

#include "qwadro/io/afxStream.h"

AFX_DEFINE_STRUCT(afxServiceConfig)
{
    int a;
};

#ifdef _AFX_CORE_C
#ifdef _AFX_SERVICE_C
AFX_OBJECT(afxService)
{

};
#endif//_AFX_SERVICE_C
#endif//_AFX_CORE_C


AFX afxError    AfxAcquireServices(afxNat cnt, afxServiceConfig const config[], afxService services[]);

#endif//AFX_SERVICE_H
