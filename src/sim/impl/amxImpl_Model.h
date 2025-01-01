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

#ifndef AMX_IMPL___CAD_H
#define AMX_IMPL___CAD_H

//#include "../dev/afxExecImplKit.h"
#include "qwadro/inc/sim/afxSimulation.h"

#ifdef _AMX_MATERIAL_C

AFX_DEFINE_STRUCT(akxMaterialMap)
{
    afxString     usage; // usage 16
    afxMaterial     sub; // afxMaterial
};

#ifdef _AMX_MATERIAL_IMPL
AFX_OBJECT(_amxMaterial)
#else
AFX_OBJECT(afxMaterial)
#endif
{
    afxString       urn; // 128
    afxRaster       tex;
    avxSampler      smp;
    afxUnit         mapCnt;
    akxMaterialMap* maps;
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

#endif//_AMX_MATERIAL_C

#ifdef _AMX_POSE_C
AFX_DEFINE_STRUCT(akxArticulation)
{
    afxReal             weight;
    afxUnit             cnt;
    afxTransform        xform;
    afxUnit             traversalId;
};

#ifdef _AMX_POSE_IMPL
AFX_OBJECT(_amxPose)
#else
AFX_OBJECT(afxPose)
#endif
{
    afxUnit             artCnt;
    akxArticulation*    arts;
    afxReal             fillThreshold;
    afxUnit             traversalId;
};
#endif//_AMX_POSE_C

#ifdef _AMX_PLACEMENT_C
#ifdef _AMX_PLACEMENT_IMPL
AFX_OBJECT(_amxPlacement)
#else
AFX_OBJECT(afxPlacement)
#endif
{
    afxUnit     matCnt;
    afxM4d*     world;
    // The delta array store the delta from a joint's rest pose to the world matrix.
    afxM4d*     delta;
};
#endif//_AMX_PLACEMENT_C

#ifdef _AMX_MODEL_C

AFX_DEFINE_STRUCT(afxMeshRig)
{
    afxMesh         msh;
    afxUnit*        biasToJointMap; // [biasCnt] --- to assembled skeleton.
    afxUnit*        biasFromJointMap; // [biasCnt] --- from original skeleton.
    afxModel        sklOrig;
    afxMeshRigFlags flags;
    afxMaterial     txd;
    void**          idd;
    void**          udd;
};

#ifdef _AMX_MODEL_IMPL
AFX_OBJECT(_amxModel)
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
    afxMeshRig*     rigs;
    afxUnit         usedRigCnt;
    afxBox          aabb;
    afxModelFlags   flags;
    afxString       urn; // 32
    void**          idd;
    void**          udd;
};

#endif

AMX afxClassConfig const _AMX_POSE_CLASS_CONFIG;
AMX afxClassConfig const _AMX_PLCE_CLASS_CONFIG;
AMX afxClassConfig const _AMX_MDL_CLASS_CONFIG;
AMX afxClassConfig const _AMX_MTL_CLASS_CONFIG;


#endif//AMX_IMPL___CAD_H
