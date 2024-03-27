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

#define _AFX_DRAW_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_INPUT_C
#include "qwadro/core/afxManager.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/math/afxOpticalMatrix.h"
#include "qwadro/draw/afxDrawSystem.h"

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
    afxDrawDevice ddev = AfxGetObjectProvider(din);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AVX afxManager* AfxGetCameraClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxManager *cls = &din->cameras;
    AfxAssertClass(cls, afxFcc_CAM);
    return cls;
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

_AVX afxDrawContext AfxGetDrawInputContext(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawContext dctx = din->dctx;
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AVX afxBool AfxDrawInputIsOnline(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return !!(AfxGetDrawInputContext(din));
}

_AVX afxBool AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawContext curr = AfxGetDrawInputContext(din);

    if (dctx != curr)
    {
        afxDrawDevice ddev = AfxGetDrawInputDevice(din);
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (dctx)
        {
            AfxAssertObjects(1, &dctx, afxFcc_DCTX);
            afxDrawDevice ddrv2 = AfxGetDrawContextDevice(dctx);
            AfxAssertObjects(1, &ddrv2, afxFcc_DDEV);

            if (ddev != ddrv2) // can not connect to context acquired of another device
                AfxThrowError();
        }

        if (!err)
        {
            if (ddev->relinkDin(ddev, dctx, 1, &din))
                AfxThrowError();
        }
    }
    AfxAssert(din->dctx == dctx);
    return !err;
}

_AVX afxError AfxDisconnectDrawInput(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!AfxReconnectDrawInput(din, NIL))
        AfxThrowError();

    AfxAssert(!AfxDrawInputIsOnline(din));
    return err;
}

_AVX void AfxDescribeClipSpace(afxDrawInput din, afxClipSpace* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(clip);
    *clip = din->cachedClipCfg;
}

_AVX void AfxComputeLookToMatrices(afxDrawInput din, afxReal const eye[3], afxReal const dir[3], afxReal v[4][4], afxReal iv[4][4])
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

_AVX void AfxComputeLookAtMatrices(afxDrawInput din, afxReal const eye[3], afxReal const target[3], afxReal v[4][4], afxReal iv[4][4])
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

_AVX void AfxComputeOrthographicMatrices(afxDrawInput din, afxReal const extent[2], afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AVX void AfxComputeOffcenterOrthographicMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AVX void AfxComputeBoundingOrthographicMatrices(afxDrawInput din, afxAabb const aabb, afxReal p[4][4], afxReal ip[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(aabb); // afxAabb deve estar no view space.
    AfxAssert(p);
    AfxComputeBoundingOrthographicMatrix(p, aabb, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX void AfxComputeBasicOrthographicMatrices(afxDrawInput din, afxReal aspectRatio, afxReal scale, afxReal range, afxReal p[4][4], afxReal ip[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(aspectRatio); // w/h
    AfxAssert(p);
    AfxComputeBasicOrthographicMatrix(p, aspectRatio, scale, range, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX void AfxComputePerspectiveMatrices(afxDrawInput din, afxReal const extent[2], afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AVX void AfxComputeFovMatrices(afxDrawInput din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AVX void AfxComputeFrustrumMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AVX void AfxComputeBasicPerspectiveMatrices(afxDrawInput din, afxReal aspectRatio, afxReal range, afxReal p[4][4], afxReal ip[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AfxAssert(p);
    AfxComputeBasicPerspectiveMatrix(p, aspectRatio, range, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AVX afxError AfxExecuteDrawStreams(afxDrawInput din, afxNat cnt, afxExecutionRequest const req[], afxFence fenc)
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

    afxDrawContext dctx;

    if (!(dctx = AfxGetDrawInputContext(din))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        if (dctx->executeCb(dctx, cnt, req, fenc))
            AfxThrowError();
    }
    return err;
}

_AVX afxError AfxUplinkTextureDictionaries(afxDrawInput din, afxNat baseSlotIdx, afxNat slotCnt, afxUri const uris[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssertRange(8, baseSlotIdx, slotCnt);
    
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

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxOpenDrawInputs(afxNat ddevId, afxNat cnt, afxDrawInputConfig const cfg[], afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(inputs);
    afxDrawDevice ddev;
    
    if (!(ddev = AfxGetDrawDevice(ddevId))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxManager* cls = AfxGetDrawInputClass(ddev);
        AfxAssertClass(cls, afxFcc_DIN);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)inputs, (void const*[]) { ddev, cfg }))
            AfxThrowError();

        AfxAssertObjects(cnt, inputs, afxFcc_DIN);
    }
    return err;
}

_AVX afxNat AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetDrawInputClass(ddev);
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(inputs);
    AfxAssert(cnt);
    afxManager* cls = AfxGetDrawInputClass(ddev);
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)inputs);
}

_AVX afxNat AfxCountDrawInputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager*cls = AfxGetDrawInputClass(ddev);
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxCountObjects(cls);
}
