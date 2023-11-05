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

#ifndef AFX_SOUND_SYSTEM_H
#define AFX_SOUND_SYSTEM_H

#include "afxSoundContext.h"
#include "afx/core/afxClass.h"
#include "afx/core/afxSystem.h"
#include "afx/core/afxSource.h"

AFX_DEFINE_HANDLE(afxSoundSystem);

AFX_OBJECT(afxSoundSystem)
{
    afxInstance obj;
};

AFX_DEFINE_STRUCT(afxSoundSystemConfiguration)
{
    afxBool test;
};




typedef afxNat32 afxBankID;
typedef afxNat32 afxBankType;
typedef afxInt8 afxPriority;
typedef afxInt32 afxMemPoolId;

AFX_CALLBACK(void, afxBankCallbackFunc)(afxNat32        in_bankID, const void *    in_pInMemoryBankPtr, afxResult in_eLoadResult, void *          in_pCookie );

typedef enum afxPreparationType
{
    Preparation_Load,   ///< \c PrepareEvent() will load required information to play the specified event.
    Preparation_Unload, ///< \c PrepareEvent() will unload required information to play the specified event.
    Preparation_LoadAndDecode ///< Vorbis media is decoded when loading, and an uncompressed PCM version is used for playback.
} afxPreparationType;

typedef enum afxBankContent
{
    AkBankContent_StructureOnly,    ///< Use AkBankContent_StructureOnly to load only the structural content, including Events, and then later use the PrepareEvent() functions to load media on demand from loose files on the disk.
    AkBankContent_All               ///< Use AkBankContent_All to load both the media and structural content.
} afxBankContent;

////////////////////////////////////////////////////////////////////////

AFX afxResult ClearBanks(void);

AFX afxResult SetBankLoadIOSettings
(
    afxReal32            in_fThroughput,         ///< Average throughput of bank data streaming (bytes/ms) (the default value is AK_DEFAULT_BANK_THROUGHPUT)
    afxPriority          in_priority             ///< Priority of bank streaming (the default value is AK_DEFAULT_PRIORITY)
);

AFX afxResult LoadBank_
(

    const char*         in_pszString,                   ///< Name of the bank to load
    afxBankID*          out_bankID,                     ///< Returned bank ID
    afxBankType          in_bankType   ///< Type of the bank to load
);

AFX afxResult LoadBank
(
    afxBankID            in_bankID,                      ///< Bank ID of the bank to load
    afxBankType          in_bankType   ///< Type of the bank to load
);

AFX afxResult LoadBankMemoryView
(
    const void *        in_pInMemoryBankPtr,    ///< Pointer to the in-memory bank to load (pointer is stored in sound engine, memory must remain valid)
    afxNat32            in_uInMemoryBankSize,   ///< Size of the in-memory bank to load
    afxBankID *          out_bankID              ///< Returned bank ID
);

AFX afxResult LoadBankMemoryView_
(
    const void *        in_pInMemoryBankPtr,    ///< Pointer to the in-memory bank to load (pointer is stored in sound engine, memory must remain valid)
    afxNat32            in_uInMemoryBankSize,   ///< Size of the in-memory bank to load
    afxBankID*          out_bankID,             ///< Returned bank ID
    afxBankType *        out_bankType            ///< Returned bank type
);

AFX afxResult LoadBankMemoryCopy
(
    const void *        in_pInMemoryBankPtr,    ///< Pointer to the in-memory bank to load (pointer is not stored in sound engine, memory can be released after return)
    afxNat32            in_uInMemoryBankSize,   ///< Size of the in-memory bank to load
    afxBankID *          out_bankID              ///< Returned bank ID
);

AFX afxResult LoadBankMemoryCopy_
(
    const void *        in_pInMemoryBankPtr,    ///< Pointer to the in-memory bank to load (pointer is not stored in sound engine, memory can be released after return)
    afxNat32            in_uInMemoryBankSize,   ///< Size of the in-memory bank to load
    afxBankID *          out_bankID,             ///< Returned bank ID
    afxBankType *        out_bankType            ///< Returned bank type
);

AFX afxResult DecodeBank
(
    const void *        in_pInMemoryBankPtr,    ///< Pointer to the in-memory bank to decode (pointer is not stored in sound engine, memory can be released after return)
    afxNat32            in_uInMemoryBankSize,   ///< Size of the in-memory bank to decode
    afxMemPoolId         in_uPoolForDecodedBank, ///< Memory pool to allocate decoded bank into. Specify AK_INVALID_POOL_ID and out_pDecodedBankPtr=NULL to obtain decoded bank size without performing the decode operation. Pass AK_INVALID_POOL_ID and out_pDecodedBankPtr!=NULL to decode bank into specified pointer.
    void **            out_pDecodedBankPtr,    ///< Decoded bank memory location.
    afxNat32*          out_uDecodedBankSize    ///< Decoded bank memory size.
);

AFX afxResult LoadBank__
(
    const char*         in_pszString,                   ///< Name/path of the bank to load
    afxBankCallbackFunc  in_pfnBankCallback,             ///< Callback function
    void *              in_pCookie,                     ///< Callback cookie (reserved to user, passed to the callback function, and also to  <tt>AK::StreamMgr::IAkFileLocationResolver::Open()</tt> as AkFileSystemFlags::pCustomParam)
    afxBankID *          out_bankID,                     ///< Returned bank ID
    afxBankType          in_bankType   ///< Type of the bank to load
);

AFX afxResult LoadBank___
(
    afxBankID            in_bankID,                      ///< Bank ID of the bank to load
    afxBankCallbackFunc  in_pfnBankCallback,             ///< Callback function
    void *              in_pCookie,                     ///< Callback cookie (reserved to user, passed to the callback function, and also to  <tt>AK::StreamMgr::IAkFileLocationResolver::Open()</tt> as AkFileSystemFlags::pCustomParam)
    afxBankType          in_bankType   ///< Type of the bank to load
);

AFX afxResult LoadBankMemoryView__
(
    const void *        in_pInMemoryBankPtr,    ///< Pointer to the in-memory bank to load (pointer is stored in sound engine, memory must remain valid)
    afxNat32            in_uInMemoryBankSize,   ///< Size of the in-memory bank to load
    afxBankCallbackFunc  in_pfnBankCallback,     ///< Callback function
    void*              in_pCookie,             ///< Callback cookie
    afxBankID*          out_bankID              ///< Returned bank ID
);

AFX afxResult LoadBankMemoryView___
(
    const void *        in_pInMemoryBankPtr,    ///< Pointer to the in-memory bank to load (pointer is stored in sound engine, memory must remain valid)
    afxNat32            in_uInMemoryBankSize,   ///< Size of the in-memory bank to load
    afxBankCallbackFunc  in_pfnBankCallback,     ///< Callback function
    void *              in_pCookie,             ///< Callback cookie
    afxBankID*          out_bankID,             ///< Returned bank ID
    afxBankType*        out_bankType            ///< Returned bank type
);

AFX afxResult LoadBankMemoryCopy__
(
    void const * in_pInMemoryBankPtr,    ///< Pointer to the in-memory bank to load (pointer is not stored in sound engine, memory can be released after callback)
    afxNat32            in_uInMemoryBankSize,   ///< Size of the in-memory bank to load
    afxBankCallbackFunc  in_pfnBankCallback,     ///< Callback function
    void* in_pCookie,               ///< Callback cookie
    afxBankID* out_bankID,               ///< Returned bank ID
    afxBankType* out_bankType            ///< Returned bank type
);

AFX afxResult LoadBankMemoryCopy___
(
    void const*        in_pInMemoryBankPtr,    ///< Pointer to the in-memory bank to load (pointer is not stored in sound engine, memory can be released after callback)
    afxNat32            in_uInMemoryBankSize,   ///< Size of the in-memory bank to load
    afxBankCallbackFunc  in_pfnBankCallback,     ///< Callback function
    void *              in_pCookie,             ///< Callback cookie
    afxBankID*          out_bankID,             ///< Returned bank ID
    afxBankType*        out_bankType            ///< Returned bank type
);

AFX afxResult UnloadBank
(
    const char*         in_pszString,                   ///< Name of the bank to unload
    const void *        in_pInMemoryBankPtr,            ///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
    afxBankType          in_bankType   ///< Type of the bank to unload
);

AFX afxResult UnloadBank_
(

    afxBankID            in_bankID,                      ///< ID of the bank to unload
    const void *        in_pInMemoryBankPtr,            ///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
    afxBankType          in_bankType   ///< Type of the bank to unload
);

AFX afxResult UnloadBank__
(
    const char*         in_pszString,                   ///< Name of the bank to unload
    const void *        in_pInMemoryBankPtr,            ///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
    afxBankCallbackFunc  in_pfnBankCallback,             ///< Callback function
    void *              in_pCookie,                     ///< Callback cookie (reserved to user, passed to the callback function)
    afxBankType          in_bankType   ///< Type of the bank to unload
);

AFX afxResult UnloadBank___
(
    afxBankID            in_bankID,                      ///< ID of the bank to unload
    const void *        in_pInMemoryBankPtr,            ///< Memory pointer from where the bank was initially loaded from. (REQUIRED to determine which bank associated to a memory pointer must be unloaded). Pass NULL if NULL was passed when loading the bank or if LoadBankMemoryCopy was used to load the bank.
    afxBankCallbackFunc  in_pfnBankCallback,             ///< Callback function
    void *              in_pCookie,                     ///< Callback cookie (reserved to user, passed to the callback function)
    afxBankType          in_bankType   ///< Type of the bank to unload
);

AFX afxResult CancelBankCallbackCookie
(
void * in_pCookie                           ///< Callback cookie to be canceled
);

AFX afxResult PrepareBank
(
    afxPreparationType in_PreparationType,         ///< Preparation type ( Preparation_Load or Preparation_Unload )
    char const* in_pszString,               ///< Name of the bank to Prepare/Unprepare.
    afxBankContent  in_uFlags,  ///< Structures only (including events) or all content.
    afxBankType in_bankType   ///< Type of the bank to Prepare/Unprepare.
);

AFX afxResult PrepareBank_
(
    afxPreparationType in_PreparationType,             ///< Preparation type ( Preparation_Load or Preparation_Unload )
    afxBankID in_bankID,                      ///< ID of the bank to Prepare/Unprepare.
    afxBankContent in_uFlags,  ///< Structures only (including events) or all content.
    afxBankType in_bankType   ///< Type of the bank to Prepare/Unprepare.
);

AFX afxResult PrepareBank__
(
    afxPreparationType in_PreparationType,         ///< Preparation type ( Preparation_Load or Preparation_Unload )
    char const* in_pszString,                               ///< Name of the bank to Prepare/Unprepare.
    afxBankCallbackFunc  in_pfnBankCallback,                         ///< Callback function
    void* in_pCookie,                                 ///< Callback cookie (reserved to user, passed to the callback function)
    afxBankContent  in_uFlags,  ///< Structures only (including events) or all content.
    afxBankType in_bankType   ///< Type of the bank to Prepare/Unprepare.
);

AFX afxResult PrepareBank___
(
    afxPreparationType in_PreparationType, ///< Preparation type ( Preparation_Load or Preparation_Unload )
    afxBankID in_bankID, ///< ID of the bank to Prepare/Unprepare.
    afxBankCallbackFunc  in_pfnBankCallback, ///< Callback function
    void* in_pCookie, ///< Callback cookie (reserved to user, passed to the callback function)
    afxBankContent  in_uFlags, ///< Structures only (including events) or all content.
    afxBankType in_bankType ///< Type of the bank to Prepare/Unprepare.
);






/// Asynchronously posts an Event to the sound engine (by event ID).
/// The callback function can be used to be noticed when markers are reached or when the event is finished.
/// An array of wave file sources can be provided to resolve External Sources triggered by the event. 
/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
/// If used, the array of external sources should contain the information for each external source triggered by the event. When triggering an event with multiple external sources, you need to differentiate each source by using the cookie property in the External Source in the Wwise project and in AkExternalSourceInfo.
/// \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project (therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures.

AFX_EXTERN_API_FUNC(afxPlayingID, PostEvent)
(
    afxUniqueID in_eventID,                          ///< Unique ID of the event
    afxGameObjectID in_gameObjectID,                 ///< Associated game object ID
    afxNat32 in_uFlags,                         ///< Bitmask: see \ref AkCallbackType
    afxCallbackFunc in_pfnCallback,           ///< Callback function
    void * in_pCookie,                       ///< Callback cookie that will be sent to the callback function along with additional information
    afxNat32 in_cExternals,                     ///< Optional count of external source structures
    afxSource *in_pExternalSources,///< Optional array of external source resolution information
    afxPlayingID in_PlayingID ///< Optional (advanced users only) Specify the playing ID to target with the event. Will Cause active actions in this event to target an existing Playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any for normal playback.
    );

/// Posts an Event to the sound engine (by Event name), using callbacks.
/// The callback function can be used to be noticed when markers are reached or when the event is finished.
/// An array of Wave file sources can be provided to resolve External Sources triggered by the event. P
/// \return The playing ID of the event launched, or AK_INVALID_PLAYING_ID if posting the event failed and an error will be displayed in the debug console and the Wwise Profiler.
/// If used, the array of external sources should contain the information for each external source triggered by the event. When triggering an Event with multiple external sources, you need to differentiate each source by using the cookie property in the External Source in the Wwise project and in AkExternalSourceInfo.
/// \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project (therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures.

AFX_EXTERN_API_FUNC(afxPlayingID, PostEvent_)
(
    const char* in_pszEventName,                    ///< Name of the event
    afxGameObjectID in_gameObjectID,                 ///< Associated game object ID
    afxNat32 in_uFlags,                         ///< Bitmask: see \ref AkCallbackType
    afxCallbackFunc in_pfnCallback,           ///< Callback function
    void * in_pCookie,                       ///< Callback cookie that will be sent to the callback function along with additional information.
    afxNat32 in_cExternals,                     ///< Optional count of external source structures
    afxSource *in_pExternalSources, ///< Optional array of external source resolution information
    afxPlayingID in_PlayingID ///< Optional (advanced users only) Specify the playing ID to target with the event. Will Cause active actions in this event to target an existing Playing ID. Let it be AK_INVALID_PLAYING_ID or do not specify any for normal playback.
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
    afxUniqueID in_eventID,                                              ///< Unique ID of the event

    afxActionOnEventType in_ActionType,                                  ///< Action to execute on all the elements that were played using the specified event.

    afxGameObjectID in_gameObjectID,            ///< Associated game object ID

    afxMillisecond in_uTransitionDuration,                                ///< Fade duration

    /*afxCurveInterpolation*/int in_eFadeCurve,   ///< Curve type to be used for the transition

    afxPlayingID in_PlayingID                    ///< Associated PlayingID
    );

/// Executes an Action on all nodes that are referenced in the specified Event in an Action of type Play.

AFX_EXTERN_API_FUNC(afxResult, ExecuteActionOnEvent_)
(
    const char* in_pszEventName,                                        ///< Name of the event
    afxActionOnEventType in_ActionType,                                  ///< Action to execute on all the elements that were played using the specified event.
    afxGameObjectID in_gameObjectID,            ///< Associated game object ID
    afxMillisecond in_uTransitionDuration,                                ///< Fade duration
    /*afxCurveInterpolation*/int in_eFadeCurve,   ///< Curve type to be used for the transition
    afxPlayingID in_PlayingID                    ///< Associated PlayingID
    );



#endif//AFX_SOUND_SYSTEM_H