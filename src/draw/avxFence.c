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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_FENCE_C
#include "avxIcd.h"

_AVX _avxDdiFenc const _AVX_FENC_DDI =
{

};

_AVX afxDrawSystem AvxGetFenceHost(avxFence fenc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    afxDrawSystem dsys = AfxGetHost(fenc);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxUnit64 AvxGetCompletedFenceValue(avxFence fenc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    // This method mimmicks the D3D12's one.
    afxUnit64 value = (afxUnit64)AfxLoadAtom64(&fenc->value);
    return value;
}

_AVX afxError AvxSignalFence(avxFence fenc, afxUnit64 value)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    
    // Also queued on D3D12
    // ID3D12Fence::Signal. Use this method to set a fence value from the CPU side.
    // Use ID3D12CommandQueue::Signal to set a fence from the GPU side.
    
    if (fenc->ddi->signalCb)
        err = fenc->ddi->signalCb(fenc, value);
    else
        AfxStoreAtom64(&fenc->value, (afxInt64)value);

    return err;
}

_AVX afxError AvxWaitForFence(avxFence fenc, afxUnit64 value, afxUnit64 timeout)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    
#if !0
    if (fenc->ddi->waitCb)
        err = fenc->ddi->waitCb(fenc, value, timeout);
#else
    if (!AfxLoadAtom64(&fenc->signaled))
        AvxWaitForFences(AvxGetFenceHost(fenc), timeout, TRUE, 1, &fenc, &value);
#endif

    return err;
}

_AVX afxError _AvxFencDtorCb(avxFence fenc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    
    return err;
}

_AVX afxError _AvxFencCtorCb(avxFence fenc, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxFenceInfo const* info = ((avxFenceInfo const*)args[1]) + invokeNo;

    fenc->waitQueuedCnt = 0;
    fenc->signalQueuedCnt = 0;
    fenc->signaled = info->initialVal;
    fenc->value = info->initialVal;
    fenc->flags = info->flags;

    fenc->tag = info->tag;
    fenc->udd = info->udd;

    fenc->ddi = &_AVX_FENC_DDI;

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_FENC =
{
    .fcc = afxFcc_FENC,
    .name = "Fence",
    .desc = "Synchronization Fence",
    .fixedSiz = sizeof(AFX_OBJECT(avxFence)),
    .ctor = (void*)_AvxFencCtorCb,
    .dtor = (void*)_AvxFencDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireFences(afxDrawSystem dsys, afxUnit cnt, avxFenceInfo const info[], avxFence fences[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(fences);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->fencCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_FENC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)fences, (void const*[]) { dsys, info }))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxDsysWaitForFencesCb_SW(afxDrawSystem dsys, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, avxFence const fences[], afxUnit64 const values[])
{
    afxError err = { 0 };
    AFX_ASSERT(fences);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxClock start, last;
    if (timeout && (timeout != AFX_TIMEOUT_INFINITE))
    {
        AfxGetClock(&start);
        last = start;
    }

    if (!waitAll || (cnt == 1))
    {
        while (1)
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                avxFence dfen = fences[i];
                if (!dfen) continue;
                AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &dfen);

                if (dfen->flags & avxFenceFlag_TIMELINE)
                {
                    AFX_ASSERT(values);
                    // vkWaitSemaphores waits until the semaphore value is greater than or equal to the specified value.
                    if (values[i] <= (afxUnit64)AfxLoadAtom64(&dfen->value))
                        return err;
                }
                else if (AfxLoadAtom32(&dfen->signaled))
                    return err;
            }

            if (timeout)
            {
                if ((timeout == AFX_TIMEOUT_INFINITE) || (timeout == AFX_TIMEOUT_INFINITE))
                    continue;

                afxClock curr;
                AfxGetClock(&curr);
                afxInt64 dt = AfxGetUltrasecondsElapsed(&last, &curr);
                last = curr;

                if (timeout < dt)
                {
                    err = afxError_TIMEOUT;
                    return err;
                }
            }
        }
    }
    else
    {
        for (afxUnit i = 0; i < cnt; i++)
        {
            avxFence dfen = fences[i];
            if (!dfen) continue;
            AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &dfen);

            while (1)
            {
                if (dfen->flags & avxFenceFlag_TIMELINE)
                {
                    AFX_ASSERT(values);
                    // vkWaitSemaphores waits until the semaphore value is greater than or equal to the specified value.
                    if (values[i] <= (afxUnit64)AfxLoadAtom64(&dfen->value))
                        break;
                }
                else if (AfxLoadAtom32(&dfen->signaled))
                    break;

                if (timeout)
                {
                    if ((timeout == AFX_TIMEOUT_INFINITE) || (timeout == AFX_TIMEOUT_INFINITE))
                        continue;

                    afxClock curr;
                    AfxGetClock(&curr);
                    afxInt64 dt = AfxGetUltrasecondsElapsed(&last, &curr);
                    last = curr;

                    if (timeout < dt)
                    {
                        err = afxError_TIMEOUT;
                        return err;
                    }
                }
            }
        }
    }

    return err;
}

_AVX afxError AvxWaitForFences(afxDrawSystem dsys, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, avxFence const fences[], afxUnit64 const values[])
{
    afxError err = { 0 };
    AFX_ASSERT(fences);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    if (_AvxDsysGetDdi(dsys)->waitFencCb(dsys, timeout, waitAll, cnt, fences, values))
        AfxThrowError();

    return err;
}

_AVX afxError AvxResetFences(afxDrawSystem dsys, afxUnit cnt, avxFence const fences[])
{
    afxError err = { 0 };
    AFX_ASSERT(fences);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    AfxThrowError();
#if 0
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxFence dfen = fences[i];
        if (!dfen) continue;
        AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &dfen);

        // Does the new synchronization primitive allow resetting its payload?
        // No, allowing the payload value to “go backwards” is problematic. 
        // Applications looking for reset behavior should create a new instance of the synchronization primitive instead.
        if (!(dfen->flags & avxFenceFlag_TIMELINE))
        {
            AfxStoreAtom64(&dfen->signaled, 0);
        }
    }
#endif
    //if (dsys->resetFencCb(dsys, cnt, fences))
        //AfxThrowError();

    return err;
}
