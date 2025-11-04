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

// This file is part of Advanced Renderware Extensions & Experiments for Qwadro.

#ifndef ARX_DRAW_INPUT_H
#define ARX_DRAW_INPUT_H

#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/render/arxRenderable.h"
#include "qwadro/render/arxRenderBody.h"
#include "qwadro/render/arxRenderModel.h"
#include "qwadro/render/arxRenderTerrain.h"
#include "qwadro/render/arxSky.h"
#include "qwadro/render/arxRenderer.h"
#include "qwadro/render/arxBuffer.h"
#include "qwadro/coll/afxFrustum.h"

// RenderLayer and Scene are currently available to help organize this mess.

#ifdef _ARX_RENDER_CONTEXT_C


#pragma pack(push, 1)
AFX_DEFINE_STRUCT(DrawIndexedIndirectCmd)
{
    avxDrawIndexedIndirect cmd;
    afxUnit jntMtxStackBase;
    afxUnit jntMapBase;
    afxUnit mtlStackBase;
    afxUnit mtlMapBase;
    afxUnit mtlIdx;
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


AFX_DEFINE_STRUCT(_arxVisualMtd)
{
    afxLink     din;
    afxLink     technique;
    arxMtd      mtd;
    avxBuffer   mtlUbo;
    afxUnit     mtlUboBase;
    afxUnit     mtlUboRange;
};

AFX_DEFINE_STRUCT(_arxVisualMesh)
{
    afxLink din;
    arxMesh msh;
    arxVertexCache vtxCache;
    arxIndexCache idxCache;
};

AFX_DEFINE_STRUCT(_arxVisualModel)
{
    afxLink     din;
    arxModel    mdl;
    afxUnit     rigCnt;
    // mapping between the rigged meshes' bias and skeleton joints.
    afxUnit     biasMapPop; // total
    avxBuffer   biasMapFbo;
    afxUnit     biasMapFboBase;
    afxUnit     biasMapFboRange;
    // mapping between rigged meshes' materials and the MTD.
    afxUnit     mtlMapPop; // total
    avxBuffer   mtlMapFbo;
    afxUnit     mtlMapFboBase;
    afxUnit     mtlMapFboRange;

    avxBufferedStream sklVbo;
    avxBufferedStream sklIbo;
};

AFX_DEFINE_STRUCT(_arxVisualBody)
{
    afxLink     din;
    arxBody     bod;
    arxModel    mdl;
    afxUnit     boneCnt;
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
    afxSize mtxStackBaseOffset;
    afxSize mtxStackBaseIdx;
    afxSize mtlStackBaseOffset;
    afxSize mtlStackBaseIdx;
    afxSize mtlMapFboBaseOffset;
    afxSize mtlMapFboBaseIdx;

    afxBool internalTransferDctx;
    afxDrawContext transferDctx;
    afxBool internalDrawDctx;
    afxDrawContext drawDctx;
    afxUnit dbId, tbId;

    avxBuffer boundDynVbo;
    afxUnit boundDynVboBaseToBeFlushed;
    afxUnit boundDynVboRangeToBeFlushed;
    avxBuffer boundDynIbo;
    afxUnit boundDynIboBaseToBeFlushed;
    afxUnit boundDynIboRangeToBeFlushed;
    avxBuffer boundDynUbos[4][32];
    afxUnit boundDynUbosBaseToBeFlushed[4][32];
    afxUnit boundDynUbosRangeToBeFlushed[4][32];
};

AFX_OBJECT(arxDrawInput)
{
    arxSimulation sim;
    afxDrawSystem   dsys;
    arxRenderContext    rctx;

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

ARX afxError ArxAcquireDrawInput(arxRenderContext rctx, arxSimulation sim, afxUnit cnt, arxSkyInfo infos[], arxDrawInput* input);

ARX afxError ArxStageMaterials(arxDrawInput din, arxMtd mtd, afxUnit cnt, afxUnit indices[]);

ARX afxError ArxUseMaterials(arxDrawInput din, afxUnit cnt, afxUnit indices[]);

ARX afxBool RenderCells(arxDrawInput din, afxDrawContext dctx, avxVertexInput vin, arxTerrain ter, afxFrustum* frustum, afxBool showFaces, afxBool showDbgLines);

ARX afxError ArxCmdRenderMeshes(arxDrawInput din, afxUnit cnt, arxMesh meshes[]);
ARX afxError ArxCmdRenderModels(arxDrawInput din, afxUnit cnt, arxModel models[]);
ARX afxError ArxCmdRenderBodies(arxDrawInput din, afxM4d m, afxUnit cnt, arxBody bodies[]);
ARX afxError ArxCmdRenderNode(arxDrawInput din, afxUnit cnt, arxNode nodes[]);
ARX afxError ArxCmdRenderTerrain(arxDrawInput din, arxTerrain ter);

/*
    Start of a new rendering frame.
    Prepares the system for rendering a new frame.

    ArxBeginFrame() is called to mark the start of frame rendering. 
    The application should still call ArxBeginFrame() but omit rendering work for the frame if shouldRender is FALSE.

    Runtimes must not perform frame synchronization or throttling through the ArxBeginFrame() function and should instead do so through ArxWaitFrame().
*/

ARX afxError ArxBeginFrame(arxDrawInput din, void* param, afxUnit* frameIdx);

/*
    Finalize the frame and present it to the screen (swap buffers).

    Every call to ArxEndFrame() must be preceded by a successful call to ArxBeginFrame().

*/

// Completes the frame. Executes buffer swaps and ensures the rendered scene is displayed.

ARX afxError ArxEndFrame(arxDrawInput din, void* param, afxUnit* perSecFrame);

// ArxWaitFrame throttles the application frame loop in order to synchronize application frame submissions with the display.

ARX afxError ArxWaitFrame(arxDrawInput din, void* param);

AFX_DEFINE_STRUCT(arxSceneScope)
{
    afxFlags    flags;
};

// Set up the scene for rendering (e.g., camera, lighting).
// Starts the definition of a scene. Typically, you would place draw calls and object updates between BeginScene and EndScene.

ARX afxError ArxBeginScene(arxDrawInput din, afxDrawContext transferDctx, afxDrawContext drawDctx);

// Finish setting up the scene (submit draw calls).
// Ends the definition of the current scene.
// Internally may batch and sort draw calls, set states, etc.

ARX afxError ArxEndScene(arxDrawInput din, afxUnit id);

ARX afxError ArxCmdUseRenderTechnique(arxDrawInput din, arxTechnique dtec, afxUnit passId, avxVertexInput vin, afxFlags dynamics);
ARX afxError ArxCmdUseCamera(arxDrawInput din, arxCamera cam, afxRect const* drawArea);

/*
    The ArxCmdPostUniform() function uploads a block of uniform data and binds it to a specified descriptor set/binding.
    Automatically handles allocation, copy, and bind. No need for user to handle buffer pointers manually if @src is specified,
    which also handles stride between elements; useful for posting arrays of structures with padding.
    @src can be NULL if you just want to reserve space and write manually.

    Usage:
    {
        typedef struct { mat4 model; vec4 color; } UniformBlock;

        UniformBlock ubo = ...;

        void* dst = ArxCmdPostUniform(din, 0, 1, sizeof(ubo), &ubo, sizeof(ubo));
    }
*/

ARX void*   ArxCmdPostUniform
// Returns an output pointer to write data into the mapped buffer.
(
    // Drawing context with per-frame data.
    arxDrawInput din, 
    // Descriptor set index (shader-level).
    afxUnit set, 
    // Binding index within descriptor set.
    afxUnit binding, 
    // The number of the arrayed data entries in uniform block to post.
    afxUnit unitCnt,
    // Size of the uniform data to post.
    afxUnit dataSiz, 
    // A pointer to source data to be copied into the uniform buffer.
    void const* src, 
    // The stride to be used to copy data from source into the uniform buffer.
    afxUnit srcStride
);

/*
    The ArxCmdPostVertices() function uploads vertex data to a dynamic vertex buffer for the current frame.
    @vtxCnt * @vtxSiz bytes will be copied from @src (with srcStride for each vertex).
    Returns a pointer to the mapped buffer region (if user wants to write manually).

    By specifying a @src, it handles interleaved or padded vertex arrays, simplifies dynamic geometry uploads 
    (for example, particle systems, UI quads), and avoids manual buffer management.

    Usage:
    {
        typedef struct { vec3 pos; vec3 normal; vec2 uv; } Vertex;

        Vertex vertices[100] = ...;

        ArxCmdPostVertices(din, 100, sizeof(Vertex), vertices, sizeof(Vertex));
    }
*/

ARX void*   ArxCmdPostVertices
(
    // Drawing context with per-frame data.
    arxDrawInput din, 
    afxUnit vtxCnt, 
    afxUnit vtxSiz, 
    void const* src, 
    afxUnit srcStride
);

/*
    The ArxCmdPostVertexIndices() function uploads vertex index data to a dynamic vertex index buffer for the current frame.
    @idxCnt * @idxSiz bytes will be copied from @src (with srcStride for each vertex index).
    Returns a pointer to the mapped buffer region (if user wants to write manually).
    @srcStride allows for tightly or loosely packed indices.
*/

ARX void*   ArxCmdPostVertexIndices
(
    // Drawing context with per-frame data.
    arxDrawInput din, 
    afxUnit idxCnt, 
    afxUnit idxSiz, 
    void const* src, 
    afxUnit srcStride
);

#endif//ARX_DRAW_INPUT_H
