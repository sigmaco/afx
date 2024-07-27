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

//#define _AUX_UX_C
//#define _AUX_OVERLAY_IMPL
//#define _AUX_OVERLAY_C

#include "../src/afx/draw/dev/AvxDevKit.h"
#include "qwadro/ux/afxScript.h"
#include "AvxToGlMapping.h"

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

#define FORCE_GL_GENERIC_FUNCS 1 // we also attempt to use specific functions as a medium of hinting for the driver.
#define SGL_DONT_USE_ROBUST_ACCESS 1

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
        avxSampler          smp;
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
    avxViewport         vps[SGL_MAX_VIEWPORTS];

    avxTopology     primTop; /// is a option defining the primitive topology. /// avxTopology_TRI_LIST
    afxBool             primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
    afxNat              patchControlPoints; /// is the number of control points per patch.

    afxBool             depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE

    avxCullMode         cullMode; /// is the triangle facing direction used for primitive culling. /// avxCullMode_BACK
    afxBool             cwFrontFacing; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)

} sglXfrmState;

typedef struct sglRasterizerState
{
    afxBool             rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE

    struct
    {
        avxCanvas       canv;
        afxRect         area;
        afxNat32        layerCnt;
        afxNat32        rasterCnt;
        avxDrawTarget   rasters[SGL_MAX_COLOR_ATTACHMENTS];
        avxDrawTarget   ds[2];
        afxBool         defFbo;
    }                   pass; // always active pass

    avxFillMode         fillMode; /// is the triangle rendering mode. /// avxFillMode_SOLID
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
    avxCompareOp        depthCompareOp; /// is a value specifying the comparison operator to use in the Depth Comparison step of the depth test. /// avxCompareOp_LESS
    afxBool             depthWriteDisabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. /// FALSE
    afxBool             depthBoundsTestEnabled; /// controls whether depth bounds testing is enabled. /// FALSE
    afxV2d              depthBounds; /// is the minimum depth bound used in the depth bounds test. /// [ min, max ]
    afxBool             stencilTestEnabled; /// FALSE
    avxStencilConfig    stencilFront; /// is the configuration values controlling the corresponding parameters of the stencil test.
    avxStencilConfig    stencilBack; /// is the configuration controlling the corresponding parameters of the stencil test.


    afxNat              areaCnt; /// 0
    afxRect             areaRects[SGL_MAX_VIEWPORTS];
    afxNat              scisCnt; /// 0
    afxRect             scisRects[SGL_MAX_VIEWPORTS];

    afxNat              outCnt;
    avxColorOutputChannel   outs[8];
    afxBool             anyBlendEnabled;
    afxReal             blendConstants[4]; /// [ 0, 0, 0, 1 ]
    afxBool             logicOpEnabled; /// FALSE
    avxLogicOp          logicOp; /// avxLogicOp_NOP
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

    avxRasterizer           activeRazr, nextRazr;
    avxPipeline             activePip, nextPip;
    avxVertexInput          activeVin, nextVin;
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
            avxSampler      smp;
            afxRaster      ras;
            afxNat          subrasIdx;
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

struct _afxDoutIdd
{
    HINSTANCE       hInst;
    HWND            hWnd;
    HDC             hDc;
    int             dcPxlFmt;
    int             dcPxlFmtBkp;
    afxBool         isWpp;
};

AFX_OBJECT(afxDrawContext)
{
    AFX_OBJECT(_avxDrawContext) m;
    // presentation stuff, only on unit 0.
    afxBool presentationSuspended;
    //GLuint presentFboGpuHandle;
    avxRasterizer presentRazr;
    avxSampler  presentSmp;
    
    avxRasterizer fntRazr;
    avxSampler  fntSamp;    
    afxRaster   fntRas;
    afxBuffer   fntDataBuf;
    afxBuffer   fntUnifBuf;

    //afxBuffer tristrippedQuad2dPosBuf;
};

AFX_OBJECT(afxDrawBridge)
{
    AFX_OBJECT(_avxDrawBridge) m;
};

AFX_OBJECT(afxDrawQueue)
{
    AFX_OBJECT(_avxDrawQueue) m;
};

AFX_DEFINE_STRUCT(_sglQueueing)
{
    afxNat      siz;
    afxLinkage  chain;
    afxNat      submNo; // this submission number ordinal (B2F)
    afxNat      reqSubmNo; // required submission num ordinal (need be executed before this). Usually submissions of resource benefiting of fetch priority.
    afxTime     pushTime; // submission (into input) time
    afxTime     pullTime; // fecth (by queue) time
    afxTime     complTime; // completation time
    afxError    (*exec)(sglDpu*, afxDrawBridge, afxNat queIdx, _sglQueueing*);
};

AFX_DEFINE_STRUCT(_sglQueueingExecute)
{
    _sglQueueing            hdr;
    afxNat                  itemCnt;
    afxExecutionRequest AFX_SIMD items[];
};

AFX_DEFINE_STRUCT(_sglQueueingWork)
{
    _sglQueueing            hdr;
    avxSubmission           subm;
    void*                   data[];
};

AFX_DEFINE_STRUCT(_sglQueueingTransfer)
{
    _sglQueueing            hdr;
    afxNat                  itemCnt;
    afxTransferRequest AFX_SIMD items[];
};

AFX_DEFINE_STRUCT(_sglQueueingPresent)
{
    _sglQueueing            hdr;
    afxNat                  itemCnt;
    afxPresentationRequest AFX_SIMD items[];
};

AFX_DEFINE_STRUCT(_sglQueueingStamp)
{
    _sglQueueing            hdr;
    afxLinkage              echoLnk;
    afxV2d                  origin;
    afxString4096           caption;
    afxNat                  itemCnt;
    afxPresentationRequest AFX_SIMD items[];
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

AFX_OBJECT(avxPipeline)
{
    AFX_OBJECT(_avxPipeline) m;
    sglUpdateFlags  updFlags;
    GLuint          glHandle;
    //sglVertexInput  vertexInput;
};

AFX_OBJECT(avxVertexInput)
{
    AFX_OBJECT(_avxVertexInput) m;
    sglUpdateFlags  updFlags;
    GLuint          glHandle;
    sglVertexInputState bindings;
};

AFX_OBJECT(avxRasterizer)
{
    AFX_OBJECT(_avxRasterizer) m;
    sglUpdateFlags  updFlags;

};

AFX_OBJECT(avxQueryPool)
{
    AFX_OBJECT(_avxQueryPool) m;
    sglUpdateFlags  updFlags;
    GLuint         *glHandle; // * cap
    GLenum          glTarget;
};

AFX_OBJECT(avxLigature)
{
    AFX_OBJECT(_avxLigature) m;
    int a;
};

AFX_OBJECT(avxShader)
{
    AFX_OBJECT(_avxShader) m;
    sglUpdateFlags      updFlags;
    afxNat              glHandle;
    afxNat              glProgHandle;
    afxBool             compiled;
};

AFX_OBJECT(avxSampler)
{
    AFX_OBJECT(_avxSampler) m;
    sglUpdateFlags  updFlags;
    afxNat          glHandle;
};

AFX_OBJECT(afxFence)
{
    AFX_OBJECT(_afxFence) m;
    sglUpdateFlags  updFlags;
    GLsync          glHandle;
};

AFX_OBJECT(afxSemaphore)
{
    AFX_OBJECT(_afxSemaphore) m;
};

AFX_OBJECT(afxBuffer)
{
    AFX_OBJECT(_avxBuffer) m;
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
    AFX_OBJECT(_avxRaster) m;
    sglUpdateFlags  updFlags;
    GLuint          glHandle;
    GLuint          glHandleAux; // for draw buffers
    GLint           glIntFmt;
    GLenum          glTarget, glFmt, glType;
};

AFX_OBJECT(avxCanvas)
{
    AFX_OBJECT(_avxCanvas) m;
    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    afxMask         storeBitmask;
};

AFX_OBJECT(avxCmdb)
{
    AFX_OBJECT(_avxCmdb) m;
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
            avxPipeline     next;
            avxPipeline     curr;
        } pip;
    } levelCaches[1];
};

typedef struct _DrawPipelineResource
{
    avxShaderStage stages;
    avxShaderResourceType resType;
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

AFX_DEFINE_UNION(sglDecodeVmt)
{
    afxCmd  cmd;
    void(*f[SGL_CMD_TOTAL])(sglDpu* dpu, _sglCmd *cmd);
};

AFX_DEFINE_STRUCT(_sglCmdEcho)
{
    _sglCmd                     cmd;
    afxLinkage                  echoLnk;
    afxM4d                      m;
    afxString                 caption;
    afxChar            AFX_SIMD strBuf[];
};

AFX_DEFINE_STRUCT(_sglCmdEchoFmt)
{
    _sglCmd                     cmd;
    afxM4d                      m;
    afxString                 caption;
    afxChar            AFX_SIMD strBuf[];
};

AFX_DEFINE_STRUCT(_sglCmdDraw)
{
    _sglCmd                     cmd;
    avxDrawIndirectCmd          data;
};

AFX_DEFINE_STRUCT(_sglCmdDrawIndirect)
{
    _sglCmd                     cmd;
    afxBuffer buf;
    afxNat32 offset;
    afxNat32 drawCnt;
    afxNat32 stride;
};

AFX_DEFINE_STRUCT(_sglCmdDrawIndirectCnt)
{
    _sglCmd                     cmd;
    afxBuffer buf;
    afxNat32 offset;
    afxBuffer cntBuf;
    afxNat32 cntBufOff;
    afxNat32 maxDrawCnt;
    afxNat32 stride;
};

AFX_DEFINE_STRUCT(_sglCmdDrawIndexed)
{
    _sglCmd                     cmd;
    avxDrawIndexedIndirectCmd   data;
};

AFX_DEFINE_STRUCT(_sglCmdDrawIndexedIndirect)
{
    _sglCmd                     cmd;
    afxBuffer buf;
    afxNat32 offset;
    afxNat32 drawCnt;
    afxNat32 stride;
};

AFX_DEFINE_STRUCT(_sglCmdDrawIndexedIndirectCnt)
{
    _sglCmd                     cmd;
    afxBuffer buf;
    afxNat32 offset;
    afxBuffer cntBuf;
    afxNat32 cntBufOff;
    afxNat32 maxDrawCnt;
    afxNat32 stride;
};

AFX_DEFINE_STRUCT(_sglCmdDispatch)
{
    _sglCmd                     cmd;
    avxDispatchIndirectCmd      data;
};

AFX_DEFINE_STRUCT(_sglCmdDispatchIndirect)
{
    _sglCmd                     cmd;
    afxBuffer buf;
    afxNat32 offset;
};

AFX_DEFINE_STRUCT(_sglCmdBeginSynthesis)
{
    _sglCmd cmd;
    afxBool defFbo;
    afxLinkage canvOpsLnk;
    avxCanvas canv;
    afxRect area;
    afxNat32 layerCnt;
    avxDrawTarget depth;
    avxDrawTarget stencil;
    afxNat32 rasterCnt;
    avxDrawTarget AFX_SIMD rasters[];
};

AFX_DEFINE_STRUCT(_sglCmdBindRasters)
{
    _sglCmd cmd;
    afxNat32 set;
    afxNat32 first, cnt;
    afxRaster AFX_SIMD tex[_SGL_MAX_ENTRY_PER_LEGO];
    afxNat subras[_SGL_MAX_ENTRY_PER_LEGO];
};

AFX_DEFINE_STRUCT(_sglCmdBindSamplers)
{
    _sglCmd cmd;
    afxNat32 set;
    afxNat32 first, cnt;
    avxSampler AFX_SIMD smp[_SGL_MAX_ENTRY_PER_LEGO];
};

AFX_DEFINE_STRUCT(_sglCmdBindBuffers)
{
    _sglCmd cmd;
    afxNat32 set;
    afxNat32 first, cnt;
    afxBuffer AFX_SIMD buf[_SGL_MAX_ENTRY_PER_LEGO];
    afxNat32 AFX_SIMD offset[_SGL_MAX_ENTRY_PER_LEGO];
    afxNat32 AFX_SIMD range[_SGL_MAX_ENTRY_PER_LEGO];
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
    sglBufferInfo AFX_SIMD info[];
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
    }   AFX_SIMD streams[];
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
    }   AFX_SIMD attrs[];
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
    avxRasterizer               razr;
    avxVertexInput              vin;
    afxFlags                    dynamics;
};

AFX_DEFINE_STRUCT(_sglCmdPipeline)
{
    _sglCmd                     cmd;
    afxNat                      segment;
    avxPipeline                 pip;
    avxVertexInput              vin;
    afxFlags                    dynamics;
};

AFX_DEFINE_STRUCT(_sglCmdVertexInput)
{
    _sglCmd                     cmd;
    avxVertexInput              vin;
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
    afxRasterIo AFX_SIMD ops[];
};

AFX_DEFINE_STRUCT(_sglCmdRwRasterRegions)
{
    _sglCmd         cmd;
    afxBool         down;
    afxRaster ras;
    afxStream ios;
    afxCodec cdc;
    afxNat opCnt;
    afxRasterIo AFX_SIMD ops[];
};

AFX_DEFINE_STRUCT(_sglCmdCopyRasterRegions)
{
    _sglCmd         cmd;
    afxRaster           src;
    afxRaster           dst;
    afxNat              opCnt;
    afxRasterCopy AFX_SIMD ops[];
};

AFX_DEFINE_STRUCT(_sglCmdRegenerateMipmaps)
{
    _sglCmd             cmd;
    afxRaster           ras;
    afxNat              baseLod;
    afxNat              lodCnt;
};

AFX_DEFINE_STRUCT(_sglCmdSwizzleRasterRegions)
{
    _sglCmd             cmd;
    afxRaster           ras;
    avxColorSwizzle     a;
    avxColorSwizzle     b;
    afxNat              rgnCnt;
    afxRasterRegion AFX_SIMD rgn[];
};

AFX_DEFINE_STRUCT(_sglCmdFlipRasterRegions)
{
    _sglCmd             cmd;
    afxRaster           ras;
    afxM4d              m;
    afxNat              rgnCnt;
    afxRasterRegion AFX_SIMD rgn[];
};

AFX_DEFINE_STRUCT(_sglCmdBufIo)
{
    _sglCmd         cmd;
    afxBuffer       buf;
    afxBool         export;
    afxStream       io;
    afxNat          opCnt;
    afxBufferIo AFX_SIMD ops[];
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
        afxByte AFX_SIMD src[];
    };
};

AFX_DEFINE_STRUCT(_sglCmdBufCpy)
{
    _sglCmd         cmd;
    afxBuffer src;
    afxBuffer dst;
    afxNat opCnt;
    afxBufferCopyOp AFX_SIMD ops[];
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
    afxRect AFX_SIMD rect[];
};

AFX_DEFINE_STRUCT(_sglCmdViewport)
{
    _sglCmd cmd;
    afxNat32 first, cnt;
    avxViewport AFX_SIMD vp[];
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
    avxCmdb AFX_SIMD subsets[8];
};

AFX_DEFINE_STRUCT(_sglCmdCopyTex)
{
    _sglCmd                     cmd;
    afxRaster                      dst;
    afxRaster                      src;
    afxNat32                        rgnCnt;
    afxRasterRegion AFX_SIMD rgn[16];
};

AFX_DEFINE_STRUCT(_sglCmdUniformVectorEXT)
{
    _sglCmd                     cmd;
    afxChar const*                  name;
    afxNat                          cnt;
    afxReal AFX_SIMD v[4];
};

AFX_DEFINE_STRUCT(_sglCmdUniformMatrixEXT)
{
    _sglCmd                     cmd;
    afxChar const*                  name;
    afxNat                          cnt;
    afxReal AFX_SIMD m[4][4];
};


// vkCmdBindDescriptorSets - Binds descriptor sets to a command buffer

//SGL afxError _SglCreateCombinedDeviceContext(WNDCLASSEXA *oglWndClss, HGLRC shareCtx, HWND *phwnd, HDC *phdc, HGLRC *phrc, wglVmt const* wgl, HMODULE opengl32);

SGL void _SglDctxDeleteGlRes(afxDrawContext dctx, afxNat type, afxSize gpuHandle);


typedef enum sglBindFlags
{
    sglBindFlag_BIND = AFX_BIT(0),
    sglBindFlag_SYNC = AFX_BIT(1),
    sglBindFlag_KEEP = AFX_BIT(2)
}sglBindFlags;

_SGL afxError AfxRegisterDrawDrivers(afxModule mdle, afxDrawSystem dsys);

SGL afxError _DpuBindAndSyncSamp(sglDpu* dpu, afxNat glUnit, avxSampler samp);
SGL afxError _DpuSyncShd(sglDpu* dpu, avxShader shd, avxShaderStage stage, glVmt const* gl);
SGL afxError _DpuSurfSync(sglDpu* dpu, afxRasterSlot surf, glVmt const* gl); // must be used before texUpdate
SGL afxError DpuBindAndSyncRas(sglDpu* dpu, afxNat glUnit, afxRaster tex);
SGL afxError _SglTexSubImage(glVmt const* gl, GLenum glTarget, afxRasterRegion const* rgn, GLenum glFmt, GLenum glType, afxAddress const src);

SGL afxError _DpuBindAndSyncVin(sglDpu* dpu, avxVertexInput vin, sglVertexInputState* nextVinBindings);
SGL afxError _DpuBindAndSyncPip(sglDpu* dpu, afxBool bind, afxBool sync, avxPipeline pip);
SGL afxError _DpuBindAndSyncRazr(sglDpu* dpu, avxRasterizer razr);
SGL afxError _DpuBindAndResolveLego(sglDpu* dpu, avxLigature legt, GLuint glHandle);
SGL afxError _DpuBindAndSyncCanv(sglDpu* dpu, afxBool bind, afxBool sync, GLenum glTarget, avxCanvas canv);


SGL afxError DpuBindAndSyncBuf(sglDpu* dpu, GLenum glTarget, afxBuffer buf);
SGL afxError _DpuRemapBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxFlags flags);
SGL afxError _DpuStoreBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxByte* dst);
SGL afxError _DpuLoadBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxByte const* src);
SGL afxError _DpuOutputBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxStream out, afxSize at);
SGL afxError _DpuInputBuf(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxStream in, afxSize at);

SGL afxError _DpuLoadBuf2(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxNat dstStride, afxByte const* src, afxNat srcStride);
SGL afxError _DpuStoreBuf2(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxNat srcStride, afxByte* dst, afxNat dstStride);
SGL afxError _DpuOutputBuf2(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxNat srcStride, afxStream out, afxSize at, afxNat dstStride);
SGL afxError _DpuInputBuf2(sglDpu* dpu, afxBuffer buf, afxSize offset, afxNat range, afxNat dstStride, afxStream in, afxSize at, afxNat srcStride);

SGL afxError _DpuLoadRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxByte const* src);
SGL afxError _DpuStoreRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxByte* dst);
SGL afxError _DpuPackRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxBuffer buf);
SGL afxError _DpuUnpackRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxBuffer buf);
SGL afxError _DpuOutputRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxStream out);
SGL afxError _DpuInputRas(sglDpu* dpu, afxRaster ras, afxRasterIo const* op, afxStream in);

SGL afxError _DpuCopyRas(sglDpu* dpu, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopy const ops[]);

SGL afxError _DpuPresentDout(sglDpu* dpu, afxDrawOutput dout, afxNat outBufIdx);

//SGL afxSize _AfxMeasureTextureRegion(afxRaster tex, afxRasterRegion const *rgn);

SGL BOOL SglMakeCurrent(HDC hdc, HGLRC hrc, wglVmt const* vmt);
SGL afxError SglSwapBuffers(HDC hdc, wglVmt const* vmt);
SGL int SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd, wglVmt const* vmt);
SGL BOOL SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd, wglVmt const* vmt);
SGL int SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd, wglVmt const* vmt);
SGL int SglGetPixelFormat(HDC hdc, wglVmt const* vmt);

SGL avxLigature _SglDrawContextFindLego(afxDrawContext dctx, afxNat bindCnt, avxPipelineRigBindingDecl const bindings[]);

SGL afxError _SglDdevOpenCb(afxDrawDevice ddev, afxDrawContext dctx, afxCookie const* cookie);
SGL afxError _SglDdevCloseCb(afxDrawDevice ddev, afxDrawContext dctx);

//SGL afxBool _SglDqueVmtSubmitCb(afxDrawContext dctx, afxDrawBridge ddge, afxDrawSubmissionSpecification const *spec, afxNat *submNo);
SGL afxBool _DdgeProcCb(afxDrawBridge ddge, afxThread thr);

SGL afxClassConfig const _SglDctxMgrCfg;

SGL afxError _SglDoutVmtFlushCb(afxDrawOutput dout, afxTime timeout);

SGL afxError _SglDdevOpenDinCb(afxDrawDevice ddev, afxDrawInput din, afxDrawInputConfig const* cfg, afxUri const* endpoint);
SGL afxError _SglDdevCloseDoutCb(afxDrawDevice ddev, afxDrawOutput dout);
SGL afxError _SglDdevOpenDoutCb(afxDrawDevice ddev, afxDrawOutput dout, afxDrawOutputConfig const* config, afxUri const* endpoint);
SGL afxError _SglDinFreeAllBuffers(afxDrawInput din);

SGL afxCmd _SglEncodeCmdVmt;
SGL void _SglBindFboAttachment(glVmt const* gl, GLenum glTarget, GLenum glAttachment, GLenum texTarget, GLuint texHandle, GLint level, GLuint layer, GLuint z);

SGL afxError _SglWaitFenc(afxBool waitAll, afxNat64 timeout, afxNat cnt, afxFence const fences[]);
SGL afxError _SglResetFenc(afxNat cnt, afxFence const fences[]);

SGL afxCmdId _SglEncodeCmdCommand(avxCmdb cmdb, afxNat id, afxNat siz, _sglCmd *cmd);

SGL afxClassConfig const _SglFencMgrCfg;
SGL afxClassConfig const _SglSemMgrCfg;

SGL afxResult _SglDdevIoctl(afxDrawDevice ddev, afxNat reqCode, va_list va);

SGL const char *glVmtNames[];
SGL void _LoadWglExtendedSymbols(HMODULE opengl32, wglVmt* wgl);
SGL afxError SglLoadGlVmt(HMODULE opengl32, afxNat base, afxNat cnt, void* vmt[], afxBool echo);
SGL afxError SglLoadWglVmt(HMODULE opengl32, afxNat base, afxNat cnt, void* vmt[], afxBool echo);
SGL void _LoadWglBaseSymbols(HMODULE opengl32, wglVmt* wgl);

SGL afxError _SglActivateDout(sglDpu* dpu, afxDrawOutput dout);

SGL afxError _SglDdgeCtor(afxDrawBridge ddge, afxCookie const* cookie);
SGL afxError _SglDdgeDtor(afxDrawBridge ddge);

SGL afxNat _SglDdgeEnqueueMmapCb(afxDrawBridge ddge, afxBuffer buf, afxSize off, afxNat ran, afxFlags flags);


SGL void _SglCopyTexSubImage(sglDpu* dpu, GLenum glDstTarget, GLenum glSrcTarget, GLuint glSrcHandle, afxNat opCnt, afxRasterCopy const ops[]);
SGL void _SglUnpackTexSubImage(sglDpu* dpu, GLenum glTarget, afxNat bpp, GLenum glFmt, GLenum glType, afxNat opCnt, afxRasterIo const ops[]);

SGL void DpuBufRw(sglDpu* dpu, afxBuffer buf, afxNat offset, afxNat range, afxBool toHost, void* srcOrDst);
SGL void DpuBufCpy(sglDpu* dpu, afxBuffer src, afxBuffer dst, afxNat opCnt, afxBufferCopyOp const ops[]);
SGL void DpuBufSet(sglDpu* dpu, afxBuffer buf, afxNat offset, afxNat range, afxNat data);

SGL void _DecodeCmdSwitchFrontFace(sglDpu* dpu, _sglCmdBool const *cmd);
SGL void _DecodeCmdSetCullMode(sglDpu* dpu, _sglCmdNat const *cmd);
SGL void _DecodeCmdSetPrimitiveTopology(sglDpu* dpu, _sglCmdNat const *cmd);
SGL void _DecodeCmdSetViewports(sglDpu* dpu, _sglCmdViewport const *cmd);
SGL void _DecodeCmdBindVertexSources(sglDpu* dpu, _sglCmdVertexSources const *cmd);
SGL void _DecodeCmdBindIndexSource(sglDpu* dpu, _sglCmdBufferRange const *cmd);
SGL void _DecodeCmdBindVertexInput(sglDpu* dpu, _sglCmdVertexInput *cmd);

SGL void _DpuBindIndexSource(sglDpu* dpu, afxBuffer buf, afxNat32 offset, afxNat32 range, afxNat32 stride);
SGL void _DpuBindVertexInput(sglDpu* dpu, avxVertexInput vin);
SGL void _DpuBindVertexSources(sglDpu* dpu, afxNat first, afxNat cnt, sglBufferInfo const src[]);
SGL void _DpuSetViewports(sglDpu* dpu, afxNat first, afxNat cnt, avxViewport const vp[]);

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

SGL void _DecodeCmdRasSubsample(sglDpu* dpu, _sglCmdRegenerateMipmaps const* cmd);
SGL void _DecodeCmdRasPack(sglDpu* dpu, _sglCmdPackRasterRegions const* cmd);
SGL void _DecodeCmdRasCopy(sglDpu* dpu, _sglCmdCopyRasterRegions const* cmd);

SGL void _DecodeCmdBufCpy(sglDpu* dpu, _sglCmdBufCpy const* cmd);
SGL void _DecodeCmdBufSet(sglDpu* dpu, _sglCmdBufSet const* cmd);
SGL void _DecodeCmdBufRw(sglDpu* dpu, _sglCmdBufRw const* cmd);

SGL void SglFlushXformStateChanges(sglDpu* dpu);
SGL void SglFlushRasterizationStateChanges(sglDpu* dpu);

SGL void _DecodeCmdNextPass(sglDpu* dpu, _sglCmd const *cmd);
SGL void _DecodeCmdFinishSynthesis(sglDpu* dpu, _sglCmd const *cmd);
SGL void _DecodeCmdBeginSynthesis(sglDpu* dpu, _sglCmdBeginSynthesis const *cmd);

SGL void _DecodeCmdBindSamplers(sglDpu* dpu, _sglCmdBindSamplers const *cmd);
SGL void _DecodeCmdBindRasters(sglDpu* dpu, _sglCmdBindRasters const *cmd);
SGL void _DecodeCmdBindBuffers(sglDpu* dpu, _sglCmdBindBuffers const *cmd);

SGL void _DecodeCmdBindPipeline(sglDpu* dpu, _sglCmdPipeline *cmd);
SGL void _DecodeCmdBindRasterizer(sglDpu* dpu, _sglCmdRasterizer *cmd);


SGL void _DecodeCmdDraw(sglDpu* dpu, _sglCmdDraw const *cmd);
SGL void _DecodeCmdDrawIndexed(sglDpu* dpu, _sglCmdDrawIndexed const* cmd);

SGL void _DpuBeginSynthesis(sglDpu* dpu, avxCanvas canv, afxRect const* area, afxNat layerCnt, afxNat cCnt, avxDrawTarget const* c, avxDrawTarget const* d, avxDrawTarget const* s, afxBool defFbo);
SGL void _DpuFinishSynthesis(sglDpu* dpu);

SGL void _DpuExecuteCommands(sglDpu* dpu, afxNat cnt, avxCmdb streams[]);
SGL void _DpuDraw(sglDpu* dpu, avxDrawIndirectCmd const* data);
SGL void _DpuDrawIndirect(sglDpu* dpu, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
SGL void _DpuDrawIndirectCount(sglDpu* dpu, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
SGL void _DpuDrawIndexed(sglDpu* dpu, avxDrawIndexedIndirectCmd const* data);
SGL void _DpuDrawIndexedIndirect(sglDpu* dpu, afxBuffer buf, afxNat32 offset, afxNat32 drawCnt, afxNat32 stride);
SGL void _DpuDrawIndexedIndirectCount(sglDpu* dpu, afxBuffer buf, afxNat32 offset, afxBuffer cntBuf, afxNat32 cntBufOff, afxNat32 maxDrawCnt, afxNat32 stride);
SGL void _DpuDispatch(sglDpu* dpu, afxNat grpCntX, afxNat grpCntY, afxNat grpCntZ);
SGL void _DpuDispatchIndirect(sglDpu* dpu, afxBuffer buf, afxNat32 offset);

SGL void _DpuBindRasters(sglDpu* dpu, afxNat set, afxNat first, afxNat cnt, afxRaster rasters[], afxNat const subras[]);
SGL void _DpuBindSamplers(sglDpu* dpu, afxNat set, afxNat first, afxNat cnt, avxSampler samplers[]);
SGL void _DpuBindBuffers(sglDpu* dpu, afxNat set, afxNat first, afxNat cnt, afxBuffer buffers[], afxNat const offsets[], afxNat const ranges[]);

SGL void _DpuBindPipeline(sglDpu* dpu, avxPipeline pip, avxVertexInput vin, afxFlags dynamics);
SGL void _DpuBindRasterizer(sglDpu* dpu, avxRasterizer razr, avxVertexInput vin, afxFlags dynamics);

SGL sglDecodeVmt _SglDecodeCmdVmt;

SGL afxError _SglPipCtor(avxPipeline pip, afxCookie const* cookie);
SGL afxError _SglPipDtor(avxPipeline pip);
SGL afxError _SglRazrDtor(avxRasterizer razr);
SGL afxError _SglRazrCtor(avxRasterizer razr, afxCookie const* cookie);
SGL afxError _SglShdCtor(avxShader shd, afxCookie const* cookie);
SGL afxError _SglShdDtor(avxShader shd);
SGL afxError _SglVinCtor(avxVertexInput vin, afxCookie const* cookie);
SGL afxError _SglVinDtor(avxVertexInput vin);
SGL afxError _SglCanvCtor(avxCanvas canv, afxCookie const* cookie);
SGL afxError _SglCanvDtor(avxCanvas canv);
SGL afxError _SglRasCtor(afxRaster ras, afxCookie const* cookie);
SGL afxError _SglRasDtor(afxRaster ras);
SGL afxError _BufCtorCb(afxBuffer buf, afxCookie const* cookie);
SGL afxError _BufDtorCb(afxBuffer buf);
SGL afxError _SglSampCtor(avxSampler samp, afxCookie const* cookie);
SGL afxError _SglSampDtor(avxSampler samp);
SGL afxError _SglBschCtor(avxLigature liga, afxCookie const* cookie);
SGL afxError _SglBschDtor(avxLigature liga);
SGL afxError _SglQrypCtor(avxQueryPool qryp, afxCookie const* cookie);
SGL afxError _SglQrypDtor(avxQueryPool qryp);

SGL afxError _SglRelinkDoutCb(afxDrawDevice ddev, afxDrawContext dctx, afxNat cnt, afxDrawOutput dout[]);

#endif//AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H
