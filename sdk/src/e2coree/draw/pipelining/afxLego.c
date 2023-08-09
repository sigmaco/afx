/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */


#define _AFX_LEGO_C
#include "../_classified/afxDrawClassified.h"
#include "afx/draw/pipelining/afxLego.h"

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxLegoBlueprintBegin(afxLegoBlueprint *blueprint, afxDrawContext dctx, afxNat estBindCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->fcc = AFX_FCC_SHDB;

    blueprint->dctx = dctx;
    afxContext mem = AfxGetDrawContextMemory(blueprint->dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);

    AfxAcquireArray(&blueprint->bindings, sizeof(afxLegoBlueprintBinding), AfxMaxi(estBindCnt, 10), AfxSpawnHint());
}

_AFXINL void AfxLegoBlueprintErase(afxLegoBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);

    for (afxNat i = 0; i < AfxGetArrayPop(&blueprint->bindings); i++)
    {
        afxLegoBlueprintBinding* binding = AfxGetArrayUnit(&blueprint->bindings, i);

        if (binding->name)
        {
            AfxDeallocateString(binding->name);
            binding->name = NIL;
        }
    }
    AfxEmptyArray(&blueprint->bindings);
}

_AFXINL afxError AfxLegoBlueprintEnd(afxLegoBlueprint *blueprint, afxLego *legt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);

    if (legt)
    {
        if (AfxGetArrayPop(&blueprint->bindings))
        {
            if (AfxBuildLegos(blueprint->dctx, 1, blueprint, legt))
            {
                AfxThrowError();
            }
        }
        else
        {
            *legt = NIL;
        }
    }

    for (afxNat i = 0; i < AfxGetArrayPop(&blueprint->bindings); i++)
    {
        afxLegoBlueprintBinding* binding = AfxGetArrayUnit(&blueprint->bindings, i);

        if (binding->name)
        {
            AfxDeallocateString(binding->name);
            binding->name = NIL;
        }
    }
    AfxReleaseArray(&blueprint->bindings);

    blueprint->fcc = NIL;
    return err;
}

_AFXINL afxError AfxLegoBlueprintAddBinding(afxLegoBlueprint *blueprint, afxNat point, afxFlags visibility, afxShaderResourceType type, afxNat cnt, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    afxNat idx;
    afxLegoBlueprintBinding *decl;

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

        if (err)
        {
            int a = 0;
        }
        AfxAssert(cnt);
        decl->cnt = AfxMaxi(cnt, 1);
        decl->name = name && !AfxStringIsEmpty(name) ? AfxCloneString(name) : NIL;
    }
    return err;
}

_AFXINL afxError AfxLegoBlueprintAddShaderContributions(afxLegoBlueprint *blueprint, afxNat set, afxNat cnt, afxShader shd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(cnt);
    AfxAssert(shd);
    AfxAssert(set < 4);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssertObject(shd[i], AFX_FCC_SHD);
        afxNat resCnt = AfxGetShaderInterfaceCount(shd[i]);
        afxBool incompatible = FALSE;

        for (afxNat k = 0; k < resCnt; k++)
        {
            afxShaderResource rsrc;
            AfxDescribeShaderInterfaces(shd[i], k, 1, &rsrc);

            if (rsrc.set == set)
            {
                afxBool entryExisting = FALSE;

                afxArray *bindings = &blueprint->bindings;

                for (afxNat j = 0; j < AfxGetArrayPop(bindings); j++)
                {
                    afxLegoBlueprintBinding *binding = AfxGetArrayUnit(bindings, j);

                    if ((entryExisting |= (rsrc.binding == binding->binding)))
                    {
                        if ((incompatible |= ((rsrc.type != binding->type) || (rsrc.cnt != binding->cnt)))) AfxThrowError();
                        else
                        {
                            binding->visibility |= /*entry->visibility |*/ AFX_FLAG(AfxShaderGetStage(shd[i]));
                        }
                        break;
                    }
                }

                if (!err && !entryExisting)
                {
                    if (AfxLegoBlueprintAddBinding(blueprint, rsrc.binding, AFX_FLAG(AfxShaderGetStage(shd[i])), rsrc.type, rsrc.cnt, rsrc.name))
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

_AFX afxResult AfxLegoDescribeBinding(afxLego legt, afxNat first, afxNat cnt, afxLegoBindingDecl decl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(legt, AFX_FCC_LEGO);
    AfxAssert(cnt);
    AfxAssertRange(legt->entryCnt, first, cnt);
    AfxAssert(decl);
    afxResult rslt = 0;

    for (afxNat i = 0; i < AfxMini(legt->entryCnt, cnt); i++)
    {
        if (i >= cnt) break;
        else
        {
            decl[i].binding = legt->entries[i].binding;
            decl[i].visibility = legt->entries[i].visibility;
            decl[i].type = legt->entries[i].type;
            decl[i].cnt = legt->entries[i].cnt;
            decl[i].name = legt->entries[i].name;
            ++rslt;
        }
    }
    return rslt;
}

_AFX afxNat32 AfxLegoGetCrc32(afxLego legt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(legt, AFX_FCC_LEGO);
    return legt->crc32;
}

_AFX afxError AfxBuildLegos(afxDrawContext dctx, afxNat cnt, afxLegoBlueprint const blueprint[], afxLego legt[])
{
    AfxEntry("dctx=%p", dctx);
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    AfxAssert(blueprint);

    if (AfxClassAcquireObjects(AfxGetLegoClass(dctx), NIL, cnt, blueprint, (afxInstance**)legt, AfxSpawnHint()))
        AfxThrowError();

    return err;
}

_AFX afxBool _AfxLegtEventHandler(afxInstance *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxLego legt = (void*)obj;
    AfxAssertObject(legt, AFX_FCC_LEGO);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxLegtEventFilter(afxInstance *obj, afxInstance *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxLego legt = (void*)obj;
    AfxAssertObject(legt, AFX_FCC_LEGO);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxLegtDtor(afxLego legt)
{
    AfxEntry("legt=%p", legt);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(legt, AFX_FCC_LEGO);

    afxDrawContext dctx = AfxObjectGetProvider(&legt->obj);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);

    if (legt->entries)
    {
        for (afxNat i = 0; i < legt->entryCnt; i++)
        {
            if (legt->entries[i].name)
                AfxDeallocateString(legt->entries[i].name);
        }

        AfxDeallocate(mem, legt->entries);
    }

    return err;
}

_AFX afxError _AfxLegtCtor(void *cache, afxNat idx, afxLego legt, afxLegoBlueprint const blueprints[])
{
    AfxEntry("legt=%p", legt);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(legt, AFX_FCC_LEGO);
    (void)cache;

    afxDrawContext dctx = AfxObjectGetProvider(&legt->obj);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, AFX_FCC_DCTX);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);

    AfxAssert(blueprints);
    afxLegoBlueprint const *blueprint = &blueprints[idx];

    afxNat bindCnt = AfxGetArrayPop(&blueprint->bindings);
    AfxAssert(bindCnt);

    afxBool incompatible = FALSE;
    legt->entryCnt = 0;
    legt->entries = NIL;

    if (bindCnt && !(legt->entries = AfxAllocate(mem, bindCnt * sizeof(legt->entries[0]), 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < bindCnt; i++)
        {
            afxLegoBlueprintBinding *bindBp = AfxGetArrayUnit(&blueprint->bindings, i);

            legt->entries[legt->entryCnt].binding = bindBp->binding;
            legt->entries[legt->entryCnt].cnt = bindBp->cnt;
            legt->entries[legt->entryCnt].type = bindBp->type;
            AfxAssert(legt->entries[legt->entryCnt].type != AFX_SHD_RES_TYPE_OUTPUT);
            legt->entries[legt->entryCnt].visibility = bindBp->visibility;
            legt->entries[legt->entryCnt].name = bindBp->name ? AfxCloneString(bindBp->name) : NIL;
            ++legt->entryCnt;
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
        AfxAssert(sizeof(tmpCrc) / sizeof(tmpCrc[0]) >= legt->entryCnt);

        for (afxNat i = 0; i < legt->entryCnt; i++)
        {
            tmpCrc[i].binding = legt->entries[i].binding;
            tmpCrc[i].type = legt->entries[i].type;
            tmpCrc[i].visibility = legt->entries[i].visibility;
            tmpCrc[i].cnt = legt->entries[i].cnt;
        }

        afxNat32 crc = 0;
        AfxCrc32(&crc, tmpCrc, sizeof(tmpCrc[0]) * legt->entryCnt);
        legt->crc32 = crc;
    }
    return err;
}

_AFX afxClassSpecification const _AfxLegtClassSpec;

afxClassSpecification const _AfxLegtClassSpec =
{
    AFX_FCC_LEGO,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxLego)),
    NIL,
    (void*)_AfxLegtCtor,
    (void*)_AfxLegtDtor,
    .event = _AfxLegtEventHandler,
    .eventFilter = _AfxLegtEventFilter,
    "afxLego",
    NIL
};
