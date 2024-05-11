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

#ifndef A4D_SDEV_H
#define A4D_SDEV_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#define AL_LIBTYPE_STATIC 1
#include "al/al.h"
#include "al/alc.h"
#include "al/efx.h"

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AFX_FENCE_C
#define _AFX_FENCE_IMPL
#define _AFX_SEMAPHORE_C
#define _AFX_SEMAPHORE_IMPL

#define _ASX_SOUND_C
#define _ASX_SOUND_SYSTEM_C
#define _ASX_SOUND_DEVICE_C
#define _ASX_SOUND_BRIDGE_C
#define _ASX_SOUND_BRIDGE_IMPL
#define _ASX_SOUND_QUEUE_C
#define _ASX_SOUND_QUEUE_IMPL
#define _ASX_SOUND_DEVICE_IMPL
#define _ASX_SOUND_CONTEXT_C
#define _ASX_SOUND_CONTEXT_IMPL
#define _ASX_SOUND_BUFFER_C
#define _ASX_SOUND_BUFFER_IMPL
#define _ASX_SOUND_IMPL
#include "qwadro/sound/afxSoundSystem.h"
#include "qwadro/core/afxSystem.h"

#ifndef ASX_DRV_SRC
#   ifdef _DEBUG
#       define A4D DLLIMPORT extern 
#       define A4DINL DLLIMPORT EMBED
#   else
#       define A4D DLLIMPORT extern 
#       define A4DINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _A4D DLLEXPORT
#       define A4D DLLEXPORT extern 
#       define _A4DINL DLLEXPORT INLINE
#       define A4DINL DLLEXPORT EMBED
#   else
#       define _A4D DLLEXPORT
#       define A4D DLLEXPORT extern 
#       define _A4DINL DLLEXPORT INLINE
#       define A4DINL DLLEXPORT EMBED
#   endif
#endif

AFX_DEFINE_UNION(alVmt)
{
    struct
    {
        // 110
        LPALENABLE Enable;
        LPALDISABLE Disable;
        LPALISENABLED IsEnabled;
        LPALGETSTRING GetString;
        LPALGETBOOLEANV GetBooleanv;
        LPALGETINTEGERV GetIntegerv;
        LPALGETFLOATV GetFloatv;
        LPALGETDOUBLEV GetDoublev;
        LPALGETBOOLEAN GetBoolean;
        LPALGETINTEGER GetInteger;
        LPALGETFLOAT GetFloat;
        LPALGETDOUBLE GetDouble;
        LPALGETERROR GetError;
        LPALISEXTENSIONPRESENT IsExtensionPresent;
        LPALGETPROCADDRESS GetProcAddress;
        LPALGETENUMVALUE GetEnumValue;
        LPALLISTENERF Listenerf;
        LPALLISTENER3F Listener3f;
        LPALLISTENERFV Listenerfv;
        LPALLISTENERI Listeneri;
        LPALLISTENER3I Listener3i;
        LPALLISTENERIV Listeneriv;
        LPALGETLISTENERF GetListenerf;
        LPALGETLISTENER3F GetListener3f;
        LPALGETLISTENERFV GetListenerfv;
        LPALGETLISTENERI GetListeneri;
        LPALGETLISTENER3I GetListener3i;
        LPALGETLISTENERIV GetListeneriv;
        LPALGENSOURCES GenSources;
        LPALDELETESOURCES DeleteSources;
        LPALISSOURCE IsSource;
        LPALSOURCEF Sourcef;
        LPALSOURCE3F Source3f;
        LPALSOURCEFV Sourcefv;
        LPALSOURCEI Sourcei;
        LPALSOURCE3I Source3i;
        LPALSOURCEIV Sourceiv;
        LPALGETSOURCEF GetSourcef;
        LPALGETSOURCE3F GetSource3f;
        LPALGETSOURCEFV GetSourcefv;
        LPALGETSOURCEI GetSourcei;
        LPALGETSOURCE3I GetSource3i;
        LPALGETSOURCEIV GetSourceiv;
        LPALSOURCEPLAYV SourcePlayv;
        LPALSOURCESTOPV SourceStopv;
        LPALSOURCEREWINDV SourceRewindv;
        LPALSOURCEPAUSEV SourcePausev;
        LPALSOURCEPLAY SourcePlay;
        LPALSOURCESTOP SourceStop;
        LPALSOURCEREWIND SourceRewind;
        LPALSOURCEPAUSE SourcePause;
        LPALSOURCEQUEUEBUFFERS SourceQueueBuffers;
        LPALSOURCEUNQUEUEBUFFERS SourceUnqueueBuffers;
        LPALGENBUFFERS GenBuffers;
        LPALDELETEBUFFERS DeleteBuffers;
        LPALISBUFFER IsBuffer;
        LPALBUFFERDATA BufferData;
        LPALBUFFERF Bufferf;
        LPALBUFFER3F Buffer3f;
        LPALBUFFERFV Bufferfv;
        LPALBUFFERI Bufferi;
        LPALBUFFER3I Buffer3i;
        LPALBUFFERIV Bufferiv;
        LPALGETBUFFERF GetBufferf;
        LPALGETBUFFER3F GetBuffer3f;
        LPALGETBUFFERFV GetBufferfv;
        LPALGETBUFFERI GetBufferi;
        LPALGETBUFFER3I GetBuffer3i;
        LPALGETBUFFERIV GetBufferiv;
        LPALDOPPLERFACTOR DopplerFactor;
        LPALDOPPLERVELOCITY DopplerVelocity;
        LPALSPEEDOFSOUND SpeedOfSound;
        LPALDISTANCEMODEL DistanceModel;

        // EFX
        LPALGENEFFECTS GenEffects;
        LPALDELETEEFFECTS alDeleteEffects;
        LPALISEFFECT alIsEffect;
        LPALEFFECTI alEffecti;
        LPALEFFECTIV alEffectiv;
        LPALEFFECTF alEffectf;
        LPALEFFECTFV alEffectfv;
        LPALGETEFFECTI alGetEffecti;
        LPALGETEFFECTIV alGetEffectiv;
        LPALGETEFFECTF alGetEffectf;
        LPALGETEFFECTFV alGetEffectfv;

        LPALGENFILTERS GenFilters;
        LPALDELETEFILTERS DeleteEffects;
        LPALISFILTER IsFilter;
        LPALFILTERI Filteri;
        LPALFILTERIV Filteriv;
        LPALFILTERF Filterf;
        LPALFILTERFV Filterfv;
        LPALGETFILTERI GetFilteri;
        LPALGETFILTERIV GetFilteriv;
        LPALGETFILTERF GetFilterf;
        LPALGETFILTERFV GetFilterfv;

        LPALGENAUXILIARYEFFECTSLOTS GetAuxiliaryEffectSlots;
        LPALDELETEAUXILIARYEFFECTSLOTS DeleteAuxiliaryEffectSlots;
        LPALISAUXILIARYEFFECTSLOT IsAuxiliaryEffectSlot;
        LPALAUXILIARYEFFECTSLOTI AuxiliaryEffectSloti;
        LPALAUXILIARYEFFECTSLOTIV AuxiliaryEffectSlotiv;
        LPALAUXILIARYEFFECTSLOTF AuxiliaryEffectSlotf;
        LPALAUXILIARYEFFECTSLOTFV AuxiliaryEffectSlotfv;
        LPALGETAUXILIARYEFFECTSLOTI GetAuxiliaryEffectSloti;
        LPALGETAUXILIARYEFFECTSLOTIV GetAuxiliaryEffectSlotiv;
        LPALGETAUXILIARYEFFECTSLOTF GetAuxiliaryEffectSlotf;
        LPALGETAUXILIARYEFFECTSLOTFV GetAuxiliaryEffectSlotfv;
    };
    void* ptr;
};

AFX_DEFINE_STRUCT(salSpu)
{
    struct
    {
        LPALCCREATECONTEXT CreateContext;
        LPALCMAKECONTEXTCURRENT MakeContextCurrent;
        LPALCPROCESSCONTEXT ProcessContext;
        LPALCSUSPENDCONTEXT SuspendContext;
        LPALCDESTROYCONTEXT DestroyContext;
        LPALCGETCURRENTCONTEXT GetCurrentContext;
        LPALCGETCONTEXTSDEVICE GetContextsDevice;
        LPALCOPENDEVICE OpenDevice;
        LPALCCLOSEDEVICE CloseDevice;
        LPALCGETERROR GetError;
        LPALCISEXTENSIONPRESENT IsExtensionPresent;
        LPALCGETPROCADDRESS GetProcAddress;
        LPALCGETENUMVALUE GetEnumValue;
        LPALCGETSTRING GetString;
        LPALCGETINTEGERV GetIntegerv;
        LPALCCAPTUREOPENDEVICE CaptureOpenDevice;
        LPALCCAPTURECLOSEDEVICE CaptureCloseDevice;
        LPALCCAPTURESTART CaptureStart;
        LPALCCAPTURESTOP CaptureStop;
        LPALCCAPTURESAMPLES CaptureSamples;
    } alc;
    alVmt const al;

    ALCdevice* alcdev;
    ALCcontext* alctx;

    afxSoundContext activeSctx;

    afxNat  verMajor, verMinor, verPatch;
    afxBool eaxEnabled;
};

struct _afxSdevIdd
{
    afxModule   openal32;
    afxNat      spuCnt;
    salSpu*     spus;
    afxThread   dedThread;
};

AFX_DEFINE_STRUCT(_salQueueing)
{
    afxNat                  siz;
    afxLinkage              chain;
    afxNat                  submNo; // this submission number ordinal (B2F)
    afxNat                  reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime                 pushTime; // submission (into input) time
    afxTime                 pullTime; // fecth (by queue) time
    afxTime                 complTime; // completation time
    afxError(*exec)(salSpu*, afxSoundBridge, afxNat queIdx, _salQueueing*);
};

AFX_DEFINE_STRUCT(_salQueueingMmap)
{
    _salQueueing            hdr;
    afxSoundBuffer          buf;
    afxSize                 off;
    afxNat                  ran;
    afxFlags                flags;
};

AFX_DEFINE_STRUCT(_salQueueingExecute)
{
    _salQueueing            hdr;
    afxNat                  itemCnt;
    afxExecutionRequest2    afxSimd(items[]);
};

AFX_DEFINE_STRUCT(_salQueueingTransfer)
{
    _salQueueing            hdr;
    afxNat                  itemCnt;
    afxTransferRequest2     afxSimd(items[]);
};

AFX_OBJECT(afxSoundContext)
{
    struct _afxBaseSoundContext base;
};

AFX_OBJECT(afxSoundBridge)
{
    struct _afxBaseSoundBridge base;

};

AFX_OBJECT(afxFence)
{
    struct _afxBaseFence base;
    //sglUpdateFlags  updFlags;
    //GLsync          glHandle;
};

AFX_OBJECT(afxSemaphore)
{
    struct _afxBaseSemaphore base;
};

AFX_OBJECT(afxSound)
{
    struct _afxBaseSound base;

};

AFX_OBJECT(afxSoundBuffer)
{
    struct _afxBaseSoundBuffer base;

};

A4D afxError _SdgeProcCb(afxSoundBridge sdge, afxThread thr);

#endif//A4D_SDEV_H
