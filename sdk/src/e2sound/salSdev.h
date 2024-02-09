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

#ifndef A4D_SDEV_H
#define A4D_SDEV_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#define AL_LIBTYPE_STATIC 1
#include "../dep/al/al.h"
#include "../dep/al/alc.h"
#include "../dep/al/efx.h"

#define _AFX_CORE_C
#define _AFX_ICD_C
#define _AFX_MODULE_C
#define _AFX_SOUND_C
#define _AFX_THREAD_C
#define _AFX_SOUND_THREAD_C
#define _AFX_SOUND_SYSTEM_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_SOUND_DEVICE_C
#define _AFX_SOUND_DEVICE_IMPL
#define _AFX_SOUND_CONTEXT_C
#define _AFX_SOUND_CONTEXT_IMPL
#define _AFX_SOUND_BUFFER_C
#define _AFX_SOUND_BUFFER_IMPL
#define _AFX_SOUND_IMPL
#include "qwadro/mem/afxArena.h"
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"

#ifndef __e2sound__
#   ifdef _DEBUG
#       define A4D _AFXIMPORT extern 
#       define A4DINL _AFXIMPORT extern inline
#   else
#       define A4D _AFXIMPORT extern 
#       define A4DINL _AFXIMPORT extern inline
#   endif
#else
#   ifdef _DEBUG
#       define _A4D _AFXEXPORT
#       define A4D _AFXEXPORT extern 
#       define _A4DINL _AFXEXPORT inline
#       define A4DINL _AFXEXPORT extern inline
#   else
#       define _A4D _AFXEXPORT
#       define A4D _AFXEXPORT extern 
#       define _A4DINL _AFXEXPORT inline
#       define A4DINL _AFXEXPORT extern inline
#   endif
#endif//__e2sound__

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

AFX_DEFINE_STRUCT(salSpuIdd)
{
    HANDLE openal32;
    LPALCCREATECONTEXT alcCreateContext;
    LPALCMAKECONTEXTCURRENT alcMakeContextCurrent;
    LPALCPROCESSCONTEXT alcProcessContext;
    LPALCSUSPENDCONTEXT alcSuspendContext;
    LPALCDESTROYCONTEXT alcDestroyContext;
    LPALCGETCURRENTCONTEXT alcGetCurrentContext;
    LPALCGETCONTEXTSDEVICE alcGetContextsDevice;
    LPALCOPENDEVICE alcOpenDevice;
    LPALCCLOSEDEVICE alcCloseDevice;
    LPALCGETERROR alcGetError;
    LPALCISEXTENSIONPRESENT alcIsExtensionPresent;
    LPALCGETPROCADDRESS alcGetProcAddress;
    LPALCGETENUMVALUE alcGetEnumValue;
    LPALCGETSTRING alcGetString;
    LPALCGETINTEGERV alcGetIntegerv;
    LPALCCAPTUREOPENDEVICE alcCaptureOpenDevice;
    LPALCCAPTURECLOSEDEVICE alcCaptureCloseDevice;
    LPALCCAPTURESTART alcCaptureStart;
    LPALCCAPTURESTOP alcCaptureStop;
    LPALCCAPTURESAMPLES alcCaptureSamples;
    alVmt const al;

    ALCdevice* alcd;
    ALCcontext* alc;

    afxNat  verMajor, verMinor, verPatch;
    afxBool eaxEnabled;
};

AFX_OBJECT(afxSoundDevice)
{
    struct _afxBaseSoundDevice base;
    afxNat      spuCnt;
    salSpuIdd*  spus;
};

AFX_OBJECT(afxSoundContext)
{
    struct _afxBaseSoundContext base;

};

AFX_OBJECT(afxSound)
{
    struct _afxBaseSound base;

};

AFX_OBJECT(afxSoundBuffer)
{
    struct _afxBaseSoundBuffer base;

};

#endif//A4D_SDEV_H
