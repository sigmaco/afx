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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_SURFACE_C
#define _AVX_DRAW_INPUT_C
#include "ddi/avxImplementation.h"
#include "../impl/afxExecImplKit.h"

//extern afxChain* _AfxGetSystemClassChain(void);
AFX afxChain* _AfxGetSystemClassChain(void);

afxDrawLimits const AVX_STD_DRAW_LIMITS =
{
    // buffer and raster storage
    .minBufMapAlign = AVX_BUFFER_ALIGNMENT,
    .optimalBufCopyOffsetAlign = AFX_SIMD_ALIGNMENT,
    .optimalBufCopyRowPitchAlign = AFX_SIMD_ALIGNMENT,

    .minUboOffsetAlign = AVX_BUFFER_ALIGNMENT,
    .maxUboRange = 16384,
    .minSsboOffsetAlign = AVX_BUFFER_ALIGNMENT,
    .maxSsboRange = 128000000, // 128MB
    .minTboOffsetAlign = AVX_BUFFER_ALIGNMENT,
    .maxTboCap = 65536,
    .bufferRasterGranularity = AVX_RASTER_ALIGNMENT,

    // texture

    .maxRasterDim1D = 1024,
    .maxRasterDim2D = 1024,
    .maxRasterDim3D = 64,
    .maxRasterDimCube = 1024,
    .maxRasterLayers = 256,

    .maxSamplerLodBias = 2.f,
    .maxSamplerAnisotropy = 0.f,

    // vertex assembler

    .maxVtxIns = 16,
    .maxVtxOutCompos = 64,
    .maxVtxInSrcs = 1,
    .maxVtxInOffset = 1024,
    .maxVtxInSrcStride = 2048,

    .maxClipDistances = 8,
    .maxCullDistances = 8,
    .maxCombinedClipAndCullDistances = 8,

    // viewport transformation

    .maxVpCnt = 16,
    .maxVpDimensions[0] = 1,
    .maxVpDimensions[1] = 1,
    .vpBoundsRange[0] = -32768,
    .vpBoundsRange[1] = 32767,
    .vpSubPixelBits = 0,

    // geometry shader

    .maxPrimInComps = 64,
    .maxPrimOutComps = 64,
    .maxPrimShadInvocations = 0,
    .maxPrimOutVertices = 0,
    .maxPrimTotalOutComps = 0,

    // tesselation

    .maxTessGenLvl = 0,
    .maxTessPatchSiz = 0,
    .maxTessCtrlPerVtxInComps = 0,
    .maxTessCtrlPerVtxOutComps = 0,
    .maxTessCtrlPerPatchOutComps = 0,
    .maxTessCtrlTotalOutComps = 0,
    .maxTessEvalInComps = 0,
    .maxTessEvalOutComps = 0,

    // canvas

    .maxColorAttachments = 8,
    .maxCanvasWhd.w = 16384,
    .maxCanvasWhd.h = 16384,
    .maxCanvasWhd.d = 2048,
    .canvasNoAttachmentsSampleCnts = 4,

    .maxSampleMaskWords = 0,
    .sampledRasterIntegerSampleCnts = 0,
    .sampledRasterColorSampleCnts = 0,
    .canvasColorSampleCnts = 0,
    .sampledRasterDepthSampleCnts = 0,
    .sampledRasterStencilSampleCnts = 0,
    .canvasDepthSampleCnts = 0,
    .canvasStencilSampleCnts = 0,

    .pointSizRange[0] = 1,
    .pointSizRange[1] = 1,
    .pointSizGranularity = 1,
    .lineWidthRange[0] = 1,
    .lineWidthRange[1] = 1,
    .lineWidthGranularity = 1,

    .maxFragInComps = 128,
    .maxFragOutAttachments = 8,
    .maxFragDualSrcAttachments = 1,
    .minTexelOffset = -8,
    .maxTexelOffset = 7,
    .minTexelGatherOffset = 1,
    .maxTexelGatherOffset = 1,

    .minInterpolationOffset = 1,
    .maxInterpolationOffset = 1,
    .subPixelInterpolationOffsetBits = 1,

    .maxComputeWarpInvocations = 0,
    .maxComputeWarpCnt.w = 0,
    .maxComputeWarpCnt.h = 0,
    .maxComputeWarpCnt.d = 0,

    .maxComputeWarpSiz.w = 0,
    .maxComputeWarpSiz.h = 0,
    .maxComputeWarpSiz.d = 0,

    .maxComputeSharedMemSiz = 0,
    
    .maxDrawIndexedIdxValue = sizeof(afxUnit32),

    .maxBoundPerLigas = 4,
    .maxPushConstsSiz = 128,

    .maxPerStageUbos = 70,
    .maxPerStageSsbos = 70,
    .maxPerStageSampledImages = 70,
    .maxPerStageSamplers = 70,

    .maxPerLigaSamplers = 16,
    .maxPerLigaSampledImages = 16,

    .maxPerLigaUbos = 36,
    .maxPerLigaSsbos = 8
};

////////////////////////////////////////////////////////////////////////////////
// DRAW DEVICE HANDLING                                                       //
////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawLimits const* _AvxDdevGetLimits(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    return &ddev->limits;
}

_AVX afxBool AvxIsDrawDevicePrompt(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    return ddev->dev.serving;
}

_AVX void* _AvxGetDrawDeviceIdd(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    return ddev->idd;
}

_AVX void AvxQueryDrawDeviceLimits(afxDrawDevice ddev, afxDrawLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    AFX_ASSERT(limits);
    *limits = ddev->limits;
}

_AVX void AfxLogDrawDeviceLimits(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    afxDrawLimits limits;
    AvxQueryDrawDeviceLimits(ddev, &limits);

    afxChar buf[AFX_U16_MAX];
    afxString s;
    AfxMakeString(&s, sizeof(buf), buf, 0);

#if 0
    AfxFormatString
    (&s,
        "maxRasterDim1D : %u\n"
        "maxRasterDim2D : %u\n"
        "maxRasterDim3D : %u\n"
        "maxRasterDimCube : %u\n"
        "maxRasterArrayLayers : %u\n"
        "maxTexelBufElements : %u\n"
        "maxUboRange : %u\n"
        "maxSsboRange : %u\n"
        "maxPushConstsSiz : %u\n"
        "maxMemAllocCnt : %u\n"
        "maxSamplerAllocCnt : %u\n"
        "maxBoundPerLigas : %u\n"
        "maxPerStageSamplers : %u\n"
        "maxPerStageUbos : %u\n"
        "maxPerStageSsbos : %u\n"
        "maxPerStageSampledImages : %u\n"
        "maxPerStageStorageImages : %u\n"
        "maxPerStageInputAttachments : %u\n"
        "maxPerStageResources : %u\n"
        "maxPerLigaSamplers : %u\n"
        "maxPerLigaUbos : %u\n"
        "maxPerLigaUbosDynamic : %u\n"
        "maxPerLigaSsbos : %u\n"
        "maxPerLigaSsbosDynamic : %u\n"
        "maxPerLigaSampledImages : %u\n"
        "maxPerLigaStorageImages : %u\n"
        "maxPerLigaInputAttachments : %u\n"
        "maxVtxIns : %u\n"
        "maxVtxInSrcs : %u\n"
        "maxVtxInOffset : %u\n"
        "maxVtxInSrcStride : %u\n"
        "maxVtxOutCompos : %u\n"
        "maxTessGenLvl : %u\n"
        "maxTessPatchSiz : %u\n"
        "maxTessCtrlPerVtxInComps : %u\n"
        "maxTessCtrlPerVtxOutComps : %u\n"
        "maxTessCtrlPerPatchOutComps : %u\n"
        "maxTessCtrlTotalOutComps : %u\n"
        "maxTessEvalInComps : %u\n"
        "maxTessEvalOutComps : %u\n"
        "maxPrimShadInvocations : %u\n"
        "maxPrimInComps : %u\n"
        "maxPrimOutComps : %u\n"
        "maxPrimOutVertices : %u\n"
        "maxPrimTotalOutComps : %u\n"
        "maxFragInComps : %u\n"
        "maxFragOutAttachments : %u\n"
        "maxFragDualSrcAttachments : %u\n"
        "maxFragCombinedOutputResources : %u\n"
        "maxComputeSharedMemSiz : %u\n"
        "maxComputeWarpCnt.w : %u\n"
        "maxComputeWarpCnt.h : %u\n"
        "maxComputeWarpCnt.d : %u\n"
        "maxComputeWarpInvocations : %u\n"
        "maxComputeWarpSiz.w : %u\n"
        "maxComputeWarpSiz.h : %u\n"
        "maxComputeWarpSiz.d : %u\n"
        "subPixelPreciBits : %u\n"
        "subTexelPreciBits : %u\n"
        "mipmapPrecisionBits : %u\n"
        "maxDrawIndexedIdxValue : %u\n"
        "maxDrawIndirectCnt : %u\n"
        " : %u\n"
        " : %u\n"
        " : %u\n"
        " : %u\n"
        " : %u\n",

        limits.maxRasterDim1D,
        limits.maxRasterDim2D,
        limits.maxRasterDim3D,
        limits.maxRasterDimCube,
        limits.maxRasterArrayLayers,
        limits.maxTexelBufElements,
        limits.maxUboRange,
        limits.maxSsboRange,
        limits.maxPushConstsSiz,
        limits.maxMemAllocCnt,
        limits.maxSamplerAllocCnt,
        limits.maxBoundPerLigas,
        limits.maxPerStageSamplers,
        limits.maxPerStageUbos,
        limits.maxPerStageSsbos,
        limits.maxPerStageSampledImages,
        limits.maxPerStageStorageImages,
        limits.maxPerStageInputAttachments,
        limits.maxPerStageResources,
        limits.maxPerLigaSamplers,
        limits.maxPerLigaUbos,
        limits.maxPerLigaUbosDynamic,
        limits.maxPerLigaSsbos,
        limits.maxPerLigaSsbosDynamic,
        limits.maxPerLigaSampledImages,
        limits.maxPerLigaStorageImages,
        limits.maxPerLigaInputAttachments,
        limits.maxVtxIns,
        limits.maxVtxInSrcs,
        limits.maxVtxInOffset,
        limits.maxVtxInSrcStride,
        limits.maxVtxOutCompos,
        limits.maxTessGenLvl,
        limits.maxTessPatchSiz,
        limits.maxTessCtrlPerVtxInComps,
        limits.maxTessCtrlPerVtxOutComps,
        limits.maxTessCtrlPerPatchOutComps,
        limits.maxTessCtrlTotalOutComps,
        limits.maxTessEvalInComps,
        limits.maxTessEvalOutComps,
        limits.maxPrimShadInvocations,
        limits.maxPrimInComps,
        limits.maxPrimOutComps,
        limits.maxPrimOutVertices,
        limits.maxPrimTotalOutComps,
        limits.maxFragInComps,
        limits.maxFragOutAttachments,
        limits.maxFragDualSrcAttachments,
        limits.maxFragCombinedOutputResources,
        limits.maxComputeSharedMemSiz,
        limits.maxComputeWarpCnt.w,
        limits.maxComputeWarpCnt.h,
        limits.maxComputeWarpCnt.d,
        limits.maxComputeWarpInvocations,
        limits.maxComputeWarpSiz.w,
        limits.maxComputeWarpSiz.h,
        limits.maxComputeWarpSiz.d,
        limits.subPixelPreciBits,
        limits.subTexelPreciBits,
        limits.mipmapPrecisionBits,
        limits.maxDrawIndexedIdxValue,
        limits.maxDrawIndirectCnt,

        );

    afxSize     bufferRasterGranularity;
    afxSize     sparseAddrSpaceSiz;

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
    afxWarp      maxCanvasWhd;
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
#else
    // TODO nest by feature
#endif
}

_AVX void AvxQueryDrawDeviceFeatures(afxDrawDevice ddev, afxDrawFeatures* features)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    AFX_ASSERT(features);
    *features = ddev->features;
}

_AVX void AvxQueryDrawCapabilities(afxDrawDevice ddev, afxDrawPortInfo* caps)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    //AFX_ASSERT_RANGE(ddev->portCnt, basePortIdx, portCnt);
    //AFX_ASSERT(caps);
    afxUnit rslt = 0;

    if (caps)
    {
        caps->acceleration = ddev->acceleration;
        caps->capabilities = ddev->capabilities;
        caps->maxQueCnt = ddev->maxQueCnt;
        caps->minQueCnt = ddev->minQueCnt;
    }
}

_AVX afxBool AvxIsDrawDeviceAcceptable(afxDrawDevice ddev, afxDrawFeatures const* features, afxDrawLimits const* limits)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    AFX_ASSERT(limits);
    AFX_ASSERT(features);
    afxBool rslt = TRUE;

    if (features)
    {
        if ((features->robustBufAccess && !ddev->features.robustBufAccess) ||
            (features->fullDrawIdxUint32 && !ddev->features.fullDrawIdxUint32) ||
            (features->rasterCubeArray && !ddev->features.rasterCubeArray) ||
            (features->independentBlend && !ddev->features.independentBlend) ||
            (features->primShader && !ddev->features.primShader) ||
            (features->tessShader && !ddev->features.tessShader) ||
            (features->sampleRateShading && !ddev->features.sampleRateShading) ||
            (features->dualSrcBlend && !ddev->features.dualSrcBlend) ||
            (features->logicOp && !ddev->features.logicOp) ||
            (features->multiDrawIndirect && !ddev->features.multiDrawIndirect) ||
            (features->drawIndirectFirstInst && !ddev->features.drawIndirectFirstInst) ||
            (features->depthClamp && !ddev->features.depthClamp) ||
            (features->depthBiasClamp && !ddev->features.depthBiasClamp) ||
            (features->fillModeNonSolid && !ddev->features.fillModeNonSolid) ||
            (features->depthBounds && !ddev->features.depthBounds) ||
            (features->wideLines && !ddev->features.wideLines) ||
            (features->largePoints && !ddev->features.largePoints) ||
            (features->alphaToOne && !ddev->features.alphaToOne) ||
            (features->multiViewport && !ddev->features.multiViewport) ||
            (features->samplerAnisotropy && !ddev->features.samplerAnisotropy) ||
            (features->etc2 && !ddev->features.etc2) ||
            (features->astc_LDR && !ddev->features.astc_LDR) ||
            (features->dxt && !ddev->features.dxt) ||
            (features->occlusionQueryPrecise && !ddev->features.occlusionQueryPrecise) ||
            (features->pipelineStatsQuery && !ddev->features.pipelineStatsQuery) ||
            (features->vtxPipelineStoresAndAtomics && !ddev->features.vtxPipelineStoresAndAtomics) ||
            (features->fragStoresAndAtomics && !ddev->features.fragStoresAndAtomics) ||
            (features->shaderTessAndPrimPointSiz && !ddev->features.shaderTessAndPrimPointSiz) ||
            (features->shaderRasterGatherExt && !ddev->features.shaderRasterGatherExt) ||
            (features->shaderStorageRasterExtFmts && !ddev->features.shaderStorageRasterExtFmts) ||
            (features->shaderStorageRasterMultisample && !ddev->features.shaderStorageRasterMultisample) ||
            (features->shaderStorageRasterReadWithoutFmt && !ddev->features.shaderStorageRasterReadWithoutFmt) ||
            (features->shaderStorageRasterWriteWithoutFmt && !ddev->features.shaderStorageRasterWriteWithoutFmt) ||
            (features->shaderUboArrayDynIndexing && !ddev->features.shaderUboArrayDynIndexing) ||
            (features->shaderSampledRasterArrayDynIndexing && !ddev->features.shaderSampledRasterArrayDynIndexing) ||
            (features->shaderSsboArrayDynIndexing && !ddev->features.shaderSsboArrayDynIndexing) ||
            (features->shaderStorageImageArrayDynIndexing && !ddev->features.shaderStorageImageArrayDynIndexing) ||
            (features->shaderClipDist && !ddev->features.shaderClipDist) ||
            (features->shaderCullDist && !ddev->features.shaderCullDist) ||
            (features->shaderFloat64 && !ddev->features.shaderFloat64) ||
            (features->shaderInt64 && !ddev->features.shaderInt64) ||
            (features->shaderInt16 && !ddev->features.shaderInt16) ||
            (features->shaderRsrcResidency && !ddev->features.shaderRsrcResidency) ||
            (features->shaderRsrcMinLod && !ddev->features.shaderRsrcMinLod) ||
            (features->sparseBinding && !ddev->features.sparseBinding) ||
            (features->sparseResidencyBuffer && !ddev->features.sparseResidencyBuffer) ||
            (features->sparseResidencyRaster2D && !ddev->features.sparseResidencyRaster2D) ||
            (features->sparseResidencyRaster3D && !ddev->features.sparseResidencyRaster3D) ||
            (features->sparseResidency2Samples && !ddev->features.sparseResidency2Samples) ||
            (features->sparseResidency4Samples && !ddev->features.sparseResidency4Samples) ||
            (features->sparseResidency8Samples && !ddev->features.sparseResidency8Samples) ||
            (features->sparseResidency16Samples && !ddev->features.sparseResidency16Samples) ||
            (features->sparseResidencyAliased && !ddev->features.sparseResidencyAliased) ||
            (features->variableMultisampleRate && !ddev->features.variableMultisampleRate) ||
            (features->inheritedQueries && !ddev->features.inheritedQueries))
        {
            rslt = FALSE;
        }
    }

    if (limits)
    {
        if ((limits->maxRasterDim1D > ddev->limits.maxRasterDim1D) ||
            (limits->maxRasterDim2D > ddev->limits.maxRasterDim2D) ||
            (limits->maxRasterDim3D > ddev->limits.maxRasterDim3D) ||
            (limits->maxRasterDimCube > ddev->limits.maxRasterDimCube) ||
            (limits->maxRasterLayers > ddev->limits.maxRasterLayers) ||
            (limits->maxTboCap > ddev->limits.maxTboCap) ||
            (limits->maxUboRange > ddev->limits.maxUboRange) ||
            (limits->maxSsboRange > ddev->limits.maxSsboRange) ||
            (limits->maxPushConstsSiz > ddev->limits.maxPushConstsSiz) ||
            (limits->maxMemAllocCnt > ddev->limits.maxMemAllocCnt) ||
            (limits->maxSamplerAllocCnt > ddev->limits.maxSamplerAllocCnt) ||
            (limits->bufferRasterGranularity > ddev->limits.bufferRasterGranularity) ||
            (limits->sparseAddrSpaceSiz > ddev->limits.sparseAddrSpaceSiz) ||
            (limits->maxBoundPerLigas > ddev->limits.maxBoundPerLigas) ||
            (limits->maxPerStageSamplers > ddev->limits.maxPerStageSamplers) ||
            (limits->maxPerStageUbos > ddev->limits.maxPerStageUbos) ||
            (limits->maxPerStageSsbos > ddev->limits.maxPerStageSsbos) ||
            (limits->maxPerStageSampledImages > ddev->limits.maxPerStageSampledImages) ||
            (limits->maxPerStageStorageImages > ddev->limits.maxPerStageStorageImages) ||
            (limits->maxPerStageInputAttachments > ddev->limits.maxPerStageInputAttachments) ||
            (limits->maxPerStageResources > ddev->limits.maxPerStageResources) ||
            (limits->maxPerLigaSamplers > ddev->limits.maxPerLigaSamplers) ||
            (limits->maxPerLigaUbos > ddev->limits.maxPerLigaUbos) ||
            (limits->maxPerLigaUbosDynamic > ddev->limits.maxPerLigaUbosDynamic) ||
            (limits->maxPerLigaSsbos > ddev->limits.maxPerLigaSsbos) ||
            (limits->maxPerLigaSsbosDynamic > ddev->limits.maxPerLigaSsbosDynamic) ||
            (limits->maxPerLigaSampledImages > ddev->limits.maxPerLigaSampledImages) ||
            (limits->maxPerLigaStorageImages > ddev->limits.maxPerLigaStorageImages) ||
            (limits->maxPerLigaInputAttachments > ddev->limits.maxPerLigaInputAttachments) ||
            (limits->maxVtxIns > ddev->limits.maxVtxIns) ||
            (limits->maxVtxInSrcs > ddev->limits.maxVtxInSrcs) ||
            (limits->maxVtxInOffset > ddev->limits.maxVtxInOffset) ||
            (limits->maxVtxInSrcStride > ddev->limits.maxVtxInSrcStride) ||
            (limits->maxVtxOutCompos > ddev->limits.maxVtxOutCompos) ||
            (limits->maxTessGenLvl > ddev->limits.maxTessGenLvl) ||
            (limits->maxTessPatchSiz > ddev->limits.maxTessPatchSiz) ||
            (limits->maxTessCtrlPerVtxInComps > ddev->limits.maxTessCtrlPerVtxInComps) ||
            (limits->maxTessCtrlPerVtxOutComps > ddev->limits.maxTessCtrlPerVtxOutComps) ||
            (limits->maxTessCtrlPerPatchOutComps > ddev->limits.maxTessCtrlPerPatchOutComps) ||
            (limits->maxTessCtrlTotalOutComps > ddev->limits.maxTessCtrlTotalOutComps) ||
            (limits->maxTessEvalInComps > ddev->limits.maxTessEvalInComps) ||
            (limits->maxTessEvalOutComps > ddev->limits.maxTessEvalOutComps) ||
            (limits->maxPrimShadInvocations > ddev->limits.maxPrimShadInvocations) ||
            (limits->maxPrimInComps > ddev->limits.maxPrimInComps) ||
            (limits->maxPrimOutComps > ddev->limits.maxPrimOutComps) ||
            (limits->maxPrimOutVertices > ddev->limits.maxPrimOutVertices) ||
            (limits->maxPrimTotalOutComps > ddev->limits.maxPrimTotalOutComps) ||
            (limits->maxFragInComps > ddev->limits.maxFragInComps) ||
            (limits->maxFragOutAttachments > ddev->limits.maxFragOutAttachments) ||
            (limits->maxFragDualSrcAttachments > ddev->limits.maxFragDualSrcAttachments) ||
            (limits->maxFragCombinedOutputResources > ddev->limits.maxFragCombinedOutputResources) ||
            (limits->maxComputeSharedMemSiz > ddev->limits.maxComputeSharedMemSiz) ||
            (limits->maxComputeWarpCnt.w > ddev->limits.maxComputeWarpCnt.w) ||
            (limits->maxComputeWarpCnt.h > ddev->limits.maxComputeWarpCnt.h) ||
            (limits->maxComputeWarpCnt.d > ddev->limits.maxComputeWarpCnt.d) ||
            (limits->maxComputeWarpInvocations > ddev->limits.maxComputeWarpInvocations) ||
            (limits->maxComputeWarpSiz.w > ddev->limits.maxComputeWarpSiz.w) ||
            (limits->maxComputeWarpSiz.h > ddev->limits.maxComputeWarpSiz.h) ||
            (limits->maxComputeWarpSiz.d > ddev->limits.maxComputeWarpSiz.d) ||
            (limits->subPixelPreciBits > ddev->limits.subPixelPreciBits) ||
            (limits->subTexelPreciBits > ddev->limits.subTexelPreciBits) ||
            (limits->mipmapPrecisionBits > ddev->limits.mipmapPrecisionBits) ||
            (limits->maxDrawIndexedIdxValue > ddev->limits.maxDrawIndexedIdxValue) ||
            (limits->maxDrawIndirectCnt > ddev->limits.maxDrawIndirectCnt) ||
            (limits->maxSamplerLodBias > ddev->limits.maxSamplerLodBias) ||
            (limits->maxSamplerAnisotropy > ddev->limits.maxSamplerAnisotropy) ||
            (limits->maxVpCnt > ddev->limits.maxVpCnt) ||
            (limits->maxVpDimensions[0] > ddev->limits.maxVpDimensions[0]) ||
            (limits->maxVpDimensions[1] > ddev->limits.maxVpDimensions[1]) ||
            (limits->vpBoundsRange[0] > ddev->limits.vpBoundsRange[0]) ||
            (limits->vpBoundsRange[1] > ddev->limits.vpBoundsRange[1]) ||
            (limits->vpSubPixelBits > ddev->limits.vpSubPixelBits) ||
            (limits->minBufMapAlign > ddev->limits.minBufMapAlign) ||
            (limits->minTboOffsetAlign > ddev->limits.minTboOffsetAlign) ||
            (limits->minUboOffsetAlign > ddev->limits.minUboOffsetAlign) ||
            (limits->minSsboOffsetAlign > ddev->limits.minSsboOffsetAlign) ||
            (limits->minTexelOffset > ddev->limits.minTexelOffset) ||
            (limits->maxTexelOffset > ddev->limits.maxTexelOffset) ||
            (limits->minTexelGatherOffset > ddev->limits.minTexelGatherOffset) ||
            (limits->maxTexelGatherOffset > ddev->limits.maxTexelGatherOffset) ||
            (limits->minInterpolationOffset > ddev->limits.minInterpolationOffset) ||
            (limits->maxInterpolationOffset > ddev->limits.maxInterpolationOffset) ||
            (limits->subPixelInterpolationOffsetBits > ddev->limits.subPixelInterpolationOffsetBits) ||
            (limits->maxCanvasWhd.w > ddev->limits.maxCanvasWhd.w) ||
            (limits->maxCanvasWhd.h > ddev->limits.maxCanvasWhd.h) ||
            (limits->maxCanvasWhd.d > ddev->limits.maxCanvasWhd.d) ||
            (limits->canvasColorSampleCnts > ddev->limits.canvasColorSampleCnts) ||
            (limits->canvasDepthSampleCnts > ddev->limits.canvasDepthSampleCnts) ||
            (limits->canvasStencilSampleCnts > ddev->limits.canvasStencilSampleCnts) ||
            (limits->canvasNoAttachmentsSampleCnts > ddev->limits.canvasNoAttachmentsSampleCnts) ||
            (limits->maxColorAttachments > ddev->limits.maxColorAttachments) ||
            (limits->sampledRasterColorSampleCnts > ddev->limits.sampledRasterColorSampleCnts) ||
            (limits->sampledRasterIntegerSampleCnts > ddev->limits.sampledRasterIntegerSampleCnts) ||
            (limits->sampledRasterDepthSampleCnts > ddev->limits.sampledRasterDepthSampleCnts) ||
            (limits->sampledRasterStencilSampleCnts > ddev->limits.sampledRasterStencilSampleCnts) ||
            (limits->storageRasterSampleCnts > ddev->limits.storageRasterSampleCnts) ||
            (limits->maxSampleMaskWords > ddev->limits.maxSampleMaskWords) ||
            (limits->timestampComputeAndGraphics > ddev->limits.timestampComputeAndGraphics) ||
            (limits->timestampPeriod > ddev->limits.timestampPeriod) ||
            (limits->maxClipDistances > ddev->limits.maxClipDistances) ||
            (limits->maxCullDistances > ddev->limits.maxCullDistances) ||
            (limits->maxCombinedClipAndCullDistances > ddev->limits.maxCombinedClipAndCullDistances) ||
            (limits->discreteQueuePriorities > ddev->limits.discreteQueuePriorities) ||
            (limits->pointSizRange[0] > ddev->limits.pointSizRange[0]) ||
            (limits->pointSizRange[1] > ddev->limits.pointSizRange[1]) ||
            (limits->lineWidthRange[0] > ddev->limits.lineWidthRange[0]) ||
            (limits->lineWidthRange[1] > ddev->limits.lineWidthRange[1]) ||
            (limits->pointSizGranularity > ddev->limits.pointSizGranularity) ||
            (limits->lineWidthGranularity > ddev->limits.lineWidthGranularity) ||
            (limits->strictLines > ddev->limits.strictLines) ||
            (limits->standardSampleLocations > ddev->limits.standardSampleLocations) ||
            (limits->optimalBufCopyOffsetAlign > ddev->limits.optimalBufCopyOffsetAlign) ||
            (limits->optimalBufCopyRowPitchAlign > ddev->limits.optimalBufCopyRowPitchAlign) ||
            (limits->nonCoherentAtomSiz > ddev->limits.nonCoherentAtomSiz))
        {
            rslt = FALSE;
        }
    }
    return rslt;
}

_AVX afxError AvxDescribeDeviceFormats(afxDrawDevice ddev, afxUnit cnt, avxFormat const formats[], avxFormatDescription descs[])
{
    afxError err = AFX_ERR_NONE;
#if AVX_VALIDATION_ENABLED
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    // @formats must be an array of valid avxFormat values.
    AFX_ASSERT(formats);
    // @descs must be a valid pointer to an array of avxFormatDescription structure.
    AFX_ASSERT(descs);
    // @cnt must be greater or equal to 1.
    AFX_ASSERT(cnt);
#endif

    for (afxUnit i = 0; i < cnt; i++)
    {
        AFX_ASSERT_RANGE(avxFormat_TOTAL, 0, formats[i]);
        avxFormatDescription const* pfd = &ddev->pfds[formats[i]];
        descs[i] = *pfd;
    }
    return err;
}

_AVX afxError _AvxDdevDtorCb(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    AfxDeregisterChainedClasses(&ddev->dev.classes);

    AfxCallDevice(&ddev->dev, 2, NIL); // terminate

    AFX_ASSERT(!ddev->idd);
    AfxDeregisterChainedClasses(&ddev->dev.classes);

    return err;
}

_AVX afxError _AvxDdevCtorCb(afxDrawDevice ddev, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    // @ddev must be a valid afxDrawDevice handle.
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    _avxDrawDeviceRegistration const* info = AFX_CAST(_avxDrawDeviceRegistration const*, args[1]) + invokeNo;
    AFX_ASSERT(info);

    if (_AFX_DEV_CLASS_CONFIG.ctor(&ddev->dev, (void*[]) { icd, (void*)&info->dev }, 0))
    {
        AfxThrowError();
        return err;
    }

    ddev->nonRhCoords = FALSE;
    ddev->clipSpaceDepth = avxClipSpaceDepth_ZERO_TO_ONE;

    ddev->idd = NIL;

    ddev->limits = info->limits;
    ddev->features = info->features;
    ddev->nonRhCoords = info->nonRhCoords;
    ddev->clipSpaceDepth = info->clipSpaceDepth;

    ddev->pfds = &_AvxStdPfds[0];

    ddev->acceleration = info->acceleration;
    ddev->capabilities = info->capabilities;
    
    ddev->maxQueCnt = info->maxQueCnt;
    ddev->minQueCnt = info->minQueCnt;

    AfxMakeString128(&ddev->desc, NIL);
    AfxMakeString8(&ddev->urn, NIL);
            
    if (AfxCallDevice(&ddev->dev, afxFcc_DSYS, NIL))
        AfxThrowError();

    if (err)
    {
        AfxDeregisterChainedClasses(&ddev->dev.classes);
    }

    return err;
}

_AVX afxClassConfig const _AVX_DDEV_CLASS_CONFIG =
{
    .fcc = afxFcc_DDEV,
    .name = "DrawDevice",
    .desc = "Draw Device Driver Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawDevice)),
    .ctor = (void*)_AvxDdevCtorCb,
    .dtor = (void*)_AvxDdevDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// IMPLEMENTATION DISCOVERY                                                   //
////////////////////////////////////////////////////////////////////////////////

_AVX afxUnit AvxEnumerateDrawDevices(afxUnit icd, afxUnit first, afxUnit cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxIcdGetDdevClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DDEV);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
        // devices must be an array of valid afxDrawDevice handles.
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, rslt, devices);
        break;
    }
    return rslt;
}

_AVX afxUnit AvxInvokeDrawDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawDevice), afxUnit cnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxIcdGetDdevClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DDEV);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_AVX afxUnit AvxEvokeDrawDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawDevice), afxUnit cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(devices);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxIcdGetDdevClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DDEV);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)devices);
        // @devices must be an array of valid afxDrawDevice handles.
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, rslt, devices);
        break;
    }
    return rslt;
}

_AVX afxUnit AvxChooseDrawDevices(afxUnit icd, afxDrawFeatures const* features, afxDrawLimits const* limits, afxDrawPortInfo const* caps, afxUnit maxCnt, afxUnit ddevId[])
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(limits);
    //AFX_ASSERT(features);
    afxUnit rslt = 0;
    afxModule mdle;
    while (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxIcdGetDdevClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DDEV);

        afxDrawDevice ddev;
        for (afxUnit i = 0; TRUE; i++)
        {
            if (!AfxEnumerateObjects(cls, i, 1, (afxObject*)&ddev)) break;
            AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

            if (features || limits)
                if (!AvxIsDrawDeviceAcceptable(ddev, features, limits))
                    continue;

            if (caps)
            {
                if ((ddev->capabilities & caps->capabilities) != caps->capabilities)
                    continue;

                if ((ddev->acceleration & caps->acceleration) != caps->acceleration)
                    continue;

                if (ddev->minQueCnt < caps->minQueCnt)
                    continue;

                if (ddev->maxQueCnt < caps->maxQueCnt)
                    continue;
            }

            {
                ddevId[rslt] = i;
                ++rslt;

                if (maxCnt > rslt)
                    break;
            }
        }
        break;
    }
    return rslt;
}

_AVX afxError _AvxRegisterDrawDevices(afxModule icd, afxUnit cnt, _avxDrawDeviceRegistration const infos[], afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(devices);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    if (!AfxTestModule(icd, afxModuleFlag_ICD | afxModuleFlag_AVX))
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AvxIcdGetDdevClass(icd);
    AFX_ASSERT_CLASS(cls, afxFcc_DDEV);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)devices, (void const*[]) { icd, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_DDEV, cnt, devices);

    }
    return err;
}
