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
#define _AFX_SERVICE_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/core/afxService.h"

_AFX afxError _AfxSvcDtor(afxService svc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &svc, afxFcc_SVC);

    return err;
}

_AFX afxError _AfxSvcCtor(afxService svc, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &svc, afxFcc_SVC);

    afxServiceConfig const *config = ((afxServiceConfig const *)cookie->udd[0]) + cookie->no;

    return err;
}

_AFX afxClassConfig const _AfxSvcClsConfig =
{
    .fcc = afxFcc_SVC,
    .name = "Autonomous Service",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxService)),
    .mmu = NIL,
    .ctor = (void*)_AfxSvcCtor,
    .dtor = (void*)_AfxSvcDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireServices(afxNat cnt, afxServiceConfig const config[], afxService services[])
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetServiceClass();
    AfxAssertClass(cls, afxFcc_SVC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)services, (void const*[]) { (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, services, afxFcc_SVC);

    return err;
}

_AFX afxNat AfxInvokeServices(afxNat first, afxNat cnt, afxBool(*f)(afxService, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetServiceClass();
    AfxAssertClass(cls, afxFcc_SVC);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateServices(afxNat first, afxNat cnt, afxService services[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(services);
    afxClass* cls = AfxGetServiceClass();
    AfxAssertClass(cls, afxFcc_SVC);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)services);
}

_AFX afxNat AfxCountServices(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetServiceClass();
    AfxAssertClass(cls, afxFcc_SVC);
    return AfxCountInstances(cls);
}
