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
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_OUTPUT_C

#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/afxCamera.h"

extern afxClassConfig const _dthrClsConfig;
extern afxClassConfig const _doutClsConfig;
extern afxClassConfig const _dinClsConfig;
extern afxClassConfig const _dctxClsConfig;
extern afxClassConfig const _camClsConfig;

AFXINL afxDrawSystem _AfxGetDsysData(void);

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

_AFX afxError _AfxDdrvDtor(afxDrawIcd ddrv)
{
    AfxEntry("ddrv=%p", ddrv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddrv, afxFcc_DDRV);

    //afxMmu mmu = AfxGetDrawSystemMmu();

    _AfxUninstallChainedClasses(&ddrv->classes);

    AfxReleaseObjects(1, (void*[]) { ddrv->mdle });

    return err;
}

_AFX afxError _AfxDdrvCtor(afxDrawIcd ddrv, afxCookie const* cookie)
{
    AfxEntry("ddrv=%p", ddrv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddrv, afxFcc_DDRV);

    afxDrawIcdInfo const* info = ((afxDrawIcdInfo const *)cookie->udd[0]) + cookie->no;
    AfxAssert(info);
    
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    //afxMmu mmu = AfxGetDrawSystemMmu(dsys);

    AfxReplicateString(&ddrv->name, info->name);
    AfxReplicateString(&ddrv->vendor, info->vendor);
    AfxReplicateString(&ddrv->website, info->website);
    AfxReplicateString(&ddrv->note, info->note);
    ddrv->verMajor = info->verMajor;
    ddrv->verMinor = info->verMinor;
    ddrv->verPatch = info->verPatch;

    afxUri file;
    AfxGetUriObject(&file, AfxGetModulePath(info->mdle));

    AfxLogMessageFormatted(0xFFFF0000, "\nInstalling '%.*s' ICD on draw system %p...\n\t%.*s %u.%u.%u\n\tVendor: %.*s <%.*s>\n\tNote: %.*s", AfxPushString(AfxGetUriString(&file)), dsys, AfxPushString(&ddrv->name), ddrv->verMajor, ddrv->verMinor, ddrv->verPatch, AfxPushString(&ddrv->vendor), AfxPushString(&ddrv->website), AfxPushString(&ddrv->note));

    ddrv->mdle = info->mdle;
    AfxAssertObjects(1, &ddrv->mdle, afxFcc_MDLE);
    AfxReacquireObjects(1, (void*[]) { ddrv->mdle });

    afxChain *classes = &ddrv->classes;
    AfxTakeChain(classes, ddrv);

    afxClassConfig tmpClsConfig = *info->devExt;
    tmpClsConfig.unitsPerPage = info->devCnt;
    
    if (AfxInstallClassExtension(&ddrv->devices, &dsys->devices, classes, &tmpClsConfig)) AfxThrowError();
    else
    {
        afxNat devCnt = AfxMin(info->devCnt, 32);
        afxDrawDevice ddev[32];
        afxDrawDeviceInfo devInfos[32];
        afxError(*getDevInfoCb)(afxNat,afxDrawDeviceInfo*) = AfxFindModuleSymbol(ddrv->mdle, "AfxGetDrawDeviceInformation");

        if (!getDevInfoCb || getDevInfoCb(devCnt, devInfos)) AfxThrowError();
        else
        {
            if (AfxAcquireObjects(&ddrv->devices, devCnt, (afxObject*)ddev, (void const*[]) { devInfos })) AfxThrowError();
            else
            {
                AfxAssertObjects(devCnt, ddev, afxFcc_DDEV);

                for (afxNat i = 0; i < devCnt; i++)
                {

                    AfxLogMessageFormatted(0xFFFF0000, "\nDraw device '%.*s' <'%.*s'> discovered on draw system %p.", AfxPushString(&ddev[i]->name), AfxPushString(&ddev[i]->domain), dsys);
                }

                if (err)
                    AfxReleaseObjects(devCnt, (afxObject*)ddev);
            }
        }

        if (err)
            _AfxUninstallChainedClasses(&ddrv->classes);
    }

    if (err)
        AfxReleaseObjects(1, (void*[]) { ddrv->mdle });

    return err;
}

_AFX afxClass* AfxGetDrawDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    afxClass* cls = &dsys->devices;
    AfxAssertClass(cls, afxFcc_DDEV);
    return cls;
}

_AFX afxClass* AfxGetCameraClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    afxClass *cls = &dsys->cameras;
    AfxAssertClass(cls, afxFcc_CAM);
    return cls;
}

_AFX afxNat AfxCountDrawDevices(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    AfxAssertClass(&dsys->devices, afxFcc_DDEV);
    return AfxCountInstances(&dsys->devices);
}

_AFX afxClass* AfxGetDrawThreadClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    afxClass* cls = &dsys->threads;
    AfxAssertClass(cls, afxFcc_DTHR);
    return cls;
}

_AFX afxNat AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice ddev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(ddev);
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    AfxAssertClass(&dsys->devices, afxFcc_DDEV);
    return AfxEnumerateInstances(&dsys->devices, first, cnt, (afxObject*)ddev);
}

_AFX afxNat AfxCurateDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxDrawSystem dsys = _AfxGetDsysData();
    AfxAssertType(dsys, afxFcc_DSYS);
    AfxAssertClass(&dsys->devices, afxFcc_DDEV);
    return AfxCurateInstances(&dsys->devices, first, cnt, (void*)f, udd);
}

_AFX afxBool AfxGetDrawDevice(afxNat ddevIdx, afxDrawDevice *ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ddev);
    afxBool rslt = FALSE;

    if ((rslt = !!(AfxEnumerateDrawDevices(ddevIdx, 1, ddev))))
    {
        AfxAssertObjects(1, ddev, afxFcc_DDEV);
    }
    return rslt;
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

_AFX afxDrawIcd _AfxDsysFindIcd(afxDrawSystem dsys, afxUri const *file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(dsys, afxFcc_DSYS);
    
    afxUri target;
    AfxAssertType(file, afxFcc_URI);
    AfxGetUriObject(&target, file);

    afxNat i = 0;
    afxDrawIcd icd;
    AfxAssertClass(&dsys->icds, afxFcc_DDRV);
    while(AfxEnumerateInstances(&dsys->icds, i, 1, (afxObject*)&icd))
    {
        afxUri tmp;
        AfxAssertObjects(1, &icd, afxFcc_DDRV);
        AfxGetUriObject(&tmp, AfxGetModulePath(icd->mdle));

        if (0 == AfxCompareUri(&target, &tmp))
            return icd;

        ++i;
    }
    return NIL;
}

_AFX afxError _AfxDsysLoadIcd(afxDrawSystem dsys, afxUri const* file, afxDrawIcd *ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(dsys, afxFcc_DSYS);
    AfxAssertType(file, afxFcc_URI);
    AfxComment("Loading draw ICD %.*s", AfxPushString(AfxGetUriString(file)));

    afxUri file2;
    AfxGetUriObject(&file2, file);
    afxDrawIcd ddrv2 = _AfxDsysFindIcd(dsys, &file2);

    if ((ddrv2 = _AfxDsysFindIcd(dsys, &file2))) AfxReacquireObjects(1, (void*[]) { ddrv2 });
    else
    {
        afxModule mdle;

        if (AfxAcquireModules(1, &file2, &mdle)) AfxThrowError();
        else
        {
            afxError(*getIcdInfoCb)(afxDrawIcdInfo*) = AfxFindModuleSymbol(mdle, "AfxGetDrawIcdInformation");
            afxDrawIcdInfo icdInfo;

            if (getIcdInfoCb)
            {
                if (getIcdInfoCb(&icdInfo)) AfxThrowError();
                else
                {
                    if (AfxAcquireObjects(&dsys->icds, 1, (afxObject*)&ddrv2, (void const*[]) { (void*)&icdInfo })) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &ddrv2, afxFcc_DDRV);

                        if (err)
                            AfxReleaseObjects(1, (void*[]) { ddrv2 });
                    }
                }
            }

            AfxReleaseObjects(1, (void*[]) { mdle });
        }
    }

    if (!err)
        *ddrv = ddrv2;

    return err;
}

_AFX afxError _AfxDsysScanForIcds(afxDrawSystem dsys, afxUri const* fileMask)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(dsys, afxFcc_DSYS);
    HANDLE fh;    
    WIN32_FIND_DATAA wfd;    
    afxFixedUri2048 pathBuf;
    AfxMakeFixedUri2048(&pathBuf, NIL);
    AfxResolveUri(fileMask, AFX_FILE_FLAG_RX, &pathBuf.uri);
    if ((fh = FindFirstFileA(AfxGetUriData(&pathBuf.uri, 0), &(wfd))))
    {
        do
        {
            afxUri uri, file;
            AfxMakeUri(&uri, wfd.cFileName, 0);
            AfxGetUriObject(&file, &uri);
            afxDrawIcd ddrv2;
            
            if (!(ddrv2 = _AfxDsysFindIcd(dsys, &file)))
            {
                if (_AfxDsysLoadIcd(dsys, &file, &ddrv2))
                    AfxThrowError();
            }
        } while (FindNextFileA(fh, &wfd));
        FindClose(fh);
    }
    return err;
}

_AFX afxError _AfxDsysCtor(afxDrawSystem dsys, afxSystem sys, afxDrawSystemConfig const* config)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxAssignFcc(dsys, afxFcc_DSYS);

    afxChain *classes = &sys->classes;

    if (config)
    {
        dsys->mmu = config->mmu;
    }
    
    if (!dsys->mmu)
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
            AfxMountClass(&dsys->threads, classes, &clsCfg);

            afxClassConfig const ddrvClsConfig =
            {
                .fcc = afxFcc_DDRV,
                .name = "Installable Client Driver for Draw Device",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawIcd)),
                .mmu = mmu,
                .ctor = (void*)_AfxDdrvCtor,
                .dtor = (void*)_AfxDdrvDtor
            };
            AfxMountClass(&dsys->icds, classes, &ddrvClsConfig);

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
            AfxMountClass(&dsys->devices, classes, &ddevClsConfig);

            clsCfg = _dctxClsConfig;
            clsCfg.mmu = mmu;
            AfxMountClass(&dsys->contexts, classes, &clsCfg);
            clsCfg = _doutClsConfig;
            clsCfg.mmu = mmu;
            AfxMountClass(&dsys->outputs, classes, &clsCfg);
            clsCfg = _dinClsConfig;
            clsCfg.mmu = mmu;
            AfxMountClass(&dsys->inputs, classes, &clsCfg);
            clsCfg = _camClsConfig;
            clsCfg.mmu = mmu;
            AfxMountClass(&dsys->cameras, classes, &clsCfg);

            afxUri uri;
            AfxMakeUri(&uri, "system/e2draw.icd", 0);

            if (_AfxDsysLoadIcd(dsys, &uri, &dsys->e2draw)) AfxThrowError();
            else
            {
                AfxMakeUri(&uri, "system/*.icd", 0);
                _AfxDsysScanForIcds(dsys, &uri);

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
                AfxDismountClass(&dsys->cameras);
                AfxDismountClass(&dsys->inputs);
                AfxDismountClass(&dsys->outputs);
                AfxDismountClass(&dsys->contexts);
                AfxDismountClass(&dsys->devices);
                AfxDismountClass(&dsys->icds);
                AfxDismountClass(&dsys->threads);                
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

    AfxDismountClass(&dsys->cameras);
    AfxDismountClass(&dsys->inputs);
    AfxDismountClass(&dsys->outputs);
    AfxDismountClass(&dsys->contexts);
    AfxDismountClass(&dsys->devices);
    AfxDismountClass(&dsys->icds);
    AfxDismountClass(&dsys->threads);

    AfxReleaseObjects(1, (void*[]) { dsys->mmu });

    return err;
}

_AFX void AfxChooseDrawSystemConfiguration(afxDrawSystemConfig *config, afxNat extendedSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    *config = (afxDrawSystemConfig) { 0 };
    config->fcc = afxFcc_DSYS;
}
