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

#define _AVX_DRAW_C
#define _AVX_VERTEX_INPUT_C
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

_AVX afxError _AvxVinStdDtor(afxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    //afxDrawContext dctx = AfxGetObjectProvider(vin);
    
    if (vin->streams)
        AfxDeallocate(vin->streams);

    if (vin->attrs)
        AfxDeallocate(vin->attrs);

    return err;
}

_AVX afxError _AvxVinStdCtor(afxVertexInput vin, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);

    afxDrawContext dctx = cookie->udd[0];
    afxNat streamCnt = *(afxNat const*)cookie->udd[1];
    afxVertexInputStream const* streams = cookie->udd[2];
    afxNat attrCnt = *(afxNat const*)cookie->udd[3];
    afxVertexInputAttr const* attrs = cookie->udd[4];

    if (attrCnt && !(vin->attrs = AfxAllocate(attrCnt, sizeof(vin->attrs[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < attrCnt; i++)
        {
            afxVertexInputAttr* attr = &vin->attrs[i];
            attr->fmt = attrs[i].fmt;
            attr->location = attrs[i].location;
            attr->offset = attrs[i].offset;
            attr->streamIdx = attrs[i].streamIdx;
            attr->normalized = !!attrs[i].normalized;
        }
        vin->attrCnt = attrCnt;

        if (streamCnt && !(vin->streams = AfxAllocate(streamCnt, sizeof(vin->attrs[0]), 0, AfxHere()))) AfxThrowError();
        else
        {
            for (afxNat i = 0; i < streamCnt; i++)
            {
                afxVertexInputStream* stream = &vin->streams[i];
                stream->slotIdx = streams[i].slotIdx;
                //stream->stride = streams[i].stride;
                stream->instanceRate = streams[i].instanceRate;
            }
            vin->streamCnt = streamCnt;

            if (err && vin->streams)
                AfxDeallocate(vin->streams);
        }

        if (err && vin->attrs)
            AfxDeallocate(vin->attrs);
    }
    return err;
}

_AVX afxClassConfig const _AvxVinStdImplementation =
{
    .fcc = afxFcc_VIN,
    .name = "VertexInput",
    .desc = "Vertex Input Declaration",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxVertexInput)),
    .ctor = (void*)_AvxVinStdCtor,
    .dtor = (void*)_AvxVinStdDtor
};

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
