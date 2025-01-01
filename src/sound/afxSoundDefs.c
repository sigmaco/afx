/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _ASX_AUDIO_C
#define _ASX_WAVEFORM_C
#define _ASX_SOUND_C
//#define _ASX_SOUND_SYSTEM_C
#include "impl/asxImplementation.h"

AFX_DEFINE_STRUCT(asxListener) // Listener information.
{
    afxTransform        pos;
    afxReal32           scalingFactor;
    afxBool             spatialized;
};

AFX_DEFINE_STRUCT(asxRamp) // volume ramp specified by end points "previous" and "next".
{
    afxReal32 prev;
    afxReal32 next;
};

#define ASX_MIN_DB_LEVEL -90.f
#define ASX_MIN_DB_GAIN 0.000032f
#define ASX_MAX_VOL_NORM_GAIN 12.f

ASXINL afxReal AfxToDecibel(afxReal gain)
{
    return (gain <= ASX_MIN_DB_GAIN) ? ASX_MIN_DB_LEVEL : (20.0f * AfxLog10f(gain));
}

ASXINL afxReal AfxFromDecibel(afxReal dB)
{
    return (dB <= ASX_MIN_DB_LEVEL) ? 0.f : AfxPowf(10.f, dB * (1.f / 20.f));
}

ASXINL void AsxResetListener(asxListener* lis)
{
    afxError err = NIL;
    AFX_ASSERT(lis);
    lis->scalingFactor = (afxReal32)1;
    lis->spatialized = TRUE;
}

ASXINL void AsxSetRamp(asxRamp* ramp, afxReal prev, afxReal next)
{
    afxError err = NIL;
    AFX_ASSERT(ramp);
    ramp->prev = (afxReal32)prev;
    ramp->next = (afxReal32)next;
}

ASXINL void AsxResetRamp(asxRamp* ramp)
{
    afxError err = NIL;
    AFX_ASSERT(ramp);
    AsxSetRamp(ramp, 1, 1);
}

ASXINL void AsxCopyRamp(asxRamp* ramp, asxRamp const* in)
{
    afxError err = NIL;
    AFX_ASSERT(ramp);
    AFX_ASSERT(in);
    ramp->prev = in->prev;
    ramp->next = in->next;
}

ASXINL void AsxScaleRamp(asxRamp* ramp, asxRamp const* rhs)
{
    afxError err = NIL;
    AFX_ASSERT(ramp);
    AFX_ASSERT(rhs);
    ramp->prev *= rhs->prev;
    ramp->next *= rhs->next;
}

ASXINL asxRamp AsxMergeRamps(asxRamp const* lhs, asxRamp const* rhs)
{
    asxRamp rslt;
    AsxCopyRamp(&rslt, lhs);
    AsxScaleRamp(&rslt, rhs);
    return rslt;
}
