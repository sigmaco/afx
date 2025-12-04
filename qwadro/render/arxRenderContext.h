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

// This file is part of Acceleration for RenderWare on Qwadro.

#ifndef ARX_DRAW_INPUT_H
#define ARX_DRAW_INPUT_H

#include "qwadro/sim/arxSimDefs.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/render/arxRenderable.h"
#include "qwadro/render/arxRenderBody.h"
#include "qwadro/render/arxRenderModel.h"
#include "qwadro/render/arxRenderTerrain.h"
#include "qwadro/render/arxWireframe.h"
#include "qwadro/scene/arxSky.h"
#include "qwadro/render/arxRenderer.h"
#include "qwadro/scene/arxBuffer.h"
#include "qwadro/coll/afxFrustum.h"

// RenderLayer and Scene are currently available to help organize this mess.

typedef enum arxSceneMode
{
    // Classic wireframe mode; drawing lines.
    arxSceneMode_WIRE       = AFX_BITMASK(0),
    // Modern wireframe mode; drawing line-shaped faces.
    arxSceneMode_WIRE_BARY  = AFX_BITMASK(1),
    // Classic solid mode; drawing faces.
    arxSceneMode_FACE       = AFX_BITMASK(10),
    arxSceneMode_TEXTURED   = AFX_BITMASK(11),
    arxSceneMode_COLORED    = AFX_BITMASK(12),
} arxSceneMode;

AFX_DEFINE_STRUCT(arxRenderConfiguration)
{
    afxUnit         frameCnt;
    arxSimulation   sim;

    void*           udd;
    afxString       tag;
};

ARX afxError ArxConfigureRenderContext
(
    arxScenario scio, 
    arxRenderConfiguration* rcfg
);

ARX afxError ArxAcquireRenderContext
(
    arxScenario scio, 
    arxRenderConfiguration const* rcfg, 
    arxRenderContext* context
);

////////////////////////////////////////////////////////////////////////////////

ARX afxDrawSystem ArxGetRenderDrawSystem
(
    arxRenderContext rctx
);


ARX void* ArxGetRenderContextUdd
(
    arxRenderContext rctx
);

ARX afxError ArxStageMaterials
(
    arxRenderContext rctx, 
    arxMtd mtd, 
    afxUnit cnt, 
    afxUnit indices[]
);

ARX afxError ArxUseMaterials
(
    arxRenderContext rctx, 
    afxUnit cnt, 
    afxUnit indices[]
);

ARX afxBool RenderCells
(
    arxRenderContext rctx, 
    afxDrawContext dctx, 
    avxVertexInput vin, 
    arxTerrain ter, 
    afxFrustum* frustum, 
    afxBool showFaces, 
    afxBool showDbgLines
);

ARX afxError ArxCmdRenderMeshes
(
    arxRenderContext rctx, 
    afxUnit cnt, 
    arxMesh meshes[]
);

ARX afxError ArxCmdRenderModels
(
    arxRenderContext rctx, 
    afxUnit cnt, 
    arxModel models[]
);

ARX afxError ArxCmdRenderBodies
(
    arxRenderContext rctx, 
    afxM4d m, 
    afxUnit cnt, 
    arxPuppet bodies[]
);

ARX afxError ArxCmdRenderNode
(
    arxRenderContext rctx, 
    afxUnit cnt, 
    arxNode nodes[]
);

ARX afxError ArxCmdRenderTerrain
(
    arxRenderContext rctx, 
    arxTerrain ter
);

AFX_DEFINE_STRUCT(arxNextFrameInfo)
{
    afxUnit64   predictedDisplayTime;
    afxUnit64   predictedDisplayPeriod;
    afxBool     shouldRender;
    afxUnit64   frameId;
    afxUnit     frameIdx;
    avxFence    frameReady;
    afxUnit64   frameReadyNextValue;
    avxFence    frameAvail;
    afxUnit64   frameAvailNextValue;
};

// ArxWaitFrame throttles the application frame loop in order to synchronize application frame submissions with the display.

ARX afxError ArxWaitFrame
(
    arxRenderContext rctx,
    void* param,
    arxNextFrameInfo* nextInfo
);

/*
    Start of a new rendering frame.
    Prepares the system for rendering a new frame.

    ArxBeginFrame() is called to mark the start of frame rendering. 
    The application should still call ArxBeginFrame() but omit rendering work for the frame if shouldRender is FALSE.

    Runtimes must not perform frame synchronization or throttling through the ArxBeginFrame() function and should instead do so through ArxWaitFrame().
*/

ARX afxError ArxBeginFrame
(
    arxRenderContext rctx, 
    void* param
);

/*
    Finalize the frame and present it to the screen (swap buffers).

    Every call to ArxEndFrame() must be preceded by a successful call to ArxBeginFrame().

*/

// Completes the frame. Executes buffer swaps and ensures the rendered scene is displayed.

ARX afxError ArxEndFrame
(
    arxRenderContext rctx, 
    void* param, 
    afxUnit* perSecFrame
);

ARX afxError ArxCancelFrame
(
    arxRenderContext rctx
);

ARX afxError ArxAdvanceFrame
(
    arxRenderContext rctx,
    afxUnit* nextFrameIdx
);

AFX_DEFINE_STRUCT(arxSceneScope)
{
    afxFlags    flags;
};

// Set up the scene for rendering (e.g., camera, lighting).
// Starts the definition of a scene. Typically, you would place draw calls and object updates between BeginScene and EndScene.

ARX afxError ArxBeginScene
(
    arxRenderContext rctx, 
    afxDrawContext transferDctx, 
    afxDrawContext drawDctx
);

// Finish setting up the scene (submit draw calls).
// Ends the definition of the current scene.
// Internally may batch and sort draw calls, set states, etc.

ARX afxError ArxEndScene
(
    arxRenderContext rctx, 
    afxUnit id
);

ARX afxError ArxAdvanceSceneLayer
(
    arxRenderContext rctx, 
    arxSceneMode mode
);

ARX afxError ArxUseRenderTechnique
(
    arxRenderContext rctx, 
    arxTechnique dtec, 
    afxUnit passId, 
    avxVertexInput vin, 
    afxFlags dynamics
);

ARX afxError ArxUseCamera
(
    arxRenderContext rctx, 
    arxCamera cam, 
    afxRect const* drawArea
);

ARX afxError ArxPostConstants
(
    arxRenderContext rctx, 
    afxUnit offset, 
    afxUnit siz, 
    void const* value
);

/*
    The ArxPostUniform() function uploads a block of uniform data and binds it to a specified descriptor set/binding.
    Automatically handles allocation, copy, and bind. No need for user to handle buffer pointers manually if @src is specified,
    which also handles stride between elements; useful for posting arrays of structures with padding.
    @src can be NULL if you just want to reserve space and write manually.

    Usage:
    {
        typedef struct { mat4 model; vec4 color; } UniformBlock;

        UniformBlock ubo = ...;

        void* dst = ArxPostUniform(rctx, 0, 1, sizeof(ubo), &ubo, sizeof(ubo));
    }
*/

ARX void*   ArxPostUniform
// Returns an output pointer to write data into the mapped buffer.
(
    // Drawing context with per-frame data.
    arxRenderContext rctx, 
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
    The ArxPostVertices() function uploads vertex data to a dynamic vertex buffer for the current frame.
    @vtxCnt * @vtxSiz bytes will be copied from @src (with srcStride for each vertex).
    Returns a pointer to the mapped buffer region (if user wants to write manually).

    By specifying a @src, it handles interleaved or padded vertex arrays, simplifies dynamic geometry uploads 
    (for example, particle systems, UI quads), and avoids manual buffer management.

    Usage:
    {
        typedef struct { vec3 pos; vec3 normal; vec2 uv; } Vertex;

        Vertex vertices[100] = ...;

        ArxPostVertices(rctx, 100, sizeof(Vertex), vertices, sizeof(Vertex));
    }
*/

ARX void*   ArxPostVertices
(
    // Drawing context with per-frame data.
    arxRenderContext rctx, 
    afxUnit vtxCnt, 
    afxUnit vtxSiz, 
    void const* src, 
    afxUnit srcStride
);

/*
    The ArxPostVertexIndices() function uploads vertex index data to a dynamic vertex index buffer for the current frame.
    @idxCnt * @idxSiz bytes will be copied from @src (with srcStride for each vertex index).
    Returns a pointer to the mapped buffer region (if user wants to write manually).
    @srcStride allows for tightly or loosely packed indices.
*/

ARX void*   ArxPostVertexIndices
(
    // Drawing context with per-frame data.
    arxRenderContext rctx, 
    afxUnit idxCnt, 
    afxUnit idxSiz, 
    void const* src, 
    afxUnit srcStride
);

#endif//ARX_DRAW_INPUT_H
