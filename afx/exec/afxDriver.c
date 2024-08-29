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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

#include <sys/stat.h>
#include <stdio.h>

#if (defined(_WIN32) || defined(_WIN64))
#   include <Windows.h>
#   include <Shlwapi.h>
#   include <combaseapi.h>
#else
#   include <unistd.h>
#endif

#define _AFX_CORE_C
#define _AFX_MODULE_C
#define _AFX_DRIVER_C
#define _AFX_DEVICE_C
#include "../dev/afxDevCoreBase.h"

_AFX void AfxGetDriverUri(afxDriver icd, afxUri* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    AfxAssert(uri);
    afxUri name;
    AfxClipUriTarget(&name, &icd->mdle.path.uri);
    AfxCopyUri(uri, &name);
}

_AFX afxManifest const* AfxGetDriverManifest(afxDriver icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    return &icd->etc;
}

_AFX void AfxGetDriverVersion(afxDriver icd, afxNat* verMajor, afxNat* verMinor, afxNat* verPatch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    AfxAssert3(verMajor, verMinor, verPatch);

    afxString s;
    afxManifest const* ini = AfxGetDriverManifest(icd);
    afxNat pagIdx, recIdx;

    if (!AfxFindManifestPage(ini, &AfxString("Qwadro.Icd"), &pagIdx)) AfxThrowError();
    else
    {
        if (!AfxFindManifestRecord(ini, pagIdx, &AfxString("DriverVer"), &recIdx) || !AfxGetManifestString(ini, pagIdx, recIdx, &s) ||
            (3 != AfxScanString(&s, "%u.%u.%u", verMajor, verMinor, verPatch)))
        {
            AfxThrowError();
        }
    }

    if (err)
    {
        verMajor = 0, verMinor = 0, verPatch = 0;
    }
}

_AFX void AfxGetDriverInfo(afxDriver icd, afxString* name, afxString* vendor, afxString* website)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    AfxAssert3(name, vendor, website);

    afxString s;
    afxManifest const* ini = AfxGetDriverManifest(icd);
    afxNat pagIdx, recIdx;

    if (!AfxFindManifestPage(ini, &AfxString("Qwadro.Icd"), &pagIdx)) AfxThrowError();
    else
    {
        if (!AfxFindManifestRecord(ini, pagIdx, &AfxString("Vendor"), &recIdx) ||
            !AfxGetManifestString(ini, pagIdx, recIdx, vendor))
        {
            AfxThrowError();
            AfxResetString(vendor);
        }

        if (!AfxFindManifestRecord(ini, pagIdx, &AfxString("Href"), &recIdx) ||
            !AfxGetManifestString(ini, pagIdx, recIdx, website))
        {
            AfxThrowError();
            AfxResetString(website);
        }

        afxUri name2;
        AfxClipUriTarget(&name2, &icd->mdle.path.uri);
        *name = name2.str;
    }
}

_AFX afxError _AfxIcdCtorCb(afxDriver icd, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);

    //afxUri const* uri = ((afxUri const*)args[1]) + invokeNo;
    afxUri name;
    AfxClipUriTarget(&name, &icd->mdle.path.uri);

    afxUri32 urib;
    AfxMakeUri32(&urib, &name);
    AfxFormatUri(&urib.uri, "%.*s.inf", AfxPushString(AfxGetUriString(&name)));

    afxManifest* ini = &icd->etc;
    AfxSetUpIni(ini);

    if (AfxIniLoadFromFile(ini, &urib.uri)) AfxThrowError();
    else
    {
        afxNat pagIdx;

        if (!AfxFindManifestPage(ini, &AfxString("Qwadro.Icd"), &pagIdx)) AfxThrowError();
        else
        {
            afxError(*icdHookFn)(afxDriver, afxUri const*) = AfxFindModuleSymbol(&icd->mdle, "afxIcdHook");

            if (!icdHookFn) AfxThrowError();
            else
            {
                AfxDeployChain(&icd->devices, icd);

                icd->icdHookFn = icdHookFn;
                icd->mdle.type = afxModuleType_ICD;

                if (icdHookFn(icd, &name)) AfxThrowError();
                else
                {


                }
            }
        }

        if (err)
        {
            AfxCleanUpIni(ini);
        }
    }

    return err;
}

_AFX afxError _AfxIcdDtorCb(afxDriver icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);

    // objects will be released at class drop.

    afxDevice dev;
    AfxChainForEveryLinkage(&icd->devices, AFX_OBJ(afxDevice), icd, dev)
    {
        AfxAssertObjects(1, &dev, afxFcc_DEV);
        while (AfxReleaseObjects(1, &dev));
    };
    AfxAssert(AfxChainIsEmpty(&icd->devices));

    AfxCleanUpIni(&icd->etc);

    return err;
}

_AFX afxClassConfig const _AfxIcdClsCfg =
{
    .fcc = afxFcc_ICD,
    .name = "Driver",
    .desc = "Installable Client Driver",
    .fixedSiz = sizeof(AFX_OBJECT(afxDriver)),
    .ctor = (void*)_AfxIcdCtorCb,
    .dtor = (void*)_AfxIcdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxInvokeDrivers(afxNat first, afxNat cnt, afxBool(*f)(afxDriver, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetDriverClass();
    AfxAssertClass(cls, afxFcc_ICD);
    return cnt ? AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxNat AfxEnumerateDrivers(afxNat first, afxNat cnt, afxDriver drivers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(drivers);
    afxClass const* cls = AfxGetDriverClass();
    AfxAssertClass(cls, afxFcc_ICD);
    return cnt ? AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)drivers) : 0;
}

_AFX afxBool AfxFindDriver(afxUri const* uri, afxDriver* driver)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;
    AfxAssert(uri);

    afxNat i = 0;
    afxDriver icd;
    while (AfxEnumerateDrivers(i, 1, &icd))
    {
        AfxAssertObjects(1, &icd, afxFcc_ICD);
        afxUri exeFile;

        if (uri->fext)
        {
            AfxClipUriFile(&exeFile, &icd->mdle.path.uri);

            if (0 == AfxCompareUri(&exeFile, uri))
            {
                found = TRUE;
                break;
            }
        }
        else
        {
            AfxClipUriTarget(&exeFile, &icd->mdle.path.uri);

            if (0 == AfxCompareUri(&exeFile, uri))
            {
                found = TRUE;
                break;
            }
        }
        ++i;
    }
    AfxAssert(driver);
    *driver = found ? icd : NIL;
    return found;
}

_AFX afxError _AfxLoadDriver(afxUri const* uri, afxFlags flags, afxDriver* driver)
{
    afxError err = AFX_ERR_NONE;
    afxDriver icd = NIL;

    if (AfxFindDriver(uri, &icd))
    {
        if (AfxReacquireObjects(1, &icd))
            AfxThrowError();
    }
    else
    {
        AfxLogEcho("Loading installable client driver... <%.*s>", AfxPushString(uri ? AfxGetUriString(uri) : &AFX_STR_EMPTY));

        afxClass* cls = (afxClass*)AfxGetDriverClass();
        AfxAssertClass(cls, afxFcc_ICD);

        if (AfxAcquireObjects(cls, 1, (afxObject*)&icd, (void const*[]) { NIL, uri })) AfxThrowError();
        else
        {

            afxString s, devDesc, providerName, providerUrl;
            afxNat verMajor, verMinor, verPatch;

            AfxGetDriverVersion(icd, &verMajor, &verMinor, &verPatch);
            AfxGetDriverInfo(icd, &s, &providerName, &providerUrl);
            
            afxDevice dev;
            AfxChainForEveryLinkageB2F(&icd->devices, AFX_OBJ(afxDevice), icd, dev)
            {
                AfxAssertObjects(1, &dev, afxFcc_DEV);
                
                afxNat recIdx = 0;

                if (!AfxFindManifestRecord(&icd->etc, dev->manifestPagNo, &AfxString("Description"), &recIdx) ||
                    !AfxGetManifestString(&icd->etc, dev->manifestPagNo, recIdx, &devDesc))
                {
                    AfxThrowError();
                    AfxResetString(&devDesc);
                }

                AfxLogY("\t<//./%.*s/>(\"%.*s\")\n\t%.*s %u.%u.%u\n\t%.*s <%.*s>\n",
                    AfxPushString(&dev->urn.uri.str), AfxPushString(&s), AfxPushString(&devDesc), verMajor, verMinor, verPatch, AfxPushString(&providerName), AfxPushString(&providerUrl));
            };
        }
    }

    AfxAssert(driver);
    *driver = icd;
    return err;
}
