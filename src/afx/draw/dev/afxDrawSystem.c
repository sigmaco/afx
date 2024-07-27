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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_INPUT_C
#include "AvxDevKit.h"

_AVX AFX_OBJECT(afxDrawSystem) TheDrawSystem = { 0 };
//extern afxChain* _AfxGetSystemClassChain(void);
extern afxClassConfig const _AvxDdevClsCfg;
extern afxClassConfig const _AvxDoutClsCfg;
extern afxClassConfig const _AvxDinClsCfg;

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

_AVX afxClass const* AvxGetDrawDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &TheDrawSystem.ddevCls;
    AfxAssertClass(cls, afxFcc_DDEV);
    return cls;
}

_AVX afxClass const* AvxGetDrawInputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &TheDrawSystem.dinCls;
    AfxAssertClass(cls, afxFcc_DIN);
    return cls;
}

_AVX afxClass const* AvxGetDrawOutputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &TheDrawSystem.doutCls;
    AfxAssertClass(cls, afxFcc_DOUT);
    return cls;
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

_AVX afxNat AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxClass const* cls = AvxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)devices);
}

_AVXINL afxBool _AvxTestDdevFltCb(afxDrawDevice ddev, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ((afxBool(*)(afxDrawDevice, void*))udd[0])(ddev, udd[1]);
}

_AVX afxNat AfxEvokeDrawDevices(afxBool(*f)(afxDrawDevice, void*), void* udd, afxNat first, afxNat cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxClass const* cls = AvxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEvokeClassInstances(cls, (void*)f, udd, first, cnt, (afxObject*)devices);
}

_AVX afxNat AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AvxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxInvokeDrawDevices2(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void* udd, afxBool(*f2)(afxDrawDevice, void*), void* udd2)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    AfxAssert(f2);
    afxClass const* cls = AvxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxInvokeClassInstances2(cls, first, cnt, (void*)f, udd, (void*)f2, udd2);
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

_AVX afxNat AfxChooseDrawDevices(afxDrawDeviceCaps const* caps, afxDrawDeviceLimits const* limits, afxNat maxCnt, afxNat ddevId[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(limits);
    AfxAssert(caps);
    afxNat rslt = 0;

    afxNat i = 0;
    afxDrawDevice ddev;
    while (AfxEnumerateDrawDevices(i, 1, &ddev))
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (AfxIsDrawDeviceAcceptable(ddev, caps, limits))
        {
            ddevId[rslt] = i;
            ++rslt;

            if (maxCnt > rslt)
                break;
        }
        i++;
    }
    return rslt;
}

_AVX afxBool _AvxParseDdevManifestCb(afxUri const* manifestUri, void* udd)
{
    afxError err = AFX_ERR_NONE;

    afxManifest ini;
    AfxSetUpIni(&ini);
    afxUri manifestFile;
    AfxClipUriFile(&manifestFile, manifestUri);
    AfxIniLoadFromFile(&ini, &manifestFile);

    afxDeviceOrder devType = afxDeviceOrder_DRAW;

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
                    afxClass* cls = (afxClass*)AvxGetDrawDeviceClass();
                    AfxAssertClass(cls, afxFcc_DDEV);

                    afxDrawDeviceInfo info = { 0 };
                    //info.dev.manifest = &ini;
                    info.dev.order = devType;
                    info.dev.manifestUri = manifestFile;
                    info.dev.manifestPagNo = devPagIdx;

                    afxDrawDevice ddev;

                    if (AfxAcquireObjects(cls, 1, (afxObject*)&ddev, (void const*[]) { udd, &info })) AfxThrowError();
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
    return err;
}

_AVX afxError _AvxScanDdevManifests(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri fileMask;
    AfxMakeUri(&fileMask, 0, "system/*.inf", 0);
    AfxFindFiles(&fileMask, _AvxParseDdevManifestCb, sys);
}

_AVX afxError _AvxScanDdevs(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    // scan for device drivers
    {
        HANDLE fh;
        WIN32_FIND_DATAA wfd;
        afxUri2048 pathBuf;
        AfxMakeUri2048(&pathBuf, NIL);
        afxUri fileMask;
        AfxMakeUri(&fileMask, 0, "//./z/*.inf", 0);
        AfxResolveUri(afxFileFlag_RX, &fileMask, &pathBuf.uri);

        afxDeviceOrder devType = afxDeviceOrder_DRAW;

        if ((fh = FindFirstFileA(AfxGetUriData(&pathBuf.uri, 0), &(wfd))))
        {
            do
            {
                afxManifest ini;
                AfxSetUpIni(&ini);
                afxUri manifestUri, manifestFile;
                AfxMakeUri(&manifestUri, 0, wfd.cFileName, 0);
                AfxClipUriFile(&manifestFile, &manifestUri);
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
                                afxClass* cls = (afxClass*)AvxGetDrawDeviceClass();
                                AfxAssertClass(cls, afxFcc_DDEV);

                                afxDrawDeviceInfo info = { 0 };
                                //info.dev.manifest = &ini;
                                info.dev.order = devType;
                                info.dev.manifestUri = manifestFile;
                                info.dev.manifestPagNo = devPagIdx;

                                afxDrawDevice ddev;

                                if (AfxAcquireObjects(cls, 1, (afxObject*)&ddev, (void const*[]) { sys, &info })) AfxThrowError();
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
    return err;
}

_AVX afxError AfxSystemIoctl(afxSystem sys, afxModule mdle, afxNat reqCode, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case 3:
    {
        AfxAssert(sys->avx.dsys == NIL);

        afxClassConfig clsCfg;

        clsCfg = _AvxDdevClsCfg;
        AfxRegisterClass(&TheDrawSystem.ddevCls, AfxGetDeviceClass(), &sys->classes, &clsCfg); // require base*

        clsCfg = _AvxDoutClsCfg;
        AfxRegisterClass(&TheDrawSystem.doutCls, NIL, &sys->classes, &clsCfg); // require ddev, dout

        clsCfg = _AvxDinClsCfg;
        AfxRegisterClass(&TheDrawSystem.dinCls, NIL, &sys->classes, &clsCfg); // require ddev, din

        if (!err)
        {
            sys->avx.dsys = &TheDrawSystem;
            TheDrawSystem.ready = TRUE;
            _AvxScanDdevs(sys, udd[0]);
        }

        break;
    }
    case 4:
    {
        AfxAssert(sys->avx.dsys == &TheDrawSystem);
        TheDrawSystem.ready = FALSE;

        AfxDeregisterClass(&TheDrawSystem.dinCls);
        AfxDeregisterClass(&TheDrawSystem.doutCls);
        AfxDeregisterClass(&TheDrawSystem.ddevCls);

        sys->avx.dsys = NIL;

        break;
    }
    default: break;
    }
    return err;
}
