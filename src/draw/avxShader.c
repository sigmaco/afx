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

_AVX afxDrawSystem AvxGetShaderHost(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    afxDrawSystem dsys = AfxGetHost(shd);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    return dsys;
}

_AVX afxError AvxSerializeShader(avxShader shd, afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &ios);
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    AfxWriteStream(ios, shd->codeLen, 0, shd->code);

    return err;
}

_AVX afxError AvxDumpShaderCode(avxShader shd, afxArray* arr)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    afxArray fCode;

    if (shd->codeLen)
    {
        AfxMakeArray(&fCode, 1, shd->codeLen, shd->code, shd->codeLen);
        AfxAppendArray(arr, &fCode);
    }
    else
    {
        afxStream file;
        afxStreamInfo iobi = { 0 };
        iobi.usage = afxIoUsage_FILE;
        iobi.flags = afxIoFlag_READABLE;
        if (AfxAcquireStream(1, &iobi, &file))
        {
            AfxThrowError();
            return err;
        }

        afxUri* path = &shd->uri.uri;
        AFX_ASSERT(!AfxIsUriBlank(path));

        if (AfxReopenFile(file, path, afxFileFlag_R)) AfxThrowError();
        else
        {
            AfxMakeArray(&fCode, sizeof(afxByte), 4096, NIL, 0);
            _AvxLoadGlScript(file, &fCode);
            AfxAppendArray(arr, &fCode);
            AfxEmptyArray(&fCode, FALSE, FALSE);
        }
        AfxDisposeObjects(1, &file);
    }

    return err;
}

_AVX afxError AvxPrintShader(avxShader shd, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    AFX_ASSERT(uri);
    AFX_ASSERT(!(AfxIsUriBlank(uri)));

    afxStream file;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &file);

    if (AfxReopenFile(file, uri, afxFileFlag_W))
    {
        AfxThrowError();
        return err;
    }

    if (AvxSerializeShader(shd, file))
        AfxThrowError();

    AfxDisposeObjects(1, &file);

    return err;
}

_AVX afxUnit AvxQueryShaderIoChannels(avxShader shd, afxUnit first, afxUnit cnt, avxShaderIoChannel channels[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    AFX_ASSERT_RANGE(shd->ioDeclCnt, first, cnt);
    AFX_ASSERT(cnt);
    afxUnit hitCnt = 0;

    cnt = AFX_MIN(shd->ioDeclCnt, cnt);
    first = AFX_MIN(first, shd->ioDeclCnt - 1);

    // if channels is not specified, it must return the total of declarations.

    if (!channels) hitCnt = shd->ioDeclCnt;
    else
    {
        avxShaderIoChannel const* ins = shd->ioDecls;

        for (afxUnit i = 0; i < cnt; i++)
        {
            AfxCopy(&channels[i], &ins[first + i], sizeof(channels[0]));
            hitCnt++;
        }
    }
    return hitCnt;
}

_AVX afxUnit AvxQueryShaderInterfaces(avxShader shd, afxUnit first, afxUnit cnt, avxShaderResource rsrc[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    AFX_ASSERT_RANGE(shd->resDeclCnt, first, cnt);
    //AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    cnt = AFX_MIN(cnt, shd->resDeclCnt);
    first = AFX_MIN(first, shd->resDeclCnt - 1);

    if (!rsrc) rslt = shd->resDeclCnt;
    else for (afxUnit i = 0; i < cnt; i++)
    {
        rsrc[i].set = shd->resDecls[first + i].set;
        rsrc[i].binding = shd->resDecls[first + i].binding;
        rsrc[i].type = shd->resDecls[first + i].type;
        rsrc[i].cnt = shd->resDecls[first + i].cnt;
        rsrc[i].name = shd->resDecls[first + i].name;
        ++rslt;
    }
    return rslt;
}

_AVX avxShaderType AvxGetShaderStage(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    return shd->stage;
}

_AVX afxUri const* AfxShaderGetUri(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    return &shd->uri.uri;
}

_AVX afxError _AvxShdDtorCb(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    afxObjectStash stashs[] =
    {
        {
            .cnt = shd->resDeclCnt,
            .siz = sizeof(shd->resDecls[0]),
            .var = (void**)&shd->resDecls
        },
        {
            .cnt = shd->ioDeclCnt,
            .siz = sizeof(shd->ioDecls[0]),
            .var = (void**)&shd->ioDecls
        },
        {
            .cnt = shd->codeLen,
            .siz = sizeof(shd->code[0]),
            .var = (void**)&shd->code
        }
    };

    if (AfxDeallocateInstanceData(shd, ARRAY_SIZE(stashs), stashs))
        AfxThrowError();

    //AfxDeallocateUri(&shd->uri);

    return err;
}

_AVX afxError _AvxShdCtorCb(avxShader shd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);

    //afxDrawSystem dsys = args[0];
    afxUri const* uri = args[1] ? ((afxUri const*)args[1]) + invokeNo : NIL;
    afxString const* code = args[2] ? ((afxString const*)args[2]) + invokeNo : NIL;
    
    AfxMakeUri128(&shd->uri, uri);

    shd->tag = (afxString) { 0 };
    shd->udd = NIL;

    //shd->verMajor = AFX_OR(cfg->verMajor, 4);
    //shd->verMinor = AFX_OR(cfg->verMajor, shd->verMajor == 4 ? 0 : (shd->verMinor == 3 ? 3 : (shd->verMinor == 2 ? 1 : 5)));
    //shd->extended = !!cfg->extended;
    shd->stage = 0;

    AfxMakeString32(&shd->pushConstName, NIL);

    afxArray fInOuts, fResources;
    AfxMakeArray(&fInOuts, sizeof(avxShaderIoChannel), 16, NIL, 0);
    AfxMakeArray(&fResources, sizeof(avxShaderResource), 16, NIL, 0); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY

    if (code && code->len)
    {
        _AvxScanGlScript(code, &fInOuts, &fResources, &shd->pushConstName.s);
        shd->codeLen = code->len + 1; // keep code in memory
    }
    else if (uri)
    {
        shd->codeLen = 0; // dont keep code in memory

        afxStream file;
        afxStreamInfo iobi = { 0 };
        iobi.usage = afxIoUsage_FILE;
        iobi.flags = afxIoFlag_READABLE;
        if (AfxAcquireStream(1, &iobi, &file)) AfxThrowError();
        else
        {
            AFX_ASSERT(!AfxIsUriBlank(uri));

            if (AfxReopenFile(file, uri, afxFileFlag_R)) AfxThrowError();
            else
            {
                afxArray fCode;
                afxString fCodeS;
                AfxMakeArray(&fCode, sizeof(afxByte), 4096, NIL, 0);
                _AvxLoadGlScript(file, &fCode);
                AfxMakeString(&fCodeS, fCode.pop, fCode.items, fCode.pop);
                _AvxScanGlScript(&fCodeS, &fInOuts, &fResources, &shd->pushConstName.s);
                AfxEmptyArray(&fCode, FALSE, FALSE);
            }
            AfxDisposeObjects(1, &file);
        }
    }
    else AfxThrowError();

    shd->resDeclCnt = fResources.pop;
    shd->resDecls = NIL;
    shd->ioDeclCnt = fInOuts.pop;
    shd->ioDecls = NIL;

    afxObjectStash stashs[] =
    {
        {
            .cnt = shd->resDeclCnt,
            .siz = sizeof(shd->resDecls[0]),
            .var = (void**)&shd->resDecls
        },
        {
            .cnt = shd->ioDeclCnt,
            .siz = sizeof(shd->ioDecls[0]),
            .var = (void**)&shd->ioDecls
        },
        {
            .cnt = shd->codeLen,
            .siz = sizeof(shd->code[0]),
            .var = (void**)&shd->code
        }
    };

    if (AfxAllocateInstanceData(shd, ARRAY_SIZE(stashs), stashs)) AfxThrowError();
    else
    {
        if (shd->codeLen)
        {
            AfxCopy(shd->code, code->start, code->len);
            shd->code[shd->codeLen] = '\0';
        }

        for (afxUnit j = 0; j < shd->resDeclCnt; j++)
        {
            avxShaderResource const *decl = AfxGetArrayUnit(&fResources, j);
            shd->resDecls[j].set = decl->set;
            AFX_ASSERT(AVX_MAX_LIGAMENT_SETS > shd->resDecls[j].set);
            shd->resDecls[j].binding = decl->binding;
            AFX_ASSERT(AVX_MAX_LIGAMENTS > shd->resDecls[j].binding);
            //shd->resDecls[j].visibility = decl->visibility;
            shd->resDecls[j].type = decl->type;
            shd->resDecls[j].cnt = decl->cnt;
            AfxMakeString16(&shd->resDecls[j].name, &decl->name.s);
        }

        for (afxUnit i = 0; i < shd->ioDeclCnt; i++)
        {
            avxShaderIoChannel* ioBp = AfxGetArrayUnit(&fInOuts, i);
            shd->ioDecls[i] = *ioBp;
            AfxMakeString16(&shd->ioDecls[i].semantic, &ioBp->semantic.s);
        }

        shd->topology = avxTopology_TRI_LIST;

        if (err && AfxDeallocateInstanceData(shd, ARRAY_SIZE(stashs), stashs))
            AfxThrowError();
    }

    AfxEmptyArray(&fInOuts, FALSE, FALSE);
    AfxEmptyArray(&fResources, FALSE, FALSE);

    AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &shd);
    return err;
}

_AVX afxClassConfig const _AVX_SHD_CLASS_CONFIG =
{
    .fcc = afxFcc_SHD,
    .name = "Shader",
    .desc = "Programmable Pipeline Module",
    .fixedSiz = sizeof(AFX_OBJECT(avxShader)),
    .ctor = (void*)_AvxShdCtorCb,
    .dtor = (void*)_AvxShdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireShaders(afxDrawSystem dsys, afxUnit cnt, afxUri const uris[], afxString const codes[], avxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(uris || codes);
    AFX_ASSERT(shaders);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->shadCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_SHD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)shaders, (void const*[]) { dsys, uris, codes }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_SHD, cnt, shaders);

#if AVX_VALIDATION_ENABLED

#endif

    return err;
}
