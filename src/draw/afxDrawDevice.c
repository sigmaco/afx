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

#define _AFX_CORE_C
#define _AFX_SYSTEM_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AVX_DRAW_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_DEVICE_C
#define _AVX_DRAW_SYSTEM_C
#define _AVX_DRAW_OUTPUT_C
#define _AVX_DRAW_INPUT_C
#include "impl/avxImplementation.h"
#include "../impl/afxExecImplKit.h"

_AVX afxString const sigmaDrawSignature = AFX_STRING(
    "      ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::      \n"
    "     :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:     \n"
    "     +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+     \n"
    "     +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+     \n"
    "     +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+     \n"
    "     #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#     \n"
    "      ###### ###  ###   ###   ###     ### #########  ###    ###  ########      \n"
    "                                                                               \n"
    "    Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E    \n"
    "                                                                               \n"
    "                               Public Test Build                               \n"
    "                           (c) 2017 SIGMA FEDERATION                           \n"
    "                                www.sigmaco.org                                \n"
    "                                                                               \n"
);

//extern afxChain* _AfxGetSystemClassChain(void);
AFX afxChain* _AfxGetSystemClassChain(void);

////////////////////////////////////////////////////////////////////////////////
// DRAW DEVICE HANDLING                                                       //
////////////////////////////////////////////////////////////////////////////////

_AVX afxDrawLimits const* _AvxAccessDrawLimits(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    return &ddev->limits;
}

_AVX afxBool AfxIsDrawDevicePrompt(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    return ddev->dev.serving;
}

_AVX void* AfxGetDrawDeviceIdd(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    return ddev->idd;
}

_AVX afxUnit AfxCountDrawPorts(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    return ddev->portCnt;
}

_AVX void AfxQueryDrawDeviceLimits(afxDrawDevice ddev, afxDrawLimits* limits)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    AFX_ASSERT(limits);
    *limits = ddev->limits;
}

_AVX void AfxQueryDrawDeviceFeatures(afxDrawDevice ddev, afxDrawFeatures* features)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    AFX_ASSERT(features);
    *features = ddev->features;
}

_AVX void AfxQueryDrawPortCapabilities(afxDrawDevice ddev, afxUnit portId, afxDrawPortCaps* caps)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    AFX_ASSERT_RANGE(ddev->portCnt, portId, 1);
    AFX_ASSERT(caps);
    *caps = ddev->ports[portId].caps;
}

_AVX afxUnit AfxChooseDrawPorts(afxDrawDevice ddev, afxDrawPortFlags caps, afxAcceleration accel, afxUnit maxCnt, afxUnit portId[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    afxUnit rslt = 0;

    for (afxUnit i = ddev->portCnt; i-- > 0;)
    {
        afxDrawPortCaps const* port = &ddev->ports[i].caps;
        
        if (!caps || (caps == (caps & port->capabilities)))
        {
            if (!accel || (accel == (accel & port->acceleration)))
            {
                portId[rslt++] = i;
            }
        }

        if (rslt == maxCnt)
            break;
    }
    return rslt;
}

_AVX afxBool AfxIsDrawDeviceAcceptable(afxDrawDevice ddev, afxDrawFeatures const* features, afxDrawLimits const* limits)
{
    afxError err = AFX_ERR_NONE;
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
            (limits->maxRasterArrayLayers > ddev->limits.maxRasterArrayLayers) ||
            (limits->maxTexelBufElements > ddev->limits.maxTexelBufElements) ||
            (limits->maxUboRange > ddev->limits.maxUboRange) ||
            (limits->maxSsboRange > ddev->limits.maxSsboRange) ||
            (limits->maxPushConstsSiz > ddev->limits.maxPushConstsSiz) ||
            (limits->maxMemAllocCnt > ddev->limits.maxMemAllocCnt) ||
            (limits->maxSamplerAllocCnt > ddev->limits.maxSamplerAllocCnt) ||
            (limits->bufferRasterGranularity > ddev->limits.bufferRasterGranularity) ||
            (limits->sparseAddrSpaceSiz > ddev->limits.sparseAddrSpaceSiz) ||
            (limits->maxBoundLigaSets > ddev->limits.maxBoundLigaSets) ||
            (limits->maxPerStageSamplers > ddev->limits.maxPerStageSamplers) ||
            (limits->maxPerStageUbos > ddev->limits.maxPerStageUbos) ||
            (limits->maxPerStageSsbos > ddev->limits.maxPerStageSsbos) ||
            (limits->maxPerStageSampledImages > ddev->limits.maxPerStageSampledImages) ||
            (limits->maxPerStageStorageImages > ddev->limits.maxPerStageStorageImages) ||
            (limits->maxPerStageInputAttachments > ddev->limits.maxPerStageInputAttachments) ||
            (limits->maxPerStageResources > ddev->limits.maxPerStageResources) ||
            (limits->maxLigaSetSamplers > ddev->limits.maxLigaSetSamplers) ||
            (limits->maxLigaSetUbos > ddev->limits.maxLigaSetUbos) ||
            (limits->maxLigaSetUbosDynamic > ddev->limits.maxLigaSetUbosDynamic) ||
            (limits->maxLigaSetSsbos > ddev->limits.maxLigaSetSsbos) ||
            (limits->maxLigaSetSsbosDynamic > ddev->limits.maxLigaSetSsbosDynamic) ||
            (limits->maxLigaSetSampledImages > ddev->limits.maxLigaSetSampledImages) ||
            (limits->maxLigaSetStorageImages > ddev->limits.maxLigaSetStorageImages) ||
            (limits->maxLigaSetInputAttachments > ddev->limits.maxLigaSetInputAttachments) ||
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
            (limits->minMemMapAlign > ddev->limits.minMemMapAlign) ||
            (limits->minTexelBufOffsetAlign > ddev->limits.minTexelBufOffsetAlign) ||
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

_AVX afxError _AvxRegisterDisplays(afxDrawDevice ddev, afxUnit cnt, avxDisplayInfo const infos[], afxDisplay displays[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);
    AFX_ASSERT(displays);
    AFX_ASSERT(infos);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)&ddev->vduCls;
    AFX_ASSERT_CLASS(cls, afxFcc_VDU);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)displays, (void const*[]) { ddev, infos, NIL }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_VDU, cnt, displays);

    }
    return err;
}

_AVX afxError _AvxDdevDtorCb(afxDrawDevice ddev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    AfxDeregisterChainedClasses(&ddev->dev.classes);

    AfxCallDevice(&ddev->dev, 2, NIL); // terminate

    AFX_ASSERT(!ddev->idd);
    AfxDeregisterChainedClasses(&ddev->dev.classes);

    afxObjectStash stashes[] =
    {
        {
            .cnt = ddev->portCnt,
            .siz = sizeof(ddev->ports[0]),
            .var = (void**)&ddev->ports
        }
    };
    AfxDeallocateInstanceData(ddev, ARRAY_SIZE(stashes), stashes);

    return err;
}

_AVX afxError _AvxDdevCtorCb(afxDrawDevice ddev, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxDrawDeviceInfo const* info = AFX_CAST(afxDrawDeviceInfo const*, args[1]) + invokeNo;
    AFX_ASSERT(info);
    afxClassConfig* vduClsCfg = AFX_CAST(afxClassConfig*, args[2]) + invokeNo;

    if (_AfxDevBaseImplementation.ctor(&ddev->dev, (void*[]){ icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        ddev->leftHandedSpace = FALSE;
        ddev->clipSpaceDepth = avxClipSpaceDepth_ZERO_TO_ONE;

        ddev->idd = NIL;

        ddev->limits = info->limits;
        ddev->features = info->features;
        ddev->leftHandedSpace = info->leftHandedSpace;
        ddev->clipSpaceDepth = info->clipSpaceDepth;

        ddev->portCnt = info->portCnt;

        afxObjectStash stashes[] =
        {
            {
                .cnt = ddev->portCnt,
                .siz = sizeof(ddev->ports[0]),
                .var = (void**)&ddev->ports
            }
        };

        if (AfxAllocateInstanceData(ddev, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
        else
        {
            if (!ddev->portCnt) AfxThrowError();
            else
            {
                AFX_ASSERT(ddev->ports);

                for (afxUnit i = 0; i < ddev->portCnt; i++)
                {
                    ddev->ports[i].caps = (afxDrawPortCaps) { 0 };
                    AfxMakeString128(&ddev->ports[i].desc, NIL);
                    AfxMakeString8(&ddev->ports[i].urn, NIL);
                }

                AfxMountClass(&ddev->vduCls, (afxClass*)AfxGetDeviceClass(), &ddev->dev.classes, vduClsCfg ? vduClsCfg : &_AVX_VDU_CLASS_CONFIG);
                
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
                AfxDeallocateInstanceData(ddev, ARRAY_SIZE(stashes), stashes);
        }
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

_AVX afxUnit AfxEnumerateDrawDevices(afxUnit icd, afxUnit first, afxUnit cnt, afxDrawDevice devices[])
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
        afxClass const* cls = _AvxGetDrawDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DDEV);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_AVX afxUnit AfxInvokeDrawDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawDevice), afxUnit cnt)
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
        afxClass const* cls = _AvxGetDrawDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DDEV);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_AVX afxUnit AfxEvokeDrawDevices(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawDevice), afxUnit cnt, afxDrawDevice devices[])
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
        afxClass const* cls = _AvxGetDrawDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DDEV);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)devices);
        break;
    }
    return rslt;
}

_AVX afxUnit AfxChooseDrawDevices(afxUnit icd, afxDrawFeatures const* features, afxDrawLimits const* limits, afxUnit maxCnt, afxUnit ddevId[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(limits);
    AFX_ASSERT(features);
    afxUnit rslt = 0;
    afxModule mdle;
    while (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxGetDrawDeviceClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DDEV);

        afxUnit i = 0;
        afxDrawDevice ddev;
        while (AfxEnumerateObjects(cls, i, 1, (afxObject*)&ddev))
        {
            AFX_ASSERT_OBJECTS(afxFcc_DDEV, 1, &ddev);

            if (AfxIsDrawDeviceAcceptable(ddev, features, limits))
            {
                ddevId[rslt] = i;
                ++rslt;

                if (maxCnt > rslt)
                    break;
            }
            i++;
        }
        break;
    }
    return rslt;
}

_AVX afxUnit AfxInvokeDrawSystems(afxUnit icd, afxUnit first, void *udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt)
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
        afxClass const* cls = _AvxGetDrawSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DSYS);
        rslt = AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
        break;
    }
    return rslt;
}

_AVX afxUnit AfxEvokeDrawSystems(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, afxDrawSystem), afxUnit cnt, afxDrawSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(f);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxGetDrawSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DSYS);
        rslt = AfxEvokeObjects(cls, (void*)f, udd, first, cnt, (afxObject*)systems);
        break;
    }
    return rslt;
}

_AVX afxUnit AfxEnumerateDrawSystems(afxUnit icd, afxUnit first, afxUnit cnt, afxDrawSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(systems);
    AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    afxModule mdle;
    while (_AvxGetIcd(icd, &mdle))
    {
        AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &mdle);
        AFX_ASSERT(AfxTestModule(mdle, afxModuleFlag_ICD | afxModuleFlag_AVX) == (afxModuleFlag_ICD | afxModuleFlag_AVX));
        afxClass const* cls = _AvxGetDrawSystemClass(mdle);
        AFX_ASSERT_CLASS(cls, afxFcc_DSYS);
        rslt = AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
        break;
    }
}
