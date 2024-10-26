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

#ifndef AFX_SLOCK_H
#define AFX_SLOCK_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/exec/afxMutex.h"
#include "qwadro/inc/base/afxFcc.h"

/**
    Slim locks enable the threads of a single process to access shared resources; 
    they are optimized for speed and occupy very little memory. 
    Slim locks cannot be shared across processes.
    
    Reader threads read data from a shared resource whereas writer threads write data to a shared resource. 
    When multiple threads are reading and writing using a shared resource, 
    exclusive locks such as a critical section or mutex can become a bottleneck 
    if the reader threads run continuously but write operations are rare.
*/

typedef struct { void *srwl; afxUnit32 tidEx; } afxSlock;

AFX afxError AfxDeploySlock(afxSlock *slck);
AFX afxError AfxDismantleSlock(afxSlock *slck);

AFX void    AfxEnterSlockShared(afxSlock *slck);
AFX void    AfxEnterSlockExclusive(afxSlock *slck);

AFX void    AfxExitSlockShared(afxSlock *slck);
AFX void    AfxExitSlockExclusive(afxSlock *slck);

AFX afxBool AfxTryEnterSlockShared(afxSlock *slck);
AFX afxBool AfxTryEnterSlockExclusive(afxSlock *slck);

#endif//AFX_SLOCK_H
