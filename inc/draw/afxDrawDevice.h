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

  //////////////////////////////////////////////////////////////////////////////
 // Qwadro Unified Video Graphics Infrastructure                             //
//////////////////////////////////////////////////////////////////////////////

/*
    The Qwadro draw I/O system refers to the overall framework or set of components involved in creating, managing, and rendering graphical content. 
    This can include both hardware and software elements that work together to produce and display visual data.

    In the draw system, graphics hardware refers to the physical components that handle the rendering of images and video.

    Graphics Processing Unit (GPU) is a specialized chip designed to accelerate rendering tasks by processing complex graphical computations.
    Graphics Card is a hardware component that includes the GPU, memory, and video output interfaces to drive displays and manage graphical output.

    Software components of the draw system include the tools and libraries used to create and manage graphical content.

    APIs and libraries like OpenGL, DirectX, Vulkan, or Metal that provide the functions and tools for rendering graphics.

    The rendering pipeline is a series of stages and processes that graphical data goes through to be rendered onto a screen.

    The integration and management aspect of the draw system involves coordinating between different components and ensuring efficient rendering and display.
    Driver Software manages communication between the system and graphics hardware.

    Draw I/O mechanisms are software systems that integrate various components to render complex scenes.
*/

/*
    A draw device typically refers to a hardware or software component used to render or display graphical content. 
    It is responsible for processing and presenting visual data, whether it's through direct output to a screen or through intermediate rendering processes.

    In graphics hardware, a draw device refers to a physical component that handles rendering tasks. It processes graphical instructions and outputs them to a display.
    In software, a draw device can refer to a virtual or logical component used to handle drawing operations. It is often part of a graphics library or API.
    In VR and AR systems, draw devices are components that render virtual environments and overlay information onto the real world.

*/

// In Qwadro Video Graphics Infrastructure, a draw device installs and operates a whole draw I/O system. This is why the draw device is who provides draw I/O mechanisms.

#ifndef AVX_DRAW_DEVICE_H
#define AVX_DRAW_DEVICE_H

#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/draw/afxDrawBridge.h"
#include "qwadro/inc/draw/op/afxDrawOutput.h"
#include "qwadro/inc/draw/op/afxDrawInput.h"

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
    afxUnit     maxRasterDim1D;
    afxUnit     maxRasterDim2D;
    afxUnit     maxRasterDim3D;
    afxUnit     maxRasterDimCube;
    afxUnit     maxRasterArrayLayers;
    afxUnit     maxTexelBufElements;
    afxUnit     maxUboRange;
    afxUnit     maxSsboRange;
    afxUnit     maxPushConstsSiz;
    afxUnit     maxMemAllocCnt;
    afxUnit     maxSamplerAllocCnt;
    afxSize     bufferRasterGranularity;
    afxSize     sparseAddrSpaceSiz;
    afxUnit     maxBoundLigaSets;
    afxUnit     maxPerStageSamplers;
    afxUnit     maxPerStageUbos;
    afxUnit     maxPerStageSsbos;
    afxUnit     maxPerStageSampledImages;
    afxUnit     maxPerStageStorageImages;
    afxUnit     maxPerStageInputAttachments;
    afxUnit     maxPerStageResources;
    afxUnit     maxLigaSetSamplers;
    afxUnit     maxLigaSetUbos;
    afxUnit     maxLigaSetUbosDynamic;
    afxUnit     maxLigaSetSsbos;
    afxUnit     maxLigaSetSsbosDynamic;
    afxUnit     maxLigaSetSampledImages;
    afxUnit     maxLigaSetStorageImages;
    afxUnit     maxLigaSetInputAttachments;
    afxUnit     maxVtxIns;
    afxUnit     maxVtxInSrcs;
    afxUnit     maxVtxInOffset;
    afxUnit     maxVtxInSrcStride;
    afxUnit     maxVtxOutCompos;
    afxUnit     maxTessGenLvl;
    afxUnit     maxTessPatchSiz;
    afxUnit     maxTessCtrlPerVtxInComps;
    afxUnit     maxTessCtrlPerVtxOutComps;
    afxUnit     maxTessCtrlPerPatchOutComps;
    afxUnit     maxTessCtrlTotalOutComps;
    afxUnit     maxTessEvalInComps;
    afxUnit     maxTessEvalOutComps;
    afxUnit     maxPrimShadInvocations;
    afxUnit     maxPrimInComps;
    afxUnit     maxPrimOutComps;
    afxUnit     maxPrimOutVertices;
    afxUnit     maxPrimTotalOutComps;
    afxUnit     maxFragInComps;
    afxUnit     maxFragOutAttachments;
    afxUnit     maxFragDualSrcAttachments;
    afxUnit     maxFragCombinedOutputResources;
    afxUnit     maxComputeSharedMemSiz;
    afxWhd      maxComputeWarpCnt;
    afxUnit     maxComputeWarpInvocations;
    afxWhd      maxComputeWarpSiz;
    afxUnit     subPixelPreciBits;
    afxUnit     subTexelPreciBits;
    afxUnit     mipmapPrecisionBits;
    afxUnit     maxDrawIndexedIdxValue;
    afxUnit     maxDrawIndirectCnt;
    afxReal     maxSamplerLodBias;
    afxReal     maxSamplerAnisotropy;
    afxUnit     maxVpCnt;
    afxUnit     maxVpDimensions[2];
    afxV2d      vpBoundsRange;
    afxUnit     vpSubPixelBits;
    afxSize     minMemMapAlign;
    afxSize     minTexelBufOffsetAlign;
    afxSize     minUboOffsetAlign;
    afxSize     minSsboOffsetAlign;
    afxInt      minTexelOffset;
    afxUnit     maxTexelOffset;
    afxInt      minTexelGatherOffset;
    afxUnit     maxTexelGatherOffset;
    afxReal     minInterpolationOffset;
    afxReal     maxInterpolationOffset;
    afxUnit     subPixelInterpolationOffsetBits;
    afxWhd      maxCanvasWhd;
    afxFlags    canvasColorSampleCnts;
    afxFlags    canvasDepthSampleCnts;
    afxFlags    canvasStencilSampleCnts;
    afxFlags    canvasNoAttachmentsSampleCnts;
    afxUnit     maxColorAttachments;
    afxFlags    sampledRasterColorSampleCnts;
    afxFlags    sampledRasterIntegerSampleCnts;
    afxFlags    sampledRasterDepthSampleCnts;
    afxFlags    sampledRasterStencilSampleCnts;
    afxFlags    storageRasterSampleCnts;
    afxUnit     maxSampleMaskWords;
    afxBool     timestampComputeAndGraphics;
    afxReal     timestampPeriod;
    afxUnit     maxClipDistances;
    afxUnit     maxCullDistances;
    afxUnit     maxCombinedClipAndCullDistances;
    afxUnit     discreteQueuePriorities;
    afxV2d      pointSizRange;
    afxV2d      lineWidthRange;
    afxReal     pointSizGranularity;
    afxReal     lineWidthGranularity;
    afxBool     strictLines;
    afxBool     standardSampleLocations;
    afxSize     optimalBufCopyOffsetAlign;
    afxSize     optimalBufCopyRowPitchAlign;
    afxSize     nonCoherentAtomSiz;
};

// DRAW DEVICE HANDLING ////////////////////////////////////////////////////////

AVX afxBool         AfxIsDrawDevicePrompt(afxDrawDevice ddev);

/// Test the features and limits of a draw device.
AVX afxBool         AfxIsDrawDeviceAcceptable(afxDrawDevice ddev, afxDrawFeatures const* features, afxDrawLimits const* limits);
AVX void            AfxQueryDrawDeviceFeatures(afxDrawDevice ddev, afxDrawFeatures* features);
AVX void            AfxQueryDrawDeviceLimits(afxDrawDevice ddev, afxDrawLimits* limits);

AVX afxUnit         AfxCountDrawPorts(afxDrawDevice ddev);
AVX void            AfxQueryDrawPortCapabilities(afxDrawDevice ddev, afxUnit portId, afxDrawPortCaps* caps);
AVX afxUnit         AfxChooseDrawPorts(afxDrawDevice ddev, afxDrawPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[]);

// Query draw port support for presentation.
AVX afxBool         AfxDoesDrawPortSupportPresentation(afxDrawDevice ddev, afxUnit portId, afxDrawOutput dout);
AVX afxUnit         AfxEnumerateVideoPresentationModes(afxDrawOutput dout, afxUnit first, afxUnit cnt, avxPresentMode modes[]);
AVX afxUnit         AfxEnumerateVideoOutputFormats(afxDrawOutput dout, afxUnit first, afxUnit cnt, avxFormat formats[]);

AVX void*           AfxGetDrawDeviceIdd(afxDrawDevice ddev);

// IMPLEMENTATION DISCOVERY ////////////////////////////////////////////////////

AVX afxUnit         AfxChooseDrawDevices(afxUnit icd, afxDrawFeatures const* features, afxDrawLimits const* limits, afxUnit maxCnt, afxUnit ddevIds[]); // return count of found devices

/**
    The AfxEnumerateDrawDevices() function lets you obtain the handle to each draw device in the current session.

    @param first the number ordinal for the first draw device to start the iteration from.
    @param cnt the number of draw devices to be retrieved.
    @return Returns the number of draw devices inserted in the @param devices.
*/

AVX afxUnit         AfxEnumerateDrawDevices(afxUnit icd, afxUnit first, afxUnit cnt, afxDrawDevice devices[]);

/// Enumerates all draw devices by passing the handle to each device, in turn, to an application-defined callback function. 
/// AfxEvokeDrawDevices() continues until the last device is enumerated or the callback function returns FALSE.
AVX afxUnit         AfxInvokeDrawDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawDevice), afxUnit cnt);

AVX afxUnit         AfxEvokeDrawDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawDevice), afxUnit cnt, afxDrawDevice devices[]);

AVX afxUnit         AfxEnumerateDrawSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxDrawSystem systems[]);
AVX afxUnit         AfxInvokeDrawSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt);
AVX afxUnit         AfxEvokeDrawSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt, afxDrawSystem systems[]);

#endif//AVX_DRAW_DEVICE_H
