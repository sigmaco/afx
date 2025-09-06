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
/// Assim sendo, uma arxAnimation é a moção de um conjunto de arxModel's animando sobre tempo.
/// A arxAnimation contém um ou mais arxMotion's, cada do qual correspondendo à moção de um modelo específico (desde que uma animação pode envolver múltiplos modelos, se o autor assim escolher).
/// O arxMotion é constituído de curvas, cada da qual especifica a translação, rotação e escala de uma junta no akxSkeleton do correspondente arxModel.

#ifndef ARX_ANIMATION_H
#define ARX_ANIMATION_H

#include "qwadro/sim/afxSimDefs.h"
#include "qwadro/sim/arxCurve.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/base/afxFixedString.h"
#include "qwadro/sim/arxMotion.h"
#include "qwadro/cad/arxPose.h"
#include "qwadro/sim/arxBody.h"
#include "qwadro/sim/arxCapstan.h"

typedef enum arxAnimationFlag
{
    arxAnimationFlag_NONE
} arxAnimationFlags;

AFX_DEFINE_STRUCT(arxAnimationBlueprint)
{
    afxReal         dur;
    afxReal         timeStep;
    afxReal         oversampling;
    afxUnit         motSlotCnt;
    arxMotion*      motions;
    // TODO make motions' names a animation property to ease portability of motions.
    afxString32     id;
};

ARX afxBool     ArxGetAnimationUrn(arxAnimation ani, afxString* id);

ARX afxBool     ArxFindMotion(arxAnimation ani, afxString const* id, afxUnit *motIdx);

ARX afxError    ArxGetMotions(arxAnimation ani, afxUnit first, afxUnit cnt, arxMotion motions[]);

ARX afxError    ArxRelinkMotions(arxAnimation ani, afxUnit baseSlot, afxUnit slotCnt, arxMotion motions[]);

ARX afxUnit     ArxPerformAnimation(arxAnimation ani, afxReal startTime, afxUnit iterCnt, afxUnit cnt, arxBody bodies[]);

//ARX afxUnit    ArxPerformAnimationBinding(arxAnimation ani, afxReal startTime, afxUnit iterCnt, struct arxInstancedAnimation *binding, afxUnit cnt, arxBody bodies[]);

ARX afxError    ArxArchiveAnimation(arxAnimation ani, afxUri const* uri);

////////////////////////////////////////////////////////////////////////////////

ARX afxError    ArxLoadAnimation(afxSimulation sim, afxString const* urn, afxUri const* uri, arxAnimation* animation);

ARX afxError    ArxAssembleAnimations(afxSimulation sim, afxUnit cnt, arxAnimationBlueprint const blueprints[], arxAnimation animations[]);

ARX void        ArxTransformAnimations(afxM3d const ltm, afxM3d const iltm, afxReal linearTol, afxV4d const atv, afxReal affineTol, afxFlags flags, afxUnit cnt, arxAnimation animations[]);

#endif//ARX_ANIMATION_H
