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

// Mixer
// A mixer is a device for merging input signals to produce a combined output in the form of sound.

#ifndef ASX_MIXER_H
#define ASX_MIXER_H

#include "qwadro/inc/sound/io/afxAudio.h"

typedef enum asxLoadOp
// An enumerated value indicating the store operation to perform on sink channel after executing the mix scope.
{
    // Loads the existing value for this sink channel into the mix scope.
    // The previous contents of the audio within the interval will be preserved as the initial values.
    asxLoadOp_LOAD,

    // Loads the clearValue for this sink channel into the mix scope.
    // The contents within the interval will be cleared to a uniform value, which is specified when a mix scope is begun
    asxLoadOp_CLEAR,

    // The previous contents within the interval need not be preserved; the contents of the sink channel will be undefined inside the interval.
    asxLoadOp_DONT_CARE
} asxLoadOp;

typedef enum asxStoreOp
// An enumerated value indicating the load operation to perform on sink channel prior to executing the mix scope.
{
    // Stores the resulting value of the mix scope for this sink channel.
    // The contents generated during the mix scope and within the interval are written to memory.
    asxStoreOp_STORE,

    // Merges the resulting value of the mix scope with the existing value for this sink channel.
    asxStoreOp_MERGE,

    // Discards the resulting value of the mix scope for this sink channel.
    // The contents within the interval are not accessed by the store operation as long as no values are written to the sink channel during the mix scope.
    asxStoreOp_DISCARD,

    // The contents within the interval are not needed after mixing, and may be discarded; the contents of the sink channel will be undefined inside the mix scope.
    asxStoreOp_DONT_CARE,
} asxStoreOp;

typedef enum
// Sample output mixing factors.
// As0 represent the first source sample, for the sample output location corresponding to the voice being mixed.
// As1 represent the second source sample, used in dual source mixing modes, 
// for the sample output location corresponding to the voice being mixed.
// Ad represent the sample of the destination. That is, the sample currently in the corresponding voice for this sample.
// Ac represent the mix constant samples, respectively.
{
    // 0
    asxMixFactor_ZERO,

    // 1
    asxMixFactor_ONE,

    // As0
    asxMixFactor_SRC,

    // 1-As0
    asxMixFactor_ONE_MINUS_SRC,

    // Ad
    asxMixFactor_DST,

    // 1-Ad
    asxMixFactor_ONE_MINUS_DST,

    // Ac
    asxMixFactor_CONST,

    // 1-Ac
    asxMixFactor_ONE_MINUS_CONST,

    // 1
    asxMixFactor_SRC_SATURATE,

    // As1
    asxMixFactor_SRC1,

    // 1-As1
    asxMixFactor_ONE_MINUS_SRC1,

    asxMixFactor_TOTAL
} asxMixFactor;

typedef enum
// Sample output mixing operations.
// Once the source and destination mix factors have been selected, 
// they along with the source and destination samples are passed to the mixing operations.
// Samples can use different operations.
{
    // A = As0 × Sa + Ad × Da
    asxMixOp_ADD,

    // A = As0 × Sa - Ad × Da
    asxMixOp_SUB,

    // A = Ad × Da - As0 × Sa
    asxMixOp_REV_SUB,

    // A = min(As0, Ad)
    asxMixOp_MIN,

    // A = max(As0, Ad)
    asxMixOp_MAX
} asxMixOp;

/*
    A mix sink is a term used in audio processing, particularly in the context of sound systems. It generally refers to 
    a destination or output point where multiple audio signals (or channels) are combined, or "mixed," before being sent 
    to their final output (such as speakers, a recording device, or a streaming service).

    In simple terms, a mix sink acts as the final point where all audio channels are summed or processed together.
    It's a conceptual or functional destination in a signal chain.

    In the context of audio mixing, a mix sink is typically a virtual or physical destination where mixed audio signals
    are sent for playback or processing. For example, when you mix multiple tracks (each with its own audio signal),
    you combine them into a final stereo or multichannel mix, which is then sent to a mix sink.
    This could be the final stereo output for playback through speakers or headphones.

    In sound systems, a mix sink might refer to an endpoint in the system where mixed signals (from multiple audio
    sources or devices) are fed before being sent to amplifiers or speakers. It's the final point in the signal
    chain where audio mixing happens.
*/

AFX_DEFINE_STRUCT(asxMixSink)
{
    asxLoadOp       loadOp;
    afxReal         clearAmpl;
    asxStoreOp      storeOp;
    afxReal         mergeConstant;
    asxMixFactor    srcFactor;
    asxMixFactor    dstFactor;
    asxMixOp        mergeOp;
};

/*
    A mix scope refers to the range or context in which audio mixing occurs. It defines the boundaries of 
    the mixing process, including which audio signals are involved in the mix, how they are processed, and 
    where the mixed output is routed. Essentially, it determines the scope or extent of the mixing operation.

    In hardware audio mixers, the term mix scope can refer to the number of channels or signal paths being processed.
    For example, the mix scope might be the full set of inputs (microphones, instruments, etc.) that are being combined for output.
*/

AFX_DEFINE_STRUCT(asxMixScope)
{
    afxFlags            flags;
    //afxAudio            sink;
    afxSink             sink;
    afxUnit             freq;
    afxUnit             baseFrame;
    afxUnit             frameCnt;
    afxUnit             baseChan;
    afxUnit             chanCnt;
    asxMixSink const*   chans;
    afxUnit             baseSamp;
    afxUnit             sampCnt;
};

typedef enum asxMixState
{
    asxMixState_INITIAL,
    asxMixState_RECORDING,
    asxMixState_EXECUTABLE,
    asxMixState_PENDING,
    asxMixState_INVALID
} asxMixState;

ASX afxUnit         AfxGetMixerPort(afxMixer mix);
ASX afxUnit         AfxGetMixerPool(afxMixer mix);
ASX asxMixState     AfxGetMixerState(afxMixer mix);

ASX afxCmdId        AsxCmdCommenceMixScope(afxMixer mix, asxMixScope const* scope);
ASX afxCmdId        AsxCmdConcludeMixScope(afxMixer mix);

ASX afxCmdId        AsxCmdReverb(afxMixer mix, afxReal wetMix, afxReal roomSiz, afxReal width, afxReal damp, afxReal dryMix);
ASX afxCmdId        AsxCmdPhaser(afxMixer mix, afxReal floor, afxReal ceil, afxReal rate, afxReal feedback, afxReal depth, afxReal phase, afxUnit stageCnt);
ASX afxCmdId        AsxCmdGainer(afxMixer mix, afxReal gain, afxReal pan, afxMask invChan);
ASX afxCmdId        AsxCmdFlanger(afxMixer mix, afxReal amount, afxReal rate, afxReal amplitude, afxReal feedback, afxReal delay, afxReal phase, afxUnit flt);
ASX afxCmdId        AsxCmdAnalog(afxMixer mix, afxUnit type, afxUnit flt, afxReal cutoff, afxReal resonance, afxReal inertia, afxReal drive, afxUnit oversamp, afxReal start, afxReal end);
ASX afxCmdId        AsxCmdCompressor(afxMixer mix, afxReal threshold, afxReal ratio, afxReal attack, afxReal release, afxReal makeup);

ASX afxCmdId        AsxCmdLineIn(afxMixer mix, afxUnit line, afxMask chanMask, afxUnit latency, afxReal pan, afxReal vol);
ASX afxCmdId        AsxCmdRemoteIn(afxMixer mix, afxUnit src, afxMask chanMask, afxReal pan, afxReal vol);

ASX afxCmdId        AsxCmdSend(afxMixer mix, afxBool muteSrc, afxReal amount, afxReal pan, afxUnit receiverSubmix);

ASX afxCmdId        AsxCmdResampleAudio(afxMixer mix, afxAudio src, afxAudio dst, afxWaveInterval const* srci, afxWaveInterval const* dsti);

ASX afxCmdId        AsxCmdFetchAudition(afxMixer mix, afxUnit headIdx);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AfxAcquireMixer(afxSoundSystem ssys, afxUnit exuIdx, afxUnit poolIdx, afxMixer* mixer);

#endif//ASX_MIXER_H
