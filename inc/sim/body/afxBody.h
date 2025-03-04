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

/// O objeto afxBody aloja o estado de um afxModel no tempo de execução.
/// O afxModel é apenas dados estruturados para um modelo particular --- isto é, este não há qualquer conceito de onde este está, qual estado este está em, ou quais animações estão interagindo em si.
/// O afxBody mantém rastreio disto e provém um número de funções úteis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades idênticas deveriam compartilhar o mesmo afxModel, desde que seus dados de malha e esqueleto sejam idênticos, cada um deveria ainda haver seu próprio afxBody.
/// Dito isto porque os dados do afxBody são todos dinâmicos, e não são compartilhados através de múltiplos modelos, mesmo se estes foram feitos do mesmo afxModel.
/// Assim, enquanto é comum colocar a hierarquia de estruturas do afxModel no cache e instanciá-las múltiplas vezes, você nunca deveria colocar objetos afxBody no cache e reusá-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da existência das entidades que eles representam na sua aplicação.

/// Note, no entanto, que estes são simplesmente ponteiros para os dados originais que vocÊ passou durante a instanciação.
/// O afxBody não mantém cópias dos dados (para minimizar uso de memória), então você não pode liberar os dados originais do modelo e esperar obter ponteiros válidos de volta aqui.
/// Para aquilo que importa, se você liberar os dados originais do modelo, nenhuma das chamadas para afxBody funcionarão, desde que elas todas dependem daqueles dados, como você esperaria.

#ifndef ASX_BODY_H
#define ASX_BODY_H

#include "qwadro/inc/sim/io/afxModel.h"
#include "qwadro/inc/sim/afxCapstan.h"

ASX afxBool     AfxGetBodyModel(afxBody bod, afxModel* model);

ASX afxUnit     AfxCountBodyMotives(afxBody bod);

ASX void        AsxResetBodyClock(afxBody bod);

ASX void        AfxRecenterBodyMotiveClocks(afxBody bod, afxReal currClock);

/// Update all the motors affecting a particular puppet.
ASX void        AfxUpdateBodyMotives(afxBody bod, afxReal newClock);

ASX void        AfxPurgeTerminatedMotives(afxBody bod);

ASX void        AfxUpdateBodyMatrix(afxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m);

ASX void        AfxAccumulateBodyAnimations(afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose rslt, afxReal allowedErr, afxUnit const sparseJntMap[]);

ASX void        AfxQueryBodyRootMotionVectors(afxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation);


AFX_DEFINE_STRUCT(afxAnimSampleContext)
{
    afxBool         accelerated;
    afxUnit         firstPivot;
    afxUnit         pivotCnt;
    afxCapstan        moto;
    afxPose         pose;
    afxPlacement    posb;
    afxReal         allowedErr;
    afxM4d const    displacement;
    afxUnit const*  sparseBoneArray;
};

ASX afxBool     AfxSampleBodyAnimationsUnified(afxBody bod, afxAnimSampleContext const* ctx);

ASX void        AsxCmdBindAttitude(afxDrawContext dctx, afxPose pose);
ASX void        AsxCmdBindPosture(afxDrawContext dctx, afxPlacement wp);
ASX void        AsxCmdBindSparsePivotMap(afxDrawContext dctx, afxUnit const* sparseBoneArray);
ASX void        AsxCmdSetAllowedSamplingError(afxDrawContext dctx, afxReal allowedErr);
ASX void        AsxCmdSampleBodyMotions(afxDrawContext dctx, afxBody bod, afxUnit basePivot, afxUnit pivotCnt);

ASX void        AfxSetBodyMass(afxBody bod, afxV3d mass);

ASX void        AfxDoBodyDynamics(afxBody bod, afxReal dt);
ASX void        AfxApplyForceAndTorque(afxBody bod, afxV3d const force, afxV3d const torque);

ASX afxBool     AfxGetBodyPose(afxBody bod, afxPose* pose);

////////////////////////////////////////////////////////////////////////////////

ASX afxError    AfxSpawnBodies(afxModel proto, afxUnit cnt, afxBody bod[]);

ASX afxError    AfxAcquireBodies(afxSimulation sim, afxModel mdl, afxUnit cnt, afxBody bodies[]);

ASX afxUnit     AsxPerformManipulatedPose(afxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, akxTrackMask* modelMask, afxUnit cnt, afxBody bodies[]);

#endif//ASX_BODY_H
