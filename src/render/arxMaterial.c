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

#define _ARX_DRAW_C
#define _ARX_MATERIAL_C
#include "../ddi/arxImpl_Input.h"

_ARX void ArxColorizeMaterial(arxMaterial mtl, afxV4d const color)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(color);
    AfxV4dCopy(mtl->color, color);
}

_ARX void ArxShineMaterial(arxMaterial mtl, afxReal shininess)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    mtl->shininess = shininess;
}

_ARX avxRaster ArxGetMaterialTexture(arxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    avxRaster tex = mtl->tex;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_RAS, 1, &tex);
    return tex;
}

_ARX void ArxRebindMaterialTexture(arxMaterial mtl, avxRaster tex)
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

_ARX void ArxReloadMaterialTexture(arxMaterial mtl, afxUri const *tex)
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

        arxRenderware din = AfxGetProvider(mtl);
        AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

        afxDrawSystem dsys = ArxGetDrawInputContext(din);

        avxRasterInfo rasi = { 0 };
        rasi.usage = avxRasterUsage_TEXTURE;

        if (AvxLoadRasters(dsys, 1, &rasi, &tex[0], &mtl->tex)) AfxThrowError();
        else
        {
            AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &mtl->tex);
        }
    }
}

_ARX afxUnit ArxCountMaterialMaps(arxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    return mtl->mapCnt;
}

_ARX arxMaterial ArxFindSubmaterial(arxMaterial mtl, afxString const *usage)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(usage);

    for (afxUnit i = 0; i < mtl->mapCnt; i++)
    {
        if (AfxCompareStrings(usage, 0, TRUE, 1, &mtl->maps[i].usage, NIL))
        {
            arxMaterial subMtl = mtl->maps[i].sub;
            AFX_TRY_ASSERT_OBJECTS(afxFcc_MTL, 1, &subMtl);
            return subMtl;
        }
    }
    return NIL;
}

_ARX arxMaterial ArxGetSubmaterial(arxMaterial mtl, afxUnit mapIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(mapIdx < mtl->mapCnt);
    arxMaterial subMtl = mtl->maps[mapIdx].sub;
    AFX_TRY_ASSERT_OBJECTS(afxFcc_MTL, 1, &subMtl);
    return subMtl;
}

_ARX void ArxRebindSubmaterial(arxMaterial mtl, afxUnit mapIdx, arxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT(mapIdx < mtl->mapCnt);
    arxMaterialMap* map = &mtl->maps[mapIdx];

    arxMaterial subMtl2 = map->sub;

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

_ARX void ArxResetMaterialMap(arxMaterial mtl, afxUnit mapIdx, afxString const* usage, arxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    AFX_ASSERT_RANGE(mtl->mapCnt, mapIdx, 1);
    arxMaterialMap* map = &mtl->maps[mapIdx];

    AfxDeallocateString(&map->usage);
    AfxCloneString(&map->usage, usage);

    arxMaterial subMtl2 = map->sub;

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

_ARX afxString const* ArxGetMaterialUrn(arxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);
    return &mtl->urn;
}

_ARX afxError _ArxMtlDtorCb(arxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    if (mtl->maps)
    {
        arxMaterial sub;

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

_ARX afxError _ArxMtlCtorCb(arxMaterial mtl, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MTL, 1, &mtl);

    arxGeome morp = args[0];
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
            
            arxMaterialMap *map = &mtl->maps[i];
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

_ARX afxClassConfig const _ARX_MTL_CLASS_CONFIG =
{
    .fcc = afxFcc_MTL,
    .name = "Material",
    .desc = "Material",
    .fixedSiz = sizeof(AFX_OBJECT(arxMaterial)),
    .ctor = (void*)_ArxMtlCtorCb,
    .dtor = (void*)_ArxMtlDtorCb
};
