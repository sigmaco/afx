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
#define _AFX_DRAW_BRIDGE_C
#define _AFX_DRAW_STREAM_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxError AfxRecycleDrawStream(afxDrawStream diob, afxBool freeRes)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    (void)freeRes;

    if (diob->state == afxDrawStreamState_PENDING) AfxThrowError();
    else
    {
        if (diob->resetCb(diob)) AfxThrowError();
        else
        {
            diob->state = afxDrawStreamState_INITIAL;
        }
    }
    return err;
}

_AVX afxError AfxCompileDrawStream(afxDrawStream diob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);

    if (diob->state != afxDrawStreamState_RECORDING) AfxThrowError();
    else
    {
        if (diob->endCb(diob)) AfxThrowError();
        else
        {
            diob->state = afxDrawStreamState_EXECUTABLE;
        }
    }
    return err;
}

_AVX afxError AfxRecordDrawStream(afxDrawStream diob, afxDrawStreamUsage usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);

    if (!(diob->state == afxDrawStreamState_INITIAL || diob->state == afxDrawStreamState_EXECUTABLE)) AfxThrowError();
    else
    {
        if (diob->beginCb(diob, usage)) AfxThrowError();
        else
        {
            diob->state = afxDrawStreamState_RECORDING;
        }
    }
    return err;
}

AVX afxDrawStreamState AfxGetDrawStreamState(afxDrawStream diob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->state;
}

AVX afxNat AfxGetDrawStreamPort(afxDrawStream diob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->portIdx;
}

_AVX afxError AfxAcquireDrawStreams(afxDrawContext dctx, afxNat portIdx, afxNat cnt, afxDrawStream scripts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat cnt2 = 0;
#if 0

    for (afxNat i = 0; i < din->scripts.cnt; i++)
    {
        afxDrawStream dscr2 = *(afxDrawStream*)AfxGetArrayUnit(&din->scripts, i);
        AfxAssertObjects(1, &dscr2, afxFcc_DIOB);

        if (afxDrawStreamState_INVALID == AfxGetDrawStreamState(dscr2))
        {
            if (AfxRecycleDrawStream(dscr2, TRUE)) AfxThrowError();
            else
            {
                scripts[cnt2] = dscr2;
                ++cnt2;
            }
        }

        if (cnt2 >= cnt)
            break;
    }
#endif

    if (cnt2 < cnt)
    {
        afxDrawBridge dxge = AfxGetDrawBridge(dctx, portIdx);

        if (AfxAcquireObjects(&dxge->scripts, cnt - cnt2, (afxObject*)&scripts[cnt2], (void const*[]) { dxge }))
            AfxThrowError();
    }

    return err;
}
