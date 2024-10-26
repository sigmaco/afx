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
#define _AVX_SHADER_C
#include "../../dev/AvxImplKit.h"

_AVX afxUnit32 AfxGetLigatureHash(avxLigature liga, afxUnit set)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_LIGA);
    AFX_ASSERT_RANGE(liga->setCnt, set, 1);
    afxUnit crc = NIL;

    for (afxUnit i = 0; i < liga->setCnt; i++)
    {
        if (liga->sets[i].set == set)
        {
            crc = liga->sets[i].crc32;
            break;
        }
    }
    return crc;
}

_AVX afxResult AfxGetLigatureEntry(avxLigature liga, afxUnit set, afxIndex first, afxUnit cnt, avxLigatureEntry decl[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_LIGA);
    AFX_ASSERT_RANGE(liga->totalEntryCnt, first, cnt);
    AFX_ASSERT_RANGE(liga->setCnt, set, 1);
    AFX_ASSERT(cnt);
    AFX_ASSERT(decl);
    afxResult rslt = 0;

    for (afxIndex i = 0; i < liga->setCnt; i++)
    {
        if (liga->sets[i].set == set)
        {
            AFX_ASSERT_RANGE(liga->sets[i].entryCnt, first, cnt);

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
    AfxAssertObjects(1, &liga, afxFcc_LIGA);

    afxDrawContext dctx = AfxGetProvider(liga);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (liga->sets)
        AfxDeallocate(liga->sets);

    if (liga->totalEntries)
        AfxDeallocate(liga->totalEntries);

    return err;
}

_AVX afxError _AvxLigaStdCtorCb(avxLigature liga, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &liga, afxFcc_LIGA);

    afxDrawContext dctx = args[0];
    avxLigatureConfig const* cfg = ((avxLigatureConfig const*)args[1]) + invokeNo;
    afxUnit shaderCnt = cfg->shaderCnt;
    avxShader* shaders = cfg->shaders;
    
    liga->pushables = FALSE;

    afxArray sets;
    afxArray entries;
    AfxMakeArray(&sets, 4, sizeof(avxLigatureSet), NIL);
    AfxMakeArray(&entries, 40, sizeof(avxLigatureEntry), NIL);
    AfxReserveArraySpace(&sets, 4);
    AfxReserveArraySpace(&entries, 40);

    for (afxUnit i = 0; i < shaderCnt; i++)
    {
        avxShader shd = shaders[i];

        if (shd)
        {
            AfxAssertObjects(1, &shd, afxFcc_SHD);
            avxShaderStage stage = AfxGetShaderStage(shd);
            afxUnit resCnt = AfxCountShaderInterfaces(shd);

            if (shd->pushConstName.str.len)
                liga->pushables = TRUE;

            for (afxUnit j = 0; j < resCnt; j++)
            {
                avxShaderResource rsrc;
                AfxDescribeShaderInterfaces(shd, j, 1, &rsrc);
                afxIndex setIdx = AFX_INVALID_INDEX;
                afxIndex resIdx = AFX_INVALID_INDEX;

                for (afxUnit k = 0; k < sets.pop; k++)
                {
                    avxLigatureSet* set = AfxGetArrayUnit(&sets, k);
                    AFX_ASSERT(set);

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
                    AFX_ASSERT(set);
                    AFX_ASSERT(set->set == rsrc.set);

                    afxBool entryExisting = FALSE;

                    for (afxUnit l = 0; l < entries.pop; l++)
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
    }

    if (!err)
    {
        liga->totalEntryCnt = 0;
        liga->totalEntries = NIL;

        if (entries.pop && !(liga->totalEntries = AfxAllocate(entries.pop, sizeof(liga->totalEntries[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            liga->setCnt = 0;
            liga->sets = NIL;

            if (sets.pop && !(liga->sets = AfxAllocate(sets.pop, sizeof(liga->sets[0]), 0, AfxHere()))) AfxThrowError();
            else
            {
                afxUnit baseEntryIdx = 0;

                for (afxUnit i = 0; i < sets.pop; i++)
                {
                    avxLigatureSet* set2 = AfxGetArrayUnit(&sets, i);
                    afxUnit entryCnt = 0;

                    for (afxUnit j = 0; j < entries.pop; j++)
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
                    AFX_ASSERT(entryCnt == set2->entryCnt);
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

    AfxCleanUpArray(&sets);
    AfxCleanUpArray(&entries);

    AfxAssertObjects(1, &liga, afxFcc_LIGA);
    return err;
}

_AVX afxClassConfig const _AvxLigaStdImplementation =
{
    .fcc = afxFcc_LIGA,
    .name = "Ligature",
    .desc = "Pipelined Resourcing Ligature",
    .fixedSiz = sizeof(AFX_OBJECT(avxLigature)),
    .ctor = (void*)_AvxLigaStdCtorCb,
    .dtor = (void*)_AvxLigaStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxBuildLigatures(afxDrawContext dctx, afxUnit cnt, avxLigatureConfig const cfg[], avxLigature ligatures[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = AfxGetLigatureClass(dctx);
    AfxAssertClass(cls, afxFcc_LIGA);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)ligatures, (void const*[]) { dctx, cfg }))
        AfxThrowError();

    return err;
}
