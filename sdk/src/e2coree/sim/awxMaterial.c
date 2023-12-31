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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */


#define _AFX_SIM_C
#define _AFX_SIMULATION_C
#define _AFX_MATERIAL_C
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/math/afxVector.h"

_AFX void AfxColorizeMaterial(awxMaterial mtl, afxV4d const color)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssert(color);
    AfxCopyV4d(mtl->color, color);
}

_AFX void AfxShineMaterial(awxMaterial mtl, afxReal shininess)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    mtl->shininess = shininess;
}

_AFX afxRaster AfxGetMaterialTexture(awxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    afxRaster tex = mtl->tex;
    AfxTryAssertObjects(1, &tex, afxFcc_RAS);
    return tex;
}

_AFX void AfxRebindMaterialTexture(awxMaterial mtl, afxRaster tex)
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

_AFX void AfxReloadMaterialTexture(awxMaterial mtl, afxUri const *tex)
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

        if (AfxLoadRastersFromTarga(sim->dctx, afxRasterUsage_SAMPLING, NIL, 1, tex, &mtl->tex)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &mtl->tex, afxFcc_RAS);
        }
    }
}

_AFX afxNat AfxCountMaterialMaps(awxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    return mtl->mapCnt;
}

_AFX awxMaterial AfxFindSubmaterial(awxMaterial mtl, afxString const *usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssertType(usage, afxFcc_STR);

    for (afxNat i = 0; i < mtl->mapCnt; i++)
    {
        if (0 == AfxCompareStringCi(usage, &mtl->maps[i].usage.str))
        {
            awxMaterial subMtl = mtl->maps[i].sub;
            AfxTryAssertObjects(1, &subMtl, afxFcc_MTL);
            return subMtl;
        }
    }
    return NIL;
}

_AFX awxMaterial AfxGetSubmaterial(awxMaterial mtl, afxNat mapIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssert(mapIdx < mtl->mapCnt);
    awxMaterial subMtl = mtl->maps[mapIdx].sub;
    AfxTryAssertObjects(1, &subMtl, afxFcc_MTL);
    return subMtl;
}

_AFX void AfxRebindSubmaterial(awxMaterial mtl, afxNat mapIdx, awxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("mtl=%p,mapIdx=%u,subMtl=%p", mtl, mapIdx, subMtl);

    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssert(mapIdx < mtl->mapCnt);
    awxMaterialMap* map = &mtl->maps[mapIdx];

    awxMaterial subMtl2 = map->sub;

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

_AFX void AfxResetMaterialMap(awxMaterial mtl, afxNat mapIdx, afxString const* usage, awxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("mtl=%p,mapIdx=%u,subMtl=%p", mtl, mapIdx, subMtl);

    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxAssertRange(mtl->mapCnt, mapIdx, 1);
    awxMaterialMap* map = &mtl->maps[mapIdx];

    AfxDeallocateString(&map->usage);
    AfxCloneString(&map->usage, usage);

    awxMaterial subMtl2 = map->sub;

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

_AFX afxString const* AfxGetMaterialId(awxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    return &mtl->id.str;
}

_AFX afxError _AfxMtlDtor(awxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxEntry("mtl=%p", mtl);

    if (mtl->maps)
    {
        afxSimulation sim = AfxGetObjectProvider(mtl);
        afxMmu mmu = AfxSimulationGetMemory(sim);
        awxMaterial sub;

        for (afxNat i = 0; i < mtl->mapCnt; i++)
        {
            if ((sub = mtl->maps[i].sub))
                AfxReleaseObjects(1, (void*[]) { sub });
        }

        AfxDeallocate(mmu, mtl->maps);
    }

    if (mtl->tex)
        AfxReleaseObjects(1, (void*[]) { mtl->tex });

    AfxDeallocateString(&mtl->id);

    return err;
}

_AFX afxError _AfxMtlCtor(awxMaterial mtl, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AfxEntry("mtl=%p", mtl);

    afxSimulation sim = cookie->udd[0];
    AfxAssert(sim == AfxGetObjectProvider(mtl));
    afxString const* id = cookie->udd[1];
    afxRaster tex = cookie->udd[2];
    afxNat mapCnt = *((afxNat const *)cookie->udd[2]);
    
    AfxCloneString(&mtl->id, id);
    
    AfxZeroV4d(mtl->color);
    mtl->shininess = 0.5f;

    afxMmu mmu = AfxSimulationGetMemory(sim);

    //afxNat mapCnt = AfxCountArrayElements(&blueprint->maps);

    if (!mapCnt)
    {
        mtl->mapCnt = 0;
        mtl->maps = NIL;
    }
    else if (!(mtl->maps = AfxAllocate(mmu, sizeof(mtl->maps[0]), mapCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < mapCnt; i++)
        {
            //awxMaterialBlueprintMap *mapBp = AfxGetArrayUnit(&blueprint->maps, i);
            
            awxMaterialMap *map = &mtl->maps[i];
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

        AfxDeallocate(mmu, mtl->maps);
    }

    return err;
}

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireMaterial(afxSimulation sim, afxString const* id, afxRaster tex, afxNat subCnt, awxMaterial *mtl)
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

_AFX afxClassConfig _AfxMtlClsConfig =
{
    .fcc = afxFcc_MTL,
    .name = "Material",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(awxMaterial)),
    .mmu = NIL,
    .ctor = (void*)_AfxMtlCtor,
    .dtor = (void*)_AfxMtlDtor
};
