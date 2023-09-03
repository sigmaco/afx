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
#include "../src/e2coree/draw/afxDrawParadigms.h"
#include "afx/draw/res/afxVertexBuffer.h"

typedef struct
{
    afxBufferSpecification buf;
    afxIndexBufferBlueprint const *blueprint;
} _afxIbufCtorArgs;

_AFXINL void AfxIndexBufferBlueprintEnd(afxIndexBufferBlueprint *blueprint, afxNat cnt, afxIndexBuffer ibuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);

    if (cnt)
    {
        AfxAssert(ibuf);

        for (afxNat i = 0; i < cnt; i++)
        {
            if (AfxBuildIndexBuffers(blueprint->dctx, 1, &ibuf[i], blueprint))
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; j++)
                {
                    AfxReleaseObjects(1, (void*[]) { ibuf[j] });
                    ibuf[j] = NIL;
                }
                break;
            }
        }
    }

    AfxReleaseArray(&blueprint->regions);
}

_AFXINL void AfxIndexBufferBlueprintErase(afxIndexBufferBlueprint *blueprint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxEmptyArray(&blueprint->regions);
}

_AFXINL void AfxIndexBufferBlueprintBegin(afxIndexBufferBlueprint *blueprint, afxDrawContext dctx, afxNat estRgnCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    blueprint->dctx = dctx;
    AfxAcquireArray(&blueprint->regions, sizeof(afxIndexBufferBlueprintRegion), estRgnCnt, AfxSpawnHint());
}

_AFXINL afxError AfxIndexBufferBlueprintPushRegion(afxIndexBufferBlueprint *blueprint, afxNat32 idxCnt, void const *src, afxNat srcStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(blueprint);
    AfxAssert(idxCnt);
    AfxAssert(!src || (src && srcStride));
    
    afxIndexBufferBlueprintRegion *rgn;
    
    afxNat idx;

    if (!(rgn = AfxInsertArrayUnit(&blueprint->regions, &idx))) AfxThrowError();
    else
    {
        rgn->idxCnt = idxCnt;
        rgn->src = src;
        rgn->srcStride = srcStride;
    }

    return err;
}

_AFX void AfxIndexBufferDescribeRegion(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat32* baseOffset, afxNat32* idxCnt, afxNat32* idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, AFX_FCC_IBUF);
    AfxAssert(rgnIdx < ibuf->regionCnt);
    
    if (baseOffset)
        *baseOffset = ibuf->regions[rgnIdx].baseOffset;

    if (idxCnt)
        *idxCnt = ibuf->regions[rgnIdx].idxCnt;

    if (idxSiz)
        *idxSiz = ibuf->regions[rgnIdx].idxSiz;
}

_AFX afxError AfxIndexBufferForEachVertex(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, void(*f)(void const *vtx, void*data), void *data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, AFX_FCC_IBUF);
    AfxAssertRange(ibuf->regionCnt, rgnIdx, 1);
    AfxAssertRange(ibuf->regions[rgnIdx].idxCnt, baseIdx, idxCnt);
    
    AfxAssertObjects(1, &vbuf, AFX_FCC_VBUF);

    AfxAssertRange(vbuf->attrCnt, attrIdx, 1);
    AfxAssertRange(vbuf->cap, baseVtx, 1);

    afxNat secBase = vbuf->sections[vbuf->attrs[attrIdx].secIdx].base;
    afxNat secRange = vbuf->sections[vbuf->attrs[attrIdx].secIdx].range;
    afxNat secStride = vbuf->sections[vbuf->attrs[attrIdx].secIdx].stride;
    afxNat attrOffset = vbuf->attrs[attrIdx].offset;
    afxNat attrSiz = AfxVertexFormatGetSize(vbuf->attrs[attrIdx].fmt);

    AfxAssertRange(secRange, secStride * baseVtx, secStride * 1);
    afxByte* vertices = AfxMapBufferRange(vbuf->buf, secBase + (secStride * baseVtx), secRange - (secBase + (secStride * baseVtx)), AFX_BUF_MAP_R);

    afxNat stride = AfxIndexBufferGetStride(ibuf, rgnIdx);
    afxByte const *indices = AfxMapBufferRange(ibuf->buf, AfxIndexBufferGetOffset(ibuf, rgnIdx, baseIdx), stride * idxCnt, AFX_BUF_MAP_R);
    
    for (afxNat i = 0; i < idxCnt; i++)
        f(&(vertices[indices[i * stride] * secStride]) + attrOffset, data);

    AfxUnmapBufferRange(vbuf->buf);
    AfxUnmapBufferRange(ibuf->buf);

    return err;
}

_AFX afxError AfxIndexBufferOptimize(afxIndexBuffer ibuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, AFX_FCC_IBUF);
    AfxError("Not implemented");
    return err;
}

_AFX afxNat AfxIndexBufferGetStride(afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    return ibuf->regions[rgnIdx].idxSiz;
}

_AFX afxNat AfxIndexBufferGetCap(afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    return ibuf->regions[rgnIdx].idxCnt;
}

_AFX afxNat AfxIndexBufferGetOffset(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt > baseIdx);
    return ibuf->regions[rgnIdx].baseOffset + (baseIdx * ibuf->regions[rgnIdx].idxSiz);
}

#if 0
_AFX void const* AfxIndexBufferGetData(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt > baseIdx);
    return AfxBufferGetData(&ibuf->buf, ibuf->regions[rgnIdx].baseOffset + (baseIdx * ibuf->regions[rgnIdx].idxSiz));
}
#endif

_AFX afxError AfxIndexBufferDump(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, void *dst, afxNat dstIdxStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt >= idxCnt);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt >= baseIdx + idxCnt);
    AfxAssert(dst);
    afxNat idxSiz = ibuf->regions[rgnIdx].idxSiz;
    AfxAssert(!dstIdxStride || dstIdxStride >= idxSiz);

    if (AfxDumpBuffer2(ibuf->buf, ibuf->regions[rgnIdx].baseOffset + (baseIdx * idxSiz), idxSiz, idxCnt, dst, dstIdxStride))
        AfxThrowError();

    return err;
}

_AFX afxError AfxIndexBufferUpdate(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, void const *src, afxNat srcIdxStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt >= idxCnt);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt >= baseIdx + idxCnt);
    AfxAssert(src);
    afxNat idxSiz = ibuf->regions[rgnIdx].idxSiz;
    AfxAssert(!srcIdxStride || srcIdxStride >= idxSiz);

    afxNat32 baseOffset = 0;
    afxNat idxCntGot = 0, idxSizGot = 0;
    AfxIndexBufferDescribeRegion(ibuf, rgnIdx, &baseOffset, &idxCntGot, &idxSizGot);
    AfxAssert(idxSizGot == idxSiz);
    AfxAssert(idxCntGot >= idxCnt);

    if (AfxUpdateBuffer2(ibuf->buf, baseOffset + (baseIdx * idxSiz), idxSiz, idxCnt, src, srcIdxStride))
        AfxThrowError();

    return err;
}

_AFX afxError AfxBuildIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBuffer ibuf[], afxIndexBufferBlueprint const blueprint[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(ibuf);
    AfxAssert(blueprint);
    AfxEntry("cnt=%u,stride=%p", cnt, blueprint);

    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);

    if (AfxAcquireObjects(&dctx->indices, cnt, (afxHandle*)ibuf, (void*[]) { (void*)blueprint }))
        AfxThrowError();

    return err;
}

_AFX afxNat AfxEnumerateIndexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxIndexBuffer ibuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(ibuf);
    AfxAssertObjects(1, &dctx, AFX_FCC_DCTX);
    return AfxEnumerateInstances(&dctx->indices, first, cnt, (afxHandle*)ibuf);
}
