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

#define _AFX_CORE_C
#define _AFX_EXECUTABLE_C
#define _AFX_ICD_C
#define _AFX_DEVICE_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"

_AFX afxUri const* AfxGetIcdIdentifier(afxIcd icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    return &icd->uri.uri;
}

_AFX afxIni const* AfxGetIcdInitializer(afxIcd icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    return &icd->ini;
}

_AFX afxExecutable AfxGetIcdExecutable(afxIcd icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    return icd->exe;
}

_AFX afxChain const* AfxGetRegisteredDevices(afxIcd icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    return &icd->devices;
}

_AFX void AfxGetIcdVersion(afxIcd icd, afxNat* major, afxNat* minor, afxNat* patch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    *major = icd->verMajor;
    *minor = icd->verMinor;
    *patch = icd->verPatch;
}

_AFX afxError _AfxIcdDtorFreeDevices(afxIcd icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);

    afxDevice dev;
    AfxChainForEveryLinkage(&icd->devices, AFX_OBJECT(afxDevice), icd, dev)
    {
        AfxAssertObjects(1, &dev, afxFcc_DEV);
        while (!AfxReleaseObjects(1, (void*[]) { dev }));
    }

    if (!AfxChainIsEmpty(&icd->devices))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxIcdDtor(afxIcd icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);

    //afxMmu mmu = AfxGetDrawSystemMmu();

    _AfxIcdDtorFreeDevices(icd);
    
    AfxCleanUpIni(&icd->ini);

    AfxReleaseObjects(1, (void*[]) { icd->exe });

    //AfxReleaseObjects(1, (void*[]) { ddrv->mdle });

    return err;
}

_AFX afxError _AfxIcdCtor(afxIcd icd, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);

    afxUri const* uri = cookie->udd[1];
    afxIni const* ini = cookie->udd[2];

    AfxSetUpChain(&icd->devices, icd);

    AfxMakeUri128(&icd->uri, uri);
    icd->ini = *ini;

    afxString s;

    if (!AfxIniGetString(ini, &AfxString("Firmware"), &AfxString("Product"), &icd->product))
        AfxResetString(&icd->product);

    if (!AfxIniGetString(ini, &AfxString("Firmware"), &AfxString("Description"), &icd->description))
        AfxResetString(&icd->description);

    if (!AfxIniGetString(ini, &AfxString("Firmware"), &AfxString("Vendor"), &icd->vendor))
        AfxResetString(&icd->vendor);

    if (!AfxIniGetUri(ini, &AfxString("Firmware"), &AfxString("Website"), &icd->website))
        AfxResetUri(&icd->website);

    if ((!AfxIniGetString(ini, &AfxString("Firmware"), &AfxString("Website"), &s)) || (3 != AfxScanString(&s, "%u.%u.%u", &icd->verMajor, &icd->verMinor, &icd->verPatch)))
        icd->verMajor = 0, icd->verMinor = 0, icd->verPatch = 0;

    AfxAdvertise("Installable client driver <%.*s> discovered.\n\tProduct: %.*s\n\tVersion: %u.%u.%u\n\tDescription: %.*s\n\tVendor: %.*s\n\tWebsite: %.*s\n\t", AfxPushString(AfxGetUriString(&icd->uri.uri)), AfxPushString(&icd->product), icd->verMajor, icd->verMinor, icd->verPatch, AfxPushString(&icd->description), AfxPushString(&icd->vendor), AfxPushString(AfxGetUriString(&icd->website)));

    afxUri uri2;

    if (AfxIniGetUri(ini, &AfxString("Firmware"), &AfxString("DllFile"), &uri2))
    {
        AfxAdvertise("Loading ICD <%.*s> executable and linkable file <%.*s>...", AfxPushString(AfxGetUriString(&icd->uri.uri)), AfxPushString(AfxGetUriString(&uri2)));

        if (AfxLoadExecutables(1, &uri2, &icd->exe)) AfxThrowError();
        else
        {
            afxError(*IcdHookPoint)(afxIcd icd) = AfxFindExecutableSymbol(icd->exe, "AfxIcdHookPoint");

            if (IcdHookPoint && IcdHookPoint(icd)) AfxThrowError();
            else
            {

            }

            if (err)
                _AfxIcdDtorFreeDevices(icd);
            
            if (err)
                AfxReleaseObjects(1, (void*[]) { icd->exe });
        }
    }

    return err;
}

_AFX afxClassConfig _AfxIcdClsConfig =
{
    .fcc = afxFcc_ICD,
    .name = "Installable Client Driver",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxIcd)),
    .mmu = NIL,
    .ctor = (void*)_AfxIcdCtor,
    .dtor = (void*)_AfxIcdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxInstallClientDrivers(afxNat cnt, afxUri const manifest[], afxIcd drivers[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxIcd icd;
        afxUri file;

        if (!AfxExcerptUriFile(&manifest[i], &file)) AfxThrowError();
        else
        {
            if ((icd = AfxFindIcd(&file)))
                AfxReacquireObjects(1, (void*[]) { icd });
            else
            {
                AfxComment("Installing client driver <%.*s>...", AfxPushString(AfxGetUriString(&file)));

                afxIni ini;
                AfxSetUpIni(&ini);
                AfxIniLoadFromFile(&ini, &file);
                afxString svc;

                if (AfxIniGetString(&ini, &AfxString("Firmware"), &AfxString("Service"), &svc))
                {
                    afxClass* cls = AfxGetIcdClass();
                    AfxAssertClass(cls, afxFcc_ICD);

                    if (AfxAcquireObjects(cls, 1, (afxObject*)&icd, (void const*[]) { AfxGetSystem(), &file, &ini })) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &icd, afxFcc_ICD);
                        
                        if (err)
                            AfxReleaseObjects(1, (void*[]) { icd });
                    }

                    if (err)
                        AfxCleanUpIni(&ini);
                }
                else
                {
                    AfxCleanUpIni(&ini);
                }
            }
        }

        drivers[i] = err ? NIL : icd;
    }
    return err;
}

_AFX afxNat AfxInvokeIcds(afxNat first, afxNat cnt, afxBool(*f)(afxIcd, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetIcdClass();
    AfxAssertClass(cls, afxFcc_ICD);
    return cnt ? AfxInvokeInstances(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxNat AfxEnumerateIcds(afxNat first, afxNat cnt, afxIcd icds[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(icds);
    afxClass* cls = AfxGetIcdClass();
    AfxAssertClass(cls, afxFcc_ICD);
    return cnt ? AfxEnumerateInstances(cls, first, cnt, (afxObject*)icds) : 0;
}

_AFX afxNat AfxCountIcds(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetIcdClass();
    AfxAssertClass(cls, afxFcc_ICD);
    return AfxCountInstances(cls);
}

_AFX afxIcd AfxFindIcd(afxUri const *manifest)
{
    afxError err = AFX_ERR_NONE;

    afxUri target;
    AfxAssert(manifest);
    AfxExcerptUriFile(manifest, &target);

    afxIcd icd;
    afxNat i = 0;
    while (AfxEnumerateIcds(i, 1, &icd))
    {
        AfxAssertObjects(1, &icd, afxFcc_ICD);

        afxUri tmp;
        AfxExcerptUriFile(&icd->uri.uri, &tmp);

        if (0 == AfxCompareUri(&target, &tmp))
            return icd;

        ++i;
    }
    return NIL;
}
