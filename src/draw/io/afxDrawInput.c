/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

//#define _AFX_CORE_C
//#define _AFX_DEVICE_C
//#define _AFX_CONTEXT_C
#define _AVX_DRAW_C
//#define _AVX_DRAW_SYSTEM_C
//#define _AVX_DRAW_DEVICE_C
//#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_INPUT_C
#include "../ddi/avxImplementation.h"

extern afxClassConfig const _AVX_TEX_CLASS_CONFIG;
extern afxClassConfig const _AVX_DTEC_CLASS_CONFIG;
extern afxClassConfig const _AVX_VBUF_CLASS_CONFIG;
//AFX afxClassConfig const _AvxIbufClsCfg;

extern afxClassConfig const _AvxMshtClsCfg;
extern afxClassConfig const _AvxGeomClsCfg;

_AVX afxDrawSystem AvxGetDrawInputContext(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxDrawSystem dsys = AfxGetProvider(din);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxDrawDevice AvxGetDrawInputDevice(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxDrawSystem dsys = AvxGetDrawInputContext(din);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxDrawDevice ddev = AfxGetProvider(din);
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    return ddev;
}

_AVX void* AvxGetDrawInputUdd(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    return din->udd;
}

_AVX afxClass const* AvxGetTextureClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    // din must be a valid afxDrawInput handle.
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->texCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TEX);
    return cls;
}

_AVX afxClass const* AvxGetDrawTechniqueClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->dtecCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DTEC);
    return cls;
}

_AVX afxClass const* AvxGetMeshTopologyClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->mshtCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSHT);
    return cls;
}

_AVX afxClass const* AvxGetGeometryClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->geomCls;
    AFX_ASSERT_CLASS(cls, afxFcc_GEO);
    return cls;
}

_AVX afxClass* AvxGetVertexBufferClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass *cls = &din->vbuffers;
    AFX_ASSERT_CLASS(cls, afxFcc_VBUF);
    return cls;
}

_AVX afxClass* AvxGetIndexBufferClass(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass *cls = &din->ibuffers;
    AFX_ASSERT_CLASS(cls, afxFcc_IBUF);
    return cls;
}

_AVX void AvxGetClipSpaceInfo(afxDrawInput din, avxClipSpaceDepth* depth, afxBool* leftHanded)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    if (depth)
        *depth = din->clipSpaceDepth;

    if (leftHanded)
        *leftHanded = din->leftHandedSpace;
}

_AVX void AvxComputeLookToMatrices(afxDrawInput din, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(dir);
    AFX_ASSERT(!AfxV3dIsZero(dir));
    AFX_ASSERT(!AfxV3dIsInfinite(dir));    
    AFX_ASSERT(v);
    AfxComputeLookToMatrix(v, eye, dir, AFX_V4D_Y, din->leftHandedSpace);

    if (iv)
        AfxM4dInvert(iv, v);
}

_AVX void AvxComputeLookAtMatrices(afxDrawInput din, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(target);
    AFX_ASSERT(eye);
    AFX_ASSERT(v);
    AfxComputeLookAtMatrix(v, eye, target, AFX_V4D_Y, din->leftHandedSpace);

    if (iv)
        AfxM4dInvert(iv, v);
}

_AVX void AvxComputeOrthographicMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(extent);
    AFX_ASSERT(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeOrthographicMatrix(p, extent, near, far, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AvxComputeOffcenterOrthographicMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeOffcenterOrthographicMatrix(p, left, right, bottom, top, near, far, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AvxComputeBoundingOrthographicMatrices(afxDrawInput din, afxBox const aabb, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    //AFX_ASSERT(aabb); // afxBox deve estar no view space.
    AFX_ASSERT(p);
    AfxComputeBoundingOrthographicMatrix(p, aabb, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AvxComputeBasicOrthographicMatrices(afxDrawInput din, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(aspectRatio); // w/h
    AFX_ASSERT(p);
    AfxComputeBasicOrthographicMatrix(p, aspectRatio, scale, range, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AvxComputePerspectiveMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(extent);
    AFX_ASSERT(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AFX_ASSERT(p);
    AfxComputePerspectiveMatrix(p, extent, near, far, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AvxComputeFovMatrices(afxDrawInput din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AFX_ASSERT(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AFX_ASSERT(p);
    AfxComputeFovPerspectiveMatrix(p, fovY, aspectRatio, near, far, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AvxComputeFrustrumMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AFX_ASSERT(p);
    AfxComputeOffcenterPerspectiveMatrix(p, left, right, bottom, top, near, far, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX void AvxComputeBasicPerspectiveMatrices(afxDrawInput din, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeBasicPerspectiveMatrix(p, aspectRatio, range, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_AVX afxError AvxUplinkTxds(afxDrawInput din, afxUnit baseSlot, afxUnit slotCnt, afxUri const uris[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT_RANGE(8, baseSlot, slotCnt);
    
    if (!uris)
    {
        for (afxUnit i = 0; i < slotCnt; i++)
        {
            AfxClearUris(1, &din->txdUris[i].uri);
        }
    }
    else
    {
        for (afxUnit i = 0; i < slotCnt; i++)
        {
            AfxCopyUri(&din->txdUris[i].uri, &uris[i]);
        }
    }
    return err;
}

_AVX afxError _AvxDinDtorCb(afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    afxDrawDevice ddev = AvxGetDrawInputDevice(din);
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    AfxExhaustChainedClasses(&din->classes);

    AFX_ASSERT(!din->idd);

    AfxDeregisterChainedClasses(&din->classes);

    return err;
}

_AVX afxError _AvxDinCtorCb(afxDrawInput din, void** args, afxUnit invokeNo)
{
    //AfxEntry("din=%p,uri=%.*s", din, endpoint ? AfxPushString(AfxGetUriString(endpoint)) : &AFX_STRING_EMPTY);
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    AfxZero(din, sizeof(din[0]));

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxDrawInputConfig const* cfg = ((afxDrawInputConfig const *)args[1]) + invokeNo;

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxChain *classes = &din->classes;
    AfxDeployChain(classes, (void*)din);

    afxUnit defStreamSiz = 32000000; // 32MB (I think it is the total of RAM in PlayStation 2)

    din->minIdxBufSiz = defStreamSiz / 4;
    din->maxIdxBufSiz = 0;
    din->minVtxBufSiz = defStreamSiz - din->minIdxBufSiz;
    din->maxVtxBufSiz = 0;

    din->udd = cfg && cfg->udd ? cfg->udd : NIL;

    din->procCb = cfg && cfg->proc ? cfg->proc : NIL;

    afxClassConfig tmpClsConf;

    tmpClsConf = _AVX_VBUF_CLASS_CONFIG;
    AfxMountClass(&din->vbuffers, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AvxGeomClsCfg;
    //AfxMountClass(&din->geomCls, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AvxMshtClsCfg;
    //AfxMountClass(&din->mshtCls, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AVX_TEX_CLASS_CONFIG;
    //AfxMountClass(&din->texCls, NIL, classes, &tmpClsConf);

    tmpClsConf = _AVX_DTEC_CLASS_CONFIG;
    AfxMountClass(&din->dtecCls, NIL, classes, &tmpClsConf);

    din->vbMgr = NIL;

    din->leftHandedSpace = FALSE;
    din->clipSpaceDepth = avxClipSpaceDepth_NEG_ONE_TO_ONE; // TODO: Get this from dsys

    return err;
}

_AVX afxClassConfig const _AVX_DIN_CLASS_CONFIG =
{
    .fcc = afxFcc_DIN,
    .name = "DrawInput",
    .desc = "Draw Input Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawInput)),
    .ctor = (void*)_AvxDinCtorCb,
    .dtor = (void*)_AvxDinDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxConfigureDrawInput(afxDrawSystem dsys, afxDrawInputConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);
    *cfg = (afxDrawInputConfig) { 0 };
    return err;
}

_AVX afxError AvxOpenDrawInput(afxDrawSystem dsys, afxDrawInputConfig const* cfg, afxDrawInput* input)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->dinCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);

    afxDrawInput din;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&din, (void const*[]) { dsys, cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(input);
    *input = din;
    return err;
}

_AVX afxUnit AvxEnumerateDrawInputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(!cnt || inputs);
    afxClass const* cls = _AvxDsysGetImpl(dsys)->dinCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)inputs);
}

_AVX afxUnit AvxEvokeDrawInputs(afxDrawSystem dsys, afxBool(*f)(afxDrawInput, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(inputs);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxDsysGetImpl(dsys)->dinCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)inputs);
}

_AVX afxUnit AvxInvokeDrawInputs(afxDrawSystem dsys, afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(f);
    afxClass const* cls = _AvxDsysGetImpl(dsys)->dinCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}
