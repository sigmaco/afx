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

#include "salSdev.h"

extern afxClassConfig const _SalSctxMgrCfg;

_A4D afxChar const sigmaSignature[] =
{
    "\n       ::::::::    :::::::::::    ::::::::    ::::     ::::       :::          "
    "\n      :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:        "
    "\n      +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+       "
    "\n      +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:      "
    "\n             +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+      "
    "\n      #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#      "
    "\n       ########    ###########    ########    ###       ###   ###     ###      "
    "\n                                                                               "
    "\n              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              "
    "\n                                                                               "
    "\n                               Public Test Build                               "
    "\n              (c) 2017 SIGMA Technology Group --- Federacao SIGMA              "
    "\n                                www.sigmaco.org                                "
    "\n                                                                               "
};

_A4D afxChar const a4dSignature[] =
{
    "\n                         :::           :::       :::::::::                     "
    "\n                       :+: :+:        :+:        :+:    :+:                    "
    "\n                      +:+   +:+      +:+ +:+     +:+    +:+                    "
    "\n                     +#++:++#++:    +#+  +:+     +#+    +:+                    "
    "\n                     +#+     +#+   +#+#+#+#+#+   +#+    +#+                    "
    "\n                     #+#     #+#         #+#     #+#    #+#                    "
    "\n                     ###     ###         ###     #########                     "
    "\n                                                                               "
    "\n              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              "
    "\n                                                                               "
    "\n                               Public Test Build                               "
    "\n              (c) 2017 SIGMA Technology Group --- Federacao SIGMA              "
    "\n                                www.sigmaco.org                                "
    "\n                                                                               "
};

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
    salSpuIdd *spu = &sdev->idd->spus[unitIdx];
    HMODULE openal32 = spu->openal32;
    spu->alcGetProcAddress = (void*)GetProcAddress(openal32, "alcGetProcAddress");

    spu->alcCreateContext = spu->alcGetProcAddress(NIL, "alcCreateContext");
    spu->alcMakeContextCurrent = spu->alcGetProcAddress(NIL, "alcMakeContextCurrent");
    spu->alcProcessContext = spu->alcGetProcAddress(NIL, "alcProcessContext");
    spu->alcSuspendContext = spu->alcGetProcAddress(NIL, "alcSuspendContext");
    spu->alcDestroyContext = spu->alcGetProcAddress(NIL, "alcDestroyContext");
    spu->alcGetCurrentContext = spu->alcGetProcAddress(NIL, "alcGetCurrentContext");
    spu->alcGetContextsDevice = spu->alcGetProcAddress(NIL, "alcGetContextsDevice");
    spu->alcOpenDevice = spu->alcGetProcAddress(NIL, "alcOpenDevice");
    spu->alcCloseDevice = spu->alcGetProcAddress(NIL, "alcCloseDevice");
    spu->alcGetError = spu->alcGetProcAddress(NIL, "alcGetError");
    spu->alcIsExtensionPresent = spu->alcGetProcAddress(NIL, "alcIsExtensionPresent");
    spu->alcGetEnumValue = spu->alcGetProcAddress(NIL, "alcGetEnumValue");
    spu->alcGetString = spu->alcGetProcAddress(NIL, "alcGetString");
    spu->alcGetIntegerv = spu->alcGetProcAddress(NIL, "alcGetIntegerv");
    spu->alcCaptureOpenDevice = spu->alcGetProcAddress(NIL, "alcCaptureOpenDevice");
    spu->alcCaptureCloseDevice = spu->alcGetProcAddress(NIL, "alcCaptureCloseDevice");
    spu->alcCaptureStart = spu->alcGetProcAddress(NIL, "alcCaptureStart");
    spu->alcCaptureStop = spu->alcGetProcAddress(NIL, "alcCaptureStop");
    spu->alcCaptureSamples = spu->alcGetProcAddress(NIL, "alcCaptureSamples");
}

afxError _SalLoadOpenAlVmt(salSpuIdd *spu, afxNat base, afxNat cnt, char const *names[], void* vmt[], afxBool echo)
{
    afxError err = AFX_ERR_NONE;

    AfxAssert(names);
    AfxAssert(vmt);
    AfxAssert(spu->openal32);
    
    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(names[base + i]);
        void *sym = NIL;

        if ((sym = (void*)GetProcAddress(spu->openal32, names[base + i])))
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

_A4D void _ListAllDevs(salSpuIdd *spu)
{
    if (spu->alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT"))
    {
        ALCchar const *pDeviceNames = spu->alcGetString(NULL, ALC_DEVICE_SPECIFIER);
        ALCchar const *pDefaultDevice = spu->alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

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

    if (spu->alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
    {
        AfxLogEcho("Listing devices available with ALC_ENUMERATE_ALL_EXT");
        ALCchar const *pDeviceNames = spu->alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
        ALCchar const *pDefaultDevice = spu->alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);

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

    if (spu->alcIsExtensionPresent(NULL, "ALC_EXT_CAPTURE"))
    {
        // Get list of available Capture Devices
        const ALchar *pDeviceList = spu->alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);

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
    salSpuIdd *spu = &sdev->idd->spus[unitIdx];
    //wglVmt const* wgl = &dpu->wgl;
    alVmt const* al = &spu->al;
    HMODULE openal32;

    if (!(spu->openal32 = (openal32 = LoadLibraryA("openal32.dll")))) AfxLogError("");
    else
    {
        _SalSpuLoadBaseSymbols(sdev, unitIdx);
        _ListAllDevs(spu);

        if (!(spu->alcd = spu->alcOpenDevice(NULL))) AfxThrowError();
        else
        {
            spu->alcGetIntegerv(spu->alcd, ALC_MAJOR_VERSION, sizeof(spu->verMajor), (ALCint*)&spu->verMajor);
            spu->alcGetIntegerv(spu->alcd, ALC_MINOR_VERSION, sizeof(spu->verMinor), (ALCint*)&spu->verMinor);
            spu->verPatch = 0;
            spu->eaxEnabled = spu->alcIsExtensionPresent(spu->alcd, "ALC_EXT_EFX");

            if (!(spu->alc = spu->alcCreateContext(spu->alcd, NULL))) AfxThrowError();
            else
            {
                if (!spu->alcMakeContextCurrent(spu->alc)) AfxThrowError();
                else
                {
                    _SalLoadOpenAlVmt(spu, 0, AFX_COUNTOF(alVmtNames), alVmtNames, (void**)&spu->al.ptr, FALSE/*unitIdx == 0*/);

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

        if (err)
        {
            FreeLibrary(spu->openal32);
        }
    }
    return err;
}

_A4D afxError _SalDestroySpu(afxSoundDevice sdev, afxNat unitIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    salSpuIdd *spu = &sdev->idd->spus[unitIdx];
    alVmt const* al = &spu->al;

    ALCcontext* dc = spu->alcGetCurrentContext();

    if (dc == spu->alc)
        spu->alcMakeContextCurrent(NIL);

    spu->alcDestroyContext(spu->alc);
    spu->alcCloseDevice(spu->alcd);
    FreeLibrary(spu->openal32);
    return err;
}

_A4D afxError _SalSctxProcCb(afxSoundContext sctx, afxThread sthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssertObjects(1, &sthr, afxFcc_STHR);


    return err;
}

_A4D afxBool _SalProcessSctxCb(afxSoundContext sctx, void *udd)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxThread sthr = (afxThread)udd;
    AfxAssertObjects(1, &sthr, afxFcc_THR);

    if (_SalSctxProcCb(sctx, sthr))
        AfxThrowError();

    return FALSE; // don't interrupt curation;
}

_A4D afxError _SalSdevProcCb(afxSoundDevice sdev, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    AfxInvokeSoundContexts(sdev, 0, AFX_N32_MAX, _SalProcessSctxCb, (void*)thr);

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
        break;
    }
    case afxThreadEvent_QUIT:
    {
        if (_SalDestroySpu(sdev, 0))
            AfxThrowError();

        break;
    }
    default:
    {
        break;
    }
    }

    return 0;
}
_A4D afxError _SalSdevIddCtor(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;

    static afxString devDomain, devName;
    AfxMakeString(&sdev->dev.domain, "a4d", 0);
    AfxMakeString(&sdev->dev.name, a4dSignature, 0);

    sdev->dev.serving = FALSE;

    afxChain *classes = &sdev->dev.classes;
    AfxSetUpChain(classes, sdev);

    afxClassConfig const sctxMgrCfg =
    {
        .fcc = afxFcc_SCTX,
        .name = "Sound Context",
        .unitsPerPage = 1,
        .size = sizeof(AFX_OBJECT(afxSoundContext)),
        .ctor = (void*)NIL,
        .dtor = (void*)NIL
    };

#if 0
    afxClassConfig const soutMgrCfg =
    {
        .fcc = afxFcc_SOUT,
        .name = "Sound Output",
        .unitsPerPage = 1,
        .size = sizeof(AFX_OBJECT(afxSoundOutput)),
        .ctor = (void*)NIL,
        .dtor = (void*)NIL
    };

    afxClassConfig const sinMgrCfg =
    {
        .fcc = afxFcc_SIN,
        .name = "Sound Input",
        .unitsPerPage = 1,
        .size = sizeof(AFX_OBJECT(afxSoundInput)),
        .ctor = (void*)NIL,
        .dtor = (void*)NIL
    };
#endif
    // dctx must be after dxge
    AfxEstablishManager(&sdev->contexts, NIL, classes, &_SalSctxMgrCfg);
    //AfxEstablishManager(&sdev->outputs, NIL, classes, info->soutMgrCfg);
    //AfxEstablishManager(&sdev->inputs, NIL, classes, info->sinMgrCfg);

    sdev->idd = NIL;

    if (!(sdev->idd = AfxAllocate(1, sizeof(sdev->idd[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        sdev->dev.proc = (void*)_SalSdevProcCb;

        sdev->idd->spuCnt = 1;

        if (!(sdev->idd->spus = AfxAllocate(sdev->idd->spuCnt, sizeof(sdev->idd->spus[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            AfxZero2(sdev->idd->spuCnt, sizeof(sdev->idd->spus[0]), sdev->idd->spus);

            for (afxNat i = 0; i < sdev->idd->spuCnt; i++)
            {
                
            }

            afxNat unitIdx = 0;
            //wglVmt const* wgl = &ddev->dpus[unitIdx].wgl;
#if 0
            if (!sdev->idd->spus[unitIdx].alcMakeContextCurrent(sdev->idd->spus[unitIdx].alc))
                AfxThrowError();
#endif
            afxSoundDevice devInfo;

            //ddev->dpuCnt = 1;

            AfxAssert(sdev->dev.proc);
            sdev->dev.serving = TRUE;
        }

        if (err)
            AfxDeallocate(sdev->idd);
    }
            
    AfxAssert(sdev->dev.proc);

    afxThread sthr;
    afxThreadConfig stCfg = {0};
    stCfg.procCb = SoundThreadProc;
    stCfg.udd = sdev;

    if (AfxAcquireThread(AfxHere(), &stCfg, &sthr)) AfxThrowError();
    else
    {
        sdev->dev.serving = TRUE;

        AfxLogY("The audience is listening");

        AfxRunThread(sthr);
    }

    if (err)
        AfxCleanUpChainedManagers(&sdev->dev.classes);

    return err;
}

_A4D afxError _SalSdevIddDtor(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    for (afxNat i = 0; i < sdev->idd->spuCnt; i++)
        if (_SalDestroySpu(sdev, i))
            AfxThrowError();

    AfxDeallocate(sdev->idd->spus);
    AfxDeallocate(sdev->idd);
    sdev->idd = NIL;


    return err;
}

_A4D afxResult _SalSdevIoctl(afxSoundDevice sdev, afxNat reqCode, va_list va)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxResult rslt = 0;

    switch (reqCode)
    {
    case _sdevReqCode_0:
    {
        rslt = _SalSdevIddDtor(sdev);
        break;
    }
    case _sdevReqCode_1:
    {
        rslt = _SalSdevIddCtor(sdev);
        break;
    }
#if 0
    case _sdevReqCode_SIN_CTOR:
    {
        afxSoundInput sin = va_arg(va, afxSoundInput);
        AfxAssertObjects(1, &sin, afxFcc_SIN);
        afxSoundInputConfig const* cfg = va_arg(va, afxSoundInputConfig const*);
        afxUri const* endpoint = va_arg(va, afxUri const*);
        break;
    }
    case _sdevReqCode_SIN_DTOR:
    {
        afxSoundInput sin = va_arg(va, afxSoundInput);
        AfxAssertObjects(1, &sin, afxFcc_SIN);
        break;
    }
    case _sdevReqCode_SIN_RLNK:
    {
        afxSoundContext sctx = va_arg(va, afxSoundContext);
        AfxTryAssertObjects(1, &sctx, afxFcc_SCTX);
        afxNat cnt = va_arg(va, afxNat);
        AfxAssert(cnt);
        afxSoundInput* psin = va_arg(va, afxSoundInput*);
        AfxAssertObjects(cnt, psin, afxFcc_SIN);
        rslt = _SalSdevRelinkSinCb(sdev, sctx, cnt, psin);
        break;
    }
    case _sdevReqCode_SOUT_CTOR:
    {
        afxSoundOutput sout = va_arg(va, afxSoundOutput);
        AfxAssertObjects(1, &sout, afxFcc_SOUT);
        afxSoundOutputConfig const* cfg = va_arg(va, afxSoundOutputConfig const*);
        afxUri const* endpoint = va_arg(va, afxUri const*);
        rslt = _SalSdevInitSout(sdev, sout, cfg, endpoint);
        break;
    }
    case _sdevReqCode_SOUT_DTOR:
    {
        afxSoundOutput sout = va_arg(va, afxSoundOutput);
        AfxAssertObjects(1, &sout, afxFcc_SOUT);
        rslt = _SalDdevDeinitSout(sdev, sout);
        break;
    }
    case _sdevReqCode_SOUT_RLNK:
    {
        afxSoundContext sctx = va_arg(va, afxSoundContext);
        AfxTryAssertObjects(1, &sctx, afxFcc_SCTX);
        afxNat cnt = va_arg(va, afxNat);
        AfxAssert(cnt);
        afxSoundOutput* psout = va_arg(va, afxSoundOutput*);
        AfxAssertObjects(cnt, psout, afxFcc_SOUT);
        rslt = _SalSdevRelinkSoutCb(sdev, sctx, cnt, psout);
        break;
    }
#endif
    default:
        break;
    }

    return rslt;
}

_A4D afxError AfxIcdHookPoint(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    sdev->dev.ioctl = (void*)_SalSdevIoctl;
    return err;
}
