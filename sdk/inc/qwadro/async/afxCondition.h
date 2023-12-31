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

#ifndef AFX_CONDITION_H
#define AFX_CONDITION_H

#include "qwadro/async/afxMutex.h"

// The QWaitCondition class provides a condition variable for synchronizing threads.

// QWaitCondition allows a thread to tell other threads that some sort of condition has been met. One or many threads can block waiting for a QWaitCondition to set a condition with wakeOne() or wakeAll(). Use wakeOne() to wake one randomly selected thread or wakeAll() to wake them all.

AFX_DEFINE_STRUCT(AfxCondition)
{
    afxByte             data[48];
};

AFX afxResult           AfxTakeWaitCondition(AfxCondition *cond);
AFX void                AfxReleaseWaitCondition(AfxCondition *cond);

AFX afxResult           AfxSignalSingleCondition(AfxCondition *cond);
AFX afxResult           AfxSignalMultipleCondition(AfxCondition *cond);
AFX afxResult           AfxWaitCondition(AfxCondition *cond, afxMutex *mtx);
AFX afxResult           AfxWaitTimedCondition(AfxCondition *cond, afxMutex *mtx, afxTimeSpec const *ts);

#endif//AFX_CONDITION_H
