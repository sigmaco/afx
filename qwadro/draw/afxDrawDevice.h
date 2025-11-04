/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Video Graphics Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Qwadro Unified Video Graphics Infrastructure                             //
//////////////////////////////////////////////////////////////////////////////

/*
    A draw device refers to a hardware or software component used to render or display graphical content. 
    It is responsible for processing and presenting visual data, whether it's through direct output to a screen or through intermediate rendering processes.

    In graphics hardware, a draw device refers to a physical component that handles rendering tasks. It processes graphical instructions and outputs them to a display.
    In software, a draw device can refer to a virtual or logical component used to handle drawing operations. It is often part of a graphics library or API.
    In VR and AR systems, draw devices are components that render virtual environments and overlay information onto the real world.

*/

// In Qwadro Video Graphics Infrastructure, a draw device installs and operates a whole draw I/O system. This is why the draw device is who provides draw I/O mechanisms.

#ifndef AVX_DRAW_DEVICE_H
#define AVX_DRAW_DEVICE_H

#include "qwadro/exec/afxDevice.h"
#include "qwadro/draw/afxDrawBridge.h"
#include "qwadro/draw/afxSurface.h"
#include "qwadro/hid/afxDisplay.h"

AFX_DEFINE_STRUCT(afxDrawFeatures)
/**
    Draw features are various capabilities or functionalities that are provided by draw devices to facilitate the drawing and rendering 
    of objects. These features are essential for creating and controlling the visual output in graphical applications, 
    such as games, simulations, or visualizations.
*/
{
    afxBool8 robustBufAccess;
    afxBool8 fullDrawIdxUint32;
    afxBool8 rasterCubeArray;
    afxBool8 independentBlend;
    afxBool8 primShader;
    afxBool8 tessShader;
    afxBool8 sampleRateShading;
    afxBool8 dualSrcBlend;
    afxBool8 logicOp;
    afxBool8 multiDrawIndirect;
    afxBool8 drawIndirectFirstInst;
    afxBool8 depthClamp;
    afxBool8 depthBiasClamp;
    afxBool8 fillModeNonSolid;
    afxBool8 depthBounds;
    afxBool8 wideLines;
    afxBool8 largePoints;
    afxBool8 alphaToOne;
    afxBool8 multiViewport;
    afxBool8 samplerAnisotropy;
    afxBool8 etc2; // texture compression.
    afxBool8 astc_LDR; // texture compression.
    afxBool8 dxt; // texture compression.
    afxBool8 occlusionQueryPrecise;
    afxBool8 pipelineStatsQuery;
    afxBool8 vtxPipelineStoresAndAtomics;
    afxBool8 fragStoresAndAtomics;
    afxBool8 shaderTessAndPrimPointSiz;
    afxBool8 shaderRasterGatherExt;
    afxBool8 shaderStorageRasterExtFmts;
    afxBool8 shaderStorageRasterMultisample;
    afxBool8 shaderStorageRasterReadWithoutFmt;
    afxBool8 shaderStorageRasterWriteWithoutFmt;
    afxBool8 shaderUboArrayDynIndexing;
    afxBool8 shaderSampledRasterArrayDynIndexing;
    afxBool8 shaderSsboArrayDynIndexing;
    afxBool8 shaderStorageImageArrayDynIndexing;
    afxBool8 shaderClipDist;
    afxBool8 shaderCullDist;
    afxBool8 shaderFloat64;
    afxBool8 shaderInt64;
    afxBool8 shaderInt16;
    afxBool8 shaderRsrcResidency;
    afxBool8 shaderRsrcMinLod;
    afxBool8 sparseBinding;
    afxBool8 sparseResidencyBuffer;
    afxBool8 sparseResidencyRaster2D;
    afxBool8 sparseResidencyRaster3D;
    afxBool8 sparseResidency2Samples;
    afxBool8 sparseResidency4Samples;
    afxBool8 sparseResidency8Samples;
    afxBool8 sparseResidency16Samples;
    afxBool8 sparseResidencyAliased;
    afxBool8 variableMultisampleRate;
    afxBool8 inheritedQueries;

    // compatible with PhysicalDeviceVulkan11Features
    afxBool8 ssbo16bitAccess;
    afxBool8 uboAndSsbo16bitAccess;
    afxBool8 storagePushConst16;
    afxBool8 storageInputOut16;
    afxBool8 multiview;
    afxBool8 multiviewPrimShader;
    afxBool8 multiviewTessShader;
    afxBool8 varPtrsStorageBuf;
    afxBool8 varPtrs;
    afxBool8 protectedMem;
    afxBool8 samplerYcbcrConv;
    afxBool8 shaderDrawParams;

    // compatible with PhysicalDeviceVulkan12Features
    afxBool8 samplerMirrorClampToEdge;
    afxBool8 drawIndirectCnt;
    afxBool8 ssbo8BitAccess;
    afxBool8 uboAndSsbo8bitAccess;
    afxBool8 storagePushConst8;
    afxBool8 shaderBufInt64Atomics;
    afxBool8 shaderSharedInt64Atomics;
    afxBool8 shaderFloat16;
    afxBool8 shaderInt8;
    afxBool8 descIndexing;

    // compatible with PhysicalDeviceDescriptorIndexingFeatures
    afxBool8 shaderInputAttachArrayDynamicIndexing;
    afxBool8 shaderUniformTexelBufArrayDynamicIndexing;
    afxBool8 shaderStorageTexelBufArrayDynamicIndexing;
    afxBool8 shaderUboArrayNonUniformIndexing;
    afxBool8 shaderSampledRasArrayNonUniformIndexing;
    afxBool8 shaderSsboArrayNonUniformIndexing;
    afxBool8 shaderStorageRasArrayNonUniformIndexing;
    afxBool8 shaderInputAttachmentArrayNonUniformIndexing;
    afxBool8 shaderUniformTexelBufArrayNonUniformIndexing;
    afxBool8 shaderStorageTexelBufArrayNonUniformIndexing;
    afxBool8 descBindingUboUpdAfterBind;
    afxBool8 descBindingSampledRasUpdAfterBind;
    afxBool8 descBindingStorageRasUpdAfterBind;
    afxBool8 descBindingSsboUpdAfterBind;
    afxBool8 descBindingUniformTexelBufUpdAfterBind;
    afxBool8 descBindingStorageTexelBufUpdAfterBind;
    afxBool8 descBindingUpdUnusedWhilePending;
    afxBool8 descBindingPartiallyBound;
    afxBool8 descBindingVarDescCnt;
    afxBool8 runtimeDescArray;

    afxBool8 samplerFilterMinMax;
    afxBool8 scalarBlockLayout;
    afxBool8 voidCanvas;
    afxBool8 uniformBufStdLayout;
    afxBool8 shaderSubgroupExtTypes;
    afxBool8 separateDepthStencilLayouts;
    afxBool8 hostQueryReset;
    afxBool8 timelineSemaphore;
    afxBool8 bufDevAddress;
    afxBool8 bufDevAddressCaptureReplay;
    afxBool8 bufDevAddressMultidev;
    afxBool8 vkMemModel;
    afxBool8 vkMemModelDevScope;
    afxBool8 vkMemModelAvailVisibChains;
    afxBool8 shaderOutViewportIdx;
    afxBool8 shaderOutLayer;
    afxBool8 subgroupBroadcastDynId;

    afxFcc*  extra;
};

AFX_DEFINE_STRUCT(afxDrawLimits)
/**
    Draw limits are various constraints or maximum values that can impact how objects are drawn or rendered. 
    These limits are typically defined by the graphics hardware, drivers, or the graphics API itself. 
    Understanding these limits is important for optimizing performance and ensuring compatibility across different hardware and platforms.
*/
{
    afxUnit     maxMemAllocCnt;
    afxSize     sparseAddrSpaceSiz;
    afxSize     nonCoherentAtomSiz;
    afxUnit     discreteQueuePriorities;
    afxBool     timestampComputeAndGraphics;
    afxReal     timestampPeriod;

    // Buffered memory
    afxSize     minBufMapAlign;
    afxSize     minTboOffsetAlign;
    afxSize     minUboOffsetAlign;
    afxSize     minSsboOffsetAlign;
    afxSize     optimalBufCopyOffsetAlign;
    afxSize     optimalBufCopyRowPitchAlign;

    afxUnit     maxTboCap; // in texels
    afxSize     bufferRasterGranularity;
    afxUnit     maxUboRange;
    afxUnit     maxSsboRange;

    // Rasters
    afxUnit     maxRasterDim1D;
    afxUnit     maxRasterDim2D;
    afxUnit     maxRasterDim3D;
    afxUnit     maxRasterDimCube;
    afxUnit     maxRasterLayers;

    afxFlags    sampledRasterColorSampleCnts;
    afxFlags    sampledRasterIntegerSampleCnts;
    afxFlags    sampledRasterDepthSampleCnts;
    afxFlags    sampledRasterStencilSampleCnts;
    afxFlags    storageRasterSampleCnts;

    // Sampler
    afxUnit     maxSamplerAllocCnt;
    afxReal     maxSamplerLodBias;
    afxReal     maxSamplerAnisotropy;

    // Pipelines
    afxUnit     maxPerStageSamplers;
    afxUnit     maxPerStageUbos;
    afxUnit     maxPerStageSsbos;
    afxUnit     maxPerStageSampledImages;
    afxUnit     maxPerStageStorageImages;
    afxUnit     maxPerStageInputAttachments;
    afxUnit     maxPerStageResources;

    // Ligatures
    afxUnit     maxPushConstsSiz;
    afxUnit     maxBoundPerLigas;
    afxUnit     maxPerLigaSamplers;
    afxUnit     maxPerLigaUbos;
    afxUnit     maxPerLigaUbosDynamic;
    afxUnit     maxPerLigaSsbos;
    afxUnit     maxPerLigaSsbosDynamic;
    afxUnit     maxPerLigaSampledImages;
    afxUnit     maxPerLigaStorageImages;
    afxUnit     maxPerLigaInputAttachments;

    // Compute shader
    afxUnit     maxComputeSharedMemSiz;
    afxWarp     maxComputeWarpCnt;
    afxUnit     maxComputeWarpInvocations;
    afxWarp     maxComputeWarpSiz;

    // Vertex input
    afxUnit     maxVtxIns;
    afxUnit     maxVtxInSrcs;
    afxUnit     maxVtxInOffset;
    afxUnit     maxVtxInSrcStride;
    afxUnit     maxVtxOutCompos;

    // Tesselation
    afxUnit     maxTessGenLvl;
    afxUnit     maxTessPatchSiz;
    afxUnit     maxTessCtrlPerVtxInComps;
    afxUnit     maxTessCtrlPerVtxOutComps;
    afxUnit     maxTessCtrlPerPatchOutComps;
    afxUnit     maxTessCtrlTotalOutComps;
    afxUnit     maxTessEvalInComps;
    afxUnit     maxTessEvalOutComps;

    // Primitive shader
    afxUnit     maxPrimShadInvocations;
    afxUnit     maxPrimInComps;
    afxUnit     maxPrimOutComps;
    afxUnit     maxPrimOutVertices;
    afxUnit     maxPrimTotalOutComps;

    // Clipping
    afxUnit     maxClipDistances;
    afxUnit     maxCullDistances;
    afxUnit     maxCombinedClipAndCullDistances;

    // Viewport
    afxUnit     maxVpCnt;
    afxUnit     maxVpDimensions[2];
    afxV2d      vpBoundsRange;
    afxUnit     vpSubPixelBits;

    // Rasterization
    afxUnit     subPixelPreciBits;
    afxUnit     subTexelPreciBits;
    afxUnit     mipmapPrecisionBits;
    afxUnit     maxDrawIndexedIdxValue;
    afxUnit     maxDrawIndirectCnt;

    afxV2d      pointSizRange;
    afxV2d      lineWidthRange;
    afxReal     pointSizGranularity;
    afxReal     lineWidthGranularity;
    afxBool     strictLines;

    afxBool     standardSampleLocations;
    afxUnit     maxSampleMaskWords;

    // Fragment shader
    afxUnit     maxFragInComps;
    afxUnit     maxFragOutAttachments;
    afxUnit     maxFragDualSrcAttachments;
    afxUnit     maxFragCombinedOutputResources;

    // Fragment operations
    afxInt      minTexelOffset;
    afxUnit     maxTexelOffset;
    afxInt      minTexelGatherOffset;
    afxUnit     maxTexelGatherOffset;
    afxReal     minInterpolationOffset;
    afxReal     maxInterpolationOffset;
    afxUnit     subPixelInterpolationOffsetBits;

    // Canvas
    afxWarp     maxCanvasWhd;
    afxFlags    canvasColorSampleCnts;
    afxFlags    canvasDepthSampleCnts;
    afxFlags    canvasStencilSampleCnts;
    afxFlags    canvasNoAttachmentsSampleCnts;
    afxUnit     maxColorAttachments;
};

// IMPLEMENTATION DISCOVERY ////////////////////////////////////////////////////

/*
    The AvxChooseDrawDevices() function provides a way to select drawing devices that match specified feature and limit requirements. 
    It returns the number of selected devices and populates an array with their device IDs. This function is helpful in scenarios 
    where an application needs to filter and choose devices based on certain hardware capabilities or constraints, such as selecting 
    GPUs that support specific rendering features or fall within particular performance limits.

    Returns the count of found devices.
*/

AVX afxUnit AvxChooseDrawDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd, 

    // A structure that specifies the features that the drawing devices must support.
    afxDrawFeatures const* features, 

    // A structure that defines the limits that the drawing devices should meet.
    afxDrawLimits const* limits, 

    // The operations or features that the device must supports.
    afxDrawPortInfo const* caps,

    // The maximum number of devices to be selected and returned in the @ddevIds array.
    afxUnit maxCnt, 

    // An array that will be populated with the device IDs of the selected drawing devices.
    afxUnit ddevIds[]
);

/**
    The AvxEnumerateDrawDevices() function enumerates the available drawing devices associated with an ICD. 
    It allows you to discover which devices are available for rendering tasks on a system, which is important in 
    scenarios where multiple devices (e.g., integrated and discrete GPUs) exist. The function provides the number 
    of devices found and populates an array with the relevant devices for further processing. This is useful for 
    applications that need to choose a specific device for rendering or computational tasks.

    Returns the number of draw devices inserted in the @devices.
*/

AVX afxUnit AvxEnumerateDrawDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd, 

    // The number ordinal for the first draw device to start the iteration from.
    afxUnit first, 
    
    // The maximum number of draw devices to be retrieved.
    afxUnit cnt, 
    
    // An array that will be populated with the drawing devices found during the enumeration.
    afxDrawDevice devices[]
);

/*
    The AvxInvokeDrawDevices() function provides an iterative mechanism to enumerate available drawing devices (e.g., GPUs) 
    and invoke a callback function for each device. This approach is useful when you want to perform operations or checks 
    on multiple devices without having to manually loop through them.

    Returns the count of found devices.
*/

AVX afxUnit AvxInvokeDrawDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd, 

    // The ordinal number for the first drawing device to start the iteration from.
    afxUnit first, 

    // A pointer to user-defined data that will be passed to the callback function for each device.
    void* udd, 

    // The callback function that will be invoked for each drawing device during enumeration.
    afxBool(*f)(void*, afxDrawDevice), 

    // The maximum number of drawing devices to be considered during the invocation.
    afxUnit maxCnt
);

/*
    The AvxEvokeDrawDevices() function provides an iterative mechanism to enumerate drawing devices and 
    process each device through a callback function. It also stores the enumerated devices in the devices[] array, 
    allowing further operations or checks to be performed after enumeration. This function offers flexibility in 
    both processing devices and retrieving them for further use, making it useful for applications that need to 
    interact with multiple available GPUs or drawing devices in a system.

    Returns the count of found devices.
*/

AVX afxUnit AvxEvokeDrawDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // The ordinal number for the first drawing device to start the iteration from.
    afxUnit first, 

    // A pointer to user-defined data that will be passed to the callback function for each device.
    void* udd,

    // The callback function that will be invoked for each drawing device during enumeration.
    afxBool(*f)(void*, afxDrawDevice), 

    // The maximum number of drawing devices to be considered during the invocation.
    afxUnit maxCnt,

    // An array that will be populated with the drawing devices found during the enumeration process.
    afxDrawDevice devices[]
);

// DRAW DEVICE HANDLING ////////////////////////////////////////////////////////

AVX afxBool AvxIsDrawDevicePrompt
(
    afxDrawDevice ddev
);

/*
    The AvxQueryDrawDeviceFeatures() function is a way to query and retrieve detailed information about 
    features of a specific drawing device. The afxDrawFeatures structure holds the queried features, and after calling this function, 
    you can access the capabilities of the device to make informed decisions about rendering or utilizing the device's features.
*/

AVX void AvxQueryDrawDeviceFeatures
(
    // A handle for a drawing device to query for its features.
    afxDrawDevice ddev,

    // A pointer to a afxDrawFeatures structure where the results will be stored.
    afxDrawFeatures* features
);

/*
    The AvxQueryDrawDeviceLimits() function queries the hardware limits of a specific drawing device. 
    The device's capabilities are often constrained by the physical hardware, and knowing these limits 
    is crucial when developing applications that need to operate efficiently within the device's capabilities. 
    These limits are often critical for tasks such as optimizing memory usage, setting rendering parameters, 
    or determining which rendering techniques can be supported.
*/

AVX void AvxQueryDrawDeviceLimits
(
    // The device to query for its limits.
    afxDrawDevice ddev, 

    // A afxDrawLimits structure where the device's limits will be stored.
    afxDrawLimits* limits
);

/*
    The AvxIsDrawDeviceAcceptable() function determines if a given drawing device is suitable for use based on both the 
    required features and hardware limits. If the device meets both the feature requirements and hardware constraints, it 
    would be considered acceptable for the application. If it fails to meet either one, the device would not be considered acceptable.
*/

AVX afxBool AvxIsDrawDeviceAcceptable
(
    // The device you are considering for use in your application.
    afxDrawDevice ddev, 
    
    // A afxDrawFeatures structure containing the required features that the drawing device must support.
    afxDrawFeatures const* features, 

    // A afxDrawLimits structure containing the hardware limits that the drawing device should meet or exceed.
    afxDrawLimits const* limits
);

AVX void AvxQueryDrawCapabilities
(
    // The drawing device to query.
    afxDrawDevice ddev, 

    afxDrawPortInfo* caps
);

// Query draw port support for presentation.

AVX afxBool AvxDoesDrawPortSupportPresentation
(
    afxDrawDevice ddev, 
    afxUnit portId, 
    afxSurface dout
);

/*
    The AvxHasDrawDeviceExtensions() function checks whether certain extensions are available for the specified drawing device.

    The function will likely iterate over the names, checking if each specified extension is supported by the referenced device.
    
    It will return the amount of supported extensions in the specified list.
*/

AVX afxUnit AvxHasDrawDeviceExtensions
(
    // The drawing device for which we want to check the extension support.
    afxDrawDevice ddev,

    // The count of extensions to check.
    afxUnit cnt,

    // The list of extension names we want to check for.
    afxString const names[],

    // This is an output array where the function will store the results.
    // For each extension name in names[], the corresponding entry in supported[] will indicate whether the device supports that extension.
    afxBool supported[]
);

/*
    The AvxDescribeDeviceFormats() funcion queries a specific drawing device for detailed information about how it 
    handles specified formats. The function then fills the provided descs[] array with information on each queried format, 
    such as whether the format is supported and what characteristics are relevant for rendering with that format.

    This is useful for applications that need to understand the device's supported formats before selecting an appropriate one 
    for tasks like texture loading, rendering, or resource allocation.
*/

AVX afxError AvxDescribeDeviceFormats
// Describes draw device's format capabilities.
(
    // The device to query for format-related properties.
    afxDrawDevice ddev, 

    // The number of formats to be queried.
    afxUnit cnt, 

    // An array of formats whose properties are to be queried.
    avxFormat const formats[], 

    // An array of structures where the properties and capabilities of each queried format for the specified device.
    avxFormatDescription descs[]
);

AVX afxUnit         AvxEnumerateVideoPresentationModes(afxSurface dout, afxUnit first, afxUnit cnt, avxPresentFlags modes[]);
AVX afxUnit         AvxEnumerateVideoOutputFormats(afxSurface dout, afxUnit first, afxUnit cnt, avxFormat formats[]);

AVX afxUnit         AvxEnumerateDisplayModes(afxDrawDevice ddev, afxUnit vdu, afxUnit cnt, avxDisplayMode modes[]);

#endif//AVX_DRAW_DEVICE_H
