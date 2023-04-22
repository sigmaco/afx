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
#define _SGL_MAX_LEGO_PER_BIND 8
#define _SGL_MAX_ENTRY_PER_LEGO 10 
#define _SGL_MAX_STAGE_PER_PIP 8
#define _SGL_MAX_VBO_PER_BIND 8
#define _SGL_MAX_VAO_PER_TIME 60
#define _SGL_MAX_VP_PER_SET 8
#define _SGL_MAX_SCISSOR_PER_SET 8
#define _SGL_MAX_INSTREAM_PER_SET 8
#define _SGL_MAX_SURF_PER_CANV 8

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
    afxString128    subsysName;
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

    afxClass      pipmClass;
    afxClass      pipaClass;
    afxClass      pipClass; // uses pipm, pipa, canv
    afxClass      legoClass; // can use smp, tex, buf

    afxClass      dscrClass;
    afxClass      dinClass;
    afxClass      dengClass;
    afxClass      doutClass; // uses canv


    afxAllocator    genrlAll;
    afxNat              queueCnt;
    afxDrawQueue        *queues;

    afxBool             open;


    afxTransistor       texInstantiationLock;
    afxTransistor       pipmInstantiationLock;
};

AFX_DEFINE_STRUCT(_sglDeleteGlRes)
{
    GLuint  gpuHandle;
    GLuint  type; // 0 buf, 1 tex, 2 surf, 3 canv, 4 smp, 5 pip, 6 pipm
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

        afxPipeline                     pip;
        afxNat                          legoCnt;
        afxLego                         lego[_SGL_MAX_LEGO_PER_BIND];

        struct
        {
            afxBool                     wild;
            union
            {
                afxBuffer               buf; // wild
                afxVertexBuffer         vbuf; // managed
            };
            union
            {
                afxNat                  offset; // wild
                struct
                {
                    afxNat              baseVtx; // managed
                    afxNat              arr; // managed
                };
            };
            afxNat                      siz;
            afxNat                      stride;
        }                               vertexBindings[_SGL_MAX_VBO_PER_BIND];
        afxNat                          vertexBindingCnt;

        afxNat                          inStreamCnt;
        afxPipelineInputStream          inStreams[_SGL_MAX_INSTREAM_PER_SET];
        afxIndexBuffer                  ibuf;
        afxNat32                        baseIdx;

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
    afxString128    subsysName;

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
    afxPipeline     presentPip;
    afxLego         presentLego;
    afxSampler      presentSmp;
    afxVertexBuffer presentVbuf;
};

AFX_OBJECT(afxDrawOutput)
{
    afxObject               obj;

    afxNat                  whd[3]; // absolute extent
    afxNat                  whdMax[3]; // absolute extent available
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
    afxString1024 title;

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

AFX_DEFINE_STRUCT(afxDrawWork)
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

    afxAllocator        cmdAll;

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
    void*               udd;
};

AFX_DEFINE_STRUCT(_afxDscrCmd)
{
    afxNat              id;
    afxLinkage          script;
};

AFX_OBJECT(afxDrawScript)
{
    afxObject           obj;
    afxDrawScriptState  state;
    afxNat32            lastUpdTime;

    afxAllocator        cachedCmdAll; // Owned by draw input. Don't release it.
    _afxDscrCmd         cmdBegin;
    _afxDscrCmd         cmdEnd;
    afxChain            commands;
    afxNat              drawCallCount;
};

AFX_OBJECT(afxPipeline)
{
    AFX_OBJECT(afxResource)         res;
    afxNat                          stageCnt;
    struct
    {
        afxConnection               shader;
        afxShaderStage              type;
    }                               stages[_SGL_MAX_STAGE_PER_PIP];    
    afxNat                          inStreamCnt;
    afxPipelineInputStream          inStreams[_SGL_MAX_INSTREAM_PER_SET];

    afxLinkage                      rig;

    afxBool                         hasVtxInAssembling;
    afxPipelineInputAssemblyState   vtxInAssembling;

    // viewport state --- render area state
    afxNat                          vpCnt;
    afxViewport                     vps[_SGL_MAX_VP_PER_SET];
    afxNat                          scissorCnt;
    afxRect                         scissors[_SGL_MAX_SCISSOR_PER_SET];

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

AFX_OBJECT(afxPipelineModule)
{
    afxObject           obj;

    afxNat              len;
    afxByte             *code;

    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    afxBool         compiled;
};

AFX_OBJECT(afxPipelineRig) // A GPUPipelineLayout defines the mapping between resources of all GPUBindGroup objects set up during command encoding in setBindGroup(), and the shaders of the pipeline set by GPURenderCommandsMixin.setPipeline or GPUComputePassEncoder.setPipeline.
{
    afxObject               obj;
    afxChain                pipelines;
    afxNat32                socketCnt;
    afxLegoSchema       sockets[_SGL_MAX_LEGO_PER_BIND];
    //afxNat32                      pushConstantRangeCount;
    //VkPushConstantRange const*    pPushConstantRanges;
};

AFX_OBJECT(afxLego) // A GPUBindGroup defines a set of resources to be bound together in a group and how the resources are used in shader stages.
{
    afxObject       obj;
    afxLegoSchema   schema; // The GPUBindGroupLayout associated with this GPUBindGroup. Read-only
    //afxNat        pointCnt;
    afxLegoPoint    points[_SGL_MAX_ENTRY_PER_LEGO]; // The set of GPUBindGroupEntrys this GPUBindGroup describes. Read-only
    afxNat          usedResources; // The set of buffer and texture subresources used by this bind group, associated with lists of the internal usage flags. Read-only
};

AFX_OBJECT(afxSampler)
{
    afxObject           obj;

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

AFX_OBJECT(afxBuffer)
{
    afxObject           obj;

    afxSize             siz;
    afxBufferUsage      usage;
    afxByte             *bytemap;

    afxBool             locked;
    afxSize             lastUpdOffset, lastUpdRange;

    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle;
    GLenum          glTarget;
    GLenum          glUsage;
};


AFX_OBJECT(afxIndexBuffer)
{
    AFX_OBJECT(afxBuffer)   buf; // IBUF
    afxNat                  idxCnt;
    afxNat                  idxSiz;
};

AFX_OBJECT(afxVertexBuffer)
{
    AFX_OBJECT(afxBuffer)   buf;
    afxNat                  vtxCnt;
    afxNat                  rowCnt;
    afxVertexDataRow        *row;
};

AFX_OBJECT(afxTexture)
{
    AFX_OBJECT(afxResource) res;
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

    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle; // if depth/stencil, should have a sidecar GL render buffer.
};

AFX_OBJECT(afxCanvas) // our concept of a framebuffer
{
    afxObject           obj;

    afxNat              whd[3]; // d= layers; just for 3D images, else case its 1.
    afxNat              surfaceCnt;
    afxSurface          surfaces[_SGL_MAX_SURF_PER_CANV]; // 8 raster surfaces [afxTexture] are minimal amount garanteed.

    // idd

    sglUpdateFlags  updFlags;
    afxNat          glHandle[_SGL_MAX_DQUE_PER_DCTX];
};

typedef struct _DrawPipelineResource
{
    afxShaderStage stages;
    afxPipelineAccessType resType;
    //afxShaerBaseResType baseType;
    //VkAccessFlags access;
    afxNat set;
    afxNat binding;
    afxNat location;
    afxNat inputAttachmentIndex;
    afxNat vecSize;
    afxNat arraySize;
    afxNat offset;
    afxNat size;
    afxChar name[16];
} _DrawPipelineResource;


typedef enum afxDrawScriptCmdId
{
    AFX_DSC_END,
    AFX_DSC_BEGIN,
    AFX_DSC_BEGIN_RENDER_PASS,
    AFX_DSC_BIND_BUFFER,
    AFX_DSC_BIND_TEXTURES,
    AFX_DSC_BIND_INDEX_BUFFER,
    AFX_DSC_BIND_PIPELINE,
    AFX_DSC_BIND_SAMPLERS,
    AFX_DSC_BIND_VERTEX_BUFFERS,
    AFX_DSC_BIND_WILD_VERTEX_BUFFERS,
    AFX_DSC_BIND_CONSTANT_BUFFERS,
    AFX_DSC_BIND_PIPELINE_PLUGS,
    AFX_DSC_BLIT_IMAGE,
    AFX_DSC_CLEAR_ATTACHMENTS,
    AFX_DSC_CLEAR_COLOR_IMAGE,
    AFX_DSC_CLEAR_DEPTH_STENCIL_IMAGE,
    AFX_DSC_COPY_BUFFER,
    AFX_DSC_COPY_BUFFER_TO_IMAGE,
    AFX_DSC_COPY_IMAGE,
    AFX_DSC_COPY_IMAGE_TO_BUFFER,
    AFX_DSC_DISPATCH,
    AFX_DSC_DISPATCH_INDIRECT,
    AFX_DSC_DRAW,
    AFX_DSC_DRAW_INDEXED,
    AFX_DSC_DRAW_INDEXED_INDIRECT,
    AFX_DSC_DRAW_INDIRECT,
    AFX_DSC_END_RENDER_PASS,
    AFX_DSC_FILL_BUFFER,
    AFX_DSC_NEXT_SUBPASS,
    AFX_DSC_PUSH_CONSTANTS,
    AFX_DSC_RESET_EVENT,
    AFX_DSC_RESOLVE_IMAGE,
    AFX_DSC_SET_BLEND_CONSTANTS,
    AFX_DSC_SET_COLOR_BLEND_STATE,
    AFX_DSC_SET_DEPTH_BIAS,
    AFX_DSC_SET_DEPTH_BOUNDS,
    AFX_DSC_SET_DEPTH_STENCIL_STATE,
    AFX_DSC_SET_EVENT,
    AFX_DSC_SET_INPUT_ASSEMBLY_STATE,
    AFX_DSC_SET_LINE_WIDTH,
    AFX_DSC_SET_MULTISAMPLE_STATE,
    AFX_DSC_SET_RASTERIZATION_STATE,
    AFX_DSC_SET_SCISSOR,
    AFX_DSC_SET_STENCIL_COMPARE_MASK,
    AFX_DSC_SET_STENCIL_REFERENCE,
    AFX_DSC_SET_STENCIL_WRITE_MASK,
    AFX_DSC_SET_VERTEX_INPUT_FORMAT,
    AFX_DSC_SET_VIEWPORT,
    AFX_DSC_SET_VIEWPORT_STATE,
    AFX_DSC_UPDATE_BUFFER,
    AFX_DSC_COPY_TEXTURE,
    AFX_DSC_BEGIN_RENDERING,
    AFX_DSC_END_RENDERING,
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

AFX_DEFINE_STRUCT(_afxDscrCmdBeginRenderPass)
{
    _afxDscrCmd                     cmd;
    afxCanvas                       canv;
    afxRect                         area;
    afxNat                          annexCnt;
    afxRenderPassAnnex              annexes[_SGL_MAX_SURF_PER_CANV];
};

AFX_DEFINE_STRUCT(_afxDscrCmdBeginRendering)
{
    _afxDscrCmd             cmd;
    afxRect                 area;
    afxNat                  layerCnt;
    afxNat                  rasterCnt;
    afxDrawTarget         rasters[_SGL_MAX_SURF_PER_CANV];
    afxDrawTarget         depth;
    afxDrawTarget         stencil;
};

AFX_DEFINE_STRUCT(_afxDscrCmdEndRendering)
{
    _afxDscrCmd                     cmd;
};

AFX_DEFINE_STRUCT(_afxDscrCmdEndRenderPass)
{
    _afxDscrCmd                     cmd;
};

AFX_DEFINE_STRUCT(_afxDscrCmdBindLego)
{
    _afxDscrCmd                     cmd;
    afxNat32                        first, cnt;
    afxLego                 lego[_SGL_MAX_LEGO_PER_BIND];
};

AFX_DEFINE_STRUCT(_afxDscrCmdBindVbuf)
{
    _afxDscrCmd                     cmd;
    afxNat32                        first, cnt;
    union
    {
        afxVertexBuffer             vbuf[_SGL_MAX_VBO_PER_BIND];
        afxBuffer                   buf[_SGL_MAX_VBO_PER_BIND];
    };
    union
    {
        struct
        {
            afxNat32                baseVtx[_SGL_MAX_VBO_PER_BIND];
            afxNat32                vtxArr[_SGL_MAX_VBO_PER_BIND];
        };
        afxSize                     offset[_SGL_MAX_VBO_PER_BIND];
    };
    afxSize                         vtxArrSiz[_SGL_MAX_VBO_PER_BIND];
    afxSize                         vtxStride[_SGL_MAX_VBO_PER_BIND];
    afxBool                         wild;
};


AFX_DEFINE_STRUCT(_afxDscrCmdBindIbuf)
{
    _afxDscrCmd                     cmd;
    afxIndexBuffer                  ibuf;
    afxNat32                        baseIdx;
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

AFX_DEFINE_STRUCT(_afxDscrCmdCopyTex)
{
    _afxDscrCmd                     cmd;
    afxTexture                      dst;
    afxTexture                      src;
    afxNat                          rgnCnt;
    afxTextureRegion                rgn[16];
};
// vkCmdBindDescriptorSets - Binds descriptor sets to a command buffer

SGL afxDrawScript _AfxDrawInputAcquireScript(afxDrawInput din, afxBool recycle);
SGL afxResult _SglCreateCombinedDeviceContext(WNDCLASSEXA *oglWndClss, HGLRC shareCtx, HWND *phwnd, HDC *phdc, HGLRC *phrc);

SGL void _SglEnqueueGlResourceDeletion(afxDrawContext dctx, afxNat queueIdx, afxNat type, GLuint gpuHandle);

// buf
SGL afxError _AfxBufferDump2(afxBuffer buf, afxSize offset, afxSize stride, afxSize cnt, void *dst, afxSize dstStride);
SGL afxError _AfxBufferDump(afxBuffer buf, afxSize base, afxSize range, void *dst);
SGL afxError _AfxBufferUpdate2(afxBuffer buf, afxSize offset, afxSize stride, afxNat cnt, void const *src, afxSize srcStride);
SGL afxError _AfxBufferUpdate(afxBuffer buf, afxSize base, afxSize range, void const *src);
SGL void const* _AfxBufferGetData(afxBuffer buf, afxSize offset);
SGL afxSize _AfxBufferGetSize(afxBuffer buf);



_SGL afxError AfxRegisterDrawDrivers(afxModule mdle, afxDrawSystem dsys);

SGL afxError _SglDqueBindAndSyncSmp(afxDrawQueue dque, afxNat unit, afxSampler smp);
SGL afxError _SglDqueSyncPipm(afxDrawQueue dque, afxPipelineModule pipm, afxShaderStage stage);
SGL afxError _SglDqueSurfSync(afxDrawQueue dque, afxSurface surf); // must be used before texUpdate
SGL afxError _SglDqueBindAndSyncTex(afxDrawQueue dque, afxNat unit, afxTexture tex);
SGL afxError _SglDqueBindAndSyncPip(afxDrawQueue dque, afxPipeline pip);
SGL afxError _SglDqueBindAndSyncLego(afxDrawQueue dque, afxNat unit, afxLego lego);
SGL afxError _SglDqueBindAndSyncCanv(afxDrawQueue dque, afxCanvas canv, GLenum target);
SGL afxError _SglDqueBindAndSyncBuf(afxDrawQueue dque, afxNat unit, afxBuffer buf, afxNat offset, afxNat rangeOrVtxStride, GLenum target, GLenum usage);

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


SGL afxResult _AfxStdDoutImplCtor(afxDrawOutput dout, afxUri const *uri);

SGL afxDctxImpl const _AfxStdDctxImpl;
SGL afxError _AfxDctxCtor(afxDrawContext dctx, afxDrawContextSpecification const *spec);
SGL afxError _AfxDctxDtor(afxDrawContext dctx);

SGL afxError _SglDoutProcess(afxDrawOutput dout);
SGL void _SglDqueProcess(afxDrawQueue dque);
SGL afxResult _AfxDqueImplDrawWorkStreaming(afxDrawQueue dque, afxDrawWork const *dwrk);
SGL afxResult _AfxDqueImplDrawWorkSubmission(afxDrawQueue dque, afxDrawWork const *dwrk);
SGL afxResult _AfxDqueImplDrawWorkPresentation(afxDrawQueue dque, afxDrawWork const *dwrk);
SGL afxError _AfxDrawInputProcess(afxDrawInput din, afxNat queueIdx);
SGL afxError _SglSwapBuffers(HDC hdc);
SGL int _SglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd);
SGL BOOL _SglSetPixelFormat(HDC hdc, int format, CONST PIXELFORMATDESCRIPTOR * ppfd);
SGL int _SglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes, LPPIXELFORMATDESCRIPTOR ppfd);

SGL afxDrawQueue _AfxDrawContextAcquireQueue(afxDrawContext dctx, afxNat idx, afxBool autonomous);
SGL afxDrawOutput _AfxDrawContextAcquireOutput(afxDrawContext dctx, afxWhd const extent, afxDrawOutputSpecification const *spec);
SGL afxDrawInput _AfxDrawContextAcquireInput(afxDrawContext dctx, afxDrawInputSpecification const *spec);
SGL afxCanvas _AfxDrawContextAcquireCanvas(afxDrawContext dctx, afxWhd const extent, afxNat surfaceCnt, afxSurfaceSpecification const *specs);
SGL afxBuffer _AfxDrawContextAcquireBuffer(afxDrawContext dctx, afxBufferSpecification const *spec);
SGL afxVertexBuffer _AfxDrawContextBuildVertexBuffer(afxDrawContext dctx, afxNat vtxCnt, afxNat rowCnt, afxVertexDataSpecification const *specs);
SGL afxSurface _AfxDrawContextAcquireSurface(afxDrawContext dctx, afxPixelFormat fmt, afxWhd const extent, afxFlags usage);
SGL afxIndexBuffer _AfxDrawContextAcquireIndexBuffer(afxDrawContext dctx, afxIndexBufferSpecification const *spec);
SGL afxDrawScript _AfxDrawInputAcquireScript(afxDrawInput din, afxBool recycle);
SGL afxError _AfxDrawContextFindTextures(afxDrawContext dctx, afxNat cnt, afxUri const name[], afxTexture tex[]);
SGL afxError _AfxDrawContextBuildTextures(afxDrawContext dctx, afxNat cnt, afxTextureBlueprint const texb[], afxTexture tex[]);
SGL afxSampler _AfxDrawContextAcquireSampler(afxDrawContext dctx, afxSamplerSpecification const *spec);
SGL afxPipelineModule _AfxDrawContextFetchPipelineModule(afxDrawContext dctx, afxUri const *uri);
SGL afxPipelineModule _AfxDrawContextFindPipelineModule(afxDrawContext dctx, afxUri const *name);
SGL afxPipelineModule _AfxDrawContextUploadPipelineModule(afxDrawContext dctx, afxUri const *uri);
SGL afxPipelineModule _AfxDrawContextBuildPipelineModule(afxDrawContext dctx, afxByte const code[], afxNat len);
SGL afxPipeline _AfxDrawContextFetchPipeline(afxDrawContext dctx, afxUri const *uri);
SGL afxPipeline _AfxDrawContextFindPipeline(afxDrawContext dctx, afxUri const *name);
SGL afxPipeline _AfxDrawContextUploadPipeline(afxDrawContext dctx, afxUri const *uri);
SGL afxPipeline _AfxDrawContextBuildPipeline(afxDrawContext dctx, afxPipelineBlueprint const *pipb);
SGL afxPipelineRig _AfxDrawContextBuildPipelineRig(afxDrawContext dctx, afxNat socketCnt, afxLegoSchema const socket[]);
SGL afxLego _AfxDrawContextAcquireLego(afxDrawContext dctx, afxLegoSchema const *socket, afxLegoPoint const points[]);

#endif//AFX_STD_DRAW_DRIVER_IMPLEMENTATION_H