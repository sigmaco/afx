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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ASX_SOUND_OUTPUT_H
#define ASX_SOUND_OUTPUT_H

#include "qwadro/inc/sound/afxSoundDefs.h"

AFX_OBJECT(asxTrack)
{
    int a;
};

AFX_OBJECT(asxMixer)
{
    struct
    {
        afxUnit  a;
    }           ins[8];
    afxUnit      inCnt;

    struct
    {
        afxReal factor;
        afxUnit  fromIn;
    }           outs[8];
    afxUnit      outCnt;
};

AFX_DEFINE_STRUCT(afxSoundOutputConfig)
{
    afxUri endpoint;
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

ASX afxSoundDevice  AfxGetSoundOutputDevice(afxSoundOutput sout);
ASX afxError        AfxGetSoundOutputIdd(afxSoundOutput sout, afxUnit code, void* dst);

// Connection

ASX afxError        AfxDisconnectSoundOutput(afxSoundOutput sout);
ASX afxBool         AfxReconnectSoundOutput(afxSoundOutput sout, afxSoundContext sctx);
ASX afxBool         AfxGetSoundOutputContext(afxSoundOutput sout, afxSoundContext* context);

#endif//ASX_SOUND_OUTPUT_H
