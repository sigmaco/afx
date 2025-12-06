/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_CONTEXT_DDK_H
#define AVX_CONTEXT_DDK_H

#include "avxFenceDDK.h"

typedef enum avxContextState
/// Each draw context is always in one of the following states
{
    // When a draw context is allocated, it is in the initial state.
    // Some commands are able to reset a draw context (or a set of command buffers) back to this state from any of the executable, recording or invalid state.
    // Command buffers in the initial state canv only be moved to the recording state, or freed.
    avxContextState_INITIAL,

    // BeginCmdBuffer() changes the state of a draw context from the initial state to the recording state.
    // Once a draw context is in the recording state, AvxCmd* commands canv be used to record to the draw context.
    avxContextState_RECORDING,

    // AfxCompileCmdBuffer() ends the recording of a draw context, and moves it from the recording state to the executable state.
    // Executable command buffers canv be submitted, reset, or recorded to another draw context.
    avxContextState_EXECUTABLE,

    // Queue submission of a draw context changes the state of a draw context from the executable state to the pending state.
    // Whilst in the pending state, applications must not attempt to modify the draw context in any way - as the device may be processing the commands recorded to it.
    // Once execution of a draw context completes, the draw context either reverts back to the executable state, or if it was recorded with ONCE flag, it moves to the invalid state.
    // A synchronization command should be used to detect when this occurs.
    avxContextState_PENDING,

    // Some operations, such as modifying or deleting a resource that was used in a command recorded to a draw context, will transition the state of that draw context into the invalid state.
    // Command buffers in the invalid state canv only be reset or freed.
    avxContextState_INVALID,

    avxContextState_INTERNAL_EXECUTING,
} avxContextState;

AFX_DEFINE_STRUCT(_avxCmdHdr)
{
    afxLink script;
    afxUnit id;
    afxUnit siz;
};

AFX_DEFINE_UNION(_avxCmd)
{
    _avxCmdHdr hdr;

    // BUFFER OPERATIONS

    struct
    {
        _avxCmdHdr hdr;

        avxBuffer src;
        avxBuffer dst;
        afxUnit opCnt;
        avxBufferCopy AFX_SIMD ops[];
    } CopyBuffer;
    struct
    {
        _avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit offset;
        afxUnit range;
        afxUnit value;
    } FillBuffer;
    struct
    {
        _avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit offset;
        afxUnit range;
        afxByte AFX_SIMD data[];
    } UpdateBuffer;

    struct
    {
        _avxCmdHdr hdr;

        avxRaster ras;
        avxBuffer buf;
        afxUnit opCnt;
        avxRasterIo AFX_SIMD ops[];
    } PackRaster;
    struct
    {
        _avxCmdHdr hdr;

        avxRaster ras;
        avxBuffer buf;
        afxUnit opCnt;
        avxRasterIo AFX_SIMD ops[];
    } UnpackRaster;
    struct
    {
        _avxCmdHdr hdr;

        avxRaster src;
        avxRaster dst;
        afxUnit opCnt;
        avxRasterCopy AFX_SIMD ops[];
    } CopyRaster;
    struct
    {
        _avxCmdHdr hdr;

        afxDrawContext dctx;
        avxRaster src;
        avxRaster dst;
        avxTexelFilter flt;
        afxUnit opCnt;
        avxRasterBlit AFX_SIMD ops[];
    } BlitRaster;
    struct
    {
        _avxCmdHdr hdr;

        afxDrawContext dctx;
        avxRaster src;
        avxRaster dst;
        afxUnit opCnt;
        avxRasterCopy AFX_SIMD ops[];
    } ResolveRaster;
    struct
    {
        _avxCmdHdr hdr;

        afxDrawContext dctx;
        avxRaster ras;
        avxClearValue value;
        afxUnit baseLod;
        afxUnit lodCnt;
        afxUnit baseLayer;
        afxUnit layerCnt;
        //afxUnit opCnt;
        //avxRasterBlock AFX_SIMD ops[];
    } ClearRaster;
    struct
    {
        _avxCmdHdr hdr;

        afxFlags flags;
        afxUnit rasCnt;
        avxRaster AFX_SIMD rasters[];
    } RegenerateMipmapsSIGMA;

    // GENERAL OPERATIONS
    struct
    {
        _avxCmdHdr hdr;

        avxBus bus;
        avxPipeline pip;
        avxVertexInput vin;
        afxFlags dynamics;
        afxFlags flags;
    } BindPipeline;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit cnt;
        struct
        {
            avxShaderType stage;
            avxCodebase shd;
        } AFX_SIMD stages[];
    } BindShadersEXT;
    struct
    {
        _avxCmdHdr hdr;

        avxBus bus;
        avxLigature liga;
        afxFlags flags;
    } UseLigature;
    struct
    {
        _avxCmdHdr hdr;

        avxBus bus;
        afxUnit set;
        afxUnit pin;
        afxUnit cnt;
        avxBufferedMap AFX_SIMD maps[];
    } BindBuffers;
    struct
    {
        _avxCmdHdr hdr;

        avxBus bus;
        afxUnit set;
        afxUnit pin;
        afxUnit cnt;
        avxRaster AFX_SIMD rasters[];
    } BindRasters;
    struct
    {
        _avxCmdHdr hdr;

        avxBus bus;
        afxUnit set;
        afxUnit pin;
        afxUnit cnt;
        avxSampler AFX_SIMD samplers[];
    } BindSamplers;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit cnt;
        struct
        {
            afxDrawContext dctx;
            afxUnit batchId;
        } AFX_SIMD contexts[];
    } ExecuteCommands;
    struct
    {
        _avxCmdHdr hdr;

        avxDrawIndirect data;
    } Draw;
    struct
    {
        _avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        afxUnit32 drawCnt;
        afxUnit32 stride;
    } DrawIndirect;
    struct
    {
        _avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        avxBuffer cntBuf;
        afxUnit32 cntBufOff;
        afxUnit32 maxDrawCnt;
        afxUnit32 stride;
    } DrawIndirect2;
    struct
    {
        _avxCmdHdr hdr;

        avxDrawIndexedIndirect data;
    } DrawIndexed;
    struct
    {
        _avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        afxUnit32 drawCnt;
        afxUnit32 stride;
    } DrawIndexedIndirect;
    struct
    {
        _avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        avxBuffer cntBuf;
        afxUnit32 cntBufOff;
        afxUnit32 maxDrawCnt;
        afxUnit32 stride;
    } DrawIndexedIndirect2;
    struct
    {
        _avxCmdHdr hdr;

        avxDispatchIndirect data;
    } Dispatch;
    struct
    {
        _avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
    } DispatchIndirect;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit offset;
        afxUnit siz;
        afxByte AFX_SIMD data[];
    } PushConstants;
    struct
    {
        _avxCmdHdr hdr;

        afxM4d  v;
        afxV2d  at;
        afxUnit len;
        afxByte AFX_SIMD data[];
    } StampDebug;

    // TRANSFORMATION OPERATIONS

    struct
    {
        _avxCmdHdr hdr;

        avxVertexInput vin;
    } UseVertexInput;
    struct
    {
        _avxCmdHdr hdr;

        afxBool cw;
    } SwitchFrontFace;
    struct
    {
        _avxCmdHdr hdr;

        avxCullMode mode;
    } SetCullMode;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit baseIdx;
        afxUnit cnt;
        avxViewport AFX_SIMD viewports[];
    } AdjustViewports;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit basePin;
        afxUnit cnt;
        avxBufferedStream AFX_SIMD src[];
    } BindVertexBuffers;
    struct
    {
        _avxCmdHdr hdr;

        avxBuffer buf;
        afxUnit32 offset;
        afxUnit32 range;
        afxUnit32 idxSiz;
    } BindIndexBuffer;
    struct
    {
        _avxCmdHdr hdr;

        avxTopology topology;
    } SetPrimitiveTopology;

    // RASTERIZATION OPERATIOS

    struct
    {
        _avxCmdHdr hdr;
        
        avxFillMode mode;
    } SetFillModeEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit firstCounterBuffer;
        afxUnit counterBufferCount;
        struct
        {
            avxBuffer cntBuf;
            afxSize cntBufOff;
        } srcs[];
    } BeginTransformFeedbackEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit firstCntBuf;
        afxUnit cnterBufCnt;
        struct
        {
            avxBuffer cntBuf;
            afxSize cntBufOff;
        } srcs[];
    } EndTransformFeedbackEXT;
    struct
    {
        _avxCmdHdr hdr;

        avxQueryPool qryp;
        afxUnit query;
        afxBool precise;
        afxUnit index;
    } BeginQueryIndexedEXT;
    struct
    {
        _avxCmdHdr hdr;

        avxQueryPool qryp;
        afxUnit query;
        afxUnit index;
    } EndQueryIndexedEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit firstBinding;
        afxUnit bindingCount;
        struct
        {
            avxBuffer buf;
            afxSize offset;
            afxSize range;
        } srcs[];
    } BindTransformFeedbackBuffersEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit instCnt;
        afxUnit firstInst;
        avxBuffer cnterBuf;
        afxSize cnterBufOffset;
        afxUnit cnterOffset;
        afxUnit vtxStride;
    } DrawIndirectByteCountEXT;
    struct
    {
        _avxCmdHdr hdr;

        // The number of local workgroups to dispatch in the X dimension.
        afxUnit w;
        // The number of local workgroups to dispatch in the Y dimension.
        afxUnit h;
        // The number of local workgroups to dispatch in the Z dimension.
        afxUnit d;
    } DrawMeshTasksEXT;
    struct
    {
        _avxCmdHdr hdr;

        // The buffer containing draw parameters.
        avxBuffer buf;
        // The byte offset into buffer where parameters begin.
        afxSize offset;
        // The number of draws to execute, and can be zero.
        afxUnit drawCnt;
        // The byte stride between successive sets of draw parameters.
        afxUnit stride;
    } DrawMeshTasksIndirectEXT;
    struct
    {
        _avxCmdHdr hdr;

        // The buffer containing draw parameters.
        avxBuffer buf;
        // The byte offset into buffer where parameters begin.
        afxSize offset;
        // The buffer containing the draw count.
        avxBuffer cntBuf;
        // The byte offset into countBuffer where the draw count begins.
        afxSize cntBufOffset;
        // The maximum number of draws that will be executed. 
        // The actual number of executed draw calls is the minimum of the count specified in @cntBuf and @maxDrawCnt.
        afxUnit maxDrawCnt;
        // The byte stride between successive sets of draw parameters.
        afxUnit stride;
    } DrawMeshTasksIndirectCountEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxBool enable;
    } EnableDepthBias;
    struct
    {
        _avxCmdHdr hdr;

        afxReal constFactor;
        afxReal clamp;
        afxReal slopeFactor;
    } SetDepthBias;
    struct
    {
        _avxCmdHdr hdr;

        afxReal lineWidth;
    } SetLineWidth;
    struct
    {
        _avxCmdHdr hdr;

        afxBool disable;
    } DisableRasterization;
    struct
    {
        _avxCmdHdr hdr;

        afxBool enable;
    } EnableDepthTest;
    struct
    {
        _avxCmdHdr hdr;

        avxCompareOp compareOp;
    } SetDepthCompareOp;
    struct
    {
        _avxCmdHdr hdr;

        afxBool disable;
    } DisableDepthWrite;
    struct
    {
        _avxCmdHdr hdr;

        afxBool enable;
    } EnableStencilTest;
    struct
    {
        _avxCmdHdr hdr;

        avxFaceMask faceMask;
        afxMask compareMask;
    } SetStencilCompareMask;
    struct
    {
        _avxCmdHdr hdr;

        avxFaceMask faceMask;
        afxMask writeMask;
    } SetStencilWriteMask;
    struct
    {
        _avxCmdHdr hdr;

        avxFaceMask faceMask;
        afxUnit32 reference;
    } SetStencilReference;
    struct
    {
        _avxCmdHdr hdr;

        avxFaceMask faceMask;
        avxStencilOp failOp;
        avxStencilOp passOp;
        avxStencilOp depthFailOp;
        avxCompareOp compareOp;
    } SetStencilOp;
    struct
    {
        _avxCmdHdr hdr;

        afxBool enable;
    } EnableDepthBoundsTest;
    struct
    {
        _avxCmdHdr hdr;

        afxV2d bounds;
    } SetDepthBounds;
    struct
    {
        _avxCmdHdr hdr;

        afxV2d blendContants;
    } SetBlendConstants;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit baseIdx;
        afxUnit cnt;
        afxRect AFX_SIMD rects[];
    } AdjustScissors;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit baseIdx;
        afxUnit cnt;
        afxRect AFX_SIMD rects[];
    } AdjustCurtainsSIGMA;
    struct
    {
        _avxCmdHdr hdr;

        afxFlags        flags;
        afxString       dbgTag;
        avxCanvas       canv;
        afxLayeredRect  bounds;
        afxBool         hasD, hasS;
        avxDrawTarget   ds[AVX_MAX_AUX_BUFFERS];
        afxUnit         targetCnt;
        avxDrawTarget AFX_SIMD targets[];
    } CommenceDrawScope;
    struct
    {
        _avxCmdHdr hdr;

        afxInt nothing;
    } ConcludeDrawScope;
    struct
    {
        _avxCmdHdr hdr;

        afxBool useAuxContexts;
    } NextPass;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit bufCnt;
        afxUnit bins[AVX_MAX_CANVAS_BUFFERS];
        avxClearValue values[AVX_MAX_CANVAS_BUFFERS];
        afxUnit areaCnt;
        afxLayeredRect AFX_SIMD areas[];
    } ClearCanvas;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit sampleLvl;
    } SetRasterizationSamplesEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit sampleLvl;
        afxMask sampleMasks[AVX_MAX_SAMPLE_MASKS];
    } SetSampleMaskEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxBool enable;
    } SwitchAlphaToCoverageEXT;    
    struct
    {
        _avxCmdHdr hdr;

        afxBool enable;
    } SwitchAlphaToOneEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit first;
        afxUnit cnt;
        afxBool enabled[];
    } SwitchColorBlendingEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit first;
        afxUnit cnt;
        avxColorBlend equations[];
    } ChangeColorBlendEquationEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit first;
        afxUnit cnt;
        avxColorMask masks[];
    } SetColorWriteMaskEXT;
    struct
    {
        _avxCmdHdr hdr;
        afxBool enabled;
    } SwitchLogicOpEXT;
    struct
    {
        _avxCmdHdr hdr;
        afxBool enabled;
    } SetDepthClampEnableEXT;
    struct
    {
        _avxCmdHdr hdr;

        afxUnit first;
        afxUnit cnt;
        afxRect rects[];
    }DiscardRectanglesEXT;

    struct
    {
        _avxCmdHdr hdr;

        avxBusStage dstStage;
        avxBusAccess dstAccess;
    } PipelineBarrier;

    // QUERY OPERATIONS
    struct
    {
        _avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit slot;
        afxBool precise;
    } BeginQuery;
    struct
    {
        _avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit slot;
    } EndQuery;
    struct
    {
        _avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit baseSlot;
        afxUnit slotCnt;
        avxBuffer buf;
        afxSize offset;
        afxSize stride;
        avxQueryResultFlags flags;
    } CopyQueryResults;
    struct
    {
        _avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit baseSlot;
        afxUnit slotCnt;
    } ResetQueries;
    struct
    {
        _avxCmdHdr hdr;

        avxQueryPool pool;
        afxUnit slot;
        avxBusStage stage;
    } QueryTimestamp;

    struct
    {
        _avxCmdHdr hdr;

        afxString2048 label;
        afxV4d color;
    } CommenceDebugScope;
    struct
    {
        _avxCmdHdr hdr;

        afxInt nothing;
    } ConcludeDebugScope;
    struct
    {
        _avxCmdHdr hdr;

        afxString2048 label;
        afxV4d color;
    } MarkDebugMilestone;
};

AFX_DEFINE_UNION(_avxCmdLut)
{
    struct
    {
        void* _reservedToBeZero;
        void* CopyBuffer;
        void* FillBuffer;
        void* UpdateBuffer;

        void* PackRaster;
        void* UnpackRaster;
        void* CopyRaster;
        void* BlitRaster;
        void* ResolveRaster;
        void* ClearRaster;
        void* RegenerateMipmapsSIGMA;

        // GENERAL OPERATIONS
        void* BindPipeline;
        void* BindShadersEXT;
        void* UseLigature;
        void* BindBuffers;
        void* BindRasters;
        void* BindSamplers;
        void* ExecuteCommands;
        void* Draw;
        void* DrawIndirect;
        void* DrawIndirect2;
        void* DrawIndexed;
        void* DrawIndexedIndirect;
        void* DrawIndexedIndirect2;
        void* Dispatch;
        void* DispatchIndirect;
        void* PushConstants;
        void* StampDebug;

        // TRANSFORMATION OPERATIONS
        void* UseVertexInput;
        void* SwitchFrontFace;
        void* SetCullMode;
        void* AdjustViewports;
        void* BindVertexBuffers;
        void* BindIndexBuffer;
        void* SetPrimitiveTopology;

        void* BeginTransformFeedbackEXT;
        void* EndTransformFeedbackEXT;
        void* BeginQueryIndexedEXT;
        void* EndQueryIndexedEXT;
        void* BindTransformFeedbackBuffersEXT;
        void* DrawIndirectByteCountEXT;
        void* DrawMeshTasksEXT;
        void* DrawMeshTasksIndirectEXT;
        void* DrawMeshTasksIndirectCountEXT;

        // RASTERIZATION OPERATIOS
        void* EnableDepthBias;
        void* SetDepthBias;
        void* SetLineWidth;
        void* DisableRasterization;
        void* EnableDepthTest;
        void* SetDepthCompareOp;
        void* DisableDepthWrite;
        void* EnableStencilTest;
        void* SetStencilCompareMask;
        void* SetStencilWriteMask;
        void* SetStencilReference;
        void* SetStencilOp;
        void* EnableDepthBoundsTest;
        void* SetDepthBounds;
        void* SetBlendConstants;
        void* AdjustScissors;
        void* AdjustCurtainsSIGMA;
        void* CommenceDrawScope;
        void* ConcludeDrawScope;
        void* ClearCanvas;
        void* NextPass;
        void* SetFillModeEXT;
        void* SetRasterizationSamplesEXT;
        void* SetSampleMaskEXT;
        void* SwitchAlphaToCoverageEXT;
        void* SwitchAlphaToOneEXT;
        void* SwitchColorBlendingEXT;
        void* ChangeColorBlendEquationEXT;
        void* SetColorWriteMaskEXT;
        void* SwitchLogicOpEXT;
        void* SetDepthClampEnableEXT;
        void* DiscardRectanglesEXT;

        void* PipelineBarrier;
        // QUERY OPERATIONS
        void* BeginQuery;
        void* EndQuery;
        void* CopyQueryResults;
        void* ResetQueries;
        void* QueryTimestamp;

        void* CommenceDebugScope;
        void* ConcludeDebugScope;
        void* MarkDebugMilestone;
    };
    void(*f[])(void*, _avxCmd const*);
};

#define _AVX_CMD_ID(cmdName_) (offsetof(_avxCmdLut, cmdName_) / sizeof(void*))

AFX_DECLARE_STRUCT(_avxIddDctx);

#ifndef _AVX_DRAW_C
AFX_DECLARE_STRUCT(_avxDdiDctx);
#else
AFX_DEFINE_STRUCT(_avxDdiDctx)
{
    afxError(*exhaust)(afxDrawContext, afxBool freeMem);
    afxError(*prepare)(afxDrawContext, afxBool purge, avxCmdFlags);
    afxError(*compile)(afxDrawContext);
    afxError(*recycle)(afxDrawContext, afxBool freeMem);
};
#endif

AFX_DEFINE_STRUCT(_avxCmdBatch)
{
    afxFcc          fcc;
    afxUnit         uniqId;
    afxLink         recyc;
    afxBool         once; // if true, at execution end, it is moved to invalid state and considered in recycle chain.
    afxBool         deferred;
    afxAtom32       submCnt; // number of submissions
    afxMask64       submQueMask; // one for each queue where this dctx was submitted into.
    afxChain        commands;
    afxArena        cmdArenaCompiled;

    struct
    {
        afxBool         inDrawScope;
        afxBool         inVideoCoding;

        avxCanvas           canv;
        afxRect             area;

        avxPipeline         pip;

        avxVertexInput      vtxd;
        avxBufferedStream   vbo[16];
        avxBufferedStream   ibo;


        afxBool             expUseIndDraws;
        afxArray            indexedDraws; // avxDrawIndexedIndirect
        afxArray            draws; // avxDrawIndirect
        // at end of context, these arrays will be used to generate a big indirect buffer object.
    };
};

AFX_DEFINE_STRUCT(_avxDctxRoll)
{


};

AFX_DEFINE_STRUCT(_avxDctxCmdBin)
{
    afxChain        commands;

};

#ifdef _AVX_DRAW_CONTEXT_C
#ifdef _AVX_DRAW_CONTEXT_IMPL
AFX_OBJECT(_avxDrawContext)
#else
AFX_OBJECT(afxDrawContext)
#endif
{
    _avxDdiDctx const*  ddi;
    _avxIddDctx*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;
    avxLimits const* devLimits; // dbg copies
    avxFeatures const*enabledFeatures; // dbg copies

    avxAptitude         caps;
    afxMask             exuMask;
    afxAtom32           submCnt; // number of submissions
    afxMask64           submQueMask; // one for each queue where this dctx was submitted into.

    // Is short-lived? That is, does not recycle batches, etc.
    //afxBool             transient;
    avxContextFlags     flags;
    afxDrawContext      pool;
    afxChain            classes;
    afxClass            dctxCls;
    afxInterlockedQueue recycQue;
    avxCmdFlags         cmdFlags;
    avxContextState    state;
    afxArena            cmdArena; // owned by dsys data for specific port
    afxChain            commands;

    afxChain            ioCmds;
    afxChain            drawCmds;
    afxChain            dispatchCmds;

    struct
    {
        avxBufferedMap  map;
        afxBool         alloced;
        afxByte*        bytemap;
        afxUnit         remainRoom;
        afxUnit         nextOffset;
    } argBufs[4];
    afxArray            objsToBeDisposed;

    struct
    {
        struct
        {
            avxLigature liga;
            afxCmdId    ligaBindCmdId;
            union
            {
                struct
                {
                    avxBuffer   buf;
                    afxSize     offset;
                    afxUnit     range;
                    afxCmdId    bufBindCmdId;
                } buf;
                struct
                {
                    avxRaster   ras;
                    avxSampler  samp;
                    afxCmdId    sampBindCmdId;
                    afxCmdId    rasBindCmdId;
                } img;
            } bindings[8][96];
        } ligatures[avxBus_TOTAL];
        struct
        {
            avxPipeline pip;
            afxFlags dynFlags;
            avxVertexInput vin;
            afxBool useSepShaders;
            avxCodebase shaders[8];
        } pipelines[avxBus_TOTAL];

        struct
        {
            avxVertexInput vin;
            afxCmdId vinBindCmdId;
            avxBufferedStream streams[32];
            afxCmdId vboBindCmdId[32];
            avxBuffer ibo;
            afxUnit32 iboOffset;
            afxUnit32 iboRange;
            afxUnit32 iboStride;
            afxCmdId iboBindCmdId;
        } vertices;

        afxBool         inDrawScope;
        _avxCmd*        inDrawScopeCmd;
        afxBool         inVideoCoding;
        afxUnit         dbgUtilOpenLabelCnt;
        afxBool         xfbActive;

        avxCanvas           canv;
        avxDrawScope        ccfg;

        avxPipeline         pip;

        avxVertexInput      vtxd;
        avxBufferedStream   vbo[16];
        avxBufferedStream   ibo;


        afxBool             expUseIndDraws;
        afxArray            indexedDraws; // avxDrawIndexedIndirect
        afxArray            draws; // avxDrawIndirect
        // at end of context, these arrays will be used to generate a big indirect buffer object.
    };
};
#endif//_AVX_DRAW_CONTEXT_C

AFX _avxCmdBatch* _AvxDctxGetCmdBatch(afxDrawContext dctx, afxUnit idx);
AVX _avxCmd* _AvxDctxPushCmd(afxDrawContext dctx, afxUnit id, afxUnit siz, afxCmdId* cmdId);
AVX afxError _AvxDctxImplResetCb(afxDrawContext dctx, afxBool freeMem, afxBool permanent);
AVX afxError _AvxDctxImplEndCb(afxDrawContext dctx);

AVX afxError _AvxDctxRecycleCb(afxDrawContext dctx, afxBool freeRes);
AVX afxError _AvxDctxCompileCb(afxDrawContext dctx);
AVX afxError _AvxDctxDiscardCb(afxDrawContext dctx, afxBool freeRes);
AVX afxError _AvxDctxPrepareCb(afxDrawContext dctx, afxBool purge, avxCmdFlags flags);
AVX afxError _AvxDctxExhaustCb(afxDrawContext dctx, afxBool freeMem);

AVX afxClass const*     _AvxDctxGetDctxClass(afxDrawContext dctx);
AVX avxContextState _AvxDctxGetStatus(afxDrawContext dctx);

AVX afxClassConfig const _AVX_CLASS_CONFIG_DCTX;
AVX _avxDdiDctx const _AVX_DDI_DCTX;


#endif//AVX_CONTEXT_DDK_H
