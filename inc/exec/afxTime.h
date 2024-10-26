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

#ifndef AFX_TIME_H
#define AFX_TIME_H

#include "qwadro/inc/base/afxCoreDefs.h"

typedef afxUnit16    afxCompactTime; // DOS time

typedef afxInt64 afxTime;

typedef afxInt32 afxMillisecond; // Time in ms

#define AFX_TIME_INFINITE (afxTime)AFX_I64_MAX

AFX_DEFINE_STRUCT(afxTimeSpec)
{
    afxTime     sec;  // Seconds - >= 0
    afxInt32        nsec; // Nanoseconds - [0, 999999999]
};

AFX_STATIC_ASSERT(sizeof(afxInt32) == sizeof(long), "");
AFX_STATIC_ASSERT(sizeof(afxTime) == sizeof(time_t), "");

AFX afxTime AfxGetTime(afxTime *t);

AFX afxUnit AfxGetTimer(void);

AFX afxUnit64 AfxGetTickCounter(void);

AFX afxUnit64 AfxGetTicksPerSecond(void);


#define AfxCompactTimeGetHour(t) ((t)>>11)
#define AfxCompactTimeGetMin(t) (((t)>>5) & 63)
#define AfxCompactTimeGetSec(t) (((t) & 31) * 2)
#define AfxCompactTimeGetTime(h,m,s) (((h)<<11) + ((m)<<5) + (s)/2)

#define AfxCompactTimeGetYear(t) (((t)>>9) + 1980)
#define AfxCompactTimeGetMonth(t) (((t)>>5) & 15)
#define AfxCompactTimeGetDay(t) ((t) & 31)
#define AfxCompactTimeGetDate(y,m,d) ((((y)-1980)<<9) + ((m)<<5) + (d))

#endif//AFX_TIME_H
