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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AVX_DRAW_C
//#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_INPUT_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_QUEUE_C
#define _AVX_CMD_BUFFER_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX avxCmdbState AfxGetCmdBufferState(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    return cmdb->state;
}

_AVX afxNat AfxGetCmdBufferPort(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    return cmdb->portIdx;
}

_AVX afxNat AfxGetCmdBufferPool(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
    return cmdb->poolIdx;
}

_AVX afxError _AvxCmdbStdDtor(avxCmdb cmdb)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

    while (AfxLoadAtom32(&cmdb->submCnt))
    {
        AfxYield();
    }

    AfxAssert(cmdb->state != avxCmdbState_PENDING);

    AfxRecycleCmdBuffers(TRUE, 1, &cmdb);

    //if (cmdb->base.vmt->dtor && cmdb->base.vmt->dtor(cmdb))
        //AfxThrowError();

    //AfxAssert(!cmdb->base.idd);

    AfxDeallocateArena(&cmdb->cmdArena);

    return err;
}

_AVX afxError _AvxCmdbStdCtor(avxCmdb cmdb, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

    afxDrawInput din = cookie->udd[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxNat poolIdx = *(afxNat*)cookie->udd[1];

    //afxDrawContext dctx = AfxGetDrawBridgeContext(ddge);

    cmdb->submCnt = 0;
    cmdb->submQueMask = NIL;
    cmdb->portIdx = /*din->portIdx*/0;
    cmdb->poolIdx = poolIdx;
    AfxAllocateArena(NIL, &cmdb->cmdArena, NIL, AfxHere());

    cmdb->disposable = TRUE;

    cmdb->state = avxCmdbState_INITIAL;

    cmdb->stdCmds = NIL;

    cmdb->beginCb = NIL;
    cmdb->endCb = NIL;
    cmdb->resetCb = NIL;

    return err;
}

_AVX afxClassConfig const _AvxCmdbStdImplementation =
{
    .fcc = afxFcc_CMDB,
    .name = "CmdBuffer",
    .desc = "Device Workload Buffer",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(avxCmdb)),
    .ctor = (void*)_AvxCmdbStdCtor,
    .dtor = (void*)_AvxCmdbStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxCompileCmdBuffers(afxNat cnt, avxCmdb streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        avxCmdb cmdb = streams[i];
        AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

        if (cmdb->state != avxCmdbState_RECORDING) AfxThrowError();
        else
        {
            if (cmdb->endCb(cmdb)) AfxThrowError();
            else
            {
                cmdb->state = avxCmdbState_EXECUTABLE;
            }
        }
    }
    return err;
}

_AVX afxError AfxRecycleCmdBuffers(afxBool freeRes, afxNat cnt, avxCmdb streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        avxCmdb cmdb = streams[i];
        AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
        afxDrawInput din = AfxGetObjectProvider(cmdb);
        AfxAssertObjects(1, &din, afxFcc_DIN);
        afxNat poolIdx = cmdb->poolIdx;
        
        if (AfxTryEnterSlockExclusive(&din->pools[poolIdx].reqLock))
        {
            if (AfxPushQueueUnit(&din->pools[poolIdx].recycQue, &cmdb))
            {
                AfxReleaseObjects(1, &cmdb);
            }
            else
            {
                cmdb->state = avxCmdbState_INVALID;
            }
            AfxExitSlockExclusive(&din->pools[poolIdx].reqLock);
        }
    }
    return err;
}

#if 0
_AVX afxError AfxRecordCmdBuffers(avxCmdbUsage usage, afxNat cnt, avxCmdb streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(streams);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        avxCmdb cmdb = streams[i];
        AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

        if (!(cmdb->state == avxCmdbState_INITIAL || cmdb->state == avxCmdbState_EXECUTABLE)) AfxThrowError();
        else
        {
            if (cmdb->beginCb(cmdb, usage)) AfxThrowError();
            else
            {
                cmdb->state = avxCmdbState_RECORDING;
            }
        }
    }
    return err;
}
#endif

_AVX afxError AfxAcquireCmdBuffers(afxDrawInput din, avxCmdbUsage usage, afxNat cnt, avxCmdb streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(streams);
    AfxAssert(cnt);
    afxNat cnt2 = 0;

    afxDrawContext dctx;

    if (!AfxGetDrawInputContext(din, &dctx)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        for (afxNat poolIdx = 0; poolIdx < din->poolCnt; poolIdx++)
        {
            if (AfxTryEnterSlockExclusive(&din->pools[poolIdx].reqLock))
            {
                avxCmdb* unit;

                while ((unit = AfxPullNextQueueUnit(&din->pools[poolIdx].recycQue)))
                {
                    avxCmdb cmdb = *unit;
                    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

                    if (cmdb->resetCb(cmdb)) AfxThrowError();
                    else
                    {
                        cmdb->state = avxCmdbState_INITIAL;

                        if (cmdb->beginCb(cmdb, usage)) AfxThrowError();
                        else
                        {
                            cmdb->state = avxCmdbState_RECORDING;

                            streams[cnt2] = cmdb;
                            ++cnt2;
                        }
                    }

                    if (err)
                        AfxReleaseObjects(1, &cmdb);

                    AfxPopNextQueue(&din->pools[poolIdx].recycQue);

                    if (cnt2 >= cnt)
                        break;
                }

                if (cnt2 < cnt)
                {
                    if (AfxAcquireObjects(&din->pools[poolIdx].streams, cnt - cnt2, (afxObject*)&streams[cnt2], (void const*[]) { din, &poolIdx }))
                    {
                        AfxThrowError();
                        AfxRecycleCmdBuffers(TRUE, cnt2, streams);
                    }
                    else
                    {
                        for (afxNat i = 0; i < cnt - cnt2; i++)
                        {
                            avxCmdb cmdb = streams[cnt2 + i];
                            AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

                            if (cmdb->beginCb(cmdb, usage)) AfxThrowError();
                            else
                            {
                                cmdb->state = avxCmdbState_RECORDING;

                                streams[cnt2] = cmdb;
                                ++cnt2;
                            }
                        }
                    }
                }

                AfxExitSlockExclusive(&din->pools[poolIdx].reqLock);
            }
        }
    }
    return err;
}

_AVX afxError AfxOpenCmdBuffers(afxDrawInput din, afxNat poolIdx, avxCmdbUsage usage, afxNat cnt, avxCmdb streams[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxNat cnt2 = 0;

    afxDrawContext dctx;

    if (!AfxGetDrawInputContext(din, &dctx)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        AfxEnterSlockExclusive(&din->pools[poolIdx].reqLock);

        avxCmdb* unit;
        while ((unit = AfxPullNextQueueUnit(&din->pools[poolIdx].recycQue)))
        {
            avxCmdb cmdb = *unit;
            AfxAssertObjects(1, &cmdb, afxFcc_CMDB);
            AfxAssert(avxCmdbState_INVALID == AfxGetCmdBufferState(cmdb));

            if (cmdb->resetCb(cmdb)) AfxThrowError();
            else
            {
                cmdb->state = avxCmdbState_INITIAL;

                if (cmdb->beginCb(cmdb, usage)) AfxThrowError();
                else
                {
                    cmdb->state = avxCmdbState_RECORDING;

                    streams[cnt2] = cmdb;
                    ++cnt2;
                }
            }

            if (err)
                AfxReleaseObjects(1, &cmdb);

            AfxPopNextQueue(&din->pools[poolIdx].recycQue);

            if (cnt2 >= cnt)
                break;
        }

        if (cnt2 < cnt)
        {
            if (AfxAcquireObjects(&din->pools[poolIdx].streams, cnt - cnt2, (afxObject*)&streams[cnt2], (void const*[]) { din, &poolIdx }))
            {
                AfxThrowError();

                for (afxNat i = 0; i < cnt2; i++)
                {
                    if (AfxPushQueueUnit(&din->pools[poolIdx].recycQue, &streams[i]))
                    {
                        AfxReleaseObjects(1, (void**)&streams[i]);
                    }
                    streams[i] = NIL;
                }
            }
            else
            {
                for (afxNat i = 0; i < cnt - cnt2; i++)
                {
                    avxCmdb cmdb = streams[cnt2 + i];
                    AfxAssertObjects(1, &cmdb, afxFcc_CMDB);

                    if (cmdb->beginCb(cmdb, usage)) AfxThrowError();
                    else
                    {
                        cmdb->state = avxCmdbState_RECORDING;

                        streams[cnt2] = cmdb;
                        ++cnt2;
                    }
                }
            }
        }

        AfxExitSlockExclusive(&din->pools[poolIdx].reqLock);
    }
    return err;
}
