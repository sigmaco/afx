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
//#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_STREAM_C
#include "qwadro/core/afxManager.h"
#include "qwadro/mem/afxArena.h"
#include "qwadro/draw/pipe/afxDrawOps.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxError AfxRecycleDrawStream(afxDrawStream dscr, afxBool freeRes)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    (void)freeRes;

    if (dscr->state == afxDrawStreamState_PENDING) AfxThrowError();
    else
    {
        if (dscr->resetCb(dscr)) AfxThrowError();
        else
        {
            dscr->state = afxDrawStreamState_INITIAL;
        }
    }
    return err;
}

_AVX afxError AfxCompileDrawStream(afxDrawStream dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);

    if (dscr->state != afxDrawStreamState_RECORDING) AfxThrowError();
    else
    {
        if (dscr->endCb(dscr)) AfxThrowError();
        else
        {
            dscr->state = afxDrawStreamState_EXECUTABLE;
        }
    }
    return err;
}

_AVX afxError AfxRecordDrawStream(afxDrawStream dscr, afxDrawStreamUsage usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);

    if (!(dscr->state == afxDrawStreamState_INITIAL || dscr->state == afxDrawStreamState_EXECUTABLE)) AfxThrowError();
    else
    {
        if (dscr->beginCb(dscr, usage)) AfxThrowError();
        else
        {
            dscr->state = afxDrawStreamState_RECORDING;
        }
    }
    return err;
}

AVX afxDrawStreamState AfxGetDrawStreamState(afxDrawStream dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->state;
}

AVX afxNat AfxGetDrawStreamPort(afxDrawStream dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    return dscr->portIdx;
}

_AVX afxDrawInput AfxGetDrawStreamInput(afxDrawStream dscr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dscr, afxFcc_DSCR);
    afxDrawInput din = dscr->din;
    //AfxAssertType(din, afxFcc_DIN);
    return din;
}

_AVX afxError AfxAcquireDrawStreams(afxDrawInput din, afxNat portIdx, afxNat cnt, afxDrawStream dscr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawContext dctx;

    if (!(dctx = AfxGetDrawInputContext(din))) AfxThrowError();
    else
    {
        //AfxAssertType(dctxD, afxFcc_DCTX);
        afxNat cnt2 = 0;

        for (afxNat i = 0; i < din->scripts.cnt; i++)
        {
            afxDrawStream dscr2 = *(afxDrawStream*)AfxGetArrayUnit(&din->scripts, i);
            AfxAssertObjects(1, &dscr2, afxFcc_DSCR);

            if (afxDrawStreamState_INVALID == AfxGetDrawStreamState(dscr2))
            {
                if (AfxRecycleDrawStream(dscr2, TRUE)) AfxThrowError();
                else
                {
                    dscr[cnt2] = dscr2;
                    ++cnt2;
                }
            }

            if (cnt2 >= cnt)
                break;
        }

        if (cnt2 < cnt)
        {
            afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);

            if (AfxAcquireObjects(&ddev->ports[portIdx].scripts, cnt - cnt2, (afxObject*)&dscr[cnt2], (void const*[]) { dctx, (void*)&portIdx, &din }))
                AfxThrowError();
        }
    }

    if (err)
        for (afxNat i = 0; i < cnt; i++)
            dscr[i] = NIL;

    return err;
}
