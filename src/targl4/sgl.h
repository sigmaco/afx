/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H
#define AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

//#include "../src/e2coree/deps/gl/glcorearb.h"
//#include "../src/e2coree/deps/gl/wglext.h"

#include "gl/glad.h"
#include "gl/glad_wgl.h"

//#define DISABLE_SHARED_HARDWARE_CONTEXT

#define _AVX_DRAW_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

//#define _AFX_MODULE_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_DEVICE_IMPL
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_CONTEXT_IMPL
#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_OUTPUT_IMPL
#define _AVX_DRAW_INPUT_C
#define _AVX_DRAW_INPUT_IMPL
#define _AVX_PIPELINE_C
#define _AVX_PIPELINE_IMPL
#define _AVX_BUFFER_C
#define _AVX_BUFFER_IMPL
#define _AVX_RASTER_C
#define _AVX_RASTER_IMPL
#define _AVX_RASTERIZER_C
#define _AVX_RASTERIZER_IMPL
#define _AVX_VERTEX_INPUT_C
#define _AVX_VERTEX_INPUT_IMPL
#define _AVX_CMD_BUFFER_C
#define _AVX_CMD_BUFFER_IMPL
#define _AVX_SAMPLER_C
#define _AVX_SAMPLER_IMPL
#define _AVX_SHADER_C
#define _AVX_SHADER_IMPL
#define _AVX_SAMPLER_C
#define _AVX_SAMPLER_IMPL
#define _AVX_CANVAS_C
#define _AVX_CANVAS_IMPL
#define _AVX_BIND_SCHEMA_C
#define _AVX_BIND_SCHEMA_IMPL
#define _AVX_DRAW_BRIDGE_C
#define _AVX_DRAW_BRIDGE_IMPL
#define _AVX_DRAW_QUEUE_C
#define _AVX_DRAW_QUEUE_IMPL
#define _AFX_FENCE_C
#define _AFX_FENCE_IMPL
#define _AFX_SEMAPHORE_C
#define _AFX_SEMAPHORE_IMPL
#define _AVX_QUERY_POOL_C
#define _AVX_QUERY_POOL_IMPL

#define _AUX_UX_C
#define _AUX_OVERLAY_IMPL
#define _AUX_OVERLAY_C

#include "qwadro/draw/avxDevKit.h"
#include "qwadro/ux/afxScript.h"
#include "sglDdrv.h"

#define _SGL_MAX_DOUT_PER_PRESENTATION 4
#define _SGL_MAX_DSCR_PER_SUBMISSION 4
#define _SGL_MAX_OBJ_PER_STREAMING 4
#define _SGL_MAX_DQUE_PER_DCTX 4
#define _SGL_MAX_SWAPCHAIN_CAP 3
#define _SGL_MAX_SURF_PER_DOUT 3
#define _SGL_MAX_LEGO_PER_BIND 4
#define _SGL_MAX_ENTRY_PER_LEGO 10 
#define _SGL_MAX_STAGE_PER_PIP 8
//#define _SGL_MAX_VBO_PER_BIND 8

#define SGL_MAX_VERTEX_ATTRIBS 8
#define SGL_MAX_VERTEX_ATTRIB_BINDINGS 8
#define SGL_MAX_VERTEX_ATTRIB_STRIDE 4096
#define SGL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 4096

#define SGL_MIN_MAP_BUFFER_ALIGNMENT 64

#define SGL_MAX_VIEWPORTS 8
#define SGL_VIEWPORT_BOUNDS_RANGE 0
#define SGL_MAX_VIEWPORT_DIMS 0
#define SGL_MAX_CANVAS_WIDTH AFX_N16_MAX
#define SGL_MAX_CANVAS_HEIGHT AFX_N16_MAX
#define SGL_MAX_CANVAS_LAYERS AFX_N16_MAX
#define SGL_DEPTH_RANGE


#define SGL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 80
#define SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT (SGL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1)

#define _SGL_MAX_VAO_PER_TIME 8
//#define _SGL_MAX_VP_PER_SET 8
//#define _SGL_MAX_SCISSOR_PER_SET 8
//#define _SGL_MAX_INSTREAM_PER_SET 8
//#define _SGL_MAX_RASTER_SURF_PER_CANV 8
#define SGL_MAX_COLOR_ATTACHMENTS 8
#define _SGL_MAX_SURF_PER_CANV SGL_MAX_COLOR_ATTACHMENTS + 2

typedef enum sglUpdateFlags
{
    SGL_UPD_FLAG_DEVICE_FLUSH   = AFX_BIT(0), // flush from host to device
    SGL_UPD_FLAG_HOST_FLUSH     = AFX_BIT(1), // flush from device to host
    SGL_UPD_FLAG_DEVICE_INST    = AFX_BIT(2), // (re)instantiate on device
    SGL_UPD_FLAG_HOST_INST      = AFX_BIT(3), // (re)instantiate on host
    
    SGL_UPD_FLAG_HOST           = (SGL_UPD_FLAG_HOST_INST | SGL_UPD_FLAG_HOST_FLUSH),
    SGL_UPD_FLAG_DEVICE         = (SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH),
} sglUpdateFlags;

typedef union wglVmt
{
    struct
    {
        HGLRC(WINAPI*CreateContext)(HDC);
        BOOL(WINAPI*DeleteContext)(HGLRC);
        HGLRC(WINAPI*GetCurrentContext)(VOID);
        HDC(WINAPI*GetCurrentDC)(VOID);
        PROC(WINAPI*GetProcAddress)(LPCSTR);
        BOOL(WINAPI*MakeCurrent)(HDC, HGLRC);
        int(WINAPI*ChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);  // "wglChoosePixelFormat" funciona com Intel mas não com AMD.
        int(WINAPI*DescribePixelFormat)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
        BOOL(WINAPI*SetPixelFormat)(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd); // "wglSetPixelFormat" funciona com Intel mas não com AMD.
        int(WINAPI*GetPixelFormat)(HDC hdc);
        BOOL(WINAPI*SwapBuffers)(HDC);
        // ARB/EXT
        PFNWGLCHOOSEPIXELFORMATARBPROC ChoosePixelFormatARB;
        PFNWGLMAKECONTEXTCURRENTARBPROC MakeContextCurrentARB;
        PFNWGLGETCURRENTREADDCARBPROC GetCurrentReadDCARB;
        PFNWGLCREATECONTEXTATTRIBSARBPROC CreateContextAttribsARB;
        PFNWGLGETEXTENSIONSSTRINGARBPROC GetExtensionsStringARB;
        PFNWGLSWAPINTERVALEXTPROC SwapIntervalEXT;
    };
    void *ptr;
} wglVmt;

AFX_DEFINE_UNION(sglLegoData) // A GPUBindGroupEntry describes a single resource to be bound in a GPUBindGroup, and has the following members:
{
    struct
    {
        afxBuffer           buf; // The GPUBuffer to bind.
        afxNat              offset; // The offset, in bytes, from the beginning of buffer to the beginning of the range exposed to the shader by the buffer binding.
        afxNat              range; // The size, in bytes, of the buffer binding. If undefined, specifies the range starting at offset and ending at the end of buffer.
    };
    struct
    {
        afxSampler          smp;
        afxRaster          tex;
    };
};

AFX_DEFINE_STRUCT(sglVertexInputState)
{
    struct
    {
        afxBuffer       buf;
        afxNat32        offset;
        afxNat32        range;
        afxNat32        stride;
    }                   sources[SGL_MAX_VERTEX_ATTRIB_BINDINGS];
    /*
    struct
    {
        afxNat          srcIdx;
        afxNat32        stride;
        afxBool         instance;
    }                   streams[SGL_MAX_VERTEX_ATTRIB_BINDINGS];
    afxNat              streamCnt;
    struct
    {
        afxNat          location;
        afxVertexFormat fmt;
        afxNat          srcIdx;
        afxNat32        offset;
    }                   attrs[SGL_MAX_VERTEX_ATTRIBS];
    afxNat              attrCnt;
    */
    afxBuffer           idxSrcBuf;
    afxNat32            idxSrcOff;
    afxNat32            idxSrcRange;
    afxNat32            idxSrcSiz;


    // used by queue
    afxMask             streamUpdMask;
    afxNat              streamUpdCnt;
    afxMask             attribUpdMask;
    afxNat              attribUpdCnt;
    afxBool             iboUpdReq;
};

typedef struct sglXfrmState
{
    afxNat              vpCnt; /// 0
    afxViewport         vps[SGL_MAX_VIEWPORTS];

    afxPrimTopology     primTop; /// is a option defining the primitive topology. /// afxPrimTopology_TRI_LIST
    afxBool             primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    afxNat              patchControlPoints; /// is the number of control points per patch.

    afxBool             depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE

    afxCullMode         cullMode; /// is the triangle facing direction used for primitive culling. /// afxCullMode_BACK
    afxBool             cwFrontFacing; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)

} sglXfrmState;

typedef struct sglRasterizerState
{
    afxBool             rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE

    struct
    {
        afxCanvas       canv;
        afxRect         area;
        afxNat32        layerCnt;
        afxNat32        rasterCnt;
        afxDrawTarget   rasters[SGL_MAX_COLOR_ATTACHMENTS];
        afxDrawTarget   ds[2];
        afxBool         defFbo;
    }                   pass; // always active pass

    afxFillMode         fillMode; /// is the triangle rendering mode. /// afxFillMode_SOLID
    afxReal             lineWidth; /// is the width of rasterized line segments. /// 1.f    

    afxBool             depthBiasEnabled; /// controls whether to bias fragment depth values. /// FALSE
    afxReal             depthBiasSlopeScale; /// is a scalar factor applied to a fragment's slope in depth bias calculations. /// 0.f
    afxReal             depthBiasConstFactor; /// is a scalar factor controlling the constant depth value added to each fragment. /// 0.f
    afxReal             depthBiasClamp; /// is the maximum (or minimum) depth bias of a fragment. /// 0.f

    afxBool             msEnabled; /// If enabld, multisample rasterization will be used. FALSE
    afxNat              sampleCnt; /// is a value specifying the number of samples used in rasterization. /// 0
    afxMask             sampleMasks[32]; /// an array of sample mask values used in the sample mask test. /// [ 1, ]
    afxBool             sampleShadingEnabled; /// used to enable Sample Shading. /// FALSE
    afxReal             minSampleShadingValue; /// specifies a minimum fraction of sample shading if sampleShadingEnable is set to TRUE. /// 0.f
    afxBool             alphaToCoverageEnabled; /// controls whether a temporary coverage value is generated based on the alpha component of the fragment's first color output. /// FALSE
    afxBool             alphaToOneEnabled; /// controls whether the alpha component of the fragment's first color output is replaced with one. /// FALSE

    afxPixelFormat      dsFmt; /// is the format of depth/stencil surface this pipeline will be compatible with.

    afxBool             depthTestEnabled; /// controls whether depth testing is enabled. /// FALSE
    afxCompareOp        depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. /// afxCompareOp_LESS
    afxBool             depthWriteDisabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. /// FALSE
    afxBool             depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled. /// FALSE
    afxV2d              depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]
    afxBool             stencilTestEnabled; /// FALSE
    afxStencilConfig    stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    afxStencilConfig    stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.


    afxNat              areaCnt; /// 0
    afxRect             areaRects[SGL_MAX_VIEWPORTS];
    afxNat              scisCnt; /// 0
    afxRect             scisRects[SGL_MAX_VIEWPORTS];

    afxNat              outCnt;
    afxColorOutputChannel   outs[8];
    afxBool             anyBlendEnabled;
    afxReal             blendConstants[4]; /// [ 0, 0, 0, 1 ]
    afxBool             logicOpEnabled; /// FALSE
    afxLogicOp          logicOp; /// afxLogicOp_NOP
} sglRasterizerState;


typedef struct
{
    glVmt                   gl;    
    wglVmt                  wgl;
    HWND                    wnd;
    HDC                     dc;
    int                     dcPxlFmt;
    PIXELFORMATDESCRIPTOR   dcPfd;
    HGLRC                   glrc;
    afxNat                  verMajor, verMinor, verPatch;
    afxString               subsysName;
    afxString               subsysVer;

    afxThread               dedThread;
    afxNat                  assignedTxuUniqueId;
    afxNat                  portIdx;
    afxSlock                deletionLock;
    afxQueue                deletionQueue;

    afxDrawContext          activeDctx;

    afxSize                 numOfFedVertices, numOfFedIndices, numOfFedInstances;
 
    sglXfrmState            activeXformState, nextXformState;
    sglRasterizerState      activeRasterState, nextRasterState;

    afxRasterizer           activeRazr, nextRazr;
    afxPipeline             activePip, nextPip;
    afxVertexInput          activeVin, nextVin;
    sglVertexInputState     nextVinBindings;

    union
    {
        struct
        {
            afxBuffer       buf; // The GPUBuffer to bind.
            afxNat32        offset; // The offset, in bytes, from the beginning of buffer to the beginning of the range exposed to the shader by the buffer binding.
            afxNat32        range; // The size, in bytes, of the buffer binding. If undefined, specifies the range starting at offset and ending at the end of buffer.
            
        };
        struct
        {
            afxSampler      smp;
            afxRaster      ras;
        };
    }                       activeResBind[_SGL_MAX_LEGO_PER_BIND][_SGL_MAX_ENTRY_PER_LEGO], nextResBind[_SGL_MAX_LEGO_PER_BIND][_SGL_MAX_ENTRY_PER_LEGO];
    afxMask                 nextResBindUpdMask[_SGL_MAX_LEGO_PER_BIND];

    afxBuffer               activeUnpakPbo;
    afxBuffer               activePakPbo;

    afxMask                 nextVinBindingsMask;
    afxNat                  nextVinBindingsCnt;
    afxMask                 nextVinAttribUpdMask;
    afxNat                  nextVinAttribUpdCnt;

    afxMask                 nextBlendConstUpd;
    afxMask                 nextViewportUpdMask;
    afxNat                  nextViewportUpdCnt;
    afxMask                 nextScissorUpdMask;
    afxNat                  nextScissorUpdCnt;
    afxMask                 nextAreaUpdMask;
    afxNat                  nextAreaUpdCnt;
    afxBool                 scissorTestEnabled;

    afxBool                 flushPip, flushPass, flushSr;
    
    afxBool                 flushDepthTest, flushDepthBoundsTest, flushStencilTest, flushDepthBias;
    afxBool                 flushRasterizer, flushPrimCulling;
    afxBool                 flushMultisample, flushSampleShading, flushAlphaToCover, flushAlphaToOne;
    afxBool                 flushColorBlend, flushLogicOp, flushColorWriteMask;

    //afxNat                  flushVbBase, flushVbCnt;
    afxNat                  flushVpBase, flushVpCnt;
    afxNat                  flushSrBase, flushSrCnt;
    //afxNat                  flushRbBase[_SGL_MAX_LEGO_PER_BIND], flushRbCnt[_SGL_MAX_LEGO_PER_BIND];

    
    afxBool                 running;

    afxBool         instanced;

    // submission stuff
    afxBool         submissionSuspended;
    GLuint          tmpFbo[_SGL_MAX_SWAPCHAIN_CAP];
    afxNat          activeTmpFboIdx;
    GLuint          vao[_SGL_MAX_VAO_PER_TIME];
    afxNat          activeVaoIdx;

    GLuint          emptyVao;
    GLuint          fboOpSrc;
    afxRaster       fboOpSrcAnnex;
    GLuint          fboOpDst;
    afxRaster       fboOpDstAnnex;
} sglDpu;

struct _afxDdevIdd
{
    WNDCLASSEX      wndClss;
    afxModule       opengl32;
    afxNat          dpuCnt;
    sglDpu*         dpus;
};

AFX_OBJECT(afxDrawContext)
{
    struct _afxBaseDrawContext base;
    // presentation stuff, only on unit 0.
    afxBool presentationSuspended;
    //GLuint presentFboGpuHandle;
    afxRasterizer presentRazr;
    afxSampler  presentSmp;
    
    afxRasterizer fntRazr;
    afxSampler  fntSamp;    
    afxRaster   fntRas;
    afxBuffer   fntDataBuf;
    afxBuffer   fntUnifBuf;

    //afxBuffer tristrippedQuad2dPosBuf;
};

AFX_OBJECT(afxDrawBridge)
{
    struct _afxBaseDrawBridge base;
};

AFX_OBJECT(afxDrawQueue)
{
    struct _afxBaseDrawQueue base;
};

AFX_DEFINE_STRUCT(_sglQueueing)
{
    afxNat                  siz;
    afxLinkage              chain;
    afxNat                  submNo; // this submission number ordinal (B2F)
    afxNat                  reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime                 pushTime; // submission (into input) time
    afxTime                 pullTime; // fecth (by queue) time
    afxTime                 complTime; // completation time
    afxError(*exec)(sglDpu*, afxDrawBridge, afxNat queIdx, _sglQueueing*);
};

AFX_DEFINE_STRUCT(_sglQueueingExecute)
{
    _sglQueueing            hdr;
    afxNat                  itemCnt;
    afxExecutionRequest     afxSimd(items[]);
};

AFX_DEFINE_STRUCT(_sglQueueingTransfer)
{
    _sglQueueing            hdr;
    afxNat                  itemCnt;
    afxTransferRequest      afxSimd(items[]);
};

AFX_DEFINE_STRUCT(_sglQueueingPresent)
{
    _sglQueueing            hdr;
    afxNat                  itemCnt;
    afxPresentationRequest  afxSimd(items[]);
};

AFX_DEFINE_STRUCT(_sglQueueingStamp)
{
    _sglQueueing            hdr;
    afxLinkage              echoLnk;
    afxV2d                  origin;
    afxString4096           caption;
    afxNat                  itemCnt;
    afxPresentationRequest  afxSimd(items[]);
};

AFX_DEFINE_STRUCT(_sglQueueingMmap)
{
    _sglQueueing            hdr;
    afxBuffer               buf;
    afxSize                 off;
    afxNat                  ran;
    afxFlags                flags;
};

AFX_DEFINE_STRUCT(_sglDeleteGlRes)
{
    union
    {
        GLuint  gpuHandle;
        void*   gpuHandlePtr;
    };
    GLuint  type; // 0 buf, 1 tex, 2 surf, 3 canv, 4 smp, 5 pip, 6 shd, 7 shd (separate) program
};

AFX_DEFINE_STRUCT(_sglCmd)
{
    afxLinkage          script;
    afxNat              id;
    afxNat              siz;
};

AFX_OBJECT(afxPipeline)
{
    struct afxBasePipeline base;
    sglUpdateFlags  updFlags;
    GLuint          glHandle;
    //sglVertexInput  vertexInput;
};

AFX_OBJECT(afxVertexInput)
{
    struct afxBaseVertexInput base;
    sglUpdateFlags  updFlags;
    GLuint          glHandle;
    sglVertexInputState bindings;
};

AFX_OBJECT(afxRasterizer)
{
    struct afxBaseRasterizer base;
    sglUpdateFlags  updFlags;

};

AFX_OBJECT(afxQueryPool)
{
    struct _afxBaseQueryPool base;
    sglUpdateFlags  updFlags;
    GLuint         *glHandle; // * cap
    GLenum          glTarget;
};

AFX_OBJECT(afxLigature)
{
    struct afxBaseLigature base;
    int a;
};

AFX_OBJECT(afxShader)
{
    struct afxBaseShader base;
    sglUpdateFlags      updFlags;
    afxNat              glHandle;
    afxNat              glProgHandle;
    afxBool             compiled;
};

AFX_OBJECT(afxSampler)
{
    struct afxBaseSampler base;
    sglUpdateFlags  updFlags;
    afxNat          glHandle;
};

AFX_OBJECT(afxFence)
{
    struct _afxBaseFence base;
    sglUpdateFlags  updFlags;
    GLsync          glHandle;
};

AFX_OBJECT(afxSemaphore)
{
    struct _afxBaseSemaphore base;
};

AFX_OBJECT(afxBuffer)
{
    struct afxBaseBuffer    base;
    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    GLenum          glTarget;
    //GLenum          glUsage;
    GLbitfield      glAccess;
    //GLintptr        glMappedOff;
    //GLsizeiptr      glMappedSiz;
};

AFX_OBJECT(afxRaster)
{
    struct afxBaseRaster base;
    sglUpdateFlags  updFlags;
    GLuint          glHandle;
    GLuint          glHandleAux; // for draw buffers
    GLint           glIntFmt;
    GLenum          glTarget, glFmt, glType;
};

AFX_OBJECT(afxCanvas)
{
    struct afxBaseCanvas base;
    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    afxMask         storeBitmask;
};

AFX_OBJECT(avxCmdb)
{
    struct afxBaseCmdBuffer    base;
    afxChain                    commands;
    afxChain                    echoes;
    afxChain                    canvOps;
    afxChain            drawCalls;
    afxChain            stateChanges;
    afxChain            pipBinds;
    afxChain            vbufBinds;
    afxChain            ibufBinds;
    afxChain            bufBinds;
    afxChain            texBinds;

    afxNat              totalCmdCnt;
    afxNat              lastDrawCmdIdx;
    afxNat              drawCmdCnt;
    afxNat              lastPipBindCmdIdx;
    afxNat              pipBindCmdCnt;
    afxNat              lastVbufBindCmdIdx;
    afxNat              vbufBindCmdCnt;
    afxNat              lastIbufBindCmdIdx;
    afxNat              ibufBindCmdCnt;
    afxNat              lastBufBindCmdIdx;
    afxNat              bufBindCmdCnt;
    afxNat              lastTexBindCmdIdx;
    afxNat              texBindCmdCnt;
    afxNat              lastRasUpdCmdIdx;
    afxNat              rasUpdCmdCnt;
    afxNat              lastDepthUpdCmdIdx;
    afxNat              depthUpdCmdCnt;
    afxNat              lastVpCmdIdx; // viewport and/or scissor changes
    afxNat              vpUpdCmdCnt;
    afxNat              lastScisCmdIdx; // viewport and/or scissor changes
    afxNat              scisUpdCmdCnt;
    struct
    {
        struct
        {
            afxPipeline     next;
            afxPipeline     curr;
        } pip;
    } levelCaches[1];
};

typedef struct _DrawPipelineResource
{
    afxShaderStage stages;
    afxShaderResourceType resType;
    //afxShaerBaseResType baseType;
    //VkAccessFlags access;
    afxNat8 set;
    afxNat8 binding;
    afxNat8 location;
    afxNat8 inputAttachmentIndex;
    afxNat8 vecSize;
    afxNat8 arraySize;
    afxNat offset;
    afxNat size;
    afxChar name[16];
} _DrawPipelineResource;

#if 0
typedef enum afxDrawCmdId
{
    SGL_CMD_END = offsetof(afxCmd, EndOfScript) / sizeof(void*),
    SGL_CMD_EXECUTE_COMMANDS = offsetof(afxCmd, ExecuteCommands) / sizeof(void*),
    SGL_CMD_BEGIN_SYNTHESIS = offsetof(afxCmd, BeginSynthesis) / sizeof(void*),
    SGL_CMD_NEXT_PASS = offsetof(afxCmd, NextPass) / sizeof(void*),
    SGL_CMD_FINISH_SYNTHESIS = offsetof(afxCmd, FinishSynthesis) / sizeof(void*),
    
    SGL_CMD_BIND_BUFFERS = offsetof(afxCmd, BindBuffers) / sizeof(void*),
    SGL_CMD_BIND_RASTERS = offsetof(afxCmd, BindRasters) / sizeof(void*),
    
    SGL_CMD_BIND_PIPELINE = offsetof(afxCmd, BindPipeline) / sizeof(void*),

    SGL_CMD_READJUST_VIEWPORTS = offsetof(afxCmd, ReadjustViewports) / sizeof(void*),
    SGL_CMD_RESET_VIEWPORTS = offsetof(afxCmd, ResetViewports) / sizeof(void*),    
    SGL_CMD_READJUST_SCISSORS = offsetof(afxCmd, ReadjustScissors) / sizeof(void*),
    SGL_CMD_RESET_SCISSORS = offsetof(afxCmd, ResetScissors) / sizeof(void*),
    SGL_CMD_READJUST_AREAS = offsetof(afxCmd, ReadjustAreas) / sizeof(void*),
    SGL_CMD_RESET_AREAS = offsetof(afxCmd, ResetAreas) / sizeof(void*),

    SGL_CMD_BIND_VERTEX_SOURCES = offsetof(afxCmd, BindVertexSources) / sizeof(void*),
    //SGL_CMD_RESET_VERTEX_STREAMS = offsetof(afxCmd, ResetVertexStreams) / sizeof(void*),
    //SGL_CMD_RESET_VERTEX_ATTRIBUTES = offsetof(afxCmd, ResetVertexAttributes) / sizeof(void*),
    SGL_CMD_BIND_INDEX_SOURCE = offsetof(afxCmd, BindIndexSource) / sizeof(void*),
    SGL_CMD_SET_PRIMITIVE_TOPOLOGY = offsetof(afxCmd, SetPrimitiveTopology) / sizeof(void*),
    
    SGL_CMD_DRAW = offsetof(afxCmd, Draw) / sizeof(void*),
    SGL_CMD_DRAW_INDEXED = offsetof(afxCmd, DrawIndexed) / sizeof(void*),

    SGL_CMD_DISABLE_RASTERIZATION = offsetof(afxCmd, DisableRasterization) / sizeof(void*),
    SGL_CMD_SWITCH_FRONT_FACE = offsetof(afxCmd, SwitchFrontFace) / sizeof(void*),
    SGL_CMD_SET_CULL_MODE = offsetof(afxCmd, SetCullMode) / sizeof(void*),
    SGL_CMD_ENABLE_DEPTH_BIAS = offsetof(afxCmd, EnableDepthBias) / sizeof(void*),
    SGL_CMD_SET_DEPTH_BIAS = offsetof(afxCmd, SetDepthBias) / sizeof(void*),
    SGL_CMD_SET_LINE_WIDTH = offsetof(afxCmd, SetLineWidth) / sizeof(void*),

    SGL_CMD_ENABLE_DEPTH_BOUNDS_TEST = offsetof(afxCmd, EnableDepthBoundsTest) / sizeof(void*),
    SGL_CMD_SET_DEPTH_BOUNDS = offsetof(afxCmd, SetDepthBounds) / sizeof(void*),
    SGL_CMD_ENABLE_STENCIL_TEST = offsetof(afxCmd, EnableStencilTest) / sizeof(void*),
    SGL_CMD_SET_STENCIL_COMPARE_MASK = offsetof(afxCmd, SetStencilCompareMask) / sizeof(void*),
    SGL_CMD_SET_STENCIL_WRITE_MASK = offsetof(afxCmd, SetStencilWriteMask) / sizeof(void*),
    SGL_CMD_SET_STENCIL_REFERENCE = offsetof(afxCmd, SetStencilReference) / sizeof(void*),
    SGL_CMD_ENABLE_DEPTH_TEST = offsetof(afxCmd, EnableDepthTest) / sizeof(void*),
    SGL_CMD_SET_DEPTH_COMPARE_OP = offsetof(afxCmd, SetDepthCompareOp) / sizeof(void*),
    SGL_CMD_ENABLE_DEPTH_WRITE = offsetof(afxCmd, EnableDepthWrite) / sizeof(void*),
    
    SGL_CMD_SET_BLEND_CONSTANTS = offsetof(afxCmd, SetBlendConstants) / sizeof(void*),

    SGL_CMD_PACK_RASTER = offsetof(afxCmd, PackRaster) / sizeof(void*),
    SGL_CMD_UNPACK_RASTER = offsetof(afxCmd, UnpackRaster) / sizeof(void*),
    SGL_CMD_COPY_RASTER = offsetof(afxCmd, CopyRaster) / sizeof(void*),
    SGL_CMD_REGENERATE_MIPMAPS = offsetof(afxCmd, RegenerateMipmaps) / sizeof(void*),
    SGL_CMD_FLIP_RASTER = offsetof(afxCmd, FlipRaster) / sizeof(void*),
    SGL_CMD_SWIZZLE_RASTER = offsetof(afxCmd, SwizzleRaster) / sizeof(void*),

    SGL_CMD_TOTAL = offsetof(afxCmd, Total) / sizeof(void*),
} afxDrawCmdId;
#else
#define SGL_CMD_TOTAL offsetof(afxCmd, Total) / sizeof(void*)
#endif

AFX_DEFINE_STRUCT(_sglCmdEcho)
{
    _sglCmd                     cmd;
    afxLinkage                  echoLnk;
    afxM4d                      m;
    afxRestring                 caption;
    afxChar                     afxSimd(strBuf[]);
};

AFX_DEFINE_STRUCT(_sglCmdEchoFmt)
{
    _sglCmd                     cmd;
    afxM4d                      m;
    afxRestring                 caption;
    afxChar                     afxSimd(strBuf[]);
};

AFX_DEFINE_STRUCT(_sglCmdDraw)
{
    _sglCmd                     cmd;
    afxNat32                        vtxCnt, instCnt, firstVtx, firstInst;
};

AFX_DEFINE_STRUCT(_sglCmdDrawIndexed)
{
    _sglCmd                     cmd;
    afxNat32                        idxCnt, instCnt, firstIdx, vtxOff, firstInst;
};

AFX_DEFINE_STRUCT(_sglCmdDrawPrefab)
{
    _sglCmd                     cmd;
    afxDrawPrefab                   prefab;
    afxNat32                        instCnt;
};

AFX_DEFINE_STRUCT(_sglCmdBeginSynthesis)
{
    _sglCmd cmd;
    afxBool defFbo;
    afxLinkage canvOpsLnk;
    afxCanvas canv;
    afxRect area;
    afxNat32 layerCnt;
    afxDrawTarget depth;
    afxDrawTarget stencil;
    afxNat32 rasterCnt;
    afxDrawTarget afxSimd(rasters[]);
};

AFX_DEFINE_STRUCT(_sglCmdBindRasters)
{
    _sglCmd cmd;
    afxNat32 set;
    afxNat32 first, cnt;
    afxSampler afxSimd(smp[_SGL_MAX_ENTRY_PER_LEGO]);
    afxRaster afxSimd(tex[_SGL_MAX_ENTRY_PER_LEGO]);
};

AFX_DEFINE_STRUCT(_sglCmdBindBuffers)
{
    _sglCmd cmd;
    afxNat32 set;
    afxNat32 first, cnt;
    afxBuffer afxSimd(buf[_SGL_MAX_ENTRY_PER_LEGO]);
    afxNat32 afxSimd(offset[_SGL_MAX_ENTRY_PER_LEGO]);
    afxNat32 afxSimd(range[_SGL_MAX_ENTRY_PER_LEGO]);
};

AFX_DEFINE_STRUCT(sglBufferInfo)
{
    afxBuffer buf;
    afxNat32 offset;
    afxNat32 range;
    afxNat32 stride;
};

AFX_DEFINE_STRUCT(_sglCmdVertexSources)
{
    _sglCmd cmd;
    afxNat32 first, cnt;
    sglBufferInfo afxSimd(info[]);
};

AFX_DEFINE_STRUCT(_sglCmdVertexStreams)
{
    _sglCmd                     cmd;
    afxNat cnt;
    struct
    {
        afxNat  srcIdx;
        afxNat  stride;
        afxBool instance;
    }           afxSimd(streams[]);
};

AFX_DEFINE_STRUCT(_sglCmdVertexAttributes)
{
    _sglCmd                     cmd;    
    afxNat cnt;
    struct
    {
        afxNat location;
        afxVertexFormat fmt;
        afxNat srcIdx;
        afxNat32 offset;
    }       afxSimd(attrs[]);
};

AFX_DEFINE_STRUCT(_sglCmdBufferRange)
{
    _sglCmd                     cmd;
    afxBuffer                       buf;
    afxNat32                        offset;
    afxNat32                        range;
    afxNat32                        stride;
};

AFX_DEFINE_STRUCT(_sglCmdRasterizer)
{
    _sglCmd                     cmd;
    afxRasterizer               razr;
    afxFlags                    dynamics;
};

AFX_DEFINE_STRUCT(_sglCmdPipeline)
{
    _sglCmd                     cmd;
    afxNat                      segment;
    afxPipeline                 pip;
    afxFlags                    dynamics;
};

AFX_DEFINE_STRUCT(_sglCmdVertexInput)
{
    _sglCmd                     cmd;
    afxVertexInput              vin;
};

AFX_DEFINE_STRUCT(_sglCmdBool)
{
    _sglCmd                     cmd;
    afxBool                         value;
};

AFX_DEFINE_STRUCT(_sglCmdReal)
{
    _sglCmd                     cmd;
    afxReal                         value;
};

AFX_DEFINE_STRUCT(_sglCmdReal2)
{
    _sglCmd                     cmd;
    afxReal                         value[2];
};

AFX_DEFINE_STRUCT(_sglCmdReal3)
{
    _sglCmd                     cmd;
    afxReal                         value[3];
};

AFX_DEFINE_STRUCT(_sglCmdReal4)
{
    _sglCmd                     cmd;
    afxReal                         value[4];
};

AFX_DEFINE_STRUCT(_sglCmdPackRasterRegions)
{
    _sglCmd         cmd;
    afxBool         unpack;
    afxRaster ras;
    afxBuffer buf;
    afxNat opCnt;
    afxRasterIo afxSimd(ops[]);
};

AFX_DEFINE_STRUCT(_sglCmdRwRasterRegions)
{
    _sglCmd         cmd;
    afxBool         down;
    afxRaster ras;
    afxStream ios;
    afxCodec cdc;
    afxNat opCnt;
    afxRasterIo afxSimd(ops[]);
};

AFX_DEFINE_STRUCT(_sglCmdCopyRasterRegions)
{
    _sglCmd         cmd;
    afxRaster           src;
    afxRaster           dst;
    afxNat              opCnt;
    afxRasterCopy     afxSimd(ops[]);
};

AFX_DEFINE_STRUCT(_sglCmdRegenerateMipmaps)
{
    _sglCmd         cmd;
    afxRaster           ras;
    afxNat              baseLod;
    afxNat              lodCnt;
};

AFX_DEFINE_STRUCT(_sglCmdSwizzleRasterRegions)
{
    _sglCmd             cmd;
    afxRaster           ras;
    afxColorSwizzle     a;
    afxColorSwizzle     b;
    afxNat              rgnCnt;
    afxRasterRegion     afxSimd(rgn[]);
};

AFX_DEFINE_STRUCT(_sglCmdFlipRasterRegions)
{
    _sglCmd             cmd;
    afxRaster           ras;
    afxM4d              m;
    afxNat              rgnCnt;
    afxRasterRegion     afxSimd(rgn[]);
};

AFX_DEFINE_STRUCT(_sglCmdBufIo)
{
    _sglCmd         cmd;
    afxBuffer       buf;
    afxBool         export;
    afxStream       io;
    afxNat          opCnt;
    afxBufferIoOp   afxSimd(ops[]);
};

AFX_DEFINE_STRUCT(_sglCmdBufRw)
{
    _sglCmd         cmd;
    afxBuffer       buf;
    afxNat          offset;
    afxNat          range;
    afxBool         toHost;
    union
    {
        void*       dst;
        afxByte     afxSimd(src[]);
    };
};

AFX_DEFINE_STRUCT(_sglCmdBufCpy)
{
    _sglCmd         cmd;
    afxBuffer src;
    afxBuffer dst;
    afxNat opCnt;
    afxBufferCopyOp afxSimd(ops[]);
};

AFX_DEFINE_STRUCT(_sglCmdBufSet)
{
    _sglCmd         cmd;
    afxBuffer buf;
    afxNat  offset;
    afxNat  range;
    afxNat data;
};

AFX_DEFINE_STRUCT(_sglCmdNat)
{
    _sglCmd                     cmd;
    afxNat                          value;
};

AFX_DEFINE_STRUCT(_sglCmdBitmaskNat32)
{
    _sglCmd                     cmd;
    afxMask                         mask;
    afxNat32                        value;
};

AFX_DEFINE_STRUCT(_sglCmdScissor)
{
    _sglCmd cmd;
    afxNat32 first, cnt;
    afxRect afxSimd(rect[]);
};

AFX_DEFINE_STRUCT(_sglCmdViewport)
{
    _sglCmd cmd;
    afxNat32 first, cnt;
    afxViewport afxSimd(vp[]);
};

AFX_DEFINE_STRUCT(_sglCmdNextPass)
{
    _sglCmd                     cmd;
    afxBool32                       useAuxScripts;
};

AFX_DEFINE_STRUCT(_sglCmdExecCmds)
{
    _sglCmd                     cmd;
    afxNat                          cnt;
    avxCmdb                   afxSimd(subsets[8]);
};

AFX_DEFINE_STRUCT(_sglCmdCopyTex)
{
    _sglCmd                     cmd;
    afxRaster                      dst;
    afxRaster                      src;
    afxNat32                        rgnCnt;
    afxRasterRegion                afxSimd(rgn[16]);
};

AFX_DEFINE_STRUCT(_sglCmdUniformVectorEXT)
{
    _sglCmd                     cmd;
    afxChar const*                  name;
    afxNat                          cnt;
    afxReal                         afxSimd(v[4]);
};

AFX_DEFINE_STRUCT(_sglCmdUniformMatrixEXT)
{
    _sglCmd                     cmd;
    afxChar const*                  name;
    afxNat                          cnt;
    afxReal                         afxSimd(m[4][4]);
};


// vkCmdBindDescriptorSets - Binds descriptor sets to a command buffer

//SGL afxError _SglCreateCombinedDeviceContext(WNDCLASSEXA *oglWndClss, HGLRC shareCtx, HWND *phwnd, HDC *phdc, HGLRC *phrc, wglVmt const* wgl, HMODULE opengl32);

SGL void _SglDctxDeleteGlRes(afxDrawContext dctx, afxNat type, void* gpuHandle);


typedef enum sglBindFlags
{
    sglBindFlag_BIND = AFX_BIT(0),
    sglBindFlag_SYNC = AFX_BIT(1),
    sglBindFlag_KEEP = AFX_BIT(2)
}sglBindFlags;

_SGL afxError AfxRegisterDrawDrivers(afxModule mdle, afxDrawSystem dsys);

SGL afxError _DpuBindAndSyncSamp(sglDpu* dpu, afxNat glUnit, afxSampler samp);
SGL afxError _DpuSyncShd(sglDpu* dpu, afxShader shd, afxShaderStage stage, glVmt const* gl);
SGL afxError _DpuSurfSync(sglDpu* dpu, afxRasterSlot surf, glVmt const* gl); // must be used before texUpdate
SGL afxError DpuBindAndSyncRas(sglDpu* dpu, afxNat glUnit, afxRaster tex);
SGL afxError _SglTexSubImage(glVmt const* gl, GLenum glTarget, afxRasterRegion const* rgn, GLenum glFmt, GLenum glType, afxAddress const src);

SGL afxError _DpuBindAndSyncVin(sglDpu* dpu, afxVertexInput vin, sglVertexInputState* nextVinBindings);
SGL afxError _DpuBindAndSyncPip(sglDpu* dpu, afxBool bind, afxBool sync, afxPipeline pip);
SGL afxError _DpuBindAndSyncRazr(sglDpu* dpu, afxRasterizer razr);
SGL afxError _DpuBindAndResolveLego(sglDpu* dpu, afxLigature legt, GLuint glHandle);
SGL afxError _DpuBindAndSyncCanv(sglDpu* dpu, afxBool bind, afxBool sync, GLenum glTarget, afxCanvas canv);


SGL afxError DpuBindAndSyncBuf(sglDpu* dpu, GLenum glTarget, afxBuffer buf);
SGL afxError _DpuRemapBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxFlags flags);
SGL afxError _DpuStoreBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxByte* dst);
SGL afxError _DpuLoadBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxByte const* src);
SGL afxError _DpuOutputBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxStream out, afxSize at);
SGL afxError _DpuInputBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxStream in, afxSize at);

SGL afxError _DpuLoadRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxByte const* src);
SGL afxError _DpuStoreRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxByte* dst);
SGL afxError _DpuPackRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxBuffer buf);
SGL afxError _DpuUnpackRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxBuffer buf);
SGL afxError _DpuOutputRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxStream out);
SGL afxError _DpuInputRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxStream in);

SGL afxError _DpuPresentDout(sglDpu* dpu, afxDrawOutput dout, afxNat outBufIdx);

//SGL afxSize _AfxMeasureTextureRegion(afxRaster tex, afxRasterRegion const *rgn);

SGL BOOL SglMakeCurrent(HDC hdc, HGLRC hrc, wglVmt const* vmt);
SGL afxError SglSwapBuffers(HDC hdc, wglVmt const* vmt);
SGL int SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd, wglVmt const* vmt);
SGL BOOL SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd, wglVmt const* vmt);
SGL int SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd, wglVmt const* vmt);
SGL int SglGetPixelFormat(HDC hdc, wglVmt const* vmt);

SGL afxLigature _SglDrawContextFindLego(afxDrawContext dctx, afxNat bindCnt, afxPipelineRigBindingDecl const bindings[]);

//SGL afxBool _SglDqueVmtSubmitCb(afxDrawContext dctx, afxDrawBridge ddge, afxDrawSubmissionSpecification const *spec, afxNat *submNo);
SGL afxError _DdgeProcCb(afxDrawBridge ddge, afxThread thr);

SGL afxClassConfig const _SglDctxMgrCfg;

SGL afxError _SglDoutVmtFlushCb(afxDrawOutput dout, afxTime timeout);

SGL afxError _SglDdevInitDinCb(afxDrawDevice ddev, afxDrawInput din, afxDrawInputConfig const* cfg, afxUri const* endpoint);
SGL afxError _SglDdevDeinitDoutCb(afxDrawDevice ddev, afxDrawOutput dout);
SGL afxError _SglDdevInitDoutCb(afxDrawDevice ddev, afxDrawOutput dout, afxDrawOutputConfig const* config, afxUri const* endpoint);
SGL afxError _SglDinFreeAllBuffers(afxDrawInput din);

SGL afxCmd _SglEncodeCmdVmt;
SGL afxCmdBuf const _SglEncodeCmdBufferVmt;
SGL afxCmdRas const _SglEncodeCmdRasterVmt;
SGL afxCmdTransformation const _SglEncodeCmdTransformationVmt;
SGL afxCmdRasterization const _SglEncodeCmdRasterizationVmt;
SGL void _SglBindFboAttachment(glVmt const* gl, GLenum glTarget, GLenum glAttachment, GLenum texTarget, GLuint texHandle, GLint level, GLuint layer, GLuint z);

SGL afxError _SglWaitFenc(afxBool waitAll, afxNat64 timeout, afxNat cnt, afxFence const fences[]);
SGL afxError _SglResetFenc(afxNat cnt, afxFence const fences[]);

SGL afxCmdId _SglEncodeCmdCommand(avxCmdb cmdb, afxNat id, afxNat siz, _sglCmd *cmd);

SGL afxClassConfig const _SglFencMgrCfg;
SGL afxClassConfig const _SglSemMgrCfg;
SGL afxClassConfig const _SglQrypMgrCfg;

SGL afxResult _SglDdevIoctl(afxDrawDevice ddev, afxNat reqCode, va_list va);

SGL const char *glVmtNames[];
SGL void _LoadWglExtendedSymbols(HMODULE opengl32, wglVmt* wgl);
SGL afxError SglLoadGlVmt(HMODULE opengl32, afxNat base, afxNat cnt, void* vmt[], afxBool echo);
SGL afxError SglLoadWglVmt(HMODULE opengl32, afxNat base, afxNat cnt, void* vmt[], afxBool echo);
SGL void _LoadWglBaseSymbols(HMODULE opengl32, wglVmt* wgl);

SGL afxError _SglActivateDout(sglDpu* dpu, afxDrawOutput dout);

SGL afxNat _SglDdgeEnqueueMmapCb(afxDrawBridge ddge, afxBuffer buf, afxSize off, afxNat ran, afxFlags flags);


SGL void _SglCopyTexSubImage(sglDpu* dpu, GLenum glDstTarget, GLenum glSrcTarget, GLuint glSrcHandle, afxNat opCnt, afxRasterCopy const ops[]);
SGL void _SglUnpackTexSubImage(sglDpu* dpu, GLenum glTarget, afxNat bpp, GLenum glFmt, GLenum glType, afxNat opCnt, afxRasterIo const ops[]);

SGL void DpuBufRw(sglDpu* dpu, afxBuffer buf, afxNat offset, afxNat range, afxBool toHost, void* srcOrDst);

SGL void _DecodeCmdSwitchFrontFace(sglDpu* dpu, _sglCmdBool const *cmd);
SGL void _DecodeCmdSetCullMode(sglDpu* dpu, _sglCmdNat const *cmd);
SGL void _DecodeCmdSetPrimitiveTopology(sglDpu* dpu, _sglCmdNat const *cmd);
SGL void _DecodeCmdSetViewports(sglDpu* dpu, _sglCmdViewport const *cmd);
SGL void _DecodeCmdBindVertexSources(sglDpu* dpu, _sglCmdVertexSources const *cmd);
SGL void _DecodeCmdBindIndexSource(sglDpu* dpu, _sglCmdBufferRange const *cmd);
SGL void _DecodeCmdBindVertexInput(sglDpu* dpu, _sglCmdVertexInput *cmd);

SGL void _DpuBindVertexSources(sglDpu* dpu, afxNat first, afxNat cnt, sglBufferInfo const src[]);
SGL void _DpuSetViewports(sglDpu* dpu, afxNat first, afxNat cnt, afxViewport const vp[]);

SGL void _DecodeCmdDisableRasterization(sglDpu* dpu, _sglCmdBool const *cmd);
SGL void _DecodeCmdEnableDepthBias(sglDpu* dpu, _sglCmdBool const *cmd);
SGL void _DecodeCmdSetDepthBias(sglDpu* dpu, _sglCmdReal3 const *cmd);
SGL void _DecodeCmdSetLineWidth(sglDpu* dpu, _sglCmdReal const *cmd);
SGL void _DecodeCmdEnableStencilTest(sglDpu* dpu, _sglCmdBool const *cmd);
SGL void _DecodeCmdSetStencilCompareMask(sglDpu* dpu, _sglCmdBitmaskNat32 const *cmd);
SGL void _DecodeCmdSetStencilWriteMask(sglDpu* dpu, _sglCmdBitmaskNat32 const *cmd);
SGL void _DecodeCmdSetStencilReference(sglDpu* dpu, _sglCmdBitmaskNat32 const *cmd);
SGL void _DecodeCmdEnableDepthTest(sglDpu* dpu, _sglCmdBool const *cmd);
SGL void _DecodeCmdSetDepthCompareOp(sglDpu* dpu, _sglCmdNat const *cmd);
SGL void _DecodeCmdDisableDepthWrite(sglDpu* dpu, _sglCmdBool const *cmd);
SGL void _DecodeCmdSetBlendConstants(sglDpu* dpu, _sglCmdReal4 const *cmd);
SGL void _DecodeCmdSetScissors(sglDpu* dpu, _sglCmdScissor const *cmd);
SGL void _DecodeCmdSetCurtains(sglDpu* dpu, _sglCmdScissor const *cmd);

SGL void _DpuCmdRasSubsample(sglDpu* dpu, _sglCmdRegenerateMipmaps const* cmd);
SGL void _DpuCmdRasPack(sglDpu* dpu, _sglCmdPackRasterRegions const* cmd);
SGL void _DpuCmdRasCopy(sglDpu* dpu, _sglCmdCopyRasterRegions const* cmd);
SGL void _DpuCmdRasSwizzle(sglDpu* dpu, _sglCmdSwizzleRasterRegions const* cmd);
SGL void _DpuCmdRasXform(sglDpu* dpu, _sglCmdFlipRasterRegions const* cmd);

SGL void _DecodeCmdBufCpy(sglDpu* dpu, _sglCmdBufCpy const* cmd);
SGL void _DecodeCmdBufSet(sglDpu* dpu, _sglCmdBufSet const* cmd);
SGL void _DecodeCmdBufRw(sglDpu* dpu, _sglCmdBufRw const* cmd);

SGL void SglFlushXformStateChanges(sglDpu* dpu);
SGL void SglFlushRasterizationStateChanges(sglDpu* dpu);

SGL void _DecodeCmdNextPass(sglDpu* dpu, _sglCmd const *cmd);
SGL void _DecodeCmdFinishSynthesis(sglDpu* dpu, _sglCmd const *cmd);
SGL void _DecodeCmdBeginSynthesis(sglDpu* dpu, _sglCmdBeginSynthesis const *cmd);

SGL void _DecodeCmdBindRasters(sglDpu* dpu, _sglCmdBindRasters const *cmd);
SGL void _DecodeCmdBindBuffers(sglDpu* dpu, _sglCmdBindBuffers const *cmd);

SGL void _DecodeCmdBindPipeline(sglDpu* dpu, _sglCmdPipeline *cmd);
SGL void _DecodeCmdBindRasterizer(sglDpu* dpu, _sglCmdRasterizer *cmd);


SGL void _DecodeCmdDraw(sglDpu* dpu, _sglCmdDraw const *cmd);
SGL void _DecodeCmdDrawIndexed(sglDpu* dpu, _sglCmdDrawIndexed const* cmd);

SGL void _DpuBeginSynthesis(sglDpu* dpu, afxCanvas canv, afxRect const* area, afxNat layerCnt, afxNat cCnt, afxDrawTarget const* c, afxDrawTarget const* d, afxDrawTarget const* s, afxBool defFbo);
SGL void _DpuFinishSynthesis(sglDpu* dpu);

SGL void _DpuDraw(sglDpu* dpu, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst);
SGL void _DpuDrawIndexed(sglDpu* dpu, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst);

SGL void _DpuBindRasters(sglDpu* dpu, afxNat set, afxNat first, afxNat cnt, afxSampler samplers[], afxRaster rasters[]);
SGL void _DpuBindBuffers(sglDpu* dpu, afxNat set, afxNat first, afxNat cnt, afxBuffer buffers[], afxNat const offsets[], afxNat const ranges[]);

SGL void _DpuBindPipeline(sglDpu* dpu, afxPipeline pip, afxFlags dynamics);
SGL void _DpuBindRasterizer(sglDpu* dpu, afxRasterizer razr, afxFlags dynamics);

#endif//AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H
