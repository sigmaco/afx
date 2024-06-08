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

#include "qwadro/sound/afxSoundSystem.h"

//extern afxChain* _AfxGetSystemClassChain(void);

//_ASX afxBool ssysReady = FALSE;
_ASX afxByte theSsysData[AFX_ALIGN(sizeof(afxObjectBase), 16) + AFX_ALIGN(sizeof(AFX_OBJECT(afxSoundSystem)), 16)] = { 0 };
_ASX afxSoundSystem TheSoundSystem = (void*)&theSsysData;
AFX_STATIC_ASSERT(sizeof(theSsysData) >= (sizeof(afxObjectBase) + sizeof(TheSoundSystem[0])), "");
extern afxClassConfig const _AsxSoutMgrCfg;
extern afxClassConfig const _AsxSinMgrCfg;

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

_ASX afxManager* AfxGetSoundContextManager(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return &sdev->contexts;
}

_ASX afxManager* AfxGetSoundDeviceManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys;
    AfxGetSoundSystem(&ssys);
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    afxManager* cls = &ssys->sdevMgr;
    AfxAssertClass(cls, afxFcc_SDEV);
    return cls;
}

_ASX afxManager* AfxGetSoundInputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys;
    AfxGetSoundSystem(&ssys);
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    afxManager* cls = &ssys->sinMgr;
    AfxAssertClass(cls, afxFcc_SIN);
    return cls;
}

_ASX afxManager* AfxGetSoundOutputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSoundSystem ssys;
    AfxGetSoundSystem(&ssys);
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    afxManager* cls = &ssys->soutMgr;
    AfxAssertClass(cls, afxFcc_SOUT);
    return cls;
}

_ASX afxNat AfxCountSoundDevices(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetSoundDeviceManager();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxCountObjects(cls);
}

_ASX afxNat AfxEnumerateSoundDevices(afxNat first, afxNat cnt, afxSoundDevice sdev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(sdev);
    afxManager* cls = AfxGetSoundDeviceManager();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)sdev);
}

_ASX afxNat AfxInvokeSoundDevices(afxNat first, afxNat cnt, afxBool(*f)(afxSoundDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetSoundDeviceManager();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
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

_ASX afxError _AsxSdevDtor(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    
    if (sdev->iddDtorCb(sdev))
        AfxThrowError();

    AfxAssert(!sdev->idd);
    AfxCleanUpChainedManagers(&sdev->dev.classes);

    AfxCleanUpMutex(&sdev->relinkedCndMtx);
    AfxCleanUpCondition(&sdev->relinkedCnd);

    if (sdev->ports)
        AfxDeallocate(sdev->ports);

    return err;
}

_ASX afxError _AsxSdevCtor(afxSoundDevice sdev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    afxSoundSystem ssys = cookie->udd[0];
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    //afxSoundDeviceInfo const* info = ((afxSoundDeviceInfo const *)cookie->udd[1]) + cookie->no;
    //AfxAssert(info);


    sdev->dev.serving = FALSE;

    AfxSetUpChain(&sdev->outputs, sdev);
    AfxSetUpChain(&sdev->inputs, sdev);

    AfxSetUpCondition(&sdev->relinkedCnd);
    AfxSetUpMutex(&sdev->relinkedCndMtx, AFX_MTX_PLAIN);

    afxSoundDeviceInfo info2 = { 0 };
    
    if (AfxCallDevice(&sdev->dev, afxFcc_SSYS, &info2)) AfxThrowError();
    else
    {
        //sdev->caps = info2.caps;
        //sdev->limits = info2.limits;
        
        sdev->dev.procCb = (void*)info2.procCb;
        sdev->idd = info2.idd;
        sdev->iddDtorCb = info2.iddDtorCb;
        sdev->sinIddCtorCb = info2.sinIddCtorCb;
        sdev->sinIddDtorCb = info2.sinIddDtorCb;
        sdev->sinRelinkCb = info2.sinRelinkCb;
        sdev->soutIddCtorCb = info2.soutIddCtorCb;
        sdev->soutIddDtorCb = info2.soutIddDtorCb;
        sdev->soutRelinkCb = info2.soutRelinkCb;

        AfxAssert(info2.portCnt);
        afxNat portCnt = AfxMax(1, info2.portCnt);
        sdev->portCnt = portCnt;
        AfxAssert(sdev->portCnt);

        if (!(sdev->ports = AfxAllocate(portCnt, sizeof(sdev->ports[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            afxChain* classes = &sdev->dev.classes;
            afxClassConfig tmpClsCfg;

            for (afxNat i = 0; i < portCnt; i++)
            {
                //sdev->ports[i].portCaps = info2.portCaps[i];

                tmpClsCfg = *info2.sdgeClsCfg;
                AfxEstablishManager(&sdev->ports[i].sdgeMgr, NIL, classes, &tmpClsCfg);
            }

            // sctx must be after ddge
            tmpClsCfg = *info2.sctxClsCfg;
            AfxEstablishManager(&sdev->contexts, NIL, classes, &tmpClsCfg); // require ddge, cmdb

            if (info2.iddCtorCb(sdev)) AfxThrowError();
            else
            {

            }

            if (err)
            {
                AfxCleanUpChainedManagers(&sdev->dev.classes);
                AfxDeallocate(sdev->ports);
            }
        }
    }
    return err;
}

afxClassConfig _AsxSdevMgrCfg =
{
    .fcc = afxFcc_SDEV,
    .name = "SoundDevice",
    .desc = "Sound Device Driver Interface",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSoundDevice)),
    .ctor = (void*)_AsxSdevCtor,
    .dtor = (void*)_AsxSdevDtor
};

_ASX afxError _AsxSsysCtor(afxSoundSystem ssys, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManifest const* ini = cookie->udd[1];
    afxSoundSystemConfig const* config = cookie->udd[2];

    afxChain* managers = &ssys->managers;
    AfxSetUpChain(managers, ssys);

    afxClassConfig clsCfg = _AsxSdevMgrCfg;
    AfxEstablishManager(&ssys->sdevMgr, AfxGetDeviceClass(), managers, &clsCfg);

    clsCfg = _AsxSoutMgrCfg;
    AfxEstablishManager(&ssys->soutMgr, NIL, managers, &clsCfg); // require sdev, sout
    clsCfg = _AsxSinMgrCfg;
    AfxEstablishManager(&ssys->sinMgr, NIL, managers, &clsCfg); // require sdev, sin


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
                AfxPickUriFile(&manifestUri, &manifestFile);
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
                                afxManager* cls = AfxGetSoundDeviceManager();
                                AfxAssertClass(cls, afxFcc_SDEV);

                                afxSoundDeviceInfo info = { 0 };
                                //info.dev.manifest = &ini;
                                info.dev.type = devType;
                                info.dev.manifestUri = manifestFile;
                                info.dev.manifestPagNo = devPagIdx;

                                afxSoundDevice sdev;

                                if (AfxAcquireObjects(cls, 1, (afxObject*)&sdev, (void const*[]) { ssys, &info })) AfxThrowError();
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

    if (err)
    {
        AfxCleanUpChainedManagers(managers);
    }
    return err;
}

_ASX afxError _AsxSsysDtor(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);

    AfxCleanUpChainedManagers(&ssys->managers);

    return err;
}

_ASX afxClassConfig const _AsxSsysMgrCfg =
{
    .fcc = afxFcc_SSYS,
    .name = "SoundSystem",
    .desc = "Sound I/O System",
    .unitsPerPage = 1,
    .maxCnt = 1,
    //.size = sizeof(AFX_OBJECT(afxSoundSystem)),
    .ctor = (void*)_AsxSsysCtor,
    .dtor = (void*)_AsxSsysDtor
};

_ASX afxError AfxSystemIoctl(afxSystem sys, afxModule mdle, afxNat reqCode, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case 2:
    {
        AfxAssert(udd);
        udd[0] = (void*)&_AsxSsysMgrCfg;
        udd[1] = TheSoundSystem;
        break;
    }
    default: break;
    }
    return err;
}
