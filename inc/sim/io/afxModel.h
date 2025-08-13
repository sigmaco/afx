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

/*
    O objeto afxModel descreve uma coleção de afxMesh'es que são todas ligadas ao mesmo afxSkeleton.
    Isso é essencialmente qualquer grupo conectado de malhas que são animadas em conjuntura.
    Um ator seria um modelo, assim seria um veículo, ou mesmo uma edificação inteira se essa fossa modelada como uma hierarquia singular.
    Um afxModel completo é feito de um afxSkeleton e um conjunto de afxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do afxModel.

    Rigging is the process of connecting a mesh with an internal poseable skeleton rig and bone structure. 
    Rigged meshes allow mesh surfaces to rotate and move where internal bones are placed within the model during the modeling process.

    O objeto afxSkeleton é uma coleção hierárquica de articulações que descrevem a estrutura articular interna de um afxModel, e auxilia no manejo e na animação do mesmo. 

    Cada articulação no arranjo do afxSkeleton aloja o transforme para aquela articulação em dois meios diferentes. 
    Primeiro, afxTransform "local" é o transforme decomposto da articulação relativa a seu parente imediato (dado pelo índice do parente).
    Se esta não houver parente (isto é, o índex do parente é igual a AFX_INVALID_INDEX), então esta é relativa a origem do afxSkeleton.

    Segundo, "iw" é uma afxM4d que é o transforme invertido no world-space para a articulação na postura padrão do afxSkeleton (isto é, a postura na qual o afxSkeleton foi originalmente modelado).

    O afxTransform "local" é usado primariamente na composição de animações, e a afxM4d "iw" é usada primariamente para deformação de malha, razão pela qual esta informação é alojada em dois diferentes formatos.

    Cada articulação também opcionalmente contém dados definidos pelo usuário "UDD".
    Este campo aponta para algum dado, estranho à lógica do Qwadro, associado com a articulação, se houver qualquer.

    Skinning is a technique for deforming geometry by linearly weighting vertices to a set of transformations, represented by nodes.
    Nodes that affect a particular geometry are usually organized into a single hierarchy called a 'skeleton', although the influencing nodes may come from unrelated parts of the hierarchy.
    The nodes of such a hierarchy represents the 'joints' of the skeleton, which should not be confused with the 'bones', which are the imaginary line segments connecting two joints.
*/

#ifndef ASX_MODEL_H
#define ASX_MODEL_H

#include "qwadro/inc/sim/io/afxMesh.h"
#include "qwadro/inc/sim/body/afxPose.h"
#include "qwadro/inc/sim/body/afxPlacement.h"

typedef enum afxModelFlag
{
    afxModelFlag_NONE
} afxModelFlags;

typedef enum afxMeshRigFlag
{
    afxMeshRigFlag_TRANSPLANTED = AFX_BITMASK(0)
} afxMeshRigFlags;

typedef enum asxJointType
{
    // No relative motion between bodies. (welded parts)
    asxJointType_FIXED,
    // Rotation about a single axis, like a door hinge. (elbow, door)
    asxJointType_HINGE,
    // Free rotation in all directions, no translation. (shoulder, hip)
    asxJointType_BALL,
    // Movement along a single axis, no rotation. (telescopic arm, piston)
    asxJointType_SLIDER,
    // Like a fixed joint, but allows soft compliance (springy or damped motion). (soft ropes, cloth physics)
    asxJointType_SPRING,
    // Fully customizable: each axis can be locked, limited, or free. (complex mechanical systems)
    asxJointType_6DOF,
} asxJointType;

AFX_DEFINE_STRUCT(asxJointConstraint)
{
    asxJointType type;
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

AFX_DEFINE_STRUCT(afxModelInfo)
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

AFX_DEFINE_STRUCT(afxMeshRigInfo)
{
    afxMeshRigFlags flags;
    afxMesh         msh;
    afxModel        origSkl;
    afxUnit const*  biasFromJnt;
    afxUnit const*  biasToJnt;
    afxMaterial     txd;
};

AFX_DEFINE_STRUCT(afxModelBlueprint)
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

ASX afxBool     AfxGetModelUrn(afxModel mdl, afxString* id);

ASX void        AfxDescribeModel(afxModel mdl, afxModelInfo* info);

// DISPLACEMENT

ASX void        AfxDisplaceModel(afxModel mdl, afxTransform const* t);
ASX void        AfxGetModelDisplacement(afxModel mdl, afxTransform* t);
ASX void        AfxComputeModelDisplacement(afxModel mdl, afxM4d m);

// MESH RIGGING

ASX void        AfxDescribeMeshRig(afxModel mdl, afxUnit rigIdx, afxMeshRigInfo* info);

ASX afxUnit     AfxGetRiggedMeshes(afxModel mdl, afxUnit baseRigIdx, afxUnit cnt, afxMesh meshes[]);
ASX afxError    AfxRigMeshes(afxModel mdl, afxModel skl, afxUnit baseRigIdx, afxUnit mshCnt, afxMesh const meshes[]);

ASX afxBool     AfxIsRiggedMeshTransplanted(afxModel mdl, afxUnit rigIdx);

ASX void        AfxSetMeshRigTxd(afxModel mdl, afxUnit rigIdx, afxMaterial mtl);
ASX afxBool     AfxFindMeshRigMaterial(afxModel mdl, afxUnit rigIdx, afxString const* id, afxMaterial* material);

ASX void        AfxComputeRiggedMeshMatrices(afxModel mdl, afxUnit rigIdx, afxPlacement plce, afxUnit baseBiasIdx, afxUnit biasCnt, afxM4d matrices[]);

// SKELETON

ASX afxUnit     AsxGetJointTransforms(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxTransform transforms[]);
ASX afxUnit     AsxGetJointLodErrors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxReal lodErrors[]);
ASX afxUnit     AsxGetJointInversors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void* matrices, afxUnit stride);
ASX afxUnit     AsxGetJointParents(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void* indices, afxUnit stride);

ASX afxError    AsxReparentJoints(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* indices, afxUnit stride);
ASX afxError    AsxResetJointTransforms(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxTransform const transforms[]);
ASX afxError    AsxResetJointInversors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, void const* matrices, afxUnit stride);
ASX afxError    AsxResetJointLodErrors(afxModel mdl, afxUnit baseJntIdx, afxUnit cnt, afxReal const lodErrors[]);

ASX afxUnit     AsxCountJoints(afxModel mdl, afxReal allowedErr);
ASX afxUnit     AsxFindJoints(afxModel mdl, afxUnit cnt, afxString const ids[], afxUnit indices[]);
ASX void        AsxQueryModelErrorTolerance(afxModel mdl, afxReal allowedErr, afxReal* allowedErrEnd, afxReal* allowedErrScaler);

ASX void        AfxBuildRiggedMeshCompositeMatrices(afxModel mdl, afxUnit rigIdx, afxPlacement plce, afxUnit cnt, afxM4d matrices[]);

ASX afxError    AsxArchiveModel(afxModel mdl, afxUri const* uri);

////////////////////////////////////////////////////////////////////////////////

ASX void        AfxTransformModels(afxM3d const ltm, afxM3d const iltm, afxReal ltmTol, afxV3d const atv, afxReal atvTol, afxFlags flags, afxUnit cnt, afxModel models[]);

ASX afxUnit     AfxEnumerateModels(afxMorphology morp, afxUnit first, afxUnit cnt, afxModel models[]);

ASX afxError    AfxAssembleModels(afxMorphology morp, afxUnit cnt, afxModelBlueprint const blueprints[], afxModel models[]);

ASX afxError    AfxLoadModel(afxMorphology morp, afxString const* urn, afxUri const* uri, afxModel* model);

#endif//ASX_MODEL_H
