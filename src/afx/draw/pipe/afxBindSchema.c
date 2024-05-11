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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_BIND_SCHEMA_C
#include "qwadro/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

_AVXINL void AfxLegoBlueprintBegin(afxPipelineRigBlueprint *blueprint, afxNat estBindCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->fcc = afxFcc_SHDB;

    blueprint->dctx = NIL;
    //afxMmu mmu = AfxGetDrawContextMmu(blueprint->dctx);
    //AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAllocateArray(&blueprint->bindings, AfxMax(estBindCnt, 10), sizeof(afxPipelineRigBlueprintBinding), NIL);
}

_AVXINL void AfxLegoBlueprintErase(afxPipelineRigBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->bindings); i++)
    {
        afxPipelineRigBlueprintBinding* binding = AfxGetArrayUnit(&blueprint->bindings, i);

        //if (binding->name)
        {
            AfxDeallocateString(&binding->name);
            //binding->name = NIL;
        }
    }
    AfxEmptyArray(&blueprint->bindings);
}

_AVXINL afxError AfxLegoBlueprintEnd(afxPipelineRigBlueprint *blueprint, afxBindSchema *lego)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);

    if (lego)
    {
        if (AfxCountArrayElements(&blueprint->bindings))
        {
            if (AfxAcquireBindSchemas(blueprint->dctx, 1, lego, blueprint))
            {
                AfxThrowError();
            }
        }
        else
        {
            *lego = NIL;
        }
    }

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->bindings); i++)
    {
        afxPipelineRigBlueprintBinding* binding = AfxGetArrayUnit(&blueprint->bindings, i);

        //if (binding->name)
        {
            AfxDeallocateString(&binding->name);
            //binding->name = NIL;
        }
    }
    AfxDeallocateArray(&blueprint->bindings);

    blueprint->fcc = NIL;
    return err;
}

_AVXINL afxError AfxLegoBlueprintAddBinding(afxPipelineRigBlueprint *blueprint, afxNat point, afxFlags visibility, afxShaderResourceType type, afxNat cnt, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    afxNat idx;
    afxPipelineRigBlueprintBinding *decl;

    if (!(decl = AfxInsertArrayUnit(&blueprint->bindings, &idx))) AfxThrowError();
    else
    {
        AfxAssert(point < 10); // hardcoded limitation
        decl->binding = point;
        AfxAssert(visibility);
        decl->visibility = visibility;
        AfxAssert(type < AFX_SHD_RES_TYPE_TOTAL);
        decl->type = type;
        AfxAssert(decl->type != AFX_SHD_RES_TYPE_OUTPUT);

        AfxAssert(cnt);
        decl->cnt = AfxMax(cnt, 1);
         AfxCloneString(&decl->name, name);
    }
    return err;
}

_AVXINL afxError AfxLegoBlueprintAddShaderContributions(afxPipelineRigBlueprint *blueprint, afxNat set, afxNat cnt, afxShaderBlueprint shdb_[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(cnt);
    AfxAssert(shdb_);
    AfxAssert(set < 4);

    for (afxNat i = 0; i < cnt; i++)
    {
        //AfxAssertObjects(1, &shdb[i], afxFcc_SHD);
        afxShaderBlueprint* shdb = &shdb_[i];
        afxNat resCnt = shdb->resources.cnt;
        afxBool incompatible = FALSE;

        for (afxNat k = 0; k < resCnt; k++)
        {
            afxShaderBlueprintResource rsrc = *(afxShaderBlueprintResource*)AfxGetArrayUnit(&shdb->resources, k);
            //AfxDescribeShaderInterfaces(shd[i], k, 1, &rsrc);

            if (rsrc.set == set)
            {
                afxBool entryExisting = FALSE;

                afxArray *bindings = &blueprint->bindings;

                for (afxNat j = 0; j < AfxCountArrayElements(bindings); j++)
                {
                    afxPipelineRigBlueprintBinding *binding = AfxGetArrayUnit(bindings, j);

                    if ((entryExisting |= (rsrc.binding == binding->binding)))
                    {
                        if ((incompatible |= ((rsrc.type != binding->type) || (rsrc.cnt != binding->cnt)))) AfxThrowError();
                        else
                        {
                            binding->visibility |= /*entry->visibility |*/ AFX_BIT_OFFSET(shdb->stage);
                        }
                        break;
                    }
                }

                if (!err && !entryExisting)
                {
                    if (AfxLegoBlueprintAddBinding(blueprint, rsrc.binding, AFX_BIT_OFFSET(shdb->stage), rsrc.type, rsrc.cnt, &rsrc.name.str))
                    {
                        AfxThrowError();
                    }
                }

                if (incompatible)
                    break;
            }
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// LEGO                                                                       //
////////////////////////////////////////////////////////////////////////////////

_AVX afxResult AfxGetBindSchemaEntry(afxBindSchema lego, afxNat first, afxNat cnt, afxPipelineRigBindingDecl decl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);
    AfxAssert(cnt);
    AfxAssertRange(lego->entryCnt, first, cnt);
    AfxAssert(decl);
    afxResult rslt = 0;

    for (afxNat i = 0; i < AfxMin(lego->entryCnt, cnt); i++)
    {
        if (i >= cnt) break;
        else
        {
            decl[i].binding = lego->entries[i].binding;
            decl[i].visibility = lego->entries[i].visibility;
            decl[i].type = lego->entries[i].type;
            decl[i].cnt = lego->entries[i].cnt;
            decl[i].name = &lego->entries[i].name.str;
            ++rslt;
        }
    }
    return rslt;
}

_AVX afxNat32 AfxGetBindSchemaHash(afxBindSchema lego)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);
    return lego->crc32;
}

_AVX afxError _AvxBschStdDtor(afxBindSchema lego)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);

    afxDrawContext dctx = AfxGetObjectProvider(lego);

    if (lego->entries)
    {
        for (afxNat i = 0; i < lego->entryCnt; i++)
            AfxDeallocateString(&lego->entries[i].name);

        AfxDeallocate(lego->entries);
    }

    return err;
}

_AVX afxError _AvxBschStdCtor(afxBindSchema lego, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &lego, afxFcc_BSCH);

    afxPipelineRigBlueprint const *blueprint = ((afxPipelineRigBlueprint const *)cookie->udd[0]) + cookie->no;

    afxDrawContext dctx = AfxGetObjectProvider(lego);
    AfxAssert(blueprint);

    afxNat bindCnt = AfxCountArrayElements(&blueprint->bindings);
    AfxAssert(bindCnt);

    lego->entryCnt = 0;
    lego->entries = NIL;

    if (bindCnt && !(lego->entries = AfxAllocate(bindCnt, sizeof(lego->entries[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < bindCnt; i++)
        {
            afxPipelineRigBlueprintBinding *bindBp = AfxGetArrayUnit(&blueprint->bindings, i);

            lego->entries[lego->entryCnt].binding = bindBp->binding;
            lego->entries[lego->entryCnt].cnt = bindBp->cnt;
            lego->entries[lego->entryCnt].type = bindBp->type;
            AfxAssert(lego->entries[lego->entryCnt].type != AFX_SHD_RES_TYPE_OUTPUT);
            lego->entries[lego->entryCnt].visibility = bindBp->visibility;
            AfxCloneString(&lego->entries[lego->entryCnt].name, &bindBp->name.str);
            ++lego->entryCnt;
        }
    }

    if (!err)
    {
        struct tmpCrc
        {
            afxNat binding;
            afxFlags visibility;
            afxShaderResourceType type;
            afxNat cnt;
        } tmpCrc[/*_SGL_MAX_ENTRY_PER_LEGO*/10] = { 0 };
        AfxAssert(sizeof(tmpCrc) / sizeof(tmpCrc[0]) >= lego->entryCnt);

        for (afxNat i = 0; i < lego->entryCnt; i++)
        {
            tmpCrc[i].binding = lego->entries[i].binding;
            tmpCrc[i].type = lego->entries[i].type;
            tmpCrc[i].visibility = lego->entries[i].visibility;
            tmpCrc[i].cnt = lego->entries[i].cnt;
        }

        afxNat32 crc = 0;
        //AfxCrc32(&crc, tmpCrc, sizeof(tmpCrc[0]) * lego->base.entryCnt);
        lego->crc32 = crc;
    }
    AfxAssertObjects(1, &lego, afxFcc_BSCH);
    return err;
}

_AVX afxClassConfig const _AvxBschStdImplementation =
{
    .fcc = afxFcc_BSCH,
    .name = "Ligature",
    .desc = "Ligature",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxBindSchema)),
    .mmu = NIL,
    .ctor = (void*)_AvxBschStdCtor,
    .dtor = (void*)_AvxBschStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireBindSchemas(afxDrawContext dctx, afxNat cnt, afxBindSchema schemas[], afxPipelineRigBlueprint const blueprint[])
{
    afxError err = AFX_ERR_NONE;

    afxManager* cls = AfxGetBindSchemaClass(dctx);
    AfxAssertClass(cls, afxFcc_BSCH);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)schemas, (void const*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
}

_AVX afxBindSchema AfxFindBindSchemas(afxDrawContext dctx, afxNat bindCnt, afxPipelineRigBindingDecl const bindings[])
{
    afxError err = AFX_ERR_NONE;

    AfxAssert(bindCnt);
    AfxAssert(bindings);

    struct tmpCrc
    {
        afxNat binding;
        afxFlags visibility;
        afxShaderResourceType type;
        afxNat cnt;
    } tmpCrc[10] = { 0 };

    for (afxNat i = 0; i < bindCnt; i++)
    {
        tmpCrc[i].binding = bindings[i].binding;
        tmpCrc[i].type = bindings[i].type;
        tmpCrc[i].visibility = bindings[i].visibility;
        tmpCrc[i].cnt = bindings[i].cnt;
    }

    afxNat32 crc = 0;
    //AfxCrc32(&crc, tmpCrc, sizeof(tmpCrc[0]) * bindCnt);

    afxManager* cls = AfxGetBindSchemaClass(dctx);

    afxNat i = 0;
    afxBindSchema lego;
    while (AfxEnumerateObjects(cls, i, 1, (afxObject*)&lego))
    {
        AfxAssertObjects(1, &lego, afxFcc_BSCH);

        if (lego->crc32 == crc)
        {
            return lego;
        }
        ++i;
    }
    return NIL;
}
