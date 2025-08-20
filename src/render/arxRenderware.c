/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _ARX_DRAW_INPUT_C
#include "ddi/arxImpl_Input.h"

_ARX afxDrawSystem ArxGetDrawInputContext(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxDrawSystem dsys = din->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_ARX void* ArxGetDrawInputUdd(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    return din->udd;
}

_ARX afxClass const* _ArxGetPoseClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->poseCls;
    AFX_ASSERT_CLASS(cls, afxFcc_POSE);
    return cls;
}

_ARX afxClass const* _ArxGetPlacementClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->plceCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PLCE);
    return cls;
}

_ARX afxClass const* _ArxDinGetGeomClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->mtlyCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MORP);
    return cls;
}

_ARX afxClass const* _ArxGetRendererClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const*cls = &din->renderers;
    AFX_ASSERT_CLASS(cls, afxFcc_RND);
    return cls;
}

_ARX afxClass const* _ArxGetSceneClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->scnCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SCN);
    return cls;
}

_ARX afxClass const* _ArxGetLightClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const*cls = &din->lights;
    AFX_ASSERT_CLASS(cls, afxFcc_LIT);
    return cls;
}

_ARX afxClass const* _ArxDinGetCamClassCb_SW(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->camCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CAM);
    return cls;
}

_ARX afxClass const* ArxGetTextureClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    // din must be a valid arxRenderware handle.
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->texCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TEX);
    return cls;
}

_ARX afxClass const* ArxGetDrawTechniqueClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->dtecCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DTEC);
    return cls;
}

_ARX afxClass const* ArxGetMeshTopologyClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->mshtCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSHT);
    return cls;
}

_ARX afxClass const* ArxGetGeometryClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass const* cls = &din->geomCls;
    AFX_ASSERT_CLASS(cls, afxFcc_GEO);
    return cls;
}

_ARX afxClass* ArxGetVertexBufferClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass *cls = &din->vbuffers;
    AFX_ASSERT_CLASS(cls, afxFcc_VBUF);
    return cls;
}

_ARX afxClass* ArxGetIndexBufferClass(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxClass *cls = &din->ibuffers;
    AFX_ASSERT_CLASS(cls, afxFcc_IBUF);
    return cls;
}

_ARX void ArxGetClipSpaceInfo(arxRenderware din, avxClipSpaceDepth* depth, afxBool* leftHanded)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    if (depth)
        *depth = din->clipSpaceDepth;

    if (leftHanded)
        *leftHanded = din->leftHandedSpace;
}

_ARX void ArxComputeLookToMatrices(arxRenderware din, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv)
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

_ARX void ArxComputeLookAtMatrices(arxRenderware din, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv)
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

_ARX void ArxComputeOrthographicMatrices(arxRenderware din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
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

_ARX void ArxComputeOffcenterOrthographicMatrices(arxRenderware din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
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

_ARX void ArxComputeBoundingOrthographicMatrices(arxRenderware din, afxBox const aabb, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    //AFX_ASSERT(aabb); // afxBox deve estar no view space.
    AFX_ASSERT(p);
    AfxComputeBoundingOrthographicMatrix(p, aabb, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeBasicOrthographicMatrices(arxRenderware din, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(aspectRatio); // w/h
    AFX_ASSERT(p);
    AfxComputeBasicOrthographicMatrix(p, aspectRatio, scale, range, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputePerspectiveMatrices(arxRenderware din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
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

_ARX void ArxComputeFovMatrices(arxRenderware din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip)
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

_ARX void ArxComputeFrustrumMatrices(arxRenderware din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
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

_ARX void ArxComputeBasicPerspectiveMatrices(arxRenderware din, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeBasicPerspectiveMatrix(p, aspectRatio, range, din->leftHandedSpace, din->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX afxError ArxUplinkTxds(arxRenderware din, afxUnit baseSlot, afxUnit slotCnt, afxUri const uris[])
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

_ARX afxError _ArxDinDtorCb(arxRenderware din)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    AfxExhaustChainedClasses(&din->classes);

    AFX_ASSERT(!din->idd);

    AfxDeregisterChainedClasses(&din->classes);

    return err;
}

_ARX afxError _ArxDinCtorCb(arxRenderware din, void** args, afxUnit invokeNo)
{
    //AfxEntry("din=%p,uri=%.*s", din, endpoint ? AfxPushString(AfxGetUriString(endpoint)) : &AFX_STRING_EMPTY);
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    AfxZero(din, sizeof(din[0]));

    afxModule drv = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
    _arxDinAcquisition const* cfg = ((_arxDinAcquisition const *)args[1]) + invokeNo;

    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &cfg->dsys);
    din->dsys = cfg->dsys;

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

    din->udd = cfg && cfg->cfg.udd ? cfg->cfg.udd : NIL;

    din->procCb = cfg && cfg->cfg.proc ? cfg->cfg.proc : NIL;

    afxClassConfig camClsCfg = /*cfg->camClsCfg ? *cfg->camClsCfg :*/ _ARX_CAM_CLASS_CONFIG;
    AFX_ASSERT(camClsCfg.fcc == afxFcc_CAM);
    AfxMountClass(&din->camCls, NIL, classes, &camClsCfg);

    afxClassConfig tmpClsConf;

    tmpClsConf = _ARX_VBUF_CLASS_CONFIG;
    AfxMountClass(&din->vbuffers, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AvxGeomClsCfg;
    //AfxMountClass(&din->geomCls, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AvxMshtClsCfg;
    //AfxMountClass(&din->mshtCls, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AVX_TEX_CLASS_CONFIG;
    //AfxMountClass(&din->texCls, NIL, classes, &tmpClsConf);



    afxClassConfig clsCfg;



    tmpClsConf = _ARX_DTEC_CLASS_CONFIG;
    AfxMountClass(&din->dtecCls, NIL, classes, &tmpClsConf);

    tmpClsConf = cfg->poseClsCfg ? *cfg->poseClsCfg : _ARX_POSE_CLASS_CONFIG;
    AFX_ASSERT(tmpClsConf.fcc == afxFcc_POSE);
    AfxMountClass(&din->poseCls, NIL, classes, &tmpClsConf);

    tmpClsConf = cfg->plceClsCfg ? *cfg->plceClsCfg : _ARX_PLCE_CLASS_CONFIG;
    AFX_ASSERT(tmpClsConf.fcc == afxFcc_PLCE);
    AfxMountClass(&din->plceCls, NIL, classes, &tmpClsConf);

    tmpClsConf = cfg->mtlyClsCfg ? *cfg->mtlyClsCfg : _ARX_MORP_CLASS_CONFIG;
    AFX_ASSERT(tmpClsConf.fcc == afxFcc_MORP);
    AfxMountClass(&din->mtlyCls, NIL, classes, &tmpClsConf); // require tex

    tmpClsConf = cfg->litClsCfg ? *cfg->litClsCfg : _ARX_LIT_CLASS_CONFIG;
    AFX_ASSERT(tmpClsConf.fcc == afxFcc_LIT);
    AfxMountClass(&din->lights, NIL, classes, &tmpClsConf);

    tmpClsConf = cfg->rndClsCfg ? *cfg->rndClsCfg : _ARX_RND_CLASS_CONFIG;
    AFX_ASSERT(tmpClsConf.fcc == afxFcc_RND);
    AfxMountClass(&din->renderers, NIL, classes, &tmpClsConf); // require all

    tmpClsConf = _ARX_SCN_CLASS_CONFIG;
    AfxMountClass(&din->scnCls, NIL, classes, &tmpClsConf);

    din->vbMgr = NIL;

    din->leftHandedSpace = FALSE;
    din->clipSpaceDepth = avxClipSpaceDepth_NEG_ONE_TO_ONE; // TODO: Get this from dsys

    return err;
}

_ARX afxClassConfig const _ARX_DIN_CLASS_CONFIG =
{
    .fcc = afxFcc_DIN,
    .name = "DrawInput",
    .desc = "Draw Input Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(arxRenderware)),
    .ctor = (void*)_ArxDinCtorCb,
    .dtor = (void*)_ArxDinDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxConfigureRenderware(afxUnit icd, arxRenderwareConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cfg);
    afxDrawSystem dsys = cfg->dsys;
    *cfg = (arxRenderwareConfig) { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    cfg->dsys = dsys;
    return err;
}

ASX afxClass const* _ArxGetRweClass(afxModule icd);
ASX afxBool _AsxGetIcd(afxUnit icdIdx, afxModule* driver);

_ARX afxError ArxOpenRenderware(afxUnit icd, arxRenderwareConfig const* cfg, arxRenderware* input)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cfg);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxDrawSystem dsys = cfg->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxModule driver;
    if (!_AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    afxClass* cls = (afxClass*)_ArxGetRweClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);

    _arxDinAcquisition cfg2 = { 0 };
    cfg2.cfg = *cfg;
    cfg2.dsys = dsys;

    arxRenderware din;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&din, (void const*[]) { driver, &cfg2 }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT(input);
    *input = din;
    return err;
}

_ARX afxUnit ArxEnumerateDrawInputs(afxUnit icd, afxUnit first, afxUnit cnt, arxRenderware inputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(!cnt || inputs);

    afxModule driver;
    if (!_AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    afxClass* cls = (afxClass*)_ArxGetRweClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)inputs);
}

_ARX afxUnit ArxEvokeDrawInputs(afxUnit icd, afxBool(*f)(arxRenderware, void*), void* udd, afxUnit first, afxUnit cnt, arxRenderware inputs[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(inputs);
    AFX_ASSERT(f);

    afxModule driver;
    if (!_AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    afxClass* cls = (afxClass*)_ArxGetRweClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)inputs);
}

_ARX afxUnit ArxInvokeDrawInputs(afxUnit icd, afxUnit first, afxUnit cnt, afxBool(*f)(arxRenderware, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(f);

    afxModule driver;
    if (!_AsxGetIcd(icd, &driver))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &driver);
    AFX_ASSERT(AfxTestModule(driver, afxModuleFlag_ICD | afxModuleFlag_ASX));

    afxClass* cls = (afxClass*)_ArxGetRweClass(driver);
    AFX_ASSERT_CLASS(cls, afxFcc_DIN);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}
