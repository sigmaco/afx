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

#include "qwadro/inc/sim/body/afxModel.h"
#include "qwadro/inc/sim/afxCapstan.h"

AMX afxBool     AfxGetBodyModel(afxBody bod, afxModel* model);
    
AMX void        AmxResetBodyClock(afxBody bod);

AMX void        AfxRecenterBodyMotiveClocks(afxBody bod, afxReal currClock);

/// Update all the motors affecting a particular puppet.
AMX void        AfxUpdateBodyMotives(afxBody bod, afxReal newClock);

AMX void        AfxPurgeTerminatedMotives(afxBody bod);

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

AMX afxBool     AfxSampleBodyAnimations(afxBody bod, afxAnimSampleContext const* ctx);

AMX void        AmxCmdBindAttitude(afxDrawContext dctx, afxPose pose);
AMX void        AmxCmdBindPosture(afxDrawContext dctx, afxPlacement wp);
AMX void        AmxCmdBindSparsePivotMap(afxDrawContext dctx, afxUnit const* sparseBoneArray);
AMX void        AmxCmdSetAllowedSamplingError(afxDrawContext dctx, afxReal allowedErr);
AMX void        AmxCmdSampleBodyMotions(afxDrawContext dctx, afxBody bod, afxUnit basePivot, afxUnit pivotCnt);

AMX void        AfxSetBodyMass(afxBody bod, afxV3d mass);

AMX void    AfxDoBodyDynamics(afxBody bod, afxReal dt);
AMX void    AfxApplyForceAndTorque(afxBody bod, afxV3d const force, afxV3d const torque);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AfxSpawnBodies(afxModel proto, afxUnit cnt, afxBody bod[]);

AMX afxError    AfxAcquireBodies(afxSimulation sim, afxModel mdl, afxUnit cnt, afxBody bodies[]);

AMX afxUnit     AfxPerformManipulatedPose(afxPose pose, afxReal startTime, afxReal duration, afxUnit iterCnt, akxTrackMask* modelMask, afxUnit cnt, afxBody bodies[]);

#endif//AMX_BODY_H
