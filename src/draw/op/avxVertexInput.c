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
#include "../ddi/avxImplementation.h"

_AVX afxUnit AvxDescribeVertexLayout(avxVertexInput vin, avxVertexLayout* layout)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    AFX_ASSERT(layout);

    layout->tag = vin->tag;
    layout->srcCnt = vin->srcCnt;
    //layout->attrCnt = vin->attrCnt;

    for (afxUnit i = 0; i < vin->totalAttrCnt; i++)
    {
        avxVertexAttr const* va = &vin->attrs[i];
        layout->attrs[i].fmt = va->fmt;
        layout->attrs[i].location = va->location;
        layout->attrs[i].offset = va->offset;
        //layout->attrs[i].pin = va->pin;
    }

    for (afxUnit i = 0; i < vin->srcCnt; i++)
    {
        avxVertexFetch const* vs = &vin->srcs[i];
        layout->srcs[i].pin = vs->pin;
        //layout->srcs[i].stride = vis->stride;
        layout->srcs[i].instRate = vs->instRate;
        layout->srcs[i].baseAttrIdx = vs->baseAttrIdx;
        layout->srcs[i].attrCnt = vs->attrCnt;
    }
    return vin->totalAttrCnt;
}

/*
    The AvxQueryVertexStride() function computes the total size in bytes of all vertex attributes per source (stream), 
    starting from @baseSrcIdx, and sums their sizes into a single value (totalSiz) returned. The idea is to find out the aggregate 
    stride across multiple vertex buffers.
*/

_AVX afxUnit AvxQueryVertexStride(avxVertexInput vin, afxUnit baseSrcIdx, afxUnit cnt, afxUnit vtxSrcSiz[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);

    baseSrcIdx = AFX_MIN(baseSrcIdx, vin->srcCnt - 1);
    cnt = AFX_MIN(cnt, vin->srcCnt - baseSrcIdx);

    afxUnit totalSiz = 0;
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit pin = baseSrcIdx + i;
        avxVertexFetch const* vf = &vin->srcs[pin];
        afxUnit siz = 0;

        for (afxUnit j = 0; j < vf->attrCnt; j++)
        {
            afxUnit attrIdx = vf->baseAttrIdx + j;
            avxVertexAttr const* via = &vin->attrs[attrIdx];

            avxFormatDescription pfd;
            AvxDescribeFormats(1, &via->fmt, &pfd);
            siz += pfd.stride;
        }

        if (vtxSrcSiz) vtxSrcSiz[i] = siz;

        totalSiz += siz;
    }
    return totalSiz;
}

_AVX afxError _AvxVtxdDtorCb(avxVertexInput vin)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);
    //afxDrawSystem dsys = AfxGetProvider(vin);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = vin->totalAttrCnt,
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
    vin->udd = layout->udd;
    vin->flags = layout->flags;

    afxBool attrUsed[AVX_MAX_VERTEX_ATTRIBS] = { FALSE };
    afxUnit highestUsedAttrIdx = 0;

    for (afxUnit i = 0; i < layout->srcCnt; ++i)
    {
        avxVertexFetch const* fetch = &layout->srcs[i];

        // Skip unused or empty bindings
        if (fetch->attrCnt == 0)
            continue;

        // Validate attr bounds
        afxUnit start = fetch->baseAttrIdx;
        afxUnit end = start + fetch->attrCnt;

        if (start >= AVX_MAX_VERTEX_ATTRIBS || end > AVX_MAX_VERTEX_ATTRIBS)
        {
            // Out of bounds
            AfxThrowError();
            return err;
        }

        // Check for ownership overlaps
        for (afxUnit j = start; j < end; ++j)
        {
            if (attrUsed[j])
            {
                // Attribute already owned by another stream
                AfxThrowError();
                return err;
            }
            attrUsed[j] = TRUE;

            // Track highest used attr index
            if (j + 1 > highestUsedAttrIdx)
                highestUsedAttrIdx = j + 1;
        }

        // Optionally clamp or clean zeroed-out attribute entries if desired
        // Or adjust attrCnt if some trailing entries are clearly invalid
        // This depends on whether you want to sanitize silently or return an error
    }

    afxUnit totalAttrCnt = 0;
    totalAttrCnt = highestUsedAttrIdx;
#if 0
#ifdef AVX_VALIDATION_ENABLED

    if (layout->srcCnt)
    {
        AFX_ASSERT(layout->srcs);

        for (afxUnit i = 0; i < layout->srcCnt; i++)
        {
            avxVertexFetch const* vs = &layout->srcs[i];

            for (afxUnit j = 0; j < i; j++)
            {
                avxVertexFetch const* vs2 = &layout->srcs[j];
                AFX_ASSERT(vs->pin != vs2->pin);

                if (vs->pin == vs2->pin)
                    AfxThrowError();

                for (afxUnit k = 0; k < vs->attrCnt; k++)
                {
                    afxUnit attrIdx = vs->baseAttrIdx + k;
                    avxVertexAttr const* va = &layout->attrs[attrIdx];

                    for (afxUnit l = 0; l < k; l++)
                    {
                        afxUnit prevAttrIdx = vs2->baseAttrIdx + l;
                        avxVertexAttr const* va2 = &layout->attrs[prevAttrIdx];
                        AFX_ASSERT(va->location != va2->location);

                        if (va->location == va2->location)
                            AfxThrowError();
                        else
                            ++totalAttrCnt;
                    }
                }
            }
        }
    }

    if (totalAttrCnt)
    {
        AFX_ASSERT(layout->attrs);

        for (afxUnit i = 0; i < totalAttrCnt; i++)
        {
            avxVertexAttr const* va = &layout->attrs[i];

            for (afxUnit j = 0; j < i; j++)
            {
                avxVertexAttr const* va2 = &layout->attrs[j];
                AFX_ASSERT(va->location != va2->location);

                if (va->location == va2->location)
                    AfxThrowError();
            }
        }
    }
#endif
#endif

    if (err)
        return err;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = totalAttrCnt,
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


    for (afxUnit i = 0; i < totalAttrCnt; i++)
    {
        avxVertexAttr* va = &vin->attrs[i];
        va->fmt = layout->attrs[i].fmt;
        va->location = layout->attrs[i].location;
        va->offset = layout->attrs[i].offset;
        //va->pin = layout->attrs[i].pin;
        //va->normalized = !!layout->attrs[i].normalized;
    }

    for (afxUnit i = 0; i < layout->srcCnt; i++)
    {
        avxVertexFetch* vs = &vin->srcs[i];
        vs->pin = layout->srcs[i].pin;
        //vs->stride = layout->srcs[i].stride;
        vs->instRate = layout->srcs[i].instRate;
        vs->baseAttrIdx = layout->srcs[i].baseAttrIdx;
        vs->attrCnt = layout->srcs[i].attrCnt;
    }

    vin->totalAttrCnt = totalAttrCnt;
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

        //AFX_ASSERT(vtxd->totalAttrCnt == layouts[i].totalAttrCnt);
        AFX_ASSERT(vtxd->srcCnt == layouts[i].srcCnt);

        for (afxUnit j = 0; j < vtxd->totalAttrCnt; j++)
        {
            AFX_ASSERT(vtxd->attrs[j].fmt == layouts[i].attrs[j].fmt);
            AFX_ASSERT(vtxd->attrs[j].location == layouts[i].attrs[j].location);
            AFX_ASSERT(vtxd->attrs[j].offset == layouts[i].attrs[j].offset);
            //AFX_ASSERT(vtxd->attrs[j].pin == layouts[i].attrs[j].pin);
        }

        for (afxUnit j = 0; j < vtxd->srcCnt; j++)
        {
            AFX_ASSERT(vtxd->srcs[j].pin == layouts[i].srcs[j].pin);
            AFX_ASSERT(vtxd->srcs[j].instRate == layouts[i].srcs[j].instRate);
            //AFX_ASSERT(vtxd->srcs[j].stride == layouts[i].srcs[j].stride);
            AFX_ASSERT(vtxd->srcs[j].baseAttrIdx == layouts[i].srcs[j].baseAttrIdx);
            AFX_ASSERT(vtxd->srcs[j].attrCnt == layouts[i].srcs[j].attrCnt);
        }
    }
#endif
    return err;
}
