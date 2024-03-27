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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_ASSET_H
#define AFX_ASSET_H

#include "qwadro/core/afxManager.h"
#include "qwadro/sim/awxEntity.h"
#include "qwadro/io/afxSource.h"
#include "qwadro/sim/rendering/awxLight.h"
#include "qwadro/draw/afxCamera.h"
#include "qwadro/sim/afxSkeleton.h"
#include "qwadro/sim/modeling/afxModel.h"
#include "qwadro/sim/afxMaterial.h"
#include "qwadro/draw/afxDrawInput.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/sim/motion/awxMotor.h"
#include "qwadro/sim/rendering/awxRenderer.h"
#include "qwadro/sim/motion/awxAnimation.h"

#pragma pack(push, 1)
AFX_DEFINE_STRUCT(_afxMddFileData)
/// Modelling diagram dictionary
{
    afxNat  mdlCnt;
    afxSize mdlDirBaseOffset;
    afxNat  sklCnt;
    afxSize sklDirBaseOffset;
    afxNat  mshCnt;
    afxSize mshDirBaseOffset;
    afxNat  mshtCnt;
    afxSize mshtDirBaseOffset;
    afxNat  vtdCnt;
    afxSize vtdDirBaseOffset;
};

AFX_DEFINE_STRUCT(_afxMtdFileData)
/// Material template dictionary
{
    afxNat  mtlCnt;
    afxSize mtlDirBaseOffset;
    afxNat  texCnt;
    afxSize texDirBaseOffset;
};

AFX_DEFINE_STRUCT(_afxSerializedChunk)
{
    afxFcc          fcc;
    afxNat          len;
    //afxNat          cnt; // if nested it is more than 1; // variable length of instances did it unusable
};

AFX_DEFINE_STRUCT(_afxSerializedStr)
{
    afxSize         start;
    afxNat          len;
};

AFX_DEFINE_STRUCT(_afxSerializedMdl)
{
    afxString       name;
    afxNat          sklIdx;
    afxTransform    init;
    afxNat          mshCnt;
    // followed by sizeof(afxNat mshIdx[mshCnt]);
};

AFX_DEFINE_STRUCT(_afxSerializedMsh)
{
    afxString       name;
    afxNat          vtdIdx;
    afxNat          mshtIdx;
    afxNat          mtlCnt;
    afxNat          pivotCnt;
    // followed by sizeof(afxNat pivotNameStrIdx[pivotCnt]);
};

AFX_DEFINE_STRUCT(_afxSerializedMsht)
{
    afxNat          triCnt;
    afxNat          surCnt;
    afxSize         firstTriOffset;
    afxSize         firstSurOffset;
    // followed by sizeof(afxMeshSurface surfs[surCnt]);
    // followed by sizeof(afxNat tris[triCnt]);
};

AFX_DEFINE_STRUCT(_afxSerializedVtxa)
{
    afxNat          biasCnt;
    afxNat          vtxCnt;
    afxNat          attrCnt;
};

AFX_DEFINE_STRUCT(_afxSerializedVtd)
{
    afxNat          biasCnt;
    afxNat          vtxCnt;
    afxNat          attrCnt;
    afxSize         firstBiasOffset;
    afxSize         firstVtxOffset;
    // followed by sizeof(afxVertexBias biases[surCnt]);
    // followed by sizeof(afxVertex vertices[triCnt]);
};

AFX_DEFINE_STRUCT(_afxSerializedSkl)
{
    afxString name;
    afxNat lodType;
    afxNat jointCnt;
    afxSize firstBoneOffset;
    afxSize firstBoneNameOfffset;
    // followed by sizeof(afxNat lt[boneCnt]);
    // followed by sizeof(afxNat iw[boneCnt]);
    // followed by sizeof(afxNat parIdx[boneCnt]);
    // followed by sizeof(afxNat lodErr[boneCnt]);
    // followed by sizeof(afxString boneNames[boneCnt]);
};
#pragma pack(pop)

AFX_DEFINE_STRUCT(afxAssertReference)
{
    afxNat              type;
    afxUri const*       file;
};

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
    afxString32     id;
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

AKX afxNat              AfxFindResourceNests(awxAsset cad, afxNat cnt, afxFcc const fcc[], afxNat nestIdx[]);
AKX afxNat              AfxFindResources(awxAsset cad, afxFcc fcc, afxNat cnt, afxString const id[], void* res[]);
AKX afxNat              AfxCountResources(awxAsset cad, afxFcc fcc);

AKX afxNat              AfxFindTextures(awxAsset cad, afxNat cnt, afxString const id[], afxRaster tex[]);
AKX afxNat              AfxFindMaterials(awxAsset cad, afxNat cnt, afxString const id[], afxMaterial mt[]);
AKX afxNat              AfxFindVertexDatas(awxAsset cad, afxNat cnt, afxString const id[], awxVertexData vtd[]);
AKX afxNat              AfxFindTopologies(awxAsset cad, afxNat cnt, afxString const id[], afxMeshTopology msht[]);
AKX afxNat              AfxFindMeshes(awxAsset cad, afxNat cnt, afxString const id[], afxMesh msh[]);
AKX afxNat              AfxFindSkeletons(awxAsset cad, afxNat cnt, afxString const id[], afxSkeleton skl[]);
AKX afxNat              AfxFindModels(awxAsset cad, afxNat cnt, afxString const id[], afxModel mdl[]);
AKX afxNat              AfxFindAnimations(awxAsset cad, afxNat cnt, afxString const id[], awxAnimation anim[]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AKX afxError            AfxAcquireAssets(afxSimulation sim, afxNat cnt, afxNat const nestCnt[], afxNat const resCap[], awxAsset cad[]);
AKX afxError            AfxBuildAssets(afxSimulation sim, awxAssetBuilder const* cadb, afxNat cnt, void* data[], awxAsset cad[]);

AKX afxError            AfxLoadAssets(afxSimulation sim, afxError(*load)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);
AKX afxError            AfxStoreAssets(afxSimulation sim, afxError(*store)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);

AKX void                AfxTransformAssets(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, awxAsset cad[]);

#endif//AFX_ASSET_H
