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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#include "../src/afx/dev/afxDevCoreBase.h"

afxString devTypeString[] =
{
    AFX_STRING(""),
    AFX_STRING("HID"),
    AFX_STRING("DRAW"),
    AFX_STRING("SOUND"),
    AFX_STRING("TELECOM"),
    AFX_STRING("SIM"),
};

afxString devAccelString[] =
{
    AFX_STRING(""),
    AFX_STRING("CPU"),
    AFX_STRING("GPU"),
    AFX_STRING("APU"),
    AFX_STRING("IGPU"),
    AFX_STRING("VGPU"),
    AFX_STRING("PPU"),
    AFX_STRING("SPU"),
    AFX_STRING("DSP")
};

_AFX afxClass* AfxGetFenceClass(afxContext ctx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObjects(1, &ddev, afxFcc_CTX);
    afxClass *cls = &ctx->fences;
    AfxAssertClass(cls, afxFcc_FENC);
    return cls;
}

_AFX afxClass* AfxGetSemaphoreClass(afxContext ctx)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObjects(1, &ddev, afxFcc_DEV);
    afxClass *cls = &ctx->semaphores;
    AfxAssertClass(cls, afxFcc_SEM);
    return cls;
}

_AFX afxError AfxDoDeviceService(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    afxThread thr;

    if (!AfxGetThread(&thr)) AfxThrowError();
    else
    {
        err = dev->procCb(dev, thr);
    }
    return err;
}

_AFX afxResult AfxCallDevice(afxDevice dev, afxNat reqCode, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    AfxAssert(dev->ioctlCb);
    va_list va;
    va_start(va, reqCode);
    afxResult rslt = dev->ioctlCb(dev, reqCode, va);
    va_end(va);
    return rslt;
}

_AFX afxUri const* AfxGetDeviceManifestUri(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    return &dev->manifestUri.uri;
}

_AFX afxManifest const* AfxGetDeviceManifest(afxDevice dev, afxNat* devPagIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    AfxAssert(devPagIdx);
    *devPagIdx = dev->manifestPagNo;
    return &dev->manifest;
}

_AFX afxModule AfxGetDeviceModule(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    afxModule mdle = dev->mdle;
    AfxTryAssertObjects(1, &mdle, afxFcc_MDLE);
    return mdle;
}

_AFX afxDeviceType AfxGetDeviceType(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    return dev->type;
}

_AFX afxDeviceStatus AfxGetDeviceStatus(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    return dev->status;
}

_AFX afxDeviceAccelor AfxGetDeviceAcceleration(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    return dev->accelor;
}

_AFX void AfxGetDeviceDriverVersion(afxDevice dev, afxNat* major, afxNat* minor, afxNat* patch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    AfxAssert3(major, minor, patch);
    afxString s;
    afxNat devPagIdx;

    if ((!AfxIniGetString(AfxGetDeviceManifest(dev, &devPagIdx), &AfxString("Device"), &AfxString("Version"), &s)) || 
        (3 != AfxScanString(&s, "%u.%u.%u", major, minor, patch)))
        *major = 0, *minor = 0, *patch = 0;
}

_AFX void AfxGetDeviceDriverInfo(afxDevice dev, afxString* name, afxString* desc, afxString* vendor, afxString* website)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    AfxAssert4(name, desc, vendor, website);

    afxNat devPagIdx;
    afxManifest const* manifest = AfxGetDeviceManifest(dev, &devPagIdx);

    if (name)
        if (!AfxIniGetString(manifest, &AfxString("Device"), &AfxString("Name"), name))
            AfxResetString(name);

    if (desc)
        if (!AfxIniGetString(manifest, &AfxString("Device"), &AfxString("Description"), desc))
            AfxResetString(desc);

    if (vendor)
        if (!AfxIniGetString(manifest, &AfxString("Device"), &AfxString("Vendor"), vendor))
            AfxResetString(vendor);

    if (website)
        if (!AfxIniGetString(manifest, &AfxString("Device"), &AfxString("Website"), website))
            AfxResetString(website);
}

_AFX afxError _AfxDevDtor(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);

    AfxAssert(AfxChainIsEmpty(&dev->classes));

    AfxCleanUpIni(&dev->manifest);

    if (dev->mdle)
        AfxReleaseObjects(1, &dev->mdle);

    return err;
}

_AFX afxError _AfxDevCtor(afxDevice dev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);

    afxDeviceInfo const* info = (afxDeviceInfo const*)cookie->udd[1] + cookie->no;
    AfxAssert(info);

    AfxSetUpChain(&dev->classes, dev);

    dev->type = info->type;
    AfxMakeUri32(&dev->manifestUri, &info->manifestUri);
    
    if (AfxUriIsBlank(&dev->manifestUri.uri)) AfxThrowError();
    else
    {
        afxManifest* ini = &dev->manifest;

        AfxSetUpIni(ini);
        AfxIniLoadFromFile(ini, &dev->manifestUri.uri);

        afxNat pagIdx = info->manifestPagNo;

        //if (!AfxFindManifestPage(ini, &info->manifestPag, &pagIdx)) AfxThrowError();
        //else
        {
            dev->manifestPagNo = pagIdx;

            afxString s, drvDesc, providerName, providerUrl;
            afxNat verMajor, verMinor, verPatch;

            if (!AfxFindManifestPage(ini, &AfxString("Qwadro.Icd"), &pagIdx)) AfxThrowError();
            else
            {
                afxNat recIdx;

                if (!AfxFindManifestRecord(ini, pagIdx, &AfxString("ProviderName"), &recIdx) ||
                    !AfxGetManifestString(ini, pagIdx, recIdx, &providerName))
                {
                    AfxThrowError();
                    AfxResetString(&providerName);
                }

                if (!AfxFindManifestRecord(ini, pagIdx, &AfxString("ProviderUrl"), &recIdx) ||
                    !AfxGetManifestString(ini, pagIdx, recIdx, &providerUrl))
                {
                    AfxThrowError();
                    AfxResetString(&providerUrl);
                }

                if (!AfxFindManifestRecord(ini, pagIdx, &AfxString("DriverDesc"), &recIdx) ||
                    !AfxGetManifestString(ini, pagIdx, recIdx, &drvDesc))
                {
                    AfxThrowError();
                    AfxResetString(&drvDesc);
                }

                if (!AfxFindManifestRecord(ini, pagIdx, &AfxString("DriverVersion"), &recIdx) ||
                    !AfxGetManifestString(ini, pagIdx, recIdx, &s) ||
                    (3 != AfxScanString(&s, "%u.%u.%u", &verMajor, &verMinor, &verPatch)))
                {
                    AfxThrowError();
                    verMajor = 0, verMinor = 0, verPatch = 0;
                }

                if (!err)
                {
                    afxString devDesc;

                    if (!AfxFindManifestRecord(ini, dev->manifestPagNo, &AfxString("Description"), &recIdx) ||
                        !AfxGetManifestString(ini, dev->manifestPagNo, recIdx, &devDesc))
                    {
                        AfxThrowError();
                        AfxResetString(&devDesc);
                    }

                    if (!err)
                    {
                        afxUri uri2;

                        if (!AfxFindManifestRecord(ini, pagIdx, &AfxString("DllFile"), &recIdx) ||
                            !AfxGetManifestUri(ini, pagIdx, recIdx, &uri2))
                        {
                            AfxThrowError();
                        }

                        if (AfxLoadModule(&uri2, NIL, &dev->mdle)) AfxThrowError();
                        else
                        {
                            AfxLogY("Installable client driver discovered.\n\tDevice: %.*s\n\tDriver: %.*s\n\tVersion: %u.%u.%u\n\tProvider: %.*s <%.*s>\n", AfxPushString(&devDesc), AfxPushString(&drvDesc), verMajor, verMinor, verPatch, AfxPushString(&providerName), AfxPushString(&providerUrl));

                            dev->ioctlCb = AfxFindModuleSymbol(dev->mdle, "AfxDeviceIoctl");

                            if (!dev->ioctlCb) AfxThrowError();
                            else
                            {

                            }

                            if (err && dev->mdle)
                                AfxReleaseObjects(1, &dev->mdle);
                        }
                    }
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

_AFX afxClassConfig const _AfxDevMgrCfg =
{
    .fcc = afxFcc_DEV,
    .name = "Device",
    .desc = "Device Driver Interface",
    .ctor = (void*)_AfxDevCtor,
    .dtor = (void*)_AfxDevDtor,
    .fixedSiz = sizeof(AFX_OBJECT(afxDevice)),    
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxInvokeDevices(afxDeviceType type, afxNat first, afxNat cnt, afxBool(*f)(afxDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxResult rslt = 0;

    if (!type)
    {
        afxClass* cls = AfxGetDeviceClass();
        AfxAssertClass(cls, afxFcc_DEV);
        rslt = AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
    }
    else
    {
        afxDevice dev;
        afxNat i = first;
        while (AfxEnumerateDevices(type, i, 1, &dev))
        {
            AfxAssertObjects(1, &dev, afxFcc_DEV);
            AfxAssert(dev->type == type);

            ++rslt;

            if (f(dev, udd))
                break;
            else if (++i == cnt)
                break;
        }
    }
    return rslt;
}

_AFX afxNat AfxEnumerateDevices(afxDeviceType type, afxNat first, afxNat cnt, afxDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxResult rslt = 0;
    afxClass* cls = AfxGetDeviceClass();
    AfxAssertClass(cls, afxFcc_DEV);

    if (!type)
        rslt = AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)devices);
    else
    {
        afxDevice dev;
        afxNat i = first;
        while (AfxEnumerateClassInstances(cls, i, cnt, (afxObject*)&dev))
        {
            AfxAssertObjects(1, &dev, afxFcc_DEV);

            if (dev->type == type)
            {
                devices[rslt] = dev;
                ++rslt;
            }

            if (++i == cnt)
                break;
        }
    }
    return rslt;
}

_AFX afxNat AfxCountDevices(afxDeviceType type)
{
    afxError err = AFX_ERR_NONE;
    afxNat rslt = 0;

    if (!type)
    {
        afxClass* cls = AfxGetDeviceClass();
        AfxAssertClass(cls, afxFcc_DEV);
        rslt = AfxCountClassInstances(cls);
    }
    else
    {
        afxNat i = 0;
        afxDevice dev;
        while (AfxEnumerateDevices(type, i, 1, &dev))
        {
            AfxAssertObjects(1, &dev, afxFcc_DEV);
            AfxAssert(dev->type == type);
            ++rslt;
            ++i;
        }
    }
    return rslt;
}

_AFX afxBool AfxFindDevice(afxDeviceType type, afxUri const* manifest, afxDevice* device)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert2(manifest, device);
    afxResult rslt = FALSE;

    afxUri target;
    AfxClipUriFile(&target, manifest);

    afxNat i = 0;
    afxDevice dev;
    while (AfxEnumerateDevices(type, i, 1, &dev))
    {
        AfxAssertObjects(1, &dev, afxFcc_DEV);
        AfxAssert(!type || (dev->type == type));

        afxUri tmp;
        AfxClipUriFile(&tmp, &dev->manifestUri.uri);

        if (0 == AfxCompareUri(&target, &tmp))
        {
            *device = dev;
            rslt = TRUE;
            break;
        }
        ++i;
    }
    return rslt;
}
