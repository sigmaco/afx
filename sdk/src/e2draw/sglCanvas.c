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
#include "afx/draw/afxCanvas.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// FRAME BUFFER                                                               //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglCanvReinstantiateIdd(sglDpuIdd* dpu, afxCanvas canv, GLenum target, glVmt const* gl)
{
    AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;

    if (canv->glHandle)
    {
        gl->DeleteFramebuffers(1, &(canv->glHandle)); _SglThrowErrorOccuried();
        canv->glHandle = NIL;
    }
    canv->updFlags = SGL_UPD_FLAG_DEVICE_INST;
    

    if (!err && !canv->glHandle)
    {
        gl->GenFramebuffers(1, &(canv->glHandle)); _SglThrowErrorOccuried();
        gl->BindFramebuffer(target, canv->glHandle); _SglThrowErrorOccuried();
        AfxAssert(gl->IsFramebuffer(canv->glHandle));
        afxNat const surfCnt = AfxGetAnnexedSurfaceCount(canv);
        afxNat rasterIdx = 0;

        for (afxNat i = 0; i < surfCnt; i++)
        {
            afxTexture surf;
            AfxGetAnnexedSurface(canv, i, &surf);
            AfxAssertObjects(1, &surf, afxFcc_TEX);

            if (!surf) AfxThrowError();
            else
            {
                afxPixelFormat pf = AfxGetTextureFormat(surf);
                GLenum glAttachment = 0;

                if (!AfxTestTexture(surf, afxTextureFlag_DRAW)) AfxThrowError();
                else
                {
                    if (AfxPixelFormatIsCombinedDepthStencil(pf))
                    {
                        glAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
                    }
                    else if (AfxPixelFormatIsDepth(pf))
                    {
                        glAttachment = GL_DEPTH_ATTACHMENT;
                    }
                    else if (AfxPixelFormatIsStencil(pf))
                    {
                        glAttachment = GL_STENCIL_ATTACHMENT;
                    }
                    else
                    {
                        glAttachment = GL_COLOR_ATTACHMENT0 + rasterIdx;
                        ++rasterIdx;
                    }
                }
                
                _SglDpuBindAndSyncTex(dpu, 0, surf, gl);
                _SglDpuBindAndSyncTex(dpu, 0, 0, gl);
                
                {
                    AfxAssert(surf->glHandle);
                    AfxAssert(gl->IsTexture(surf->glHandle));

#if 0
                    if (!surfIdd) AfxThrowError();
                    else
                    {
                        AfxAssert(surfIdd->glHandle);
                        AfxAssert(gl->IsRenderBuffer(surfIdd->glHandle));
                        gl->FramebufferRenderbuffer(target, glAttachment, GL_RENDERBUFFER, surfIdd->glHandle); _SglThrowErrorOccuried();
                    }
#endif

                    switch (surf->glTarget)
                    {
#if 0
                    case GL_TEXTURE_CUBE_MAP:
                    {
                        AfxAssert(isCubemap);
                        AfxAssert(extent[2] == 6);
                        gl->FramebufferTexture(target, glAttachment, surf->tex.glHandle, 0); _SglThrowErrorOccuried();
                        break;
                    }
                    case GL_TEXTURE_3D:
                    {
                        gl->FramebufferTexture3D(target, glAttachment, surf->tex.glTarget, surf->tex.glHandle, 0, 0); _SglThrowErrorOccuried();
                        break;
                    }
                    case GL_TEXTURE_2D_ARRAY:
                    {
                        AfxAssert(isLayered);
                        gl->FramebufferTexture3D(target, glAttachment, surf->tex.glTarget, surf->tex.glHandle, 0, 0); _SglThrowErrorOccuried();
                        break;
                    }
                    case GL_TEXTURE_2D:
                    {
                        gl->FramebufferTexture2D(target, glAttachment, surf->tex.glTarget, surf->tex.glHandle, 0); _SglThrowErrorOccuried();
                        break;
                    }
                    case GL_TEXTURE_1D_ARRAY:
                    {
                        AfxAssert(isLayered);
                        gl->FramebufferTexture2D(target, glAttachment, surf->tex.glTarget, surf->tex.glHandle, 0); _SglThrowErrorOccuried();
                        break;
                    }
                    case GL_TEXTURE_1D:
                    {
                        gl->FramebufferTexture1D(target, glAttachment, surf->tex.glTarget, surf->tex.glHandle, 0); _SglThrowErrorOccuried();
                        break;
                    }
#endif
                    default:
                        gl->FramebufferTexture(target, glAttachment, surf->glHandle, 0); _SglThrowErrorOccuried();
                        break;
                    };
                }
            }
        }

        switch (gl->CheckFramebufferStatus(target))
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
    return err;
}

_SGL afxError _SglDpuBindAndSyncCanv(sglDpuIdd* dpu, afxCanvas canv, GLenum target, glVmt const* gl)
{
    //AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    (void)dpu;

    if (canv)
    {
        AfxAssertObjects(1, &canv, afxFcc_CANV);

        if (canv->updFlags & SGL_UPD_FLAG_DEVICE_INST || AfxTestCanvas(canv, AfxGetBitOffset(AFX_CANV_FLAG_REVALIDATE)))
        {
            if (_SglCanvReinstantiateIdd(dpu, canv, target, gl))
                AfxThrowError();

            if (!err)
                AfxUnflagCanvas(canv, AfxGetBitOffset(AFX_CANV_FLAG_REVALIDATE));
        }

        {
            {
                AfxAssert(canv->glHandle);
                gl->BindFramebuffer(target, canv->glHandle); _SglThrowErrorOccuried();
            }

            afxNat rasterIdx = 0;

            for (afxNat i = 0; i < AfxGetAnnexedSurfaceCount(canv); i++)
            {
                afxTexture surf;
                AfxGetAnnexedSurface(canv, i, &surf);
                AfxAssertObjects(1, &surf, afxFcc_TEX);

                if (!surf) AfxThrowError();
                else
                {
                    _SglDpuBindAndSyncTex(dpu, 0, surf, gl);
                    _SglDpuBindAndSyncTex(dpu, 0, 0, gl);
                    //_SglDquSurfSync(dpu, surf, gl);
                }
            }
        }
    }
    else
    {
        ///if (dqueIdd->state.renderPass.canv)
        {
            gl->BindFramebuffer(target, 0); _SglThrowErrorOccuried();
        }
    }
    return err;
}

_SGL afxError _AfxReadjustCanvas(afxCanvas canv, afxWhd const from, afxWhd const extent)
{
	afxError err = AFX_ERR_NONE;
	AfxAssertObjects(1, &canv, afxFcc_CANV);
	AfxAssert(extent);
	AfxAssert(extent[0]);
	AfxAssert(extent[1]);
	AfxAssert(extent[2]);
    AfxAssert(extent[0] != canv->base.whd[0] || extent[1] != canv->base.whd[1] || extent[2] != canv->base.whd[2]);

	if (canv->base.whd[0] != extent[0] || canv->base.whd[1] != extent[1] || canv->base.whd[2] != extent[2])
	{
        if (canv)
            canv->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
	}
	return err;
}

_SGL afxError _AfxCanvDropAllSurfaces(afxCanvas canv)
{
	AfxEntry("canv=%p", canv);
	afxError err = AFX_ERR_NONE;
	AfxAssertObjects(1, &canv, afxFcc_CANV);

	for (afxNat i = 0; i < canv->base.annexCnt; i++)
	{
		afxTexture tex = canv->base.annexes[i].tex;

        if (tex)
        {
            AfxAssertObjects(1, &tex, afxFcc_TEX);
            AfxReleaseObjects(1, (void*[]) { tex });
            canv->base.annexes[i].tex = NIL;
        }
	}

	//canv->base.rasterCnt = 0;
    //canv->base.surfCnt = 0;
	return err;
}

_SGL afxError _SglCanvDtor(afxCanvas canv)
{
    AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &canv, afxFcc_CANV);

    afxDrawContext dctx = AfxGetObjectProvider(canv);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    _AfxCanvDropAllSurfaces(canv);

    if (canv->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 3, canv->glHandle);
        canv->glHandle = 0;
    }
    //AfxAssert(!canv->idd);

    AfxDeallocate(mem, canv->base.annexes);

    return err;
}

_SGL afxError _SglCanvCtor(afxCanvas canv, afxCookie const* cookie)
{
    AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;

    afxCanvasBlueprint const *blueprint = ((afxCanvasBlueprint const *)cookie->udd[0]) + cookie->no;
    AfxAssert(blueprint);

    afxDrawContext dctx = AfxGetObjectProvider(canv);
    afxContext ctx = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &ctx, afxFcc_CTX);

    canv->base.udd[0] = blueprint->udd[0];
    canv->base.udd[1] = blueprint->udd[1];

    canv->base.whd[0] = blueprint->extent[0];
    canv->base.whd[1] = blueprint->extent[1];
    canv->base.whd[2] = blueprint->extent[2];

    afxWhd whd;
    afxTexture tex;
    afxPixelFormat fmt;

    if (0 == (canv->base.whd[0] * canv->base.whd[1] * canv->base.whd[2]))
    {
        for (afxNat i = 0; i < blueprint->rasterCnt; i++)
        {
            if ((tex = blueprint->rasters[i].tex))
            {
                AfxAssertObjects(1, &tex, afxFcc_TEX);

                AfxGetTextureExtent(tex, 0, whd);

                canv->base.whd[0] = AfxMini(canv->base.whd[0], whd[0]);
                canv->base.whd[1] = AfxMini(canv->base.whd[1], whd[1]);
                canv->base.whd[2] = AfxMini(canv->base.whd[2], whd[2]);
            }
        }

        for (afxNat i = 0; i < sizeof(blueprint->ds) / sizeof(blueprint->ds[0]); i++)
        {
            if ((tex = blueprint->ds[i].tex))
            {
                AfxAssertObjects(1, &tex, afxFcc_TEX);

                AfxGetTextureExtent(tex, 0, whd);

                canv->base.whd[0] = AfxMini(canv->base.whd[0], whd[0]);
                canv->base.whd[1] = AfxMini(canv->base.whd[1], whd[1]);
                canv->base.whd[2] = AfxMini(canv->base.whd[2], whd[2]);
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

        if (depthFmt == AFX_PFD_D24S8 && (!stencilFmt))
            depthFmt = AFX_PFD_D24;

        if ((!depthFmt) && stencilFmt == AFX_PFD_D24S8)
            depthFmt = AFX_PFD_S8;

#ifndef _AFX_OPTIMIZATION_CANV_DS_FMT_COMBO

        if ((depthFmt == AFX_PFD_D24 && stencilFmt == AFX_PFD_S8))
            depthFmt = (stencilFmt = AFX_PFD_D24S8);
#endif
#endif
        combinedDs = (depthFmt && stencilFmt && depthFmt == stencilFmt);

        canv->base.annexCnt = blueprint->rasterCnt;

        if (depthFmt || stencilFmt)
        {
            if (combinedDs) ++canv->base.annexCnt;
            else
            {
                if (depthFmt)
                    ++canv->base.annexCnt;

                if (stencilFmt)
                    ++canv->base.annexCnt;
            }
        }

        canv->base.rasterCnt = 0;
        canv->base.ownershipMask = NIL;

        if (!(canv->base.annexes = AfxAllocate(ctx, canv->base.annexCnt * sizeof(canv->base.annexes[0]), 0, AfxSpawnHint()))) AfxThrowError();
        else
        {
            afxTexture tex;

            for (afxNat i = 0; i < canv->base.annexCnt; i++)
                AfxZero(&canv->base.annexes[i], sizeof(canv->base.annexes[0]));

            for (afxNat i = 0; i < blueprint->rasterCnt; i++)
            {
                if ((tex = blueprint->rasters[i].tex))
                {
                    if (!AfxTestTexture(tex, afxTextureFlag_DRAW)) AfxThrowError();
                    else
                    {
                        AfxGetTextureExtent(tex, 0, whd);

                        if (canv->base.whd[0] > whd[0] || canv->base.whd[1] > whd[1] || canv->base.whd[2] > whd[2]) AfxThrowError();
                        else
                        {
                            if (AfxReacquireObjects(1, (void*[]) { tex })) AfxThrowError();
                            else
                            {
                                canv->base.annexes[i].tex = tex;
                                canv->base.annexes[i].usage = AfxTestTexture(tex, AFX_N32_MAX);
                                canv->base.annexes[i].fmt = AfxGetTextureFormat(tex);
                                ++canv->base.rasterCnt;
                                //canv->base.ownershipMask |= AfxGetBitOffset(i);
                            }
                        }
                    }
                }
                else
                {
                    fmt = blueprint->rasters[i].fmt;

                    if (!fmt) AfxThrowError();
                    else
                    {
                        //AfxAssert(!(blueprint->rasters[i].usage & afxTextureFlag_DRAW));
                        afxTextureFlags usage = blueprint->rasters[i].usage;
                        usage |= afxTextureFlag_DRAW;
                        afxTextureInfo texi = { 0 };
                        texi.whd[0] = canv->base.whd[0];
                        texi.whd[1] = canv->base.whd[1];
                        texi.whd[2] = canv->base.whd[2];
                        texi.imgCnt = 1;
                        texi.fmt = fmt;
                        texi.usage = usage;

                        if (AfxAcquireTextures(dctx, 1, &texi, &tex)) AfxThrowError();
                        else
                        {
                            AfxAssertObjects(1, &tex, afxFcc_TEX);
                            AfxAssert(AfxTestTexture(tex, afxTextureFlag_DRAW));
                            canv->base.annexes[i].tex = tex;
                            canv->base.annexes[i].usage = usage;
                            canv->base.annexes[i].fmt = fmt;
                            canv->base.ownershipMask |= AfxGetBitOffset(i);
                            ++canv->base.rasterCnt;
                        }
                    }
                }
            }

            if (!err)
            {
                for (afxNat i = 0; i < sizeof(blueprint->ds) / sizeof(blueprint->ds[0]); i++)
                {
                    if ((tex = blueprint->ds[i].tex))
                    {
                        if (!AfxTestTexture(tex, afxTextureFlag_DRAW)) AfxThrowError();
                        else
                        {
                            AfxGetTextureExtent(tex, 0, whd);

                            if (canv->base.whd[0] > whd[0] || canv->base.whd[1] > whd[1] || canv->base.whd[2] > whd[2]) AfxThrowError();
                            else
                            {
                                if (!(AfxReacquireObjects(1, (void*[]) { tex }))) AfxThrowError();
                                else
                                {
                                    canv->base.dsIdx[i] = canv->base.rasterCnt + i;
                                    canv->base.annexes[canv->base.dsIdx[i]].tex = tex;
                                    canv->base.annexes[canv->base.dsIdx[i]].usage = AfxTestTexture(tex, AFX_N32_MAX);
                                    canv->base.annexes[canv->base.dsIdx[i]].fmt = AfxGetTextureFormat(tex);

                                    //canv->base.ownershipMask |= AfxGetBitOffset(canv->base.rasterCnt + i);
                                }
                            }
                        }
                    }
                    else
                    {
                        fmt = blueprint->ds[i].fmt;

                        if (fmt)
                        {
                            //AfxAssert(!(blueprint->ds[i].usage & afxTextureFlag_DRAW));
                            afxTextureFlags usage = blueprint->ds[i].usage;
                            usage |= afxTextureFlag_DRAW;
                            afxTextureInfo texi = { 0 };
                            texi.whd[0] = canv->base.whd[0];
                            texi.whd[1] = canv->base.whd[1];
                            texi.whd[2] = canv->base.whd[2];
                            texi.imgCnt = 1;
                            texi.fmt = fmt;
                            texi.usage = usage;

                            if (AfxAcquireTextures(dctx, 1, &texi, &tex)) AfxThrowError();
                            else
                            {
                                AfxAssertObjects(1, &tex, afxFcc_TEX);
                                AfxAssert(AfxTestTexture(tex, afxTextureFlag_DRAW));

                                canv->base.dsIdx[i] = canv->base.rasterCnt + i;
                                canv->base.annexes[canv->base.dsIdx[i]].tex = tex;
                                canv->base.annexes[canv->base.dsIdx[i]].usage = usage;
                                canv->base.annexes[canv->base.dsIdx[i]].fmt = fmt;

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

                        for (afxNat i = 0; i < canv->base.annexCnt; i++)
                        {
                            //AfxObjectInstallEventFilter(&canv->base.annexes[i].surf->tex.obj, &canv->base.obj);
                        }
                    }
                }
            }

            if (err)
            {
                _AfxCanvDropAllSurfaces(canv);

                AfxDeallocate(ctx, canv->base.annexes);
            }
        }
    }
    return err;
}

_SGL afxClassConfig _SglCanvClsConfig =
{
    .fcc = afxFcc_CANV,
    .name = "Canvas",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxCanvas)),
    .ctx = NIL,
    .ctor = (void*)_SglCanvCtor,
    .dtor = (void*)_SglCanvDtor
};
