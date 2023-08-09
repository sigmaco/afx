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
            if (AfxBuildIndexBuffers(blueprint->dctx, 1, blueprint, &ibuf[i]))
            {
                AfxThrowError();

                for (afxNat j = 0; j < i; j++)
                {
                    AfxReleaseObject(&ibuf[j]->buf.obj);
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

_AFX void AfxIndexBufferDescribeRegion(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat32 *baseOffset, afxNat *idxCnt, afxNat *idxSiz)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(rgnIdx < ibuf->regionCnt);
    
    AfxAssert(baseOffset);
    *baseOffset = ibuf->regions[rgnIdx].baseOffset;

    AfxAssert(idxCnt);
    *idxCnt = ibuf->regions[rgnIdx].idxCnt;

    AfxAssert(idxSiz);
    *idxSiz = ibuf->regions[rgnIdx].idxSiz;
}

_AFX afxError AfxIndexBufferForEachVertex(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, void(*f)(void const *vtx, void*data), void *data)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt >= idxCnt);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt >= baseIdx + idxCnt);
    AfxAssert(idxCnt);
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(vbuf->cap > baseVtx);

    afxNat stride = AfxIndexBufferGetStride(ibuf, rgnIdx);
    afxNat vtxStride = AfxVertexBufferGetStride(vbuf, rowIdx);
    afxByte const *vertices = AfxMapBufferRange(&vbuf->buf, AfxVertexBufferGetOffset(vbuf, baseVtx, rowIdx), vtxStride * vbuf->cap, AFX_BUF_MAP_R);
    afxByte const *indices = AfxMapBufferRange(&ibuf->buf, AfxIndexBufferGetOffset(ibuf, rgnIdx, baseIdx), stride * idxCnt, AFX_BUF_MAP_R);
    
    for (afxNat i = 0; i < idxCnt; i++)
        f(&(vertices[indices[i * stride] * vtxStride]), data);

    AfxUnmapBufferRange(&vbuf->buf);
    AfxUnmapBufferRange(&ibuf->buf);

    return err;
}

_AFX afxError AfxIndexBufferOptimize(afxIndexBuffer ibuf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxError("Not implemented");
    return err;
}

_AFX afxNat AfxIndexBufferGetStride(afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    return ibuf->regions[rgnIdx].idxSiz;
}

_AFX afxNat AfxIndexBufferGetCap(afxIndexBuffer ibuf, afxNat rgnIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    return ibuf->regions[rgnIdx].idxCnt;
}

_AFX afxNat AfxIndexBufferGetOffset(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt > baseIdx);
    return ibuf->regions[rgnIdx].baseOffset + (baseIdx * ibuf->regions[rgnIdx].idxSiz);
}

#if 0
_AFX void const* AfxIndexBufferGetData(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt > baseIdx);
    return AfxBufferGetData(&ibuf->buf, ibuf->regions[rgnIdx].baseOffset + (baseIdx * ibuf->regions[rgnIdx].idxSiz));
}
#endif

_AFX afxError AfxIndexBufferDump(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, void *dst, afxNat dstIdxStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->regionCnt > rgnIdx);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt >= idxCnt);
    AfxAssert(ibuf->regions[rgnIdx].idxCnt >= baseIdx + idxCnt);
    AfxAssert(dst);
    afxNat idxSiz = ibuf->regions[rgnIdx].idxSiz;
    AfxAssert(!dstIdxStride || dstIdxStride >= idxSiz);

    if (AfxDumpBuffer2(&ibuf->buf, ibuf->regions[rgnIdx].baseOffset + (baseIdx * idxSiz), idxSiz, idxCnt, dst, dstIdxStride))
        AfxThrowError();

    return err;
}

_AFX afxError AfxIndexBufferUpdate(afxIndexBuffer ibuf, afxNat rgnIdx, afxNat baseIdx, afxNat idxCnt, void const *src, afxNat srcIdxStride)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
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

    if (AfxUpdateBuffer2(&ibuf->buf, baseOffset + (baseIdx * idxSiz), idxSiz, idxCnt, src, srcIdxStride))
        AfxThrowError();

    return err;
}

_AFX afxError AfxBuildIndexBuffers(afxDrawContext dctx, afxNat cnt, afxIndexBufferBlueprint const blueprint[], afxIndexBuffer ibuf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(blueprint);
    AfxAssert(ibuf);
    AfxEntry("cnt=%u,stride=%p", cnt, blueprint);
    
    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat totalSiz = 0;

        for (afxNat j = 0; j < AfxGetArrayPop(&blueprint->regions); j++)
        {
            afxIndexBufferBlueprintRegion* rgn = AfxGetArrayUnit(&blueprint->regions, j);
            AfxAssert(rgn->idxCnt);
            afxNat idxSiz = rgn->idxCnt < AFX_N8_MAX ? sizeof(afxNat8) : (rgn->idxCnt < AFX_N16_MAX ? sizeof(afxNat16) : sizeof(afxNat32));
            
            totalSiz += idxSiz * rgn->idxCnt;
        }

        _afxIbufCtorArgs args =
        {
            .buf = {
                totalSiz,
                AFX_BUF_USAGE_INDEX,
                NIL
            },
            &blueprint[i]
        };


        if (AfxClassAcquireObjects(AfxGetIndexBufferClass(dctx), NIL, 1, &args, (afxInstance**)&ibuf[i], AfxSpawnHint()))
        {
            AfxThrowError();

            for (afxNat j = 0; j < i; j++)
            {
                AfxReleaseObject(&ibuf[j]->buf.obj);
                ibuf[j] = NIL;
            }
            break;
        }
        else
        {
            AfxAssertObject(ibuf[i], AFX_FCC_IBUF);
        }
    }
    return err;
}

_AFX afxBool _AfxIbufEventHandler(afxInstance *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxIndexBuffer ibuf = (void*)obj;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    (void)ev;
    return FALSE;
}

_AFX afxBool _AfxIbufEventFilter(afxInstance *obj, afxInstance *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxIndexBuffer ibuf = (void*)obj;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    (void)watched;
    (void)ev;
    return FALSE;
}

_AFX afxError _AfxIbufDtor(afxIndexBuffer ibuf)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("ibuf=%p", ibuf);
    AfxAssertObject(ibuf, AFX_FCC_IBUF);

    afxContext mem = AfxGetDrawMemory();

    AfxDeallocate(mem, ibuf->regions);

    return err;
}

_AFX afxError _AfxIbufCtor(void *cache, afxNat idx, afxIndexBuffer ibuf, _afxIbufCtorArgs *args)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    (void)cache;
    AfxEntry("ibuf=%p", ibuf);
    afxIndexBufferBlueprint const *blueprint = args[idx].blueprint;
    AfxAssert(blueprint);

    afxContext mem = AfxGetDrawMemory();

    ibuf->regionCnt = 0;

    if (!(ibuf->regions = AfxAllocate(mem, sizeof(ibuf->regions[0]) * AfxGetArrayPop(&blueprint->regions), 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        afxNat32 baseOffset = 0;

        for (afxNat j = 0; j < AfxGetArrayPop(&blueprint->regions); j++)
        {
            afxIndexBufferBlueprintRegion* rgn = AfxGetArrayUnit(&blueprint->regions, j);
            AfxAssert(rgn->idxCnt);
            afxNat idxSiz = rgn->idxCnt < AFX_N8_MAX ? sizeof(afxNat8) : (rgn->idxCnt < AFX_N16_MAX ? sizeof(afxNat16) : sizeof(afxNat32));

            afxNat byteSiz = idxSiz * rgn->idxCnt;

            ibuf->regions[ibuf->regionCnt].baseOffset = baseOffset;
            ibuf->regions[ibuf->regionCnt].idxCnt = rgn->idxCnt;
            ibuf->regions[ibuf->regionCnt].idxSiz = idxSiz;

            baseOffset += byteSiz;
            AfxAssert(ibuf->regionCnt == j);
            ++ibuf->regionCnt;

            if (rgn->src)
            {
                AfxAssert(rgn->srcStride);

                if (AfxIndexBufferUpdate(ibuf, j, 0, rgn->idxCnt, rgn->src, rgn->srcStride))
                    AfxThrowError();
            }
        }
    }
    return err;
}

_AFX afxClassSpecification const _AfxIbufClassSpec;

afxClassSpecification const _AfxIbufClassSpec =
{
    AFX_FCC_IBUF,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxIndexBuffer)),
    NIL,
    (void*)_AfxIbufCtor,
    (void*)_AfxIbufDtor,
    .event = _AfxIbufEventHandler,
    .eventFilter = _AfxIbufEventFilter,
    "afxIndexBuffer",
    NIL
};
