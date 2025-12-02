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

// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_SHADER_C
#include "avxIcd.h"

_AVX afxBool AvxGetProgram(avxCodebase shd, afxUnit unit, void** slot)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    _avxCodeBlock* slot2;
    afxBool rslt = AfxGetPoolUnit(&shd->codes, unit, (void**)&slot2);
    AFX_ASSERT(slot);
    *slot = slot2;
    return rslt;
}

_AVX afxBool AvxFindPrograms(avxCodebase shd, afxUnit cnt, afxString const names[], afxUnit units[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    AFX_ASSERT(names);
    afxBool rslt = 0;

    for (afxUnit iter = 0; iter < cnt; iter++)
    {
        afxString const* name = &names[iter];

        _avxCodeBlock* unit;
        for (afxUnit pageIdx = 0; pageIdx < (&shd->codes)->pageCnt; ++pageIdx)
        {
            afxPoolPage* _page = (&shd->codes)->pages[pageIdx];
            for (afxMask _u = _page->usage, slotIdx = 0; slotIdx < (&shd->codes)->unitsPerPage; ++slotIdx)
            {
                if (!(_u & (1u << slotIdx))) continue;
                unit = (_avxCodeBlock*)(_page->base + (slotIdx * (&shd->codes)->unitSiz));

                afxUnit matchId;
                if (AfxCompareStrings(name, 0, FALSE, 1, &unit->name.s, &matchId))
                {
                    if (units)
                        units[iter] = (pageIdx * shd->codes.unitsPerPage + slotIdx);

                    ++rslt;
                    break; // iterate pool
                }
            }
        }
    }
    return rslt;
}

_AVX afxDrawSystem AvxGetShaderHost(avxCodebase shd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    afxDrawSystem dsys = AfxGetHost(shd);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxBool AvxDoesShaderHavePushConstants(avxCodebase shd, afxUnit prog)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return 0;

    return slot->pushConstName.s.len;
}

_AVX afxError AvxSerializeShader(avxCodebase shd, afxUnit prog, afxStream ios)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &ios);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return afxError_NOT_FOUND;

    AfxWriteStream(ios, slot->codeLen, 0, slot->code);

    return err;
}

_AVX afxError AvxDumpShaderCode(avxCodebase shd, afxUnit prog, afxArray* arr)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return afxError_NOT_FOUND;

    afxArray fCode;

    if (slot->codeLen)
    {
        AfxMakeArray(&fCode, 1, slot->codeLen, slot->code, slot->codeLen);
        AfxAppendArray(arr, &fCode);
    }
    return err;
}

_AVX afxError AvxPrintShader(avxCodebase shd, afxUnit prog, afxUri const *uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return afxError_NOT_FOUND;

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    AFX_ASSERT(uri);
    AFX_ASSERT(!(AfxIsUriBlank(uri)));
    if (AfxReopenFile(file, uri, afxFileFlag_W))
    {
        AfxThrowError();
        return err;
    }

    if (AvxSerializeShader(shd, prog, file))
        AfxThrowError();

    AfxDisposeObjects(1, &file);

    return err;
}

_AVX afxUnit AvxQueryShaderIns(avxCodebase shd, afxUnit prog, afxUnit first, afxUnit cnt, avxShaderIoChannel channels[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return afxError_NOT_FOUND;

    AFX_ASSERT_RANGE(slot->inCnt, first, cnt);
    AFX_ASSERT(cnt);
    afxUnit hitCnt = 0;

    cnt = AFX_MIN(slot->inCnt, cnt);
    first = AFX_MIN(first, slot->inCnt - 1);

    // if channels is not specified, it must return the total of declarations.

    if (!channels) hitCnt = slot->inCnt;
    else
    {
        avxShaderIoChannel const* ins = slot->ins;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxCopy(&channels[i], &ins[first + i], sizeof(channels[0]));
            hitCnt++;
        }
    }
    return hitCnt;
}

_AVX afxUnit AvxQueryShaderOuts(avxCodebase shd, afxUnit prog, afxUnit first, afxUnit cnt, avxShaderIoChannel channels[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return afxError_NOT_FOUND;

    AFX_ASSERT_RANGE(slot->outCnt, first, cnt);
    AFX_ASSERT(cnt);
    afxUnit hitCnt = 0;

    cnt = AFX_MIN(slot->outCnt, cnt);
    first = AFX_MIN(first, slot->outCnt - 1);

    // if channels is not specified, it must return the total of declarations.

    if (!channels) hitCnt = slot->outCnt;
    else
    {
        avxShaderIoChannel const* ins = slot->outs;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxCopy(&channels[i], &ins[first + i], sizeof(channels[0]));
            hitCnt++;
        }
    }
    return hitCnt;
}

_AVX afxUnit AvxQueryShaderInterfaces(avxCodebase shd, afxUnit prog, afxUnit first, afxUnit cnt, avxShaderResource rsrc[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return afxError_NOT_FOUND;

    AFX_ASSERT_RANGE(slot->resDeclCnt, first, cnt);
    //AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    cnt = AFX_MIN(cnt, slot->resDeclCnt);
    first = AFX_MIN(first, slot->resDeclCnt - 1);

    if (!rsrc) rslt = slot->resDeclCnt;
    else for (afxUnit i = 0; i < cnt; i++)
    {
        rsrc[i].set = slot->resDecls[first + i].set;
        rsrc[i].binding = slot->resDecls[first + i].binding;
        rsrc[i].type = slot->resDecls[first + i].type;
        rsrc[i].cnt = slot->resDecls[first + i].cnt;
        rsrc[i].name = slot->resDecls[first + i].name;
        ++rslt;
    }
    return rslt;
}

_AVX avxShaderType AvxGetShaderStage(avxCodebase shd, afxUnit prog)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return NIL;

    return slot->stage;
}

_AVX afxString const* AfxGetShaderName(avxCodebase shd, afxUnit prog)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return NIL;// afxError_NOT_FOUND;

    return &slot->name.s;
}

_AVX afxError AvxRecompileShader(avxCodebase shd, afxUnit prog, afxString const* code)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return afxError_NOT_FOUND;

    // Deallocate, free and remove previous data.

    if (slot->codeLen)
    {
        afxObjectStash stashs[] =
        {
            {
                .cnt = slot->resDeclCnt,
                .siz = sizeof(slot->resDecls[0]),
                .var = (void**)&slot->resDecls
            },
            {
                .cnt = slot->inCnt,
                .siz = sizeof(slot->ins[0]),
                .var = (void**)&slot->ins
            },
            {
                .cnt = slot->outCnt,
                .siz = sizeof(slot->outs[0]),
                .var = (void**)&slot->outs
            },
            {
                .cnt = slot->codeLen,
                .siz = sizeof(slot->code[0]),
                .var = (void**)&slot->code
            }
        };

        if (AfxDeallocateInstanceData(shd, ARRAY_SIZE(stashs), stashs))
            AfxThrowError();

        slot->codeLen = 0;
        AFX_ASSERT(slot->code == NIL);
        AFX_ASSERT(slot->ins == NIL);
        AFX_ASSERT(slot->outs == NIL);
        AFX_ASSERT(slot->resDecls == NIL);
    }

    // Abandon the mission if any error does have occuried.
    if (err) return err;

    // How to set it? Make it persistent? Remove it?
    slot->tag = (afxString) { 0 };
    slot->udd = NIL;

    //shd->verMajor = AFX_OR(cfg->verMajor, 4);
    //shd->verMinor = AFX_OR(cfg->verMajor, shd->verMajor == 4 ? 0 : (shd->verMinor == 3 ? 3 : (shd->verMinor == 2 ? 1 : 5)));
    //shd->extended = !!cfg->extended;
    slot->stage = 0;

    AfxMakeString32(&slot->pushConstName, NIL);

    afxArray fIns, fOuts, fResources;
    AfxMakeArray(&fIns, sizeof(avxShaderIoChannel), 16, NIL, 0);
    AfxMakeArray(&fOuts, sizeof(avxShaderIoChannel), 16, NIL, 0);
    AfxMakeArray(&fResources, sizeof(avxShaderResource), 16, NIL, 0); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY

    if (code && code->len)
    {
        _AvxScanGlScript(code, &fIns, &fOuts, &fResources, &slot->pushConstName.s);
        slot->codeLen = code->len + 1; // keep code in memory
    }
    else AfxThrowError();

    slot->resDeclCnt = fResources.pop;
    slot->resDecls = NIL;
    slot->inCnt = fIns.pop;
    slot->ins = NIL;
    slot->outCnt = fOuts.pop;
    slot->outs = NIL;

    afxObjectStash stashs[] =
    {
        {
            .cnt = slot->resDeclCnt,
            .siz = sizeof(slot->resDecls[0]),
            .var = (void**)&slot->resDecls
        },
        {
            .cnt = slot->inCnt,
            .siz = sizeof(slot->ins[0]),
            .var = (void**)&slot->ins
        },
        {
            .cnt = slot->outCnt,
            .siz = sizeof(slot->outs[0]),
            .var = (void**)&slot->outs
        },
        {
            .cnt = slot->codeLen,
            .siz = sizeof(slot->code[0]),
            .var = (void**)&slot->code
        }
    };

    if (AfxAllocateInstanceData(shd, ARRAY_SIZE(stashs), stashs)) AfxThrowError();
    else
    {
        if (slot->codeLen)
        {
            AfxCopy(slot->code, code->start, code->len);
            slot->code[slot->codeLen] = '\0';
        }

        for (afxUnit j = 0; j < slot->resDeclCnt; j++)
        {
            avxShaderResource const *decl = AfxGetArrayUnit(&fResources, j);
            slot->resDecls[j].set = decl->set;
            AFX_ASSERT(AVX_MAX_LIGAMENT_SETS > slot->resDecls[j].set);
            slot->resDecls[j].binding = decl->binding;
            AFX_ASSERT(AVX_MAX_LIGAMENTS > slot->resDecls[j].binding);
            //shd->resDecls[j].visibility = decl->visibility;
            slot->resDecls[j].type = decl->type;
            slot->resDecls[j].cnt = decl->cnt;
            AfxMakeString16(&slot->resDecls[j].name, &decl->name.s);
        }

        for (afxUnit i = 0; i < slot->inCnt; i++)
        {
            avxShaderIoChannel* ioBp = AfxGetArrayUnit(&fIns, i);
            slot->ins[i] = *ioBp;
            AfxMakeString16(&slot->ins[i].semantic, &ioBp->semantic.s);
        }

        for (afxUnit i = 0; i < slot->outCnt; i++)
        {
            avxShaderIoChannel* ioBp = AfxGetArrayUnit(&fOuts, i);
            slot->outs[i] = *ioBp;
            AfxMakeString16(&slot->outs[i].semantic, &ioBp->semantic.s);
        }

        slot->topology = avxTopology_TRI_LIST;

        if (err && AfxDeallocateInstanceData(shd, ARRAY_SIZE(stashs), stashs))
            AfxThrowError();
    }

    AfxEmptyArray(&fIns, FALSE, FALSE);
    AfxEmptyArray(&fOuts, FALSE, FALSE);
    AfxEmptyArray(&fResources, FALSE, FALSE);

    return err;
}

_AVX afxError AvxRecompileShaderFromDisk(avxCodebase shd, afxUnit prog, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return afxError_NOT_FOUND;

    AFX_ASSERT(uri);
    AFX_ASSERT(!AfxIsUriBlank(uri));

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    if (AfxAcquireStream(1, &iobi, &file)) AfxThrowError();
    else
    {
        if (AfxReopenFile(file, uri, afxFileFlag_R)) AfxThrowError();
        else
        {
            afxArray fCode;
            afxString fCodeS;
            AfxMakeArray(&fCode, sizeof(afxByte), 4096, NIL, 0);
            _AvxLoadGlScript(file, &fCode);
            AfxMakeString(&fCodeS, fCode.pop, fCode.items, fCode.pop);

            if (AvxRecompileShader(shd, prog, &fCodeS))
                AfxThrowError();

            AfxEmptyArray(&fCode, FALSE, FALSE);
        }
        AfxDisposeObjects(1, &file);
    }

    return err;
}

_AVX afxError AvxCompileShader(avxCodebase shd, afxString const* prog, afxString const* code)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    afxUnit slotId;
    _avxCodeBlock* slot;
    if (!AvxFindPrograms(shd, 1, prog, &slotId))
    {
        // If there is not a code unit, allocate a new one.

        if (AfxRequestPoolUnits(&shd->codes, AfxHere(), 1, &slotId, (void**)&slot))
        {
            AfxThrowError();
            return err;
        }

        AfxZero(slot, sizeof(slot[0]));

        AfxMakeString32(&slot->name, prog);
        //slot->tag;
        //slot->udd;
    }
    else
    {
        if (!AvxGetProgram(shd, slotId, (void**)&slot))
        {
            AfxThrowError();
            return err;
        }
    }

    if (err) return err;

    if (code)
        if (AvxRecompileShader(shd, slotId, code))
            AfxThrowError();

    return err;
}

_AVX afxError AvxCompileShaderFromDisk(avxCodebase shd, afxString const* prog, afxUri const* uri)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    afxUnit slotId;
    _avxCodeBlock* slot;
    if (!AvxFindPrograms(shd, 1, prog, &slotId))
    {
        // If there is not a code unit, allocate a new one.

        if (AfxRequestPoolUnits(&shd->codes, AfxHere(), 1, &slotId, (void**)&slot))
        {
            AfxThrowError();
            return err;
        }

        AfxZero(slot, sizeof(slot[0]));

        AfxMakeString32(&slot->name, prog);
        //slot->tag;
        //slot->udd;
    }
    else
    {
        if (!AvxGetProgram(shd, slotId, (void**)&slot))
        {
            AfxThrowError();
            return err;
        }
    }

    if (err) return err;

    AFX_ASSERT(uri);
    AFX_ASSERT(!AfxIsUriBlank(uri));

    if (AvxRecompileShaderFromDisk(shd, slotId, uri))
        AfxThrowError();

    return err;
}

_AVX afxError _AvxShdDtorCb(avxCodebase shd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    // Detach and free every code unit.

    _avxCodeBlock* slot;
    for (afxUnit pageIdx = 0; pageIdx < (&shd->codes)->pageCnt; ++pageIdx)
    {
        afxPoolPage* _page = (&shd->codes)->pages[pageIdx];
        for (afxMask _u = _page->usage, slotIdx = 0; slotIdx < (&shd->codes)->unitsPerPage; ++slotIdx)
        {
            if (!(_u & (1u << slotIdx))) continue;
            slot = (_avxCodeBlock*)(_page->base + (slotIdx * (&shd->codes)->unitSiz));

            if (slot->codeLen)
            {
                afxObjectStash stashs[] =
                {
                    {
                        .cnt = slot->resDeclCnt,
                        .siz = sizeof(slot->resDecls[0]),
                        .var = (void**)&slot->resDecls
                    },
                    {
                        .cnt = slot->inCnt,
                        .siz = sizeof(slot->ins[0]),
                        .var = (void**)&slot->ins
                    },
                    {
                        .cnt = slot->outCnt,
                        .siz = sizeof(slot->outs[0]),
                        .var = (void**)&slot->outs
                    },
                    {
                        .cnt = slot->codeLen,
                        .siz = sizeof(slot->code[0]),
                        .var = (void**)&slot->code
                    }
                };

                if (AfxDeallocateInstanceData(shd, ARRAY_SIZE(stashs), stashs))
                    AfxThrowError();

                slot->codeLen = 0;
                AFX_ASSERT(slot->code == NIL);
                AFX_ASSERT(slot->ins == NIL);
                AFX_ASSERT(slot->outs == NIL);
                AFX_ASSERT(slot->resDecls == NIL);
            }
        }
    }

    // Deallocate the pool.
    AfxExhaustPool(&shd->codes, FALSE);

    return err;
}

_AVX afxError _AvxShdCtorCb(avxCodebase shd, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxUri const* url = args[1] ? AFX_CAST(afxUri const*, args[1]) + invokeNo : NIL;
    
    AfxMakeUri128(&shd->url, url);
    AfxSetUpPool(&shd->codes, sizeof(_avxCodeBlock), 1, 0);

    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_CODB =
{
    .fcc = afxFcc_SHD,
    .name = "Shader",
    .desc = "Programmable Pipeline Module",
    .fixedSiz = sizeof(AFX_OBJECT(avxCodebase)),
    .ctor = (void*)_AvxShdCtorCb,
    .dtor = (void*)_AvxShdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireCodebases(afxDrawSystem dsys, afxUnit cnt, afxUri const urls[], avxCodebase codebases[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(codebases);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->shadCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_SHD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)codebases, (void const*[]) { dsys, urls }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SHD, cnt, codebases);

#if AVX_VALIDATION_ENABLED

#endif

    return err;
}
