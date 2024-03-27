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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"
#include "qwadro/draw/pipe/afxCanvas.h"
#include "qwadro/draw/afxDrawOutput.h"
#include "qwadro/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// FRAME BUFFER                                                               //
////////////////////////////////////////////////////////////////////////////////

_SGL void _SglBindFboAttachment(glVmt const* gl, GLenum glTarget, GLenum glAttachment, GLenum texTarget, GLuint texHandle, GLint level, GLuint layer, GLuint z)
{
    afxError err = AFX_ERR_NONE;

    switch (texTarget)
    {
    case GL_TEXTURE_2D:
    {
        gl->FramebufferTexture2D(glTarget, glAttachment, texTarget, texHandle, level); _SglThrowErrorOccuried();
        break;
    }
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
    {
        gl->FramebufferTexture3D(glTarget, glAttachment, texTarget, texHandle, level, z); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_2D_ARRAY:
    {
        gl->FramebufferTexture3D(glTarget, glAttachment, texTarget, texHandle, level, layer); _SglThrowErrorOccuried();
        break;
    }
    case GL_TEXTURE_CUBE_MAP:
    default:
        gl->FramebufferTexture(glTarget, glAttachment, texHandle, level); _SglThrowErrorOccuried();
        break;
    };
}

_SGL afxError _SglDpuBindAndSyncCanv(sglDpuIdd* dpu, afxBool bind, afxBool sync, GLenum glTarget, afxCanvas canv)
{
    //AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxBool bound = (canv == dpu->activeRasterState.pass.canv);

    if (!canv)
    {
        if (bind && !bound)
        {
            gl->BindFramebuffer(glTarget, 0); _SglThrowErrorOccuried();
            dpu->activeRasterState.pass.canv = NIL;
        }
    }
    else
    {
        if (bind || sync)
        {
            AfxAssertObjects(1, &canv, afxFcc_CANV);
            GLuint glHandle = canv->glHandle;
            sglUpdateFlags devUpdReq = (canv->updFlags & SGL_UPD_FLAG_DEVICE);
            afxNat surCnt = AfxCountSurfaces(canv);
            afxRaster surfaces[_SGL_MAX_SURF_PER_CANV];
            
            if ((!glHandle) || (devUpdReq & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (glHandle)
                {
                    AfxAssert(gl->IsFramebuffer(glHandle));
                    gl->DeleteFramebuffers(1, &(glHandle)); _SglThrowErrorOccuried();
                    canv->glHandle = NIL;
                    glHandle = NIL;
                }
                gl->GenFramebuffers(1, &(glHandle)); _SglThrowErrorOccuried();
                gl->BindFramebuffer(glTarget, glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsFramebuffer(glHandle));
                canv->glHandle = glHandle;
                AfxAssert(!bound);
                bound = TRUE;
                dpu->activeRasterState.pass.canv = canv;

                if (surCnt)
                {
                    AfxAssert(_SGL_MAX_SURF_PER_CANV >= surCnt);
                    AfxGetDrawBuffers(canv, 0, surCnt, surfaces);

                    afxNat dsSurIdx[2];
                    AfxGetDepthSurface(canv, &dsSurIdx[0]);
                    afxBool combinedDs = (AfxGetStencilSurface(canv, &dsSurIdx[1]) && (dsSurIdx[1] == dsSurIdx[0]));

                    afxNat colorIdx = 0;
                    GLenum glAttachment;
                    GLuint glTexHandle;
                    GLenum glTexTarget;

                    for (afxNat i = 0; i < surCnt; i++)
                    {
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
                            glAttachment = GL_COLOR_ATTACHMENT0 + colorIdx;
                            ++colorIdx;
                        }

                        afxRaster ras = surfaces[i];

                        if (!ras)
                        {
                            AfxThrowError();
                            glTexHandle = 0;
                            glTexTarget = GL_TEXTURE_2D;
                        }
                        else
                        {
                            AfxAssertObjects(1, &ras, afxFcc_RAS);
                            AfxAssert(AfxTestRasterUsage(ras, afxRasterUsage_DRAW));

                            _SglBindAndSyncRas(dpu, sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);
                            glTexHandle = ras->glHandle;
                            glTexTarget = ras->glTarget;
                            AfxAssert(gl->IsTexture(glTexHandle));
                        }
                        _SglBindFboAttachment(&dpu->gl, glTarget, glAttachment, glTexTarget, glTexHandle, 0, 0, 0);
                    }
                }

                switch (gl->CheckFramebufferStatus(glTarget))
                {
                case GL_FRAMEBUFFER_COMPLETE:
                    canv->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                    AfxEcho("afxCanvas %p hardware-side data instanced.", canv);
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    AfxError("Not all framebuffer attachment points are framebuffer attachment complete.");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    AfxError("No images are attached to the framebuffer.");
                    break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    AfxError("The combination of internal formats of the attached images violates an implementation-dependent set of restrictions.");
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
#if !0
                if (!bind)
                {
                    gl->BindFramebuffer(glTarget, 0); _SglThrowErrorOccuried();
                    bound = FALSE;
                    dpu->activeRasterState.pass.canv = NIL;
                }
#endif
            }
            else
            {
                if (bind && !bound)
                {
                    gl->BindFramebuffer(glTarget, glHandle); _SglThrowErrorOccuried();
                    bound = TRUE;
                    dpu->activeRasterState.pass.canv = canv;
                }

                if (sync)
                {
                    if (surCnt)
                    {
                        AfxAssert(_SGL_MAX_SURF_PER_CANV >= surCnt);
                        AfxGetDrawBuffers(canv, 0, surCnt, surfaces);

                        for (afxNat i = 0; i < surCnt; i++)
                        {
                            afxRaster ras = surfaces[i];

                            if (ras)
                            {
                                AfxAssertObjects(1, &ras, afxFcc_RAS);
                                _SglBindAndSyncRas(dpu, sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);
                            }
                        }
                    }
                }
            }
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
            if (!ras)
            {
                canv->base.surfaces[surfIdx].managed = TRUE;
            }
            else
            {
                AfxAssertObjects(1, &ras, afxFcc_RAS);

                if (!AfxTestRasterUsage(ras, afxRasterUsage_DRAW)) AfxThrowError();
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

_SGL afxError _SglReadjustCanvasCb(afxCanvas canv, afxWhd const whd)
{
	afxError err = AFX_ERR_NONE;
	AfxAssertObjects(1, &canv, afxFcc_CANV);
	AfxAssert(whd);
	AfxAssert(whd[0]);
	AfxAssert(whd[1]);
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

    if ((whd[0] > minWhd[0]) || (whd[1] > minWhd[1]))
        AfxThrowError();

    if ((whd[0] != minWhd[0]) || (whd[1] != minWhd[1]))
    {
        canv->base.wh[0] = whd[0] < minWhd[0] ? whd[0] : minWhd[0];
        canv->base.wh[1] = whd[1] < minWhd[1] ? whd[1] : minWhd[1];
        canv->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
    }
	return err;
}

_SGL afxError _AfxCanvDropAllSurfaces(afxCanvas canv)
{
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
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);

    afxDrawContext dctx = AfxGetObjectProvider(canv);
    afxMmu mmu = AfxGetDrawContextMmu(dctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    _AfxCanvDropAllSurfaces(canv);

    if (canv->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 3, (void*)canv->glHandle);
        canv->glHandle = 0;
    }
    //AfxAssert(!canv->idd);

    AfxDeallocate(canv->base.surfaces);

    return err;
}

_SGL afxError _SglCanvCtor(afxCanvas canv, afxCookie const* cookie)
{
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
    canv->storeBitmask = NIL;

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

    if (!(canv->base.surfaces = AfxAllocate(surfaceCnt, sizeof(canv->base.surfaces[0]), 0, AfxHint()))) AfxThrowError();
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
            surf->usage = surCfgs[i].rasUsage | afxRasterUsage_DRAW;
            surf->flags = surCfgs[i].rasFlags;
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
            surf->usage = surCfgs[stencilInIdx].rasUsage | afxRasterUsage_DRAW;
            surf->flags = surCfgs[stencilInIdx].rasFlags;
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
            surf->usage = surCfgs[depthInIdx].rasUsage | afxRasterUsage_DRAW;
            surf->flags = surCfgs[depthInIdx].rasFlags;
            surf->sampleCnt = surCfgs[depthInIdx].sampleCnt;
        }
    }
    return err;
}

_SGL afxClassConfig const _SglCanvClsConfig =
{
    .fcc = afxFcc_CANV,
    .name = "Canvas",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxCanvas)),
    .mmu = NIL,
    .ctor = (void*)_SglCanvCtor,
    .dtor = (void*)_SglCanvDtor
};
