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

#ifndef ARX_RENDER_CONTEXT_H
#define ARX_RENDER_CONTEXT_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/render/arxRenderable.h"
#include "qwadro/inc/render/arxRenderableBody.h"
#include "qwadro/inc/render/arxRenderableModel.h"
#include "qwadro/inc/render/arxRenderableTerrain.h"
#include "qwadro/inc/render/arxScene.h"
#include "qwadro/inc/render/akxRenderer.h"
#include "qwadro/inc/math/coll/afxFrustum.h"

#ifdef _ARX_RENDER_CONTEXT_C


#pragma pack(push, 1)
AFX_DEFINE_STRUCT(DrawIndexedIndirectCmd)
{
    avxDrawIndexedIndirect cmd;
    afxUnit mtlDataOffset;
    afxUnit mtxStackOffset;
    afxUnit jntMapOffset;
};

AFX_DEFINE_STRUCT(StepCmd)
{
    afxUnit nodId;
    afxUnit bodId;
    afxReal dist;
};

AFX_DEFINE_STRUCT(RenderCmd)
{
    afxUnit nodId;
    afxUnit bodId;
    afxUnit prtId;
    afxUnit mdlId;
    afxUnit mshId;
    afxUnit mshsId;
    afxUnit primId;
    afxReal dist;

    avxDrawIndexedIndirect cmd;
    afxUnit mtlDataOffset;
    afxUnit mtxStackOffset;
    afxUnit jntMapOffset;
};
#pragma pack(pop)


AFX_DEFINE_STRUCT(_arxVisualMaterial)
{
    afxLink rctx;
    afxLink     technique;
    arxMaterial mtl;
    avxBuffer   dataUbo;
    afxUnit     dataUboBase;
    afxUnit     dataUboRange;
};

AFX_DEFINE_STRUCT(_arxVisualMesh)
{
    afxLink rctx;
    arxMesh msh;
    arxVertexCache vtxCache;
    arxIndexCache idxCache;
};

AFX_DEFINE_STRUCT(_arxVisualModel)
{
    afxLink     rctx;
    arxModel    mdl;
    afxUnit     rigCnt;
    afxUnit     biasMapPop; // total
    avxBuffer   biasMapFbo;
    afxUnit     biasMapFboBase;
    afxUnit     biasMapFboRange;

    avxBufferedStream sklVbo;
    avxBufferedStream sklIbo;
};

AFX_DEFINE_STRUCT(_arxVisualBody)
{
    afxLink     rctx;
    arxBody     bod;
    arxModel    mdl;
    afxUnit     jntCnt;
    struct
    {
        avxBuffer   mtxUbo;
        afxUnit     mtxUboBase;
        afxUnit     mtxUboRange;
    } perFrame[3];
};

AFX_DEFINE_STRUCT(arxRenderFrame)
{
    arxCamera activeCam;
    akxViewConstants viewConsts;

    avxBuffer viewUbo;

    arxModel activeSkl;
    afxSize biasMapFboBaseOffset;
    afxSize biasMapFboBaseIdx;
    afxSize mtxStackUboBaseOffset;
    afxSize mtlDataUboBaseOffset;

    afxBool internalTransferDctx;
    afxDrawContext transferDctx;
    afxBool internalDrawDctx;
    afxDrawContext drawDctx;
    afxUnit dbId, tbId;

    avxBuffer boundDynVbo;
    afxUnit boundDynVboRangeToBeFlused;
    avxBuffer boundDynIbo;
    afxUnit boundDynIboRangeToBeFlused;
    avxBuffer boundDynUbos[4][8];
    afxUnit boundDynUbosRangeToBeFlused[4][8];
};

AFX_OBJECT(arxRenderContext)
{
    afxSimulation sim;
    afxDrawSystem   dsys;
    arxRenderware    din;

    afxChain        commands;
    afxArena        cmdArena; // owned by dsys data for specific port

    afxPool visualMaterials;
    afxPool visualMeshes;
    afxPool visualModels;
    afxPool visualBodies;

    afxBool inFrameGoingOn;
    afxUnit frameIdx;
    afxUnit frameCnt;
    arxRenderFrame frames[3];
    afxUnit perSecFrame;

    arxBufferedPump dynVtxAlloc;
    arxBufferedPump dynIdxAlloc;
    arxBufferedPump dynUnfmAlloc;
};
#endif//_ARX_RENDER_CONTEXT_C

ARX afxError ArxAcquireRenderContext(arxRenderware din, afxSimulation sim, afxUnit cnt, arxSceneInfo infos[], arxRenderContext* context);

ARX afxError ArxStageMaterials(arxRenderContext rctx, arxGeome morp, afxUnit cnt, afxUnit indices[]);

ARX afxBool RenderCells(arxRenderContext rctx, afxDrawContext dctx, avxVertexInput vin, arxTerrain ter, afxFrustum* frustum, afxBool showFaces, afxBool showDbgLines);

ARX afxError ArxCmdRenderMeshes(arxRenderContext rctx, afxUnit cnt, arxMesh meshes[]);
ARX afxError ArxCmdRenderModels(arxRenderContext rctx, afxUnit cnt, arxModel models[]);
ARX afxError ArxCmdRenderBodies(arxRenderContext rctx, afxM4d m, afxUnit cnt, arxBody bodies[]);
ARX afxError ArxCmdRenderNode(arxRenderContext rctx, afxUnit cnt, afxNode nodes[]);
ARX afxError ArxCmdRenderTerrain(arxRenderContext rctx, arxTerrain ter);

/*
    Start of a new rendering frame.
    Prepares the system for rendering a new frame.

    ArxBeginFrame() is called to mark the start of frame rendering. 
    The application should still call ArxBeginFrame() but omit rendering work for the frame if shouldRender is FALSE.

    Runtimes must not perform frame synchronization or throttling through the ArxBeginFrame() function and should instead do so through ArxWaitFrame().
*/

ARX afxError ArxBeginFrame(arxRenderContext rctx, void* param, afxUnit* frameIdx);

/*
    Finalize the frame and present it to the screen (swap buffers).

    Every call to ArxEndFrame() must be preceded by a successful call to ArxBeginFrame().

*/

// Completes the frame. Executes buffer swaps and ensures the rendered scene is displayed.

ARX afxError ArxEndFrame(arxRenderContext rctx, void* param, afxUnit* perSecFrame);

// ArxWaitFrame throttles the application frame loop in order to synchronize application frame submissions with the display.

ARX afxError ArxWaitFrame(arxRenderContext rctx, void* param);

// Set up the scene for rendering (e.g., camera, lighting).
// Starts the definition of a scene. Typically, you would place draw calls and object updates between BeginScene and EndScene.

ARX afxError ArxBeginScene(arxRenderContext rctx, afxDrawContext transferDctx, afxDrawContext drawDctx);

// Finish setting up the scene (submit draw calls).
// Ends the definition of the current scene.
// Internally may batch and sort draw calls, set states, etc.

ARX afxError ArxEndScene(arxRenderContext rctx, afxUnit id);

ARX afxError ArxCmdUseRenderTechnique(arxRenderContext rctx, arxTechnique dtec, afxUnit passId, avxVertexInput vin, afxFlags dynamics);
ARX afxError ArxCmdUseCamera(arxRenderContext rctx, arxCamera cam, afxRect const* drawArea);

ARX afxError ArxCmdPostUniform(arxRenderContext rctx, afxUnit set, afxUnit binding, afxUnit dataSiz, void** pData);
ARX afxError ArxCmdPostVertices(arxRenderContext rctx, afxUnit vtxCnt, afxUnit vtxSiz, void** pVertices);
ARX afxError ArxCmdPostVertexIndices(arxRenderContext rctx, afxUnit idxCnt, afxUnit idxSiz, void** pIndices);

#endif//ARX_RENDER_CONTEXT_H
