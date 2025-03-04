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
#define _AVX_VERTEX_DECL_C
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
    return vin->srcCnt;
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
            AvxDescribeFormats(1, &via->fmt, &pfd);
            siz += pfd.stride;// AfxVertexFormatGetSize(via->fmt);
        }
    }
    return siz;
}

_AVX afxError _AvxVtxdDtorCb(avxVertexDecl vin)
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
            .cnt = vin->srcCnt,
            .siz = sizeof(vin->srcs[0]),
            .var = (void**)&vin->srcs
        }
    };

    if (AfxDeallocateInstanceData(vin, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_AVX afxError _AvxVtxdCtorCb(avxVertexDecl vin, void** args, afxUnit invokeNo)
{
    (void)invokeNo;
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxVertexLayout const* layout = AFX_CAST(avxVertexLayout const*, args[1]) + invokeNo;
    AFX_ASSERT(args[1]);

#ifdef _AFX_DEBUG
    if (layout->attrCnt)
    {
        AFX_ASSERT(layout->attrs);

        for (afxUnit i = 0; i < layout->attrCnt; i++)
        {
            for (afxUnit j = 0; j < i; j++)
            {
                AFX_ASSERT(layout->attrs[i].location != layout->attrs[j].location);

                if (layout->attrs[i].location == layout->attrs[j].location)
                    AfxThrowError();
            }
        }
    }

    if (layout->srcCnt)
    {
        AFX_ASSERT(layout->srcs);

        for (afxUnit i = 0; i < layout->srcCnt; i++)
        {
            for (afxUnit j = 0; j < i; j++)
            {
                AFX_ASSERT(layout->srcs[i].srcIdx != layout->srcs[j].srcIdx);

                if (layout->srcs[i].srcIdx == layout->srcs[j].srcIdx)
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
            .cnt = layout->attrCnt,
            .siz = sizeof(vin->attrs[0]),
            .var = (void**)&vin->attrs
        },
        {
            .cnt = layout->srcCnt,
            .siz = sizeof(vin->srcs[0]),
            .var = (void**)&vin->srcs
        }
    };

    if (AfxAllocateInstanceData(vin, ARRAY_SIZE(stashes), stashes))
    {
        AfxThrowError();
        return err;
    }

    for (afxUnit i = 0; i < layout->attrCnt; i++)
    {
        avxVertexInput* attr = &vin->attrs[i];
        attr->fmt = layout->attrs[i].fmt;
        attr->location = layout->attrs[i].location;
        attr->offset = layout->attrs[i].offset;
        attr->srcIdx = layout->attrs[i].srcIdx;
        //attr->normalized = !!layout->attrs[i].normalized;
    }

    for (afxUnit i = 0; i < layout->srcCnt; i++)
    {
        avxVertexFetch* src = &vin->srcs[i];
        src->srcIdx = layout->srcs[i].srcIdx;
        //src->stride = layout->srcs[i].stride;
        src->instanceRate = layout->srcs[i].instanceRate;
    }

    vin->attrCnt = layout->attrCnt;
    vin->srcCnt = layout->srcCnt;

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
    .ctor = (void*)_AvxVtxdCtorCb,
    .dtor = (void*)_AvxVtxdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxDeclareVertexLayouts(afxDrawSystem dsys, afxUnit cnt, avxVertexLayout const layouts[], avxVertexDecl declarations[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    AFX_ASSERT(declarations);
    AFX_ASSERT(layouts);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)_AvxDsysGetImpl(dsys)->vtxdCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_VIN);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)declarations, (void const*[]) { dsys, layouts }))
    {
        AfxThrowError();
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_VIN, cnt, declarations);        
    }
    return err;
}
