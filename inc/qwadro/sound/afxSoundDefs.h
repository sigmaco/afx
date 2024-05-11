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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ASX_SOUND_DEFS_H
#define ASX_SOUND_DEFS_H

#include "qwadro/core/afxSystem.h"

#ifndef __e2sound__
#   ifdef _DEBUG
#       define ASX DLLIMPORT extern 
#       define ASXINL DLLIMPORT EMBED
#   else
#       define ASX DLLIMPORT extern 
#       define ASXINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _ASX DLLEXPORT
#       define ASX DLLEXPORT extern 
#       define _ASXINL DLLEXPORT INLINE
#       define ASXINL DLLEXPORT EMBED
#   else
#       define _ASX DLLEXPORT
#       define ASX DLLEXPORT extern 
#       define _ASXINL DLLEXPORT INLINE
#       define ASXINL DLLEXPORT EMBED
#   endif
#endif//__e2sound__

//AFX_DEFINE_HANDLE(afxSoundSystem);
AFX_DEFINE_HANDLE(afxSoundDevice);
AFX_DEFINE_HANDLE(afxSoundContext);
AFX_DEFINE_HANDLE(afxSoundOutput);
AFX_DEFINE_HANDLE(afxSoundInput);
AFX_DEFINE_HANDLE(afxSoundBridge);
//AFX_DEFINE_HANDLE(afxSoundQueue);
AFX_DEFINE_HANDLE(asxBank);
AFX_DEFINE_HANDLE(asxTracker);
AFX_DEFINE_HANDLE(afxSoundBuffer);
AFX_DEFINE_HANDLE(afxSound);

typedef enum asxCurveInterpolation // curve interpolation types
{
    // curves from 0 to LastFadeCurve NEED TO BE A MIRROR IMAGE AROUND LINEAR (ex. Log3 is the inverse of Exp3)
    asxCurveInterpolation_Log3 = 0, // log3
    asxCurveInterpolation_Sine = 1, // sine
    asxCurveInterpolation_Log1 = 2, // log1
    asxCurveInterpolation_InvSCurve = 3, // inversed S curve
    asxCurveInterpolation_Linear = 4, // linear (default)
    asxCurveInterpolation_SCurve = 5, // S curve
    asxCurveInterpolation_Exp1 = 6, // exp1
    asxCurveInterpolation_SineRecip = 7, // reciprocal of sine curve
    asxCurveInterpolation_Exp3 = 8, // exp3
    asxCurveInterpolation_LastFadeCurve = 8, // update this value to reflect last curve available for fades
    asxCurveInterpolation_Constant = 9  // constant ( not valid for fading values )
} asxCurveInterpolation;

AFX_DEFINE_STRUCT(asxListener) // Listener information.
{
    afxTransform        pos;
    afxReal32           scalingFactor;
    afxBool             spatialized;
};

ASXINL void AaxAcquireListener(asxListener *list)
{
    afxError err = NIL;
    AfxAssert(list);
    list->scalingFactor = (afxReal32)1;
    list->spatialized = TRUE;
}

AFX_DEFINE_STRUCT(asxRamp) // volume ramp specified by end points "previous" and "next".
{
    afxReal32 prev;
    afxReal32 next;
};

ASXINL void AaxTakeRamp(asxRamp *ramp, afxReal prev, afxReal next)
{
    afxError err = NIL;
    AfxAssert(ramp);
    ramp->prev = (afxReal32)prev;
    ramp->next = (afxReal32)next;
}

ASXINL void AaxResetRamp(asxRamp *ramp)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AaxTakeRamp(ramp, 1, 1);
}

ASXINL void AaxCopyRamp(asxRamp *ramp, asxRamp const* in)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(in);
    ramp->prev = in->prev;
    ramp->next = in->next;
}

ASXINL void AaxScaleRamp(asxRamp *ramp, asxRamp const *rhs)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(rhs);
    ramp->prev *= rhs->prev;
    ramp->next *= rhs->next;
}

ASXINL asxRamp AaxMergeRamps(asxRamp const* lhs, asxRamp const* rhs)
{
    asxRamp rslt;
    AaxCopyRamp(&rslt, lhs);
    AaxScaleRamp(&rslt, rhs);
    return rslt;
}

AFX_DEFINE_STRUCT(asxChannelConfig)
{
    afxNat32 uNumChannels : 8;   // the number of channels, identified (deduced from channel mask) or anonymous (set directly). 
    afxNat32 eConfigType : 4;    // a code that completes the identification of channels by uChannelMask.
    afxNat32 uChannelMask : 20; // a bit field, whose channel identifiers depend on asxChannelConfigType (up to 20). Channel bits are defined in Speaker Config.
};

#endif//ASX_SOUND_DEFS_H
