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
#include "../dev/AvxImplKit.h"

extern afxClassConfig const _AvxTxdClsCfg;
extern afxClassConfig const _AvxTexClsCfg;
extern afxClassConfig const _AvxCamClsCfg;
extern afxClassConfig const _AvxDtecClsCfg;
extern afxClassConfig const _AvxVbufMgrCfg;
//AFX afxClassConfig const _AvxIbufClsCfg;

extern afxClassConfig const _AvxMshClsCfg;
extern afxClassConfig const _AvxGeomClsCfg;

_AVX afxDrawDevice AfxGetDrawInputDevice(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxDrawDevice ddev = AfxGetProvider(din);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev;
}

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

_AVX afxClass const* AfxGetTextureClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    /// din must be a valid afxDrawInput handle.
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = &din->texCls;
    AfxAssertClass(cls, afxFcc_TEX);
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

_AVX afxClass const* AfxGetMeshClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = &din->mshCls;
    AfxAssertClass(cls, afxFcc_MSH);
    return cls;
}

_AVX afxClass const* AfxGetGeometryClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxClass const* cls = &din->geomCls;
    AfxAssertClass(cls, afxFcc_VTD);
    return cls;
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
                    if (AfxWaitForDrawContext(from, AFX_TIME_INFINITE))
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

_AVX afxStringBase AfxGetPivotTagStringBase(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!din->strbJointBiasesTags)
        AfxAcquireStringCatalogs(1, &din->strbJointBiasesTags);

    return din->strbJointBiasesTags;
}

_AVX afxStringBase AfxGetMorphTagStringBase(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    if (!din->strbMorphTags)
        AfxAcquireStringCatalogs(1, &din->strbMorphTags);

    return din->strbMorphTags;
}

_AVX afxError _AvxDinDtorCb(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxDrawDevice ddev = AfxGetDrawInputDevice(din);
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    AfxDisconnectDrawInput(din);

    AfxExhaustChainedClasses(&din->classes);

    AfxAssert(!din->idd);

    AfxDeregisterChainedClasses(&din->classes);

    return err;
}

_AVX afxError _AvxDinCtorCb(afxDrawInput din, void** args, afxNat invokeNo)
{
    //AfxEntry("din=%p,uri=%.*s", din, endpoint ? AfxPushString(AfxGetUriString(endpoint)) : &AFX_STR_EMPTY);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    AfxZero(din, sizeof(din[0]));

    afxDrawDevice ddev = args[0];
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxDrawInputConfig const* cfg = ((afxDrawInputConfig const *)args[1]) + invokeNo;
    
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

    tmpClsConf = _AvxTexClsCfg;
    AfxRegisterClass(&din->texCls, NIL, classes, &tmpClsConf);

    tmpClsConf = _AvxTxdClsCfg;
    AfxRegisterClass(&din->txdCls, NIL, classes, &tmpClsConf);

    tmpClsConf = _AvxGeomClsCfg;
    AfxRegisterClass(&din->geomCls, NIL, classes, &tmpClsConf);
    
    tmpClsConf = _AvxMshClsCfg;
    AfxRegisterClass(&din->mshCls, NIL, classes, &tmpClsConf); // require msht, vtd

    din->vbMgr = NIL;

    din->clipSpace = ddev->clipSpace;

    return err;
}

_AVX afxClassConfig const _AvxDinStdImplementation =
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

_AVX afxError AfxOpenDrawInput(afxNat ddevId, afxDrawInputConfig const* cfg, afxDrawInput* input)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev;

    if (!(AfxGetDrawDevice(ddevId, &ddev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (AfxCallDevice(&ddev->dev, 1, NIL)) AfxThrowError(); // let the device build its DPUs.
        else
        {
            afxClass* cls = (afxClass*)AvxGetDrawInputClass(ddev);
            AfxAssertClass(cls, afxFcc_DIN);
            afxDrawInput din;

            if (!cfg) AfxThrowError();
            else
            {
                if (AfxAcquireObjects(cls, 1, (afxObject*)&din, (void const*[]) { ddev, cfg })) AfxThrowError();
                else
                {
                    AfxAssertObjects(1, &din, afxFcc_DIN);
                    AfxAssert(input);
                    *input = din;
                }
            }
        }
    }
    return err;
}
