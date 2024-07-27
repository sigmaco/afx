#pragma once
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

/// Qwadro armazena animações em partes baseadas em quão muitos modelos estão envolvidos numa animação.
/// Assim sendo, uma akxAnimation é a moção de um conjunto de afxModel's animando sobre tempo.
/// A akxAnimation contém um ou mais akxMotion's, cada do qual correspondendo à moção de um modelo específico (desde que uma animação pode envolver múltiplos modelos, se o autor assim escolher).
/// O akxMotion é constituído de curvas, cada da qual especifica a translação, rotação e escala de uma junta no akxSkeleton do correspondente afxModel.

#ifndef AKX_ANIMATION_H
#define AKX_ANIMATION_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/space/afxCurve.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/space/akxMotion.h"
#include "qwadro/space/akxPose.h"

AFX_DEFINE_STRUCT(akxAnimationBlueprint)
{
    afxReal         dur;
    afxReal         timeStep;
    afxReal         oversampling;
    afxNat          motSlotCnt;
    akxMotion*      motions;
    afxString32     id;
};

AKX afxBool     AfxGetAnimationUrn(akxAnimation ani, afxString* id);

AKX afxBool     AfxFindMotion(akxAnimation ani, afxString const* id, afxNat *motIdx);

AKX afxError    AkxRelinkMotions(akxAnimation ani, afxNat baseSlot, afxNat slotCnt, akxMotion motions[]);

AKX afxNat      AfxRunAnimation(akxAnimation ani, afxReal startTime, afxNat iterCnt, afxNat cnt, akxBody bodies[]);

//AKX afxNat    AfxRunAnimationBinding(akxAnimation ani, afxReal startTime, afxNat iterCnt, struct animation_binding *binding, afxNat cnt, akxBody bodies[]);

////////////////////////////////////////////////////////////////////////////////

AKX afxError    AkxAssembleAnimations(afxSimulation sim, afxNat cnt, akxAnimationBlueprint const blueprints[], akxAnimation animations[]);

AKX void        AkxTransformAnimations(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxNat cnt, akxAnimation ani[]);

#endif//AKX_ANIMATION_H
