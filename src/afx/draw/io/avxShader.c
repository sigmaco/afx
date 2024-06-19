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
#include "../dev/AvxDevKit.h"
#include "qwadro/draw/io/afxXsh.h"

////////////////////////////////////////////////////////////////////////////////
// SHADER BLUEPRINT                                                           //
////////////////////////////////////////////////////////////////////////////////

_AVXINL void AfxShaderBlueprintRename(avxShaderBlueprint *blueprint, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    AfxCopyUri(&blueprint->uri.uri, uri);
}

_AVXINL void AfxShaderBlueprintChooseEntryPoint(avxShaderBlueprint *blueprint, afxString const *entry)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    AfxCopyString(&blueprint->entry.str, entry);
}

_AVXINL void AfxShaderBlueprintSetStage(avxShaderBlueprint *blueprint, avxShaderStage stage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    //AfxAssert(stage);
    blueprint->stage = stage;
}

_AVXINL afxError AfxShaderBlueprintAddCode(avxShaderBlueprint *blueprint, void const *start, afxNat range)
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

_AVXINL afxError AfxShaderBlueprintAddCodeFromStream(avxShaderBlueprint *blueprint, afxStream ios, afxSize offset, afxNat range)
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

_AVXINL afxError AfxShaderBlueprintAddCodeFromResource(avxShaderBlueprint *blueprint, afxUri const *uri)
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

_AVXINL afxError AfxShaderBlueprintDeclareResource(avxShaderBlueprint *blueprint, afxNat set, afxNat binding, avxShaderResourceType type, afxNat cnt, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    afxNat idx;
    avxShaderBlueprintResource *decl;

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

_AVXINL afxError AfxShaderBlueprintDeclareInOut(avxShaderBlueprint *blueprint, afxNat location, afxVertexFormat fmt, afxNat stream, afxString const *semantic)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    afxNat idx;
    avxShaderIoChannel *decl;

    if (!(decl = AfxInsertArrayUnit(&blueprint->inOuts, &idx))) AfxThrowError();
    else
    {
        AfxAssert(location < 16); // hardcoded limitation
        decl->location = location;
        AfxAssert(fmt < afxVertexFormat_TOTAL);
        decl->fmt = fmt;
        decl->stream = stream;
         //AfxCloneString(&decl->semantic, semantic);
    }
    return err;
}

_AVXINL void AfxShaderBlueprintErase(avxShaderBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);

    AfxClearUri(&blueprint->uri.uri);
    blueprint->stage = NIL;
    AfxClearString(&blueprint->entry.str);

    AfxEmptyArray(&blueprint->codes);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->resources); i++)
    {
        avxShaderBlueprintResource *decl = AfxGetArrayUnit(&blueprint->resources, i);

        //if (decl->name)
        {
            AfxDeallocateString(&decl->name);
            //decl->name = NIL;
        }
    }
    AfxEmptyArray(&blueprint->resources);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->inOuts); i++)
    {
        avxShaderIoChannel *decl = AfxGetArrayUnit(&blueprint->inOuts, i);

        //if (decl->semantic)
        {
            //AfxDeallocateString(&decl->semantic);
            //decl->semantic = NIL;
        }
    }
    AfxEmptyArray(&blueprint->inOuts);

    blueprint->topology = NIL;
}

_AVXINL afxError AfxShaderBlueprintEnd(avxShaderBlueprint *blueprint, afxNat cnt, avxShader shd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    

    afxMmu mmu = blueprint->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);

    AfxDeallocateArray(&blueprint->codes);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->resources); i++)
    {
        avxShaderBlueprintResource *decl = AfxGetArrayUnit(&blueprint->resources, i);

        //if (decl->name)
            //AfxDeallocateString(&decl->name);
    }
    AfxDeallocateArray(&blueprint->resources);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->inOuts); i++)
    {
        avxShaderIoChannel *decl = AfxGetArrayUnit(&blueprint->inOuts, i);

        //if (decl->semantic)
            //AfxDeallocateString(&decl->semantic);
    }
    AfxDeallocateArray(&blueprint->inOuts);

    blueprint->fcc = NIL;
    return err;
}

_AVXINL void AfxShaderBlueprintBegin(avxShaderBlueprint* blueprint, avxShaderStage stage, afxUri const *uri, afxString const *entry, afxNat estCodeLen, afxNat estIoCnt, afxNat estResCnt)
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
    AfxAllocateArray(&blueprint->resources, AfxMax(estResCnt, 16), sizeof(avxShaderBlueprintResource), NIL); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY
    AfxAllocateArray(&blueprint->inOuts, AfxMax(estIoCnt, 16), sizeof(avxShaderIoChannel), NIL);

    blueprint->topology = NIL;
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

    avxShaderBlueprint shdb;
    AfxShaderBlueprintBegin(&shdb, NIL, NIL, NIL, 0, 0, 0);
    AfxLoadGlScript(&shdb, &shd->uri);
    AfxAppendArray(arr, &shdb.codes);
    AfxShaderBlueprintEnd(&shdb, 0, NIL);

    return err;
}

_AVX afxError AfxPrintShader(avxShader shd, afxUri const *uri)
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
        AfxCopy2(1, sizeof(channels[0]), &ins[first + i], &channels[i]);
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
        rsrc[i].name = shd->resDecls[first + i].name.str;
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

_AVX afxBool AfxGetShaderStringBase(avxShader shd, afxStringBase* base)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AfxAssert(base);
    afxStringBase strb = shd->strb;
    AfxTryAssertObjects(1, &strb, afxFcc_STRB);
    *base = shd->strb;
    return !!strb;
}

_AVX afxUri const* AfxShaderGetUri(avxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return &shd->uri;
}

_AVX afxError _AvxShdStdDtor(avxShader shd)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = AfxGetObjectProvider(shd);

    if (shd->resDecls)
        AfxDeallocate(shd->resDecls);

    if (shd->ioDecls)
        AfxDeallocate(shd->ioDecls);

    if (shd->code)
        AfxDeallocate(shd->code);

    if (shd->strb)
        AfxReleaseObjects(1, &shd->strb);

    AfxDeallocateUri(&shd->uri);

    return err;
}

_AVX afxError _AvxShdStdCtor(avxShader shd, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    afxDrawContext dctx = cookie->udd[0];
    avxShaderConfig const* cfg = ((avxShaderConfig const*)cookie->udd[1]) + cookie->no;
    
    AfxDuplicateUri(&shd->uri, &cfg->uri);

    shd->verMajor = AfxElse(cfg->verMajor, 4);
    shd->verMinor = AfxElse(cfg->verMajor, shd->verMajor == 4 ? 0 : (shd->verMinor == 3 ? 3 : (shd->verMinor == 2 ? 1 : 5)));
    shd->extended = !!cfg->extended;
    shd->stage = cfg->stage;

    if (AfxAcquireStringCatalogs(1, &shd->strb)) AfxThrowError();
    else
    {
        //AfxCatalogStrings2(&shd->strb, 1, &blueprint->entry.str.str, &shd->entry);
        
        avxShaderBlueprint shdb;
        AfxShaderBlueprintBegin(&shdb, NIL, NIL, NIL, 0, 0, 0);
        AfxLoadGlScript(&shdb, &shd->uri);

        shd->code = NIL;
        shd->codeLen = 0;
        afxNat codeLen = shdb.codes.cnt;

        if (codeLen && !(shd->code = AfxAllocate(codeLen, sizeof(afxChar), 0, AfxHere()))) AfxThrowError();
        else
        {
            shd->codeLen = codeLen;
            AfxDumpArray(&shdb.codes, shd->code);
            AfxLogComment("%.*s", shd->codeLen, shd->code);
        }

        if (!err)
        {
            shd->resDeclCnt = 0;
            shd->resDecls = NIL;

            afxNat resDeclCnt = AfxCountArrayElements(&shdb.resources);

            if (resDeclCnt && !(shd->resDecls = AfxAllocate(resDeclCnt, sizeof(shd->resDecls[0]), 0, AfxHere()))) AfxThrowError();
            else
            {
                for (afxNat j = 0; j < resDeclCnt; j++)
                {
                    avxShaderBlueprintResource const *decl = AfxGetArrayUnit(&shdb.resources, j);
                    shd->resDecls[j].set = decl->set;
                    AfxAssert(4/*_SGL_MAX_LEGO_PER_BIND*/ > shd->resDecls[j].set);
                    shd->resDecls[j].binding = decl->binding;
                    AfxAssert(8/*_SGL_MAX_ENTRY_PER_LEGO*/ > shd->resDecls[j].binding);
                    //shd->resDecls[j].visibility = decl->visibility;
                    shd->resDecls[j].type = decl->type;
                    shd->resDecls[j].cnt = decl->cnt;
                    AfxCloneString(&shd->resDecls[j].name, &decl->name.str);
                    ++shd->resDeclCnt;
                }
            }

            if (!err)
            {
                afxNat ioCnt = AfxCountArrayElements(&shdb.inOuts);
                shd->ioDeclCnt = 0;
                shd->ioDecls = NIL;

                if (ioCnt && !(shd->ioDecls = AfxAllocate(ioCnt, sizeof(shd->ioDecls[0]), 0, AfxHere()))) AfxThrowError();
                else
                {
                    for (afxNat i = 0; i < ioCnt; i++)
                    {
                        avxShaderIoChannel* ioBp = AfxGetArrayUnit(&shdb.inOuts, i);
                        shd->ioDecls[i] = *ioBp;
                        AfxCatalogStrings2(shd->strb, 1, &ioBp->semantic, &shd->ioDecls[i].semantic);
                        ++shd->ioDeclCnt;
                    }
                }

                shd->topology = shdb.topology;

                if (err && shd->ioDecls)
                    AfxDeallocate(shd->ioDecls);
            }

            if (err && shd->resDecls)
                AfxDeallocate(shd->resDecls);
        }

        AfxShaderBlueprintEnd(&shdb, 0, NIL);

        if (err && shd->code)
            AfxDeallocate(shd->code);

        if (err)
            AfxReleaseObjects(1, &shd->strb);
    }
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return err;
}

_AVX afxClassConfig const _AvxShdStdImplementation =
{
    .fcc = afxFcc_SHD,
    .name = "Shader",
    .desc = "Programmable Pipeline Module",
    .fixedSiz = sizeof(AFX_OBJECT(avxShader)),
    .ctor = (void*)_AvxShdStdCtor,
    .dtor = (void*)_AvxShdStdDtor
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

_AVX avxShader AfxCompileShaderFromXml(afxDrawContext dctx, afxNat specIdx, afxXml const* xml, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertType(xml, afxFcc_XML);
    avxShader shd = NIL;

    avxShaderBlueprint blueprint;
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
        AfxMakeUriFromString(&tmpUri, &tmp.str);
        AfxShaderBlueprintRename(&blueprint, &tmpUri);
#endif
        if (AfxCompileShaders(dctx, 1, &blueprint, &shd))
            AfxThrowError();
    }
    AfxShaderBlueprintEnd(&blueprint, 0, NIL);

    AfxTryAssertObjects(1, &shd, afxFcc_SHD);
    return shd;
}

_AVX avxShader AfxCompileShaderFromXsh(afxDrawContext dctx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(uri);
    AfxAssert(!AfxUriIsBlank(uri));
    avxShader shd = NIL;

    AfxLogEcho("Uploading shader '%.*s'", AfxPushString(&uri->str.str));

    afxUri fext;
    AfxClipUriExtension(&fext, uri, FALSE);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath;
        AfxClipUriPath(&fpath, uri);

        if (0 == AfxCompareStringCil(AfxGetUriString(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);

                afxString query;
                AfxGetUriQueryString(uri, TRUE, &query);

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

_AVX afxError AfxCompileShadersFromXsh(afxDrawContext dctx, afxNat cnt, afxUri const uri[], avxShader shaders[])
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
