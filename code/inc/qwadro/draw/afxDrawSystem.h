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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// The Unified Qwadro Video Graphics Infrastructure
// This section is part of SIGMA GL/2.

#ifndef AFX_DRAW_SYSTEM_H
#define AFX_DRAW_SYSTEM_H

#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/afxDrawOutput.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawThread.h"
#include "qwadro/draw/afxCamera.h"

#include "qwadro/draw/afxColor.h"
#include "qwadro/draw/afxPixel.h"

#include "qwadro/core/afxModule.h"
#include "qwadro/core/afxApplication.h"

typedef enum afxProcessorType
{
    afxProcessorType_CPU, // CPU with no graphic support --- ex.: Ryzen 7
    afxProcessorType_GPU, // dedicated GPU --- ex.: AMD/NVidia PCIE adapter
    afxProcessorType_APU, // combined CPU-GPU --- ex.: AMD A-series
    afxProcessorType_IGPU, // integrated GPU --- ex.: Intel HD Graphics
    afxProcessorType_VGPU, // virtual GPU --- ex.: VM/sandboxed instantiation.
    afxProcessorType_PPU, // physics processing unit --- ex.: Ageia PhysX card
    afxProcessorType_SPU, // synergistic processing unit
    afxProcessorType_DSP,
    afxProcessorType_TOTAL
} afxProcessorType;

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
    afxRasterFlags     supportedUsageFlags;
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
AFX_OBJECT(afxDrawDevice)
#else
struct afxBaseDrawDevice
#endif
{
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

    afxError                (*procCb)(afxDrawDevice,afxDrawThread); // call their draw threads.
    afxError                (*relinkDin)(afxDrawDevice,afxDrawInput,afxDrawContext);
    afxError                (*relinkDout)(afxDrawDevice,afxDrawOutput,afxDrawContext);
};
#endif

AFX_DEFINE_STRUCT(afxDrawIcdInfo)
{
    afxModule               mdle;
    afxString const*        name; // driver name: SIGMA GL/2
    afxString const*        vendor; // author: SIGMA Technology Group (The technology arm of Federação SIGMA)
    afxString const*        website; // website: sigmaco.org
    afxString const*        note; // The standard QWADRO draw system implementation.
    afxNat                  verMajor; // 1
    afxNat                  verMinor; // 0
    afxNat                  verPatch; // 0
    afxClassConfig const*   devExt;
    afxNat                  devCnt;
};

AFX_DEFINE_HANDLE(afxDrawIcd);

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_DRIVER_C
AFX_OBJECT(afxDrawIcd)
{
    afxChain            classes;
    afxClass            devices;

    afxModule           mdle;
    afxString           name;
    afxString           vendor;
    afxString           website;
    afxString           note;
    afxNat              verMajor; // 1
    afxNat              verMinor; // 0
    afxNat              verPatch; // 0
};
#endif
#endif

AFX_DEFINE_STRUCT(afxDrawSystemConfig)
{
    afxFcc              fcc;
    afxMmu              mmu;
    afxSize const*      attrs[2];
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_SYSTEM_C
AFX_OBJECT(afxDrawSystem)
{
    afxFcc              fcc;
    afxMmu              mmu;
    afxArena            aren;
    afxClass            icds;
    afxClass            devices;
    afxClass            contexts;
    afxClass            outputs;
    afxClass            inputs;
    afxClass            threads;
    afxClass            cameras;
    afxDrawIcd          e2draw; // SIGMA GL/2 is required for minimal operability since core has no more embedded fallback.

    struct
    // the scale and units used to calculate depth values when load files missing any of this config.
    {
        afxReal factor;
        // Specifies a scale factor that is used to create a variable depth offset for each polygon.The initial value is 0.

        afxReal units;
        // Is multiplied by an implementation - specific value to create a constant depth offset.The initial value is 0.
    } polygonOffset;
};
#endif
#endif

AFX void            AfxChooseDrawSystemConfiguration(afxDrawSystemConfig *config, afxNat extendedSiz);

AFX afxMmu          AfxGetDrawSystemMmu(void);

AFX afxClass*       AfxGetDrawThreadClass(void);
AFX afxClass*       AfxGetDrawDeviceClass(void);
AFX afxClass*       AfxGetCameraClass(void);

AFX afxNat          AfxCountDrawThreads(void);
AFX afxNat          AfxCountDrawDevices(void);

AFX afxNat          AfxEnumerateDrawThreads(afxNat first, afxNat cnt, afxDrawThread dthr[]);
AFX afxNat          AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice ddev[]);
AFX afxNat          AfxEnumerateDrawInputs(afxNat ddevId, afxNat first, afxNat cnt, afxDrawInput din[]);
AFX afxNat          AfxEnumerateDrawOutputs(afxNat ddevId, afxNat first, afxNat cnt, afxDrawOutput dout[]);
AFX afxNat          AfxEnumerateDrawContexts(afxNat ddevId, afxNat first, afxNat cnt, afxDrawContext dctx[]);
AFX afxNat          AfxEnumerateCameras(afxNat first, afxNat cnt, afxCamera cam[]);

AFX afxNat          AfxCurateDrawThreads(afxNat first, afxNat cnt, afxBool(*f)(afxDrawThread, void*), void *udd);
AFX afxNat          AfxCurateDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd);
AFX afxNat          AfxCurateDrawInputs(afxNat ddevId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AFX afxNat          AfxCurateDrawOutputs(afxNat ddevId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AFX afxNat          AfxCurateDrawContexts(afxNat ddevId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd);

AFX afxBool         AfxGetDrawDevice(afxNat ddevIdx, afxDrawDevice *ddev);
AFX afxNat          AfxChooseDrawDevice(afxDrawDeviceCaps const* caps, afxDrawDeviceLimits const* limits, afxNat maxCnt, afxDrawDevice ddevIdx[]); // return count of found devices

////////////////////////////////////////////////////////////////////////////////
// DRAW DEVICE                                                                //
////////////////////////////////////////////////////////////////////////////////

AFX afxBool         AfxDrawDeviceIsRunning(afxDrawDevice ddev);

AFX void            AfxGetDrawDeviceCaps(afxDrawDevice ddev, afxDrawDeviceCaps* caps);
AFX void            AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawDeviceLimits* limits);

AFX afxNat          AfxCountDrawPorts(afxDrawDevice ddev);
AFX void            AfxGetDrawPortCaps(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps);

AFX afxClass*       AfxGetDrawInputClass(afxDrawDevice ddev);
AFX afxClass*       AfxGetDrawOutputClass(afxDrawDevice ddev);
AFX afxClass*       AfxGetDrawContextClass(afxDrawDevice ddev);

AFX afxNat          AfxCountDrawInputs(afxDrawDevice ddev);
AFX afxNat          AfxCountDrawOutputs(afxDrawDevice ddev);
AFX afxNat          AfxCountDrawContexts(afxDrawDevice ddev);

////////////////////////////////////////////////////////////////////////////////
// DRAW DEVICE                                                                //
////////////////////////////////////////////////////////////////////////////////

AFX afxError        AfxAcquireDrawContexts(afxNat ddevId, afxNat cnt, afxDrawContextConfig const config[], afxDrawContext dctx[]);

////////////////////////////////////////////////////////////////////////////////
// DRAW OUTPUT                                                                //
////////////////////////////////////////////////////////////////////////////////

AFX afxNat          AfxChooseDrawOutputEndpoint(afxNat ddevId, afxDrawOutputEndpointCaps const* caps, afxNat maxCnt, afxNat endpointIdx[]);

AFX afxError        AfxOpenDrawOutputs(afxNat ddevId, afxNat cnt, afxDrawOutputConfig const config[], afxDrawOutput dout[]);

////////////////////////////////////////////////////////////////////////////////
// DRAW INPUT                                                                 //
////////////////////////////////////////////////////////////////////////////////

AFX afxNat          AfxChooseDrawInputEndpoint(afxNat ddevId, afxNat maxCnt, afxNat endpointIdx[]);

AFX afxError        AfxOpenDrawInputs(afxNat ddevId, afxNat cnt, afxDrawInputConfig const config[], afxDrawInput din[]);

#endif//AFX_DRAW_SYSTEM_H
