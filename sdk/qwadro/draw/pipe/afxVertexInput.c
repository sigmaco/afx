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

#define _AFX_DRAW_C
#define _AFX_VERTEX_INPUT_C
#include "qwadro/draw/afxDrawSystem.h"

_AVX afxNat AfxCountVertexInputAttributes(afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return vin->attrCnt;
}

_AVX afxNat AfxCountVertexInputStreams(afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return vin->streamCnt;
}

_AVX afxNat AfxMeasureVertexInputStream(afxVertexInput vin, afxNat streamIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    afxNat siz = 0;

    for (afxNat i = 0; i < vin->attrCnt; i++)
    {
        afxVertexInputAttr const* via = &vin->attrs[i];

        if (via->streamIdx == streamIdx)
        {
            siz += AfxVertexFormatGetSize(via->fmt);
        }
    }
    return siz;
}

////////////////////////////////////////////////////////////////////////////////

_AVX afxVertexInput AfxAcquireVertexInput(afxDrawContext dctx, afxNat streamCnt, afxVertexInputStream const streams[], afxNat attrCnt, afxVertexInputAttr const attrs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(streamCnt);
    AfxAssert(streams);
    AfxAssert(attrCnt);
    AfxAssert(attrs);

    afxVertexInput vsup = NIL;

    afxManager* cls = AfxGetVertexInputClass(dctx);
    AfxAssertClass(cls, afxFcc_VIN);

    if (AfxAcquireObjects(cls, 1, (afxObject*)&vsup, (void const*[]) { dctx, &streamCnt, streams, &attrCnt, attrs }))
        AfxThrowError();

    return vsup;
}
