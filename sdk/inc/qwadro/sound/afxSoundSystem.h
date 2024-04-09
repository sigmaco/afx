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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// The Unified Qwadro Accoustic Infrastructure
// This section is part of SIGMA A4D.
// Advanced Audio Extensions for Qwadro.

#ifndef AFX_SOUND_SYSTEM_H
#define AFX_SOUND_SYSTEM_H

#include "qwadro/core/afxManager.h"
#include "qwadro/io/afxSource.h"
#include "qwadro/core/afxDevice.h"
#include "qwadro/sound/afxSoundContext.h"

typedef enum _sdevReqCode
{
    _sdevReqCode_0,
    _sdevReqCode_1,
} _sdevReqCode;

AFX_DEFINE_STRUCT(afxSoundDeviceInfo)
{
    afxDeviceInfo           dev;
};

#ifdef _AFX_SOUND_C
#ifdef _AFX_SOUND_DEVICE_C

AFX_DEFINE_STRUCT(afxSoundInputEndpoint)
{
    afxLinkage              ddev;
    afxChain                mechanisms;
};

AFX_DEFINE_STRUCT(afxSoundOutputEndpoint)
{
    afxLinkage              ddev;
    afxChain                mechanisms;
    
};

AFX_OBJECT(afxSoundDevice)
{
    AFX_OBJECT(afxDevice)   dev;
    afxManager              contexts;
    afxManager              outputs;
    afxManager              inputs;

    struct _afxSdevIdd*     idd;
};
#endif//_AFX_SOUND_DEVICE_C
#endif//_AFX_SOUND_C

AFX_DEFINE_STRUCT(afxSoundSystemConfig)
{

};

#ifdef _AFX_SOUND_C
#ifdef _AFX_SOUND_SYSTEM_C
AFX_OBJECT(afxSoundSystem)
{
    afxChain            mgrChn;
    afxManager          devices;
    //afxIcd            e2sound; // SIGMA A4D is required for minimal operability since core has no more embedded fallback.
};
#endif//_AFX_SOUND_SYSTEM_C
#endif//_AFX_SOUND_C

AAX void            AfxChooseSoundSystemConfiguration(afxSoundSystemConfig *config, afxNat extendedSiz);

AAX afxBool         AfxGetSoundSystem(afxSoundSystem* ssys);

AAX afxManager*     AfxGetSoundDeviceClass(void);

AAX afxNat          AfxCountSoundDevices(void);

AAX afxNat          AfxEnumerateSoundDevices(afxNat first, afxNat cnt, afxSoundDevice sdev[]);

AAX afxNat          AfxInvokeSoundDevices(afxNat first, afxNat cnt, afxBool(*f)(afxSoundDevice, void*), void *udd);

////////////////////////////////////////////////////////////////////////////////
// SOUND DEVICE                                                               //
////////////////////////////////////////////////////////////////////////////////

AAX afxSoundDevice  AfxGetSoundDevice(afxNat sdevIdx);

AAX afxBool         AfxSoundDeviceIsRunning(afxSoundDevice sdev);

AAX afxManager*       AfxGetSoundContextClass(afxSoundDevice sdev);

AAX afxNat          AfxCountSoundContexts(afxSoundDevice sdev);

AAX afxNat          AfxEnumerateSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundContext contexts[]);

AAX afxNat          AfxInvokeSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundContext, void*), void *udd);

AFX_DEFINE_STRUCT(afxSoundContextConfig)
{
};

AAX afxError        AfxAcquireSoundContexts(afxNat sdevId, afxNat cnt, afxSoundContextConfig const config[], afxSoundContext contexts[]);





////////////////////////////////////////////////////////////////////////






/// Asynchronously posts an Event to the sound engine (by event ID).
/// The callback function can be used to be noticed when markers are reached or when the event is finished.
/// An array of wave file sources can be provided to resolve External Sources triggered by the event. 
/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
/// If used, the array of external sources should contain the information for each external source triggered by the event. When triggering an event with multiple external sources, you need to differentiate each source by using the cookie property in the External Source in the Wwise project and in AkExternalSourceInfo.
/// \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project (therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures.

AFX_EXTERN_API_FUNC(aaxPlayingId, PostEvent)
(
    aaxUniqueId in_eventID,                          ///< Unique ID of the event
    aaxGameObjectId in_gameObjectID,                 ///< Associated game object ID
    afxNat32 in_uFlags,                         ///< Bitmask: see \ref AkCallbackType
    afxCallbackFunc in_pfnCallback,           ///< Callback function
    void * in_pCookie,                       ///< Callback cookie that will be sent to the callback function along with additional information
    afxNat32 in_cExternals,                     ///< Optional count of external source structures
    afxSource *in_pExternalSources,///< Optional array of external source resolution information
    aaxPlayingId in_PlayingID ///< Optional (advanced users only) Specify the playing ID to target with the event. Will Cause active actions in this event to target an existing Playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any for normal playback.
    );

/// Posts an Event to the sound engine (by Event name), using callbacks.
/// The callback function can be used to be noticed when markers are reached or when the event is finished.
/// An array of Wave file sources can be provided to resolve External Sources triggered by the event. P
/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
/// If used, the array of external sources should contain the information for each external source triggered by the event. When triggering an Event with multiple external sources, you need to differentiate each source by using the cookie property in the External Source in the Wwise project and in AkExternalSourceInfo.
/// \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project (therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures.

AFX_EXTERN_API_FUNC(aaxPlayingId, PostEvent_)
(
    const char* in_pszEventName,                    ///< Name of the event
    aaxGameObjectId in_gameObjectID,                 ///< Associated game object ID
    afxNat32 in_uFlags,                         ///< Bitmask: see \ref AkCallbackType
    afxCallbackFunc in_pfnCallback,           ///< Callback function
    void * in_pCookie,                       ///< Callback cookie that will be sent to the callback function along with additional information.
    afxNat32 in_cExternals,                     ///< Optional count of external source structures
    afxSource *in_pExternalSources, ///< Optional array of external source resolution information
    aaxPlayingId in_PlayingID ///< Optional (advanced users only) Specify the playing ID to target with the event. Will Cause active actions in this event to target an existing Playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any for normal playback.
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
    aaxUniqueId in_eventID,                                              ///< Unique ID of the event

    afxActionOnEventType in_ActionType,                                  ///< Action to execute on all the elements that were played using the specified event.

    aaxGameObjectId in_gameObjectID,            ///< Associated game object ID

    afxMillisecond in_uTransitionDuration,                                ///< Fade duration

    aaxCurveInterpolation in_eFadeCurve,   ///< Curve type to be used for the transition

    aaxPlayingId in_PlayingID                    ///< Associated PlayingID
    );

/// Executes an Action on all nodes that are referenced in the specified Event in an Action of type Play.

AFX_EXTERN_API_FUNC(afxResult, ExecuteActionOnEvent_)
(
    const char* in_pszEventName,                                        ///< Name of the event
    afxActionOnEventType in_ActionType,                                  ///< Action to execute on all the elements that were played using the specified event.
    aaxGameObjectId in_gameObjectID,            ///< Associated game object ID
    afxMillisecond in_uTransitionDuration,                                ///< Fade duration
    aaxCurveInterpolation in_eFadeCurve,   ///< Curve type to be used for the transition
    aaxPlayingId in_PlayingID                    ///< Associated PlayingID
    );



#endif//AFX_SOUND_SYSTEM_H
