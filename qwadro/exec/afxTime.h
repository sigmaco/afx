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

#include "qwadro/base/afxCoreDefs.h"

typedef afxInt64 afxTime;

#define AFX_TIMEOUT_IGNORED 0
#define AFX_TIMEOUT_INFINITE AFX_U64_MAX

AFX_DEFINE_STRUCT(afxTimeSpec)
// Structure holding an interval broken down into seconds and nanoseconds.
{
    afxTime     secs;  // Seconds - >= 0
    afxInt32    nsecs; // Nanoseconds - [0, 999999999]
};

AFX_STATIC_ASSERT(sizeof(afxInt32) == sizeof(long), "");
AFX_STATIC_ASSERT(sizeof(afxTime) == sizeof(time_t), "");

// nanoseconds per millisecond
// Get nanoseconds from milliseconds
#define AFX_NANOSECS_PER_MILLISECS(millisecs_) ((1000000) * (millisecs_)) // 1 ms = 1'000'000 ns

// Get millisecond from nanoseconds
// Pass 1 to get 1:1 constant milliseconds per nanosecond.
#define AFX_MILLISECS_PER_NANOSECS(nanosecs_) ((afxUnit32)((nanosecs_) / AFX_NANOSECS_PER_MILLISECS(1)))

// nanoseconds per seconds
// Get nanoseconds from seconds
// Pass 1 second to get 1:1 constant nanoseconds per second.
#define AFX_NANOSECS_PER_SECS(secs_) ((1000000000) * (secs_)) // 1 s = 1'000'000'000 ns

// second per nanoseconds
// Get seconds per nanoseconds
#define AFX_SECS_PER_NANOSECS(nanosecs_) ((afxUnit32)((nanosecs_) / AFX_NANOSECS_PER_SECS(1)))


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


typedef afxUnit16    afxCompactTime; // DOS time
#define AfxCompactTimeGetHour(t) ((afxUnit16)((t)>>11))
#define AfxCompactTimeGetMin(t) ((afxUnit16)(((t)>>5) & 63))
#define AfxCompactTimeGetSec(t) ((afxUnit16)(((t) & 31) * 2))
#define AfxCompactTimeGetTime(h,m,s) ((afxUnit16)(((h)<<11) + ((m)<<5) + (s)/2))

#define AfxCompactTimeGetYear(t) ((afxUnit16)(((t)>>9) + 1980)
#define AfxCompactTimeGetMonth(t) ((afxUnit16)(((t)>>5) & 15)
#define AfxCompactTimeGetDay(t) ((afxUnit16)((t) & 31)
#define AfxCompactTimeGetDate(y,m,d) ((afxUnit16)((((y)-1980)<<9) + ((m)<<5) + (d))

#endif//AFX_TIME_H
