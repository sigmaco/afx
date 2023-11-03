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


#include "sgl.h"
#include "afx/draw/afxBuffer.h"
#include "afx/draw/afxDrawSystem.h"
#include "../e2coree/draw/afxDrawParadigms.h"
#include "afx/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// DRAW BUFFER                                                                //
////////////////////////////////////////////////////////////////////////////////

#if 0
_SGL void _AfxStdUbufImplUnmap(afxBuffer buf)
{
    if (!buf->base.mapped) AfxError("");
    else
    {
        afxBuffer base = AfxBuffer.GetBase(buf);

        buf->base.mapped = FALSE;

        if (base->gpuHandle)
        {
            glVmt const* gl = &(((afxDrawSystem)AfxGetObjectProvider(&buf->base.obj))->vmt);
            afxFlags usage = base->usage;
            GLenum target = NIL;

            if (usage & afxBufferUsage_VERTEX)
                target = GL_ARRAY_BUFFER;
            else if (usage & afxBufferUsage_INDEX)
                target = GL_ELEMENT_ARRAY_BUFFER;
            else if (usage & afxBufferUsage_UNIFORM)
                target = GL_UNIFORM_BUFFER;
            else if (usage & afxBufferUsage_STORAGE)
                target = GL_SHADER_STORAGE_BUFFER;
            else if (usage & afxBufferUsage_INDIRECT)
                target = GL_DRAW_INDIRECT_BUFFER;
            else if (usage & afxBufferUsage_UNIFORM_TEXELS)
                target = GL_PIXEL_UNPACK_BUFFER;
            else if (usage & afxBufferUsage_STORAGE_TEXELS)
                target = GL_PIXEL_PACK_BUFFER;
            else if (usage == afxBufferUsage_SRC)
                target = GL_COPY_READ_BUFFER;
            else if (usage == afxBufferUsage_DST)
                target = GL_COPY_WRITE_BUFFER;
            else AfxError("");

            GLint glaccess;

            if (usage & (afxBufferUsage_SRC | afxBufferUsage_DST))
            {
                // copy
                glaccess = GL_READ_WRITE;
            }
            else if (usage & afxBufferUsage_SRC)
            {
                glaccess = GL_READ_ONLY;
            }
            else if (usage & afxBufferUsage_DST)
            {
                glaccess = GL_WRITE_ONLY;
            }
            else
            {
                glaccess = NIL;
                AfxError("");
            }

            AfxAssert(gl->IsBuffer(base->gpuHandle));

            gl->BindBuffer(target, base->gpuHandle); _SglThrowErrorOccuried();
            gl->BufferSubData(target, buf->base.mappedOff, buf->base.mappedRange, &(((afxByte*)buf->base.buf)[buf->base.mappedOff])); _SglThrowErrorOccuried();
            gl->BindBuffer(target, 0); _SglThrowErrorOccuried();

            base->lastUpdTime = AfxGetTimer();
            buf->lastUpdTime = AfxGetTimer();
        }
    }
}

_SGL void* _AfxStdUbufImplMap(afxBuffer buf, afxSize off, afxSize siz)
{
    void *ptr = NIL;

    if (buf->base.mapped) AfxError("");
    else
    {
        buf->base.mappedOff = off;
        buf->base.mappedRange = siz ? siz : buf->base.siz - off;
        buf->base.mapped = TRUE;
    }
    return ptr;
}
#endif//0

_SGL afxError _SglDpuBindAndSyncBuf(sglDpuIdd* dpu, afxNat unit, afxBuffer buf, afxNat offset, afxNat rangeOrVtxStride, GLenum target, GLenum usage, glVmt const* gl)
{
    //AfxEntry("buf=%p", buf);
    afxError err = AFX_ERR_NONE;
    (void)dpu;
    (void)unit;
    (void)offset;
    (void)rangeOrVtxStride;

    if (buf)
    {
        if ((buf->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((buf->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (buf->glHandle)
                {
                    gl->DeleteBuffers(1, &(buf->glHandle)); _SglThrowErrorOccuried();
                    buf->glHandle = NIL;
                }

                AfxAssert(NIL == buf->glHandle);

                buf->glTarget = target;
                buf->glUsage = usage;
                gl->GenBuffers(1, &(buf->glHandle)); _SglThrowErrorOccuried();
                gl->BindBuffer(buf->glTarget, buf->glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsBuffer(buf->glHandle));

                gl->BufferData(buf->glTarget, buf->base.siz, buf->base.bytemap, buf->glUsage); _SglThrowErrorOccuried();
                buf->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("afxBuffer %p hardware-side data instanced.", buf);
            }
            else if ((buf->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(buf->glHandle);
                AfxAssert(buf->glTarget == target);
                AfxAssert(buf->glUsage == usage);

                gl->BindBuffer(buf->glTarget, buf->glHandle); _SglThrowErrorOccuried();
                gl->BufferSubData(buf->glTarget, buf->lastUpdOffset, buf->lastUpdRange, &(buf->base.bytemap[buf->lastUpdOffset])); _SglThrowErrorOccuried();
                buf->updFlags &= ~(SGL_UPD_FLAG_DEVICE_FLUSH);
            }

            gl->BindBuffer(buf->glTarget, 0); _SglThrowErrorOccuried();
            buf->lastUpdOffset = 0;
            buf->lastUpdRange = 0;
        }
#if 0
        {
            AfxAssert(buf->glHandle);
            AfxAssert(buf->glTarget);

            switch (target)
            {
            case GL_ARRAY_BUFFER:
            {
                AfxAssertObjects(1, &buf, afxFcc_BUF);
                //AfxAssert(gl->BindVertexBuffer);

                if (gl->BindVertexBuffer)
                {
                    gl->BindVertexBuffer(unit, buf->glHandle, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
                }
#if !0
                else
                {
                    gl->BindBuffer(target, buf->glHandle); _SglThrowErrorOccuried();
                    AfxAssert(offset == 0);
                    AfxAssert(rangeOrVtxStride == 0);
                    AfxAssert(unit == 0);
                }
#endif
                break;
            }
            case GL_ELEMENT_ARRAY_BUFFER:
            {
                AfxAssertObjects(1, &buf, afxFcc_BUF);

                gl->BindBuffer(target, buf->glHandle); _SglThrowErrorOccuried();
                break;
            }
            default:
            {
                AfxAssertObjects(1, &buf, afxFcc_BUF);

                if (rangeOrVtxStride)
                {
                    gl->BindBufferRange(target, unit, buf->glHandle, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
                }
                else
                {
                    gl->BindBufferBase(target, unit, buf->glHandle); _SglThrowErrorOccuried();
                }
                break;
            }
            }
        }
#endif
    }
#if 0
    else
    {
        switch (target)
        {
        case GL_ARRAY_BUFFER:
        {
            //AfxAssert(gl->BindVertexBuffer);

            if (gl->BindVertexBuffer)
            {
                gl->BindVertexBuffer(unit, 0, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
            }
#if !0
            else
            {
                gl->BindBuffer(target, 0); _SglThrowErrorOccuried();
                AfxAssert(offset == 0);
                AfxAssert(rangeOrVtxStride == 0);
                AfxAssert(unit == 0);
            }
#endif
            break;
        }
        case GL_ELEMENT_ARRAY_BUFFER:
        {
            gl->BindBuffer(target, 0); _SglThrowErrorOccuried();
            AfxAssert(offset == 0);
            AfxAssert(rangeOrVtxStride == 0);
            AfxAssert(unit == 0);
            break;
        }
        default:
        {
            if (rangeOrVtxStride)
            {
                gl->BindBufferRange(target, unit, 0, offset, rangeOrVtxStride); _SglThrowErrorOccuried();
            }
            else
            {
                gl->BindBufferBase(target, unit, 0); _SglThrowErrorOccuried();
            }
            break;
        }
        }
    }
#endif
    return err;
}

_SGL void* _SglMapBufferRange(afxBuffer buf, afxSize offset, afxNat range, afxFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(buf->base.siz, offset, range);
    
    void *map = &(buf->base.bytemap[offset]);

    ++buf->locked;
    buf->lastUpdOffset = offset;
    buf->lastUpdRange = range;

    if (flags & AFX_BUF_MAP_W)
        buf->updFlags |= SGL_UPD_FLAG_DEVICE_FLUSH;

    if (flags & AFX_BUF_MAP_X)
    {
        buf->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
    }

    return map;
}

_SGL afxError _SglUnmapBufferRange(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    --buf->locked;
    return err;
}

_SGL _afxBufVmt const _SglBufVmt =
{
    _SglMapBufferRange,
    _SglUnmapBufferRange
};

_SGL afxError _SglBufDtor(afxBuffer buf)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("buf=%p", buf);

    afxDrawContext dctx = AfxGetObjectProvider(buf);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    if (buf->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 0, buf->glHandle);
        buf->glHandle = 0;
    }

    if (buf->base.bytemap)
    {
        AfxDeallocate(mem, buf->base.bytemap);
    }
    return err;
}

_SGL afxError _SglBufCtor(afxBuffer buf, afxCookie const* cookie)
{
    AfxEntry("buf=%p", buf);
    afxResult err = NIL;
    AfxAssertObjects(1, &buf, afxFcc_BUF);

    afxBufferSpecification const *spec = ((afxBufferSpecification const *)cookie->udd[0]) + cookie->no;

    AfxAssert(spec);

    buf->base.siz = spec->siz;
    buf->base.usage = spec->usage;

    afxDrawContext dctx = AfxGetObjectProvider(buf);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    if (!(buf->base.bytemap = AfxAllocate(mem, buf->base.siz, 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        if (spec->src)
            AfxCopy(buf->base.bytemap, spec->src, buf->base.siz);

        buf->lastUpdOffset = 0;
        buf->lastUpdRange = buf->base.siz;
        buf->locked = FALSE;

        buf->glHandle = NIL;
        buf->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        buf->base.vmt = &_SglBufVmt;

        if (err)
            AfxDeallocate(mem, buf->base.bytemap);
    }
    return err;
}

_SGL afxError _SglVbufDtor(afxVertexBuffer vbuf)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("vbuf=%p", vbuf);

    //AfxAssert(vbuf->base.row);

    afxDrawContext dctx = AfxGetObjectProvider(vbuf);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    for (afxNat i = 0; i < vbuf->base.attrCnt; i++)
    {
        AfxDeallocateString(&vbuf->base.attrs[i].semantic);
    }

    AfxAssert(vbuf->base.attrs);
    AfxDeallocate(mem, vbuf->base.attrs);

    AfxReleaseObjects(1, (void*[]) { vbuf->base.buf });

    AfxAssert(vbuf->base.storages);
    AfxDeallocate(mem, vbuf->base.storages);

    return err;
}

#if 0
_SGL afxError _SglVbufCtor(afxVertexBuffer vbuf, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    AfxEntry("vbuf=%p", vbuf);

    afxVertexBufferBlueprint const *blueprint = ((afxVertexBufferBlueprint const *)cookie->udd[0]) + cookie->no;

    AfxAssert(blueprint);

    AfxAssert(blueprint->cap);
    vbuf->base.cap = blueprint->cap;

    AfxAssert(blueprint->rowCnt);
    //vbuf->base.rowCnt = spec->rowCnt;

    afxDrawContext dctx = AfxGetObjectProvider(vbuf);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    afxNat totalBytemapSiz = 0;

    for (afxNat j = 0; j < blueprint->rowCnt; j++)
    {
        afxVertexFormat fmt = blueprint->row[j].fmt ? blueprint->row[j].fmt : blueprint->row[j].srcFmt;
        AfxAssert(afxVertexFormat_TOTAL > fmt);

        if (err)
            break;

        totalBytemapSiz += AFX_ALIGN(AfxVertexFormatGetSize(fmt) * blueprint->cap, AFX_SIMD_ALIGN);
    }

    AfxAssert(totalBytemapSiz);

    afxBufferSpecification bufSpec =
    {
        totalBytemapSiz,
        afxBufferUsage_VERTEX,
        NIL
    };

    if (AfxAcquireBuffers(dctx, 1, &vbuf->base.buf, &bufSpec)) AfxThrowError();
    else
    {
        if (!(vbuf->base.row = AfxAllocate(mem, blueprint->rowCnt * sizeof(vbuf->base.row[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            afxVertexFormat fmt;
            afxNat nextStreamOffset = 0;
            vbuf->base.rowCnt = 0;

            for (afxNat i = 0; i < blueprint->rowCnt; i++)
            {
                afxVertexDataRow *row = &vbuf->base.row[i];

                if (!AfxStringIsEmpty(&blueprint->row[i].semantic.str))
                    AfxCloneString(&row->semantic, &blueprint->row[i].semantic.str);
                else
                    AfxResetString(&row->semantic);

                row->usage = blueprint->row[i].usage;
                row->relOffset = 0;

                fmt = blueprint->row[i].fmt ? blueprint->row[i].fmt : blueprint->row[i].srcFmt;
                AfxAssert(afxVertexFormat_TOTAL > fmt);
                row->fmt = fmt;
                row->cnt = 1;

                afxNat stride = AfxVertexFormatGetSize(fmt) * row->cnt;
                AfxAssert(stride);
                row->streamIdx = 0;
                row->streamOffset = nextStreamOffset;
                afxNat streamRange = AFX_ALIGN(stride * vbuf->base.cap, AFX_SIMD_ALIGN);
                nextStreamOffset += streamRange;

                vbuf->base.rowCnt++;

                if (blueprint->row[i].src)
                {
                    AfxAssert(afxVertexFormat_TOTAL > blueprint->row[i].srcFmt);

                    if (AfxUpdateVertexBuffer(vbuf, i, 0, vbuf->base.cap, blueprint->row[i].src, blueprint->row[i].srcFmt))
                        AfxThrowError();
                }
            }

            if (err)
            {
                AfxAssert(vbuf->base.row);
                AfxDeallocate(mem, vbuf->base.row);
            }
        }
    }
    return err;
}
#endif

_SGL afxError _SglVbufCtorNEW(afxVertexBuffer vbuf, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &vbuf, afxFcc_VBUF);
    AfxEntry("vbuf=%p", vbuf);

    afxVertexBufferBlueprint const *blueprint = ((afxVertexBufferBlueprint const *)cookie->udd[0]) + cookie->no;

    AfxAssert(blueprint);

    AfxAssert(blueprint->cap);
    vbuf->base.cap = blueprint->cap;

    AfxAssert(blueprint->attrCnt);
    //vbuf->base.rowCnt = spec->rowCnt;

    afxDrawContext dctx = AfxGetObjectProvider(vbuf);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    afxNat secCnt = 0;
    afxNat secIdxRemap[16] = { 0 };
    afxNat secRange[16] = { 0 };
    afxNat secStride[16] = { 0 };
    afxNat attrCnt = blueprint->attrCnt;
    afxNat totalBufSiz = 0;

    for (afxNat i = 0; i < attrCnt; i++)
    {
        afxVertexRowBlueprint const* spec = &blueprint->spec[i];
        afxBool found = FALSE;
        afxNat secIdx;

        for (afxNat j = 0; j < secCnt; j++)
        {
            if ((found = (spec->secIdx == secIdxRemap[j])))
            {
                secIdx = j;
                break;
            }
        }

        if (!found)
        {
            secIdx = secCnt;
            secIdxRemap[secIdx] = blueprint->spec[i].secIdx;
            secCnt++;
        }

        afxVertexFormat fmt = spec->fmt ? spec->fmt : spec->srcFmt;
        AfxAssertRange(afxVertexFormat_TOTAL, fmt, 1);
        afxNat attrSiz = AfxVertexFormatGetSize(fmt);
        afxNat attrArraySiz = AFX_ALIGN(attrSiz * blueprint->cap, AFX_SIMD_ALIGN);
        totalBufSiz += attrArraySiz;
        secRange[secIdx] += attrArraySiz;
        secStride[secIdx] += attrSiz;
    }

    if (!(vbuf->base.storages = AfxAllocate(mem, secCnt * sizeof(vbuf->base.storages[0]), 0, AfxSpawnHint()))) AfxThrowError();
    else
    {
        AfxAssert(totalBufSiz);

        afxBufferSpecification bufSpec =
        {
            totalBufSiz,
            afxBufferUsage_VERTEX,
            NIL
        };

        if (AfxAcquireBuffers(dctx, 1, &vbuf->base.buf, &bufSpec)) AfxThrowError();
        else
        {
            afxNat nextSecBase = 0;
            vbuf->base.secCnt = 0;

            for (afxNat i = 0; i < secCnt; i++)
            {
                vbuf->base.storages[i].base = nextSecBase;
                vbuf->base.storages[i].range = secRange[i];
                vbuf->base.storages[i].stride = secStride[i];

                nextSecBase += vbuf->base.storages[i].range;
                ++vbuf->base.secCnt;
            }

            if (!(vbuf->base.attrs = AfxAllocate(mem, attrCnt * sizeof(vbuf->base.attrs[0]), 0, AfxSpawnHint()))) AfxThrowError();
            else
            {
                afxNat nextAttrOffset[16] = { 0 };
                vbuf->base.attrCnt = 0;

                for (afxNat i = 0; i < attrCnt; i++)
                {
                    afxVertexRowBlueprint const* spec = &blueprint->spec[i];
                    AfxCloneString(&vbuf->base.attrs[i].semantic, &spec->semantic.str);
                    
                    afxVertexFormat fmt = spec->fmt ? spec->fmt : spec->srcFmt;
                    AfxAssertRange(afxVertexFormat_TOTAL, fmt, 1);
                    vbuf->base.attrs[i].fmt = fmt;
                    afxNat attrSiz = AfxVertexFormatGetSize(fmt);
                    
                    afxNat secIdx = 0;
                    afxBool found = FALSE;

                    for (afxNat j = 0; j < secCnt; j++)
                    {
                        if ((found = (spec->secIdx == secIdxRemap[j])))
                        {
                            secIdx = j;
                            break;
                        }
                    }

                    vbuf->base.attrs[i].srcIdx = secIdx;
                    vbuf->base.attrs[i].usage = spec->usage;
                    vbuf->base.attrs[i].offset = nextAttrOffset[secIdx];
                    nextAttrOffset[secIdx] += attrSiz;

                    vbuf->base.attrCnt++;

                    if (spec->src)
                    {
                        AfxAssertRange(afxVertexFormat_TOTAL, spec->srcFmt, 1);

                        //if (AfxUpdateVertexBuffer(vbuf, i, 0, vbuf->base.cap, spec->src, spec->srcFmt, spec->srcStride))
                            AfxThrowError();
                    }
                }

                if (err)
                {
                    AfxAssert(vbuf->base.attrs);
                    AfxDeallocate(mem, vbuf->base.attrs);
                }
            }

            if (err)
                AfxReleaseObjects(1, (void*[]) { vbuf->base.buf });
        }


        if (err)
        {
            AfxAssert(vbuf->base.storages);
            AfxDeallocate(mem, vbuf->base.storages);
        }
    }
    return err;
}

_SGL afxError _SglIbufDtor(afxIndexBuffer ibuf)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("ibuf=%p", ibuf);
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);

    afxDrawContext dctx = AfxGetObjectProvider(ibuf);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    AfxDeallocate(mem, ibuf->base.regions);

    AfxReleaseObjects(1, (void*[]) { ibuf->base.buf });

    return err;
}

_SGL afxError _SglIbufCtor(afxIndexBuffer ibuf, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ibuf, afxFcc_IBUF);
    AfxEntry("ibuf=%p", ibuf);
    
    afxIndexBufferBlueprint const *blueprint = ((afxIndexBufferBlueprint const *)cookie->udd[0]) + cookie->no;
    AfxAssert(blueprint);

    afxDrawContext dctx = AfxGetObjectProvider(ibuf);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);


    afxNat totalSiz = 0;

    for (afxNat j = 0; j < AfxCountArrayElements(&blueprint->regions); j++)
    {
        afxIndexBufferBlueprintRegion* rgn = AfxGetArrayUnit(&blueprint->regions, j);
        AfxAssert(rgn->idxCnt);
        afxNat idxSiz = rgn->idxCnt < AFX_N8_MAX ? sizeof(afxNat8) : (rgn->idxCnt < AFX_N16_MAX ? sizeof(afxNat16) : sizeof(afxNat32));

        totalSiz += idxSiz * rgn->idxCnt;
    }

    afxBufferSpecification bufSpec =
    {
        totalSiz,
        afxBufferUsage_INDEX,
        NIL
    };

    if (AfxAcquireBuffers(dctx, 1, &ibuf->base.buf, &bufSpec)) AfxThrowError();
    else
    {
        ibuf->base.regionCnt = 0;

        if (!(ibuf->base.regions = AfxAllocate(mem, sizeof(ibuf->base.regions[0]) * AfxCountArrayElements(&blueprint->regions), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            afxNat32 baseOffset = 0;

            for (afxNat j = 0; j < AfxCountArrayElements(&blueprint->regions); j++)
            {
                afxIndexBufferBlueprintRegion* rgn = AfxGetArrayUnit(&blueprint->regions, j);
                AfxAssert(rgn->idxCnt);
                afxNat idxSiz = rgn->idxCnt < AFX_N8_MAX ? sizeof(afxNat8) : (rgn->idxCnt < AFX_N16_MAX ? sizeof(afxNat16) : sizeof(afxNat32));

                afxNat byteSiz = idxSiz * rgn->idxCnt;

                ibuf->base.regions[ibuf->base.regionCnt].baseOffset = baseOffset;
                ibuf->base.regions[ibuf->base.regionCnt].idxCnt = rgn->idxCnt;
                ibuf->base.regions[ibuf->base.regionCnt].idxSiz = idxSiz;

                baseOffset += byteSiz;
                AfxAssert(ibuf->base.regionCnt == j);
                ++ibuf->base.regionCnt;

                if (rgn->src)
                {
                    AfxAssert(rgn->srcIdxSiz);

                    if (AfxIndexBufferUpdate(ibuf, j, 0, rgn->idxCnt, rgn->src, rgn->srcIdxSiz))
                        AfxThrowError();
                }
            }
        }

        if (err)
            AfxReleaseObjects(1, (void*[]) { ibuf->base.buf });
    }
    return err;
}

_SGL afxClassConfig _SglBufClsConfig =
{
    .fcc = afxFcc_BUF,
    .name = "Buffer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxBuffer)),
    .ctx = NIL,
    .ctor = (void*)_SglBufCtor,
    .dtor = (void*)_SglBufDtor
};

_SGL afxClassConfig _SglVbufClsConfig =
{
    .fcc = afxFcc_VBUF,
    .name = "Vertex Buffer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxVertexBuffer)),
    .ctx = NIL,
    .ctor = (void*)_SglVbufCtorNEW,
    .dtor = (void*)_SglVbufDtor
};

_SGL afxClassConfig _SglIbufClsConfig =
{
    .fcc = afxFcc_IBUF,
    .name = "Index Buffer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxIndexBuffer)),
    .ctx = NIL,
    .ctor = (void*)_SglIbufCtor,
    .dtor = (void*)_SglIbufDtor
};
