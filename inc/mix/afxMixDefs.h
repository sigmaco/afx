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

#ifndef AMX_MIX_DEFS_H
#define AMX_MIX_DEFS_H

#include "qwadro/inc/exec/afxSystem.h"
#include "qwadro/inc/sim/afxSimulation.h"

#ifndef __e2mix__
#   ifdef _DEBUG
#       define AMX DLLIMPORT extern 
#       define AMXINL DLLIMPORT EMBED
#   else
#       define AMX DLLIMPORT extern 
#       define AMXINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _AMX DLLEXPORT
#       define AMX DLLEXPORT extern 
#       define _AMXINL DLLEXPORT INLINE
#       define AMXINL DLLEXPORT EMBED
#   else
#       define _AMX DLLEXPORT
#       define AMX DLLEXPORT extern 
#       define _AMXINL DLLEXPORT INLINE
#       define AMXINL DLLEXPORT EMBED
#   endif
#endif//__e2mix__

//AFX_DEFINE_HANDLE(afxMixSystem);
AFX_DEFINE_HANDLE(afxMixDevice);
AFX_DEFINE_HANDLE(afxMixSystem);
AFX_DEFINE_HANDLE(afxMixOutput);
AFX_DEFINE_HANDLE(afxSink);
AFX_DEFINE_HANDLE(afxMixInput);
AFX_DEFINE_HANDLE(afxMixBridge);
AFX_DEFINE_HANDLE(afxMixQueue);
AFX_DEFINE_HANDLE(amxBank);
AFX_DEFINE_HANDLE(amxTracker);
AFX_DEFINE_HANDLE(afxSubmix);
AFX_DEFINE_HANDLE(afxBroker);
AFX_DEFINE_HANDLE(afxMixContext);
AFX_DEFINE_HANDLE(afxWaveform);
AFX_DEFINE_HANDLE(afxAudio);
AFX_DEFINE_HANDLE(afxSound);
AFX_DEFINE_HANDLE(afxAudient);

AFX_DEFINE_HANDLE(amxBuffer);

AFX_DEFINE_HANDLE(afxVideoDecoder);
AFX_DEFINE_HANDLE(afxVideoEncoder);
AFX_DEFINE_HANDLE(amxSession);
AFX_DEFINE_HANDLE(amxSource);
AFX_DEFINE_HANDLE(amxCodec);
AFX_DEFINE_HANDLE(amxEncoder);
AFX_DEFINE_HANDLE(amxDecoder);

typedef enum amxFormat
// Sound sample formats.
{
    amxFormat_A32f = 1, // 32-bit float amplitude sample [-1.f, 1.f]
    amxFormat_S32f, // 32-bit float interleaved amplutide block [-1.f, 1.f]
    amxFormat_A32i, // 32-bit signed int amplitude sample [-2147483648, 2147483647]
    amxFormat_S32i, // 32-bit signed int interleaved amplutide block [-2147483648, 2147483647]
    amxFormat_A24i, // 24-bit signed int amplitude sample [-8388608, 8388607]
    amxFormat_S24i, // 24-bit signed int interleaved amplutide block [-8388608, 8388607]
    amxFormat_A16i, // 16-bit signed int amplitude sample [-32768, 32767]
    amxFormat_S16i, // 16-bit signed int interleaved amplutide block [-32768, 32767]
    amxFormat_A8i,  // 8-bit amplitude sample [0, 255]
    amxFormat_S8i  // 8-bit interleaved amplutide block [0, 255]
} amxFormat;

typedef enum amxCurveInterpolation
// curve interpolation types
{
    // curves from 0 to LastFadeCurve NEED TO BE A MIRROR IMAGE AROUND LINEAR (ex. Log3 is the inverse of Exp3)
    amxCurveInterpolation_Log3 = 0, // log3
    amxCurveInterpolation_Sine = 1, // sine
    amxCurveInterpolation_Log1 = 2, // log1
    amxCurveInterpolation_InvSCurve = 3, // inversed S curve
    amxCurveInterpolation_Linear = 4, // linear (default)
    amxCurveInterpolation_SCurve = 5, // S curve
    amxCurveInterpolation_Exp1 = 6, // exp1
    amxCurveInterpolation_SineRecip = 7, // reciprocal of sine curve
    amxCurveInterpolation_Exp3 = 8, // exp3
    amxCurveInterpolation_LastFadeCurve = 8, // update this value to reflect last curve available for fades
    amxCurveInterpolation_Constant = 9  // constant ( not valid for fading values )
} amxCurveInterpolation;

#endif//AMX_MIX_DEFS_H
