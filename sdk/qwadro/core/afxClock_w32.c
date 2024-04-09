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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1

#if (defined(_WIN32))
#   define WIN32_LEAN_AND_MEAN 1
#   include <Windows.h>
#else
#   include <unistd.h>
#endif
#include "../dep/tinycthread/tinycthread.h"

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_THREAD_C
#include "qwadro/core/afxSystem.h"

_Thread_local static LARGE_INTEGER QPC_InitialVal = { 0 };
_Thread_local static unsigned int LastTickCount = 0;
_Thread_local static __int64 LastMicroseconds = 0;
_Thread_local static __int64 CurrentAdjustment = 0;
_Thread_local static LARGE_INTEGER QPC_Frequency = { 0 };

#define LODWORD(x)  (*((DWORD*)&(x)))  // low dword
#define HIDWORD(x)  (*((DWORD*)&(x)+1))

#define __SPAIR16__(high, low)  (((afxInt16)  (high) <<  8) | (afxNat8) (low))
#define __SPAIR32__(high, low)  (((afxInt32)  (high) << 16) | (afxNat16)(low))
#define __SPAIR64__(high, low)  (((afxInt64)  (high) << 32) | (afxNat32)(low))
#define __SPAIR128__(high, low) (((int128) (high) << 64) | (afxNat64)(low))
#define __PAIR16__(high, low)   (((afxNat16) (high) <<  8) | (afxNat8) (low))
#define __PAIR32__(high, low)   (((afxNat32) (high) << 16) | (afxNat16)(low))
#define __PAIR64__(high, low)   (((afxNat64) (high) << 32) | (afxNat32)(low))
#define __PAIR128__(high, low)  (((uint128)(high) << 64) | (afxNat64)(low))

#define __PAIR__ __SPAIR64__

_AFX DWORD CheckTimerInit(void)
{
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
    return result;
}

_AFX void AfxGetClock(afxClock *clock)
{
    afxError err = NIL;
    AfxAssert(clock);

    unsigned int v2; // edx
    unsigned int v3; // ebp
    signed __int64 v4; // rcx
    signed __int64 v5; // kr08_8
    __int64 count; // [esp+8h] [ebp-10h]
    afxReal64 v7; // [esp+10h] [ebp-8h]

    CheckTimerInit();
    QueryPerformanceCounter((LARGE_INTEGER *)&count);
    DWORD v1 = GetTickCount();
    v7 = (afxReal64)(count - QPC_InitialVal.QuadPart) * 1000000.0 / (afxReal64)QPC_Frequency.QuadPart;
    v2 = (unsigned __int64)((signed __int64)v7 + CurrentAdjustment) >> 32;
    v3 = LastMicroseconds;
    v4 = (signed __int64)v7 + CurrentAdjustment;
    v7 = (afxReal64)(v1 - LastTickCount) * 1000.0;
    v5 = (signed __int64)v7 - (v4 - LastMicroseconds);

    if (v5 > 200000 || v5 < -200000)
    {
        CurrentAdjustment += v5;
        v2 = (v5 + __PAIR__(v2, (unsigned int)v4)) >> 32;
        LODWORD(v4) = v5 + v4;
    }

    if ((signed __int64)__PAIR__(v2, (unsigned int)v4) >= LastMicroseconds)
    {
        LastTickCount = v1;
        LODWORD(LastMicroseconds) = v4;
        HIDWORD(LastMicroseconds) = v2;
        clock->data[0] = v4;
        clock->data[1] = v2;
    }
    else
    {
        clock->data[1] = HIDWORD(LastMicroseconds);
        clock->data[0] = v3;
    }
}

_AFX afxReal64 AfxGetSecondsElapsed(afxClock const *start, afxClock const *end)
{
    afxInt32 v2 = start->data[1];
    afxNat32 v3 = start->data[0];
    afxNat32 v4 = end->data[0];
    afxInt32 v5 = end->data[1];
    afxReal64 rslt;

    if (v2 > v5 || (v2 >= v5 && v3 >= v4))
        rslt = 0.0;
    else
        rslt = (afxReal64)(__PAIR__(v5, v4) - __PAIR__(v2, v3)) * 0.000001;

    return rslt;
}
