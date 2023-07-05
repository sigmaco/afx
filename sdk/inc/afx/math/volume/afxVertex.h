/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/core/afxCoreDefs.h"

typedef afxNat8 afxVertexIndex8;
typedef afxNat16 afxVertexIndex16;
typedef afxNat32 afxVertexIndex32;
typedef afxVertexIndex8 afxIndexedPoint8[1];
typedef afxVertexIndex16 afxIndexedPoint16[1];
typedef afxVertexIndex32 afxIndexedPoint32[1];
typedef afxVertexIndex8 afxIndexedLine8[2];
typedef afxVertexIndex16 afxIndexedLine16[2];
typedef afxVertexIndex32 afxIndexedLine32[2];
typedef afxVertexIndex8 afxIndexedTriangle8[3];
typedef afxVertexIndex16 afxIndexedTriangle16[3];
typedef afxVertexIndex32 afxIndexedTriangle32[3];

#define AFX_INVALID_INDEX8 (afxVertexIndex8)AFX_N8_MAX
#define AFX_INVALID_INDEX16 (afxVertexIndex16)AFX_N16_MAX
#define AFX_INVALID_INDEX32 (afxVertexIndex32)AFX_N32_MAX
