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
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/ux/afxApplication.h"
#include "qwadro/async/afxThread.h"

AAX afxClassConfig const _sthrClsConfig;
AAX afxSoundSystem AfxGetSoundSystem(void);

AAX afxError _AfxSysLoadIcd(afxSystem sys, afxUri const* file, afxIcd *icd);

_AAX afxBool AfxSoundDeviceIsRunning(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev->dev.serving;
}

_AAX afxClass* AfxGetSoundContextClass(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return &sdev->contexts;
}

_AAX afxClass* AfxGetSoundDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys = AfxGetSoundSystem();
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    afxClass* cls = &ssys->devices;
    AfxAssertClass(cls, afxFcc_SDEV);
    return cls;
}

_AAX afxClass* AfxGetSoundThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys = AfxGetSoundSystem();
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    afxClass* cls = &ssys->threads;
    AfxAssertClass(cls, afxFcc_STHR);
    return cls;
}

_AAX afxNat AfxCountSoundDevices(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxCountInstances(cls);
}

_AAX afxNat AfxEnumerateSoundDevices(afxNat first, afxNat cnt, afxSoundDevice sdev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(sdev);
    afxClass* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)sdev);
}

_AAX afxNat AfxInvokeSoundDevices(afxNat first, afxNat cnt, afxBool(*f)(afxSoundDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
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
    afxSoundSystem ssys = AfxGetSoundSystem();
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    afxMmu mmu = ssys->mmu;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AAX afxError _AfxSdevDtorFreeTxus(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    for (afxNat i = sdev->dev.txus.cnt; i-- > 0;)
    {
        afxTxu txu = *(afxTxu*)AfxGetArrayUnit(&sdev->dev.txus, i);
        AfxAssertObjects(1, &txu, afxFcc_TXU);
        while (!AfxReleaseObjects(1, (void*[]) { txu }));
    }
    return err;
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

    if (sdev->iddDtor && sdev->iddDtor(sdev))
        AfxThrowError();

    if (sdev->idd)
        AfxDeallocate(NIL, sdev->idd);

    _AfxUninstallChainedClasses(&sdev->dev.classes);

    _AfxSdevDtorFreeThreads(sdev);
    _AfxSdevDtorFreeTxus(sdev);
    AfxDeallocateArray(&sdev->dev.threads);
    AfxDeallocateArray(&sdev->dev.txus);

    AfxPopLinkage(&sdev->dev.icd);

    return err;
}

_AAX afxError AfxRegisterSoundDevices(afxIcd icd, afxNat cnt, afxSoundDeviceInfo const info[], afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    AfxAssert(devices);
    AfxAssert(info);
    AfxAssert(cnt);

    afxClass* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, info })) AfxThrowError();
    else
    {
        AfxAssertObjects(cnt, devices, afxFcc_SDEV);

        if (err)
            AfxReleaseObjects(cnt, (void**)devices);
    }

    return err;
}

_AAX afxError _AfxSdevCtor(afxSoundDevice sdev, afxCookie const* cookie)
{
    AfxEntry("sdev=%p", sdev);
    afxError err = AFX_ERR_NONE;

    afxIcd icd = cookie->udd[0];
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    afxSoundDeviceInfo const* info = ((afxSoundDeviceInfo const *)cookie->udd[1]) + cookie->no;
    AfxAssert(info);

    afxSoundSystem ssys = AfxGetSoundSystem();
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);

    afxMmu mmu = AfxGetSoundSystemMmu();

    AfxReflectString(info->domain, &sdev->dev.domain);
    AfxReflectString(info->name, &sdev->dev.name);

    if (AfxAllocateArray(&sdev->dev.txus, 0, sizeof(afxTxu), (afxTxu[]) { 0 })) AfxThrowError();
    else
    {
        if (AfxAllocateArray(&sdev->dev.threads, 0, sizeof(afxSoundThread), (afxSoundThread[]) { 0 })) AfxThrowError();
        else
        {
            AfxPushLinkage(&sdev->dev.icd, (afxChain*)AfxGetRegisteredDevices(icd));

            sdev->dev.serving = FALSE;

            sdev->procCb = NIL;
            sdev->relinkDin = NIL;
            sdev->relinkDout = NIL;
            AfxTakeMutex(&sdev->ioConMtx, AFX_MTX_PLAIN);

            afxChain *classes = &sdev->dev.classes;
            AfxSetUpChain(classes, sdev);

            afxClassConfig const sctxClsConfig =
            {
                .fcc = afxFcc_SCTX,
                .name = "Sound Context",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxSoundContext)),
                .mmu = mmu,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };

#if 0
            afxClassConfig const soutClsConfig =
            {
                .fcc = afxFcc_SOUT,
                .name = "Sound Output",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxSoundOutput)),
                .mmu = mmu,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };

            afxClassConfig const sinClsConfig =
            {
                .fcc = afxFcc_SIN,
                .name = "Sound Input",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxSoundInput)),
                .mmu = mmu,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };
#endif
            // dctx must be after dque
            AfxMountClass(&sdev->contexts, NIL, classes, info->sctxClsConfig);
            //AfxMountClass(&sdev->outputs, NIL, classes, info->soutClsConfig);
            //AfxMountClass(&sdev->inputs, NIL, classes, info->sinClsConfig);

            sdev->procCb = NIL;
            sdev->relinkDin = NIL;
            sdev->relinkDout = NIL;

            sdev->iddSiz = info->iddSiz;
            sdev->idd = NIL;

            sdev->iddCtor = info->iddCtor;
            sdev->iddDtor = info->iddDtor;

            if (sdev->iddCtor && sdev->iddCtor(sdev)) AfxThrowError();
            else
            {
                afxNat unitIdx;
                AfxGetThreadingUnit(&unitIdx);

                AfxAssert(sdev->procCb);
                sdev->dev.serving = TRUE;

                AfxAdvertise("The audience is listening");
            }

            if (err)
                _AfxUninstallChainedClasses(&sdev->dev.classes);

            if (err)
                AfxPopLinkage(&sdev->dev.icd);

            if (err)
            {
                _AfxSdevDtorFreeThreads(sdev);
                AfxDeallocateArray(&sdev->dev.threads);
            }
        }

        if (err)
        {
            _AfxSdevDtorFreeTxus(sdev);
            AfxDeallocateArray(&sdev->dev.txus);
        }
    }

    return err;
}

_AAX afxError _AfxSsysCtor(afxSoundSystem ssys, afxCookie const *cookie)
{
    AfxEntry("ssys=%p", ssys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxIni const* ini = cookie->udd[1];
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
            AfxMountClass(&ssys->txus, AfxGetThreadingUnitClass(), classes, &stxuClsCfg);

            afxClassConfig clsCfg;

            clsCfg = _sthrClsConfig;
            clsCfg.unitsPerPage = AfxGetThreadingCapacity();
            clsCfg.mmu = mmu;
            AfxMountClass(&ssys->threads, AfxGetThreadClass(), classes, &clsCfg);

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
            AfxMountClass(&ssys->devices, AfxGetDeviceClass(), classes, &sdevClsConfig);


            afxUri uri;
            AfxMakeUri(&uri, "system/e2sound.inf", 0);

#if 0
            if (AfxInstallClientDrivers(1, &uri, &ssys->e2sound)) AfxThrowError();
            else
#endif
            {
                afxSoundThread sthr[16];
                afxNat threadCnt = AfxMin(AfxGetThreadingCapacity(), 1);
                afxSoundThreadConfig sthrConfig = { 0 };
                //dthrConfig.udd = ddev->dpus[]

                for (afxNat i = 0; i < threadCnt; i++)
                {
                    sthrConfig.base.baseTxu = i;
                    sthrConfig.base.txuCnt = 1;

                    if (AfxAcquireSoundThreads(1, &sthrConfig, sthr)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &sthr[i], afxFcc_STHR);
                        afxThread thr = AfxGetSoundThreadBase(sthr[i]);
                        AfxRunThread(thr);
                    }
                }

#if 0
                if (err)
                    AfxReleaseObjects(1, (void*[]) { ssys->e2sound });
#endif
            }

            if (err)
            {
                //AfxDismountClass(&ssys->inputs);
                //AfxDismountClass(&ssys->outputs);
                //AfxDismountClass(&ssys->scontexts);
                //AfxDismountClass(&ssys->sdevices);
                //AfxDismountClass(&ssys->sthreads);
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
    AfxEntry("ssys=%p", ssys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);

    //AfxReleaseObjects(1, (void*[]) { dsys->e2draw });

    //AfxDismountClass(&ssys->inputs);
    //AfxDismountClass(&ssys->outputs);
    //AfxDismountClass(&ssys->scontexts);
    //AfxDismountClass(&ssys->sdevices);
    //AfxDismountClass(&ssys->sthreads);
    
    _AfxUninstallChainedClasses(&ssys->classes);

    AfxReleaseObjects(1, (void*[]) { ssys->mmu });

    return err;
}

_AAX afxClassConfig const _AfxSsysClsConfig =
{
    .fcc = afxFcc_SSYS,
    .name = "SIGMA Sound System",
    .maxCnt = 1,
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSoundSystem)),
    .mmu = NIL,
    .ctor = (void*)_AfxSsysCtor,
    .dtor = (void*)_AfxSsysDtor
};

_AAX afxClass* _AfxGetSsysClass(void)
{
    afxError err = AFX_ERR_NONE;
    static afxClass _ssysCls = { 0 };
    static afxBool ssysClsMounted = FALSE;

    if (!ssysClsMounted)
    {
        AfxMountClass(&_ssysCls, NIL, NIL, &_AfxSsysClsConfig);
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
