/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_THREAD_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_THREAD_C
#include "_classified/afxDrawClassified.h"
#include "afx/draw/afxDrawThread.h"

_AFX afxBool _AfxGetDthrD(afxDrawThread dthr, struct _afxDthrD **dthrD, struct _afxDsysD* dsysD)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssert(dthrD);
    return AfxGetObjectsResidency(1, &dthr, (void**)dthrD, &dsysD->threads);
}

_AFX afxDrawQueue AfxGetDrawThreadActiveQueue(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    struct _afxDthrD *dthrD;
    _AfxGetDthrD(dthr, &dthrD, dsysD);
    AfxAssertType(dthrD, AFX_FCC_DTHR);
    afxDrawQueue dque = dthrD->dque;
    AfxTryAssertObject(dque, AFX_FCC_DQUE);
    return dque;
}

_AFX afxDrawContext AfxGetDrawThreadActiveContext(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    struct _afxDthrD *dthrD;
    _AfxGetDthrD(dthr, &dthrD, dsysD);
    AfxAssertType(dthrD, AFX_FCC_DTHR);
    afxDrawContext dctx = dthrD->dctx;
    AfxTryAssertObject(dctx, AFX_FCC_DCTX);
    return dctx;
}

_AFX afxDrawDriver AfxGetDrawThreadActiveDriver(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    struct _afxDthrD *dthrD;
    _AfxGetDthrD(dthr, &dthrD, dsysD);
    AfxAssertType(dthrD, AFX_FCC_DTHR);
    afxDrawDriver ddrv = dthrD->ddrv;
    AfxTryAssertObject(ddrv, AFX_FCC_DDRV);
    return ddrv;
}

_AFX afxError _AfxDthrDtor(afxDrawThread dthr, struct _afxDthrD *dthrD)
{
    AfxEntry("dthr=%p", dthr);
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    AfxAssertType(dthrD, AFX_FCC_DTHR);

    AfxReleaseThreads(1, &dthrD->thr);

    return err;
}

_AFX void _AfxObjDeallocDthr(afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    struct _afxDthrD *dthrD;
    _AfxGetDthrD(dthr, &dthrD, dsysD);
    AfxAssertType(dthrD, AFX_FCC_DTHR);
    _AfxDthrDtor(dthr, dthrD);
    AfxDeallocateObjects(1, &dthr, &dsysD->threads);
}

_AFX void AfxReleaseDrawThreads(afxNat cnt, afxDrawThread dthr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, dthr, AFX_FCC_DTHR);
    AfxReleaseObjects(cnt, dthr, _AfxObjDeallocDthr);
}

_AFX afxBool _AfxProcessDrawDriverCb(afxDrawDriver ddrv, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ddrv, AFX_FCC_DDRV);
    afxDrawThread dthr = (afxDrawThread)udd;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    struct _afxDthrD *dthrD;
    _AfxGetDthrD(dthr, &dthrD, dsysD);
    AfxAssertType(dthrD, AFX_FCC_DTHR);
    AfxAssert(dthr == (afxDrawThread)AfxGetThreadUdd(dthrD->thr));

    if (AfxDrawDriverIsRunning(ddrv))
    {
        dthrD->ddrv = ddrv;
        dthrD->dctx = NIL;
        dthrD->dque = NIL;

        if (ddrv->vmt->proc(ddrv, dthr))
            AfxThrowError();

        dthrD->ddrv = NIL;
        dthrD->dctx = NIL;
        dthrD->dque = NIL;
    }
    return FALSE; // dont interrupt curation
}

_AFX afxError _AfxThrProcDthrCb(afxThread thr, void *udd, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;
    afxDrawThread dthr = (afxDrawThread)udd;// AfxRebase(thr, afxDrawThread, thr);
    AfxAssert(dthr == (afxDrawThread)AfxGetThreadUdd(thr));
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);
    struct _afxDthrD *dthrD;
    _AfxGetDthrD(dthr, &dthrD, dsysD);
    AfxAssertType(dthrD, AFX_FCC_DTHR);
    AfxAssert(dthr == (afxDrawThread)AfxGetThreadUdd(dthrD->thr));

    AfxCurateDrawDrivers(0, AFX_N32_MAX, _AfxProcessDrawDriverCb, (void*)dthr);

    return err;
}

_AFX afxError _AfxDthrCtor(afxDrawThread dthr, struct _afxDthrD* dthrD, afxDrawThreadConfig const *config)
{
    AfxEntry("dthr=%p", dthr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dthr, AFX_FCC_DTHR);

    AfxAssignFcc(dthrD, AFX_FCC_DTHR);

    dthrD->dthrObj = dthr;

    dthrD->ddrv = NIL;
    dthrD->dctx = NIL;
    dthrD->dque = NIL;
    dthrD->queueIdx = 0;

    dthrD->mem = NIL;

    dthrD->dthrObj = dthr;

    afxThreadConfig thrConfig = { 0 };
    thrConfig.proc = _AfxThrProcDthrCb;
    thrConfig.udd = (void*)dthr;
    AfxAcquireThreads(1, &dthrD->thr, &thrConfig, AfxSpawnHint());

    return err;
}

_AFX afxError AfxAcquireDrawThreads(afxNat cnt, afxDrawThread dthr[], afxDrawThreadConfig const config[])
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cnt=%u,dthr=%p,config=%p", cnt, dthr, config);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    if (AfxAcquireNamedObjects(cnt, dthr, AFX_FCC_DTHR, "Draw Thread")) AfxThrowError();
    else
    {
        if (AfxAllocateObjects(cnt, dthr, &dsysD->threads)) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < cnt; i++)
            {
                struct _afxDthrD *dthrD;
                _AfxGetDthrD(dthr[i], &dthrD, dsysD);

                if (_AfxDthrCtor(dthr[i], dthrD, config ? &config[i] : NIL))
                {
                    AfxThrowError();
                    afxNat j = i;

                    for (afxNat k = 0; k < i; k++)
                        if (_AfxDthrDtor(dthr[j--], dthrD))
                            AfxThrowError();
                }
            }
        }

        if (err)
            AfxReleaseObjects(cnt, dthr, NIL);
    }

    if (err)
    {
        for (afxNat i = 0; i < cnt; i++)
            dthr[i] = NIL;
    }
    else
    {
        AfxAssertObjects(cnt, dthr, AFX_FCC_DTHR);
    }
    return err;
};
