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



    return err;
}

_AVX afxError _AvxVduCtorCb(afxDisplay vdu, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VDU, 1, &vdu);

    afxDrawDevice ddev = AFX_CAST(afxDrawDevice, args[0]);
    avxDisplayInfo const* info = AFX_CAST(avxDisplayInfo const*, args[1]) + invokeNo;
    
    //if (_AfxDevBaseImplementation.ctor(&vdu->dev, (void*[]) { ddev, (void*)&info->dev }, 0)) AfxThrowError();
    //else
    {
        vdu->dimWh[0] = info->dimWh[0];
        vdu->dimWh[1] = info->dimWh[1];
        vdu->resWh[0] = info->resWh[0];
        vdu->resWh[1] = info->resWh[1];
        vdu->persistentContent = info->persistentContent;
        vdu->planeReorder = info->planeReorder;
        vdu->supportedXforms = info->supportedXforms;
        vdu->name = info->name;
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
