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

#ifndef AFX_TRANSISTOR_H
#define AFX_TRANSISTOR_H

#include "afx/core/async/afxMutex.h"
#include "afx/core/afxFcc.h"

typedef struct { afxFcc fcc;  void *srwl; afxNat tid; } afxTransistor;

AFX afxError AfxTransistorDeploy(afxTransistor *lck);
AFX afxError AfxTransistorDrop(afxTransistor *lck);

AFX void    AfxTransistorEnterShared(afxTransistor *lck);
AFX void    AfxTransistorEnterExclusive(afxTransistor *lck);

AFX void    AfxTransistorExitShared(afxTransistor *lck);
AFX void    AfxTransistorExitExclusive(afxTransistor *lck);

AFX afxBool AfxTransistorTryEnterShared(afxTransistor *lck);
AFX afxBool AfxTransistorTryEnterExclusive(afxTransistor *lck);

#endif//AFX_TRANSISTOR_H