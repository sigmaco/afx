/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_MODEL_H
#define AFX_MODEL_H

#include "afxMesh.h"
#include "afx/sim/afxSkeleton.h"

/// O objeto afxModel descreve uma coleção de afxMesh'es que são todas ligadas ao mesmo afxSkeleton.
/// Isso é essencialmente qualquer grupo conectado de malhas que são animadas em conjuntura.
/// Um ator seria um modelo, assim seria um veículo, ou mesmo uma edificação inteira se essa fossa modelada como uma hierarquia singular.
/// Um afxModel completo é feito de um afxSkeleton e um conjunto de afxMesh'es, ambos dos quais podem ser acessados diretamente da estrutura do afxModel.

AFX_DEFINE_STRUCT(afxMeshSlot)
{
    afxMesh             msh;
};

#ifdef _AFX_MODEL_C
AFX_OBJECT(afxModel)
{
    afxUri              uri; // 128
    afxSkeleton         skl;
    afxTransform        init;
    afxAabb             aabb;
    afxNat              meshSlotCnt;
    afxMeshSlot         *meshSlots;
};
#endif


AFX afxError            AfxFetchModels(afxSimulation sim, afxNat cnt, afxUri const uri[], afxModel mdl[]);

AFX afxModel            AfxAssembleModel(afxSimulation sim, afxUri const* name, afxSkeleton skl, afxTransform const* init, afxNat mshCnt, afxMesh msh[]);

AFX afxUri const*       AfxGetModelUri(afxModel mdl);

AFX afxSkeleton         AfxGetModelSkeleton(afxModel mdl);
AFX void                AfxSetModelSkeleton(afxModel mdl, afxSkeleton skl);

AFX void                AfxGetModelInitialPlacement(afxModel mdl, afxReal m[4][4]);
AFX void                AfxSetModelInitialPlacement(afxModel mdl, afxTransform const* xform);

AFX void                AfxTransformModel(afxModel mdl, afxReal const affine[3], afxReal const linear[3][3], afxReal const invLinear[3][3], afxReal affineTol, afxReal linearTol, afxFlags flags);

AFX afxMesh             AfxGetModelMesh(afxModel mdl, afxNat slotIdx);
AFX void                AfxSetModelMesh(afxModel mdl, afxNat slotIdx, afxMesh msh);

AFX afxNat              AfxCountModelMeshes(afxModel mdl);

#endif//AFX_MODEL_H