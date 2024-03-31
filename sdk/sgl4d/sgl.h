/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H
#define AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

//#include "../src/e2coree/deps/gl/glcorearb.h"
//#include "../src/e2coree/deps/gl/wglext.h"

#include "../dep/gl/glad.h"
#include "../dep/gl/glad_wgl.h"

//#define DISABLE_SHARED_HARDWARE_CONTEXT

#define _AFX_DRAW_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
//#define _AFX_MODULE_C

#define _AFX_DRAW_DEVICE_C
#define _AFX_DRAW_DEVICE_IMPL
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_CONTEXT_IMPL
#define _AFX_DRAW_OUTPUT_C
#define _AFX_DRAW_OUTPUT_IMPL
#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_INPUT_IMPL
#define _AFX_PIPELINE_C
#define _AFX_PIPELINE_IMPL
#define _AFX_BUFFER_C
#define _AFX_BUFFER_IMPL
#define _AFX_RASTER_C
#define _AFX_RASTER_IMPL
#define _AFX_RASTERIZER_C
#define _AFX_RASTERIZER_IMPL
#define _AFX_VERTEX_INPUT_C
#define _AFX_VERTEX_INPUT_IMPL
#define _AFX_DRAW_STREAM_C
#define _AFX_DRAW_STREAM_IMPL
#define _AFX_SAMPLER_C
#define _AFX_SAMPLER_IMPL
#define _AFX_SHADER_C
#define _AFX_SHADER_IMPL
#define _AFX_SAMPLER_C
#define _AFX_SAMPLER_IMPL
#define _AFX_CANVAS_C
#define _AFX_CANVAS_IMPL
#define _AFX_BIND_SCHEMA_C
#define _AFX_BIND_SCHEMA_IMPL
#define _AFX_DRAW_BRIDGE_C
#define _AFX_DRAW_BRIDGE_IMPL
#define _AFX_FENCE_C
#define _AFX_FENCE_IMPL
#define _AFX_SEMAPHORE_C
#define _AFX_SEMAPHORE_IMPL
#define _AFX_QUERY_POOL_C
#define _AFX_QUERY_POOL_IMPL

#include "qwadro/core/afxManager.h"
#include "qwadro/mem/afxArena.h"
#include "qwadro/mem/afxQueue.h"
#include "qwadro/draw/pipe/afxRasterizer.h"
#include "qwadro/draw/pipe/afxCanvas.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/pipe/afxDrawOps.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/pipe/afxSampler.h"
#include "qwadro/draw/pipe/afxDrawStream.h"
#include "qwadro/draw/pipe/afxVertexInput.h"
#include "qwadro/core/afxSystem.h"
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
    SGL_UPD_FLAG_DEVICE_FLUSH   = AFX_BIT_OFFSET(0), // flush from host to device
    SGL_UPD_FLAG_HOST_FLUSH     = AFX_BIT_OFFSET(1), // flush from device to host
    SGL_UPD_FLAG_DEVICE_INST    = AFX_BIT_OFFSET(2), // (re)instantiate on device
    SGL_UPD_FLAG_HOST_INST      = AFX_BIT_OFFSET(3), // (re)instantiate on host
    
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
    glVmt const             gl;    
    wglVmt const            wgl;
    HWND                    wnd;
    HDC                     dc;
    int                     dcPxlFmt;
    PIXELFORMATDESCRIPTOR   dcPfd;
    HGLRC                   glrc;
    HMODULE                 opengl32;
    afxNat                  verMajor, verMinor, verPatch;
    afxString               subsysName;
    afxString               subsysVer;

    afxNat                  assignedTxuUniqueId;
    afxNat                  portIdx;
    afxSlock                deletionLock;
    afxQueue                deletionQueue;

    afxDrawContext          activeDctx;

    afxSize                 numOfFedVertices, numOfFedIndices, numOfFedInstances;
 
    sglXfrmState            activeXformState, nextXformState;
    sglRasterizerState      activeRasterState, nextRasterState;

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

    afxBuffer               activePixelUnpackBuf;
    afxBuffer               activePixelPackBuf;

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
} sglDpuIdd;

struct _afxDdevIdd
{
    WNDCLASSEX     wndClss;
    afxNat          dpuCnt;
    sglDpuIdd*      dpus;
    afxMutex        ioConMtx;

};

AFX_OBJECT(afxDrawContext)
{
    struct afxBaseDrawContext base;
    // presentation stuff, only on unit 0.
    afxBool presentationSuspended;
    //GLuint presentFboGpuHandle;
    afxPipeline presentPip;
    afxSampler  presentSmp;
    
    afxPipeline fntPip;
    afxSampler  fntSamp;    
    afxRaster   fntRas;
    afxBuffer   fntDataBuf;
    afxBuffer   fntUnifBuf;

    //afxBuffer tristrippedQuad2dPosBuf;
};

AFX_OBJECT(afxDrawBridge)
{
    struct afxBaseDrawBridge base;
};

AFX_DEFINE_STRUCT(_sglQueueing)
{
    afxLinkage              chain;
    afxNat                  submNo; // this submission number ordinal (B2F)
    afxNat                  reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime                 pushTime; // submission (into input) time
    afxTime                 pullTime; // fecth (by queue) time
    afxTime                 complTime; // completation time
    afxError(*exec)(sglDpuIdd*, _sglQueueing*);
};

AFX_DEFINE_STRUCT(_sglQueueingExecute)
{
    _sglQueueing            hdr;
    afxNat                  itemCnt;
    afxExecutionRequest     afxSimd(items[]);
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

AFX_DEFINE_STRUCT(_sglDeleteGlRes)
{
    union
    {
        GLuint  gpuHandle;
        void*   gpuHandlePtr;
    };
    GLuint  type; // 0 buf, 1 tex, 2 surf, 3 canv, 4 smp, 5 pip, 6 shd, 7 shd (separate) program
};

typedef struct _afxDoutIdd
{
    afxFcc      fcc;
    afxBool     instanced;

    HWND        wnd;
    HDC         dc;
    int         dcPxlFmt;
    
    HDC         dcDeskBkp;
    int         dcPxlFmtDeskBkp;
} _afxDoutIdd;

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

AFX_OBJECT(afxBindSchema)
{
    struct afxBaseBindSchema base;
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
    
    afxSize         lastUpdOffset;
    afxNat          lastUpdRange;
    afxBool         locked;
};

AFX_OBJECT(afxRaster)
{
    struct afxBaseRaster base;
    sglUpdateFlags  updFlags;
    GLuint          glHandle;
    GLuint          glHandleAux; // for draw buffers
    GLint           glIntFmt;
    GLenum          glTarget, glFmt, glType;
    
    afxNat          lastUpdLodBase, lastUpdLodRange;
    afxNat          lastUpdImgBase, lastUpdImgRange;
    afxNat          lastUpdOffset[3];
    afxNat          lastUpdRange[3];
    afxBool         locked;
};

AFX_OBJECT(afxCanvas)
{
    struct afxBaseCanvas base;
    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    afxMask         storeBitmask;
};

AFX_OBJECT(afxDrawStream)
{
    struct afxBaseDrawStream    base;
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
    _sglCmd                 cmd;
    afxBool                 defFbo;
    afxLinkage              canvOpsLnk;
    afxCanvas               canv;
    afxRect                 area;
    afxNat32                layerCnt;
    afxDrawTarget           depth;
    afxDrawTarget           stencil;
    afxNat32                rasterCnt;
    afxDrawTarget           raster0;
    afxDrawTarget           afxSimd(raster_[]);
};

AFX_DEFINE_STRUCT(_sglCmdBindRasters)
{
    _sglCmd                     cmd;
    afxNat32                        set;
    afxNat32                        first, cnt;
    afxSampler                      afxSimd(smp[_SGL_MAX_ENTRY_PER_LEGO]);
    afxRaster                      afxSimd(tex[_SGL_MAX_ENTRY_PER_LEGO]);
};

AFX_DEFINE_STRUCT(_sglCmdBindBuffers)
{
    _sglCmd                     cmd;
    afxNat32                        set;
    afxNat32                        first, cnt;
    afxBuffer                       afxSimd(buf[_SGL_MAX_ENTRY_PER_LEGO]);
    afxNat32                        afxSimd(offset[_SGL_MAX_ENTRY_PER_LEGO]);
    afxNat32                        afxSimd(range[_SGL_MAX_ENTRY_PER_LEGO]);
};

AFX_DEFINE_STRUCT(_sglCmdVertexSources)
{
    _sglCmd                     cmd;
    afxNat32                        first, cnt;
    //afxVertexInputSource            spec[SGL_MAX_VERTEX_ATTRIB_BINDINGS];
    struct
    {
        afxBuffer       buf;
        afxNat32        offset;
        afxNat32        range;
        afxNat32        stride;
    }                   source0;
    struct
    {
        afxBuffer       buf;
        afxNat32        offset;
        afxNat32        range;
        afxNat32        stride;
    }                   afxSimd(source_[]);
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

AFX_DEFINE_STRUCT(_sglCmdPipeline)
{
    _sglCmd                     cmd;
    afxNat                          segment;
    afxPipeline                     pip;
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
    afxRasterIoOp afxSimd(ops[]);
};

AFX_DEFINE_STRUCT(_sglCmdRwRasterRegions)
{
    _sglCmd         cmd;
    afxBool         down;
    afxRaster ras;
    afxStream ios;
    afxCodec cdc;
    afxNat opCnt;
    afxRasterIoOp afxSimd(ops[]);
};

AFX_DEFINE_STRUCT(_sglCmdCopyRasterRegions)
{
    _sglCmd         cmd;
    afxRaster           src;
    afxRaster           dst;
    afxNat              opCnt;
    afxRasterCopyOp     afxSimd(ops[]);
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
    _sglCmd                     cmd;
    afxNat32                        first, cnt;
    afxBool                         reset;
    afxRect                         afxSimd(rect[]);
};

AFX_DEFINE_STRUCT(_sglCmdArea)
{
    _sglCmd                     cmd;
    afxBool                         exclusive;
    afxNat32                        first, cnt;
    afxBool                         reset;
    afxRect                         afxSimd(rect[]);
};

AFX_DEFINE_STRUCT(_sglCmdViewport)
{
    _sglCmd                     cmd;
    afxNat32                        first, cnt;
    afxBool                         reset;
    afxViewport                     vp0;
    afxViewport                     afxSimd(vp_[]);
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
    afxDrawStream                   afxSimd(subsets[8]);
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
    sglBindFlag_BIND,
    sglBindFlag_SYNC,
    sglBindFlag_KEEP,
}sglBindFlags;

_SGL afxError AfxRegisterDrawDrivers(afxModule mdle, afxDrawSystem dsys);

SGL afxError _SglDpuBindAndSyncSamp(sglDpuIdd* dpu, sglBindFlags bindFlags, afxNat glUnit, afxSampler samp);
SGL afxError _SglDpuSyncShd(sglDpuIdd* dpu, afxShader shd, afxShaderStage stage, glVmt const* gl);
SGL afxError _SglDpuSurfSync(sglDpuIdd* dpu, afxSurface surf, glVmt const* gl); // must be used before texUpdate
SGL afxError _SglBindAndSyncRas(sglDpuIdd* dpu, sglBindFlags bindFlags, afxNat glUnit, afxRaster tex);
SGL afxError _SglTexSubImage(glVmt const* gl, GLenum glTarget, GLint level, afxNat baseLayer, afxNat layerCnt, afxWhd const xyz, afxWhd const whd, GLenum glFmt, GLenum glType, afxAddress const src);

SGL afxError _SglDpuBindAndSyncVin(sglDpuIdd* dpu, afxVertexInput vin, sglVertexInputState* nextVinBindings);
SGL afxError _SglDpuBindAndSyncPip(sglDpuIdd* dpu, afxBool bind, afxBool sync, afxPipeline pip);
SGL afxError _SglDpuBindAndSyncRazr(sglDpuIdd* dpu, afxRasterizer razr);
SGL afxError _SglDpuBindAndResolveLego(sglDpuIdd* dpu, GLuint glHandle, afxNat unit, afxBindSchema legt, glVmt const* gl);
SGL afxError _SglDpuBindAndSyncCanv(sglDpuIdd* dpu, afxBool bind, afxBool sync, GLenum glTarget, afxCanvas canv);
SGL afxError _SglBindAndSyncBuf(sglDpuIdd* dpu, sglBindFlags bindFlags, GLenum glTarget, afxBuffer buf, afxNat offset, afxNat range, afxNat stride, GLenum usage);

//SGL afxSize _AfxMeasureTextureRegion(afxRaster tex, afxRasterRegion const *rgn);

SGL BOOL SglMakeCurrent(HDC hdc, HGLRC hrc, sglDpuIdd const *dpu);
SGL afxError SglSwapBuffers(HDC hdc, sglDpuIdd const *dpu);
SGL int SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd, sglDpuIdd const *dpu);
SGL BOOL SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd, sglDpuIdd const *dpu);
SGL int SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd, sglDpuIdd const *dpu);
SGL int SglGetPixelFormat(HDC hdc, sglDpuIdd const *dpu);

SGL afxBindSchema _SglDrawContextFindLego(afxDrawContext dctx, afxNat bindCnt, afxPipelineRigBindingDecl const bindings[]);

SGL afxBool _SglDqueVmtSubmitCb(afxDrawContext dctx, afxDrawBridge dxge, afxDrawSubmissionSpecification const *spec, afxNat *submNo);
SGL afxError _SglDdevProcDpuCb(afxDrawThread dthr, afxDrawContext dctx);

SGL afxClassConfig const _SglDctxClsConfig;

SGL afxError _SglDoutVmtFlushCb(afxDrawOutput dout, afxTime timeout);

SGL afxError _SglDdevDeinitDout(afxDrawDevice ddev, afxDrawOutput dout);
SGL afxError _SglDdevInitDout(afxDrawDevice ddev, afxDrawOutput dout, afxDrawOutputConfig const* config, afxUri const* endpoint);
SGL afxError _SglDinFreeAllBuffers(afxDrawInput din);

SGL afxCmd _SglEncodeCmdVmt;
SGL afxCmdBuffer const _SglEncodeCmdBufferVmt;
SGL afxCmdRaster const _SglEncodeCmdRasterVmt;
SGL afxCmdTransformation const _SglEncodeCmdTransformationVmt;
SGL afxCmdRasterization const _SglEncodeCmdRasterizationVmt;
SGL void _SglBindFboAttachment(glVmt const* gl, GLenum glTarget, GLenum glAttachment, GLenum texTarget, GLuint texHandle, GLint level, GLuint layer, GLuint z);

SGL afxError _SglWaitFenc(afxBool waitAll, afxNat64 timeout, afxNat cnt, afxFence const fences[]);
SGL afxError _SglResetFenc(afxNat cnt, afxFence const fences[]);

SGL afxCmdId _SglEncodeCmdCommand(afxDrawStream diob, afxNat id, afxNat siz, _sglCmd *cmd);

SGL afxClassConfig const _SglFencClsConfig;
SGL afxClassConfig const _SglSemClsConfig;
SGL afxClassConfig const _SglQrypClsConfig;

SGL afxResult _SglDdevIoctl(afxDrawDevice ddev, afxNat reqCode, va_list va);

#endif//AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H