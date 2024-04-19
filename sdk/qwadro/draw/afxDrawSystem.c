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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

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

_AVX afxBool dsysReady = FALSE;
_AVX afxByte theDsysData[AFX_ALIGN(sizeof(afxObjectBase), 16) + AFX_ALIGN(sizeof(AFX_OBJECT(afxDrawSystem)), 16)] = { 0 };
_AVX afxDrawSystem TheDrawSystem = (void*)&theDsysData;
static_assert(sizeof(theDsysData) >= (sizeof(afxObjectBase) + sizeof(TheDrawSystem[0])), "");

//extern afxChain* _AfxGetSystemClassChain(void);
extern afxClassConfig const _dthrMgrCfg;
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

_AVX afxBool AfxGetDrawSystem(afxDrawSystem* dsys)
{
    afxError err = AFX_ERR_NONE;
    //AfxTryAssertObjects(1, &TheDrawSystem, afxFcc_DSYS);
    AfxAssert(dsys);
    *dsys = TheDrawSystem;
    return dsysReady;
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

_AVX afxReal64 AfxDeterminePhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight)
{
    afxReal64 ratio, div = (afxReal64)screenWidth / (afxReal64)screenHeight;
    
    if (div <= 1.4)
        ratio = 1.33;
    else if (div <= 1.6)
        ratio = 1.5599999;
    else
        ratio = 1.78;

    return ratio;
}

_AVX afxReal64 AfxDetermineAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam)
{
    return AfxSin(fovY * 0.5) / AfxCos(fovY * 0.5) * errInPixels * distanceFromCam / ((afxReal64)vpHeightInPixels * 0.5);
}

_AVX afxError _AvxDdevDtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

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

_AVX afxBool AfxGetDrawDevice(afxNat ddevId, afxDrawDevice* ddev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev2 = NIL;
    while (AfxEnumerateDrawDevices(ddevId, 1, &ddev2))
    {
        AfxAssertObjects(1, &ddev2, afxFcc_DDEV);
        AfxAssert(ddev);
        *ddev = ddev2;
        break;
    }
    return !!ddev2;
}

_AVX afxError _AvxDsysCtor(afxDrawSystem dsys, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManifest const* ini = cookie->udd[1];
    afxDrawSystemConfig const* config = cookie->udd[2];

    afxChain* mgrChn = &dsys->mgrChn;
    AfxSetUpChain(mgrChn, dsys);
    afxClassConfig clsCfg;

    clsCfg = _AvxCamMgrCfg;
    AfxEstablishManager(&dsys->cameras, NIL, mgrChn, &clsCfg);

    afxClassConfig const ddevMgrCfg =
    {
        .fcc = afxFcc_DDEV,
        .name = "DrawDevice",
        .desc = "Draw Device Management",
        .size = sizeof(AFX_OBJECT(afxDrawDevice)),
        .ctor = (void*)_AvxDdevCtor,
        .dtor = (void*)_AvxDdevDtor
    };
    AfxEstablishManager(&dsys->devices, AfxGetDeviceClass(), mgrChn, &ddevMgrCfg); // require dtxu, dthr

    clsCfg = _AvxDoutMgrCfg;
    AfxEstablishManager(&dsys->outputs, NIL, mgrChn, &clsCfg); // require ddev, dout

    clsCfg = _AvxDinMgrCfg;
    AfxEstablishManager(&dsys->inputs, NIL, mgrChn, &clsCfg); // require ddev, din

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
        AfxCleanUpChainedManagers(mgrChn);
    }
    return err;
}

_AVX afxError _AvxDsysDtor(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    //AfxReleaseObjects(1, (void*[]) { dsys->e2draw });

    //AfxAbolishManager(&dsys->dinputs);
    //AfxAbolishManager(&dsys->doutputs);
    //AfxAbolishManager(&dsys->dcontexts);
    //AfxAbolishManager(&dsys->ddevices);
    //AfxAbolishManager(&dsys->dthreads);
    AfxCleanUpChainedManagers(&dsys->mgrChn);

    return err;
}

_AVX afxManager* _AfxGetDsysMgr(void)
{
    //afxError err = AFX_ERR_NONE;
    static afxManager dsysMgr = { 0 };
    static afxBool dsysMgrReady = FALSE;
    static afxClassConfig const dsysMgrCfg =
    {
        .fcc = afxFcc_DSYS,
        .name = "Unified Video Graphics Infrastructure",
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
#if 0
    case 2:
    {
        afxDrawSystem dsys;

        if (!AfxGetDrawSystem(&dsys)) AfxThrowError();
        else
        {
            dsysReady = FALSE;

            afxManager* mgr = _AfxGetDsysMgr();
            AfxAssertClass(mgr, afxFcc_DSYS);

            AfxAssert(TheDrawSystem == dsys);

            if (_AfxDestructObjects(mgr, 1, (void**)&TheDrawSystem))
                AfxThrowError();

            AfxAssert(TheDrawSystem != dsys); // Attention! Dtor moves the object pointer to expose the object base.
            AfxZero(TheDrawSystem, sizeof(afxObjectBase));
        }
        break;
    }
    case 1:
    {
        afxDrawSystem dsys;

        if (AfxGetDrawSystem(&dsys)) AfxThrowError();
        else
        {
            AfxAssert(TheDrawSystem == dsys);
            AfxZero(TheDrawSystem, sizeof(afxObjectBase));

            afxManager* mgr = _AfxGetDsysMgr();
            AfxAssertClass(mgr, afxFcc_DSYS);

            va_list va;
            va_start(va, reqCode);

            if (_AfxConstructObjects(mgr, 1, (void**)&TheDrawSystem, (void*[]) { sys, va_arg(va, afxManifest*), va_arg(va, afxDrawSystemConfig*) })) AfxThrowError();
            else
            {
                AfxAssert(TheDrawSystem != dsys); // Attention! Ctor moves the object pointer to hide out the object base.
                dsys = TheDrawSystem;
                AfxAssertObjects(1, &dsys, afxFcc_DSYS);
                
                AfxPrintf(0xFF00FF00, "\n");

                for (afxNat i = 0; i < 1120; i++)
                    AfxPrintf(0xFF00FF00, "%.*s", 1, &sgl2Signature.start[i * 1]);
                
                Sleep(1000);

                dsysReady = TRUE;
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

_AVX void AfxChooseDrawSystemConfiguration(afxDrawSystemConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    *cfg = (afxDrawSystemConfig) { 0 };
}

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

            afxManager* mgr = _AfxGetDsysMgr();
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
            AfxAssertObjects(1, &dsys, afxFcc_DSYS);
            dsysReady = FALSE;

            afxManager* mgr = _AfxGetDsysMgr();
            AfxAssertClass(mgr, afxFcc_DSYS);

            AfxAssert(TheDrawSystem == dsys);

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
