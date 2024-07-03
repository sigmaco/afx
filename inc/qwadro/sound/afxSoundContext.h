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

#ifndef ASX_SOUND_CONTEXT_H
#define ASX_SOUND_CONTEXT_H

#include "qwadro/mem/afxArena.h"
#include "qwadro/base/afxClass.h"
#include "qwadro/sound/io/afxSoundBuffer.h"
#include "qwadro/sound/afxListener.h"
#include "qwadro/sound/afxSoundOutput.h"
#include "qwadro/sound/afxSoundInput.h"

AFX_DEFINE_STRUCT(afxSoundContextConfig)
{
    afxNat              bridgeCnt;
    afxSoundBridgeConfig const* bridges;
    void*               udd;
};

ASX afxSoundDevice  AfxGetSoundContextDevice(afxSoundContext sctx);

ASX afxBool         AfxGetSoundBridge
/// Get a bridge handle from a device context.
(
    afxSoundContext sctx,
    afxNat          portIdx, /// the index of the bridge.
    afxSoundBridge* bridge /// a pointer to a afxSoundBridge object that will be filled with the handle for the requested bridge.
);

ASX afxBool         AfxGetSoundQueue
/// Get a queue handle from a device context.
(
    afxSoundContext sctx,
    afxNat          portIdx, /// the index of the bridge to which the queue belongs.
    afxNat          queIdx, /// the index within this bridge of the queue to retrieve.
    afxSoundQueue*  queue /// a pointer to a afxSoundQueue object that will be filled with the handle for the requested queue.
);

ASX afxNat          AfxCountSoundBridges(afxSoundContext sctx);
ASX afxNat          AfxCountSoundQueues(afxSoundContext sctx, afxNat portIdx);

ASX afxError        AfxWaitForSoundContext
/// Wait for a device context to become idle.
(
    afxSoundContext sctx
);

ASX afxError        AfxWaitForIdleSoundBridge
/// Wait for a bridge to become idle. To wait on the host for the completion of outstanding queue operations for a given bridge.
(
    afxSoundContext sctx,
    afxNat          portIdx /// the bridge on which to wait.
);

ASX afxError        AfxWaitForIdleSoundQueue
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.
(
    afxSoundContext sctx,
    afxNat          portIdx, /// the bridge on which to wait.
    afxNat          queIdx /// the queue on which to wait.
);

ASX afxNat              AfxCountSoundOutputConnections(afxSoundContext sctx);
ASX afxNat              AfxCountSoundInputConnections(afxSoundContext sctx);
ASX afxBool             AfxGetConnectedSoundInput(afxSoundContext sctx, afxNat conNo, afxSoundInput* input);
ASX afxBool             AfxGetConnectedSoundOutput(afxSoundContext sctx, afxNat conNo, afxSoundOutput* output);
ASX afxNat              AfxInvokeConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxSoundInput, void*), void *udd);
ASX afxNat              AfxInvokeConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxSoundOutput, void*), void *udd);
ASX afxNat              AfxEnumerateConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxSoundInput inputs[]);
ASX afxNat              AfxEnumerateConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxSoundOutput outputs[]);
ASX afxError            AfxDisconnectSoundContext(afxSoundContext sctx, afxBool keepInputs, afxBool keepOutputs);

////////////////////////////////////////////////////////////////////////////////

ASX afxError            AfxOpenSoundDevice(afxNat sdevId, afxSoundContextConfig const* cfg, afxSoundContext* context);


#if 0


/// Asynchronously posts an Event to the sound engine (by event ID).
/// The callback function can be used to be noticed when markers are reached or when the event is finished.
/// An array of wave file sources can be provided to resolve External Sources triggered by the event. 
/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
/// If used, the array of external sources should contain the information for each external source triggered by the event. When triggering an event with multiple external sources, you need to differentiate each source by using the cookie property in the External Source in the Wwise project and in AkExternalSourceInfo.
/// \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project (therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures.

AFX_EXTERN_API_FUNC(asxPlayingId, PostEvent)
(
    asxUniqueId in_eventID,                          ///< Unique ID of the event
    asxGameObjectId in_gameObjectID,                 ///< Associated game object ID
    afxNat32 in_uFlags,                         ///< Bitmask: see \ref AkCallbackType
    afxCallbackFunc in_pfnCallback,           ///< Callback function
    void * in_pCookie,                       ///< Callback cookie that will be sent to the callback function along with additional information
    afxNat32 in_cExternals,                     ///< Optional count of external source structures
    afxSource *in_pExternalSources,///< Optional array of external source resolution information
    asxPlayingId in_PlayingID ///< Optional (advanced users only) Specify the playing ID to target with the event. Will Cause active actions in this event to target an existing Playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any for normal playback.
    );

/// Posts an Event to the sound engine (by Event name), using callbacks.
/// The callback function can be used to be noticed when markers are reached or when the event is finished.
/// An array of Wave file sources can be provided to resolve External Sources triggered by the event. P
/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
/// If used, the array of external sources should contain the information for each external source triggered by the event. When triggering an Event with multiple external sources, you need to differentiate each source by using the cookie property in the External Source in the Wwise project and in AkExternalSourceInfo.
/// \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project (therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures.

AFX_EXTERN_API_FUNC(asxPlayingId, PostEvent_)
(
    const char* in_pszEventName,                    ///< Name of the event
    asxGameObjectId in_gameObjectID,                 ///< Associated game object ID
    afxNat32 in_uFlags,                         ///< Bitmask: see \ref AkCallbackType
    afxCallbackFunc in_pfnCallback,           ///< Callback function
    void * in_pCookie,                       ///< Callback cookie that will be sent to the callback function along with additional information.
    afxNat32 in_cExternals,                     ///< Optional count of external source structures
    afxSource *in_pExternalSources, ///< Optional array of external source resolution information
    asxPlayingId in_PlayingID ///< Optional (advanced users only) Specify the playing ID to target with the event. Will Cause active actions in this event to target an existing Playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any for normal playback.
    );

typedef enum afxActionOnEventType
{
    AfxActionOnEventType_Stop = 0,            ///< Stop
    AfxActionOnEventType_Pause = 1,            ///< Pause
    AfxActionOnEventType_Resume = 2,            ///< Resume
    AfxActionOnEventType_Break = 3,            ///< Break
    AfxActionOnEventType_ReleaseEnvelope = 4     ///< Release envelope
}afxActionOnEventType;

/// Executes an action on all nodes that are referenced in the specified event in an action of type play.

AFX_EXTERN_API_FUNC(afxResult, ExecuteActionOnEvent)
(
    asxUniqueId in_eventID,                                              ///< Unique ID of the event

    afxActionOnEventType in_ActionType,                                  ///< Action to execute on all the elements that were played using the specified event.

    asxGameObjectId in_gameObjectID,            ///< Associated game object ID

    afxMillisecond in_uTransitionDuration,                                ///< Fade duration

    asxCurveInterpolation in_eFadeCurve,   ///< Curve type to be used for the transition

    asxPlayingId in_PlayingID                    ///< Associated PlayingID
    );

/// Executes an Action on all nodes that are referenced in the specified Event in an Action of type Play.

AFX_EXTERN_API_FUNC(afxResult, ExecuteActionOnEvent_)
(
    const char* in_pszEventName,                                        ///< Name of the event
    afxActionOnEventType in_ActionType,                                  ///< Action to execute on all the elements that were played using the specified event.
    asxGameObjectId in_gameObjectID,            ///< Associated game object ID
    afxMillisecond in_uTransitionDuration,                                ///< Fade duration
    asxCurveInterpolation in_eFadeCurve,   ///< Curve type to be used for the transition
    asxPlayingId in_PlayingID                    ///< Associated PlayingID
    );

#endif

#if 0
////////////////////////////////////////////////////////////////////////////////
// QWADRO PLUGIN SYSTEM
////////////////////////////////////////////////////////////////////////////////

typedef void afxPlugin;
typedef void afxPluginParam;

/// Registered plugin creation function prototype.

AFX_CALLBACK(afxPlugin*, afxCreatePluginCallback)(afxAllocator * in_pAllocator);

// Registered plugin parameter node creation function prototype.

AFX_CALLBACK(afxPluginParam*, afxCreateParamCallback)(afxAllocator * in_pAllocator);

// Registered plugin device enumeration function prototype, used for providing lists of devices by plug-ins.

AFX_CALLBACK(afxResult, afxGetDeviceListCallback)
(
    afxNat32    *io_maxNumDevices,                 ///< In: The length of the out_deviceDescriptions array, or zero is out_deviceDescriptions is null. Out: If out_deviceDescriptions is not-null, this should be set to the number of entries in out_deviceDescriptions that was populated (and should be less-than-or-equal to the initial value). If out_deviceDescriptions is null, this should be set to the maximum number of devices that may be returned by this callback.
    afxDeviceDescription* out_deviceDescriptions ///< The output array of device descriptions. If this is not-null, there will be a number of entries equal to the input value of io_maxNumDevices.
    );

/// Plug-in type.

typedef enum afxPluginType
{
    afxPluginTypeNone = 0,   ///< Unknown/invalid plug-in type.
    afxPluginTypeCodec = 1,  ///< Compressor/decompressor plug-in (allows support for custom audio file types).
    afxPluginTypeSource = 2, ///< Source plug-in: creates sound by synthesis method (no input, just output).
    afxPluginTypeEffect = 3, ///< Effect plug-in: applies processing to audio data.
    afxPluginTypeMixer = 6,  ///< Mixer plug-in: mix voices at the bus level.
    afxPluginTypeSink = 7,   ///< Sink plug-in: implement custom sound engine end point.
    afxPluginTypeGlobalExtension = 8,    ///< Global Extension plug-in: (e.g. Spatial Audio, Interactive Music)
    afxPluginTypeMetadata = 9,   ///< Metadata plug-in: applies object-based processing to audio data
    afxPluginTypeMask = 0xf  ///< Plug-in type mask is 4 bits.
} afxPluginType;

/// Registers a plug-in with the sound engine and sets the callback functions to create the plug-in and its parameter node.  
/// This function is deprecated. Registration is now automatic if you link plug-ins statically. If plug-ins are dynamic libraries (such as DLLs or SOs), use \c RegisterPluginDLL.
/// Codecs and plug-ins must be registered before loading banks that use them.
/// Loading a bank referencing an unregistered plug-in or codec will result in a load bank success, but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, posting the event will fail.

AFX_EXTERN_API_FUNC(afxError, RegisterPlugin)
(
    afxPluginType in_eType, // Plug-in type (for example, source or effect)
    afxNat32 in_ulCompanyID, // Company identifier (as declared in the plug-in description XML file)
    afxNat32 in_ulPluginID, // Plug-in identifier (as declared in the plug-in description XML file)
    afxCreatePluginCallback in_pCreateFunc, // Pointer to the plug-in's creation function
    afxCreateParamCallback in_pCreateParamFunc, // Pointer to the plug-in's parameter node creation function
    afxGetDeviceListCallback in_pGetDeviceList // Optional pointer to the plug-in's device enumeration function. Specify for a sink plug-in to support \ref AK::SoundEngine::GetDeviceList.
    );



#endif


#endif//ASX_SOUND_CONTEXT_H
