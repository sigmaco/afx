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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_DRAW_TECHNIQUE_C
#include "../impl/avxImplementation.h"
#include "qwadro/inc/draw/kit/afxDrawTechnique.h"

AVX afxError _AvxParseXmlPipelineBlueprint(afxXml const* xml, afxUnit elemIdx, afxUnit specId, avxPipelineBlueprint* pipb, avxShaderStage shaderStages[], afxUri shaderUris[], afxString shaderFns[]);

////////////////////////////////////////////////////////////////////////////////

_AVX afxCmdId AvxCmdApplyDrawTechnique(afxDrawContext dctx, afxDrawTechnique dtec, afxUnit passIdx, avxVertexDecl vin, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);
    AFX_ASSERT_RANGE(dtec->passCnt, passIdx, 1);
    afxDrawPass* pass = &dtec->passes[passIdx];
    return  AvxCmdBindPipeline(dctx, 0, pass->pip, vin, dynamics);
}

_AVX afxError AfxUpdateRasterizationPass(afxDrawTechnique dtec, afxUnit passIdx, avxPipelineBlueprint const* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);

    afxDrawInput din = AfxGetProvider(dtec);
    afxDrawSystem dsys = AfxGetDrawInputContext(din);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxPipeline pip;
    AfxAssemblePipelines(dsys, 1, cfg, &pip);

    if (dtec->passes[passIdx].pip)
    {
        AfxDisposeObjects(1, &dtec->passes[passIdx].pip);
    }
    dtec->passes[passIdx].pip = pip;
    return err;
}

_AVX afxError _AvxDtecDtorCb(afxDrawTechnique dtec)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);
    
    AfxDeallocate((void**)&dtec->passes, AfxHere());

    return err;
}

_AVX afxError _AvxDtecCtorCb(afxDrawTechnique dtec, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DTEC, 1, &dtec);

    afxDrawInput din = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxUnit const passCnt = *(((afxUnit const*)args[1]) + invokeNo);

     AfxAllocate(passCnt * sizeof(dtec->passes[0]), 0, AfxHere(), (void**)&dtec->passes);

    for (afxUnit i = 0; i < passCnt; i++)
    {
        dtec->passes[i].pip = NIL;
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

_AVX afxError AfxLoadDrawTechnique(afxDrawInput din, afxUri const* uri, afxDrawTechnique* technique)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
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
    else if (!AfxTestXmlRoot(&xml, &AfxStaticString("DrawTechnique")))
    {
        AfxThrowError();
        AfxCleanUpXml(&xml);
        return err;
    }

    AFX_ASSERT(xmlElemIdx != AFX_INVALID_INDEX);

    afxUnit passCnt = AfxFindXmlTaggedElements(&xml, xmlElemIdx, 0, &AfxStaticString("Pass"), NIL, 0, NIL, NIL);

    afxClass* cls = (afxClass*)AvxGetDrawTechniqueClass(din);
    AFX_ASSERT_CLASS(cls, afxFcc_DTEC);

    if (AfxAcquireObjects(cls, 1, (afxObject*)technique, (void const*[]) { din, (void*)&passCnt })) AfxThrowError();
    else
    {
        afxUnit n0ChildCnt = AfxCountXmlChilds(&xml, xmlElemIdx);
        afxString name, ncontent, acontent;
        afxInt passIdx = 0;

        avxShaderStage shaderStages[6] = { 0 };
        afxUri shaderUris[6] = { 0 };
        afxString shaderFns[6] = { 0 };

        for (afxUnit nodeIter0 = 0; nodeIter0 < n0ChildCnt; nodeIter0++)
        {
            afxUnit nodeIdx = AfxGetXmlChild(&xml, xmlElemIdx, nodeIter0);
            AfxQueryXmlElement(&xml, nodeIdx, &name, &ncontent);
            afxUnit nodeChildTagCnt = AfxCountXmlTags(&xml, nodeIdx);

            if (0 == AfxCompareStrings(&name, 0, TRUE, 1, &AfxStaticString("Pass")))
            {
                afxUnit arrelIdx;
                avxPipelineBlueprint config = { 0 };
                _AvxParseXmlPipelineBlueprint(&xml, nodeIdx, 0, &config, shaderStages, shaderUris, shaderFns);

                AfxUpdateRasterizationPass(*technique, passIdx, &config);

                for (afxUnit i = 0; i < config.stageCnt; i++)
                {
                    AfxUplinkPipelineFunction((*technique)->passes[passIdx].pip, shaderStages[i], &shaderUris[i], &shaderFns[i], NIL, NIL);
                }

                ++passIdx;
            }
        }
    }

    AfxCleanUpXml(&xml);

    return err;
}
