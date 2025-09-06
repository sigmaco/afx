/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_AUDIO_C
#define _AMX_AUDIO_C
#define _AMX_MIX_C
//#define _AMX_MIX_SYSTEM_C
#include "ddi/amxImplementation.h"

AFX_DEFINE_STRUCT(amxListener) // Listener information.
{
    afxTransform        pos;
    afxReal32           scalingFactor;
    afxBool             spatialized;
};

AFX_DEFINE_STRUCT(amxRamp) // volume ramp specified by end points "previous" and "next".
{
    afxReal32 prev;
    afxReal32 next;
};

#define AMX_MIN_DB_LEVEL -90.f
#define AMX_MIN_DB_GAIN 0.000032f
#define AMX_MAX_VOL_NORM_GAIN 12.f

AMXINL afxReal AfxToDecibel(afxReal gain)
{
    return (gain <= AMX_MIN_DB_GAIN) ? AMX_MIN_DB_LEVEL : (20.0f * AfxLog10f(gain));
}

AMXINL afxReal AfxFromDecibel(afxReal dB)
{
    return (dB <= AMX_MIN_DB_LEVEL) ? 0.f : AfxPowf(10.f, dB * (1.f / 20.f));
}

AMXINL void AmxResetListener(amxListener* lis)
{
    afxError err = NIL;
    AFX_ASSERT(lis);
    lis->scalingFactor = (afxReal32)1;
    lis->spatialized = TRUE;
}

AMXINL void AmxSetRamp(amxRamp* ramp, afxReal prev, afxReal next)
{
    afxError err = NIL;
    AFX_ASSERT(ramp);
    ramp->prev = (afxReal32)prev;
    ramp->next = (afxReal32)next;
}

AMXINL void AmxResetRamp(amxRamp* ramp)
{
    afxError err = NIL;
    AFX_ASSERT(ramp);
    AmxSetRamp(ramp, 1, 1);
}

AMXINL void AmxCopyRamp(amxRamp* ramp, amxRamp const* in)
{
    afxError err = NIL;
    AFX_ASSERT(ramp);
    AFX_ASSERT(in);
    ramp->prev = in->prev;
    ramp->next = in->next;
}

AMXINL void AmxScaleRamp(amxRamp* ramp, amxRamp const* rhs)
{
    afxError err = NIL;
    AFX_ASSERT(ramp);
    AFX_ASSERT(rhs);
    ramp->prev *= rhs->prev;
    ramp->next *= rhs->next;
}

AMXINL amxRamp AmxMergeRamps(amxRamp const* lhs, amxRamp const* rhs)
{
    amxRamp rslt;
    AmxCopyRamp(&rslt, lhs);
    AmxScaleRamp(&rslt, rhs);
    return rslt;
}
