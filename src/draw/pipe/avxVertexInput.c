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
#include "../../dev/AvxImplKit.h"

_AVX afxUnit AfxCountVertexInputAttributes(avxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return vin->attrCnt;
}

_AVX afxUnit AfxCountVertexInputStreams(avxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    return vin->streamCnt;
}

_AVX afxUnit AfxMeasureVertexInputStream(avxVertexInput vin, afxUnit streamIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    afxUnit siz = 0;

    for (afxUnit i = 0; i < vin->attrCnt; i++)
    {
        avxVertexInputAttr const* via = &vin->attrs[i];

        if (via->streamIdx == streamIdx)
        {
            siz += AfxVertexFormatGetSize(via->fmt);
        }
    }
    return siz;
}

_AVX afxError _AvxVinStdDtor(avxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);
    //afxDrawContext dctx = AfxGetProvider(vin);
    
    if (vin->streams)
        AfxDeallocate(vin->streams);

    if (vin->attrs)
        AfxDeallocate(vin->attrs);

    return err;
}

_AVX afxError _AvxVinStdCtor(avxVertexInput vin, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vin, afxFcc_VIN);

    afxDrawContext dctx = args[0];
    afxUnit streamCnt = *(afxUnit const*)args[1];
    avxVertexInputStream const* streams = args[2];
    afxUnit attrCnt = *(afxUnit const*)args[3];
    avxVertexInputAttr const* attrs = args[4];

    if (attrCnt && !(vin->attrs = AfxAllocate(attrCnt, sizeof(vin->attrs[0]), 0, AfxHere()))) AfxThrowError();
    else
    {
        for (afxUnit i = 0; i < attrCnt; i++)
        {
            avxVertexInputAttr* attr = &vin->attrs[i];
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
            for (afxUnit i = 0; i < streamCnt; i++)
            {
                avxVertexInputStream* stream = &vin->streams[i];
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
    .fixedSiz = sizeof(AFX_OBJECT(avxVertexInput)),
    .ctor = (void*)_AvxVinStdCtor,
    .dtor = (void*)_AvxVinStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX avxVertexInput AfxAcquireVertexInput(afxDrawContext dctx, afxUnit streamCnt, avxVertexInputStream const streams[], afxUnit attrCnt, avxVertexInputAttr const attrs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    AFX_ASSERT(streamCnt);
    AFX_ASSERT(streams);
    AFX_ASSERT(attrCnt);
    AFX_ASSERT(attrs);

    avxVertexInput vsup = NIL;

    afxClass* cls = AfxGetVertexInputClass(dctx);
    AfxAssertClass(cls, afxFcc_VIN);

    if (AfxAcquireObjects(cls, 1, (afxObject*)&vsup, (void const*[]) { dctx, &streamCnt, streams, &attrCnt, attrs }))
        AfxThrowError();

    return vsup;
}
