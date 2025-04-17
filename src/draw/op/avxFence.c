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

#define _AVX_DRAW_C
#define _AVX_FENCE_C
#include "../../impl/afxExecImplKit.h"
#include "../impl/avxImplementation.h"

_AVX afxDrawSystem AvxGetFenceContext(avxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    afxDrawSystem dsys = AfxGetProvider(fenc);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxBool AvxIsFenceSignaled(avxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &fenc);
    return AfxLoadAtom32(&fenc->signaled);
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
    afxBool signaled = *(afxBool const*)args[1];

    fenc->signaled = !!signaled;

    return err;
}

_AVX afxClassConfig const _AVX_FENC_CLASS_CONFIG =
{
    .fcc = afxFcc_FENC,
    .name = "Fence",
    .desc = "Drawing Device Synchronization Fence",
    .fixedSiz = sizeof(AFX_OBJECT(avxFence)),
    .ctor = (void*)_AvxFencCtorCb,
    .dtor = (void*)_AvxFencDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireFences(afxDrawSystem dsys, afxBool signaled, afxUnit cnt, avxFence fences[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cnt);
    AFX_ASSERT(fences);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->fencCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_FENC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)fences, (void const*[]) { dsys, &signaled }))
        AfxThrowError();

    return err;
}

_AVX afxError AvxWaitForFences(afxDrawSystem dsys, afxBool waitAll, afxUnit64 timeout, afxUnit cnt, avxFence const fences[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, cnt, fences);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxClock start, last;
    if (timeout && (timeout != AFX_TIME_INFINITE))
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
                AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &dfen);

                if (dfen->signaled)
                    return err;

                if (AfxLoadAtom32(&dfen->signaled))
                    return err;
            }

            if (timeout)
            {
                if (timeout == AFX_TIME_INFINITE)
                    continue;

                afxClock curr;
                AfxGetClock(&curr);
                afxReal64 dt = AfxGetClockUltrasecondsElapsed(&last, &curr);
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
            AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &dfen);

            while (1)
            {
                if (dfen->signaled)
                    break;

                if (timeout)
                {
                    if (timeout == AFX_TIME_INFINITE)
                        continue;

                    afxClock curr;
                    AfxGetClock(&curr);
                    afxReal64 dt = AfxGetClockUltrasecondsElapsed(&last, &curr);
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

    //if (dsys->waitFenc(dsys, waitAll, timeout, cnt, fences))
        //AfxThrowError();

    return err;
}

_AVX afxError AvxResetFences(afxDrawSystem dsys, afxUnit cnt, avxFence const fences[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FENC, cnt, fences);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxFence dfen = fences[i];
        AFX_ASSERT_OBJECTS(afxFcc_FENC, 1, &dfen);
        dfen->signaled = FALSE;
    }

    //if (dsys->resetFenc(dsys, cnt, fences))
        //AfxThrowError();

    return err;
}
