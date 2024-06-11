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

#ifndef AVX_DRAW_SYSTEM_H
#define AVX_DRAW_SYSTEM_H

#include "qwadro/core/afxDevice.h"
#include "qwadro/draw/afxDrawDefs.h"
#include "qwadro/draw/afxColor.h"
#include "qwadro/draw/afxPixel.h"
// provided classes.
#include "qwadro/draw/afxDrawBridge.h"
#include "qwadro/draw/avxCmdb.h"
#include "qwadro/draw/pipe/afxDrawOps.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/afxDrawOutput.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/io/afxTarga.h"
// provided classes.
#include "qwadro/draw/afxCamera.h"

AFX_DEFINE_STRUCT(afxDrawDeviceLimits)
{
    afxNat maxRasterDim1D;
    afxNat maxRasterDim2D;
    afxNat maxRasterDim3D;
    afxNat maxRasterDimCube;
    afxNat maxRasterArrayLayers;
    afxNat maxTexelBufElements;
    afxNat maxUniformBufRange;
    afxNat maxStorageBufRange;
    afxNat maxPushConstantsSiz;
    afxNat maxMemAllocCnt;
    afxNat maxSamplerAllocCnt;
    afxSize bufferRasterGranularity;
    afxSize sparseAddrSpaceSiz;
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
    afxNat maxVtxInputAttributes;
    afxNat maxVtxInputBindings;
    afxNat maxVtxInputAttributeOffset;
    afxNat maxVtxInputBindingStride;
    afxNat maxVtxOutputCompos;
    afxNat maxTessGenLvl;
    afxNat maxTessPatchSiz;
    afxNat maxTessCtrlPerVtxInComps;
    afxNat maxTessCtrlPerVtxOutComps;
    afxNat maxTessCtrlPerPatchOutComps;
    afxNat maxTessCtrlTotalOutComps;
    afxNat maxTessEvalInComps;
    afxNat maxTessEvalOutComps;
    afxNat maxPrimShaderInvocations;
    afxNat maxPrimInComps;
    afxNat maxPrimOutComps;
    afxNat maxPrimOutVertices;
    afxNat maxPrimTotalOutComps;
    afxNat maxFragInComps;
    afxNat maxFragOutAttachments;
    afxNat maxFragDualSrcAttachments;
    afxNat maxFragCombinedOutputResources;
    afxNat maxComputeSharedMemorySiz;
    afxNat maxComputeWorkGroupCnt[3];
    afxNat maxComputeWorkGroupInvocations;
    afxNat maxComputeWorkGroupSiz[3];
    afxNat subPixelPrecisionBits;
    afxNat subTexelPrecisionBits;
    afxNat mipmapPrecisionBits;
    afxNat maxDrawIndexedIdxValue;
    afxNat maxDrawIndirectCnt;
    afxReal maxSamplerLodBias;
    afxReal maxSamplerAnisotropy;
    afxNat maxViewports;
    afxNat maxViewportDimensions[2];
    afxReal viewportBoundsRange[2];
    afxNat viewportSubPixelBits;
    afxSize minMemMapAlign;
    afxSize minTexelBufOffsetAlign;
    afxSize minUniformBufOffsetAlign;
    afxSize minStorageBufOffsetAlign;
    afxInt minTexelOffset;
    afxNat maxTexelOffset;
    afxInt minTexelGatherOffset;
    afxNat maxTexelGatherOffset;
    afxReal minInterpolationOffset;
    afxReal maxInterpolationOffset;
    afxNat subPixelInterpolationOffsetBits;
    afxNat maxCanvasWidth;
    afxNat maxCanvasHeight;
    afxNat maxCanvasLayers;
    afxFlags canvasColorSampleCnts;
    afxFlags canvasDepthSampleCnts;
    afxFlags canvasStencilSampleCnts;
    afxFlags canvasNoAttachmentsSampleCnts;
    afxNat maxColorAttachments;
    afxFlags sampledRasterColorSampleCnts;
    afxFlags sampledRasterIntegerSampleCnts;
    afxFlags sampledRasterDepthSampleCnts;
    afxFlags sampledRasterStencilSampleCnts;
    afxFlags storageRasterSampleCnts;
    afxNat maxSampleMaskWords;
    afxBool timestampComputeAndGraphics;
    afxReal timestampPeriod;
    afxNat maxClipDistances;
    afxNat maxCullDistances;
    afxNat maxCombinedClipAndCullDistances;
    afxNat discreteQueuePriorities;
    afxReal pointSizRange[2];
    afxReal lineWidthRange[2];
    afxReal pointSizGranularity;
    afxReal lineWidthGranularity;
    afxBool strictLines;
    afxBool standardSampleLocations;
    afxSize optimalBufCopyOffsetAlign;
    afxSize optimalBufCopyRowPitchAlign;
    afxSize nonCoherentAtomSiz;
};

AFX_DEFINE_STRUCT(afxDrawDeviceCaps)
{
    afxBool robustBufAccess;
    afxBool fullDrawIdxUint32;
    afxBool rasterCubeArray;
    afxBool independentBlend;
    afxBool primShader;
    afxBool tessShader;
    afxBool sampleRateShading;
    afxBool dualSrcBlend;
    afxBool logicOp;
    afxBool multiDrawIndirect;
    afxBool drawIndirectFirstInst;
    afxBool depthClamp;
    afxBool depthBiasClamp;
    afxBool fillModeNonSolid;
    afxBool depthBounds;
    afxBool wideLines;
    afxBool largePoints;
    afxBool alphaToOne;
    afxBool multiViewport;
    afxBool samplerAnisotropy;
    afxBool etc2; // texture compression.
    afxBool astc_LDR; // texture compression.
    afxBool dxt; // texture compression.
    afxBool occlusionQueryPrecise;
    afxBool pipelineStatsQuery;
    afxBool vtxPipelineStoresAndAtomics;
    afxBool fragStoresAndAtomics;
    afxBool shaderTessAndPrimPointSiz;
    afxBool shaderRasterGatherExt;
    afxBool shaderStorageRasterExtFmts;
    afxBool shaderStorageRasterMultisample;
    afxBool shaderStorageRasterReadWithoutFmt;
    afxBool shaderStorageRasterWriteWithoutFmt;
    afxBool shaderUniformBufferArrayDynIndexing;
    afxBool shaderSampledRasterArrayDynIndexing;
    afxBool shaderStorageBufferArrayDynIndexing;
    afxBool shaderStorageImageArrayDynIndexing;
    afxBool shaderClipDist;
    afxBool shaderCullDist;
    afxBool shaderFloat64;
    afxBool shaderInt64;
    afxBool shaderInt16;
    afxBool shaderRsrcResidency;
    afxBool shaderRsrcMinLod;
    afxBool sparseBinding;
    afxBool sparseResidencyBuffer;
    afxBool sparseResidencyRaster2D;
    afxBool sparseResidencyRaster3D;
    afxBool sparseResidency2Samples;
    afxBool sparseResidency4Samples;
    afxBool sparseResidency8Samples;
    afxBool sparseResidency16Samples;
    afxBool sparseResidencyAliased;
    afxBool variableMultisampleRate;
    afxBool inheritedQueries;
};

AFX_DEFINE_STRUCT(afxDrawPortCaps)
{
    afxDrawBridgeFlags  queFlags;
    afxNat              queCnt;
};

#ifdef _AVX_DRAW_C
#ifdef _AVX_DRAW_DEVICE_C
AFX_DEFINE_STRUCT(afxDrawInputEndpoint)
{
    afxLinkage          ddev;
    afxChain            instances;
    afxMutex            mtx;
    afxCondition        cnd;
    afxString           name;
};

AFX_OBJECT(afxDrawDevice)
{
    AFX_OBJECT(afxDevice) dev;

    afxManager          contexts;
    afxChain            outputs;
    afxChain            inputs;
    afxChain            ineps;

    afxDrawDeviceCaps   caps;
    afxDrawDeviceLimits limits;
    afxNat              portCnt;
    struct
    {
        afxDrawPortCaps portCaps;
        afxManager      ddgeMgr;
        afxManager      dqueMgr;
    }*                  ports;
    
    afxClipSpace        clipCfg;

    afxBool             relinking;
    afxCondition        relinkedCnd;
    afxMutex            relinkedCndMtx;

    //afxError            (*procCb)(afxDrawDevice,afxThread); // call their draw threads.    
    afxError            (*iddDtorCb)(afxDrawDevice);
    afxError            (*dinIddDtorCb)(afxDrawDevice,afxDrawInput);
    afxError            (*dinIddCtorCb)(afxDrawDevice,afxDrawInput,afxDrawInputConfig const*,afxUri const*);
    afxError            (*dinRelinkCb)(afxDrawDevice,afxDrawContext,afxNat,afxDrawInput[]);
    afxError            (*doutIddDtorCb)(afxDrawDevice,afxDrawOutput);
    afxError            (*doutIddCtorCb)(afxDrawDevice,afxDrawOutput,afxDrawOutputConfig const*,afxUri const*);
    afxError            (*doutRelinkCb)(afxDrawDevice,afxDrawContext,afxNat,afxDrawOutput[]);

    struct _afxDdevIdd* idd;
};
#endif//_AVX_DRAW_DEVICE_C
#ifdef _AVX_DRAW_SYSTEM_C
AFX_OBJECT(afxDrawSystem)
{
    afxChain            managers;
    afxManager          ddevMgr;
    afxManager          doutEndMgr;
    afxManager          doutMgr;
    afxManager          dinEndMgr;
    afxManager          dinMgr;

    afxManager          camMgr;
};
#endif//_AVX_DRAW_SYSTEM_C
#endif//_AVX_DRAW_C

////////////////////////////////////////////////////////////////////////////////

AVX afxReal64       AfxFindAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam);

/// If you don't know what the physical aspect ratio is of the device you're using 
/// (for example, if you're using a standard PC, there is no way to determine for sure what kind of monitor is attached), 
/// you can either assume square pixels (pass the width of the screen divided by the height), or you can use Qwadro's "best guess": 

AVX afxReal64       AfxFindPhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight);

AVX afxManager*     AfxGetDrawDeviceManager(void);
AVX afxManager*     AfxGetDrawOutputClass(void);
AVX afxManager*     AfxGetDrawInputClass(void);
AVX afxManager*     AfxGetCameraClass(void);

AVX afxNat          AfxCountDrawDevices(void);
AVX afxNat          AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void* udd);
AVX afxNat          AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice devices[]);

AVX afxNat          AfxCountCameras(void);
AVX afxNat          AfxInvokeCameras(afxNat first, afxNat cnt, afxBool(*f)(afxCamera, void*), void* udd);
AVX afxNat          AfxEnumerateCameras(afxNat first, afxNat cnt, afxCamera cameras[]);

AVX afxBool         AfxGetDrawDevice(afxNat ddevId, afxDrawDevice* device);

AVX afxNat          AfxChooseDrawDevices(afxDrawDeviceCaps const* caps, afxDrawDeviceLimits const* limits, afxNat maxCnt, afxNat ddevId[]); // return count of found devices
AVX afxNat          AfxChooseDrawOutputEndpoint(afxNat ddevId, afxDrawOutputCaps const* caps, afxNat maxCnt, afxNat endpointIdx[]);
AVX afxNat          AfxChooseDrawInputEndpoint(afxNat ddevId, afxNat maxCnt, afxNat endpointIdx[]);

////////////////////////////////////////////////////////////////////////////////
// DRAW DEVICE                                                                //
////////////////////////////////////////////////////////////////////////////////

AVX afxBool         AfxDrawDeviceIsRunning(afxDrawDevice ddev);
AVX void*           AfxGetDrawDeviceIdd(afxDrawDevice ddev);
AVX afxManager*     AfxGetDrawBridgeClass(afxDrawDevice ddev, afxNat portIdx);
AVX afxManager*     AfxGetDrawQueueClass(afxDrawDevice ddev, afxNat portIdx);
AVX afxManager*     AfxGetDrawContextClass(afxDrawDevice ddev);

/// Test the capabilities and limits of a draw device.
AVX afxBool         AfxIsDrawDeviceAcceptable(afxDrawDevice ddev, afxDrawDeviceCaps const* caps, afxDrawDeviceLimits const* limits);

AVX afxNat          AfxCountDrawPorts(afxDrawDevice ddev);
AVX void            AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawDeviceLimits* limits);
AVX void            AfxGetDrawPortCapabilities(afxDrawDevice ddev, afxNat portIdx, afxDrawPortCaps* caps);
AVX void            AfxGetDrawDeviceCapabilities(afxDrawDevice ddev, afxDrawDeviceCaps* caps);

AVX afxNat          AfxCountDrawContexts(afxDrawDevice ddev);
AVX afxNat          AfxInvokeDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd);
AVX afxNat          AfxEnumerateDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawContext contexts[]);

AVX afxNat          AfxCountDrawOutputs(afxDrawDevice ddev);
AVX afxNat          AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AVX afxNat          AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput outputs[]);

AVX afxNat          AfxCountDrawInputs(afxDrawDevice ddev);
AVX afxNat          AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);
AVX afxNat          AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput inputs[]);

#endif//AVX_DRAW_SYSTEM_H
