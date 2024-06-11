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
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C

#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_CONTEXT_C
#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_INPUT_C

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/avxDevKit.h"

//_AVX afxBool dsysReady = FALSE;
_AVX afxByte theDsysData[AFX_ALIGN(sizeof(afxObjectBase), 16) + AFX_ALIGN(sizeof(AFX_OBJECT(afxDrawSystem)), 16)] = { 0 };
_AVX afxDrawSystem TheDrawSystem = (void*)&theDsysData;
AFX_STATIC_ASSERT(sizeof(theDsysData) >= (sizeof(afxObjectBase) + sizeof(TheDrawSystem[0])), "");

//extern afxChain* _AfxGetSystemClassChain(void);
extern afxClassConfig const _AvxCamMgrCfg;
extern afxClassConfig const _AvxDoutMgrCfg;
extern afxClassConfig const _AvxDinMgrCfg;

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

_AVX afxManager* AfxGetCameraClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager *cls = &dsys->camMgr;
    AfxAssertClass(cls, afxFcc_CAM);
    return cls;
}

_AVX afxManager* AfxGetDrawDeviceManager(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->ddevMgr;
    AfxAssertClass(cls, afxFcc_DDEV);
    return cls;
}

_AVX afxManager* AfxGetDrawBridgeClass(afxDrawDevice ddev, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertRange(ddev->portCnt, portIdx, 1);
    afxManager* cls = &ddev->ports[portIdx].ddgeMgr;
    AfxAssertClass(cls, afxFcc_DDGE);
    return cls;
}

_AVX afxManager* AfxGetDrawQueueClass(afxDrawDevice ddev, afxNat portIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertRange(ddev->portCnt, portIdx, 1);
    afxManager* cls = &ddev->ports[portIdx].dqueMgr;
    AfxAssertClass(cls, afxFcc_DQUE);
    return cls;
}

_AVX afxManager* AfxGetDrawContextClass(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    afxManager* cls = &ddev->contexts;
    AfxAssertClass(cls, afxFcc_DCTX);
    return cls;
}

_AVX void* AfxGetDrawDeviceIdd(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->idd;
}

_AVX afxManager* AfxGetDrawInputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->dinMgr;
    AfxAssertClass(cls, afxFcc_DIN);
    return cls;
}

_AVX afxManager* AfxGetDrawOutputClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    afxManager* cls = &dsys->doutMgr;
    AfxAssertClass(cls, afxFcc_DOUT);
    return cls;
}

_AVX afxBool AfxDrawDeviceIsRunning(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    return ddev->dev.serving;
}

_AVX afxNat AfxCountDrawDevices(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetDrawDeviceManager();
    AfxAssertClass(cls, afxFcc_SDEV);
    return AfxCountObjects(cls);
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
    *caps = ddev->ports[portIdx].portCaps;
}

_AVX void AfxGetDrawDeviceCapabilities(afxDrawDevice ddev, afxDrawDeviceCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(caps);
    *caps = ddev->caps;
}

_AVX void AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawDeviceLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssert(limits);
    *limits = ddev->limits;
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
        if (
            (caps->robustBufAccess && !ddev->caps.robustBufAccess) ||
            (caps->fullDrawIdxUint32 && !ddev->caps.fullDrawIdxUint32) ||
            (caps->rasterCubeArray && !ddev->caps.rasterCubeArray) ||
            (caps->independentBlend && !ddev->caps.independentBlend) ||
            (caps->primShader && !ddev->caps.primShader) ||
            (caps->tessShader && !ddev->caps.tessShader) ||
            (caps->sampleRateShading && !ddev->caps.sampleRateShading) ||
            (caps->dualSrcBlend && !ddev->caps.dualSrcBlend) ||
            (caps->logicOp && !ddev->caps.logicOp) ||
            (caps->multiDrawIndirect && !ddev->caps.multiDrawIndirect) ||
            (caps->drawIndirectFirstInst && !ddev->caps.drawIndirectFirstInst) ||
            (caps->depthClamp && !ddev->caps.depthClamp) ||
            (caps->depthBiasClamp && !ddev->caps.depthBiasClamp) ||
            (caps->fillModeNonSolid && !ddev->caps.fillModeNonSolid) ||
            (caps->depthBounds && !ddev->caps.depthBounds) ||
            (caps->wideLines && !ddev->caps.wideLines) ||
            (caps->largePoints && !ddev->caps.largePoints) ||
            (caps->alphaToOne && !ddev->caps.alphaToOne) ||
            (caps->multiViewport && !ddev->caps.multiViewport) ||
            (caps->samplerAnisotropy && !ddev->caps.samplerAnisotropy) ||
            (caps->etc2 && !ddev->caps.etc2) ||
            (caps->astc_LDR && !ddev->caps.astc_LDR) ||
            (caps->dxt && !ddev->caps.dxt) ||
            (caps->occlusionQueryPrecise && !ddev->caps.occlusionQueryPrecise) ||
            (caps->pipelineStatsQuery && !ddev->caps.pipelineStatsQuery) ||
            (caps->vtxPipelineStoresAndAtomics && !ddev->caps.vtxPipelineStoresAndAtomics) ||
            (caps->fragStoresAndAtomics && !ddev->caps.fragStoresAndAtomics) ||
            (caps->shaderTessAndPrimPointSiz && !ddev->caps.shaderTessAndPrimPointSiz) ||
            (caps->shaderRasterGatherExt && !ddev->caps.shaderRasterGatherExt) ||
            (caps->shaderStorageRasterExtFmts && !ddev->caps.shaderStorageRasterExtFmts) ||
            (caps->shaderStorageRasterMultisample && !ddev->caps.shaderStorageRasterMultisample) ||
            (caps->shaderStorageRasterReadWithoutFmt && !ddev->caps.shaderStorageRasterReadWithoutFmt) ||
            (caps->shaderStorageRasterWriteWithoutFmt && !ddev->caps.shaderStorageRasterWriteWithoutFmt) ||
            (caps->shaderUniformBufferArrayDynIndexing && !ddev->caps.shaderUniformBufferArrayDynIndexing) ||
            (caps->shaderSampledRasterArrayDynIndexing && !ddev->caps.shaderSampledRasterArrayDynIndexing) ||
            (caps->shaderStorageBufferArrayDynIndexing && !ddev->caps.shaderStorageBufferArrayDynIndexing) ||
            (caps->shaderStorageImageArrayDynIndexing && !ddev->caps.shaderStorageImageArrayDynIndexing) ||
            (caps->shaderClipDist && !ddev->caps.shaderClipDist) ||
            (caps->shaderCullDist && !ddev->caps.shaderCullDist) ||
            (caps->shaderFloat64 && !ddev->caps.shaderFloat64) ||
            (caps->shaderInt64 && !ddev->caps.shaderInt64) ||
            (caps->shaderInt16 && !ddev->caps.shaderInt16) ||
            (caps->shaderRsrcResidency && !ddev->caps.shaderRsrcResidency) ||
            (caps->shaderRsrcMinLod && !ddev->caps.shaderRsrcMinLod) ||
            (caps->sparseBinding && !ddev->caps.sparseBinding) ||
            (caps->sparseResidencyBuffer && !ddev->caps.sparseResidencyBuffer) ||
            (caps->sparseResidencyRaster2D && !ddev->caps.sparseResidencyRaster2D) ||
            (caps->sparseResidencyRaster3D && !ddev->caps.sparseResidencyRaster3D) ||
            (caps->sparseResidency2Samples && !ddev->caps.sparseResidency2Samples) ||
            (caps->sparseResidency4Samples && !ddev->caps.sparseResidency4Samples) ||
            (caps->sparseResidency8Samples && !ddev->caps.sparseResidency8Samples) ||
            (caps->sparseResidency16Samples && !ddev->caps.sparseResidency16Samples) ||
            (caps->sparseResidencyAliased && !ddev->caps.sparseResidencyAliased) ||
            (caps->variableMultisampleRate && !ddev->caps.variableMultisampleRate) ||
            (caps->inheritedQueries && !ddev->caps.inheritedQueries))
        {
            rslt = FALSE;
        }
    }

    if (limits)
    {
        if (
            (limits->maxRasterDim1D > ddev->limits.maxRasterDim1D) ||
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
            (limits->maxViewports > ddev->limits.maxViewports) ||
            (limits->maxViewportDimensions[0] > ddev->limits.maxViewportDimensions[0]) ||
            (limits->maxViewportDimensions[1] > ddev->limits.maxViewportDimensions[1]) ||
            (limits->viewportBoundsRange[0] > ddev->limits.viewportBoundsRange[0]) ||
            (limits->viewportBoundsRange[1] > ddev->limits.viewportBoundsRange[1]) ||
            (limits->viewportSubPixelBits > ddev->limits.viewportSubPixelBits) ||
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
            (limits->maxCanvasWidth > ddev->limits.maxCanvasWidth) ||
            (limits->maxCanvasHeight > ddev->limits.maxCanvasHeight) ||
            (limits->maxCanvasLayers > ddev->limits.maxCanvasLayers) ||
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
            (limits->nonCoherentAtomSiz > ddev->limits.nonCoherentAtomSiz)
            )
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

_AVX afxNat AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetDrawDeviceManager();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AVX afxNat AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice devices[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(devices);
    afxManager* cls = AfxGetDrawDeviceManager();
    AfxAssertClass(cls, afxFcc_DDEV);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
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

_AVX afxError _AvxDdevDtor(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    
    if (ddev->iddDtorCb(ddev))
        AfxThrowError();

    AfxAssert(!ddev->idd);
    AfxCleanUpChainedManagers(&ddev->dev.classes);

    AfxCleanUpMutex(&ddev->relinkedCndMtx);
    AfxCleanUpCondition(&ddev->relinkedCnd);

    if (ddev->ports)
        AfxDeallocate(ddev->ports);

    return err;
}

_AVX afxError _AvxDdevCtor(afxDrawDevice ddev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);

    afxDrawSystem dsys = cookie->udd[0];
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);
    //afxDrawDeviceInfo const* info = ((afxDrawDeviceInfo const *)cookie->udd[1]) + cookie->no;
    //AfxAssert(info);

    ddev->dev.serving = FALSE;

    AfxSetUpChain(&ddev->outputs, ddev);
    AfxSetUpChain(&ddev->inputs, ddev);

    AfxSetUpCondition(&ddev->relinkedCnd);
    AfxSetUpMutex(&ddev->relinkedCndMtx, AFX_MTX_PLAIN);
    
    afxDrawDeviceInfo info2 = { 0 };
    info2.clipSpace = AFX_CLIP_SPACE_QWADRO;

    if (AfxCallDevice(&ddev->dev, afxFcc_DSYS, &info2)) AfxThrowError();
    else
    {
        ddev->caps = info2.caps;
        ddev->limits = info2.limits;
        ddev->clipCfg = info2.clipSpace;

        ddev->dev.procCb = (void*)info2.procCb;
        ddev->idd = info2.idd;
        ddev->iddDtorCb = info2.iddDtorCb;
        ddev->dinIddCtorCb = info2.dinIddCtorCb;
        ddev->dinIddDtorCb = info2.dinIddDtorCb;
        ddev->dinRelinkCb = info2.dinRelinkCb;
        ddev->doutIddCtorCb = info2.doutIddCtorCb;
        ddev->doutIddDtorCb = info2.doutIddDtorCb;
        ddev->doutRelinkCb = info2.doutRelinkCb;

        AfxAssert(info2.portCnt);
        afxNat portCnt = AfxMax(1, info2.portCnt);
        ddev->portCnt = portCnt;
        AfxAssert(ddev->portCnt);

        if (!(ddev->ports = AfxAllocate(portCnt, sizeof(ddev->ports[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            afxChain* classes = &ddev->dev.classes;
            afxClassConfig tmpClsCfg;

            for (afxNat i = 0; i < portCnt; i++)
            {
                ddev->ports[i].portCaps = info2.portCaps[i];

                tmpClsCfg = *info2.dqueClsCfg;
                AfxEstablishManager(&ddev->ports[i].dqueMgr, NIL, classes, &tmpClsCfg);

                tmpClsCfg = *info2.ddgeClsCfg;
                AfxEstablishManager(&ddev->ports[i].ddgeMgr, NIL, classes, &tmpClsCfg);
            }

            // dctx must be after ddge
            tmpClsCfg = *info2.dctxClsCfg;
            AfxEstablishManager(&ddev->contexts, NIL, classes, &tmpClsCfg); // require ddge, cmdb

            if (info2.iddCtorCb(ddev)) AfxThrowError();
            else
            {

            }

            if (err)
            {
                AfxCleanUpChainedManagers(&ddev->dev.classes);
                AfxDeallocate(ddev->ports);
            }
        }
    }
    return err;
}

_AVX afxClassConfig const _AvxDdevMgrCfg =
{
    .fcc = afxFcc_DDEV,
    .name = "DrawDevice",
    .desc = "Draw Device Driver Interface",
    .size = sizeof(AFX_OBJECT(afxDrawDevice)),
    .ctor = (void*)_AvxDdevCtor,
    .dtor = (void*)_AvxDdevDtor
};

_AVX afxError _AvxDsysCtor(afxDrawSystem dsys, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    afxSystem sys = cookie->udd[0];
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxManifest const* ini = cookie->udd[1];
    //afxDrawSystemConfig const* config = cookie->udd[2];

    afxChain* managers = &dsys->managers;
    AfxSetUpChain(managers, dsys);
    afxClassConfig clsCfg;

    clsCfg = _AvxCamMgrCfg;
    AfxEstablishManager(&dsys->camMgr, NIL, managers, &clsCfg);

    clsCfg = _AvxDdevMgrCfg;
    AfxEstablishManager(&dsys->ddevMgr, AfxGetDeviceClass(), managers, &clsCfg); // require base*

    clsCfg = _AvxDoutMgrCfg;
    AfxEstablishManager(&dsys->doutMgr, NIL, managers, &clsCfg); // require ddev, dout

    clsCfg = _AvxDinMgrCfg;
    AfxEstablishManager(&dsys->dinMgr, NIL, managers, &clsCfg); // require ddev, din

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
                                afxManager* cls = AfxGetDrawDeviceManager();
                                AfxAssertClass(cls, afxFcc_DDEV);

                                afxDrawDeviceInfo info = { 0 };
                                //info.dev.manifest = &ini;
                                info.dev.type = devType;
                                info.dev.manifestUri = manifestFile;
                                info.dev.manifestPagNo = devPagIdx;

                                afxDrawDevice ddev;

                                if (AfxAcquireObjects(cls, 1, (afxObject*)&ddev, (void const*[]) { dsys, &info })) AfxThrowError();
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

    if (err)
    {
        AfxCleanUpChainedManagers(managers);
    }
    return err;
}

_AVX afxError _AvxDsysDtor(afxDrawSystem dsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dsys, afxFcc_DSYS);

    AfxCleanUpChainedManagers(&dsys->managers);

    return err;
}

_AVX afxClassConfig const _AvxDsysMgrCfg =
{
    .fcc = afxFcc_DSYS,
    .name = "DrawSystem",
    .desc = "Draw I/O System",
    .unitsPerPage = 1,
    .maxCnt = 1,
    //.size = sizeof(AFX_OBJECT(afxDrawSystem)),
    .ctor = (void*)_AvxDsysCtor,
    .dtor = (void*)_AvxDsysDtor
};

_AVX afxError AfxSystemIoctl(afxSystem sys, afxModule mdle, afxNat reqCode, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertObjects(1, &mdle, afxFcc_MDLE);

    switch (reqCode)
    {
    case 2:
    {
        AfxAssert(udd);
        udd[0] = (void*)&_AvxDsysMgrCfg;
        udd[1] = TheDrawSystem;
        break;
    }
    default: break;
    }
    return err;
}
