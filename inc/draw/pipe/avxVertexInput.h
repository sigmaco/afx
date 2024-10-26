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

/**
    When the vertex stage is present in a pipeline, the vertex shader input variables form an interface with the vertex input attributes.
    All vertex shader inputs declared must have a corresponding attribute and binding in the pipeline.
*/

#ifndef AVX_VERTEX_INPUT_H
#define AVX_VERTEX_INPUT_H

#include "qwadro/inc/draw/afxDrawDefs.h"

AFX_DEFINE_STRUCT(avxVertexInputAttr)
{
    afxUnit                  location;
    afxUnit                  streamIdx;
    afxUnit32                offset;
    afxVertexFormat         fmt;
    afxBool                 normalized;
};

AFX_DEFINE_STRUCT(avxVertexInputStream)
{
    afxUnit                  slotIdx;
    //afxUnit32                stride;
    afxUnit                  instanceRate; // 0
};

AVX afxUnit              AfxCountVertexInputStreams(avxVertexInput vin);
AVX afxUnit              AfxCountVertexInputAttributes(avxVertexInput vin);

AVX afxUnit              AfxMeasureVertexInputStream(avxVertexInput vin, afxUnit streamIdx);

////////////////////////////////////////////////////////////////////////////////

AVX avxVertexInput      AfxAcquireVertexInput(afxDrawContext dctx, afxUnit streamCnt, avxVertexInputStream const streams[], afxUnit attrCnt, avxVertexInputAttr const attrs[]);

#endif//AVX_VERTEX_INPUT_H
