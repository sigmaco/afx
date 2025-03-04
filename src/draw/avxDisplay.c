/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AFX_DEVICE_C
//#define _AVX_DRAW_DEVICE_C
#define _AVX_DISPLAY_C
//#define _AVX_DRAW_QUEUE_C
//#define _AVX_DRAW_OUTPUT_C
//#define _AVX_DRAW_CONTEXT_C
//#define _AVX_BUFFER_C
#include "impl/avxImplementation.h"

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

    if (_AFX_DEV_CLASS_CONFIG.ctor(&vdu->dev, (void*[]) { icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        vdu->dimWh[0] = info->dimWh[0];
        vdu->dimWh[1] = info->dimWh[1];
        vdu->resWh[0] = info->resWh[0];
        vdu->resWh[1] = info->resWh[1];
        vdu->persistentContent = info->persistentContent;
        vdu->planeReorder = info->planeReorder;
        vdu->supportedXforms = info->supportedXforms;
        AfxStrcpy(vdu->name, info->name);
        AfxStrcpy(vdu->label, info->label);
    }

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

_AVX afxUnit AfxEnumerateDisplays(afxDrawDevice ddev, afxUnit first, afxUnit cnt, afxDisplay displays[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    AFX_ASSERT(displays);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;
    afxClass const* cls = _AvxGetDisplayClass(ddev);
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);
    rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)displays);
    // @displays must be an array of valid afxDisplay handles.
    AFX_ASSERT_OBJECTS(afxFcc_VDU, rslt, displays);
    return rslt;
}
