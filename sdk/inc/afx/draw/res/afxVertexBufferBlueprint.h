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

#ifndef AFX_VERTEX_BUFFER_BLUEPRINT_H
#define AFX_VERTEX_BUFFER_BLUEPRINT_H

#include "afx/draw/res/afxVertexBuffer.h"

AFX_DEFINE_STRUCT(afxVertexRowBlueprint)
{
    afxString8              semantic; // 8
    afxVertexFormat         fmt;
    afxVertexUsage          usage;
    void const              *src;
    afxVertexFormat         srcFmt;
};

AFX_DEFINE_STRUCT(afxVertexBufferBlueprint)
{
    afxFcc                  fcc; // VBUB
    afxNat                  cap;
    afxNat                  rowCnt;
    afxVertexRowBlueprint   row[8];
};

AFXINL void                 AfxVertexBufferBlueprintReset(afxVertexBufferBlueprint *blueprint, afxNat cap);
AFXINL void                 AfxVertexBufferBlueprintDiscard(afxVertexBufferBlueprint *blueprint);
AFXINL void                 AfxVertexBufferBlueprintErase(afxVertexBufferBlueprint *blueprint);

AFXINL afxError             AfxVertexBufferBlueprintAddRow(afxVertexBufferBlueprint *blueprint, afxString const *semantic, afxVertexFormat fmt, afxVertexUsage usage, void const *src, afxVertexFormat srcFmt);
AFXINL afxError             AfxVertexBufferBlueprintResetRow(afxVertexBufferBlueprint *blueprint, afxNat idx, afxString const *semantic, afxVertexFormat fmt, afxVertexUsage usage, void const *src, afxVertexFormat srcFmt);

#endif//AFX_VERTEX_BUFFER_BLUEPRINT_H