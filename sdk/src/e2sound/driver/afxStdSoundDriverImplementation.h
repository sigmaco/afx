#ifndef AFX_STD_SOUND_DRIVER_IMPLEMENTATION_H
#define AFX_STD_SOUND_DRIVER_IMPLEMENTATION_H


#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>


#include "../afxSoundSystem.h"

#define SAL AFX
#define ALC_API SAL
#define AL_LIBTYPE_STATIC 1
#include "../../deps/al/al.h"
#include "../../deps/al/alc.h"
#include "../../deps/al/efx.h"

SAL HMODULE openal32;
SAL afxBool _AfxLoadOpenal(void);
SAL LPALCCREATECONTEXT salcCreateContext;
SAL LPALCMAKECONTEXTCURRENT salcMakeContextCurrent;
SAL LPALCPROCESSCONTEXT salcProcessContext;
SAL LPALCSUSPENDCONTEXT salcSuspendContext;
SAL LPALCDESTROYCONTEXT salcDestroyContext;
SAL LPALCGETCURRENTCONTEXT salcGetCurrentContext;
SAL LPALCGETCONTEXTSDEVICE salcGetContextsDevice;
SAL LPALCOPENDEVICE salcOpenDevice;
SAL LPALCCLOSEDEVICE salcCloseDevice;
SAL LPALCGETERROR salcGetError;
SAL LPALCISEXTENSIONPRESENT salcIsExtensionPresent;
SAL LPALCGETPROCADDRESS salcGetProcAddress;
SAL LPALCGETENUMVALUE salcGetEnumValue;
SAL LPALCGETSTRING salcGetString;
SAL LPALCGETINTEGERV salcGetIntegerv;
SAL LPALCCAPTUREOPENDEVICE salcCaptureOpenDevice;
SAL LPALCCAPTURECLOSEDEVICE salcCaptureCloseDevice;
SAL LPALCCAPTURESTART salcCaptureStart;
SAL LPALCCAPTURESTOP salcCaptureStop;
SAL LPALCCAPTURESAMPLES salcCaptureSamples;

#define SAL_VMT_SIZE_1 69
#define SAL_VMT_SIZE_EFX 33



typedef union salVmt
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
        LPALGENSOURCES GetSources;
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
    };
    struct
    {
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
    void* ptr[1];
} salVmt;

struct afxSoundSystemImplementationData
{
    afxBool     instanced;

    ALCdevice   *oalPrimeDev;
    ALCcontext  *oalPrimeCtx;
    afxNat      oalPrimeCtxVerMajor, oalPrimeCtxVerMinor, oalPrimeCtxVerPatch;
    //salVmt      vmt;

    void*       udd; // user-defined data
};

struct afxSoundContextImplementationData
{
    afxBool instanced;

    // Pointers and handles to active non-owned OpenGL resources. Don't release them.
    ALCdevice   *activeOalDev;
    ALCcontext  *activeOalCtx;
    //salVmt      *activeOalVmt;

    void*   udd; // user-defined data
};

struct afxSoundOutputImplementationData
{
    afxBool     instanced;

    ALCdevice   *oalDev;
    ALCcontext  *oalCtx;
    afxNat      oalCtxVerMajor, oalCtxVerMinor, oalCtxVerPatch;
    //salVmt      vmt;

    void*       udd; // user-defined data
};

struct afxSoundInputImplementationData
{
    afxBool instanced;
    void*   udd; // user-defined data
    union
    {
        struct
        {
            void *dummy;
        } dummy;
    };
};

#endif//AFX_STD_SOUND_DRIVER_IMPLEMENTATION_H