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

#include "qwadro/core/afxClass.h"
#include "qwadro/sim/awxEntity.h"
#include "qwadro/io/afxSource.h"
#include "qwadro/sim/rendering/awxLight.h"
#include "qwadro/draw/afxCamera.h"
#include "qwadro/sim/awxSkeleton.h"
#include "qwadro/sim/modeling/awxModel.h"
#include "qwadro/sim/awxMaterial.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/sim/anim/awxMotor.h"
#include "qwadro/sim/rendering/awxRenderer.h"
#include "qwadro/sim/anim/awxAnimation.h"

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

AFX_DEFINE_HANDLE(awxAsset);

AFX_DEFINE_STRUCT(afxCadResource)
{
    awxAsset        asset;
    afxUri          name; // resource name
    void*           obj;
};

AFX_DEFINE_STRUCT(afxResourceSlot)
{
    afxFixedString32     id;
    void*           obj;
    afxUri          uri;
};

AFX_OBJECT(awxAsset)
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

AFX_DEFINE_STRUCT(awxAssetBuilder)
{
    void(*GetInfo)(void* data, afxNat *typeCnt, afxNat* resCnt, afxUri* name);
    void(*GetTypeInfo)(void* data, afxNat setIdx, afxFcc* resType, afxNat* resCnt);
    void*(*GetResourceInfo)(void* data, afxFcc type, afxNat resIdx, afxUri* name);
    afxBool(*GetToolInfo)(void* data, afxCadToolInfo* toolInfo, afxCadExporterInfo* exporterInfo);
    afxError(*AddResources)(void* data, afxFcc type, afxUri const* name);
};

AFX afxNat              AfxFindResourceNests(awxAsset cad, afxNat cnt, afxFcc const fcc[], afxNat nestIdx[]);
AFX afxNat              AfxFindResources(awxAsset cad, afxFcc fcc, afxNat cnt, afxString const id[], void* res[]);
AFX afxNat              AfxCountResources(awxAsset cad, afxFcc fcc);

AFX afxNat              AfxFindTextures(awxAsset cad, afxNat cnt, afxString const id[], afxRaster tex[]);
AFX afxNat              AfxFindMaterials(awxAsset cad, afxNat cnt, afxString const id[], awxMaterial mt[]);
AFX afxNat              AfxFindVertexDatas(awxAsset cad, afxNat cnt, afxString const id[], awxVertexData vtd[]);
AFX afxNat              AfxFindTopologies(awxAsset cad, afxNat cnt, afxString const id[], awxMeshTopology msht[]);
AFX afxNat              AfxFindMeshes(awxAsset cad, afxNat cnt, afxString const id[], awxMesh msh[]);
AFX afxNat              AfxFindSkeletons(awxAsset cad, afxNat cnt, afxString const id[], awxSkeleton skl[]);
AFX afxNat              AfxFindModels(awxAsset cad, afxNat cnt, afxString const id[], awxModel mdl[]);
AFX afxNat              AfxFindAnimations(awxAsset cad, afxNat cnt, afxString const id[], awxAnimation anim[]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AfxAcquireAssets(afxSimulation sim, afxNat cnt, afxNat const nestCnt[], afxNat const resCap[], awxAsset cad[]);
AFX afxError            AfxBuildAssets(afxSimulation sim, awxAssetBuilder const* cadb, afxNat cnt, void* data[], awxAsset cad[]);

AFX afxError            AfxLoadAssets(afxSimulation sim, afxError(*load)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);
AFX afxError            AfxStoreAssets(afxSimulation sim, afxError(*store)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);

AFX void                AfxTransformAssets(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, awxAsset cad[]);

#endif//AFX_ASSET_H
