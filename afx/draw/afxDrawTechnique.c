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

#define _AFX_DRAW_TECHNIQUE_C
#include "../dev/AvxImplKit.h"
#include "qwadro/draw/io/afxXsh.h"
#include "qwadro/draw/afxDrawTechnique.h"

////////////////////////////////////////////////////////////////////////////////

_AVX afxCmdId AvxCmdApplyDrawTechnique(avxCmdb cmdb, afxDrawTechnique dtec, afxNat passIdx, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dtec, afxFcc_DTEC);
    AfxAssertRange(dtec->passCnt, passIdx, 1);
    afxDrawPass* pass = &dtec->passes[passIdx];
    return  AvxCmdBindRasterizer(cmdb, pass->razr, vin, dynamics);
}

_AVX afxError AfxUpdateRasterizationPass(afxDrawTechnique dtec, afxNat passIdx, avxRasterizationConfig const* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dtec, afxFcc_DTEC);

    afxDrawInput din = AfxGetParent(dtec);
    afxDrawContext dctx;
    AfxGetDrawInputContext(din, &dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    avxRasterizer razr;
    AfxAssembleRasterizers(dctx, 1, cfg, &razr);

    if (dtec->passes[passIdx].razr)
    {
        AfxReleaseObjects(1, &dtec->passes[passIdx].razr);
    }
    dtec->passes[passIdx].razr = razr;
    return err;
}


_AVX afxError _AvxDtecDtorCb(afxDrawTechnique dtec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dtec, afxFcc_DTEC);
    
    AfxDeallocate(dtec->passes);

    return err;
}

_AVX afxError _AvxDtecCtorCb(afxDrawTechnique dtec, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dtec, afxFcc_DTEC);

    afxDrawInput din = args[0];
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxNat const passCnt = *(((afxNat const*)args[1]) + invokeNo);

    dtec->passes = AfxAllocate(passCnt, sizeof(dtec->passes[0]), 0, AfxHere());

    for (afxNat i = 0; i < passCnt; i++)
    {
        dtec->passes[i].pip = NIL;
        dtec->passes[i].razr = NIL;
    }

    dtec->passCnt = passCnt;

    return err;
}

_AVX afxClassConfig const _AvxDtecClsCfg =
{
    .fcc = afxFcc_DTEC,
    .name = "DrawTechnique",
    .desc = "Draw Technique",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawTechnique)),
    .ctor = (void*)_AvxDtecCtorCb,
    .dtor = (void*)_AvxDtecDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireDrawTechniques(afxDrawInput din, afxNat cnt, afxNat const passCnt[], afxDrawTechnique techniques[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(techniques);
    AfxAssert(passCnt);
    AfxAssert(cnt);

    afxClass* cls = (afxClass*)AfxGetDrawTechniqueClass(din);
    AfxAssertClass(cls, afxFcc_DTEC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)techniques, (void const*[]) { din, (void*)passCnt }))
        AfxThrowError();

    return err;
}

_AVX afxError AfxLoadDrawTechniques(afxDrawInput din, afxNat cnt, afxUri const uris[], afxDrawTechnique techniques[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    AfxAssert(techniques);
    AfxAssert(uris);
    AfxAssert(cnt);

    for (afxNat dtecIdx = 0; dtecIdx < cnt; dtecIdx++)
    {
        afxXml xml;
        afxUri fpath;
        afxUri const* uri = &uris[dtecIdx];
        AfxClipUriPath(&fpath, uri);

        if (AfxLoadXml(&xml, &fpath)) AfxThrowError();
        else
        {
            //AfxAssertType(&xml, afxFcc_XML);
            afxBool isQwadroXml = AfxTestXmlRoot(&xml, &AfxStaticString("Qwadro"));
            AfxAssert(isQwadroXml);

            afxString query;
            AfxGetUriQueryString(uri, TRUE, &query);

            afxNat xmlElemIdx;
            afxNat foundCnt = AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("DrawTechnique"), &AfxStaticString("id"), 1, &query, &xmlElemIdx);
            AfxAssert(xmlElemIdx != AFX_INVALID_INDEX);

            if (foundCnt)
            {
                afxArray razrCfgs, pipCfgs;
                AfxAllocateArray(&razrCfgs, 1, sizeof(avxRasterizationConfig), NIL);
                //AfxAllocateArray(&pipCfgs, 1, sizeof(avxPipelineBlueprint), NIL);

                afxNat n0ChildCnt = AfxCountXmlChilds(&xml, xmlElemIdx);
                afxString name, content;

                for (afxNat nodeIter0 = 0; nodeIter0 < n0ChildCnt; nodeIter0++)
                {
                    afxNat nodeIdx = AfxGetXmlChild(&xml, xmlElemIdx, nodeIter0);
                    AfxQueryXmlElement(&xml, nodeIdx, &name, &content);
                    afxNat nodeChildTagCnt = AfxCountXmlTags(&xml, nodeIdx);

                    if (0 == AfxCompareString(&name, &AfxStaticString("Pass")))
                    {
                        afxNat arrelIdx;
                        avxRasterizationConfig* config = AfxInsertArrayUnit(&razrCfgs, &arrelIdx);

                        if (!config) AfxThrowError();
                        else
                        {
                            *config = (avxRasterizationConfig) { 0 };
                            config->depthCompareOp = avxCompareOp_LESS;

                            afxNat passChildCnt = AfxCountXmlChilds(&xml, nodeIdx);
                            
                            for (afxNat passChildIter = 0; passChildIter < passChildCnt; passChildIter++)
                            {
                                afxNat childIdx = AfxGetXmlChild(&xml, nodeIdx, passChildIter);
                                AfxQueryXmlElement(&xml, childIdx, &name, &content);
                                afxNat childTagCnt = AfxCountXmlTags(&xml, childIdx);

                                afxNat baseShd = config->xfmrPipb.shdCnt;
                                afxNat foundShdCnt = 0;

                                if (0 == AfxCompareString(&name, &AfxStaticString("PrimitiveTopology")))
                                {
                                    avxTopology primTop;

                                    if ((primTop = AfxFindPrimitiveTopology(&content)))
                                    {
                                        AfxAssertRange(avxTopology_TOTAL, primTop, 1);
                                        config->xfmrPipb.primTop = primTop;
                                        config->xfmrPipb.primFlags |= avxPipelineFlag_TOPOLOGY;
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("CullMode")))
                                {
                                    avxCullMode cullMode;

                                    if ((cullMode = AfxFindCullMode(&content)))
                                    {
                                        AfxAssertRange(avxCullMode_TOTAL, cullMode, 1);
                                        config->xfmrPipb.cullMode = cullMode;
                                        config->xfmrPipb.primFlags |= avxPipelineFlag_CULL_MODE;
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("FrontFace")))
                                {
                                    afxBool invertFrontFacing;

                                    if ((invertFrontFacing = (0 == AfxCompareString(&content, &AfxStaticString("CW")))))
                                    {
                                        AfxAssert(!invertFrontFacing && (0 == AfxCompareString(&content, &AfxStaticString("CCW"))));
                                        AfxAssertBool(invertFrontFacing);
                                        config->xfmrPipb.invertFrontFacing = invertFrontFacing;
                                        config->xfmrPipb.primFlags |= avxPipelineFlag_FRONT_FACE_INV;
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("DepthClampEnabled")))
                                {
                                    config->xfmrPipb.depthClampEnabled = TRUE;
                                    config->xfmrPipb.primFlags |= avxPipelineFlag_DEPTH_CLAMP;
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("VertexShader")))
                                {
                                    if (!AfxStringIsEmpty(&content))
                                    {
                                        afxUri tempUri;
                                        AfxWrapUriString(&tempUri, &content);
                                        AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                                        AfxResetUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt]);
                                        AfxReplicateUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt], &tempUri);
                                        config->xfmrPipb.shdStage[config->xfmrPipb.shdCnt] = avxShaderStage_VERTEX;
                                        ++config->xfmrPipb.shdCnt;
                                    }
                                    else
                                    {
                                        // ?
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("VertexInputStream")))
                                {
                                    afxNat instanceRate = 0;
                                    afxNat visSlot = 0;

                                    if (!AfxStringIsEmpty(&content))
                                    {
                                        if (AfxScanString(&content, "%u", &visSlot))
                                        {

                                        }
                                    }
                                    else
                                    {
                                        // ?
                                    }

                                    afxNat childTagCnt = AfxCountXmlTags(&xml, childIdx);

                                    for (afxNat j = 0; j < childTagCnt; j++)
                                    {
                                        AfxQueryXmlTag(&xml, childIdx, j, &name, &content);

                                        if (0 == AfxCompareString(&name, &AfxStaticString("instanceRate")))
                                        {
                                            if (AfxScanString(&content, "%u", &instanceRate))
                                            {

                                            }
                                        }
                                        else
                                        {
                                            AfxLogY("Attribute '%.*s' not handled.", AfxPushString(&name));
                                        }
                                    }

                                    //config->vis[config->visCnt].instanceRate = instanceRate;
                                    //config->vis[config->visCnt].slot = visSlot;
                                    //config->visCnt++;
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("HullShader")))
                                {
                                    if (!AfxStringIsEmpty(&content))
                                    {
                                        afxUri tempUri;
                                        AfxWrapUriString(&tempUri, &content);
                                        AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                                        AfxResetUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt]);
                                        AfxReplicateUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt], &tempUri);
                                        config->xfmrPipb.shdStage[config->xfmrPipb.shdCnt] = avxShaderStage_TESS_EVAL;
                                        ++config->xfmrPipb.shdCnt;
                                    }
                                    else
                                    {
                                        // ?
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("DomainShader")))
                                {
                                    if (!AfxStringIsEmpty(&content))
                                    {
                                        afxUri tempUri;
                                        AfxWrapUriString(&tempUri, &content);
                                        AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                                        AfxResetUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt]);
                                        AfxReplicateUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt], &tempUri);
                                        config->xfmrPipb.shdStage[config->xfmrPipb.shdCnt] = avxShaderStage_TESS_CTRL;
                                        ++config->xfmrPipb.shdCnt;
                                    }
                                    else
                                    {
                                        // ?
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("PrimitiveShader")))
                                {
                                    if (!AfxStringIsEmpty(&content))
                                    {
                                        afxUri tempUri;
                                        AfxWrapUriString(&tempUri, &content);
                                        AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                                        AfxResetUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt]);
                                        AfxReplicateUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt], &tempUri);
                                        config->xfmrPipb.shdStage[config->xfmrPipb.shdCnt] = avxShaderStage_PRIMITIVE;
                                        ++config->xfmrPipb.shdCnt;
                                    }
                                    else
                                    {
                                        // ?
                                    }
                                }
                                else // RASTERIZATION DATA AHEAD                                
                                if (0 == AfxCompareString(&name, &AfxStaticString("RasterizerDiscardEnable")))
                                {
                                    afxBool rasterizationDisabled;

                                    if ((rasterizationDisabled = (0 == AfxCompareString(&content, &AfxStaticString("true")))))
                                    {
                                        AfxAssertBool(rasterizationDisabled);
                                        config->rasFlags |= NIL;

                                        // retornar sem a chance de buscar por mais entradas, uma vez que não faria sentido as ter se não para usar.
                                        break;
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("FillMode")))
                                {
                                    avxFillMode fillMode = AfxFindFillMode(&content);
                                    AfxAssertRange(avxFillMode_TOTAL, fillMode, 1);
                                    config->fillMode = fillMode;
                                    config->rasFlags |= avxRasterizationFlag_FILL_MODE;
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("DepthBias")))
                                {
                                    afxBool enabledSet = FALSE, constantSet = FALSE, clampSet = FALSE, slopeSet = FALSE;

                                    for (afxNat j = 0; j < childTagCnt; j++)
                                    {
                                        AfxQueryXmlTag(&xml, childIdx, j, &name, &content);

                                        if (0 == AfxCompareString(&name, &AfxStaticString("constant")))
                                        {
                                            afxReal depthBiasConstFactor;

                                            if (AfxScanString(&content, "%f", &depthBiasConstFactor))
                                            {
                                                config->depthBiasConstFactor = depthBiasConstFactor;
                                                constantSet = TRUE;
                                                enabledSet = TRUE;
                                            }
                                        }
                                        else if (0 == AfxCompareString(&name, &AfxStaticString("clamp")))
                                        {
                                            afxReal depthBiasClamp;

                                            if (AfxScanString(&content, "%f", &depthBiasClamp))
                                            {
                                                config->depthBiasClamp = depthBiasClamp;
                                                clampSet = TRUE;
                                                enabledSet = TRUE;
                                            }
                                        }
                                        else if (0 == AfxCompareString(&name, &AfxStaticString("slope")))
                                        {
                                            afxReal depthBiasSlopeScale;

                                            if (AfxScanString(&content, "%f", &depthBiasSlopeScale))
                                            {
                                                config->depthBiasSlopeScale = depthBiasSlopeScale;
                                                slopeSet = TRUE;
                                                enabledSet = TRUE;
                                            }
                                        }
                                        else
                                        {
                                            AfxLogY("Attribute '%.*s' not handled.", AfxPushString(&name));
                                        }
                                    }

                                    if (enabledSet)
                                    {
                                        config->rasFlags |= avxRasterizationFlag_DEPTH_BIAS;
#if 0
                                        if (!clampSet)
                                        {
                                            config->depthBiasClamp = identity->depthBiasClamp;
                                            AfxLogY("");
                                        }

                                        if (!constantSet)
                                        {
                                            config->depthBiasConstFactor = identity->depthBiasConstFactor;
                                            AfxLogY("");
                                        }

                                        if (!slopeSet)
                                        {
                                            config->depthBiasSlopeScale = identity->depthBiasSlopeScale;
                                            AfxLogY("");
                                        }
#endif
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("LineWidth")))
                                {
                                    afxReal lineWidth;

                                    if (AfxScanString(&content, "%f", &lineWidth))
                                    {
                                        config->lineWidth = lineWidth;
                                        config->rasFlags |= avxRasterizationFlag_LINE_WIDTH;
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("DepthTestEnabled")))
                                {
                                    config->depthTestEnabled = TRUE;
                                    config->dsFlags |= avxDepthStencilFlag_TEST;
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("DepthWriteDisabled")))
                                {
                                    config->depthWriteDisabled = TRUE;
                                    config->dsFlags |= avxDepthStencilFlag_DONT_WRITE;
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("DepthCompareOp")))
                                {
                                    if ((config->depthCompareOp = AfxFindCompareOp(&content)))
                                    {
                                        config->dsFlags |= avxDepthStencilFlag_COMPARE;
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("DepthBoundsTest")))
                                {
                                    afxBool boundsSet = FALSE, boundsMinSet = FALSE, boundsMaxSet = FALSE;

                                    for (afxNat j = 0; j < childTagCnt; j++)
                                    {
                                        AfxQueryXmlTag(&xml, childIdx, j, &name, &content);

                                        if (0 == AfxCompareString(&name, &AfxStaticString("min")))
                                        {
                                            afxReal mini;

                                            if (AfxScanString(&content, "%f", &mini))
                                            {
                                                config->depthBounds[0] = mini;
                                                boundsMinSet = TRUE;
                                                boundsSet = TRUE;
                                            }
                                        }
                                        else if (0 == AfxCompareString(&name, &AfxStaticString("max")))
                                        {
                                            afxReal maxi;

                                            if (AfxScanString(&content, "%f", &maxi))
                                            {
                                                config->depthBounds[1] = maxi;
                                                boundsMinSet = TRUE;
                                                boundsSet = TRUE;
                                            }
                                        }
                                        else
                                        {
                                            AfxLogY("Attribute '%.*s' not handled.", AfxPushString(&name));
                                        }
                                    }

                                    if (boundsSet)
                                    {
                                        config->dsFlags = avxDepthStencilFlag_BOUNDS_TEST;
#if 0
                                        if (!boundsMinSet)
                                        {
                                            config->depthBounds[0] = identity->depthBounds[0];
                                            AfxLogY("");
                                        }

                                        if (!boundsMaxSet)
                                        {
                                            config->depthBounds[1] = identity->depthBounds[1];
                                            AfxLogY("");
                                        }
#endif
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("StencilTest")))
                                {
                                    for (afxNat j = 0; j < childTagCnt; j++)
                                    {
                                        AfxQueryXmlTag(&xml, childIdx, j, &name, &content);

                                        if (0 == AfxCompareString(&name, &AfxStaticString("enable")))
                                        {
                                            if ((config->stencilTestEnabled = (0 == AfxCompareString(&content, &AfxStaticString("true")))))
                                            {
                                                config->dsFlags |= avxDepthStencilFlag_STENCIL_TEST;
#if !0
                                                afxNat childCnt2 = AfxCountXmlChilds(&xml, childIdx);

                                                for (afxNat k = 0; k < childCnt2; k++)
                                                {
                                                    afxNat childIdx2 = AfxGetXmlChild(&xml, childIdx, k);
                                                    AfxQueryXmlElement(&xml, childIdx2, &name, &content);

                                                    if (0 == AfxCompareString(&name, &AfxStaticString("Front")))
                                                    {
                                                        //_AfxParseXmlBackedPipelineDepthStateStencilFace(childChild, elemIdx, specIdx, TRUE, state, state, foundMask); // 7 - 8 - 9 - 10

                                                    }
                                                    else if (0 == AfxCompareString(&name, &AfxStaticString("Back")))
                                                    {
                                                        //_AfxParseXmlBackedPipelineDepthStateStencilFace(childChild, elemIdx, specIdx, FALSE, state, state, foundMask); // 11 - 12 - 13 - 14
                                                    }
                                                    else
                                                    {
                                                        AfxLogY("Node '%.*s' not handled.", AfxPushString(&name));
                                                    }
                                                }
#endif
                                            }
                                        }
                                        else
                                        {
                                            AfxLogY("Attribute '%.*s' not handled.", AfxPushString(&name));
                                        }
                                    }
                                }
                                else if (0 == AfxCompareString(&name, &AfxStaticString("FragmentShader")))
                                {
                                    if (!AfxStringIsEmpty(&content))
                                    {
                                        afxUri tempUri;
                                        AfxWrapUriString(&tempUri, &content);
                                        AfxLogEcho("%.*s", AfxPushString(AfxGetUriString(&tempUri)));

                                        AfxResetUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt]);
                                        AfxReplicateUri(&config->xfmrPipb.shdUri[config->xfmrPipb.shdCnt], &tempUri);
                                        config->xfmrPipb.shdStage[config->xfmrPipb.shdCnt] = avxShaderStage_FRAGMENT;
                                        ++config->xfmrPipb.shdCnt;
                                    }
                                    else
                                    {
                                        // ?
                                    }
                                }
                                else
                                {
                                    AfxLogY("Node '%.*s' not handled.", AfxPushString(&name));
                                }
                            }

                        }
                    }
                }

                AfxAcquireDrawTechniques(din, 1, &razrCfgs.cnt, &techniques[dtecIdx]);

                afxNat itemIdx;
                avxRasterizationConfig* itemCfg;
                for (afxNat itemIdx = 0; itemIdx < razrCfgs.cnt; ++itemIdx)
                {
                    itemCfg = AfxGetArrayUnit(&razrCfgs, itemIdx);
                    AfxUpdateRasterizationPass(techniques[dtecIdx], itemIdx, itemCfg);
                }

                AfxDeallocateArray(&razrCfgs);
            }

            AfxCleanUpXml(&xml);
        }
    }
    return err;
}
