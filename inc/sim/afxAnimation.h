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
/// Assim sendo, uma afxAnimation é a moção de um conjunto de afxModel's animando sobre tempo.
/// A afxAnimation contém um ou mais afxMotion's, cada do qual correspondendo à moção de um modelo específico (desde que uma animação pode envolver múltiplos modelos, se o autor assim escolher).
/// O afxMotion é constituído de curvas, cada da qual especifica a translação, rotação e escala de uma junta no akxSkeleton do correspondente afxModel.

#ifndef AMX_ANIMATION_H
#define AMX_ANIMATION_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/math/afxCurve.h"
#include "qwadro/inc/math/afxTransform.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/sim/afxMotion.h"
#include "qwadro/inc/cad/afxPose.h"

AFX_DEFINE_STRUCT(afxAnimationBlueprint)
{
    afxReal         dur;
    afxReal         timeStep;
    afxReal         oversampling;
    afxUnit          motSlotCnt;
    afxMotion*      motions;
    afxString32     id;
};

AMX afxBool     AfxGetAnimationUrn(afxAnimation ani, afxString* id);

AMX afxBool     AfxFindMotion(afxAnimation ani, afxString const* id, afxUnit *motIdx);

AMX afxError    AkxRelinkMotions(afxAnimation ani, afxUnit baseSlot, afxUnit slotCnt, afxMotion motions[]);

AMX afxUnit      AfxPerformAnimation(afxAnimation ani, afxReal startTime, afxUnit iterCnt, afxUnit cnt, afxBody bodies[]);

//AMX afxUnit    AfxPerformAnimationBinding(afxAnimation ani, afxReal startTime, afxUnit iterCnt, struct animation_binding *binding, afxUnit cnt, afxBody bodies[]);

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AkxAssembleAnimations(afxSimulation sim, afxUnit cnt, afxAnimationBlueprint const blueprints[], afxAnimation animations[]);

AMX void        AkxTransformAnimations(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxUnit cnt, afxAnimation ani[]);

#endif//AMX_ANIMATION_H
