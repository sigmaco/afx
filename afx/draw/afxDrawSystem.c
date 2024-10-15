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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_INPUT_C
#include "../dev/AvxImplKit.h"

_AVX afxString const targaSignature = AFX_STRING(
    "           :::::::::::     :::     :::::::::   ::::::::      :::               \n"
    "               :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:             \n"
    "               +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+            \n"
    "               +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:           \n"
    "               +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+           \n"
    "               #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#           \n"
    "               ###     ###     ### ###    ###  ########  ###     ###           \n"
    "                                                                               \n"
    "              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                           (c) 2017 SIGMA FEDERATION                           \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

//extern afxChain* _AfxGetSystemClassChain(void);
AFX afxChain* _AfxGetSystemClassChain(void);

_AVX afxClass const* AvxGetDrawDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &sys->avx.ddevCls;
    AfxAssertClass(cls, afxFcc_DDEV);
    return cls;
}

_AVX afxReal64 AfxFindPhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(screenWidth);
    AfxAssert(screenHeight);
    afxReal64 ratio, div = (afxReal64)screenWidth / (afxReal64)screenHeight;

    if (div <= 1.4)
        ratio = 1.33;
    else if (div <= 1.6)
        ratio = 1.5599999;
    else
        ratio = 1.78;

    return ratio;
}

_AVX afxReal64 AfxFindAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam)
{
    return AfxSin(fovY * 0.5) / AfxCos(fovY * 0.5) * errInPixels * distanceFromCam / ((afxReal64)vpHeightInPixels * 0.5);
}

////////////////////////////////////////////////////////////////////////////////
// DRAW DEVICE HANDLING                                                       //
////////////////////////////////////////////////////////////////////////////////

_AVX afxClass const* _AvxGetDrawThreadClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    /// ddev must be a valid afxDrawDevice handle.
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass const* cls = &ddev->dthrCls;
    AfxAssertClass(cls, afxFcc_DTHR);
    return cls;
}

_AVX afxClass const* _AvxGetDrawQueueClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    /// ddev must be a valid afxDrawDevice handle.
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass const* cls = &ddev->dqueCls;
    AfxAssertClass(cls, afxFcc_DQUE);
    return cls;
}

_AVX afxClass const* _AvxGetDrawBridgeClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    /// ddev must be a valid afxDrawDevice handle.
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass const* cls = &ddev->dexuCls;
    AfxAssertClass(cls, afxFcc_DEXU);
    return cls;
}

_AVX afxClass const* AfxGetDrawContextClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass const* cls = &ddev->dctxCls;
    AfxAssertClass(cls, afxFcc_DCTX);
    return cls;
}

_AVX afxClass const* AvxGetDrawOutputClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass const* cls = &ddev->doutCls;
    AfxAssertClass(cls, afxFcc_DOUT);
    return cls;
}

_AVX afxClass const* AvxGetDrawInputClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass const* cls = &ddev->dinCls;
    AfxAssertClass(cls, afxFcc_DIN);
    return cls;
}

_AVX afxBool AfxIsDrawDevicePrompt(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->dev.serving;
}

_AVX void* AfxGetDrawDeviceIdd(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->idd;
}

_AVX afxNat AfxCountDrawPorts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->portCnt;
}

_AVX void AfxGetDrawPortCapabilities(afxDrawDevice ddev, afxNat portId, afxDrawPortCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertRange(ddev->portCnt, portId, 1);
    AfxAssert(caps);
    *caps = ddev->portCaps[portId];
}

_AVX void AfxGetDrawDeviceFeatures(afxDrawDevice ddev, afxDrawFeatures* features)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(features);
    *features = ddev->features;
}

_AVX void AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(limits);
    *limits = ddev->limits;
}

_AVX afxBool AfxIsDrawDeviceAcceptable(afxDrawDevice ddev, afxDrawFeatures const* features, afxDrawLimits const* limits)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(limits);
    AfxAssert(features);
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
            (features->shaderUniformBufferArrayDynIndexing && !ddev->features.shaderUniformBufferArrayDynIndexing) ||
            (features->shaderSampledRasterArrayDynIndexing && !ddev->features.shaderSampledRasterArrayDynIndexing) ||
            (features->shaderStorageBufferArrayDynIndexing && !ddev->features.shaderStorageBufferArrayDynIndexing) ||
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
            (limits->maxRasterArrayLayers > ddev->limits.maxRasterArrayLayers) ||
            (limits->maxTexelBufElements > ddev->limits.maxTexelBufElements) ||
            (limits->maxUniformBufRange > ddev->limits.maxUniformBufRange) ||
            (limits->maxStorageBufRange > ddev->limits.maxStorageBufRange) ||
            (limits->maxPushConstantsSiz > ddev->limits.maxPushConstantsSiz) ||
            (limits->maxMemAllocCnt > ddev->limits.maxMemAllocCnt) ||
            (limits->maxSamplerAllocCnt > ddev->limits.maxSamplerAllocCnt) ||
            (limits->bufferRasterGranularity > ddev->limits.bufferRasterGranularity) ||
            (limits->sparseAddrSpaceSiz > ddev->limits.sparseAddrSpaceSiz) ||
            (limits->maxBoundDescriptorSets > ddev->limits.maxBoundDescriptorSets) ||
            (limits->maxPerStageDescriptorSamplers > ddev->limits.maxPerStageDescriptorSamplers) ||
            (limits->maxPerStageDescriptorUniformBuffers > ddev->limits.maxPerStageDescriptorUniformBuffers) ||
            (limits->maxPerStageDescriptorStorageBuffers > ddev->limits.maxPerStageDescriptorStorageBuffers) ||
            (limits->maxPerStageDescriptorSampledImages > ddev->limits.maxPerStageDescriptorSampledImages) ||
            (limits->maxPerStageDescriptorStorageImages > ddev->limits.maxPerStageDescriptorStorageImages) ||
            (limits->maxPerStageDescriptorInputAttachments > ddev->limits.maxPerStageDescriptorInputAttachments) ||
            (limits->maxPerStageResources > ddev->limits.maxPerStageResources) ||
            (limits->maxDescriptorSetSamplers > ddev->limits.maxDescriptorSetSamplers) ||
            (limits->maxDescriptorSetUniformBuffers > ddev->limits.maxDescriptorSetUniformBuffers) ||
            (limits->maxDescriptorSetUniformBuffersDynamic > ddev->limits.maxDescriptorSetUniformBuffersDynamic) ||
            (limits->maxDescriptorSetStorageBuffers > ddev->limits.maxDescriptorSetStorageBuffers) ||
            (limits->maxDescriptorSetStorageBuffersDynamic > ddev->limits.maxDescriptorSetStorageBuffersDynamic) ||
            (limits->maxDescriptorSetSampledImages > ddev->limits.maxDescriptorSetSampledImages) ||
            (limits->maxDescriptorSetStorageImages > ddev->limits.maxDescriptorSetStorageImages) ||
            (limits->maxDescriptorSetInputAttachments > ddev->limits.maxDescriptorSetInputAttachments) ||
            (limits->maxVtxInputAttributes > ddev->limits.maxVtxInputAttributes) ||
            (limits->maxVtxInputBindings > ddev->limits.maxVtxInputBindings) ||
            (limits->maxVtxInputAttributeOffset > ddev->limits.maxVtxInputAttributeOffset) ||
            (limits->maxVtxInputBindingStride > ddev->limits.maxVtxInputBindingStride) ||
            (limits->maxVtxOutputCompos > ddev->limits.maxVtxOutputCompos) ||
            (limits->maxTessGenLvl > ddev->limits.maxTessGenLvl) ||
            (limits->maxTessPatchSiz > ddev->limits.maxTessPatchSiz) ||
            (limits->maxTessCtrlPerVtxInComps > ddev->limits.maxTessCtrlPerVtxInComps) ||
            (limits->maxTessCtrlPerVtxOutComps > ddev->limits.maxTessCtrlPerVtxOutComps) ||
            (limits->maxTessCtrlPerPatchOutComps > ddev->limits.maxTessCtrlPerPatchOutComps) ||
            (limits->maxTessCtrlTotalOutComps > ddev->limits.maxTessCtrlTotalOutComps) ||
            (limits->maxTessEvalInComps > ddev->limits.maxTessEvalInComps) ||
            (limits->maxTessEvalOutComps > ddev->limits.maxTessEvalOutComps) ||
            (limits->maxPrimShaderInvocations > ddev->limits.maxPrimShaderInvocations) ||
            (limits->maxPrimInComps > ddev->limits.maxPrimInComps) ||
            (limits->maxPrimOutComps > ddev->limits.maxPrimOutComps) ||
            (limits->maxPrimOutVertices > ddev->limits.maxPrimOutVertices) ||
            (limits->maxPrimTotalOutComps > ddev->limits.maxPrimTotalOutComps) ||
            (limits->maxFragInComps > ddev->limits.maxFragInComps) ||
            (limits->maxFragOutAttachments > ddev->limits.maxFragOutAttachments) ||
            (limits->maxFragDualSrcAttachments > ddev->limits.maxFragDualSrcAttachments) ||
            (limits->maxFragCombinedOutputResources > ddev->limits.maxFragCombinedOutputResources) ||
            (limits->maxComputeSharedMemorySiz > ddev->limits.maxComputeSharedMemorySiz) ||
            (limits->maxComputeWorkGroupCnt[0] > ddev->limits.maxComputeWorkGroupCnt[0]) ||
            (limits->maxComputeWorkGroupCnt[1] > ddev->limits.maxComputeWorkGroupCnt[1]) ||
            (limits->maxComputeWorkGroupCnt[2] > ddev->limits.maxComputeWorkGroupCnt[2]) ||
            (limits->maxComputeWorkGroupInvocations > ddev->limits.maxComputeWorkGroupInvocations) ||
            (limits->maxComputeWorkGroupSiz[0] > ddev->limits.maxComputeWorkGroupSiz[0]) ||
            (limits->maxComputeWorkGroupSiz[1] > ddev->limits.maxComputeWorkGroupSiz[1]) ||
            (limits->maxComputeWorkGroupSiz[2] > ddev->limits.maxComputeWorkGroupSiz[2]) ||
            (limits->subPixelPrecisionBits > ddev->limits.subPixelPrecisionBits) ||
            (limits->subTexelPrecisionBits > ddev->limits.subTexelPrecisionBits) ||
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
            (limits->minMemMapAlign > ddev->limits.minMemMapAlign) ||
            (limits->minTexelBufOffsetAlign > ddev->limits.minTexelBufOffsetAlign) ||
            (limits->minUniformBufOffsetAlign > ddev->limits.minUniformBufOffsetAlign) ||
            (limits->minStorageBufOffsetAlign > ddev->limits.minStorageBufOffsetAlign) ||
            (limits->minTexelOffset > ddev->limits.minTexelOffset) ||
            (limits->maxTexelOffset > ddev->limits.maxTexelOffset) ||
            (limits->minTexelGatherOffset > ddev->limits.minTexelGatherOffset) ||
            (limits->maxTexelGatherOffset > ddev->limits.maxTexelGatherOffset) ||
            (limits->minInterpolationOffset > ddev->limits.minInterpolationOffset) ||
            (limits->maxInterpolationOffset > ddev->limits.maxInterpolationOffset) ||
            (limits->subPixelInterpolationOffsetBits > ddev->limits.subPixelInterpolationOffsetBits) ||
            (limits->maxCanvasWhd[0] > ddev->limits.maxCanvasWhd[0]) ||
            (limits->maxCanvasWhd[1] > ddev->limits.maxCanvasWhd[1]) ||
            (limits->maxCanvasWhd[2] > ddev->limits.maxCanvasWhd[2]) ||
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

_AVX afxNat AfxInvokeDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetDrawContextClass(ddev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AvxGetDrawOutputClass(ddev);
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AvxGetDrawInputClass(ddev);
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEvokeDrawContexts(afxDrawDevice ddev, afxBool(*f)(afxDrawContext, void*), void* udd, afxNat first, afxNat cnt, afxDrawContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(contexts);
    AfxAssert(f);
    AfxAssert(cnt);
    afxClass const* cls = AfxGetDrawContextClass(ddev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxEvokeClassInstances(cls, (void*)f, udd, first, cnt, (afxObject*)contexts);
}

_AVX afxNat AfxEvokeDrawOutputs(afxDrawDevice ddev, afxBool(*f)(afxDrawOutput, void*), void* udd, afxNat first, afxNat cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(outputs);
    afxClass const* cls = AvxGetDrawOutputClass(ddev);
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxEvokeClassInstances(cls, (void*)f, udd, first, cnt, (afxObject*)outputs);
}

_AVX afxNat AfxEvokeDrawInputs(afxDrawDevice ddev, afxBool(*f)(afxDrawInput, void*), void* udd, afxNat first, afxNat cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(inputs);
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AvxGetDrawInputClass(ddev);
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxEvokeClassInstances(cls, (void*)f, udd, first, cnt, (afxObject*)inputs);
}

_AVX afxNat AfxEnumerateDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(contexts);
    AfxAssert(cnt);
    afxClass const* cls = AfxGetDrawContextClass(ddev);
    AfxAssertClass(cls, afxFcc_DCTX);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)contexts);
}

_AVX afxNat AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(cnt);
    AfxAssert(outputs);
    afxClass const* cls = AvxGetDrawOutputClass(ddev);
    AfxAssertClass(cls, afxFcc_DOUT);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)outputs);
}

_AVX afxNat AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(inputs);
    AfxAssert(cnt);
    afxClass const* cls = AvxGetDrawInputClass(ddev);
    AfxAssertClass(cls, afxFcc_DIN);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)inputs);
}

_AVX afxError _AvxDdevDtorCb(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    AfxDeregisterChainedClasses(&ddev->dev.classes);

    AfxCallDevice(&ddev->dev, 2, NIL); // terminate

    AfxAssert(!ddev->idd);
    AfxDeregisterChainedClasses(&ddev->dev.classes);

    AfxCleanUpMutex(&ddev->relinkedCndMtx);
    AfxCleanUpCondition(&ddev->relinkedCnd);

    afxObjectStash stashes[] =
    {
        {
            .cnt = ddev->portCnt,
            .siz = sizeof(ddev->portCaps[0]),
            .var = (void**)&ddev->portCaps
        }
    };
    AfxDeallocateInstanceData(ddev, AFX_COUNTOF(stashes), stashes);

    return err;
}

_AVX afxError _AvxDdevCtorCb(afxDrawDevice ddev, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxModule icd = args[0];
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    afxDrawDeviceInfo const* info = (afxDrawDeviceInfo const *)(args[1]) + invokeNo;
    AfxAssert(info);

    if (_AfxDevBaseImplementation.ctor(&ddev->dev, (void*[]){ icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        AfxSetUpCondition(&ddev->relinkedCnd);
        AfxSetUpMutex(&ddev->relinkedCndMtx, AFX_MTX_PLAIN);

        ddev->clipSpace = AVX_CLIP_SPACE_QWADRO;

        ddev->idd = NIL;

        ddev->relinkDinCb = info->relinkDinCb;
        ddev->relinkDoutCb = info->relinkDoutCb;

        ddev->limits = info->limits;
        ddev->features = info->features;
        ddev->clipSpace = info->clipSpace;

        ddev->portCnt = info->portCnt;

        afxClassConfig clsCfg;

        clsCfg = info->dqueClsCfg ? *info->dqueClsCfg : _AvxDqueStdImplementation;
        AfxRegisterClass(&ddev->dqueCls, NIL, &ddev->dev.classes, &clsCfg);

        clsCfg = info->ddgeClsCfg ? *info->ddgeClsCfg : _AvxDexuStdImplementation;
        AfxRegisterClass(&ddev->dexuCls, NIL, &ddev->dev.classes, &clsCfg);

        clsCfg = info->dctxClsCfg ? *info->dctxClsCfg : _AvxDctxStdImplementation;
        AfxRegisterClass(&ddev->dctxCls, NIL, &ddev->dev.classes, &clsCfg); // require dexu

        clsCfg = info->doutClsCfg ? *info->doutClsCfg : _AvxDoutStdImplementation;
        AfxRegisterClass(&ddev->doutCls, NIL, &ddev->dev.classes, &clsCfg); // require ddev, dout

        clsCfg = info->dinClsCfg ? *info->dinClsCfg : _AvxDinStdImplementation;
        AfxRegisterClass(&ddev->dinCls, NIL, &ddev->dev.classes, &clsCfg); // require ddev, din

        afxObjectStash stashes[] =
        {
            {
                .cnt = ddev->portCnt,
                .siz = sizeof(ddev->portCaps[0]),
                .var = (void**)&ddev->portCaps
            }
        };

        if (AfxAllocateInstanceData(ddev, AFX_COUNTOF(stashes), stashes)) AfxThrowError();
        else
        {
            if (!ddev->portCnt) AfxThrowError();
            else
            {
                AfxAssert(ddev->portCaps);

                for (afxNat i = 0; i < ddev->portCnt; i++)
                {
                    ddev->portCaps[i] = info->portCaps[i];
                }

                if (AfxCallDevice(&ddev->dev, afxFcc_DSYS, NIL)) AfxThrowError();
                else
                {
                    if (err)
                    {
                        AfxDeregisterChainedClasses(&ddev->dev.classes);
                    }
                }
            }

            if (err)
                AfxDeallocateInstanceData(ddev, AFX_COUNTOF(stashes), stashes);
        }
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////
// DRAW DEVICE DISCOVERY                                                      //
////////////////////////////////////////////////////////////////////////////////

_AVX afxNat AfxChooseDrawDevices(afxDrawFeatures const* features, afxDrawLimits const* limits, afxNat maxCnt, afxNat ddevId[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(limits);
    AfxAssert(features);
    afxNat rslt = 0;

    afxNat i = 0;
    afxDrawDevice ddev;
    while (AfxEnumerateDrawDevices(i, 1, &ddev))
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (AfxIsDrawDeviceAcceptable(ddev, features, limits))
        {
            ddevId[rslt] = i;
            ++rslt;

            if (maxCnt > rslt)
                break;
        }
        i++;
    }
    return rslt;
}

_AVX afxNat AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AvxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEvokeDrawDevices(afxBool(*f)(afxDrawDevice, void*), void* udd, afxNat first, afxNat cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxClass const* cls = AvxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEvokeClassInstances(cls, (void*)f, udd, first, cnt, (afxObject*)devices);
}

_AVX afxNat AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxClass const* cls = AvxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)devices);
}

_AVX afxBool AfxGetDrawDevice(afxNat ddevId, afxDrawDevice* device)
{
    afxError err = AFX_ERR_NONE;
    afxDrawDevice ddev = NIL;
    while (AfxEnumerateDrawDevices(ddevId, 1, &ddev))
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);
        AfxAssert(device);
        *device = ddev;
        break;
    }
    return !!ddev;
}

_AVX afxError _AvxRegisterDrawDevices(afxModule icd, afxNat cnt, afxDrawDeviceInfo const infos[], afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_MDLE);
    AfxAssert(devices);

    afxClass* cls = (afxClass*)AvxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);

    AfxEnterSlockExclusive(&cls->poolLock);

    if (cls->pool.totalUsedCnt >= cls->maxInstCnt) AfxThrowError();
    else
    {
        if (_AfxAllocateObjects(cls, cnt, (afxObject*)devices)) AfxThrowError();
        else
        {   
            if (_AfxConstructObjects(cls, cnt, (afxObject*)devices, (void*[]) { icd, (void*)infos }))
            {
                AfxThrowError();
            }

            if (!err)
            {
                AfxAssertObjects(cnt, devices, afxFcc_DDEV);
            }
            else
            {
                _AfxDeallocateObjects(cls, cnt, (afxObject*)devices);
            }
        }
    }

    AfxExitSlockExclusive(&cls->poolLock);

    return err;
}

_AVX afxError afxScmHook(afxModule mdle, afxManifest const* ini)
{
    afxError err = NIL;
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    afxClassConfig const _AvxDdevClsCfg =
    {
        .fcc = afxFcc_DDEV,
        .name = "DrawDevice",
        .desc = "Draw Device Driver Interface",
        .fixedSiz = sizeof(AFX_OBJECT(afxDrawDevice)),
        .ctor = (void*)_AvxDdevCtorCb,
        .dtor = (void*)_AvxDdevDtorCb
    };
    AfxRegisterClass(&sys->avx.ddevCls, (afxClass*)AfxGetDeviceClass(), _AfxGetSystemClassChain(), &_AvxDdevClsCfg); // require base*

    if (!err)
    {
        //sys->avx.ready = TRUE;
    }

    return err;
}
