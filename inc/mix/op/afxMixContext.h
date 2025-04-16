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

#ifndef AMX_MIX_CONTEXT_H
#define AMX_MIX_CONTEXT_H

#include "qwadro/inc/mix/io/amxAudio.h"

typedef enum amxLoadOp
// An enumerated value indicating the store operation to perform on sink channel after executing the mix scope.
{
    // Loads the existing value for this sink channel into the mix scope.
    // The previous contents of the audio within the interval will be preserved as the initial values.
    amxLoadOp_LOAD,

    // Loads the clearValue for this sink channel into the mix scope.
    // The contents within the interval will be cleared to a uniform value, which is specified when a mix scope is begun
    amxLoadOp_CLEAR,

    // The previous contents within the interval need not be preserved; the contents of the sink channel will be undefined inside the interval.
    amxLoadOp_DONT_CARE
} amxLoadOp;

typedef enum amxStoreOp
// An enumerated value indicating the load operation to perform on sink channel prior to executing the mix scope.
{
    // Stores the resulting value of the mix scope for this sink channel.
    // The contents generated during the mix scope and within the interval are written to memory.
    amxStoreOp_STORE,

    // Merges the resulting value of the mix scope with the existing value for this sink channel.
    amxStoreOp_MERGE,

    // Discards the resulting value of the mix scope for this sink channel.
    // The contents within the interval are not accessed by the store operation as long as no values are written to the sink channel during the mix scope.
    amxStoreOp_DISCARD,

    // The contents within the interval are not needed after mixing, and may be discarded; the contents of the sink channel will be undefined inside the mix scope.
    amxStoreOp_DONT_CARE,
} amxStoreOp;

typedef enum amxMixFactor
// Sample output mixing factors.
// As0 represent the first source sample, for the sample output location corresponding to the voice being mixed.
// As1 represent the second source sample, used in dual source mixing modes, 
// for the sample output location corresponding to the voice being mixed.
// Ad represent the sample of the destination. That is, the sample currently in the corresponding voice for this sample.
// Ac represent the mix constant samples, respectively.
{
    // 0
    amxMixFactor_ZERO,

    // 1
    amxMixFactor_ONE,

    // As0
    amxMixFactor_SRC,

    // 1-As0
    amxMixFactor_ONE_MINUS_SRC,

    // Ad
    amxMixFactor_DST,

    // 1-Ad
    amxMixFactor_ONE_MINUS_DST,

    // Ac
    amxMixFactor_CONST,

    // 1-Ac
    amxMixFactor_ONE_MINUS_CONST,

    // 1
    amxMixFactor_SRC_SATURATE,

    // As1
    amxMixFactor_SRC1,

    // 1-As1
    amxMixFactor_ONE_MINUS_SRC1,

    amxMixFactor_TOTAL
} amxMixFactor;

typedef enum amxMixOp
// Sample output mixing operations.
// Once the source and destination mix factors have been selected, 
// they along with the source and destination samples are passed to the mixing operations.
// Samples can use different operations.
{
    // A = As0 × Sa + Ad × Da
    amxMixOp_ADD,

    // A = As0 × Sa - Ad × Da
    amxMixOp_SUB,

    // A = Ad × Da - As0 × Sa
    amxMixOp_REV_SUB,

    // A = min(As0, Ad)
    amxMixOp_MIN,

    // A = max(As0, Ad)
    amxMixOp_MAX,

    amxMixOp_TOTAL
} amxMixOp;

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

AFX_DEFINE_STRUCT(amxMixSink)
{
    amxStoreOp      storeOp;
    amxLoadOp       loadOp;
    afxReal         clearAmpl;
    afxReal         mergeConstant;
    amxMixFactor    srcFactor;
    amxMixFactor    dstFactor;
    amxMixOp        mergeOp;
};

/*
    A mix scope refers to the range or context in which audio mixing occurs. It defines the boundaries of 
    the mixing process, including which audio signals are involved in the mix, how they are processed, and 
    where the mixed output is routed. Essentially, it determines the scope or extent of the mixing operation.

    In hardware audio mixers, the term mix scope can refer to the number of channels or signal paths being processed.
    For example, the mix scope might be the full set of inputs (microphones, instruments, etc.) that are being combined for output.
*/

AFX_DEFINE_STRUCT(amxMixScope)
{
    afxFlags            flags;
    //amxAudio            sink;
    afxSink             sink;
    afxUnit             freq;
    afxUnit             baseFrame;
    afxUnit             frameCnt;
    afxUnit             baseChan;
    afxUnit             chanCnt;
    amxMixSink const*   chans;
    afxUnit             baseSamp;
    afxUnit             sampCnt;
};

typedef enum amxMixState
{
    amxMixState_INITIAL,
    amxMixState_RECORDING,
    amxMixState_EXECUTABLE,
    amxMixState_PENDING,
    amxMixState_INVALID
} amxMixState;

AMX afxUnit     AfxGetMixerPort(afxMixContext mix);
AMX afxUnit     AfxGetMixerPool(afxMixContext mix);
AMX amxMixState AfxGetMixerState(afxMixContext mix);

AMX afxCmdId AmxCmdCommenceMixScope(afxMixContext mix, amxMixScope const* scope);
AMX afxCmdId AmxCmdConcludeMixScope(afxMixContext mix);

AMX afxCmdId AmxCmdReverb
(
    afxMixContext mix, 
    afxUnit voice, 
    afxReal wetMix, 
    afxReal roomSiz, 
    afxReal width, 
    afxReal damp, 
    afxReal dryMix
);

AMX afxCmdId AmxCmdPhaser
(
    afxMixContext mix, 
    afxUnit voice, 
    afxReal floor, 
    afxReal ceil, 
    afxReal rate, 
    afxReal feedback, 
    afxReal depth, 
    afxReal phase, 
    afxUnit stageCnt
);

AMX afxCmdId AmxCmdGainer
(
    afxMixContext mix, 
    afxUnit voice, 
    afxReal gain, 
    afxReal pan, 
    afxMask invChan
);

AMX afxCmdId AmxCmdAnalog
(
    afxMixContext mix, 
    afxUnit voice, 
    afxUnit type, 
    afxUnit flt, 
    afxReal cutoff, 
    afxReal resonance, 
    afxReal inertia, 
    afxReal drive, 
    afxUnit oversamp, 
    afxReal start, 
    afxReal end
);

AMX afxCmdId AmxCmdFlanger
(
    afxMixContext mix, 
    afxUnit voice, 
    afxReal amount, 
    afxReal rate, 
    afxReal amplitude, 
    afxReal feedback, 
    afxReal delay, 
    afxReal phase, 
    afxUnit flt
);

AMX afxCmdId AmxCmdCompressor
(
    afxMixContext mix, 
    afxUnit voice, 
    afxReal threshold, 
    afxReal ratio, 
    afxReal attack, 
    afxReal release, 
    afxReal makeup
);

AMX afxCmdId AmxCmdLineIn
(
    afxMixContext mix, 
    afxUnit voice, 
    afxUnit line, 
    afxMask chanMask, 
    afxUnit latency, 
    afxReal pan, 
    afxReal vol
);

AMX afxCmdId AmxCmdRemoteIn
(
    afxMixContext mix, 
    afxUnit voice, 
    afxUnit src, 
    afxMask chanMask, 
    afxReal pan, 
    afxReal vol
);

AMX afxCmdId AmxCmdSend
(
    afxMixContext mix, 
    afxUnit voice, 
    afxBool muteSrc, 
    afxReal amount, 
    afxReal pan, 
    afxUnit receiverSubmix
);

AMX afxCmdId AmxCmdFetchAudition
(
    afxMixContext mix, 
    afxUnit voice, 
    afxUnit headIdx
);

AMX afxCmdId AmxCmdResampleAudio
(
    afxMixContext mix, 
    amxAudio src, 
    amxAudio dst, 
    amxAudioPeriod const* srci, 
    amxAudioPeriod const* dsti
);

AMX afxCmdId AmxCmdBindBuffers
(
    afxMixContext mix,
    afxUnit bank,
    amxBuffer buf,
    afxUnit offset,
    afxUnit range,
    afxUnit freq
);

AMX afxCmdId AmxCmdBindAudio
(
    afxMixContext mix,
    afxUnit bank,
    amxAudio aud
);

AMX afxCmdId AmxCmdPlayAudioClip
(
    afxMixContext mix,
    afxUnit voice,
    afxUnit bank,
    afxUnit clip,
    afxUnit baseSamp,
    afxUnit baseCh,
    afxUnit sampCnt,
    afxUnit chCnt
);

AFX_DEFINE_STRUCT(amxVideoPicture)
{
    // the offset in texels of the image subregion to use.
    avxOrigin       codedOrigin; // D is the base layer.
    // the size in pixels of the coded image data.
    avxRange2      codedExtent;
    // the video picture resource.
    avxRaster       rasBinding;
};

AFX_DEFINE_STRUCT(amxVideoDecode)
{
    afxFlags            flags;
    avxBuffer           srcBuf;
    afxSize             srcOffset;
    afxUnit             srcRange;
    amxVideoPicture     dstPicRsrc;
    //const VkVideoReferenceSlotInfoKHR*    pSetupReferenceSlot;
    uint32_t                              referenceSlotCount;
    //const VkVideoReferenceSlotInfoKHR*    pReferenceSlots;
};

AMX afxCmdId        AmxCmdDecodeVideo(afxMixContext mix, amxVideoDecode const* param);

AMX afxError        AmxRollMixContext(afxMixContext mix);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(afxMixConfig)
{
    afxUnit exuIdx;
    afxUnit freq;
    afxUnit clipCnt;
    afxUnit voiceCnt;
};

AMX afxError        AmxAcquireMixContext(afxMixSystem msys, afxMixConfig const* cfg, afxMixContext* mixer);

#endif//AMX_MIX_CONTEXT_H
