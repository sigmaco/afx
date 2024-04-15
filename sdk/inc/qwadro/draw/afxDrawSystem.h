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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

// The Unified Qwadro Video Graphics Infrastructure

#ifndef AFX_DRAW_SYSTEM_H
#define AFX_DRAW_SYSTEM_H

#include "qwadro/core/afxDevice.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/afxColor.h"
#include "qwadro/draw/afxPixel.h"
// provided classes.
#include "qwadro/draw/dev/afxDrawBridge.h"
#include "qwadro/draw/dev/afxDrawStream.h"
#include "qwadro/draw/pipe/afxDrawOps.h"
#include "qwadro/draw/dev/afxDrawContext.h"
#include "qwadro/draw/dev/afxDrawOutput.h"
#include "qwadro/draw/dev/afxDrawInput.h"
// provided classes.
#include "qwadro/draw/afxCamera.h"

AFX_DEFINE_STRUCT(afxDrawDeviceLimits)
{
    afxNat maxImageDimension1D;
    afxNat maxImageDimension2D;
    afxNat maxImageDimension3D;
    afxNat maxImageDimensionCube;
    afxNat maxImageArrayLayers;
    afxNat maxTexelBufferElements;
    afxNat maxUniformBufferRange;
    afxNat maxStorageBufferRange;
    afxNat maxPushConstantsSize;
    afxNat maxMemoryAllocationCount;
    afxNat maxSamplerAllocationCount;
    afxSize bufferImageGranularity;
    afxSize sparseAddressSpaceSize;
    afxNat maxBoundDescriptorSets;
    afxNat maxPerStageDescriptorSamplers;
    afxNat maxPerStageDescriptorUniformBuffers;
    afxNat maxPerStageDescriptorStorageBuffers;
    afxNat maxPerStageDescriptorSampledImages;
    afxNat maxPerStageDescriptorStorageImages;
    afxNat maxPerStageDescriptorInputAttachments;
    afxNat maxPerStageResources;
    afxNat maxDescriptorSetSamplers;
    afxNat maxDescriptorSetUniformBuffers;
    afxNat maxDescriptorSetUniformBuffersDynamic;
    afxNat maxDescriptorSetStorageBuffers;
    afxNat maxDescriptorSetStorageBuffersDynamic;
    afxNat maxDescriptorSetSampledImages;
    afxNat maxDescriptorSetStorageImages;
    afxNat maxDescriptorSetInputAttachments;
    afxNat maxVertexInputAttributes;
    afxNat maxVertexInputBindings;
    afxNat maxVertexInputAttributeOffset;
    afxNat maxVertexInputBindingStride;
    afxNat maxVertexOutputComponents;
    afxNat maxTessellationGenerationLevel;
    afxNat maxTessellationPatchSize;
    afxNat maxTessellationControlPerVertexInputComponents;
    afxNat maxTessellationControlPerVertexOutputComponents;
    afxNat maxTessellationControlPerPatchOutputComponents;
    afxNat maxTessellationControlTotalOutputComponents;
    afxNat maxTessellationEvaluationInputComponents;
    afxNat maxTessellationEvaluationOutputComponents;
    afxNat maxGeometryShaderInvocations;
    afxNat maxGeometryInputComponents;
    afxNat maxGeometryOutputComponents;
    afxNat maxGeometryOutputVertices;
    afxNat maxGeometryTotalOutputComponents;
    afxNat maxFragmentInputComponents;
    afxNat maxFragmentOutputAttachments;
    afxNat maxFragmentDualSrcAttachments;
    afxNat maxFragmentCombinedOutputResources;
    afxNat maxComputeSharedMemorySize;
    afxNat maxComputeWorkGroupCount[3];
    afxNat maxComputeWorkGroupInvocations;
    afxNat maxComputeWorkGroupSize[3];
    afxNat subPixelPrecisionBits;
    afxNat subTexelPrecisionBits;
    afxNat mipmapPrecisionBits;
    afxNat maxDrawIndexedIndexValue;
    afxNat maxDrawIndirectCount;
    afxReal maxSamplerLodBias;
    afxReal maxSamplerAnisotropy;
    afxNat maxViewports;
    afxNat maxViewportDimensions[2];
    afxReal viewportBoundsRange[2];
    afxNat viewportSubPixelBits;
    afxSize minMemoryMapAlignment;
    afxSize minTexelBufferOffsetAlignment;
    afxSize minUniformBufferOffsetAlignment;
    afxSize minStorageBufferOffsetAlignment;
    afxInt minTexelOffset;
    afxNat maxTexelOffset;
    afxInt minTexelGatherOffset;
    afxNat maxTexelGatherOffset;
    afxReal minInterpolationOffset;
    afxReal maxInterpolationOffset;
    afxNat subPixelInterpolationOffsetBits;
    afxNat maxFramebufferWidth;
    afxNat maxFramebufferHeight;
    afxNat maxFramebufferLayers;
    afxFlags framebufferColorSampleCounts;
    afxFlags framebufferDepthSampleCounts;
    afxFlags framebufferStencilSampleCounts;
    afxFlags framebufferNoAttachmentsSampleCounts;
    afxNat maxColorAttachments;
    afxFlags sampledImageColorSampleCounts;
    afxFlags sampledImageIntegerSampleCounts;
    afxFlags sampledImageDepthSampleCounts;
    afxFlags sampledImageStencilSampleCounts;
    afxFlags storageImageSampleCounts;
    afxNat maxSampleMaskWords;
    afxBool timestampComputeAndGraphics;
    afxReal timestampPeriod;
    afxNat maxClipDistances;
    afxNat maxCullDistances;
    afxNat maxCombinedClipAndCullDistances;
    afxNat discreteQueuePriorities;
    afxReal pointSizeRange[2];
    afxReal lineWidthRange[2];
    afxReal pointSizeGranularity;
    afxReal lineWidthGranularity;
    afxBool strictLines;
    afxBool standardSampleLocations;
    afxSize optimalBufferCopyOffsetAlignment;
    afxSize optimalBufferCopyRowPitchAlignment;
    afxSize nonCoherentAtomSize;
};

AFX_DEFINE_STRUCT(afxDrawDeviceCaps)
{
    afxBool robustBufferAccess;
    afxBool fullDrawIndexUint32;
    afxBool imageCubeArray;
    afxBool independentBlend;
    afxBool geometryShader;
    afxBool tessellationShader;
    afxBool sampleRateShading;
    afxBool dualSrcBlend;
    afxBool logicOp;
    afxBool multiDrawIndirect;
    afxBool drawIndirectFirstInstance;
    afxBool depthClamp;
    afxBool depthBiasClamp;
    afxBool fillModeNonSolid;
    afxBool depthBounds;
    afxBool wideLines;
    afxBool largePoints;
    afxBool alphaToOne;
    afxBool multiViewport;
    afxBool samplerAnisotropy;
    afxBool textureCompressionETC2;
    afxBool textureCompressionASTC_LDR;
    afxBool textureCompressionBC;
    afxBool occlusionQueryPrecise;
    afxBool pipelineStatisticsQuery;
    afxBool vertexPipelineStoresAndAtomics;
    afxBool fragmentStoresAndAtomics;
    afxBool shaderTessellationAndGeometryPointSize;
    afxBool shaderImageGatherExtended;
    afxBool shaderStorageImageExtendedFormats;
    afxBool shaderStorageImageMultisample;
    afxBool shaderStorageImageReadWithoutFormat;
    afxBool shaderStorageImageWriteWithoutFormat;
    afxBool shaderUniformBufferArrayDynamicIndexing;
    afxBool shaderSampledImageArrayDynamicIndexing;
    afxBool shaderStorageBufferArrayDynamicIndexing;
    afxBool shaderStorageImageArrayDynamicIndexing;
    afxBool shaderClipDistance;
    afxBool shaderCullDistance;
    afxBool shaderFloat64;
    afxBool shaderInt64;
    afxBool shaderInt16;
    afxBool shaderResourceResidency;
    afxBool shaderResourceMinLod;
    afxBool sparseBinding;
    afxBool sparseResidencyBuffer;
    afxBool sparseResidencyImage2D;
    afxBool sparseResidencyImage3D;
    afxBool sparseResidency2Samples;
    afxBool sparseResidency4Samples;
    afxBool sparseResidency8Samples;
    afxBool sparseResidency16Samples;
    afxBool sparseResidencyAliased;
    afxBool variableMultisampleRate;
    afxBool inheritedQueries;
};

AFX_DEFINE_STRUCT(afxDrawPortCaps)
{
    afxDrawBridgeFlags       queFlags;
    afxNat                  queCnt;
};

AFX_DEFINE_STRUCT(afxDrawDeviceInfo)
{
    afxDeviceInfo   dev;
};

typedef enum _ddevReqCode
{
    _ddevReqCode_0,
    _ddevReqCode_1,
    _ddevReqCode_DIN_DTOR,
    _ddevReqCode_DIN_CTOR,
    _ddevReqCode_DIN_RLNK,
    _ddevReqCode_DOUT_RLNK,
    _ddevReqCode_DOUT_DTOR,
    _ddevReqCode_DOUT_CTOR,
    _ddevReqCode_DCTX_DTOR,
    _ddevReqCode_DCTX_CTOR,
}
_ddevReqCode;

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_DEVICE_C

AFX_DEFINE_STRUCT(afxDrawInputEndpoint)
{
    afxLinkage                  ddev;
    afxChain                    instances;
    afxMutex                    mtx;
    afxCondition                cnd;
    afxString                   name;
};

AFX_OBJECT(afxDrawDevice)
{
    AFX_OBJECT(afxDevice)   dev;

    afxManager              contexts;
    afxManager              semaphores;
    afxManager              fences;
    afxManager              queries;

    afxChain                outputs;
    afxChain                inputs;

    afxChain                ineps;

    afxDrawDeviceCaps       caps;
    afxDrawDeviceLimits     limits;
    afxNat                  portCnt;
    struct
    {
        afxDrawPortCaps     portCaps;
        afxManager          ddgeMgr;
    }*                      ports;
    
    afxClipSpace            clipCfg;

    afxError                (*procCb)(afxDrawDevice,afxThread); // call their draw threads.

    struct _afxDdevIdd*     idd;
};
#endif//_AFX_DRAW_DEVICE_C
#endif//_AFX_DRAW_C

AFX_DEFINE_STRUCT(afxDrawSystemConfig)
{
    afxSize const*      attrs[2];
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_SYSTEM_C
AFX_OBJECT(afxDrawSystem)
{
    afxChain            mgrChn;
    afxManager          devices;
    afxManager          outputs;
    afxManager          inputs;
    afxManager          cameras;

    afxManager          oends;
    afxManager          iends;
};
#endif//_AFX_DRAW_SYSTEM_C
#endif//_AFX_DRAW_C

AVX void            AfxChooseDrawSystemConfiguration(afxDrawSystemConfig* cfg);

AVX afxBool         AfxGetDrawSystem(afxDrawSystem* dsys);

AVX afxManager*     AfxGetDrawDeviceClass(void);
AVX afxManager*     AfxGetDrawOutputClass(void);
AVX afxManager*     AfxGetDrawInputClass(void);
AVX afxManager*     AfxGetCameraClass(void);

AVX afxNat          AfxCountDrawDevices(void);
AVX afxNat          AfxCountCameras(void);

AVX afxNat          AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice devices[]);
AVX afxNat          AfxEnumerateCameras(afxNat first, afxNat cnt, afxCamera cameras[]);

AVX afxNat          AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void* udd);
AVX afxNat          AfxInvokeCameras(afxNat first, afxNat cnt, afxBool(*f)(afxCamera, void*), void* udd);

AVX afxNat          AfxChooseDrawDevice(afxDrawDeviceCaps const* caps, afxDrawDeviceLimits const* limits, afxNat maxCnt, afxDrawDevice ddevIdx[]); // return count of found devices

AVX afxReal64       AfxDetermineAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam);

// If you don't know what the physical aspect ratio is of the device you're using (for example, if you're using a standard PC, there is no way to determine for sure what kind of monitor is attached), 
// you can either assume square pixels (pass the width of the screen divided by the height), or you can use Qwadro's "best guess": 

AVX afxReal64       AfxDeterminePhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight);

////////////////////////////////////////////////////////////////////////////////
// DRAW DEVICE                                                                //
////////////////////////////////////////////////////////////////////////////////

AVX afxBool         AfxGetDrawDevice(afxNat ddevId, afxDrawDevice* ddev);

AVX afxBool         AfxDrawDeviceIsRunning(afxDrawDevice ddev);

AVX afxManager*     AfxGetDrawContextClass(afxDrawDevice ddev);
AVX afxManager*     AfxGetSemaphoreClass(afxDrawDevice ddev);
AVX afxManager*     AfxGetQueryPoolClass(afxDrawDevice ddev);
AVX afxManager*     AfxGetFenceClass(afxDrawDevice ddev);

AVX void            AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawDeviceLimits* limits);
AVX void            AfxGetDrawDeviceCaps(afxDrawDevice ddev, afxDrawDeviceCaps* caps);

AVX afxNat          AfxCountDrawPorts(afxDrawDevice ddev);
AVX void            AfxGetDrawPortCaps(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps);

AVX afxNat          AfxCountDrawContexts(afxDrawDevice ddev);
AVX afxNat          AfxCountDrawOutputs(afxDrawDevice ddev);
AVX afxNat          AfxCountDrawInputs(afxDrawDevice ddev);

AVX afxNat          AfxEnumerateDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawContext contexts[]);
AVX afxNat          AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput outputs[]);
AVX afxNat          AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput inputs[]);

AVX afxNat          AfxInvokeDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd);
AVX afxNat          AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AVX afxNat          AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);

AVX afxNat          AfxChooseDrawOutputEndpoint(afxNat ddevId, afxDrawOutputCaps const* caps, afxNat maxCnt, afxNat endpointIdx[]);
AVX afxNat          AfxChooseDrawInputEndpoint(afxNat ddevId, afxNat maxCnt, afxNat endpointIdx[]);

#endif//AFX_DRAW_SYSTEM_H
