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

_AAX afxBool ssysReady = FALSE;
_AAX afxByte theSsysData[AFX_ALIGN(sizeof(afxObjectBase), 16) + AFX_ALIGN(sizeof(AFX_OBJECT(afxSoundSystem)), 16)] = { 0 };
_AAX afxSoundSystem TheSoundSystem = (void*)&theSsysData;
static_assert(sizeof(theSsysData) >= (sizeof(afxObjectBase) + sizeof(TheSoundSystem[0])), "");

AAX afxClassConfig const _sthrMgrCfg;

_AAX afxBool AfxGetSoundSystem(afxSoundSystem* ssys)
{
    afxError err = AFX_ERR_NONE;
    //AfxTryAssertObjects(1, &TheSoundSystem, afxFcc_SSYS);
    AfxAssert(ssys);
    *ssys = TheSoundSystem;
    return ssysReady;
}

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

_AAX afxError _AfxSdevDtor(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    if (!sdev->dev.ioctl) AfxThrowError();
    else if (sdev->dev.ioctl(&sdev->dev, _sdevReqCode_0, NIL))
        AfxThrowError();

    AfxAssert(!sdev->idd);

    AfxCleanUpChainedManagers(&sdev->dev.classes);

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

    afxChain*mgrChn = &ssys->mgrChn;
    AfxSetUpChain(mgrChn, ssys);

    afxClassConfig const sdevMgrCfg =
    {
        .fcc = afxFcc_SDEV,
        .name = "Sound Device",
        .unitsPerPage = 1,
        .size = sizeof(AFX_OBJECT(afxSoundDevice)),
        .ctor = (void*)_AfxSdevCtor,
        .dtor = (void*)_AfxSdevDtor
    };
    AfxEstablishManager(&ssys->devices, AfxGetDeviceClass(), mgrChn, &sdevMgrCfg);

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
        //AfxAbolishManager(&ssys->inputs);
        //AfxAbolishManager(&ssys->outputs);
        //AfxAbolishManager(&ssys->scontexts);
        //AfxAbolishManager(&ssys->sdevices);
        //AfxAbolishManager(&ssys->sthreads);
        AfxCleanUpChainedManagers(mgrChn);
    }
    return err;
}

_AAX afxError _AfxSsysDtor(afxSoundSystem ssys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ssys, afxFcc_SSYS);

    //AfxReleaseObjects(1, (void*[]) { dsys->e2draw });

    //AfxAbolishManager(&ssys->inputs);
    //AfxAbolishManager(&ssys->outputs);
    //AfxAbolishManager(&ssys->scontexts);
    //AfxAbolishManager(&ssys->sdevices);
    //AfxAbolishManager(&ssys->sthreads);
    
    AfxCleanUpChainedManagers(&ssys->mgrChn);

    //AfxReleaseObjects(1, (void*[]) { ssys->mmu });

    return err;
}

_AAX afxManager* _AfxGetSsysMgr(void)
{
    //afxError err = AFX_ERR_NONE;
    static afxManager ssysMgr = { 0 };
    static afxBool ssysMgrReady = FALSE;
    static afxClassConfig const ssysMgrCfg =
    {
        .fcc = afxFcc_SSYS,
        .name = "SoundSystem",
        .desc = "Unified 4D Accoustics Infrastructure",
        .maxCnt = 1,
        //.size = sizeof(AFX_OBJECT(afxSoundSystem)),
        .ctor = (void*)_AfxSsysCtor,
        .dtor = (void*)_AfxSsysDtor
    };

    if (ssysMgr.fcc != afxFcc_CLS)
    {
        AfxEstablishManager(&ssysMgr, NIL, /*_AfxGetSystemClassChain()*/NIL, &ssysMgrCfg);
        ssysMgrReady = TRUE;
    }
    return &ssysMgr;
}

#if 0
_AAX afxError DoSdevService(afxSoundDevice sdev, )
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    afxThread thr;
    if (AfxGetThread(&thr))
    {
        err = dev->proc(dev, thr);
    }
    return err;
}
#endif

_AAX afxResult _AaxSsysctl(afxSystem sys, afxInt reqCode, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    switch (reqCode)
    {
    case 0:
    {

        //AfxInvokeSoundDevices(NIL, 0, AFX_N32_MAX, (void*)DoSdevService, sys->primeThr);
        break;
    }
#if 0
    case 2:
    {
        afxSoundSystem ssys;

        if (!AfxGetSoundSystem(&ssys)) AfxThrowError();
        else
        {
            ssysReady = FALSE;

            afxManager* mgr = _AfxGetSsysMgr();
            AfxAssertClass(mgr, afxFcc_SSYS);

            AfxAssert(TheSoundSystem == ssys);

            if (_AfxDestructObjects(mgr, 1, (void**)&TheSoundSystem))
                AfxThrowError();

            AfxAssert(TheSoundSystem != ssys); // Attention! Dtor moves the object pointer to expose the object base.
            AfxZero(TheSoundSystem, sizeof(afxObjectBase));
        }
        break;
    }
    case 1:
    {
        afxSoundSystem ssys;

        if (AfxGetSoundSystem(&ssys)) AfxThrowError();
        else
        {
            AfxAssert(TheSoundSystem == ssys);
            AfxZero(TheSoundSystem, sizeof(afxObjectBase));

            afxManager* mgr = _AfxGetSsysMgr();
            AfxAssertClass(mgr, afxFcc_SSYS);

            va_list va;
            va_start(va, reqCode);

            if (_AfxConstructObjects(mgr, 1, (void**)&TheSoundSystem, (void*[]) { sys, va_arg(va, afxManifest*), va_arg(va, afxSoundSystemConfig*) })) AfxThrowError();
            else
            {
                AfxAssert(TheSoundSystem != ssys); // Attention! Ctor moves the object pointer to hide out the object base.
                ssys = TheSoundSystem;
                AfxAssertObjects(1, &ssys, afxFcc_SSYS);
                ssysReady = TRUE;
            }
            va_end(va);
        }
        break;
    }
#endif
    default:
    {
        AfxThrowError();
        break;
    }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AAX void AfxChooseSoundSystemConfiguration(afxSoundSystemConfig *config, afxNat extendedSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    *config = (afxSoundSystemConfig) { 0 };
}

_AAX afxError AfxEntryPoint(afxModule mdle, afxNat reqCode, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case afxFcc_SYS:
    {
        afxSoundSystem ssys;

        if (!AfxGetSoundSystem(&ssys))
        {
            AfxAssert(TheSoundSystem == ssys);
            AfxZero(TheSoundSystem, sizeof(afxObjectBase));

            afxManager* mgr = _AfxGetSsysMgr();
            AfxAssertClass(mgr, afxFcc_SSYS);

            if (_AfxConstructObjects(mgr, 1, (void**)&TheSoundSystem, udd)) AfxThrowError();
            else
            {
                AfxAssert(TheSoundSystem != ssys); // Attention! Ctor moves the object pointer to hide out the object base.
                ssys = TheSoundSystem;
                AfxAssertObjects(1, &ssys, afxFcc_SSYS);
                ssysReady = TRUE;
            }
        }
        else
        {
            AfxAssertObjects(1, &ssys, afxFcc_SSYS);
            ssysReady = FALSE;

            afxManager* mgr = _AfxGetSsysMgr();
            AfxAssertClass(mgr, afxFcc_SSYS);

            AfxAssert(TheSoundSystem == ssys);

            if (_AfxDestructObjects(mgr, 1, (void**)&TheSoundSystem))
                AfxThrowError();

            AfxAssert(TheSoundSystem != ssys); // Attention! Dtor moves the object pointer to expose the object base.
            AfxZero(TheSoundSystem, sizeof(afxObjectBase));
        }
    }
    default: break;
    }
    return err;
}
