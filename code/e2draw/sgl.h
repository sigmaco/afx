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
 *                   (c) 2017 SIGMA Technology Group � Federa��o SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H
#define AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1

//#include "../src/e2coree/deps/gl/glcorearb.h"
//#include "../src/e2coree/deps/gl/wglext.h"

#include "../deps/gl/glad.h"
#include "../deps/gl/glad_wgl.h"

#define _AFX_DRAW_C

#include "afx/draw/afxRasterizer.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/draw/afxSampler.h"
#include "afx/draw/afxDrawScript.h"
#include "sglDrawDriver.h"
#include "afx/core/afxArena.h"

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

#define SGL_MAX_VIEWPORTS 8
#define SGL_VIEWPORT_BOUNDS_RANGE 0
#define SGL_MAX_VIEWPORT_DIMS 0
#define SGL_DEPTH_RANGE


#define SGL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 80


#define _SGL_MAX_VAO_PER_TIME 60
//#define _SGL_MAX_VP_PER_SET 8
//#define _SGL_MAX_SCISSOR_PER_SET 8
//#define _SGL_MAX_INSTREAM_PER_SET 8
//#define _SGL_MAX_RASTER_SURF_PER_CANV 8
#define SGL_MAX_COLOR_ATTACHMENTS 8
#define _SGL_MAX_SURF_PER_CANV SGL_MAX_COLOR_ATTACHMENTS + 2

typedef enum sglUpdateFlags
{
    SGL_UPD_FLAG_DEVICE_FLUSH   = AfxGetBitOffset(0), // flush from host to device
    SGL_UPD_FLAG_HOST_FLUSH     = AfxGetBitOffset(1), // flush from device to host
    SGL_UPD_FLAG_DEVICE_INST    = AfxGetBitOffset(2), // (re)instantiate on device
    SGL_UPD_FLAG_HOST_INST      = AfxGetBitOffset(3), // (re)instantiate on host
    
    SGL_UPD_FLAG_HOST           = (SGL_UPD_FLAG_HOST_INST | SGL_UPD_FLAG_HOST_FLUSH),
    SGL_UPD_FLAG_DEVICE         = (SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH),
} sglUpdateFlags;

typedef union wglVmt
{
    struct
    {
#if 0
        HGLRC(WINAPI*CreateContext)(HDC);
        BOOL(WINAPI*DeleteContext)(HGLRC);
        HGLRC(WINAPI*GetCurrentContext)(VOID);
        HDC(WINAPI*GetCurrentDC)(VOID);
        PROC(WINAPI*GetProcAddress)(LPCSTR);
        BOOL(WINAPI*MakeCurrent)(HDC, HGLRC);
        int(WINAPI*ChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);  // "wglChoosePixelFormat" funciona com Intel mas n�o com AMD.
        int(WINAPI*DescribePixelFormat)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd); 
        BOOL(WINAPI*SetPixelFormat)(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd); // "wglSetPixelFormat" funciona com Intel mas n�o com AMD.
        int(WINAPI*GetPixelFormat)(HDC hdc);
        BOOL(WINAPI*SwapBuffers)(HDC);
#endif
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

AFX_DEFINE_STRUCT(sglVertexInput)
{
    struct
    {
        afxBuffer       buf;
        afxNat32        offset;
        afxNat32        range;
    }                   sources[SGL_MAX_VERTEX_ATTRIB_BINDINGS];
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

    afxBuffer           idxSrcBuf;
    afxNat32            idxSrcOff;
    afxNat32            idxSrcRange;
    afxNat32            idxSrcSiz;
    GLuint              vao;
};

typedef struct
{
    glVmt const             gl;
    HGLRC                   (WINAPI*CreateContext)(HDC);
    BOOL                    (WINAPI*DeleteContext)(HGLRC);
    HGLRC                   (WINAPI*GetCurrentContext)(VOID);
    HDC                     (WINAPI*GetCurrentDC)(VOID);
    PROC                    (WINAPI*GetProcAddress)(LPCSTR);
    BOOL                    (WINAPI*MakeCurrent)(HDC, HGLRC);
    int                     (WINAPI*ChoosePixelFormat)(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);  // "wglChoosePixelFormat" funciona com Intel mas n�o com AMD.
    int                     (WINAPI*DescribePixelFormat)(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);
    BOOL                    (WINAPI*SetPixelFormat)(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd); // "wglSetPixelFormat" funciona com Intel mas n�o com AMD.
    int                     (WINAPI*GetPixelFormat)(HDC hdc);
    BOOL                    (WINAPI*SwapBuffers)(HDC);
    //wglVmt const            wgl;
    HWND                    wnd;
    HDC                     dc;
    int                     dcPxlFmt;
    PIXELFORMATDESCRIPTOR   dcPfd;
    HGLRC                   glrc;
    HMODULE                 opengl32;
    afxNat                  verMajor, verMinor, verPatch;
    afxString               subsysName;
    afxString               subsysVer;

    afxNat                  portIdx;
    afxSlock                deletionLock;
    afxQueue                deletionQueue;

    afxDrawContext          activeDctx;

    afxSize                 numOfFedVertices, numOfFedIndices, numOfFedInstances;

    struct
    {
        struct
        {
            //afxCanvas                   canv;
            afxRect                     area;
            afxNat                      layerCnt; // dyn
            afxNat                      rasterCnt;
            afxDrawTarget               rasters[_SGL_MAX_SURF_PER_CANV];
            afxDrawTarget               depthRt;
            afxDrawTarget               stencilRt;
            afxNat                      activeSubpass;
        }                               renderPass;
    }state;

    struct
    {
        afxNat              vpCnt; /// 0
        afxViewport         vps[SGL_MAX_VIEWPORTS];

        afxPrimTopology     primTop; /// is a option defining the primitive topology. /// afxPrimTopology_TRI_LIST
        afxBool             primRestartEnabled; /// controls whether a special vertex index value (0xFF, 0xFFFF, 0xFFFFFFFF) is treated as restarting the assembly of primitives. /// FALSE
        afxNat              patchControlPoints; /// is the number of control points per patch.
        
        afxBool             depthClampEnabled; /// controls whether to clamp the fragment's depth values as described in Depth Test. /// FALSE

        afxCullMode         cullMode; /// is the triangle facing direction used for primitive culling. /// afxCullMode_BACK
        afxBool             cwFrontFacing; /// If this member is TRUE, a triangle will be considered front-facing if its vertices are clockwise. /// FALSE (CCW)

    }                       activeXformState, nextXformState;

    struct
    {
        afxBool             rasterizationDisabled; /// controls whether primitives are discarded immediately before the rasterization stage. /// FALSE
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
        afxBool             depthWriteEnabled; /// controls whether depth writes are enabled when depthTestEnable is TRUE. Depth writes are always disabled when depthTestEnable is FALSE. /// FALSE
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

    }                       activeRasterState, nextRasterState;

    afxPipeline             activePip, nextPip;

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
            afxRaster      tex;
        };
    }                       activeResBind[_SGL_MAX_LEGO_PER_BIND][_SGL_MAX_ENTRY_PER_LEGO], nextResBind[_SGL_MAX_LEGO_PER_BIND][_SGL_MAX_ENTRY_PER_LEGO];
    afxMask                 nextResBindUpdMask[_SGL_MAX_LEGO_PER_BIND];


    sglVertexInput          tmpVertexInput[30];    
    sglVertexInput*         activeVertexInput;
    sglVertexInput          nextVertexInput;
    afxMask                 nextVtxInStreamUpdMask;
    afxNat                  nextVtxInStreamUpdCnt;
    afxMask                 nextVtxInAttribUpdMask;
    afxNat                  nextVtxInAttribUpdCnt;

    afxMask                 nextBlendConstUpd;
    afxMask                 nextViewportUpdMask;
    afxNat                  nextViewportUpdCnt;
    afxMask                 nextScissorUpdMask;
    afxNat                  nextScissorUpdCnt;
    afxMask                 nextAreaUpdMask;
    afxNat                  nextAreaUpdCnt;
    afxBool                 scissorTestEnabled;

    afxBool                 flushPip, flushPass, flushSr, flushIbb;
    
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
} sglDpuIdd;

AFX_OBJECT(afxDrawDevice)
{
    struct afxBaseDrawDevice base;
    WNDCLASSEX     wndClss;
    afxNat          dpuCnt;
    sglDpuIdd*      dpus;
};

AFX_OBJECT(afxDrawContext)
{
    struct afxBaseDrawContext base;
    // presentation stuff, only on unit 0.
    afxBool presentationSuspended;
    //GLuint presentFboGpuHandle;
    afxPipeline presentPip;
    afxSampler presentSmp;
    //afxBuffer tristrippedQuad2dPosBuf;
};

AFX_OBJECT(afxDrawQueue)
{
    struct afxBaseDrawQueue base;
};

AFX_DEFINE_STRUCT(_sglDeleteGlRes)
{
    GLuint  gpuHandle;
    GLuint  type; // 0 buf, 1 tex, 2 surf, 3 canv, 4 smp, 5 pip, 6 shd, 7 shd (separate) program
};

AFX_OBJECT(afxDrawOutput)
{
    struct afxBaseDrawOutput base;
    afxFcc      fcc;
    afxBool     instanced;

    HWND        wnd;
    HDC         dc;
    int         dcPxlFmt;
    
    HDC         dcDeskBkp;
    int         dcPxlFmtDeskBkp;
};

AFX_OBJECT(afxDrawInput)
{
    struct afxBaseDrawInput base;
    int a;
};

AFX_DEFINE_STRUCT(_afxDscrCmd)
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
    afxBool         assembled;
    sglVertexInput  vertexInput;
};

AFX_OBJECT(afxRasterizer)
{
    struct afxBaseRasterizer base;
    sglUpdateFlags  updFlags;
};

AFX_OBJECT(afxPipelineRig)
{
    struct afxBasePipelineRig base;
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

AFX_OBJECT(afxBuffer)
{
    struct afxBaseBuffer    base;
    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    GLenum          glTarget;
    GLenum          glUsage;
    
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

AFX_OBJECT(afxSurface)
{
    struct afxBaseSurface base;
    sglUpdateFlags  updFlags;
    GLuint          glHandle; // if depth/stencil, should have a sidecar GL render buffer.
};

AFX_OBJECT(afxCanvas)
{
    struct afxBaseCanvas base;
    sglUpdateFlags  updFlags;
    afxNat          glHandle;
};

AFX_OBJECT(afxDrawScript)
{
    struct afxBaseDrawScript    base;
    afxChain                    commands;
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


typedef enum afxDrawCmdId
{
    AFX_DCMD_END = offsetof(afxCmd, EndOfScript) / sizeof(void*),
    AFX_DCMD_EXECUTE_COMMANDS = offsetof(afxCmd, ExecuteCommands) / sizeof(void*),
    AFX_DCMD_BEGIN_CANVAS = offsetof(afxCmd, BeginCanvas) / sizeof(void*),
    AFX_DCMD_NEXT_PASS = offsetof(afxCmd, NextPass) / sizeof(void*),
    AFX_DCMD_END_CANVAS = offsetof(afxCmd, EndCanvas) / sizeof(void*),
    
    AFX_DCMD_BIND_BUFFERS = offsetof(afxCmd, BindBuffers) / sizeof(void*),
    AFX_DCMD_BIND_TEXTURES = offsetof(afxCmd, BindTextures) / sizeof(void*),
    
    AFX_DCMD_BIND_PIPELINE = offsetof(afxCmd, BindPipeline) / sizeof(void*),

    AFX_DCMD_READJUST_VIEWPORTS = offsetof(afxCmd, ReadjustViewports) / sizeof(void*),
    AFX_DCMD_RESET_VIEWPORTS = offsetof(afxCmd, ResetViewports) / sizeof(void*),    
    AFX_DCMD_READJUST_SCISSORS = offsetof(afxCmd, ReadjustScissors) / sizeof(void*),
    AFX_DCMD_RESET_SCISSORS = offsetof(afxCmd, ResetScissors) / sizeof(void*),
    AFX_DCMD_READJUST_AREAS = offsetof(afxCmd, ReadjustAreas) / sizeof(void*),
    AFX_DCMD_RESET_AREAS = offsetof(afxCmd, ResetAreas) / sizeof(void*),

    AFX_DCMD_BIND_VERTEX_SOURCES = offsetof(afxCmd, BindVertexSources) / sizeof(void*),
    AFX_DCMD_RESET_VERTEX_STREAMS = offsetof(afxCmd, ResetVertexStreams) / sizeof(void*),
    AFX_DCMD_RESET_VERTEX_ATTRIBUTES = offsetof(afxCmd, ResetVertexAttributes) / sizeof(void*),
    AFX_DCMD_BIND_INDEX_SOURCE = offsetof(afxCmd, BindIndexSource) / sizeof(void*),
    AFX_DCMD_SET_PRIMITIVE_TOPOLOGY = offsetof(afxCmd, SetPrimitiveTopology) / sizeof(void*),
    
    AFX_DCMD_DRAW = offsetof(afxCmd, Draw) / sizeof(void*),
    AFX_DCMD_DRAW_INDEXED = offsetof(afxCmd, DrawIndexed) / sizeof(void*),

    AFX_DCMD_DISABLE_RASTERIZATION = offsetof(afxCmd, DisableRasterization) / sizeof(void*),
    AFX_DCMD_SWITCH_FRONT_FACE = offsetof(afxCmd, SwitchFrontFace) / sizeof(void*),
    AFX_DCMD_SET_CULL_MODE = offsetof(afxCmd, SetCullMode) / sizeof(void*),
    AFX_DCMD_ENABLE_DEPTH_BIAS = offsetof(afxCmd, EnableDepthBias) / sizeof(void*),
    AFX_DCMD_SET_DEPTH_BIAS = offsetof(afxCmd, SetDepthBias) / sizeof(void*),
    AFX_DCMD_SET_LINE_WIDTH = offsetof(afxCmd, SetLineWidth) / sizeof(void*),

    AFX_DCMD_ENABLE_DEPTH_BOUNDS_TEST = offsetof(afxCmd, EnableDepthBoundsTest) / sizeof(void*),
    AFX_DCMD_SET_DEPTH_BOUNDS = offsetof(afxCmd, SetDepthBounds) / sizeof(void*),
    AFX_DCMD_ENABLE_STENCIL_TEST = offsetof(afxCmd, EnableStencilTest) / sizeof(void*),
    AFX_DCMD_SET_STENCIL_COMPARE_MASK = offsetof(afxCmd, SetStencilCompareMask) / sizeof(void*),
    AFX_DCMD_SET_STENCIL_WRITE_MASK = offsetof(afxCmd, SetStencilWriteMask) / sizeof(void*),
    AFX_DCMD_SET_STENCIL_REFERENCE = offsetof(afxCmd, SetStencilReference) / sizeof(void*),
    AFX_DCMD_ENABLE_DEPTH_TEST = offsetof(afxCmd, EnableDepthTest) / sizeof(void*),
    AFX_DCMD_SET_DEPTH_COMPARE_OP = offsetof(afxCmd, SetDepthCompareOp) / sizeof(void*),
    AFX_DCMD_ENABLE_DEPTH_WRITE = offsetof(afxCmd, EnableDepthWrite) / sizeof(void*),
    
    AFX_DCMD_SET_BLEND_CONSTANTS = offsetof(afxCmd, SetBlendConstants) / sizeof(void*),

    AFX_DCMD_TOTAL = offsetof(afxCmd, Total) / sizeof(void*),
} afxDrawCmdId;


AFX_DEFINE_STRUCT(_afxDscrCmdDraw)
{
    _afxDscrCmd                     cmd;
    afxNat32                        vtxCnt, instCnt, firstVtx, firstInst;
};

AFX_DEFINE_STRUCT(_afxDscrCmdDrawIndexed)
{
    _afxDscrCmd                     cmd;
    afxNat32                        idxCnt, instCnt, firstIdx, vtxOff, firstInst;
};

AFX_DEFINE_STRUCT(_afxDscrCmdDrawPrefab)
{
    _afxDscrCmd                     cmd;
    afxDrawPrefab                   prefab;
    afxNat32                        instCnt;
};

AFX_DEFINE_STRUCT(_afxDscrCmdBeginCanvas)
{
    _afxDscrCmd             cmd;
    afxLinkage              canvOpsLnk;
    //afxCanvas               canv;
    afxRect                 area;
    afxNat32                layerCnt;
    afxNat32                rasterCnt;
    afxDrawTarget           rasters[_SGL_MAX_SURF_PER_CANV];
    afxDrawTarget           depth;
    afxDrawTarget           stencil;
    
    GLuint                  fboHandle;
};

AFX_DEFINE_STRUCT(_afxDscrCmdBindTextures)
{
    _afxDscrCmd                     cmd;
    afxNat32                        set;
    afxNat32                        first, cnt;
    afxSampler                      smp[_SGL_MAX_ENTRY_PER_LEGO];
    afxRaster                      tex[_SGL_MAX_ENTRY_PER_LEGO];
};

AFX_DEFINE_STRUCT(_afxDscrCmdBindBuffers)
{
    _afxDscrCmd                     cmd;
    afxNat32                        set;
    afxNat32                        first, cnt;
    afxBuffer                       buf[_SGL_MAX_ENTRY_PER_LEGO];
    afxNat32                        offset[_SGL_MAX_ENTRY_PER_LEGO];
    afxNat32                        range[_SGL_MAX_ENTRY_PER_LEGO];
};

AFX_DEFINE_STRUCT(_afxDscrCmdVertexSources)
{
    _afxDscrCmd                     cmd;
    afxNat32                        first, cnt;
    //afxVertexInputSource            spec[SGL_MAX_VERTEX_ATTRIB_BINDINGS];
    afxBuffer           buf[SGL_MAX_VERTEX_ATTRIB_BINDINGS];
    afxNat32            offset[SGL_MAX_VERTEX_ATTRIB_BINDINGS]; /// the start of buffer.
    afxNat32            range[SGL_MAX_VERTEX_ATTRIB_BINDINGS]; /// the size in bytes of vertex data bound from buffer.
    //afxNat32            stride[SGL_MAX_VERTEX_ATTRIB_BINDINGS]; /// the byte stride between consecutive elements within the buffer.
};

AFX_DEFINE_STRUCT(_afxDscrCmdVertexStreams)
{
    _afxDscrCmd                     cmd;
    afxNat cnt;
    afxNat srcIdx[SGL_MAX_VERTEX_ATTRIB_BINDINGS];
    afxNat stride[SGL_MAX_VERTEX_ATTRIB_BINDINGS];
    afxBool instance[SGL_MAX_VERTEX_ATTRIB_BINDINGS];
};

AFX_DEFINE_STRUCT(_afxDscrCmdVertexAttributes)
{
    _afxDscrCmd                     cmd;    
    afxNat cnt;
    afxNat location[SGL_MAX_VERTEX_ATTRIBS];
    afxVertexFormat fmt[SGL_MAX_VERTEX_ATTRIBS];
    afxNat srcIdx[SGL_MAX_VERTEX_ATTRIBS];
    afxNat32 offset[SGL_MAX_VERTEX_ATTRIBS];
};

AFX_DEFINE_STRUCT(_afxDscrCmdBufferRange)
{
    _afxDscrCmd                     cmd;
    afxBuffer                       buf;
    afxNat32                        offset;
    afxNat32                        range;
    afxNat32                        stride;
};

AFX_DEFINE_STRUCT(_afxDscrCmdPipeline)
{
    _afxDscrCmd                     cmd;
    afxNat                          level;
    afxPipeline                     pip;
};

AFX_DEFINE_STRUCT(_afxDscrCmdBool)
{
    _afxDscrCmd                     cmd;
    afxBool                         value;
};

AFX_DEFINE_STRUCT(_afxDscrCmdReal)
{
    _afxDscrCmd                     cmd;
    afxReal                         value;
};

AFX_DEFINE_STRUCT(_afxDscrCmdReal2)
{
    _afxDscrCmd                     cmd;
    afxReal                         value[2];
};

AFX_DEFINE_STRUCT(_afxDscrCmdReal3)
{
    _afxDscrCmd                     cmd;
    afxReal                         value[3];
};

AFX_DEFINE_STRUCT(_afxDscrCmdReal4)
{
    _afxDscrCmd                     cmd;
    afxReal                         value[4];
};

AFX_DEFINE_STRUCT(_afxDscrCmdNat)
{
    _afxDscrCmd                     cmd;
    afxNat                          value;
};

AFX_DEFINE_STRUCT(_afxDscrCmdBitmaskNat32)
{
    _afxDscrCmd                     cmd;
    afxMask                      mask;
    afxNat32                        value;
};

AFX_DEFINE_STRUCT(_afxDscrCmdScissor)
{
    _afxDscrCmd                     cmd;
    afxNat32                        first, cnt;
    afxRect                         rect[SGL_MAX_VIEWPORTS];
    afxBool                         reset;
};

AFX_DEFINE_STRUCT(_afxDscrCmdArea)
{
    _afxDscrCmd                     cmd;
    afxBool                         exclusive;
    afxNat32                        first, cnt;
    afxRect                         rect[SGL_MAX_VIEWPORTS];
    afxBool                         reset;
};

AFX_DEFINE_STRUCT(_afxDscrCmdViewport)
{
    _afxDscrCmd                     cmd;
    afxNat32                        first, cnt;
    afxViewport                     vp[SGL_MAX_VIEWPORTS];
    afxBool                         reset;
};

AFX_DEFINE_STRUCT(_afxDscrCmdNextPass)
{
    _afxDscrCmd                     cmd;
    afxBool32                       useAuxScripts;
};

AFX_DEFINE_STRUCT(_afxDscrCmdExecCmds)
{
    _afxDscrCmd                     cmd;
    afxNat                          cnt;
    afxDrawScript                   subsets[8];
};

AFX_DEFINE_STRUCT(_afxDscrCmdCopyTex)
{
    _afxDscrCmd                     cmd;
    afxRaster                      dst;
    afxRaster                      src;
    afxNat32                        rgnCnt;
    afxRasterRegion                rgn[16];
};
// vkCmdBindDescriptorSets - Binds descriptor sets to a command buffer

//SGL afxError _SglCreateCombinedDeviceContext(WNDCLASSEXA *oglWndClss, HGLRC shareCtx, HWND *phwnd, HDC *phdc, HGLRC *phrc, wglVmt const* wgl, HMODULE opengl32);

SGL void _SglDctxDeleteGlRes(afxDrawContext dctx, afxNat type, GLuint gpuHandle);


_SGL afxError AfxRegisterDrawDrivers(afxModule mdle, afxDrawSystem dsys);

SGL afxError _SglDpuBindAndSyncSamp(sglDpuIdd* dpu, afxNat unit, afxSampler smp, glVmt const* gl);
SGL afxError _SglDpuSyncShd(sglDpuIdd* dpu, afxShader shd, afxShaderStage stage, glVmt const* gl);
SGL afxError _SglDpuSurfSync(sglDpuIdd* dpu, afxSurface surf, glVmt const* gl); // must be used before texUpdate
SGL afxError _SglDpuBindAndSyncTex(sglDpuIdd* dpu, afxNat unit, afxRaster tex, glVmt const* gl);
SGL afxError _SglDpuBindAndSyncPip(sglDpuIdd* dpu, afxPipeline pip, afxBool bindPip, afxBool bindVao, afxBool bindRas);
SGL afxError _SglDpuBindAndSyncRasterizer(sglDpuIdd* dpu, afxRasterizer rast);
SGL afxError _SglDpuBindAndResolveLego(sglDpuIdd* dpu, afxPipeline pip, afxNat unit, afxPipelineRig legt, glVmt const* gl);
SGL afxError _SglDpuBindAndSyncCanv(sglDpuIdd* dpu, afxCanvas canv, GLenum target, glVmt const* gl);
SGL afxError _SglDpuBindAndSyncBuf(sglDpuIdd* dpu, afxNat unit, afxBuffer buf, afxNat offset, afxNat range, afxNat stride, GLenum target, GLenum usage, glVmt const* gl);

//SGL afxSize _AfxMeasureTextureRegion(afxRaster tex, afxRasterRegion const *rgn);

SGL BOOL SglMakeCurrent(HDC hdc, HGLRC hrc, sglDpuIdd const *dpu);
SGL afxError SglSwapBuffers(HDC hdc, sglDpuIdd const *dpu);
SGL int SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd, sglDpuIdd const *dpu);
SGL BOOL SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd, sglDpuIdd const *dpu);
SGL int SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd, sglDpuIdd const *dpu);
SGL int SglGetPixelFormat(HDC hdc, sglDpuIdd const *dpu);

SGL afxPipelineRig _SglDrawContextFindLego(afxDrawContext dctx, afxNat bindCnt, afxPipelineRigBindingDecl const bindings[]);

SGL afxBool _SglDqueVmtSubmitCb(afxDrawContext dctx, afxDrawQueue dque, afxDrawSubmissionSpecification const *spec, afxNat *submNo);

SGL afxClassConfig _SglDctxClsConfig;
SGL afxClassConfig _SglDoutClsConfig;
SGL afxClassConfig _SglDinClsConfig;

SGL afxError _SglDinFreeAllBuffers(afxDrawInput din);

SGL afxCmd _SglEncodeCmdVmt;

#endif//AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H