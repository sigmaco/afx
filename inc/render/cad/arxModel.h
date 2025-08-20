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

#ifndef ARX_MODEL_H
#define ARX_MODEL_H

/*
    O objeto arxModel descreve uma coleção de arxMesh'es que são todas ligadas ao mesmo arxSkeleton.
    Isso é essencialmente qualquer grupo conectado de malhas que são animadas em conjuntura.
    Um ator seria um modelo, assim seria um veículo, ou mesmo uma edificação inteira se essa fossa modelada como uma hierarquia singular.
    Um arxModel completo é feito de um arxSkeleton e um conjunto de arxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do arxModel.

    Rigging is the process of connecting a mesh with an internal poseable skeleton rig and bone structure. 
    Rigged meshes allow mesh surfaces to rotate and move where internal bones are placed within the model during the modeling process.

    O objeto arxSkeleton é uma coleção hierárquica de articulações que descrevem a estrutura articular interna de um arxModel, e auxilia no manejo e na animação do mesmo. 

    Cada articulação no arranjo do arxSkeleton aloja o transforme para aquela articulação em dois meios diferentes. 
    Primeiro, afxTransform "local" é o transforme decomposto da articulação relativa a seu parente imediato (dado pelo índice do parente).
    Se esta não houver parente (isto é, o índex do parente é igual a AFX_INVALID_INDEX), então esta é relativa a origem do arxSkeleton.

    Segundo, "iw" é uma afxM4d que é o transforme invertido no world-space para a articulação na postura padrão do arxSkeleton (isto é, a postura na qual o arxSkeleton foi originalmente modelado).

    O afxTransform "local" é usado primariamente na composição de animações, e a afxM4d "iw" é usada primariamente para deformação de malha, razão pela qual esta informação é alojada em dois diferentes formatos.

    Cada articulação também opcionalmente contém dados definidos pelo usuário "UDD".
    Este campo aponta para algum dado, estranho à lógica do Qwadro, associado com a articulação, se houver qualquer.

    Skinning is a technique for deforming geometry by linearly weighting vertices to a set of transformations, represented by nodes.
    Nodes that affect a particular geometry are usually organized into a single hierarchy called a 'skeleton', although the influencing nodes may come from unrelated parts of the hierarchy.
    The nodes of such a hierarchy represents the 'joints' of the skeleton, which should not be confused with the 'bones', which are the imaginary line segments connecting two joints.
*/

#include "qwadro/inc/render/cad/arxMesh.h"
#include "qwadro/inc/render/cad/arxPose.h"
#include "qwadro/inc/render/cad/arxPlacement.h"

typedef enum arxModelFlag
{
    arxModelFlag_NONE
} arxModelFlags;

typedef enum arxMeshRigFlag
{
    arxMeshRigFlag_TRANSPLANTED = AFX_BITMASK(0)
} arxMeshRigFlags;

typedef enum arxJointType
{
    // No relative motion between bodies. (welded parts)
    arxJointType_FIXED,
    // Rotation about a single axis, like a door hinge. (elbow, door)
    arxJointType_HINGE,
    // Free rotation in all directions, no translation. (shoulder, hip)
    arxJointType_BALL,
    // Movement along a single axis, no rotation. (telescopic arm, piston)
    arxJointType_SLIDER,
    // Like a fixed joint, but allows soft compliance (springy or damped motion). (soft ropes, cloth physics)
    arxJointType_SPRING,
    // Fully customizable: each axis can be locked, limited, or free. (complex mechanical systems)
    arxJointType_6DOF,
} arxJointType;

AFX_DEFINE_STRUCT(arxJointConstraint)
{
    arxJointType type;
    afxV3d anchorA; // local
    afxV3d anchorB; // local
    union
    {
        //struct { } fixed;
        struct
        {
            afxV3d axis; // Hinge axis in local space
            afxReal minAngle;
            afxReal maxAngle;
            afxReal damping;
        } hinge;
        struct
        {
            afxReal swingLimit;
            afxReal twistLimit;
        } ball;
        struct
        {
            afxV3d axis; // Translation axis
            afxReal minLimit;
            afxReal maxLimit;
            afxReal damping;
        } slider;
        struct
        {
            afxReal stiffness; // Hooke's Law constant
            afxReal damping; // Energy loss
            afxReal restLen;
        } spring;
        struct
        {
            // locked, limited or free

            afxDof flags;

            afxV3d linearLimitMin;
            afxV3d linearLimitMax;
            afxV3d angularLimitMin; // in radians
            afxV3d angularLimitMax;
            afxReal stiffness;
            afxReal damping;
        } dof;
    };
};

AFX_DEFINE_STRUCT(arxModelInfo)
{
    afxUnit         jntCnt;
    afxString*      joints;
    afxUnit*        jntPi;
    afxTransform*   jntLt;
    afxM4d*         jntIw;
    afxReal*        jntLe;

    afxUnit         lodType;
    afxReal         allowedLodErrFadingFactor;
    afxTransform    displace;

    afxUnit         rigCnt;
    afxUnit         riggedMeshCnt;
};

AFX_DEFINE_STRUCT(arxMeshRigInfo)
{
    arxMeshRigFlags flags;
    arxMesh         msh;
    arxModel        origSkl;
    afxUnit const*  biasFromJnt;
    afxUnit const*  biasToJnt;
    arxMaterial     txd;
};

AFX_DEFINE_STRUCT(arxModelBlueprint)
/// Data needed for model assembly
{
    afxString32         urn;
    afxUnit             lodType;
    afxBool             scalable;
    afxBool             deformable;
    afxUnit             jointCnt;
    afxString const*    joints;
    //afxUnit const*      parents;
    //afxTransform const* transforms; // local
    //afxM4d const*       matrices; // inverse world
    //afxReal const*      lodErrors;
    afxTransform        displace;
    afxUnit             rigCnt;
};

ARX afxBool     ArxGetModelUrn(arxModel mdl, afxString* id);

ARX void        ArxDescribeModel(arxModel mdl, arxModelInfo* info);

// DISPLACEMENT

ARX void        ArxDisplaceModel(arxModel mdl, afxTransform const* t);
ARX void        ArxGetModelDisplacement(arxModel mdl, afxTransform* t);
ARX void        ArxComputeModelDisplacement(arxModel mdl, afxM4d m);

// MESH RIGGING

ARX void        ArxDescribeMeshRig(arxModel mdl, afxUnit rigIdx, arxMeshRigInfo* info);

ARX afxUnit     ArxGetRiggedMeshes(arxModel mdl, afxUnit baseRigIdx, afxUnit cnt, arxMesh meshes[]);
ARX afxError    ArxRigMeshes(arxModel mdl, arxModel skl, afxUnit baseRigIdx, afxUnit mshCnt, arxMesh const meshes[]);

ARX afxBool     ArxIsRiggedMeshTransplanted(arxModel mdl, afxUnit rigIdx);

ARX void        ArxSetMeshRigTxd(arxModel mdl, afxUnit rigIdx, arxMaterial mtl);
ARX afxBool     ArxFindMeshRigMaterial(arxModel mdl, afxUnit rigIdx, afxString const* id, arxMaterial* material);

ARX void        ArxComputeRiggedMeshMatrices(arxModel mdl, afxUnit rigIdx, arxPlacement plce, afxUnit baseBiasIdx, afxUnit biasCnt, afxM4d matrices[]);

// SKELETON

ARX afxUnit     ArxGetJointTransforms(arxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxTransform transforms[]);
ARX afxUnit     ArxGetJointLodErrors(arxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxReal lodErrors[]);
ARX afxUnit     ArxGetJointInversors(arxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void* matrices, afxUnit stride);
ARX afxUnit     ArxGetJointParents(arxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void* indices, afxUnit stride);

ARX afxError    ArxReparentJoints(arxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* indices, afxUnit stride);
ARX afxError    ArxResetJointTransforms(arxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxTransform const transforms[]);
ARX afxError    ArxResetJointInversors(arxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* matrices, afxUnit stride);
ARX afxError    ArxResetJointLodErrors(arxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxReal const lodErrors[]);

ARX afxUnit     ArxCountJoints(arxModel mdl, afxReal allowedErr);
ARX afxUnit     ArxFindJoints(arxModel mdl, afxUnit cnt, afxString const ids[], afxUnit indices[]);
ARX void        ArxQueryModelErrorTolerance(arxModel mdl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler);

ARX void        ArxBuildRiggedMeshCompositeMatrices(arxModel mdl, afxUnit rigIdx, arxPlacement plce, afxUnit cnt, afxM4d matrices[]);

ARX afxError    ArxArchiveModel(arxModel mdl, afxUri const* uri);

////////////////////////////////////////////////////////////////////////////////

ARX void        ArxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltmTol, afxV3d const atv, afxReal atvTol, afxFlags flags, afxUnit cnt, arxModel models[]);

ARX afxUnit     ArxEnumerateModels(arxGeome morp, afxUnit first, afxUnit cnt, arxModel models[]);

ARX afxError    ArxAssembleModels(arxGeome morp, afxUnit cnt, arxModelBlueprint const blueprints[], arxModel models[]);

ARX afxError    ArxLoadModel(arxGeome morp, afxString const* urn, afxUri const* uri, arxModel* model);

#endif//ARX_MODEL_H
