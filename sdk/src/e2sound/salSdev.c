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

extern afxClassConfig const _SalSctxClsConfig;
AFX afxSoundSystem _AfxGetSsysData(void);

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
    salSpuIdd *spu = &sdev->spus[unitIdx];
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
                AfxEcho("%.3u %s core found.", i, names[base + i]);
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
                AfxEcho("%-2d - %s", iDeviceIndex, pDeviceNames);
                iDeviceIndex++;
                pDeviceNames += strlen(pDeviceNames) + 1;
            }
        }
    }

    if (spu->alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT"))
    {
        AfxEcho("Listing devices available with ALC_ENUMERATE_ALL_EXT");
        ALCchar const *pDeviceNames = spu->alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);
        ALCchar const *pDefaultDevice = spu->alcGetString(NULL, ALC_DEFAULT_ALL_DEVICES_SPECIFIER);

        if (strlen(pDeviceNames))
        {
            afxNat iDeviceIndex = 0;
            while (pDeviceNames && *pDeviceNames)
            {
                AfxEcho("%-2d - %s", iDeviceIndex, pDeviceNames);
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
            AfxEcho("Listing available capture devices:");

            afxNat iDeviceIndex = 0;
            while (*pDeviceList)
            {
                AfxEcho("%-2d - %s", iDeviceIndex, pDeviceList);
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
    salSpuIdd *spu = &sdev->spus[unitIdx];
    //wglVmt const* wgl = &dpu->wgl;
    alVmt const* al = &spu->al;
    HMODULE openal32;

    if (!(spu->openal32 = (openal32 = LoadLibraryA("openal32.dll")))) AfxError("");
    else
    {
        _SalSpuLoadBaseSymbols(sdev, unitIdx);
        _ListAllDevs(spu);

        if (!(spu->alcd = spu->alcOpenDevice(NULL))) AfxThrowError();
        else
        {
            spu->alcGetIntegerv(spu->alcd, ALC_MAJOR_VERSION, sizeof(spu->verMajor), &spu->verMajor);
            spu->alcGetIntegerv(spu->alcd, ALC_MINOR_VERSION, sizeof(spu->verMinor), &spu->verMinor);
            spu->verPatch = 0;
            spu->eaxEnabled = spu->alcIsExtensionPresent(spu->alcd, "ALC_EXT_EFX");

            if (!(spu->alc = spu->alcCreateContext(spu->alcd, NULL))) AfxThrowError();
            else
            {
                if (!spu->alcMakeContextCurrent(spu->alc)) AfxThrowError();
                else
                {
                    _SalLoadOpenAlVmt(spu, 0, AFX_COUNTOF(alVmtNames), alVmtNames, &spu->al.ptr, unitIdx == 0);

                    afxNat uiBuffer;
                    al->GenBuffers(1, &uiBuffer);

                    afxNat uiSource;
                    al->GenSources(1, &uiSource);

                    afxFile fd;
                    afxUri uri;
                    AfxMakeUri(&uri, "sound/qwadroDeepNote44kHz", 0);
                    AfxOpenFiles(afxFileFlag_R, 1, &uri, &fd);

                    afxNat dataSiz = AfxMeasureStream(AfxGetFileStream(fd));
                    void *data = AfxAllocate(NIL, dataSiz, 1, 0, AfxHint());
                    AfxReadStream(AfxGetFileStream(fd), data, dataSiz, 0);

                    al->BufferData(uiBuffer, AL_FORMAT_STEREO16, data, dataSiz, 44100);
                    AfxReleaseObjects(1, (void*[]) { fd });

                    al->Sourcei(uiSource, AL_BUFFER, uiBuffer);
                    al->SourcePlay(uiSource);
                    
                    afxNat iState;
                    do
                    {
                        Sleep(100);
                        // Get Source State
                        al->GetSourcei(uiSource, AL_SOURCE_STATE, &iState);
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
    salSpuIdd *spu = &sdev->spus[unitIdx];
    alVmt const* al = &spu->al;

    afxMmu mmu = AfxGetSoundSystemMmu();

    ALCcontext* dc = spu->alcGetCurrentContext();

    if (dc == spu->alc)
        spu->alcMakeContextCurrent(NIL);

    spu->alcDestroyContext(spu->alc);
    spu->alcCloseDevice(spu->alcd);
    FreeLibrary(spu->openal32);
    return err;
}

_A4D afxBool _SalProcessSctxCb(afxSoundContext sctx, void *udd)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundThread sthr = (afxSoundThread)udd;
    AfxAssertObjects(1, &sthr, afxFcc_STHR);

    if (sthr->sdev != AfxGetObjectProvider(sctx)) AfxThrowError();
    else
    {
        sthr->sctx = sctx;

        if (sctx->base.procCb(sctx, sthr))
            AfxThrowError();
    }
    return FALSE; // don't interrupt curation;
}

_A4D afxError _SalSdevProcCb(afxSoundDevice sdev, afxSoundThread sthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssertObjects(1, &sthr, afxFcc_STHR);
    afxSoundSystem ssys = _AfxGetSsysData();
    AfxAssertType(ssys, afxFcc_SSYS);

    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);

    if (unitIdx > 0)
        return err;

    sthr->portIdx = unitIdx;

    AfxInvokeSoundContexts(sdev, 0, AFX_N32_MAX, _SalProcessSctxCb, (void*)sthr);

    return err;
}

_A4D afxError _SalSdevCtor(afxSoundDevice sdev, afxCookie const* cookie)
{
    AfxEntry("sdev=%p", sdev);
    afxError err = AFX_ERR_NONE;

    afxSoundDeviceInfo const* info = ((afxSoundDeviceInfo const *)cookie->udd[1]) + cookie->no;
    AfxAssert(info);

    afxSoundSystem ssys = _AfxGetSsysData();
    AfxAssertType(ssys, afxFcc_SSYS);

    afxMmu mmu = AfxGetSoundSystemMmu();

    AfxReplicateString(&sdev->base.domain, info->domain);
    AfxReplicateString(&sdev->base.name, info->name);

    afxChain *classes = &sdev->base.classes;
    AfxTakeChain(classes, sdev);

    AfxMountClass(&sdev->base.contexts, &ssys->scontexts, classes, info->sctxClsConfig);

    sdev->base.serving = FALSE;

    sdev->base.procCb = _SalSdevProcCb;

    sdev->spuCnt = 1;

    if (!(sdev->spus = AfxAllocate(mmu, sdev->spuCnt, sizeof(sdev->spus[0]), 0, AfxHint()))) AfxThrowError();
    else
    {
        AfxZero(sdev->spuCnt, sizeof(sdev->spus[0]), sdev->spus);

        for (afxNat i = 0; i < sdev->spuCnt; i++)
        {
            if (_SalBuildSpu(sdev, i))
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; j++)
                    if (_SalDestroySpu(sdev, j))
                        AfxThrowError();
            }
        }

        afxNat unitIdx;
        AfxGetThreadingUnit(&unitIdx);

        //wglVmt const* wgl = &ddev->dpus[unitIdx].wgl;

        if (!sdev->spus[unitIdx].alcMakeContextCurrent(sdev->spus[unitIdx].alc))
            AfxThrowError();

        afxSoundDevice devInfo;

        //ddev->dpuCnt = 1;

        AfxAssert(sdev->base.procCb);
        sdev->base.serving = TRUE;
    }

    if (err)
    {
        _AfxUninstallChainedClasses(&sdev->base.classes);
    }

    return err;
}

_A4D afxError _SalSdevDtor(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    afxMmu mmu = AfxGetSoundSystemMmu();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    for (afxNat i = 0; i < sdev->spuCnt; i++)
        if (_SalDestroySpu(sdev, i))
            AfxThrowError();

    AfxDeallocate(mmu, sdev->spus);

    _AfxUninstallChainedClasses(&sdev->base.classes);

    return err;
}

_A4D afxError AfxIcdHookPoint(afxIcd icd)
{
    afxError err = AFX_ERR_NONE;

    if (!_AfxGetSsysData()) AfxThrowError();
    else
    {
        //afxUri file;
        //AfxMakeUri(&file, "e2draw.icd", 0);
        static afxString name, vendor, website, note;
        AfxMakeString(&icd->name, "A4D --- Qwadro Execution Ecosystem", 0);
        AfxMakeString(&icd->vendor, "SIGMA Technology Group", 0);
        AfxMakeString(&icd->website, "www.sigmaco.org", 0);
        AfxMakeString(&icd->note, sigmaSignature, 0);
        icd->verMajor = 0;
        icd->verMinor = 7;
        icd->verPatch = 2;

        afxUri file;
        AfxGetUriObject(&file, AfxGetModulePath(&icd->mdle));

        AfxLogMessageFormatted(0xFFFF0000, "\nInstalling '%.*s' ICD on sound system...\n\t%.*s %u.%u.%u\n\tVendor: %.*s <%.*s>\n\tNote: %.*s", AfxPushString(AfxGetUriString(&file)), AfxPushString(&icd->name), icd->verMajor, icd->verMinor, icd->verPatch, AfxPushString(&icd->vendor), AfxPushString(&icd->website), AfxPushString(&icd->note));

        //ddrv->mdle = info->mdle;
        //AfxAssertObjects(1, &ddrv->mdle, afxFcc_MDLE);
        //AfxReacquireObjects(1, (void*[]) { ddrv->mdle });

        afxChain *classes = &icd->classes;
        AfxTakeChain(classes, icd);

        static afxClassConfig const devClsExt =
        {
            .fcc = afxFcc_SDEV,
            .name = "Sound Device",
            .unitsPerPage = 1,
            .size = sizeof(AFX_OBJECT(afxSoundDevice)),
            .mmu = NIL,
            .ctor = (void*)_SalSdevCtor,
            .dtor = (void*)_SalSdevDtor
        };

        if (AfxMountClass(&icd->devices, AfxGetSoundDeviceClass(), classes, &devClsExt)) AfxThrowError();
        else
        {
            static afxString devDomain, devName;
            AfxMakeString(&devDomain, "a4d", 0);
            AfxMakeString(&devName, a4dSignature, 0);
            afxSoundDeviceInfo const devInfo[] =
            {
                {
                    .domain = &devDomain,
                    .name = &devName,
                    .sctxClsConfig = &_SalSctxClsConfig,
                }
            };
            afxNat const devCnt = AFX_COUNTOF(devInfo);
            afxSoundDevice sdev[AFX_COUNTOF(devInfo)];

            if (AfxAcquireObjects(&icd->devices, devCnt, (afxObject*)sdev, (void const*[]) { NIL, devInfo })) AfxThrowError();
            else
            {
                AfxAssertObjects(devCnt, sdev, afxFcc_SDEV);

                for (afxNat i = 0; i < 1; i++)
                {

                    AfxLogMessageFormatted(0xFFFF0000, "\Sound device '%.*s' <'%.*s'> registered on sound system", AfxPushString(devInfo[i].name), AfxPushString(devInfo[i].domain));
                }

                if (err)
                    AfxReleaseObjects(devCnt, (afxObject*)sdev);
            }

            if (err)
                _AfxUninstallChainedClasses(&icd->classes);
        }
    }
    return err;
}