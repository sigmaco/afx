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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_C
//#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_BRIDGE_C
#define _AFX_DRAW_STREAM_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxDrawStreamState AfxGetDrawStreamState(afxDrawStream diob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->state;
}

_AVX afxNat AfxGetDrawStreamPort(afxDrawStream diob)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    return diob->portIdx;
}

_AVX afxError AfxRecycleDrawStreams(afxBool freeRes, afxNat cnt, afxDrawStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxDrawStream diob = streams[i];
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
    }
    return err;
}

_AVX afxError AfxCompileDrawStreams(afxNat cnt, afxDrawStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxDrawStream diob = streams[i];
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
    }
    return err;
}

_AVX afxError AfxRecordDrawStreams(afxDrawStreamUsage usage, afxNat cnt, afxDrawStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxDrawStream diob = streams[i];
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
    }
    return err;
}

_AVX afxError AfxAcquireDrawStreams(afxDrawContext dctx, afxNat portIdx, afxNat* queIdxPtr, afxNat cnt, afxDrawStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(queIdxPtr);
    AfxAssert(streams);
    AfxAssert(cnt);
    afxNat cnt2 = 0;

    afxDrawBridge ddge = AfxGetDrawBridge(dctx, portIdx);
    AfxAssertObjects(1, &ddge, afxFcc_DDGE);

    for (afxNat queIdx = 0; queIdx < ddge->queCnt; queIdx++)
    {
        afxDrawQueue* dque = &ddge->queues[queIdx];

        if (AfxTryEnterSlockExclusive(&dque->reqLock))
        {
            afxDrawStream* unit;
            while ((unit = AfxPullNextQueueUnit(&dque->recycQue)))
            {
                afxDrawStream diob = *unit;
                AfxAssertObjects(1, &diob, afxFcc_DIOB);

                if (afxDrawStreamState_INVALID == AfxGetDrawStreamState(diob))
                {
                    if (AfxRecycleDrawStreams(TRUE, 1, &diob)) AfxThrowError();
                    else
                    {
                        streams[cnt2] = diob;
                        ++cnt2;
                    }
                }

                AfxPopNextQueue(&dque->recycQue);

                if (cnt2 >= cnt)
                    break;
            }

            if (cnt2 < cnt)
            {
                if (AfxAcquireObjects(&dque->streams, cnt - cnt2, (afxObject*)&streams[cnt2], (void const*[]) { ddge, &queIdx }))
                {
                    AfxThrowError();

                    for (afxNat i = 0; i < cnt2; i++)
                    {
                        if (AfxPushQueueUnit(&dque->recycQue, &streams[i]))
                        {
                            AfxReleaseObjects(1, (void**)&streams[i]);
                        }
                        streams[i] = NIL;
                    }
                }
            }
            AfxExitSlockExclusive(&dque->reqLock);
            *queIdxPtr = (!err) ? queIdx : AFX_INVALID_INDEX;

            if (!err)
                break;
        }
    }
    return err;
}

_AVX afxError AfxOpenDrawStreams(afxDrawContext dctx, afxNat portIdx, afxNat queIdx, afxNat cnt, afxDrawStream streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat cnt2 = 0;

    afxDrawBridge ddge = AfxGetDrawBridge(dctx, portIdx);
    AfxAssertRange(ddge->queCnt, queIdx, 1);
    afxDrawQueue* dque = &ddge->queues[queIdx];

    AfxEnterSlockExclusive(&dque->reqLock);

    afxDrawStream* unit;
    while ((unit = AfxPullNextQueueUnit(&dque->recycQue)))
    {
        afxDrawStream diob = *unit;
        AfxAssertObjects(1, &diob, afxFcc_DIOB);

        if (afxDrawStreamState_INVALID == AfxGetDrawStreamState(diob))
        {
            if (AfxRecycleDrawStreams(TRUE, 1, &diob)) AfxThrowError();
            else
            {
                streams[cnt2] = diob;
                ++cnt2;
            }
        }

        AfxPopNextQueue(&dque->recycQue);

        if (cnt2 >= cnt)
            break;
    }

    if (cnt2 < cnt)
    {
        if (AfxAcquireObjects(&dque->streams, cnt - cnt2, (afxObject*)&streams[cnt2], (void const*[]) { ddge, &queIdx }))
        {
            AfxThrowError();

            for (afxNat i = 0; i < cnt2; i++)
            {
                if (AfxPushQueueUnit(&dque->recycQue, &streams[i]))
                {
                    AfxReleaseObjects(1, (void**)&streams[i]);
                }
                streams[i] = NIL;
            }
        }
    }

    AfxExitSlockExclusive(&dque->reqLock);

    return err;
}
