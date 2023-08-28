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
#include "afxSkeleton.h"

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

//AFX afxError            AfxEmbodyModel(afxModel mdl, afxBody *bod);

AFX afxUri const*       AfxModelGetUri(afxModel mdl);
AFX afxSkeleton         AfxModelGetSkeleton(afxModel mdl);

AFX void                AfxModelGetInitialPlacement(afxModel mdl, afxM4d m);
AFX void                AfxModelTransform(afxModel mdl, afxV3d const affine, afxM3d const linear, afxM3d const invLinear, afxReal affineTol, afxReal linearTol, afxFlags flags);

AFX afxMesh             AfxModelGetBoundMesh(afxModel mdl, afxNat mshIdx);
AFX afxNat              AfxModelGetMeshBindingCount(afxModel mdl);

#endif//AFX_MODEL_H