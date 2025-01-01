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

#ifndef ASX_SOUND_DEFS_H
#define ASX_SOUND_DEFS_H

#include "qwadro/inc/exec/afxSystem.h"
#include "qwadro/inc/sim/afxSimulation.h"

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
AFX_DEFINE_HANDLE(afxSoundSystem);
AFX_DEFINE_HANDLE(afxSoundOutput);
AFX_DEFINE_HANDLE(afxSink);
AFX_DEFINE_HANDLE(afxSoundInput);
AFX_DEFINE_HANDLE(afxSoundBridge);
AFX_DEFINE_HANDLE(afxSoundQueue);
AFX_DEFINE_HANDLE(asxBank);
AFX_DEFINE_HANDLE(asxTracker);
AFX_DEFINE_HANDLE(afxSubmix);
AFX_DEFINE_HANDLE(afxBroker);
AFX_DEFINE_HANDLE(afxMixer);
AFX_DEFINE_HANDLE(afxWaveform);
AFX_DEFINE_HANDLE(afxAudio);
AFX_DEFINE_HANDLE(afxSound);
AFX_DEFINE_HANDLE(afxAudient);

typedef enum asxFormat
// Sound sample formats.
{
    asxFormat_A32f = 1, // 32-bit float amplitude sample [-1.f, 1.f]
    asxFormat_S32f, // 32-bit float interleaved amplutide block [-1.f, 1.f]
    asxFormat_A32i, // 32-bit signed int amplitude sample [-2147483648, 2147483647]
    asxFormat_S32i, // 32-bit signed int interleaved amplutide block [-2147483648, 2147483647]
    asxFormat_A24i, // 24-bit signed int amplitude sample [-8388608, 8388607]
    asxFormat_S24i, // 24-bit signed int interleaved amplutide block [-8388608, 8388607]
    asxFormat_A16i, // 16-bit signed int amplitude sample [-32768, 32767]
    asxFormat_S16i, // 16-bit signed int interleaved amplutide block [-32768, 32767]
    asxFormat_A8i,  // 8-bit amplitude sample [0, 255]
    asxFormat_S8i  // 8-bit interleaved amplutide block [0, 255]
} asxFormat;

typedef enum asxCurveInterpolation
// curve interpolation types
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

#endif//ASX_SOUND_DEFS_H
