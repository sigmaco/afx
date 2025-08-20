/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _AFX_DRAW_C
#define _ARX_SCENE_C
#define _ARX_MODEL_C
#define _ARX_SKELETON_C
#define _ARX_RENDER_CONTEXT_C
#include "ddi/arxImpl_Input.h"
#include "../draw/ddi/avxImplementation.h"
#include "qwadro/inc/render/arxScene.h"
#include "qwadro/inc/render/arxRenderContext.h"
#include "qwadro/inc/render/akxRenderer.h"
#include "qwadro/inc/render/arxTerrain.h"

AFX_DEFINE_STRUCT(_arxVisualTechnique)
{
    avxPipeline pipeline;
    afxChain    materials;
};

_ARX afxError ArxCmdUseArgumentBuffer(arxRenderContext rctx, afxUnit baseSetIdx, afxUnit setCnt, avxBuffer buffers[], afxSize const offsets[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    int64_t a = 'came';
    return err;
}

_ARX afxError ArxCmdPostUniform(arxRenderContext rctx, afxUnit set, afxUnit binding, afxUnit dataSiz, void** pData)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    avxBuffer out_buffer;
    afxSize out_offset;
    void* p = ArxRequestBufferedPump(&rctx->dynUnfmAlloc, dataSiz, &out_buffer, &out_offset);
    AFX_ASSERT(pData);
    *pData = p;

    if (frame->boundDynUbos[set][binding] != out_buffer)
    {
        if (frame->boundDynUbos[set][binding])
        {
            AvxCohereMappedBuffer(frame->boundDynUbos[set][binding], 0, frame->boundDynUbosRangeToBeFlused[set][binding], NIL, FALSE);
            frame->boundDynUbosRangeToBeFlused[set][binding] = 0;
        }

        frame->boundDynUbos[set][binding] = out_buffer;

        if (out_buffer)
        {
            frame->boundDynUbosRangeToBeFlused[set][binding] = out_offset + dataSiz;
            avxBufferedMap map = AVX_BUFFERED_MAP(out_buffer, 0, AvxGetBufferCapacity(out_buffer, 0), NIL);
            AvxCmdBindBuffers(frame->drawDctx, set, binding, 1, &map);
        }
    }
    else
    {
        frame->boundDynUbosRangeToBeFlused[set][binding] = AFX_MAX(frame->boundDynUbosRangeToBeFlused[set][binding], out_offset) + dataSiz;
    }
}

_ARX afxError ArxCmdPostVertices(arxRenderContext rctx, afxUnit vtxCnt, afxUnit vtxSiz, void** pVertices)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    afxUnit dataSiz = vtxCnt * vtxSiz;

    avxBuffer out_buffer;
    afxSize out_offset;
    void* p = ArxRequestBufferedPump(&rctx->dynVtxAlloc, dataSiz, &out_buffer, &out_offset);
    AFX_ASSERT(pVertices);
    *pVertices = p;

    if (frame->boundDynVbo != out_buffer)
    {
        if (frame->boundDynVbo)
        {
            AvxCohereMappedBuffer(frame->boundDynVbo, 0, frame->boundDynVboRangeToBeFlused, NIL, FALSE);
            frame->boundDynVboRangeToBeFlused = 0;
        }

        frame->boundDynVbo = out_buffer;

        if (out_buffer)
        {
            frame->boundDynVboRangeToBeFlused = out_offset + dataSiz;
            avxBufferedStream map = AVX_BUFFERED_STREAM(out_buffer, 0, AvxGetBufferCapacity(out_buffer, 0), vtxSiz);
            AvxCmdBindVertexBuffers(frame->drawDctx, 0, 1, &map);
        }
    }
    else
    {
        frame->boundDynVboRangeToBeFlused = AFX_MAX(frame->boundDynVboRangeToBeFlused, out_offset) + dataSiz;
    }
}

_ARX afxError ArxCmdPostVertexIndices(arxRenderContext rctx, afxUnit idxCnt, afxUnit idxSiz, void** pIndices)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    afxUnit dataSiz = idxCnt * idxSiz;

    avxBuffer out_buffer;
    afxSize out_offset;
    void* p = ArxRequestBufferedPump(&rctx->dynIdxAlloc, dataSiz, &out_buffer, &out_offset);
    AFX_ASSERT(pIndices);
    *pIndices = p;

    if (frame->boundDynIbo != out_buffer)
    {
        if (frame->boundDynIbo)
        {
            AvxCohereMappedBuffer(frame->boundDynIbo, 0, frame->boundDynIboRangeToBeFlused, NIL, FALSE);
            frame->boundDynIboRangeToBeFlused = 0;
        }

        frame->boundDynIbo = out_buffer;

        if (out_buffer)
        {
            frame->boundDynIboRangeToBeFlused = out_offset + dataSiz;
            avxBufferedMap map = AVX_BUFFERED_MAP(out_buffer, 0, AvxGetBufferCapacity(out_buffer, 0), NIL);
            AvxCmdBindIndexBuffer(frame->drawDctx, out_buffer, 0, map.range, idxSiz);
        }
    }
    else
    {
        frame->boundDynIboRangeToBeFlused = AFX_MAX(frame->boundDynIboRangeToBeFlused, out_offset) + dataSiz;
    }
}

_ARX afxError ArxStageMaterials(arxRenderContext rctx, arxGeome morp, afxUnit cnt, afxUnit indices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    afxClass* mtlCls = (afxClass*)_ArxMorpGetMaterialClass(morp);

    for (afxUnit i = 0; i < cnt; i++)
    {
        arxMaterial mtl;
        ArxEnumerateMaterials(morp, indices[i], 1, &mtl);

        _arxVisualMaterial* instMtl;
        afxUnit mtlId = AfxGetObjectId(mtl);
        if (!AfxGetPoolUnit(&rctx->visualMaterials, mtlId, &instMtl))
        {
            AfxRequestPoolUnitsAt(&rctx->visualMaterials, 1, &mtlId, &instMtl);

            instMtl->mtl = mtl;

            avxBuffer buf;
            avxBufferInfo bufi = { 0 };
            bufi.size = 64;
            bufi.usage = avxBufferUsage_UNIFORM;
            bufi.flags = avxBufferFlag_WX/* | avxBufferFlag_COHERENT*/;
            AvxAcquireBuffers(rctx->dsys, 1, &bufi, &buf);

            instMtl->dataUbo = buf;
            instMtl->dataUboBase = 0;
            instMtl->dataUboRange = bufi.size;
        }
    }

    return err;
}

_ARX afxBool ArxStdNodeCullCallback(void* udd, afxNode nod)
{
    return FALSE; // do not cull
}

_ARX afxBool ArxStdBodyCullCallback(void* udd, arxBody bod)
{
    return FALSE; // do not cull
}

_ARX afxBool ArxStdModelCullCallback(void* udd, arxModel mdl)
{
    return FALSE;
}

_ARX afxError ArxExecuteRenderContext(arxRenderContext rctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    afxCmdId lastId = 0; // DBG

    return err;
}

_ARX afxError ArxCmdUseRenderTechnique(arxRenderContext rctx, arxTechnique dtec, afxUnit passId, avxVertexInput vin, afxFlags dynamics)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    AvxCmdUseDrawTechniqueSIGMA(frame->drawDctx, dtec, passId, vin, dynamics);

    return err;
}

_ARX afxError ArxCmdUseCamera(arxRenderContext rctx, arxCamera cam, afxRect const* drawArea)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    akxViewConstants* viewConsts = &frame->viewConsts;

    if ((frame->activeCam = cam))
    {
        //cam = rnd->activeCamera;

        afxV2d extent = { drawArea->w, drawArea->h };
        ArxSetCameraAspectRatios(cam, AvxFindPhysicalAspectRatio(drawArea->w, drawArea->h), extent, extent);

        viewConsts->viewExtent[0] = drawArea->w;
        viewConsts->viewExtent[1] = drawArea->h;

        afxV4d viewPos;
        ArxGetCameraPosition(cam, viewPos);
        AfxV4dCopyAtv3d(viewConsts->viewPos, viewPos);

        afxM4d v, iv, p, ip, pv, ipv;
        ArxRecomputeCameraMatrices(cam);
        ArxGetCameraMatrices(cam, p, pv, v, ip, ipv, iv);

        AfxM4dCopy(viewConsts->p, p);
        AfxM4dCopy(viewConsts->ip, ip);
        AfxM4dCopyAtm(viewConsts->v, v);
        AfxM4dCopy(viewConsts->iv, iv);
        AfxM4dCopy(viewConsts->pv, pv);
        AfxM4dCopy(viewConsts->ipv, ipv);
    }

    AvxCmdUpdateBuffer(frame->transferDctx, frame->viewUbo, 0, sizeof(*viewConsts), viewConsts);

    AvxCmdBindBuffers(frame->drawDctx, 0, 0, 1, (avxBufferedMap[]) { { .buf = frame->viewUbo } });

    return err;
}

_ARX afxError ArxBeginFrame(arxRenderContext rctx, void* param, afxUnit* frameIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    if (rctx->inFrameGoingOn)
    {
        AFX_ASSERT(!rctx->inFrameGoingOn);
        return err;
    }
    rctx->inFrameGoingOn = TRUE;

    afxUnit frameIdx2 = (rctx->frameIdx + 1) % rctx->frameCnt;
    rctx->frameIdx = frameIdx2;
    if (frameIdx) *frameIdx = frameIdx2;

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    ArxAdvanceBufferedPump(&rctx->dynIdxAlloc);
    ArxAdvanceBufferedPump(&rctx->dynVtxAlloc);
    ArxAdvanceBufferedPump(&rctx->dynUnfmAlloc);

    if (!frame->viewUbo)
    {
        avxBuffer buf;
        avxBufferInfo bufi = { 0 };
        bufi.size = sizeof(frame->viewConsts);
        bufi.usage = avxBufferUsage_UNIFORM;
        bufi.flags = avxBufferFlag_WX/* | avxBufferFlag_C*/;
        AvxAcquireBuffers(rctx->dsys, 1, &bufi, &buf);
        frame->viewUbo = buf;
    }
    return err;
}

_ARX afxError ArxEndFrame(arxRenderContext rctx, void* param, afxUnit* perSecFrame)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    if (!rctx->inFrameGoingOn)
    {
        AFX_ASSERT(rctx->inFrameGoingOn);
        return err;
    }
    rctx->inFrameGoingOn = FALSE;

    if (perSecFrame) *perSecFrame = rctx->perSecFrame;

    return err;
}

_ARX afxError ArxWaitFrame(arxRenderContext rctx, void* param)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    return err;
}

_ARX afxError ArxBeginScene(arxRenderContext rctx, afxDrawContext transferDctx, afxDrawContext drawDctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    if (!transferDctx)
    {
        afxDrawContext dctx;
        AvxAcquireDrawContexts(rctx->dsys, afxDrawCaps_TRANSFER, NIL, TRUE, FALSE, 1, &dctx);
        
        if (frame->transferDctx)
            AfxDisposeObjects(1, &frame->transferDctx);

        frame->transferDctx = dctx;
        frame->internalTransferDctx = TRUE;

        afxUnit tbId;
        AvxRecordDrawCommands(dctx, TRUE, FALSE, &tbId);
        frame->tbId = tbId;
    }
    else
    {
        frame->internalTransferDctx = FALSE;
        frame->transferDctx = transferDctx;
        AfxReacquireObjects(1, &frame->transferDctx);
    }

    if (!drawDctx)
    {
        afxDrawContext dctx;
        AvxAcquireDrawContexts(rctx->dsys, afxDrawCaps_DRAW, NIL, TRUE, FALSE, 1, &dctx);

        if (frame->drawDctx)
            AfxDisposeObjects(1, &frame->drawDctx);

        frame->drawDctx = dctx;
        frame->internalDrawDctx = TRUE;

        afxUnit dbId;
        AvxRecordDrawCommands(dctx, TRUE, FALSE, &dbId);
        frame->dbId = dbId;
    }
    else
    {
        frame->internalDrawDctx = FALSE;
        frame->drawDctx = drawDctx;
        AfxReacquireObjects(1, &frame->drawDctx);
    }

    return err;
}

_ARX afxError ArxEndScene(arxRenderContext rctx, afxUnit id)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);
    
    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];

    // flush dynamic buffers

    if (frame->boundDynVbo)
    {
        if (frame->boundDynVboRangeToBeFlused)
        {
            AvxCohereMappedBuffer(frame->boundDynVbo, 0, frame->boundDynVboRangeToBeFlused, NIL, FALSE);
            frame->boundDynVboRangeToBeFlused = 0;
        }
        frame->boundDynVbo = NIL;
    }

    if (frame->boundDynIbo)
    {
        if (frame->boundDynIboRangeToBeFlused)
        {
            AvxCohereMappedBuffer(frame->boundDynIbo, 0, frame->boundDynIboRangeToBeFlused, NIL, FALSE);
            frame->boundDynIboRangeToBeFlused = 0;
        }
        frame->boundDynIbo = NIL;
    }

    for (afxUnit i = 0; i < 4; i++)
    {
        for (afxUnit j = 0; j < 8; j++)
        {
            if (frame->boundDynUbos[i][j])
            {
                if (frame->boundDynUbosRangeToBeFlused[i][j])
                {
                    AvxCohereMappedBuffer(frame->boundDynUbos[i][j], 0, frame->boundDynUbosRangeToBeFlused[i][j], NIL, FALSE);
                    frame->boundDynUbosRangeToBeFlused[i][j] = 0;
                }
                frame->boundDynUbos[i][j] = NIL;
            }
        }
    }


    ArxExecuteRenderContext(rctx);


    if (frame->internalTransferDctx && frame->internalDrawDctx)
    {
        AvxCompileDrawCommands(frame->drawDctx, frame->dbId);
        AvxCompileDrawCommands(frame->transferDctx, frame->tbId);

        avxSubmission subms[] =
        {
            {
                .dctx = frame->transferDctx,
                .batchId = frame->tbId,
            },
            {
                .dctx = frame->drawDctx,
                .batchId = frame->dbId
            }
        };
        if (AvxExecuteDrawCommands(rctx->dsys, 2, subms))
            AfxThrowError();
    }
    else if (frame->internalTransferDctx)
    {
        AvxCompileDrawCommands(frame->transferDctx, frame->tbId);

        avxSubmission subm = { .dctx = frame->transferDctx, .batchId = frame->tbId };
        if (AvxExecuteDrawCommands(rctx->dsys, 1, &subm))
            AfxThrowError();
    }
    else if(frame->internalDrawDctx)
    {
        AvxCompileDrawCommands(frame->drawDctx, frame->dbId);

        avxSubmission subm = { .dctx = frame->drawDctx, .batchId = frame->dbId };
        if (AvxExecuteDrawCommands(rctx->dsys, 1, &subm))
            AfxThrowError();
    }

    AfxDisposeObjects(1, &frame->drawDctx);
    AfxDisposeObjects(1, &frame->transferDctx);

    return err;
}

_ARX afxError _ArxRctxDtorCb(arxRenderContext rctx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    ArxDismantleBufferedPump(&rctx->dynVtxAlloc);
    ArxDismantleBufferedPump(&rctx->dynIdxAlloc);
    ArxDismantleBufferedPump(&rctx->dynUnfmAlloc);

    return err;
}

_ARX afxError _ArxRctxCtorCb(arxRenderContext rctx, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RCTX, 1, &rctx);

    arxRenderware din = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxSimulation sim = args[1];
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    arxSceneInfo const* info = AFX_CAST(arxSceneInfo const*, args[1]) + invokeNo;
    AFX_ASSERT(info);

    //arxRenderware din = info->din;
    afxDrawSystem dsys = ArxGetDrawInputContext(din); // temp workaround to get a context.

    rctx->sim = sim;
    rctx->dsys = dsys;
    rctx->din = din;

    AfxMakeArena(&rctx->cmdArena, NIL, AfxHere());
    AfxDeployChain(&rctx->commands, rctx);

    AfxDeployPool(&rctx->visualBodies, sizeof(_arxVisualBody), 1, NIL);
    AfxDeployPool(&rctx->visualMeshes, sizeof(_arxVisualMesh), 1, NIL);
    AfxDeployPool(&rctx->visualModels, sizeof(_arxVisualModel), 1, NIL);
    AfxDeployPool(&rctx->visualMaterials, sizeof(_arxVisualMaterial), 1, NIL);

    rctx->frameCnt = 3;
    rctx->frameIdx = rctx->frameCnt - 1;
    arxRenderFrame* frame = &rctx->frames[rctx->frameIdx];
    AfxZero(rctx->frames, sizeof(rctx->frames[0]) * rctx->frameCnt);
    frame->drawDctx = NIL;
    frame->transferDctx = NIL;

    ArxDeployBufferedPump(&rctx->dynVtxAlloc);
    rctx->dynVtxAlloc.bufUsage = avxBufferUsage_VERTEX;
    rctx->dynVtxAlloc.dsys = dsys;
    ArxDeployBufferedPump(&rctx->dynIdxAlloc);
    rctx->dynIdxAlloc.bufUsage = avxBufferUsage_INDEX;
    rctx->dynIdxAlloc.dsys = dsys;
    ArxDeployBufferedPump(&rctx->dynUnfmAlloc);
    rctx->dynUnfmAlloc.bufUsage = avxBufferUsage_UNIFORM;
    rctx->dynUnfmAlloc.dsys = dsys;

    return err;
}

_ARX afxClassConfig const _ARX_RCTX_CLASS_CONFIG =
{
    .fcc = afxFcc_RCTX,
    .name = "RenderContext",
    .desc = "Renderization Context",
    .fixedSiz = sizeof(AFX_OBJECT(arxRenderContext)),
    .ctor = (void*)_ArxRctxCtorCb,
    .dtor = (void*)_ArxRctxDtorCb,
};

////////////////////////////////////////////////////////////////////////////////

_ARX afxError ArxAcquireRenderContext(arxRenderware din, afxSimulation sim, afxUnit cnt, arxSceneInfo infos[], arxRenderContext* context)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    AFX_ASSERT_OBJECTS(afxFcc_SIM, 1, &sim);
    AFX_ASSERT(context);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    static afxBool rctxClsInitied = FALSE;
    static afxClass rctxClass;

    if (!rctxClsInitied)
    {
        AfxMountClass(&rctxClass, NIL, NIL, &_ARX_RCTX_CLASS_CONFIG);
        rctxClsInitied = TRUE;
    }

    afxClass* cls = (afxClass*)&rctxClass;
    AFX_ASSERT_CLASS(cls, afxFcc_RCTX);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)context, (void const*[]) { din, sim, infos }))
        AfxThrowError();

    return err;
}
