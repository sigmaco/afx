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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef ASX_IMPL___CAD_H
#define ASX_IMPL___CAD_H

//#include "../impl/afxExecImplKit.h"
#include "qwadro/inc/sim/afxSimulation.h"

#ifdef _ASX_MATERIAL_C

AFX_DEFINE_STRUCT(asxMaterialMap)
{
    afxString     usage; // usage 16
    afxMaterial     sub; // afxMaterial
};

#ifdef _ASX_MATERIAL_IMPL
AFX_OBJECT(_asxMaterial)
#else
AFX_OBJECT(afxMaterial)
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
    asxMaterialMap* maps;
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

//AFX_STATIC_ASSERT(offsetof(afxMaterial, color) % AFX_MTL_ALIGN == 0, "");

#endif//_ASX_MATERIAL_C

AFX_DEFINE_STRUCT(asxMeshRig)
// This new rig representation allow both model and mesh to access rigging info.
// Access to this info is important to let mesh instantiaion be bundled and to solve vertices' bias index indirection.
{
    afxMesh         msh;
    afxUnit         rigId; // afxMesh.rigs;

    afxModel        mdl; // afxModel.rigs;
    afxUnit         slotIdx; // mesh rig linkage index
    afxUnit*        biasToJntMap; // [afxMesh.biasCnt] --- to assembled skeleton.
    afxModel        skl; // original skeleton
    afxUnit*        biasFromJntMap; // [afxMesh.biasCnt] --- from original skeleton.
    afxUnit*        mtlToTxdMap; // [afxMesh.mtlCnt] --- map into global list.
    afxMaterial     txd;
    afxMeshRigFlags flags;
    void**          idd;
    void**          udd;
};

#ifdef _ASX_MESH_C

AFX_DEFINE_STRUCT(asxMeshAttr)
{
    afxString8          usage; // 8
    afxVertexFlags      flags;
    avxFormat           fmt;
    afxBox              aabb; // mainly used to optimize memory usage with ATV data.
    afxBool             aabbUpdReq;
};

AFX_DEFINE_STRUCT(avxVertexBuffer)
{
    afxChain    meshes;
    afxLink  buf;

};

// Meshes just reference a material ID. This avoids material DAGs attached directly to geometry.

AFX_OBJECT(afxMesh)
{
    // TOPOLOGY DATA
    afxMeshFlags        flags;
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
    afxMeshSection*     sections; // [secCnt]
    afxUnit             biasCnt;
    afxMeshBias*        biases;
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
    asxMeshAttr*        attrInfo;
    afxByte**           vtxAttrData; // [attrCnt]

    // SHAPE DATA
    afxUnit             morphCnt;
    afxMeshMorph*       morphs;
    // nested morph tags for fast lookup.
    afxString*          morphTags; // [morphCnt]
    // nested section AABB for fast lookup
    afxBox*             secAabb; // [morphCnt][secCnt]
    // nested bias OBB for fast lookup.
    afxBox*             biasObb; // [morphCnt][biasCnt]

    avxVertexCache      vtxCache;
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

#ifdef _ASX_MODEL_C

#ifdef _ASX_MODEL_IMPL
AFX_OBJECT(_asxModel)
#else
AFX_OBJECT(afxModel)
#endif
{
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
    afxTransform    displacement;

    afxUnit         rigCnt;
    asxMeshRig**    rigs;
    afxUnit         usedRigCnt;
    afxBox          aabb;
    afxModelFlags   flags;
    afxString       urn; // 32
    void**          idd;
    void**          udd;
};

#endif

AFX_DEFINE_STRUCT(asxArticulation)
{
    afxReal         weight;
    afxUnit         cnt;
    afxTransform    xform;
    afxUnit         traversalId;
};

#ifdef _ASX_POSE_C
#ifdef _ASX_POSE_IMPL
AFX_OBJECT(_asxPose)
#else
AFX_OBJECT(afxPose)
#endif
{
    afxUnit             artCnt;
    asxArticulation*    arts;
    afxReal             fillThreshold;
    afxUnit             traversalId;
};
#endif//_ASX_POSE_C

#ifdef _ASX_PLACEMENT_C
#ifdef _ASX_PLACEMENT_IMPL
AFX_OBJECT(_asxPlacement)
#else
AFX_OBJECT(afxPlacement)
#endif
{
    afxUnit     matCnt;
    afxM4d*     world;
    // The delta array store the delta from a joint's rest pose to the world matrix.
    afxM4d*     delta;
};
#endif//_ASX_PLACEMENT_C

ASX afxClassConfig const _ASX_POSE_CLASS_CONFIG;
ASX afxClassConfig const _ASX_PLCE_CLASS_CONFIG;
ASX afxClassConfig const _ASX_MDL_CLASS_CONFIG;
ASX afxClassConfig const _ASX_MTL_CLASS_CONFIG;
ASX afxClassConfig const _ASX_MSH_CLASS_CONFIG;

ASX afxPool* _AsxGetMeshRigPool(afxMesh msh);
ASX asxMeshRig* _AsxGetMeshRig(afxMesh msh, afxUnit rigId);

ASX afxTransform* _AsxMdlGetLtArray(afxModel mdl, afxUnit base);
ASX afxM4d* _AsxMdlGetIwArray(afxModel mdl, afxUnit base);
ASX afxUnit* _AsxMdlGetPiArray(afxModel mdl, afxUnit base);
ASX afxReal* _AsxMdlGetLeArray(afxModel mdl, afxUnit base);

// pose
ASX asxArticulation* _AsxPoseGetPaArray(afxPose pose, afxUnit base);
// placement
ASX afxM4d* _AsxPlceGetWorldArray(afxPlacement plce, afxUnit base);
ASX afxM4d* _AsxPlceGetDeltaArray(afxPlacement plce, afxUnit base);

#endif//ASX_IMPL___CAD_H
