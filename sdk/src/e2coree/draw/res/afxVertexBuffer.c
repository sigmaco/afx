/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_CONTEXT_C
#define _AFX_BUFFER_C
#define _AFX_INDEX_BUFFER_C
#define _AFX_VERTEX_BUFFER_C
#include "../_classified/afxDrawClassified.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afx/draw/afxDrawSystem.h"
#include "../afxDrawParadigms.h"

typedef struct
{
    afxBufferSpecification buf;
    afxVertexBufferBlueprint const *spec;
} _afxVbufCtorArgs;

_AFXINL void AfxVertexBufferBlueprintErase(afxVertexBufferBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->cap = 0;
    blueprint->rowCnt = 0;
}

_AFXINL void AfxVertexBufferBlueprintDiscard(afxVertexBufferBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->fcc = NIL;
}

_AFXINL void AfxVertexBufferBlueprintReset(afxVertexBufferBlueprint *blueprint, afxNat cap)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->fcc = AFX_FCC_VBUB;
    blueprint->cap = cap;
    blueprint->rowCnt = 0;
}

_AFXINL afxError AfxVertexBufferBlueprintAddRow(afxVertexBufferBlueprint *blueprint, afxString const *semantic, afxVertexFormat fmt, afxVertexUsage usage, void const *src, afxVertexFormat srcFmt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_VBUB);
    AfxString8(&blueprint->row[blueprint->rowCnt].semantic);
    AfxCopyString(&blueprint->row[blueprint->rowCnt].semantic.str, semantic);
    blueprint->row[blueprint->rowCnt].fmt = fmt;
    blueprint->row[blueprint->rowCnt].usage = usage;
    blueprint->row[blueprint->rowCnt].src = src;
    blueprint->row[blueprint->rowCnt].srcFmt = srcFmt;
    ++blueprint->rowCnt;
    return err;
}

_AFXINL afxError AfxVertexBufferBlueprintResetRow(afxVertexBufferBlueprint *blueprint, afxNat idx, afxString const *semantic, afxVertexFormat fmt, afxVertexUsage usage, void const *src, afxVertexFormat srcFmt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_VBUB);
    AfxAssert(blueprint->rowCnt > idx);
    AfxString8(&blueprint->row[idx].semantic);
    AfxCopyString(&blueprint->row[idx].semantic.str, semantic);
    blueprint->row[idx].fmt = fmt;
    blueprint->row[idx].usage = usage;
    blueprint->row[idx].src = src;
    blueprint->row[idx].srcFmt = srcFmt;
    return err;
}

////////////////////////////////////////////////////////////////////////////////

#if 0
_AFX afxNat AfxGetVertexStreamCount(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    return vbuf->streamCnt;
}

_AFX afxNat AfxGetVertexAttrCount(afxVertexBuffer vbuf, afxNat streamIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->streamCnt, streamIdx, 1);
    return vbuf->streams[streamIdx].attrCnt;
}

_AFX afxNat AfxGetVertexFormat(afxVertexBuffer vbuf, afxNat streamIdx, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->streamCnt, streamIdx, 1);
    AfxAssertRange(vbuf->streams[streamIdx].attrCnt, attrIdx, 1);
    return vbuf->streams[streamIdx].attrs[attrIdx].fmt;
}

_AFX afxNat AfxLocateVertexBufferRegion(afxVertexBuffer vbuf, afxNat streamIdx, afxNat attrIdx, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->streamCnt, streamIdx, 1);
    AfxAssertRange(vbuf->streams[streamIdx].attrCnt, attrIdx, 1);
    AfxAssertRange(vbuf->streams[streamIdx].attrs[attrIdx].cachedElemCnt, elemIdx, 1);
    //AfxAssertRange(vbuf->attrCnt, baseAttr, 1); // >= is a evil trick to use it to measure tex
    



    afxNat bpp = AfxGetBitsPerPixel(tex->fmt);
    afxNat bppTimesWhd;

    afxWhd offset2 = { offset[0], offset[1], offset[2] };
    afxWhd whd = { tex->whd[0], tex->whd[1], tex->whd[2] };
    afxNat memOff = 0;

    do
    {
        offset2[0] = offset2[0] >> lodIdx;
        offset2[1] = offset2[1] >> lodIdx;
        offset2[2] = offset2[2] >> lodIdx;
        whd[0] = whd[0] >> lodIdx;
        whd[1] = whd[1] >> lodIdx;
        whd[2] = whd[2] >> lodIdx;
        bppTimesWhd = (whd[0] * (whd[1] * (whd[2] * bpp)));
        memOff += (imgIdx * (bppTimesWhd / AFX_BYTE_SIZE));
    } while (lodIdx--);

    bppTimesWhd = (offset2[2] * (offset2[1] * (offset2[0] * bpp)));
    memOff += (bppTimesWhd / AFX_BYTE_SIZE);

    return memOff;
}

_AFX afxNat AfxMeasureVertexBufferRegion(afxVertexBuffer vbuf, afxVertexBufferRegion const *rgn) // can't be multiple because regions couldn't be continous.
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(rgn);
    AfxAssertRange(vbuf->streamCnt, rgn->streamIdx, 1);
    AfxAssertRange(vbuf->streams[rgn->streamIdx].attrCnt, rgn->baseAttr, rgn->attrCnt);
    AfxAssertRange(vbuf->streams[rgn->streamIdx].attrs[rgn->baseAttr].cachedElemCnt, rgn->baseElem, rgn->elemCnt);

    afxNat off = AfxLocateVertexBufferRegion(vbuf, rgn->streamIdx, rgn->attrCnt - 1, rgn->elemCnt);
    off -= AfxLocateVertexBufferRegion(vbuf, rgn->streamIdx, rgn->baseAttr, rgn->baseElem);
    return off;
}
#endif
////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxVertexBufferForEachVertex(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void(*f)(void const *vtx, void*data), void *data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(vbuf->cap > baseVtx);
    AfxAssert(vbuf->cap >= baseVtx + vtxCnt);
    AfxAssert(vtxCnt);

    afxNat stride = vbuf->row[rowIdx].stride;
    afxByte const *bytemap = AfxMapBufferRange(vbuf->buf, vbuf->row[rowIdx].offset + (baseVtx * stride), stride * vbuf->cap, AFX_BUF_MAP_R);

    for (afxNat i = 0; i < vtxCnt; i++)
        f(&(bytemap[i * stride]), data);

    AfxUnmapBufferRange(vbuf->buf);

    return err;
}

_AFX afxError AfxVertexBufferDump(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void *dst, afxNat dstVtxStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(baseVtx + vtxCnt <= vbuf->cap);
    AfxAssert(vtxCnt);
    AfxAssert(dst);
    AfxAssert(!dstVtxStride || dstVtxStride >= vbuf->row[rowIdx].stride);

    if (AfxDumpBuffer2(vbuf->buf, vbuf->row[rowIdx].offset + (vbuf->row[rowIdx].stride * baseVtx), vbuf->row[rowIdx].stride, vtxCnt, dst, dstVtxStride))
        AfxThrowError();

    return err;
}

_AFX afxError AfxVertexBufferUpdate(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void const *src, afxVertexFormat srcFmt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(baseVtx + vtxCnt <= vbuf->cap);
    AfxAssert(vtxCnt);
    AfxAssert(src);
    AfxAssert(srcFmt);

    if (AfxUpdateBuffer2(vbuf->buf, vbuf->row[rowIdx].offset + (vbuf->row[rowIdx].stride * baseVtx), vbuf->row[rowIdx].stride, vtxCnt, src, AfxVertexFormatGetSize(srcFmt)))
        AfxThrowError();

    return err;
}

_AFX afxNat AfxVertexBufferGetStride(afxVertexBuffer vbuf, afxNat rowIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    return vbuf->row[rowIdx].stride;
}

_AFX afxNat AfxVertexBufferGetRange(afxVertexBuffer vbuf, afxNat baseRow, afxNat rowCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > baseRow);
    AfxAssert(vbuf->rowCnt >= rowCnt);
    afxNat range = 0;

    for (afxNat i = 0; i < rowCnt; i++)
        range += AFX_ALIGN(vbuf->row[baseRow + i].stride * vbuf->cap, AFX_SIMD_ALIGN); // ALIGNMENT WHERE?

    return range;
}

_AFX afxNat AfxVertexBufferGetOffset(afxVertexBuffer vbuf, afxNat vtxIdx, afxNat rowIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(vbuf->cap > vtxIdx);
    return vbuf->row[rowIdx].offset + (vbuf->row[rowIdx].stride * vtxIdx);
}

#if 0
_AFX void const* AfxVertexBufferGetData(afxVertexBuffer vbuf, afxNat rowIdx, afxNat vtxIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(vbuf->cap > vtxIdx);
    return AfxBufferGetData(&vbuf->buf, vbuf->row[rowIdx].offset + (vtxIdx * vbuf->row[rowIdx].stride));
}
#endif

_AFX afxVertexFormat AfxVertexBufferGetFormat(afxVertexBuffer vbuf, afxNat rowIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    return vbuf->row[rowIdx].fmt;
}

_AFX afxString const* AfxVertexBufferGetSemantic(afxVertexBuffer vbuf, afxNat rowIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    return &vbuf->row[rowIdx].semantic;
}

_AFX afxError AfxVertexBufferDescribeRow(afxVertexBuffer vbuf, afxNat rowIdx, afxVertexRowSpecification *spec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(spec);
    spec->fmt = vbuf->row[rowIdx].fmt;
    spec->semantic = &vbuf->row[rowIdx].semantic;
    spec->src = NIL; //AfxBufferGetData(&vbuf->buf, vbuf->row[rowIdx].offset);
    spec->usage = vbuf->row[rowIdx].usage;
    return err;
}

_AFX afxNat AfxVertexBufferGetRowCount(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    return vbuf->rowCnt;
}

_AFX afxNat AfxVertexBufferFindArrange(afxVertexBuffer vbuf, afxString const *name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertString(name);

    for (afxNat i = 0; i < vbuf->rowCnt; i++)
    {
        if (0 == AfxCompareStringCi(name, &vbuf->row[i].semantic))
            return i;
    }

    return AFX_INVALID_INDEX;
}

_AFX afxNat AfxVertexBufferGetCap(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    return vbuf->cap;
}

_AFX afxNat AfxVertexBufferGetPerVertexSize(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    afxNat siz = 0;

    for (afxNat i = 0; i < vbuf->rowCnt; i++)
        siz += vbuf->row[i].stride;

    return siz;
}

_AFX afxError AfxVertexBufferOptimize(afxVertexBuffer vbuf, afxNat rowIdx, afxBool favorSpeedOverSize)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    (void)rowIdx;
    (void)favorSpeedOverSize;
    AfxError("Not implemented yet");
    return err;
}

_AFX afxError AfxBuildVertexBuffers(afxDrawContext dctx, afxNat cnt, afxVertexBuffer vbuf[], afxVertexBufferBlueprint const blueprint[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(blueprint);
    AfxAssert(vbuf);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    if (AfxAcquireObjects(&dctx->vertices, cnt, (afxHandle*)vbuf, (void*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
}

_AFX afxNat AfxEnumerateVertexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxVertexBuffer vbuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(vbuf);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    return AfxEnumerateInstances(&dctx->vertices, first, cnt, (afxHandle*)vbuf);
}
