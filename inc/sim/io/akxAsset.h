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

#ifndef AMX_ASSET_H
#define AMX_ASSET_H

#include "qwadro/inc/base/afxClass.h"
#include "qwadro/inc/io/afxSource.h"
#include "qwadro/inc/sim/rendering/akxLighting.h"
#include "qwadro/inc/cad/afxSkeleton.h"
#include "qwadro/inc/cad/afxModel.h"
#include "qwadro/inc/cad/afxMaterial.h"
#include "qwadro/inc/sim/afxMotor.h"
#include "qwadro/inc/sim/rendering/akxRenderer.h"
#include "qwadro/inc/sim/afxAnimation.h"

// .mdd --- model dictionary
// .txd --- texture dictionary
// .and --- animation dictionary

/*
    { AFX FILE HEADER }
    rasCnt
    texCnt
    array of [texCnt] name strings.
    TEXTURE INFO 0 { mapCnt, mapRasIdx[mapCnt], mapUsage, color }
    TEXTURE INFO 1
    TEXTURE INFO #
    RASTER INFO 0 // albedo/diffuse
    RASTER INFO 1 // _n --- normal (aka bump)
    RASTER INFO 2 // _s --- specular
    RASTER INFO 3 // _a --- alpha (or opacity) (if separated)
    RASTER INFO 4 // _g --- roughness (aka gloss) (PBR)
    RASTER INFO 5 // _m --- metalness (PBR)
    RASTER INFO 6 // _h --- height (aka displacement)
    RASTER INFO 7 // _o --- ambient occlusion
    RASTER INFO 8 // _r --- refraction
    RASTER INFO #
    // --- seg for each tex
    // seg0
    tex0 RASTER DATA
    // seg1
    tex1 RASTER DATA
*/

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(_afxStrChunk)
{
    afxNat  len;
    afxSize start;
};

AFX_DEFINE_STRUCT(_afxTxdChunk)
{
    afxNat  texCnt;
    afxNat  texUrns;
};

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
    // followed by sizeof(akxVertex vertices[triCnt]);
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

AFX_DEFINE_HANDLE(akxAsset);

AFX_DEFINE_STRUCT(afxCadResource)
{
    akxAsset        asset;
    afxUri          name; // resource name
    void*           obj;
};

AFX_DEFINE_STRUCT(afxResourceSlot)
{
    afxString32     id;
    void*           obj;
    afxUri          uri;
};

AFX_OBJECT(akxAsset)
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

AFX_DEFINE_STRUCT(akxAssetBuilder)
{
    void(*GetInfo)(void* data, afxNat *typeCnt, afxNat* resCnt, afxUri* name);
    void(*GetTypeInfo)(void* data, afxNat setIdx, afxFcc* resType, afxNat* resCnt);
    void*(*GetResourceInfo)(void* data, afxFcc type, afxNat resIdx, afxUri* name);
    afxBool(*GetToolInfo)(void* data, afxCadToolInfo* toolInfo, afxCadExporterInfo* exporterInfo);
    afxError(*AddResources)(void* data, afxFcc type, afxUri const* name);
};

AMX afxNat              AfxFindResourceNests(akxAsset cad, afxNat cnt, afxFcc const fcc[], afxNat nestIdx[]);
AMX afxNat              AfxFindResources(akxAsset cad, afxFcc fcc, afxNat cnt, afxString const id[], void* res[]);
AMX afxNat              AfxCountResources(akxAsset cad, afxFcc fcc);

AMX afxNat              AfxFindTextures(akxAsset cad, afxNat cnt, afxString const id[], afxRaster tex[]);
AMX afxNat              AfxFindMaterials(akxAsset cad, afxNat cnt, afxString const id[], afxMaterial mt[]);
AMX afxNat              AfxFindVertexDatas(akxAsset cad, afxNat cnt, afxString const id[], afxGeometry vtd[]);
AMX afxNat              AfxFindTopologies(akxAsset cad, afxNat cnt, afxString const id[], afxMeshTopology msht[]);
AMX afxNat              AfxFindMeshes(akxAsset cad, afxNat cnt, afxString const id[], afxMesh msh[]);
AMX afxNat              AfxFindSkeletons(akxAsset cad, afxNat cnt, afxString const id[], afxSkeleton skl[]);
AMX afxNat              AfxFindModels(akxAsset cad, afxNat cnt, afxString const id[], afxModel mdl[]);
AMX afxNat              AfxFindAnimations(akxAsset cad, afxNat cnt, afxString const id[], afxAnimation anim[]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AMX afxError            AfxAcquireAssets(afxSimulation sim, afxNat cnt, afxNat const nestCnt[], afxNat const resCap[], akxAsset cad[]);
AMX afxError            AfxBuildAssets(afxSimulation sim, akxAssetBuilder const* cadb, afxNat cnt, void* data[], akxAsset cad[]);

AMX afxError            AfxLoadAssets(afxSimulation sim, afxError(*load)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);
AMX afxError            AfxStoreAssets(afxSimulation sim, afxError(*store)(afxUri const* file, afxFlags flags), afxFlags flags, afxNat cnt, afxUri const file[]);

AMX void                AfxTransformAssets(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, akxAsset cad[]);


AMX afxError            AfxDeserializeSkeletons(afxStream in, afxSimulation sim, afxNat cnt, afxSkeleton skl[]);
AMX afxError            AfxSerializeSkeletons(afxStream out, afxNat cnt, afxSkeleton const skl[]);

#endif//AMX_ASSET_H
