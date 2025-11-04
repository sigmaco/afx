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
#include "../impl/afxExecImplKit.h"
#include "ddi/avxImplementation.h"

_AVX afxDrawSystem AvxGetFenceHost(avxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    afxDrawSystem dsys = AfxGetHost(fenc);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxBool AvxCheckForFenceSignaled(avxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    return AfxLoadAtom32(&fenc->signaled);
}

_AVX afxError AvxGetTimelineFenceCounter(avxFence fenc, afxUnit64* value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    AFX_ASSERT(fenc->flags & avxFenceFlag_TIMELINE);
    AFX_ASSERT(value);
    *value = (afxUnit64)AfxLoadAtom64(&fenc->value);
    return err;
}

_AVX afxError AvxSignalTimelineFence(avxFence fenc, afxUnit64 value)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    AFX_ASSERT(fenc->flags & avxFenceFlag_TIMELINE);
    AfxStoreAtom64(&fenc->value, (afxInt64)value);
    return err;
}

_AVX afxError _AvxFencDtorCb(avxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    
    return err;
}

_AVX afxError _AvxFencCtorCb(avxFence fenc, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxFenceInfo const* info = ((avxFenceInfo const*)args[1]) + invokeNo;

    fenc->signaled = info->initialVal;
    fenc->value = info->initialVal;
    fenc->flags = info->flags;

    fenc->tag = info->tag;
    fenc->udd = info->udd;

    return err;
}

_AVX afxClassConfig const _AVX_FENC_CLASS_CONFIG =
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
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(fences);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->fencCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_FENC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)fences, (void const*[]) { dsys, info }))
        AfxThrowError();

    return err;
}

_AVX afxError AvxWaitForFences(afxDrawSystem dsys, afxUnit64 timeout, afxBool waitAll, afxUnit cnt, avxFence const fences[], afxUnit64 const values[])
{
    afxError err = AFX_ERR_NONE;
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

    //if (dsys->waitFencCb(dsys, waitAll, timeout, cnt, fences))
        //AfxThrowError();

    return err;
}

_AVX afxError AvxResetFences(afxDrawSystem dsys, afxUnit cnt, avxFence const fences[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(fences);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    
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

    //if (dsys->resetFencCb(dsys, cnt, fences))
        //AfxThrowError();

    return err;
}
