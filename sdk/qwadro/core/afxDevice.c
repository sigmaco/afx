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
#define _AFX_DEVICE_C
#include "qwadro/core/afxSystem.h"

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

_AFX afxError AfxDoDeviceService(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);

    afxThread thr;
    if (AfxGetThread(&thr))
    {
        err = dev->proc(dev, thr);
    }
    return err;
}

_AFX afxResult AfxCallDevice(afxDevice dev, afxNat reqCode, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    AfxAssert(dev->ioctl);
    va_list va;
    va_start(va, reqCode);
    afxResult rslt = dev->ioctl(dev, reqCode, va);
    va_end(va);
    return rslt;
}

_AFX afxUri const* AfxGetDeviceManifestUri(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    return &dev->uri.uri;
}

_AFX afxManifest const* AfxGetDeviceManifest(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
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

_AFX void AfxGetDeviceDriverVersion(afxDevice dev, afxNat* major, afxNat* minor, afxNat* patch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    AfxAssert3(major, minor, patch);
    afxString s;

    if ((!AfxIniGetString(AfxGetDeviceManifest(dev), &AfxString("Device"), &AfxString("Version"), &s)) || (3 != AfxScanString(&s, "%u.%u.%u", major, minor, patch)))
        *major = 0, *minor = 0, *patch = 0;
}

_AFX void AfxGetDeviceDriverInfo(afxDevice dev, afxString* name, afxString* desc, afxString* vendor, afxString* website)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    AfxAssert4(name, desc, vendor, website);

    afxManifest const* manifest = AfxGetDeviceManifest(dev);

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

    AfxCleanUpIni(&dev->manifest);

    if (dev->mdle)
        AfxReleaseObjects(1, (void*[]) { dev->mdle });

    return err;
}

_AFX afxError _AfxDevCtor(afxDevice dev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);

    afxDeviceInfo const* info = (afxDeviceInfo const*)cookie->udd[1] + cookie->no;
    AfxAssert(info);

    dev->type = info->type;
    AfxMakeUri32(&dev->uri, &info->uri);
    dev->manifest = *info->manifest;

    afxString s, product, description, vendor, website;
    afxNat verMajor, verMinor, verPatch;

    if (!AfxIniGetString(&dev->manifest, &AfxString("Device"), &AfxString("Product"), &product))
        AfxResetString(&product);

    if (!AfxIniGetString(&dev->manifest, &AfxString("Device"), &AfxString("Description"), &description))
        AfxResetString(&description);

    if (!AfxIniGetString(&dev->manifest, &AfxString("Device"), &AfxString("Vendor"), &vendor))
        AfxResetString(&vendor);

    if (!AfxIniGetString(&dev->manifest, &AfxString("Device"), &AfxString("Website"), &website))
        AfxResetString(&website);

    if ((!AfxIniGetString(&dev->manifest, &AfxString("Device"), &AfxString("Version"), &s)) || (3 != AfxScanString(&s, "%u.%u.%u", &verMajor, &verMinor, &verPatch)))
        verMajor = 0, verMinor = 0, verPatch = 0;

    AfxLogY("Installable client driver <%.*s> discovered.\n\tProduct: %.*s\n\tVersion: %u.%u.%u\n\tDescription: %.*s\n\tVendor: %.*s\n\tWebsite: %.*s\n\t", AfxPushString(AfxGetUriString(&dev->uri.uri)), AfxPushString(&product), verMajor, verMinor, verPatch, AfxPushString(&description), AfxPushString(&vendor), AfxPushString(&website));

    afxUri uri2;

    if (AfxIniGetUri(&dev->manifest, &AfxString("Device"), &AfxString("DllFile"), &uri2))
    {
        AfxLogY("Loading ICD <%.*s> executable and linkable file <%.*s>...", AfxPushString(AfxGetUriString(&dev->uri.uri)), AfxPushString(AfxGetUriString(&uri2)));

        if (AfxLoadModule(&uri2, NIL, &dev->mdle)) AfxThrowError();
        else
        {
            afxError(*IcdHookPoint)(afxDevice dev) = AfxFindModuleSymbol(dev->mdle, "AfxIcdHookPoint");

            if (!IcdHookPoint || IcdHookPoint(dev)) AfxThrowError();
            else
            {

            }

            if (err && dev->mdle)
                AfxReleaseObjects(1, (void*[]) { dev->mdle });
        }
    }

    return err;
}

_AFX afxClassConfig const _AfxDevMgrCfg =
{
    .fcc = afxFcc_DEV,
    .name = "Device",
    .desc = "Device Driver Interface",
    .unitsPerPage = 2,
    .ctor = (void*)_AfxDevCtor,
    .dtor = (void*)_AfxDevDtor,
    .size = sizeof(AFX_OBJECT(afxDevice)),    
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
        afxManager* cls = AfxGetDeviceClass();
        AfxAssertClass(cls, afxFcc_DEV);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
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
    afxManager* cls = AfxGetDeviceClass();
    AfxAssertClass(cls, afxFcc_DEV);

    if (!type)
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
    else
    {
        afxDevice dev;
        afxNat i = first;
        while (AfxEnumerateObjects(cls, i, cnt, (afxObject*)&dev))
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
        afxManager* cls = AfxGetDeviceClass();
        AfxAssertClass(cls, afxFcc_DEV);
        rslt = AfxCountObjects(cls);
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
    AfxPickUriFile(manifest, &target);

    afxNat i = 0;
    afxDevice dev;
    while (AfxEnumerateDevices(type, i, 1, &dev))
    {
        AfxAssertObjects(1, &dev, afxFcc_DEV);
        AfxAssert(!type || (dev->type == type));

        afxUri tmp;
        AfxPickUriFile(&dev->uri.uri, &tmp);
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
