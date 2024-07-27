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

#include "qwadro/sound/afxSoundDefs.h"

typedef enum asxFormat
{
    asxFormat_M8,
    asxFormat_S8,
    asxFormat_M16,
    asxFormat_S16,
    asxFormat_M24,
    asxFormat_S24,
    asxFormat_M32,
    asxFormat_S32,
    asxFormat_M32F,
    asxFormat_S32F
} asxFormat;

AFX_OBJECT(asxTrack)
{
    int a;
};

AFX_OBJECT(asxMixer)
{
    struct
    {
        afxNat  a;
    }           ins[8];
    afxNat      inCnt;

    struct
    {
        afxReal factor;
        afxNat  fromIn;
    }           outs[8];
    afxNat      outCnt;
};

AFX_DEFINE_STRUCT(afxSoundOutputConfig)
{
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
ASX afxError        AfxGetSoundOutputIdd(afxSoundOutput sout, afxNat code, void* dst);

// Connection

ASX afxError        AfxDisconnectSoundOutput(afxSoundOutput sout);
ASX afxBool         AfxReconnectSoundOutput(afxSoundOutput sout, afxSoundContext sctx);
ASX afxBool         AfxGetSoundOutputContext(afxSoundOutput sout, afxSoundContext* context);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AfxAcquireSoundOutput(afxNat sdevId, afxSoundOutputConfig const* cfg, afxSoundOutput* output);

ASX afxError        AfxOpenSoundOutput(afxNat sdevId, afxUri const* endpoint, afxSoundOutputConfig const* cfg, afxSoundOutput* output);

#endif//ASX_SOUND_OUTPUT_H
