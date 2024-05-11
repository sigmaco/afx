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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_INPUT_C
#include "qwadro/draw/afxDrawSystem.h"

extern afxClassConfig const _AvxVbufMgrCfg;
//AFX afxClassConfig const _AvxIbufMgrCfg;

_AVX void* AfxGetDrawInputUdd(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return din->udd;
}

_AVX afxDrawDevice AfxGetDrawInputDevice(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawDevice ddev = AfxGetLinker(&din->ddev);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxManager* AfxGetVertexBufferClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxManager *cls = &din->vbuffers;
    AfxAssertClass(cls, afxFcc_VBUF);
    return cls;
}

_AVX afxManager* AfxGetIndexBufferClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxManager *cls = &din->ibuffers;
    AfxAssertClass(cls, afxFcc_IBUF);
    return cls;
}

_AVX afxBool AfxGetDrawInputContext(afxDrawInput din, afxDrawContext* context)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawContext dctx = AfxGetLinker(&din->dctx);
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(context);
    *context = dctx;
    return !!dctx;
}

_AVX afxBool AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawDevice ddev = AfxGetDrawInputDevice(din);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    AfxLockMutex(&ddev->relinkedCndMtx);

    while (ddev->relinking)
        AfxWaitCondition(&ddev->relinkedCnd, &ddev->relinkedCndMtx);

    afxDrawContext from;
    AfxGetDrawInputContext(din, &from);

    if (dctx != from)
    {
        ddev->relinking = TRUE;
        din->reconnecting = TRUE;

        if (dctx)
        {
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            afxDrawDevice ddrv2 = AfxGetDrawContextDevice(dctx);
            AfxAssertObjects(1, &ddrv2, afxFcc_DDEV);

            if (ddev != ddrv2) // can not connect to context acquired of another device
            {
                AfxThrowError();
                dctx = NIL; // so just disconnect
            }
        }

        if (!ddev->dinRelinkCb)
        {
            if (from)
            {
                afxDrawInput din2;
                AfxIterateLinkageB2F(AFX_OBJECT(afxDrawInput), din2, &from->inputs, dctx)
                {
                    AfxAssertObjects(1, &din2, afxFcc_DIN);

                    if (din2 == din)
                    {
                        AfxPopLinkage(&din->dctx);
                        AfxExhaustChainedManagers(&din->classes);
                        AfxReleaseObjects(1, &from);
                        break;
                    }
                }
            }

            if (dctx)
            {
                AfxReacquireObjects(1, &dctx);
                AfxPushLinkage(&din->dctx, &dctx->inputs);

                din->cachedClipCfg = dctx->clipCfg;
            }
        }
        else if (ddev->dinRelinkCb(ddev, dctx, 1, &din))
            AfxThrowError();

        din->reconnecting = FALSE;
        ddev->relinking = FALSE;
    }

    AfxUnlockMutex(&ddev->relinkedCndMtx);
    AfxSignalCondition(&ddev->relinkedCnd);
    return !err;
}

_AVX afxError AfxDisconnectDrawInput(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!AfxReconnectDrawInput(din, NIL))
        AfxThrowError();

    AfxAssert(!AfxGetLinker(&din->dctx));
    return err;
}

_AVX void AfxDescribeClipSpace(afxDrawInput din, afxClipSpace* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(clip);
    *clip = din->cachedClipCfg;
}

_AVX void AfxComputeLookToMatrices(afxDrawInput din, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(dir);
    AfxAssert(!AfxV3dIsZero(dir));
    AfxAssert(!AfxV3dIsInfinite(dir));    
    AfxAssert(v);
    AfxComputeLookToMatrix(v, eye, dir, AFX_V4D_Y, &din->cachedClipCfg);

    if (iv)
        AfxInvertM4d(v, iv);
}

_AVX void AfxComputeLookAtMatrices(afxDrawInput din, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(target);
    AfxAssert(eye);
    AfxAssert(v);
    AfxComputeLookAtMatrix(v, eye, target, AFX_V4D_Y, &din->cachedClipCfg);

    if (iv)
        AfxInvertM4d(v, iv);
}

_AVX void AfxComputeOrthographicMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(p);
    AfxComputeOrthographicMatrix(p, extent, near, far, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX void AfxComputeOffcenterOrthographicMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(p);
    AfxComputeOffcenterOrthographicMatrix(p, left, right, bottom, top, near, far, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX void AfxComputeBoundingOrthographicMatrices(afxDrawInput din, afxAabb const aabb, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(aabb); // afxAabb deve estar no view space.
    AfxAssert(p);
    AfxComputeBoundingOrthographicMatrix(p, aabb, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX void AfxComputeBasicOrthographicMatrices(afxDrawInput din, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(aspectRatio); // w/h
    AfxAssert(p);
    AfxComputeBasicOrthographicMatrix(p, aspectRatio, scale, range, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX void AfxComputePerspectiveMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(extent);
    AfxAssert(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(near > 0.f && far > 0.f);
    AfxAssert(p);
    AfxComputePerspectiveMatrix(p, extent, near, far, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX void AfxComputeFovMatrices(afxDrawInput din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AfxAssert(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(near > 0.f && far > 0.f);
    AfxAssert(p);
    AfxComputeFovPerspectiveMatrix(p, fovY, aspectRatio, near, far, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX void AfxComputeFrustrumMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(near > 0.f && far > 0.f);
    AfxAssert(p);
    AfxComputeOffcenterPerspectiveMatrix(p, left, right, bottom, top, near, far, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX void AfxComputeBasicPerspectiveMatrices(afxDrawInput din, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AfxAssert(p);
    AfxComputeBasicPerspectiveMatrix(p, aspectRatio, range, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX afxNat AfxExecuteDrawStreams(afxDrawInput din, afxNat cnt, afxExecutionRequest const req[], afxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(req);

    /*
        If any command buffer submitted to this queue is in the executable state, it is moved to the pending state. 
        Once execution of all submissions of a command buffer complete, it moves from the pending state, back to the executable state. 
        If a command buffer was recorded with the VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT flag, it instead moves back to the invalid state.
    */

    afxNat queIdx = AFX_INVALID_INDEX;
    afxDrawContext dctx;

    if (!AfxGetDrawInputContext(din, &dctx)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxDrawBridge ddge = AfxGetDrawBridge(dctx, 0);
        AfxAssertObjects(1, &ddge, afxFcc_DDGE);

        if (AFX_INVALID_INDEX == (queIdx = AfxEnqueueExecutionRequest(ddge, fenc, 1, req)))
            AfxThrowError();
    }
    return queIdx;
}

_AVX afxError AfxUplinkTxds(afxDrawInput din, afxNat baseSlot, afxNat slotCnt, afxUri const uris[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssertRange(8, baseSlot, slotCnt);
    
    if (!uris)
    {
        for (afxNat i = 0; i < slotCnt; i++)
        {
            AfxClearUri(&din->txdUris[i].uri);
        }
    }
    else
    {
        for (afxNat i = 0; i < slotCnt; i++)
        {
            AfxCopyUri(&din->txdUris[i].uri, &uris[i]);
        }
    }
    return err;
}

_AVX afxError _AvxDinDtor(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    AfxDisconnectDrawInput(din);

    afxDrawDevice ddev = AfxGetDrawInputDevice(din);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    if (ddev->dinIddDtorCb && ddev->dinIddDtorCb(ddev, din))
        AfxThrowError();

    AfxAssert(!din->idd);

    AfxCleanUpChainedManagers(&din->classes);

    return err;
}

_AVX afxError _AvxDinCtor(afxDrawInput din, afxCookie const *cookie)
{
    //AfxEntry("din=%p,uri=%.*s", din, endpoint ? AfxPushString(AfxGetUriString(endpoint)) : &AFX_STR_EMPTY);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    AfxZero(din, sizeof(din[0]));

    afxDrawDevice ddev = cookie->udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxUri const* endpoint = cookie->udd[1];
    afxDrawInputConfig const* cfg = ((afxDrawInputConfig const *)cookie->udd[2]) + cookie->no;
    
    AfxPushLinkage(&din->ddev, &ddev->inputs);
    AfxPushLinkage(&din->dctx, NIL);

    afxChain *classes = &din->classes;
    AfxSetUpChain(classes, (void*)din);

    AfxAllocateArray(&din->scripts, 32, sizeof(afxDrawStream), NIL);
    din->minScriptReserve = 2;

    afxNat defStreamSiz = 32000000; // 32MB (I think it is the total of RAM in PlayStation 2)

    din->minIdxBufSiz = defStreamSiz / 4;
    din->maxIdxBufSiz = 0;
    din->minVtxBufSiz = defStreamSiz - din->minIdxBufSiz;
    din->maxVtxBufSiz = 0;

    din->udd = cfg && cfg->udd ? cfg->udd : NIL;

    din->procCb = cfg && cfg->proc ? cfg->proc : NIL;

    afxClassConfig tmpClsConf;

    tmpClsConf = _AvxVbufMgrCfg;
    AfxEstablishManager(&din->vbuffers, NIL, classes, &tmpClsConf);

    din->cachedClipCfg = ddev->clipCfg;

    din->idd = NIL;

    if (ddev->dinIddCtorCb && ddev->dinIddCtorCb(ddev, din, cfg, endpoint)) AfxThrowError();
    else
    {
        
    }
    return err;
}

_AVX afxClassConfig const _AvxDinMgrCfg =
{
    .fcc = afxFcc_DIN,
    .name = "DrawInput",
    .desc = "Draw Input Mechanism",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxDrawInput)),
    .mmu = NIL,
    .ctor = (void*)_AvxDinCtor,
    .dtor = (void*)_AvxDinDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxOpenDrawInput(afxNat ddevId, afxUri const* endpoint, afxDrawInputConfig const* cfg, afxDrawInput* input)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxManager* cls = AfxGetDrawInputClass();
        AfxAssertClass(cls, afxFcc_DIN);
        afxDrawInput din;

        if (AfxAcquireObjects(cls, 1, (afxObject*)&din, (void const*[]) { ddev, endpoint, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &din, afxFcc_DIN);
            AfxAssert(input);
            *input = din;
        }
    }
    return err;
}

_AVX afxError AfxAcquireDrawInput(afxNat ddevId, afxDrawInputConfig const* cfg, afxDrawInput* input)
{
    afxError err = AFX_ERR_NONE;
    afxDrawInput din = NIL;

    if (AfxOpenDrawInput(ddevId, NIL, cfg, &din)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &din, afxFcc_DIN);
        AfxAssert(input);
        *input = din;
    }
    return err;
}

_AVX afxNat AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetDrawInputClass();
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(inputs);
    AfxAssert(cnt);
    afxManager* cls = AfxGetDrawInputClass();
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)inputs);
}

_AVX afxNat AfxCountDrawInputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager*cls = AfxGetDrawInputClass();
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxCountObjects(cls);
}
