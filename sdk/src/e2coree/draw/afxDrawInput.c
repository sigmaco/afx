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
#define _AFX_DRAW_INPUT_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/math/afxOpticalMatrix.h"

AFXINL afxDrawSystem _AfxGetDsysData(void);

_AFX void* AfxGetDrawInputUdd(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return din->udd;
}

_AFX afxDrawDevice AfxGetDrawInputDevice(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawDevice ddev = AfxGetObjectProvider(din);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

_AFX afxClass* AfxGetCameraClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass *cls = &din->cameras;
    AfxAssertClass(cls, afxFcc_CAM);
    return cls;
}

_AFX afxClass* AfxGetVertexBufferClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass *cls = &din->vbuffers;
    AfxAssertClass(cls, afxFcc_VBUF);
    return cls;
}

_AFX afxClass* AfxGetIndexBufferClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass *cls = &din->ibuffers;
    AfxAssertClass(cls, afxFcc_IBUF);
    return cls;
}

_AFX afxDrawContext AfxGetDrawInputContext(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawContext dctx = din->dctx;
    AfxTryAssertObjects(1, &dctx, afxFcc_DCTX);
    return dctx;
}

_AFX afxBool AfxDrawInputIsOnline(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    return !!(AfxGetDrawInputContext(din));
}

_AFX afxBool AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx)
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

_AFX afxError AfxDisconnectDrawInput(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!AfxReconnectDrawInput(din, NIL))
        AfxThrowError();

    AfxAssert(!AfxDrawInputIsOnline(din));
    return err;
}

_AFX void AfxDescribeClipSpace(afxDrawInput din, afxClipSpace* clip)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(clip);
    *clip = din->cachedClipCfg;
}

_AFX void AfxComputeLookToMatrices(afxDrawInput din, afxReal const eye[3], afxReal const dir[3], afxReal v[4][4], afxReal iv[4][4])
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

_AFX void AfxComputeLookAtMatrices(afxDrawInput din, afxReal const eye[3], afxReal const target[3], afxReal v[4][4], afxReal iv[4][4])
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

_AFX void AfxComputeOrthographicMatrices(afxDrawInput din, afxReal const extent[2], afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AFX void AfxComputeOffcenterOrthographicMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AFX void AfxComputeBoundingOrthographicMatrices(afxDrawInput din, afxAabb const aabb, afxReal p[4][4], afxReal ip[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(aabb); // afxAabb deve estar no view space.
    AfxAssert(p);
    AfxComputeBoundingOrthographicMatrix(p, aabb, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AFX void AfxComputeBasicOrthographicMatrices(afxDrawInput din, afxReal aspectRatio, afxReal scale, afxReal range, afxReal p[4][4], afxReal ip[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(aspectRatio); // w/h
    AfxAssert(p);
    AfxComputeBasicOrthographicMatrix(p, aspectRatio, scale, range, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AFX void AfxComputePerspectiveMatrices(afxDrawInput din, afxReal const extent[2], afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AFX void AfxComputeFovMatrices(afxDrawInput din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AFX void AfxComputeFrustrumMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxReal p[4][4], afxReal ip[4][4])
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

_AFX void AfxComputeBasicPerspectiveMatrices(afxDrawInput din, afxReal aspectRatio, afxReal range, afxReal p[4][4], afxReal ip[4][4])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AfxAssert(p);
    AfxComputeBasicPerspectiveMatrix(p, aspectRatio, range, &din->cachedClipCfg);

    if (ip)
        AfxInvertM4d(p, ip);
}

_AFX afxError AfxExecuteDrawScripts(afxDrawInput din, afxNat cnt, afxDrawScript scripts[], afxSemaphore wait[], afxPipelineStage const waitStage[], afxSemaphore signal[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(scripts);

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

        if (din->submitCb(din, cnt, scripts, wait, waitStage, signal))
            AfxThrowError();
    }
    return err;
}

_AFX afxError AfxPresentDrawBuffers(afxDrawInput din, afxNat cnt, afxDrawOutput outputs[], afxNat outBufIdx[], afxSemaphore wait[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(cnt);
    AfxAssert(outputs);
    AfxAssert(outBufIdx);

    /*
        Any writes to memory backing the images referenced by the pImageIndices and pSwapchains members of pPresentInfo, 
        that are available before vkQueuePresentKHR is executed, are automatically made visible to the read access performed by the presentation engine. 
        This automatic visibility operation for an image happens-after the semaphore signal operation, and happens-before the presentation engine accesses the image.

        Queueing an image for presentation defines a set of queue operations, including waiting on the semaphores and submitting a presentation request to the presentation engine. 
        However, the scope of this set of queue operations does not include the actual processing of the image by the presentation engine.
    */
    
    afxDrawContext dctx;

    if (!(dctx = AfxGetDrawInputContext(din))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        if (din->presentCb(din, cnt, outputs, outBufIdx, wait))
            AfxThrowError();
    }
    return err;
}

_AFX afxError AfxUplinkTextureDictionaries(afxDrawInput din, afxNat baseSlotIdx, afxNat slotCnt, afxUri const uris[])
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

_AFX afxError AfxOpenDrawInputs(afxNat ddevId, afxNat cnt, afxDrawInputConfig const cfg[], afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev;
    
    if (!(ddev = AfxGetDrawDevice(ddevId))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        afxClass* cls = AfxGetDrawInputClass(ddev);
        AfxAssertClass(cls, afxFcc_DIN);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)din, (void const*[]) { ddev, cfg }))
            AfxThrowError();

        AfxAssertObjects(cnt, din, afxFcc_DIN);
    }
    return err;
}

_AFX afxNat AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls;

    if (ddev)
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        cls = AfxGetDrawInputClass(ddev);            
    }
    else
    {
        afxDrawSystem dsys = _AfxGetDsysData();
        AfxAssertType(dsys, afxFcc_DSYS);
        cls = &dsys->dinputs;
    }
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(din);
    afxClass* cls;

    if (ddev)
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        cls = AfxGetDrawInputClass(ddev);            
    }
    else
    {
        afxDrawSystem dsys = _AfxGetDsysData();
        AfxAssertType(dsys, afxFcc_DSYS);
        cls = &dsys->dinputs;
    }
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)din);
}

_AFX afxNat AfxCountDrawInputs(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass*cls = AfxGetDrawInputClass(ddev);
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxCountInstances(cls);
}
