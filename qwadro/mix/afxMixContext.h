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

typedef enum amxCmdFlag
{
    // Commands will be submitted once only and then automatically invalidated.
    amxCmdFlag_ONCE = AFX_BITMASK(0),
    // Commands will be side-loaded (inlined) by a front context.
    amxCmdFlag_DEFERRED = AFX_BITMASK(1),
    // Commands are considered entirely inside a mixing scope (to be used by a front context).
    amxCmdFlag_SCOPED = AFX_BITMASK(2),
    // Commands will be shared across more than one MPU concurrently.
    amxCmdFlag_SHARED = AFX_BITMASK(3)
} amxCmdFlags;

typedef enum amxContextFlag
{
    amxContextFlag_TRANSIENT = AFX_BITMASK(0),
    amxContextFlag_MONOLITHIC = AFX_BITMASK(1)
} amxContextFlags;

AFX_DEFINE_STRUCT(amxContextConfig)
{
    amxAptitude     caps;
    afxMask         exuMask;
    amxContextFlags flags;
    amxCmdFlags     cmdFlags;
    afxUnit         auxCnt;
    // The capacity of recycle queue.
    afxUnit         recycCap;
    void*           udd;
    afxString       tag;
};

AMX afxError AmxAcquireMixContexts
(
    afxMixSystem msys,
    afxMixContext pool,
    amxContextConfig const* info,
    afxUnit cnt,
    afxMixContext contexts[]
);

AMX afxError AmxRecycleMixContexts
(
    afxBool freeRes, 
    afxUnit cnt, 
    afxMixContext contexts[]
);

AMX afxError AmxExecuteMixCommands
(
    afxMixSystem msys,
    afxUnit cnt, 
    amxSubmission const subms[]
);

////////////////////////////////////////////////////////////////////////////////

AMX afxMask AmxGetCommandPort
(
    afxMixContext mix
);

AMX afxMixContext AmxGetCommandPool
(
    afxMixContext mix
);

AMX afxError AmxExhaustMixContext
(
    afxMixContext mctx,
    afxBool freeMem
);

AMX afxError AmxPrepareMixCommands
(
    // The mix context which the batch will be allocated from.
    afxMixContext mctx,
    afxBool purge,
    amxCmdFlags flags
);

AMX afxError AmxCompileMixCommands
(
    // The mix context recording commands.
    afxMixContext mctx
);

#endif//AMX_MIX_CONTEXT_H
