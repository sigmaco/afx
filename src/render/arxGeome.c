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

#define _ARX_GEOME_C
#define _ARX_MATERIAL_C
#include "../ddi/arxImpl_Input.h"

_ARX afxClass const* _ArxMorpGetMaterialClass(arxGeome morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = &morp->mtlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);
    return cls;
}

_ARX afxClass const* _ArxMorpGetMeshClass(arxGeome morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = &morp->mshCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSH);
    return cls;
}

_ARX afxClass const* _ArxMorpGetModelClass(arxGeome morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = &morp->mdlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);
    return cls;
}

_ARX afxClass const* _ArxMorpGetTerrainClass(arxGeome morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = &morp->terCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TER);
    return cls;
}

_ARX afxStringBase _ArxMorpGetModelUrnStringBase(arxGeome morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    return morp->strbMdlSklMotUrns;
}

_ARX afxStringBase _ArxMorpGetPivotTagStringBase(arxGeome morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    return morp->strbJointBiasesTags;
}

_ARX afxStringBase _ArxMorpGetMorphTagStringBase(arxGeome morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    return morp->strbMorphTags;
}

_ARX afxError _ArxMorpDtorCb(arxGeome morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    AfxDeregisterChainedClasses(&morp->classes);

    return err;
}

_ARX afxError _ArxMorpCtorCb(arxGeome morp, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    arxRenderware din = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxString const* id = AFX_CAST(afxString const*, args[1]) + invokeNo;

    afxChain* classes = &morp->classes;
    AfxDeployChain(classes, (void*)morp);

    afxClassConfig mtlClsCfg = /*cfg->mtlClsCfg ? *cfg->mtlClsCfg :*/ _ARX_MTL_CLASS_CONFIG;
    AFX_ASSERT(mtlClsCfg.fcc == afxFcc_MTL);
    AfxMountClass(&morp->mtlCls, NIL, classes, &mtlClsCfg); // require tex

    afxClassConfig mshClsCfg = /*cfg->mshClsCfg ? *cfg->mshClsCfg :*/ _ARX_MSH_CLASS_CONFIG;
    AFX_ASSERT(mshClsCfg.fcc == afxFcc_MSH);
    AfxMountClass(&morp->mshCls, NIL, classes, &mshClsCfg); // require msht, vtd

    afxClassConfig mdlClsCfg = /*cfg->mdlClsCfg ? *cfg->mdlClsCfg :*/ _ARX_MDL_CLASS_CONFIG;
    AFX_ASSERT(mdlClsCfg.fcc == afxFcc_MDL);
    AfxMountClass(&morp->mdlCls, NIL, classes, &mdlClsCfg); // require skl, msh

    afxClassConfig terClsCfg = /*cfg->terClsCfg ? *cfg->terClsCfg :*/ _ARX_TER_CLASS_CONFIG;
    AFX_ASSERT(terClsCfg.fcc == afxFcc_TER);
    AfxMountClass(&morp->terCls, NIL, classes, &terClsCfg); // require all

    morp->strbMdlSklMotUrns = NIL;
    morp->strbJointBiasesTags = NIL;
    morp->strbMorphTags = NIL;

    AfxAcquireStringCatalogs(1, &morp->strbMdlSklMotUrns);
    AfxAcquireStringCatalogs(1, &morp->strbJointBiasesTags);
    AfxAcquireStringCatalogs(1, &morp->strbMorphTags);

    return err;
}

_ARX afxClassConfig const _ARX_MORP_CLASS_CONFIG =
{
    .fcc = afxFcc_MORP,
    .name = "Morphology",
    .desc = "Morphology",
    .fixedSiz = sizeof(AFX_OBJECT(arxGeome)),
    .ctor = (void*)_ArxMorpCtorCb,
    .dtor = (void*)_ArxMorpDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireMorphology(arxRenderware din, arxGeome* morphology)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    afxClass* cls = (afxClass*)_ArxDinGetGeomClass(din);
    AFX_ASSERT_CLASS(cls, afxFcc_MORP);

    arxGeome morp;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&morp, (void const*[]) { din }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    AFX_ASSERT(morphology);
    *morphology = morp;

    return err;
}

_ARX afxBool _ArxFindNamedMtlCb(arxMaterial mtl, void* udd)
{
    struct
    {
        afxUnit cnt;
        afxString const* materials;
        afxUnit* indices;
        afxUnit rslt;
    } *udd2 = udd;

    afxUnit idx;
    if (AfxCompareStrings(&mtl->urn, 0, TRUE, udd2->cnt, &udd2->materials[udd2->rslt], &idx))
    {
        udd2->indices[idx] = AfxGetObjectId(mtl);
        ++udd2->rslt;
    }
}

_ARX afxUnit ArxFindMaterialIndices(arxGeome morp, afxUnit cnt, afxString const materials[], afxUnit indices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    afxClass* cls = (afxClass*)_ArxMorpGetMaterialClass(morp);
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);

    afxUnit rslt = 0;

    struct
    {
        afxUnit cnt;
        afxString const* materials;
        afxUnit* indices;
        afxUnit rslt;
    }
    udd2 =
    {
        .cnt = cnt,
        .materials = materials,
        .indices = indices,
        .rslt = 0
    };

    rslt = AfxInvokeObjects(cls, 0, cnt, (void*)_ArxFindNamedMtlCb, &udd2);

    return rslt;
}

_ARX afxError ArxDeclareMaterials(arxGeome morp, afxUnit cnt, afxString const ids[], arxMaterial materials[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    AFX_ASSERT(materials);
    AFX_ASSERT(ids);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_ArxMorpGetMaterialClass(morp);
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)materials, (void const*[]) { morp, ids }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_ARX afxUnit ArxEnumerateMaterials(arxGeome morp, afxUnit first, afxUnit cnt, arxMaterial materials[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = _ArxMorpGetMaterialClass(morp);
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)materials);
}
