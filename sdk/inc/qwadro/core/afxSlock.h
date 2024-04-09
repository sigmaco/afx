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

#ifndef AFX_SLOCK_H
#define AFX_SLOCK_H

#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxMutex.h"
#include "qwadro/core/afxFcc.h"

/// Slim locks enable the threads of a single process to access shared resources; they are optimized for speed and occupy very little memory. 
/// Slim locks cannot be shared across processes.

/// Reader threads read data from a shared resource whereas writer threads write data to a shared resource. 
/// When multiple threads are reading and writing using a shared resource, 
/// exclusive locks such as a critical section or mutex can become a bottleneck if the reader threads run continuously but write operations are rare.

typedef struct { _AFX_DBG_FCC; void *srwl; afxAtomic tidEx; } afxSlock;

AFX afxError AfxSetUpSlock(afxSlock *slck);
AFX afxError AfxCleanUpSlock(afxSlock *slck);

AFX void    AfxEnterSlockShared(afxSlock *slck);
AFX void    AfxEnterSlockExclusive(afxSlock *slck);

AFX void    AfxExitSlockShared(afxSlock *slck);
AFX void    AfxExitSlockExclusive(afxSlock *slck);

AFX afxBool AfxTryEnterSlockShared(afxSlock *slck);
AFX afxBool AfxTryEnterSlockExclusive(afxSlock *slck);

#endif//AFX_SLOCK_H
