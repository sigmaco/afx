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

#ifndef AMX_BODY_H
#define AMX_BODY_H

#include "qwadro/inc/cad/afxModel.h"
#include "qwadro/inc/sim/afxMotor.h"

AMX afxBool     AfxGetBodyModel(afxBody bod, afxModel* model);
AMX afxBool     AfxGetBodySkeleton(afxBody bod, afxSkeleton* skeleton);
    
AMX void        AkxResetBodyClock(afxBody bod);
AMX void        AfxUpdateBodyMatrix(afxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m);

AMX void        AfxRecenterBodyMotiveClocks(afxBody bod, afxReal currClock);

/// Update all the motors affecting a particular puppet.
AMX void        AfxUpdateBodyMotives(afxBody bod, afxReal newClock);

AMX void        AfxPurgeTerminatedMotives(afxBody bod);

AMX void        AfxGetBodyRootMotionVectors(afxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation);

AFX_DEFINE_STRUCT(afxAnimSampleContext)
{
    afxBool         accelerated;
    afxUnit          firstPivot;
    afxUnit          pivotCnt;
    afxMotor        moto;
    afxPose         pose;
    afxPoseBuffer   posb;
    afxReal         allowedErr;
    afxM4d const    displacement;
    afxUnit const*   sparseBoneArray;
};

AMX afxBool     AfxSampleBodyAnimations(afxBody bod, afxAnimSampleContext const* ctx);

AMX void        AfxSampleBodyAnimationsLODSparse(afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose rslt, afxReal allowedErr, afxUnit const* sparseBoneArray);
AMX void        AfxSampleBodyAnimationsAcceleratedLOD(afxBody bod, afxUnit pivotCnt, afxM4d const offset, afxPose scratch, afxPoseBuffer rslt, afxReal allowedErr);
AMX afxBool     AfxSampleSingleBodyAnimationLODSparse(afxBody bod, afxMotor moto, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose rslt, afxReal allowedErr, afxUnit const* sparseBoneArray);
AMX void        AfxAccumulateBodyAnimationsLODSparse(afxBody bod, afxUnit basePivotIdx, afxUnit pivotCnt, afxPose rslt, afxReal allowedErr, afxUnit const* sparseBoneArray);

AMX void        AkxCmdBindPose(avxCmdb cmdb, afxPose pose);
AMX void        AkxCmdBindPoseBuffer(avxCmdb cmdb, afxPoseBuffer wp);
AMX void        AkxCmdBindSparsePivotMap(avxCmdb cmdb, afxUnit const* sparseBoneArray);
AMX void        AkxCmdSetAllowedSamplingError(avxCmdb cmdb, afxReal allowedErr);
AMX void        AkxCmdSampleBodyMotions(avxCmdb cmdb, afxBody bod, afxUnit basePivot, afxUnit pivotCnt);

AMX void        AfxSetBodyMass(afxBody bod, afxV3d mass);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AfxSpawnBodies(afxModel proto, afxUnit cnt, afxBody bod[]);

AMX afxError    AfxAcquireBodies(afxSimulation sim, afxModel mdl, afxUnit cnt, afxBody bodies[]);

AMX afxUnit      AfxPerformManipulatedPose(afxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, akxTrackMask* modelMask, afxUnit cnt, afxBody bodies[]);

#endif//AMX_BODY_H
