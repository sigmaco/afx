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
    afxInt32    nsec; // Nanoseconds - [0, 999999999]
};

AFX_STATIC_ASSERT(sizeof(afxInt32) == sizeof(long), "");
AFX_STATIC_ASSERT(sizeof(afxTime) == sizeof(time_t), "");

AFX afxTime AfxGetTime(afxTime *t);

AFX afxUnit AfxGetTimer(void);

AFX afxUnit64 AfxGetTickCounter(void);

AFX afxUnit64 AfxGetTicksPerSecond(void);

/*
    The AfxMakeTimeSpace() function deploys a new afxTimeSpec converted from a uint64_t value representing 
    nanoseconds into a afxTimeSpec structure. This structure breaks the uint64_t nanoseconds value into 
    seconds and the remaining nanoseconds.
*/

AFX afxTimeSpec* AfxMakeTimeSpec(afxTimeSpec* ts, afxUnit64 nsec);

/*
    The AfxMakeTimeSpecInterval() function computes the time difference (delta) between two afxTimeSpec structures.

    The seconds and nanoseconds components separately and manage the potential carry-over from nanoseconds into seconds.

    If the end time is earlier than the start time (negative delta), the logic would still work, 
    though you might want to handle such cases explicitly depending on the use case.

    The code assumes the end time is after the start time, but it can be adapted to handle both directions by checking 
    for negative differences and taking absolute values if necessary.
*/

AFX afxTimeSpec* AfxMakeTimeSpecInterval(afxTimeSpec* ts, afxTimeSpec const* start, afxTimeSpec const* end);

/*
    The AfxGetTimeSpecDelta() function calculates the difference in nanoseconds between two afxTimeSpec structures.

    The delta is computed as the total nanosecond difference between the two afxTimeSpec values.
    The result is a single value (in nanoseconds) rather than separate seconds and nanoseconds,
    which is useful for precise calculations or performance measurements where just the nanosecond difference is required.

    If the result is negative (i.e., end time is earlier than start time),
    the difference will reflect that in negative nanoseconds.
    If end time is earlier than start time, the result will be negative.
    For example, if start time is later than end time, the result would show a negative number of nanoseconds.
*/

AFX afxInt64 AfxGetTimeSpecDelta(afxTimeSpec const* start, afxTimeSpec const* end);


#define AfxCompactTimeGetHour(t) ((t)>>11)
#define AfxCompactTimeGetMin(t) (((t)>>5) & 63)
#define AfxCompactTimeGetSec(t) (((t) & 31) * 2)
#define AfxCompactTimeGetTime(h,m,s) (((h)<<11) + ((m)<<5) + (s)/2)

#define AfxCompactTimeGetYear(t) (((t)>>9) + 1980)
#define AfxCompactTimeGetMonth(t) (((t)>>5) & 15)
#define AfxCompactTimeGetDay(t) ((t) & 31)
#define AfxCompactTimeGetDate(y,m,d) ((((y)-1980)<<9) + ((m)<<5) + (d))

#endif//AFX_TIME_H
