/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *             Q W A D R O   R E N D E R I N G   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// Morphology is the study of form and structure. 
// Morphology involves the study of landforms, their shapes, and how they were formed.
// Morphology refers to the form and structure of organisms, both internal and external.

#ifndef ARX_GEOME_H
#define ARX_GEOME_H

#include "qwadro/inc/sim/afxSimDefs.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/render/cad/arxModel.h"
#include "qwadro/inc/render/cad/arxMaterial.h"

////////////////////////////////////////////////////////////////////////////////

ARX afxUnit         ArxFindMaterialIndices(arxGeome morp, afxUnit cnt, afxString const materials[], afxUnit indices[]);

ARX afxUnit         ArxEnumerateMaterials(arxGeome morp, afxUnit first, afxUnit cnt, arxMaterial materials[]);

ARX afxError        ArxDeclareMaterials(arxGeome morp, afxUnit cnt, afxString const ids[], arxMaterial materials[]);

ARX afxError        ArxAcquireMorphology(arxRenderware din, arxGeome* morphology);

#endif//ARX_GEOME_H
