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
    Audio Interface
    Audio interface is basically a contraption that helps musicians and audio engineers utilize various programs and plug-ins, 
    allowing for a substantially more ‘digital’ approach. Most audio interfaces come supplied with various built-in features 
    that can drastically change how the recording process functions and how the end result turns out to be.
*/

/*
    Channel
    The term ‘channel’ in the world of audio engineering means pretty much the same as in its traditional form. 
    A channel is a single line of controls bound to a single input on the mixer (mixing console).
*/

/*
    Mixer
    A mixer is a contraption that is used to merge several signals together. 
    Bigger models are typically called ‘consoles’ and ‘desks.’
*/

#ifndef AMX_SINK_H
#define AMX_SINK_H

#include "qwadro/mix/afxMixDefs.h"
#include "qwadro/mix/amxBuffer.h"

AFX_DECLARE_STRUCT(afxSinkInterface);

// one video frame is equivalent to one audio interval

typedef enum amxUsage { amxUsage_CAPTURE, amxUsage_MIX, amxUsage_RESAMPLE, amxUsage_STORAGE, amxUsage_SOUND } amxUsage;
typedef enum amxFlag { amxFlag_X } amxFlags;

typedef enum amxSinkUsage
{
    amxSinkUsage_AUDIO_OUT,
    amxSinkUsage_AUDIO_IN,
    amxSinkUsage_VIDEO_OUT,
    amxSinkUsage_VIDEO_IN,
    
    amxSinkUsage_PHONE,
    amxSinkUsage_MICROPHONE,
    amxSinkUsage_CAMERA,
} amxSinkUsage;

typedef enum amxSinkFlag
{
    amxSinkFlag_DEFAULT, // marked as default in host system.
    amxSinkFlag_HEADSET, // ex.: head-mounted speakers and/or microphone
    amxSinkFlag_HANDSET, // ex.: combined speaker and/or microphone handheld.
} amxSinkFlags;

afxError AmxOpenSink(afxMixSystem msys, amxSinkUsage usage);

AFX_DEFINE_STRUCT(amxSinkPin)
{
    amxFormat   fmt; // amxFormat_M32f
    amxUsage    bufUsage;
    amxFlags    bufFlags;
    afxUnit     chanCnt; // 2 --- stereo
    afxUnit     freq; // 48000 --- 48kHz
    afxUnit     samplesPerFrame; // (freq / 60)
    afxUnit     latency; // 3
};

AFX_DEFINE_STRUCT(amxSinkConfig)
{
    afxUnit     pinCnt;
    amxSinkPin  pins[32];
};

AFX_DEFINE_STRUCT(afxSinkConfig)
{
    afxUnit     vaioId;
    afxUri      endpoint;
    amxFormat   fmt; // amxFormat_M32f
    afxUnit     chanCnt; // 2 --- stereo
    afxUnit     freq; // 48000 --- 48kHz
    afxUnit     samplesPerFrame; // (freq / 60)
    afxUnit     latency; // 3
#if 0
    // bass
    int device;
    DWORD freq;
    DWORD flags;
    HWND win;

    // miles quick
    S32 use_digital;
    S32 use_MIDI;
    U32 output_rate;
    S32 output_bits;
    S32 output_channels;

    // miles dig
    U32 frequency;
    S32 bits;
    S32 channels;
    U32 flags;

    // wwise out
    uint devSet;
    uint devId;
    panning rule; // angle for spekear or headphone
    uint numOfChannels;
    uint confType; // anom, std, ambisonic, object-based, main dev, passthrough
    // wwise init
    uint uNumSamplesPerFrame;		///< Number of samples per audio frame (256, 512, 1024, or 2048).
#endif
    afxSinkInterface*ctrl;
    void*           udd;
    union
    {
#ifdef AFX_ON_WINDOWS
        struct
        {
            void*   hInstance; // HINSTANCE
            void*   hWnd; // HWND
        }           w32;
#endif
    };
};

AFX_DEFINE_STRUCT(afxSinkInterface)
{
    afxError(*init)(afxSink, afxSinkConfig const*);
    afxError(*quit)(afxSink);
    afxError(*start)(afxSink);
    afxError(*pause)(afxSink);
    afxError(*stop)(afxSink);
    afxError(*req)(afxSink);
    afxError(*push)(afxSink, void const*, afxUnit);
    afxError(*pull)(afxSink, afxUnit, void*, afxUnit);
    afxError(*getPushRoom)(afxSink, afxUnit*);
    afxError(*getPullRoom)(afxSink, afxUnit*);
    afxError(*ioctl)(afxSink, afxUnit, ...);
};

AMX afxMixDevice    AfxGetAudioSinkDevice(afxSink sink);
AMX afxMixSystem    AmxGetSinkHost(afxSink sink);

AMX afxError        AmxGetSinkIdd(afxSink sink, afxUnit code, void* dst);

// Connection

AMX afxError        AfxMuteAudioSink(afxSink sink, afxBool mute);

AFX_DEFINE_STRUCT(amxBufferedTrack)
{
    amxBuffer   buf;
    afxSize     offset;
    afxUnit     range;
    //afxUnit     stride;
    afxUnit     frameCnt;
    afxUnit     freq;
};

AFX_DEFINE_STRUCT(amxBufferedAudio)
{
    amxBuffer   buf;
    afxSize     offset;
    afxUnit     range;
    amxFormat   fmt;
    afxUnit     sampRate;
    afxUnit     w, h, d;
};

AMX afxError        AmxGetSinkTrack
(
    afxSink sink, 
    amxAudio* track
);

AMX afxError        AmxLockSinkBuffer
(
    afxSink sink, 
    afxUnit64 timeout, 
    afxMask exuMask, 
    afxUnit minFrameCnt, 
    amxBufferedTrack* room
);

AMX afxError        AmxUnlockSinkBuffer
(
    afxSink sink, 
    afxFlags flags
);

////////////////////////////////////////////////////////////////////////////////

AMX afxError        AfxConfigureAudioSink(afxMixSystem msys, afxSinkConfig* cfg);

AMX afxError        AfxOpenAudioSink(afxMixSystem msys, afxSinkConfig const* cfg, afxSink* sink);

AFX_DEFINE_STRUCT(amxSink)
// Media sinks. Stream sinks handle the actual processing of data on each stream.
{
    int a;
};

// Sets the presentation clock on the media sink.
// During streaming, the media sink attempts to match rates with the presentation clock. Ideally, the media sink presents samples at the correct time according to the presentation clock and does not fall behind. Rateless media sinks are an exception to this rule, as they consume samples as quickly as possible and ignore the clock.
afxError AmxSetSinkClock(amxSink msnk, afxClock* clock);
// Shuts down the media sink and releases the resources it is using.
// AfxDisposeObjects(1, &msnk)

// Requests a sample from the media source.
afxError AmxRequestStreamSample(amxSink msnk);

#endif//AMX_SINK_H
