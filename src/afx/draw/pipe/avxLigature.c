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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_BIND_SCHEMA_C
#include "../dev/AvxDevKit.h"

////////////////////////////////////////////////////////////////////////////////
// BLUEPRINT                                                                  //
////////////////////////////////////////////////////////////////////////////////

_AVXINL void AfxLegoBlueprintBegin(avxPipelineRigBlueprint *blueprint, afxNat estBindCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->fcc = afxFcc_SHDB;

    blueprint->dctx = NIL;
    //afxMmu mmu = AfxGetDrawContextMmu(blueprint->dctx);
    //AfxAssertObjects(1, &mmu, afxFcc_MMU);

    AfxAllocateArray(&blueprint->bindings, AfxMax(estBindCnt, 10), sizeof(avxPipelineRigBlueprintBinding), NIL);
}

_AVXINL void AfxLegoBlueprintErase(avxPipelineRigBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->bindings); i++)
    {
        avxPipelineRigBlueprintBinding* binding = AfxGetArrayUnit(&blueprint->bindings, i);

        //if (binding->name)
        {
            AfxDeallocateString(&binding->name);
            //binding->name = NIL;
        }
    }
    AfxEmptyArray(&blueprint->bindings);
}

_AVXINL afxError AfxLegoBlueprintEnd(avxPipelineRigBlueprint *blueprint, avxLigature *liga)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->bindings); i++)
    {
        avxPipelineRigBlueprintBinding* binding = AfxGetArrayUnit(&blueprint->bindings, i);

        //if (binding->name)
        {
            //AfxDeallocateString(&binding->name);
            //binding->name = NIL;
        }
    }
    AfxDeallocateArray(&blueprint->bindings);

    blueprint->fcc = NIL;
    return err;
}

_AVXINL afxError AfxLegoBlueprintAddBinding(avxPipelineRigBlueprint *blueprint, afxNat point, afxFlags visibility, avxShaderResourceType type, afxNat cnt, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    afxNat idx;
    avxPipelineRigBlueprintBinding *decl;

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

_AVXINL afxError AfxLegoBlueprintAddShaderContributions(avxPipelineRigBlueprint *blueprint, afxNat set, afxNat cnt, avxShaderBlueprint shdb_[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(cnt);
    AfxAssert(shdb_);
    AfxAssert(set < 4);

    for (afxNat i = 0; i < cnt; i++)
    {
        //AfxAssertObjects(1, &shdb[i], afxFcc_SHD);
        avxShaderBlueprint* shdb = &shdb_[i];
        afxNat resCnt = shdb->resources.cnt;
        afxBool incompatible = FALSE;

        for (afxNat k = 0; k < resCnt; k++)
        {
            avxShaderBlueprintResource rsrc = *(avxShaderBlueprintResource*)AfxGetArrayUnit(&shdb->resources, k);
            //AfxDescribeShaderInterfaces(shd[i], k, 1, &rsrc);

            if (rsrc.set == set)
            {
                afxBool entryExisting = FALSE;

                afxArray *bindings = &blueprint->bindings;

                for (afxNat j = 0; j < AfxCountArrayElements(bindings); j++)
                {
                    avxPipelineRigBlueprintBinding *binding = AfxGetArrayUnit(bindings, j);

                    if ((entryExisting |= (rsrc.binding == binding->binding)))
                    {
                        if ((incompatible |= ((rsrc.type != binding->type) || (rsrc.cnt != binding->cnt)))) AfxThrowError();
                        else
                        {
                            binding->visibility |= /*entry->visibility |*/ AFX_BIT(shdb->stage);
                        }
                        break;
                    }
                }

                if (!err && !entryExisting)
                {
                    if (AfxLegoBlueprintAddBinding(blueprint, rsrc.binding, AFX_BIT(shdb->stage), rsrc.type, rsrc.cnt, &rsrc.name))
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

_AVX afxNat32 AfxGetLigatureHash(avxLigature liga, afxNat set)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);
    AfxAssertRange(liga->setCnt, set, 1);
    afxNat crc = NIL;

    for (afxNat i = 0; i < liga->setCnt; i++)
    {
        if (liga->sets[i].set == set)
        {
            crc = liga->sets[i].crc32;
            break;
        }
    }
    return crc;
}

_AVX afxResult AfxGetLigatureEntry(avxLigature liga, afxNat set, afxIndex first, afxNat cnt, avxPipelineRigBindingDecl decl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);
    AfxAssertRange(liga->totalEntryCnt, first, cnt);
    AfxAssertRange(liga->setCnt, set, 1);
    AfxAssert(cnt);
    AfxAssert(decl);
    afxResult rslt = 0;

    for (afxIndex i = 0; i < liga->setCnt; i++)
    {
        if (liga->sets[i].set == set)
        {
            AfxAssertRange(liga->sets[i].entryCnt, first, cnt);

            if (i >= cnt) break;
            else
            {
                afxIndex resIdx = first + i;
                decl[i].binding = liga->totalEntries[resIdx].binding;
                decl[i].visibility = liga->totalEntries[resIdx].visibility;
                decl[i].type = liga->totalEntries[resIdx].type;
                decl[i].cnt = liga->totalEntries[resIdx].cnt;
                decl[i].name = &liga->totalEntries[resIdx].name.str;
                ++rslt;
            }
        }
    }
    return rslt;
}

_AVX afxError _AvxBschStdDtor(avxLigature liga)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);

    afxDrawContext dctx = AfxGetObjectProvider(liga);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (liga->sets)
        AfxDeallocate(liga->sets);

    if (liga->totalEntries)
        AfxDeallocate(liga->totalEntries);

    return err;
}

_AVX afxError _AvxBschStdCtor(avxLigature liga, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);

    afxDrawContext dctx = cookie->udd[0];
    avxLigatureConfig const* cfg = ((avxLigatureConfig const*)cookie->udd[1]) + cookie->no;
    afxNat shaderCnt = cfg->shaderCnt;
    avxShader* shaders = cfg->shaders;
    
    afxArray sets;
    afxArray entries;
    AfxAllocateArray(&sets, 4, sizeof(avxLigatureSet), NIL);
    AfxAllocateArray(&entries, 40, sizeof(avxLigatureEntry), NIL);
    AfxReserveArraySpace(&sets, 4);
    AfxReserveArraySpace(&entries, 40);

    for (afxNat i = 0; i < shaderCnt; i++)
    {
        avxShader shd = shaders[i];
        AfxAssertObjects(1, &shd, afxFcc_SHD);
        avxShaderStage stage = AfxGetShaderStage(shd);
        afxNat resCnt = AfxCountShaderInterfaces(shd);

        for (afxNat j = 0; j < resCnt; j++)
        {
            avxShaderResource rsrc;
            AfxDescribeShaderInterfaces(shd, j, 1, &rsrc);
            afxIndex setIdx = AFX_INVALID_INDEX;
            afxIndex resIdx = AFX_INVALID_INDEX;
            
            for (afxNat k = 0; k < sets.cnt; k++)
            {
                avxLigatureSet* set = AfxGetArrayUnit(&sets, k);
                AfxAssert(set);

                if (rsrc.set == set->set)
                {
                    setIdx = k;
                    break;
                }
            }

            if (setIdx == AFX_INVALID_INDEX)
            {
                avxLigatureSet* set;

                if (!(set = AfxInsertArrayUnit(&sets, &setIdx))) AfxThrowError();
                else
                {
                    set->set = rsrc.set;
                    set->baseEntry = 0;
                    set->entryCnt = 0;
                }
            }

            if (!err)
            {
                afxBool incompatible = FALSE;
                avxLigatureSet* set = AfxGetArrayUnit(&sets, setIdx);
                AfxAssert(set);
                AfxAssert(set->set == rsrc.set);

                afxBool entryExisting = FALSE;

                for (afxNat l = 0; l < entries.cnt; l++)
                {
                    avxLigatureEntry* binding = AfxGetArrayUnit(&entries, l);

                    if (binding->set == rsrc.set)
                    {
                        if ((entryExisting |= (rsrc.binding == binding->binding)))
                        {
                            if (((rsrc.type != binding->type) || (rsrc.cnt != binding->cnt)))
                            {
                                AfxThrowError();
                                incompatible |= TRUE;
                            }
                            else
                            {
                                binding->visibility |= /*entry->visibility |*/ AFX_BIT(stage);
                            }
                            break;
                        }
                    }
                }

                if (!err && !entryExisting)
                {
#if 0
                    afxString s;
                    afxStringBase strb;
                    AfxGetShaderStringBase(dctx, &strb);
                    AfxRStrings2(strb, 1, &rsrc.name, &s);
#endif
                    avxLigatureEntry* ent;

                    if (!(ent = AfxInsertArrayUnit(&entries, NIL))) AfxThrowError();
                    else
                    {
                        ent->binding = rsrc.binding;
                        ent->visibility = AFX_BIT(stage);
                        ent->type = rsrc.type;
                        ent->cnt = rsrc.cnt;
                        ent->set = rsrc.set;

                        AfxMakeString8(&ent->name, &rsrc.name);
                        ++set->entryCnt;
                    }
                }

                if (incompatible)
                    break;
            }
        }
    }

    if (!err)
    {
        liga->totalEntryCnt = 0;
        liga->totalEntries = NIL;

        if (entries.cnt && !(liga->totalEntries = AfxAllocate(entries.cnt, sizeof(liga->totalEntries[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            liga->setCnt = 0;
            liga->sets = NIL;

            if (sets.cnt && !(liga->sets = AfxAllocate(sets.cnt, sizeof(liga->sets[0]), 0, AfxHere()))) AfxThrowError();
            else
            {
                afxNat baseEntryIdx = 0;

                for (afxNat i = 0; i < sets.cnt; i++)
                {
                    avxLigatureSet* set2 = AfxGetArrayUnit(&sets, i);
                    afxNat entryCnt = 0;

                    for (afxNat j = 0; j < entries.cnt; j++)
                    {
                        avxLigatureEntry* ent2 = AfxGetArrayUnit(&entries, j);
                        
                        if (ent2->set == set2->set)
                        {
                            avxLigatureEntry* ent = &liga->totalEntries[liga->totalEntryCnt];

                            ent->binding = ent2->binding;
                            ent->visibility = ent2->visibility;
                            ent->type = ent2->type;
                            ent->cnt = ent2->cnt;
                            ent->set = ent2->set;

                            AfxMakeString8(&ent->name, &ent2->name.str);
                            ++entryCnt;

                            ++liga->totalEntryCnt;
                        }
                    }
                    AfxAssert(entryCnt == set2->entryCnt);
                    avxLigatureSet* set = &liga->sets[liga->setCnt];
                    set->set = set2->set;
                    set->baseEntry = baseEntryIdx;
                    set->entryCnt = entryCnt;
                    baseEntryIdx += entryCnt;

                    ++liga->setCnt;
                }
            }
        }
    }

    AfxDeallocateArray(&sets);
    AfxDeallocateArray(&entries);

    AfxAssertObjects(1, &liga, afxFcc_BSCH);
    return err;
}

_AVX afxClassConfig const _AvxBschStdImplementation =
{
    .fcc = afxFcc_BSCH,
    .name = "Ligature",
    .desc = "Pipelined Resourcing Ligature",
    .fixedSiz = sizeof(AFX_OBJECT(avxLigature)),
    .ctor = (void*)_AvxBschStdCtor,
    .dtor = (void*)_AvxBschStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxBuildLigatures(afxDrawContext dctx, afxNat cnt, avxLigatureConfig const cfg[], avxLigature ligatures[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(cfg);
    AfxAssert(cnt);

    afxClass* cls = AfxGetLigatureClass(dctx);
    AfxAssertClass(cls, afxFcc_BSCH);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)ligatures, (void const*[]) { dctx, cfg }))
        AfxThrowError();

    return err;
}

_AVX avxLigature AfxFindLigatures(afxDrawContext dctx, afxNat bindCnt, avxPipelineRigBindingDecl const bindings[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(bindCnt);
    AfxAssert(bindings);

    return NIL;
}
