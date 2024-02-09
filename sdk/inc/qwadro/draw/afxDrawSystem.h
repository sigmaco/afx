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

// The Unified Qwadro Video Graphics Infrastructure
// This section is part of SIGMA GL/2.

#ifndef AFX_DRAW_SYSTEM_H
#define AFX_DRAW_SYSTEM_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/afxColor.h"
#include "qwadro/draw/afxPixel.h"
// provided classes.
#include "qwadro/draw/afxDrawQueue.h"
#include "qwadro/draw/pipe/afxDrawScript.h"
#include "qwadro/draw/pipe/afxDrawCommands.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/afxDrawOutput.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawThread.h"

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

AFX_DEFINE_STRUCT(afxDrawOutputEndpointCaps)
{
    afxNat              minBufCnt;
    afxNat              maxBufCnt;
    afxNat              currExtent[2];
    afxWhd              minWhd;
    afxWhd              maxWhd;
    afxNat              maxLayerCnt;
    afxPresentTransform supportedTransforms;
    afxPresentTransform currTransform;
    afxPresentAlpha     supportedCompositeAlpha;
    afxRasterFlags      supportedUsageFlags;
    afxPresentScaling   supportedScaling;
    afxNat              supportedModeCnt;
    afxPresentMode      supportedModes;
};

AFX_DEFINE_STRUCT(afxDrawPortCaps)
{
    afxDrawQueueFlags       queFlags;
    afxNat                  queCnt;
};

AFX_DEFINE_STRUCT(afxDrawDeviceInfo)
{
    afxString const*        domain;
    afxString const*        name;
    afxDrawDeviceCaps const*caps;
    afxDrawDeviceLimits const*limits;
    afxNat                  portCnt;
    afxDrawPortCaps const*  portCaps;
    afxClassConfig const*   dctxClsConfig;
    afxClassConfig const*   doutClsConfig;
    afxClassConfig const*   dinClsConfig;
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_DEVICE_C

AFX_DEFINE_STRUCT(afxDrawInputEndpoint)
{
    afxLinkage              ddev;
    afxChain                mechanisms;
};

AFX_DEFINE_STRUCT(afxDrawOutputEndpoint)
{
    afxLinkage                  ddev;
    afxChain                    mechanisms;
    afxDrawOutputEndpointCaps   caps;
};

#ifndef _AFX_DRAW_DEVICE_IMPL
AFX_OBJECT(afxDrawDevice)
#else
struct _afxBaseDrawDevice
#endif
{
    AFX_OBJECT(afxDevice)   dev;
    afxChain                classes;
    afxClass                contexts;
    afxClass                outputs;
    afxClass                inputs;
    
    afxDrawDeviceCaps       caps;
    afxDrawDeviceLimits     limits;
    afxNat                  portCnt;
    struct
    {
        afxDrawPortCaps     portCaps;
        afxClass            queues;
        afxClass            scripts;
    }*                      ports;
    afxBool                 serving;

    afxString               domain;
    afxString               name;

    afxClipSpace            clipCfg;

    afxError                (*procCb)(afxDrawDevice,afxDrawThread); // call their draw threads.
    afxError                (*relinkDin)(afxDrawDevice,afxDrawContext,afxNat,afxDrawInput[]);
    afxError                (*relinkDout)(afxDrawDevice,afxDrawContext,afxNat,afxDrawOutput[]);
    afxMutex                ioConMtx;
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
    afxFcc              fcc;
    afxMmu              mmu;
    afxArena            aren;
    afxChain            classes;
    afxClass            dthreads;
    afxClass            dcontexts;
    afxClass            oends;
    afxClass            doutputs;
    afxClass            iends;
    afxClass            dinputs;
    afxClass            ddevices;
    afxIcd              e2draw; // SIGMA GL/2 is required for minimal operability since core has no more embedded fallback.
};
#endif//_AFX_DRAW_SYSTEM_C
#endif//_AFX_DRAW_C

AFX void            AfxChooseDrawSystemConfiguration(afxDrawSystemConfig* cfg);

AFX afxMmu          AfxGetDrawSystemMmu(void);

AFX afxClass*       AfxGetDrawThreadClass(void);
AFX afxClass*       AfxGetDrawDeviceClass(void);

AFX afxNat          AfxCountDrawThreads(void);
AFX afxNat          AfxCountDrawDevices(void);

AFX afxNat          AfxEnumerateDrawThreads(afxNat first, afxNat cnt, afxDrawThread dthr[]);
AFX afxNat          AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice ddev[]);

AFX afxNat          AfxInvokeDrawThreads(afxNat first, afxNat cnt, afxBool(*f)(afxDrawThread, void*), void *udd);
AFX afxNat          AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd);

AFX afxDrawDevice   AfxGetDrawDevice(afxNat ddevIdx);

AFX afxNat          AfxChooseDrawDevice(afxDrawDeviceCaps const* caps, afxDrawDeviceLimits const* limits, afxNat maxCnt, afxDrawDevice ddevIdx[]); // return count of found devices

////////////////////////////////////////////////////////////////////////////////
// DRAW DEVICE                                                                //
////////////////////////////////////////////////////////////////////////////////

AFX afxBool         AfxDrawDeviceIsRunning(afxDrawDevice ddev);

AFX void            AfxGetDrawDeviceCaps(afxDrawDevice ddev, afxDrawDeviceCaps* caps);
AFX void            AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawDeviceLimits* limits);

AFX afxNat          AfxCountDrawPorts(afxDrawDevice ddev);
AFX void            AfxGetDrawPortCaps(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps);

AFX afxClass*       AfxGetDrawContextClass(afxDrawDevice ddev);
AFX afxClass*       AfxGetDrawOutputClass(afxDrawDevice ddev);
AFX afxClass*       AfxGetDrawInputClass(afxDrawDevice ddev);

AFX afxNat          AfxCountDrawContexts(afxDrawDevice ddev);
AFX afxNat          AfxCountDrawOutputs(afxDrawDevice ddev);
AFX afxNat          AfxCountDrawInputs(afxDrawDevice ddev);

AFX afxNat          AfxEnumerateDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawContext dctx[]);
AFX afxNat          AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput dout[]);
AFX afxNat          AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput din[]);

AFX afxNat          AfxInvokeDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd);
AFX afxNat          AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AFX afxNat          AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);

AFX afxNat          AfxChooseDrawOutputEndpoint(afxNat ddevId, afxDrawOutputEndpointCaps const* caps, afxNat maxCnt, afxNat endpointIdx[]);
AFX afxNat          AfxChooseDrawInputEndpoint(afxNat ddevId, afxNat maxCnt, afxNat endpointIdx[]);

AFX afxError        AfxAcquireDrawContexts(afxNat ddevId, afxNat cnt, afxDrawContextConfig const cfg[], afxDrawContext dctx[]);
AFX afxError        AfxOpenDrawOutputs(afxNat ddevId, afxNat cnt, afxDrawOutputConfig const cfg[], afxDrawOutput dout[]);
AFX afxError        AfxOpenDrawInputs(afxNat ddevId, afxNat cnt, afxDrawInputConfig const cfg[], afxDrawInput din[]);

#endif//AFX_DRAW_SYSTEM_H
