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
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
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

#include "afx/core/afxSystem.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/afxCamera.h"

extern afxClassConfig const _AfxDthrClsConfig;
extern afxClassConfig const _AfxCamClsConfig;

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

_AFX afxBool _AfxDdrvCurateProxyDctxCb(afxHandle obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (afxDrawContext)obj;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(dctx) != proxy->ddev || proxy->fdctx(dctx, proxy->udd); // don't interrupt curation;
}

_AFX afxBool _AfxDdrvCurateProxyDoutCb(afxHandle obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (afxDrawOutput)obj;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetDrawOutputDevice(dout) != proxy->ddev || proxy->fdout(dout, proxy->udd); // don't interrupt curation;
}

_AFX afxBool _AfxDdrvCurateProxyDinCb(afxHandle obj, void *udd)
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
    *caps = ddev->portCaps[portIdx];
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

    afxDrawDevice ddrv = AfxGetObjectProvider(ddev);
    AfxAssertObjects(1, &ddrv, afxFcc_DDRV);
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxContext ctx = AfxGetDrawSystemMemory(dsys);
    AfxAssertObjects(1, &ctx, afxFcc_CTX);

    _AfxUninstallChainedClasses(&ddev->classes);

    return err;
}

_AFX afxError _AfxDdevCtor(afxDrawDevice ddev, afxCookie const* cookie)
{
    AfxEntry("ddev=%p", ddev);
    afxError err = AFX_ERR_NONE;

    afxDrawDeviceInfo const* info = ((afxDrawDeviceInfo const *)cookie->udd[0]) + cookie->no;
    AfxAssert(info);

    afxDrawDevice ddrv = AfxGetObjectProvider(ddev);
    AfxAssertObjects(1, &ddrv, afxFcc_DDRV);
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxContext ctx = AfxGetDrawSystemMemory(dsys);

    AfxReplicateString(&ddev->domain, info->domain);
    AfxReplicateString(&ddev->name, info->name);

    afxChain *classes = &ddev->classes;
    AfxAcquireChain(classes, ddev);

    AfxInstallClassExtension(&ddev->contexts, &dsys->contexts, classes, info->dctxClsConfig);
    AfxInstallClassExtension(&ddev->outputs, &dsys->outputs, classes, info->doutClsConfig);
    AfxInstallClassExtension(&ddev->inputs, &dsys->inputs, classes, info->dinClsConfig);

    ddev->caps = info->caps ? *info->caps : (afxDrawDeviceCaps) { 0 };
    ddev->limits = info->limits ? *info->limits : (afxDrawDeviceLimits) { 0 };

    ddev->portCnt = AfxMaxi(1, info->portCnt);
    AfxAssert(ddev->portCnt);
    ddev->portCaps = AfxAllocate(ctx, sizeof(ddev->portCaps[0]), ddev->portCnt, 0, AfxHint());

    ddev->serving = FALSE;

    for (afxNat i = 0; i < ddev->portCnt; i++)
        ddev->portCaps[i] = info->portCaps[i];

    ddev->procCb = NIL;
    ddev->serving = FALSE;

    if (err)
        _AfxUninstallChainedClasses(&ddev->classes);

    return err;
}

_AFX afxError _AfxDdrvDtor(afxDrawIcd ddrv)
{
    AfxEntry("ddrv=%p", ddrv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddrv, afxFcc_DDRV);

    //afxContext mem = AfxGetDrawSystemMemory();

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
    
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    //afxContext ctx = AfxGetDrawSystemMemory(dsys);

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
    AfxAcquireChain(classes, ddrv);

    afxClassConfig tmpClsConfig = *info->devExt;
    tmpClsConfig.unitsPerPage = info->devCnt;
    
    if (AfxInstallClassExtension(&ddrv->devices, &dsys->devices, classes, &tmpClsConfig)) AfxThrowError();
    else
    {
        afxNat devCnt = AfxMini(info->devCnt, 32);
        afxDrawDevice ddev[32];
        afxDrawDeviceInfo devInfos[32];
        afxError(*getDevInfoCb)(afxNat,afxDrawDeviceInfo*) = AfxFindModuleSymbol(ddrv->mdle, "AfxGetDrawDeviceInformation");

        if (!getDevInfoCb || getDevInfoCb(devCnt, devInfos)) AfxThrowError();
        else
        {
            if (AfxAcquireObjects(&ddrv->devices, devCnt, (afxHandle*)ddev, (void*[]) { devInfos })) AfxThrowError();
            else
            {
                AfxAssertObjects(devCnt, ddev, afxFcc_DDEV);

                for (afxNat i = 0; i < devCnt; i++)
                {

                    AfxLogMessageFormatted(0xFFFF0000, "\nDraw device '%.*s' <'%.*s'> discovered on draw system %p.", AfxPushString(&ddev[i]->name), AfxPushString(&ddev[i]->domain), dsys);
                }

                if (err)
                    AfxReleaseObjects(devCnt, (afxHandle*)ddev);
            }
        }

        if (err)
            _AfxUninstallChainedClasses(&ddrv->classes);
    }

    if (err)
        AfxReleaseObjects(1, (void*[]) { ddrv->mdle });

    return err;
}

_AFX afxClass* AfxGetDrawDeviceClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxClass* cls = &dsys->devices;
    AfxAssertClass(cls, afxFcc_DDEV);
    return cls;
}

_AFX afxClass* AfxGetCameraClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxClass *class = &dsys->cameras;
    AfxAssertClass(class, afxFcc_CAM);
    return class;
}

_AFX afxNat AfxCountDrawDevices(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    AfxAssertClass(&dsys->devices, afxFcc_DDEV);
    return AfxCountInstances(&dsys->devices);
}

_AFX afxClass* AfxGetDrawThreadClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxClass* cls = &dsys->threads;
    AfxAssertClass(cls, afxFcc_DTHR);
    return cls;
}

_AFX afxNat AfxEnumerateDrawDevices(afxDrawSystem dsys, afxNat first, afxNat cnt, afxDrawDevice ddev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(ddev);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    AfxAssertClass(&dsys->devices, afxFcc_DDEV);
    return AfxEnumerateInstances(&dsys->devices, first, cnt, (afxHandle*)ddev);
}

_AFX afxNat AfxCurateDrawDevices(afxDrawSystem dsys, afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    AfxAssertClass(&dsys->devices, afxFcc_DDEV);
    return AfxCurateInstances(&dsys->devices, first, cnt, (void*)f, udd);
}

_AFX afxBool AfxGetDrawDevice(afxDrawSystem dsys, afxNat ddevIdx, afxDrawDevice *ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    AfxAssert(ddev);
    afxBool rslt = FALSE;

    if ((rslt = !!(AfxEnumerateDrawDevices(dsys, ddevIdx, 1, ddev))))
    {
        AfxAssertObjects(1, ddev, afxFcc_DDEV);
    }
    return rslt;
}

_AFX afxContext AfxGetDrawSystemMemory(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxContext ctx = dsys->ctx;
    AfxAssertObjects(1, &ctx, afxFcc_CTX);
    return ctx;
}

_AFX afxError _AfxDsysDtor(afxDrawSystem dsys)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    //AfxReleaseObjects(1, (void*[]) { dsys->e2draw });

    _AfxUninstallChainedClasses(&dsys->classes);

    AfxReleaseObjects(1, (void*[]) { dsys->ctx });

    return err;
}

_AFX afxDrawIcd _AfxDsysFindIcd(afxDrawSystem dsys, afxUri const *file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    
    afxUri target;
    AfxAssertType(file, afxFcc_URI);
    AfxGetUriObject(&target, file);

    afxNat i = 0;
    afxDrawIcd icd;
    AfxAssertClass(&dsys->icds, afxFcc_DDRV);
    while(AfxEnumerateInstances(&dsys->icds, i, 1, (afxHandle*)&icd))
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
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    AfxAssertType(file, afxFcc_URI);
    AfxComment("Loading draw ICD %.*s", AfxPushString(AfxGetUriString(file)));

    afxUri file2;
    AfxGetUriObject(&file2, file);
    afxDrawIcd ddrv2 = _AfxDsysFindIcd(dsys, &file2);

    if ((ddrv2 = _AfxDsysFindIcd(dsys, &file2))) AfxReacquireObjects(1, (void*[]) { ddrv2 });
    else
    {
        afxModule mdle;

        if (AfxAcquireModules(1, &mdle, &file2)) AfxThrowError();
        else
        {
            afxError(*getIcdInfoCb)(afxDrawIcdInfo*) = AfxFindModuleSymbol(mdle, "AfxGetDrawIcdInformation");
            afxDrawIcdInfo icdInfo;

            if (getIcdInfoCb)
            {
                if (getIcdInfoCb(&icdInfo)) AfxThrowError();
                else
                {
                    if (AfxAcquireObjects(&dsys->icds, 1, (afxHandle*)&ddrv2, (void*[]) { (void*)&icdInfo })) AfxThrowError();
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
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    HANDLE fh;    
    WIN32_FIND_DATAA wfd;    
    afxUri2048 pathBuf;
    AfxUri2048(&pathBuf);
    AfxResolveUri(fileMask, AFX_FILE_FLAG_RX, &pathBuf.uri);
    if ((fh = FindFirstFileA(AfxGetUriData(&pathBuf.uri, 0), &(wfd))))
    {
        do
        {
            afxUri uri, file;
            AfxMakeUri(&uri, wfd.cFileName, 0);
            AfxGetUriObject(&file, &uri);
            afxDrawIcd ddrv2 = _AfxDsysFindIcd(dsys, &file);
            
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

_AFX afxError _AfxDsysCtor(afxDrawSystem dsys, afxCookie const* cookie)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    afxDrawSystemConfig const *config = ((afxDrawSystemConfig const *)cookie->udd[0]) + cookie->no;
    AfxAssert(config);

    if (!(dsys->ctx = config->ctx))
    {
        dsys->ctx = AfxGetSystemContext();
    }

    afxContext ctx = dsys->ctx;

    AfxAssertObjects(1, &dsys->ctx, afxFcc_CTX);

    if (AfxReacquireObjects(1, (void*[]) { dsys->ctx })) AfxThrowError();
    else
    {
        {
            afxChain *classes = &dsys->classes;
            AfxAcquireChain(classes, (void*)dsys);

            afxClassConfig clsConfig = _AfxDthrClsConfig;
            clsConfig.unitsPerPage = AfxGetThreadingCapacity();
            clsConfig.ctx = ctx;
            AfxMountClass(&dsys->threads, classes, &clsConfig);

            afxClassConfig const ddrvClsConfig =
            {
                .fcc = afxFcc_DDRV,
                .name = "Installable Client (Draw) Driver",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawIcd)),
                .ctx = ctx,
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
                .ctx = ctx,
                .ctor = (void*)_AfxDdevCtor,
                .dtor = (void*)_AfxDdevDtor
            };
            AfxMountClass(&dsys->devices, classes, &ddevClsConfig);

            afxClassConfig const dctxClsConfig =
            {
                .fcc = afxFcc_DCTX,
                .name = "Draw Context",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawContext)),
                .ctx = ctx,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };
            AfxMountClass(&dsys->contexts, classes, &dctxClsConfig);

            afxClassConfig const doutClsConfig =
            {
                .fcc = afxFcc_DOUT,
                .name = "Draw Output",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawOutput)),
                .ctx = ctx,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };
            AfxMountClass(&dsys->outputs, classes, &doutClsConfig);

            afxClassConfig const dinClsConfig =
            {
                .fcc = afxFcc_DIN,
                .name = "Draw Input",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawInput)),
                .ctx = ctx,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };
            AfxMountClass(&dsys->inputs, classes, &dinClsConfig);

            AfxMountClass(&dsys->cameras, classes, &_AfxCamClsConfig);

            afxUri uri;
            AfxMakeUri(&uri, "system/e2draw.icd", 0);

            if (_AfxDsysLoadIcd(dsys, &uri, &dsys->e2draw)) AfxThrowError();
            else
            {
                AfxMakeUri(&uri, "system/*.icd", 0);
                _AfxDsysScanForIcds(dsys, &uri);

                afxDrawThread dthr[16];
                afxNat threadCnt = AfxMini(AfxGetThreadingCapacity(), 1);
                //afxDrawThreadConfig dthrConfig = { 0 };
                //dthrConfig.udd = ddev->dpus[]

                if (AfxAcquireDrawThreads(dsys, threadCnt, NIL, dthr)) AfxThrowError();
                else
                {
                    for (afxNat i = 0; i < threadCnt; i++)
                    {
                        AfxAssertObjects(1, &dthr[i], afxFcc_DTHR);
                        afxThread thr = AfxGetDrawThreadBase(dthr[i]);
                        AfxSetThreadAffinity(thr, i);
                        AfxRunThread(thr);
                    }
                }

                if (err)
                    AfxReleaseObjects(1, (void*[]) { dsys->e2draw });
            }

            if (err)
                _AfxUninstallChainedClasses(&dsys->classes);
        }

        if (err)
            AfxReleaseObjects(1, (void*[]) { dsys->ctx });
    }

    return err;
}

_AFX afxError AfxAcquireDrawSystems(afxNat cnt, afxDrawSystemConfig const config[], afxDrawSystem dsys[])
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if (!AfxGetSystem(&sys)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sys, afxFcc_SYS);

        if (AfxAcquireObjects(&sys->dsystems, cnt, (afxHandle*)dsys, (void*[]) { (void*)config }))
            AfxThrowError();
    }
    return err;
}

_AFX void AfxChooseDrawSystemConfiguration(afxDrawSystemConfig *config, afxNat extendedSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    AfxZero(1, AfxMaxi(extendedSiz, sizeof(*config)), config);
}

_AFX afxClassConfig _AfxDsysClsConfig =
{
    .fcc = afxFcc_DSYS,
    .name = "Draw I/O System",
    .maxCnt = 1,
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawSystem)),
    .ctx = NIL,
    .ctor = (void*)_AfxDsysCtor,
    .dtor = (void*)_AfxDsysDtor
};