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
#include "ddi/avxImplementation.h"

#if 0 // VS

struct inVtx
{
    vec3 position @0;
    vec3 normal @1;
    vec2 uv @2;
};

struct outRslt
{
    vec4 position;
}

outRslt main(int vtxIdx, int instIdx, inVtx in)
{
    outRslt r;
    return r;
}
#endif

_AVX afxString const vtxFmtString[avxFormat_TOTAL] =
{
    AFX_STRING(""),
    [avxFormat_R32f] = AFX_STRING("float"),
    [avxFormat_RG32f] = AFX_STRING("vec2"),
    [avxFormat_RGB32f] = AFX_STRING("vec3"),
    [avxFormat_RGBA32f] = AFX_STRING("vec4"),
    AFX_STRING("mat2"),
    AFX_STRING("mat3"),
    AFX_STRING("mat4"),

    //AFX_STRING("V4B"),
    //AFX_STRING("v8B"),
};
_AVX avxFormat AfxFindVertexFormat(afxString const *str)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxFormat_TOTAL; i++)
    {
        if (AfxCompareStrings(str, 0, TRUE, 1, &vtxFmtString[i], NIL))
            return (avxFormat)i;
    }

    return (avxFormat)-1;
}

_AVX afxError _AvxScanGlScript(afxString const* code, afxArray* fInOuts, afxArray* fResources, afxString* pushConstsName)
{
    afxError err = NIL;

    afxUri inc;
    afxChar buf[2048] = { 0 };
    //afxUnit baseChar;
    afxString4096 src;
    AfxMakeString4096(&src, NIL);

    static afxString const keywords[] =
    {
        AFX_STRING("IN"),
        AFX_STRING("OUT"),
        AFX_STRING("UNIFORM"),
        AFX_STRING("TEXTURE"),
        AFX_STRING("BUFFER"),
        AFX_STRING("FETCH"),
        AFX_STRING("PUSH"),
    };

    // Shared among cases.
    afxUnit location = 0;
    afxUnit set = 0, binding = 0, resCnt = 1;

    afxUnit base = 0, strStep = 0;
    afxString line;
    while (1)
    {
        line = AfxExcerptStringLine(code, base);

        if (!line.len) break;

        strStep = line.len;
        base += strStep;

        afxUnit keywIdx;
        afxString excerpt = AfxFindSubstrings(&line, 0, FALSE, ARRAY_SIZE(keywords), keywords, &keywIdx);

        if (!excerpt.len)
            continue;

        switch (keywIdx)
        {
        case 0: // IN
        {
            afxString32 fmtName = { 0 };
            afxString32 varName = { 0 };
            afxUnit fnd = AfxScanString(&line, "IN(%u, %32[A-Za-z0-9], %32[a-zA-Z0-9_] );", &location, fmtName.buf, varName.buf);

            if (fnd == 3)
            {
                AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                AfxMakeString(&fmtName.s, 0, fmtName.buf, 0);
                AfxMakeString(&varName.s, 0, varName.buf, 0);

                avxFormat fmt = AfxFindVertexFormat(&fmtName.s);

                afxUnit idx;
                avxShaderIoChannel* decl;

                if (!(decl = AfxPushArrayUnits(fInOuts, 1, &idx, NIL, 0))) AfxThrowError();
                else
                {
                    AFX_ASSERT(location < 16); // hardcoded limitation
                    decl->location = location;
                    AFX_ASSERT(fmt < avxFormat_TOTAL);
                    decl->fmt = fmt;
                    AfxMakeString16(&decl->semantic, &varName.s);
                }
            }
            break;
        }
        case 1: // OUT
        {
            afxString32 fmtName = { 0 };
            afxString32 varName = { 0 };
            afxUnit fnd = AfxScanString(&line, "OUT(%u, %32[A-Za-z0-9], %32[a-zA-Z0-9_] );", &location, fmtName.buf, varName.buf);

            if (fnd == 3)
            {
                AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                AfxMakeString(&fmtName.s, 0, fmtName.buf, 0);
                AfxMakeString(&varName.s, 0, varName.buf, 0);
                avxFormat fmt = AfxFindVertexFormat(&fmtName.s);

                afxUnit idx;
                avxShaderIoChannel* decl;

                if (!(decl = AfxPushArrayUnits(fInOuts, 1, &idx, NIL, 0))) AfxThrowError();
                else
                {
                    AFX_ASSERT(location < 16); // hardcoded limitation
                    decl->location = location;
                    AFX_ASSERT(fmt < avxFormat_TOTAL);
                    decl->fmt = fmt;
                    AfxMakeString16(&decl->semantic, &varName.s);
                }
            }
            break;
        }
        case 2: // UNIFORM
        {
            afxString32 varName = { 0 };
            afxString32 layoutName = { 0 };
            afxUnit fnd = AfxScanString(&line, "UNIFORM(%u, %u, %32[A-Za-z0-9_], %32[A-Za-z0-9_] )", &set, &binding, layoutName.buf, varName.buf);

            if (fnd == 4)
            {
                AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                AfxMakeString(&varName.s, 0, varName.buf, 0);

                avxShaderParam resType = avxShaderParam_UNIFORM;

                afxUnit idx;
                avxShaderResource *decl;

                if (!(decl = AfxPushArrayUnits(fResources, 1, &idx, NIL, 0))) AfxThrowError();
                else
                {
                    AFX_ASSERT(set < AVX_MAX_LIGAMENT_SETS);
                    decl->set = set;
                    AFX_ASSERT(binding < AVX_MAX_LIGAMENTS);
                    decl->binding = binding;
                    AFX_ASSERT(resType < avxShaderParam_TOTAL);
                    decl->type = resType;
                    AFX_ASSERT(decl->type != avxShaderParam_OUT);
                    AFX_ASSERT(resCnt);
                    decl->cnt = AFX_MAX(resCnt, 1);
                    AfxMakeString16(&decl->name, &varName.s);
                }
            }
            break;
        }
        case 3: // TEXTURE
        {
            afxString32 varName = { 0 };
            afxString32 typeName = { 0 };
            afxUnit fnd = AfxScanString(&line, "TEXTURE(%u, %u, %32[A-Za-z0-9], %32[a-zA-Z0-9_] );", &set, &binding, typeName.buf, varName.buf);

            if (fnd == 4)
            {
                AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                AfxMakeString(&typeName.s, 0, typeName.buf, 0);
                AfxMakeString(&varName.s, 0, varName.buf, 0);

                avxShaderParam resType = 0;

                if ((AfxCompareStrings(&typeName.s, 0, FALSE, 1, &AFX_STRING("sampler1D"), NIL)) ||
                    (AfxCompareStrings(&typeName.s, 0, FALSE, 1, &AFX_STRING("sampler2D"), NIL)) ||
                    (AfxCompareStrings(&typeName.s, 0, FALSE, 1, &AFX_STRING("sampler3D"), NIL)) ||
                    (AfxCompareStrings(&typeName.s, 0, FALSE, 1, &AFX_STRING("samplerCube"), NIL))
                    )
                {
                    resType = avxShaderParam_TEXTURE;
                }

                afxUnit idx;
                avxShaderResource *decl;

                if (!(decl = AfxPushArrayUnits(fResources, 1, &idx, NIL, 0))) AfxThrowError();
                else
                {
                    AFX_ASSERT(set < AVX_MAX_LIGAMENT_SETS);
                    decl->set = set;
                    AFX_ASSERT(binding < AVX_MAX_LIGAMENTS);
                    decl->binding = binding;
                    AFX_ASSERT(resType < avxShaderParam_TOTAL);
                    decl->type = resType;
                    AFX_ASSERT(decl->type != avxShaderParam_OUT);
                    AFX_ASSERT(resCnt);
                    decl->cnt = AFX_MAX(resCnt, 1);
                    AfxMakeString16(&decl->name, &varName.s);
                }
            }
            break;
        }
        case 4: // BUFFER
        {
            afxString32 varName = { 0 };
            afxString32 layoutName = { 0 };
            afxString32 accessName = { 0 };
            afxUnit fnd = AfxScanString(&line, "BUFFER(%u, %u, %32[A-Za-z0-9], %32[A-Za-z0-9], %32[A-Za-z0-9_] )", &set, &binding, layoutName.buf, accessName.buf, varName.buf);

            if (fnd == 5)
            {
                AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                AfxMakeString(&accessName.s, 0, accessName.buf, 0);
                AfxMakeString(&varName.s, 0, varName.buf, 0);

                avxShaderParam resType = avxShaderParam_BUFFER;

                afxUnit idx;
                avxShaderResource *decl;

                if (!(decl = AfxPushArrayUnits(fResources, 1, &idx, NIL, 0))) AfxThrowError();
                else
                {
                    AFX_ASSERT(set < AVX_MAX_LIGAMENT_SETS);
                    decl->set = set;
                    AFX_ASSERT(binding < AVX_MAX_LIGAMENTS);
                    decl->binding = binding;
                    AFX_ASSERT(resType < avxShaderParam_TOTAL);
                    decl->type = resType;
                    AFX_ASSERT(decl->type != avxShaderParam_OUT);
                    AFX_ASSERT(resCnt);
                    decl->cnt = AFX_MAX(resCnt, 1);
                    AfxMakeString16(&decl->name, &varName.s);
                }
            }
            break;
        }
        case 5: // FETCH
        {
            afxString32 varName = { 0 };
            afxUnit fnd = AfxScanString(&line, "FETCH(%u, %u, %32[A-Za-z0-9_] )", &set, &binding, varName.buf);

            if (fnd == 3)
            {
                AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);

                AfxMakeString(&varName.s, 0, varName.buf, 0);

                avxShaderParam resType = avxShaderParam_FETCH;

                afxUnit idx;
                avxShaderResource *decl;

                if (!(decl = AfxPushArrayUnits(fResources, 1, &idx, NIL, 0))) AfxThrowError();
                else
                {
                    AFX_ASSERT(set < AVX_MAX_LIGAMENT_SETS);
                    decl->set = set;
                    AFX_ASSERT(binding < AVX_MAX_LIGAMENTS);
                    decl->binding = binding;
                    AFX_ASSERT(resType < avxShaderParam_TOTAL);
                    decl->type = resType;
                    AFX_ASSERT(decl->type != avxShaderParam_OUT);
                    AFX_ASSERT(resCnt);
                    decl->cnt = AFX_MAX(resCnt, 1);
                    AfxMakeString16(&decl->name, &varName.s);
                }
            }
            break;
        }
        case 6: // PUSH
        {
            afxString32 varName = { 0 };
            afxUnit fnd = AfxScanString(&line, "PUSH(%32[A-Za-z0-9_] )", varName.buf);

            if (fnd == 1)
            {
                AfxMakeUri(&inc, ARRAY_SIZE(buf), buf, 0);
                AfxMakeString(&varName.s, 0, varName.buf, 0);
                AfxCopyString(pushConstsName, 0, &varName.s, 0);
            }
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_AVX afxError _AvxLoadGlScript(afxStream file, afxArray* fCode)
{
    afxError err = NIL;

    afxStream inc;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &inc);

    afxChar buf[2048] = { 0 };
    afxUnit baseChar;
    afxString4096 src;
    AfxMakeString4096(&src, NIL);
    afxString2048 line;
    AfxMakeString2048(&line, NIL);

    while (!AfxReadFileLine(file, &line.s))
    {
        afxString excerpt =AfxFindSubstring(&line.s, 0, FALSE, &AFX_STRING("#include "));

        if (excerpt.len)
        {
            afxBool fnd = AfxScanString(&line.s, "#include <%128[^>]>\n", buf);

            afxUri incUri;
            AfxMakeUri(&incUri, 0, buf, 0);

            if (AfxReopenFile(inc, &incUri, afxFileFlag_R)) AfxThrowError();
            else
            {
                //afxChar* room = AfxPushArrayUnits(&bp->codes, line.s.len + 3, &baseChar, NIL);
                //AfxDumpString(&AFX_STRING("// "), 0, 3, room);
                //AfxDumpString(&line.s, 0, line.s.len, &room[3]);

                if (_AvxLoadGlScript(inc, fCode))
                    AfxThrowError();
            }
            continue;
        }

        void* room = AfxPushArrayUnits(fCode, line.s.len, &baseChar, NIL, 0);
        AfxDumpString(&line.s, 0, line.s.len, room);
    }
    AfxDisposeObjects(1, &inc);
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AVX afxBool AvxGetProgram(avxCodebase shd, afxUnit unit, void** slot)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    _avxCodeBlock* slot2;
    afxBool rslt = AfxGetPoolUnit(&shd->codes, unit, (void**)&slot2);
    AFX_ASSERT(slot);
    *slot = slot2;
    return rslt;
}

_AVX afxBool AvxFindPrograms(avxCodebase shd, afxUnit cnt, afxString const names[], afxUnit units[])
{
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    afxDrawSystem dsys = AfxGetHost(shd);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxBool AvxDoesShaderHavePushConstants(avxCodebase shd, afxUnit prog)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return 0;

    return slot->pushConstName.s.len;
}

_AVX afxError AvxSerializeShader(avxCodebase shd, afxUnit prog, afxStream ios)
{
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
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

_AVX afxUnit AvxQueryShaderIoChannels(avxCodebase shd, afxUnit prog, afxUnit first, afxUnit cnt, avxShaderIoChannel channels[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return afxError_NOT_FOUND;

    AFX_ASSERT_RANGE(slot->ioDeclCnt, first, cnt);
    AFX_ASSERT(cnt);
    afxUnit hitCnt = 0;

    cnt = AFX_MIN(slot->ioDeclCnt, cnt);
    first = AFX_MIN(first, slot->ioDeclCnt - 1);

    // if channels is not specified, it must return the total of declarations.

    if (!channels) hitCnt = slot->ioDeclCnt;
    else
    {
        avxShaderIoChannel const* ins = slot->ioDecls;

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
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return NIL;

    return slot->stage;
}

_AVX afxString const* AfxGetShaderName(avxCodebase shd, afxUnit prog)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    _avxCodeBlock* slot;
    if (!AvxGetProgram(shd, prog, (void**)&slot))
        return NIL;// afxError_NOT_FOUND;

    return &slot->name.s;
}

_AVX afxError AvxRecompileShader(avxCodebase shd, afxUnit prog, afxString const* code)
{
    afxError err = AFX_ERR_NONE;
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
                .cnt = slot->ioDeclCnt,
                .siz = sizeof(slot->ioDecls[0]),
                .var = (void**)&slot->ioDecls
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
        AFX_ASSERT(slot->ioDecls == NIL);
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

    afxArray fInOuts, fResources;
    AfxMakeArray(&fInOuts, sizeof(avxShaderIoChannel), 16, NIL, 0);
    AfxMakeArray(&fResources, sizeof(avxShaderResource), 16, NIL, 0); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY

    if (code && code->len)
    {
        _AvxScanGlScript(code, &fInOuts, &fResources, &slot->pushConstName.s);
        slot->codeLen = code->len + 1; // keep code in memory
    }
    else AfxThrowError();

    slot->resDeclCnt = fResources.pop;
    slot->resDecls = NIL;
    slot->ioDeclCnt = fInOuts.pop;
    slot->ioDecls = NIL;

    afxObjectStash stashs[] =
    {
        {
            .cnt = slot->resDeclCnt,
            .siz = sizeof(slot->resDecls[0]),
            .var = (void**)&slot->resDecls
        },
        {
            .cnt = slot->ioDeclCnt,
            .siz = sizeof(slot->ioDecls[0]),
            .var = (void**)&slot->ioDecls
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

        for (afxUnit i = 0; i < slot->ioDeclCnt; i++)
        {
            avxShaderIoChannel* ioBp = AfxGetArrayUnit(&fInOuts, i);
            slot->ioDecls[i] = *ioBp;
            AfxMakeString16(&slot->ioDecls[i].semantic, &ioBp->semantic.s);
        }

        slot->topology = avxTopology_TRI_LIST;

        if (err && AfxDeallocateInstanceData(shd, ARRAY_SIZE(stashs), stashs))
            AfxThrowError();
    }

    AfxEmptyArray(&fInOuts, FALSE, FALSE);
    AfxEmptyArray(&fResources, FALSE, FALSE);

    return err;
}

_AVX afxError AvxRecompileShaderFromDisk(avxCodebase shd, afxUnit prog, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
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
    afxError err = AFX_ERR_NONE;
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
                        .cnt = slot->ioDeclCnt,
                        .siz = sizeof(slot->ioDecls[0]),
                        .var = (void**)&slot->ioDecls
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
                AFX_ASSERT(slot->ioDecls == NIL);
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
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxUri const* url = args[1] ? AFX_CAST(afxUri const*, args[1]) + invokeNo : NIL;
    
    AfxMakeUri128(&shd->url, url);
    AfxSetUpPool(&shd->codes, sizeof(_avxCodeBlock), 1, 0);

    return err;
}

_AVX afxClassConfig const _AVX_SHD_CLASS_CONFIG =
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
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(codebases);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->shadCls(dsys);
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
