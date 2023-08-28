/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
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

#define _AFX_DRAW_DRIVER_C
#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_SYSTEM_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_OUTPUT_C

#include "_classified/afxDrawClassified.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxSystem.h"
#include "afx/core/mem/afxContext.h"
#include "afx/draw/io/afxTarga.h"
#include "afxDrawParadigms.h"
#include "../src/e2coree/core/afxCoreHideout.h"

AFX_DEFINE_STRUCT(afxTextureStorageRegistry)
{
    afxFcc                  fcc;
    afxLinkage              dsys;

    afxString const*    name; // driver name: SIGGL
    afxString const*    author; // author: SIGMA Technology Group
    afxString const*    website; // website: sigmaco.org
    afxString const*    note; // The standard QWADRO draw system implementation.
    afxNat                  verMajor; // 1
    afxNat                  verMinor; // 0
    afxNat                  verPatch; // 0

    afxNat                  minScale;
    afxNat                  maxScale;
    afxNat                  minDepth;
    afxNat                  maxDepth;
    afxNat                  minBpp;
    afxNat                  maxBpp;
    afxBool                 hasAlpha;

    afxNat                  supportedFmtCnt;
    afxString const*    exts;
    afxError                (*acq)(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxUri const uri[]);
    // coacquire for batch layered textures such as DDS?
    afxError                (*upload)(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
    afxError                (*download)(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
    afxError                (*uploadRgn)(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
    afxError                (*downloadRgn)(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
};

extern afxClassConfig const _AfxDthrClsConfig;

_AFX afxBool AfxDrawDeviceIsRunning(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    return ddev->serving;
}

_AFX afxNat AfxCountDrawContexts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    return AfxCountInstances(&ddev->contexts);
}

_AFX afxNat AfxCountDrawOutputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    return AfxCountInstances(&ddev->outputs);
}

_AFX afxNat AfxCountDrawInputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    return AfxCountInstances(&ddev->inputs);
}

_AFX afxClass* AfxGetDrawContextClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    return &ddev->contexts;
}

_AFX afxClass* AfxGetDrawInputClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    return &ddev->inputs;
}

_AFX afxClass* AfxGetDrawOutputClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    return &ddev->outputs;
}

_AFX afxNat AfxEnumerateDrawInputs(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(din);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((devId != AFX_N32_MAX) && !(AfxGetInstance(&dsys->devices, devId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((devId != AFX_N32_MAX))
        {
            AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
            rslt = AfxEnumerateInstances(&ddev->inputs, first, cnt, (afxHandle*)din);
        }
        else
        {
            rslt = AfxEnumerateInstances(&ddev->inputs, first, cnt, (afxHandle*)din);
        }
    }
    return rslt;
}

_AFX afxNat AfxEnumerateDrawOutputs(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxDrawOutput dout[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(dout);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((devId != AFX_N32_MAX) && !(AfxGetInstance(&dsys->devices, devId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((devId != AFX_N32_MAX))
        {
            AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
            rslt = AfxEnumerateInstances(&ddev->outputs, first, cnt, (afxHandle*)dout);
        }
        else
        {
            rslt = AfxEnumerateInstances(&ddev->outputs, first, cnt, (afxHandle*)dout);
        }
    }
    return rslt;
}

_AFX afxNat AfxEnumerateDrawContexts(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxDrawContext dctx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(dctx);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((devId != AFX_N32_MAX) && !(AfxGetInstance(&dsys->devices, devId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((devId != AFX_N32_MAX))
        {
            AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
            rslt = AfxEnumerateInstances(&ddev->contexts, first, cnt, (afxHandle*)dctx);
        }
        else
        {
            rslt = AfxEnumerateInstances(&ddev->contexts, first, cnt, (afxHandle*)dctx);
        }
    }
    return rslt;
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
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(dctx) != proxy->ddev || proxy->fdctx(dctx, proxy->udd); // don't interrupt curation;
}

_AFX afxBool _AfxDdrvCurateProxyDoutCb(afxHandle obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (afxDrawOutput)obj;
    AfxAssertObjects(1, &dout, AFX_FCC_DOUT);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetDrawOutputDevice(dout) != proxy->ddev || proxy->fdout(dout, proxy->udd); // don't interrupt curation;
}

_AFX afxBool _AfxDdrvCurateProxyDinCb(afxHandle obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawInput din = (afxDrawInput)obj;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(din) != proxy->ddev || proxy->fdin(din, proxy->udd); // don't interrupt curation;
}

_AFX afxNat AfxCurateDrawContexts(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(cnt);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((devId != AFX_N32_MAX) && !(AfxGetInstance(&dsys->devices, devId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((devId != AFX_N32_MAX))
        {
            AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
            rslt = AfxCurateInstances(&ddev->contexts, first, cnt, (void*)f, udd);
        }
        else
        {
            rslt = AfxCurateInstances(&dsys->contexts, first, cnt, (void*)f, udd);
        }
    }
    return rslt;
}

_AFX afxNat AfxCurateDrawOutputs(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(cnt);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((devId != AFX_N32_MAX) && !(AfxGetInstance(&dsys->devices, devId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((devId != AFX_N32_MAX))
        {
            AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
            rslt = AfxCurateInstances(&ddev->outputs, first, cnt, (void*)f, udd);
        }
        else
        {
            rslt = AfxCurateInstances(&dsys->outputs, first, cnt, (void*)f, udd);
        }
    }
    return rslt;
}

_AFX afxNat AfxCurateDrawInputs(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(f);
    AfxAssert(cnt);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxDrawDevice ddev;
    afxNat rslt = 0;

    if ((devId != AFX_N32_MAX) && !(AfxGetInstance(&dsys->devices, devId, (afxHandle*)&ddev))) AfxThrowError();
    else
    {
        if ((devId != AFX_N32_MAX))
        {
            AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
            rslt = AfxCurateInstances(&ddev->inputs, first, cnt, (void*)f, udd);
        }
        else
        {
            rslt = AfxCurateInstances(&dsys->inputs, first, cnt, (void*)f, udd);
        }
    }
    return rslt;
}

_AFX afxNat AfxCountDrawPorts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    return ddev->portCnt;
}

_AFX void AfxGetDrawPortCaps(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    AfxAssertRange(ddev->portCnt, portIdx, 1);
    AfxAssert(caps);
    *caps = ddev->portCaps[portIdx];
}

_AFX void AfxGetDrawDeviceCaps(afxDrawDevice ddev, afxDrawDeviceCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    AfxAssert(caps);
    *caps = ddev->caps;
}

_AFX void AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawDeviceLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);
    AfxAssert(limits);
    *limits = ddev->limits;
}

_AFX afxError _AfxDdevDtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, AFX_FCC_DDEV);

    afxDrawDevice ddrv = AfxGetObjectProvider(ddev);
    AfxAssertObjects(1, &ddrv, AFX_FCC_DDRV);
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxContext ctx = AfxGetDrawSystemContext(dsys);
    AfxAssertObjects(1, &ctx, AFX_FCC_CTX);

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
    AfxAssertObjects(1, &ddrv, AFX_FCC_DDRV);
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxContext ctx = AfxGetDrawSystemContext(dsys);

    AfxExcerptString(&ddev->domain, info->domain);
    AfxExcerptString(&ddev->name, info->name);

    afxChain *classes = &ddev->classes;
    AfxAcquireChain(classes, ddev);

    AfxInstallClassExtension(&ddev->contexts, &dsys->contexts, classes, info->dctxClsConfig);
    AfxInstallClassExtension(&ddev->outputs, &dsys->outputs, classes, info->doutClsConfig);
    AfxInstallClassExtension(&ddev->inputs, &dsys->inputs, classes, info->dinClsConfig);

    ddev->caps = info->caps ? *info->caps : (afxDrawDeviceCaps) { 0 };
    ddev->limits = info->limits ? *info->limits : (afxDrawDeviceLimits) { 0 };

    ddev->portCnt = AfxMaxi(1, info->portCnt);
    AfxAssert(ddev->portCnt);
    ddev->portCaps = AfxAllocate(ctx, ddev->portCnt * sizeof(ddev->portCaps[0]), 0, AfxSpawnHint());

    ddev->serving = FALSE;

    for (afxNat i = 0; i < ddev->portCnt; i++)
        ddev->portCaps[i] = info->portCaps[i];

    ddev->vmt = NIL;
    ddev->serving = FALSE;

    if (err)
        _AfxUninstallChainedClasses(&ddev->classes);

    return err;
}

_AFX afxError _AfxDdrvDtor(afxDrawIcd ddrv)
{
    AfxEntry("ddrv=%p", ddrv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddrv, AFX_FCC_DDRV);

    //afxContext mem = AfxGetDrawSystemContext();

    _AfxUninstallChainedClasses(&ddrv->classes);

    AfxReleaseObjects(1, (void*[]) { ddrv->mdle });

    return err;
}

_AFX afxError _AfxDdrvCtor(afxDrawIcd ddrv, afxCookie const* cookie)
{
    AfxEntry("ddrv=%p", ddrv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddrv, AFX_FCC_DDRV);

    afxDrawIcdInfo const* info = ((afxDrawIcdInfo const *)cookie->udd[0]) + cookie->no;
    AfxAssert(info);
    
    afxDrawSystem dsys = AfxGetObjectProvider(ddrv);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    //afxContext ctx = AfxGetDrawSystemContext(dsys);

    AfxExcerptString(&ddrv->name, info->name);
    AfxExcerptString(&ddrv->vendor, info->vendor);
    AfxExcerptString(&ddrv->website, info->website);
    AfxExcerptString(&ddrv->note, info->note);
    ddrv->verMajor = info->verMajor;
    ddrv->verMinor = info->verMinor;
    ddrv->verPatch = info->verPatch;

    afxUri file;
    AfxExcerptUriObject(&file, AfxGetModulePath(info->mdle));

    AfxLogMessageFormatted(0xFFFF0000, "\nInstalling '%.*s' ICD on draw system %p...\n\t%.*s %u.%u.%u\n\tVendor: %.*s <%.*s>\n\tNote: %.*s", AfxPushString(AfxUriGetStringConst(&file)), dsys, AfxPushString(&ddrv->name), ddrv->verMajor, ddrv->verMinor, ddrv->verPatch, AfxPushString(&ddrv->vendor), AfxPushString(&ddrv->website), AfxPushString(&ddrv->note));

    ddrv->mdle = info->mdle;
    AfxAssertObjects(1, &ddrv->mdle, AFX_FCC_MDLE);
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
                AfxAssertObjects(devCnt, ddev, AFX_FCC_DDEV);

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
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxClass* cls = &dsys->devices;
    AfxAssertClass(cls, AFX_FCC_DDEV);
    return cls;
}

_AFX afxNat AfxCountDrawThreads(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssertClass(&dsys->threads, AFX_FCC_DTHR);
    return AfxCountInstances(&dsys->threads);
}

_AFX afxNat AfxCountDrawDevices(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssertClass(&dsys->devices, AFX_FCC_DDEV);
    return AfxCountInstances(&dsys->devices);
}

_AFX afxClass* AfxGetDrawThreadClass(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxClass* cls = &dsys->threads;
    AfxAssertClass(cls, AFX_FCC_DTHR);
    return cls;
}

_AFX afxNat AfxEnumerateDrawThreads(afxDrawSystem dsys, afxNat first, afxNat cnt, afxDrawThread dthr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(dthr);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssertClass(&dsys->threads, AFX_FCC_DTHR);
    return AfxEnumerateInstances(&dsys->threads, first, cnt, (afxHandle*)dthr);
}

_AFX afxNat AfxEnumerateDrawDevices(afxDrawSystem dsys, afxNat first, afxNat cnt, afxDrawDevice ddev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(ddev);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssertClass(&dsys->devices, AFX_FCC_DDEV);
    return AfxEnumerateInstances(&dsys->devices, first, cnt, (afxHandle*)ddev);
}

_AFX afxNat AfxCurateDrawDevices(afxDrawSystem dsys, afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssertClass(&dsys->devices, AFX_FCC_DDEV);
    return AfxCurateInstances(&dsys->devices, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxCurateDrawThreads(afxDrawSystem dsys, afxNat first, afxNat cnt, afxBool(*f)(afxDrawThread, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssertClass(&dsys->threads, AFX_FCC_DTHR);
    return AfxCurateInstances(&dsys->threads, first, cnt, (void*)f, udd);
}

_AFX afxBool AfxGetDrawDevice(afxDrawSystem dsys, afxNat devIdx, afxDrawDevice *ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssert(ddev);
    afxBool rslt = FALSE;

    if ((rslt = !!(AfxEnumerateDrawDevices(dsys, devIdx, 1, ddev))))
    {
        AfxAssertObjects(1, ddev, AFX_FCC_DDEV);
    }
    return rslt;
}

_AFX afxNat AfxChooseTextureStorage(afxDrawSystem dsys, afxTexture tex, afxTextureRegion const *rgn)
{
    afxError err = AFX_ERR_NONE;
    (void)dsys;
    (void)tex;
    (void)rgn;
    AfxThrowError();
    return 0;
}

_AFX afxResult AfxDescribeTextureStorages(afxDrawSystem dsys, afxNat first, afxNat cnt, afxTextureStorageSpecification spec[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(spec);

    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);;
    AfxAssertRange(dsys->texIoCodecs.cnt, first, cnt);
    
    afxResult rslt = 0;

    afxNat posn = 0;
    afxTextureStorageRegistry *codec;
    AfxChainForEveryLinkage(&dsys->texIoCodecs, afxTextureStorageRegistry, dsys, codec)
    {
        AfxAssert(codec->fcc == AFX_FCC_IMIO);

        if (posn >= first)
        {
            spec[rslt].name = codec->name;
            spec[rslt].author = codec->author;
            spec[rslt].website = codec->website;
            spec[rslt].note = codec->note;
            spec[rslt].verMajor = codec->verMajor;
            spec[rslt].verMinor = codec->verMinor;
            spec[rslt].verPatch = codec->verPatch;
            spec[rslt].extCnt = codec->supportedFmtCnt;
            spec[rslt].exts = codec->exts;
            spec[rslt].minScale = codec->minScale;
            spec[rslt].maxScale = codec->maxScale;
            spec[rslt].minDepth = codec->minDepth;
            spec[rslt].maxDepth = codec->maxDepth;
            spec[rslt].minBpp = codec->minBpp;
            spec[rslt].maxBpp = codec->maxBpp;
            spec[rslt].hasAlpha = codec->hasAlpha;
            spec[rslt].acq = codec->acq;
            spec[rslt].upload = codec->upload;
            spec[rslt].uploadRgn = codec->uploadRgn;
            spec[rslt].download = codec->download;
            spec[rslt].downloadRgn = codec->downloadRgn;
            ++rslt;
        }

        ++posn;

        if (posn >= first + cnt)
            break;
    }
    return rslt;
}

_AFX afxResult AfxEnumerateTextureStorages(afxDrawSystem dsys, afxNat first, afxNat cnt, afxTextureStorageRegistry *storage[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssert(cnt);
    AfxAssertRange(dsys->texIoCodecs.cnt, first, cnt);
    AfxAssert(storage);
    afxResult rslt = 0;

    afxNat posn = 0;
    afxTextureStorageRegistry *codec;
    AfxChainForEveryLinkage(&dsys->texIoCodecs, afxTextureStorageRegistry, dsys, codec)
    {
        AfxAssert(codec->fcc == AFX_FCC_IMIO);

        if (posn >= first)
        {
            storage[rslt] = codec;
            ++rslt;
        }

        ++posn;

        if (posn >= first + cnt)
            break;
    }
    return rslt;
}

_AFX afxResult AfxRegisterTextureStorages(afxDrawSystem dsys, afxNat cnt, afxTextureStorageSpecification const specs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssert(cnt);
    AfxAssert(specs);
    afxResult hitCnt = 0;
    afxTextureStorageRegistry* codec;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (!(codec = AfxAllocate(dsys->ctx, sizeof(*codec), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            codec->fcc = AFX_FCC_IMIO;
            AfxPushLinkage(&codec->dsys, &dsys->texIoCodecs);
            codec->supportedFmtCnt = 0;

            codec->name = specs[i].name;
            codec->author = specs[i].author;
            codec->website = specs[i].website;
            codec->note = specs[i].note;

            codec->verMajor = specs[i].verMajor;
            codec->verMinor = specs[i].verMinor;
            codec->verPatch = specs[i].verPatch;

            codec->acq = specs[i].acq;
            codec->upload = specs[i].upload;
            codec->uploadRgn = specs[i].uploadRgn;
            codec->download = specs[i].download;
            codec->downloadRgn = specs[i].downloadRgn;
            
            codec->exts = specs[i].exts;
            codec->supportedFmtCnt = specs[i].extCnt;

            AfxLogMessageFormatted(0xFFFF0000, "\nNew texture codec registered.");
            AfxLogMessageFormatted(0xFFFF0000, "\nName: %.*s", AfxPushString(codec->name));
            AfxLogMessageFormatted(0xFFFF0000, "\nAuthor: %.*s", AfxPushString(codec->author));
            AfxLogMessageFormatted(0xFFFF0000, "\nVersion: %u.%u.%u", codec->verMajor, codec->verMinor, codec->verPatch);
            AfxLogMessageFormatted(0xFFFF0000, "\nNote: %.*s", AfxPushString(codec->note));

            hitCnt++;
        }
    }
    return hitCnt;
}

_AFX afxContext AfxGetDrawSystemContext(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    afxContext ctx = dsys->ctx;
    AfxAssertObjects(1, &ctx, AFX_FCC_CTX);
    return ctx;
}

_AFX afxError _AfxDsysDtor(afxDrawSystem dsys)
{
    AfxEntry("dsys=%p", dsys);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);

    //AfxReleaseObjects(1, (void*[]) { dsys->e2draw });

    _AfxUninstallChainedClasses(&dsys->classes);

    while (1)
    {
        afxChain *codecs = &dsys->texIoCodecs;
        afxLinkage *first = AfxGetLastLinkage(codecs);

        if (!first) break;
        else
        {
            afxTextureStorageRegistry *codec = AFX_REBASE(first, afxTextureStorageRegistry, dsys);
            AfxAssertType(codec, AFX_FCC_IMIO);
            AfxPopLinkage(&codec->dsys);
            AfxDeallocate(dsys->ctx, codec);
        }
    }

    AfxReleaseObjects(1, (void*[]) { dsys->ctx });

    return err;
}

_AFX afxDrawIcd _AfxDsysFindIcd(afxDrawSystem dsys, afxUri const *file)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    
    afxUri target;
    AfxAssertType(file, AFX_FCC_URI);
    AfxExcerptUriObject(&target, file);

    afxNat i = 0;
    afxDrawIcd icd;
    AfxAssertClass(&dsys->icds, AFX_FCC_DDRV);
    while(AfxEnumerateInstances(&dsys->icds, i, 1, (afxHandle*)&icd))
    {
        afxUri tmp;
        AfxAssertObjects(1, &icd, AFX_FCC_DDRV);
        AfxExcerptUriObject(&tmp, AfxGetModulePath(icd->mdle));

        if (AfxUriIsEquivalent(&target, &tmp))
            return icd;

        ++i;
    }
    return NIL;
}

_AFX afxError _AfxDsysLoadIcd(afxDrawSystem dsys, afxUri const* file, afxDrawIcd *ddrv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    AfxAssertType(file, AFX_FCC_URI);
    AfxComment("Loading draw ICD %.*s", AfxPushString(AfxUriGetStringConst(file)));

    afxUri file2;
    AfxExcerptUriObject(&file2, file);
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
                        AfxAssertObjects(1, &ddrv2, AFX_FCC_DDRV);

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
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    HANDLE fh;    
    WIN32_FIND_DATAA wfd;    
    afxUri2048 pathBuf;
    AfxUri2048(&pathBuf);
    AfxResolveUri(fileMask, AFX_FILE_FLAG_RX, &pathBuf.uri);
    if ((fh = FindFirstFileA(AfxGetUriDataConst(&pathBuf.uri, 0), &(wfd))))
    {
        do
        {
            afxUri uri, file;
            AfxUriWrapLiteral(&uri, wfd.cFileName, 0);
            AfxExcerptUriObject(&file, &uri);
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
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);

    afxDrawSystemConfig const *config = ((afxDrawSystemConfig const *)cookie->udd[0]) + cookie->no;
    AfxAssert(config);

    if (!(dsys->ctx = config->ctx))
    {
        dsys->ctx = AfxGetSystemContext();
    }

    afxContext ctx = dsys->ctx;

    AfxAssertObjects(1, &dsys->ctx, AFX_FCC_CTX);

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
                .fcc = AFX_FCC_DDRV,
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
                .fcc = AFX_FCC_DDEV,
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
                .fcc = AFX_FCC_DCTX,
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
                .fcc = AFX_FCC_DOUT,
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
                .fcc = AFX_FCC_DIN,
                .name = "Draw Input",
                .unitsPerPage = 1,
                .size = sizeof(AFX_OBJECT(afxDrawInput)),
                .ctx = ctx,
                .ctor = (void*)NIL,
                .dtor = (void*)NIL
            };
            AfxMountClass(&dsys->inputs, classes, &dinClsConfig);

            AfxAcquireChain(&dsys->texIoCodecs, (void*)dsys);

            static afxString codecName, codecAuthor, codecWebsite, codecNote, ext;
            afxTextureStorageSpecification stdTexCodec = { 0 };
            AfxWrapStringLiteral(&codecName, "SIGGL", 0);
            AfxWrapStringLiteral(&codecAuthor, "SIGMA Technology Group", 0);
            AfxWrapStringLiteral(&codecWebsite, "www.sigmaco.org", 0);
            AfxWrapStringLiteral(&codecNote, "", 0);
            AfxWrapStringLiteral(&ext, ".tga", 0);

            stdTexCodec.name = &codecName;
            stdTexCodec.author = &codecAuthor;
            stdTexCodec.website = &codecWebsite;
            stdTexCodec.note = &codecNote;
            stdTexCodec.verMajor = 0;
            stdTexCodec.verMinor = 5;
            stdTexCodec.verPatch = 1;
            stdTexCodec.extCnt = 1;
            stdTexCodec.exts = &ext;
            stdTexCodec.acq = AfxLoadTexturesTarga;
            stdTexCodec.download = AfxPrintTextureToTarga;
            stdTexCodec.downloadRgn = AfxPrintTextureRegionsToTarga;
            stdTexCodec.upload = AfxFetchTextureFromTarga;
            stdTexCodec.uploadRgn = AfxFetchTextureRegionsFromTarga;

            if (1 != AfxRegisterTextureStorages(dsys, 1, &stdTexCodec)) AfxThrowError();
            else
            {
                AfxEcho("Truecolor Targa texture serializer installed!");

                afxUri uri;
                AfxUriWrapLiteral(&uri, "system/e2draw.icd", 0);

                if (_AfxDsysLoadIcd(dsys, &uri, &dsys->e2draw)) AfxThrowError();
                else
                {
                    AfxUriWrapLiteral(&uri, "system/*.icd", 0);
                    _AfxDsysScanForIcds(dsys, &uri);

                    afxDrawThread dthr[16];
                    afxNat threadCnt = AfxMini(AfxGetThreadingCapacity(), 1);
                    afxDrawThreadConfig dthrConfig = { 0 };
                    //dthrConfig.udd = ddev->dpus[]

                    if (AfxAcquireDrawThreads(dsys, threadCnt, dthr, NIL)) AfxThrowError();
                    else
                    {
                        for (afxNat i = 0; i < threadCnt; i++)
                        {
                            AfxAssertObjects(1, &dthr[i], AFX_FCC_DTHR);
                            afxThread thr = AfxGetDrawThreadBase(dthr[i]);
                            AfxSetThreadAffinity(thr, i);
                            AfxRunThread(thr);
                        }
                    }

                    if (err)
                        AfxReleaseObjects(1, (void*[]) { dsys->e2draw });
                }
            }

            if (err)
                _AfxUninstallChainedClasses(&dsys->classes);
        }

        if (err)
            AfxReleaseObjects(1, (void*[]) { dsys->ctx });
    }

    return err;
}

_AFX afxError AfxAcquireDrawSystems(afxNat cnt, afxDrawSystem dsys[], afxDrawSystemConfig const config[])
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;

    if (!AfxGetSystem(&sys)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sys, AFX_FCC_SYS);

        if (AfxAcquireObjects(&sys->dsystems, cnt, (afxHandle*)dsys, (void*[]) { (void*)config }))
            AfxThrowError();
    }
    return err;
}

_AFX void AfxChooseDrawSystemConfiguration(afxDrawSystemConfig *config, afxNat extendedSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(config);
    AfxZero(config, AfxMaxi(extendedSiz, sizeof(*config)));
}

_AFX afxClassConfig _AfxDsysClsConfig =
{
    .fcc = AFX_FCC_DSYS,
    .name = "Draw System",
    .maxCnt = 1,
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawSystem)),
    .ctx = NIL,
    .ctor = (void*)_AfxDsysCtor,
    .dtor = (void*)_AfxDsysDtor
};
