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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
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
#define _AFX_MODULE_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_OUTPUT_C

#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/afxCamera.h"

extern afxClassConfig const _dthrClsConfig;

AFXINL afxDrawSystem _AfxGetDsysData(void);
AFX afxError _AfxSysLoadIcd(afxSystem sys, afxUri const* file, afxIcd *icd);

_AFX afxBool AfxDrawDeviceIsRunning(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->serving;
}

_AFX afxClass* AfxGetDrawContextClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return &ddev->contexts;
}

_AFX afxClass* AfxGetDrawInputClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return &ddev->inputs;
}

_AFX afxClass* AfxGetDrawOutputClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return &ddev->outputs;
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

_AFX afxBool _AfxDdrvCurateProxyDctxCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (afxDrawContext)obj;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(dctx) != proxy->ddev || proxy->fdctx(dctx, proxy->udd); // don't interrupt curation;
}

_AFX afxBool _AfxDdrvCurateProxyDoutCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (afxDrawOutput)obj;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetDrawOutputDevice(dout) != proxy->ddev || proxy->fdout(dout, proxy->udd); // don't interrupt curation;
}

_AFX afxBool _AfxDdrvCurateProxyDinCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawInput din = (afxDrawInput)obj;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(din) != proxy->ddev || proxy->fdin(din, proxy->udd); // don't interrupt curation;
}

_AFX afxNat AfxCountDrawPorts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->portCnt;
}

_AFX void AfxGetDrawPortCaps(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertRange(ddev->portCnt, portIdx, 1);
    AfxAssert(caps);
    *caps = ddev->ports[portIdx].portCaps;
}

_AFX void AfxGetDrawDeviceCaps(afxDrawDevice ddev, afxDrawDeviceCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(caps);
    *caps = ddev->caps;
}

_AFX void AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawDeviceLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(limits);
    *limits = ddev->limits;
}

_AFX afxError _AfxDdevDtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxMmu mmu = AfxGetDrawSystemMmu();
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    _AfxUninstallChainedClasses(&ddev->classes);

    return err;
}

_AFX afxClass* AfxGetDrawDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    afxClass* cls = &dsys->ddevices;
    AfxAssertClass(cls, afxFcc_DDEV);
    return cls;
}

_AFX afxClass* AfxGetDrawThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    afxClass* cls = &dsys->dthreads;
    AfxAssertClass(cls, afxFcc_DTHR);
    return cls;
}

_AFX afxNat AfxCountDrawDevices(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxCountInstances(cls);
}

_AFX afxNat AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice ddev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(ddev);
    afxClass* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)ddev);
}

_AFX afxNat AfxCurateDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxCurateInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxDrawDevice AfxGetDrawDevice(afxNat ddevIdx)
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

_AFX afxMmu AfxGetDrawSystemMmu(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    afxMmu mmu = dsys->mmu;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxError _AfxDsysCtor(afxDrawSystem dsys, afxSystem sys, afxDrawSystemConfig const* config)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxAssignFcc(dsys, afxFcc_DSYS);

    afxChain *classes = &sys->classes;
    dsys->mmu = AfxGetSystemContext();

    afxMmu mmu = dsys->mmu;

    AfxAssertObjects(1, &dsys->mmu, afxFcc_MMU);

    if (AfxReacquireObjects(1, (void*[]) { dsys->mmu })) AfxThrowError();
    else
    {
        {
            afxClassConfig const ddevClsConfig =
            {
                .fcc = afxFcc_DDEV,
                .name = "Draw Device",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawDevice)),
                .mmu = mmu,
                .ctor = (void*)NIL,
                .dtor = (void*)_AfxDdevDtor
            };
            AfxMountClass(&dsys->ddevices, AfxGetDeviceClass(), classes, &ddevClsConfig);
            
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
            AfxMountClass(&dsys->dcontexts, NIL, classes, &dctxClsConfig);

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
            AfxMountClass(&dsys->doutputs, NIL, classes, &doutClsConfig);

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
            AfxMountClass(&dsys->dinputs, NIL, classes, &dinClsConfig);

            afxClassConfig clsCfg;

            clsCfg = _dthrClsConfig;
            clsCfg.unitsPerPage = AfxGetThreadingCapacity();
            clsCfg.mmu = mmu;
            AfxMountClass(&dsys->dthreads, AfxGetThreadClass(), classes, &clsCfg);

            afxUri uri;
            AfxMakeUri(&uri, "system/e2draw.icd", 0);

            if (_AfxSysLoadIcd(sys, &uri, &dsys->e2draw)) AfxThrowError();
            else
            {
                afxDrawThread dthr[16];
                afxNat threadCnt = AfxMin(AfxGetThreadingCapacity(), 1);
                afxDrawThreadConfig dthrConfig = { 0 };
                //dthrConfig.udd = ddev->dpus[]

                for (afxNat i = 0; i < threadCnt; i++)
                {
                    dthrConfig.base.baseTxu = i;
                    dthrConfig.base.txuCnt = 1;

                    if (AfxAcquireDrawThreads(1, &dthrConfig, dthr)) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &dthr[i], afxFcc_DTHR);
                        afxThread thr = AfxGetDrawThreadBase(dthr[i]);
                        AfxRunThread(thr);
                    }
                }

                if (err)
                    AfxReleaseObjects(1, (void*[]) { dsys->e2draw });
            }

            if (err)
            {
                AfxDismountClass(&dsys->dinputs);
                AfxDismountClass(&dsys->doutputs);
                AfxDismountClass(&dsys->dcontexts);
                AfxDismountClass(&dsys->ddevices);
                AfxDismountClass(&dsys->dthreads);                
            }
        }

        if (err)
            AfxReleaseObjects(1, (void*[]) { dsys->mmu });
    }

    return err;
}

_AFX afxError _AfxDsysDtor(afxDrawSystem dsys)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    AfxAssertType(dsys, afxFcc_DSYS);

    //AfxReleaseObjects(1, (void*[]) { dsys->e2draw });

    AfxDismountClass(&dsys->dinputs);
    AfxDismountClass(&dsys->doutputs);
    AfxDismountClass(&dsys->dcontexts);
    AfxDismountClass(&dsys->ddevices);
    AfxDismountClass(&dsys->dthreads);

    AfxReleaseObjects(1, (void*[]) { dsys->mmu });

    return err;
}

_AFX void AfxChooseDrawSystemConfiguration(afxDrawSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    *cfg = (afxDrawSystemConfig) { 0 };
}
