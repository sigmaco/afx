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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ASX_SOUND_CONTEXT_H
#define ASX_SOUND_CONTEXT_H

#include "qwadro/mem/afxArena.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/sound/io/afxSoundBuffer.h"
#include "qwadro/sound/io/afxCurve.h"
#include "qwadro/sound/afxListener.h"
#include "qwadro/sound/dev/afxSoundOutput.h"
#include "qwadro/sound/dev/afxSoundInput.h"

AFX_DEFINE_STRUCT(afxSoundContextConfig)
{
    void*               udd;
};

#ifdef _ASX_SOUND_C
#ifdef _ASX_SOUND_CONTEXT_C
#ifdef _ASX_SOUND_CONTEXT_IMPL
struct _afxBaseSoundContext
#else
AFX_OBJECT(afxSoundContext)
#endif
{
    AFX_OBJECT(afxContext)  ctx;
    afxBool             running;
    afxArena            aren;

    afxNat              ownedBridgeCnt;
    afxSoundBridge*     ownedBridges;

    afxChain            inputs;
    afxChain            outputs;

    //afxChain            classes;
    afxManager          buffers;
    afxManager          curves;

    afxError            (*waitCb)(afxSoundContext);

    struct _afxSctxIdd* idd;
    void*               udd; // user-defined data
};

#ifdef _ASX_SOUND_CONTEXT_IMPL
AFX_STATIC_ASSERT(offsetof(struct _afxBaseSoundContext, ctx) == 0x00, "");
#else
AFX_STATIC_ASSERT(offsetof(AFX_OBJECT(afxSoundContext), ctx) == 0x00, "");
#endif

#endif//_ASX_SOUND_CONTEXT_C
#endif//_ASX_SOUND_C

ASX afxSoundDevice      AfxGetSoundContextDevice(afxSoundContext sctx);

ASX afxNat              AfxCountSoundBridges(afxSoundContext sctx);
ASX afxSoundBridge      AfxGetSoundBridge(afxSoundContext sctx, afxNat portIdx);

ASX afxError            AfxWaitForIdleSoundContext(afxSoundContext sctx);

ASX afxNat              AfxCountSoundOutputConnections(afxSoundContext sctx);
ASX afxNat              AfxCountSoundInputConnections(afxSoundContext sctx);
ASX afxBool             AfxGetConnectedSoundInput(afxSoundContext sctx, afxNat conNo, afxSoundInput* input);
ASX afxBool             AfxGetConnectedSoundOutput(afxSoundContext sctx, afxNat conNo, afxSoundOutput* output);
ASX afxNat              AfxInvokeConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxSoundInput, void*), void *udd);
ASX afxNat              AfxInvokeConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxBool(*f)(afxSoundOutput, void*), void *udd);
ASX afxNat              AfxEnumerateConnectedSoundInputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxSoundInput inputs[]);
ASX afxNat              AfxEnumerateConnectedSoundOutputs(afxSoundContext sctx, afxNat baseCon, afxNat conCnt, afxSoundOutput outputs[]);
ASX afxError            AfxDisconnectSoundContext(afxSoundContext sctx, afxBool inputs, afxBool outputs);

////////////////////////////////////////////////////////////////////////////////

ASX afxError            AfxAcquireSoundContext(afxNat sdevId, afxSoundContextConfig const* cfg, afxSoundContext* context);


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


#endif//ASX_SOUND_CONTEXT_H
