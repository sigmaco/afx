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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

// SIGMA bring back to you the old "bring your own driver" paradigm.

#ifndef AFX_DRAW_DRIVER_H
#define AFX_DRAW_DRIVER_H

#include "afx/draw/afxDrawContext.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/draw/afxDrawInput.h"

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

AFX_DEFINE_STRUCT(afxDrawOutputCaps)
{
    afxNat              minBufCnt;
    afxNat              maxBufCnt;
    afxNat              currentExtent[2];
    afxNat              minExtent[2];
    afxNat              maxExtent[2];
    afxNat              maxLayerCnt;
    afxPresentTransform supportedTransforms;
    afxPresentTransform currentTransform;
    afxPresentAlpha     supportedCompositeAlpha;
    afxTextureFlags     supportedUsageFlags;
};

AFX_DECLARE_STRUCT(_afxDdrvVmt);

AFX_DEFINE_STRUCT(afxDrawDriverSpecification)
{
    afxModule                   mdle;
    afxString const*            slug; // sgl
    afxString const*            name; // driver name: SIGMA GL
    afxString const*            author; // author: SIGMA Technology Group (The technology arm of Federação SIGMA)
    afxString const*            website; // website: sigmaco.org
    afxString const*            note; // The standard QWADRO draw system implementation.
    afxNat                      verMajor; // 1
    afxNat                      verMinor; // 0
    afxNat                      verPatch; // 0
    afxDrawDriverFeatures const*features;
    afxNat                      execPortCnt;
    afxError                    (*ctor)(afxDrawDriver,afxDrawDriverSpecification const*);
};

AFX_OBJECT(afxDrawDriver)
{
    afxObject                   obj;
    _afxDdrvVmt const*          vmt;
    void*                       idd;
#ifdef _AFX_DRAW_DRIVER_C
    afxChain                    provisions;
    afxClass                    dscrClass;
    afxClass                    dqueClass;
    afxClass                    dctxClass;
    afxClass                    doutClass;
    afxClass                    dinClass;

    afxModule                   mdle;

    afxString                   name; // driver name: SIGMA GL
    afxString                   author; // author: SIGMA Technology Group (The technology arm of Federação SIGMA)
    afxString                   website; // website: sigmaco.org
    afxString                   note; // The standard QWADRO draw system implementation.
    afxNat                      verMajor; // 1
    afxNat                      verMinor; // 0
    afxNat                      verPatch; // 0

    afxDrawDriverFeatures*      features;

    afxNat                      portCnt;
    afxDrawQueueFlags*          portCaps;
    afxBool                     serving;
#endif
};

AFX afxModule               AfxGetDrawDriverModule(afxDrawDriver ddrv);
AFX void                    AfxGetDrawDriverFeatures(afxDrawDriver ddrv, afxDrawDriverFeatures *features);
AFX afxDrawQueueFlags       AfxGetDrawDriverPortCaps(afxDrawDriver ddrv, afxNat portIdx);
AFX afxNat                  AfxGetDrawDriverPortCount(afxDrawDriver ddrv);
AFX afxError                AfxAcquireDrawQueues(afxDrawDriver ddrv, afxDrawQueueSpecification const *spec, afxNat cnt, afxDrawQueue dque[]);

AFX afxNat                  AfxEnumerateDrawScripts(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawScript dscr[]);
AFX afxNat                  AfxEnumerateDrawQueues(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawQueue dque[]);
AFX afxNat                  AfxEnumerateDrawContexts(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawContext dctx[]);
AFX afxNat                  AfxEnumerateDrawOutputs(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawOutput dout[]);
AFX afxNat                  AfxEnumerateDrawInputs(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxDrawInput din[]);

AFX afxNat                  AfxCurateDrawContexts(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd);
AFX afxNat                  AfxCurateDrawOutputs(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AFX afxNat                  AfxCurateDrawInputs(afxDrawDriver ddrv, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);

AFX afxNat                  AfxGetDrawContextCount(afxDrawDriver ddrv);
AFX afxNat                  AfxGetDrawOutputCount(afxDrawDriver ddrv);
AFX afxNat                  AfxGetDrawInputCount(afxDrawDriver ddrv);

AFX afxClass*               AfxGetDrawScriptClass(afxDrawDriver ddrv);
AFX afxClass*               AfxGetDrawQueueClass(afxDrawDriver ddrv);
AFX afxClass*               AfxGetDrawContextClass(afxDrawDriver ddrv);
AFX afxClass*               AfxGetDrawOutputClass(afxDrawDriver ddrv);
AFX afxClass*               AfxGetDrawInputClass(afxDrawDriver ddrv);

AFX afxBool                 AfxDrawDriverIsRunning(afxDrawDriver ddrv);

#endif//AFX_DRAW_DRIVER_H