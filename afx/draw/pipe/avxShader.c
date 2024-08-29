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

#define _AVX_DRAW_C
#define _AVX_SHADER_C
#include "../../dev/AvxImplKit.h"
#include "qwadro/draw/io/afxXsh.h"

_AVX afxError AfxLoadGlScript(afxUri const* path, afxArray* fCode, afxArray* fInOuts, afxArray* fResources)
{
    afxError err = NIL;

    afxStream file;

    if (AfxOpenFile(path, afxIoFlag_R, &file)) AfxThrowError();
    else
    {
        afxUri inc;
        afxChar buf[2048] = { 0 };
        afxNat baseChar;
        afxString4096 src;
        AfxMakeString4096(&src, NIL);
        afxString2048 line;
        AfxMakeString2048(&line, NIL);

        while (!AfxReadFileString(file, &line.str))
        {
            afxString excerpt;

            if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("#include ")))
            {
                afxBool fnd = AfxScanString(&line.str, "#include <%128[^>]>\n", buf);

                AfxMakeUri(&inc, 0, buf, 0);

                //afxChar* room = AfxInsertArrayUnits(&bp->codes, line.str.len + 3, &baseChar, NIL);
                //AfxDumpString(&AfxString("// "), 0, 3, room);
                //AfxDumpString(&line.str, 0, line.str.len, &room[3]);

                if (AfxLoadGlScript(&inc, fCode, fInOuts, fResources))
                    AfxThrowError();

                continue;
            }
            else
            {
                if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("OUT")))
                {
                    afxNat location = 0;
                    afxString32 fmtName = { 0 };
                    afxString32 varName = { 0 };
                    afxNat fnd = AfxScanString(&line.str, "OUT(%u, %32[A-Za-z0-9], %32[a-zA-Z0-9_]);", &location, fmtName.buf, varName.buf);

                    if (fnd == 3)
                    {
                        AfxMakeUri(&inc, AFX_COUNTOF(buf), buf, 0);

                        AfxMakeString(&fmtName.str, 0, fmtName.buf, 0);
                        AfxMakeString(&varName.str, 0, varName.buf, 0);
                        afxVertexFormat fmt = AfxFindVertexFormat(&fmtName.str);

                        afxNat idx;
                        avxShaderIoChannel* decl;

                        if (!(decl = AfxInsertArrayUnit(fInOuts, &idx))) AfxThrowError();
                        else
                        {
                            AfxAssert(location < 16); // hardcoded limitation
                            decl->location = location;
                            AfxAssert(fmt < afxVertexFormat_TOTAL);
                            decl->fmt = fmt;
                            AfxMakeString8(&decl->semantic, &varName.str);
                        }
                    }
                }

                if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("IN")))
                {
                    afxNat location = 0;
                    afxString32 fmtName = { 0 };
                    afxString32 varName = { 0 };
                    afxNat fnd = AfxScanString(&line.str, "IN(%u, %32[A-Za-z0-9], %32[a-zA-Z0-9_]);", &location, fmtName.buf, varName.buf);

                    if (fnd == 3)
                    {
                        AfxMakeUri(&inc, AFX_COUNTOF(buf), buf, 0);

                        AfxMakeString(&fmtName.str, 0, fmtName.buf, 0);
                        AfxMakeString(&varName.str, 0, varName.buf, 0);

                        afxVertexFormat fmt = AfxFindVertexFormat(&fmtName.str);

                        afxNat idx;
                        avxShaderIoChannel* decl;

                        if (!(decl = AfxInsertArrayUnit(fInOuts, &idx))) AfxThrowError();
                        else
                        {
                            AfxAssert(location < 16); // hardcoded limitation
                            decl->location = location;
                            AfxAssert(fmt < afxVertexFormat_TOTAL);
                            decl->fmt = fmt;
                            AfxMakeString8(&decl->semantic, &varName.str);
                        }
                    }
                }

                {
                    if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("SAMPLING_UNIT")))
                    {
                        afxNat set = 0, binding = 0, resCnt = 1;
                        afxString32 typeName = { 0 };
                        afxString32 varName = { 0 };
                        afxNat fnd = AfxScanString(&line.str, "SAMPLING_UNIT(%u, %u, %32[A-Za-z0-9], %32[a-zA-Z0-9_] );", &set, &binding, typeName.buf, varName.buf);

                        if (fnd == 4)
                        {
                            AfxMakeUri(&inc, AFX_COUNTOF(buf), buf, 0);

                            AfxMakeString(&typeName.str, 0, typeName.buf, 0);
                            AfxMakeString(&varName.str, 0, varName.buf, 0);

                            avxShaderResType resType = 0;

                            if ((0 == AfxCompareString(&typeName.str, &AfxString("sampler1D"))) ||
                                (0 == AfxCompareString(&typeName.str, &AfxString("sampler2D"))) ||
                                (0 == AfxCompareString(&typeName.str, &AfxString("sampler3D"))) ||
                                (0 == AfxCompareString(&typeName.str, &AfxString("samplerCube")))
                                )
                            {
                                resType = avxShaderResType_COMBINED_IMAGE_SAMPLER;
                            }

                            afxNat idx;
                            avxShaderResource *decl;

                            if (!(decl = AfxInsertArrayUnit(fResources, &idx))) AfxThrowError();
                            else
                            {
                                AfxAssert(set < 4);
                                decl->set = set;
                                AfxAssert(binding < 10);
                                decl->binding = binding;
                                AfxAssert(resType < avxShaderResType_TOTAL);
                                decl->type = resType;
                                AfxAssert(decl->type != avxShaderResType_OUTPUT);
                                AfxAssert(resCnt);
                                decl->cnt = AfxMax(resCnt, 1);
                                AfxMakeString16(&decl->name, &varName.str);
                            }
                        }
                    }

                    if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("UNIFORM_UNIT")))
                    {
                        afxNat set = 0, binding = 0, resCnt = 1;
                        afxString32 varName = { 0 };
                        afxNat fnd = AfxScanString(&line.str, "UNIFORM_UNIT(%u, %u, %32[A-Za-z0-9_] );", &set, &binding, varName.buf);

                        if (fnd == 3)
                        {
                            AfxMakeUri(&inc, AFX_COUNTOF(buf), buf, 0);

                            AfxMakeString(&varName.str, 0, varName.buf, 0);

                            avxShaderResType resType = avxShaderResType_CONSTANT_BUFFER;

                            afxNat idx;
                            avxShaderResource *decl;

                            if (!(decl = AfxInsertArrayUnit(fResources, &idx))) AfxThrowError();
                            else
                            {
                                AfxAssert(set < 4);
                                decl->set = set;
                                AfxAssert(binding < 10);
                                decl->binding = binding;
                                AfxAssert(resType < avxShaderResType_TOTAL);
                                decl->type = resType;
                                AfxAssert(decl->type != avxShaderResType_OUTPUT);
                                AfxAssert(resCnt);
                                decl->cnt = AfxMax(resCnt, 1);
                                AfxMakeString16(&decl->name, &varName.str);
                            }
                        }
                    }

                    if (AFX_INVALID_INDEX != AfxFindSubstring(&line.str, &AfxString("STORAGE_UNIT")))
                    {
                        afxNat set = 0, binding = 0, resCnt = 1;
                        afxString32 accessName = { 0 };
                        afxString32 varName = { 0 };
                        afxNat fnd = AfxScanString(&line.str, "STORAGE_UNIT(%u, %u, %32[A-Za-z0-9], %32[A-Za-z0-9_] );", &set, &binding, accessName.buf, varName.buf);

                        if (fnd == 4)
                        {
                            AfxMakeUri(&inc, AFX_COUNTOF(buf), buf, 0);

                            AfxMakeString(&accessName.str, 0, accessName.buf, 0);
                            AfxMakeString(&varName.str, 0, varName.buf, 0);

                            avxShaderResType resType = avxShaderResType_STORAGE_BUFFER;

                            afxNat idx;
                            avxShaderResource *decl;

                            if (!(decl = AfxInsertArrayUnit(fResources, &idx))) AfxThrowError();
                            else
                            {
                                AfxAssert(set < 4);
                                decl->set = set;
                                AfxAssert(binding < 10);
                                decl->binding = binding;
                                AfxAssert(resType < avxShaderResType_TOTAL);
                                decl->type = resType;
                                AfxAssert(decl->type != avxShaderResType_OUTPUT);
                                AfxAssert(resCnt);
                                decl->cnt = AfxMax(resCnt, 1);
                                AfxMakeString16(&decl->name, &varName.str);
                            }
                        }
                    }
                }
            }

            void* room = AfxInsertArrayUnits(fCode, line.str.len, &baseChar, NIL);
            AfxDumpString(&line.str, 0, line.str.len, room);
        }
        AfxReleaseObjects(1, &file);
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxSerializeShader(avxShader shd, afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOB);
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    AfxWriteStream(ios, shd->codeLen, 0, shd->code);

    return err;
}

_AVX afxError AfxDumpShaderCode(avxShader shd, afxArray* arr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    afxArray fCode, fInOuts, fResources;
    AfxAllocateArray(&fCode, 4096, sizeof(afxByte), NIL);
    AfxAllocateArray(&fInOuts, 16, sizeof(avxShaderIoChannel), NIL);
    AfxAllocateArray(&fResources, 16, sizeof(avxShaderResource), NIL); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY

    AfxLoadGlScript(&shd->uri.uri, &fCode, &fInOuts, &fResources);
    AfxAppendArray(arr, &fCode);

    AfxDeallocateArray(&fCode);
    AfxDeallocateArray(&fInOuts);
    AfxDeallocateArray(&fResources);

    return err;
}

_AVX afxError AfxPrintShader(avxShader shd, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AfxAssert(uri);
    AfxAssert(!(AfxUriIsBlank(uri)));

    afxStream file;

    if (AfxOpenFile(uri, afxIoFlag_W, &file)) AfxThrowError();
    else
    {
        if (AfxSerializeShader(shd, file)) AfxThrowError();
        else
        {
            // success
        }
        AfxReleaseObjects(1, &file);
    }
    return err;
}

_AVX afxNat AfxCountShaderIoChannels(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return shd->ioDeclCnt;
}

_AVX afxNat AfxGetShaderIoChannels(avxShader shd, afxNat first, afxNat cnt, avxShaderIoChannel channels[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AfxAssertRange(shd->ioDeclCnt, first, cnt);
    AfxAssert(channels);
    AfxAssert(cnt);
    afxNat hitCnt = 0;

    avxShaderIoChannel const* ins = shd->ioDecls;
    afxNat cnt2 = AfxMin(shd->ioDeclCnt, cnt);

    for (afxNat i = 0; i < cnt2; i++)
    {
        AfxCopy(&channels[i], &ins[first + i], sizeof(channels[0]));
        hitCnt++;
    }
    return hitCnt;
}

_AVX afxNat AfxCountShaderInterfaces(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return shd->resDeclCnt;
}

_AVX afxResult AfxDescribeShaderInterfaces(avxShader shd, afxNat first, afxNat cnt, avxShaderResource rsrc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AfxAssertRange(shd->resDeclCnt, first, cnt);
    AfxAssert(rsrc);
    AfxAssert(cnt);

    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
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

_AVX avxShaderStage AfxGetShaderStage(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return shd->stage;
}

_AVX afxUri const* AfxShaderGetUri(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return &shd->uri.uri;
}

_AVX afxError _AvxShdStdDtorCb(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

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

    if (AfxDeallocateInstanceData(shd, AFX_COUNTOF(stashs), stashs))
        AfxThrowError();

    //AfxDeallocateUri(&shd->uri);

    return err;
}

_AVX afxError _AvxShdStdCtorCb(avxShader shd, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    afxDrawContext dctx = args[0];
    avxShaderConfig const* cfg = ((avxShaderConfig const*)args[1]) + invokeNo;
    
    AfxMakeUri128(&shd->uri, &cfg->uri);

    shd->verMajor = AfxElse(cfg->verMajor, 4);
    shd->verMinor = AfxElse(cfg->verMajor, shd->verMajor == 4 ? 0 : (shd->verMinor == 3 ? 3 : (shd->verMinor == 2 ? 1 : 5)));
    shd->extended = !!cfg->extended;
    shd->stage = cfg->stage;

    afxArray fCode, fInOuts, fResources;
    AfxAllocateArray(&fCode, 4096, sizeof(afxByte), NIL);
    AfxAllocateArray(&fInOuts, 16, sizeof(avxShaderIoChannel), NIL);
    AfxAllocateArray(&fResources, 16, sizeof(avxShaderResource), NIL); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY

    AfxLoadGlScript(&shd->uri.uri, &fCode, &fInOuts, &fResources);

    shd->codeLen = 0;
    shd->code = NIL;
    shd->resDeclCnt = AfxCountArrayElements(&fResources);
    shd->resDecls = NIL;
    shd->ioDeclCnt = AfxCountArrayElements(&fInOuts);
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

    if (AfxAllocateInstanceData(shd, AFX_COUNTOF(stashs), stashs)) AfxThrowError();
    else
    {
        for (afxNat j = 0; j < shd->resDeclCnt; j++)
        {
            avxShaderResource const *decl = AfxGetArrayUnit(&fResources, j);
            shd->resDecls[j].set = decl->set;
            AfxAssert(4/*_SGL_MAX_LEGO_PER_BIND*/ > shd->resDecls[j].set);
            shd->resDecls[j].binding = decl->binding;
            AfxAssert(8/*_SGL_MAX_ENTRY_PER_LEGO*/ > shd->resDecls[j].binding);
            //shd->resDecls[j].visibility = decl->visibility;
            shd->resDecls[j].type = decl->type;
            shd->resDecls[j].cnt = decl->cnt;
            AfxMakeString16(&shd->resDecls[j].name, &decl->name.str);
        }

        afxStringBase strb;
        AfxGetShaderStringBase(dctx, &strb);

        for (afxNat i = 0; i < shd->ioDeclCnt; i++)
        {
            avxShaderIoChannel* ioBp = AfxGetArrayUnit(&fInOuts, i);
            shd->ioDecls[i] = *ioBp;
            AfxMakeString8(&shd->ioDecls[i].semantic, &ioBp->semantic.str);
        }

        shd->topology = avxTopology_TRI_LIST;

        if (err && AfxDeallocateInstanceData(shd, AFX_COUNTOF(stashs), stashs))
            AfxThrowError();
    }

    AfxDeallocateArray(&fCode);
    AfxDeallocateArray(&fInOuts);
    AfxDeallocateArray(&fResources);

    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return err;
}

_AVX afxClassConfig const _AvxShdStdImplementation =
{
    .fcc = afxFcc_SHD,
    .name = "Shader",
    .desc = "Programmable Pipeline Module",
    .fixedSiz = sizeof(AFX_OBJECT(avxShader)),
    .ctor = (void*)_AvxShdStdCtorCb,
    .dtor = (void*)_AvxShdStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxCompileShaders(afxDrawContext dctx, afxNat cnt, avxShaderConfig const cfg[], avxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(shaders);
    AfxAssert(cfg);
    AfxAssert(cnt);

    afxClass* cls = AfxGetShaderClass(dctx);
    AfxAssertClass(cls, afxFcc_SHD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)shaders, (void const*[]) { dctx, cfg }))
        AfxThrowError();

    return err;
}

_AVX afxError AfxCompileShadersFromFile(afxDrawContext dctx, afxNat cnt, afxUri const uri[], avxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(shaders);
    AfxAssert(uri);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxUri const* uri2 = &uri[i];
        AfxAssert(!AfxUriIsBlank(uri2));

        avxShader shd = NIL;
        avxShaderConfig cfg = { 0 };
        cfg.uri = *uri2;
        
        if (AfxCompileShaders(dctx, 1, &cfg, &shd))
        {
            AfxThrowError();
            AfxReleaseObjects(i, (void**)shaders);
            break;
        }
        shaders[i] = shd;
    }
    return err;
}
