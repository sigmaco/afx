/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <timeapi.h>
#include <time.h>
#pragma comment (lib, "winmm")

#include "../exec/afxSystemDDK.h"

afxTime AfxGetTime(afxTime *t)
{
    return time((void*)t);
}

afxUnit AfxGetTimer(void)
{
    return timeGetTime();
}

afxUnit64 AfxGetTickCounter(void)
{
    afxUnit64 ticks;
#ifdef AFX_ON_X86_64
    // Using intrinsics for AMD64 until the compiler supports __asm
    ticks = __rdtsc();
#else // Win32 or Xbox
    // note: using cpuid as a serializing makes timings more accurate, 
    // at the expense of more overhead. (1.5% without versus 5% with cpuid)
#   ifdef AFX_ON_X86_32
    __asm
    {
        pushad
        cpuid
        rdtsc
        mov dword ptr[ticks], eax
        mov dword ptr[ticks + 4], edx
        popad
    }
#   elif defined(AFX_PLATFORM_ARM) 
    //unsigned int value;
    // Read CCNT Register
    //__asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));
    LARGE_INTEGER i;
    QueryPerformanceCounter(&i);
    ticks = i.QuadPart;
#   else // PPC
    //ticks = __mftb();
#   endif
#endif
    return ticks;
}

afxUnit64 AfxGetTicksPerSecond(void)
{
    static afxUnit64 freq = 0;

    if (freq == 0)
    {
        afxUnit64 ticks;
        afxUnit64 qticks;
        afxUnit64 ticks2;
        afxUnit64 qticks2;
        afxReal64 minFactor = 1e6f;

        // Iterate several times
        // We take the minimum value beacuse Sleep() sleeps for at least the specified time
        for (afxInt32 iter = 0; iter < 10; ++iter)
        {
            ticks = AfxGetTickCounter();
            QueryPerformanceCounter((LARGE_INTEGER*)&(qticks));

            ///
            //    Sleep for a little while
            ///
            volatile afxInt32 x = 1;

            for (afxInt32 j = 0; j < 5000; ++j)
                x += x * x;

            ticks2 = AfxGetTickCounter();
            QueryPerformanceCounter((LARGE_INTEGER*)&(qticks2));

            // We assume that this is fixed & regular 
            QueryPerformanceFrequency((LARGE_INTEGER*)&(freq));

            // Work our calibration factor
            afxUnit64 diff = ticks2 - ticks, qdiff = qticks2 - qticks;

            afxReal64 factor = (afxReal64)diff / (afxReal64)qdiff;

            // Is this smaller?
            if (factor < minFactor)
                minFactor = factor;
        }
        freq = (afxUnit64)minFactor * freq;
    }
    return freq;
}

_AFX afxTimeSpec* AfxMakeTimeSpec(afxTimeSpec* ts, afxUnit64 nsec)
{
    afxError err = { 0 };
    AFX_ASSERT(ts);

    if (!nsec) *ts = (afxTimeSpec) { 0 };
    else
    {
        // Convert nanoseconds to timespec
        afxTimeSpec ts2;
        
        // Divides the nanoseconds by 1 billion to get the full seconds (sec).
        ts2.secs = nsec / 1000000000; // Get full seconds

        // Uses the remainder to get the nanoseconds (nsec), which is the part that doesn't complete a full second.
        ts2.nsecs = nsec % 1000000000; // Get the remainder nanoseconds
        *ts = ts2;
    }
    return ts;
}

_AFX afxTimeSpec* AfxMakeTimeSpecInterval(afxTimeSpec* ts, afxTimeSpec const* start, afxTimeSpec const* end)
{
    afxError err = { 0 };
    AFX_ASSERT(ts);
    AFX_ASSERT(start);
    AFX_ASSERT(end);

    // If the end time is earlier than the start time (negative delta), 
    // the logic would still work, though you might want to handle such cases explicitly depending on the use case.
    // The code assumes the end time is after the start time, 
    // but it can be adapted to handle both directions by checking for negative differences and taking absolute values if necessary.

    // Subtract the seconds
    ts->secs = end->secs - start->secs;

    // Subtract the nanoseconds
    ts->nsecs = end->nsecs - start->nsecs;

    // If nanoseconds are negative, adjust by borrowing 1 second (1e9 nanoseconds)
    if (ts->nsecs < 0)
    {
        ts->nsecs += 1000000000; // 1 second in nanoseconds
        ts->secs -= 1;           // Borrow 1 second
    }

    return ts;
}

_AFX afxInt64 AfxGetTimeSpecDelta(afxTimeSpec const* start, afxTimeSpec const* end)
{
    afxError err = { 0 };
    AFX_ASSERT(start);
    AFX_ASSERT(end);

    // The delta is computed as the total nanosecond difference between the two timespec values.
    // The result is a single value (in nanoseconds) rather than separate seconds and nanoseconds, 
    // which is useful for precise calculations or performance measurements where just the nanosecond difference is required.

    // If the result is negative (i.e., end time is earlier than start time), 
    // the difference will reflect that in negative nanoseconds.
    // If end time is earlier than start time, the result will be negative. 
    // For example, if start time is later than end time, the result would show a negative number of nanoseconds.

    // Convert both start and end times to nanoseconds
    afxInt64 startNs = (afxInt64)start->secs * 1000000000 + start->nsecs;
    afxInt64 endNs = (afxInt64)end->secs * 1000000000 + end->nsecs;

    // Return the difference in nanoseconds
    return (endNs - startNs);
}


AFX_DEFINE_STRUCT(afxTimer)
{
    afxClock prevClock;
    afxUnit stepsPerSec;
    afxUnit stepCnt;
    afxBool running;
};


_AFX afxError AfxMakeTimer(afxTimer* timer)
{
    afxError err = { 0 };
    AFX_ASSERT(timer);
    return err;
}

_AFX afxError AfxFixTimerStep(afxTimer* timer, afxUnit stepsPerSec)
{
    afxError err = { 0 };
    AFX_ASSERT(timer);
    timer->stepsPerSec = (stepsPerSec > 0 && stepsPerSec < 5) ? 5 : stepsPerSec < 0 ? 0 : stepsPerSec;
    return err;
}

_AFX afxError AfxStepTimer(afxTimer* timer)
{
    afxError err = { 0 };
    AFX_ASSERT(timer);
    
    if (!timer->running)
    {
        AfxGetClock(&timer->prevClock);
        timer->running = TRUE;
    }

    afxClock clock;
    AfxGetClock(&clock);
    afxReal64 dt = AfxGetSecondsElapsed(&timer->prevClock, &clock);

    if (timer->stepsPerSec)
    {

    }

    ++timer->stepCnt;

    return err;
}
