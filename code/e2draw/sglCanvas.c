/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "sgl.h"
#include "qwadro/draw/pipe/afxCanvas.h"
#include "qwadro/draw/afxDrawOutput.h"
#include "qwadro/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// FRAME BUFFER                                                               //
////////////////////////////////////////////////////////////////////////////////

_SGL void _SglBindFboAttachment(glVmt const* gl, GLenum glTarget, GLenum glAttachment, GLenum texTarget, GLuint texHandle, GLint level, GLuint z)
{
    afxError err = AFX_ERR_NONE;

    switch (texTarget)
    {
    case GL_TEXTURE_2D:
    case GL_TEXTURE_1D_ARRAY:
    {
        gl->FramebufferTexture2D(glTarget, glAttachment, texTarget, texHandle, level); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_1D:
    {
        gl->FramebufferTexture1D(glTarget, glAttachment, texTarget, texHandle, level); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_3D:
    case GL_TEXTURE_2D_ARRAY:
    {
        gl->FramebufferTexture3D(glTarget, glAttachment, texTarget, texHandle, level, z); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    default:
        gl->FramebufferTexture(glTarget, glAttachment, texHandle, level); _SglThrowErrorOccuried();
        break;
    };
}

_SGL afxError _SglDpuBindAndSyncCanv(glVmt const* gl, afxBool syncOnly, GLenum glTarget, afxCanvas canv)
{
    //AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    GLuint glHandle;

    if (!canv)
    {
        glHandle = 0;
        gl->BindFramebuffer(glTarget, glHandle); _SglThrowErrorOccuried();
    }
    else
    {
        AfxAssertObjects(1, &canv, afxFcc_CANV);
        glHandle = canv->glHandle;
        afxBool bound = FALSE;
        sglUpdateFlags devUpdReq = (canv->updFlags & SGL_UPD_FLAG_DEVICE);

        afxRaster ras;
        afxNat const surfCnt = AfxCountSurfaces(canv);

        if ((!glHandle) || (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (glHandle)
            {
                gl->DeleteFramebuffers(1, &(glHandle)); _SglThrowErrorOccuried();
                canv->glHandle = NIL;
                glHandle = NIL;
            }

            gl->GenFramebuffers(1, &(glHandle)); _SglThrowErrorOccuried();
            gl->BindFramebuffer(glTarget, glHandle); _SglThrowErrorOccuried();
            bound = TRUE;
            AfxAssert(gl->IsFramebuffer(glHandle));
            canv->glHandle = glHandle;

            afxNat rasterColorIdx = 0;

            afxNat dsSurIdx[2];
            AfxGetDepthSurface(canv, &dsSurIdx[0]);
            afxBool combinedDs = (AfxGetStencilSurface(canv, &dsSurIdx[1]) && (dsSurIdx[1] == dsSurIdx[0]));

            for (afxNat i = 0; i < surfCnt; i++)
            {
                GLenum glAttachment;
                GLuint glTexHandle;
                GLenum glTexTarget;

                if (!AfxGetSurfaceRasters(canv, i, 1, &ras))
                {
                    AfxThrowError();
                    glTexHandle = 0;
                    glTexTarget = GL_TEXTURE_2D;
                    glAttachment = GL_COLOR_ATTACHMENT0 + rasterColorIdx;
                }
                else
                {
                    AfxAssertObjects(1, &ras, afxFcc_RAS);
                    AfxAssert(AfxTestRaster(ras, afxRasterFlag_DRAW));
                    
                    if (dsSurIdx[0] == i) // depth
                    {
                        if (!combinedDs)
                            glAttachment = GL_DEPTH_ATTACHMENT;
                        else
                            glAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
                    }
                    else if (!combinedDs && (dsSurIdx[1] == i)) glAttachment = GL_STENCIL_ATTACHMENT;
                    else
                    {
                        glAttachment = GL_COLOR_ATTACHMENT0 + rasterColorIdx;
                        ++rasterColorIdx;
                    }

                    _SglBindAndSyncTex(gl, TRUE, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);
                    glTexHandle = ras->glHandle;
                    glTexTarget = ras->glTarget;
                    AfxAssert(gl->IsTexture(glTexHandle));
                }
                _SglBindFboAttachment(gl, glTarget, glAttachment, glTexTarget, glTexHandle, 0, 0);
            }

            switch (gl->CheckFramebufferStatus(glTarget))
            {
            case GL_FRAMEBUFFER_COMPLETE:
                canv->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("afxCanvas %p hardware-side data instanced.", canv);
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                AfxError("Not all framebuffer attachment points are framebuffer attachment complete.");
                _SglThrowErrorOccuried();
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                AfxError("No images are attached to the framebuffer.");
                _SglThrowErrorOccuried();
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                AfxError("The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.");
                _SglThrowErrorOccuried();
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                AfxError("Incomplete draw frame buffer.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                AfxError("Incomplete read frame buffer.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                AfxError("Incomplete multisample.");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                AfxError("Incomplete layer targets.");
                break;
            case GL_FRAMEBUFFER_UNDEFINED:
                AfxError("Default framebuffer does not exist");
                break;
            default:
                AfxError("UNKNOWN");
                _SglThrowErrorOccuried();
                break;
            }

            AfxEcho("afxCanvas %p hardware-side data instanced.", canv);
        }
        else
        {
            for (afxNat i = 0; i < surfCnt; i++)
            {
                AfxGetSurfaceRasters(canv, i, 1, &ras);
                AfxAssertObjects(1, &ras, afxFcc_RAS);
                _SglBindAndSyncTex(gl, TRUE, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);
            }

            if (!syncOnly)
            {
                gl->BindFramebuffer(glTarget, glHandle); _SglThrowErrorOccuried();
                bound = TRUE;
            }
        }

        if (bound && syncOnly)
        {
            gl->BindFramebuffer(glTarget, 0); _SglThrowErrorOccuried();
        }
    }
    return err;
}

_SGL afxError _SglRelinkSurfacesCb(afxCanvas canv, afxBool managed, afxNat baseSurf, afxNat surfCnt, afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);
    AfxAssertRange(canv->base.surfaceCnt, baseSurf, surfCnt);
    AfxAssert(rasters);

    for (afxNat i = 0; i < surfCnt; i++)
    {
        afxNat surfIdx = baseSurf + i;
        afxRaster curr = canv->base.surfaces[surfIdx].ras;
        afxRaster ras = rasters ? rasters[i] : NIL;

        if (ras != curr)
        {
            if (ras)
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);

                if (!AfxTestRaster(ras, afxRasterFlag_DRAW)) AfxThrowError();
                else
                {
                    if (canv->base.surfaces[surfIdx].fmt != AfxGetRasterFormat(ras)) AfxThrowError();
                    else
                    {
                        afxWhd ext;
                        AfxGetRasterExtent(ras, 0, ext);
                        afxNat layerCnt = AfxCountRasterLayers(ras);

                        if ((ext[0] < canv->base.wh[0]) && (ext[1] < canv->base.wh[1]) && (layerCnt < canv->base.layerCnt)) AfxThrowError();
                        else
                        {
                            AfxReacquireObjects(1, (void*[]) { ras });
                            canv->base.surfaces[surfIdx].managed = !!managed;
                            ++canv->base.linkedCnt;
                        }
                    }
                }
            }
            else
            {
                canv->base.surfaces[surfIdx].managed = TRUE;
            }

            canv->base.surfaces[surfIdx].ras = ras;

            if (curr)
            {
                AfxAssertObjects(1, &curr, afxFcc_RAS);
                AfxReleaseObjects(1, (void*[]) { curr });
                --canv->base.linkedCnt;
            }
        }
    }
    return err;
}

_SGL afxError _SglReadjustCanvasCb(afxCanvas canv, afxNat const wh[])
{
	afxError err = AFX_ERR_NONE;
	AfxAssertObjects(1, &canv, afxFcc_CANV);
	AfxAssert(wh);
	AfxAssert(wh[0]);
	AfxAssert(wh[1]);
    afxWhd minWhd = { SGL_MAX_CANVAS_WIDTH, SGL_MAX_CANVAS_HEIGHT, SGL_MAX_CANVAS_LAYERS }, surWhd;
    
    for (afxNat i = 0; i < canv->base.surfaceCnt; i++)
    {
        afxRaster ras = canv->base.surfaces[i].ras;

        if (ras)
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
            AfxGetRasterExtent(ras, 0, surWhd);
            AfxMinWhd(minWhd, minWhd, surWhd);
        }
    }

    if ((wh[0] > minWhd[0]) || (wh[1] > minWhd[1]))
        AfxThrowError();

    if ((wh[0] != minWhd[0]) || (wh[1] != minWhd[1]))
    {
        canv->base.wh[0] = wh[0] < minWhd[0] ? wh[0] : minWhd[0];
        canv->base.wh[1] = wh[1] < minWhd[1] ? wh[1] : minWhd[1];
        canv->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
    }
	return err;
}

_SGL afxError _AfxCanvDropAllSurfaces(afxCanvas canv)
{
	AfxEntry("canv=%p", canv);
	afxError err = AFX_ERR_NONE;
	AfxAssertObjects(1, &canv, afxFcc_CANV);

	for (afxNat i = 0; i < canv->base.surfaceCnt; i++)
	{
		afxRaster ras = canv->base.surfaces[i].ras;

        if (ras)
        {
            AfxAssertObjects(1, &ras, afxFcc_RAS);
            AfxReleaseObjects(1, (void*[]) { ras });
            canv->base.surfaces[i].ras = NIL;
        }
	}

	//canv->base.colorCnt = 0;
    //canv->base.surfCnt = 0;
	return err;
}

_SGL afxError _SglCanvDtor(afxCanvas canv)
{
    AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);

    afxDrawContext dctx = AfxGetObjectProvider(canv);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    _AfxCanvDropAllSurfaces(canv);

    if (canv->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 3, canv->glHandle);
        canv->glHandle = 0;
    }
    //AfxAssert(!canv->idd);

    AfxDeallocate(mmu, canv->base.surfaces);

    return err;
}

_SGL afxError _SglCanvCtor(afxCanvas canv, afxCookie const* cookie)
{
    AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = cookie->udd[0];
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);
    afxNat const* wh = cookie->udd[1];
    afxNat layerCnt = *(afxNat const*)cookie->udd[2];
    afxNat surCnt = *(afxNat const *)cookie->udd[3];
    afxSurfaceConfig const* surCfgs = cookie->udd[4];

    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    canv->base.wh[0] = wh[0];
    canv->base.wh[1] = wh[1];
    canv->base.layerCnt = layerCnt;

    canv->base.readjust = _SglReadjustCanvasCb;
    canv->base.relink = _SglRelinkSurfacesCb;
    canv->updFlags = SGL_UPD_FLAG_DEVICE_INST;
    canv->glHandle = 0;

    afxBool combinedDs = FALSE;
    afxNat depthInIdx = AFX_INVALID_INDEX;
    afxNat stencilInIdx = AFX_INVALID_INDEX;
    afxNat surfaceCnt = 0;
    afxNat colorCnt = 0;
    afxRaster colorRasters[SGL_MAX_COLOR_ATTACHMENTS];

    for (afxNat i = 0; i < surCnt; i++)
    {
        afxSurfaceConfig const* sur = &surCfgs[i];
        AfxAssert(sur->fmt);
        AfxAssert(sur->sampleCnt);

        if (depthInIdx == AFX_INVALID_INDEX)
        {
            if (AfxPixelFormatIsDepth(sur->fmt))
            {
                depthInIdx = i;

                if ((combinedDs = AfxPixelFormatIsCombinedDepthStencil(sur->fmt)))
                    stencilInIdx = depthInIdx;

                ++surfaceCnt;
                continue;
            }
        }

        if (!combinedDs && (stencilInIdx == AFX_INVALID_INDEX))
        {
            if (AfxPixelFormatIsStencil(sur->fmt))
            {
                stencilInIdx = i;
                ++surfaceCnt;
                continue;
            }
        }

        ++colorCnt;
        ++surfaceCnt;
    }

    canv->base.linkedCnt = 0;
    canv->base.colorCnt = colorCnt;
    canv->base.ownershipMask = NIL;
    canv->base.combinedDs = combinedDs;

    if (!(canv->base.surfaces = AfxAllocate(mmu, sizeof(canv->base.surfaces[0]), surfaceCnt, 0, AfxHint()))) AfxThrowError();
    else
    {
        afxSurface* surf;

        for (afxNat i = 0; i < surfaceCnt; i++)
        {
            surf = &canv->base.surfaces[i];
            surf->ras = NIL;
            surf->resolve = NIL;
            surf->managed = TRUE;
            surf->fmt = surCfgs[i].fmt;
            surf->usage = afxRasterFlag_DRAW;
            surf->sampleCnt = surCfgs[i].sampleCnt;
        }

        canv->base.surfaceCnt = surfaceCnt;

        if (stencilInIdx == AFX_INVALID_INDEX) canv->base.dsIdx[1] = AFX_INVALID_INDEX;
        else
        {
            canv->base.dsIdx[1] = surfaceCnt - 1;

            surf = &canv->base.surfaces[canv->base.dsIdx[1]];
            surf->ras = NIL;
            surf->resolve = NIL;
            surf->managed = TRUE;
            surf->fmt = surCfgs[stencilInIdx].fmt;
            surf->usage = afxRasterFlag_DRAW;
            surf->sampleCnt = surCfgs[stencilInIdx].sampleCnt;
        }

        if (depthInIdx == AFX_INVALID_INDEX) canv->base.dsIdx[0] = AFX_INVALID_INDEX;
        else
        {
            if (combinedDs)
                canv->base.dsIdx[0] = canv->base.dsIdx[1];
            else if (stencilInIdx != AFX_INVALID_INDEX)
                canv->base.dsIdx[0] = canv->base.dsIdx[1] - 1;
            else
                canv->base.dsIdx[0] = surfaceCnt - 1;

            surf = &canv->base.surfaces[canv->base.dsIdx[0]];
            surf->ras = NIL;
            surf->resolve = NIL;
            surf->managed = TRUE;
            surf->fmt = surCfgs[depthInIdx].fmt;
            surf->usage = afxRasterFlag_DRAW;
            surf->sampleCnt = surCfgs[depthInIdx].sampleCnt;
        }
        

    }
#if 0
    afxWhd wh;
    afxRaster ras;
    afxPixelFormat fmt;

    if (0 == (canv->base.whd[0] * canv->base.whd[1] * canv->base.whd[2]))
    {
        for (afxNat i = 0; i < blueprint->crtCnt; i++)
        {
            if ((ras = blueprint->rgba[i].ras))
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);

                AfxGetRasterExtent(ras, 0, wh);

                canv->base.whd[0] = AfxMin(canv->base.whd[0], wh[0]);
                canv->base.whd[1] = AfxMin(canv->base.whd[1], wh[1]);
                canv->base.whd[2] = AfxMin(canv->base.whd[2], wh[2]);
            }
        }

        for (afxNat i = 0; i < sizeof(blueprint->ds) / sizeof(blueprint->ds[0]); i++)
        {
            if ((ras = blueprint->ds[i].ras))
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);

                AfxGetRasterExtent(ras, 0, wh);

                canv->base.whd[0] = AfxMin(canv->base.whd[0], wh[0]);
                canv->base.whd[1] = AfxMin(canv->base.whd[1], wh[1]);
                canv->base.whd[2] = AfxMin(canv->base.whd[2], wh[2]);
            }
        }
    }

    if (0 == (canv->base.whd[0] * canv->base.whd[1] * canv->base.whd[2])) AfxThrowError();
    else
    {
        canv->base.dsIdx[0] = AFX_INVALID_INDEX;
        canv->base.dsIdx[1] = AFX_INVALID_INDEX;

        afxPixelFormat depthFmt = blueprint->ds[0].fmt;
        afxPixelFormat stencilFmt = blueprint->ds[1].fmt;
        afxBool combinedDs;

#ifndef _AFX_OPTIMIZATION_CANV_DS_FMT_ROBUSTNESS

        if (depthFmt == afxPixelFormat_D24S8 && (!stencilFmt))
            depthFmt = afxPixelFormat_D24;

        if ((!depthFmt) && stencilFmt == afxPixelFormat_D24S8)
            depthFmt = afxPixelFormat_S8;

#ifndef _AFX_OPTIMIZATION_CANV_DS_FMT_COMBO

        if ((depthFmt == afxPixelFormat_D24 && stencilFmt == afxPixelFormat_S8))
            depthFmt = (stencilFmt = afxPixelFormat_D24S8);
#endif
#endif
        combinedDs = (depthFmt && stencilFmt && depthFmt == stencilFmt);

        canv->base.surfaceCnt = blueprint->crtCnt;

        if (depthFmt || stencilFmt)
        {
            if (combinedDs) ++canv->base.surfaceCnt;
            else
            {
                if (depthFmt)
                    ++canv->base.surfaceCnt;

                if (stencilFmt)
                    ++canv->base.surfaceCnt;
            }
        }

        canv->base.colorCnt = 0;
        canv->base.ownershipMask = NIL;

        if (!(canv->base.surfaces = AfxAllocate(mmu, sizeof(canv->base.surfaces[0]), canv->base.surfaceCnt, 0, AfxHint()))) AfxThrowError();
        else
        {
            afxRaster ras;

            for (afxNat i = 0; i < canv->base.surfaceCnt; i++)
                AfxZero(1, sizeof(canv->base.surfaces[0]), &canv->base.surfaces[i]);

            for (afxNat i = 0; i < blueprint->crtCnt; i++)
            {
                if ((ras = blueprint->rgba[i].ras))
                {
                    if (!AfxTestRaster(ras, afxRasterFlag_DRAW)) AfxThrowError();
                    else
                    {
                        AfxGetRasterExtent(ras, 0, wh);

                        if (canv->base.whd[0] > wh[0] || canv->base.whd[1] > wh[1] || canv->base.whd[2] > wh[2]) AfxThrowError();
                        else
                        {
                            if (AfxReacquireObjects(1, (void*[]) { ras })) AfxThrowError();
                            else
                            {
                                canv->base.surfaces[i].ras = ras;
                                canv->base.surfaces[i].usage = AfxTestRaster(ras, AFX_N32_MAX);
                                canv->base.surfaces[i].fmt = AfxGetRasterFormat(ras);
                                ++canv->base.colorCnt;
                                //canv->base.ownershipMask |= AfxGetBitOffset(i);
                            }
                        }
                    }
                }
                else
                {
                    fmt = blueprint->rgba[i].fmt;

                    if (!fmt) AfxThrowError();
                    else
                    {
                        //AfxAssert(!(blueprint->rasters[i].usage & afxRasterFlag_DRAW));
                        afxRasterFlags usage = blueprint->rgba[i].usage;
                        usage |= afxRasterFlag_DRAW;
                        afxRasterInfo texi = { 0 };
                        texi.whd[0] = canv->base.whd[0];
                        texi.whd[1] = canv->base.whd[1];
                        texi.whd[2] = canv->base.whd[2];
                        texi.layerCnt = 1;
                        texi.fmt = fmt;
                        texi.usage = usage;

                        if (AfxAcquireRasters(dctx, 1, &texi, &ras)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &ras, afxFcc_RAS);
                            AfxAssert(AfxTestRaster(ras, afxRasterFlag_DRAW));
                            canv->base.surfaces[i].ras = ras;
                            canv->base.surfaces[i].usage = usage;
                            canv->base.surfaces[i].fmt = fmt;
                            canv->base.ownershipMask |= AfxGetBitOffset(i);
                            ++canv->base.colorCnt;
                        }
                    }
                }
            }

            if (!err)
            {
                for (afxNat i = 0; i < sizeof(blueprint->ds) / sizeof(blueprint->ds[0]); i++)
                {
                    if ((ras = blueprint->ds[i].ras))
                    {
                        if (!AfxTestRaster(ras, afxRasterFlag_DRAW)) AfxThrowError();
                        else
                        {
                            AfxGetRasterExtent(ras, 0, wh);

                            if (canv->base.whd[0] > wh[0] || canv->base.whd[1] > wh[1] || canv->base.whd[2] > wh[2]) AfxThrowError();
                            else
                            {
                                if (!(AfxReacquireObjects(1, (void*[]) { ras }))) AfxThrowError();
                                else
                                {
                                    canv->base.dsIdx[i] = canv->base.colorCnt + i;
                                    canv->base.surfaces[canv->base.dsIdx[i]].ras = ras;
                                    canv->base.surfaces[canv->base.dsIdx[i]].usage = AfxTestRaster(ras, AFX_N32_MAX);
                                    canv->base.surfaces[canv->base.dsIdx[i]].fmt = AfxGetRasterFormat(ras);

                                    //canv->base.ownershipMask |= AfxGetBitOffset(canv->base.colorCnt + i);
                                }
                            }
                        }
                    }
                    else
                    {
                        fmt = blueprint->ds[i].fmt;

                        if (fmt)
                        {
                            //AfxAssert(!(blueprint->ds[i].usage & afxRasterFlag_DRAW));
                            afxRasterFlags usage = blueprint->ds[i].usage;
                            usage |= afxRasterFlag_DRAW;
                            afxRasterInfo texi = { 0 };
                            texi.whd[0] = canv->base.whd[0];
                            texi.whd[1] = canv->base.whd[1];
                            texi.whd[2] = canv->base.whd[2];
                            texi.layerCnt = 1;
                            texi.fmt = fmt;
                            texi.usage = usage;

                            if (AfxAcquireRasters(dctx, 1, &texi, &ras)) AfxThrowError();
                            else
                            {
                                AfxAssertObjects(1, &ras, afxFcc_RAS);
                                AfxAssert(AfxTestRaster(ras, afxRasterFlag_DRAW));

                                canv->base.dsIdx[i] = canv->base.colorCnt + i;
                                canv->base.surfaces[canv->base.dsIdx[i]].ras = ras;
                                canv->base.surfaces[canv->base.dsIdx[i]].usage = usage;
                                canv->base.surfaces[canv->base.dsIdx[i]].fmt = fmt;

                                canv->base.ownershipMask |= AfxGetBitOffset(canv->base.dsIdx[i]);
                            }
                        }
                    }
                }

                if (!err)
                {
                    //canv->base.vmt = NIL;
                    //canv->base.idd = NIL;

                    //if (dctxD->vmt->canv && dctxD->vmt->canv(canv)) AfxThrowError();
                    //else
                    {
                        //AfxAssert(canv->base.vmt);
                        canv->updFlags = SGL_UPD_FLAG_DEVICE_INST;
                        canv->glHandle = 0;

                        for (afxNat i = 0; i < canv->base.surfaceCnt; i++)
                        {
                            //AfxObjectInstallEventFilter(&canv->base.surfaces[i].surf->ras.obj, &canv->base.obj);
                        }
                    }
                }
            }

            if (err)
            {
                _AfxCanvDropAllSurfaces(canv);

                AfxDeallocate(mmu, canv->base.surfaces);
            }
        }
    }
#endif
    return err;
}

_SGL afxClassConfig _SglCanvClsConfig =
{
    .fcc = afxFcc_CANV,
    .name = "Canvas",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxCanvas)),
    .mmu = NIL,
    .ctor = (void*)_SglCanvCtor,
    .dtor = (void*)_SglCanvDtor
};
