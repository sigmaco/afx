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

// Morphology is the study of form and structure. 
// Morphology involves the study of landforms, their shapes, and how they were formed.
// Morphology refers to the form and structure of organisms, both internal and external.

#ifndef ASX_MORPHOLOGY_H
#define ASX_MORPHOLOGY_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/sim/io/afxModel.h"
#include "qwadro/inc/sim/body/afxMaterial.h"

////////////////////////////////////////////////////////////////////////////////

ASX afxUnit         AfxFindMaterialIndices(afxMorphology morp, afxUnit cnt, afxString const materials[], afxUnit indices[]);

ASX afxUnit         AfxEnumerateMaterials(afxMorphology morp, afxUnit first, afxUnit cnt, afxMaterial materials[]);

ASX afxError        AfxDeclareMaterials(afxMorphology morp, afxUnit cnt, afxString const ids[], afxMaterial materials[]);

ASX afxError        AfxAcquireMorphology(afxSimulation sim, afxMorphology* morphology);

#endif//ASX_MORPHOLOGY_H
