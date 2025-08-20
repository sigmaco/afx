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

#ifndef AMX_VOICE_H
#define AMX_VOICE_H

// Voice
// each of the notes or sounds able to be produced simultaneously by a musical instrument (especially an electronic one) or a computer.
// (in an electronic musical instrument) each of a number of preset or programmable tones.

#include "qwadro/inc/mix/afxMixDefs.h"

typedef enum amxVoiceCap
{
    amxVoiceCap_REVERB,
    amxVoiceCap_PHASE,
    amxVoiceCap_GAIN,
    amxVoiceCap_ANALOG,
    amxVoiceCap_FLANGE,
    amxVoiceCap_COMPRESS,

} amxVoiceCaps;

////////////////////////////////////////////////////////////////////////////////

AMX afxError AmxDeallocateVoices(afxMixContext mix, afxUnit firstId, afxUnit cnt, afxUnit voiceIdMap[]);
AMX afxError AmxAllocateVoices(afxMixContext mix, amxVoiceCaps caps, afxUnit cnt, afxUnit voiceIds[]);

#endif//AMX_VOICE_H
