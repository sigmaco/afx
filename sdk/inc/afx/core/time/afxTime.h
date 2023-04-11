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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_TIMER_H
#define AFX_TIMER_H

#include "../base/afxCoreDefs.h"

typedef afxInt64 afxTime;

AFX_DEFINE_STRUCT(afxTimeSpec)
{
    afxTime     sec;  // Seconds - >= 0
    afxInt32        nsec; // Nanoseconds - [0, 999999999]
};

static_assert(sizeof(afxInt32) == sizeof(long), "");
static_assert(sizeof(afxTime) == sizeof(time_t), "");

AFX afxTime AfxGetTime(afxTime *t);

AFX afxNat AfxGetTimer(void);

AFX afxNat64 AfxGetTickCounter(void);

AFX afxNat64 AfxGetTicksPerSecond(void);

#endif//AFX_TIMER_H