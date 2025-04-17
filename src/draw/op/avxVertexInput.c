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

_AVX void AvxDescribeVertexLayout(avxVertexInput vin, avxVertexLayout* layout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    AFX_ASSERT(layout);

    layout->tag = vin->tag;
    layout->srcCnt = vin->srcCnt;
    layout->attrCnt = vin->attrCnt;

    for (afxUnit i = 0; i < vin->attrCnt; i++)
    {
        avxVertexAttr const* via = &vin->attrs[i];
        layout->attrs[i].fmt = via->fmt;
        layout->attrs[i].location = via->location;
        layout->attrs[i].offset = via->offset;
        layout->attrs[i].srcIdx = via->srcIdx;
    }

    for (afxUnit i = 0; i < vin->srcCnt; i++)
    {
        avxVertexFetch const* vis = &vin->srcs[i];
        layout->srcs[i].instanceRate = vis->instanceRate;
        layout->srcs[i].srcIdx = vis->srcIdx;
    }
}

_AVX afxUnit AvxMeasureVertexInputStream(avxVertexInput vin, afxUnit srcIdx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    afxUnit siz = 0;

    for (afxUnit i = 0; i < vin->attrCnt; i++)
    {
        avxVertexAttr const* via = &vin->attrs[i];

        if (via->srcIdx == srcIdx)
        {
            avxFormatDescription pfd;
            AvxDescribeFormats(1, &via->fmt, &pfd);
            siz += pfd.stride;// AfxVertexFormatGetSize(via->fmt);
        }
    }
    return siz;
}

_AVX afxError _AvxVtxdDtorCb(avxVertexInput vin)
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

_AVX afxError _AvxVtxdCtorCb(avxVertexInput vin, void** args, afxUnit invokeNo)
{
    (void)invokeNo;
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);

    afxDrawSystem dsys = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    avxVertexLayout const* layout = AFX_CAST(avxVertexLayout const*, args[1]) + invokeNo;
    AFX_ASSERT(args[1]);

    vin->tag = layout->tag;

#ifdef AVX_VALIDATION_ENABLED
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
        avxVertexAttr* attr = &vin->attrs[i];
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
    .desc = "Pipelined Vertex Input Declaration",
    .fixedSiz = sizeof(AFX_OBJECT(avxVertexInput)),
    .ctor = (void*)_AvxVtxdCtorCb,
    .dtor = (void*)_AvxVtxdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxDeclareVertexInputs(afxDrawSystem dsys, afxUnit cnt, avxVertexLayout const layouts[], avxVertexInput declarations[])
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
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_VIN, cnt, declarations);

#if AVX_VALIDATION_ENABLED
    for (afxUnit i = 0; i < cnt; i++)
    {
        avxVertexInput vtxd = declarations[i];

        AFX_ASSERT(vtxd->attrCnt == layouts[i].attrCnt);
        AFX_ASSERT(vtxd->srcCnt == layouts[i].srcCnt);

        for (afxUnit j = 0; j < vtxd->attrCnt; j++)
        {
            AFX_ASSERT(vtxd->attrs[j].fmt == layouts[i].attrs[j].fmt);
            AFX_ASSERT(vtxd->attrs[j].location == layouts[i].attrs[j].location);
            AFX_ASSERT(vtxd->attrs[j].offset == layouts[i].attrs[j].offset);
            AFX_ASSERT(vtxd->attrs[j].srcIdx == layouts[i].attrs[j].srcIdx);
        }

        for (afxUnit j = 0; j < vtxd->srcCnt; j++)
        {
            AFX_ASSERT(vtxd->srcs[j].instanceRate == layouts[i].srcs[j].instanceRate);
            AFX_ASSERT(vtxd->srcs[j].srcIdx == layouts[i].srcs[j].srcIdx);
        }
    }
#endif
    return err;
}
