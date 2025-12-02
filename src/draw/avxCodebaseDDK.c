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
    afxError err = { 0 };
    AFX_ASSERT(str);

    for (afxUnit i = 0; i < avxFormat_TOTAL; i++)
    {
        if (AfxCompareStrings(str, 0, TRUE, 1, &vtxFmtString[i], NIL))
            return (avxFormat)i;
    }

    return (avxFormat)-1;
}

_AVX afxError _AvxScanGlScript(afxString const* code, afxArray* fIns, afxArray* fOuts, afxArray* fResources, afxString* pushConstsName)
{
    afxError err = { 0 };

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
        AFX_STRING("#include")
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

                if (!(decl = AfxPushArrayUnits(fIns, 1, &idx, NIL, 0))) AfxThrowError();
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

                if (!(decl = AfxPushArrayUnits(fOuts, 1, &idx, NIL, 0))) AfxThrowError();
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
        case 7: // #include
        {
            afxChar buf[128] = { 0 };
            afxBool fnd = AfxScanString(&line, "#include <%128[^>]>\n", buf);
#if !0
            afxStream inc;
            afxStreamInfo iobi = { 0 };
            iobi.usage = afxIoUsage_FILE;
            iobi.flags = afxIoFlag_READABLE;
            if (AfxAcquireStream(1, &iobi, &inc))
            {
                AfxThrowError();
                break;
            }

            afxArray array2;
            AfxMakeArray(&array2, 1, 512, 0, 0);

            afxUri incUri;
            AfxMakeUri(&incUri, 0, buf, 0);
            if (AfxReopenFile(inc, &incUri, afxFileFlag_R))
            {
                AfxThrowError();
                AfxEmptyArray(&array2, FALSE, FALSE);
                AfxDisposeObjects(1, &inc);
                break;
            }

            if (_AvxLoadGlScript(inc, &array2))
                AfxThrowError();

            afxString s;
            AfxMakeString(&s, 0, array2.bytemap, array2.pop);
            _AvxScanGlScript(&s, fIns, fOuts, fResources, pushConstsName);

            AfxEmptyArray(&array2, FALSE, FALSE);
            AfxDisposeObjects(1, &inc);
#endif
            break;
        }
        default: AfxThrowError(); break;
        }
    }
    return err;
}

_AVX afxError _AvxLoadGlScript(afxStream file, afxArray* fCode)
{
    afxError err = { 0 };

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
        afxString excerpt = AfxFindSubstring(&line.s, 0, FALSE, &AFX_STRING("#include "));

        if (excerpt.len)
        {
            afxBool fnd = AfxScanString(&line.s, "#include <%128[^>]>\n", buf);

            if (!fnd)
                fnd = AfxScanString(&line.s, "#inc <%128[^>]>\n", buf);

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

_AVX afxError _AvxConvertToGlsl(afxString const* src, afxArray* fCode)
{
    afxError err = { 0 };

    afxStream inc;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &inc);

    afxChar buf[2048] = { 0 };
    afxUnit baseChar;
    afxString line;
    
    afxUnit base = 0;
    while ((line = AfxExcerptStringLine(src, base)), line.len)
    {
        base += line.len;
        afxString excerpt = AfxFindSubstring(&line, 0, FALSE, &AFX_STRING("#include "));

        if (excerpt.len)
        {
            afxBool fnd = AfxScanString(&line, "#include <%128[^>]>\n", buf);

            if (!fnd)
                fnd = AfxScanString(&line, "#inc <%128[^>]>\n", buf);

            afxUri incUri;
            AfxMakeUri(&incUri, 0, buf, 0);
            if (AfxReopenFile(inc, &incUri, afxFileFlag_R))
            {
                AfxThrowError();
                break;
            }

            afxArray array2;
            AfxMakeArray(&array2, 1, 512, 0, 0);

            if (_AvxLoadGlScript(inc, &array2))
                AfxThrowError();

            afxString s;
            AfxMakeString(&s, 0, array2.bytemap, array2.pop);
            _AvxConvertToGlsl(&s, fCode);

            AfxEmptyArray(&array2, FALSE, FALSE);
            
            continue;
        }

        void* room = AfxPushArrayUnits(fCode, line.len, &baseChar, NIL, 0);
        AfxDumpString(&line, 0, line.len, room);
    }
    AfxDisposeObjects(1, &inc);
    return err;
}
