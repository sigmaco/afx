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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SOUND_DEFS_H
#define AFX_SOUND_DEFS_H

#include "afx/core/afxDebug.h"
#include "afx/core/afxSimd.h"
#include "afx/math/afxTransform.h"

typedef enum afxCurveInterpolation // curve interpolation types
{
    // curves from 0 to LastFadeCurve NEED TO BE A MIRROR IMAGE AROUND LINEAR (ex. Log3 is the inverse of Exp3)
    afxCurveInterpolation_Log3 = 0, // log3
    afxCurveInterpolation_Sine = 1, // sine
    afxCurveInterpolation_Log1 = 2, // log1
    afxCurveInterpolation_InvSCurve = 3, // inversed S curve
    afxCurveInterpolation_Linear = 4, // linear (default)
    afxCurveInterpolation_SCurve = 5, // S curve
    afxCurveInterpolation_Exp1 = 6, // exp1
    afxCurveInterpolation_SineRecip = 7, // reciprocal of sine curve
    afxCurveInterpolation_Exp3 = 8, // exp3
    afxCurveInterpolation_LastFadeCurve = 8, // update this value to reflect last curve available for fades
    afxCurveInterpolation_Constant = 9  // constant ( not valid for fading values )
} afxCurveInterpolation;

AFX_DEFINE_STRUCT(afxListener) // Listener information.
{
    afxTransform        pos; /// Listener position (see AK::SoundEngine::SetPosition()).

    afxReal32           scalingFactor; /// Listener scaling factor (see AK::SoundEngine::SetListenerScalingFactor()).
    afxBool             spatialized; /// Whether listener is spatialized or not (see AK::SoundEngine::SetListenerSpatialization()).
};

AFXINL void AfxAcquireListener(afxListener *list)
{
    afxError err = NIL;
    AfxAssert(list);
    list->scalingFactor = (afxReal32)1;
    list->spatialized = TRUE;
}

AFX_DEFINE_STRUCT(afxRamp) // volume ramp specified by end points "previous" and "next".
{
    afxReal32 prev;
    afxReal32 next;
};

AFXINL void AfxAcquireRamp(afxRamp *ramp, afxReal prev, afxReal next)
{
    afxError err = NIL;
    AfxAssert(ramp);
    ramp->prev = (afxReal32)prev;
    ramp->next = (afxReal32)next;
}

AFXINL void AfxResetRamp(afxRamp *ramp)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAcquireRamp(ramp, 1, 1);
}

AFXINL void AfxCopyRamp(afxRamp *ramp, afxRamp const* in)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(in);
    ramp->prev = in->prev;
    ramp->next = in->next;
}

AFXINL void AfxScaleRamp(afxRamp *ramp, afxRamp const *rhs)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(rhs);
    ramp->prev *= rhs->prev;
    ramp->next *= rhs->next;
}

AFXINL afxRamp AfxMergeRamps(afxRamp const* lhs, afxRamp const* rhs)
{
    afxRamp rslt;
    AfxCopyRamp(&rslt, lhs);
    AfxScaleRamp(&rslt, rhs);
    return rslt;
}

AFX_DEFINE_STRUCT(afxChannelConfig)
{
    afxNat32 uNumChannels : 8;   // the number of channels, identified (deduced from channel mask) or anonymous (set directly). 
    afxNat32 eConfigType : 4;    // a code that completes the identification of channels by uChannelMask.
    afxNat32 uChannelMask : 20; // a bit field, whose channel identifiers depend on afxChannelConfigType (up to 20). Channel bits are defined in Speaker Config.
};

#endif//AFX_SOUND_DEFS_H
