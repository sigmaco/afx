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

#ifndef ARX_RENDER_CONTEXT_H
#define ARX_RENDER_CONTEXT_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/sim/render/arxRenderable.h"
#include "qwadro/inc/sim/render/arxRenderableBody.h"
#include "qwadro/inc/sim/render/arxRenderableModel.h"
#include "qwadro/inc/sim/render/arxRenderableTerrain.h"
#include "qwadro/inc/sim/io/afxScene.h"
#include "qwadro/inc/sim/akxRenderer.h"

AFX_DEFINE_STRUCT(akxViewConstants) // frame
{
    afxM4d  v;
    afxM4d  iv;
    afxM4d  p;
    afxM4d  ip;
    afxM4d  pv;
    afxM4d  ipv;

    afxV4d  viewPos; // view point
    afxV2d  viewExtent;

    afxV3d starDir;
    afxV4d starPos;
    afxV4d starKd;
};

AFX_DEFINE_HANDLE(arxRenderContext);

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
    afxMaterial mtl;
    avxBuffer   dataUbo;
    afxUnit     dataUboBase;
    afxUnit     dataUboRange;
};

AFX_DEFINE_STRUCT(_arxVisualMesh)
{
    afxLink rctx;
    afxMesh msh;
    avxVertexCache vtxCache;
    avxIndexCache idxCache;
};

AFX_DEFINE_STRUCT(_arxVisualModel)
{
    afxLink     rctx;
    afxModel    mdl;
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
    afxBody     bod;
    afxModel    mdl;
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
    avxCamera activeCam;
    akxViewConstants viewConsts;

    avxBuffer viewUbo;

    afxModel activeSkl;
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
    afxDrawInput    din;

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

    avxBufferedPump dynVtxAlloc;
    avxBufferedPump dynIdxAlloc;
    avxBufferedPump dynUnfmAlloc;
};
#endif//_ARX_RENDER_CONTEXT_C

ARX afxError ArxAcquireRenderContext(afxSimulation sim, afxUnit cnt, afxSceneInfo infos[], arxRenderContext* context);

ARX afxError ArxStageMaterials(arxRenderContext rctx, afxMorphology morp, afxUnit cnt, afxUnit indices[]);

ASX afxBool RenderCells(arxRenderContext rctx, afxDrawContext dctx, avxVertexInput vin, afxTerrain ter, afxFrustum* frustum, afxBool showFaces, afxBool showDbgLines);

ARX afxError ArxCmdRenderMeshes(arxRenderContext rctx, afxUnit cnt, afxMesh meshes[]);
ARX afxError ArxCmdRenderModels(arxRenderContext rctx, afxUnit cnt, afxModel models[]);
ARX afxError ArxCmdRenderBodies(arxRenderContext rctx, afxM4d m, afxUnit cnt, afxBody bodies[]);
ARX afxError ArxCmdRenderNode(arxRenderContext rctx, afxUnit cnt, afxNode nodes[]);
ARX afxError ArxCmdRenderTerrain(arxRenderContext rctx, afxTerrain ter);

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

ARX afxError ArxCmdUseRenderTechnique(arxRenderContext rctx, afxDrawTechnique dtec, afxUnit passId, avxVertexInput vin, afxFlags dynamics);
ARX afxError ArxCmdUseCamera(arxRenderContext rctx, avxCamera cam, afxRect const* drawArea);

ARX afxError ArxCmdPostUniform(arxRenderContext rctx, afxUnit set, afxUnit binding, afxUnit dataSiz, void** pData);
ARX afxError ArxCmdPostVertices(arxRenderContext rctx, afxUnit vtxCnt, afxUnit vtxSiz, void** pVertices);
ARX afxError ArxCmdPostVertexIndices(arxRenderContext rctx, afxUnit idxCnt, afxUnit idxSiz, void** pIndices);

#endif//ARX_RENDER_CONTEXT_H
