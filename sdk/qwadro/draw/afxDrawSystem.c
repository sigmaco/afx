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
#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

#define _AFX_CORE_C
#define _AFX_DRAW_C
#define _AFX_DRAW_DRIVER_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_SYSTEM_C
//#define _AFX_MODULE_C
#define _AFX_DEVICE_C
//#define _AFX_ICD_C
#define _AFX_TXU_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_OUTPUT_C
#include "qwadro/draw/afxDrawSystem.h"

//extern afxChain* _AfxGetSystemClassChain(void);
extern afxClassConfig const _dthrClsConfig;
extern afxClassConfig const _AvxCamClsConfig;
extern afxClassConfig const _AvxDoutClsConfig;
extern afxClassConfig const _AvxDinClsConfig;

_AVX afxMmu AfxGetDrawSystemMmu(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxMmu mmu = dsys->mmu;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AVX afxManager* AfxGetCameraClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager *cls = &dsys->cameras;
    AfxAssertClass(cls, afxFcc_CAM);
    return cls;
}

_AVX afxManager* AfxGetDrawDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->devices;
    AfxAssertClass(cls, afxFcc_DDEV);
    return cls;
}

_AVX afxManager* AfxGetDrawThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->threads;
    AfxAssertClass(cls, afxFcc_DTHR);
    return cls;
}

_AVX afxManager* AfxGetQueryPoolClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager *cls = &ddev->queries;
    AfxAssertClass(cls, afxFcc_QRYP);
    return cls;
}

_AVX afxManager* AfxGetFenceClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager *cls = &ddev->fences;
    AfxAssertClass(cls, afxFcc_FENC);
    return cls;
}

_AVX afxManager* AfxGetSemaphoreClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager *cls = &ddev->semaphores;
    AfxAssertClass(cls, afxFcc_SEM);
    return cls;
}

_AVX afxManager* AfxGetDrawContextClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager* cls = &ddev->contexts;
    AfxAssertClass(cls, afxFcc_DCTX);
    return cls;
}

_AVX afxManager* AfxGetDrawInputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->inputs;
    AfxAssertClass(cls, afxFcc_DIN);
    return cls;
}

_AVX afxManager* AfxGetDrawOutputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->outputs;
    AfxAssertClass(cls, afxFcc_DOUT);
    return cls;
}

_AVX afxBool AfxDrawDeviceIsRunning(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->dev.serving;
}

_AVX afxNat AfxCountDrawDevices(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxCountObjects(cls);
}

struct _DdrvCurateProxyCb
{
    afxDrawDevice ddev;
    union
    {
        afxBool(*f)(void*, void*);
        afxBool(*fdctx)(afxDrawContext, void*);
        afxBool(*fdout)(afxDrawOutput, void*);
        afxBool(*fdin)(afxDrawInput, void*);
    };
    void *udd;
};

_AVX afxBool _AfxDdrvCurateProxyDctxCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (afxDrawContext)obj;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(dctx) != proxy->ddev || proxy->fdctx(dctx, proxy->udd); // don't interrupt curation;
}

_AVX afxBool _AfxDdrvCurateProxyDoutCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (afxDrawOutput)obj;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetDrawOutputDevice(dout) != proxy->ddev || proxy->fdout(dout, proxy->udd); // don't interrupt curation;
}

_AVX afxBool _AfxDdrvCurateProxyDinCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawInput din = (afxDrawInput)obj;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(din) != proxy->ddev || proxy->fdin(din, proxy->udd); // don't interrupt curation;
}

_AVX afxNat AfxCountDrawPorts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->portCnt;
}

_AVX void AfxGetDrawPortCaps(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertRange(ddev->portCnt, portIdx, 1);
    AfxAssert(caps);
    *caps = ddev->ports[portIdx].portCaps;
}

_AVX void AfxGetDrawDeviceCaps(afxDrawDevice ddev, afxDrawDeviceCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(caps);
    *caps = ddev->caps;
}

_AVX void AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawDeviceLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(limits);
    *limits = ddev->limits;
}

_AVX afxReal AfxDeterminePhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight)
{
    afxReal64 div = (afxReal64)screenWidth / (afxReal64)screenHeight;

    if (div <= 1.4)
        return 1.33f;

    if (div <= 1.6)
        return 1.5599999f;

    return 1.78f;
}

_AVX afxReal AfxDetermineAllowedCameraLodError(afxReal errInPixels, afxInt vpHeightInPixels, afxReal fovY, afxReal distanceFromCam)
{
    return AfxSinf(fovY * 0.5f) / AfxCosf(fovY * 0.5f) * errInPixels * distanceFromCam / ((afxReal)vpHeightInPixels * 0.5f);
}

_AVX afxError _AvxDdevDtorFreeThreads(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    for (afxNat i = ddev->dev.threads.cnt; i-- > 0;)
    {
        afxDrawThread dthr = *(afxDrawThread*)AfxGetArrayUnit(&ddev->dev.threads, i);
        AfxAssertObjects(1, &dthr, afxFcc_DTHR);
        while (!AfxReleaseObjects(1, (void*[]) { dthr }));
    }
    return err;
}

_AVX afxError _AvxDdevDtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxMmu mmu = AfxGetDrawSystemMmu();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (!ddev->dev.ioctl) AfxThrowError();
    else if (ddev->dev.ioctl(&ddev->dev, _ddevReqCode_0, NIL))
        AfxThrowError();

    AfxAssert(!ddev->idd);

    return err;
}

_AVX afxError _AvxDdevCtor(afxDrawDevice ddev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    afxDrawDeviceInfo const* info = ((afxDrawDeviceInfo const *)cookie->udd[1]) + cookie->no;
    AfxAssert(info);

    if (!ddev->dev.ioctl) AfxThrowError();
    else
    {
        if (ddev->dev.ioctl(&ddev->dev, _ddevReqCode_1, NIL)) AfxThrowError();
        else
        {
            AfxAssert(!AfxStringIsEmpty(&ddev->dev.domain));
            AfxAssert(!AfxStringIsEmpty(&ddev->dev.name));

            AfxAssert(info);
        }
    }
    return err;
}

_AVX afxNat AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxManager* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
}

_AVX afxBool AfxGetDrawDevice(afxNat ddevNo, afxDrawDevice* ddev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev2 = NIL;
    while (AfxEnumerateDrawDevices(ddevNo, 1, &ddev2))
    {
        AfxAssertObjects(1, &ddev2, afxFcc_DDEV);
        AfxAssert(ddev);
        *ddev = ddev2;
        break;
    }
    return !!ddev2;
}

_AVX afxError _AfxDsysCtor(afxDrawSystem dsys, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManifest const* ini = cookie->udd[1];
    afxDrawSystemConfig const* config = cookie->udd[2];

    afxChain *classes = &dsys->classes;
    AfxSetUpChain(classes, sys);

    dsys->mmu = AfxGetSystemContext();

    afxMmu mmu = dsys->mmu;

    AfxAssertObjects(1, &dsys->mmu, afxFcc_MMU);

    if (AfxReacquireObjects(1, (void*[]) { dsys->mmu })) AfxThrowError();
    else
    {
        {
            afxClassConfig clsCfg;

            clsCfg = _AvxCamClsConfig;
            clsCfg.unitsPerPage = 2;
            clsCfg.mmu = mmu;
            AfxSetUpManager(&dsys->cameras, NIL, classes, &clsCfg);

            clsCfg = _dthrClsConfig;
            clsCfg.unitsPerPage = AfxGetThreadingCapacity();
            clsCfg.mmu = mmu;
            AfxSetUpManager(&dsys->threads, AfxGetThreadClass(), classes, &clsCfg);

            afxClassConfig const dtxuClsCfg =
            {
                .fcc = afxFcc_DENG,
                .name = "DrawBridge",
                .desc = "Draw Execution Unit",
                .size = sizeof(AFX_OBJECT(afxTxu)),
                .mmu = mmu,
                .unitsPerPage = AfxGetThreadingCapacity()
            };
            AfxSetUpManager(&dsys->txus, AfxGetTxuClass(), classes, &dtxuClsCfg);

            afxClassConfig const ddevClsConfig =
            {
                .fcc = afxFcc_DDEV,
                .name = "DrawDevice",
                .desc = "Draw Device Management",
                .unitsPerPage = 2,
                .size = sizeof(AFX_OBJECT(afxDrawDevice)),
                .mmu = mmu,
                .ctor = (void*)_AvxDdevCtor,
                .dtor = (void*)_AvxDdevDtor
            };
            AfxSetUpManager(&dsys->devices, AfxGetDeviceClass(), classes, &ddevClsConfig); // require dtxu, dthr

            clsCfg = _AvxDoutClsConfig;
            AfxSetUpManager(&dsys->outputs, NIL, classes, &clsCfg); // require ddev, dout

            clsCfg = _AvxDinClsConfig;
            AfxSetUpManager(&dsys->inputs, NIL, classes, &clsCfg); // require ddev, din

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
                        afxUri manifestUri, manifestFile;
                        AfxMakeUri(&manifestUri, wfd.cFileName, 0);
                        AfxPickUriFile(&manifestUri, &manifestFile);

                        afxDrawDevice ddev;

                        if (AfxFindDevice(devType, &manifestFile, (afxDevice*)&ddev))
                        {
                            AfxAssertObjects(1, &ddev, afxFcc_DDEV);
                            AfxReacquireObjects(1, (void*[]) { ddev });
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
                                if (0 == AfxCompareString(&s, &AfxString("DRAW")))
                                {
                                    afxManager* cls = AfxGetDrawDeviceClass();
                                    AfxAssertClass(cls, afxFcc_DDEV);

                                    afxDeviceInfo info = { 0 };
                                    info.manifest = &ini;
                                    info.type = devType;
                                    info.uri = manifestFile;

                                    if (AfxAcquireObjects(cls, 1, (afxObject*)&ddev, (void const*[]) { NIL, &info })) AfxThrowError();
                                    else
                                    {
                                        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

                                        clearIni = FALSE;

                                        if (err)
                                            AfxReleaseObjects(1, (void*[]) { ddev }), ddev = NIL;
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
                _AfxUninstallChainedClasses(classes);
            }
        }

        if (err)
            AfxReleaseObjects(1, (void*[]) { dsys->mmu });
    }

    return err;
}

_AVX afxError _AfxDsysDtor(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    //AfxReleaseObjects(1, (void*[]) { dsys->e2draw });

    //AfxCleanUpManager(&dsys->dinputs);
    //AfxCleanUpManager(&dsys->doutputs);
    //AfxCleanUpManager(&dsys->dcontexts);
    //AfxCleanUpManager(&dsys->ddevices);
    //AfxCleanUpManager(&dsys->dthreads);
    _AfxUninstallChainedClasses(&dsys->classes);

    AfxReleaseObjects(1, (void*[]) { dsys->mmu });

    return err;
}

_AVX afxClassConfig const _AfxDsysClsConfig =
{
    .fcc = afxFcc_DSYS,
    .name = "Unified Video Graphics Infrastructure",
    .maxCnt = 1,
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawSystem)),
    .mmu = NIL,
    .ctor = (void*)_AfxDsysCtor,
    .dtor = (void*)_AfxDsysDtor
};

_AVX afxManager* _AfxGetDsysClass(void)
{
    //afxError err = AFX_ERR_NONE;
    static afxManager _dsysCls = { 0 };
    static afxBool dsysClsMounted = FALSE;

    if (_dsysCls.fcc != afxFcc_CLS)
    {
        AfxSetUpManager(&_dsysCls, NIL, /*_AfxGetSystemClassChain()*/NIL, &_AfxDsysClsConfig);
        dsysClsMounted = TRUE;
    }
    return &_dsysCls;
}

////////////////////////////////////////////////////////////////////////////////

_AVX void AfxChooseDrawSystemConfiguration(afxDrawSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    *cfg = (afxDrawSystemConfig) { 0 };
}
