#include "sgl.h"
#include "afx/draw/res/afxIndexBuffer.h"
#include "afx/draw/afxDrawSystem.h"
#include "../e2coree/draw/afxDrawParadigms.h"

typedef struct
{
    _afxUbufCtorArgs buf;
    afxIndexBufferSpecification const *spec;
} _afxIbufCtorArgs;

_SGL afxError _AfxIndexBufferForEachVertex(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt, afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, void(*f)(void const *vtx, void*data), void *data)
{
    afxError err = NIL;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->idxCnt > baseIdx);
    AfxAssert(ibuf->idxCnt >= baseIdx + idxCnt);
    AfxAssert(idxCnt);
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(vbuf->vtxCnt > baseVtx);

    afxByte const *vertices = AfxVertexBufferGetData(vbuf, rowIdx, baseVtx);
    afxByte const *indices = AfxIndexBufferGetData(ibuf, baseIdx);
    afxNat stride = AfxIndexBufferGetStride(ibuf);
    afxNat vtxStride = AfxVertexBufferGetStride(vbuf, rowIdx);

    for (afxNat i = 0; i < idxCnt; i++)
        f(&(vertices[indices[i * stride] * vtxStride]), data);

    return err;
}

_SGL afxError _AfxIndexBufferOptimize(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxError("Not implemented");
    return err;
}

_SGL afxNat _AfxIndexBufferGetStride(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    return ibuf->idxSiz;
}

_SGL afxNat _AfxIndexBufferGetCount(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    return ibuf->idxCnt;
}

_SGL void const* _AfxIndexBufferGetData(afxIndexBuffer ibuf, afxNat baseIdx)
{
    afxError err = NIL;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(baseIdx < ibuf->idxCnt);
    return AfxBufferGetData(&ibuf->buf, baseIdx * ibuf->idxSiz);
}

_SGL afxError _AfxIndexBufferDump(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt, void *dst, afxNat dstIdxStride)
{
    afxError err = NIL;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->idxCnt >= idxCnt);
    AfxAssert(ibuf->idxCnt >= baseIdx + idxCnt);
    AfxAssert(dst);
    afxNat idxSiz = ibuf->idxSiz;
    AfxAssert(!dstIdxStride || dstIdxStride >= idxSiz);

    if (AfxBufferDump2(&ibuf->buf, baseIdx * idxSiz, idxSiz, idxCnt, dst, dstIdxStride))
        AfxThrowError();

    return err;
}

_SGL afxError _AfxIndexBufferUpdate(afxIndexBuffer ibuf, afxNat baseIdx, afxNat idxCnt, void const *src, afxNat srcIdxStride)
{
    afxError err = NIL;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxAssert(ibuf->idxCnt >= idxCnt);
    AfxAssert(ibuf->idxCnt >= baseIdx + idxCnt);
    AfxAssert(src);
    afxNat idxSiz = ibuf->idxSiz;
    AfxAssert(!srcIdxStride || srcIdxStride >= idxSiz);

    if (AfxBufferUpdate2(&ibuf->buf, baseIdx * idxSiz, idxSiz, idxCnt, src, srcIdxStride))
        AfxThrowError();

    return err;
}

_SGL afxIndexBuffer _AfxDrawContextAcquireIndexBuffer(afxDrawContext dctx, afxIndexBufferSpecification const *spec)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxEntry("dctx=%p,idxCnt=%u,src=%p,stride=%p", dctx, spec->cnt, spec->src, spec->stride);

    afxBufferSpecification bufSpec =
    {
        spec->cnt * spec->siz,
        AFX_BUF_USAGE_INDICES,
        NIL
    };
    _afxIbufCtorArgs args =
    {
        .buf = { dctx, &bufSpec },
        spec
    };

    afxIndexBuffer ibuf = AfxObjectAcquire(AfxDrawContextGetIndexBufferClass(dctx), &args, AfxSpawnHint());
    AfxAssertObject(ibuf, AFX_FCC_IBUF);

    return ibuf;
}

_SGL afxError _AfxIbufDtor(afxIndexBuffer ibuf)
{
    afxError err = NIL;
    AfxEntry("ibuf=%p", ibuf);
    AfxAssertObject(ibuf, AFX_FCC_IBUF);

    return err;
}

_SGL afxIbufImpl const _AfxStdIbufImpl;
afxIbufImpl const _AfxStdIbufImpl =
{
    {
        _AfxBufferGetData,
        _AfxBufferGetSize,
        _AfxBufferDump,
        _AfxBufferDump2,
        _AfxBufferUpdate,
        _AfxBufferUpdate2,
        NIL,
        NIL
    },
    _AfxIndexBufferGetData,
    _AfxIndexBufferGetCount,
    _AfxIndexBufferGetStride,
    _AfxIndexBufferOptimize,
    _AfxIndexBufferDump,
    _AfxIndexBufferUpdate,
    _AfxIndexBufferForEachVertex
};

_SGL afxError _AfxIbufCtor(afxIndexBuffer ibuf, _afxIbufCtorArgs *args)
{
    afxError err = NIL;
    AfxAssertObject(ibuf, AFX_FCC_IBUF);
    AfxEntry("ibuf=%p", ibuf);
    afxIndexBufferSpecification const *spec = args->spec;
    AfxAssert(spec);

    AfxAssert(spec->cnt);
    ibuf->idxCnt = spec->cnt;
    AfxAssert(spec->siz);
    ibuf->idxSiz = spec->siz; // internally set related to sizeof each index. Actually it is always sizeof(afxVertexIndex).

    if (spec->src)
        if (AfxBufferUpdate2(&ibuf->buf, ibuf->idxSiz, 0, ibuf->idxCnt, spec->src, spec->stride))
            AfxThrowError();

    return err;
}

_SGL afxClassSpecification const _AfxIbufClassSpec;

afxClassSpecification const _AfxIbufClassSpec =
{
    AFX_FCC_IBUF,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxIndexBuffer)),
    NIL,
    (void*)_AfxIbufCtor,
    (void*)_AfxIbufDtor,
    "Index Buffer",
    &_AfxStdIbufImpl
};
