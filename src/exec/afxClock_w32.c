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

#if (defined(_WIN32))
#   define WIN32_LEAN_AND_MEAN 1
#   include <Windows.h>
#else
#   include <unistd.h>
#   include <time.h>
#   include <sys/time.h>
#endif
//#include "../dep/tinycthread/tinycthread.h"

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_THREAD_C
#include "afxSystemDDK.h"

#ifdef _WIN32

#ifdef _WIN32
AFX_THREAD_LOCAL static LARGE_INTEGER QPC_InitialVal = { 0 };
AFX_THREAD_LOCAL static afxUnit32 LastTickCount = 0;
AFX_THREAD_LOCAL static afxInt64 LastMicroseconds = 0;
AFX_THREAD_LOCAL static afxInt64 CurrentAdjustment = 0;
AFX_THREAD_LOCAL static LARGE_INTEGER QPC_Frequency = { 0 };
#else
// Thread-local storage for Unix
AFX_THREAD_LOCAL static struct timespec QPC_InitialVal = { 0 };
AFX_THREAD_LOCAL static uint32_t LastTickCount = 0;
AFX_THREAD_LOCAL static int64_t LastMicroseconds = 0;
AFX_THREAD_LOCAL static int64_t CurrentAdjustment = 0;
AFX_THREAD_LOCAL static struct timespec QPC_Frequency = { 0 };
#endif

#define LODWORD(x)  (*((DWORD*)&(x)))  // low dword
#define HIDWORD(x)  (*((DWORD*)&(x)+1))

#define __SPAIR16__(high, low)  (((afxInt16)  (high) <<  8) | (afxUnit8) (low))
#define __SPAIR32__(high, low)  (((afxInt32)  (high) << 16) | (afxUnit16)(low))
#define __SPAIR64__(high, low)  (((afxInt64)  (high) << 32) | (afxUnit32)(low))
#define __SPAIR128__(high, low) (((int128) (high) << 64) | (afxUnit64)(low))
#define __PAIR16__(high, low)   (((afxUnit16) (high) <<  8) | (afxUnit8) (low))
#define __PAIR32__(high, low)   (((afxUnit32) (high) << 16) | (afxUnit16)(low))
#define __PAIR64__(high, low)   (((afxUnit64) (high) << 32) | (afxUnit32)(low))
#define __PAIR128__(high, low)  (((uint128)(high) << 64) | (afxUnit64)(low))

#define __PAIR__ __SPAIR64__

_AFXINL void CheckTimerInit(void)
{
#ifdef _WIN32
    DWORD result; // eax

    result = QPC_Frequency.HighPart | QPC_Frequency.LowPart;

    if (!QPC_Frequency.QuadPart)
    {
        QueryPerformanceFrequency(&QPC_Frequency);
        QueryPerformanceCounter(&QPC_InitialVal);
        LastMicroseconds = 0i64;
        result = GetTickCount();
        LastTickCount = result;
    }
#else
    if (QPC_Frequency.tv_sec == 0)
    {
        struct timespec freq;
        clock_getres(CLOCK_MONOTONIC, &freq);
        QPC_Frequency = freq;  // Assuming we want to use the resolution of CLOCK_MONOTONIC

        struct timespec initial_val;
        clock_gettime(CLOCK_MONOTONIC, &initial_val);
        QPC_InitialVal = initial_val;

        LastMicroseconds = 0;
        LastTickCount = 0;
    }
#endif
}

#endif

_AFXINL void AfxResetClock(afxClock *clock)
{
    afxError err;
    AFX_ASSERT(clock);
    *clock = (afxClock) { 0 };
}

_AFXINL void AfxCopyClock(afxClock* clock, afxClock const* src)
{
    afxError err;
    AFX_ASSERT(clock);
    AFX_ASSERT(src);
    *clock = *src;
}

#if 0

_AFX void AfxGetClock(afxClock* clock)
{
    afxError err = { 0 };
    AFX_ASSERT(clock);

    // Initialize the timer if necessary
    CheckTimerInit();

#ifdef _WIN32
    // Get the current performance counter value
    afxInt64 count;
    QueryPerformanceCounter((LARGE_INTEGER *)&count);
    DWORD currentTickCount = GetTickCount();

    // Calculate time in microseconds based on performance counter
    afxReal64 t = (afxReal64)(count - QPC_InitialVal.QuadPart) * 1000000.0 / AFX_CAST(afxReal64, QPC_Frequency.QuadPart);

    // High 32-bits of the calculated microseconds
    afxUnit32 v2 = (afxUnit64)((afxInt64)t + CurrentAdjustment) >> 32;
    afxUnit32 v3 = (afxUnit32)LastMicroseconds;

    // Calculating microseconds with adjustment
    afxInt64 msecs = (afxInt64)t + CurrentAdjustment;

    // Calculate the difference in milliseconds
    t = (afxReal64)(currentTickCount - LastTickCount) * 1000.0;

    // Adjust based on the difference between last and current microseconds
    afxInt64 adjChk = (afxInt64)t - (msecs - LastMicroseconds);

    // If the discrepancy exceeds a threshold, adjust the current adjustment
    if (adjChk > 200000 || adjChk < -200000)
    {
        CurrentAdjustment += adjChk;
        v2 = (adjChk + (((afxUnit64)v2 << 32) | (afxUnit32)msecs)) >> 32;
        LODWORD(msecs) = adjChk + msecs;
    }

    // If the new time is greater than the last time, update the clock
    if ((afxInt64)(((afxUnit64)v2 << 32) | (afxUnit32)msecs) >= LastMicroseconds)
    {
        LastTickCount = currentTickCount;
        LODWORD(LastMicroseconds) = msecs;
        HIDWORD(LastMicroseconds) = v2;
        // Set the clock's data with the new time values
        clock->data[0] = msecs;
        clock->data[1] = v2;
    }
    else
    {
        // If the new time is smaller, keep the last known values
        clock->data[1] = HIDWORD(LastMicroseconds);
        clock->data[0] = v3;
    }
#else
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    uint32_t currentTickCount;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    currentTickCount = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    // Convert timespec to microseconds
    int64_t count = (current_time.tv_sec - QPC_InitialVal.tv_sec) * 1000000 + (current_time.tv_nsec / 1000 - QPC_InitialVal.tv_nsec / 1000);

    int64_t v7 = (double)(currentTickCount - LastTickCount) * 1000.0;
    int64_t v5 = v7 - (count - LastMicroseconds);

    if (v5 > 200000 || v5 < -200000)
    {
        CurrentAdjustment += v5;
    }

    int64_t result = count + CurrentAdjustment;
    clock->data[0] = result;
    clock->data[1] = (result >> 32);

    LastTickCount = currentTickCount;
    LastMicroseconds = count;
#endif
}

_AFXINL afxReal64 AfxGetMicrosecondsElapsed(afxClock const* prev, afxClock const* last)
{
    // Extract higher and lower 32-bits from 'from' and 'last' clock data
    afxInt32 fromHigh = prev->data[1];
    afxUnit32 fromLow = prev->data[0];
    afxUnit32 untilLow = last->data[0];
    afxInt32 untilHigh = last->data[1];
    afxReal64 rslt;

#ifdef _WIN32
    // Check if 'last' time is earlier than 'from' time
    if (fromHigh > untilHigh || (fromHigh == untilHigh && fromLow >= untilLow))
        rslt = 0.0; // No time elapsed if 'last' is earlier or equal
    else
    {
        // Combine high and low parts to form 64-bit timestamp values for both 'from' and 'last'
        afxInt64 fromTimestamp = ((afxInt64)fromHigh << 32) | fromLow;
        afxInt64 untilTimestamp = ((afxInt64)untilHigh << 32) | untilLow;

        // Calculate elapsed time in seconds
        rslt = (afxReal64)(untilTimestamp - fromTimestamp);
    }
#else
    // Combine high and low parts to form 64-bit timestamp values for both 'from' and 'last'
    afxInt64 fromTimestamp = ((afxInt64)fromHigh << 32) | fromLow;
    afxInt64 untilTimestamp = ((afxInt64)untilHigh << 32) | untilLow;

    // Check if 'last' time is earlier than or equal to 'from' time
    if (fromTimestamp >= untilTimestamp)
    {
        rslt = 0.0;  // No time elapsed if 'last' is earlier or equal
    }
    else
    {
        // Calculate elapsed time in seconds
        rslt = (afxReal64)(untilTimestamp - fromTimestamp);
    }
#endif
    return rslt;
}

_AFXINL afxReal64 AfxGetSecondsElapsed(afxClock const* prev, afxClock const* last)
{
    // Extract higher and lower 32-bits from 'prev' and 'last' clock data
    afxInt32 fromHigh = prev->data[1];
    afxUnit32 fromLow = prev->data[0];
    afxUnit32 untilLow = last->data[0];
    afxInt32 untilHigh = last->data[1];
    afxReal64 rslt;

#ifdef _WIN32
    // Check if 'last' time is earlier than 'prev' time
    if (fromHigh > untilHigh || (fromHigh == untilHigh && fromLow >= untilLow)) rslt = 0.0; // No time elapsed if 'last' is earlier or equal
    else
    {
        // Combine high and low parts to form 64-bit timestamp values for both 'prev' and 'last'
        afxInt64 fromTimestamp = ((afxInt64)fromHigh << 32) | fromLow;
        afxInt64 untilTimestamp = ((afxInt64)untilHigh << 32) | untilLow;

        // Calculate elapsed time in seconds
        rslt = (afxReal64)(untilTimestamp - fromTimestamp) * 0.000001;
    }
#else
    // Combine high and low parts to form 64-bit timestamp values for both 'prev' and 'last'
    int64_t fromTimestamp = ((int64_t)fromHigh << 32) | fromLow;
    int64_t untilTimestamp = ((int64_t)untilHigh << 32) | untilLow;

    // Check if 'last' time is earlier than or equal to 'prev' time
    if (fromTimestamp >= untilTimestamp)
    {
        result = 0.0;  // No time elapsed if 'last' is earlier or equal
    }
    else
    {
        // Calculate elapsed time in seconds
        result = (double)(untilTimestamp - fromTimestamp) * 0.000001;
    }
#endif
    return rslt;
}
#endif

/*
    The AfxGetClock() function initializes a clock by getting the start time and frequency.
*/

_AFXINL void AfxGetClock(afxClock* clock)
{
    afxError err = { 0 };
    AFX_ASSERT(clock);
#ifdef _WIN32
    // Get the frequency (ticks per second)
    QueryPerformanceFrequency(&clock->f);
    // Get the current time (ticks since system start)
    QueryPerformanceCounter(&clock->c);
#else // POSIX
    // Get the current time (seconds and nanoseconds since boot)
    clock_gettime(CLOCK_MONOTONIC, &clock->t);
#endif
}

/*
    The AfxGetClockTicksElapsed() function calculates the elapsed time in ticks.
*/

_AFXINL afxInt64 AfxGetClockTicksElapsed(afxClock* prev, afxClock* last)
{
    afxError err = { 0 };
    AFX_ASSERT(prev);
    AFX_ASSERT(last);
#ifdef _WIN32
    // Calculate the elapsed time in ticks
    afxInt64 elapsedTicks = last->c - prev->c;
#else // POSIX
    // Calculate the elapsed time in ticks (nanoseconds)
    afxInt64 elapsedTicks = (last->t.secs - prev->t.secs) * 1000000000LL + (last->t.nsecs - prev->t.nsecs);
#endif
    return elapsedTicks;
}

/*
    The AfxGetNanosecondsElapsed() function calculates the elapsed time in nanoseconds.
    This function calculates the elapsed time in nanoseconds by converting the difference between 
    the current time and start time (measured in ticks) to nanoseconds.
*/

_AFXINL afxInt64 AfxGetNanosecondsElapsed(afxClock* prev, afxClock* last)
{
    afxError err = { 0 };
    AFX_ASSERT(prev);
    AFX_ASSERT(last);
#ifdef _WIN32
    afxInt64 elapsedTicks = AfxGetClockTicksElapsed(prev, last);
    // Convert ticks to nanoseconds
    afxInt64 elapsedNsecs = (elapsedTicks * 1000000000LL) / prev->f;
#else // POSIX
    // Calculate the elapsed time in nanoseconds
    afxInt64 elapsedNsecs = (last->t.secs - prev->t.secs) * 1000000000LL + (last->t.nsecs - prev->t.nsecs);
#endif
    return elapsedNsecs;
}

/*
    The AfxGetUltrasecondsElapsed() function calculates the elapsed time in ultraseconds (microseconds, in Qwadro parlance).
    This function calculates the elapsed time in microseconds by converting the ticks to microseconds.
*/

_AFXINL afxInt64 AfxGetUltrasecondsElapsed(afxClock* prev, afxClock* last)
{
    afxError err = { 0 };
    AFX_ASSERT(prev);
    AFX_ASSERT(last);
#ifdef _WIN32
    afxInt64 elapsedTicks = AfxGetClockTicksElapsed(prev, last);
    // Convert ticks to ultraseconds (microseconds, in Qwadro parlance)
    afxInt64 elapsedUsecs = (elapsedTicks * 1000000LL) / prev->f;
#else
    // Calculate the elapsed time in microseconds
    afxInt64 elapsedUsecs = (last->t.secs - prev->t.secs) * 1000000LL + (last->t.nsecs / 1000LL - prev->t.nsecs / 1000LL);
#endif
    return elapsedUsecs;
}

/*
    The AfxGetMillisecondsElapsed() function calculates the elapsed time in milliseconds.
    This function calculates the elapsed time in milliseconds by converting the ticks to milliseconds.
*/

_AFXINL afxInt64 AfxGetMillisecondsElapsed(afxClock* prev, afxClock* last)
{
    afxError err = { 0 };
    AFX_ASSERT(prev);
    AFX_ASSERT(last);
#ifdef _WIN32
    afxInt64 elapsedTicks = AfxGetClockTicksElapsed(prev, last);
    // Convert ticks to milliseconds
    afxInt64 elapsedMsecs = (elapsedTicks * 1000LL) / prev->f;
#else
    // Calculate the elapsed time in milliseconds
    afxInt64 elapsedMsecs = (last->t.secs - prev->t.secs) * 1000LL + (last->t.nsecs / 1000000LL - prev->t.nsecs / 1000000LL);
#endif
    return elapsedMsecs;
}

/*
    The AfxGetSecondsElapsed() function calculates the elapsed time in seconds.
    This function calculates the elapsed time in seconds (as a 64-bit real) by dividing the ticks by the frequency.
*/

_AFXINL afxReal64 AfxGetSecondsElapsed(afxClock* prev, afxClock* last)
{
    afxError err = { 0 };
    AFX_ASSERT(prev);
    AFX_ASSERT(last);
#ifdef _WIN32
    afxInt64 elapsedTicks = AfxGetClockTicksElapsed(prev, last);
    // Convert ticks to seconds
    afxReal64 elapsedSecs = (afxReal64)elapsedTicks / prev->f;
#else
    // Calculate the elapsed time in seconds
    afxReal64 elapsedSecs = (afxReal64)(last->t.secs - prev->t.secs) + (afxReal64)(last->t.nsecs - prev->t.nsecs) / 1000000000.0;
#endif
    return elapsedSecs;
}
