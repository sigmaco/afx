#include "../../afxQwadro.h"
#include "afxStdSoundDriverImplementation.h"
#ifdef AFX_PLAT_W64
//#pragma comment(lib, "../../sound/openal/x64/openal32.lib")
#else 
//#pragma comment(lib, "../../sound/openal/x86/openal32.lib")
#endif

HMODULE openal32 = NIL;
LPALCCREATECONTEXT salcCreateContext = NIL;
LPALCMAKECONTEXTCURRENT salcMakeContextCurrent = NIL;
LPALCPROCESSCONTEXT salcProcessContext = NIL;
LPALCSUSPENDCONTEXT salcSuspendContext = NIL;
LPALCDESTROYCONTEXT salcDestroyContext = NIL;
LPALCGETCURRENTCONTEXT salcGetCurrentContext = NIL;
LPALCGETCONTEXTSDEVICE salcGetContextsDevice = NIL;
LPALCOPENDEVICE salcOpenDevice = NIL;
LPALCCLOSEDEVICE salcCloseDevice = NIL;
LPALCGETERROR salcGetError = NIL;
LPALCISEXTENSIONPRESENT salcIsExtensionPresent = NIL;
LPALCGETPROCADDRESS salcGetProcAddress = NIL;
LPALCGETENUMVALUE salcGetEnumValue = NIL;
LPALCGETSTRING salcGetString = NIL;
LPALCGETINTEGERV salcGetIntegerv = NIL;
LPALCCAPTUREOPENDEVICE salcCaptureOpenDevice = NIL;
LPALCCAPTURECLOSEDEVICE salcCaptureCloseDevice = NIL;
LPALCCAPTURESTART salcCaptureStart = NIL;
LPALCCAPTURESTOP salcCaptureStop = NIL;
LPALCCAPTURESAMPLES salcCaptureSamples = NIL;

afxBool _AfxLoadOpenal(void)
{
    if (!openal32 && !(openal32 = LoadLibraryA("soft_oal.dll"))) AfxThrowError();
    else
    {
        salcGetProcAddress = (void*)GetProcAddress(openal32, "alcGetProcAddress");

        salcCreateContext = salcGetProcAddress(NIL, "alcCreateContext");
        salcMakeContextCurrent = salcGetProcAddress(NIL, "alcMakeContextCurrent");
        salcProcessContext = salcGetProcAddress(NIL, "alcProcessContext");
        salcSuspendContext = salcGetProcAddress(NIL, "alcSuspendContext");
        salcDestroyContext = salcGetProcAddress(NIL, "alcDestroyContext");
        salcGetCurrentContext = salcGetProcAddress(NIL, "alcGetCurrentContext");
        salcGetContextsDevice = salcGetProcAddress(NIL, "alcGetContextsDevice");
        salcOpenDevice = salcGetProcAddress(NIL, "alcOpenDevice");
        salcCloseDevice = salcGetProcAddress(NIL, "alcCloseDevice");
        salcGetError = salcGetProcAddress(NIL, "alcGetError");
        salcIsExtensionPresent = salcGetProcAddress(NIL, "alcIsExtensionPresent");
        salcGetEnumValue = salcGetProcAddress(NIL, "alcGetEnumValue");
        salcGetString = salcGetProcAddress(NIL, "alcGetString");
        salcGetIntegerv = salcGetProcAddress(NIL, "alcGetIntegerv");
        salcCaptureOpenDevice = salcGetProcAddress(NIL, "alcCaptureOpenDevice");
        salcCaptureCloseDevice = salcGetProcAddress(NIL, "alcCaptureCloseDevice");
        salcCaptureStart = salcGetProcAddress(NIL, "alcCaptureStart");
        salcCaptureStop = salcGetProcAddress(NIL, "alcCaptureStop");
        salcCaptureSamples = salcGetProcAddress(NIL, "alcCaptureSamples");
        return TRUE;
    }
    return FALSE;
}

static char const *alsyms_v1[] =
{
    "alEnable",
    "alDisable",
    "alIsEnabled",
    "alGetString",
    "alGetBooleanv",
    "alGetIntegerv",
    "alGetFloatv",
    "alGetDoublev",
    "alGetBoolean",
    "alGetInteger",
    "alGetFloat",
    "alGetDouble",
    "alGetError",
    "alIsExtensionPresent",
    "alGetProcAddress",
    "alGetEnumValue",
    "alListenerf",
    "alListener3f",
    "alListenerfv",
    "alListeneri",
    "alListener3i",
    "alListeneriv",
    "alGetListenerf",
    "alGetListener3f",
    "alGetListenerfv",
    "alGetListeneri",
    "alGetListener3i",
    "alGetListeneriv",
    "alGenSources",
    "alDeleteSources",
    "alIsSource",
    "alSourcef",
    "alSource3f",
    "alSourcefv",
    "alSourcei",
    "alSource3i",
    "alSourceiv",
    "alGetSourcef",
    "alGetSource3f",
    "alGetSourcefv",
    "alGetSourcei",
    "alGetSource3i",
    "alGetSourceiv",
    "alSourcePlayv",
    "alSourceStopv",
    "alSourceRewindv",
    "alSourcePausev",
    "alSourcePlay",
    "alSourceStop",
    "alSourceRewind",
    "alSourcePause",
    "alSourceQueueBuffers",
    "alSourceUnqueueBuffers",
    "alGenBuffers",
    "alDeleteBuffers",
    "alIsBuffer",
    "alBufferData",
    "alBufferf",
    "alBuffer3f",
    "alBufferfv",
    "alBufferi",
    "alBuffer3i",
    "alBufferiv",
    "alGetBufferf",
    "alGetBuffer3f",
    "alGetBufferfv",
    "alGetBufferi",
    "alGetBuffer3i",
    "alGetBufferiv"
};

static char const *alsyms_efx[] =
{
    "alGenEffects",
    "alDeleteEffects",
    "alIsEffect",
    "alEffecti",
    "alEffectiv",
    "alEffectf",
    "alEffectfv",
    "alGetEffecti",
    "alGetEffectiv",
    "alGetEffectf",
    "alGetEffectfv",
    "alGenFilters",
    "alDeleteFilters",
    "alIsFilter",
    "alFilteri",
    "alFilteriv",
    "alFilterf",
    "alFilterfv",
    "alGetFilteri",
    "alGetFilteriv",
    "alGetFilterf",
    "alGetFilterfv",

    "alGetAuxiliaryEffectSlots",
    "alDeleteAuxiliaryEffectSlots",
    "alIsAuxiliaryEffectSlot",
    "alAuxiliaryEffectSloti",
    "alAuxiliaryEffectSlotiv",
    "alAuxiliaryEffectSlotf",
    "alAuxiliaryEffectSlotfv",
    "alGetAuxiliaryEffectSloti",
    "alGetAuxiliaryEffectSlotiv",
    "alGetAuxiliaryEffectSlotf",
    "alGetAuxiliaryEffectSlotfv"
};

afxResult _SalLoadVmtSubset(salVmt* vmt, afxNat base, afxNat cnt, afxChar const *names[])
{
    afxError err = NIL;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (!(vmt->ptr[base + i] = salcGetProcAddress(NIL, names[i])))
        {
            AfxError("%s @ %i not found.", names[i], i);
            AfxThrowError();
        }
    }
    AfxCatchError(rslt);
    return rslt;
}

void _SalLoadVmt(salVmt* vmt, afxNat verMajor, afxNat verMinor)
{
    AfxEntry("vmt=%p,verMajor=%u,verMinor=%u", vmt, verMajor, verMinor);

    if (!openal32)
        _AfxLoadOpenal();

    if (!openal32) AfxThrowError();
    else
    {
        int base = 0;

        if (verMajor >= 1)
        {
            _SalLoadVmtSubset(vmt, base, SAL_VMT_SIZE_1, alsyms_v1);
            base += SAL_VMT_SIZE_1;

            _SalLoadVmtSubset(vmt, base, SAL_VMT_SIZE_EFX, alsyms_efx);
            base += SAL_VMT_SIZE_EFX;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// SOUND SYSTEM                                                               //
////////////////////////////////////////////////////////////////////////////////

SAL afxResult _SalFindSctxCallback(afxSoundContext sctx, void* data)
{
    struct { void* handle; afxSoundContext found; } *data2 = data;

    if (AfxDeref2(sctx, afxSoundContext)->data && AfxDeref2(sctx, afxSoundContext)->data->activeOalCtx == data2->handle)
    {
        data2->found = sctx;
        return 0; // break
    }
    return 1; // continue
}

SAL afxSoundContext _AfxStdSsysImplSctxFind(afxSoundSystem ssys, void* handle)
{
    AfxAssertObject(ssys, AFX_FCC_SSYS);
    AfxAssert(handle);
    struct { void* handle; afxSoundContext found; } data = { handle, NIL };
    AfxSoundSystem_ForEachOutput(ssys, (void*)_SalFindSctxCallback, &data);
    return data.found;
}

SAL afxResult _SalFindSoutCallback(afxSoundOutput sout, void* data)
{
    struct { void* handle; afxSoundOutput found; } *data2 = data;

    if (AfxDeref2(sout, afxSoundOutput)->data && AfxDeref2(sout, afxSoundOutput)->data->oalDev == data2->handle)
    {
        data2->found = sout;
        return 0; // break
    }
    return 1; // continue
}

SAL afxSoundOutput _AfxStdSsysImplSoutFind(afxSoundSystem ssys, void* handle)
{
    AfxAssertObject(ssys, AFX_FCC_SSYS);
    AfxAssert(handle);
    struct { void* handle; afxSoundOutput found; } data = { handle, NIL };
    AfxSoundSystem_ForEachOutput(ssys, (void*)_SalFindSoutCallback, &data);
    return data.found;
}

SAL afxSoundInput _AfxStdSsysImplSinFind(afxSoundSystem ssys, void* handle)
{
    AfxAssertObject(ssys, AFX_FCC_SSYS);
    AfxAssert(handle);
    return NIL;
}

SAL afxResult _AfxSsysProcessSctxInstanceCb(afxSoundContext sctx, afxSoundSystem ssys)
{
    AfxAssertObject(sctx, AFX_FCC_SCTX);
    (void)ssys;
    AfxSoundContext_Process(sctx);
    return AFX_SUCCESS;
}

SAL afxResult _AfxStdSsysImplProcess(afxSoundSystem ssys)
{
    AfxAssertObject(ssys, AFX_FCC_SSYS);
    AfxClass_ForEachInstance(AfxSoundSystem_GetContextClass(ssys), _AfxSsysProcessSctxInstanceCb, ssys);
    return AFX_SUCCESS;
}

SAL afxResult _AfxStdSsysImplDtor(afxSoundSystem ssys)
{
    AfxAssertObject(ssys, AFX_FCC_SSYS);
    AfxEntry("ssys=%p", ssys);

    salcMakeContextCurrent(NIL);
    salcDestroyContext(AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeCtx);
    salcCloseDevice(AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeDev);

    return AFX_SUCCESS;
}

SAL afxSoundSystemImplementation const _AfxStdSsysImpl;

afxSoundSystemImplementation const _AfxStdSsysImpl =
{
    _AfxStdSsysImplDtor,
    _AfxStdSsysImplProcess,
    _AfxStdSsysImplSinFind,
    _AfxStdSsysImplSoutFind,
    _AfxStdSsysImplSctxFind,

    0,//_AfxStdSinImplCtor,
    0,//_AfxStdSoutImplCtor,
    0//_AfxStdSctxImplCtor
};

SAL afxResult _AfxSsysImplCtor(afxSoundSystem ssys)
{
    AfxAssertObject(ssys, AFX_FCC_SSYS);
    afxResult rslt = 0;
    AfxDeref2(ssys, afxSoundSystem)->impl = &_AfxStdSsysImpl;

    if (!(AfxDeref2(ssys, afxSoundSystem)->data = AfxAllocate(NIL, sizeof(*AfxDeref2(ssys, afxSoundSystem)->data), AfxSpawnHint()))) AfxThrowError();
    else
    {
        AFX_ZERO(AfxDeref2(ssys, afxSoundSystem)->data);

        if (!openal32)
            _AfxLoadOpenal();

        ALCchar const *defaultDeviceName = salcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

        if (!(AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeDev = salcOpenDevice(defaultDeviceName))) AfxThrowError();
        else
        {
            if (!(AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeCtx = salcCreateContext(AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeDev, NULL))) AfxThrowError();
            else
            {
                if (!salcMakeContextCurrent(AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeCtx)) AfxThrowError();
                else
                {
                    afxString ver;
                    AfxMapStringConst(&ver, 0, (afxChar const*)salcGetString(NIL, AL_VERSION));
                    AfxScanString(&ver, "%u.%u.%u", &AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeCtxVerMajor, &AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeCtxVerMinor, &AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeCtxVerPatch);

                    afxString rnd;
                    AfxMapStringConst(&rnd, 0, (afxChar const*)salcGetString(NIL, AL_RENDERER));
                    AfxComment("SIGMA/OpenAL Sound System over %.*s v%.*s", AfxPushString(&rnd), AfxPushString(&ver));

                    AfxDeref2(ssys, afxSoundSystem)->data->instanced = TRUE;
                    return rslt;
                }
                salcMakeContextCurrent(NIL);
                salcDestroyContext(AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeCtx);
            }
            salcCloseDevice(AfxDeref2(ssys, afxSoundSystem)->data->oalPrimeDev);
        }
        AfxDeallocate(AfxDeref2(ssys, afxSoundSystem)->data);
        AfxDeref2(ssys, afxSoundSystem)->data = NIL;
    }
    AfxDeref2(ssys, afxSoundSystem)->impl = NIL;
    AfxCatchError(rslt);
    return rslt;
}
