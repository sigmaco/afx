/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *          Q W A D R O   4 D   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_DRAW_TECHNIQUE_C
#include "ddi/arxImpl_Input.h"

ARX afxError _AvxParseXmlPipelineBlueprint(afxXml const* xml, afxUnit elemIdx, afxUnit specId, avxPipelineConfig* pipb, avxShaderType shaderStages[], afxUri shaderUris[], afxString shaderFns[]);

////////////////////////////////////////////////////////////////////////////////

_ARX afxCmdId AvxCmdUseDrawTechniqueSIGMA(afxDrawContext dctx, arxTechnique dtec, afxUnit passIdx, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);
    AFX_ASSERT_RANGE(dtec->passCnt, passIdx, 1);
    arxDrawPass* pass = &dtec->passes[passIdx];
    return  AvxCmdBindPipeline(dctx, 0, pass->pip, vin, dynamics);
}

_ARX afxError ArxUpdateRasterizationPass(arxTechnique dtec, afxUnit passIdx, avxPipelineConfig const* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);

    arxRenderware rwe = AfxGetHost(dtec);
    afxDrawSystem dsys = ArxGetRenderwareDrawSystem(rwe);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxPipeline pip;
    AvxAssemblePipelines(dsys, 1, cfg, &pip);

    if (dtec->passes[passIdx].pip)
    {
        AfxDisposeObjects(1, &dtec->passes[passIdx].pip);
    }
    dtec->passes[passIdx].pip = pip;
    return err;
}

_ARX afxError _ArxDtecDtorCb(arxTechnique dtec)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);
    
    AfxDeallocate((void**)&dtec->passes, AfxHere());

    return err;
}

_ARX afxError _ArxDtecCtorCb(arxTechnique dtec, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);

    arxRenderware rwe = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    afxUnit const passCnt = *(((afxUnit const*)args[1]) + invokeNo);

     AfxAllocate(passCnt * sizeof(dtec->passes[0]), 0, AfxHere(), (void**)&dtec->passes);

    for (afxUnit i = 0; i < passCnt; i++)
    {
        dtec->passes[i].pip = NIL;
    }

    dtec->passCnt = passCnt;

    return err;
}

_ARX afxClassConfig const _ARX_DTEC_CLASS_CONFIG =
{
    .fcc = afxFcc_DTEC,
    .name = "DrawTechnique",
    .desc = "Draw Technique",
    .fixedSiz = sizeof(AFX_OBJECT(arxTechnique)),
    .ctor = (void*)_ArxDtecCtorCb,
    .dtor = (void*)_ArxDtecDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxLoadDrawTechnique(arxRenderware rwe, afxUri const* uri, arxTechnique* technique)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RWE, 1, &rwe);
    AFX_ASSERT(technique);
    AFX_ASSERT(uri);

    afxXml xml;
    afxUri fpath;
    afxString query;
    AfxClipUriPath(&fpath, uri);
    AfxGetUriQueryString(uri, TRUE, &query);
    afxUnit xmlElemIdx = 0;

    if (AfxLoadXml(&xml, &fpath))
    {
        AfxThrowError();
        return err;
    }
    //else if (!AfxFindXmlTaggedElements(&xml, 0, 0, &AfxStaticString("DrawTechnique"), &AfxStaticString("id"), 1, &query, &xmlElemIdx))
    else if (!AfxTestXmlRoot(&xml, &AFX_STRING("DrawTechnique")))
    {
        AfxThrowError();
        AfxCleanUpXml(&xml);
        return err;
    }

    AFX_ASSERT(xmlElemIdx != AFX_INVALID_INDEX);

    afxUnit passCnt = AfxFindXmlTaggedElements(&xml, xmlElemIdx, 0, &AFX_STRING("Pass"), NIL, 0, NIL, NIL);

    afxClass* cls = (afxClass*)_ArxRweGetTechClass(rwe);
    AFX_ASSERT_CLASS(cls, afxFcc_DTEC);

    if (AfxAcquireObjects(cls, 1, (afxObject*)technique, (void const*[]) { rwe, (void*)&passCnt })) AfxThrowError();
    else
    {
        afxUnit n0ChildCnt = AfxCountXmlChilds(&xml, xmlElemIdx);
        afxString name, ncontent, acontent;
        afxInt passIdx = 0;

        avxShaderType shaderStages[6] = { 0 };
        afxUri shaderUris[6] = { 0 };
        afxString shaderFns[6] = { 0 };

        for (afxUnit nodeIter0 = 0; nodeIter0 < n0ChildCnt; nodeIter0++)
        {
            afxUnit nodeIdx = AfxGetXmlChild(&xml, xmlElemIdx, nodeIter0);
            AfxQueryXmlElement(&xml, nodeIdx, &name, &ncontent);
            afxUnit nodeChildTagCnt = AfxCountXmlTags(&xml, nodeIdx);

            if (AfxCompareStrings(&name, 0, TRUE, 1, &AFX_STRING("Pass"), NIL))
            {
                afxUnit arrelIdx;
                avxPipelineConfig config = { 0 };
                _AvxParseXmlPipelineBlueprint(&xml, nodeIdx, 0, &config, shaderStages, shaderUris, shaderFns);

                ArxUpdateRasterizationPass(*technique, passIdx, &config);

                for (afxUnit i = 0; i < config.stageCnt; i++)
                {
                    AvxUplinkPipelineFunction((*technique)->passes[passIdx].pip, shaderStages[i], &shaderUris[i], &shaderFns[i], NIL, NIL);
                }

                ++passIdx;
            }
        }
    }

    AfxCleanUpXml(&xml);

    return err;
}
