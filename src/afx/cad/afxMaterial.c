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


#define _AFX_SIM_C
#define _AFX_SIMULATION_C
#define _AFX_MATERIAL_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxVector.h"

_AKX void AfxColorizeMaterial(afxMaterial mtl, afxV4d const color)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssert(color);
    AfxCopyV4d(mtl->color, color);
}

_AKX void AfxShineMaterial(afxMaterial mtl, afxReal shininess)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    mtl->shininess = shininess;
}

_AKX afxRaster AfxGetMaterialTexture(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    afxRaster tex = mtl->tex;
    AfxTryAssertObjects(1, &tex, afxFcc_RAS);
    return tex;
}

_AKX void AfxRebindMaterialTexture(afxMaterial mtl, afxRaster tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);

    afxRaster tex2 = mtl->tex;

    if (tex2)
    {
        AfxAssertObjects(1, &tex2, afxFcc_RAS);
        AfxReleaseObjects(1, (void*[]) { mtl->tex });
    }

    if (tex)
    {
        AfxAssertObjects(1, &tex, afxFcc_RAS);
        
        if (!(AfxReacquireObjects(1, (void*[]) { tex })))
            AfxThrowError();
    }
}

_AKX void AfxReloadMaterialTexture(afxMaterial mtl, afxUri const *tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);

    afxSimulation sim = AfxGetObjectProvider(mtl);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    afxRaster tex2 = mtl->tex;

    if (tex2)
    {
        AfxAssertObjects(1, &tex2, afxFcc_RAS);
        AfxReleaseObjects(1, (void*[]) { mtl->tex });
    }

    mtl->tex = NIL;

    if (tex)
    {
        AfxAssertObjects(1, &tex, afxFcc_RAS);

        if (AfxLoadRasters(sim->dctx, afxRasterUsage_SAMPLING, NIL, 1, tex, &mtl->tex)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &mtl->tex, afxFcc_RAS);
        }
    }
}

_AKX afxNat AfxCountMaterialMaps(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    return mtl->mapCnt;
}

_AKX afxMaterial AfxFindSubmaterial(afxMaterial mtl, afxString const *usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssert(usage);

    for (afxNat i = 0; i < mtl->mapCnt; i++)
    {
        if (0 == AfxCompareStringCi(usage, &mtl->maps[i].usage.str))
        {
            afxMaterial subMtl = mtl->maps[i].sub;
            AfxTryAssertObjects(1, &subMtl, afxFcc_MTL);
            return subMtl;
        }
    }
    return NIL;
}

_AKX afxMaterial AfxGetSubmaterial(afxMaterial mtl, afxNat mapIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssert(mapIdx < mtl->mapCnt);
    afxMaterial subMtl = mtl->maps[mapIdx].sub;
    AfxTryAssertObjects(1, &subMtl, afxFcc_MTL);
    return subMtl;
}

_AKX void AfxRebindSubmaterial(afxMaterial mtl, afxNat mapIdx, afxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssert(mapIdx < mtl->mapCnt);
    afxMaterialMap* map = &mtl->maps[mapIdx];

    afxMaterial subMtl2 = map->sub;

    if (subMtl2)
    {
        AfxAssertObjects(1, &subMtl2, afxFcc_MTL);
        AfxReleaseObjects(1, (void*[]) { subMtl2 });
    }

    if (subMtl)
    {
        AfxAssertObjects(1, &subMtl, afxFcc_MTL);

        if (!(AfxReacquireObjects(1, (void*[]) { subMtl })))
            AfxThrowError();

        map->sub = subMtl;
    }
}

_AKX void AfxResetMaterialMap(afxMaterial mtl, afxNat mapIdx, afxString const* usage, afxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssertRange(mtl->mapCnt, mapIdx, 1);
    afxMaterialMap* map = &mtl->maps[mapIdx];

    AfxDeallocateString(&map->usage);
    AfxCloneString(&map->usage, usage);

    afxMaterial subMtl2 = map->sub;

    if (subMtl2)
    {
        AfxAssertObjects(1, &subMtl2, afxFcc_MTL);
        AfxReleaseObjects(1, (void*[]) { subMtl2 });
    }

    if (subMtl)
    {
        AfxAssertObjects(1, &subMtl, afxFcc_MTL);

        if (!(AfxReacquireObjects(1, (void*[]) { subMtl })))
            AfxThrowError();

        map->sub = subMtl;
    }
}

_AKX afxString const* AfxGetMaterialId(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    return &mtl->id.str;
}

_AKX afxError _AkxMtlDtor(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);

    if (mtl->maps)
    {
        afxSimulation sim = AfxGetObjectProvider(mtl);
        afxMmu mmu = AfxGetSimulationMmu(sim);
        afxMaterial sub;

        for (afxNat i = 0; i < mtl->mapCnt; i++)
        {
            if ((sub = mtl->maps[i].sub))
                AfxReleaseObjects(1, (void*[]) { sub });
        }

        AfxDeallocate(mtl->maps);
    }

    if (mtl->tex)
        AfxReleaseObjects(1, (void*[]) { mtl->tex });

    AfxDeallocateString(&mtl->id);

    return err;
}

_AKX afxError _AkxMtlCtor(afxMaterial mtl, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);

    afxSimulation sim = cookie->udd[0];
    AfxAssert(sim == AfxGetObjectProvider(mtl));
    afxString const* id = cookie->udd[1];
    afxRaster tex = cookie->udd[2];
    afxNat mapCnt = *((afxNat const *)cookie->udd[2]);
    
    AfxCloneString(&mtl->id, id);
    
    AfxZeroV4d(mtl->color);
    mtl->shininess = 0.5f;

    afxMmu mmu = AfxGetSimulationMmu(sim);

    //afxNat mapCnt = AfxCountArrayElements(&blueprint->maps);

    if (!mapCnt)
    {
        mtl->mapCnt = 0;
        mtl->maps = NIL;
    }
    else if (!(mtl->maps = AfxAllocate(mapCnt, sizeof(mtl->maps[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < mapCnt; i++)
        {
            //afxMaterialBlueprintMap *mapBp = AfxGetArrayUnit(&blueprint->maps, i);
            
            afxMaterialMap *map = &mtl->maps[i];
            //AfxCloneString(&map->usage, &(mapBp->semantic.str));
            AfxResetString(&map->usage.str);
            
#if 0
            if (AfxUriIsBlank(&mapBp->uri.uri)) map->sub = NIL;
            else
            {
                if (AfxAcquireMaterials(sim, 1, &map->sub, &mapBp->uri.uri))
                    AfxThrowError();
            }
#endif
            map->sub = NIL;

            ++mtl->mapCnt;
        }
        AfxAssert(mtl->mapCnt == mapCnt);
    }

    if (!err)
    {
#if 0
        if (AfxUriIsBlank(&blueprint->tex.uri)) mtl->tex = NIL;
        else
        {
            if (AfxLoadTextures(sim->dctx, 1, &mtl->tex, &blueprint->tex.uri)) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &mtl->tex, afxFcc_RAS);
            }
        }
#endif
        if ((mtl->tex = tex))
        {
            AfxReacquireObjects(1, (void*[]) { tex });
        }
    }

    if (err && mtl->maps)
    {
        for (afxNat i = 0; i < mtl->mapCnt; i++)
            if (mtl->maps[i].sub)
                AfxReleaseObjects(1, (void*[]) { mtl->maps[i].sub });

        AfxDeallocate(mtl->maps);
    }

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AKX afxError AfxAcquireMaterial(afxSimulation sim, afxString const* id, afxRaster tex, afxNat subCnt, afxMaterial *mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AfxAssert(subCnt);
    AfxAssert(mtl);
    AfxAssert(id);

    if (AfxAcquireObjects(&sim->materials, 1, (afxObject*)mtl, (void const*[]) { sim, id, tex, &subCnt }))
        AfxThrowError();

    return err;
}

_AKX afxClassConfig _AkxMtlMgrCfg =
{
    .fcc = afxFcc_MTL,
    .name = "Material",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxMaterial)),
    .mmu = NIL,
    .ctor = (void*)_AkxMtlCtor,
    .dtor = (void*)_AkxMtlDtor
};
