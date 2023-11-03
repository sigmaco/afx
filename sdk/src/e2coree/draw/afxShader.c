/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_SHADER_C
#include "afxDrawClassified.h"
#include "afx/core/afxSystem.h"
#include "afx/core/afxUri.h"
#include "afx/draw/afxXsh.h"
#include "afx/draw/afxShaderBlueprint.h"

////////////////////////////////////////////////////////////////////////////////
// SHADER BLUEPRINT                                                           //
////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxShaderBlueprintRename(afxShaderBlueprint *blueprint, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    AfxTryAssertType(uri, afxFcc_URI);
    AfxCopyUri(&blueprint->uri.uri, uri);
}

_AFXINL void AfxShaderBlueprintChooseEntryPoint(afxShaderBlueprint *blueprint, afxString const *entry)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);
    AfxTryAssertType(entry, afxFcc_STR);
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

    afxContext mem = blueprint->ctx;
    AfxTryAssertObjects(1, &mem, afxFcc_CTX);

    afxNat idx;
    afxByte iniVal = 0;
    afxByte *bytecode;

    if (!(bytecode = AfxInsertArrayUnits(&blueprint->codes, range, &idx, &iniVal))) AfxThrowError();
    else
    {
        AfxCopy(bytecode, start, range);
    }
    return err;
}

_AFXINL afxError AfxShaderBlueprintAddCodeFromStream(afxShaderBlueprint *blueprint, afxStream *ios, afxSize offset, afxNat range)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);

    AfxAssert(ios);
    AfxAssert(offset);
    AfxAssert(range);

    AfxAssertType(ios, afxFcc_IOS);
    afxNat cachedPosn = AfxAskStreamPosn(ios);

    if (AfxGoToStreamBegin(ios, offset)) AfxThrowError();
    else
    {
        afxContext mem = blueprint->ctx;
        AfxAssertObjects(1, &mem, afxFcc_CTX);

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

    afxContext mem = blueprint->ctx;
    AfxTryAssertObjects(1, &mem, afxFcc_CTX);

    afxFile file;
    AfxAssertType(uri, afxFcc_URI);

    if (AfxOpenFiles(1, &file, uri, (afxFileFlags[]) {AFX_FILE_FLAG_RX})) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);
        afxStream *ios = AfxGetFileStream(file);
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
        decl->cnt = AfxMaxi(cnt, 1);
         AfxCloneString(&decl->name, name);
    }
    return err;
}

_AFXINL afxError AfxShaderBlueprintDeclareInOut(afxShaderBlueprint *blueprint, afxNat location, afxVertexFormat fmt, afxString const *semantic)
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
         AfxCloneString(&decl->semantic, semantic);
    }
    return err;
}

_AFXINL void AfxShaderBlueprintErase(afxShaderBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_SHDB);

    AfxEraseUri(&blueprint->uri.uri);
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
            if (AfxBuildShaders(blueprint->dctx, 1, &shd[i], blueprint))
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

    afxContext mem = blueprint->ctx;
    AfxTryAssertObjects(1, &mem, afxFcc_CTX);

    AfxReleaseArray(&blueprint->codes);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->resources); i++)
    {
        afxShaderBlueprintResource *decl = AfxGetArrayUnit(&blueprint->resources, i);

        //if (decl->name)
            AfxDeallocateString(&decl->name);
    }
    AfxReleaseArray(&blueprint->resources);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->inOuts); i++)
    {
        afxShaderBlueprintInOut *decl = AfxGetArrayUnit(&blueprint->inOuts, i);

        //if (decl->semantic)
            AfxDeallocateString(&decl->semantic);
    }
    AfxReleaseArray(&blueprint->inOuts);

    blueprint->fcc = NIL;
    return err;
}

_AFXINL void AfxShaderBlueprintBegin(afxShaderBlueprint* blueprint, afxShaderStage stage, afxUri const *uri, afxString const *entry, afxNat estCodeLen, afxNat estIoCnt, afxNat estResCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->fcc = afxFcc_SHDB;

    afxContext mem = NIL;
    //AfxAssertObjects(1, &mem, afxFcc_CTX);
    blueprint->ctx = mem;

    AfxUri128(&blueprint->uri);

    if (uri)
        AfxCopyUri(&blueprint->uri.uri, uri);

    blueprint->stage = stage;

    AfxString8(&blueprint->entry);

    if (entry)
        AfxCopyString(&blueprint->entry.str, entry);

    AfxAcquireArray(&blueprint->codes, sizeof(afxByte), AfxMaxi(estCodeLen, 2048), AfxSpawnHint());
    AfxAcquireArray(&blueprint->resources, sizeof(afxShaderBlueprintResource), AfxMaxi(estResCnt, 16), AfxSpawnHint()); // if data is reallocated, autoreference by strings will be broken YYYYYYYYYY
    AfxAcquireArray(&blueprint->inOuts, sizeof(afxShaderBlueprintInOut), AfxMaxi(estIoCnt, 16), AfxSpawnHint());

    blueprint->topology = NIL;
}

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxPrintShader(afxShader shd, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!(AfxUriIsBlank(uri)));

    afxFile file;

    if (AfxOpenFiles(1, &file, uri, (afxFileFlags[]) { AFX_FILE_FLAG_W })) AfxThrowError();
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

_AFX afxError AfxSerializeShader(afxShader shd, afxStream *ios)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(ios, afxFcc_IOS);
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
        rsrc[i].name = shd->resDecls[first + i].name;
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

_AFX afxError AfxBuildShaders(afxDrawContext dctx, afxNat cnt, afxShader shd[], afxShaderBlueprint const blueprint[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxEntry("cnt=%u,shd=%p,blueprint=%p", cnt, shd, blueprint);
    AfxAssert(cnt);
    AfxAssert(blueprint);
    AfxAssert(shd);

    if (AfxAcquireObjects(&dctx->shaders, cnt, (afxHandle*)shd, (void*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
}

_AFX afxShader AfxCompileShaderFromXml(afxDrawContext dctx, afxXmlNode const* element)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(element);
    afxShader shd = NIL;

    if (AfxCompareString(AfxGetXmlNodeName(element), &g_str_Shader)) AfxThrowError();
    else
    {
        afxShaderBlueprint blueprint;
        AfxShaderBlueprintBegin(&blueprint, NIL, NIL, NIL, 0, 0, 0);

        if (AfxParseXmlBackedShaderBlueprint(element, &blueprint)) AfxThrowError();
        else
        {
#if 0
            afxString128 tmp;
            AfxString128(&tmp);
            AfxCopyString(&tmp.str, AfxUriGetStringConst(&fpath));

            if (!AfxUriIsBlank(&blueprint.uri.uri))
            {
                AfxAppendStringLiteral(&tmp.str, "?", 1);
                AfxAppendString(&tmp.str, AfxUriGetStringConst(&blueprint.uri.uri));
            }

            afxUri tmpUri;
            AfxReflectUriString(&tmpUri, &tmp.str);
            AfxShaderBlueprintRename(&blueprint, &tmpUri);
#endif
            if (AfxBuildShaders(dctx, 1, &shd, &blueprint))
                AfxThrowError();
        }
        AfxShaderBlueprintEnd(&blueprint, 0, NIL);
    }

    AfxTryAssertObjects(1, &shd, afxFcc_SHD);
    return shd;
}

_AFX afxShader AfxCompileShaderFromXsh(afxDrawContext dctx, afxUri const* uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(uri));
    afxShader shd = NIL;

    AfxEcho("Uploading shader '%.*s'", AfxPushString(&uri->str));

    afxUri fext;
    AfxExcerptUriExtension(&fext, uri, FALSE);

    if (AfxUriIsBlank(&fext)) AfxThrowError();
    else
    {
        afxUri fpath, query;
        AfxExcerptUriPath(&fpath, uri);
        AfxExcerptUriQuery(&query, uri, TRUE);

        if (0 == AfxCompareStringLiteralCi(AfxUriGetStringConst(&fext), 0, ".xml", 4))
        {
            afxXml xml;

            if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
            else
            {
                AfxAssertType(&xml, afxFcc_XML);

                afxXmlNode const *node = AfxGetXmlRoot(&xml);
                afxString const *name = AfxGetXmlNodeName(node);
                AfxAssert(0 == AfxCompareString(name, &g_str_Qwadro));
                afxString const *queryStr = AfxUriGetStringConst(&query);
                afxBool hasQuery = !AfxStringIsEmpty(queryStr);
                node = AfxXmlNodeFindChild(node, &g_str_Shader, hasQuery ? &g_str_name : NIL, hasQuery ? queryStr : NIL);

                if (!node) AfxThrowError();
                else
                {
                    if (!(shd = AfxCompileShaderFromXml(dctx, node))) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &shd, afxFcc_SHD);
                    }
                }
                AfxReleaseXml(&xml);
            }
        }
    }
    return shd;
}

#if 0
_AFX afxResult AfxFindShaders(afxDrawContext dctx, afxNat cnt, afxShader shd[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssertType(uri, afxFcc_URI);
    AfxAssert(shd);
    afxResult rslt = 0;

    afxNat i = 0;
    afxShader tmp;
    while (AfxEnumerateShaders(dctx, i, 1, &tmp))
    {
        AfxAssertObjects(1, &tmp, afxFcc_SHD);

        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxUriIsEquivalent(AfxShaderGetUri(tmp), &uri[i]))
            {
                shd[i] = tmp;
                rslt++;
                break;
            }
        }

        if (rslt == (afxResult)cnt)
            break;

        ++i;
    }
    return rslt;
}

_AFX afxError AfxUploadShaders(afxDrawContext dctx, afxNat cnt, afxShader shd[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(shd);
    afxResult rslt = 0;

    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    afxShaderBlueprint blueprint;
    AfxShaderBlueprintBegin(&blueprint, NIL, NIL, NIL, 0, 0, 0);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssertType(&uri[i], afxFcc_URI);
        AfxAssert(!AfxUriIsBlank(&uri[i]));

        AfxEcho("Uploading shader '%.*s'", AfxPushString(&uri[i].str));

        afxUri fext;
        AfxExcerptUriExtension(&fext, &uri[i], FALSE);

        if (AfxUriIsBlank(&fext)) AfxThrowError();
        else
        {
            afxUri fpath, query;
            AfxExcerptUriPath(&fpath, &uri[i]);
            AfxExcerptUriQuery(&query, &uri[i], TRUE);

            if (0 == AfxCompareStringLiteralCi(AfxUriGetStringConst(&fext), 0, ".xml", 4))
            {
                afxXml xml;

                if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
                else
                {
                    AfxAssertType(&xml, afxFcc_XML);

                    afxXmlNode const *node = AfxGetXmlRoot(&xml);
                    afxString const *name = AfxGetXmlNodeName(node);
                    AfxAssert(0 == AfxCompareString(name, &g_str_Qwadro));
                    afxString const *queryStr = AfxUriGetStringConst(&query);
                    afxBool hasQuery = !AfxStringIsEmpty(queryStr);
                    node = AfxXmlNodeFindChild(node, &g_str_Shader, hasQuery ? &g_str_name : NIL, hasQuery ? queryStr : NIL);

                    if (node)
                    {
                        if (AfxParseXmlBackedShaderBlueprint(node, &blueprint)) AfxThrowError();
                        else
                        {
                            afxString128 tmp;
                            AfxString128(&tmp);
                            AfxCopyString(&tmp.str, AfxUriGetStringConst(&fpath));

                            if (!AfxUriIsBlank(&blueprint.uri.uri))
                            {
                                AfxAppendStringLiteral(&tmp.str, "?", 1);
                                AfxAppendString(&tmp.str, AfxUriGetStringConst(&blueprint.uri.uri));
                            }

                            afxUri tmpUri;
                            AfxReflectUriString(&tmpUri, &tmp.str);
                            AfxShaderBlueprintRename(&blueprint, &tmpUri);

                            if (AfxBuildShaders(dctx, 1, &shd[i], &blueprint)) AfxThrowError();
                            else
                            {
                                AfxAssertObjects(1, &shd[i], afxFcc_SHD);
                                ++rslt;
                            }
                        }

                    }
                    AfxReleaseXml(&xml);
                }
            }
            else
            {
                AfxError("Extension (%.*s) not supported.", AfxPushString(AfxUriGetStringConst(&fext)));
                AfxThrowError();
            }
        }

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
            {
                AfxReleaseObjects(1, (void*[]) { shd[j] });
                shd[j] = NIL;
            }
            break;
        }
    }

    AfxShaderBlueprintEnd(&blueprint, 0, NIL);

    return err;
}

_AFX afxError AfxAcquireShaders(afxDrawContext dctx, afxNat cnt, afxShader shd[], afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cnt=%u,uri=%p,shd=%p", cnt, uri, shd);
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(shd);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxUri name;
        AfxAssertType(&uri[i], afxFcc_URI);
        AfxAssert(!(AfxUriIsBlank(&uri[i])));
        AfxExcerptUriName(&name, &uri[i]);
        AfxAssert(!(AfxUriIsBlank(&name)));

        if (1 == AfxFindShaders(dctx, 1, &shd[i], &name))
        {
            AfxAssertObjects(1, &shd[i], afxFcc_SHD);
            AfxReacquireObjects(1, (void*[]) { shd[i] });
            rslt++;
        }
        else
        {
            if (AfxUploadShaders(dctx, 1, &shd[i], &uri[i])) AfxThrowError();
            else
            {
                AfxAssertObjects(1, &shd[i], afxFcc_SHD);
                ++rslt;
            }
        }

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
            {
                AfxReleaseObjects(1, (void*[]) { shd[j] });
                shd[j] = NIL;
            }
            break;
        }
    }
    return err;
}
#endif
