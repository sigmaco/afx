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


#define _AMX_DRAW_C
#define _AMX_MATERIAL_C
#include "../dev/AmxCadImplKit.h"

_AMX void AfxColorizeMaterial(afxMaterial mtl, afxV4d const color)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AFX_ASSERT(color);
    AfxV4dCopy(mtl->color, color);
}

_AMX void AfxShineMaterial(afxMaterial mtl, afxReal shininess)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    mtl->shininess = shininess;
}

_AMX afxRaster AfxGetMaterialTexture(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    afxRaster tex = mtl->tex;
    AfxTryAssertObjects(1, &tex, afxFcc_RAS);
    return tex;
}

_AMX void AfxRebindMaterialTexture(afxMaterial mtl, afxRaster tex)
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

_AMX void AfxReloadMaterialTexture(afxMaterial mtl, afxUri const *tex)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);

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

        afxDrawInput din = AfxGetProvider(mtl);
        AfxAssertObjects(1, &din, afxFcc_DIN);

        afxDrawContext dctx;
        AfxGetDrawInputContext(din, &dctx);

        afxRasterInfo rasi = { 0 };
        rasi.usage = afxRasterUsage_SAMPLING;

        if (AfxLoadRasters(dctx, 1, &rasi, &tex[0], &mtl->tex)) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &mtl->tex, afxFcc_RAS);
        }
    }
}

_AMX afxUnit AfxCountMaterialMaps(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    return mtl->mapCnt;
}

_AMX afxMaterial AfxFindSubmaterial(afxMaterial mtl, afxString const *usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AFX_ASSERT(usage);

    for (afxUnit i = 0; i < mtl->mapCnt; i++)
    {
        if (0 == AfxCompareStrings(usage, TRUE, 1, &mtl->maps[i].usage))
        {
            afxMaterial subMtl = mtl->maps[i].sub;
            AfxTryAssertObjects(1, &subMtl, afxFcc_MTL);
            return subMtl;
        }
    }
    return NIL;
}

_AMX afxMaterial AfxGetSubmaterial(afxMaterial mtl, afxUnit mapIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AFX_ASSERT(mapIdx < mtl->mapCnt);
    afxMaterial subMtl = mtl->maps[mapIdx].sub;
    AfxTryAssertObjects(1, &subMtl, afxFcc_MTL);
    return subMtl;
}

_AMX void AfxRebindSubmaterial(afxMaterial mtl, afxUnit mapIdx, afxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AFX_ASSERT(mapIdx < mtl->mapCnt);
    akxMaterialMap* map = &mtl->maps[mapIdx];

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

_AMX void AfxResetMaterialMap(afxMaterial mtl, afxUnit mapIdx, afxString const* usage, afxMaterial subMtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    AFX_ASSERT_RANGE(mtl->mapCnt, mapIdx, 1);
    akxMaterialMap* map = &mtl->maps[mapIdx];

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

_AMX afxString const* AfxGetMaterialUrn(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);
    return &mtl->urn;
}

_AMX afxError _AmxMtlDtorCb(afxMaterial mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);

    if (mtl->maps)
    {
        afxMaterial sub;

        for (afxUnit i = 0; i < mtl->mapCnt; i++)
        {
            if ((sub = mtl->maps[i].sub))
                AfxReleaseObjects(1, (void*[]) { sub });
        }

        AfxDeallocate(mtl->maps);
    }

    if (mtl->tex)
        AfxReleaseObjects(1, (void*[]) { mtl->tex });

    AfxDeallocateString(&mtl->urn);

    return err;
}

_AMX afxError _AmxMtlCtorCb(afxMaterial mtl, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mtl, afxFcc_MTL);

    afxSimulation sim = args[0];
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxString const* id = args[1];
    afxRaster tex = args[2];
    afxUnit mapCnt = *((afxUnit const *)args[2]);
    
    AfxCloneString(&mtl->urn, id);
    
    AfxV4dZero(mtl->color);
    mtl->shininess = 0.5f;

    //afxUnit mapCnt = AfxGetArrayPop(&blueprint->maps);

    if (!mapCnt)
    {
        mtl->mapCnt = 0;
        mtl->maps = NIL;
    }
    else if (!(mtl->maps = AfxAllocate(mapCnt, sizeof(mtl->maps[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxUnit i = 0; i < mapCnt; i++)
        {
            //akxMaterialBlueprintMap *mapBp = AfxGetArrayUnit(&blueprint->maps, i);
            
            akxMaterialMap *map = &mtl->maps[i];
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
        for (afxUnit i = 0; i < mtl->mapCnt; i++)
            if (mtl->maps[i].sub)
                AfxReleaseObjects(1, (void*[]) { mtl->maps[i].sub });

        AfxDeallocate(mtl->maps);
    }

    return err;
}

_AMX afxClassConfig _AmxMtlClsCfg =
{
    .fcc = afxFcc_MTL,
    .name = "Material",
    .desc = "Material",
    .fixedSiz = sizeof(AFX_OBJECT(afxMaterial)),
    .ctor = (void*)_AmxMtlCtorCb,
    .dtor = (void*)_AmxMtlDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

_AMX afxError AfxAcquireMaterial(afxSimulation sim, afxString const* id, afxRaster tex, afxUnit subCnt, afxMaterial *mtl)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    AFX_ASSERT(subCnt);
    AFX_ASSERT(mtl);
    AFX_ASSERT(id);

    afxClass* cls = (afxClass*)AfxGetMaterialClass(sim);
    AfxAssertClass(cls, afxFcc_MTL);

    if (AfxAcquireObjects(cls, 1, (afxObject*)mtl, (void const*[]) { sim, id, tex, &subCnt }))
        AfxThrowError();

    return err;
}

_AMX afxUnit AfxEnumerateMaterials(afxSimulation sim, afxUnit first, afxUnit cnt, afxMaterial materials[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    afxClass const* cls = AfxGetMaterialClass(sim);
    AfxAssertClass(cls, afxFcc_MTL);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)materials);
}
