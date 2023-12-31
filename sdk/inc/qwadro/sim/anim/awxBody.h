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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_BODY_H
#define AFX_BODY_H

#include "../modeling/awxModel.h"

/// O objeto awxBody aloja o estado de um awxModel no tempo de execução.
/// O awxModel é apenas dados estruturados para um modelo particular --- isto é, este não há qualquer conceito de onde este está, qual estado este está em, ou quais animações estão interagindo em si.
/// O awxBody mantém rastreio disto e provém um número de funções úteis para facilitar o trabalho com estados de modelo. 

/// Enquanto todas as suas entidades idênticas deveriam compartilhar o mesmo awxModel, desde que seus dados de malha e esqueleto sejam idênticos, cada um deveria ainda haver seu próprio awxBody.
/// Dito isto porque os dados do awxBody são todos dinâmicos, e não são compartilhados através de múltiplos modelos, mesmo se estes foram feitos do mesmo awxModel.
/// Assim, enquanto é comum colocar a hierarquia de estruturas do awxModel no cache e instanciá-las múltiplas vezes, você nunca deveria colocar objetos awxBody no cache e reusá-los.
/// Eles deveriam sempre ser instanciados e liberados ao longo da existência das entidades que eles representam na sua aplicação.

/// Note, no entanto, que estes são simplesmente ponteiros para os dados originais que vocÊ passou durante a instanciação.
/// O awxBody não mantém cópias dos dados (para minimizar uso de memória), então você não pode liberar os dados originais do modelo e esperar obter ponteiros válidos de volta aqui.
/// Para aquilo que importa, se você liberar os dados originais do modelo, nenhuma das chamadas para awxBody funcionarão, desde que elas todas dependem daqueles dados, como você esperaria.

AFX_OBJECT(awxBody)
{
    awxModel            mdl; // awxModel
    awxSkeleton         cachedSkl;
    void*               cachedBones;
    afxNat              cachedBoneCnt;
    afxChain            motors;
    void*               userData[4];
    afxNat32            reserved0,
                        reserved1;

    afxReal             mass; // totally of masses of parts. Additive to initial "fake" value. May be used to create physical behavoir on empty bodies.
    afxV4d              centre;
    afxV4d              velocity;
    afxV4d              force;
    afxV4d              torque;

    afxReal             linearDamping;
    afxReal             angularDamping;
    awxPose             pose;
};

AFX awxModel            AwxGetBodyModel(awxBody bod);
AFX awxSkeleton         AwxGetBodySkeleton(awxBody bod);
    
AFX void                AwxResetBodyClock(awxBody bod);
AFX void                AwxUpdateBodyModelMatrix(awxBody bod, afxReal secsElapsed, afxBool inverse, afxReal const mm[4][4], afxReal m[4][4]);

AFX void                AwxRecenterBodyAllClocks(awxBody bod, afxReal currClock);

AFX void                AwxSetBodyClock(awxBody bod, afxReal newClock);
AFX void                AwxFreeCompletedBodyControls(awxBody bod);
AFX void **             AwxGetBodyUserDataArray(awxBody bod);

AFX void                AwxGetBodyRootMotionVectors(awxBody bod, afxReal secsElapsed, afxBool inverse, afxReal translation[3], afxReal rotation[3]);
#if 0
GRANNY_DYNLINK(void) GrannyClipRootMotionVectors(granny_real32 const * Translation3,
    granny_real32 const * Rotation3,
    granny_uint32 AllowableDOFs,
    granny_real32 * AllowedTranslation3,
    granny_real32 * AllowedRotation3,
    granny_real32 * DisallowedTranslation3,
    granny_real32 * DisallowedRotation3);

GRANNY_DYNLINK(void) GrannyApplyRootMotionVectorsToLocalPose(granny_local_pose * Pose,
    granny_real32 const * Translation3,
    granny_real32 const * Rotation3);

#endif

////////////////////////////////////////////////////////////////////////////////
// MASSIVE OPERATIONS                                                         //
////////////////////////////////////////////////////////////////////////////////

AFX afxError            AwxEmbodyModel(awxModel mdl, afxNat cnt, awxBody bod[]);

#endif//AFX_BODY_H
