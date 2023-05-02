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

#ifndef AFX_TIME_H
#define AFX_TIME_H

#include "afx/core/afxCoreDefs.h"

typedef afxNat16    afxCompactTime; // DOS time

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


#define AfxCompactTimeGetHour(t) ((t)>>11)
#define AfxCompactTimeGetMin(t) (((t)>>5) & 63)
#define AfxCompactTimeGetSec(t) (((t) & 31) * 2)
#define AfxCompactTimeGetTime(h,m,s) (((h)<<11) + ((m)<<5) + (s)/2)

#define AfxCompactTimeGetYear(t) (((t)>>9) + 1980)
#define AfxCompactTimeGetMonth(t) (((t)>>5) & 15)
#define AfxCompactTimeGetDay(t) ((t) & 31)
#define AfxCompactTimeGetDate(y,m,d) ((((y)-1980)<<9) + ((m)<<5) + (d))

#endif//AFX_TIME_H