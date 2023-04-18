#include "sgl.h"
#include "afx/draw/res/afxVertexBuffer.h"
#include "afx/draw/afxDrawSystem.h"
#include "../src/e2coree/draw/afxDrawParadigms.h"

typedef struct
{
    _afxUbufCtorArgs buf;
    afxNat vtxCnt;
    afxNat rowCnt;
    afxVertexDataSpecification const *specs;
} _afxVbufCtorArgs;

_SGL afxError _AfxVertexBufferForEachVertex(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void(*f)(void const *vtx, void*data), void *data)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(vbuf->vtxCnt > baseVtx);
    AfxAssert(vbuf->vtxCnt >= baseVtx + vtxCnt);
    AfxAssert(vtxCnt);

    afxNat stride = vbuf->row[rowIdx].stride;
    afxByte const *bytemap = AfxBufferGetData(&vbuf->buf, vbuf->row[rowIdx].offset + (baseVtx * stride));

    for (afxNat i = 0; i < vtxCnt; i++)
        f(&(bytemap[i * stride]), data);

    return err;
}

_SGL afxError _AfxVertexBufferDump(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void *dst, afxNat dstVtxStride)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(baseVtx + vtxCnt <= vbuf->vtxCnt);
    AfxAssert(vtxCnt);
    AfxAssert(dst);
    AfxAssert(!dstVtxStride || dstVtxStride >= vbuf->row[rowIdx].stride);

    if (AfxBufferDump2(&vbuf->buf, vbuf->row[rowIdx].offset, vbuf->row[rowIdx].stride, vtxCnt, dst, dstVtxStride))
        AfxThrowError();

    return err;
}

_SGL afxError _AfxVertexBufferUpdate(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void const *src, afxVertexFormat srcFmt)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(baseVtx + vtxCnt <= vbuf->vtxCnt);
    AfxAssert(vtxCnt);
    AfxAssert(src);
    AfxAssert(srcFmt);

    if (AfxBufferUpdate2(&vbuf->buf, vbuf->row[rowIdx].offset, vbuf->row[rowIdx].stride, vtxCnt, src, AfxVertexFormatGetSize(srcFmt)))
        AfxThrowError();

    return err;
}

_SGL afxNat _AfxVertexBufferGetStride(afxVertexBuffer vbuf, afxNat rowIdx)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    return vbuf->row[rowIdx].stride;
}

_SGL afxNat _AfxVertexBufferGetRange(afxVertexBuffer vbuf, afxNat baseRow, afxNat rowCnt)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > baseRow);
    AfxAssert(vbuf->rowCnt >= rowCnt);
    afxNat range = 0;

    for (afxNat i = 0; i < rowCnt; i++)
        range += AFX_ALIGN(vbuf->row[baseRow + i].stride * vbuf->vtxCnt, AFX_SIMD_ALIGN); // ALIGNMENT WHERE?

    return range;
}

_SGL afxNat _AfxVertexBufferGetOffset(afxVertexBuffer vbuf, afxNat vtxIdx, afxNat rowIdx)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(vbuf->vtxCnt > vtxIdx);
    return vbuf->row[rowIdx].offset + (vbuf->row[rowIdx].stride * vtxIdx);
}

_SGL void const* _AfxVertexBufferGetData(afxVertexBuffer vbuf, afxNat rowIdx, afxNat vtxIdx)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    AfxAssert(vbuf->vtxCnt > vtxIdx);
    return AfxBufferGetData(&vbuf->buf, vbuf->row[rowIdx].offset + (vtxIdx * vbuf->row[rowIdx].stride));
}

_SGL afxVertexFormat _AfxVertexBufferGetFormat(afxVertexBuffer vbuf, afxNat rowIdx)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    return vbuf->row[rowIdx].fmt;
}

_SGL afxString const* _AfxVertexBufferGetSemantic(afxVertexBuffer vbuf, afxNat rowIdx)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssert(vbuf->rowCnt > rowIdx);
    return &(vbuf->row[rowIdx].semantic.str);
}

_SGL afxNat _AfxVertexBufferGetRowCount(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    return vbuf->rowCnt;
}

_SGL afxNat _AfxVertexBufferFindArrange(afxVertexBuffer vbuf, afxString const *name)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxAssertString(name);

    for (afxNat i = 0; i < vbuf->rowCnt; i++)
    {
        if (0 == AfxStringCompareCi(name, &(vbuf->row[i].semantic.str)))
            return i;
    }

    return AFX_INVALID_INDEX;
}

_SGL afxNat _AfxVertexBufferGetCount(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    return vbuf->vtxCnt;
}

_SGL afxNat _AfxVertexBufferGetPerVertexSize(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);

    afxNat siz = 0;

    for (afxNat i = 0; i < vbuf->rowCnt; i++)
        siz += vbuf->row[i].stride;

    return siz;
}

_SGL afxError _AfxVertexBufferOptimize(afxVertexBuffer vbuf, afxNat rowIdx, afxBool favorSpeedOverSize)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    (void)rowIdx;
    (void)favorSpeedOverSize;
    AfxError("Not implemented yet");
    return err;
}

_SGL afxVertexBuffer _AfxDrawContextAcquireVertexBuffer(afxDrawContext dctx, afxNat vtxCnt, afxNat rowCnt, afxVertexDataSpecification const *specs)
{
    afxError err = NIL;
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    AfxAssert(vtxCnt);
    AfxAssert(rowCnt);
    AfxAssert(specs);

    afxNat totalBytemapSiz = 0;

    for (afxNat i = 0; i < rowCnt; i++)
    {
        afxVertexFormat fmt = specs[i].fmt ? specs[i].fmt : specs[i].srcFmt;
        AfxAssert(AFX_VTX_FMT_TOTAL > fmt);

        if (err)
            break;

        totalBytemapSiz += AFX_ALIGN(AfxVertexFormatGetSize(fmt) * vtxCnt, AFX_SIMD_ALIGN);
    }

    AfxAssert(totalBytemapSiz);

    afxBufferSpecification bufSpec =
    {
        totalBytemapSiz,
        AFX_BUF_USAGE_VERTICES,
        NIL
    };

    _afxVbufCtorArgs args =
    {
        .buf = { dctx, &bufSpec },
        vtxCnt,
        rowCnt,
        specs
    };

    afxVertexBuffer vbuf = AfxObjectAcquire(AfxDrawContextGetVertexBufferClass(dctx), &args, AfxSpawnHint());
    AfxTryAssertObject(vbuf, AFX_FCC_VBUF);

    return vbuf;
}

_SGL afxError _AfxVbufDtor(afxVertexBuffer vbuf)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxEntry("vbuf=%p", vbuf);

    AfxAssert(vbuf->row);
    AfxDeallocate(NIL, vbuf->row);

    return err;
}

_SGL afxVbufImpl _AfxStdVbufImpl;

afxVbufImpl _AfxStdVbufImpl =
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
    _AfxVertexBufferDump,
    _AfxVertexBufferFindArrange,
    _AfxVertexBufferGetRowCount,
    _AfxVertexBufferGetData,
    _AfxVertexBufferGetFormat,
    _AfxVertexBufferGetSemantic,
    _AfxVertexBufferGetRange,
    _AfxVertexBufferGetOffset,
    _AfxVertexBufferGetStride,
    _AfxVertexBufferGetCount,
    _AfxVertexBufferGetPerVertexSize,
    _AfxVertexBufferForEachVertex,
    _AfxVertexBufferOptimize,
    _AfxVertexBufferUpdate,
};

_SGL afxError _AfxVbufCtor(afxVertexBuffer vbuf, _afxVbufCtorArgs *args)
{
    afxError err = NIL;
    AfxAssertObject(vbuf, AFX_FCC_VBUF);
    AfxEntry("vbuf=%p", vbuf);
    AfxAssert(args);
    afxVertexDataSpecification const *specs = args->specs;
    AfxAssert(specs);

    AfxAssert(args->vtxCnt);
    vbuf->vtxCnt = args->vtxCnt;

    AfxAssert(args->rowCnt);
    //vbuf->rowCnt = spec->rowCnt;

    if (!(vbuf->row = AfxAllocate(NIL, args->rowCnt * sizeof(vbuf->row[0]), AfxSpawnHint()))) AfxThrowError();
    else
    {
        afxVertexFormat fmt;
        afxNat nextArrOffset = 0;
        vbuf->rowCnt = 0;

        for (afxNat i = 0; i < args->rowCnt; i++)
        {
            afxVertexDataRow *row = &vbuf->row[i];
            afxString const *semantic = specs[i].semantic;
            AfxAssert(semantic);
            AfxString8(&row->semantic, semantic);

            row->usage = specs[i].usage;

            fmt = specs[i].fmt ? specs[i].fmt : specs[i].srcFmt;
            AfxAssert(AFX_VTX_FMT_TOTAL > fmt);

            row->stride = AfxVertexFormatGetSize((row->fmt = fmt));
            AfxAssert(row->stride);
            row->offset = nextArrOffset;
            nextArrOffset += AFX_ALIGN(row->stride * vbuf->vtxCnt, AFX_SIMD_ALIGN);

            vbuf->rowCnt++;

            if (specs[i].src)
            {
                AfxAssert(AFX_VTX_FMT_TOTAL > specs[i].srcFmt);

                if (AfxVertexBufferUpdate(vbuf, i, 0, vbuf->vtxCnt, specs[i].src, specs[i].srcFmt))
                    AfxThrowError();
            }
        }

        if (err)
        {
            AfxAssert(vbuf->row);
            AfxDeallocate(NIL, vbuf->row);
        }
    }

    return err;
}

_SGL afxClassSpecification const _AfxVbufClassSpec;

afxClassSpecification const _AfxVbufClassSpec =
{
    AFX_FCC_VBUF,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxVertexBuffer)),
    NIL,
    (void*)_AfxVbufCtor,
    (void*)_AfxVbufDtor,
    "Vertex Buffer",
    &_AfxStdVbufImpl
};
