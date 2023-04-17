/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_DRAW_DRIVER_H
#define AFX_DRAW_DRIVER_H

#include "afxDrawInput.h"
#include "afxDrawOutput.h"
#include "pipelining/afxPipeline.h"
#include "afxDrawEngine.h"

AFX_DEFINE_STRUCT(afxDrawDriverFeatures)
{
    afxBool robustBufferAccess;
    afxBool fullDrawIndexUint32;
    afxBool imageCubeArray;
    afxBool independentBlend;
    afxBool geometryShader;
    afxBool tessellationShader;
    afxBool sampleRateShading;
    afxBool dualSrcBlend;
    afxBool logicOp;
    afxBool multiDrawIndirect;
    afxBool drawIndirectFirstInstance;
    afxBool depthClamp;
    afxBool depthBiasClamp;
    afxBool fillModeNonSolid;
    afxBool depthBounds;
    afxBool wideLines;
    afxBool largePoints;
    afxBool alphaToOne;
    afxBool multiViewport;
    afxBool samplerAnisotropy;
    afxBool textureCompressionETC2;
    afxBool textureCompressionASTC_LDR;
    afxBool textureCompressionBC;
    afxBool occlusionQueryPrecise;
    afxBool pipelineStatisticsQuery;
    afxBool vertexPipelineStoresAndAtomics;
    afxBool fragmentStoresAndAtomics;
    afxBool shaderTessellationAndGeometryPointSize;
    afxBool shaderImageGatherExtended;
    afxBool shaderStorageImageExtendedFormats;
    afxBool shaderStorageImageMultisample;
    afxBool shaderStorageImageReadWithoutFormat;
    afxBool shaderStorageImageWriteWithoutFormat;
    afxBool shaderUniformBufferArrayDynamicIndexing;
    afxBool shaderSampledImageArrayDynamicIndexing;
    afxBool shaderStorageBufferArrayDynamicIndexing;
    afxBool shaderStorageImageArrayDynamicIndexing;
    afxBool shaderClipDistance;
    afxBool shaderCullDistance;
    afxBool shaderFloat64;
    afxBool shaderInt64;
    afxBool shaderInt16;
    afxBool shaderResourceResidency;
    afxBool shaderResourceMinLod;
    afxBool sparseBinding;
    afxBool sparseResidencyBuffer;
    afxBool sparseResidencyImage2D;
    afxBool sparseResidencyImage3D;
    afxBool sparseResidency2Samples;
    afxBool sparseResidency4Samples;
    afxBool sparseResidency8Samples;
    afxBool sparseResidency16Samples;
    afxBool sparseResidencyAliased;
    afxBool variableMultisampleRate;
    afxBool inheritedQueries;
};

AFX_DEFINE_HANDLE(afxDrawDriver);

AFX_DEFINE_STRUCT(afxDrawDriverSpecification)
{
    afxString const*            name;
    afxString const*            author;
    afxString const*            website;
    afxString const*            note;
    afxNat                      verMajor;
    afxNat                      verMinor;
    afxNat                      verPatch;
    afxDrawDriverFeatures const*features;
    afxNat                      dctxSiz;
    afxNat                      dctxMaxCnt;
    afxError                    (*dctxCtor)(afxDrawContext dctx, void *args);
    afxError                    (*dctxDtor)(afxDrawContext dctx);
    void const*                 dctxVmt;
    afxError                    (*process)(afxDrawDriver ddrv); // things to be processed on main thread
    void*                       idd;
    afxError                    (*iddDtor)(afxDrawDriver ddrv);
};

#ifndef AFX_DRAW_SRC

AFX_OBJECT(afxDrawDriver) { afxObject obj; };

#endif

AFX afxError    AfxDrawDriverGetFeatures(afxDrawDriver ddrv, afxDrawDriverFeatures *features);

AFX afxNat      AfxDrawDriverGetContextCount(afxDrawDriver ddrv);
AFX afxResult   AfxDrawDriverEnumerateContexts(afxDrawDriver ddrv, afxNat base, afxNat cnt, afxDrawContext dctx[]);
AFX afxClass*   AfxDrawDriverGetContextClass(afxDrawDriver ddrv);

AFX void*       AfxDrawDriverGetDrawSystem(afxDrawDriver ddrv);

AFX afxError    _AfxDrawDriverProcess(afxDrawDriver ddrv); // Called by draw system. Reserved for enginners at SIGMA.

#endif//AFX_DRAW_DRIVER_H