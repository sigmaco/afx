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

#include "qwadro/inc/mix/afxMixDefs.h"

AFX_DECLARE_STRUCT(afxSinkInterface);

// one video frame is equivalent to one audio interval

typedef enum amxUsage { amxUsage_CAPTURE, amxUsage_MIX, amxUsage_RESAMPLE, amxUsage_STORAGE, amxUsage_SOUND } amxUsage;
typedef enum amxFlag { amxFlag_X } amxFlags;

AFX_DEFINE_STRUCT(amxSinkPin)
{
    amxFormat   fmt; // amxFormat_A32f
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
    amxFormat   fmt; // amxFormat_A32f
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
#ifdef AFX_OS_WIN
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
AMX afxMixSystem    AfxGetAudioSinkContext(afxSink sink);

AMX afxError        AfxGetAudioSinkIdd(afxSink sink, afxUnit code, void* dst);

// Connection

AMX afxError        AfxMuteAudioSink(afxSink sink, afxBool mute);

AMX afxError        AfxRequestSinkBuffer(afxSink sink, afxTime timeout, afxUnit *bufIdx);
AMX afxError        AfxDiscardSinkBuffer(afxSink sink, afxUnit bufIdx);

////////////////////////////////////////////////////////////////////////////////

AMX afxError        AfxConfigureAudioSink(afxMixSystem msys, afxSinkConfig* cfg);

AMX afxError        AfxOpenAudioSink(afxMixSystem msys, afxSinkConfig const* cfg, afxSink* sink);

#endif//AMX_SINK_H
