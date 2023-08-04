/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_THREAD_C
#define _AFX_THREAD_C
#include "_classified/afxDrawClassified.h"
#include "afx/draw/afxDrawThread.h"

_AFX afxDrawQueue AfxGetDrawThreadActiveQueue(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dthr, AFX_FCC_DTHR);
    afxDrawQueue dque = dthr->dque;
    AfxTryAssertObject(dque, AFX_FCC_DQUE);
    return dque;
}

_AFX afxDrawContext AfxGetDrawThreadActiveContext(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dthr, AFX_FCC_DTHR);
    afxDrawContext dctx = dthr->dctx;
    AfxTryAssertObject(dctx, AFX_FCC_DCTX);
    return dctx;
}

_AFX afxDrawDriver AfxGetDrawThreadActiveDriver(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dthr, AFX_FCC_DTHR);
    afxDrawDriver ddrv = dthr->ddrv;
    AfxTryAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX void AfxReleaseDrawThreads(afxNat cnt, afxDrawThread dthr[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssertObject(dthr[i], AFX_FCC_DTHR);
        AfxReleaseObject(&dthr[i]->thr.obj);
    }
}

_AFX afxError AfxAcquireDrawThreads(afxDrawThreadSpecification const *spec, afxNat cnt, afxDrawThread dthr[])
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("spec=%p,cnt=%u", spec, cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        if (AfxClassAcquireObjects(AfxGetDrawThreadClass(), NIL, 1, spec, (afxObject**)&dthr[i], AfxSpawnHint()))
        {
            AfxThrowError();

            for (afxNat j = 0; j < i; j++)
            {
                AfxAssertObject(dthr[j], AFX_FCC_DTHR);
                AfxReleaseObject(&dthr[j]->thr.obj);
                dthr[j] = NIL;
            }
            break;
        }
        else
        {
            AfxAssertObject(dthr[i], AFX_FCC_DTHR);
        }
    }

    if (err)
        for (afxNat i = 0; i < cnt; i++)
            dthr[i] = NIL;

    return err;
};

_AFX afxBool _AfxDthrEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawThread dthr = (void*)obj;
    AfxAssertObject(dthr, AFX_FCC_DTHR);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxDthrEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawThread dthr = (void*)obj;
    AfxAssertObject(dthr, AFX_FCC_DTHR);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxDthrDtor(afxDrawThread dthr)
{
    AfxEntry("dthr=%p", dthr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dthr, AFX_FCC_DTHR);

    return err;
}

_AFX afxBool _AfxProcessDrawDriverCb(afxDrawDriver ddrv, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxDrawThread dthr = AfxRebase(udd, afxDrawThread, thr);
    AfxAssertObject(dthr, AFX_FCC_DTHR);

    if (AfxDrawDriverIsRunning(ddrv))
    {
        dthr->ddrv = ddrv;
        dthr->dctx = NIL;
        dthr->dque = NIL;

        if (ddrv->vmt->proc(ddrv, dthr))
            AfxThrowError();

        dthr->ddrv = NIL;
        dthr->dctx = NIL;
        dthr->dque = NIL;
    }
    return FALSE; // dont interrupt curation
}

_AFX afxError _AfxDthrThrProc(afxThread thr, void *udd, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;
    afxDrawThread dthr = AfxRebase(thr, afxDrawThread, thr);
    AfxAssertObject(dthr, AFX_FCC_DTHR);

    AfxCurateDrawDrivers(0, AFX_N32_MAX, _AfxProcessDrawDriverCb, thr);

    return err;
}

_AFX afxError _AfxDthrCtor(void *cache, afxNat idx, afxDrawThread dthr, afxDrawThreadSpecification const *specs)
{
    AfxEntry("dthr=%p", dthr);
    afxError err = AFX_ERR_NONE;
    (void)cache;
    AfxAssertObject(dthr, AFX_FCC_DTHR);

    afxDrawThreadSpecification const *spec = &specs[idx];

    dthr->ddrv = NIL;
    dthr->dctx = NIL;
    dthr->dque = NIL;
    dthr->queueIdx = 0;

    dthr->mem = NIL;

    dthr->thr.proc = _AfxDthrThrProc;

    return err;
}

_AFX afxClassSpecification const _AfxDthrClassSpec;

afxClassSpecification const _AfxDthrClassSpec =
{
    AFX_FCC_DTHR,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawThread)),
    NIL,
    (void*)_AfxDthrCtor,
    (void*)_AfxDthrDtor,
    .event = _AfxDthrEventHandler,
    .eventFilter = _AfxDthrEventFilter,
    "afxDrawThread",
    NIL
};
