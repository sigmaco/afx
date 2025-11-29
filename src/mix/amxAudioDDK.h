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

#ifndef AMX_AUDIO_DDK_H
#define AMX_AUDIO_DDK_H

#include "amxBufferDDK.h"

AFX_DEFINE_STRUCT(afxTrackedNote)
{
    amxAudio    aud;
    afxUnit     srcId;
    afxUnit     iterCnt;
    afxUnit     sampleOffset;
    afxUnit     sampleCnt;
    afxUnit     frameIdx;

    afxUnit     playBegin; // sample index
    afxUnit     playLen; // sample count
    afxUnit     loopBegin; // sample index
    afxUnit     loopLen; // sample count
    afxUnit     loopCnt;
};

#ifdef _AMX_AUDIO_C
#ifdef _AMX_WAVEFORM_IMPL
AFX_OBJECT(_amxAudio)
#else
AFX_OBJECT(amxAudio)
#endif
{
    amxFormat       fmt; // type and bits per sample
    afxUnit         sampCnt; // Number of samples (e.g., time slots)
    // its length, meaning the number of sample frames inside the buffer.
    afxUnit         chanCnt; // Number of channels (e.g., stereo = 2)
    afxUnit         segCnt; // Number of frames (playable intervals)
    afxUnit         freq; // the sample rate, the number of sample frames played per second.

    afxUnit         fmtBps;
    afxUnit         fmtStride;
    
    afxUnit64 current_sample_time;

    afxString       tag;
    void*           udd;
    
    amxBufferFlags  bufFlags;
    afxUnit         bufCap; // with any alignment
    amxBuffer       buf;
};
#endif

#endif//AMX_AUDIO_DDK_H
