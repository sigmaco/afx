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
#define _AFX_SOUND_DEVICE_C
#define _AFX_SOUND_SYSTEM_C
#define _AFX_SOUND_CONTEXT_C
#include "qwadro/mem/afxMmu.h"
#include "qwadro/mem/afxArena.h"
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/ux/afxApplication.h"
#include "qwadro/async/afxThread.h"

AFX afxClassConfig const _sthrClsConfig;
AFX afxSoundSystem _AfxGetSsysData(void);

AFX afxError _AfxSysLoadIcd(afxSystem sys, afxUri const* file, afxIcd *icd);

_AFX afxBool AfxSoundDeviceIsRunning(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev->serving;
}

_AFX afxClass* AfxGetSoundContextClass(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return &sdev->contexts;
}

_AFX afxClass* AfxGetSoundDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys = _AfxGetSsysData();
    AfxAssertType(ssys, afxFcc_SSYS);
    afxClass* cls = &ssys->sdevices;
    AfxAssertClass(cls, afxFcc_SDEV);
    return cls;
}

_AFX afxClass* AfxGetSoundThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys = _AfxGetSsysData();
    AfxAssertType(ssys, afxFcc_SSYS);
    afxClass* cls = &ssys->sthreads;
    AfxAssertClass(cls, afxFcc_STHR);
    return cls;
}

_AFX afxNat AfxCountSoundDevices(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxCountInstances(cls);
}

_AFX afxNat AfxEnumerateSoundDevices(afxNat first, afxNat cnt, afxSoundDevice sdev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(sdev);
    afxClass* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)sdev);
}

_AFX afxNat AfxInvokeSoundDevices(afxNat first, afxNat cnt, afxBool(*f)(afxSoundDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxSoundDevice AfxGetSoundDevice(afxNat sdevIdx)
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

_AFX afxMmu AfxGetSoundSystemMmu(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys = _AfxGetSsysData();
    AfxAssertType(ssys, afxFcc_SSYS);
    afxMmu mmu = ssys->mmu;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxError _AfxSsysCtor(afxSoundSystem ssys, afxSystem sys, afxSoundSystemConfig const* config)
{
    AfxEntry("ssys=%p", ssys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxAssignFcc(ssys, afxFcc_SSYS);

    afxChain *classes = &ssys->classes;
    AfxTakeChain(classes, sys);

    ssys->mmu = AfxGetSystemContext();

    afxMmu mmu = ssys->mmu;

    AfxAssertObjects(1, &ssys->mmu, afxFcc_MMU);

    if (AfxReacquireObjects(1, (void*[]) { ssys->mmu })) AfxThrowError();
    else
    {
        {
            afxClassConfig const sdevClsConfig =
            {
                .fcc = afxFcc_SDEV,
                .name = "Sound Device",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxSoundDevice)),
                .mmu = mmu,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };
            AfxMountClass(&ssys->sdevices, AfxGetDeviceClass(), classes, &sdevClsConfig);

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
            AfxMountClass(&ssys->scontexts, NIL, classes, &sctxClsConfig);
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
            AfxMountClass(&ssys->outputs, NIL, classes, &soutClsConfig);

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
            AfxMountClass(&ssys->inputs, NIL, classes, &sinClsConfig);
#endif
            afxClassConfig clsCfg;

            clsCfg = _sthrClsConfig;
            clsCfg.unitsPerPage = AfxGetThreadingCapacity();
            clsCfg.mmu = mmu;
            AfxMountClass(&ssys->sthreads, AfxGetThreadClass(), classes, &clsCfg);

            afxUri uri;
            AfxMakeUri(&uri, "system/e2sound.icd", 0);

            if (_AfxSysLoadIcd(sys, &uri, &ssys->e2sound)) AfxThrowError();
            else
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

                if (err)
                    AfxReleaseObjects(1, (void*[]) { ssys->e2sound });
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

_AFX afxError _AfxSsysDtor(afxSoundSystem ssys)
{
    AfxEntry("ssys=%p", ssys);
    afxError err = AFX_ERR_NONE;
    AfxAssertType(ssys, afxFcc_SSYS);

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

_AFX void AfxChooseSoundSystemConfiguration(afxSoundSystemConfig *config, afxNat extendedSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    *config = (afxSoundSystemConfig) { 0 };

}
