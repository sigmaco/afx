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

#define AMX_BASE_VIDEO_FORMAT 100

typedef enum amxFormat
// Sound sample formats.
{
    amxFormat_M32f = 1, // 32-bit float amplitude sample [-1.f, 1.f]
    amxFormat_S32f, // 32-bit float interleaved amplutide block [-1.f, 1.f]
    amxFormat_M32i, // 32-bit signed int amplitude sample [-2147483648, 2147483647]
    amxFormat_S32i, // 32-bit signed int interleaved amplutide block [-2147483648, 2147483647]
    amxFormat_M24i, // 24-bit signed int amplitude sample [-8388608, 8388607]
    amxFormat_S24i, // 24-bit signed int interleaved amplutide block [-8388608, 8388607]
    amxFormat_M16i, // 16-bit signed int amplitude sample [-32768, 32767]
    amxFormat_S16i, // 16-bit signed int interleaved amplutide block [-32768, 32767]
    amxFormat_M8i,  // 8-bit amplitude sample [0, 255]
    amxFormat_S8i,  // 8-bit interleaved amplutide block [0, 255]

    // Qwadro video signal formats and its correspondence in Vulkan.
    amxFormat_YVYU8_422 = AMX_BASE_VIDEO_FORMAT, // G8B8G8R8_422
    amxFormat_VYUY8_422, // B8G8R8G8_422
    amxFormat_YVU8_420P3, // G8_B8_R8_3PLANE_420
    amxFormat_YVU8_420P2, // G8_B8R8_2PLANE_420
    amxFormat_YVU8_422P3, // G8_B8_R8_3PLANE_422
    amxFormat_YVU8_422P2, // G8_B8R8_2PLANE_422
    amxFormat_YVU8_444P3, // G8_B8_R8_3PLANE_444
    amxFormat_Y10X6, // R10X6_PACK16
    amxFormat_UY10X6, // R10X6G10X6_2PACK16
    amxFormat_UYVA10X6, // R10X6G10X6B10X6A10X6_4PACK16
    amxFormat_YVYU10X6_422, // G10X6B10X6G10X6R10X6_422_4PACK16
    amxFormat_VYUY10X6_422, // B10X6G10X6R10X6G10X6_422_4PACK16
    amxFormat_YVU10X6_420P3, // G10X6_B10X6_R10X6_3PLANE_420_3PACK16
    amxFormat_YVU10X6_420P2, // G10X6_B10X6R10X6_2PLANE_420_3PACK16
    amxFormat_YVU10X6_422P3, // G10X6_B10X6_R10X6_3PLANE_422_3PACK16
    amxFormat_YVU10X6_422P2, // G10X6_B10X6R10X6_2PLANE_422_3PACK16
    amxFormat_YVU10X6_444P3, // G10X6_B10X6_R10X6_3PLANE_444_3PACK16
    amxFormat_Y12X4, // R12X4_PACK16
    amxFormat_UY12X4, // R12X4G12X4_2PACK16
    amxFormat_UYVA12X4, // R12X4G12X4B12X4A12X4_4PACK16
    amxFormat_YVYU12X4_422, // G12X4B12X4G12X4R12X4_422_4PACK16
    amxFormat_VYUY12X4_422, // B12X4G12X4R12X4G12X4_422_4PACK16
    amxFormat_YVU12X4_420P3, // G12X4_B12X4_R12X4_3PLANE_420_3PACK16
    amxFormat_YVU12X4_420P2, // G12X4_B12X4R12X4_2PLANE_420_3PACK16
    amxFormat_YVU12X4_422P3, // G12X4_B12X4_R12X4_3PLANE_422_3PACK16
    amxFormat_YVU12X4_422P2, // G12X4_B12X4R12X4_2PLANE_422_3PACK16
    amxFormat_YVU12X4_444P3, // G12X4_B12X4_R12X4_3PLANE_444_3PACK16
    amxFormat_YVYU16_422, // G16B16G16R16_422
    amxFormat_VYUY16_422, // B16G16R16G16_422
    amxFormat_YVU16_420P3, // G16_B16_R16_3PLANE_420
    amxFormat_YVU16_420P2, // G16_B16R16_2PLANE_420
    amxFormat_YVU16_422P3, // G16_B16_R16_3PLANE_422
    amxFormat_YVU16_422P2, // G16_B16R16_2PLANE_422
    amxFormat_YVU16_444P3, // G16_B16_R16_3PLANE_444
    amxFormat_YVU8_444P2, // G8_B8R8_2PLANE_444
    amxFormat_YVU10X6_444P2, // G10X6_B10X6R10X6_2PLANE_444_3PACK16
    amxFormat_YVU12X4_444P2, // G12X4_B12X4R12X4_2PLANE_444_3PACK16
    amxFormat_YVU16_444P2, // G16_B16R16_2PLANE_444

} amxFormat;

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
