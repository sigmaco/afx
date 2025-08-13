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
#define _ASX_MATERIAL_C
#include "../impl/asxImplementation.h"

_ASX void AfxColorizeMaterial(afxMaterial mtl, afxV4d const color)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(color);
    AfxV4dCopy(mtl->color, color);
}

_ASX void AfxShineMaterial(afxMaterial mtl, afxReal shininess)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    mtl->shininess = shininess;
}

_ASX avxRaster AfxGetMaterialTexture(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    avxRaster tex = mtl->tex;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &tex);
    return tex;
}

_ASX void AfxRebindMaterialTexture(afxMaterial mtl, avxRaster tex)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    avxRaster tex2 = mtl->tex;

    if (tex2)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &tex2);
        AfxDisposeObjects(1, (void*[]) { mtl->tex });
    }

    if (tex)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &tex);
        
        if (!(AfxReacquireObjects(1, (void*[]) { tex })))
            AfxThrowError();
    }
}

_ASX void AfxReloadMaterialTexture(afxMaterial mtl, afxUri const *tex)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    avxRaster tex2 = mtl->tex;

    if (tex2)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &tex2);
        AfxDisposeObjects(1, (void*[]) { mtl->tex });
    }

    mtl->tex = NIL;

    if (tex)
    {
        AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &tex);

        afxDrawInput din = AfxGetProvider(mtl);
        AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

        afxDrawSystem dsys = AvxGetDrawInputContext(din);

        avxRasterInfo rasi = { 0 };
        rasi.usage = avxRasterUsage_TEXTURE;

        if (AvxLoadRasters(dsys, 1, &rasi, &tex[0], &mtl->tex)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &mtl->tex);
        }
    }
}

_ASX afxUnit AfxCountMaterialMaps(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    return mtl->mapCnt;
}

_ASX afxMaterial AfxFindSubmaterial(afxMaterial mtl, afxString const *usage)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(usage);

    for (afxUnit i = 0; i < mtl->mapCnt; i++)
    {
        if (AfxCompareStrings(usage, 0, TRUE, 1, &mtl->maps[i].usage, NIL))
        {
            afxMaterial subMtl = mtl->maps[i].sub;
            AFX_TRY_ASSERT_OBJECTS(afxFcc_MTL, 1, &subMtl);
            return subMtl;
        }
    }
    return NIL;
}

_ASX afxMaterial AfxGetSubmaterial(afxMaterial mtl, afxUnit mapIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(mapIdx < mtl->mapCnt);
    afxMaterial subMtl = mtl->maps[mapIdx].sub;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_MTL, 1, &subMtl);
    return subMtl;
}

_ASX void AfxRebindSubmaterial(afxMaterial mtl, afxUnit mapIdx, afxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(mapIdx < mtl->mapCnt);
    asxMaterialMap* map = &mtl->maps[mapIdx];

    afxMaterial subMtl2 = map->sub;

    if (subMtl2)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &subMtl2);
        AfxDisposeObjects(1, (void*[]) { subMtl2 });
    }

    if (subMtl)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &subMtl);

        if (!(AfxReacquireObjects(1, (void*[]) { subMtl })))
            AfxThrowError();

        map->sub = subMtl;
    }
}

_ASX void AfxResetMaterialMap(afxMaterial mtl, afxUnit mapIdx, afxString const* usage, afxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT_RANGE(mtl->mapCnt, mapIdx, 1);
    asxMaterialMap* map = &mtl->maps[mapIdx];

    AfxDeallocateString(&map->usage);
    AfxCloneString(&map->usage, usage);

    afxMaterial subMtl2 = map->sub;

    if (subMtl2)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &subMtl2);
        AfxDisposeObjects(1, (void*[]) { subMtl2 });
    }

    if (subMtl)
    {
        AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &subMtl);

        if (!(AfxReacquireObjects(1, (void*[]) { subMtl })))
            AfxThrowError();

        map->sub = subMtl;
    }
}

_ASX afxString const* AfxGetMaterialUrn(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    return &mtl->urn;
}

_ASX afxError _AsxMtlDtorCb(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    if (mtl->maps)
    {
        afxMaterial sub;

        for (afxUnit i = 0; i < mtl->mapCnt; i++)
        {
            if ((sub = mtl->maps[i].sub))
                AfxDisposeObjects(1, (void*[]) { sub });
        }

        AfxDeallocate((void**)&mtl->maps, AfxHere());
    }

    if (mtl->tex)
        AfxDisposeObjects(1, (void*[]) { mtl->tex });

    AfxDeallocateString(&mtl->urn);

    return err;
}

_ASX afxError _AsxMtlCtorCb(afxMaterial mtl, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    afxMorphology morp = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MORP, 1, &morp);
    afxString const* id = AFX_CAST(afxString const*, args[1]) + invokeNo;
    
    AfxCloneString(&mtl->urn, id);
    
    AfxV4dZero(mtl->color);
    mtl->shininess = 0.5f;

    //afxUnit mapCnt = AfxGetArrayPop(&blueprint->maps);

    afxUnit mapCnt = 0;

    if (!mapCnt)
    {
        mtl->mapCnt = 0;
        mtl->maps = NIL;
    }
    else if (AfxAllocate(mapCnt * sizeof(mtl->maps[0]), 0, AfxHere(), (void**)&mtl->maps)) AfxThrowError();
    else
    {
        for (afxUnit i = 0; i < mapCnt; i++)
        {
            //akxMaterialBlueprintMap *mapBp = AfxGetArrayUnit(&blueprint->maps, i);
            
            asxMaterialMap *map = &mtl->maps[i];
            //AfxCloneString(&map->usage, &(mapBp->semantic.str));
            AfxResetStrings(1, &map->usage);
            
#if 0
            if (AfxIsUriBlank(&mapBp->uri.uri)) map->sub = NIL;
            else
            {
                if (AfxAcquireMaterials(sim, 1, &map->sub, &mapBp->uri.uri))
                    AfxThrowError();
            }
#endif
            map->sub = NIL;

            ++mtl->mapCnt;
        }
        AFX_ASSERT(mtl->mapCnt == mapCnt);
    }

    if (!err)
    {
#if 0
        if (AfxIsUriBlank(&blueprint->tex.uri)) mtl->tex = NIL;
        else
        {
            if (AfxLoadTextures(sim->dsys, 1, &mtl->tex, &blueprint->tex.uri)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &mtl->tex, afxFcc_RAS);
            }
        }
#endif
        if ((mtl->tex = NIL))
        {
            AfxReacquireObjects(1, (void*[]) { mtl->tex });
        }
    }

    if (err && mtl->maps)
    {
        for (afxUnit i = 0; i < mtl->mapCnt; i++)
            if (mtl->maps[i].sub)
                AfxDisposeObjects(1, (void*[]) { mtl->maps[i].sub });

        AfxDeallocate((void**)&mtl->maps, AfxHere());
    }

    return err;
}

_ASX afxClassConfig const _ASX_MTL_CLASS_CONFIG =
{
    .fcc = afxFcc_MTL,
    .name = "Material",
    .desc = "Material",
    .fixedSiz = sizeof(AFX_OBJECT(afxMaterial)),
    .ctor = (void*)_AsxMtlCtorCb,
    .dtor = (void*)_AsxMtlDtorCb
};
