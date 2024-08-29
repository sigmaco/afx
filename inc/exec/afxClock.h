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

#include "qwadro/inc/base/afxDebug.h"

AFX_DEFINE_STRUCT_SIMD(afxClock)
{
    afxNat32 /*AFX_SIMD*/ data[4];
};

/// Get the current clock state for the current thread.
AFXINL void     AfxGetClock(afxClock* clock);

AFXINL void     AfxResetClock(afxClock* clock);

AFXINL void     AfxCopyClock(afxClock* clock, afxClock const* src);

/// Find out how many seconds have elapsed from a previous clock up to another clock.
AFX afxReal64   AfxGetSecondsElapsed(afxClock const* from, afxClock const* until);

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

#endif//AFX_CLOCK_H
