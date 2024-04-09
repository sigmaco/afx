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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// When the vertex stage is present in a pipeline, the vertex shader input variables form an interface with the vertex input attributes.
/// All vertex shader inputs declared must have a corresponding attribute and binding in the pipeline.

#ifndef AFX_VERTEX_INPUT_H
#define AFX_VERTEX_INPUT_H

#include "qwadro/draw/afxDrawDefs.h"

AFX_DEFINE_STRUCT(afxVertexInputAttr)
{
    afxNat                  location;
    afxNat                  streamIdx;
    afxNat32                offset;
    afxVertexFormat         fmt;
    afxBool                 normalized;
};

AFX_DEFINE_STRUCT(afxVertexInputStream)
{
    afxNat                  slotIdx;
    //afxNat32                stride;
    afxNat                  instanceRate; // 0
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_VERTEX_INPUT_C
#ifndef _AFX_VERTEX_INPUT_IMPL
AFX_OBJECT(afxVertexInput)
#else
struct afxBaseVertexInput
#endif
{
    afxNat                  attrCnt;
    afxVertexInputAttr*     attrs;
    afxNat                  streamCnt;
    afxVertexInputStream*   streams;
};
#endif
#endif

AVX afxNat              AfxCountVertexInputStreams(afxVertexInput vin);
AVX afxNat              AfxCountVertexInputAttributes(afxVertexInput vin);

AVX afxNat              AfxMeasureVertexInputStream(afxVertexInput vin, afxNat streamIdx);

////////////////////////////////////////////////////////////////////////////////

AVX afxVertexInput      AfxAcquireVertexInput(afxDrawContext dctx, afxNat streamCnt, afxVertexInputStream const streams[], afxNat attrCnt, afxVertexInputAttr const attrs[]);

#endif//AFX_VERTEX_INPUT_H
