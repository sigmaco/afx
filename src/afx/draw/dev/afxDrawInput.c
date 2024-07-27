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
#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_INPUT_C
#include "AvxDevKit.h"

extern afxClassConfig const _AvxTxdClsCfg;
extern afxClassConfig const _AvxCamClsCfg;
extern afxClassConfig const _AvxDtecClsCfg;
extern afxClassConfig const _AvxVbufMgrCfg;
//AFX afxClassConfig const _AvxIbufMgrCfg;

_AVX void* AfxGetDrawInputUdd(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return din->udd;
}

_AVX afxClass const* AfxGetTxdClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    /// din must be a valid afxDrawInput handle.
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = &din->txdCls;
    AfxAssertClass(cls, afxFcc_TXD);
    return cls;
}

_AVX afxClass const* AfxGetCameraClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = &din->camCls;
    AfxAssertClass(cls, afxFcc_CAM);
    return cls;
}

_AVX afxClass const* AfxGetDrawTechniqueClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = &din->dtecCls;
    AfxAssertClass(cls, afxFcc_DTEC);
    return cls;
}

_AVX afxDrawDevice AfxGetDrawInputDevice(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawDevice ddev = AfxGetLinker(&din->ddev);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxClass* AfxGetVertexBufferClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass *cls = &din->vbuffers;
    AfxAssertClass(cls, afxFcc_VBUF);
    return cls;
}

_AVX afxClass* AfxGetIndexBufferClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass *cls = &din->ibuffers;
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

    afxDrawContext from = NIL;
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

        if (from)
        {
            afxDrawInput din2;
            AfxIterateLinkageB2F(AFX_OBJECT(afxDrawInput), din2, &from->inputs, dctx)
            {
                AfxAssertObjects(1, &din2, afxFcc_DIN);

                if (din2 == din)
                {
                    if (AfxWaitForDrawContext(from))
                        AfxThrowError();

                    if (ddev->relinkDinCb && ddev->relinkDinCb(ddev, NIL, 1, &din)) // if disconnection was refused
                    {
                        AfxThrowError();
                    }
                    else
                    {
                        AfxExhaustChainedClasses(&din->classes);
                        AfxPopLinkage(&din->dctx);
                        AfxReleaseObjects(1, &from);
                    }
                    break;
                }
            }
        }

        if (!err && dctx)
        {
            AfxReacquireObjects(1, &dctx);
            AfxPushLinkage(&din->dctx, &dctx->inputs);

            din->clipSpace = dctx->clipSpace;

            if (ddev->relinkDinCb && ddev->relinkDinCb(ddev, dctx, 1, &din)) // if reconnection was refused
            {
                AfxThrowError();
                AfxPopLinkage(&din->dctx);
                AfxReleaseObjects(1, &dctx);
            }

            // reset camera clip space.
        }

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

    afxDrawContext dctx;
    AfxAssert(!AfxGetDrawInputContext(din, &dctx));
    return err;
}

_AVX void AfxDescribeClipSpace(afxDrawInput din, avxClipSpace* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(clip);
    *clip = din->clipSpace;
}

_AVX void AfxComputeLookToMatrices(afxDrawInput din, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(dir);
    AfxAssert(!AfxV3dIsZero(dir));
    AfxAssert(!AfxV3dIsInfinite(dir));    
    AfxAssert(v);
    AfxComputeLookToMatrix(v, eye, dir, AFX_V4D_Y, &din->clipSpace);

    if (iv)
        AfxM4dInvert(iv, v);
}

_AVX void AfxComputeLookAtMatrices(afxDrawInput din, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(target);
    AfxAssert(eye);
    AfxAssert(v);
    AfxComputeLookAtMatrix(v, eye, target, AFX_V4D_Y, &din->clipSpace);

    if (iv)
        AfxM4dInvert(iv, v);
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
    AfxComputeOrthographicMatrix(p, extent, near, far, &din->clipSpace);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AfxComputeOffcenterOrthographicMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(!AfxRealIsEqual(right, left, 0.00001f));
    AfxAssert(!AfxRealIsEqual(top, bottom, 0.00001f));
    AfxAssert(!AfxRealIsEqual(far, near, 0.00001f));
    AfxAssert(p);
    AfxComputeOffcenterOrthographicMatrix(p, left, right, bottom, top, near, far, &din->clipSpace);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AfxComputeBoundingOrthographicMatrices(afxDrawInput din, afxBox const aabb, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(aabb); // afxBox deve estar no view space.
    AfxAssert(p);
    AfxComputeBoundingOrthographicMatrix(p, aabb, &din->clipSpace);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AfxComputeBasicOrthographicMatrices(afxDrawInput din, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(aspectRatio); // w/h
    AfxAssert(p);
    AfxComputeBasicOrthographicMatrix(p, aspectRatio, scale, range, &din->clipSpace);

    if (ip)
        AfxM4dInvert(ip, p);
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
    AfxComputePerspectiveMatrix(p, extent, near, far, &din->clipSpace);

    if (ip)
        AfxM4dInvert(ip, p);
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
    AfxComputeFovPerspectiveMatrix(p, fovY, aspectRatio, near, far, &din->clipSpace);

    if (ip)
        AfxM4dInvert(ip, p);
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
    AfxComputeOffcenterPerspectiveMatrix(p, left, right, bottom, top, near, far, &din->clipSpace);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AfxComputeBasicPerspectiveMatrices(afxDrawInput din, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AfxAssert(p);
    AfxComputeBasicPerspectiveMatrix(p, aspectRatio, range, &din->clipSpace);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX afxNat AfxExecuteCmdBuffers(afxDrawInput din, afxNat cnt, afxExecutionRequest const req[], afxFence fenc)
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
        afxDrawBridge ddge;
        AfxGetDrawBridge(dctx, 0, &ddge);
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

_AVX afxError _AvxDinDtorCb(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    AfxDisconnectDrawInput(din);

    afxDrawDevice ddev = AfxGetDrawInputDevice(din);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    if (ddev->closeDinCb && ddev->closeDinCb(ddev, din))
        AfxThrowError();

    AfxAssert(!din->idd);

    for (afxNat i = 0; i < din->poolCnt; i++)
    {
        AfxDeregisterClass(&din->pools[i].streams);
        AfxCleanUpSlock(&din->pools[i].reqLock);
        AfxCleanUpQueue(&din->pools[i].recycQue);
    }
    AfxDeallocate(din->pools);

    AfxCleanUpChainedClasses(&din->classes);

    return err;
}

_AVX afxError _AvxDinCtorCb(afxDrawInput din, afxCookie const *cookie)
{
    //AfxEntry("din=%p,uri=%.*s", din, endpoint ? AfxPushString(AfxGetUriString(endpoint)) : &AFX_STR_EMPTY);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    AfxZero(din, sizeof(din[0]));

    afxDrawDevice ddev = cookie->udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxUri const* endpoint = cookie->udd[1];
    afxDrawInputConfig const* cfg = ((afxDrawInputConfig const *)cookie->udd[2]) + cookie->no;
    
    AfxPushLinkage(&din->ddev, &ddev->openedDinChain);
    AfxPushLinkage(&din->dctx, NIL);

    afxChain *classes = &din->classes;
    AfxDeployChain(classes, (void*)din);

    afxNat defStreamSiz = 32000000; // 32MB (I think it is the total of RAM in PlayStation 2)

    din->minIdxBufSiz = defStreamSiz / 4;
    din->maxIdxBufSiz = 0;
    din->minVtxBufSiz = defStreamSiz - din->minIdxBufSiz;
    din->maxVtxBufSiz = 0;

    din->udd = cfg && cfg->udd ? cfg->udd : NIL;

    din->procCb = cfg && cfg->proc ? cfg->proc : NIL;

    afxClassConfig tmpClsConf;

    tmpClsConf = _AvxCamClsCfg;
    AfxRegisterClass(&din->camCls, NIL, classes, &tmpClsConf);

    tmpClsConf = _AvxDtecClsCfg;
    AfxRegisterClass(&din->dtecCls, NIL, classes, &tmpClsConf);

    tmpClsConf = _AvxVbufMgrCfg;
    AfxRegisterClass(&din->vbuffers, NIL, classes, &tmpClsConf);

    tmpClsConf = _AvxTxdClsCfg;
    AfxRegisterClass(&din->txdCls, NIL, classes, &tmpClsConf);

    din->clipSpace = ddev->clipSpace;

    din->poolCnt = 1;

    if (!(din->pools = AfxAllocate(din->poolCnt, sizeof(din->pools[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < din->poolCnt; i++)
        {
            din->pools[i].lockedForReq = FALSE;
            AfxSetUpSlock(&din->pools[i].reqLock);
            AfxSetUpQueue(&din->pools[i].recycQue, sizeof(avxCmdb), 3);

            afxClassConfig tmpClsCfg = _AvxCmdbStdImplementation;
            AfxRegisterClass(&din->pools[i].streams, NIL, classes, &tmpClsCfg);
        }

        if (!err)
        {
            din->idd = NIL;

            if (ddev->openDinCb && ddev->openDinCb(ddev, din, cfg, endpoint)) AfxThrowError();
            else
            {

            }
        }

        if (err)
            AfxDeallocate(din->pools);
    }

    return err;
}

_AVX afxClassConfig const _AvxDinClsCfg =
{
    .fcc = afxFcc_DIN,
    .name = "DrawInput",
    .desc = "Draw Input Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawInput)),
    .ctor = (void*)_AvxDinCtorCb,
    .dtor = (void*)_AvxDinDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxConfigureDrawInput(afxNat ddevId, afxDrawInputConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ddevId != AFX_INVALID_INDEX);
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        AfxAssert(cfg);
        *cfg = (afxDrawInputConfig) { 0 };        
    }
    return err;
}

_AVX afxError AfxOpenDrawInput(afxNat ddevId, afxUri const* endpoint, afxDrawInputConfig const* cfg, afxDrawInput* input)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxClass* cls = (afxClass*)AvxGetDrawInputClass();
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

_AVXINL afxBool _AvxTestDinIsFromDdevFltCb(afxDrawInput din, afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return (ddev == AfxGetDrawInputDevice(din));
}

_AVXINL afxBool _AvxTestDinIsFromDdevFlt2Cb(afxDrawInput din, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawDevice ddev = udd[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return (ddev == AfxGetDrawInputDevice(din)) && ((afxBool(*)(afxDrawInput,void*))udd[1])(din, udd[2]);
}

_AVX afxNat AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(inputs);
    AfxAssert(cnt);
    afxClass const* cls = AvxGetDrawInputClass();
    AfxAssertClass(cls, afxFcc_DIN);
    afxNat rslt = 0;

    if (!ddev) rslt = AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)inputs);
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        rslt = AfxEvokeClassInstances(cls, (void*)_AvxTestDinIsFromDdevFltCb, ddev, first, cnt, (afxObject*)inputs);
    }
    return rslt;
}

_AVX afxNat AfxEvokeDrawInputs(afxDrawDevice ddev, afxBool(*f)(afxDrawInput, void*), void* udd, afxNat first, afxNat cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(inputs);
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AvxGetDrawInputClass();
    AfxAssertClass(cls, afxFcc_DIN);
    afxNat rslt = 0;

    if (!ddev) rslt = AfxEvokeClassInstances(cls, (void*)f, udd, first, cnt, (afxObject*)inputs);
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        rslt = AfxEvokeClassInstances(cls, (void*)_AvxTestDinIsFromDdevFlt2Cb, (void*[]) { ddev, f, udd }, first, cnt, (afxObject*)inputs);
    }
    return rslt;
}

_AVX afxNat AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AvxGetDrawInputClass();
    AfxAssertClass(cls, afxFcc_DIN);
    afxNat rslt = 0;

    if (!ddev) rslt = AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)_AvxTestDinIsFromDdevFltCb, ddev, (void*)f, udd);
    }
    return rslt;
}

_AVX afxNat AfxInvokeDrawInputs2(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput,void*), void* udd, afxBool(*f2)(afxDrawInput,void*), void* udd2)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AvxGetDrawInputClass();
    AfxAssertClass(cls, afxFcc_DIN);
    afxNat rslt = 0;

    if (!ddev) rslt = AfxInvokeClassInstances(cls, first, cnt, (void*)f2, udd2);
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)_AvxTestDinIsFromDdevFlt2Cb, (void*[]) { ddev, f, udd }, (void*)f2, udd2);
    }
    return rslt;
}
