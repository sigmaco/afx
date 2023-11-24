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

#ifndef AFX_POSE_H
#define AFX_POSE_H

#include "afx/math/afxTransform.h"

AFX_DEFINE_STRUCT(afxPoseTransform)
{
    afxReal             weight;
    afxNat              cnt;
    afxTransform        xform;
    afxNat              traversalId;
};

AFX_DEFINE_STRUCT(afxPose)
{
    afxNat              cap;
    afxPoseTransform*   xforms;
    afxReal             fillThreshold;
    afxNat              traversalId;
};

/// O objeto afxPostura é um buffer usado para manter o estado de um afxSkeleton de um afxModel como expressado em sua "pose local". 
/// Neste contexto, "pose local" significa que cada articulação no afxSkeleton é representada por um afxTransform que é relativo a sua articulação-parente imediata. 
/// Você pode criar uma afxPostura assim: 

AFX afxError        AfxAcquirePoses(void *sim, afxNat cnt, afxNat const cap[], afxPose *lp[]);

/// e quando você estiver satisfeito de usá-lo, você deve liberá-lo assim: 

AFX void            AfxReleasePoses(afxNat cnt, afxPose *lp[]);

/// Você pode encontrar o número de articulações representadas pelo afxPose assim: 

AFX afxNat          AfxGetPoseCapacity(afxPose const *lp);

/// Em qualquer tempo, você pode inspecionar ou modificar o estado alojado de uma articulação na afxPose. Você acessa o estado da articulação como um afxTransform assim: 

AFX afxTransform*   AfxGetPoseTransform(afxPose const *pose, afxNat artIdx);

/// Note que você recebe um ponteiro para o afxTransform alojado para a articulação, assim sendo, modificá-lo modificará a versão permanente alojada na afxPose. 
/// Portanto, não há uma função "Set" para AfxGetPoseTransform() devido ao fato de que você pode ler de ou escrever para o ponteiro que você recebe.

AFX void            AfxCopyPose(afxPose *pose, afxPose const *from);

#endif//AFX_POSE_H
