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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_INPUT_C

#include "qwadro/draw/afxDrawSystem.h"

_AVX afxBool dsysReady = FALSE;
_AVX afxByte theDsysData[AFX_ALIGN(sizeof(afxObjectBase), 16) + AFX_ALIGN(sizeof(AFX_OBJECT(afxDrawSystem)), 16)] = { 0 };
_AVX afxDrawSystem TheDrawSystem = (void*)&theDsysData;
AFX_STATIC_ASSERT(sizeof(theDsysData) >= (sizeof(afxObjectBase) + sizeof(TheDrawSystem[0])), "");

//extern afxChain* _AfxGetSystemClassChain(void);
extern afxClassConfig const _AvxCamMgrCfg;
extern afxClassConfig const _AvxDoutMgrCfg;
extern afxClassConfig const _AvxDinMgrCfg;

_AVX afxString const sgl2Signature = AFX_STRING(
    "           :::     :::::::::   ::::::::  :::              :::  ::::::::        \n"
    "          :+:      :+:    :+: :+:    :+: :+:             :+:  :+:    :+:       \n"
    "         +:+ +:+   +:+    +:+ +:+        +:+            +:+         +:+        \n"
    "        +#+  +:+   +#+    +:+ :#:        +#+           +#+        +#+          \n"
    "       +#+#+#+#+#+ +#+    +#+ +#+   +#+# +#+          +#+       +#+            \n"
    "             #+#   #+#    #+# #+#    #+# #+#         #+#       #+#             \n"
    "             ###   #########   ########  ########## ###       ##########       \n"
    "                                                                               \n"
    "              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                   (c) 2017 SIGMA, Engineering In Technology                   \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

_AVX afxBool AfxGetDrawSystem(afxDrawSystem* system)
{
    afxError err = AFX_ERR_NONE;
    //AfxTryAssertObjects(1, &TheDrawSystem, afxFcc_DSYS);
    AfxAssert(system);
    *system = TheDrawSystem;
    return dsysReady;
}

_AVX afxManager* AfxGetCameraClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager *cls = &dsys->camMgr;
    AfxAssertClass(cls, afxFcc_CAM);
    return cls;
}

_AVX afxManager* AfxGetDrawDeviceManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->ddevMgr;
    AfxAssertClass(cls, afxFcc_DDEV);
    return cls;
}

_AVX afxManager* AfxGetDrawBridgeClass(afxDrawDevice ddev, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertRange(ddev->portCnt, portIdx, 1);
    afxManager* cls = &ddev->contexts;
    AfxAssertClass(cls, afxFcc_DCTX);
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

_AVX void* AfxGetDrawDeviceIdd(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->idd;
}

_AVX afxManager* AfxGetDrawInputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->dinMgr;
    AfxAssertClass(cls, afxFcc_DIN);
    return cls;
}

_AVX afxManager* AfxGetDrawOutputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->doutMgr;
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
    afxManager* cls = AfxGetDrawDeviceManager();
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

_AVX afxBool _AvxDdrvCurateProxyDctxCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (afxDrawContext)obj;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(dctx) != proxy->ddev || proxy->fdctx(dctx, proxy->udd); // don't interrupt curation;
}

_AVX afxBool _AvxDdrvCurateProxyDoutCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (afxDrawOutput)obj;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetDrawOutputDevice(dout) != proxy->ddev || proxy->fdout(dout, proxy->udd); // don't interrupt curation;
}

_AVX afxBool _AvxDdrvCurateProxyDinCb(afxObject obj, void *udd)
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

_AVX void AfxGetDrawPortCapabilities(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertRange(ddev->portCnt, portIdx, 1);
    AfxAssert(caps);
    *caps = ddev->ports[portIdx].portCaps;
}

_AVX void AfxGetDrawDeviceCapabilities(afxDrawDevice ddev, afxDrawDeviceCaps* caps)
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

_AVX afxReal64 AfxFindPhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(screenWidth);
    AfxAssert(screenHeight);
    afxReal64 ratio, div = (afxReal64)screenWidth / (afxReal64)screenHeight;
    
    if (div <= 1.4)
        ratio = 1.33;
    else if (div <= 1.6)
        ratio = 1.5599999;
    else
        ratio = 1.78;

    return ratio;
}

_AVX afxReal64 AfxFindAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam)
{
    return AfxSin(fovY * 0.5) / AfxCos(fovY * 0.5) * errInPixels * distanceFromCam / ((afxReal64)vpHeightInPixels * 0.5);
}

_AVX afxNat AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetDrawDeviceManager();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxManager* cls = AfxGetDrawDeviceManager();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
}

_AVX afxBool AfxGetDrawDevice(afxNat ddevId, afxDrawDevice* device)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev = NIL;
    while (AfxEnumerateDrawDevices(ddevId, 1, &ddev))
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        AfxAssert(device);
        *device = ddev;
        break;
    }
    return !!ddev;
}

_AVX afxError _AvxDdevDtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    
    if (ddev->iddDtorCb(ddev))
        AfxThrowError();

    AfxAssert(!ddev->idd);
    AfxCleanUpChainedManagers(&ddev->dev.classes);

    AfxCleanUpMutex(&ddev->relinkedCndMtx);
    AfxCleanUpCondition(&ddev->relinkedCnd);

    if (ddev->ports)
        AfxDeallocate(ddev->ports);

    return err;
}

_AVX afxError _AvxDdevCtor(afxDrawDevice ddev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxDrawSystem dsys = cookie->udd[0];
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    //afxDrawDeviceInfo const* info = ((afxDrawDeviceInfo const *)cookie->udd[1]) + cookie->no;
    //AfxAssert(info);

    ddev->dev.serving = FALSE;

    AfxSetUpChain(&ddev->outputs, ddev);
    AfxSetUpChain(&ddev->inputs, ddev);

    AfxSetUpCondition(&ddev->relinkedCnd);
    AfxSetUpMutex(&ddev->relinkedCndMtx, AFX_MTX_PLAIN);
    
    afxDrawDeviceInfo info2 = { 0 };
    info2.clipSpace = AFX_CLIP_SPACE_QWADRO;

    if (AfxCallDevice(&ddev->dev, afxFcc_DSYS, &info2)) AfxThrowError();
    else
    {
        ddev->caps = info2.caps;
        ddev->limits = info2.limits;
        ddev->clipCfg = info2.clipSpace;

        if (!(AfxSumV3d(ddev->clipCfg.boundMin) + AfxSumV3d(ddev->clipCfg.boundMax)))
        {
            ddev->clipCfg = AFX_CLIP_SPACE_QWADRO;
        }

        ddev->dev.procCb = (void*)info2.procCb;
        ddev->idd = info2.idd;
        ddev->iddDtorCb = info2.iddDtorCb;
        ddev->dinIddCtorCb = info2.dinIddCtorCb;
        ddev->dinIddDtorCb = info2.dinIddDtorCb;
        ddev->dinRelinkCb = info2.dinRelinkCb;
        ddev->doutIddCtorCb = info2.doutIddCtorCb;
        ddev->doutIddDtorCb = info2.doutIddDtorCb;
        ddev->doutRelinkCb = info2.doutRelinkCb;

        AfxAssert(info2.portCnt);
        afxNat portCnt = AfxMax(1, info2.portCnt);
        ddev->portCnt = portCnt;
        AfxAssert(ddev->portCnt);

        if (!(ddev->ports = AfxAllocate(portCnt, sizeof(ddev->ports[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            afxChain* classes = &ddev->dev.classes;
            afxClassConfig tmpClsCfg;

            for (afxNat i = 0; i < portCnt; i++)
            {
                ddev->ports[i].portCaps = info2.portCaps[i];

                tmpClsCfg = *info2.ddgeClsCfg;
                AfxEstablishManager(&ddev->ports[i].ddgeMgr, NIL, classes, &tmpClsCfg);
            }

            // dctx must be after ddge
            tmpClsCfg = *info2.dctxClsCfg;
            AfxEstablishManager(&ddev->contexts, NIL, classes, &tmpClsCfg); // require ddge, diob

            if (info2.iddCtorCb(ddev)) AfxThrowError();
            else
            {

            }

            if (err)
            {
                AfxCleanUpChainedManagers(&ddev->dev.classes);
                AfxDeallocate(ddev->ports);
            }
        }
    }
    return err;
}

_AVX afxClassConfig const _AvxDdevMgrCfg =
{
    .fcc = afxFcc_DDEV,
    .name = "DrawDevice",
    .desc = "Draw Device Driver Interface",
    .size = sizeof(AFX_OBJECT(afxDrawDevice)),
    .ctor = (void*)_AvxDdevCtor,
    .dtor = (void*)_AvxDdevDtor
};

_AVX afxError _AvxDsysCtor(afxDrawSystem dsys, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManifest const* ini = cookie->udd[1];
    //afxDrawSystemConfig const* config = cookie->udd[2];

    afxChain* managers = &dsys->managers;
    AfxSetUpChain(managers, dsys);
    afxClassConfig clsCfg;

    clsCfg = _AvxCamMgrCfg;
    AfxEstablishManager(&dsys->camMgr, NIL, managers, &clsCfg);

    clsCfg = _AvxDdevMgrCfg;
    AfxEstablishManager(&dsys->ddevMgr, AfxGetDeviceClass(), managers, &clsCfg); // require base*

    clsCfg = _AvxDoutMgrCfg;
    AfxEstablishManager(&dsys->doutMgr, NIL, managers, &clsCfg); // require ddev, dout

    clsCfg = _AvxDinMgrCfg;
    AfxEstablishManager(&dsys->dinMgr, NIL, managers, &clsCfg); // require ddev, din

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
                                (0 == AfxCompareString(&s, &AfxString("DRAW"))))
                            {
                                afxManager* cls = AfxGetDrawDeviceManager();
                                AfxAssertClass(cls, afxFcc_DDEV);

                                afxDrawDeviceInfo info = { 0 };
                                //info.dev.manifest = &ini;
                                info.dev.type = devType;
                                info.dev.manifestUri = manifestFile;
                                info.dev.manifestPagNo = devPagIdx;

                                afxDrawDevice ddev;

                                if (AfxAcquireObjects(cls, 1, (afxObject*)&ddev, (void const*[]) { dsys, &info })) AfxThrowError();
                                else
                                {
                                    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

                                    if (err)
                                        AfxReleaseObjects(1, &ddev), ddev = NIL;
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

_AVX afxError _AvxDsysDtor(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    AfxCleanUpChainedManagers(&dsys->managers);

    return err;
}

_AVX afxManager* _AvxGetDsysMgr(void)
{
    //afxError err = AFX_ERR_NONE;
    static afxManager dsysMgr = { 0 };
    static afxBool dsysMgrReady = FALSE;
    static afxClassConfig const dsysMgrCfg =
    {
        .fcc = afxFcc_DSYS,
        .name = "DrawSystem",
        .desc = "Unified Video Graphics Infrastructure",
        .maxCnt = 1,
        //.size = sizeof(AFX_OBJECT(afxDrawSystem)),
        .ctor = (void*)_AvxDsysCtor,
        .dtor = (void*)_AvxDsysDtor
    };

    if (dsysMgr.fcc != afxFcc_CLS)
    {
        AfxEstablishManager(&dsysMgr, NIL, /*_AfxGetSystemClassChain()*/NIL, &dsysMgrCfg);
        dsysMgrReady = TRUE;
    }
    return &dsysMgr;
}

_AVX afxResult _AvxDsysctl(afxSystem sys, afxInt reqCode, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    switch (reqCode)
    {
    case 0:
    {

        break;
    }
    default:
    {
        AfxThrowError();
        break;
    }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxEntryPoint(afxModule mdle, afxNat reqCode, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case afxFcc_SYS:
    {
        afxDrawSystem dsys;

        if (!AfxGetDrawSystem(&dsys))
        {
            AfxAssert(TheDrawSystem == dsys);
            AfxZero(TheDrawSystem, sizeof(afxObjectBase));

            afxManager* mgr = _AvxGetDsysMgr();
            AfxAssertClass(mgr, afxFcc_DSYS);

            if (_AfxConstructObjects(mgr, 1, (void**)&TheDrawSystem, udd)) AfxThrowError();
            else
            {
                AfxAssert(TheDrawSystem != dsys); // Attention! Ctor moves the object pointer to hide out the object base.
                dsys = TheDrawSystem;
                AfxAssertObjects(1, &dsys, afxFcc_DSYS);
                dsysReady = TRUE;
            }
        }
        else
        {
            AfxAssert(TheDrawSystem == dsys);
            AfxAssertObjects(1, &dsys, afxFcc_DSYS);
            dsysReady = FALSE;

            afxManager* mgr = _AvxGetDsysMgr();
            AfxAssertClass(mgr, afxFcc_DSYS);

            if (_AfxDestructObjects(mgr, 1, (void**)&TheDrawSystem))
                AfxThrowError();

            AfxAssert(TheDrawSystem != dsys); // Attention! Dtor moves the object pointer to expose the object base.
            AfxZero(TheDrawSystem, sizeof(afxObjectBase));
        }
    }
    default: break;
    }
    return err;
}
