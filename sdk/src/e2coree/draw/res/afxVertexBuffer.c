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
    blueprint->attrCnt = 0;
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
    blueprint->attrCnt = 0;
}

_AFXINL afxError AfxVertexBufferBlueprintAddRow(afxVertexBufferBlueprint *blueprint, afxString const *semantic, afxVertexFormat fmt, afxVertexUsage usage, void const *src, afxVertexFormat srcFmt, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_VBUB);
    AfxString8(&blueprint->spec[blueprint->attrCnt].semantic);
    AfxCopyString(&blueprint->spec[blueprint->attrCnt].semantic.str, semantic);
    blueprint->spec[blueprint->attrCnt].fmt = fmt;
    blueprint->spec[blueprint->attrCnt].usage = usage;
    blueprint->spec[blueprint->attrCnt].src = src;
    blueprint->spec[blueprint->attrCnt].srcFmt = srcFmt;
    blueprint->spec[blueprint->attrCnt].srcStride = srcStride;
    ++blueprint->attrCnt;
    return err;
}

_AFXINL afxError AfxVertexBufferBlueprintResetRow(afxVertexBufferBlueprint *blueprint, afxNat idx, afxString const *semantic, afxVertexFormat fmt, afxVertexUsage usage, void const *src, afxVertexFormat srcFmt, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_VBUB);
    AfxAssert(blueprint->attrCnt > idx);
    AfxString8(&blueprint->spec[idx].semantic);
    AfxCopyString(&blueprint->spec[idx].semantic.str, semantic);
    blueprint->spec[idx].fmt = fmt;
    blueprint->spec[idx].usage = usage;
    blueprint->spec[idx].src = src;
    blueprint->spec[idx].srcFmt = srcFmt;
    blueprint->spec[idx].srcStride = srcStride;
    return err;
}

_AFXINL afxError AfxVertexBufferBlueprintAddAttributes(afxVertexBufferBlueprint *blueprint, afxNat cnt, afxVertexSpec const spec[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(blueprint, AFX_FCC_VBUB);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxString8(&blueprint->spec[blueprint->attrCnt].semantic);
        AfxCopyString(&blueprint->spec[blueprint->attrCnt].semantic.str, spec[i].semantic);
        blueprint->spec[blueprint->attrCnt].secIdx = spec[i].secIdx;
        blueprint->spec[blueprint->attrCnt].fmt = spec[i].fmt;
        blueprint->spec[blueprint->attrCnt].usage = spec[i].usage;
        blueprint->spec[blueprint->attrCnt].src = spec[i].src;
        blueprint->spec[blueprint->attrCnt].srcFmt = spec[i].srcFmt;
        blueprint->spec[blueprint->attrCnt].srcStride = spec[i].srcStride;
        ++blueprint->attrCnt;
    }
    return err;
}

////////////////////////////////////////////////////////////////////////////////

#if 0
_AFX afxNat AfxCountVertexSections(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    return vbuf->streamCnt;
}

_AFX afxNat AfxCountVertexAttributes(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    return vbuf->attrCnt;
}

_AFX afxNat AfxGetVertexFormat(afxVertexBuffer vbuf, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    return vbuf->attrs[attrIdx].fmt;
}

_AFX afxNat AfxLocateVertexBufferRegion(afxVertexBuffer vbuf, afxNat streamIdx, afxNat attrIdx, afxNat elemIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->streamCnt, streamIdx, 1);
    AfxAssertRange(vbuf->streams[streamIdx].attrCnt, attrIdx, 1);
    AfxAssertRange(vbuf->streams[streamIdx].attrs[attrIdx].cachedElemCnt, elemIdx, 1);
    //AfxAssertRange(vbuf->attrCnt, baseAttr, 1); // >= is a evil trick to use it to measure tex
    


    afxPixelLayout pfd;
    AfxDescribePixelFormat(tex->fmt, &pfd);
    afxNat pixelSiz = AFX_ALIGN(pfd.bpp, 32) / AFX_BYTE_SIZE;

    afxWhd offset2 = { offset[0], offset[1], offset[2] };
    afxWhd whd = { tex->whd[0], tex->whd[1], tex->whd[2] };
    afxNat memOff = 0;

    while (lodIdx)
    {
        offset2[0] = offset2[0] >> lodIdx;
        offset2[1] = offset2[1] >> lodIdx;
        offset2[2] = offset2[2] >> lodIdx;
        whd[0] = whd[0] >> lodIdx;
        whd[1] = whd[1] >> lodIdx;
        whd[2] = whd[2] >> lodIdx;

        memOff += (tex->imgCnt * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
        --lodIdx;
    };

    memOff += (imgIdx * (whd[2] * (whd[1] * (whd[0] * pixelSiz))));
    memOff += (offset2[2] * (offset2[1] * (offset2[0] * pixelSiz)));
    memOff = AFX_ALIGN(memOff, AFX_SIMD_ALIGN);

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

    afxNat size = 0;
    afxNat streamIdx = rgn->streamIdx;

    for (afxNat i = 0; i < rgn->attrCnt; i++)
    {
        afxNat attrIdx = rgn->baseAttr + i;
        afxVertexFormat fmt = AfxGetVertexFormat(vbuf, attrIdx);
        afxNat fmtSiz = AfxVertexFormatGetSize(fmt);
        afxNat elemCnt = vbuf->attrs[attrIdx].cnt;
        afxNat totalAttrLen = vbuf->attrs[attrIdx].totalAttrLen;

        if (rgn->baseAttr != i)
            size += fmtSiz * elemCnt * vbuf->cap;
        else
            size += fmtSiz * rgn->elemCnt * rgn->vtxCnt;
    }
    return size;
}

_AFX afxError AfxUpdateVertexBufferRegion(afxVertexBuffer vbuf, afxVertexBufferRegion const *rgn, void const *src, afxVertexFormat fmt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(rgn);
    AfxAssert(src);
    AfxAssert(fmt);


}

#endif

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxVertexBufferForEachVertex(afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void(*f)(void const *vtx, void*data), void *data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->cap, baseVtx, vtxCnt);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);

    afxNat secBase = vbuf->sections[vbuf->attrs[attrIdx].secIdx].base;
    afxNat secRange = vbuf->sections[vbuf->attrs[attrIdx].secIdx].range;
    afxNat secStride = vbuf->sections[vbuf->attrs[attrIdx].secIdx].stride;
    afxNat attrOffset = vbuf->attrs[attrIdx].offset;
    afxNat attrSiz = AfxVertexFormatGetSize(vbuf->attrs[attrIdx].fmt);

    AfxAssertRange(secRange, secStride * baseVtx, secStride * vtxCnt);
    afxByte* src = AfxMapBufferRange(vbuf->buf, secBase + (secStride * baseVtx), secStride * vtxCnt, AFX_BUF_MAP_R);

    for (afxNat i = 0; i < vtxCnt; i++)
        f(&src[i * attrSiz] + attrOffset, data);

    AfxUnmapBufferRange(vbuf->buf);

    return err;
}

_AFX afxError AfxVertexBufferDump(afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void *dst, afxNat dstStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->cap, baseVtx, vtxCnt);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    AfxAssert(dst);

    afxNat secBase = vbuf->sections[vbuf->attrs[attrIdx].secIdx].base;
    afxNat secRange = vbuf->sections[vbuf->attrs[attrIdx].secIdx].range;
    afxNat secStride = vbuf->sections[vbuf->attrs[attrIdx].secIdx].stride;
    afxNat attrOffset = vbuf->attrs[attrIdx].offset;
    afxNat attrSiz = AfxVertexFormatGetSize(vbuf->attrs[attrIdx].fmt);

    AfxAssertRange(secRange, secStride * baseVtx, secStride * vtxCnt);
    afxByte* src = AfxMapBufferRange(vbuf->buf, secBase + (secStride * baseVtx), secStride * vtxCnt, AFX_BUF_MAP_R);
    afxByte* dst2 = dst;

    if (!dstStride)
        dstStride = /*AfxVertexFormatGetSize(dstFmt)*/attrSiz;

    for (afxNat i = 0; i < vtxCnt; i++)
    {
        AfxCopy(&dst2[i * dstStride], &src[i * attrSiz] + attrOffset, attrSiz);
    }

    AfxUnmapBufferRange(vbuf->buf);

    return err;
}

_AFX afxError AfxUpdateVertexBuffer(afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const *src, afxVertexFormat srcFmt, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->cap, baseVtx, vtxCnt);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    AfxAssert(src);
    AfxAssert(srcFmt);

    afxNat secBase = vbuf->sections[vbuf->attrs[attrIdx].secIdx].base;
    afxNat secRange = vbuf->sections[vbuf->attrs[attrIdx].secIdx].range;
    afxNat secStride = vbuf->sections[vbuf->attrs[attrIdx].secIdx].stride;
    afxNat attrOffset = vbuf->attrs[attrIdx].offset;
    afxNat attrSiz = AfxVertexFormatGetSize(vbuf->attrs[attrIdx].fmt);

    AfxAssertRange(secRange, secStride * baseVtx, secStride * vtxCnt);
    afxByte* dst = AfxMapBufferRange(vbuf->buf, secBase + (secStride * baseVtx), secStride * vtxCnt, AFX_BUF_MAP_W);
    afxByte const* src2 = src;

    if (!srcStride)
        srcStride = AfxVertexFormatGetSize(srcFmt);

    for (afxNat i = 0; i < vtxCnt; i++)
    {
        AfxCopy(&dst[i * attrSiz] + attrOffset, &src2[i * srcStride], attrSiz);
    }

    AfxUnmapBufferRange(vbuf->buf);

    return err;
}

_AFX afxNat AfxDescribeVertexSection(afxVertexBuffer vbuf, afxNat secIdx, afxNat32* base, afxNat32* range, afxNat32* stride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->secCnt, secIdx, 1);
    
    if (base)
        *base = vbuf->sections[secIdx].base;

    if (range)
        *range = vbuf->sections[secIdx].range;

    if (stride)
        *stride = vbuf->sections[secIdx].stride;
}

#if 0
_AFX void const* AfxVertexBufferGetData(afxVertexBuffer vbuf, afxNat attrIdx, afxNat vtxIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->attrCnt > attrIdx);
    AfxAssert(vbuf->cap > vtxIdx);
    return AfxBufferGetData(&vbuf->buf, vbuf->attr[attrIdx].offset + (vtxIdx * vbuf->attr[attrIdx].stride));
}
#endif

_AFX afxNat AfxGetVertexAttributeSection(afxVertexBuffer vbuf, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    return vbuf->attrs[attrIdx].secIdx;
}

_AFX afxVertexFormat AfxGetVertexAttributeFormat(afxVertexBuffer vbuf, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    return vbuf->attrs[attrIdx].fmt;
}

_AFX afxNat AfxGetVertexAttributeSize(afxVertexBuffer vbuf, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    return AfxVertexFormatGetSize(vbuf->attrs[attrIdx].fmt);
}

_AFX afxNat AfxGetVertexAttributeOffset(afxVertexBuffer vbuf, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    return vbuf->attrs[attrIdx].offset;
}

_AFX afxVertexUsage AfxGetVertexAttributeUsage(afxVertexBuffer vbuf, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    return vbuf->attrs[attrIdx].usage;
}

_AFX afxString const* AfxGetVertexAttributeName(afxVertexBuffer vbuf, afxNat attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    return &vbuf->attrs[attrIdx].semantic;
}

_AFX afxError AfxDescribeVertexAttribute(afxVertexBuffer vbuf, afxNat attrIdx, afxNat* secIdx, afxVertexFormat* fmt, afxNat32* offset, afxVertexUsage* usage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);

    if (secIdx)
        *secIdx = vbuf->attrs[attrIdx].secIdx;

    if (fmt)
        *fmt = vbuf->attrs[attrIdx].fmt;

    if (offset)
        *offset = vbuf->attrs[attrIdx].offset;

    if (usage)
        *usage = vbuf->attrs[attrIdx].usage;

    return err;
}

_AFX afxNat AfxCountVertexAttributes(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    return vbuf->attrCnt;
}

_AFX afxBool AfxFindVertexAttribute(afxVertexBuffer vbuf, afxString const *name, afxNat* attrIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    AfxAssertString(name);

    for (afxNat i = 0; i < vbuf->attrCnt; i++)
    {
        if (0 == AfxCompareStringCi(name, &vbuf->attrs[i].semantic))
        {
            if (attrIdx)
                *attrIdx = i;

            return TRUE;
        }
    }

    return FALSE;
}

_AFX afxNat AfxGetVertexCapacity(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    return vbuf->cap;
}

_AFX afxNat AfxGetVertexSize(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    afxNat siz = 0;

    for (afxNat i = 0; i < vbuf->attrCnt; i++)
        siz += AfxVertexFormatGetSize(vbuf->attrs[i].fmt);

    return siz;
}

_AFX afxError AfxOptimizeVertexBuffer(afxVertexBuffer vbuf, afxNat attrIdx, afxBool favorSpeedOverSize)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);
    (void)attrIdx;
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
