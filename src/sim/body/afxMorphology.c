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

#define _ASX_DRAW_C
#define _ASX_MORPHOLOGY_C
#define _ASX_MATERIAL_C
#include "../impl/asxImplementation.h"

_ASX afxClass const* _AsxMorpGetMaterialClass(afxMorphology morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = &morp->mtlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);
    return cls;
}

_ASX afxClass const* _AsxMorpGetMeshClass(afxMorphology morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = &morp->mshCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MSH);
    return cls;
}

_ASX afxClass const* _AsxMorpGetModelClass(afxMorphology morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = &morp->mdlCls;
    AFX_ASSERT_CLASS(cls, afxFcc_MDL);
    return cls;
}

_ASX afxClass const* _AsxMorpGetTerrainClass(afxMorphology morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = &morp->terCls;
    AFX_ASSERT_CLASS(cls, afxFcc_TER);
    return cls;
}

_ASX afxStringBase _AsxMorpGetModelUrnStringBase(afxMorphology morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    return morp->strbMdlSklMotUrns;
}

_ASX afxStringBase _AsxMorpGetPivotTagStringBase(afxMorphology morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    return morp->strbJointBiasesTags;
}

_ASX afxStringBase _AsxMorpGetMorphTagStringBase(afxMorphology morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    return morp->strbMorphTags;
}

_ASX afxError _AsxMorpDtorCb(afxMorphology morp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    AfxDeregisterChainedClasses(&morp->classes);

    return err;
}

_ASX afxError _AsxMorpCtorCb(afxMorphology morp, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    afxSimulation sim = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    afxString const* id = AFX_CAST(afxString const*, args[1]) + invokeNo;

    afxChain* classes = &morp->classes;
    AfxDeployChain(classes, (void*)morp);

    afxClassConfig mtlClsCfg = /*cfg->mtlClsCfg ? *cfg->mtlClsCfg :*/ _ASX_MTL_CLASS_CONFIG;
    AFX_ASSERT(mtlClsCfg.fcc == afxFcc_MTL);
    AfxMountClass(&morp->mtlCls, NIL, classes, &mtlClsCfg); // require tex

    afxClassConfig mshClsCfg = /*cfg->mshClsCfg ? *cfg->mshClsCfg :*/ _ASX_MSH_CLASS_CONFIG;
    AFX_ASSERT(mshClsCfg.fcc == afxFcc_MSH);
    AfxMountClass(&morp->mshCls, NIL, classes, &mshClsCfg); // require msht, vtd

    afxClassConfig mdlClsCfg = /*cfg->mdlClsCfg ? *cfg->mdlClsCfg :*/ _ASX_MDL_CLASS_CONFIG;
    AFX_ASSERT(mdlClsCfg.fcc == afxFcc_MDL);
    AfxMountClass(&morp->mdlCls, NIL, classes, &mdlClsCfg); // require skl, msh

    afxClassConfig terClsCfg = /*cfg->terClsCfg ? *cfg->terClsCfg :*/ _ASX_TER_CLASS_CONFIG;
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

_ASX afxClassConfig const _ASX_MORP_CLASS_CONFIG =
{
    .fcc = afxFcc_MORP,
    .name = "Morphology",
    .desc = "Morphology",
    .fixedSiz = sizeof(AFX_OBJECT(afxMorphology)),
    .ctor = (void*)_AsxMorpCtorCb,
    .dtor = (void*)_AsxMorpDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxAcquireMorphology(afxSimulation sim, afxMorphology* morphology)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);

    afxClass* cls = (afxClass*)_AsxSimGetMaterialityClass(sim);
    AFX_ASSERT_CLASS(cls, afxFcc_MORP);

    afxMorphology morp;
    if (AfxAcquireObjects(cls, 1, (afxObject*)&morp, (void const*[]) { sim }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    AFX_ASSERT(morphology);
    *morphology = morp;

    return err;
}

_ASX afxBool _AsxFindNamedMtlCb(afxMaterial mtl, void* udd)
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

_ASX afxUnit AfxFindMaterialIndices(afxMorphology morp, afxUnit cnt, afxString const materials[], afxUnit indices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);

    afxClass* cls = (afxClass*)_AsxMorpGetMaterialClass(morp);
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

    rslt = AfxInvokeObjects(cls, 0, cnt, (void*)_AsxFindNamedMtlCb, &udd2);

    return rslt;
}

_ASX afxError AfxDeclareMaterials(afxMorphology morp, afxUnit cnt, afxString const ids[], afxMaterial materials[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    AFX_ASSERT(materials);
    AFX_ASSERT(ids);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AsxMorpGetMaterialClass(morp);
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)materials, (void const*[]) { morp, ids }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_ASX afxUnit AfxEnumerateMaterials(afxMorphology morp, afxUnit first, afxUnit cnt, afxMaterial materials[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxClass const* cls = _AsxMorpGetMaterialClass(morp);
    AFX_ASSERT_CLASS(cls, afxFcc_MTL);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)materials);
}
