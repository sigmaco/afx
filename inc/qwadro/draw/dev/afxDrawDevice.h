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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // Advanced Visual Experience on Qwadro                                     //
//////////////////////////////////////////////////////////////////////////////
// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_DRAW_DEVICE_H
#define AVX_DRAW_DEVICE_H

#include "qwadro/exec/afxDevice.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/math/avxColor.h"
#include "qwadro/draw/afxPixel.h"
#include "qwadro/draw/math/avxMatrix.h"
// provided classes.
#include "qwadro/draw/dev/afxDrawBridge.h"
#include "qwadro/draw/avxCmdb.h"
#include "qwadro/draw/pipe/avxDrawOps.h"
//#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/dev/afxDrawOutput.h"
#include "qwadro/draw/dev/afxDrawInput.h"
#include "qwadro/draw/io/afxTarga.h"
// provided classes.
#include "qwadro/draw/avxCamera.h"

AFX_DEFINE_STRUCT(afxDrawDeviceCaps)
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
    afxBool8 shaderUniformBufferArrayDynIndexing;
    afxBool8 shaderSampledRasterArrayDynIndexing;
    afxBool8 shaderStorageBufferArrayDynIndexing;
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

    afxBool8 storageBuf16bitAccess;
    afxBool8 uniformAndStorageBuf16bitAccess;
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

    afxBool8 samplerMirrorClampToEdge;
    afxBool8 drawIndirectCnt;
    afxBool8 storageBuffer8BitAccess;
    afxBool8 uniformAndStorageBuffer8bitAccess;
    afxBool8 storagePushConst8;
    afxBool8 shaderBufInt64Atomics;
    afxBool8 shaderSharedInt64Atomics;
    afxBool8 shaderFloat16;
    afxBool8 shaderInt8;
    afxBool8 descIndexing;
    afxBool8 shaderInputAttachArrayDynamicIndexing;
    afxBool8 shaderUniformTexelBufArrayDynamicIndexing;
    afxBool8 shaderStorageTexelBufArrayDynamicIndexing;
    afxBool8 shaderUniformBufArrayNonUniformIndexing;
    afxBool8 shaderSampledRasArrayNonUniformIndexing;
    afxBool8 shaderStorageBufArrayNonUniformIndexing;
    afxBool8 shaderStorageRasArrayNonUniformIndexing;
    afxBool8 shaderInputAttachmentArrayNonUniformIndexing;
    afxBool8 shaderUniformTexelBufArrayNonUniformIndexing;
    afxBool8 shaderStorageTexelBufArrayNonUniformIndexing;
    afxBool8 descBindingUniformBufUpdAfterBind;
    afxBool8 descBindingSampledRasUpdAfterBind;
    afxBool8 descBindingStorageRasUpdAfterBind;
    afxBool8 descBindingStorageBufUpdAfterBind;
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

AFX_DEFINE_STRUCT(afxDrawDeviceLimits)
{
    afxNat      maxRasterDim1D;
    afxNat      maxRasterDim2D;
    afxNat      maxRasterDim3D;
    afxNat      maxRasterDimCube;
    afxNat      maxRasterArrayLayers;
    afxNat      maxTexelBufElements;
    afxNat      maxUniformBufRange;
    afxNat      maxStorageBufRange;
    afxNat      maxPushConstantsSiz;
    afxNat      maxMemAllocCnt;
    afxNat      maxSamplerAllocCnt;
    afxSize     bufferRasterGranularity;
    afxSize     sparseAddrSpaceSiz;
    afxNat      maxBoundDescriptorSets;
    afxNat      maxPerStageDescriptorSamplers;
    afxNat      maxPerStageDescriptorUniformBuffers;
    afxNat      maxPerStageDescriptorStorageBuffers;
    afxNat      maxPerStageDescriptorSampledImages;
    afxNat      maxPerStageDescriptorStorageImages;
    afxNat      maxPerStageDescriptorInputAttachments;
    afxNat      maxPerStageResources;
    afxNat      maxDescriptorSetSamplers;
    afxNat      maxDescriptorSetUniformBuffers;
    afxNat      maxDescriptorSetUniformBuffersDynamic;
    afxNat      maxDescriptorSetStorageBuffers;
    afxNat      maxDescriptorSetStorageBuffersDynamic;
    afxNat      maxDescriptorSetSampledImages;
    afxNat      maxDescriptorSetStorageImages;
    afxNat      maxDescriptorSetInputAttachments;
    afxNat      maxVtxInputAttributes;
    afxNat      maxVtxInputBindings;
    afxNat      maxVtxInputAttributeOffset;
    afxNat      maxVtxInputBindingStride;
    afxNat      maxVtxOutputCompos;
    afxNat      maxTessGenLvl;
    afxNat      maxTessPatchSiz;
    afxNat      maxTessCtrlPerVtxInComps;
    afxNat      maxTessCtrlPerVtxOutComps;
    afxNat      maxTessCtrlPerPatchOutComps;
    afxNat      maxTessCtrlTotalOutComps;
    afxNat      maxTessEvalInComps;
    afxNat      maxTessEvalOutComps;
    afxNat      maxPrimShaderInvocations;
    afxNat      maxPrimInComps;
    afxNat      maxPrimOutComps;
    afxNat      maxPrimOutVertices;
    afxNat      maxPrimTotalOutComps;
    afxNat      maxFragInComps;
    afxNat      maxFragOutAttachments;
    afxNat      maxFragDualSrcAttachments;
    afxNat      maxFragCombinedOutputResources;
    afxNat      maxComputeSharedMemorySiz;
    afxWhd      maxComputeWorkGroupCnt;
    afxNat      maxComputeWorkGroupInvocations;
    afxWhd      maxComputeWorkGroupSiz;
    afxNat      subPixelPrecisionBits;
    afxNat      subTexelPrecisionBits;
    afxNat      mipmapPrecisionBits;
    afxNat      maxDrawIndexedIdxValue;
    afxNat      maxDrawIndirectCnt;
    afxReal     maxSamplerLodBias;
    afxReal     maxSamplerAnisotropy;
    afxNat      maxVpCnt;
    afxNat      maxVpDimensions[2];
    afxV2d      vpBoundsRange;
    afxNat      vpSubPixelBits;
    afxSize     minMemMapAlign;
    afxSize     minTexelBufOffsetAlign;
    afxSize     minUniformBufOffsetAlign;
    afxSize     minStorageBufOffsetAlign;
    afxInt      minTexelOffset;
    afxNat      maxTexelOffset;
    afxInt      minTexelGatherOffset;
    afxNat      maxTexelGatherOffset;
    afxReal     minInterpolationOffset;
    afxReal     maxInterpolationOffset;
    afxNat      subPixelInterpolationOffsetBits;
    afxWhd      maxCanvasWhd;
    afxFlags    canvasColorSampleCnts;
    afxFlags    canvasDepthSampleCnts;
    afxFlags    canvasStencilSampleCnts;
    afxFlags    canvasNoAttachmentsSampleCnts;
    afxNat      maxColorAttachments;
    afxFlags    sampledRasterColorSampleCnts;
    afxFlags    sampledRasterIntegerSampleCnts;
    afxFlags    sampledRasterDepthSampleCnts;
    afxFlags    sampledRasterStencilSampleCnts;
    afxFlags    storageRasterSampleCnts;
    afxNat      maxSampleMaskWords;
    afxBool     timestampComputeAndGraphics;
    afxReal     timestampPeriod;
    afxNat      maxClipDistances;
    afxNat      maxCullDistances;
    afxNat      maxCombinedClipAndCullDistances;
    afxNat      discreteQueuePriorities;
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

AFX_DEFINE_STRUCT(afxDrawPortCaps)
{
    afxDrawBridgeFlags  queFlags;
    afxNat              queCnt; // the count of queues in this port. Each port must support at least one queue.
};

AVX afxBool         AfxDrawDeviceIsRunning(afxDrawDevice ddev);

AVX void*           AfxGetDrawDeviceIdd(afxDrawDevice ddev);

AVX afxClass const* AfxGetDrawContextClass(afxDrawDevice ddev);

/// Test the capabilities and limits of a draw device.
AVX afxBool         AfxIsDrawDeviceAcceptable(afxDrawDevice ddev, afxDrawDeviceCaps const* caps, afxDrawDeviceLimits const* limits);
AVX void            AfxGetDrawDeviceCapabilities(afxDrawDevice ddev, afxNat subset, afxDrawDeviceCaps* caps);
AVX void            AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxNat subset, afxDrawDeviceLimits* limits);

AVX afxNat          AfxCountDrawPorts(afxDrawDevice ddev);
AVX void            AfxGetDrawPortCapabilities(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps);

AVX afxNat          AfxEnumerateDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawContext contexts[]);
AVX afxNat          AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput outputs[]);
AVX afxNat          AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput inputs[]);

AVX afxNat          AfxEvokeDrawContexts(afxDrawDevice ddev, afxBool(*f)(afxDrawContext, void*), void* udd, afxNat first, afxNat cnt, afxDrawContext contexts[]);
AVX afxNat          AfxEvokeDrawOutputs(afxDrawDevice ddev, afxBool(*f)(afxDrawOutput, void*), void* udd, afxNat first, afxNat cnt, afxDrawOutput outputs[]);
AVX afxNat          AfxEvokeDrawInputs(afxDrawDevice ddev, afxBool(*f)(afxDrawInput, void*), void* udd, afxNat first, afxNat cnt, afxDrawInput inputs[]);

AVX afxNat          AfxInvokeDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd);
AVX afxNat          AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AVX afxNat          AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);

AVX afxNat          AfxInvokeDrawContexts2(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void* udd, afxBool(*f2)(afxDrawContext,void*), void* udd2);
AVX afxNat          AfxInvokeDrawOutputs2(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd, afxBool(*f2)(afxDrawOutput,void*), void* udd2);
AVX afxNat          AfxInvokeDrawInputs2(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput,void*), void* udd, afxBool(*f2)(afxDrawInput, void*), void* udd2);

#endif//AVX_DRAW_DEVICE_H
