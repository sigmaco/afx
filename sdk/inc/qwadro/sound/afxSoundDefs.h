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

#ifndef AFX_SOUND_DEFS_H
#define AFX_SOUND_DEFS_H

#include "qwadro/core/afxSystem.h"

#ifndef __e2sound__
#   ifdef _DEBUG
#       define AAX _AFXIMPORT extern 
#       define AAXINL _AFXIMPORT extern inline
#   else
#       define AAX _AFXIMPORT extern 
#       define AAXINL _AFXIMPORT extern inline
#   endif
#else
#   ifdef _DEBUG
#       define _AAX _AFXEXPORT
#       define AAX _AFXEXPORT extern 
#       define _AAXINL _AFXEXPORT inline
#       define AAXINL _AFXEXPORT extern inline
#   else
#       define _AAX _AFXEXPORT
#       define AAX _AFXEXPORT extern 
#       define _AAXINL _AFXEXPORT inline
#       define AAXINL _AFXEXPORT extern inline
#   endif
#endif//__e2sound__

//AFX_DEFINE_HANDLE(afxSoundSystem);
AFX_DEFINE_HANDLE(afxSoundThread);
AFX_DEFINE_HANDLE(afxSoundDevice);
AFX_DEFINE_HANDLE(afxSoundContext);
AFX_DEFINE_HANDLE(afxSoundOutput);
AFX_DEFINE_HANDLE(afxSoundInput);
AFX_DEFINE_HANDLE(afxSoundQueue);
AFX_DEFINE_HANDLE(aaxBank);
AFX_DEFINE_HANDLE(aaxTracker);
AFX_DEFINE_HANDLE(afxSoundBuffer);
AFX_DEFINE_HANDLE(afxSound);

#ifdef _AFX_SOUND_C
#ifdef _AFX_SOUND_C
#ifndef _AFX_SOUND_IMPL
AFX_OBJECT(afxSound)
#else
struct _afxBaseSound
#endif
{

};
#endif//_AFX_SOUND_C
#endif//_AFX_SOUND_C

typedef enum aaxCurveInterpolation // curve interpolation types
{
    // curves from 0 to LastFadeCurve NEED TO BE A MIRROR IMAGE AROUND LINEAR (ex. Log3 is the inverse of Exp3)
    aaxCurveInterpolation_Log3 = 0, // log3
    aaxCurveInterpolation_Sine = 1, // sine
    aaxCurveInterpolation_Log1 = 2, // log1
    aaxCurveInterpolation_InvSCurve = 3, // inversed S curve
    aaxCurveInterpolation_Linear = 4, // linear (default)
    aaxCurveInterpolation_SCurve = 5, // S curve
    aaxCurveInterpolation_Exp1 = 6, // exp1
    aaxCurveInterpolation_SineRecip = 7, // reciprocal of sine curve
    aaxCurveInterpolation_Exp3 = 8, // exp3
    aaxCurveInterpolation_LastFadeCurve = 8, // update this value to reflect last curve available for fades
    aaxCurveInterpolation_Constant = 9  // constant ( not valid for fading values )
} aaxCurveInterpolation;

AFX_DEFINE_STRUCT(aaxListener) // Listener information.
{
    afxTransform        pos;
    afxReal32           scalingFactor;
    afxBool             spatialized;
};

AAXINL void AaxAcquireListener(aaxListener *list)
{
    afxError err = NIL;
    AfxAssert(list);
    list->scalingFactor = (afxReal32)1;
    list->spatialized = TRUE;
}

AFX_DEFINE_STRUCT(aaxRamp) // volume ramp specified by end points "previous" and "next".
{
    afxReal32 prev;
    afxReal32 next;
};

AAXINL void AaxTakeRamp(aaxRamp *ramp, afxReal prev, afxReal next)
{
    afxError err = NIL;
    AfxAssert(ramp);
    ramp->prev = (afxReal32)prev;
    ramp->next = (afxReal32)next;
}

AAXINL void AaxResetRamp(aaxRamp *ramp)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AaxTakeRamp(ramp, 1, 1);
}

AAXINL void AaxCopyRamp(aaxRamp *ramp, aaxRamp const* in)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(in);
    ramp->prev = in->prev;
    ramp->next = in->next;
}

AAXINL void AaxScaleRamp(aaxRamp *ramp, aaxRamp const *rhs)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(rhs);
    ramp->prev *= rhs->prev;
    ramp->next *= rhs->next;
}

AAXINL aaxRamp AaxMergeRamps(aaxRamp const* lhs, aaxRamp const* rhs)
{
    aaxRamp rslt;
    AaxCopyRamp(&rslt, lhs);
    AaxScaleRamp(&rslt, rhs);
    return rslt;
}

AFX_DEFINE_STRUCT(aaxChannelConfig)
{
    afxNat32 uNumChannels : 8;   // the number of channels, identified (deduced from channel mask) or anonymous (set directly). 
    afxNat32 eConfigType : 4;    // a code that completes the identification of channels by uChannelMask.
    afxNat32 uChannelMask : 20; // a bit field, whose channel identifiers depend on aaxChannelConfigType (up to 20). Channel bits are defined in Speaker Config.
};

#endif//AFX_SOUND_DEFS_H
