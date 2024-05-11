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

#include "salSdev.h"

extern afxClassConfig const _SalSctxMgrCfg;
extern afxClassConfig const _SalSdgeMgrCfg;

static char const *alVmtNames[] =
{
    // v1
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
    "alGetBufferiv",

    // EFX
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

_A4D void _SalSpuLoadBaseSymbols(afxSoundDevice sdev, afxNat unitIdx)
{
    salSpu *spu = &sdev->idd->spus[unitIdx];
    HMODULE openal32 = AfxGetModuleIdd(sdev->idd->openal32);
    spu->alc.GetProcAddress = (void*)GetProcAddress(openal32, "alcGetProcAddress");

    spu->alc.CreateContext = spu->alc.GetProcAddress(NIL, "alcCreateContext");
    spu->alc.MakeContextCurrent = spu->alc.GetProcAddress(NIL, "alcMakeContextCurrent");
    spu->alc.ProcessContext = spu->alc.GetProcAddress(NIL, "alcProcessContext");
    spu->alc.SuspendContext = spu->alc.GetProcAddress(NIL, "alcSuspendContext");
    spu->alc.DestroyContext = spu->alc.GetProcAddress(NIL, "alcDestroyContext");
    spu->alc.GetCurrentContext = spu->alc.GetProcAddress(NIL, "alcGetCurrentContext");
    spu->alc.GetContextsDevice = spu->alc.GetProcAddress(NIL, "alcGetContextsDevice");
    spu->alc.OpenDevice = spu->alc.GetProcAddress(NIL, "alcOpenDevice");
    spu->alc.CloseDevice = spu->alc.GetProcAddress(NIL, "alcCloseDevice");
    spu->alc.GetError = spu->alc.GetProcAddress(NIL, "alcGetError");
    spu->alc.IsExtensionPresent = spu->alc.GetProcAddress(NIL, "alcIsExtensionPresent");
    spu->alc.GetEnumValue = spu->alc.GetProcAddress(NIL, "alcGetEnumValue");
    spu->alc.GetString = spu->alc.GetProcAddress(NIL, "alcGetString");
    spu->alc.GetIntegerv = spu->alc.GetProcAddress(NIL, "alcGetIntegerv");
    spu->alc.CaptureOpenDevice = spu->alc.GetProcAddress(NIL, "alcCaptureOpenDevice");
    spu->alc.CaptureCloseDevice = spu->alc.GetProcAddress(NIL, "alcCaptureCloseDevice");
    spu->alc.CaptureStart = spu->alc.GetProcAddress(NIL, "alcCaptureStart");
    spu->alc.CaptureStop = spu->alc.GetProcAddress(NIL, "alcCaptureStop");
    spu->alc.CaptureSamples = spu->alc.GetProcAddress(NIL, "alcCaptureSamples");
}

afxError _SalLoadOpenAlVmt(HMODULE openal32, afxNat base, afxNat cnt, char const *names[], void* vmt[], afxBool echo)
{
    afxError err = AFX_ERR_NONE;

    AfxAssert(names);
    AfxAssert(vmt);
    AfxAssert(openal32);
    
    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(names[base + i]);
        void *sym = NIL;

        if ((sym = (void*)GetProcAddress(openal32, names[base + i])))
        {
            if (echo)
            {
                AfxLogEcho("%.3u %s core found.", i, names[base + i]);
            }
        }

        vmt[base + i] = sym;
    }
    return err;
}

_A4D void _ListAllDevs(salSpu *spu)
{
    if (spu->alc.IsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
    {
        ALCchar const *pDeviceNames = spu->alc.GetString(NULL, ALC_DEVICE_SPECIFIER);
        ALCchar const *pDefaultDevice = spu->alc.GetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

        if (strlen(pDeviceNames))
        {
            afxNat iDeviceIndex = 0;
            while (pDeviceNames && *pDeviceNames)
            {
                AfxLogEcho("%-2d - %s", iDeviceIndex, pDeviceNames);
                iDeviceIndex++;
                pDeviceNames += strlen(pDeviceNames) + 1;
            }
        }
    }

    if (spu->alc.IsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
    {
        AfxLogEcho("Listing devices available with ALC_ENUMERATE_ALL_EXT");
        ALCchar const *pDeviceNames = spu->alc.GetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
        ALCchar const *pDefaultDevice = spu->alc.GetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);

        if (strlen(pDeviceNames))
        {
            afxNat iDeviceIndex = 0;
            while (pDeviceNames && *pDeviceNames)
            {
                AfxLogEcho("%-2d - %s", iDeviceIndex, pDeviceNames);
                iDeviceIndex++;
                pDeviceNames += strlen(pDeviceNames) + 1;
            }
        }
    }

    // Check for Capture Extension support

    if (spu->alc.IsExtensionPresent(NULL, "ALC_EXT_CAPTURE"))
    {
        // Get list of available Capture Devices
        const ALchar *pDeviceList = spu->alc.GetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);

        if (pDeviceList)
        {
            AfxLogEcho("Listing available capture devices:");

            afxNat iDeviceIndex = 0;
            while (*pDeviceList)
            {
                AfxLogEcho("%-2d - %s", iDeviceIndex, pDeviceList);
                pDeviceList += strlen(pDeviceList) + 1;
                iDeviceIndex++;
            }
        }
    }
}

_A4D afxError _SalBuildSpu(afxSoundDevice sdev, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    //afxNat procUnitIdx = AfxGetThreadingUnit();
    salSpu *spu = &sdev->idd->spus[unitIdx];
    //wglVmt const* wgl = &dpu->wgl;
    alVmt const* al = &spu->al;
    HMODULE openal32 = AfxGetModuleIdd(sdev->idd->openal32);

    _SalSpuLoadBaseSymbols(sdev, unitIdx);
    _ListAllDevs(spu);

    if (!(spu->alcdev = spu->alc.OpenDevice(NULL))) AfxThrowError();
    else
    {
        spu->alc.GetIntegerv(spu->alcdev, ALC_MAJOR_VERSION, sizeof(spu->verMajor), (ALCint*)&spu->verMajor);
        spu->alc.GetIntegerv(spu->alcdev, ALC_MINOR_VERSION, sizeof(spu->verMinor), (ALCint*)&spu->verMinor);
        spu->verPatch = 0;
        spu->eaxEnabled = spu->alc.IsExtensionPresent(spu->alcdev, "ALC_EXT_EFX");

        if (!(spu->alctx = spu->alc.CreateContext(spu->alcdev, NULL))) AfxThrowError();
        else
        {
            if (!spu->alc.MakeContextCurrent(spu->alctx)) AfxThrowError();
            else
            {
                _SalLoadOpenAlVmt(openal32, 0, AFX_COUNTOF(alVmtNames), alVmtNames, (void**)&spu->al.ptr, FALSE/*unitIdx == 0*/);

                afxNat uiBuffer;
                al->GenBuffers(1, &uiBuffer);

                afxNat uiSource;
                al->GenSources(1, &uiSource);

                afxStream fd;
                afxUri uri;
                AfxMakeUri(&uri, "system/qwadroDeepNote", 0);
                fd = AfxOpenFile(&uri, afxIoFlag_R);

                afxNat dataSiz = AfxMeasureStream(fd);
                void *data = AfxAllocate(dataSiz, 1, 0, AfxHere());
                AfxReadStream(fd, dataSiz, 0, data);

                al->BufferData(uiBuffer, AL_FORMAT_STEREO16, data, dataSiz, 44100);
                AfxReleaseObjects(1, (void*[]) { fd });

                al->Sourcei(uiSource, AL_BUFFER, uiBuffer);
                al->SourcePlay(uiSource);
                    
                afxNat iState;
                do
                {
                    Sleep(100);
                    // Get Source State
                    al->GetSourcei(uiSource, AL_SOURCE_STATE, (ALCint*)&iState);
                    break;
                } while (iState == AL_PLAYING);

                al->SourceStop(uiSource);
                al->DeleteSources(1, &uiSource);
                al->DeleteBuffers(1, &uiBuffer);
            }
        }
    }
    
    return err;
}

_A4D afxError _SalDestroySpu(afxSoundDevice sdev, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    salSpu *spu = &sdev->idd->spus[unitIdx];
    alVmt const* al = &spu->al;

    ALCcontext* dc = spu->alc.GetCurrentContext();

    if (dc == spu->alctx)
        spu->alc.MakeContextCurrent(NIL);

    spu->alc.DestroyContext(spu->alctx);
    spu->alc.CloseDevice(spu->alcdev);

    return err;
}

_A4D afxBool _SalProcessSctxCb(afxSoundContext sctx, void *udd)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxThread thr = (afxThread)udd;
    AfxAssertObjects(1, &thr, afxFcc_THR);

    for (afxNat i = 0; i < sctx->base.ownedBridgeCnt; i++)
    {
        afxSoundBridge sdge = sctx->base.ownedBridges[i];
        AfxAssertObjects(1, &sdge, afxFcc_SDGE);
        _SdgeProcCb(sdge, thr);
    }

    return FALSE; // don't interrupt curation;
}

_A4D afxError _SalSdevProcCb(afxSoundDevice sdev, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    //AfxInvokeSoundContexts(sdev, 0, AFX_N32_MAX, _SalProcessSctxCb, (void*)thr);

    afxNat portCnt = sdev->portCnt;

    for (afxNat i = 0; i < portCnt; i++)
    {
        afxManager* mgr = &sdev->ports[i].sdgeMgr;
        AfxAssertClass(mgr, afxFcc_SDGE);
        AfxInvokeObjects(mgr, 0, AFX_N32_MAX, (void*)_SdgeProcCb, thr);
    }

    return err;
}

_A4D afxResult SoundThreadProc(afxThread thr, afxEvent* ev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxSoundDevice sdev = AfxGetThreadUdd(thr);

    switch (ev->id)
    {
    case afxThreadEvent_RUN:
    {
        if (_SalBuildSpu(sdev, 0))
        {
            AfxThrowError();
            AfxExitThread(err);
        }
        else
        {
            sdev->dev.serving = TRUE;
        }
        break;
    }
    case afxThreadEvent_QUIT:
    {
        if (_SalDestroySpu(sdev, 0))
        {
            AfxThrowError();
        }
        else
        {
            sdev->dev.serving = FALSE;
        }
        break;
    }
    default:
    {
        if (sdev->dev.serving)
            _SalSdevProcCb(sdev, thr);

        break;
    }
    }

    return 0;
}

_A4D afxError _SalSdevIddCtorCb(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;

    sdev->idd = NIL;

    if (!(sdev->idd = AfxAllocate(1, sizeof(sdev->idd[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        afxUri uri;
        afxNat devPagIdx, recIdx;
        afxManifest const* ini = AfxGetDeviceManifest(&sdev->dev, &devPagIdx);

        if (!AfxFindManifestRecord(ini, devPagIdx, &AfxString("OpenalDll"), &recIdx) ||
            !AfxGetManifestUri(ini, devPagIdx, recIdx, &uri)) AfxThrowError();
        else
        {
            if (AfxLoadModule(&uri, NIL, &sdev->idd->openal32)) AfxThrowError();
            else
            {
                sdev->idd->spuCnt = sdev->portCnt;

                if (!(sdev->idd->spus = AfxAllocate(sdev->idd->spuCnt, sizeof(sdev->idd->spus[0]), 0, AfxHere()))) AfxThrowError();
                else
                {
                    AfxZero2(sdev->idd->spuCnt, sizeof(sdev->idd->spus[0]), sdev->idd->spus);

                    AfxAssert(sdev->dev.procCb);

                    afxThread sthr;
                    afxThreadConfig stCfg = { 0 };
                    stCfg.procCb = SoundThreadProc;
                    stCfg.udd = sdev;

                    if (AfxAcquireThread(AfxHere(), &stCfg, &sthr)) AfxThrowError();
                    else
                    {
                        sdev->idd->dedThread = sthr;
                        sdev->dev.serving = TRUE;
                        AfxRunThread(sthr);

                        AfxLogY("The audience is listening");
                    }
                    AfxAssert(sdev->dev.procCb);
                }

                if (err)
                    AfxReleaseObjects(1, &sdev->idd->openal32);
            }
        }

        if (err)
            AfxDeallocate(sdev->idd);
    }

    if (err)
        AfxCleanUpChainedManagers(&sdev->dev.classes);
    
    return err;
}

_A4D afxError _SalSdevIddDtorCb(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    AfxCleanUpChainedManagers(&sdev->dev.classes);

    afxResult exitCode;
    AfxRequestThreadInterruption(sdev->idd->dedThread);
    AfxWaitForThread(sdev->idd->dedThread, &exitCode);
    AfxReleaseObjects(1, &sdev->idd->dedThread);

#if 0
    for (afxNat i = 0; i < sdev->idd->spuCnt; i++)
        if (_SalDestroySpu(sdev, i))
            AfxThrowError();
#endif

    AfxDeallocate(sdev->idd->spus);
    sdev->idd->spus = NIL;

    //AfxCleanUpMutex(&sdev->idd->ioConMtx);

    AfxReleaseObjects(1, &sdev->idd->openal32);

    AfxDeallocate(sdev->idd);
    sdev->idd = NIL;

    if (sdev->ports)
        AfxDeallocate(sdev->ports);

    return err;
}

_A4D afxResult AfxDeviceIoctl(afxSoundDevice sdev, afxNat reqCode, va_list va)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxResult rslt = 0;

    switch (reqCode)
    {
    case afxFcc_SSYS:
    {
#if 0
        static afxSoundPortCaps const portCaps[] =
        {
            {
                .queFlags = NIL,
                .queCnt = 16
            },
            {
                .queFlags = NIL,
                .queCnt = 2
            },
        };
#endif

        afxSoundDeviceInfo* info2 = va_arg(va, afxSoundDeviceInfo*);
        AfxAssert(info2);
        info2->procCb = _SalSdevProcCb;
        info2->iddCtorCb = _SalSdevIddCtorCb;
        info2->iddDtorCb = _SalSdevIddDtorCb;
        //info2->soutIddDtorCb = _SalSdevDeinitDoutCb;
        //info2->soutIddCtorCb = _SalSdevInitDoutCb;
        info2->portCnt = 1;
        //info2->portCaps = portCaps;

        info2->sctxClsCfg = &_SalSctxMgrCfg;
        info2->sdgeClsCfg = &_SalSdgeMgrCfg;
        break;
    }
    default: break;
    }
    return rslt;
}
