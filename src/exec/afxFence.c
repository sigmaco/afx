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
#define _AFX_CONTEXT_C
#define _AFX_FENCE_C
#include "../dev/afxExecImplKit.h"

_AFX afxContext AfxGetFenceContext(afxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fenc, afxFcc_FENC);
    afxContext ctx = AfxGetProvider(fenc);
    //AfxAssertObjects(1, &ctx, afxFcc_CTX);
    AFX_ASSERT(ctx);
    return ctx;
}

_AFX afxDevice AfxGetFenceDevice(afxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fenc, afxFcc_FENC);
    afxContext ctx = AfxGetFenceContext(fenc);
    AFX_ASSERT(ctx);
    afxDevice dev = AfxGetProvider(ctx);
    //AfxAssertObjects(1, &ctx, afxFcc_CTX);
    AFX_ASSERT(dev);
    return dev;
}

_AFX afxBool AfxFenceIsSignaled(afxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fenc, afxFcc_FENC);
    return AfxLoadAtom32(&fenc->signaled);
}

_AFX afxError _AfxFencStdDtor(afxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fenc, afxFcc_FENC);
    
    return err;
}

_AFX afxError _AfxFencStdCtor(afxFence fenc, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fenc, afxFcc_FENC);

    afxContext ctx = args[0];
    afxBool signaled = *(afxBool const*)args[1];

    fenc->signaled = !!signaled;

    return err;
}

_AFX afxClassConfig const _AfxFencStdImplementation =
{
    .fcc = afxFcc_FENC,
    .name = "Fence",
    .desc = "Device Synchronization Fence",
    .fixedSiz = sizeof(AFX_OBJECT(afxFence)),
    .ctor = (void*)_AfxFencStdCtor,
    .dtor = (void*)_AfxFencStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxWaitForFences(afxBool waitAll, afxUnit64 timeout, afxUnit cnt, afxFence const fences[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, fences, afxFcc_FENC);
    afxContext ctx = AfxGetFenceContext(fences[0]);
    AFX_ASSERT(ctx);
    //AfxAssertObjects(1, &ctx, afxFcc_DCTX);

    if (ctx->waitFenc(waitAll, timeout, cnt, fences))
        AfxThrowError();

    return err;
}

_AFX afxError AfxResetFences(afxUnit cnt, afxFence const fences[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, fences, afxFcc_FENC);
    afxContext ctx = AfxGetFenceContext(fences[0]);
    AFX_ASSERT(ctx);
    //AfxAssertObjects(1, &ctx, afxFcc_DCTX);
    
    if (ctx->resetFenc(cnt, fences))
        AfxThrowError();

    return err;
}

_AFX afxError AfxAcquireFences(afxContext ctx, afxBool signaled, afxUnit cnt, afxFence fences[])
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertObjects(1, &ctx, afxFcc_DCTX);
    AFX_ASSERT(cnt);
    AFX_ASSERT(fences);

    afxClass* cls = AfxGetFenceClass(ctx);
    AfxAssertClass(cls, afxFcc_FENC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)fences, (void const*[]) { ctx, &signaled }))
        AfxThrowError();

    return err;
}
