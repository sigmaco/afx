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

/// O objeto arxBody aloja o estado de um arxModel no tempo de execução.
/// O arxModel é apenas dados estruturados para um modelo particular --- isto é, este não há qualquer conceito de onde este está, qual estado este está em, ou quais animações estão interagindo em si.
/// O arxBody mantém rastreio disto e provém um número de funções úteis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades idênticas deveriam compartilhar o mesmo arxModel, desde que seus dados de malha e esqueleto sejam idênticos, cada um deveria ainda haver seu próprio arxBody.
/// Dito isto porque os dados do arxBody são todos dinâmicos, e não são compartilhados através de múltiplos modelos, mesmo se estes foram feitos do mesmo arxModel.
/// Assim, enquanto é comum colocar a hierarquia de estruturas do arxModel no cache e instanciá-las múltiplas vezes, você nunca deveria colocar objetos arxBody no cache e reusá-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da existência das entidades que eles representam na sua aplicação.

/// Note, no entanto, que estes são simplesmente ponteiros para os dados originais que vocÊ passou durante a instanciação.
/// O arxBody não mantém cópias dos dados (para minimizar uso de memória), então você não pode liberar os dados originais do modelo e esperar obter ponteiros válidos de volta aqui.
/// Para aquilo que importa, se você liberar os dados originais do modelo, nenhuma das chamadas para arxBody funcionarão, desde que elas todas dependem daqueles dados, como você esperaria.

#ifndef ARX_BODY_H
#define ARX_BODY_H

#include "qwadro/cad/arxModel.h"
#include "qwadro/sim/arxCapstan.h"
#include "qwadro/sim/arxNode.h"

ARX afxBool     ArxGetBodyModel(arxBody bod, arxModel* model);

ARX afxUnit     ArxCountBodyMotives(arxBody bod);

ARX void        ArxResetBodyClock(arxBody bod);

ARX void        ArxRecenterBodyMotiveClocks(arxBody bod, afxReal currClock);

/// Update all the motors affecting a particular puppet.
ARX void        ArxStepBodyMotives(arxBody bod, afxReal /* NOT delta */ time);

ARX void        ArxPurgeTerminatedMotives(arxBody bod);

ARX void        ArxAccumulateBodyAnimations(arxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, arxPose rslt, afxReal allowedErr, afxUnit const sparseJntMap[]);

ARX void        ArxComputeBodyMotionVectors(arxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation);

ARX void        ArxComputeBodyMotionMatrix(arxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m);


AFX_DEFINE_STRUCT(arxAnimSampleContext)
{
    afxBool         accelerated;
    afxUnit         firstPivot;
    afxUnit         pivotCnt;
    arxCapstan        moto;
    arxPose         pose;
    arxPlacement    posb;
    afxReal         allowedErr;
    afxM4d const    displacement;
    afxUnit const*  sparseBoneArray;
};

ARX afxBool     ArxSampleBodyAnimationsUnified(arxBody bod, arxAnimSampleContext const* ctx);

ARX void        ArxCmdBindAttitude(afxDrawContext dctx, arxPose pose);
ARX void        ArxCmdBindPosture(afxDrawContext dctx, arxPlacement wp);
ARX void        ArxCmdBindSparsePivotMap(afxDrawContext dctx, afxUnit const* sparseBoneArray);
ARX void        ArxCmdSetAllowedSamplingError(afxDrawContext dctx, afxReal allowedErr);
ARX void        ArxCmdSampleBodyMotions(afxDrawContext dctx, arxBody bod, afxUnit basePivot, afxUnit pivotCnt);

ARX void        ArxSetBodyMass(arxBody bod, afxV3d mass);

ARX void        ArxDoBodyDynamics(arxBody bod, afxReal dt);
ARX void        ArxApplyForceAndTorque(arxBody bod, afxV3d const force, afxV3d const torque);

ARX afxBool     ArxGetBodyPose(arxBody bod, arxPose* pose, arxPlacement* placement);

ARX afxError    ArxNodulateBody
(
    arxBody     bod, 
    afxUnit     basePartIdx, 
    afxUnit     cnt, 
    arxNode     nod, 
    void        (*sync)(arxNodular*), 
    afxFlags    flags, 
    afxMask     mask
);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(arxBodyInfo)
{
    afxReal mass;
    afxTransform startWorldTransform;
    //asxShape* collisionShape;
    arxModel mdl;
    afxV3d localInertia;
    afxReal linearDamp;
    afxReal angularDamp;
    afxReal friction; // 0.5
    afxReal rollFriction;
    afxReal spinFriction; //torsional friction around contact normal
    afxReal restitution;
    afxReal linearSleepThreshold; // 0.8
    afxReal angularSleepThreshold; // 1.0
};

ARX afxError    ArxSpawnBodies(arxModel proto, afxUnit cnt, arxBody bod[]);

ARX afxError    ArxAcquireBodies(afxSimulation sim, arxModel mdl, afxUnit cnt, arxBody bodies[]);

ARX afxUnit     ArxPerformManipulatedPose(arxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, arxTrackMask* modelMask, afxUnit cnt, arxBody bodies[]);

ASX afxUnit         AfxEnumerateBodies(afxSimulation sim, afxUnit first, afxUnit cnt, arxBody bodies[]);
ASX afxUnit         AfxInvokeBodies(afxSimulation sim, afxUnit first, afxUnit cnt, afxBool(*f)(arxBody, void*), void *udd);

#endif//ARX_BODY_H
