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

#ifndef AFX_CONDITION_H
#define AFX_CONDITION_H

#include "qwadro/inc/exec/afxMutex.h"

// The QWaitCondition class provides a condition variable for synchronizing threads.

// QWaitCondition allows a thread to tell other threads that some sort of condition has been met. One or many threads can block waiting for a QWaitCondition to set a condition with wakeOne() or wakeAll(). Use wakeOne() to wake one randomly selected thread or wakeAll() to wake them all.

AFX_DEFINE_STRUCT(afxCondition)
{
    //afxByte             data[64];
    afxAtom32           data[16];
};

AFX afxError            AfxSetUpCondition(afxCondition* cond);
AFX void                AfxCleanUpCondition(afxCondition* cond);

AFX afxError            AfxSignalCondition(afxCondition* cond);
AFX afxError            AfxSignalCondition2(afxCondition* cond);
AFX afxError            AfxWaitCondition(afxCondition* cond, afxMutex* mtx);
AFX afxResult           AfxWaitTimedCondition(afxCondition* cond, afxMutex* mtx, afxTimeSpec const* ts);

#endif//AFX_CONDITION_H
