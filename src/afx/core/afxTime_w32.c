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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <timeapi.h>
#include <time.h>
#pragma comment (lib, "winmm")

#include "qwadro/core/afxSystem.h"

afxTime AfxGetTime(afxTime *t)
{
    return time((void*)t);
}

afxNat AfxGetTimer(void)
{
    return timeGetTime();
}

afxNat64 AfxGetTickCounter(void)
{
    afxNat64 ticks;
#ifdef AFX_ISA_X86_64
    // Using intrinsics for AMD64 until the compiler supports __asm
    ticks = __rdtsc();
#else // Win32 or Xbox
    // note: using cpuid as a serializing makes timings more accurate, 
    // at the expense of more overhead. (1.5% without versus 5% with cpuid)
#   ifdef AFX_ISA_X86_32
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

afxNat64 AfxGetTicksPerSecond(void)
{
    static afxNat64 freq = 0;

    if (freq == 0)
    {
        afxNat64 ticks;
        afxNat64 qticks;
        afxNat64 ticks2;
        afxNat64 qticks2;
        afxReal64 minFactor = 1e6f;

        // Iterate several times
        // We take the minimum value beacuse Sleep() sleeps for at least the specified time
        for (afxInt32 iter = 0; iter < 10; ++iter)
        {
            ticks = AfxGetTickCounter();
            QueryPerformanceCounter((LARGE_INTEGER*)&(qticks));

            ///
            ///    Sleep for a little while
            ///
            volatile afxInt32 x = 1;

            for (afxInt32 j = 0; j < 5000; ++j)
                x += x * x;

            ticks2 = AfxGetTickCounter();
            QueryPerformanceCounter((LARGE_INTEGER*)&(qticks2));

            // We assume that this is fixed & regular 
            QueryPerformanceFrequency((LARGE_INTEGER*)&(freq));

            // Work our calibration factor
            afxNat64 diff = ticks2 - ticks, qdiff = qticks2 - qticks;

            afxReal64 factor = (afxReal64)diff / (afxReal64)qdiff;

            // Is this smaller?
            if (factor < minFactor)
                minFactor = factor;
        }
        freq = (afxNat64)minFactor * freq;
    }
    return freq;
}
