/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_CORE_C
#define _AFX_THREAD_POOL_C
#include "../exec/afxSystemDDK.h"

_AFX afxInt _AFX_THRP_PROC(afxThreadPool thrp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_THRP, 1, &thrp);
    afxThreadedTask task;

    for (;;)
    {
        AfxLockMutex(&thrp->lock);
        while ((thrp->cnt == 0) && (!thrp->shutdown))
            AfxWaitCondition(&thrp->notify, &thrp->lock);

        if ((thrp->shutdown == 1) ||
            ((thrp->shutdown == 2) &&
            (thrp->cnt == 0))) {
            break;
        }

        task.f = thrp->workQue[thrp->head].f;
        task.arg = thrp->workQue[thrp->head].arg;
        thrp->head = (thrp->head + 1) % thrp->workQueCap;
        thrp->cnt -= 1;

        AfxUnlockMutex(&thrp->lock);

        (*(task.f))(task.arg);
    }

    thrp->started--;

    AfxUnlockMutex(&thrp->lock);
    return 0;
}

_AFX afxInt AfxPushThreadedTask(afxThreadPool thrp, int flags, void(*f)(void*), void* arg)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_THRP, 1, &thrp);
    AFX_ASSERT(f);

    if (AfxLockMutex(&thrp->lock))
        AfxThrowError();

    afxInt next = (thrp->tail + 1) % thrp->workQueCap;

    do
    {
        if (thrp->cnt == thrp->workQueCap)
        {
            AfxThrowError();
            break;
        }

        if (thrp->shutdown)
        {
            AfxThrowError();
            break;
        }

        thrp->workQue[thrp->tail].f = f;
        thrp->workQue[thrp->tail].arg = arg;
        thrp->tail = next;
        thrp->cnt += 1;

        if (AfxSignalCondition(&thrp->notify))
        {
            AfxThrowError();
            break;
        }
    } while (0);

    if (AfxUnlockMutex(&thrp->lock))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxThrpDtorCb(afxThreadPool thrp)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_THRP, 1, &thrp);

    if (AfxLockMutex(&(thrp->lock)))
    {
        AfxThrowError();
        return err;
    }

    do
    {
        if (thrp->shutdown)
        {
            AfxThrowError();
            break;
        }

        thrp->shutdown = 2;

        if ((AfxSignalCondition2(&thrp->notify) != 0) || (AfxUnlockMutex(&thrp->lock)))
        {
            AfxThrowError();
            break;
        }

        for (afxInt i = 0; i < thrp->thrCnt; i++)
        {
            afxResult exitCode;
            if (!AfxWaitForThreadExit(thrp->threads[i], &exitCode))
            {
                AfxThrowError();
            }
        }
    } while (0);

    if (thrp->threads)
        AfxDeallocate((void**)&thrp->threads, AfxHere());

    if (thrp->workQue)
        AfxDeallocate((void**)&thrp->workQue, AfxHere());

    AfxLockMutex(&thrp->lock);
    AfxDismantleMutex(&thrp->lock);
    AfxDismantleCondition(&thrp->notify);

    return err;
}

_AFX afxError _AfxThrpCtorCb(afxThreadPool thrp, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_THRP, 1, &thrp);
    (void)invokeNo;

    //AfxZero(thr, sizeof(thr[0]));

    afxSize const* hint = args[1];
    afxThreadPoolInfo const *info = AFX_CAST(afxThreadPoolInfo const*, args[2]);// +invokeNo; --- not specific
    AFX_ASSERT(info);
    
    if (!info->thrCnt)
    {
        AfxThrowError();
        return err;
    }

    thrp->thrCnt = info->thrCnt;
    thrp->workQueCap = info->workQueCap;
    thrp->head = 0;
    thrp->tail;
    thrp->cnt = 0;
    thrp->shutdown = 0;
    thrp->started = 0;

    /* Allocate thread and task queue */

    if (AfxAllocate(thrp->thrCnt * sizeof(afxThread), 0, AfxHere(), (void**)&thrp->threads))
    {
        AfxThrowError();
        return err;
    }

    if (AfxAllocate(thrp->workQueCap * sizeof(afxThread), 0, AfxHere(), (void**)&thrp->workQue))
    {
        AfxThrowError();
        AfxDeallocate((void**)&thrp->threads, AfxHere());
        return err;
    }

    // Initialize mutex and conditional variable first

    if (AfxDeployMutex(&thrp->lock, 0))
    {
        AfxThrowError();
        AfxDeallocate((void**)&thrp->threads, AfxHere());
        AfxDeallocate((void**)&thrp->workQue, AfxHere());
        return err;
    }

    if (AfxDeployCondition(&thrp->notify))
    {
        AfxThrowError();
        AfxDeallocate((void**)&thrp->threads, AfxHere());
        AfxDeallocate((void**)&thrp->workQue, AfxHere());
        AfxDismantleMutex(&thrp->lock);
        return err;
    }

    afxThreadConfig tc = { 0 };
    tc.usage = info->usage;
    tc.flags = info->flags;

    if (AfxAcquireThreads(AfxHere(), &tc, thrp->thrCnt, thrp->threads))
    {
        AfxThrowError();
        AfxDeallocate((void**)&thrp->threads, AfxHere());
        AfxDeallocate((void**)&thrp->workQue, AfxHere());
        AfxDismantleMutex(&thrp->lock);
        AfxDismantleCondition(&thrp->notify);
        return err;
    }

    for (afxInt i = 0; i < thrp->thrCnt; i++)
    {
        if (AfxRunThread(thrp->threads[i], (void*)_AFX_THRP_PROC, thrp))
        {
            AfxThrowError();
        }
        else
        {
            thrp->started++;
        }

        if (err)
        {
            for (afxInt j = i; j-- > 0;)
            {
                afxResult exitCode;
                AfxWaitForThreadExit(thrp->threads[j], &exitCode);
            }
            AfxThrowError();
            AfxDeallocate((void**)&thrp->threads, AfxHere());
            AfxDeallocate((void**)&thrp->workQue, AfxHere());
            AfxDismantleMutex(&thrp->lock);
            AfxDismantleCondition(&thrp->notify);
        }
    }

    return err;
}

_AFX afxClassConfig const _AFX_THRP_CLASS_CONFIG =
{
    .fcc = afxFcc_THR,
    .name = "ThreadPool",
    .desc = "Thread Pool",
    .fixedSiz = sizeof(AFX_OBJECT(afxThreadPool)),
    .ctor = (void*)_AfxThrpCtorCb,
    .dtor = (void*)_AfxThrpDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxBuildThreadPool(afxHere const hint, afxThreadPoolInfo const* info, afxThreadPool* pool)
{
    afxError err = { 0 };
    AFX_ASSERT(hint);
    AFX_ASSERT(info);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass* cls = (afxClass*)_AfxSysGetThrpClass(sys);
    AFX_ASSERT_CLASS(cls, afxFcc_THRP);
    
    afxThreadPool thrp;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&thrp, (void const*[]) { sys, hint, (void*)info, }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_THRP, 1, &thrp);
        AFX_ASSERT(pool);
        *pool = thrp;
    }
    return err;
}
