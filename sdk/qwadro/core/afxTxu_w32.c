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
 
#include "../dep/tinycthread/tinycthread.h"

#define _AFX_CORE_C
#define _AFX_THREAD_C
#define _AFX_TXU_C
#define _AFX_SYSTEM_C
#include "qwadro/core/afxSystem.h"

_AFX void AfxGetThreadingId(afxNat32 *tid)
{
#if defined(_WIN32)
    afxError err = NIL;
    AfxAssert(tid);
    *tid = GetCurrentThreadId();
#else
#error "";
#endif
}

_AFX void AfxYieldThreading(void)
{
    thrd_yield();
}

typedef struct _afxThrParadigm
{
    void(*start)(afxThread thr, void *udd);
    void *udd;
    void(*exec)(afxThread thr);
} _afxThrParadigm;

struct _txuFindThreadParam
{
    afxNat32    tid;
    afxThread*  thr;
};

struct _txuFindUnitParam
{
    afxNat32    tid;
    afxNat*     unitIdx;
};

_AFXINL afxBool _TxuFindThreadCb(afxTxu txu, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txu, afxFcc_TXU);
    struct _txuFindThreadParam* param = udd;

    if (param->tid == txu->tid)
    {
        afxThread thr2 = txu->activeThr;
        AfxAssertObjects(1, &thr2, afxFcc_THR);
        *param->thr = thr2;
        return TRUE; // interrupt
    }
    return FALSE; // continue
}

_AFX void AfxGetThread(afxThread* thr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(thr);
    *thr = NIL;
    afxNat32 tid;
    AfxGetThreadingId(&tid);
    struct _txuFindThreadParam param = { .thr = thr, .tid = tid};
    AfxInvokeTxus(0, AFX_N32_MAX, _TxuFindThreadCb, &param);
}

_AFXINL afxBool _TxuFindUnitCb(afxTxu txu, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txu, afxFcc_TXU);
    struct _txuFindUnitParam* param = udd;

    if (param->tid == txu->tid)
    {
        *param->unitIdx = AfxIdentifyObject(txu);
        return TRUE; // interrupt
    }
    return FALSE; // continue
}

_AFX void AfxGetThreadingUnit(afxNat* txuIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(txuIdx);
    *txuIdx = AFX_INVALID_INDEX;
    afxNat32 tid;
    AfxGetThreadingId(&tid);
    struct _txuFindUnitParam param = { .unitIdx = txuIdx, .tid = tid };
    AfxInvokeTxus(0, AFX_N32_MAX, _TxuFindUnitCb, &param);
}

_AFX void AfxGetThreadCounter(afxNat* iterNo, afxNat* lastFreq)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(iterNo || lastFreq);

    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    if (iterNo)
        *iterNo = thr->iterNo;

    if (lastFreq)
        *lastFreq = thr->lastIterCnt;
}

_AFX void AfxQueryThreadTime(afxReal64* ct, afxReal64* dt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ct || dt);

    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxClock currClock;
    AfxGetClock(&currClock);

    if (ct)
    {
        afxReal64 ct2 = AfxGetSecondsElapsed(&thr->startClock, &currClock);
        *ct = ct2;
    }

    if (dt)
    {
        afxReal64 dt2 = AfxGetSecondsElapsed(&thr->lastClock, &currClock);
        *dt = dt2;
    }
    thr->lastClock = currClock;
}

_AFX void AfxGetThreadClock(afxClock* curr, afxClock* last)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(curr || last);

    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxClock currClock;
    AfxGetClock(&currClock);

    if (curr)
        *curr = currClock;

    if (last)
        *last = thr->lastClock;

    thr->lastClock = currClock;
}

_AFX void AfxExitThread(afxInt code)
{
    afxError err = AFX_ERR_NONE;
    
    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    // Tells the thread's event loop to exit with a return code.
    // After calling this function, the thread leaves the event loop and returns from the call to exec(). The exec() function returns returnCode.
    // By convention, a returnCode of 0 means success, any non - zero value indicates an error.
    // Note that unlike the C library function of the same name, this function does return to the caller -- it is event processing that stops.
    // No EventLoops will be started anymore in this thread until exec() has been called again.If the eventloop in exec() is not running then the next call to exec() will also return immediately.

    thr->exitCode = code;
    thr->exited = TRUE;

    thr->running = FALSE;
}

_AFX void AfxQuitThread(void)
{
    // Tells the thread's event loop to exit with return code 0 (success). Equivalent to calling exit(0).
    // This function does nothing if the thread does not have an event loop.
    AfxExitThread(0);
}

_AFX afxBool AfxThreadShouldBeInterrupted(void)
{
    afxError err = AFX_ERR_NONE;

    afxThread thr;
    AfxGetThread(&thr);
    AfxAssertObjects(1, &thr, afxFcc_THR);

    afxBool rslt = FALSE;

    if (!(rslt = !!(thr->interruptionRequested)))
    {
        rslt = (thr->running && (!thr->finished) && (!thr->isInFinish));
    }
    return rslt;
}

_AFX afxBool _AfxThrExecuteCb(afxThread thr, void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    afxTxu txu = udd;
    AfxAssertObjects(1, &txu, afxFcc_TXU);
    afxNat txuIdx = txu->unitIdx;
    
    if (thr->baseTxu >= txuIdx && txuIdx < thr->txuCnt - thr->baseTxu)
    {
        afxThreadOpcode thrOpcode = NIL;

        if (AfxTryEnterSlockExclusive(&thr->txuSlock))
        {
            afxThread backedUpThr;

            if (thr != (backedUpThr = txu->activeThr)) // do not allow reentrancy on same thread
            {
                txu->activeThr = thr;

                {
                    if (thr->running && !thr->started && !thr->finished && AfxSystemIsExecuting())
                    {
                        thrOpcode = AFX_THR_OPCODE_RUN;
                        thr->started = TRUE;
                    }

                    if (thr->running)
                    {
                        afxClock currClock;
                        AfxGetClock(&currClock);
                        afxReal64 ct = AfxGetSecondsElapsed(&txu->startClock, &currClock);
                        afxReal64 dt = AfxGetSecondsElapsed(&txu->lastClock, &currClock);
                        txu->lastClock = currClock;

                        if (1 < AfxGetSecondsElapsed(&thr->iterCntSwapClock, &currClock))
                        {
                            thr->iterCntSwapClock = currClock;
                            thr->lastIterCnt = thr->iterNo;
                            thr->iterNo = 0;
                        }
                        else
                        {
                            ++thr->iterNo;
                        }
                        
#if 0
                        if ((ev = AfxPullNextQueueUnit(&thr->events)))
                        {

                            AfxPopNextQueue(&thr->events);
                        }
#endif

                        if (!AfxSystemIsExecuting())
                            AfxRequestThreadInterruption(thr);

                        if (thr->interruptionRequested)
                        {
                            thrOpcode = AFX_THR_OPCODE_QUIT;
                            thr->running = FALSE;
                        }

                        //afxInt exitCode = 0;
                        afxError rslt = 0;

                        if (thr->proc && (rslt = thr->proc(thr, thr->udd, thrOpcode)))
                        {
                            AfxThrowError();
                            AfxExitThread(-1);
                        }

                        if (thrOpcode == AFX_THR_OPCODE_QUIT || !AfxSystemIsExecuting())
                        {
                            AfxQuitThread();
                        }

                    }
                }

                txu->activeThr = backedUpThr;
            }
            AfxExitSlockExclusive(&thr->txuSlock);
        }
    }
    return FALSE; // dont interrupt curation
}

_AFX afxResult _AfxDoSystemThreading(afxSystem sys, afxTime timeout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    (void)timeout;
    afxResult code = 0;

    afxNat txuIdx = AFX_INVALID_INDEX;
    AfxGetThreadingUnit(&txuIdx);
    
    if (txuIdx == AFX_INVALID_INDEX) AfxThrowError();
    else
    {
        afxTxu txu;
        AfxEnumerateTxus(txuIdx, 1, &txu);
        AfxAssertObjects(1, &txu, afxFcc_TXU);

        afxClock currClock;
        AfxGetClock(&currClock);
        afxReal64 ct = AfxGetSecondsElapsed(&txu->startClock, &currClock);
        afxReal64 dt = AfxGetSecondsElapsed(&txu->lastClock, &currClock);
        txu->lastClock = currClock;

        if (1.0 < AfxGetSecondsElapsed(&txu->iterCntSwapClock, &currClock))
        {
            txu->iterCntSwapClock = currClock;
            txu->lastIterCnt = txu->iterNo;
            txu->iterNo = 0;
        }
        else
        {
            ++txu->iterNo;
        }

        AfxInvokeThreads(0, AFX_N32_MAX, _AfxThrExecuteCb, txu);

        if (sys->isInShutdown)
            code = sys->exitCode;
    }
    
    return code;
}

_AFX int __stdcall startCbOnSysProcUnit(afxTxu txu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txu, afxFcc_TXU);

    txu->tid = GetCurrentThreadId();
    txu->osHandle = thrd_current();
    AfxGetClock(&txu->startClock);
    
    AfxYieldThreading();

    AfxDbgLogf(6, NIL, "Starting Thread Execution Unit %u", txu->unitIdx);
    
    while (AfxSystemIsExecuting())
    {
        AfxDoSystemExecution(0);
    }

    AfxDbgLogf(6, NIL, "Stopping Thread Execution Unit %u", txu->unitIdx);
    
    txu->activeThr = NIL;
    txu->osHandle = NIL;
    txu->tid = 0;

    return 0;
}

_AFX afxError _AfxTxuCtor(afxTxu txu, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txu, afxFcc_TXU);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    txu->unitIdx = AfxIdentifyObject(txu);
    txu->activeThr = NIL;

    AfxSetUpChain(&txu->threads, txu);

    txu->QPC_InitialVal = (LARGE_INTEGER) { 0 };
    txu->LastMicroseconds = 0;
    txu->LastTickCount = 0;
    txu->CurrentAdjustment = 0;
    txu->QPC_Frequency = (LARGE_INTEGER) { 0 };

    AfxGetClock(&txu->startClock);
    txu->lastClock = txu->startClock;
    txu->iterCntSwapClock = txu->lastClock;
    txu->iterNo = 0;

    //AfxSetUpChain(&procUnit->threads, sys);

    afxNat tid = 0;
    void* osHandle = 0;

    if (txu->unitIdx == 0) // the first
    {
        AfxGetThreadingId(&tid);
        osHandle = (void*)OpenThread(THREAD_QUERY_INFORMATION, NIL, tid);
        AfxGetClock(&txu->startClock);
    }
    else
    {
        thrd_t thrd = NIL;

        if (thrd_success != thrd_create(&thrd, (void*)startCbOnSysProcUnit, txu)) AfxThrowError();
        else
        {
            //AfxYieldThreading();
            //AfxAssert(osHandle == procUnit->osHandle); // GetCurrentThread returns pseudo-handle
            //AfxAssert(tid == procUnit->tid);
        }
        txu->osHandle = thrd;
    }

    txu->tid = tid;
    txu->osHandle = osHandle;

    return err;
}

_AFX afxError _AfxTxuDtor(afxTxu txu)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txu, afxFcc_TXU);

    AfxAssert(AfxChainIsEmpty(&txu->threads));
    return err;
}

_AFX afxClassConfig const _AfxTxuClsConfig =
{
    .fcc = afxFcc_TXU,
    .name = "TXU",
    .desc = "Thread Execution Unit",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxTxu)),
    .mmu = NIL,
    .ctor = (void*)_AfxTxuCtor,
    .dtor = (void*)_AfxTxuDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError _AfxAcquireTxus(afxNat cnt, afxThread threads[])
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetTxuClass();
    AfxAssertClass(cls, afxFcc_TXU);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxThread thr = threads[i];
        AfxAssertObjects(1, &thr, afxFcc_THR);

        afxNat baseTxu = thr->baseTxu;
        afxNat txuCnt = thr->txuCnt;
        
        for (afxNat j = 0; j < txuCnt; j++)
        {
            afxTxu txu;

            if ((txu = AfxGetObjectAt(cls, baseTxu + j)))
            {
                AfxReacquireObjects(1, (void*[]) { txu });
            }
            else
            {
                if (_AfxAllocateObjectsAt(cls, baseTxu + j, 1, (void*[]) { txu })) AfxThrowError();
                else
                {
                    if (_AfxConstructObjects(cls, 1, (void*[]) { txu }, NIL))
                    {
                        AfxThrowError();
                        _AfxDeallocateObjects(cls, 1, (void*[]) { txu });
                    }
                }
            }

            if (!err)
            {
                afxThreadLinkage* thrk = AfxAllocate(1, sizeof(*thrk), 0, AfxHint());

                if (!thrk) AfxThrowError();
                else
                {
                    AfxPushLinkage(&thrk->thr, &thr->txus);
                    AfxPushLinkage(&thrk->txu, &txu->threads);
                }
            }

            if (err)
            {
                for (afxNat k = j; k-- > 0;)
                {
                    txu = (void*)AfxGetObjectAt(cls, k);
                    AfxTryAssertObjects(1, (void*[]) { txu }, afxFcc_TXU);
                    AfxReleaseObjects(1, (void*[]) { txu });
                }
                break;
            }
        }
    }
    return err;
}

_AFX afxError _AfxReleaseTxus(afxNat cnt, afxThread threads[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat i = cnt; i-- > 0;)
    {
        afxThread thr = threads[i];
        afxThreadLinkage* thrk;
        AfxIterateLinkage(afxThreadLinkage, thrk, &thr->txus, thr)
        {
            AfxPopLinkage(&thrk->thr);
            afxTxu txu = AfxGetLinker(&thrk->txu);
            AfxTryAssertObjects(1, (void*[]) { txu }, afxFcc_TXU);
            AfxPopLinkage(&thrk->txu);
            AfxDeallocate(thrk);
            AfxReleaseObjects(1, (void*[]) { txu });
        }
    }
    return err;
}

_AFX afxNat AfxInvokeTxus(afxNat first, afxNat cnt, afxBool(*f)(afxTxu, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetTxuClass();
    AfxAssertClass(cls, afxFcc_TXU);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateTxus(afxNat first, afxNat cnt, afxTxu txus[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(txus);
    afxManager* cls = AfxGetTxuClass();
    AfxAssertClass(cls, afxFcc_TXU);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)txus);
}

_AFX afxNat AfxCountTxus(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetTxuClass();
    AfxAssertClass(cls, afxFcc_TXU);
    return AfxCountObjects(cls);
}
