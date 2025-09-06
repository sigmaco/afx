/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_CANVAS_C
#include "avxImplementation.h"

_AVX afxSize _AvxGetRasterRange(avxRaster ras, avxRasterRegion const* rgn, afxSize* offset)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
    AFX_ASSERT(offset);
    AFX_ASSERT(rgn);
    return 0;
}

_AVX afxError _AvxDpuClearCanvas(avxDpu* dpu, afxUnit binCnt, afxUnit const bins[], avxClearValue const values[], afxUnit areaCnt, afxLayeredRect const areas[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu->inDrawScope);
    // This is not a transfer command, as it required a ongoing draw scope to having a target canvas.

    // void vkCmdClearAttachments( VkCommandBuffer commandBuffer, uint32_t attachmentCount, const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);

    // TODO: It will be need to generate a new FBO, bind attachments to it, to be able to choose LOD and layer, then clear.

    for (afxUnit i = 0; i < binCnt; i++)
    {
        avxRaster ras;
        if (!AvxGetDrawBuffers(dpu->canv, bins[i], 1, &ras))
            continue;

        afxByte* dstData = _AvxGetClientRasterData(ras, 0);

        for (afxUnit j = 0; j < areaCnt; j++)
        {
            afxLayeredRect const* area = &areas[j];

            avxRasterRegion rgn = { 0 };
            rgn.origin.x = area->area.x;
            rgn.origin.y = area->area.y;
            rgn.origin.z = (afxInt)area->baseLayer;
            rgn.whd.w = area->area.w;
            rgn.whd.h = area->area.h;
            rgn.whd.d = area->layerCnt;

            avxFormat fmt = AvxGetRasterFormat(ras);
            avxRasterArrangement lay = { 0 };
            AvxQueryRasterArrangement(ras, &rgn, &lay);

            avxColor cache[32];
            AvxConvertFormat(32, 1, &dstData[lay.offset], 0, fmt, avxFormat_RGBA32f, cache, sizeof(cache[0][0]));

            afxUnit cacheIter = (area->area.w * area->area.h * area->layerCnt) / 32;
            afxUnit cacheLeft = (area->area.w * area->area.h * area->layerCnt) % 32;

            for (afxUnit i = 0; i < cacheIter; i++)
            {
                avxRasterArrangement lay;
                AvxQueryRasterArrangement(ras, &rgn, &lay);
                AvxConvertFormat(lay.rowStride, 1, cache, sizeof(32), avxFormat_RGBA32f, fmt, &dstData[lay.offset], lay.rowStride);
            }
        }
    }
}

_AVX void _AvxDpuCommenceDrawScope(avxDpu* dpu, avxCanvas canv, afxRect const* area, afxUnit baseLayer, afxUnit layerCnt, afxUnit cCnt, avxDrawTarget const* c, avxDrawTarget const* d, avxDrawTarget const* s, afxString const* dbgTag, afxBool defFbo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.
    dpu->inDrawScope = TRUE;

    avxDrawTarget const* dt;

    if (!canv)
    {
        return;
    }

    afxRect areaMax;
    AvxGetCanvasArea(canv, AVX_ORIGIN_ZERO, &areaMax);
    avxRange canvWhd = { areaMax.w, areaMax.h, 1 };

    afxUnit maxColSurCnt;
    afxUnit dsSurIdx[2] = { AFX_INVALID_INDEX, AFX_INVALID_INDEX };
    afxUnit maxSurCnt = AvxQueryCanvasBins(canv, &maxColSurCnt, &dsSurIdx[0], &dsSurIdx[1]);
    afxBool hasDs = ((dsSurIdx[1] != AFX_INVALID_INDEX) || (dsSurIdx[0] != AFX_INVALID_INDEX));
    afxBool combinedDs = (hasDs && (dsSurIdx[1] == dsSurIdx[0]));
    cCnt = AFX_MIN(cCnt, maxColSurCnt);

    afxUnit storeCnt = 0;
    afxUnit storeBufs[AVX_MAX_CANVAS_BUFFERS];
    afxMask storeBitmask = NIL;

    afxUnit clearCnt = 0;
    afxUnit clearBufs[AVX_MAX_CANVAS_BUFFERS];
    afxUnit invalidCnt = 0;
    afxUnit invalidBufs[AVX_MAX_CANVAS_BUFFERS];

    //dpu->invalidDrawBufCnt = 0;

    for (afxUnit i = 0; i < cCnt; i++)
    {
        dt = &c[i];

        if (!(i < maxColSurCnt))
            continue;

        switch (dt->storeOp)
        {
        case avxStoreOp_STORE:
        {
            storeBufs[storeCnt] = i;
            storeCnt++;
            storeBitmask |= AFX_BITMASK(i);
            break;
        }
        case avxStoreOp_DISCARD:
        case avxStoreOp_DONT_CARE:
        default:
        {
            storeBufs[storeCnt] = i;
            storeCnt++;
            storeBitmask |= AFX_BITMASK(8 + i);

            invalidBufs[invalidCnt] = i;
            //dpu->invalidDrawBufs[invalidCnt] = i;
            //dpu->invalidDrawBufCnt = invalidCnt;
            ++invalidCnt;
            break;
        }
        }

        // Prepare cleanups and invalidations.

        if (dt->loadOp != avxLoadOp_LOAD)
        {
            /*
                The application should always call glClear() for every render target at the start of
                each render target's rendering sequence, provided that the previous contents of the
                attachments are not needed, of course.
            */
            clearBufs[clearCnt] = i;
            ++clearCnt;
        }
    }

    if ((dsSurIdx[0] != AFX_INVALID_INDEX)) // depth (and stencil)
    {
        dt = d;
        // Prepare cleanups and invalidations.

        if (dt && (dt->loadOp != avxLoadOp_LOAD))
        {
            /*
                The application should always call glClear() for every render target at the start of
                each render target's rendering sequence, provided that the previous contents of the
                attachments are not needed, of course.
            */
            clearBufs[clearCnt] = dsSurIdx[0];
            ++clearCnt;
        }

        if (dt && (dt->storeOp != avxStoreOp_STORE))
        {
            invalidBufs[invalidCnt] = dsSurIdx[0];
            ++invalidCnt;
        }
    }

    if ((dsSurIdx[1] != AFX_INVALID_INDEX) && !combinedDs) // stencil
    {
        dt = s;
        // Prepare cleanups and invalidations.

        if (dt && (dt->loadOp != avxLoadOp_LOAD))
        {
            /*
                The application should always call glClear() for every render target at the start of
                each render target's rendering sequence, provided that the previous contents of the
                attachments are not needed, of course.
            */
            clearBufs[clearCnt] = dsSurIdx[1];
            ++clearCnt;
        }

        if (dt && (dt->storeOp != avxStoreOp_STORE))
        {
            invalidBufs[invalidCnt] = dsSurIdx[1];
            ++invalidCnt;
        }
    }

    // TODO iterate for each canvas surface against arguments

    //dpu->activeRs.canv = canv;

    afxBool clipped = ((area->x && (area->x > 0)) ||
        (area->y && (area->y > 0)) ||
        (area->w && (area->w < canvWhd.w)) ||
        (area->h && (area->h < canvWhd.h)));
    //dpu->drawAreaClipped = clipped;

    if (invalidCnt)
    {
        if (clipped)
        {
            //gl->InvalidateSubFramebuffer(GL_DRAW_FRAMEBUFFER, invalidCnt, dpu->invalidDrawBufs, area->x, area->y, area->w, area->h); _ZglThrowErrorOccuried();
        }
        else
        {
            //gl->InvalidateFramebuffer(GL_DRAW_FRAMEBUFFER, invalidCnt, dpu->invalidDrawBufs); _ZglThrowErrorOccuried();
        }
    }

#if 0
    if (canv->storeBitmask != storeBitmask)
#endif
    {
        //canv->storeBitmask = storeBitmask; // cache it
        //gl->DrawBuffers(storeCnt, storeBufs); _ZglThrowErrorOccuried();

        // What to do with depth/stencil storage set to discard?
        // Should we force a glDepthMask to zero right there?
        // Should we force a glStencilMask to zero right there?
    }

    // Effectively invalidate the buffers.

    // TODO: Sanitize area to canvas' bounds.

    // We must store the scissor rects to avoid DPU to further apply invalid rects.
    for (afxUnit iter = 0; iter < AVX_MAX_VIEWPORTS; iter++)
    {
        //dpu->nextScisRects[iter] = *area;
        //dpu->nextScisUpdMask |= AFX_BITMASK(iter);
    }

    // Effectively clear the buffers.

    for (afxUnit i = 0; i < clearCnt; i++)
    {

    }
}

_AVX void _AvxDpuConcludeDrawScope(avxDpu* dpu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu->inDrawScope != FALSE); // This is a transfer operation.
    dpu->inDrawScope = FALSE;

    avxCanvas canv = dpu->canv;

    if (canv)
    {
        afxUnit surCnt;
        surCnt = AvxQueryCanvasBins(canv, NIL, NIL, NIL);
#if 0
        if (surCnt)
        {
            AFX_ASSERT(_AVX_MAX_SURF_PER_CANV >= surCnt);
            avxRaster surfaces[_AVX_MAX_SURF_PER_CANV];
            AvxGetDrawBuffers(canv, 0, surCnt, surfaces);

            for (afxUnit i = 0; i < surCnt; i++)
            {
                avxRaster ras = surfaces[i];

                if (ras)
                {
                    AFX_ASSERT_OBJECTS(afxFcc_RAS, 1, &ras);
                    ras->updFlags |= AVX_UPD_FLAG_HOST_FLUSH;
                }
            }
        }
#endif
    }
}

_AVX void _AvxDpuNextPass(avxDpu* dpu)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(dpu->inDrawScope != FALSE); // This is a transfer operation.
}

_AVX afxError _AvxDpuResolveCanvas(avxDpu* dpu, avxCanvas src, avxCanvas dst, afxUnit opCnt, avxRasterCopy const ops[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &dst);
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &src);

    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    /*
        vkCmdResolveImage
        Purpose: Resolves a multisampled image to a non-multisampled image.
        Filter: Always uses nearest filtering (VK_FILTER_NEAREST).
        Restrictions:
            Source must be multisampled.
            Destination must be single-sampled.
            Format must match.
            Only supports resolving a 1:1 region (no scaling or filtering choice).

        To mimic vkCmdResolveImage, we should:
            Use nearest filtering only (to match Vulkan's behavior).
            Ensure that we resolve from a multisample framebuffer into a single-sample texture.
            Avoid any format conversion or scaling.
    */

    for (afxUnit sbufIdx = 0, dbufIdx = 0; (sbufIdx < src->binCnt) && (dbufIdx < dst->binCnt); sbufIdx++, dbufIdx++)
    {
        if (src->bins[sbufIdx].buf && dst->bins[dbufIdx].buf)
            _AvxDpuResolveRaster(dpu, dst->bins[dbufIdx].buf, opCnt, ops, src->bins[sbufIdx].buf);
    }
    return err;
}

_AVX afxError _AvxDpuBlitCanvas(avxDpu* dpu, avxCanvas src, avxCanvas dst, afxUnit opCnt, avxRasterBlit const ops[], avxTexelFilter flt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &dst);
    AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &src);

    AFX_ASSERT(dpu->inDrawScope == FALSE); // This is a transfer operation.

    /*
        vkCmdBlitImage
        Purpose: General-purpose image copy, potentially with format conversion and filtering.
        Filter: User-specified --- VK_FILTER_NEAREST or VK_FILTER_LINEAR.
        More flexible, can scale images and convert formats.

        To match Vulkan's vkCmdBlitImage, OpenGL's glBlitFramebuffer can be used similarly,
        but with either GL_LINEAR or GL_NEAREST as needed. Or, for more general flexibility
        (e.g., for mipmap generation or format conversion), a shader-based blit might be required.
    */

    for (afxUnit sbufIdx = 0, dbufIdx = 0; (sbufIdx < src->binCnt) && (dbufIdx < dst->binCnt); sbufIdx++, dbufIdx++)
    {
        if (src->bins[sbufIdx].buf && dst->bins[dbufIdx].buf)
            _AvxDpuBlitRaster(dpu, dst->bins[dbufIdx].buf, opCnt, ops, src->bins[sbufIdx].buf, flt);
    }
    return err;
}
