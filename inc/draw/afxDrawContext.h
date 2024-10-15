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

// A draw context is created as a connection to a draw device.

#ifndef AVX_DRAW_CONTEXT_H
#define AVX_DRAW_CONTEXT_H

#include "qwadro/inc/draw/io/afxBuffer.h"
#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/draw/pipe/avxCanvas.h"
#include "qwadro/inc/draw/pipe/avxLigature.h"
#include "qwadro/inc/draw/pipe/avxPipeline.h"
#include "qwadro/inc/draw/pipe/avxQueryPool.h"
#include "qwadro/inc/draw/pipe/avxRasterizer.h"
#include "qwadro/inc/draw/pipe/avxSampler.h"
#include "qwadro/inc/draw/pipe/avxShader.h"
#include "qwadro/inc/draw/pipe/avxVertexInput.h"
#include "qwadro/inc/draw/pipe/avxDrawOps.h"
//#include "qwadro/inc/draw/afxDrawOutput.h"
//#include "qwadro/inc/draw/afxDrawInput.h"
//#include "qwadro/inc/math/avxMatrix.h"
//#include "qwadro/inc/base/afxClass.h"
//#include "qwadro/inc/mem/afxArena.h"

AFX_DEFINE_STRUCT(afxDrawFeatures)
// Draw features are various capabilities or functionalities that are provided to facilitate the drawing and rendering of objects on the screen. 
// These features are essential for creating and controlling the visual output in graphical applications, such as games, simulations, or visualizations.
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

AFX_DEFINE_STRUCT(afxDrawContextConfig)
{
    afxDrawBridgeConfig mainBridge;
    afxNat              auxBridgeCnt;
    afxDrawBridgeConfig*auxBridges;

    afxDrawFeatures     requirements;
    afxNat              extensionCnt;
    afxString const*    extensions;
    void*               udd;
};

AVX afxDrawDevice   AfxGetDrawContextDevice(afxDrawContext dctx);

AVX void            AfxQueryDrawBridges(afxDrawContext dctx, afxNat portId, afxNat* baseExuIdx, afxNat* exuCnt);

AVX afxNat          AfxGetDrawBridges(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawBridge bridges[]);
AVX afxNat          AfxGetDrawQueues(afxDrawContext dctx, afxNat first, afxNat cnt, afxDrawQueue queues[]);

AVX void            AfxCountDrawContextConnections(afxDrawContext dctx, afxNat* inputCnt, afxNat* outputCnt);
AVX afxError        AfxDisconnectDrawContext(afxDrawContext dctx, afxBool keepInputs, afxBool keepOutputs);

AVX afxBool         AfxGetConnectedDrawInput(afxDrawContext dctx, afxNat conNo, afxDrawInput* input);
AVX afxNat          AfxGetConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat cnt, afxDrawInput inputs[]);
AVX afxNat          AfxInvokeConnectedDrawInputs(afxDrawContext dctx, afxNat baseCon, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);

AVX afxBool         AfxGetConnectedDrawOutput(afxDrawContext dctx, afxNat conNo, afxDrawOutput* output);
AVX afxNat          AfxGetConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat cnt, afxDrawOutput outputs[]);
AVX afxNat          AfxInvokeConnectedDrawOutputs(afxDrawContext dctx, afxNat baseCon, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);

AVX afxClass const* AfxGetBufferClass(afxDrawContext dctx);
AVX afxClass*       AfxGetCanvasClass(afxDrawContext dctx);
AVX afxClass*       AfxGetLigatureClass(afxDrawContext dctx);
AVX afxClass*       AfxGetPipelineClass(afxDrawContext dctx);
AVX afxClass*       AfxGetQueryPoolClass(afxDrawContext dctx);
AVX afxClass*       AfxGetRasterClass(afxDrawContext dctx);
AVX afxClass*       AfxGetRasterizerClass(afxDrawContext dctx);
AVX afxClass*       AfxGetSamplerClass(afxDrawContext dctx);
AVX afxClass*       AfxGetShaderClass(afxDrawContext dctx);
AVX afxClass*       AfxGetVertexInputClass(afxDrawContext dctx);

AVX afxBool         AfxGetShaderStringBase(afxDrawContext dctx, afxStringBase* base);


/// Open a queue debug label region.
/// Close a queue debug label region.
/// Insert a label into a queue.

AVX void            AfxBeginDrawQueueDebugScope(afxDrawContext dctx, afxNat exuIdx, afxNat queIdx, afxString const* name, afxColor const color);
AVX void            AfxPushDrawQueueDebugLabel(afxDrawContext dctx, afxNat exuIdx, afxNat queIdx, afxString const* name, afxColor const color);
AVX void            AfxEndDrawQueueDebugScope(afxDrawContext dctx, afxNat exuIdx, afxNat queIdx);

/// Wait for a device context to become idle.
/// Wait for a bridge to become idle. To wait on the host for the completion of outstanding queue operations for a given bridge.
/// Wait for a queue to become idle. To wait on the host for the completion of outstanding queue operations for a given queue.

AVX afxError        AfxWaitForDrawContext(afxDrawContext dctx, afxTime timeout);
AVX afxError        AfxWaitForDrawBridge(afxDrawContext dctx, afxNat exuIdx, afxTime timeout);
AVX afxError        AfxWaitForDrawQueue(afxDrawContext dctx, afxNat queIdx, afxTime timeout);

AVX afxError        AfxSubmitDrawCommands(afxDrawContext dctx, avxSubmission* ctrl, afxNat cnt, avxCmdb cmdb[]);

AVX afxError        AfxPresentDrawOutputBuffers(afxDrawContext dctx, avxPresentation* ctrl, afxNat cnt, afxDrawOutput outputs[], afxNat const bufIdx[]);

#endif//AVX_DRAW_CONTEXT_H
