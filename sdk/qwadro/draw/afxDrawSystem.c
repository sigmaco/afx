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

#include "qwadro/core/afxManager.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/pipe/afxDrawOps.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/afxCamera.h"

//extern afxChain* _AfxGetSystemClassChain(void);
extern afxClassConfig const _dthrClsConfig;

_AVX afxMmu AfxGetDrawSystemMmu(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = AfxGetDrawSystem();
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxMmu mmu = dsys->mmu;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AVX afxManager* AfxGetDrawDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = AfxGetDrawSystem();
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->devices;
    AfxAssertClass(cls, afxFcc_DDEV);
    return cls;
}

_AVX afxManager* AfxGetDrawThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = AfxGetDrawSystem();
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->threads;
    AfxAssertClass(cls, afxFcc_DTHR);
    return cls;
}

_AVX afxBool AfxDrawDeviceIsRunning(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->dev.serving;
}

_AVX afxManager* AfxGetDrawContextClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return &ddev->contexts;
}

_AVX afxManager* AfxGetDrawInputClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return &ddev->inputs;
}

_AVX afxManager* AfxGetDrawOutputClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return &ddev->outputs;
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

_AVX afxError _AfxDdevDtorFreeThreads(afxDrawDevice ddev)
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

_AVX afxError _AfxDdevDtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxMmu mmu = AfxGetDrawSystemMmu();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    if (ddev->iddDtor && ddev->iddDtor(ddev))
        AfxThrowError();

    AfxAssert(!ddev->idd);

    _AfxUninstallChainedClasses(&ddev->dev.classes);

    if (ddev->ports)
        AfxDeallocate(ddev->ports);

    _AfxDdevDtorFreeThreads(ddev);
    AfxDeallocateArray(&ddev->dev.threads);


    AfxPopLinkage(&ddev->dev.icd);

    return err;
}

_AVX afxError _AfxDdevCtor(afxDrawDevice ddev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    afxIcd icd = cookie->udd[0];
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    afxDrawDeviceInfo const* info = ((afxDrawDeviceInfo const *)cookie->udd[1]) + cookie->no;
    AfxAssert(info);

    afxDrawSystem dsys = AfxGetDrawSystem();
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    afxMmu mmu = AfxGetDrawSystemMmu();

    AfxReflectString(info->domain, &ddev->dev.domain);
    AfxReflectString(info->name, &ddev->dev.name);

    if (AfxAllocateArray(&ddev->dev.threads, 0, sizeof(afxDrawThread), (afxDrawThread[]) { 0 })) AfxThrowError();
    else
    {
        AfxPushLinkage(&ddev->dev.icd, (afxChain*)AfxGetRegisteredDevices(icd));

        ddev->caps = info->caps ? *info->caps : (afxDrawDeviceCaps) { 0 };
        ddev->limits = info->limits ? *info->limits : (afxDrawDeviceLimits) { 0 };

        ddev->dev.serving = FALSE;

        ddev->clipCfg = AFX_CLIP_SPACE_QWADRO;

        ddev->procCb = NIL;
        ddev->relinkDin = NIL;
        ddev->relinkDout = NIL;

        afxNat portCnt = AfxMax(1, AfxMin(AfxGetThreadingCapacity(), info->portCnt));
        ddev->portCnt = portCnt;
        AfxAssert(ddev->portCnt);

        if (!(ddev->ports = AfxAllocate(portCnt, sizeof(ddev->ports[0]), 0, AfxHint()))) AfxThrowError();
        else
        {
            afxChain *classes = &ddev->dev.classes;
            AfxSetUpChain(classes, ddev);

            for (afxNat i = 0; i < portCnt; i++)
            {
                ddev->ports[i].portCaps = info->portCaps[i];

                afxClassConfig tmpClsCfg;

                tmpClsCfg = *info->dscrClsConfig;
                tmpClsCfg.mmu = mmu;
                AfxSetUpManager(&ddev->ports[i].scripts, NIL, classes, &tmpClsCfg);

                tmpClsCfg = *info->dqueClsConfig;
                tmpClsCfg.mmu = mmu;
                AfxSetUpManager(&ddev->ports[i].queues, NIL, classes, &tmpClsCfg);
            }

            afxClassConfig const dctxClsConfig =
            {
                .fcc = afxFcc_DCTX,
                .name = "Draw Context",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawContext)),
                .mmu = mmu,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };

            afxClassConfig const doutClsConfig =
            {
                .fcc = afxFcc_DOUT,
                .name = "Draw Output",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawOutput)),
                .mmu = mmu,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };

            afxClassConfig const dinClsConfig =
            {
                .fcc = afxFcc_DIN,
                .name = "Draw Input",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawInput)),
                .mmu = mmu,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };

            // dctx must be after dque
            AfxSetUpManager(&ddev->contexts, NIL, classes, info->dctxClsConfig);
            AfxSetUpManager(&ddev->outputs, NIL, classes, info->doutClsConfig);
            AfxSetUpManager(&ddev->inputs, NIL, classes, info->dinClsConfig);

            ddev->procCb = NIL;
            ddev->relinkDin = NIL;
            ddev->relinkDout = NIL;

            ddev->iddSiz = info->iddSiz;
            ddev->idd = NIL;

            ddev->iddCtor = info->iddCtor;
            ddev->iddDtor = info->iddDtor;

            if (ddev->iddCtor && ddev->iddCtor(ddev)) AfxThrowError();
            else
            {
                afxDrawThread dthr[2];
                afxDrawThreadConfig dtCfg[2] = { {0},{0} };
                dtCfg[0].ddev = ddev;
                dtCfg[0].base.txuCnt = AfxGetThreadingCapacity();
                dtCfg[1].ddev = ddev;
                dtCfg[1].base.txuCnt = dtCfg[0].base.txuCnt;

                if (AfxAcquireDrawThreads(dtCfg, AfxHint(), 2, dthr)) AfxThrowError();
                else
                {
                    AfxAssert(ddev->procCb);
                    ddev->dev.serving = TRUE;

                    AfxRunThreads(2, (afxThread*)dthr);
                }
            }

            if (err)
            {
                _AfxUninstallChainedClasses(&ddev->dev.classes);
                AfxDeallocate(ddev->ports);
            }
        }

        if (err)
            AfxPopLinkage(&ddev->dev.icd);

        if (err)
        {
            _AfxDdevDtorFreeThreads(ddev);
            AfxDeallocateArray(&ddev->dev.threads);
        }
    }
    return err;
}

_AVX afxError AfxRegisterDrawDevices(afxIcd icd, afxNat cnt, afxDrawDeviceInfo const info[], afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    AfxAssert(devices);
    AfxAssert(info);
    AfxAssert(cnt);

    afxManager* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, info })) AfxThrowError();
    else
    {
        AfxAssertObjects(cnt, devices, afxFcc_DDEV);

        if (err)
            AfxReleaseObjects(cnt, (void**)devices);
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

_AVX afxNat AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice ddev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(ddev);
    afxManager* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)ddev);
}

_AVX afxDrawDevice AfxGetDrawDevice(afxNat ddevIdx)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev = NIL;
    while (AfxEnumerateDrawDevices(ddevIdx, 1, &ddev))
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        break;
    }
    return ddev;
}

_AVX afxError _AfxDsysCtor(afxDrawSystem dsys, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxIni const* ini = cookie->udd[1];
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

            clsCfg = _dthrClsConfig;
            clsCfg.unitsPerPage = AfxGetThreadingCapacity();
            clsCfg.mmu = mmu;
            AfxSetUpManager(&dsys->threads, AfxGetThreadClass(), classes, &clsCfg);

            afxClassConfig const dtxuClsCfg =
            {
                .fcc = afxFcc_DENG,
                .name = "Draw Bridge",
                .size = sizeof(AFX_OBJECT(afxTxu)),
                .mmu = mmu,
                .unitsPerPage = AfxGetThreadingCapacity()
            };
            AfxSetUpManager(&dsys->txus, AfxGetTxuClass(), classes, &dtxuClsCfg);

            afxClassConfig const ddevClsConfig =
            {
                .fcc = afxFcc_DDEV,
                .name = "Draw Device",
                .unitsPerPage = 2,
                .size = sizeof(AFX_OBJECT(afxDrawDevice)),
                .mmu = mmu,
                .ctor = (void*)_AfxDdevCtor,
                .dtor = (void*)_AfxDdevDtor
            };
            AfxSetUpManager(&dsys->devices, AfxGetDeviceClass(), classes, &ddevClsConfig);
            
            afxUri uri;
            AfxMakeUri(&uri, "system/e2draw.inf", 0);

#if 0
            if (AfxInstallClientDrivers(1, &uri, &dsys->e2draw)) AfxThrowError();
            else
#endif
            {
#if 0
                afxDrawThread dthr[16];
                afxNat threadCnt = AfxMin(AfxGetThreadingCapacity(), 1);
                afxDrawThreadConfig dthrConfig = { 0 };
                //dthrConfig.udd = ddev->dpus[]

                for (afxNat i = 0; i < threadCnt; i++)
                {
                    dthrConfig.base.baseTxu = i;
                    dthrConfig.base.txuCnt = 1;

                    if (AfxAcquireDrawThreads(&dthrConfig, AfxHint(), 1, dthr)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &dthr[i], afxFcc_DTHR);
                        afxThread thr = AfxGetDrawThreadBase(dthr[i]);
                        AfxRunThread(thr);
                    }
                }
#endif

#if 0
                if (err)
                    AfxReleaseObjects(1, (void*[]) { dsys->e2draw });
#endif
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
    afxError err = AFX_ERR_NONE;
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
