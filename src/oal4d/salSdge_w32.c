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

#include "salSdev.h"

extern afxClassConfig _SalSiobMgrCfg;

_A4D void* _AfxDqueRequestArenaSpace(afxSoundBridge sdge, afxNat queIdx, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);

    afxSoundQueue sque = sdge->m.queues[queIdx];

    AfxEnterSlockExclusive(&sque->m.workArenaSlock);

    void *block = AfxAllocateArena(&sque->m.workArena, siz);

    if (!block)
        AfxThrowError();

    AfxExitSlockExclusive(&sque->m.workArenaSlock);

    return block;
}

_A4D void _AfxDqueRecycleArenaSpace(afxSoundBridge sdge, afxNat queIdx, void *block, afxNat siz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);

    afxSoundQueue sque = sdge->m.queues[queIdx];

    AfxEnterSlockExclusive(&sque->m.workArenaSlock);

    AfxAssert(block);

    AfxRecycleArena(&sque->m.workArena, block, siz);

    AfxExitSlockExclusive(&sque->m.workArenaSlock);
}

_A4D afxError _SalSdevProcPortUnlocked(salSpu *spu, afxSoundBridge sdge, afxNat queIdx)
{
    afxError err = AFX_ERR_NONE;
    afxBool ctxEntered = FALSE;
    afxNat portIdx = sdge->m.portIdx;
    afxSoundContext sctx = AfxGetSoundBridgeContext(sdge);

    if (!ctxEntered)
    {
        ALCcontext* alctx = spu->alc.GetCurrentContext();

        if (alctx == spu->alctx) ctxEntered = TRUE;
        else
        {
            if (!(ctxEntered = !!spu->alc.MakeContextCurrent(spu->alctx)))
            {
                AfxThrowError();
                AfxLogError("DPU %u failed to take active context.", portIdx);
            }
        }

        if (ctxEntered)
            spu->activeSctx = sctx;
    }

    if (ctxEntered)
    {
        afxSoundQueue sque = sdge->m.queues[queIdx];

        _salQueueing* subm;
        AfxChainForEveryLinkageB2F(&sque->m.workChn, _salQueueing, chain, subm)
        {
            AfxGetTime(&subm->pullTime);

            subm->exec(spu, sdge, queIdx, subm);

            AfxGetTime(&subm->complTime);

            AfxPopLinkage(&subm->chain);

            //AfxEnterSlockExclusive(&sctxD->ports[sdge->portIdx].recyclSubmChainSlock);

            //if (sctxD->ports[sdge->portIdx].minRecyclSubmCnt > AfxGetChainLength(&sctxD->ports[sdge->portIdx].recyclSubmChain))
            {
                //AfxPushLinkage(&subm->chain, &sctxD->ports[sdge->portIdx].recyclSubmChain);
            }
            //else
            {
                _AfxDqueRecycleArenaSpace(sdge, queIdx, subm, subm->siz);
            }
            //AfxExitSlockExclusive(&sctxD->ports[sdge->portIdx].recyclSubmChainSlock);
        }
        //AfxSignalCondition2(&sque->idleCnd);
    }
    return err;
}

_A4D afxBool _SdgeProcCb(afxSoundBridge sdge, afxThread thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxSoundDevice sdev = AfxGetObjectProvider(sdge);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    afxNat portIdx = sdge->m.portIdx;
    AfxAssertRange(sdev->idd->spuCnt, portIdx, 1);
    salSpu *spu = &sdev->idd->spus[portIdx];
        
    AfxAssert(portIdx == sdge->m.portIdx);

    for (afxNat queIdx = 0; queIdx < sdge->m.queCnt; queIdx++)
    {
        afxSoundQueue sque = sdge->m.queues[queIdx];

        if (AfxTryLockMutex(&sque->m.workChnMtx))
        {
            _SalSdevProcPortUnlocked(spu, sdge, queIdx);
            AfxUnlockMutex(&sque->m.workChnMtx);
            AfxSignalCondition2(&sque->m.idleCnd);
        }
    }
    return TRUE;
}

_A4D afxNat _SalSdgeEnqueueTransferCb(afxSoundBridge sdge, afxFence fenc, afxNat cnt, afxTransferRequest2 const req[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    AfxAssert(cnt);
    AfxAssert(req);
    afxNat queIdx = AFX_INVALID_INDEX;
    afxBool queued = FALSE;

    do
    {
        for (afxNat i = 0; i < sdge->m.queCnt; i++)
        {
            afxSoundQueue sque = sdge->m.queues[i];

            if (AfxTryLockMutex(&sque->m.workChnMtx))
            {
                _salQueueingTransfer* subm;
                afxNat queuingSiz = sizeof(*subm) + (cnt * sizeof(subm->items[0]));
                subm = _AfxDqueRequestArenaSpace(sdge, i, queuingSiz);
                *subm = (_salQueueingTransfer) { 0 };
                subm->hdr.siz = queuingSiz;
                AfxGetTime(&subm->hdr.pushTime);
                subm->hdr.exec = (void*)NIL;// _SpuTransfer;
                subm->itemCnt = cnt;

                for (afxNat i = 0; i < cnt; i++)
                {
                    subm->items[i] = req[i];
                }

                AfxPushLinkage(&subm->hdr.chain, &sque->m.workChn);
                AfxUnlockMutex(&sque->m.workChnMtx);
                queued = TRUE;
                queIdx = i;
                break;
            }
        }

        if (queued) break;
        else AfxYield();

    } while (1);

    return queIdx;
}

_A4D afxNat _SalSdgeEnqueueMmapCb(afxSoundBridge sdge, afxSoundBuffer buf, afxSize off, afxNat ran, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);
    afxNat queIdx = AFX_INVALID_INDEX;
    afxBool queued = FALSE;

    do
    {
        for (afxNat i = 0; i < sdge->m.queCnt; i++)
        {
            afxSoundQueue sque = sdge->m.queues[i];

            if (AfxTryLockMutex(&sque->m.workChnMtx))
            {
                _salQueueingMmap* subm;
                afxNat queuingSiz = sizeof(*subm);
                subm = _AfxDqueRequestArenaSpace(sdge, i, queuingSiz);
                *subm = (_salQueueingMmap) { 0 };
                subm->hdr.siz = queuingSiz;
                subm->hdr.exec = (void*)NIL;// _SpuMmap;
                AfxGetTime(&subm->hdr.pushTime);

                subm->buf = buf;
                subm->off = off;
                subm->ran = ran;
                subm->flags = flags;

                AfxPushLinkage(&subm->hdr.chain, &sque->m.workChn);
                AfxUnlockMutex(&sque->m.workChnMtx);
                queued = TRUE;
                queIdx = i;
                break;
            }
        }

        if (queued) break;
        else AfxYield();

    } while (1);

    return queIdx;
}

_A4D afxError _SalSdgeDtor(afxSoundBridge sdge)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);

    if (_AsxSdgeStdImplementation.dtor(sdge))
        AfxThrowError();

    return err;
}

_A4D afxError _SalSdgeCtorCb(afxSoundBridge sdge, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdge, afxFcc_SDGE);

    if (_AsxSdgeStdImplementation.ctor(sdge, cookie)) AfxThrowError();
    else
    {
        sdge->m.executeCb = NIL;// _SalSdgeEnqueueExecuteCb;
        sdge->m.transferCb = _SalSdgeEnqueueTransferCb;

        if (err && _AsxSdgeStdImplementation.dtor(sdge))
            AfxThrowError();
    }
    return err;
}
