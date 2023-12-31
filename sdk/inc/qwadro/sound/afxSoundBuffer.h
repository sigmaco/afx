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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SOUND_BUFFER_H
#define AFX_SOUND_BUFFER_H

#include "qwadro/sound/afxSoundDefs.h"

/// PCM
/// The Pulse-Code Modulation file is a digital representation of analogue signals. 
/// It is an uncompressed and lossless audio format and serves as a standard file format for CDs and DVDs. 
/// PCM files work by converting waveforms into digital bits, they do this by sampling analogue sounds and recording them at certain intervals or pulses. 
/// This means PCM files have a sampling rate and bit depth, where sampling rate refers to how often a sound sample is made and bit depth is the number of bits used to represent each sample.


typedef enum afxSoundFormat
{
    afxSoundFormat_M8,
    afxSoundFormat_M16,
    afxSoundFormat_S8,
    afxSoundFormat_S16
} afxSoundFormat;

#ifdef _AFX_SOUND_C
#ifdef _AFX_SOUND_BUFFER_C
#ifndef _AFX_SOUND_BUFFER_IMPL
AFX_OBJECT(afxSoundBuffer)
#else
struct _afxBaseSoundBuffer
#endif
{
    afxSize             siz;
    afxSoundFormat      fmt;
    afxNat              freq;
    afxByte*            bytemap;
};
#endif//_AFX_SOUND_BUFFER_C
#endif//_AFX_SOUND_C





#endif//AFX_SOUND_BUFFER_H
