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

#ifndef AMX_MIX_CONTEXT_H
#define AMX_MIX_CONTEXT_H

// Mixer
// A mixer is a device for merging input signals to produce a combined output in the form of sound.

/*
    Audio Interface
    Audio interface is basically a contraption that helps musicians and audio engineers utilize various programs and plug-ins, 
    allowing for a substantially more 'digital' approach. Most audio interfaces come supplied with various built-in features 
    that can drastically change how the recording process functions and how the end result turns out to be.
*/

/*
    Channel
    The term 'channel' in the world of audio engineering means pretty much the same as in its traditional form. 
    A channel is a single line of controls bound to a single input on the mixer (mixing console).
*/

/*
    Mixer
    A mixer is a contraption that is used to merge several signals together. 
    Bigger models are typically called 'consoles' and 'desks'.
*/

// Voices could be named decks for a broader range.

#include "qwadro/mix/amxAudio.h"
#include "qwadro/mix/amxVoice.h"

AFX_DEFINE_STRUCT(afxMixConfig)
{
    afxFlags    flags;
    afxString   tag;
    void*       udd;
};

AMX afxError AmxAcquireMixContext
(
    afxMixSystem msys,
    afxMixConfig const* mcfg,
    afxMixContext* context
);

AMX afxError AmxExecuteMixCommands
(
    afxMixSystem msys,
    afxUnit cnt, 
    amxSubmission const subms[]
);

////////////////////////////////////////////////////////////////////////////////

AMX afxUnit AmxGetCommandPort
(
    afxMixContext mix
);

AMX afxUnit AmxGetCommandPool
(
    afxMixContext mix
);

AMX afxError AmxExhaustMixContext
(
    afxMixContext mctx,
    afxBool freeMem
);

AMX afxError AmxRecordMixCommands
(
    // The mix context which the batch will be allocated from.
    afxMixContext mctx,
    // A flag specifying a one-time submission batch.
    afxBool once,
    // A flag specifying a inlineable batch.
    afxBool deferred
);

AMX afxError AmxDiscardMixCommands
(
    afxMixContext mctx,
    afxBool freeRes
);

AMX afxError AmxCompileMixCommands
(
    // The mix context recording commands.
    afxMixContext mctx
);

AMX afxError AmxRecycleMixCommands
(
    // The mix context that holds the commands.
    afxMixContext mctx,
    // A flag that indicates whether all or most of the resources owned by the batch should be reclaimed by the system.
    afxBool freeRes
);

#endif//AMX_MIX_CONTEXT_H
