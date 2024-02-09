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
 *                      (c 2017 SIGMA Co. & SIGMA Technology Group
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_ANIMATION_H
#define AFX_ANIMATION_H

#include "qwadro/sim/awxSimDefs.h"
#include "afxCurve2.h"
#include "qwadro/math/afxTransform.h"
#include "qwadro/core/afxFixedString.h"

AFX_DEFINE_STRUCT(awxMotionData)
{
    afxFixedString32         id;
    afxNat              seqKey;
    afxInt              dimension;
    afxCurve*           valueCurve;
};

AFX_DEFINE_STRUCT(awxMotionNoteEntry)
{
    afxReal             timeStamp;
    afxFixedString128        text;
};

AFX_DEFINE_STRUCT(awxMotionNote)
{
    afxFixedString8          id;
    afxNat              entryCnt;
    awxMotionNoteEntry* entries;
};

AFX_DEFINE_STRUCT(awxPeriodicLoop)
{
    afxReal             radius;
    afxReal             dAngle;
    afxReal             dZ;
    afxV3d              basisX;
    afxV3d              basisY;
    afxV3d              axis;
};

AFX_DEFINE_STRUCT(awxMotionTransform)
{
    afxFixedString32         id;
    afxMask             flags;
    afxCurve*           rotCurve;
    afxCurve*           posCurve;
    afxCurve*           deformCurve;
};

#ifdef _AFX_MOTION_C
AFX_OBJECT(awxMotion)
{
    afxFixedString32         id;
    afxMask             flags;
    afxNat              xformSeqCnt;
    awxMotionTransform* xformSeqs; // uma arranjo de registros da moção de junta de afxSkeleton do correspondente afxModel.
    afxNat              xformLodErrorCnt;
    afxReal*            xformLodErrors;
    afxNat              vecSeqCnt;
    awxMotionData*      vecSeqs;
    afxNat              noteSeqCnt;
    awxMotionNote*      noteSeqs;
    afxTransform        init;
    awxMotionTransform* rootMotion;
    afxV3d              loopTranslation;
    awxPeriodicLoop*    periodicLoop;    
};
#endif

/// Qwadro armazena animações em partes baseadas em quão muitos modelos estão envolvidos numa animação.
/// Assim sendo, uma awxAnimation é a moção de um conjunto de afxModel's animando sobre tempo.
/// A awxAnimation contém um ou mais awxMotion's, cada do qual correspondendo à moção de um modelo específico (desde que uma animação pode envolver múltiplos modelos, se o autor assim escolher).
/// O awxMotion é constituído de curvas, cada da qual especifica a translação, rotação e escala de uma junta no afxSkeleton do correspondente afxModel.

#ifdef _AFX_ANIMATION_C
AFX_OBJECT(awxAnimation)
{
    afxFixedString32         id;

    afxReal             dur; /// especifica quão demorado, em segundos, a animação leva para executar a completação.
    afxReal             timeStep; /// especifica quão muitos segundos há entre cada frame.
    afxReal             oversampling; /// 1.f

    /// @timeStep é o intervalo após oversampling.
    /// Quando @timeStep e @oversampling forem iguais a 0.00416667 e 4.0, respectivamente, uma animação foi produzida a 60 FPS (0.016667 segundos entre cada frame) com oversampling de 4x.
    /// Multiplicando @timeStep por @oversampling, pode-se encontrar o intervalo original antes do oversampling.
    /// Tanto @timeStep quanto @oversampling não são usadas pelo Qwadro.
    /// Estão aqui de forma informativa preservando conhecimento sobre a intenção original do autor.

    afxNat              motionCnt;
    awxMotion*          motions;
};
#endif

AFX void        AwxTransformAnimations(afxReal const ltm[3][3], afxReal const iltm[3][3], afxReal const atv[4], afxReal affineTol, afxReal linearTol, afxFlags flags, afxNat cnt, awxAnimation ani[]);

#endif//AFX_ANIMATION_H
