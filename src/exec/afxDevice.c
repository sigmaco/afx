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
#include "../dev/afxExecImplKit.h"

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

_AFX afxModule AfxGetDeviceDriver(afxDevice dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    afxModule icd = AfxGetLinker(&dev->icd);
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
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

_AFX afxResult AfxCallDevice(afxDevice dev, afxUnit reqCode, ...)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);
    afxResult rslt = NIL;

    if (dev->ioctlCb)
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

    AfxDeregisterChainedClasses(&dev->classes);
    AFX_ASSERT(AfxChainIsEmpty(&dev->classes));
    
    AfxPopLinkage(&dev->icd);

    return err;
}

_AFX afxError _AfxDevCtorCb(afxDevice dev, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dev, afxFcc_DEV);

    afxModule icd = args[0];
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    afxDeviceInfo const* info = (afxDeviceInfo const*)(args[1]) + invokeNo;
    AFX_ASSERT(info);

    AfxDeployChain(&dev->classes, dev);

    dev->type = info->type;
    dev->status = afxDeviceStatus_DISABLED;
    dev->acceleration = info->acceleration;

    AfxPushLinkage(&dev->icd, &icd->devices);

    afxUri urn;
    AfxWrapUriString(&urn, &info->urn);
    AfxMakeUri8(&dev->urn, &urn);

    if (AfxIsUriBlank(&dev->urn.uri)) AfxThrowError();
    else
    {
        //afxManifest const* ini = AfxGetModuleManifest(icd);

        afxUnit pagIdx = info->manifestPagNo;

        //if (!AfxFindInitializationSection(ini, &info->manifestPag, &pagIdx)) AfxThrowError();
        //else
        {

            afxString s;

            //if (!AfxFindInitializationSection(ini, &AfxString("Qwadro.Icd"), &pagIdx)) AfxThrowError();
            //else
            {
                afxString128 s128;
                AfxMakeString128(&s128, NIL);
                AfxFormatString(&s128.str, "Device.%.*s", AfxPushString(&dev->urn.uri.str));

                //if (!AfxFindInitializationSection(ini, &s128.str, &pagIdx))
                //    AfxThrowError();

                dev->manifestPagNo = pagIdx;

                afxUnit recIdx;

                if (!err)
                {
                    afxString devDesc = { 0 };
#if !0
                    //if (!AfxFindInitializationRecord(ini, dev->manifestPagNo, &AfxString("Description"), &recIdx) ||
                    //    !AfxGetManifestString(ini, dev->manifestPagNo, recIdx, &devDesc))
                    {
                    //    AfxThrowError();
                        AfxResetStrings(1, &devDesc);
                    }
#endif

                    if (!err)
                    {
                        dev->procCb = info->procCb;
                        dev->ioctlCb = info->ioctl;
                        
                        if (!dev->ioctlCb)
                            dev->ioctlCb = AfxFindModuleSymbol(icd, "AfxDeviceIoctl");

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

_AFX afxUnit AfxInvokeDevices(afxDeviceType type, afxUnit first, afxUnit cnt, afxBool(*f)(afxDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
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
        afxUnit i = first;
        while (AfxEnumerateDevices(type, i, 1, &dev))
        {
            AfxAssertObjects(1, &dev, afxFcc_DEV);
            AFX_ASSERT(dev->type == type);

            ++rslt;

            if (f(dev, udd))
                break;
            else if (++i == cnt)
                break;
        }
    }
    return rslt;
}

_AFX afxUnit AfxEnumerateDevices(afxDeviceType type, afxUnit first, afxUnit cnt, afxDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(devices);
    afxResult rslt = 0;
    afxClass const* cls = AfxGetDeviceClass();
    AfxAssertClass(cls, afxFcc_DEV);

    if (!type)
        rslt = AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)devices);
    else
    {
        afxDevice dev;
        afxUnit i = first;
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

_AFX afxUnit AfxCountDevices(afxDeviceType type)
{
    afxError err = AFX_ERR_NONE;
    afxUnit rslt = 0;

    if (!type)
    {
        afxClass const* cls = AfxGetDeviceClass();
        AfxAssertClass(cls, afxFcc_DEV);
        rslt = AfxCountClassInstances(cls);
    }
    else
    {
        afxUnit i = 0;
        afxDevice dev;
        while (AfxEnumerateDevices(type, i, 1, &dev))
        {
            AfxAssertObjects(1, &dev, afxFcc_DEV);
            AFX_ASSERT(dev->type == type);
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

    afxUnit i = 0;
    afxDevice dev;
    while (AfxEnumerateDevices(type, i, 1, &dev))
    {
        AfxAssertObjects(1, &dev, afxFcc_DEV);
        AFX_ASSERT(!type || (dev->type == type));

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