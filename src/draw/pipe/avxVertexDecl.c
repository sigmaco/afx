/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_VERTEX_INPUT_C
#include "../impl/avxImplementation.h"

_AVX afxUnit AfxCountVertexInputAttributes(avxVertexDecl vin)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    return vin->attrCnt;
}

_AVX afxUnit AfxCountVertexInputStreams(avxVertexDecl vin)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    return vin->streamCnt;
}

_AVX afxUnit AfxMeasureVertexInputStream(avxVertexDecl vin, afxUnit srcIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    afxUnit siz = 0;

    for (afxUnit i = 0; i < vin->attrCnt; i++)
    {
        avxVertexInput const* via = &vin->attrs[i];

        if (via->srcIdx == srcIdx)
        {
            avxFormatDescription pfd;
            AfxDescribePixelFormat(via->fmt, &pfd);
            siz += pfd.stride;// AfxVertexFormatGetSize(via->fmt);
        }
    }
    return siz;
}

_AVX afxError _AvxVinStdDtor(avxVertexDecl vin)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    //afxDrawSystem dsys = AfxGetProvider(vin);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = vin->attrCnt,
            .siz = sizeof(vin->attrs[0]),
            .var = (void**)&vin->attrs
        },
        {
            .cnt = vin->streamCnt,
            .siz = sizeof(vin->streams[0]),
            .var = (void**)&vin->streams
        }
    };

    if (AfxDeallocateInstanceData(vin, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_AVX afxError _AvxVinStdCtor(avxVertexDecl vin, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);

    afxDrawSystem dsys = args[0];
    afxUnit streamCnt = *(afxUnit const*)args[1];
    avxVertexFetch const* streams = args[2];
    afxUnit attrCnt = *(afxUnit const*)args[3];
    avxVertexInput const* attrs = args[4];
    
#ifdef _AFX_DEBUG
    if (attrCnt)
    {
        AFX_ASSERT(attrs);

        for (afxUnit i = 0; i < attrCnt; i++)
        {
            for (afxUnit j = 0; j < i; j++)
            {
                AFX_ASSERT(attrs[i].location != attrs[j].location);

                if (attrs[i].location == attrs[j].location)
                    AfxThrowError();
            }
        }
    }

    if (streamCnt)
    {
        AFX_ASSERT(streams);

        for (afxUnit i = 0; i < streamCnt; i++)
        {
            for (afxUnit j = 0; j < i; j++)
            {
                AFX_ASSERT(streams[i].srcIdx != streams[j].srcIdx);

                if (streams[i].srcIdx == streams[j].srcIdx)
                    AfxThrowError();
            }
        }
    }
#endif

    if (err)
        return err;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = attrCnt,
            .siz = sizeof(vin->attrs[0]),
            .var = (void**)&vin->attrs
        },
        {
            .cnt = streamCnt,
            .siz = sizeof(vin->streams[0]),
            .var = (void**)&vin->streams
        }
    };

    if (AfxAllocateInstanceData(vin, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < attrCnt; i++)
    {
        avxVertexInput* attr = &vin->attrs[i];
        attr->fmt = attrs[i].fmt;
        attr->location = attrs[i].location;
        attr->offset = attrs[i].offset;
        attr->srcIdx = attrs[i].srcIdx;
        //attr->normalized = !!attrs[i].normalized;
    }

    for (afxUnit i = 0; i < streamCnt; i++)
    {
        avxVertexFetch* stream = &vin->streams[i];
        stream->srcIdx = streams[i].srcIdx;
        //stream->stride = streams[i].stride;
        stream->instanceRate = streams[i].instanceRate;
    }

    vin->attrCnt = attrCnt;
    vin->streamCnt = streamCnt;

    if (err && AfxAllocateInstanceData(vin, ARRAY_SIZE(stashes), stashes))
        AfxThrowError();

    return err;
}

_AVX afxClassConfig const _AVX_VIN_CLASS_CONFIG =
{
    .fcc = afxFcc_VIN,
    .name = "VertexInput",
    .desc = "Vertex Input Declaration",
    .fixedSiz = sizeof(AFX_OBJECT(avxVertexDecl)),
    .ctor = (void*)_AvxVinStdCtor,
    .dtor = (void*)_AvxVinStdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxDeclareVertexLayout(afxDrawSystem dsys, afxUnit streamCnt, avxVertexFetch const streams[], afxUnit attrCnt, avxVertexInput const attrs[], avxVertexDecl* decl)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(streamCnt);
    AFX_ASSERT(streams);
    AFX_ASSERT(attrCnt);
    AFX_ASSERT(attrs);

    avxVertexDecl vsup = NIL;

    afxClass* cls = AvxGetVertexInputClass(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_VIN);

    if (AfxAcquireObjects(cls, 1, (afxObject*)&vsup, (void const*[]) { dsys, &streamCnt, streams, &attrCnt, attrs }))
        AfxThrowError();

    AFX_ASSERT(decl);
    *decl = vsup;
    return err;
}
