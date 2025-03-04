/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_LIGATURE_C
#define _AVX_SHADER_C
#include "../impl/avxImplementation.h"

_AVX afxUnit32 AfxGetLigatureHash(avxLigature liga, afxUnit set)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
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
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
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

            if (i >= cnt)
                break;

            afxIndex resIdx = first + i;
            decl[i].binding = liga->totalEntries[resIdx].binding;
            decl[i].visibility = liga->totalEntries[resIdx].visibility;
            decl[i].type = liga->totalEntries[resIdx].type;
            decl[i].cnt = liga->totalEntries[resIdx].cnt;
            decl[i].name = liga->totalEntries[resIdx].name;
            ++rslt;
        }
    }
    return rslt;
}

_AVX afxError _AvxLigaDtorCb(avxLigature liga)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);

    afxDrawSystem dsys = AfxGetProvider(liga);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = liga->totalEntryCnt,
            .siz = sizeof(liga->totalEntries[0]),
            .var = (void**)&liga->totalEntries
        },
        {
            .cnt = liga->setCnt,
            .siz = sizeof(liga->sets[0]),
            .var = (void**)&liga->sets
        }
    };

    if (AfxDeallocateInstanceData(liga, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxLigaCtorCb(avxLigature liga, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);

    //afxDrawSystem dsys = args[0];
    avxLigatureConfig const* cfg = ((avxLigatureConfig const*)args[1]) + invokeNo;
    afxUnit shaderCnt = cfg->shaderCnt;
    avxShader* shaders = cfg->shaders;
    
    liga->label = cfg->label;

    liga->pushables = FALSE;

    afxArray sets;
    afxArray entries;
    AfxMakeArray(&sets, sizeof(avxLigatureSet), 4, NIL, 0);
    AfxMakeArray(&entries, sizeof(avxLigatureEntry), 40, NIL, 0);
    AfxReserveArraySpace(&sets, 4);
    AfxReserveArraySpace(&entries, 40);

    for (afxUnit i = 0; i < shaderCnt; i++)
    {
        avxShader shd = shaders[i];

        if (!shd)
            continue;

        AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
        avxShaderStage stage = AfxGetShaderStage(shd);
        afxUnit resCnt = AfxQueryShaderInterfaces(shd, 0, 0, NIL);

        if (shd->pushConstName.str.len)
            liga->pushables = TRUE;

        for (afxUnit j = 0; j < resCnt; j++)
        {
            avxShaderResource rsrc;
            AfxQueryShaderInterfaces(shd, j, 1, &rsrc);
            afxIndex setIdx = AFX_INVALID_INDEX;
            //afxIndex resIdx = AFX_INVALID_INDEX;

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

                if (!(set = AfxPushArrayUnit(&sets, &setIdx))) AfxThrowError();
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
                    AvxGetShaderStringBase(dsys, &strb);
                    AfxRStrings2(strb, 1, &rsrc.name, &s);
#endif
                    avxLigatureEntry* ent;

                    if (!(ent = AfxPushArrayUnit(&entries, NIL))) AfxThrowError();
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
        liga->setCnt = 0;
        liga->sets = NIL;

        afxObjectStash const stashes[] =
        {
            {
                .cnt = entries.pop,
                .siz = sizeof(liga->totalEntries[0]),
                .var = (void**)&liga->totalEntries
            },
            {
                .cnt = sets.pop,
                .siz = sizeof(liga->sets[0]),
                .var = (void**)&liga->sets
            }
        };

        if (AfxAllocateInstanceData(liga, ARRAY_SIZE(stashes), stashes))
            AfxThrowError();
    }

    if (!err)
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

    AfxCleanUpArray(&sets);
    AfxCleanUpArray(&entries);

    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
    return err;
}

_AVX afxClassConfig const _AVX_LIGA_CLASS_CONFIG =
{
    .fcc = afxFcc_LIGA,
    .name = "Ligature",
    .desc = "Pipelined Resourcing Ligature",
    .fixedSiz = sizeof(AFX_OBJECT(avxLigature)),
    .ctor = (void*)_AvxLigaCtorCb,
    .dtor = (void*)_AvxLigaDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxDeclareLigatures(afxDrawSystem dsys, afxUnit cnt, avxLigatureConfig const cfg[], avxLigature ligatures[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->ligaCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_LIGA);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)ligatures, (void const*[]) { dsys, cfg }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_LIGA, cnt, ligatures);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxLigature liga = ligatures[i];
    }
#endif

    return err;
}
