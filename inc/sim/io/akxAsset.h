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
    afxUnit  len;
    afxSize start;
};

AFX_DEFINE_STRUCT(_afxTxdChunk)
{
    afxUnit  texCnt;
    afxUnit  texUrns;
};

AFX_DEFINE_STRUCT(_afxMddFileData)
/// Modelling diagram dictionary
{
    afxUnit  mdlCnt;
    afxSize mdlDirBaseOffset;
    afxUnit  sklCnt;
    afxSize sklDirBaseOffset;
    afxUnit  mshCnt;
    afxSize mshDirBaseOffset;
    afxUnit  mshtCnt;
    afxSize mshtDirBaseOffset;
    afxUnit  vtdCnt;
    afxSize vtdDirBaseOffset;
};

AFX_DEFINE_STRUCT(_afxMtdFileData)
/// Material template dictionary
{
    afxUnit  mtlCnt;
    afxSize mtlDirBaseOffset;
    afxUnit  texCnt;
    afxSize texDirBaseOffset;
};

AFX_DEFINE_STRUCT(_afxSerializedChunk)
{
    afxFcc          fcc;
    afxUnit          len;
    //afxUnit          cnt; // if nested it is more than 1; // variable length of instances did it unusable
};

AFX_DEFINE_STRUCT(_afxSerializedMdl)
{
    afxString       name;
    afxUnit          sklIdx;
    afxTransform    init;
    afxUnit          mshCnt;
    // followed by sizeof(afxUnit mshIdx[mshCnt]);
};

AFX_DEFINE_STRUCT(_afxSerializedMsh)
{
    afxString       name;
    afxUnit          vtdIdx;
    afxUnit          mshtIdx;
    afxUnit          mtlCnt;
    afxUnit          pivotCnt;
    // followed by sizeof(afxUnit pivotNameStrIdx[pivotCnt]);
};

AFX_DEFINE_STRUCT(_afxSerializedMsht)
{
    afxUnit          triCnt;
    afxUnit          surCnt;
    afxSize         firstTriOffset;
    afxSize         firstSurOffset;
    // followed by sizeof(afxMeshSection surfs[surCnt]);
    // followed by sizeof(afxUnit tris[triCnt]);
};

AFX_DEFINE_STRUCT(_afxSerializedVtxa)
{
    afxUnit          biasCnt;
    afxUnit          vtxCnt;
    afxUnit          attrCnt;
};

AFX_DEFINE_STRUCT(_afxSerializedVtd)
{
    afxUnit          biasCnt;
    afxUnit          vtxCnt;
    afxUnit          attrCnt;
    afxSize         firstBiasOffset;
    afxSize         firstVtxOffset;
    // followed by sizeof(afxVertexBias biases[surCnt]);
    // followed by sizeof(akxVertex vertices[triCnt]);
};

AFX_DEFINE_STRUCT(_afxSerializedSkl)
{
    afxString name;
    afxUnit lodType;
    afxUnit jointCnt;
    afxSize firstBoneOffset;
    afxSize firstBoneNameOfffset;
    // followed by sizeof(afxUnit lt[boneCnt]);
    // followed by sizeof(afxUnit iw[boneCnt]);
    // followed by sizeof(afxUnit parIdx[boneCnt]);
    // followed by sizeof(afxUnit lodErr[boneCnt]);
    // followed by sizeof(afxString boneNames[boneCnt]);
};
#pragma pack(pop)

AFX_DEFINE_STRUCT(afxAssertReference)
{
    afxUnit              type;
    afxUri const*       file;
};

AFX_DEFINE_STRUCT(afxCadToolInfo)
{
    afxString const*    name; // from art tool name
    afxUnit              majorRev;
    afxUnit              minorRev;
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
    afxUnit              majorRev;
    afxUnit              minorRev;
    afxUnit              customization;
    afxUnit              buildNum;
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

    afxUnit              resCap;
    afxResourceSlot*    resSlots;
    struct
    {
        afxFcc          fcc;
        afxUnit          baseResIdx;
        afxUnit          resCnt;
    }                  *nests;
    afxUnit              nestCnt;
};

AFX_DEFINE_STRUCT(akxAssetBuilder)
{
    void(*GetInfo)(void* data, afxUnit *typeCnt, afxUnit* resCnt, afxUri* name);
    void(*GetTypeInfo)(void* data, afxUnit setIdx, afxFcc* resType, afxUnit* resCnt);
    void*(*GetResourceInfo)(void* data, afxFcc type, afxUnit resIdx, afxUri* name);
    afxBool(*GetToolInfo)(void* data, afxCadToolInfo* toolInfo, afxCadExporterInfo* exporterInfo);
    afxError(*AddResources)(void* data, afxFcc type, afxUri const* name);
};

AMX afxUnit              AfxFindResourceNests(akxAsset cad, afxUnit cnt, afxFcc const fcc[], afxUnit nestIdx[]);
AMX afxUnit              AfxFindResources(akxAsset cad, afxFcc fcc, afxUnit cnt, afxString const id[], void* res[]);
AMX afxUnit              AfxCountResources(akxAsset cad, afxFcc fcc);

AMX afxUnit              AfxFindTextures(akxAsset cad, afxUnit cnt, afxString const id[], afxRaster tex[]);
AMX afxUnit              AfxFindMaterials(akxAsset cad, afxUnit cnt, afxString const id[], afxMaterial mt[]);
AMX afxUnit              AfxFindVertexDatas(akxAsset cad, afxUnit cnt, afxString const id[], afxGeometry vtd[]);
AMX afxUnit              AfxFindTopologies(akxAsset cad, afxUnit cnt, afxString const id[], afxMeshTopology msht[]);
AMX afxUnit              AfxFindMeshes(akxAsset cad, afxUnit cnt, afxString const id[], afxMesh msh[]);
AMX afxUnit              AfxFindSkeletons(akxAsset cad, afxUnit cnt, afxString const id[], afxSkeleton skl[]);
AMX afxUnit              AfxFindModels(akxAsset cad, afxUnit cnt, afxString const id[], afxModel mdl[]);
AMX afxUnit              AfxFindAnimations(akxAsset cad, afxUnit cnt, afxString const id[], afxAnimation anim[]);

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AMX afxError            AfxAcquireAssets(afxSimulation sim, afxUnit cnt, afxUnit const nestCnt[], afxUnit const resCap[], akxAsset cad[]);
AMX afxError            AfxBuildAssets(afxSimulation sim, akxAssetBuilder const* cadb, afxUnit cnt, void* data[], akxAsset cad[]);

AMX afxError            AfxLoadAssets(afxSimulation sim, afxError(*load)(afxUri const* file, afxFlags flags), afxFlags flags, afxUnit cnt, afxUri const file[]);
AMX afxError            AfxStoreAssets(afxSimulation sim, afxError(*store)(afxUri const* file, afxFlags flags), afxFlags flags, afxUnit cnt, afxUri const file[]);

AMX void                AfxTransformAssets(afxM3d const ltm, afxM3d const iltm, afxV4d const atv, afxReal affineTol, afxReal linearTol, afxFlags flags, afxUnit cnt, akxAsset cad[]);


AMX afxError            AfxDeserializeSkeletons(afxStream in, afxSimulation sim, afxUnit cnt, afxSkeleton skl[]);
AMX afxError            AfxSerializeSkeletons(afxStream out, afxUnit cnt, afxSkeleton const skl[]);

#endif//AMX_ASSET_H
