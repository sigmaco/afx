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

#define _ARX_SCENE_C
#define _ARX_RENDER_CONTEXT_C
#define _ASX_NODE_C
#include "ddi/arxImpl_Input.h"
#include "../draw/ddi/avxImplementation.h"
#include "qwadro/render/arxScene.h"
#include "qwadro/sim/afxSimulation.h"


_ARX afxError ArxCmdRenderBodies(arxRenderContext rctx, afxM4d m, afxUnit cnt, arxBody bodies[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    for (afxUnit bodIdx = 0; bodIdx < cnt; bodIdx++)
    {
        arxBody bod = bodies[bodIdx];
        if (!bod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

        arxModel mdl;
        if (!ArxGetBodyModel(bod, &mdl)) continue;
        AFX_ASSERT_OBJECTS(afxFcc_MDL, 1, &mdl);

        arxModelInfo mdli;
        ArxDescribeModel(mdl, &mdli);

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

        arxPose pose;
        arxPlacement plce;
        ArxGetBodyPose(instBod->bod, &pose, &plce);

#if 0
        AfxSampleBodyAnimationsAccelerated(bod, instBod->jntCnt, /*bod->placement*/NIL, pose, plce, 0.0);
#else
        if (!ArxCountBodyMotives(bod))
        {
            ArxBuildPlacement(plce, NIL, mdl, 0, instBod->jntCnt, 0, instBod->jntCnt, m ? m : AFX_M4D_IDENTITY, FALSE);
        }
        else
        {
            afxReal allowedErr = 0.f;
            afxUnit lodJntCnt = ArxCountJoints(mdl, allowedErr);
            ArxCommencePoseAccumulation(pose, 0, instBod->jntCnt, NIL);
            ArxAccumulateBodyAnimations(bod, 0, instBod->jntCnt, pose, allowedErr, NIL);
            ArxConcludePoseAccumulation(pose, 0, instBod->jntCnt, mdl, allowedErr, NIL);
            ArxBuildPlacement(plce, pose, mdl, 0, instBod->jntCnt, 0, lodJntCnt, m ? m : AFX_M4D_IDENTITY, FALSE);
        }
#endif

        afxM4d matrices[256];
        AFX_ASSERT(ARRAY_SIZE(matrices) >= instBod->jntCnt);
        ArxBuildCompositeMatrices(plce, instBod->mdl, 0, instBod->jntCnt, FALSE, matrices);
#if !0
        //AvxCmdUpdateBuffer(frame->transferDctx, instBod->perFrame[rctx->frameIdx].mtxUbo, instBod->perFrame[rctx->frameIdx].mtxUboBase, instBod->perFrame[rctx->frameIdx].mtxUboRange, matrices);
        AvxCmdUpdateBuffer(frame->transferDctx, instBod->perFrame[rctx->frameIdx].mtxUbo, instBod->perFrame[rctx->frameIdx].mtxUboBase, instBod->perFrame[rctx->frameIdx].mtxUboRange, matrices);

        frame->mtxStackUboBaseOffset = instBod->perFrame[rctx->frameIdx].mtxUboBase;
        AvxCmdBindBuffers(frame->drawDctx, avxBus_DRAW, 3, 30, 1, (avxBufferedMap[]) { {.buf = instBod->perFrame[rctx->frameIdx].mtxUbo } });
#else
        void* matrixStack;
        ArxCmdPostUniform(rctx, 3, 30, instBod->perFrame[rctx->frameIdx].mtxUboRange, &matrixStack);
        AfxCopy(matrixStack, matrices, sizeof(matrices[0]) * instBod->jntCnt);
#endif
        ArxCmdRenderModels(rctx, 1, &mdl);
    }
    return err;
}

_ARX afxError ArxCmdRenderNode(arxRenderContext rctx, afxUnit cnt, arxNode nodes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    for (afxUnit nodIdx = 0; nodIdx < cnt; nodIdx++)
    {
        arxNode nod = nodes[nodIdx];
        if (!nod) continue;
        AFX_ASSERT_OBJECTS(afxFcc_NOD, 1, &nod);

        if (nod->type == arxNodeType_Leaf_AnimBlend)
        {
            arxBody bod = nod->bod;
            if (!bod) continue;
            AFX_ASSERT_OBJECTS(afxFcc_BOD, 1, &bod);

            afxM4d w;
            ArxGetNodeMatrix(nod, w);
            ArxCmdRenderBodies(rctx, w, 1, &bod);
        }
    }
    return err;
}
