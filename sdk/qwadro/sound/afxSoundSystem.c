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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <stdio.h>
#include <Windows.h>

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_SOUND_C
#define _AFX_DEVICE_C
//#define _AFX_ICD_C
#define _AFX_TXU_C
#define _AFX_SOUND_DEVICE_C
#define _AFX_SOUND_INPUT_C
#define _AFX_SOUND_OUTPUT_C
#define _AFX_SOUND_SYSTEM_C
#define _AFX_SOUND_CONTEXT_C
#include "qwadro/mem/afxMmu.h"
#include "qwadro/mem/afxArena.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/core/afxThread.h"
#include "qwadro/sound/afxSoundSystem.h"

//extern afxChain* _AfxGetSystemClassChain(void);

AAX afxClassConfig const _sthrClsConfig;

_AAX afxBool AfxSoundDeviceIsRunning(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev->dev.serving;
}

_AAX afxManager* AfxGetSoundContextClass(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return &sdev->contexts;
}

_AAX afxManager* AfxGetSoundDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys;
    AfxGetSoundSystem(&ssys);
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    afxManager* cls = &ssys->devices;
    AfxAssertClass(cls, afxFcc_SDEV);
    return cls;
}

_AAX afxManager* AfxGetSoundThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys;
    AfxGetSoundSystem(&ssys);
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    afxManager* cls = &ssys->threads;
    AfxAssertClass(cls, afxFcc_STHR);
    return cls;
}

_AAX afxNat AfxCountSoundDevices(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxCountObjects(cls);
}

_AAX afxNat AfxEnumerateSoundDevices(afxNat first, afxNat cnt, afxSoundDevice sdev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(sdev);
    afxManager* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)sdev);
}

_AAX afxNat AfxInvokeSoundDevices(afxNat first, afxNat cnt, afxBool(*f)(afxSoundDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AAX afxSoundDevice AfxGetSoundDevice(afxNat sdevIdx)
{
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev = NIL;    
    while (AfxEnumerateSoundDevices(sdevIdx, 1, &sdev))
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        break;
    }
    return sdev;
}

_AAX afxMmu AfxGetSoundSystemMmu(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys;
    AfxGetSoundSystem(&ssys);
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    afxMmu mmu = ssys->mmu;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AAX afxError _AfxSdevDtorFreeThreads(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    for (afxNat i = sdev->dev.threads.cnt; i-- > 0;)
    {
        afxSoundThread dthr = *(afxSoundThread*)AfxGetArrayUnit(&sdev->dev.threads, i);
        AfxAssertObjects(1, &dthr, afxFcc_DTHR);
        while (!AfxReleaseObjects(1, (void*[]) { dthr }));
    }
    return err;
}

_AAX afxError _AfxSdevDtor(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    afxMmu mmu = AfxGetSoundSystemMmu();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (!sdev->dev.ioctl) AfxThrowError();
    else if (sdev->dev.ioctl(&sdev->dev, _sdevReqCode_0, NIL))
        AfxThrowError();

    AfxAssert(!sdev->idd);

    _AfxUninstallChainedClasses(&sdev->dev.classes);

    _AfxSdevDtorFreeThreads(sdev);
    AfxDeallocateArray(&sdev->dev.threads);

    return err;
}

_AAX afxError _AfxSdevCtor(afxSoundDevice sdev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    afxSoundDeviceInfo const* info = ((afxSoundDeviceInfo const *)cookie->udd[1]) + cookie->no;
    AfxAssert(info);

    if (!sdev->dev.ioctl) AfxThrowError();
    else
    {
        if (sdev->dev.ioctl(&sdev->dev, _sdevReqCode_1, NIL)) AfxThrowError();
        else
        {
            AfxAssert(!AfxStringIsEmpty(&sdev->dev.domain));
            AfxAssert(!AfxStringIsEmpty(&sdev->dev.name));

            AfxAssert(info);
        }
    }

    afxSoundSystem ssys;
    AfxGetSoundSystem(&ssys);
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);

    afxMmu mmu = AfxGetSoundSystemMmu();

    return err;
}

_AAX afxError _AfxSsysCtor(afxSoundSystem ssys, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManifest const* ini = cookie->udd[1];
    afxSoundSystemConfig const* config = cookie->udd[2];

    afxChain *classes = &ssys->classes;
    AfxSetUpChain(classes, sys);

    ssys->mmu = AfxGetSystemContext();

    afxMmu mmu = ssys->mmu;

    AfxAssertObjects(1, &ssys->mmu, afxFcc_MMU);

    if (AfxReacquireObjects(1, (void*[]) { ssys->mmu })) AfxThrowError();
    else
    {
        {
            afxClassConfig const stxuClsCfg =
            {
                .fcc = afxFcc_SENG,
                .name = "Sound Bridge",
                .size = sizeof(AFX_OBJECT(afxTxu)),
                .mmu = mmu,
                .unitsPerPage = AfxGetThreadingCapacity()
            };
            AfxSetUpManager(&ssys->txus, AfxGetTxuClass(), classes, &stxuClsCfg);

            afxClassConfig clsCfg;

            clsCfg = _sthrClsConfig;
            clsCfg.unitsPerPage = AfxGetThreadingCapacity();
            clsCfg.mmu = mmu;
            AfxSetUpManager(&ssys->threads, AfxGetThreadClass(), classes, &clsCfg);

            afxClassConfig const sdevClsConfig =
            {
                .fcc = afxFcc_SDEV,
                .name = "Sound Device",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxSoundDevice)),
                .mmu = mmu,
                .ctor = (void*)_AfxSdevCtor,
                .dtor = (void*)_AfxSdevDtor
            };
            AfxSetUpManager(&ssys->devices, AfxGetDeviceClass(), classes, &sdevClsConfig);


            // scan for device drivers
            {
                HANDLE fh;
                WIN32_FIND_DATAA wfd;
                afxUri2048 pathBuf;
                AfxMakeUri2048(&pathBuf, NIL);
                afxUri fileMask;
                AfxMakeUri(&fileMask, "system/*.inf", 0);
                AfxResolveUri(afxFileFlag_RX, &fileMask, &pathBuf.uri);

                afxDeviceType devType = afxDeviceType_SOUND;

                if ((fh = FindFirstFileA(AfxGetUriData(&pathBuf.uri, 0), &(wfd))))
                {
                    do
                    {
                        afxUri manifestUri, manifestFile;
                        AfxMakeUri(&manifestUri, wfd.cFileName, 0);
                        AfxPickUriFile(&manifestUri, &manifestFile);

                        afxSoundDevice sdev;

                        if (AfxFindDevice(devType, &manifestFile, (afxDevice*)&sdev))
                        {
                            AfxAssertObjects(1, &sdev, afxFcc_SDEV);
                            AfxReacquireObjects(1, (void*[]) { sdev });
                        }
                        else
                        {
                            AfxLogComment("Installing client driver <%.*s>...", AfxPushString(AfxGetUriString(&manifestFile)));

                            afxBool clearIni = TRUE;
                            afxManifest ini;
                            AfxSetUpIni(&ini);
                            AfxIniLoadFromFile(&ini, &manifestFile);
                            afxString s;

                            if (AfxIniGetString(&ini, &AfxString("Device"), &AfxString("Type"), &s))
                            {
                                if (0 == AfxCompareString(&s, &AfxString("SOUND")))
                                {
                                    afxManager* cls = AfxGetSoundDeviceClass();
                                    AfxAssertClass(cls, afxFcc_SDEV);

                                    afxDeviceInfo info = { 0 };
                                    info.manifest = &ini;
                                    info.type = devType;
                                    info.uri = manifestFile;

                                    if (AfxAcquireObjects(cls, 1, (afxObject*)&sdev, (void const*[]) { NIL, &info })) AfxThrowError();
                                    else
                                    {
                                        AfxAssertObjects(1, &sdev, afxFcc_SDEV);

                                        clearIni = FALSE;

                                        if (err)
                                            AfxReleaseObjects(1, (void*[]) { sdev }), sdev = NIL;
                                    }
                                }
                            }

                            if (clearIni)
                                AfxCleanUpIni(&ini);
                        }
                    } while (FindNextFileA(fh, &wfd));
                    FindClose(fh);
                }
            }

            if (err)
            {
                //AfxCleanUpManager(&ssys->inputs);
                //AfxCleanUpManager(&ssys->outputs);
                //AfxCleanUpManager(&ssys->scontexts);
                //AfxCleanUpManager(&ssys->sdevices);
                //AfxCleanUpManager(&ssys->sthreads);
                _AfxUninstallChainedClasses(classes);
            }
        }

        if (err)
            AfxReleaseObjects(1, (void*[]) { ssys->mmu });
    }

    return err;
}

_AAX afxError _AfxSsysDtor(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);

    //AfxReleaseObjects(1, (void*[]) { dsys->e2draw });

    //AfxCleanUpManager(&ssys->inputs);
    //AfxCleanUpManager(&ssys->outputs);
    //AfxCleanUpManager(&ssys->scontexts);
    //AfxCleanUpManager(&ssys->sdevices);
    //AfxCleanUpManager(&ssys->sthreads);
    
    _AfxUninstallChainedClasses(&ssys->classes);

    //AfxReleaseObjects(1, (void*[]) { ssys->mmu });

    return err;
}

_AAX afxClassConfig const _AfxSsysClsConfig =
{
    .fcc = afxFcc_SSYS,
    .name = "Unified Accoustics Experience Infrastructure",
    .maxCnt = 1,
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSoundSystem)),
    .mmu = NIL,
    .ctor = (void*)_AfxSsysCtor,
    .dtor = (void*)_AfxSsysDtor
};

_AAX afxManager* _AfxGetSsysClass(void)
{
    afxError err = AFX_ERR_NONE;
    static afxManager _ssysCls = { 0 };
    static afxBool ssysClsMounted = FALSE;

    if (_ssysCls.fcc != afxFcc_CLS)
    {
        AfxSetUpManager(&_ssysCls, NIL, /*_AfxGetSystemClassChain()*/NIL, &_AfxSsysClsConfig);
        ssysClsMounted = TRUE;
    }
    return &_ssysCls;
}

////////////////////////////////////////////////////////////////////////////////

_AAX void AfxChooseSoundSystemConfiguration(afxSoundSystemConfig *config, afxNat extendedSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    *config = (afxSoundSystemConfig) { 0 };

}
