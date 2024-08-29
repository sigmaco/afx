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
#define _AFX_MODULE_C
#define _AFX_DRIVER_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#include "../dev/afxDevCoreBase.h"

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

_AFX afxDriver AfxGetDeviceDriver(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    afxDriver icd = AfxGetLinker(&dev->icd);
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    return icd;
}

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
        if (dev->procCb)
            err = dev->procCb(dev, thr, NIL);
    }
    return err;
}

_AFX afxResult AfxCallDevice(afxDevice dev, afxNat reqCode, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    afxResult rslt = NIL;

    if (!dev->ioctlCb) AfxThrowError();
    else
    {
        va_list va;
        va_start(va, reqCode);
        afxResult rslt = dev->ioctlCb(dev, reqCode, va);
        va_end(va);
    }
    return rslt;
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

_AFX afxAcceleration AfxGetDeviceAcceleration(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    return dev->acceleration;
}

_AFX afxError _AfxDevDtorCb(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);

    AfxCleanUpChainedClasses(&dev->classes);
    AfxAssert(AfxChainIsEmpty(&dev->classes));
    
    AfxPopLinkage(&dev->icd);

    return err;
}

_AFX afxError _AfxDevCtorCb(afxDevice dev, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);

    afxDriver icd = args[0];
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    afxDeviceInfo const* info = (afxDeviceInfo const*)(args[1]) + invokeNo;
    AfxAssert(info);

    AfxDeployChain(&dev->classes, dev);

    dev->type = info->type;
    dev->status = afxDeviceStatus_DISABLED;
    dev->acceleration = info->acceleration;

    AfxPushLinkage(&dev->icd, &icd->devices);

    afxUri urn;
    AfxWrapUriString(&urn, &info->urn);
    AfxMakeUri8(&dev->urn, &urn);

    if (AfxUriIsBlank(&dev->urn.uri)) AfxThrowError();
    else
    {
        afxManifest const* ini = AfxGetDriverManifest(icd);

        afxNat pagIdx = info->manifestPagNo;

        //if (!AfxFindManifestPage(ini, &info->manifestPag, &pagIdx)) AfxThrowError();
        //else
        {

            afxString s;

            if (!AfxFindManifestPage(ini, &AfxString("Qwadro.Icd"), &pagIdx)) AfxThrowError();
            else
            {
                afxString128 s128;
                AfxMakeString128(&s128, NIL);
                AfxFormatString(&s128.str, "Device.%.*s", AfxPushString(&dev->urn.uri.str));

                if (!AfxFindManifestPage(ini, &s128.str, &pagIdx))
                    AfxThrowError();

                dev->manifestPagNo = pagIdx;

                afxNat recIdx;

                if (!err)
                {
                    afxString devDesc = { 0 };
#if !0
                    if (!AfxFindManifestRecord(ini, dev->manifestPagNo, &AfxString("Description"), &recIdx) ||
                        !AfxGetManifestString(ini, dev->manifestPagNo, recIdx, &devDesc))
                    {
                        AfxThrowError();
                        AfxResetString(&devDesc);
                    }
#endif

                    if (!err)
                    {
                        dev->procCb = info->procCb;
                        dev->ioctlCb = info->ioctl;
                        
                        if (!dev->ioctlCb)
                            dev->ioctlCb = AfxFindModuleSymbol(&icd->mdle, "AfxDeviceIoctl");

                        //if (!dev->ioctlCb) AfxThrowError();
                        //else
                        {

                        }
                    }
                }
            }
        }
    }

    if (err)
    {
        AfxPopLinkage(&dev->icd);
    }

    return err;
}

_AFX afxClassConfig const _AfxDevBaseImplementation =
{
    .fcc = afxFcc_DEV,
    .name = "Device",
    .desc = "Device Driver Interface",
    .ctor = (void*)_AfxDevCtorCb,
    .dtor = (void*)_AfxDevDtorCb,
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
        afxClass const* cls = AfxGetDeviceClass();
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
    afxClass const* cls = AfxGetDeviceClass();
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
        afxClass const* cls = AfxGetDeviceClass();
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

_AFX afxBool AfxFindDevice(afxDeviceType type, afxUri const* urn, afxDevice* device)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert2(urn, device);
    afxResult rslt = FALSE;
    
    afxUri target;
    AfxClipUriFile(&target, urn);

    afxNat i = 0;
    afxDevice dev;
    while (AfxEnumerateDevices(type, i, 1, &dev))
    {
        AfxAssertObjects(1, &dev, afxFcc_DEV);
        AfxAssert(!type || (dev->type == type));

        afxUri tmp;
        AfxClipUriFile(&tmp, &dev->urn.uri);

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
