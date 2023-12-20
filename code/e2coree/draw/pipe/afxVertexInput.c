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

#define _AFX_DRAW_C
#define _AFX_VERTEX_INPUT_C
#include "qwadro/draw/pipe/afxVertexInput.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/afxXsh.h"

_AFX afxNat AfxCountVertexInputAttributes(afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return vin->attrCnt;
}

_AFX afxNat AfxCountVertexInputStreams(afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return vin->streamCnt;
}

_AFX afxNat AfxMeasureVertexInputStream(afxVertexInput vin, afxNat streamIdx)
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

_AFX afxVertexInput AfxAcquireVertexInput(afxDrawContext dctx, afxNat streamCnt, afxVertexInputStream const streams[], afxNat attrCnt, afxVertexInputAttr const attrs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AfxAssert(streamCnt);
    AfxAssert(streams);
    AfxAssert(attrCnt);
    AfxAssert(attrs);

    afxVertexInput vsup = NIL;

    afxClass* cls = AfxGetVertexInputClass(dctx);
    AfxAssertClass(cls, afxFcc_VIN);

    if (AfxAcquireObjects(cls, 1, (afxObject*)&vsup, (void const*[]) { dctx, &streamCnt, streams, &attrCnt, attrs }))
        AfxThrowError();

    return vsup;
}
