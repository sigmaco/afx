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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_ASSET_H
#define AFX_ASSET_H

#include "afx/core/afxClass.h"
#include "afx/sim/afxEntity.h"
#include "afx/core/afxSource.h"
#include "afx/sim/rendering/afxLight.h"
#include "afx/sim/rendering/afxCamera.h"
#include "afx/sim/afxSkeleton.h"
#include "afx/sim/modeling/afxModel.h"
#include "afx/sim/afxMaterial.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/sim/anim/afxAnimator.h"
#include "afx/sim/rendering/afxRenderer.h"
#include "afx/sim/anim/afxAnimation.h"

AFX_DEFINE_STRUCT(afxCadToolInfo)
{
    afxString const*    name; // from art tool name
    afxNat              majorRev;
    afxNat              minorRev;
    afxV3d              right; // X-vector (aka right) of art tool.
    afxV3d              up; // Y-vector (aka up) of art tool.
    afxV3d              at; // Z-vector (aka at) of art tool.
    afxV3d              origin;
    afxReal             unitsPerMeter;
    void*               udd;
};

AFX_DEFINE_STRUCT(afxCadExporterInfo)
{
    afxString const*    name;
    afxNat              majorRev;
    afxNat              minorRev;
    afxNat              customization;
    afxNat              buildNum;
    void*               udd;
};

AFX_DEFINE_HANDLE(afxAsset);

AFX_DEFINE_STRUCT(afxCadResource)
{
    afxAsset        asset;
    afxUri          name; // resource name
    void*           obj;
};

AFX_DEFINE_STRUCT(afxResourceSlot)
{
    afxString32     id;
    void*           obj;
    afxUri          uri;
};

AFX_OBJECT(afxAsset)
{
    afxUri              file;
    afxCadToolInfo*     toolInfo;
    afxCadExporterInfo* exporterInfo;

    afxNat              resCap;
    afxResourceSlot*    resSlots;
    struct
    {
        afxFcc          fcc;
        afxNat          baseResIdx;
        afxNat          resCnt;
    }                  *nests;
    afxNat              nestCnt;
};

AFX_DEFINE_STRUCT(afxAssetBuilder)
{
    void(*GetInfo)(void* data, afxNat *typeCnt, afxNat* resCnt, afxUri* name);
    void(*GetTypeInfo)(void* data, afxNat setIdx, afxFcc* resType, afxNat* resCnt);
    void*(*GetResourceInfo)(void* data, afxFcc type, afxNat resIdx, afxUri* name);
    afxBool(*GetToolInfo)(void* data, afxCadToolInfo* toolInfo, afxCadExporterInfo* exporterInfo);
    afxError(*AddResources)(void* data, afxFcc type, afxUri const* name);
};

AFX afxNat              AfxFindResourceNests(afxAsset cad, afxNat cnt, afxFcc const fcc[], afxNat nestIdx[]);
AFX afxNat              AfxFindResources(afxAsset cad, afxFcc fcc, afxNat cnt, afxString const id[], void* res[]);
AFX afxNat              AfxCountResources(afxAsset cad, afxFcc fcc);

AFX afxNat              AfxFindTextures(afxAsset cad, afxNat cnt, afxString const id[], afxRaster tex[]);
AFX afxNat              AfxFindMaterials(afxAsset cad, afxNat cnt, afxString const id[], afxMaterial mt[]);
AFX afxNat              AfxFindVertexDatas(afxAsset cad, afxNat cnt, afxString const id[], afxVertexData vtd[]);
AFX afxNat              AfxFindTopologies(afxAsset cad, afxNat cnt, afxString const id[], afxMeshTopology msht[]);
AFX afxNat              AfxFindMeshes(afxAsset cad, afxNat cnt, afxString const id[], afxMesh msh[]);
AFX afxNat              AfxFindSkeletons(afxAsset cad, afxNat cnt, afxString const id[], afxSkeleton skl[]);
AFX afxNat              AfxFindModels(afxAsset cad, afxNat cnt, afxString const id[], afxModel mdl[]);
AFX afxNat              AfxFindAnimations(afxAsset cad, afxNat cnt, afxString const id[], afxAnimation anim[]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxAcquireAssets(afxSimulation sim, afxNat cnt, afxNat const nestCnt[], afxNat const resCap[], afxAsset cad[]);
AFX afxError            AfxBuildAssets(afxSimulation sim, afxAssetBuilder const* cadb, afxNat cnt, void* data[], afxAsset cad[]);

AFX afxError            AfxLoadAssets(afxSimulation sim, afxError(*load)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);
AFX afxError            AfxStoreAssets(afxSimulation sim, afxError(*store)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);

AFX void                AfxTransformAssets(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, afxAsset cad[]);

#endif//AFX_ASSET_H
