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

#ifndef AFX_CLOCK_H
#define AFX_CLOCK_H

#include "qwadro/exec/afxDebug.h"
#include "qwadro/exec/afxTime.h"

AFX_DEFINE_UNION_ALIGNED(AFX_SIMD_ALIGNMENT, afxClock)
{
    afxUnit32 /*AFX_SIMD*/ data[4];
    struct
    {
        afxInt64    c; // ticks since system start
        afxInt64    f; // ticks per second
    };
    afxTimeSpec     t;
};

/// Get the current clock state for the current thread.
AFXINL void     AfxGetClock(afxClock* clock);

AFXINL void     AfxResetClock(afxClock* clock);

AFXINL void     AfxCopyClock(afxClock* clock, afxClock const* src);

//AFX afxReal64   AfxGetMicrosecondsElapsed(afxClock const* prev, afxClock const* last);

/// Find out how many seconds have elapsed from a previous clock up to another clock.
//AFX afxReal64   AfxGetSecondsElapsed(afxClock const* prev, afxClock const* last);

/*
    afxClock last;
    AfxGetClock(&last);

    while(1)
    {
        afxClock curr;
        AfxGetClock(&curr);
        afxReal64 dt = AfxGetSecondsElapsed(&last, &curr);
        last = curr;
        Step(dt);
    }
*/



/*
    The AfxGetClock() function initializes a clock by getting the start time and frequency.
*/

AFXINL void AfxGetClock(afxClock* clock);

/*
    The AfxGetClockTicksElapsed() function calculates the elapsed time in ticks.
*/

AFXINL afxInt64 AfxGetClockTicksElapsed(afxClock* prev, afxClock* last);

/*
    The AfxGetNanosecondsElapsed() function calculates the elapsed time in nanoseconds.
    This function calculates the elapsed time in nanoseconds by converting the difference between
    the current time and start time (measured in ticks) to nanoseconds.
*/

AFXINL afxInt64 AfxGetNanosecondsElapsed(afxClock* prev, afxClock* last);

/*
    The AfxGetClockMicrosecondsElapsed() function calculates the elapsed time in ultraseconds (microseconds, in Qwadro parlance).
    This function calculates the elapsed time in microseconds by converting the ticks to microseconds.
*/

AFXINL afxInt64 AfxGetUltrasecondsElapsed(afxClock* prev, afxClock* last);

/*
    The AfxGetMillisecondsElapsed() function calculates the elapsed time in milliseconds.
    This function calculates the elapsed time in milliseconds by converting the ticks to milliseconds.
*/

AFXINL afxInt64 AfxGetMillisecondsElapsed(afxClock* prev, afxClock* last);

/*
    The AfxGetSecondsElapsed() function calculates the elapsed time in seconds.
    This function calculates the elapsed time in seconds (as a 64-bit real) by dividing the ticks by the frequency.
*/

AFXINL afxReal64 AfxGetSecondsElapsed(afxClock* prev, afxClock* last);

#endif//AFX_CLOCK_H
