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

// The Autonomous Draw System, responsible to manage resources and provide drawing capabilities.

#ifndef AFX_DRAW_SYSTEM_H
#define AFX_DRAW_SYSTEM_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawThread.h"

#include "afx/draw/afxColor.h"
#include "afx/draw/afxVertex1.h"

#include "afx/draw/pipelining/afxDrawOperation.h"
#include "afx/core/afxModule.h"
#include "afx/core/afxApplication.h"

AFX_DECLARE_STRUCT(afxTextureStorageRegistry);

AFX_DEFINE_STRUCT(afxTextureStorageSpecification)
{
    afxString const*    name;
    afxString const*    author;
    afxString const*    website;
    afxString const*    note;
    afxNat              verMajor;
    afxNat              verMinor;
    afxNat              verPatch;
    afxNat              extCnt;
    afxString const*    exts;

    afxNat              minScale;
    afxNat              maxScale;
    afxNat              minDepth;
    afxNat              maxDepth;
    afxNat              minBpp;
    afxNat              maxBpp;
    afxBool             hasAlpha;

    afxError            (*acq)(afxDrawContext dctx, afxNat cnt, afxTexture tex[], afxUri const uri[]);
    // coacquire for batch layered textures such as DDS?
    afxError            (*upload)(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
    afxError            (*download)(afxTexture tex, afxNat lodIdx, afxNat baseImg, afxNat imgCnt, afxUri const *uri);
    afxError            (*uploadRgn)(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
    afxError            (*downloadRgn)(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
};

typedef enum afxDrawDeviceType
{
    AFX_DDEV_TYPE_CPU, // CPU with no graphic support --- ex.: Ryzen 7
    AFX_DDEV_TYPE_IGPU, // integrated GPU --- ex.: Intel HD Graphics
    AFX_DDEV_TYPE_DGPU, // dedicated GPU --- ex.: AMD/NVidia PCIE adapter
    AFX_DDEV_TYPE_VGPU, // virtual GPU --- ex.: VM/sandboxed instantiation.
} afxDrawDeviceType;

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

AFX_DEFINE_STRUCT(afxSurfaceCaps)
{
    afxNat              minBufCnt;
    afxNat              maxBufCnt;
    afxNat              currentExtent[2];
    afxNat              minExtent[2];
    afxNat              maxExtent[2];
    afxNat              maxLayerCnt;
    afxPresentTransform supportedTransforms;
    afxPresentTransform currentTransform;
    afxPresentAlpha     supportedCompositeAlpha;
    afxTextureFlags     supportedUsageFlags;
};

AFX_DEFINE_STRUCT(afxDrawPortCaps)
{
    afxDrawQueueFlags       queFlags;
    afxNat                  queCnt;
};

AFX_DECLARE_STRUCT(_afxDdevVmt);

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

#ifdef _AFX_DRAW_DEVICE_C
AFX_OBJECT(afxDrawDevice)
#else
struct afxBaseDrawDevice
#endif
{
    _afxDdevVmt const*      vmt;
    afxChain                classes;
    afxClass                contexts;
    afxClass                outputs;
    afxClass                inputs;

    afxDrawDeviceCaps       caps;
    afxDrawDeviceLimits     limits;
    afxNat                  portCnt;
    afxDrawPortCaps*        portCaps;
    afxBool                 serving;

    afxString               domain;
    afxString               name;
};

AFX_DEFINE_STRUCT(afxDrawIcdInfo)
{
    afxModule               mdle;
    afxString const*        name; // driver name: SIGMA GL
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

AFX_DEFINE_STRUCT(afxDrawSystemConfig)
{
    afxContext          ctx;
    afxSize const*      attrs[2];
};

#ifdef _AFX_DRAW_SYSTEM_C
AFX_OBJECT(afxDrawSystem)
{
    afxContext          ctx;
    afxArena            aren;
    afxChain            classes;
    afxClass            icds;
    afxClass            devices;
    afxClass            contexts;
    afxClass            outputs;
    afxClass            inputs;
    afxClass            threads;
    afxChain            texIoCodecs;
    afxDrawIcd          e2draw; // SIGMA GL is required for minimal operability since core has no more embedded fallback.
};
#endif

AFX void                AfxChooseDrawSystemConfiguration(afxDrawSystemConfig *config, afxNat extendedSiz);
AFX afxError            AfxAcquireDrawSystems(afxNat cnt, afxDrawSystem dsys[], afxDrawSystemConfig const config[]);

AFX afxContext          AfxGetDrawSystemContext(afxDrawSystem dsys);

AFX afxBool             AfxGetDrawDevice(afxDrawSystem dsys, afxNat devIdx, afxDrawDevice *ddev);
AFX afxError            AfxRegisterDrawDevices(afxDrawSystem dsys, afxNat cnt, afxDrawDevice ddev[], afxDrawDeviceInfo const *spec);
AFX afxError            AfxAcquireDrawThreads(afxDrawSystem dsys, afxNat cnt, afxDrawThread dthr[], afxDrawThreadConfig const config[]);

AFX afxNat              AfxEnumerateDrawThreads(afxDrawSystem dsys, afxNat first, afxNat cnt, afxDrawThread dthr[]);
AFX afxNat              AfxEnumerateDrawDevices(afxDrawSystem dsys, afxNat first, afxNat cnt, afxDrawDevice ddev[]);

AFX afxNat              AfxCurateDrawThreads(afxDrawSystem dsys, afxNat first, afxNat cnt, afxBool(*f)(afxDrawThread, void*), void *udd);
AFX afxNat              AfxCurateDrawDevices(afxDrawSystem dsys, afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd);

AFX afxNat              AfxCountDrawThreads(afxDrawSystem dsys);
AFX afxNat              AfxCountDrawDevices(afxDrawSystem dsys);

AFX afxClass*           AfxGetDrawThreadClass(afxDrawSystem dsys);
AFX afxClass*           AfxGetDrawDeviceClass(afxDrawSystem dsys);

AFX afxNat              AfxChooseTextureStorage(afxDrawSystem dsys, afxTexture tex, afxTextureRegion const *rgn);
AFX afxResult           AfxDescribeTextureStorages(afxDrawSystem dsys, afxNat first, afxNat cnt, afxTextureStorageSpecification spec[]);
AFX afxResult           AfxEnumerateTextureStorages(afxDrawSystem dsys, afxNat first, afxNat cnt, afxTextureStorageRegistry *storage[]);
AFX afxResult           AfxRegisterTextureStorages(afxDrawSystem dsys, afxNat cnt, afxTextureStorageSpecification const spec[]);

//AFX afxNat              AfxPostDrawEvent(afxUniqueID eventId, );

AFX void                AfxGetDrawDeviceCaps(afxDrawDevice ddev, afxDrawDeviceCaps* caps);
AFX void                AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawDeviceLimits* limits);
AFX void                AfxGetDrawPortCaps(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps);
AFX afxNat              AfxCountDrawPorts(afxDrawDevice ddev);

AFX afxBool             AfxDrawDeviceIsRunning(afxDrawDevice ddev);
AFX afxClass*           AfxGetDrawInputClass(afxDrawDevice ddev);
AFX afxNat              AfxCountDrawInputs(afxDrawDevice ddev);

AFX afxNat              AfxEnumerateDrawInputs(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxDrawInput din[]);
AFX afxNat              AfxCurateDrawInputs(afxDrawSystem dsys, afxNat devId, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);

#endif//AFX_DRAW_SYSTEM_H
