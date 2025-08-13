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

#define _AFX_DRAW_C
#define _AVX_SCENE_C
#define _ARX_RENDER_CONTEXT_C
#include "../impl/asxImplementation.h"
#include "../../draw/ddi/avxImplementation.h"
#include "qwadro/inc/sim/io/afxScene.h"


_ARX afxError ArxCmdRenderBodies(arxRenderContext rctx, afxM4d m, afxUnit cnt, afxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    for (afxUnit bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        afxBody bod = bodies[bodIdx];
        if (!bod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

        afxModel mdl;
        if (!AfxGetBodyModel(bod, &mdl)) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        afxModelInfo mdli;
        AfxDescribeModel(mdl, &mdli);

        _arxVisualBody* instBod;
        afxUnit bodId = AfxGetObjectId(bod);
        if (!AfxGetPoolUnit(&rctx->visualBodies, bodId, &instBod))
        {
            AfxRequestPoolUnitsAt(&rctx->visualBodies, 1, &bodId, &instBod);

            afxUnit mtxBufSiz = sizeof(afxM4d) * mdli.jntCnt;

            // workaround to get triple buffered matrices
            for (afxUnit i = 0; i < rctx->frameCnt; i++)
            {
                avxBuffer buf;
                avxBufferInfo bufi = { 0 };
                bufi.usage = avxBufferUsage_UNIFORM;
                bufi.flags = avxBufferFlag_WX;
                bufi.size = mtxBufSiz;
                AvxAcquireBuffers(rctx->dsys, 1, &bufi, &buf);

                instBod->bod = bod;
                instBod->mdl = mdl;
                instBod->jntCnt = mdli.jntCnt;
                instBod->perFrame[i].mtxUbo = buf;
                instBod->perFrame[i].mtxUboBase = 0;
                instBod->perFrame[i].mtxUboRange = mtxBufSiz;
            }
        }

        afxPose pose;
        afxPlacement plce;
        AfxGetBodyPose(instBod->bod, &pose, &plce);

#if 0
        AfxSampleBodyAnimationsAccelerated(bod, instBod->jntCnt, /*bod->placement*/NIL, pose, plce, 0.0);
#else
        if (!AfxCountBodyMotives(bod))
        {
            AfxBuildPlacement(plce, NIL, mdl, 0, instBod->jntCnt, 0, instBod->jntCnt, m ? m : AFX_M4D_IDENTITY, FALSE);
        }
        else
        {
            afxReal allowedErr = 0.f;
            afxUnit lodJntCnt = AsxCountJoints(mdl, allowedErr);
            AsxCommencePoseAccumulation(pose, 0, instBod->jntCnt, NIL);
            AfxAccumulateBodyAnimations(bod, 0, instBod->jntCnt, pose, allowedErr, NIL);
            AsxConcludePoseAccumulation(pose, 0, instBod->jntCnt, mdl, allowedErr, NIL);
            AfxBuildPlacement(plce, pose, mdl, 0, instBod->jntCnt, 0, lodJntCnt, m ? m : AFX_M4D_IDENTITY, FALSE);
        }
#endif

        afxM4d matrices[256];
        AFX_ASSERT(ARRAY_SIZE(matrices) >= instBod->jntCnt);
        AfxBuildCompositeMatrices(plce, instBod->mdl, 0, instBod->jntCnt, FALSE, matrices);
#if 0
        AvxCmdUpdateBuffer(frame->transferDctx, instBod->perFrame[rctx->frameIdx].mtxUbo, instBod->perFrame[rctx->frameIdx].mtxUboBase, instBod->perFrame[rctx->frameIdx].mtxUboRange, matrices);

        frame->mtxStackUboBaseOffset = instBod->perFrame[rctx->frameIdx].mtxUboBase;
        AvxCmdBindBuffers(frame->drawDctx, 3, 0, 1, (avxBufferedMap[]) { {.buf = instBod->perFrame[rctx->frameIdx].mtxUbo } });
#else
        void* matrixStack;
        ArxCmdPostUniform(rctx, 3, 30, instBod->perFrame[rctx->frameIdx].mtxUboRange, &matrixStack);
        AfxCopy(matrixStack, matrices, sizeof(matrices[0]) * instBod->jntCnt);
#endif
        ArxCmdRenderModels(rctx, 1, &mdl);
    }
    return err;
}

_ARX afxError ArxCmdRenderNode(arxRenderContext rctx, afxUnit cnt, afxNode nodes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    for (afxUnit nodIdx = 0; nodIdx < cnt; nodIdx++)
    {
        afxNode nod = nodes[nodIdx];
        if (!nod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

        if (nod->type == asxNodeType_Leaf_AnimBlend)
        {
            afxBody bod = nod->animBlend.bod;
            if (!bod) continue;
            AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

            afxM4d w;
            AsxGetNodeMatrix(nod, w);
            ArxCmdRenderBodies(rctx, w, 1, &bod);
        }
    }
    return err;
}
