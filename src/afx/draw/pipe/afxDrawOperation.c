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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#include "qwadro/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// DRAW OPERATION BLUEPRINT                                                   //
////////////////////////////////////////////////////////////////////////////////

_AVXINL void AfxDrawOperationBlueprintErase(afxDrawOperationBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);

    AfxClearUri(&blueprint->uri.uri);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->techniques); i++)
    {
        afxDrawOperationBlueprintTechnique *tec = AfxGetArrayUnit(&blueprint->techniques, i);
        
        if (tec->name)
        {
            AfxDeallocateString(tec->name);
            tec->name = NIL;
        }

        for (afxNat j = 0; j < AfxCountArrayElements(&tec->passes); j++)
        {
            afxDrawOperationBlueprintTechniquePass *pass = AfxGetArrayUnit(&tec->passes, j);

            if (pass->name)
            {
                AfxDeallocateString(pass->name);
                pass->name = NIL;
            }

            for (afxNat K = 0; K < AfxCountArrayElements(&pass->shaders); K++)
            {
                afxUri *reqShdUri = *(afxUri **)AfxGetArrayUnit(&pass->shaders, K);

                if (reqShdUri)
                    AfxDeallocateUri(reqShdUri);
            }
            AfxDeallocateArray(&pass->shaders);
        }
        AfxDeallocateArray(&tec->passes);
    }
    AfxEmptyArray(&blueprint->techniques);
}

_AVXINL afxError AfxDrawOperationBlueprintEnd(afxDrawOperationBlueprint *blueprint, afxNat cnt, afxDrawOperation dop[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);

    if (cnt)
    {
        AfxAssert(dop);

        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxBuildDrawOperations(blueprint->dctx, 1, blueprint, &dop[i]))
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; j++)
                {
                    AfxReleaseObject(&dop[j]->obj);
                    dop[j] = NIL;
                }
                break;
            }
        }
    }

    afxMmu mmu = AfxGetDrawContextMmu(blueprint->dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    for (afxNat i = 0; i < AfxCountArrayElements(&blueprint->techniques); i++)
    {
        afxDrawOperationBlueprintTechnique *tec = AfxGetArrayUnit(&blueprint->techniques, i);

        if (tec->name)
        {
            AfxDeallocateString(tec->name);
            tec->name = NIL;
        }

        for (afxNat j = 0; j < AfxCountArrayElements(&tec->passes); j++)
        {
            afxDrawOperationBlueprintTechniquePass *pass = AfxGetArrayUnit(&tec->passes, j);

            if (pass->name)
            {
                AfxDeallocateString(pass->name);
                pass->name = NIL;
            }

            for (afxNat k = 0; k < AfxCountArrayElements(&pass->shaders); k++)
            {
                afxUri *reqShdUri = *(afxUri **)AfxGetArrayUnit(&pass->shaders, k);

                if (reqShdUri)
                    AfxDeallocateUri(reqShdUri);
            }
            AfxDeallocateArray(&pass->shaders);
        }
        AfxDeallocateArray(&tec->passes);
    }
    AfxDeallocateArray(&blueprint->techniques);
    blueprint->fcc = NIL;
    return err;
}

_AVX void AfxDrawOperationBlueprintBegin(afxDrawOperationBlueprint* blueprint, afxDrawContext dctx, afxUri const *uri, afxNat estShaderCnt, afxNat estTechCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->dctx = dctx;
    blueprint->fcc = afxFcc_DOPB;
    AfxMakeUri128(&blueprint->uri);
    
    if (uri)
        AfxCopyUri(&blueprint->uri.uri, uri);

    AfxAllocateArray(&blueprint->techniques, sizeof(afxDrawOperationBlueprintTechnique), estTechCnt, AfxHere());
}

_AVXINL void AfxDrawOperationBlueprintRename(afxDrawOperationBlueprint *blueprint, afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);
    AfxCopyUri(&blueprint->uri.uri, uri);
}

_AVXINL afxError AfxDrawOperationBlueprintAddShaders(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxNat cnt, afxUri const uri[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);
    AfxAssert(AfxCountArrayElements(&blueprint->techniques) > tecIdx);
    afxDrawOperationBlueprintTechnique *tec = AfxGetArrayUnit(&blueprint->techniques, tecIdx);
    AfxAssert(passIdx < AfxCountArrayElements(&tec->passes));
    afxDrawOperationBlueprintTechniquePass *pass = AfxGetArrayUnit(&tec->passes, passIdx);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat foundIdx = AFX_INVALID_INDEX;

        for (afxNat j = 0; j < AfxCountArrayElements(&pass->shaders); j++)
        {
            if (AfxUriIsEquivalent(&uri[i], *(afxUri**)AfxGetArrayUnit(&pass->shaders, j)))
            {
                foundIdx = j;
                break;
            }
        }

        if (foundIdx == AFX_INVALID_INDEX)
        {
            afxUri **reqShdUri;

            if (!(reqShdUri = AfxInsertArrayUnit(&pass->shaders, &foundIdx))) AfxThrowError();
            else
            {
                *reqShdUri = AfxDuplicateUri(&uri[i]);
            }
        }
    }
    return err;
}

_AVXINL afxError AfxDrawOperationBlueprintAddTechnique(afxDrawOperationBlueprint *blueprint, afxNat *tecIdx, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);
    AfxAssert(tecIdx);
    afxDrawOperationBlueprintTechnique *tec;

    if (!(tec = AfxInsertArrayUnit(&blueprint->techniques, tecIdx))) AfxThrowError();
    else
    {
        tec->name = name && !AfxStringIsEmpty(name) ? AfxCloneString(name) : NIL;
        AfxAllocateArray(&tec->passes, sizeof(afxDrawOperationBlueprintTechniquePass), 1, AfxHere()); // at least one passes
    }
    return err;
}

_AVXINL void AfxDrawOperationBlueprintRenameTechnique(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);
    AfxAssert(AfxCountArrayElements(&blueprint->techniques) > tecIdx);
    afxDrawOperationBlueprintTechnique *tec = AfxGetArrayUnit(&blueprint->techniques, tecIdx);

    if (tec->name)
        AfxDeallocateString(tec->name);

    tec->name = name && !AfxStringIsEmpty(name) ? AfxCloneString(name) : NIL;
}

_AVXINL afxError AfxDrawOperationBlueprintAddPass(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat *passIdx, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);
    AfxAssert(passIdx);
    AfxAssert(AfxCountArrayElements(&blueprint->techniques) > tecIdx);

    afxDrawOperationBlueprintTechnique *tec = AfxGetArrayUnit(&blueprint->techniques, tecIdx);
    afxDrawOperationBlueprintTechniquePass *pass;

    if (!(pass = AfxInsertArrayUnit(&tec->passes, passIdx))) AfxThrowError();
    else
    {
        pass->name = name && !AfxStringIsEmpty(name) ? AfxCloneString(name) : NIL;

        pass->hasDs = FALSE;
        pass->hasIa = FALSE;
        pass->hasRs = FALSE;
        pass->inputAssembly = (afxPipelinePrimitiveState) { 0 };
        pass->rasterization = (afxPipelineRasterizerState) { 0 };
        pass->depthHandling = (afxPipelineDepthState) { 0 };
        
        AfxAllocateArray(&pass->shaders, sizeof(afxUri*), 2, AfxHere()); // at least vertex and fragment shaders
    }
    return err;
}

_AVXINL void AfxDrawOperationBlueprintRenamePass(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);
    AfxAssert(tecIdx < AfxCountArrayElements(&blueprint->techniques));
    afxDrawOperationBlueprintTechnique *tec = AfxGetArrayUnit(&blueprint->techniques, tecIdx);
    AfxAssert(passIdx < AfxCountArrayElements(&tec->passes));
    afxDrawOperationBlueprintTechniquePass *pass = AfxGetArrayUnit(&tec->passes, passIdx);
    
    if (pass->name)
        AfxDeallocateString(pass->name);

    pass->name = name && !AfxStringIsEmpty(name) ? AfxCloneString(name) : NIL;
}

_AVXINL void AfxDrawOperationBlueprintConfigRasterizerState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineRasterizerState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);
    AfxAssert(tecIdx < AfxCountArrayElements(&blueprint->techniques));
    afxDrawOperationBlueprintTechnique *tec = AfxGetArrayUnit(&blueprint->techniques, tecIdx);
    AfxAssert(passIdx < AfxCountArrayElements(&tec->passes));
    afxDrawOperationBlueprintTechniquePass *pass = AfxGetArrayUnit(&tec->passes, passIdx);
    pass->rasterization = state ? *state : (afxPipelineRasterizerState) { 0 };
    pass->hasRs = !!state;
}

_AVXINL void AfxDrawOperationBlueprintConfigDepthState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelineDepthState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);
    AfxAssert(tecIdx < AfxCountArrayElements(&blueprint->techniques));
    afxDrawOperationBlueprintTechnique *tec = AfxGetArrayUnit(&blueprint->techniques, tecIdx);
    AfxAssert(passIdx < AfxCountArrayElements(&tec->passes));
    afxDrawOperationBlueprintTechniquePass *pass = AfxGetArrayUnit(&tec->passes, passIdx);
    pass->depthHandling = state ? *state : (afxPipelineDepthState) { 0 };
    pass->hasDs = !!state;
}

_AVXINL void AfxDrawOperationBlueprintConfigInputAssemblyState(afxDrawOperationBlueprint *blueprint, afxNat tecIdx, afxNat passIdx, afxPipelinePrimitiveState const *state)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, afxFcc_DOPB);
    AfxAssert(tecIdx < AfxCountArrayElements(&blueprint->techniques));
    afxDrawOperationBlueprintTechnique *tec = AfxGetArrayUnit(&blueprint->techniques, tecIdx);
    AfxAssert(passIdx < AfxCountArrayElements(&tec->passes));
    afxDrawOperationBlueprintTechniquePass *pass = AfxGetArrayUnit(&tec->passes, passIdx);
    pass->inputAssembly = state ? *state : (afxPipelinePrimitiveState) { 0 };
    pass->hasIa = !!state;
}

////////////////////////////////////////////////////////////////////////////////
// DRAW OPERATION                                                             //
////////////////////////////////////////////////////////////////////////////////

_AVX afxPipeline AfxDrawOperationGetPipeline(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);
    AfxAssert(dop->techCnt > tecIdx);
    AfxAssert(dop->techniques[tecIdx].passCnt > passIdx);
    afxPipeline pip = dop->techniques[tecIdx].passes[passIdx].pip;
    AfxAssertObjects(1, &pip, afxFcc_PIP);
    return pip;
}

_AVX afxString const* AfxDrawOperationGetPassName(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);
    AfxAssert(dop->techCnt > tecIdx);
    AfxAssert(dop->techniques[tecIdx].passCnt > passIdx);
    return dop->techniques[tecIdx].passes[passIdx].name;
}

_AVX afxNat AfxDrawOperationGetPassCount(afxDrawOperation dop, afxNat tecIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);
    AfxAssert(dop->techCnt > tecIdx);
    return dop->techniques[tecIdx].passCnt;
}

_AVX afxBool AfxDrawOperationFindPass(afxDrawOperation dop, afxNat tecIdx, afxString const *name, afxNat *idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);
    AfxAssert(dop->techCnt > tecIdx);

    for (afxNat i = 0; i < dop->techCnt; i++)
    {
        afxDrawTechnique *dtec = &dop->techniques[i];

        for (afxNat j = 0; j < dtec->passCnt; j++)
        {
            afxDrawPass *dpas = &dtec->passes[j];

            if (0 == AfxCompareStringCi(name, dpas->name))
            {
                AfxAssert(idx);
                *idx = j;
                return TRUE;
            }
        }
    }
    return FALSE;
}

_AVX afxString const* AfxDrawOperationGetTechniqueName(afxDrawOperation dop, afxNat tecIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);
    AfxAssert(dop->techCnt > tecIdx);
    return dop->techniques[tecIdx].name;
}

_AVX afxNat AfxDrawOperationGetTechniqueCount(afxDrawOperation dop)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);
    return dop->techCnt;
}

_AVX afxBool AfxDrawOperationFindTechnique(afxDrawOperation dop, afxString const *name, afxNat *idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);

    for (afxNat i = 0; i < dop->techCnt; i++)
    {
        afxDrawTechnique *dtec = &dop->techniques[i];

        if (0 == AfxCompareStringCi(name, dtec->name))
        {
            AfxAssert(idx);
            *idx = i;
            return TRUE;
        }
    }

    return FALSE;
}

_AVX void* AfxDrawOperationGetContext(afxDrawOperation dop)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);
    afxDrawContext dctx = AfxGetObjectProvider(&dop->obj);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, afxFcc_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, afxFcc_DCTX);
    return dctx;
}

_AVX afxError AfxBuildDrawOperations(afxDrawContext dctx, afxNat cnt, afxDrawOperationBlueprint const blueprint[], afxDrawOperation dop[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, afxFcc_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(blueprint);
    AfxAssert(dop);
    
    if (AfxClassAcquireObjects(AfxGetDrawOperationClass(dctx), NIL, cnt, blueprint, (afxHandle**)dop, AfxHere()))
        AfxThrowError();

    return err;
}

_AVX afxResult AfxFindDrawOperations(afxDrawContext dctx, afxNat cnt, afxUri const name[], afxDrawOperation dop[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, afxFcc_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(name);
    AfxAssert(dop);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(!AfxUriIsBlank(name));
    }

    rslt += 0;// = AfxDrawContextGetVmt(dctx)->other.findTex(dctx, cnt, name, tex);

    for (afxNat i = 0; i < cnt; i++)
    {
        dop[i] = NIL;
        AfxTryAssertObject(dop[i], afxFcc_DOP);
    }
    return rslt;
}

_AVX afxError AfxUploadDrawOperations(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxDrawOperation dop[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, afxFcc_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(dop);
    afxResult rslt = 0;

    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(!AfxUriIsBlank(&uri[i]));
        
        AfxLogEcho("Uploading draw operation '%.*s'", AfxPushString(&uri[i].str));
        
        afxUri fext;
        AfxPickUriExtension(&fext, &uri[i], FALSE);
        
        if (AfxUriIsBlank(&fext)) AfxThrowError();
        else
        {
            afxUri fpath, query;
            AfxPickUriPath(&fpath, &uri[i]);
            AfxPickUriQuery(&query, &uri[i], TRUE);

            if (0 == AfxCompareStringCil(AfxGetUriString(&fext), 0, ".xml", 4))
            {
                afxXml xml;

                if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
                else
                {
                    AfxAssertType(&xml, afxFcc_XML);

                    afxXmlNode const *node = AfxGetXmlRoot(&xml);
                    afxString const *name = AfxGetXmlNodeName(node);
                    AfxAssert(0 == AfxCompareString(name, &g_str_Qwadro));
                    afxString const *queryStr = AfxGetUriString(&query);
                    afxBool hasQuery = !AfxStringIsEmpty(queryStr);
                    node = AfxXmlNodeFindChild(node, &g_str_DrawOperation, hasQuery ? &g_str_name : NIL, hasQuery ? queryStr : NIL);

                    if (node)
                    {
                        afxDrawOperationBlueprint blueprint;
                        AfxDrawOperationBlueprintBegin(&blueprint, dctx, NIL, 0, 1);

                        if (AfxParseXmlBackedDrawOperationBlueprint(node, &blueprint)) AfxThrowError();
                        else
                        {
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
                            AfxDrawOperationBlueprintRename(&blueprint, &tmpUri);

                            if (AfxBuildDrawOperations(dctx, 1, &blueprint, &dop[i])) AfxThrowError();
                            else
                            {
                                AfxAssertObject(dop[i], afxFcc_DOP);
                                ++rslt;
                            }
                        }
                        AfxDrawOperationBlueprintEnd(&blueprint, 0, NIL);
                    }

                    AfxCleanUpXml(&xml);
                }
            }
            else
            {
                AfxLogError("Extension (%.*s) not supported.", AfxPushString(AfxGetUriString(&fext)));
                AfxThrowError();
            }
        }

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
            {
                AfxReleaseObject(&dop[j]->obj);
                dop[j] = NIL;
            }
            break;
        }
    }
    return err;
}

_AVX afxError AfxAcquireDrawOperations(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxDrawOperation dop[])
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, afxFcc_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, afxFcc_DCTX);
    AfxAssert(cnt);
    AfxAssert(uri);
    AfxAssert(dop);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxUri name;
        AfxAssert(!(AfxUriIsBlank(&uri[i])));
        AfxPickUriObject(&name, &uri[i]);
        AfxAssert(!(AfxUriIsBlank(&name)));

        if (1 == AfxFindDrawOperations(dctx, 1, &name, &dop[i]))
        {
            AfxAssertObject(dop[i], afxFcc_DOP);
            AfxObjectReacquire(&(dop[i]->obj), NIL, NIL, NIL, NIL);
            rslt++;
        }
        else
        {
            if (AfxUploadDrawOperations(dctx, 1, &uri[i], &dop[i])) AfxThrowError();
            else
            {
                AfxAssertObject(dop[i], afxFcc_DOP);
                ++rslt;
            }
        }

        if (err)
        {
            for (afxNat j = 0; j < i; j++)
            {
                AfxReleaseObject(&dop[j]->obj);
                dop[j] = NIL;
            }
            break;
        }
    }
    return err;
}


_AVX afxBool _AfxDopEventHandler(afxHandle *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOperation dop = (void*)obj;
    AfxAssertObject(dop, afxFcc_DOP);
    (void)ev;
    return FALSE;
}

_AVX afxBool _AfxDopEventFilter(afxHandle *obj, afxHandle *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOperation dop = (void*)obj;
    AfxAssertObject(dop, afxFcc_DOP);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AVX afxError _AfxDopDtor(afxDrawOperation dop)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);
    AfxEntry("dop=%p", dop);

    afxDrawContext dctx = AfxGetObjectProvider(&dop->obj);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, afxFcc_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, afxFcc_DCTX);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    for (afxNat i = 0; i < dop->techCnt; i++)
    {
        for (afxNat j = 0; j < dop->techniques[i].passCnt; j++)
        {
            AfxReleaseObject(&(dop->techniques[i].passes[j].pip->obj));

            if (dop->techniques[i].passes[j].name)
                AfxDeallocateString(dop->techniques[i].passes[j].name);
        }

        AfxAssert(dop->techniques[i].passes);
        AfxDeallocate(mmu, dop->techniques[i].passes);

        if (dop->techniques[i].name)
            AfxDeallocateString(dop->techniques[i].name);
    }

    AfxAssert(dop->techniques);
    AfxDeallocate(mmu, dop->techniques);

    if (dop->uri)
        AfxDeallocateUri(dop->uri);

    return err;
}

_AVX afxError _AfxDopCtor(void *cache, afxNat idx, afxDrawOperation dop, afxDrawOperationBlueprint const *blueprints)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(dop, afxFcc_DOP);
    (void)cache;
    AfxEntry("dop=%p", dop);
    //AfxAssert(args);
    //afxDrawOperationBlueprintTechniquePass const *spec = args->spec;
    afxDrawOperationBlueprint const *blueprint = &blueprints[idx];
    AfxAssert(blueprint);

    afxDrawContext dctx = AfxGetObjectProvider(&dop->obj);
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, afxFcc_DSYS);
    struct _afxDctxD *dctxD;
    _AfxGetDctxD(dctx, &dctxD, dsysD);
    AfxAssertType(dctxD, afxFcc_DCTX);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    dop->uri = AfxUriIsBlank(&blueprint->uri.uri) ? NIL : AfxDuplicateUri(&blueprint->uri.uri);
    
    dop->techCnt = 0;

    afxArray const *techniques = &blueprint->techniques;
    afxNat techCnt = AfxCountArrayElements(techniques);
    AfxAssert(techCnt);

    if (!(dop->techniques = AfxAllocate(mmu, techCnt, sizeof(dop->techniques[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < techCnt; i++)
        {
            afxDrawOperationBlueprintTechnique const *dopbt = AfxGetArrayUnit(techniques, i);

            dop->techniques[dop->techCnt].fcc = afxFcc_DTEC;
            dop->techniques[dop->techCnt].name = dopbt->name && !AfxStringIsEmpty(dopbt->name) ? AfxCloneString(dopbt->name) : NIL;
            dop->techniques[dop->techCnt].passCnt = 0;

            afxArray const *passes = &dopbt->passes;
            afxNat passCnt = AfxCountArrayElements(passes);
            AfxAssert(passCnt);

            if (!(dop->techniques[dop->techCnt].passes = AfxAllocate(mmu, sizeof(dop->techniques[dop->techCnt].passes[0]) * passCnt, 0, AfxHere()))) AfxThrowError();
            else
            {
                for (afxNat j = 0; j < passCnt; j++)
                {
                    afxDrawOperationBlueprintTechniquePass const *dopbtp = AfxGetArrayUnit(passes, j);

                    dop->techniques[dop->techCnt].passes[dop->techniques[dop->techCnt].passCnt].fcc = afxFcc_DPAS;
                    dop->techniques[dop->techCnt].passes[dop->techniques[dop->techCnt].passCnt].name = dopbtp->name && !AfxStringIsEmpty(dopbtp->name) ? AfxCloneString(dopbtp->name) : NIL;

                    afxPipelineBlueprint pipb;
                    AfxFormulatePipelineBlueprint(&pipb, dctx);

                    afxArray const *shaders = &dopbtp->shaders;

                    for (afxNat k = 0; k < AfxCountArrayElements(shaders); k++)
                    {
                        afxUri *reqShdUri = *(afxUri**)AfxGetArrayUnit(shaders, k);
                        AfxPipelineBlueprintAddShaders(&pipb, 1, reqShdUri);
                    }

                    if (dopbtp->hasRs)
                        AfxPipelineBlueprintConfigRasterizerState(&pipb, &(dopbtp->rasterization));

                    if (dopbtp->hasDs)
                        AfxPipelineBlueprintConfigDepthState(&pipb, &(dopbtp->depthHandling));

                    if (dopbtp->hasIa)
                        AfxPipelineBlueprintConfigInputAssemblyState(&pipb, &dopbtp->inputAssembly);

                    if (AfxAssemblePipelines(dctx, 1, &pipb, &(dop->techniques[dop->techCnt].passes[dop->techniques[dop->techCnt].passCnt].pip))) AfxThrowError();
                    else
                    {
                        AfxAssertObject(dop->techniques[dop->techCnt].passes[dop->techniques[dop->techCnt].passCnt].pip, afxFcc_PIP);
                        dop->techniques[dop->techCnt].passCnt++;
                    }

                    AfxPipelineBlueprintDiscard(&pipb);
                }
            }

            if (!err)
            {
                dop->techCnt++;
            }
        }

        if (err)
        {
            AfxDeallocate(mmu, dop->techniques);
        }
    }
    return err;
}

_AVX afxClassConfig const _AfxDopClassSpec =
{
    afxFcc_DOP,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxDrawOperation)),
    NIL,
    (void*)_AfxDopCtor,
    (void*)_AfxDopDtor,
    .event = _AfxDopEventHandler,
    .eventFilter = _AfxDopEventFilter,
    "afxDrawOperation",
    NIL
};
