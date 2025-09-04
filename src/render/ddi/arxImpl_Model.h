/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This section is part of SIGMA Simulation Infrastructure.

#ifndef ARX_IMPL___CAD_H
#define ARX_IMPL___CAD_H

#include "qwadro/inc/render/arxRenderware.h"

#ifdef _ARX_MATERIAL_C

AFX_DEFINE_STRUCT(arxMaterialMap)
{
    afxString     usage; // usage 16
    arxMaterial     sub; // arxMaterial
};

#ifdef _ARX_MATERIAL_IMPL
AFX_OBJECT(_arxMaterial)
#else
AFX_OBJECT(arxMaterial)
#endif
{
    // Physical
    // The coefficient of friction is a measure of the frictional force between two surfaces in contact, represented by the ratio of 
    // the frictional force to the normal force pressing the surfaces together. It varies depending on the materials involved and 
    // can be classified into static (when surfaces are at rest) and kinetic (when surfaces are moving) coefficients.
    afxReal     friction; // [ 0, 1 ]
    // The coefficient of restitution, is the ratio of the relative velocity of separation after collision to the relative velocity 
    // of approach before collision. It can aIso be defined as the square root of the ratio of the final kinetic energy to the initial 
    // kinetic energy. It normally ranges from 0 to 1 where 1 would be a perfectly elastic collision. A perfectly inelastic collision 
    // has a coefficient of 0, but a 0 value does not have to be perfectly inelastic. It is measured in the Leeb rebound hardness test.
    afxReal     restitution; // [ 0, 1 ]
    afxReal     density; // kg/m³ or unitless

    // Visual
    afxV3d      albedo; // base color --- (1.0, 0.5, 0.3)
    afxReal     roughness; // [ 0, 1 ] --- 0.4
    afxReal     metallic; // [ 0, 1 ] --- 0.1
    avxRaster   texture; // optional
    avxRaster   normal; // optional

    // Metadata (could help for batching)
    afxUnit32   hash; // hashed signature of the entire material

    afxAabb     usageField;

    afxString       urn; // 128
    avxRaster       tex;
    avxSampler      smp;
    afxUnit         mapCnt;
    arxMaterialMap* maps;
    void*           extData;

    // non-Granny stuff
    afxInt          s[3];
    afxV4d          color; // albedo
    afxReal         shininess;

    afxV3d          Ka; // ambient color. The default is (0.2,0.2,0.2);
    afxV3d          Kd; // diffuse color. The default is (0.8,0.8,0.8);
    afxReal         Tr; // transparency. The default is 0.0 (not transparent at all)
    afxV3d          Ks; // specular color. This color shows up in highlights. The default is (1.0,1.0,1.0);
    afxReal         Ns; // specular exponent (aka shininess). The default is 0.0. A high exponent results in a tight, concentrated highlight.
    afxV3d          Ke; // 
    afxV3d          Tf; // Any light passing through the object is filtered by the transmission filter, which only allows the specifiec colors to pass through. For example, Tf 0 1 0 allows all the green to pass through and filters out all the red and blue.
    afxReal         Ni; // optical density for the surface.  This is also known as index of refraction. The values can range from 0.001 to 10. A value of 1.0 means that light does not bend as it passes through an object.
};

//AFX_STATIC_ASSERT(offsetof(arxMaterial, color) % AFX_MTL_ALIGN == 0, "");

#endif//_ARX_MATERIAL_C

#ifdef _ARX_GEOME_C

#ifdef _ARX_GEOME_IMPL
AFX_OBJECT(_arxMateriality)
#else
AFX_OBJECT(arxGeome)
#endif
{
    afxChain        classes;
    afxClass        mtlCls;
    afxClass        mshCls;
    afxClass        mdlCls;
    afxClass        terCls;

    afxStringBase   strbMdlSklMotUrns;
    afxStringBase   strbJointBiasesTags;
    afxStringBase   strbMorphTags;
};

#endif//_ARX_GEOME_C

AFX_DEFINE_STRUCT(_arxRiggedMesh)
// This new rig representation allow both model and mesh to access rigging info.
// Access to this info is important to let mesh instantiaion be bundled and to solve vertices' bias index indirection.
// Mesh juncture.
{
    arxMesh         msh;
    afxUnit         rigId; // arxMesh.rigs;

    arxModel        mdl; // arxModel.rigs;
    afxUnit         slotIdx; // mesh rig linkage index

    arxModel        skl; // original skeleton
    afxUnit*        biasFromJntMap; // [arxMesh.biasCnt] --- from original skeleton.
    afxUnit*        biasToJntMap; // [arxMesh.biasCnt] --- to assembled skeleton.
    // TODO? Unify to jntMap, and use baseFromIdx and baseToIdx.

    afxUnit*        mtlToTxdMap; // [arxMesh.mtlCnt] --- map into global list.
    arxMaterial     txd;
    arxMeshRigFlags flags;
    afxMask         renderMask;
    void**          idd;
    void**          udd;
};

#ifdef _ARX_MESH_C

AFX_DEFINE_STRUCT(arxMeshAttr)
{
    afxString8          usage; // 8
    arxVertexFlags      flags;
    avxFormat           fmt;
    afxBox              aabb; // mainly used to optimize memory usage with ATV data.
    afxBool             aabbUpdReq;
};

AFX_DEFINE_STRUCT(arxVertexBuffer)
{
    afxChain    meshes;
    afxLink  buf;

};

// Meshes just reference a material ID. This avoids material DAGs attached directly to geometry.

AFX_OBJECT(arxMesh)
{
    // TOPOLOGY DATA
    arxMeshFlags        flags;
    avxTopology         topology; // actually only TRILIST
    afxUnit             triCnt; // count of primitives.
    afxUnit*            sideToAdjacentMap; // [edgeCnt]
    union
    {
        // for collision
        afxV3d*         edgeNrm; // [edgeCnt]
        afxV4d*         edgeNrmw; // [edgeCnt]
    };
    afxUnit             mtlCnt; // used by sections
    afxString*          materials; // [mtlCnt]
    afxUnit             secCnt;
    arxMeshSection*     sections; // [secCnt]
    afxUnit             biasCnt;
    arxMeshBias*        biases;
    // nested bias identifier strings for fast lookup.
    afxString*          biasId;
    afxUnit             jointsForTriCnt;
    afxUnit*            jointsForTriMap;
    afxUnit             triToJointCnt;
    afxUnit*            triToJointMap;

    // VERTEX DATA
    afxUnit             vtxCnt;
    afxUnit*            vtxToVtxMap; // [vtxCnt]
    afxUnit*            vtxToTriMap; // [vtxCnt]
    afxUnit             minIdxSiz;
    afxUnit             idxCnt; // count of primitive indices.
    afxUnit*            indices; // [idxCnt] --- indices into primitive vertices.
    afxUnit             attrCnt; // used by morphes.
    afxString*          attrIds;
    arxMeshAttr*        attrInfo;
    afxByte**           vtxAttrData; // [attrCnt]

    // SHAPE DATA
    afxUnit             morphCnt;
    arxMeshMorph*       morphs;
    // nested morph tags for fast lookup.
    afxString*          morphTags; // [morphCnt]
    // nested section AABB for fast lookup
    afxBox*             secAabb; // [morphCnt][secCnt]
    // nested bias OBB for fast lookup.
    afxBox*             biasObb; // [morphCnt][biasCnt]

    // TODO: move to mesh rig
    arxVertexCache      vtxCache;
    avxBuffer           ibo;
    afxUnit32           iboBase;
    afxUnit32           iboRange;
    afxUnit32           iboStride;

    afxString           urn; // 32
    void*               idd;
    void*               udd;

    afxPool             rigs;
};

#endif

#ifdef _ARX_MODEL_C

#ifdef _ARX_MODEL_IMPL
AFX_OBJECT(_arxModel)
#else
AFX_OBJECT(arxModel)
#endif
{
    // In the past, skeleton (as arxSkeleton) was separated from model.
    // Mainly to simplify serialization, they were merged.
    // Anyway, there was a second arrangement aimed to optimized reutilization of skeleton,
    // where a arxModel would be a superclass of arxSkeleton. The arxModel would point to itself or other model as a skeleton.
    // Fragments of this approach can be observed in mesh rigs.

    afxUnit         jntCnt;
    // array of per-joint parent indexes
    afxUnit*        jntPi; // [jntCnt]
    // array of per-joint local transforms
    afxTransform*   jntLt; // [jntCnt]
    // array of per-joint inverse world matrices
    afxM4d*         jntIw; // [jntCnt]
    // array of per-joint LOD error values
    afxReal*        jntLe; // [jntCnt]
    // array of per-joint identier strings.
    afxString*      jntId; // [jntCnt]
    // array of per-joint IDD
    void**          jntIdd; // [jntCnt]
    // array of per-joint UDD
    void**          jntUdd; // [jntCnt]

    afxUnit         lodType;
    afxReal         allowedLodErrFadingFactor;
    afxTransform    displace;

    afxUnit         rigCnt;
    _arxRiggedMesh**    rigs;
    afxUnit         usedRigCnt;
    afxBox          aabb;
    arxModelFlags   flags;
    afxString       urn; // 32
    void**          idd;
    void**          udd;
};

#endif

AFX_DEFINE_STRUCT(arxArticulation)
{
    afxReal         weight;
    afxUnit         cnt;
    afxTransform    xform;
    afxUnit         traversalId;
};

#ifdef _ARX_POSE_C
#ifdef _ARX_POSE_IMPL
AFX_OBJECT(_arxPose)
#else
AFX_OBJECT(arxPose)
#endif
{
    afxString           tag;
    void*               udd;
    afxUnit             artCnt;
    arxArticulation*    arts;
    afxReal             fillThreshold;
    afxUnit             traversalId;
};
#endif//_ARX_POSE_C

#ifdef _ARX_PLACEMENT_C
#ifdef _ARX_PLACEMENT_IMPL
AFX_OBJECT(_arxPlacement)
#else
AFX_OBJECT(arxPlacement)
#endif
{
    afxString   tag;
    void*       udd;
    afxUnit     matCnt;
    afxM4d*     world;
    // The delta array store the delta from a joint's rest pose to the world matrix.
    afxM4d*     delta;
};
#endif//_ARX_PLACEMENT_C


#ifdef _ARX_TERRAIN_C
AFX_DEFINE_STRUCT(_arxTerrSec)
{
    afxUnit     gridNode, gridNodeX, gridNodeZ;

    afxUnit     vtxIdxCnt;
    afxUnit     vtxCnt;
    afxUnit     quadCnt;
    afxUnit     vtxStartX;
    afxUnit     vtxStartZ;
    afxUnit     vtxPerRow;
    afxUnit     vtxPerCol;

    afxUnit     visuReqCnt;
    afxUnit     visuVtxCnt;
    afxV3d*     visuVtxPos;
    afxUnit     visuVtxIdxCnt;
    afxUnit16*  visuVtxMap;

    afxSphere   bsph;
    afxBox      aabb;
    afxV4d      centre;
    afxUnit     collTriCnt;
    afxV3d*     collTriEdgeNrm; // [collTriCnt * 3]
    afxUnit     collVtxCnt;
    afxV3d*     collVtx;
    afxV3d*     collVtxTan;
    afxV3d*     collVtxBit;
    afxV3d*     collVtxNrm;
    afxV3d*     collVtxRgb;

    arxMesh     msh;

    avxBuffer   vbo;
    afxUnit     vboBase;
    afxUnit     vboRange;
    afxUnit     vboStride;

    avxBuffer   ibo;
    afxUnit     iboBase;
    afxUnit     iboRange;
    afxUnit     iboStride;

};

AFX_OBJECT(arxTerrain)
{
    afxString   tag;
    void*       udd;
    afxDrawSystem dsys;
    avxBuffer   vbo;
    avxBuffer   ibo;
    avxBuffer   dbgLinesVbo;
    afxUnit     dbgLinesVboRange;
    afxUnit     dbgLinesVboStride;
    avxBuffer   dbgLinesIbo;
    afxUnit     dbgLinesIboRange;
    afxUnit     dbgLinesIboStride;

    avxPipeline dbgAabbPip;
    avxPipeline meshPip;
    avxRaster   texd;
    avxRaster   texn;

    afxUnit     width; // terrain width
    afxUnit     depth; // terrain depth
    afxUnit     secWidth; // sector width
    afxUnit     secDepth; // sector depth
    afxUnit     rowSecCnt; // row-sectors
    afxUnit     sliceSecCnt; // slice-sectors
    afxReal     heightScale;
    afxUri      heightmap;

    afxUnit     secCnt;
    _arxTerrSec*sectors;
    afxBox*     secAabb;

};
#endif//_ARX_TERRAIN_C

ARX afxClassConfig const _ARX_POSE_CLASS_CONFIG;
ARX afxClassConfig const _ARX_PLCE_CLASS_CONFIG;
ARX afxClassConfig const _ARX_MDL_CLASS_CONFIG;
ARX afxClassConfig const _ARX_MTL_CLASS_CONFIG;
ARX afxClassConfig const _ARX_MORP_CLASS_CONFIG;
ARX afxClassConfig const _ARX_MSH_CLASS_CONFIG;

ARX afxPool* _ArxGetMeshRigPool(arxMesh msh);
ARX _arxRiggedMesh* _ArxGetMeshRig(arxMesh msh, afxUnit rigId);

ARX afxTransform* _ArxMdlGetLtArray(arxModel mdl, afxUnit base);
ARX afxM4d* _ArxMdlGetIwArray(arxModel mdl, afxUnit base);
ARX afxUnit* _ArxMdlGetPiArray(arxModel mdl, afxUnit base);
ARX afxReal* _ArxMdlGetLeArray(arxModel mdl, afxUnit base);

// pose
ARX arxArticulation* _ArxPoseGetPaArray(arxPose pose, afxUnit base);
// placement
ARX afxM4d* _ArxPlceGetWorldArray(arxPlacement plce, afxUnit base);
ARX afxM4d* _ArxPlceGetDeltaArray(arxPlacement plce, afxUnit base);

#endif//ARX_IMPL___CAD_H
