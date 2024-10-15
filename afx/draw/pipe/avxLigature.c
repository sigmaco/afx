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
#define _AVX_LIGATURE_C
#include "../../dev/AvxImplKit.h"

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

_AVX afxResult AfxGetLigatureEntry(avxLigature liga, afxNat set, afxIndex first, afxNat cnt, avxLigatureEntry decl[])
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
                decl[i].name = liga->totalEntries[resIdx].name;
                ++rslt;
            }
        }
    }
    return rslt;
}

_AVX afxError _AvxLigaStdDtorCb(avxLigature liga)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);

    afxDrawContext dctx = AfxGetProvider(liga);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (liga->sets)
        AfxDeallocate(liga->sets);

    if (liga->totalEntries)
        AfxDeallocate(liga->totalEntries);

    return err;
}

_AVX afxError _AvxLigaStdCtorCb(avxLigature liga, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_BSCH);

    afxDrawContext dctx = args[0];
    avxLigatureConfig const* cfg = ((avxLigatureConfig const*)args[1]) + invokeNo;
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

                        AfxMakeString16(&ent->name, &rsrc.name.str);
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

                            AfxMakeString16(&ent->name, &ent2->name.str);
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

_AVX afxClassConfig const _AvxLigaStdImplementation =
{
    .fcc = afxFcc_BSCH,
    .name = "Ligature",
    .desc = "Pipelined Resourcing Ligature",
    .fixedSiz = sizeof(AFX_OBJECT(avxLigature)),
    .ctor = (void*)_AvxLigaStdCtorCb,
    .dtor = (void*)_AvxLigaStdDtorCb
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
