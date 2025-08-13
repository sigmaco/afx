/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_BUFFER_C
#define _AVX_VERTEX_INPUT_C
#include "avxImplementation.h"

afxError AfxReadBuffer(avxBuffer buf, afxSize offset, afxUnit range, afxUnit stride, void* data)
{
    AfxCopy(data, &(AFX_CAST(afxByte*, buf->storage[0].hostedAlloc.addr)[offset]), stride);
}

void DoVertex(afxUnit vtxId, void* data)
{

}

void _AvxFetchVertices(avxVertexInput vtxd, afxUnit vtxCnt, afxUnit instCnt, avxBufferedStream const streams[], afxByte* data)
{
    avxBuffer srcBuf[AVX_MAX_VERTEX_SOURCES];
    afxByte* srcPtr[AVX_MAX_VERTEX_SOURCES];
    afxUnit srcStride[AVX_MAX_VERTEX_SOURCES];
    avxFormatDescription attrFd[AVX_MAX_VERTEX_ATTRIBS];

    // bind buffers

    for (afxUnit fIdx = 0; fIdx < vtxd->srcCnt; fIdx++)
    {
        avxVertexFetch const* vtxf = &vtxd->srcs[fIdx];
        avxBufferedStream const* vtxs = &streams[vtxf->pin];
        AfxReadBuffer(vtxs->buf, vtxs->offset, vtxs->range, vtxs->stride, data);

        srcBuf[vtxf->pin] = vtxs->buf;
        srcPtr[vtxf->pin] = &(AFX_CAST(afxByte*, vtxs->buf->storage[0].hostedAlloc.addr)[vtxs->offset]);
        srcStride[vtxf->pin] = vtxs->stride;
    }

    // describe attributes

    for (afxUnit aIdx = 0; aIdx < vtxd->totalAttrCnt; aIdx++)
    {
        avxVertexAttr const* vtxa = &vtxd->attrs[aIdx];
        AvxDescribeFormats(1, &vtxa->fmt, &attrFd[aIdx]);
    }

    for (afxUnit instIdx = 0; instIdx < instCnt; instIdx++)
    {
        for (afxUnit vtxIdx = 0; vtxIdx < vtxCnt; vtxIdx++)
        {
            // step per-vertex stream fetch

            for (afxUnit fIdx = 0; fIdx < vtxd->srcCnt; fIdx++)
            {
                avxVertexFetch const* vs = &vtxd->srcs[fIdx];

                for (afxUnit aIdx = 0; aIdx < vtxd->totalAttrCnt; aIdx++)
                {
                    avxVertexAttr const* va = &vtxd->attrs[aIdx];
                    AfxCopy(data, &srcPtr[fIdx][va->offset], attrFd[aIdx].stride);
                    data += attrFd[aIdx].stride;
                }

                DoVertex(vtxIdx, data);

                if (!vs->instRate)
                    srcPtr[vs->pin] += srcStride[vs->pin];
            }
        }

        // step per-instance stream fetch

        for (afxUnit fIdx = 0; fIdx < vtxd->srcCnt; fIdx++)
        {
            avxVertexFetch const* vtxf = &vtxd->srcs[fIdx];

            if (vtxf->instRate)
                srcPtr[vtxf->pin] += srcStride[vtxf->pin];
        }
    }
}



