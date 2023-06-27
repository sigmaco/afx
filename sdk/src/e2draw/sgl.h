/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H
#define AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H

#include "../src/e2coree/draw/_classified/afxDrawClassified.h"
#include "sglDriver.h"

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

AFX_DEFINE_STRUCT(_sglDriverIdd)
{
    WNDCLASSEXA     oglWndClss;
    HWND            wglPrimeWnd;
    HDC             wglPrimeDc;
    afxInt          wglPrimeDcPxlFmt;
    HGLRC           wglPrimeGlrc;
    sglVmt const    wglPrimeVmt;
    afxNat          wglPrimeGlrcVerMajor, wglPrimeGlrcVerMinor, wglPrimeGlrcVerPatch;
    afxString*      subsysName;
};

AFX_OBJECT(afxDrawContext)
{
    afxObject           obj;
    afxChain            provisions;
    afxClass      bufClass;
    afxClass      vbufClass;
    afxClass      ibufClass;

    afxClass      smpClass;
    afxClass      texClass;
    afxClass      surfClass;
    afxClass      canvClass; // uses surf

    afxClass      shdClass;
    afxClass      pipClass; // uses shd, pipa, canv
    afxClass      legtClass;
    afxClass      dopClass;

    afxClass      dscrClass;
    afxClass      dinClass;
    afxClass      dengClass;
    afxClass      doutClass; // uses canv


    afxMemory    genrlMem;
    afxNat              queueCnt;
    afxDrawQueue        *queues;

    afxBool             open;


    afxTransistor       texInstantiationLock;
    afxTransistor       shdInstantiationLock;
};

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

AFX_OBJECT(afxDrawQueue)
{
    afxObject           obj; // AFX_FCC_DQUE
    afxNat              queueIdx;
    afxThread           thread;

    afxTransistor       deletionLock;
    afxQueue            deletionQueue;
    afxChain            instantiationList;

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
            afxDrawOperation            dop;
            afxNat                      activeTec;
            afxNat                      activePass;
            afxCanvas                   canv;
            afxRect                     area;
            afxNat                      annexCnt;
            afxRenderPassAnnex          annexes[_SGL_MAX_SURF_PER_CANV];

            afxNat                      layerCnt; // dyn
            afxNat                      rasterCnt;
            afxDrawTarget               rasters[_SGL_MAX_SURF_PER_CANV];
            afxDrawTarget               depthRt;
            afxDrawTarget               stencilRt;
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
            afxNat                      siz;
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

    // idd
    afxBool         instanced;

    // deveria ter hglrc, hdc, etc para ser usado como transfer queue, assim as filas poderia criar contextos compartilhados sobre este.
    // deve haver ao menos uma 1 draw queue e todas as outras deveriam ser compartilhadas com ela. A primeira deveria ser preferencial para streaming.
    HWND            wglWnd;
    HDC             wglDc;
    afxInt          wglDcPxlFmt;
    HGLRC           wglGlrc;
    sglVmt const    wglVmt;
    afxNat          wglGlrcVerMajor, wglGlrcVerMinor, wglGlrcVerPatch;
    afxString*      subsysName;

    //_gfxRendererContextState state;

    // submission stuff
    afxBool         submissionSuspended;
    GLuint          tmpFbo[_SGL_MAX_SWAPCHAIN_CAP];
    afxNat          activeTmpFboIdx;
    GLuint          vao[_SGL_MAX_VAO_PER_TIME];
    afxNat          activeVaoIdx;

    // presentation stuff
    afxBool         presentationSuspended;
    GLuint          presentFboGpuHandle;
    afxDrawOperation    presentDop;
    afxSampler      presentSmp;
    afxVertexBuffer presentVbuf;
};

AFX_OBJECT(afxDrawOutput)
{
    afxObject               obj;

    afxWhd                  whd; // absolute extent
    afxWhd                  whdMax; // absolute extent available
    afxReal                 aspectRatio;
    afxBool                 resizable;

    // swapchain-related data
    afxFlags                flags;
    void*                   surface; // external handle, usually a OS window handle.
    afxPixelFormat          pixelFmt; // pixel format of raster surfaces.
    afxColorSpace           colorSpc; // color space of raster surfaces. sRGB is the default.
    afxTextureUsage         bufUsage; // what evil things we will do with it? Usually AFX_TEX_USAGE_SURFACE_RASTER
    afxNat                  bufCnt; // usually 2 or 3; double or triple buffered.
    afxTransistor           buffersLock;
    afxSurface              buffers[_SGL_MAX_SURF_PER_DOUT]; // afxCanvas // should have 1 fb for each swapchain raster.
    afxNat                  lastAcqBufIdx;
    afxBool                 swapping;
    afxChain                swapchain; // display order
    afxPresentAlpha         presentAlpha; // consider transparency for external composing (usually on windowing system).
    afxPresentTransform     presentTransform; // NIL leaves it as it is.
    afxPresentMode          presentMode; // FIFO
    afxBool                 clipped; // usually true to don't do off-screen draw.

    afxV2d                  absCursorPos,
                            absCursorMove,
                            ndcCursorPos,
                            ndcCursorMove;

    afxBool                 open;

    // idd
    afxBool     instanced;
    afxString*  title;

    HWND        wglWnd;
    HDC         wglDc;
    afxInt      wglDcPxlFmt;

    HDC         wglDcDeskBkp;
    afxInt      wglDcPxlFmtDeskBkp;
};

typedef enum afxDrawWorkType
{
    AFX_DRAW_WORK_TYPE_SUBMISSION,
    AFX_DRAW_WORK_TYPE_PRESENTATION,
    AFX_DRAW_WORK_TYPE_STREAMING // flushes mapped and/or cached memory ranges for CPU/GPU.
} afxDrawWorkType;

AFX_DEFINE_STRUCT(afxDrawWorkload)
{
    afxDrawWorkType         type;
    afxTime                 timestamp;
    union
    {
        struct
        {
            afxNat          scriptCnt;
            afxDrawScript   scripts[_SGL_MAX_DSCR_PER_SUBMISSION];
        }                   submission;
        struct
        {
            afxNat          outputCnt;
            afxDrawOutput   outputs[_SGL_MAX_DOUT_PER_PRESENTATION];
            afxNat          outputBufIdx[_SGL_MAX_DOUT_PER_PRESENTATION];
        }                   presentation;
        struct
        {
            afxNat          resCnt;
            afxObject       *resources[_SGL_MAX_OBJ_PER_STREAMING];
            afxNat          base[3]; // buffers uses only 0.
            afxNat          range[3]; // buffers uses only 0.
        }                   streaming;
    };
};

AFX_OBJECT(afxDrawInput)
{
    afxObject           obj;

    afxMemory        cmdAll;

    afxTransistor       streamingLock;
    afxQueue            streamingQueue;
    afxNat              streamingThreadEnabled[_SGL_MAX_DQUE_PER_DCTX];

    afxTransistor       submissionLock;
    afxQueue            submissionQueue;
    afxNat              submissionThreadEnabled[_SGL_MAX_DQUE_PER_DCTX];

    afxTransistor       presentationLock;
    afxQueue            presentationQueue;
    afxNat              presentationThreadEnabled[_SGL_MAX_DQUE_PER_DCTX];

    afxTransistor       prefetchLock;
    afxError            (*prefetchProc)(afxDrawInput din, afxNat queueIdx, void *udd);
    afxBool             prefetchThreadEnabled[_SGL_MAX_DQUE_PER_DCTX];
    void*               udd[4];
};

AFX_DEFINE_STRUCT(_afxDscrCmd)
{
    afxNat              id;
    afxNat              siz;
    afxLinkage          script;
};

AFX_OBJECT(afxDrawScript)
{
    afxObject           obj;
    afxDrawScriptState  state;
    afxNat32            lastUpdTime;

    afxMemory           cmdAll; // Owned by draw input. Don't release it.
    _afxDscrCmd         cmdEnd;
    afxChain            commands;
    afxNat              drawCallCount;
};

AFX_OBJECT(afxPipeline)
{
    afxObject                       obj;
    afxNat                          shaderCnt;
    afxShader*                      shaders;

    struct
    {
        afxNat                      set;
        afxLego             legt;
        afxBool                     resolved;
    }                               *wiring;
    afxNat                          wiringCnt;

    afxNat                          inCnt;
    afxPipelineInputStream          ins[8];

    afxBool                         hasVtxInAssembling;
    afxPipelineInputAssemblyState   vtxInAssembling;

    // viewport state --- render area state
    afxNat                          vpCnt;
    afxViewport*                    vps;
    afxNat                          scissorCnt;
    afxRect*                        scissors;

    afxBool                         hasRasterization;
    afxPipelineRasterizerState      rasterization;
    afxBool                         hasMultisampling;
    afxPipelineMultisampleState     multisampling;
    afxBool                         hasDepthHandling;
    afxPipelineDepthState           depthHandling;
    afxBool                         hasColorBlending;
    afxPipelineColorBlendState      colorBlending;


    sglUpdateFlags  updFlags;
    afxNat          glHandle[_SGL_MAX_DQUE_PER_DCTX];
    afxBool         assembled;
};

AFX_OBJECT(afxShader)
{
    afxObject           obj;
    afxUri*             uri;
    afxShaderStage      stage;

    afxByte*            code;
    afxNat16            codeLen;
    afxString*          entry;

    afxNat8                     resDeclCnt;
    afxShaderBlueprintResource  *resDecls;

    afxNat8                     ioDeclCnt;
    afxShaderBlueprintInOut     *ioDecls;
    afxPrimTopology             topology;

    sglUpdateFlags      updFlags;
    afxNat              glHandle;
    afxNat              glProgHandle[_SGL_MAX_DQUE_PER_DCTX];
    afxBool             compiled;
};

AFX_DEFINE_STRUCT(afxLegoEntry) // A GPUBindGroupLayoutEntry describes a single shader resource binding to be included in a GPUBindGroupLayout.
{
    // A afxLegoDataScheme describes a single shader resource binding to be included in a afxLegoScheme.
    afxNat32                binding; // A unique identifier for a resource binding within the afxLegoScheme, corresponding to a afxLegoDataScheme.binding and a @binding attribute in the afxShader.
    afxFlags                visibility; // A bitset of the members of afxShaderStage. Each set bit indicates that a afxLegoDataScheme's resource will be accessible from the associated shader stage.
    afxShaderResourceType   type;
    afxNat                  cnt;
    afxString*              name; // by QWADRO

    union
    {
        struct
        {
            afxBool hasDynOffsets; // if exist it is a push constant block?
            afxNat  minSizeBound;
            afxBool writeable; // if true it is no uniform buffer?
        } buf;
        struct
        {
            int a;
        } img;
    };
};

AFX_OBJECT(afxLego) // A GPUBindGroupLayout defines the interface between a set of resources bound in a GPUBindGroup and their accessibility in shader stages.
{
    // A afxLegoScheme defines the interface between a set of resources bound in a GPUBindGroup and their accessibility in shader stages.
    afxObject               obj;
    afxNat32                crc32;
    afxNat                  entryCnt;
    afxLegoEntry    *entries; // The map of binding indices pointing to the GPUBindGroupLayoutEntrys, which this GPUBindGroupLayout describes.    
};

AFX_OBJECT(afxSampler)
{
    afxObject           obj;
    afxNat32            crc32;
    afxTexelFilter      magFilter; // LINEAR. The texture magnification function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be magified.
    afxTexelFilter      minFilter; // NEAREST. The texture minifying function is used whenever the level-of-detail function used when sampling from the texture determines that the texture should be minified. There are six defined minifying functions.
    afxTexelFilter      mipmapFilter; // LINEAR.
    afxTexelAddress     uvw[3]; // REPEAT. Sets the wrap parameter for texture coordinates.
    afxBool             anisotropyEnabled; // FALSE
    afxReal             anisotropyMaxDegree; // 1
    afxBool             compareEnabled;
    afxCompareOp        compareOp; // LEQUAL
    afxReal             lodBias; // 0. Specifies a fixed bias value that is to be added to the level-of-detail parameter for the texture before texture sampling.
    afxReal             minLod; // -1000. Sets the minimum level-of-detail parameter. This floating-point value limits the selection of highest resolution mipmap (lowest mipmap level).
    afxReal             maxLod; // 1000. Sets the maximum level-of-detail parameter. This floating-point value limits the selection of the lowest resolution mipmap (highest mipmap level).
    afxColor            borderColor; // (0, 0, 0, 0). Specifies the color that should be used for border texels. If a texel is sampled from the border of the texture, this value is used for the non-existent texel data. If the texture contains depth components, the first component of this color is interpreted as a depth value.
    afxBool             unnormalizedCoords;


    sglUpdateFlags  updFlags;
    afxNat          glHandle;
};

typedef struct
{
    afxByte             *bytemap;

    afxBool             locked;
    afxSize             lastUpdOffset, lastUpdRange;

    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    GLenum          glTarget;
    GLenum          glUsage;
} sglBufIdd;

static_assert(AFX_BUF_IDD >= sizeof(sglBufIdd), "");

AFX_OBJECT(afxTexture)
{
    afxObject               obj;
    afxTextureFlag          flags;

    afxPixelFormat          fmt;
    afxNat                  whd[3]; // extent of image
    afxByte                 *bytemap; // afxByte*
    afxByte                 *sidecar; // afxByte*

    afxNat                  layerCnt;
    afxNat                  lodCnt; // mip level cnt
    afxNat                  sampleCnt; // 1, 2, 4, 8, 16, 32, or 64.
    afxBool                 linearTiling; // optimal tiling (for driver) if false.
    afxFlags                usage;
    afxColorSwizzling const *swizzling;

    afxBool                 locked;
    afxNat                  lastUpdOffset[3];
    afxNat                  lastUpdRange[3];

    afxLinkage              res;
    afxUri*                 uri;

    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    GLint           glIntFmt;
    GLenum          glTarget, glFmt, glType;
};

AFX_OBJECT(afxSurface) // our concept of a framebuffer
{
    AFX_OBJECT(afxTexture)  tex;
    afxSurfaceState         state;
    afxLinkage              swapchain; // if state == PENDING, must have a swapchain linked to this canvas. Used by draw context on canvas presentation to surface.

    //afxCanvasSurfaceInterlink interlinkSentinel;

    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle; // if depth/stencil, should have a sidecar GL render buffer.
};

AFX_OBJECT(afxCanvas) // our concept of a framebuffer
{
    afxObject       obj;

    afxNat          whd[3]; // d= layers; just for 3D images, else case its 1.

    afxNat          surfCnt;
    afxSurface      surfaces[_SGL_MAX_SURF_PER_CANV]; // 8 raster surfaces [afxTexture] are minimal amount garanteed.

    //afxCanvasSurfaceInterlink interlinkSentinel;

    afxNat          rasterCnt;
    afxNat          depthIdx;
    afxNat          stencilIdx; // should be the same as depth.
    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle[_SGL_MAX_DQUE_PER_DCTX];
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


typedef enum afxDrawScriptCmdId
{
    AFX_DSC_END,

    AFX_DSC_BEGIN_COMBINATION,
    AFX_DSC_BEGIN_OPERATION,
    AFX_DSC_EMPLOY_TECHNIQUE,
    AFX_DSC_NEXT_PASS,
    AFX_DSC_END_OPERATION,
    AFX_DSC_END_COMBINATION,

    AFX_DSC_BIND_PIPELINE,

    AFX_DSC_SET_INPUT_ASSEMBLY_STATE,
    AFX_DSC_SET_VIEWPORTS,
    AFX_DSC_SET_RASTERIZATION_STATE,
    AFX_DSC_SET_DEPTH_STATE,
    AFX_DSC_SET_SCISSORS,

    AFX_DSC_BIND_BUFFERS,
    AFX_DSC_BIND_VERTEX_BUFFERS,
    AFX_DSC_BIND_INDEX_BUFFER,
    AFX_DSC_BIND_TEXTURES,

    AFX_DSC_DRAW,
    AFX_DSC_DRAW_INDEXED,

    AFX_DSC_TOTAL
} afxDrawScriptCmdId;


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

AFX_DEFINE_STRUCT(_afxDscrCmdBeginOp)
{
    _afxDscrCmd                     cmd;
    afxDrawOperation                dop;
    afxNat                          tecIdx;
    afxCanvas                       canv;
    afxRect                         area;
    afxNat32                        annexCnt;
    afxRenderPassAnnex              annexes[_SGL_MAX_SURF_PER_CANV];
};

AFX_DEFINE_STRUCT(_afxDscrCmdBeginComb)
{
    _afxDscrCmd             cmd;
    afxRect                 area;
    afxNat32                layerCnt;
    afxNat32                rasterCnt;
    afxDrawTarget         rasters[_SGL_MAX_SURF_PER_CANV];
    afxDrawTarget         depth;
    afxDrawTarget         stencil;
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
    afxSize                         vtxArrSiz[_SGL_MAX_VBO_PER_BIND];
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

AFX_DEFINE_STRUCT(_afxDscrCmdEmployTec)
{
    _afxDscrCmd                     cmd;
    afxNat                          tecIdx;
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

SGL afxError _SglCreateCombinedDeviceContext(WNDCLASSEXA *oglWndClss, HGLRC shareCtx, HWND *phwnd, HDC *phdc, HGLRC *phrc);

SGL void _SglEnqueueGlResourceDeletion(afxDrawContext dctx, afxNat queueIdx, afxNat type, GLuint gpuHandle);

// buf
SGL afxBufImpl const _SglBufImpl;
SGL afxError _AfxBufferDump2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride);
SGL afxError _AfxBufferDump(afxBuffer buf, afxSize base, afxSize range, void *dst);
SGL afxError _AfxBufferUpdate2(afxBuffer buf, afxSize offset, afxSize stride, afxNat cnt, void const *src, afxSize srcStride);
SGL afxError _AfxBufferUpdate(afxBuffer buf, afxSize base, afxSize range, void const *src);
SGL void const* _AfxBufferGetData(afxBuffer buf, afxSize offset);
SGL afxSize _AfxBufferGetSize(afxBuffer buf);



_SGL afxError AfxRegisterDrawDrivers(afxModule mdle, afxDrawSystem dsys);

SGL afxError _SglDqueBindAndSyncSmp(afxDrawQueue dque, afxNat unit, afxSampler smp);
SGL afxError _SglDqueSyncShd(afxDrawQueue dque, afxShader shd, afxShaderStage stage);
SGL afxError _SglDqueSurfSync(afxDrawQueue dque, afxSurface surf); // must be used before texUpdate
SGL afxError _SglDqueBindAndSyncTex(afxDrawQueue dque, afxNat unit, afxTexture tex);
SGL afxError _SglDqueBindAndSyncPip(afxDrawQueue dque, afxPipeline pip);
SGL afxError _SglDqueBindAndResolveLego(afxDrawQueue dque, afxNat unit, afxLego legt);
SGL afxError _SglDqueBindAndSyncCanv(afxDrawQueue dque, afxCanvas canv, GLenum target);
SGL afxError _SglDqueBindAndSyncBuf(afxDrawQueue dque, afxNat unit, afxBuffer buf, afxNat offset, afxNat rangeOrVtxStride, GLenum target, GLenum usage);

SGL afxUri const* _AfxTextureGetUri(afxTexture tex);
SGL afxBool _AfxTextureTestFlags(afxTexture tex, afxTextureFlag flags);
SGL afxNat _AfxTextureGetLodCount(afxTexture tex);
SGL afxNat _AfxTextureGetLayerCount(afxTexture tex);
SGL afxColorSwizzling const* _AfxTextureGetColorSwizzling(afxTexture tex);
SGL afxNat _AfxTextureGetSampleCount(afxTexture tex);
SGL afxResult _AfxTextureTestUsageFlags(afxTexture tex, afxFlags flags);
SGL afxPixelFormat _AfxTextureGetFormat(afxTexture tex);
SGL afxNat* _AfxTextureGetExtent(afxTexture tex, afxNat lod, afxWhd extent);
SGL afxError _AfxTextureSetExtent(afxTexture tex, afxNat layerCnt, afxWhd const extent);
SGL void _AfxTextureUnmap(afxTexture tex);
SGL void* _AfxTextureMap(afxTexture tex, afxTextureRegion const *rgn, afxSize *size);
SGL void const* _AfxTextureGetData(afxTexture tex, afxNat lod, afxNat layer, afxWhd const offset);
//SGL afxSize _AfxTextureMeasure(afxTexture tex, afxTextureRegion const *rgn);
SGL afxError _AfxTextureUpdateRegions(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], void const *src[], afxPixelFormat const fmt[]);
SGL afxError _AfxTextureGenerateLods(afxTexture tex, afxNat base, afxNat cnt);


SGL afxError _AfxStdDoutImplCtor(void *cache, afxDrawOutput dout, afxUri const *uri);

SGL afxDctxImpl const _AfxStdDctxImpl;
SGL afxError _AfxDctxCtor(void *cache, afxNat idx, afxDrawContext dctx, _afxDctxCtorArgs const *args);
SGL afxError _AfxDctxDtor(afxDrawContext dctx);
SGL afxBool _SglDctxEventHandler(afxObject *obj, afxEvent *ev);
SGL afxBool _SglDctxEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev);

SGL afxError _SglDoutProcess(afxDrawOutput dout);
SGL void _SglDqueProcess(afxDrawQueue dque);
SGL afxError _AfxDrawInputProcess(afxDrawInput din, afxNat queueIdx);
SGL afxError _SglSwapBuffers(HDC hdc);
SGL int _SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);
SGL BOOL _SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd);
SGL int _SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);

SGL afxResult _AfxLegoDescribe(afxLego legt, afxNat base, afxNat cnt, afxLegoBindingDecl decl[]);

SGL afxLego _SglDrawContextFindLego(afxDrawContext dctx, afxNat bindCnt, afxLegoBindingDecl const bindings[]);

#endif//AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H