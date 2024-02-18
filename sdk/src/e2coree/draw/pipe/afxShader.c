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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_DRAW_C
#define _AFX_SHADER_C
#include "qwadro/core/afxClass.h"
#include "qwadro/io/afxFile.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/io/afxXsh.h"
#include "qwadro/draw/pipe/afxShaderBlueprint.h"

////////////////////////////////////////////////////////////////////////////////
// SHADER BLUEPRINT                                                           //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxShaderBlueprintRename(afxShaderBlueprint *blueprint, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    AfxCopyUri(&blueprint->uri.uri, uri);
}

_AFXINL void AfxShaderBlueprintChooseEntryPoint(afxShaderBlueprint *blueprint, afxString const *entry)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    AfxCopyString(&blueprint->entry.str, entry);
}

_AFXINL void AfxShaderBlueprintSetStage(afxShaderBlueprint *blueprint, afxShaderStage stage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    //AfxAssert(stage);
    blueprint->stage = stage;
}

_AFXINL afxError AfxShaderBlueprintAddCode(afxShaderBlueprint *blueprint, void const *start, afxNat range)
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
        AfxCopy(1, range, start, bytecode);
    }
    return err;
}

_AFXINL afxError AfxShaderBlueprintAddCodeFromStream(afxShaderBlueprint *blueprint, afxStream ios, afxSize offset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);

    AfxAssert(ios);
    AfxAssert(offset);
    AfxAssert(range);

    AfxAssertObjects(1, &ios, afxFcc_IOS);
    afxNat cachedPosn = AfxAskStreamPosn(ios);

    if (AfxGoToStreamBegin(ios, offset)) AfxThrowError();
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
            if (AfxReadStream(ios, bytecode, range, 0))
                AfxThrowError();
        }
    }
    AfxGoToStreamBegin(ios, (afxInt)cachedPosn);
    return err;
}

_AFXINL afxError AfxShaderBlueprintAddCodeFromResource(afxShaderBlueprint *blueprint, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);

    afxMmu mmu = blueprint->mmu;
    AfxTryAssertObjects(1, &mmu, afxFcc_MMU);

    afxFile file;
    AfxAssert(uri);

    if (AfxOpenFiles(afxFileFlag_RX, 1, uri, &file)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);
        afxStream ios = AfxGetFileStream(file);
        afxNat size = AfxMeasureStream(ios);
        AfxAssert(size);

        afxNat idx;
        afxByte iniVal = 0;
        afxByte *bytecode;

        if (!(bytecode = AfxInsertArrayUnits(&blueprint->codes, size, &idx, &iniVal))) AfxThrowError();
        else
        {
            if (AfxReadStream(ios, bytecode, size, 0))
                AfxThrowError();
        }
        AfxReleaseObjects(1, (void*[]) { file });
    }
    return err;
}

_AFXINL afxError AfxShaderBlueprintDeclareResource(afxShaderBlueprint *blueprint, afxNat set, afxNat binding, afxShaderResourceType type, afxNat cnt, afxString const *name)
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

_AFXINL afxError AfxShaderBlueprintDeclareInOut(afxShaderBlueprint *blueprint, afxNat location, afxVertexFormat fmt, afxNat stream, afxString const *semantic)
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

_AFXINL void AfxShaderBlueprintErase(afxShaderBlueprint *blueprint)
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

_AFXINL afxError AfxShaderBlueprintEnd(afxShaderBlueprint *blueprint, afxNat cnt, afxShader shd[])
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
                    AfxReleaseObjects(1, (void*[]) { shd[j] });
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

_AFXINL void AfxShaderBlueprintBegin(afxShaderBlueprint* blueprint, afxShaderStage stage, afxUri const *uri, afxString const *entry, afxNat estCodeLen, afxNat estIoCnt, afxNat estResCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->fcc = afxFcc_SHDB;

    afxMmu mmu = NIL;
    //AfxAssertObjects(1, &mmu, afxFcc_MMU);
    blueprint->mmu = mmu;

    AfxMakeFixedUri128(&blueprint->uri, NIL);

    if (uri)
        AfxCopyUri(&blueprint->uri.uri, uri);

    blueprint->stage = stage;

    AfxMakeFixedString8(&blueprint->entry, entry);

    AfxAllocateArray(&blueprint->codes, AfxMax(estCodeLen, 2048), sizeof(afxByte), NIL);
    AfxAllocateArray(&blueprint->resources, AfxMax(estResCnt, 16), sizeof(afxShaderBlueprintResource), NIL); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY
    AfxAllocateArray(&blueprint->inOuts, AfxMax(estIoCnt, 16), sizeof(afxShaderBlueprintInOut), NIL);

    blueprint->topology = NIL;
}

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxPrintShader(afxShader shd, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AfxAssert(uri);
    AfxAssert(!(AfxUriIsBlank(uri)));

    afxFile file;

    if (AfxOpenFiles(afxFileFlag_W, 1, uri, &file)) AfxThrowError();
    else
    {
        if (AfxSerializeShader(shd, AfxGetFileStream(file))) AfxThrowError();
        else
        {
            // success
        }
        AfxReleaseObjects(1, (void*[]) { file });
    }
    return err;
}

_AFX afxError AfxSerializeShader(afxShader shd, afxStream ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ios, afxFcc_IOS);
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    AfxWriteStream(ios, shd->code, shd->codeLen, 0);

    return err;
}

_AFX afxNat AfxCountShaderInterfaces(afxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    return shd->resDeclCnt;
}

_AFX afxResult AfxDescribeShaderInterfaces(afxShader shd, afxNat first, afxNat cnt, afxShaderResource rsrc[])
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

_AFX afxShaderStage AfxGetShaderStage(afxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return shd->stage;
}

_AFX afxUri const* AfxShaderGetUri(afxShader shd)
{
    afxError err = AFX_ERR_NONE;

    AfxAssertObjects(1, &shd, afxFcc_SHD);

    return &shd->uri;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxNat AfxEnumerateShaders(afxDrawContext dctx, afxNat first, afxNat cnt, afxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(shaders);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxClass* cls = AfxGetSamplerClass(dctx);
    AfxAssertClass(cls, afxFcc_SHD);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)shaders);
}

_AFX afxError AfxCompileShaders(afxDrawContext dctx, afxNat cnt, afxShaderBlueprint const blueprint[], afxShader shaders[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(blueprint);
    AfxAssert(shaders);
    AfxAssert(cnt);

    afxClass* cls = AfxGetShaderClass(dctx);
    AfxAssertClass(cls, afxFcc_SHD);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)shaders, (void const*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
}

_AFX afxShader AfxCompileShaderFromXml(afxDrawContext dctx, afxNat specIdx, afxXml const* xml, afxNat elemIdx)
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
        afxFixedString128 tmp;
        AfxMakeFixedString128(&tmp);
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

_AFX afxShader AfxCompileShaderFromXsh(afxDrawContext dctx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(uri);
    AfxAssert(!AfxUriIsBlank(uri));
    afxShader shd = NIL;

    AfxEcho("Uploading shader '%.*s'", AfxPushString(&uri->str.str));

    afxUri fext;
    AfxExcerptUriExtension(uri, FALSE, &fext);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath;
        AfxExcerptUriPath(uri, &fpath);

        if (0 == AfxCompareStringCil(AfxGetUriString(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);

                afxString query;
                AfxExcerptUriQueryToString(uri, TRUE, &query);

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

_AFX afxError AfxCompileShadersFromXsh(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxShader shaders[])
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
