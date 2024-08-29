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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// When the vertex stage is present in a pipeline, the vertex shader input variables form an interface with the vertex input attributes.
/// All vertex shader inputs declared must have a corresponding attribute and binding in the pipeline.

#ifndef AVX_VERTEX_INPUT_H
#define AVX_VERTEX_INPUT_H

#include "qwadro/inc/draw/afxDrawDefs.h"

AFX_DEFINE_STRUCT(avxVertexInputAttr)
{
    afxNat                  location;
    afxNat                  streamIdx;
    afxNat32                offset;
    afxVertexFormat         fmt;
    afxBool                 normalized;
};

AFX_DEFINE_STRUCT(avxVertexInputStream)
{
    afxNat                  slotIdx;
    //afxNat32                stride;
    afxNat                  instanceRate; // 0
};

AVX afxNat              AfxCountVertexInputStreams(avxVertexInput vin);
AVX afxNat              AfxCountVertexInputAttributes(avxVertexInput vin);

AVX afxNat              AfxMeasureVertexInputStream(avxVertexInput vin, afxNat streamIdx);

////////////////////////////////////////////////////////////////////////////////

AVX avxVertexInput      AfxAcquireVertexInput(afxDrawContext dctx, afxNat streamCnt, avxVertexInputStream const streams[], afxNat attrCnt, avxVertexInputAttr const attrs[]);

#endif//AVX_VERTEX_INPUT_H
