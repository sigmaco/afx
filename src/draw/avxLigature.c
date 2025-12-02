/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_LIGATURE_C
#define _AVX_SHADER_C
#include "avxIcd.h"

_AVX afxDrawSystem AvxGetLigatureHost(avxLigature liga)
{
    afxError err = { 0 };
    // @liga must be a valid avxLigature handle.
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
    afxDrawSystem dsys = AfxGetHost(liga);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxFlags AvxGetLigatureFlags(avxLigature liga, afxFlags mask)
{
    afxError err = { 0 };
    // @liga must be a valid avxLigature handle.
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
    return (!mask) ? liga->flags : (liga->flags & mask);
}

_AVX afxUnit32 AvxGetLigatureHash(avxLigature liga, afxUnit set)
{
    afxError err = { 0 };
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

_AVX afxResult AvxDescribeLigament(avxLigature liga, afxUnit set, afxIndex first, afxUnit cnt, avxLigament descs[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);
    //AFX_ASSERT_RANGE(liga->totalEntryCnt, first, cnt);
    //AFX_ASSERT_RANGE(liga->setCnt, set, 1);
    AFX_ASSERT(cnt);
    AFX_ASSERT(descs);
    afxResult rslt = 0;

    for (afxUnit setIdx = 0; setIdx < liga->setCnt; setIdx++)
    {
        _avxLigamentSet* pSet = &liga->sets[setIdx];

        if (pSet->set == set)
        {
            for (afxUnit k = 0; k < cnt; k++)
            {
                avxLigament* desc = &descs[k];
                afxBool found = FALSE;

                //AFX_ASSERT_RANGE(pSet->entryCnt, first, cnt);

                for (afxUnit absResIdx = 0; absResIdx < pSet->entryCnt; absResIdx++)
                {
                    _avxLigament* bind = &liga->totalEntries[pSet->baseEntry + absResIdx];
                    afxIndex resIdx = bind->binding;

                    if ((resIdx != first + k))
                        continue;

                    found = TRUE;

                    desc->binding = bind->binding;
                    desc->visibility = bind->visibility;
                    desc->type = bind->type;
                    desc->cnt = bind->cnt;
                    desc->name = bind->name;
                    ++rslt;

                    if (rslt >= cnt)
                        break;
                }

                if (!found)
                    *desc = (avxLigament) { 0 };
            }
        }
    }
    return rslt;
}

_AVX afxError _AvxLigaDtorCb(avxLigature liga)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);

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
        },
        {
            .cnt = liga->pushCnt,
            .siz = sizeof(liga->pushes[0]),
            .var = (void**)&liga->pushes
        }
    };

    if (AfxDeallocateInstanceData(liga, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxLigaCtorCb(avxLigature liga, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);

    //afxDrawSystem dsys = args[0];
    avxLigatureConfig const* cfg = ((avxLigatureConfig const*)args[1]) + invokeNo;
    
    // TODO: Acquire from draw system's limits
    afxUnit maxPushConstantSize = 512;

    // Validate push constants
    if (cfg->pushCnt > AVX_MAX_PUSH_RANGES)
    {
        // invalid range
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < cfg->pushCnt; ++i)
    {
        avxPushRange const* p = &cfg->pushes[i];

        if (p->offset % 4 != 0 || p->size % 4 != 0)
        {
            // invalid alignment
            AfxThrowError();
            return err;
        }
#if 0
        if (p->size == 0 || (p->offset + p->size) > maxPushConstantSize)
        {
            // invalid size
            AfxThrowError();
            return err;
        }
#endif
        for (afxUnit j = i + 1; j < cfg->pushCnt; ++j)
        {
            avxPushRange const* q = &cfg->pushes[j];

            if ((p->offset < q->offset + q->size) && (q->offset < p->offset + p->size))
            {
                // overlapping range
                AfxThrowError();
                return err;
            }
        }
    }

    // Validate ligaments
    if (cfg->pointCnt > AVX_MAX_LIGAMENTS)
    {
        // invalid range
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < cfg->pointCnt; ++i)
    {
        avxLigament const* l = &cfg->points[i];
#if 0
        if (l->cnt == 0)
        {
            // invalid count
            AfxThrowError();
            return err;
        }
#endif
#if 0
        if (l->visibility == 0)
        {
            // invalid visibility
            AfxThrowError();
            return err;
        }
#endif
        if (l->type == avxShaderParam_UNIFORM || l->type == avxShaderParam_BUFFER)
        {
#if 0
            if (l->buf.minSizeBound == 0)
            {
                // invalid siz
                AfxThrowError();
                return err;
            }
#endif
            if (l->buf.writeable && l->type != avxShaderParam_BUFFER)
            {
                // type mismatch
                AfxThrowError();
                return err;
            }
            if (!l->buf.writeable && l->type != avxShaderParam_UNIFORM)
            {
                // type mismatch
                AfxThrowError();
                return err;
            }
        }
    }

    // Validate ligature sets
    if (cfg->setCnt > AVX_MAX_LIGAMENT_SETS)
    {
        // invalid range
        AfxThrowError();
        return err;
    }

    // Used ligament flags
    afxBool ligamentUsed[AVX_MAX_LIGAMENTS] = { 0 };
    afxUnit totalPointCnt = 0;

    for (afxUnit i = 0; i < cfg->setCnt; ++i)
    {
        avxLigamentSet const* set = &cfg->sets[i];

        // Bounds check
        if ((set->baseEntryIdx + set->entryCnt) > cfg->pointCnt)
        {
            // invalid range
            AfxThrowError();
            return err;
        }

        for (afxUnit j = 0; j < set->entryCnt; ++j)
        {
            afxUnit ligIdx = set->baseEntryIdx + j;

            if (ligamentUsed[ligIdx])
            {
                // Duplicate ligament
                AfxThrowError();
                return err; // Ligament in more than one set
            }
            ligamentUsed[ligIdx] = 1;
        }

        totalPointCnt += set->entryCnt;

        // Optional: ensure no duplicate 'set' IDs across ligature sets
        for (afxUnit k = i + 1; k < cfg->setCnt; ++k)
        {
            if (cfg->sets[i].set == cfg->sets[k].set)
            {
                // Duplicated set
                AfxThrowError();
                return err;
            }
        }
    }

    liga->pushes = NIL;
    liga->totalEntries = NIL;
    liga->sets = NIL;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = totalPointCnt,
            .siz = sizeof(liga->totalEntries[0]),
            .var = (void**)&liga->totalEntries
        },
        {
            .cnt = cfg->setCnt,
            .siz = sizeof(liga->sets[0]),
            .var = (void**)&liga->sets
        },
        {
            .cnt = cfg->pushCnt,
            .siz = sizeof(liga->pushes[0]),
            .var = (void**)&liga->pushes
        }
    };

    if (AfxAllocateInstanceData(liga, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    afxUnit baseEntryIdx = 0;

    for (afxUnit i = 0; i < cfg->setCnt; i++)
    {
        avxLigamentSet const* sc = &cfg->sets[i];
        _avxLigamentSet* s = &liga->sets[i];

        afxUnit entryCnt = 0;
        for (afxUnit j = 0; j < sc->entryCnt; j++)
        {
            avxLigament const* pc = &cfg->points[baseEntryIdx + j];
            _avxLigament* p = &liga->totalEntries[baseEntryIdx + j];

            p->binding = pc->binding;
            p->cnt = pc->cnt;
            p->flags = pc->flags;
            p->idd = NIL;
            p->type = pc->type;
            p->visibility = pc->visibility;
            AfxMakeString16(&p->name, &pc->name.s);
            ++entryCnt;
        }

        AFX_ASSERT(entryCnt == sc->entryCnt);
        liga->totalEntryCnt += entryCnt;

        s->set = sc->set;
        s->baseEntry = baseEntryIdx;
        baseEntryIdx += entryCnt;
        s->entryCnt = entryCnt;
        s->flags = sc->flags;
        s->idd = NIL;
        s->crc32 = NIL;
    }

    liga->totalEntryCnt = baseEntryIdx;
    AFX_ASSERT(liga->totalEntryCnt == cfg->pointCnt);
    liga->setCnt = cfg->setCnt;
    
    liga->pushCnt = cfg->pushCnt;
    for (afxUnit i = 0; i < cfg->pushCnt; i++)
    {
        avxPushRange const* prc = &cfg->pushes[i];
        _avxPushRange* pr = &liga->pushes[i];

        pr->offset = prc->offset;
        pr->size = prc->size;
        pr->visibility = prc->visibility;
    }

    liga->udd = cfg->udd;
    liga->tag = cfg->tag;

    AFX_ASSERT_OBJECTS(afxFcc_LIGA, 1, &liga);

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_LIGA =
{
    .fcc = afxFcc_LIGA,
    .name = "Ligature",
    .desc = "Resource Pipelining Ligature",
    .fixedSiz = sizeof(AFX_OBJECT(avxLigature)),
    .ctor = (void*)_AvxLigaCtorCb,
    .dtor = (void*)_AvxLigaDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireLigatures(afxDrawSystem dsys, afxUnit cnt, avxLigatureConfig const cfg[], avxLigature ligatures[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->ligaCls(dsys);
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

_AVX afxError AvxConfigureLigature(afxDrawSystem dsys, avxLigatureConfig* cfg, avxCodebase codb, afxUnit progCnt, afxUnit progs[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(cfg);

    *cfg = (avxLigatureConfig) { 0 };

#if 0
    afxUnit pointCnt = 0;
    afxUnit setMap[AVX_MAX_LIGAMENT_SETS];
    afxUnit setUsed = 0;
    AfxFillPattern(setMap, sizeof(setMap), (afxByte[]) { 0xFF }, 1); // Initialize to invalid set (-1)

    for (afxUnit s = 0; s < shaderCnt; s++)
    {
        avxCodebase shd = shaders[s];
        if (!shd) continue;
        AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

        avxShaderType stage = AvxGetShaderStage(shd);
        afxUnit resCnt = AvxQueryShaderInterfaces(shd, 0, 0, NIL);

        if (shd->pushConstName.s.len)
        {
            cfg->pushCnt = 1;
        }

#if 0

        // Find total amount of points and make sets for them.
        for (afxUnit j = 0; j < resCnt; j++)
        {
            avxShaderResource rsrc;
            AvxQueryShaderInterfaces(shd, j, 1, &rsrc);

            afxIndex setIdx;
            avxLigamentSet* set;
            afxBool setFound = FALSE;
            for (setIdx = 0; setIdx < cfg->setCnt; setIdx++)
            {
                set = &cfg->sets[setIdx];
                if (rsrc.set == set->set)
                {
                    setFound = TRUE;
                    break;
                }
            }

            if (!setFound)
            {
                setIdx = cfg->setCnt;
                set = &cfg->sets[setIdx];
                set->set = rsrc.set;
                set->baseEntryIdx = 0;
                set->entryCnt = 1;
                set->flags = NIL;
                ++cfg->setCnt;
            }
            else
            {
                ++set->entryCnt;
            }
        }

        // Determine the base index for points in sets
        afxUnit basePointIdx = 0;
        for (afxUnit j = 0; j < cfg->setCnt; j++)
        {
            avxLigamentSet* set = &cfg->sets[j];
            set->baseEntryIdx = basePointIdx;
            basePointIdx += set->entryCnt;
        }

        // Arrange the points linearly according to the sets.
        for (afxUnit j = 0; j < resCnt; j++)
        {
            avxShaderResource rsrc;
            AvxQueryShaderInterfaces(shd, j, 1, &rsrc);

            afxIndex setIdx;
            avxLigamentSet* set;
            afxBool setFound = FALSE;
            for (setIdx = 0; setIdx < cfg->setCnt; setIdx++)
            {
                set = &cfg->sets[setIdx];
                if (rsrc.set == set->set)
                {
                    setFound = TRUE;
                    break;
                }
            }
            AFX_ASSERT(setFound);
            AFX_ASSERT(set->set == rsrc.set);

            afxBool incompatible = FALSE;
            afxBool entryExisting = FALSE;

            for (afxUnit l = 0; l < set->entryCnt; l++)
            {
                avxLigament* binding = &cfg->points[set->baseEntryIdx + l];

                //if (set->set == rsrc.set)
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
                            binding->visibility |= /*entry->visibility |*/ AFX_BITMASK(stage);
                        }
                        break;
                    }
                }
            }

            if (!err && !entryExisting)
            {
                avxLigament* ent = &cfg->points[set->baseEntryIdx + set->entryCnt];
                ent->binding = rsrc.binding;
                ent->visibility = AFX_BITMASK(stage);
                ent->type = rsrc.type;
                ent->cnt = rsrc.cnt;
                AfxMakeString16(&ent->name, &rsrc.name.s);
                ++set->entryCnt;
                ++cfg->pointCnt;
            }

            if (incompatible)
                break;
        }
#else
#if 0
        afxUnit ligIdx = 0;
        afxUnit setCnt = 0;
        afxUnit setMap[AVX_MAX_LIGAMENT_SETS]; // Tracks existing sets
        afxUnit setUsed = 0;

        AfxFillPattern(setMap, sizeof(setMap), (afxByte[]) { 0xFF }, 1); // Initialize to invalid set (-1)

        for (afxUnit i = 0; i < resCnt; ++i)
        {
            avxShaderResource res;
            if (AvxQueryShaderInterfaces(shd, i, 1, &res) != 1)
            {
                // failture
                AfxThrowError();
                continue;
            }

            if (ligIdx >= AVX_MAX_LIGAMENTS)
            {
                // overflow
                AfxThrowError();
                continue;
            }

            // Look for existing set
            afxUnit s = 0xFFFF;
            for (afxUnit j = 0; j < setUsed; ++j)
            {
                if (setMap[j] == res.set)
                {
                    s = j;
                    break;
                }
            }

            // New set?
            if (s == 0xFFFF)
            {
                if (setUsed >= AVX_MAX_LIGAMENT_SETS)
                {
                    // overflow
                    AfxThrowError();
                    continue;
                }

                s = setUsed;
                setMap[s] = res.set;

                cfg->sets[s].set = res.set;
                cfg->sets[s].baseEntryIdx = ligIdx;
                cfg->sets[s].entryCnt = 0;
                cfg->sets[s].crc32 = 0;
                cfg->sets[s].flags = 0;
                cfg->sets[s].idd = NIL;
                setUsed++;
            }

            avxLigament* lig = &cfg->points[ligIdx];
            lig->binding = res.binding;
            lig->type = res.type;
            lig->cnt = res.cnt;
            AfxMakeString16(&lig->name, &res.name.s);
            lig->visibility = AFX_BITMASK(stage); // Adjust if you can reflect stage
            lig->flags = 0;

            cfg->sets[s].entryCnt++;
            ligIdx++;
        }
        cfg->pointCnt = ligIdx;
        cfg->setCnt = setUsed;
#else
        
        for (afxUnit i = 0; i < resCnt; ++i)
        {
            avxShaderResource res;
            if (AvxQueryShaderInterfaces(shd, i, 1, &res) != 1)
            {
                // failture
                AfxThrowError();
                continue;
            }
            afxBool found = FALSE;

            // Try to find a matching existing ligament
            for (afxUnit j = 0; j < pointCnt; ++j)
            {
                avxLigament* lig = &cfg->points[j];
                if (lig->binding == res.binding &&
                    lig->type == res.type &&
                    lig->cnt == res.cnt/* &&
                    afxStrEq(lig->name, res.name)*/)
                {
                    lig->visibility |= stage;
                    found = TRUE;
                    break;
                }
            }

            if (!found)
            {
                if (pointCnt >= AVX_MAX_LIGAMENTS)
                {
                    // overflow
                    AfxThrowError();
                    continue;
                }
                avxLigament* lig = &cfg->points[pointCnt++];
                lig->binding = res.binding;
                lig->type = res.type;
                lig->cnt = res.cnt;
                AfxMakeString16(&lig->name, &res.name.s);
                lig->visibility = stage;
                lig->flags = 0;

                // Track which set it belongs to
                afxUnit setIdx = 0xFFFF;
                for (afxUnit m = 0; m < setUsed; ++m)
                {
                    if (setMap[m] == res.set)
                    {
                        setIdx = m;
                        break;
                    }
                }

                if (setIdx == 0xFFFF)
                {
                    if (setUsed >= AVX_MAX_LIGAMENT_SETS)
                    {
                        // overflow
                        AfxThrowError();
                        continue;
                    }
                    setIdx = setUsed;
                    setMap[setIdx] = res.set;
                    cfg->sets[setIdx].set = res.set;
                    cfg->sets[setIdx].entryCnt = 0;
                    cfg->sets[setIdx].baseEntryIdx = 0; // temporary
                    cfg->sets[setIdx].crc32 = 0;
                    cfg->sets[setIdx].flags = 0;
                    cfg->sets[setIdx].idd = NIL;
                    setUsed++;
                }
                cfg->sets[setIdx].entryCnt++;
            }
        }
#endif
#endif
    }

    // Assign baseEntryIdx now that ligaments are complete
    afxUnit nextIdx = 0;
    for (afxUnit i = 0; i < setUsed; ++i)
    {
        cfg->sets[i].baseEntryIdx = nextIdx;

        // Compact ligaments of this set into contiguous slots
        afxUnit end = nextIdx + cfg->sets[i].entryCnt;
        afxUnit moved = 0;
        for (afxUnit j = nextIdx; j < pointCnt; ++j)
        {
            avxLigament temp = cfg->points[j];
            afxUnit resSet = 0xFFFF;

            // Recover the set ID for this ligament
            for (afxUnit s = 0; s < shaderCnt; ++s)
            {
                afxUnit resCnt = AvxQueryShaderInterfaces(shaders[s], 0, 0, NIL);
                for (afxUnit r = 0; r < resCnt; ++r)
                {
                    avxShaderResource res;
                    AvxQueryShaderInterfaces(shaders[s], r, 1, &res);
                    if (res.binding == temp.binding &&
                        res.type == temp.type /*&&
                        afxStrEq(res.name, temp.name)*/)
                    {
                        resSet = res.set;
                        break;
                    }
                }
                if (resSet != 0xFFFF)
                    break;
            }

            if (resSet == cfg->sets[i].set)
            {
                // Swap ligament into place
                avxLigament swap = cfg->points[nextIdx + moved];
                cfg->points[nextIdx + moved] = temp;
                cfg->points[j] = swap;
                moved++;
                if (moved >= cfg->sets[i].entryCnt)
                    break;
            }
        }

        nextIdx = end;
    }

    cfg->pointCnt = pointCnt;
    cfg->setCnt = setUsed;
#endif

    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);

    avxLigament merged[AVX_MAX_LIGAMENTS];
    afxUnit mergedSet[AVX_MAX_LIGAMENTS];
    afxUnit mergedCount = 0;

    for (afxUnit i = 0; i < progCnt; ++i)
    {
        afxUnit prog = progs[i];

        // TODO continue if invalid?

        afxMask stage = AvxGetShaderStage(codb, prog);

        cfg->pushCnt += AvxDoesShaderHavePushConstants(codb, prog) ? 1 : 0;

        afxUnit resCnt = AvxQueryShaderInterfaces(codb, prog, 0, 0, NIL);
        for (afxUnit j = 0; j < resCnt; ++j)
        {
            avxShaderResource res;
            if (AvxQueryShaderInterfaces(codb, prog, j, 1, &res) != 1)
            {
                // failure
                AfxThrowError();
                continue;
            }
            // Search for existing match
            afxBool found = FALSE;
            for (afxUnit m = 0; m < mergedCount; ++m)
            {
                avxLigament* ml = &merged[m];
                if (mergedSet[m] == res.set &&
                    ml->binding == res.binding &&
                    ml->type == res.type &&
                    ml->cnt == res.cnt /*&&
                    afxStrEq(ml->name, res.name)*/)
                {
                    ml->visibility |= stage;
                    found = TRUE;
                    break;
                }
            }

            if (!found)
            {
                if (mergedCount >= AVX_MAX_LIGAMENTS)
                {
                    // overflow
                    AfxThrowError();
                    continue;
                }
                avxLigament* ml = &merged[mergedCount++];
                mergedSet[mergedCount - 1] = res.set;
                ml->binding = res.binding;
                ml->type = res.type;
                ml->cnt = res.cnt;
                ml->visibility = stage;
                AfxMakeString16(&ml->name, &res.name.s);
            }
        }
    }

    // Group by set
    afxUnit setMap[AVX_MAX_LIGAMENT_SETS];
    afxUnit setCount = 0;
    AfxFillPattern(setMap, sizeof(setMap), (afxByte[]) { 0xFF }, 1);

    // First, map unique sets
    for (afxUnit i = 0; i < mergedCount; ++i)
    {
        afxUnit set = mergedSet[i];
        afxBool known = FALSE;
        for (afxUnit s = 0; s < setCount; ++s)
        {
            if (setMap[s] == set)
            {
                known = TRUE;
                break;
            }
        }
        if (!known)
        {
            if (setCount >= AVX_MAX_LIGAMENT_SETS)
            {
                // overflow
                AfxThrowError();
                continue;
            }
            setMap[setCount++] = set;
        }
    }

    afxUnit pointIdx = 0;
    for (afxUnit s = 0; s < setCount; ++s)
    {
        afxUnit currentSet = setMap[s];
        avxLigamentSet* ligSet = &cfg->sets[s];
        ligSet->set = currentSet;
        ligSet->baseEntryIdx = pointIdx;
        ligSet->entryCnt = 0;
        ligSet->crc32 = 0;
        ligSet->flags = 0;
        ligSet->idd = NIL;

        for (afxUnit i = 0; i < mergedCount; ++i)
        {
            avxLigament const* ml = &merged[i];

            if (mergedSet[i] == currentSet)
            {
                if (pointIdx >= AVX_MAX_LIGAMENTS)
                {
                    // overflow
                    AfxThrowError();
                    continue;
                }

                avxLigament* lig = &cfg->points[pointIdx++];
                lig->binding = ml->binding;
                lig->type = ml->type;
                lig->cnt = ml->cnt;
                lig->visibility = ml->visibility;
                lig->flags = 0;
                AfxMakeString16(&lig->name, &ml->name.s);

                ligSet->entryCnt++;
            }
        }
    }

    cfg->pointCnt = pointIdx;
    cfg->setCnt = setCount;

    return err;
}
