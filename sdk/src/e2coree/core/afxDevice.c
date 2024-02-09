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
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"

_AFX afxClassConfig const _AfxDevClsConfig =
{
    .fcc = afxFcc_DEV,
    .name = "Device",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxDevice)),    
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxInvokeDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetDeviceClass();
    AfxAssertClass(cls, afxFcc_DEV);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateDevices(afxNat first, afxNat cnt, afxDevice dev[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(dev);
    afxClass* cls = AfxGetDeviceClass();
    AfxAssertClass(cls, afxFcc_DEV);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)dev);
}

_AFX afxNat AfxCountDevices(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetDeviceClass();
    AfxAssertClass(cls, afxFcc_DEV);
    return AfxCountInstances(cls);
}
