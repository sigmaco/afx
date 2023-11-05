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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_THREAD_C
#include "afxDrawClassified.h"
#include "afx/draw/afxDrawThread.h"

_AFX afxBool AfxGetDrawThreadActiveQueue(afxDrawThread dthr, afxDrawQueue* dque)
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

_AFX afxBool AfxGetDrawThreadActiveContext(afxDrawThread dthr, afxDrawContext* dctx)
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

_AFX afxDrawDevice AfxGetDrawThreadDriver(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    afxDrawDevice ddev = dthr->ddev;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AFX void* AfxGetDrawThreadUdd(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    return dthr->udd;
}

_AFX afxThread AfxGetDrawThreadBase(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    afxThread thr = dthr->thr;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr;
}

_AFX afxError _AfxDthrDtor(afxDrawThread dthr)
{
    AfxEntry("dthr=%p", dthr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    
    AfxReleaseObjects(1, (void*[]) { dthr->thr });

    return err;
}

_AFX afxBool _AfxProcessDdevCb(afxDrawDevice ddev, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawThread dthr = (afxDrawThread)udd;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    AfxAssert(dthr == (afxDrawThread)AfxGetThreadUdd(dthr->thr));

    if (AfxDrawDeviceIsRunning(ddev))
    {
        dthr->ddev = ddev;
        dthr->dctx = NIL;
        dthr->dque = NIL;

        if (ddev->procCb(ddev, dthr))
            AfxThrowError();

        dthr->ddev = NIL;
        dthr->dctx = NIL;
        dthr->dque = NIL;
    }
    return FALSE; // dont interrupt curation
}

_AFX afxError _AfxThrProcDthrCb(afxThread thr, void *udd, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;
    afxDrawThread dthr = (afxDrawThread)udd;// AfxRebase(thr, afxDrawThread, thr);
    AfxAssert(dthr == (afxDrawThread)AfxGetThreadUdd(thr));
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);
    (void)opcode;

    AfxAssert(dthr == (afxDrawThread)AfxGetThreadUdd(dthr->thr));

    afxDrawSystem dsys = AfxGetObjectProvider(dthr);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    AfxCurateDrawDevices(dsys, 0, AFX_N32_MAX, _AfxProcessDdevCb, dthr);

    return err;
}

_AFX afxError _AfxDthrCtor(afxDrawThread dthr, afxCookie const* cookie)
{
    AfxEntry("dthr=%p", dthr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, afxFcc_DTHR);

    afxDrawThreadConfig const *config = ((afxDrawThreadConfig const*)cookie->udd[0]) + cookie->no;

    dthr->ddev = NIL;
    dthr->dctx = NIL;
    dthr->dque = NIL;
    dthr->queueIdx = 0;

    dthr->udd = NIL;

    afxThreadConfig thrConfig = { 0 };
    thrConfig.proc = _AfxThrProcDthrCb;
    thrConfig.udd = (void*)dthr;
    AfxAcquireThreads(1, &dthr->thr, &thrConfig, AfxSpawnHint());

    return err;
}

_AFX afxError AfxAcquireDrawThreads(afxDrawSystem dsys, afxNat cnt, afxDrawThreadConfig const config[], afxDrawThread dthr[])
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cnt=%u,dthr=%p,config=%p", cnt, dthr, config);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    AfxAssertClass(&dsys->threads, afxFcc_DTHR);

    if (AfxAcquireObjects(&dsys->threads, cnt, (afxHandle*)dthr, (void*[]) { (void*)config }))
        AfxThrowError();

    AfxAssertObjects(cnt, dthr, afxFcc_DTHR);

    return err;
};

_AFX afxClassConfig _AfxDthrClsConfig =
{
    .fcc = afxFcc_DTHR,
    .name = "Draw Thread",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawThread)),
    .ctx = NIL,
    .ctor = (void*)_AfxDthrCtor,
    .dtor = (void*)_AfxDthrDtor
};
