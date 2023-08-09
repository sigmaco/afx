/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H
#define AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H

#include "../src/e2coree/draw/_classified/afxDrawClassified.h"
#include "sglDrawDriver.h"
#include "afx/core/mem/afxArena.h"

#define _SGL_MAX_DOUT_PER_PRESENTATION 4
#define _SGL_MAX_DSCR_PER_SUBMISSION 4
#define _SGL_MAX_OBJ_PER_STREAMING 4
#define _SGL_MAX_DQUE_PER_DCTX 4
#define _SGL_MAX_SWAPCHAIN_CAP 3
#define _SGL_MAX_SURF_PER_DOUT 3
#define _SGL_MAX_LEGO_PER_BIND 4
#define _SGL_MAX_ENTRY_PER_LEGO 10 
#define _SGL_MAX_STAGE_PER_PIP 8
#define _SGL_MAX_VBO_PER_BIND 8
#define _SGL_MAX_VAO_PER_TIME 60
#define _SGL_MAX_VP_PER_SET 8
#define _SGL_MAX_SCISSOR_PER_SET 8
#define _SGL_MAX_INSTREAM_PER_SET 8
#define _SGL_MAX_RASTER_SURF_PER_CANV 8
#define _SGL_MAX_SURF_PER_CANV _SGL_MAX_RASTER_SURF_PER_CANV + 2

typedef enum sglUpdateFlags
{
    SGL_UPD_FLAG_DEVICE_FLUSH   = AFX_FLAG(0), // flush from host to device
    SGL_UPD_FLAG_HOST_FLUSH     = AFX_FLAG(1), // flush from device to host
    SGL_UPD_FLAG_DEVICE_INST    = AFX_FLAG(2), // (re)instantiate on device
    SGL_UPD_FLAG_HOST_INST      = AFX_FLAG(3), // (re)instantiate on host
    
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
    void *ptr[1];
} wglVmt;

typedef struct
{
    HWND            wnd;
    HDC             dc;
    int             dcPxlFmt;
    HGLRC           glrc;
    wglVmt const   wgl;
    glVmt const    gl;
    afxNat          verMajor, verMinor, verPatch;
    afxString       subsysName;
    afxString       subsysVer;

    afxNat          portIdx;
    afxSlock        deletionLock;
    afxQueue        deletionQueue;
} sglDpuIdd;

AFX_DEFINE_STRUCT(_sglDdrvIdd)
{
    HMODULE         opengl32;
    WNDCLASSEXA     wndClss;
    afxNat          dpuCnt;
    sglDpuIdd*      dpus;
};

typedef struct
{
    // presentation stuff, only on unit 0.
    afxBool presentationSuspended;
    GLuint presentFboGpuHandle;
    afxPipeline presentPip;
    afxSampler presentSmp;
    afxVertexBuffer presentVbuf;
} sglDctxIdd;

AFX_DEFINE_STRUCT(_sglDeleteGlRes)
{
    GLuint  gpuHandle;
    GLuint  type; // 0 buf, 1 tex, 2 surf, 3 canv, 4 smp, 5 pip, 6 shd, 7 shd (separate) program
};

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
        afxTexture          tex;
    };
};

typedef struct
{
    afxSize             numOfFedVertices, numOfFedIndices, numOfFedInstances;

    afxTime             lastStreamBeginTime;
    afxTime             lastStreamEndTime;
    afxTime             lastSubmisBeginTime;
    afxTime             lastSubmisEndTime;
    afxTime             lastPresentBeginTime;
    afxTime             lastPresntEndTime;

    afxOffset           frameNum, fps;
    afxSize             frameTime, swapTime;
    afxReal             deltaTime;

    struct
    {
        struct
        {
            afxCanvas                   canv;
            afxRect                     area;            
            afxNat                      layerCnt; // dyn
            afxNat                      rasterCnt;
            afxDrawTarget               rasters[_SGL_MAX_SURF_PER_CANV];
            afxDrawTarget               depthRt;
            afxDrawTarget               stencilRt;
            afxNat                      activeSubpass;
        }                               renderPass;

        struct
        {
            afxPipeline                 pip;
            afxNat                      shdCnt;
            afxShader                   shd[6];
            afxShaderStage              stages[6];
        };
        //afxNat                          legoCnt;
        sglLegoData                     boundRes[_SGL_MAX_LEGO_PER_BIND][_SGL_MAX_ENTRY_PER_LEGO];

        struct
        {
            afxBuffer                   buf;
            afxNat                      offset;
            //afxNat                      siz;
            afxNat                      stride;
        }                               vertexBindings[_SGL_MAX_VBO_PER_BIND];
        afxNat                          vertexBindingCnt;

        afxNat                          inStreamCnt;
        afxPipelineInputStream          inStreams[_SGL_MAX_INSTREAM_PER_SET];
        struct
        {
            afxBuffer                   buf;
            afxNat32                    offset;
            afxNat32                    idxSiz;
        }                               indexBinding;

        // vertex pos-processing
        afxPipelineInputAssemblyState   ia;
        afxNat                          vpCnt;
        afxViewport                     vp[_SGL_MAX_VP_PER_SET];


        afxPipelineRasterizerState      raster;
        afxPipelineDepthState           depth;
        afxPipelineMultisampleState     multisample;
        afxNat                          scissorCnt;
        afxRect                         scissorArea[_SGL_MAX_SCISSOR_PER_SET];
        afxPipelineColorBlendState      colorBlend;

    }                       state;
    /*
    struct
    {
        afxBool             pipBound;
        afxBool             vbufBound;
        afxNat              vbufBase;
        afxNat              vbufCnt;
        afxBool             ibufBound;
        afxBool             iaSet;
        afxBool             scisSet;
        afxNat              scisBase;
        afxNat              scisCnt;
        afxBool             vpSet;
        afxNat              vpBase;
        afxNat              vpCnt;
    }                       stateChanges;
    */
    afxBool                 running;

    // idd
    afxBool         instanced;

    //_gfxRendererContextState state;

    // submission stuff
    afxBool         submissionSuspended;
    GLuint          tmpFbo[_SGL_MAX_SWAPCHAIN_CAP];
    afxNat          activeTmpFboIdx;
    GLuint          vao[_SGL_MAX_VAO_PER_TIME];
    afxNat          activeVaoIdx;
} sglDqueIdd;

typedef struct
{
    afxFcc      fcc;
    afxBool     instanced;

    HWND        wnd;
    HDC         dc;
    int         dcPxlFmt;
    
    HDC         dcDeskBkp;
    int         dcPxlFmtDeskBkp;
} sglDoutIdd;

AFX_DEFINE_STRUCT(_afxDscrCmd)
{
    afxNat              id;
    afxNat              siz;
    afxLinkage          script;
};

typedef struct
{
    afxPipeline     pip;
    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    afxBool         assembled;
} sglPipIdd;

typedef struct
{
    afxShader           shd;
    sglUpdateFlags      updFlags;
    afxNat              glHandle;
    afxNat              glProgHandle;
    afxBool             compiled;
} sglShdIdd;

typedef struct
{
    afxSampler      smp;
    sglUpdateFlags  updFlags;
    afxNat          glHandle;
} sglSampIdd;

typedef struct
{
    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    GLenum          glTarget;
    GLenum          glUsage;
    
    afxSize         lastUpdOffset;
    afxNat          lastUpdRange;
    afxBool         locked;
} sglBufIdd;

static_assert(AFX_BUF_IDD >= sizeof(sglBufIdd), "");

typedef struct 
{
    afxTexture      tex;
    sglUpdateFlags  updFlags;
    GLuint          glHandle;
    GLint           glIntFmt;
    GLenum          glTarget, glFmt, glType;
    
    afxNat          lastUpdLodBase, lastUpdLodRange;
    afxNat          lastUpdImgBase, lastUpdImgRange;
    afxNat          lastUpdOffset[3];
    afxNat          lastUpdRange[3];
    afxBool         locked;
}sglTexIdd;

typedef struct
{
    afxSurface      surf;
    sglUpdateFlags  updFlags;
    GLuint          glHandle; // if depth/stencil, should have a sidecar GL render buffer.
} sglSurfIdd;

typedef struct
{
    afxCanvas       canv;
    sglUpdateFlags  updFlags;
    afxNat          glHandle;
} sglCanvIdd;

typedef struct
{
    afxChain    commands;
} sglDscrIdd;

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
    AFX_DCMD_END,

    AFX_DCMD_BEGIN_DRAW_PASS,
    AFX_DCMD_NEXT_PASS,
    AFX_DCMD_END_DRAW_PASS,

    AFX_DCMD_BIND_PIPELINE,

    AFX_DCMD_SET_INPUT_ASSEMBLY_STATE,
    AFX_DCMD_SET_VIEWPORTS,
    AFX_DCMD_SET_RASTERIZATION_STATE,
    AFX_DCMD_SET_DEPTH_STATE,
    AFX_DCMD_SET_SCISSORS,

    AFX_DCMD_BIND_BUFFERS,
    AFX_DCMD_BIND_VERTEX_BUFFERS,
    AFX_DCMD_BIND_INDEX_BUFFER,
    AFX_DCMD_BIND_TEXTURES,

    AFX_DCMD_DRAW,
    AFX_DCMD_DRAW_INDEXED,

    AFX_DCMD_TOTAL
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

AFX_DEFINE_STRUCT(_afxDscrCmdSetRasterizerState)
{
    _afxDscrCmd                     cmd;
    afxPipelineRasterizerState   state;
};

AFX_DEFINE_STRUCT(_afxDscrCmdSetDepthState)
{
    _afxDscrCmd                     cmd;
    afxPipelineDepthState           state;
};

AFX_DEFINE_STRUCT(_afxDscrCmdSetInputAssemblyState)
{
    _afxDscrCmd                     cmd;
    afxPipelineInputAssemblyState   state;
};

AFX_DEFINE_STRUCT(_afxDscrCmdBeginDrawPass)
{
    _afxDscrCmd             cmd;
    afxCanvas               canv;
    afxRect                 area;
    afxNat32                layerCnt;
    afxNat32                rasterCnt;
    afxDrawTarget           rasters[_SGL_MAX_SURF_PER_CANV];
    afxDrawTarget           depth;
    afxDrawTarget           stencil;
};

AFX_DEFINE_STRUCT(_afxDscrCmdBindTextures)
{
    _afxDscrCmd                     cmd;
    afxNat32                        set;
    afxNat32                        first, cnt;
    afxSampler                      smp[_SGL_MAX_ENTRY_PER_LEGO];
    afxTexture                      tex[_SGL_MAX_ENTRY_PER_LEGO];
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

AFX_DEFINE_STRUCT(_afxDscrCmdBindVbuf)
{
    _afxDscrCmd                     cmd;
    afxNat32                        first, cnt;

    afxBuffer                       buf[_SGL_MAX_VBO_PER_BIND];
    afxSize                         offset[_SGL_MAX_VBO_PER_BIND];
    //afxSize                         vtxArrSiz[_SGL_MAX_VBO_PER_BIND];
    afxSize                         vtxStride[_SGL_MAX_VBO_PER_BIND];
};

AFX_DEFINE_STRUCT(_afxDscrCmdBindIbuf)
{
    _afxDscrCmd                     cmd;
    afxBuffer                       buf;
    afxNat32                        offset;
    afxNat32                        idxSiz;
};

AFX_DEFINE_STRUCT(_afxDscrCmdBindPip)
{
    _afxDscrCmd                     cmd;
    afxPipeline                     pip;
};

AFX_DEFINE_STRUCT(_afxDscrCmdSetScissor)
{
    _afxDscrCmd                     cmd;
    afxNat32                        first, cnt;
    afxRect                         rect[_SGL_MAX_SCISSOR_PER_SET];
};

AFX_DEFINE_STRUCT(_afxDscrCmdSetVp)
{
    _afxDscrCmd                     cmd;
    afxNat32                        first, cnt;
    afxViewport                     vp[_SGL_MAX_VP_PER_SET];
};

AFX_DEFINE_STRUCT(_afxDscrCmdNextPass)
{
    _afxDscrCmd                     cmd;
    afxBool32                       useAuxScripts;
};

AFX_DEFINE_STRUCT(_afxDscrCmdCopyTex)
{
    _afxDscrCmd                     cmd;
    afxTexture                      dst;
    afxTexture                      src;
    afxNat32                        rgnCnt;
    afxTextureRegion                rgn[16];
};
// vkCmdBindDescriptorSets - Binds descriptor sets to a command buffer

SGL afxError _SglCreateCombinedDeviceContext(WNDCLASSEXA *oglWndClss, HGLRC shareCtx, HWND *phwnd, HDC *phdc, HGLRC *phrc, wglVmt const* wgl, HMODULE opengl32);

SGL void _SglDeleteGlRes(afxDrawContext dctx, afxNat type, GLuint gpuHandle);


_SGL afxError AfxRegisterDrawDrivers(afxModule mdle, afxDrawSystem dsys);

SGL afxError _SglDqueBindAndSyncSmp(afxDrawQueue dque, afxNat unit, afxSampler smp, glVmt const* gl);
SGL afxError _SglDqueSyncShd(afxDrawQueue dque, afxShader shd, afxShaderStage stage, glVmt const* gl);
SGL afxError _SglDqueSurfSync(afxDrawQueue dque, afxSurface surf, glVmt const* gl); // must be used before texUpdate
SGL afxError _SglDqueBindAndSyncTex(afxDrawQueue dque, afxNat unit, afxTexture tex, glVmt const* gl);
SGL afxError _SglDqueBindAndSyncPip(afxDrawQueue dque, afxPipeline pip, glVmt const* gl);
SGL afxError _SglDqueBindAndResolveLego(afxDrawQueue dque, afxNat unit, afxLego legt, glVmt const* gl);
SGL afxError _SglDqueBindAndSyncCanv(afxDrawQueue dque, afxCanvas canv, GLenum target, glVmt const* gl);
SGL afxError _SglDqueBindAndSyncBuf(afxDrawQueue dque, afxNat unit, afxBuffer buf, afxNat offset, afxNat rangeOrVtxStride, GLenum target, GLenum usage, glVmt const* gl);

//SGL afxSize _AfxMeasureTextureRegion(afxTexture tex, afxTextureRegion const *rgn);

SGL BOOL SglMakeCurrent(HDC hdc, HGLRC hrc, wglVmt const *wgl);
SGL afxError SglSwapBuffers(HDC hdc, wglVmt const *wgl);
SGL int SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd, wglVmt const *wgl);
SGL BOOL SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd, wglVmt const *wgl);
SGL int SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd, wglVmt const *wgl);
SGL int SglGetPixelFormat(HDC hdc, wglVmt const *wgl);

SGL afxLego _SglDrawContextFindLego(afxDrawContext dctx, afxNat bindCnt, afxLegoBindingDecl const bindings[]);

SGL afxError _SglRegisterDoutEndpoints(afxDrawDriver ddrv);

SGL afxError _SglDdrvVmtDctxCb(afxDrawContext dctx, afxDrawContextConfig const *spec);
SGL afxError _SglDdrvVmtDoutCb(afxDrawOutput dout, afxUri const* endpoint, afxDrawOutputConfig const *spec);
SGL afxError _SglDdrvVmtDinCb(afxDrawInput din, afxUri const* endpoint, afxDrawInputConfig const *spec);

SGL afxError _SglDctxVmtDscrCb(afxDrawScript dscr);
SGL afxError _SglDdrvVmtDqueCb(afxDrawQueue dque, afxDrawQueueSpecification const *spec);

SGL afxError _SglBufCtor(afxBuffer buf);
SGL afxError _AfxCanvCtor(afxCanvas canv);
SGL afxError _AfxPipCtor(afxPipeline pip);
SGL afxError _SglTexCtor(afxTexture tex);
SGL afxError _AfxSurfCtor(afxSurface surf);
SGL afxError _AfxShdCtor(afxShader shd);
SGL afxError _AfxSmpCtor(afxSampler smp);


#endif//AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H