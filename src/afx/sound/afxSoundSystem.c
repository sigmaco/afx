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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <stdio.h>
#include <Windows.h>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AFX_SYSTEM_C

#define _ASX_SOUND_C
#define _ASX_SOUND_DEVICE_C
#define _ASX_SOUND_INPUT_C
#define _ASX_SOUND_OUTPUT_C
#define _ASX_SOUND_SYSTEM_C
#define _ASX_SOUND_CONTEXT_C
#include "../src/afx/sound/dev/asxDevKit.h"

_ASX AFX_OBJECT(afxSoundSystem) TheSoundSystem = { 0 };
extern afxClassConfig const _AsxSoutClsCfg;
extern afxClassConfig const _AsxSinClsCfg;

_ASX afxBool AfxSoundDeviceIsRunning(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev->dev.serving;
}

_ASX afxNat AfxCountSoundPorts(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev->portCnt;
}

_ASX afxClass* AfxGetSoundContextClass(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return &sdev->sctxCls;
}

_ASX afxClass const* AfxGetSoundDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &TheSoundSystem.sdevCls;
    AfxAssertClass(cls, afxFcc_SDEV);
    return cls;
}

_ASX afxClass const* AfxGetSoundOutputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &TheSoundSystem.soutCls;
    AfxAssertClass(cls, afxFcc_SOUT);
    return cls;
}

_ASX afxClass const* AfxGetSoundInputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &TheSoundSystem.sinCls;
    AfxAssertClass(cls, afxFcc_SIN);
    return cls;
}

_ASX afxNat AfxEnumerateSoundDevices(afxNat first, afxNat cnt, afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxClass const* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)devices);
}

_ASX afxNat AfxEvokeSoundDevices(afxBool(*flt)(afxSoundDevice, void*), void* fdd, afxNat first, afxNat cnt, afxSoundDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxClass const* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)devices);
}

_ASX afxNat AfxInvokeSoundDevices(afxNat first, afxNat cnt, afxBool(*f)(afxSoundDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_ASX afxNat AfxInvokeSoundDevices2(afxNat first, afxNat cnt, afxBool(*flt)(afxSoundDevice, void*), void *fdd, afxBool(*f)(afxSoundDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(flt);
    AfxAssert(f);
    afxClass const* cls = AfxGetSoundDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxInvokeClassInstances2(cls, first, cnt, (void*)flt, fdd, (void*)f, udd);
}

_ASX afxBool AfxGetSoundDevice(afxNat sdevId, afxSoundDevice* device)
{
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev = NIL;    
    while (AfxEnumerateSoundDevices(sdevId, 1, &sdev))
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        AfxAssert(device);
        *device = sdev;
        break;
    }
    return !!sdev;
}

_ASX afxError _AsxSdevDtorCb(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    AfxAssert(sdev->stopCb);

    if (sdev->stopCb(sdev))
        AfxThrowError();

    AfxAssert(!sdev->idd);
    AfxCleanUpChainedClasses(&sdev->dev.classes);

    AfxCleanUpMutex(&sdev->relinkedCndMtx);
    AfxCleanUpCondition(&sdev->relinkedCnd);

    return err;
}

_ASX afxError _AsxSdevCtorCb(afxSoundDevice sdev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    afxSoundSystem ssys = cookie->udd[0];
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    //afxSoundDeviceInfo const* info = ((afxSoundDeviceInfo const *)cookie->udd[1]) + cookie->no;
    //AfxAssert(info);

    sdev->dev.serving = FALSE;

    AfxSetUpChain(&sdev->openedSoutChain, sdev);
    AfxSetUpChain(&sdev->openedSinChain, sdev);

    AfxSetUpCondition(&sdev->relinkedCnd);
    AfxSetUpMutex(&sdev->relinkedCndMtx, AFX_MTX_PLAIN);

    sdev->dev.procCb = NIL;
    sdev->idd = NIL;
    sdev->startCb = NIL;
    sdev->stopCb = NIL;
    sdev->openCb = NIL;
    sdev->closeCb = NIL;
    sdev->sinOpenCb = NIL;
    sdev->sinCloseCb = NIL;
    sdev->sinRelinkCb = NIL;
    sdev->soutCloseCb = NIL;
    sdev->soutOpenCb = NIL;
    sdev->soutRelinkCb = NIL;

    if (AfxCallDevice(&sdev->dev, afxFcc_SSYS, NIL)) AfxThrowError();
    else
    {
        if (!sdev->portCnt) AfxThrowError();
        else
        {
            AfxAssert(sdev->portCaps);
        }

        if (err)
        {
            AfxCleanUpChainedClasses(&sdev->dev.classes);
        }
    }
    return err;
}

afxClassConfig _AsxSdevClsCfg =
{
    .fcc = afxFcc_SDEV,
    .name = "SoundDevice",
    .desc = "Sound Device Driver Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundDevice)),
    .ctor = (void*)_AsxSdevCtorCb,
    .dtor = (void*)_AsxSdevDtorCb
};

_ASX afxError _AsxScanSdevs(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    // scan for device drivers
    {
        HANDLE fh;
        WIN32_FIND_DATAA wfd;
        afxUri2048 pathBuf;
        AfxMakeUri2048(&pathBuf, NIL);
        afxUri fileMask;
        AfxMakeUri(&fileMask, "system/*.inf", 0);
        AfxResolveUri(afxFileFlag_RX, &fileMask, &pathBuf.uri);

        afxDeviceType devType = afxDeviceType_DRAW;

        if ((fh = FindFirstFileA(AfxGetUriData(&pathBuf.uri, 0), &(wfd))))
        {
            do
            {
                afxManifest ini;
                AfxSetUpIni(&ini);
                afxUri manifestUri, manifestFile;
                AfxMakeUri(&manifestUri, wfd.cFileName, 0);
                AfxClipUriFile(&manifestFile, &manifestUri);
                AfxIniLoadFromFile(&ini, &manifestFile);

                afxNat icdPagIdx, listPagIdx, listCnt, devPagIdx, recIdx;
                afxString devPag, pag, rec, s;

                if (AfxFindManifestPage(&ini, &AfxString("Qwadro.Icd"), &icdPagIdx) &&
                    AfxFindManifestRecord(&ini, icdPagIdx, &AfxString("Devices"), &recIdx) &&
                    AfxGetManifestString(&ini, icdPagIdx, recIdx, &pag) &&
                    AfxFindManifestPage(&ini, &pag, &listPagIdx) &&
                    (listCnt = AfxCountManifestRecords(&ini, listPagIdx)))
                {
                    for (afxNat i = 0; i < listCnt; i++)
                    {
                        if (AfxGetManifestString(&ini, listPagIdx, i, &devPag) &&
                            AfxFindManifestPage(&ini, &devPag, &devPagIdx))
                        {
                            if (AfxFindManifestRecord(&ini, devPagIdx, &AfxString("Class"), &recIdx) &&
                                AfxGetManifestString(&ini, devPagIdx, recIdx, &s) &&
                                (0 == AfxCompareString(&s, &AfxString("SOUND"))))
                            {
                                afxClass* cls = (afxClass*)AfxGetSoundDeviceClass();
                                AfxAssertClass(cls, afxFcc_SDEV);

                                afxSoundDeviceInfo info = { 0 };
                                //info.dev.manifest = &ini;
                                info.dev.type = devType;
                                info.dev.manifestUri = manifestFile;
                                info.dev.manifestPagNo = devPagIdx;

                                afxSoundDevice sdev;

                                if (AfxAcquireObjects(cls, 1, (afxObject*)&sdev, (void const*[]) { sys, &info })) AfxThrowError();
                                else
                                {
                                    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

                                    if (err)
                                        AfxReleaseObjects(1, &sdev), sdev = NIL;
                                }
                            }
                        }
                    }
                }

                AfxCleanUpIni(&ini);
            } while (FindNextFileA(fh, &wfd));
            FindClose(fh);
        }
    }
    return err;
}

_ASX afxError AfxSystemIoctl(afxSystem sys, afxModule mdle, afxNat reqCode, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case 3:
    {
        AfxAssert(sys->asx.ssys == NIL);

        afxClassConfig clsCfg = _AsxSdevClsCfg;
        AfxRegisterClass(&TheSoundSystem.sdevCls, AfxGetDeviceClass(), &sys->classes, &clsCfg);

        clsCfg = _AsxSoutClsCfg;
        AfxRegisterClass(&TheSoundSystem.soutCls, NIL, &sys->classes, &clsCfg); // require sdev, sout

        clsCfg = _AsxSinClsCfg;
        AfxRegisterClass(&TheSoundSystem.sinCls, NIL, &sys->classes, &clsCfg); // require sdev, sin

        if (!err)
        {
            sys->asx.ssys = &TheSoundSystem;
            TheSoundSystem.ready = TRUE;
            _AsxScanSdevs(sys, udd[0]);
        }
        break;
    }
    case 4:
    {
        AfxAssert(sys->asx.ssys == &TheSoundSystem);
        TheSoundSystem.ready = FALSE;

        AfxExhaustClass(&TheSoundSystem.sinCls);
        AfxExhaustClass(&TheSoundSystem.soutCls);
        AfxExhaustClass(&TheSoundSystem.sdevCls);

        sys->avx.dsys = NIL;

        break;
    }
    default: break;
    }
    return err;
}
