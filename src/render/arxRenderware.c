/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _ARX_RENDER_CONTEXT_C
#include "ddi/arxImpl_Input.h"

_ARX afxDrawSystem ArxGetRenderwareDrawSystem(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxDrawSystem dsys = rwe->dsys;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_ARX void* ArxGetRenderwareUdd(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    return rwe->udd;
}

_ASX afxClass const* _ArxRweGetRbufClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->sbufCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SBUF);
    return cls;
}

_ARX afxClass const* _ArxRweGetMtlClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->mtlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);
    return cls;
}

_ARX afxClass const* _ArxRweGetMshClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->mshCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSH);
    return cls;
}

_ARX afxClass const* _ArxRweGetMdlClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->mdlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);
    return cls;
}

_ARX afxClass const* _ArxRweGetTerClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->terCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TER);
    return cls;
}

_ARX afxStringBase _ArxRweGetModelUrnStringBase(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    return rwe->strbMdlSklMotUrns;
}

_ARX afxStringBase _ArxRweGetPivotTagStringBase(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    return rwe->strbJointBiasesTags;
}

_ARX afxStringBase _ArxRweGetMorphTagStringBase(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    return rwe->strbMorphTags;
}

_ARX afxClass const* _ArxRweGetPoseClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->poseCls;
    AFX_ASSERT_CLASS(cls, afxFcc_POSE);
    return cls;
}

_ARX afxClass const* _ArxRweGetPlceClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->plceCls;
    AFX_ASSERT_CLASS(cls, afxFcc_PLCE);
    return cls;
}

_ARX afxClass const* _ArxRweGetRndrClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const*cls = &rwe->renderers;
    AFX_ASSERT_CLASS(cls, afxFcc_RND);
    return cls;
}

_ARX afxClass const* _ArxRweGetRctxClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const*cls = &rwe->rctxCls;
    AFX_ASSERT_CLASS(cls, afxFcc_RCTX);
    return cls;
}

_ARX afxClass const* _ArxRweGetScnClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->scnCls;
    AFX_ASSERT_CLASS(cls, afxFcc_SCN);
    return cls;
}

_ARX afxClass const* _ArxRweGetLitClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const*cls = &rwe->lights;
    AFX_ASSERT_CLASS(cls, afxFcc_LIT);
    return cls;
}

_ARX afxClass const* _ArxRweGetCamClassCb_SW(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    // @dsys must be a valid afxDrawSystem handle.
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->camCls;
    AFX_ASSERT_CLASS(cls, afxFcc_CAM);
    return cls;
}

_ARX afxClass const* _ArxRweGetTexClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    // rwe must be a valid arxRenderware handle.
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->texCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TEX);
    return cls;
}

_ARX afxClass const* _ArxRweGetTechClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->dtecCls;
    AFX_ASSERT_CLASS(cls, afxFcc_DTEC);
    return cls;
}

_ARX afxClass const* _ArxRweGetMshtClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->mshtCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSHT);
    return cls;
}

_ARX afxClass const* _ArxRweGetGeomClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass const* cls = &rwe->geomCls;
    AFX_ASSERT_CLASS(cls, afxFcc_GEO);
    return cls;
}

_ARX afxClass* _ArxRweGetVbufClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass *cls = &rwe->vbuffers;
    AFX_ASSERT_CLASS(cls, afxFcc_VBUF);
    return cls;
}

_ARX afxClass* _ArxRweGetIbufClass(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxClass *cls = &rwe->ibuffers;
    AFX_ASSERT_CLASS(cls, afxFcc_IBUF);
    return cls;
}

_ARX void ArxGetClipSpaceInfo(arxRenderware rwe, avxClipSpaceDepth* depth, afxBool* leftHanded)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);

    if (depth)
        *depth = rwe->clipSpaceDepth;

    if (leftHanded)
        *leftHanded = rwe->leftHandedSpace;
}

_ARX void ArxComputeLookToMatrices(arxRenderware rwe, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(dir);
    AFX_ASSERT(!AfxV3dIsZero(dir));
    AFX_ASSERT(!AfxV3dIsInfinite(dir));    
    AFX_ASSERT(v);
    AfxComputeLookToMatrix(v, eye, dir, AFX_V4D_Y, rwe->leftHandedSpace);

    if (iv)
        AfxM4dInvert(iv, v);
}

_ARX void ArxComputeLookAtMatrices(arxRenderware rwe, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(target);
    AFX_ASSERT(eye);
    AFX_ASSERT(v);
    AfxComputeLookAtMatrix(v, eye, target, AFX_V4D_Y, rwe->leftHandedSpace);

    if (iv)
        AfxM4dInvert(iv, v);
}

_ARX void ArxComputeOrthographicMatrices(arxRenderware rwe, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(extent);
    AFX_ASSERT(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeOrthographicMatrix(p, extent, near, far, rwe->leftHandedSpace, rwe->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeOffcenterOrthographicMatrices(arxRenderware rwe, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeOffcenterOrthographicMatrix(p, left, right, bottom, top, near, far, rwe->leftHandedSpace, rwe->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeBoundingOrthographicMatrices(arxRenderware rwe, afxBox const aabb, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    //AFX_ASSERT(aabb); // afxBox deve estar no view space.
    AFX_ASSERT(p);
    AfxComputeBoundingOrthographicMatrix(p, aabb, rwe->leftHandedSpace, rwe->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeBasicOrthographicMatrices(arxRenderware rwe, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(aspectRatio); // w/h
    AFX_ASSERT(p);
    AfxComputeBasicOrthographicMatrix(p, aspectRatio, scale, range, rwe->leftHandedSpace, rwe->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputePerspectiveMatrices(arxRenderware rwe, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(extent);
    AFX_ASSERT(!AfxRealIsEqual(extent[0], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(extent[1], 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AFX_ASSERT(p);
    AfxComputePerspectiveMatrix(p, extent, near, far, rwe->leftHandedSpace, rwe->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeFovMatrices(arxRenderware rwe, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(!AfxRealIsEqual(fovY, 0.0f, 0.00001f * 2.0f));
    AFX_ASSERT(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AFX_ASSERT(p);
    AfxComputeFovPerspectiveMatrix(p, fovY, aspectRatio, near, far, rwe->leftHandedSpace, rwe->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeFrustrumMatrices(arxRenderware rwe, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(!AfxRealIsEqual(right, left, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(top, bottom, 0.00001f));
    AFX_ASSERT(!AfxRealIsEqual(far, near, 0.00001f));
    AFX_ASSERT(near > 0.f && far > 0.f);
    AFX_ASSERT(p);
    AfxComputeOffcenterPerspectiveMatrix(p, left, right, bottom, top, near, far, rwe->leftHandedSpace, rwe->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX void ArxComputeBasicPerspectiveMatrices(arxRenderware rwe, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(!AfxRealIsEqual(aspectRatio, 0.0f, 0.00001f));
    AFX_ASSERT(p);
    AfxComputeBasicPerspectiveMatrix(p, aspectRatio, range, rwe->leftHandedSpace, rwe->clipSpaceDepth);

    if (ip)
        AfxM4dInvert(ip, p);
}

_ARX afxError ArxUplinkTxds(arxRenderware rwe, afxUnit baseSlot, afxUnit slotCnt, afxUri const uris[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT_RANGE(8, baseSlot, slotCnt);
    
    if (!uris)
    {
        for (afxUnit i = 0; i < slotCnt; i++)
        {
            AfxClearUris(1, &rwe->txdUris[i].uri);
        }
    }
    else
    {
        for (afxUnit i = 0; i < slotCnt; i++)
        {
            AfxCopyUri(&rwe->txdUris[i].uri, &uris[i]);
        }
    }
    return err;
}

_ARX afxError _ArxRweDtorCb(arxRenderware rwe)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);

    AfxExhaustChainedClasses(&rwe->classes);

    AFX_ASSERT(!rwe->idd);

    AfxDeregisterChainedClasses(&rwe->classes);

    return err;
}

_ARX afxError _ArxRweCtorCb(arxRenderware rwe, void** args, afxUnit invokeNo)
{
    //AfxEntry("rwe=%p,uri=%.*s", rwe, endpoint ? AfxPushString(AfxGetUriString(endpoint)) : &AFX_STRING_EMPTY);
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);

    AfxZero(rwe, sizeof(rwe[0]));

    afxModule drv = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &drv);
    AFX_ASSERT(args[1]);
    _arxRweAcquisition const* cfg = ((_arxRweAcquisition const *)args[1]) + invokeNo;

    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &cfg->dsys);
    rwe->dsys = cfg->dsys;

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxChain *classes = &rwe->classes;
    AfxDeployChain(classes, (void*)rwe);

    afxUnit defStreamSiz = 32000000; // 32MB (I think it is the total of RAM in PlayStation 2)

    rwe->minIdxBufSiz = defStreamSiz / 4;
    rwe->maxIdxBufSiz = 0;
    rwe->minVtxBufSiz = defStreamSiz - rwe->minIdxBufSiz;
    rwe->maxVtxBufSiz = 0;

    rwe->udd = cfg && cfg->cfg.udd ? cfg->cfg.udd : NIL;

    rwe->procCb = cfg && cfg->cfg.proc ? cfg->cfg.proc : NIL;

    afxClassConfig camClsCfg = /*cfg->camClsCfg ? *cfg->camClsCfg :*/ _ARX_CAM_CLASS_CONFIG;
    AFX_ASSERT(camClsCfg.fcc == afxFcc_CAM);
    AfxMountClass(&rwe->camCls, NIL, classes, &camClsCfg);

    afxClassConfig tmpClsConf;

    tmpClsConf = _ARX_VBUF_CLASS_CONFIG;
    AfxMountClass(&rwe->vbuffers, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AvxGeomClsCfg;
    //AfxMountClass(&rwe->geomCls, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AvxMshtClsCfg;
    //AfxMountClass(&rwe->mshtCls, NIL, classes, &tmpClsConf);

    //tmpClsConf = _AVX_TEX_CLASS_CONFIG;
    //AfxMountClass(&rwe->texCls, NIL, classes, &tmpClsConf);

    afxClassConfig sbufClsCfg = cfg->sbufClsCfg ? *cfg->sbufClsCfg : _ARX_SBUF_CLASS_CONFIG;
    AFX_ASSERT(sbufClsCfg.fcc == afxFcc_SBUF);
    AfxMountClass(&rwe->sbufCls, NIL, classes, &sbufClsCfg);

    afxClassConfig mtlClsCfg = /*cfg->mtlClsCfg ? *cfg->mtlClsCfg :*/ _ARX_MTL_CLASS_CONFIG;
    AFX_ASSERT(mtlClsCfg.fcc == afxFcc_MTL);
    AfxMountClass(&rwe->mtlCls, NIL, classes, &mtlClsCfg); // require tex

    afxClassConfig mshClsCfg = /*cfg->mshClsCfg ? *cfg->mshClsCfg :*/ _ARX_MSH_CLASS_CONFIG;
    AFX_ASSERT(mshClsCfg.fcc == afxFcc_MSH);
    AfxMountClass(&rwe->mshCls, NIL, classes, &mshClsCfg); // require msht, vtd

    afxClassConfig mdlClsCfg = /*cfg->mdlClsCfg ? *cfg->mdlClsCfg :*/ _ARX_MDL_CLASS_CONFIG;
    AFX_ASSERT(mdlClsCfg.fcc == afxFcc_MDL);
    AfxMountClass(&rwe->mdlCls, NIL, classes, &mdlClsCfg); // require skl, msh

    afxClassConfig terClsCfg = /*cfg->terClsCfg ? *cfg->terClsCfg :*/ _ARX_TER_CLASS_CONFIG;
    AFX_ASSERT(terClsCfg.fcc == afxFcc_TER);
    AfxMountClass(&rwe->terCls, NIL, classes, &terClsCfg); // require all

    afxClassConfig rctxClsCfg = /*cfg->rctxClsCfg ? *cfg->rctxClsCfg :*/ _ARX_RCTX_CLASS_CONFIG;
    AFX_ASSERT(rctxClsCfg.fcc == afxFcc_RCTX);
    AfxMountClass(&rwe->rctxCls, NIL, classes, &rctxClsCfg); // require all

    rwe->strbMdlSklMotUrns = NIL;
    rwe->strbJointBiasesTags = NIL;
    rwe->strbMorphTags = NIL;

    AfxAcquireStringCatalogs(1, &rwe->strbMdlSklMotUrns);
    AfxAcquireStringCatalogs(1, &rwe->strbJointBiasesTags);
    AfxAcquireStringCatalogs(1, &rwe->strbMorphTags);


    afxClassConfig clsCfg;



    tmpClsConf = _ARX_DTEC_CLASS_CONFIG;
    AfxMountClass(&rwe->dtecCls, NIL, classes, &tmpClsConf);

    tmpClsConf = cfg->poseClsCfg ? *cfg->poseClsCfg : _ARX_POSE_CLASS_CONFIG;
    AFX_ASSERT(tmpClsConf.fcc == afxFcc_POSE);
    AfxMountClass(&rwe->poseCls, NIL, classes, &tmpClsConf);

    tmpClsConf = cfg->plceClsCfg ? *cfg->plceClsCfg : _ARX_PLCE_CLASS_CONFIG;
    AFX_ASSERT(tmpClsConf.fcc == afxFcc_PLCE);
    AfxMountClass(&rwe->plceCls, NIL, classes, &tmpClsConf);

    
    tmpClsConf = cfg->litClsCfg ? *cfg->litClsCfg : _ARX_LIT_CLASS_CONFIG;
    AFX_ASSERT(tmpClsConf.fcc == afxFcc_LIT);
    AfxMountClass(&rwe->lights, NIL, classes, &tmpClsConf);

    tmpClsConf = cfg->rndClsCfg ? *cfg->rndClsCfg : _ARX_RND_CLASS_CONFIG;
    AFX_ASSERT(tmpClsConf.fcc == afxFcc_RND);
    AfxMountClass(&rwe->renderers, NIL, classes, &tmpClsConf); // require all

    tmpClsConf = _ARX_SCN_CLASS_CONFIG;
    AfxMountClass(&rwe->scnCls, NIL, classes, &tmpClsConf);

    rwe->vbMgr = NIL;

    rwe->leftHandedSpace = FALSE;
    rwe->clipSpaceDepth = avxClipSpaceDepth_NEG_ONE_TO_ONE; // TODO: Get this from dsys

    return err;
}

_ARX afxClassConfig const _ARX_RWE_CLASS_CONFIG =
{
    .fcc = afxFcc_RWE,
    .name = "Renderware",
    .desc = "Draw Input Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(arxRenderware)),
    .ctor = (void*)_ArxRweCtorCb,
    .dtor = (void*)_ArxRweDtorCb
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
    AFX_ASSERT_CLASS(cls, afxFcc_RWE);

    _arxRweAcquisition cfg2 = { 0 };
    cfg2.cfg = *cfg;
    cfg2.dsys = dsys;

    arxRenderware rwe;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&rwe, (void const*[]) { driver, &cfg2 }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(input);
    *input = rwe;
    return err;
}
