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

// This section is part of SIGMA GL/2.

// A draw operation manages a set of state objects, resources, and shaders for implementing a rendering effect.
// An draw operation contains one or more techniques; each technique contains one or more passes; each pass contains state assignments.
// A draw technique is a collection of interrelated passes. Each technique contains one or more passes.
// A draw pass encapsulates state assignments within a technique.
// A pass is a block of code that sets render-state objects and shaders. A pass is declared within a technique.

#ifndef AFX_DRAW_TECHNIQUE_H
#define AFX_DRAW_TECHNIQUE_H

#include "afx/draw/afxPipeline.h"

AFX_DEFINE_HANDLE(afxDrawTechnique);

AFX_DEFINE_STRUCT(afxDrawPass)
{
    _AFX_DBG_FCC                    // afxFcc_DPAS
    afxString*                      name; // 32
    afxPipeline                     pip;
};

AFX_OBJECT(afxDrawTechnique)
{
    afxString*                      name; // 32
    afxNat                          passCnt;
    afxDrawPass*                    passes;
};

AFX void*                           AfxDrawOperationGetContext(afxDrawOperation dop);

AFX afxNat                          AfxDrawOperationGetTechniqueCount(afxDrawOperation dop);
AFX afxBool                         AfxDrawOperationFindTechnique(afxDrawOperation dop, afxString const *name, afxNat *idx);
AFX afxString const*                AfxDrawOperationGetTechniqueName(afxDrawOperation dop, afxNat tecIdx);

AFX afxNat                          AfxDrawOperationGetPassCount(afxDrawOperation dop, afxNat tecIdx);
AFX afxBool                         AfxDrawOperationFindPass(afxDrawOperation dop, afxNat tecIdx, afxString const *name, afxNat *idx);
AFX afxString const*                AfxDrawOperationGetPassName(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx);
AFX afxPipeline                     AfxDrawOperationGetPipeline(afxDrawOperation dop, afxNat tecIdx, afxNat passIdx);

#endif//AFX_DRAW_TECHNIQUE_H