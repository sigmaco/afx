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

/// O objeto akxBody aloja o estado de um afxModel no tempo de execução.
/// O afxModel é apenas dados estruturados para um modelo particular --- isto é, este não há qualquer conceito de onde este está, qual estado este está em, ou quais animações estão interagindo em si.
/// O akxBody mantém rastreio disto e provém um número de funções úteis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades idênticas deveriam compartilhar o mesmo afxModel, desde que seus dados de malha e esqueleto sejam idênticos, cada um deveria ainda haver seu próprio akxBody.
/// Dito isto porque os dados do akxBody são todos dinâmicos, e não são compartilhados através de múltiplos modelos, mesmo se estes foram feitos do mesmo afxModel.
/// Assim, enquanto é comum colocar a hierarquia de estruturas do afxModel no cache e instanciá-las múltiplas vezes, você nunca deveria colocar objetos akxBody no cache e reusá-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da existência das entidades que eles representam na sua aplicação.

/// Note, no entanto, que estes são simplesmente ponteiros para os dados originais que vocÊ passou durante a instanciação.
/// O akxBody não mantém cópias dos dados (para minimizar uso de memória), então você não pode liberar os dados originais do modelo e esperar obter ponteiros válidos de volta aqui.
/// Para aquilo que importa, se você liberar os dados originais do modelo, nenhuma das chamadas para akxBody funcionarão, desde que elas todas dependem daqueles dados, como você esperaria.

#ifndef AKX_BODY_H
#define AKX_BODY_H

#include "qwadro/cad/afxModel.h"
#include "qwadro/space/akxMotor.h"

AKX afxBool     AfxGetBodyModel(akxBody bod, afxModel* model);
AKX afxBool     AfxGetBodySkeleton(akxBody bod, afxSkeleton* skeleton);
    
AKX void        AkxResetBodyClock(akxBody bod);
AKX void        AfxUpdateBodyMatrix(akxBody bod, afxReal secsElapsed, afxBool inverse, afxM4d const mm, afxM4d m);

AKX void        AfxRecenterLinkedMotorClocks(akxBody bod, afxReal currClock);

/// Update all the motors affecting a particular puppet.
AKX void        AfxReclockMotives(akxBody bod, afxReal newClock);

AKX void        AfxPurgeTerminatedMotives(akxBody bod);

AKX void        AfxGetBodyRootMotionVectors(akxBody bod, afxReal secsElapsed, afxBool inverse, afxV3d translation, afxV3d rotation);

AKX void        AfxSampleBodyAnimationsLODSparse(akxBody bod, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray);
AKX void        AfxSampleBodyAnimationsAcceleratedLOD(akxBody bod, afxNat pivotCnt, afxM4d const offset, akxPose scratch, akxPoseBuffer rslt, afxReal allowedErr);
AKX afxBool     AfxSampleSingleBodyAnimationLODSparse(akxBody bod, akxMotor moto, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray);
AKX void        AfxAccumulateBodyAnimationsLODSparse(akxBody bod, afxNat basePivotIdx, afxNat pivotCnt, akxPose rslt, afxReal allowedErr, afxNat const* sparseBoneArray);

AKX void        AkxCmdBindPose(avxCmdb cmdb, akxPose pose);
AKX void        AkxCmdBindPoseBuffer(avxCmdb cmdb, akxPoseBuffer wp);
AKX void        AkxCmdBindSparsePivotMap(avxCmdb cmdb, afxNat const* sparseBoneArray);
AKX void        AkxCmdSetAllowedSamplingError(avxCmdb cmdb, afxReal allowedErr);
AKX void        AkxCmdSampleBodyMotions(avxCmdb cmdb, akxBody bod, afxNat basePivot, afxNat pivotCnt);

AKX void        AfxSetBodyMass(akxBody bod, afxV3d mass);

////////////////////////////////////////////////////////////////////////////////

AKX afxError    AfxSpawnBodies(afxModel proto, afxNat cnt, akxBody bod[]);

AKX afxError    AfxAcquireBodies(afxModel mdl, afxNat cnt, akxBody bod[]);

#endif//AKX_BODY_H
