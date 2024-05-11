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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AVX_DRAW_C
#define _AVX_SHADER_C
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/io/afxXsh.h"

////////////////////////////////////////////////////////////////////////////////
// SHADER BLUEPRINT                                                           //
////////////////////////////////////////////////////////////////////////////////

_AVXINL void AfxShaderBlueprintRename(afxShaderBlueprint *blueprint, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    AfxCopyUri(&blueprint->uri.uri, uri);
}

_AVXINL void AfxShaderBlueprintChooseEntryPoint(afxShaderBlueprint *blueprint, afxString const *entry)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    AfxCopyString(&blueprint->entry.str, entry);
}

_AVXINL void AfxShaderBlueprintSetStage(afxShaderBlueprint *blueprint, afxShaderStage stage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    //AfxAssert(stage);
    blueprint->stage = stage;
}

_AVXINL afxError AfxShaderBlueprintAddCode(afxShaderBlueprint *blueprint, void const *start, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);

    AfxAssert(range);

    afxMmu mmu = blueprint->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);

    afxNat idx;
    afxByte iniVal = 0;
    afxByte *bytecode;

    if (!(bytecode = AfxInsertArrayUnits(&blueprint->codes, range, &idx, &iniVal))) AfxThrowError();
    else
    {
        AfxCopy2(1, range, start, bytecode);
    }
    return err;
}

_AVXINL afxError AfxShaderBlueprintAddCodeFromStream(afxShaderBlueprint *blueprint, afxStream ios, afxSize offset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);

    AfxAssert(ios);
    AfxAssert(offset);
    AfxAssert(range);

    AfxAssertObjects(1, &ios, afxFcc_IOB);
    afxNat cachedPosn = AfxGetStreamPosn(ios);

    if (AfxSeekStreamFromBegin(ios, offset)) AfxThrowError();
    else
    {
        afxMmu mmu = blueprint->mmu;
        AfxAssertObjects(1, &mmu, afxFcc_MMU);

        afxNat idx;
        afxByte iniVal = 0;
        afxByte *bytecode;

        if (!(bytecode = AfxInsertArrayUnits(&blueprint->codes, range, &idx, &iniVal))) AfxThrowError();
        else
        {
            if (AfxReadStream(ios, range, 0, bytecode))
                AfxThrowError();
        }
    }
    AfxSeekStreamFromBegin(ios, (afxInt)cachedPosn);
    return err;
}

_AVXINL afxError AfxShaderBlueprintAddCodeFromResource(afxShaderBlueprint *blueprint, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);

    afxMmu mmu = blueprint->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);

    afxStream file;
    AfxAssert(uri);

    if (!(file = AfxOpenFile(uri, afxIoFlag_RX))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);
        afxNat size = AfxMeasureStream(file);
        AfxAssert(size);

        afxNat idx;
        afxByte iniVal = 0;
        afxByte *bytecode;

        if (!(bytecode = AfxInsertArrayUnits(&blueprint->codes, size, &idx, &iniVal))) AfxThrowError();
        else
        {
            if (AfxReadStream(file, size, 0, bytecode))
                AfxThrowError();
        }
        AfxReleaseObjects(1, &file);
    }
    return err;
}

_AVXINL afxError AfxShaderBlueprintDeclareResource(afxShaderBlueprint *blueprint, afxNat set, afxNat binding, afxShaderResourceType type, afxNat cnt, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    afxNat idx;
    afxShaderBlueprintResource *decl;

    if (!(decl = AfxInsertArrayUnit(&blueprint->resources, &idx))) AfxThrowError();
    else
    {
        AfxAssert(set < 4);
        decl->set = set;
        AfxAssert(binding < 10);
        decl->binding = binding;
        AfxAssert(type < AFX_SHD_RES_TYPE_TOTAL);
        decl->type = type;
        AfxAssert(decl->type != AFX_SHD_RES_TYPE_OUTPUT);
        AfxAssert(cnt);
        decl->cnt = AfxMax(cnt, 1);
         AfxCloneString(&decl->name, name);
    }
    return err;
}

_AVXINL afxError AfxShaderBlueprintDeclareInOut(afxShaderBlueprint *blueprint, afxNat location, afxVertexFormat fmt, afxNat stream, afxString const *semantic)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    afxNat idx;
    afxShaderBlueprintInOut *decl;

    if (!(decl = AfxInsertArrayUnit(&blueprint->inOuts, &idx))) AfxThrowError();
    else
    {
        AfxAssert(location < 16); // hardcoded limitation
        decl->location = location;
        AfxAssert(fmt < afxVertexFormat_TOTAL);
        decl->fmt = fmt;
        decl->stream = stream;
         AfxCloneString(&decl->semantic, semantic);
    }
    return err;
}

_AVXINL void AfxShaderBlueprintErase(afxShaderBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);

    AfxClearUri(&blueprint->uri.uri);
    blueprint->stage = NIL;
    AfxClearString(&blueprint->entry.str);

    AfxEmptyArray(&blueprint->codes);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->resources); i++)
    {
        afxShaderBlueprintResource *decl = AfxGetArrayUnit(&blueprint->resources, i);

        //if (decl->name)
        {
            AfxDeallocateString(&decl->name);
            //decl->name = NIL;
        }
    }
    AfxEmptyArray(&blueprint->resources);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->inOuts); i++)
    {
        afxShaderBlueprintInOut *decl = AfxGetArrayUnit(&blueprint->inOuts, i);

        //if (decl->semantic)
        {
            AfxDeallocateString(&decl->semantic);
            //decl->semantic = NIL;
        }
    }
    AfxEmptyArray(&blueprint->inOuts);

    blueprint->topology = NIL;
}

_AVXINL afxError AfxShaderBlueprintEnd(afxShaderBlueprint *blueprint, afxNat cnt, afxShader shd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    
    if (cnt)
    {
        AfxAssert(shd);

        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxCompileShaders(blueprint->dctx, 1, blueprint, &shd[i]))
            {
                AfxThrowError();
                
                for (afxNat j = 0; j < i; j++)
                {
                    AfxReleaseObjects(1, &shd[j]);
                    shd[j] = NIL;
                }
                break;
            }
        }
    }

    afxMmu mmu = blueprint->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);

    AfxDeallocateArray(&blueprint->codes);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->resources); i++)
    {
        afxShaderBlueprintResource *decl = AfxGetArrayUnit(&blueprint->resources, i);

        //if (decl->name)
            AfxDeallocateString(&decl->name);
    }
    AfxDeallocateArray(&blueprint->resources);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->inOuts); i++)
    {
        afxShaderBlueprintInOut *decl = AfxGetArrayUnit(&blueprint->inOuts, i);

        //if (decl->semantic)
            AfxDeallocateString(&decl->semantic);
    }
    AfxDeallocateArray(&blueprint->inOuts);

    blueprint->fcc = NIL;
    return err;
}

_AVXINL void AfxShaderBlueprintBegin(afxShaderBlueprint* blueprint, afxShaderStage stage, afxUri const *uri, afxString const *entry, afxNat estCodeLen, afxNat estIoCnt, afxNat estResCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->fcc = afxFcc_SHDB;

    afxMmu mmu = NIL;
    //AfxAssertObjects(1, &mmu, afxFcc_MMU);
    blueprint->mmu = mmu;

    AfxMakeUri128(&blueprint->uri, NIL);

    if (uri)
        AfxCopyUri(&blueprint->uri.uri, uri);

    blueprint->stage = stage;

    AfxMakeString8(&blueprint->entry, entry);

    AfxAllocateArray(&blueprint->codes, AfxMax(estCodeLen, 2048), sizeof(afxByte), NIL);
    AfxAllocateArray(&blueprint->resources, AfxMax(estResCnt, 16), sizeof(afxShaderBlueprintResource), NIL); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY
    AfxAllocateArray(&blueprint->inOuts, AfxMax(estIoCnt, 16), sizeof(afxShaderBlueprintInOut), NIL);

    blueprint->topology = NIL;
}

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxPrintShader(afxShader shd, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AfxAssert(uri);
    AfxAssert(!(AfxUriIsBlank(uri)));

    afxStream file;

    if (!(file = AfxOpenFile(uri, afxIoFlag_W))) AfxThrowError();
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

_AVX afxError AfxSerializeShader(afxShader shd, afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOB);
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    AfxWriteStream(ios, shd->codeLen, 0, shd->code);

    return err;
}

_AVX afxNat AfxCountShaderInterfaces(afxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    return shd->resDeclCnt;
}

_AVX afxResult AfxDescribeShaderInterfaces(afxShader shd, afxNat first, afxNat cnt, afxShaderResource rsrc[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(rsrc);
    AfxAssert(cnt);

    AfxAssertObjects(1, &shd, afxFcc_SHD);

    AfxAssert(first < shd->resDeclCnt);
    AfxAssert(first + cnt <= shd->resDeclCnt);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        rsrc[i].set = shd->resDecls[first + i].set;
        rsrc[i].binding = shd->resDecls[first + i].binding;
        rsrc[i].type = shd->resDecls[first + i].type;
        rsrc[i].cnt = shd->resDecls[first + i].cnt;
        rsrc[i].name = shd->resDecls[first + i].name.str;
        ++rslt;
    }
    return rslt;
}

_AVX afxShaderStage AfxGetShaderStage(afxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return shd->stage;
}

_AVX afxUri const* AfxShaderGetUri(afxShader shd)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &shd, afxFcc_SHD);

    return &shd->uri;
}

_AVX afxError _AvxShdStdDtor(afxShader shd)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = AfxGetObjectProvider(shd);

    if (shd->resDecls)
    {
        for (afxNat j = 0; j < shd->resDeclCnt; j++)
        {
            AfxDeallocateString(&shd->resDecls[j].name);
        }

        AfxDeallocate(shd->resDecls);
    }

    if (shd->ioDecls)
    {
        for (afxNat j = 0; j < shd->ioDeclCnt; j++)
        {
            AfxDeallocateString(&shd->ioDecls[j].semantic);
        }

        AfxDeallocate(shd->ioDecls);
    }

    if (shd->code)
    {
        AfxDeallocate(shd->code);
    }

    AfxDeallocateUri(&shd->uri);

    return err;
}

_AVX afxError _AvxShdStdCtor(afxShader shd, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    afxShaderBlueprint const *blueprint = ((afxShaderBlueprint const *)cookie->udd[0]) + cookie->no;

    afxDrawContext dctx = AfxGetObjectProvider(shd);

    AfxDuplicateUri(&shd->uri, &blueprint->uri.uri);

    shd->stage = blueprint->stage;

    AfxCloneString(&shd->entry, &blueprint->entry.str.str);

    shd->code = NIL;
    afxNat codeLen = AfxCountArrayElements(&blueprint->codes);

    if (codeLen && !(shd->code = AfxAllocate(codeLen, sizeof(afxChar), 0, AfxHere()))) AfxThrowError();
    else
    {
        AfxAssertType(&blueprint->codes, afxFcc_ARR);
        AfxDumpArray(&blueprint->codes, shd->code);
    }

    shd->codeLen = codeLen;
    //shd->code[shd->codeLen] = '\0';

    AfxLogComment("%.*s", shd->codeLen, shd->code);

    if (!err)
    {
        afxString src;
        AfxMakeString(&src, shd->code, shd->codeLen);
        afxString128 line;
        AfxMakeString128(&line, NIL);
        afxString32 type;
        AfxMakeString32(&type, NIL);
        afxString128 name;
        AfxMakeString128(&name, NIL);

        while (1)
        {
            AfxScanString(&src, "%99[^\n]", line.buf);
            AfxMakeRestring(&line.str, sizeof(line.buf), line.buf, 0);
            AfxMakeString(&src, &shd->code[src.len], shd->codeLen - src.len);

            afxNat location = AFX_INVALID_INDEX;

            if (shd->stage == afxShaderStage_VERTEX)
                AfxScanString(&line.str.str, "layout ( location = %u ) in %[A-Za-z0-9] %[A-Za-z0-9]", &location, &type.buf, &name.buf);
            else if (shd->stage == afxShaderStage_FRAGMENT)
                AfxScanString(&line.str.str, "layout ( location = %u ) out %[A-Za-z0-9] %[A-Za-z0-9]", &location, &type.buf, &name.buf);

            AfxMakeRestring(&type.str, sizeof(type.buf), type.buf, 0);
            AfxMakeRestring(&name.str, sizeof(name.buf), name.buf, 0);

            AfxLogEcho("layout ( location = %u ) out %[A-Za-z0-9] %[A-Za-z0-9]", location, type.buf, name.buf);
            break;
        }




        shd->resDeclCnt = 0;
        shd->resDecls = NIL;

        afxNat resDeclCnt = AfxCountArrayElements(&blueprint->resources);

        if (resDeclCnt && !(shd->resDecls = AfxAllocate(resDeclCnt, sizeof(shd->resDecls[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            for (afxNat j = 0; j < resDeclCnt; j++)
            {
                afxShaderBlueprintResource const *decl = AfxGetArrayUnit(&blueprint->resources, j);
                shd->resDecls[j].set = decl->set;
                AfxAssert(4/*_SGL_MAX_LEGO_PER_BIND*/ > shd->resDecls[j].set);
                shd->resDecls[j].binding = decl->binding;
                AfxAssert(8/*_SGL_MAX_ENTRY_PER_LEGO*/ > shd->resDecls[j].binding);
                //shd->resDecls[j].visibility = decl->visibility;
                shd->resDecls[j].type = decl->type;
                AfxAssert(shd->resDecls[j].type != AFX_SHD_RES_TYPE_OUTPUT);
                shd->resDecls[j].cnt = decl->cnt;
                AfxCloneString(&shd->resDecls[j].name, &decl->name.str);
                ++shd->resDeclCnt;
            }
        }

        if (!err)
        {
            afxNat ioCnt = AfxCountArrayElements(&blueprint->inOuts);
            shd->ioDeclCnt = 0;
            shd->ioDecls = NIL;

            if (ioCnt && !(shd->ioDecls = AfxAllocate(ioCnt, sizeof(shd->ioDecls[0]), 0, AfxHere()))) AfxThrowError();
            else
            {
                for (afxNat i = 0; i < ioCnt; i++)
                {
                    afxShaderBlueprintInOut* ioBp = AfxGetArrayUnit(&blueprint->inOuts, i);
                    shd->ioDecls[i] = *ioBp;
                    AfxCloneString(&shd->ioDecls[i].semantic, &ioBp->semantic.str);
                    ++shd->ioDeclCnt;
                }
            }

            shd->topology = blueprint->topology;

            if (err && shd->ioDecls)
                AfxDeallocate(shd->ioDecls);
        }

        if (err && shd->resDecls)
            AfxDeallocate(shd->resDecls);
    }

    if (err && shd->code)
        AfxDeallocate(shd->code);

    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return err;
}

_AVX afxClassConfig const _AvxShdStdImplementation =
{
    .fcc = afxFcc_SHD,
    .name = "Shader",
    .desc = "Shader Management",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxShader)),
    .ctor = (void*)_AvxShdStdCtor,
    .dtor = (void*)_AvxShdStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxCompileShaders(afxDrawContext dctx, afxNat cnt, afxShaderBlueprint const blueprint[], afxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(blueprint);
    AfxAssert(shaders);
    AfxAssert(cnt);

    afxManager* cls = AfxGetShaderClass(dctx);
    AfxAssertClass(cls, afxFcc_SHD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)shaders, (void const*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
}

_AVX afxShader AfxCompileShaderFromXml(afxDrawContext dctx, afxNat specIdx, afxXml const* xml, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertType(xml, afxFcc_XML);
    afxShader shd = NIL;

    afxShaderBlueprint blueprint;
    AfxShaderBlueprintBegin(&blueprint, NIL, NIL, NIL, 0, 0, 0);

    if (AfxParseXmlBackedShaderBlueprint(&blueprint, specIdx, xml, elemIdx)) AfxThrowError();
    else
    {
#if 0
        afxString128 tmp;
        AfxMakeString128(&tmp);
        AfxCopyString(&tmp.str, AfxGetUriString(&fpath));

        if (!AfxUriIsBlank(&blueprint.uri.uri))
        {
            AfxConcatenateStringL(&tmp.str, "?", 1);
            AfxConcatenateString(&tmp.str, AfxGetUriString(&blueprint.uri.uri));
        }

        afxUri tmpUri;
        AfxUriFromString(&tmpUri, &tmp.str);
        AfxShaderBlueprintRename(&blueprint, &tmpUri);
#endif
        if (AfxCompileShaders(dctx, 1, &blueprint, &shd))
            AfxThrowError();
    }
    AfxShaderBlueprintEnd(&blueprint, 0, NIL);

    AfxTryAssertObjects(1, &shd, afxFcc_SHD);
    return shd;
}

_AVX afxShader AfxCompileShaderFromXsh(afxDrawContext dctx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(uri);
    AfxAssert(!AfxUriIsBlank(uri));
    afxShader shd = NIL;

    AfxLogEcho("Uploading shader '%.*s'", AfxPushString(&uri->str.str));

    afxUri fext;
    AfxPickUriExtension(uri, FALSE, &fext);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath;
        AfxPickUriPath(uri, &fpath);

        if (0 == AfxCompareStringCil(AfxGetUriString(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);

                afxString query;
                AfxPickUriQueryToString(uri, TRUE, &query);

                afxNat xmlElemIdx = 0;
                afxNat foundCnt = AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("Shader"), &AfxStaticString("id"), 1, &query, &xmlElemIdx);
                AfxAssert(xmlElemIdx != AFX_INVALID_INDEX);

                if (foundCnt)
                {
                    if (!(shd = AfxCompileShaderFromXml(dctx, 0, &xml, xmlElemIdx))) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &shd, afxFcc_SHD);
                    }
                }
                AfxCleanUpXml(&xml);
            }
        }
    }
    return shd;
}

_AVX afxError AfxCompileShadersFromXsh(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(shaders);
    AfxAssert(uri);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(!AfxUriIsBlank(&uri[i]));

        if (!(shaders[i] = AfxCompileShaderFromXsh(dctx, &uri[i])))
        {
            AfxThrowError();
            AfxReleaseObjects(i, (void**)shaders);
            break;
        }
    }
    return err;
}
