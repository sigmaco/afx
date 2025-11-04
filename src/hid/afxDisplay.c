/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.
// This software is part of Advanced User Experiences Extensions & Experiments.

#define _AVX_DRAW_C
#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DISPLAY_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_SURFACE_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "../draw/ddi/avxImplementation.h"
#define _AUX_UX_C
#include "../ux/impl/auxImplementation.h"

_AVX afxReal64 AvxFindPhysicalAspectRatio(afxUnit screenWidth, afxUnit screenHeight)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(screenWidth);
    AFX_ASSERT(screenHeight);
    afxReal64 ratio, div = (afxReal64)screenWidth / (afxReal64)screenHeight;

    if (div <= 1.4)
        ratio = 1.33;
    else if (div <= 1.6)
        ratio = 1.5599999;
    else
        ratio = 1.78;

    return ratio;
}

_AVX afxError _AvxVduDtorCb(afxDisplay vdu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

    _AFX_DEV_CLASS_CONFIG.dtor(&vdu->dev);

    return err;
}

_AVX afxError _AvxVduCtorCb(afxDisplay vdu, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

    afxModule icd = AFX_CAST(afxModule, args[0]);
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    avxDisplayInfo const* info = AFX_CAST(avxDisplayInfo const*, args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AFX_DEV_CLASS_CONFIG.ctor(&vdu->dev, (void*[]) { icd, (void*)&info->dev }, 0))
    {
        AfxThrowError();
        return err;
    }

    vdu->dimWh[0] = info->dimWh[0];
    vdu->dimWh[1] = info->dimWh[1];
    vdu->resWh[0] = info->resWh[0];
    vdu->resWh[1] = info->resWh[1];
    vdu->dpi[0] = info->dpi[0];
    vdu->dpi[1] = info->dpi[1];
    vdu->persistentContent = info->persistentContent;
    vdu->planeReorder = info->planeReorder;
    vdu->supportedXforms = info->supportedXforms;
    AfxStrcpy(vdu->name, info->name);
    AfxStrcpy(vdu->label, info->label);

    vdu->portCnt = 0;
    vdu->ports[0] = (_avxDisplayPort) { 0 };
    vdu->ports[1] = (_avxDisplayPort) { 0 };

    return err;
}

_AVX afxClassConfig const _AVX_VDU_CLASS_CONFIG =
{
    .fcc = afxFcc_VDU,
    .name = "Display",
    .desc = "Video Display Unit",
    .fixedSiz = sizeof(AFX_OBJECT(afxDisplay)),
    .ctor = (void*)_AvxVduCtorCb,
    .dtor = (void*)_AvxVduDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AvxRegisterDisplays(afxModule icd, afxUnit cnt, avxDisplayInfo const infos[], afxDisplay displays[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(displays);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);
    
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX));

    afxClass* cls = (afxClass*)_AvxGetDisplayClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)displays, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_VDU, cnt, displays);

    }
    return err;
}

_AVX afxUnit AfxEnumerateDisplays(afxModule icd, afxUnit first, afxUnit cnt, afxDisplay displays[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(displays);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;
    afxClass const* cls = _AvxGetDisplayClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);
    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)displays);
    // @displays must be an array of valid afxDisplay handles.
    AFX_ASSERT_OBJECTS(afxFcc_VDU, rslt, displays);
    return rslt;
}

_AVX afxUnit AfxEnumerateDisplays2(afxUnit dIcd, afxUnit first, afxUnit cnt, afxDisplay displays[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(displays);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule drv;
    _AvxGetIcd(dIcd, &drv);
#if 0
    afxClass const* cls = _AvxGetDisplayClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);
    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)displays);
    // @displays must be an array of valid afxDisplay handles.
    AFX_ASSERT_OBJECTS(afxFcc_VDU, rslt, displays);
#endif
    return rslt;
}
