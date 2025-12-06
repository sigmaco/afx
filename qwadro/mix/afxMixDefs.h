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

/*
    Media synthesis refers to the use of advanced technologies, including artificial intelligence (AI),
    to generate or manipulate media content such as images, videos, audio, or text. It involves creating 
    synthetic (i.e., artificially produced) media that can appear realistic or entirely fictional.

    Examples of Media Synthesis:
     - Text generation
     - Image synthesis
     - Audio synthesis
       - Text-to-speech systems producing realistic human voices.
       - Voice cloning, where a person’s voice is mimicked.
     - Video synthesis:
*/

#ifndef AMX_MIX_DEFS_H
#define AMX_MIX_DEFS_H

#include "qwadro/exec/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"

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
AFX_DEFINE_HANDLE(amxFence);
AFX_DEFINE_HANDLE(amxVoice);
AFX_DEFINE_HANDLE(amxBank);
AFX_DEFINE_HANDLE(amxTrack);
AFX_DEFINE_HANDLE(amxTracker);
AFX_DEFINE_HANDLE(afxSubmix);
AFX_DEFINE_HANDLE(afxBroker);
AFX_DEFINE_HANDLE(afxMixContext);
AFX_DEFINE_HANDLE(afxWaveform);
AFX_DEFINE_HANDLE(amxAudio);
AFX_DEFINE_HANDLE(amxSound);
AFX_DEFINE_HANDLE(amxSoundscape);

AFX_DEFINE_HANDLE(amxBuffer);

AFX_DEFINE_HANDLE(amxVideo);
AFX_DEFINE_HANDLE(amxVideoDecoder);
AFX_DEFINE_HANDLE(afxVideoEncoder);
AFX_DEFINE_HANDLE(amxTracker);
AFX_DEFINE_HANDLE(amxSource);
AFX_DEFINE_HANDLE(amxCodec);
AFX_DEFINE_HANDLE(amxEncoder);
AFX_DEFINE_HANDLE(amxDecoder);

typedef enum amxCurveInterpolation
// curve interpolation types
{
    // curves from 0 to LastFadeCurve NEED TO BE A MIRROR IMAGE AROUND LINEAR (ex. Log3 is the inverse of Exp3)
    // log3
    amxWaveInterpolation_LOG3 = 0,
    // sine
    amxWaveInterpolation_SINE = 1,
    // log1
    amxWaveInterpolation_LOG1 = 2,
    // inversed S curve
    amxWaveInterpolation_INV_S_CURVE = 3,
    // linear (default)
    amxWaveeInterpolation_LINEAR = 4,
    // S curve
    amxWaveInterpolation_SCURVE = 5,
    // exp1
    amxWaveInterpolation_EXP1 = 6,
    // reciprocal of sine curve
    amxWaveInterpolation_SINE_RECIP = 7,
    // exp3
    amxWaveInterpolation_EXP3 = 8,
    // update this value to reflect last curve available for fades
    amxWaveInterpolation_LAST_FADE_CURVE = 8,
    // constant ( not valid for fading values )
    amxWaveInterpolation_CONST = 9
} amxWaveInterpolation;

#endif//AMX_MIX_DEFS_H
