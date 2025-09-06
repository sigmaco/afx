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

typedef enum amxBus
{
    amxBus_VOICE,
    amxBus_DISPATCH,
    amxBus_AMBISONIC,
} amxBus;

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

AFX_DEFINE_STRUCT(amxMixTarget)
{
    afxUnit         voiceId;
    afxUnit         trackId;
    afxUnit         chIdx;

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
    amxAudio            sink;
    afxUnit             baseSamp;
    afxUnit             baseChan;
    afxUnit             baseSeg;
    afxUnit             sampCnt;
    afxUnit             chanCnt;
    afxUnit             segCnt;
    amxMixTarget        chans[32];
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

AMX afxError        AmxExhaustMixContext
(
    afxMixContext  dctx,
    afxBool         freeMem
);

AMX afxError        AmxRecordMixCommands
(
    // The draw context which the batch will be allocated from.
    afxMixContext  dctx,
    // A flag specifying a one-time submission batch.
    afxBool         once,
    // A flag specifying a inlineable batch.
    afxBool         deferred,
    // A variable to hold the unique identifier for the batch.
    afxUnit*        batchId
);

AMX afxError        AmxDiscardMixCommands
(
    afxMixContext  dctx,
    afxBool         freeRes
);

AMX afxError        AmxCompileMixCommands
(
    // The draw context recording commands.
    afxMixContext  dctx,
    // The batch which commands will be compiled into.
    afxUnit         batchId
);

AMX afxError        AmxRecycleMixCommands
(
    // The draw context that holds the commands.
    afxMixContext  dctx,
    // The unique identifier for the draw batch.
    afxUnit         batchId,
    // A flag that indicates whether all or most of the resources owned by the batch should be reclaimed by the system.
    afxBool         freeRes
);

AMX afxBool AmxDoesMixCommandsExist(afxMixContext dctx, afxUnit batchId);

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

AMX afxCmdId AmxCmdGenerateSineWave(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxCmdId AmxCmdGenerateSquareWave(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxCmdId AmxCmdGenerateTriangleWave(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxCmdId AmxCmdGenerateSawtoothWave(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxCmdId AmxCmdGenerateWhiteNoise(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxCmdId AmxCmdGeneratePinkNoise(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxCmdId AmxCmdGenerateBrownianNoise(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxCmdId AmxCmdGenerateBlueNoise(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxCmdId AmxCmdGenerateVioletNoise(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);
AMX afxCmdId AmxCmdGenerateGrayNoise(afxMixContext mix, afxUnit sampleRate, afxReal amplitude, afxReal freq, afxReal dur);

AMX afxCmdId AmxCmdDeclareTimelineBarrier(afxMixContext mix, afxReal time);

AMX afxCmdId AmxCmdTrack(afxMixContext mix, int a);

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

AMX afxCmdId AmxCmdCommenceMixScope
(
    afxMixContext mix,
    amxMixScope const* scope
);

AMX afxCmdId AmxCmdConcludeMixScope
(
    afxMixContext mix
);

AFX_DEFINE_STRUCT(amxVideoPicture)
// Structure specifying the parameters of a video picture resource.
{
    // The video picture resource.
    avxRaster       pic;
    // The meaning of the rect depends on the command and context the video picture resource is used in, 
    // as well as on the used video profile and corresponding codec-specific semantics.
    // The image subresource referred to by such a structure is defined as the image layer index specified in 
    // baseLayer relative to the image subresource range the image view specified in @pic was created with.
    afxLayeredRect  codedRc;
};

AFX_DEFINE_STRUCT(amxVideoDecode)
// Structure specifying video decode parameters.
{
    afxFlags        flags;
    // The video picture resource to use as the decode output picture.
    amxVideoPicture pic;
    // The index of the DPB slot or a negative integer value.
    afxInt          baseRefSlotId;
    // Structure describing the video picture resource associated with the DPB slot index specified by @baseRefSlotIdx.
    amxVideoPicture baseRefPic;
    // The number of elements in the @refSlots array.
    afxUnit         refSlotCnt;
    // An array of structures describing the DPB slots and corresponding reference picture resources to use in this video decode operation (the set of active reference pictures).
    amxVideoPicture refSlots[8];
    // The source video bitstream buffer to read the encoded bitstream from.
    avxBuffer       srcBuf;
    // The starting offset in bytes from the start of @srcBuf to read the encoded bitstream from.
    afxSize         srcOffset;
    // The size in bytes of the encoded bitstream to decode from @srcBuf, starting from @srcOffset.
    afxUnit         srcRange;
    
};

AFX_DEFINE_STRUCT(amxVideoEncode)
// Structure specifying video encode parameters.
{
    afxFlags        flags;
    // The video picture resource to use as the encode input picture.
    amxVideoPicture pic;
    // The index of the DPB slot or a negative integer value.
    afxInt          baseRefSlotId;
    // Structure describing the video picture resource associated with the DPB slot index specified by @baseRefSlotIdx.
    amxVideoPicture baseRefPic;
    // The number of elements in the @refSlots array.
    afxUnit         refSlotCnt;
    // An array of structures describing the DPB slots and corresponding reference picture resources to use in this video encode operation (the set of active reference pictures).
    amxVideoPicture refSlots[8];
    // The number of bytes externally encoded by the application to the video bitstream and is used to update the internal state of the implementation's rate control algorithm to account for the bitrate budget consumed by these externally encoded bytes.
    afxUnit         precedingEncBytes;
    // The destination video bitstream buffer to write the encoded bitstream to.
    avxBuffer       dstBuf;
    // The starting offset in bytes from the start of @dstBuf to write the encoded bitstream to.
    afxSize         dstOffset;
    // The maximum bitstream size in bytes that can be written to @dstBuf, starting from @dstOffset.
    afxUnit         dstRange;
};

AMX afxCmdId        AmxCmdEncodeVideo(afxMixContext mix, amxVideoEncode const* param);
AMX afxCmdId        AmxCmdDecodeVideo(afxMixContext mix, amxVideoDecode const* param);

AFX_DEFINE_STRUCT(amxVideoScope)
{
    afxFlags        flags;
    afxUnit         trackId; // video session in Vulkan
    void*           param; // video session parameters in Vulkan
    afxUnit         refCnt;
    amxVideoPicture refSlots[8];
};

AFX_DEFINE_STRUCT(amxVideoEncRate)
// Structure to set encode per-layer rate control parameters.
{
    // The average bitrate to be targeted by the implementation's rate control algorithm.
    afxUnit64       avgBitrate;
    // The peak bitrate to be targeted by the implementation's rate control algorithm.
    afxUnit64       maxBitrate;
    // The numerator of the frame rate assumed by the implementation's rate control algorithm.
    afxUnit64       frameRateNumer;
    // The denominator of the frame rate assumed by the implementation's rate control algorithm.
    afxUnit64       frameRateDenom;
};

AFX_DEFINE_STRUCT(amxVideoEncScope)
{
    afxFlags        flags;
    afxUnit         trackId; // video session in Vulkan
    void*           param; // video session parameters in Vulkan
    afxUnit         refCnt;
    amxVideoPicture refSlots[8];

    afxFlags        ctrlFlags;

    // The used video encode quality level.
    afxUnit         qualityLvl;

    afxFlags        encFlags;
    // A value specifying the rate control mode.
    afxFlags        rateCtrl;
    // The number of rate control layers to use.
    afxUnit         rateCtrlCnt;
    // An array of structures, each specifying the rate control configuration of the corresponding rate control layer.
    amxVideoEncRate rateCtrls[8];
    // The size in milliseconds of the virtual buffer used by the implementation's rate control algorithm for the leaky bucket model, 
    // with respect to the average bitrate of the stream calculated by summing the values of the avgBitrate members of the elements of the layers array.
    afxUnit         virtBufSizInMs;
    // The initial occupancy in milliseconds of the virtual buffer used by the implementation's rate control algorithm for the leaky bucket model.
    afxUnit         initialVirtBufSizInMs;
};

AMX afxCmdId        AmxCmdCommenceVideo(afxMixContext mix, amxVideoScope const* scope);
AMX afxCmdId        AmxCmdConcludeVideo(afxMixContext mix);

AMX afxCmdId        AmxCmdCathodeVideo(afxMixContext mix, afxUnit track, afxReal yShift, afxReal uShift, afxReal vShift);
AMX afxCmdId        AmxCmdStaticHiVideo(afxMixContext mix, afxUnit track, afxReal i);

AMX afxCmdId        AmxCmdComposeVideo(afxMixContext mix, afxUnit track, amxVideo vid, avxRaster composite);

AMX afxError        AmxExecuteMixCommands(afxMixContext mix, afxUnit cnt, amxSubmission const subms[]);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(afxMixConfig)
{
    afxFlags    flags;
    afxString   tag;
    void*       udd;
};

AMX afxError            AmxAcquireMixContext
(
    afxMixSystem        msys, 
    afxMixConfig const* cfg, 
    afxMixContext*      mixer
);

#endif//AMX_MIX_CONTEXT_H
