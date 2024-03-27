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
#define _AFX_DRAW_C
#define _AFX_DRAW_THREAD_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_THREAD_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxBool AfxGetDrawThreadActiveQueue(afxDrawThread dthr, afxDrawQueue* dque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    afxDrawQueue dque2;
    afxBool rslt;

    if ((rslt = !!(dque2 = dthr->dque)))
    {
        AfxAssertObjects(1, &dque2, afxFcc_DQUE);

        if (dque)
            *dque = dque2;
    }
    return rslt;
}

_AVX afxBool AfxGetDrawThreadActiveContext(afxDrawThread dthr, afxDrawContext* dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    afxDrawContext dctx2;
    afxBool rslt;

    if ((rslt = !!(dctx2 = dthr->dctx)))
    {
        AfxAssertObjects(1, &dctx2, afxFcc_DCTX);

        if (dctx)
            *dctx = dctx2;
    }
    return rslt;
}

_AVX afxDrawDevice AfxGetDrawThreadDevice(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    afxDrawDevice ddev = dthr->ddev;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX void* AfxGetDrawThreadUdd(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    return dthr->udd;
}

_AVX afxThread AfxGetDrawThreadBase(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    afxThread thr = &dthr->thr;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr;
}

_AVX afxBool _AfxProcessDdevCb(afxDrawDevice ddev, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawThread dthr = (afxDrawThread)udd;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    //AfxAssert(dthr == (afxDrawThread)AfxGetThreadUdd(&dthr->thr));

    if (AfxDrawDeviceIsRunning(ddev))
    {
        dthr->ddev = ddev;
        dthr->dctx = NIL;
        dthr->dque = NIL;

        if (ddev->procCb(ddev, dthr))
            AfxThrowError();

        //dthr->ddev = NIL;
        dthr->dctx = NIL;
        dthr->dque = NIL;
    }
    return FALSE; // dont interrupt curation
}

_AVX afxError _AfxThrProcDthrCb(afxThread thr, void *udd, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;
    afxDrawThread dthr = (afxDrawThread)thr;// AfxRebase(thr, afxDrawThread, thr);
    //AfxAssert(dthr == (afxDrawThread)AfxGetThreadUdd(thr));
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    (void)opcode;

    //AfxAssert(dthr == (afxDrawThread)AfxGetThreadUdd(&dthr->thr));

    //AfxInvokeDrawDevices(0, AFX_N32_MAX, _AfxProcessDdevCb, dthr);

    _AfxProcessDdevCb(dthr->ddev, dthr);

    return err;
}

_AVX afxError _AfxDthrCtor(afxDrawThread dthr, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    afxDrawThreadConfig const *config = ((afxDrawThreadConfig const*)cookie->udd[0]) + cookie->no;

    dthr->thr.proc = _AfxThrProcDthrCb;

    dthr->ddev = config->ddev;
    dthr->dctx = NIL;
    dthr->dque = NIL;
    dthr->queueIdx = 0;

    dthr->udd = NIL;

    return err;
}

_AVX afxError _AfxDthrDtor(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    return err;
}

_AVX afxClassConfig const _dthrClsConfig =
{
    .fcc = afxFcc_DTHR,
    .name = "Draw Thread",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxDrawThread)),
    .mmu = NIL,
    .ctor = (void*)_AfxDthrCtor,
    .dtor = (void*)_AfxDthrDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireDrawThreads(afxDrawThreadConfig const* cfg, afxHint const hint, afxNat cnt, afxDrawThread dthr[])
{
    afxError err = AFX_ERR_NONE;
    
    afxManager* cls = AfxGetDrawThreadClass();
    AfxAssertClass(cls, afxFcc_DTHR);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)dthr, (void const*[]) { cfg, hint }))
        AfxThrowError();

    AfxAssertObjects(cnt, dthr, afxFcc_DTHR);

    return err;
};

_AVX afxNat AfxInvokeDrawThreads(afxNat first, afxNat cnt, afxBool(*f)(afxDrawThread, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetDrawThreadClass();
    AfxAssertClass(cls, afxFcc_DTHR);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEnumerateDrawThreads(afxNat first, afxNat cnt, afxDrawThread dthr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(dthr);
    afxManager* cls = AfxGetDrawThreadClass();
    AfxAssertClass(cls, afxFcc_DTHR);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)dthr);
}

_AVX afxNat AfxCountDrawThreads(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetDrawThreadClass();
    AfxAssertClass(cls, afxFcc_DTHR);
    return AfxCountObjects(cls);
}
