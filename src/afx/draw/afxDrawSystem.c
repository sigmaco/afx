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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

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

#include "dev/AvxDevKit.h"

_AVX AFX_OBJECT(afxDrawSystem) TheDrawSystem = { 0 };
//extern afxChain* _AfxGetSystemClassChain(void);
extern afxClassConfig const _AvxDoutClsCfg;
extern afxClassConfig const _AvxDinClsCfg;

_AVX afxString const sgl2Signature = AFX_STRING(
    "           :::     :::::::::   ::::::::  :::              :::  ::::::::        \n"
    "          :+:      :+:    :+: :+:    :+: :+:             :+:  :+:    :+:       \n"
    "         +:+ +:+   +:+    +:+ +:+        +:+            +:+         +:+        \n"
    "        +#+  +:+   +#+    +:+ :#:        +#+           +#+        +#+          \n"
    "       +#+#+#+#+#+ +#+    +#+ +#+   +#+# +#+          +#+       +#+            \n"
    "             #+#   #+#    #+# #+#    #+# #+#         #+#       #+#             \n"
    "             ###   #########   ########  ########## ###       ##########       \n"
    "                                                                               \n"
    "              Q W A D R O   E X E C U T I O N   E C O S Y S T E M              \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                   (c) 2017 SIGMA, Engineering In Technology                   \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

_AVX afxClass const* AfxGetDrawDeviceClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &TheDrawSystem.ddevCls;
    AfxAssertClass(cls, afxFcc_DDEV);
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

_AVX void* AfxGetDrawDeviceIdd(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->idd;
}

_AVX afxClass const* AfxGetDrawInputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &TheDrawSystem.dinCls;
    AfxAssertClass(cls, afxFcc_DIN);
    return cls;
}

_AVX afxClass const* AfxGetDrawOutputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &TheDrawSystem.doutCls;
    AfxAssertClass(cls, afxFcc_DOUT);
    return cls;
}

_AVX afxClass const* _AvxGetDrawBridgeClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    /// ddev must be a valid afxDrawDevice handle.
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxClass const* cls = &ddev->ddgeCls;
    AfxAssertClass(cls, afxFcc_DDGE);
    return cls;
}

_AVX afxBool AfxDrawDeviceIsRunning(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->dev.serving;
}

struct _DdrvCurateProxyCb
{
    afxDrawDevice ddev;
    union
    {
        afxBool(*f)(void*, void*);
        afxBool(*fdctx)(afxDrawContext, void*);
        afxBool(*fdout)(afxDrawOutput, void*);
        afxBool(*fdin)(afxDrawInput, void*);
    };
    void *udd;
};

_AVX afxBool _AvxDdrvCurateProxyDctxCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = (afxDrawContext)obj;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(dctx) != proxy->ddev || proxy->fdctx(dctx, proxy->udd); // don't interrupt curation;
}

_AVX afxBool _AvxDdrvCurateProxyDoutCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawOutput dout = (afxDrawOutput)obj;
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetDrawOutputDevice(dout) != proxy->ddev || proxy->fdout(dout, proxy->udd); // don't interrupt curation;
}

_AVX afxBool _AvxDdrvCurateProxyDinCb(afxObject obj, void *udd)
{
    afxError err = AFX_ERR_NONE;
    afxDrawInput din = (afxDrawInput)obj;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    struct _DdrvCurateProxyCb const *proxy = udd;
    return AfxGetObjectProvider(din) != proxy->ddev || proxy->fdin(din, proxy->udd); // don't interrupt curation;
}

_AVX afxNat AfxCountDrawPorts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->portCnt;
}

_AVX void AfxGetDrawPortCapabilities(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertRange(ddev->portCnt, portIdx, 1);
    AfxAssert(caps);
    *caps = ddev->portCaps[portIdx];
}

_AVX void AfxGetDrawDeviceCapabilities(afxDrawDevice ddev, afxNat subset, afxDrawDeviceCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(ddev->caps);
    AfxAssert(caps);
    *caps = *ddev->caps;
}

_AVX void AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxNat subset, afxDrawDeviceLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(ddev->limits);
    AfxAssert(limits);
    *limits = *ddev->limits;
}

_AVX afxBool AfxIsDrawDeviceAcceptable(afxDrawDevice ddev, afxDrawDeviceCaps const* caps, afxDrawDeviceLimits const* limits)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(limits);
    AfxAssert(caps);
    afxBool rslt = TRUE;

    if (caps)
    {
        if ((caps->robustBufAccess && !ddev->caps->robustBufAccess) ||
            (caps->fullDrawIdxUint32 && !ddev->caps->fullDrawIdxUint32) ||
            (caps->rasterCubeArray && !ddev->caps->rasterCubeArray) ||
            (caps->independentBlend && !ddev->caps->independentBlend) ||
            (caps->primShader && !ddev->caps->primShader) ||
            (caps->tessShader && !ddev->caps->tessShader) ||
            (caps->sampleRateShading && !ddev->caps->sampleRateShading) ||
            (caps->dualSrcBlend && !ddev->caps->dualSrcBlend) ||
            (caps->logicOp && !ddev->caps->logicOp) ||
            (caps->multiDrawIndirect && !ddev->caps->multiDrawIndirect) ||
            (caps->drawIndirectFirstInst && !ddev->caps->drawIndirectFirstInst) ||
            (caps->depthClamp && !ddev->caps->depthClamp) ||
            (caps->depthBiasClamp && !ddev->caps->depthBiasClamp) ||
            (caps->fillModeNonSolid && !ddev->caps->fillModeNonSolid) ||
            (caps->depthBounds && !ddev->caps->depthBounds) ||
            (caps->wideLines && !ddev->caps->wideLines) ||
            (caps->largePoints && !ddev->caps->largePoints) ||
            (caps->alphaToOne && !ddev->caps->alphaToOne) ||
            (caps->multiViewport && !ddev->caps->multiViewport) ||
            (caps->samplerAnisotropy && !ddev->caps->samplerAnisotropy) ||
            (caps->etc2 && !ddev->caps->etc2) ||
            (caps->astc_LDR && !ddev->caps->astc_LDR) ||
            (caps->dxt && !ddev->caps->dxt) ||
            (caps->occlusionQueryPrecise && !ddev->caps->occlusionQueryPrecise) ||
            (caps->pipelineStatsQuery && !ddev->caps->pipelineStatsQuery) ||
            (caps->vtxPipelineStoresAndAtomics && !ddev->caps->vtxPipelineStoresAndAtomics) ||
            (caps->fragStoresAndAtomics && !ddev->caps->fragStoresAndAtomics) ||
            (caps->shaderTessAndPrimPointSiz && !ddev->caps->shaderTessAndPrimPointSiz) ||
            (caps->shaderRasterGatherExt && !ddev->caps->shaderRasterGatherExt) ||
            (caps->shaderStorageRasterExtFmts && !ddev->caps->shaderStorageRasterExtFmts) ||
            (caps->shaderStorageRasterMultisample && !ddev->caps->shaderStorageRasterMultisample) ||
            (caps->shaderStorageRasterReadWithoutFmt && !ddev->caps->shaderStorageRasterReadWithoutFmt) ||
            (caps->shaderStorageRasterWriteWithoutFmt && !ddev->caps->shaderStorageRasterWriteWithoutFmt) ||
            (caps->shaderUniformBufferArrayDynIndexing && !ddev->caps->shaderUniformBufferArrayDynIndexing) ||
            (caps->shaderSampledRasterArrayDynIndexing && !ddev->caps->shaderSampledRasterArrayDynIndexing) ||
            (caps->shaderStorageBufferArrayDynIndexing && !ddev->caps->shaderStorageBufferArrayDynIndexing) ||
            (caps->shaderStorageImageArrayDynIndexing && !ddev->caps->shaderStorageImageArrayDynIndexing) ||
            (caps->shaderClipDist && !ddev->caps->shaderClipDist) ||
            (caps->shaderCullDist && !ddev->caps->shaderCullDist) ||
            (caps->shaderFloat64 && !ddev->caps->shaderFloat64) ||
            (caps->shaderInt64 && !ddev->caps->shaderInt64) ||
            (caps->shaderInt16 && !ddev->caps->shaderInt16) ||
            (caps->shaderRsrcResidency && !ddev->caps->shaderRsrcResidency) ||
            (caps->shaderRsrcMinLod && !ddev->caps->shaderRsrcMinLod) ||
            (caps->sparseBinding && !ddev->caps->sparseBinding) ||
            (caps->sparseResidencyBuffer && !ddev->caps->sparseResidencyBuffer) ||
            (caps->sparseResidencyRaster2D && !ddev->caps->sparseResidencyRaster2D) ||
            (caps->sparseResidencyRaster3D && !ddev->caps->sparseResidencyRaster3D) ||
            (caps->sparseResidency2Samples && !ddev->caps->sparseResidency2Samples) ||
            (caps->sparseResidency4Samples && !ddev->caps->sparseResidency4Samples) ||
            (caps->sparseResidency8Samples && !ddev->caps->sparseResidency8Samples) ||
            (caps->sparseResidency16Samples && !ddev->caps->sparseResidency16Samples) ||
            (caps->sparseResidencyAliased && !ddev->caps->sparseResidencyAliased) ||
            (caps->variableMultisampleRate && !ddev->caps->variableMultisampleRate) ||
            (caps->inheritedQueries && !ddev->caps->inheritedQueries))
        {
            rslt = FALSE;
        }
    }

    if (limits)
    {
        if ((limits->maxRasterDim1D > ddev->limits->maxRasterDim1D) ||
            (limits->maxRasterDim2D > ddev->limits->maxRasterDim2D) ||
            (limits->maxRasterDim3D > ddev->limits->maxRasterDim3D) ||
            (limits->maxRasterDimCube > ddev->limits->maxRasterDimCube) ||
            (limits->maxRasterArrayLayers > ddev->limits->maxRasterArrayLayers) ||
            (limits->maxTexelBufElements > ddev->limits->maxTexelBufElements) ||
            (limits->maxUniformBufRange > ddev->limits->maxUniformBufRange) ||
            (limits->maxStorageBufRange > ddev->limits->maxStorageBufRange) ||
            (limits->maxPushConstantsSiz > ddev->limits->maxPushConstantsSiz) ||
            (limits->maxMemAllocCnt > ddev->limits->maxMemAllocCnt) ||
            (limits->maxSamplerAllocCnt > ddev->limits->maxSamplerAllocCnt) ||
            (limits->bufferRasterGranularity > ddev->limits->bufferRasterGranularity) ||
            (limits->sparseAddrSpaceSiz > ddev->limits->sparseAddrSpaceSiz) ||
            (limits->maxBoundDescriptorSets > ddev->limits->maxBoundDescriptorSets) ||
            (limits->maxPerStageDescriptorSamplers > ddev->limits->maxPerStageDescriptorSamplers) ||
            (limits->maxPerStageDescriptorUniformBuffers > ddev->limits->maxPerStageDescriptorUniformBuffers) ||
            (limits->maxPerStageDescriptorStorageBuffers > ddev->limits->maxPerStageDescriptorStorageBuffers) ||
            (limits->maxPerStageDescriptorSampledImages > ddev->limits->maxPerStageDescriptorSampledImages) ||
            (limits->maxPerStageDescriptorStorageImages > ddev->limits->maxPerStageDescriptorStorageImages) ||
            (limits->maxPerStageDescriptorInputAttachments > ddev->limits->maxPerStageDescriptorInputAttachments) ||
            (limits->maxPerStageResources > ddev->limits->maxPerStageResources) ||
            (limits->maxDescriptorSetSamplers > ddev->limits->maxDescriptorSetSamplers) ||
            (limits->maxDescriptorSetUniformBuffers > ddev->limits->maxDescriptorSetUniformBuffers) ||
            (limits->maxDescriptorSetUniformBuffersDynamic > ddev->limits->maxDescriptorSetUniformBuffersDynamic) ||
            (limits->maxDescriptorSetStorageBuffers > ddev->limits->maxDescriptorSetStorageBuffers) ||
            (limits->maxDescriptorSetStorageBuffersDynamic > ddev->limits->maxDescriptorSetStorageBuffersDynamic) ||
            (limits->maxDescriptorSetSampledImages > ddev->limits->maxDescriptorSetSampledImages) ||
            (limits->maxDescriptorSetStorageImages > ddev->limits->maxDescriptorSetStorageImages) ||
            (limits->maxDescriptorSetInputAttachments > ddev->limits->maxDescriptorSetInputAttachments) ||
            (limits->maxVtxInputAttributes > ddev->limits->maxVtxInputAttributes) ||
            (limits->maxVtxInputBindings > ddev->limits->maxVtxInputBindings) ||
            (limits->maxVtxInputAttributeOffset > ddev->limits->maxVtxInputAttributeOffset) ||
            (limits->maxVtxInputBindingStride > ddev->limits->maxVtxInputBindingStride) ||
            (limits->maxVtxOutputCompos > ddev->limits->maxVtxOutputCompos) ||
            (limits->maxTessGenLvl > ddev->limits->maxTessGenLvl) ||
            (limits->maxTessPatchSiz > ddev->limits->maxTessPatchSiz) ||
            (limits->maxTessCtrlPerVtxInComps > ddev->limits->maxTessCtrlPerVtxInComps) ||
            (limits->maxTessCtrlPerVtxOutComps > ddev->limits->maxTessCtrlPerVtxOutComps) ||
            (limits->maxTessCtrlPerPatchOutComps > ddev->limits->maxTessCtrlPerPatchOutComps) ||
            (limits->maxTessCtrlTotalOutComps > ddev->limits->maxTessCtrlTotalOutComps) ||
            (limits->maxTessEvalInComps > ddev->limits->maxTessEvalInComps) ||
            (limits->maxTessEvalOutComps > ddev->limits->maxTessEvalOutComps) ||
            (limits->maxPrimShaderInvocations > ddev->limits->maxPrimShaderInvocations) ||
            (limits->maxPrimInComps > ddev->limits->maxPrimInComps) ||
            (limits->maxPrimOutComps > ddev->limits->maxPrimOutComps) ||
            (limits->maxPrimOutVertices > ddev->limits->maxPrimOutVertices) ||
            (limits->maxPrimTotalOutComps > ddev->limits->maxPrimTotalOutComps) ||
            (limits->maxFragInComps > ddev->limits->maxFragInComps) ||
            (limits->maxFragOutAttachments > ddev->limits->maxFragOutAttachments) ||
            (limits->maxFragDualSrcAttachments > ddev->limits->maxFragDualSrcAttachments) ||
            (limits->maxFragCombinedOutputResources > ddev->limits->maxFragCombinedOutputResources) ||
            (limits->maxComputeSharedMemorySiz > ddev->limits->maxComputeSharedMemorySiz) ||
            (limits->maxComputeWorkGroupCnt[0] > ddev->limits->maxComputeWorkGroupCnt[0]) ||
            (limits->maxComputeWorkGroupCnt[1] > ddev->limits->maxComputeWorkGroupCnt[1]) ||
            (limits->maxComputeWorkGroupCnt[2] > ddev->limits->maxComputeWorkGroupCnt[2]) ||
            (limits->maxComputeWorkGroupInvocations > ddev->limits->maxComputeWorkGroupInvocations) ||
            (limits->maxComputeWorkGroupSiz[0] > ddev->limits->maxComputeWorkGroupSiz[0]) ||
            (limits->maxComputeWorkGroupSiz[1] > ddev->limits->maxComputeWorkGroupSiz[1]) ||
            (limits->maxComputeWorkGroupSiz[2] > ddev->limits->maxComputeWorkGroupSiz[2]) ||
            (limits->subPixelPrecisionBits > ddev->limits->subPixelPrecisionBits) ||
            (limits->subTexelPrecisionBits > ddev->limits->subTexelPrecisionBits) ||
            (limits->mipmapPrecisionBits > ddev->limits->mipmapPrecisionBits) ||
            (limits->maxDrawIndexedIdxValue > ddev->limits->maxDrawIndexedIdxValue) ||
            (limits->maxDrawIndirectCnt > ddev->limits->maxDrawIndirectCnt) ||
            (limits->maxSamplerLodBias > ddev->limits->maxSamplerLodBias) ||
            (limits->maxSamplerAnisotropy > ddev->limits->maxSamplerAnisotropy) ||
            (limits->maxViewports > ddev->limits->maxViewports) ||
            (limits->maxViewportDimensions[0] > ddev->limits->maxViewportDimensions[0]) ||
            (limits->maxViewportDimensions[1] > ddev->limits->maxViewportDimensions[1]) ||
            (limits->viewportBoundsRange[0] > ddev->limits->viewportBoundsRange[0]) ||
            (limits->viewportBoundsRange[1] > ddev->limits->viewportBoundsRange[1]) ||
            (limits->viewportSubPixelBits > ddev->limits->viewportSubPixelBits) ||
            (limits->minMemMapAlign > ddev->limits->minMemMapAlign) ||
            (limits->minTexelBufOffsetAlign > ddev->limits->minTexelBufOffsetAlign) ||
            (limits->minUniformBufOffsetAlign > ddev->limits->minUniformBufOffsetAlign) ||
            (limits->minStorageBufOffsetAlign > ddev->limits->minStorageBufOffsetAlign) ||
            (limits->minTexelOffset > ddev->limits->minTexelOffset) ||
            (limits->maxTexelOffset > ddev->limits->maxTexelOffset) ||
            (limits->minTexelGatherOffset > ddev->limits->minTexelGatherOffset) ||
            (limits->maxTexelGatherOffset > ddev->limits->maxTexelGatherOffset) ||
            (limits->minInterpolationOffset > ddev->limits->minInterpolationOffset) ||
            (limits->maxInterpolationOffset > ddev->limits->maxInterpolationOffset) ||
            (limits->subPixelInterpolationOffsetBits > ddev->limits->subPixelInterpolationOffsetBits) ||
            (limits->maxCanvasWidth > ddev->limits->maxCanvasWidth) ||
            (limits->maxCanvasHeight > ddev->limits->maxCanvasHeight) ||
            (limits->maxCanvasLayers > ddev->limits->maxCanvasLayers) ||
            (limits->canvasColorSampleCnts > ddev->limits->canvasColorSampleCnts) ||
            (limits->canvasDepthSampleCnts > ddev->limits->canvasDepthSampleCnts) ||
            (limits->canvasStencilSampleCnts > ddev->limits->canvasStencilSampleCnts) ||
            (limits->canvasNoAttachmentsSampleCnts > ddev->limits->canvasNoAttachmentsSampleCnts) ||
            (limits->maxColorAttachments > ddev->limits->maxColorAttachments) ||
            (limits->sampledRasterColorSampleCnts > ddev->limits->sampledRasterColorSampleCnts) ||
            (limits->sampledRasterIntegerSampleCnts > ddev->limits->sampledRasterIntegerSampleCnts) ||
            (limits->sampledRasterDepthSampleCnts > ddev->limits->sampledRasterDepthSampleCnts) ||
            (limits->sampledRasterStencilSampleCnts > ddev->limits->sampledRasterStencilSampleCnts) ||
            (limits->storageRasterSampleCnts > ddev->limits->storageRasterSampleCnts) ||
            (limits->maxSampleMaskWords > ddev->limits->maxSampleMaskWords) ||
            (limits->timestampComputeAndGraphics > ddev->limits->timestampComputeAndGraphics) ||
            (limits->timestampPeriod > ddev->limits->timestampPeriod) ||
            (limits->maxClipDistances > ddev->limits->maxClipDistances) ||
            (limits->maxCullDistances > ddev->limits->maxCullDistances) ||
            (limits->maxCombinedClipAndCullDistances > ddev->limits->maxCombinedClipAndCullDistances) ||
            (limits->discreteQueuePriorities > ddev->limits->discreteQueuePriorities) ||
            (limits->pointSizRange[0] > ddev->limits->pointSizRange[0]) ||
            (limits->pointSizRange[1] > ddev->limits->pointSizRange[1]) ||
            (limits->lineWidthRange[0] > ddev->limits->lineWidthRange[0]) ||
            (limits->lineWidthRange[1] > ddev->limits->lineWidthRange[1]) ||
            (limits->pointSizGranularity > ddev->limits->pointSizGranularity) ||
            (limits->lineWidthGranularity > ddev->limits->lineWidthGranularity) ||
            (limits->strictLines > ddev->limits->strictLines) ||
            (limits->standardSampleLocations > ddev->limits->standardSampleLocations) ||
            (limits->optimalBufCopyOffsetAlign > ddev->limits->optimalBufCopyOffsetAlign) ||
            (limits->optimalBufCopyRowPitchAlign > ddev->limits->optimalBufCopyRowPitchAlign) ||
            (limits->nonCoherentAtomSiz > ddev->limits->nonCoherentAtomSiz))
        {
            rslt = FALSE;
        }
    }
    return rslt;
}

_AVX afxNat AfxChooseDrawDevices(afxDrawDeviceCaps const* caps, afxDrawDeviceLimits const* limits, afxNat maxCnt, afxNat ddevId[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(limits);
    AfxAssert(caps);
    afxNat rslt = 0;

    afxNat i = 0;
    afxDrawDevice ddev;
    while (AfxEnumerateDrawDevices(i, 1, &ddev))
    {
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        if (AfxIsDrawDeviceAcceptable(ddev, caps, limits))
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

_AVX afxNat AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxClass const* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)devices);
}

_AVXINL afxBool _AvxTestDdevFltCb(afxDrawDevice ddev, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ((afxBool(*)(afxDrawDevice, void*))udd[0])(ddev, udd[1]);
}

_AVX afxNat AfxEvokeDrawDevices(afxBool(*flt)(afxDrawDevice, void*), void* fdd, afxNat first, afxNat cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxClass const* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)devices);    
}

_AVX afxNat AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxInvokeDrawDevices2(afxNat first, afxNat cnt, afxBool(*flt)(afxDrawDevice, void*), void *fdd, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(flt);
    AfxAssert(f);
    afxClass const* cls = AfxGetDrawDeviceClass();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxInvokeClassInstances2(cls, first, cnt, (void*)flt, fdd, (void*)f, udd);
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

_AVX afxError _AvxDdevDtorCb(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    
    AfxAssert(ddev->stopCb);

    if (ddev->stopCb(ddev))
        AfxThrowError();

    AfxAssert(!ddev->idd);
    AfxCleanUpChainedClasses(&ddev->dev.classes);

    AfxCleanUpMutex(&ddev->relinkedCndMtx);
    AfxCleanUpCondition(&ddev->relinkedCnd);

    return err;
}

_AVX afxError _AvxDdevCtorCb(afxDrawDevice ddev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    //afxDrawDeviceInfo const* info = ((afxDrawDeviceInfo const *)cookie->udd[1]) + cookie->no;
    //AfxAssert(info);

    ddev->dev.serving = FALSE;

    AfxSetUpChain(&ddev->openedDoutChain, ddev);
    AfxSetUpChain(&ddev->openedDinChain, ddev);

    AfxSetUpCondition(&ddev->relinkedCnd);
    AfxSetUpMutex(&ddev->relinkedCndMtx, AFX_MTX_PLAIN);
    
    ddev->clipSpace = AVX_CLIP_SPACE_QWADRO;

    ddev->dev.procCb = NIL;
    ddev->idd = NIL;
    ddev->startCb = NIL;
    ddev->stopCb = NIL;
    ddev->openCb = NIL;
    ddev->closeCb = NIL;
    ddev->openDinCb = NIL;
    ddev->closeDinCb = NIL;
    ddev->relinkDinCb = NIL;
    ddev->closeDoutCb = NIL;
    ddev->openDoutCb = NIL;
    ddev->relinkDoutCb = NIL;

    if (AfxCallDevice(&ddev->dev, afxFcc_DSYS, NIL)) AfxThrowError();
    else
    {
        if (!ddev->portCnt) AfxThrowError();
        else
        {
            AfxAssert(ddev->portCaps);
        }

        if (err)
        {
            AfxCleanUpChainedClasses(&ddev->dev.classes);
        }
    }
    return err;
}

_AVX afxClassConfig const _AvxDdevClsCfg =
{
    .fcc = afxFcc_DDEV,
    .name = "DrawDevice",
    .desc = "Draw Device Driver Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawDevice)),
    .ctor = (void*)_AvxDdevCtorCb,
    .dtor = (void*)_AvxDdevDtorCb
};

_AVX afxBool _AvxParseDdevManifestCb(afxUri const* manifestUri, void* udd)
{
    afxError err = AFX_ERR_NONE;

    afxManifest ini;
    AfxSetUpIni(&ini);
    afxUri manifestFile;
    AfxClipUriFile(&manifestFile, manifestUri);
    AfxIniLoadFromFile(&ini, &manifestFile);

    afxDeviceType devType = afxDeviceType_DRAW;

    afxNat icdPagIdx, listPagIdx, listCnt, devPagIdx, recIdx;
    afxString devPag, pag, rec, s;

    if (AfxFindManifestPage(&ini, &AfxString("Qwadro.Icd"), &icdPagIdx) &&
        AfxFindManifestRecord(&ini, icdPagIdx, &AfxString("Devices"), &recIdx) &&
        AfxGetManifestString(&ini, icdPagIdx, recIdx, &pag) &&
        AfxFindManifestPage(&ini, &pag, &listPagIdx) &&
        (listCnt = AfxCountManifestRecords(&ini, listPagIdx)))
    {
        for (afxNat i = 0; i < listCnt; i++)
        {
            if (AfxGetManifestString(&ini, listPagIdx, i, &devPag) &&
                AfxFindManifestPage(&ini, &devPag, &devPagIdx))
            {
                if (AfxFindManifestRecord(&ini, devPagIdx, &AfxString("Class"), &recIdx) &&
                    AfxGetManifestString(&ini, devPagIdx, recIdx, &s) &&
                    (0 == AfxCompareString(&s, &AfxString("DRAW"))))
                {
                    afxClass* cls = (afxClass*)AfxGetDrawDeviceClass();
                    AfxAssertClass(cls, afxFcc_DDEV);

                    afxDrawDeviceInfo info = { 0 };
                    //info.dev.manifest = &ini;
                    info.dev.type = devType;
                    info.dev.manifestUri = manifestFile;
                    info.dev.manifestPagNo = devPagIdx;

                    afxDrawDevice ddev;

                    if (AfxAcquireObjects(cls, 1, (afxObject*)&ddev, (void const*[]) { udd, &info })) AfxThrowError();
                    else
                    {
                        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

                        if (err)
                            AfxReleaseObjects(1, &ddev), ddev = NIL;
                    }
                }
            }
        }
    }

    AfxCleanUpIni(&ini);
    return err;
}

_AVX afxError _AvxScanDdevManifests(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxUri fileMask;
    AfxMakeUri(&fileMask, "system/*.inf", 0);
    AfxFindFiles(&fileMask, _AvxParseDdevManifestCb, sys);
}

_AVX afxError _AvxScanDdevs(afxSystem sys, afxManifest const* ini)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    // scan for device drivers
    {
        HANDLE fh;
        WIN32_FIND_DATAA wfd;
        afxUri2048 pathBuf;
        AfxMakeUri2048(&pathBuf, NIL);
        afxUri fileMask;
        AfxMakeUri(&fileMask, "system/*.inf", 0);
        AfxResolveUri(afxFileFlag_RX, &fileMask, &pathBuf.uri);

        afxDeviceType devType = afxDeviceType_DRAW;

        if ((fh = FindFirstFileA(AfxGetUriData(&pathBuf.uri, 0), &(wfd))))
        {
            do
            {
                afxManifest ini;
                AfxSetUpIni(&ini);
                afxUri manifestUri, manifestFile;
                AfxMakeUri(&manifestUri, wfd.cFileName, 0);
                AfxClipUriFile(&manifestFile, &manifestUri);
                AfxIniLoadFromFile(&ini, &manifestFile);

                afxNat icdPagIdx, listPagIdx, listCnt, devPagIdx, recIdx;
                afxString devPag, pag, rec, s;

                if (AfxFindManifestPage(&ini, &AfxString("Qwadro.Icd"), &icdPagIdx) &&
                    AfxFindManifestRecord(&ini, icdPagIdx, &AfxString("Devices"), &recIdx) &&
                    AfxGetManifestString(&ini, icdPagIdx, recIdx, &pag) &&
                    AfxFindManifestPage(&ini, &pag, &listPagIdx) &&
                    (listCnt = AfxCountManifestRecords(&ini, listPagIdx)))
                {
                    for (afxNat i = 0; i < listCnt; i++)
                    {
                        if (AfxGetManifestString(&ini, listPagIdx, i, &devPag) &&
                            AfxFindManifestPage(&ini, &devPag, &devPagIdx))
                        {
                            if (AfxFindManifestRecord(&ini, devPagIdx, &AfxString("Class"), &recIdx) &&
                                AfxGetManifestString(&ini, devPagIdx, recIdx, &s) &&
                                (0 == AfxCompareString(&s, &AfxString("DRAW"))))
                            {
                                afxClass* cls = (afxClass*)AfxGetDrawDeviceClass();
                                AfxAssertClass(cls, afxFcc_DDEV);

                                afxDrawDeviceInfo info = { 0 };
                                //info.dev.manifest = &ini;
                                info.dev.type = devType;
                                info.dev.manifestUri = manifestFile;
                                info.dev.manifestPagNo = devPagIdx;

                                afxDrawDevice ddev;

                                if (AfxAcquireObjects(cls, 1, (afxObject*)&ddev, (void const*[]) { sys, &info })) AfxThrowError();
                                else
                                {
                                    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

                                    if (err)
                                        AfxReleaseObjects(1, &ddev), ddev = NIL;
                                }
                            }
                        }
                    }
                }

                AfxCleanUpIni(&ini);
            } while (FindNextFileA(fh, &wfd));
            FindClose(fh);
        }
    }
    return err;
}

_AVX afxError AfxSystemIoctl(afxSystem sys, afxModule mdle, afxNat reqCode, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case 3:
    {
        AfxAssert(sys->avx.dsys == NIL);

        afxClassConfig clsCfg;

        clsCfg = _AvxDdevClsCfg;
        AfxRegisterClass(&TheDrawSystem.ddevCls, AfxGetDeviceClass(), &sys->classes, &clsCfg); // require base*

        clsCfg = _AvxDoutClsCfg;
        AfxRegisterClass(&TheDrawSystem.doutCls, NIL, &sys->classes, &clsCfg); // require ddev, dout

        clsCfg = _AvxDinClsCfg;
        AfxRegisterClass(&TheDrawSystem.dinCls, NIL, &sys->classes, &clsCfg); // require ddev, din

        if (!err)
        {
            sys->avx.dsys = &TheDrawSystem;
            TheDrawSystem.ready = TRUE;
            _AvxScanDdevs(sys, udd[0]);
        }

        break;
    }
    case 4:
    {
        AfxAssert(sys->avx.dsys == &TheDrawSystem);
        TheDrawSystem.ready = FALSE;

        AfxExhaustClass(&TheDrawSystem.dinCls);
        AfxExhaustClass(&TheDrawSystem.doutCls);
        AfxExhaustClass(&TheDrawSystem.ddevCls);

        sys->avx.dsys = NIL;

        break;
    }
    default: break;
    }
    return err;
}
