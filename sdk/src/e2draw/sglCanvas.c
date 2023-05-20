/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#include "sgl.h"
#include "afx/draw/res/afxCanvas.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/draw/afxDrawSystem.h"

typedef struct
{
	afxDrawContext dctx;
    afxCanvasBlueprint const *blueprint;
} _afxCanvCtorArgs;

////////////////////////////////////////////////////////////////////////////////
// FRAME BUFFER                                                               //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglCanvDtor(afxCanvas canv, afxDrawQueue dque)
{
    afxError err = NIL;
    AfxEntry("canv=%p", canv);
    AfxAssertObject(canv, AFX_FCC_CANV);
    afxDrawContext dctx = AfxObjectGetProvider(&canv->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    sglVmt const* gl = &(dque->wglVmt);

    if (canv->glHandle[dque->queueIdx])
    {
        gl->DeleteFramebuffers(1, &(canv->glHandle[dque->queueIdx])); _SglThrowErrorOccuried();
        canv->glHandle[dque->queueIdx] = NIL;
    }
    return err;
}

_SGL afxError _SglDqueBindAndSyncCanv(afxDrawQueue dque, afxCanvas canv, GLenum target)
{
    //AfxEntry("canv=%p", canv);
    afxError err = NIL;
    sglVmt const* gl = &dque->wglVmt;

    if (canv)
    {
        AfxAssertObject(canv, AFX_FCC_CANV);

        GLuint glHandle = canv->glHandle[dque->queueIdx];

        if ((canv->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((canv->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (glHandle)
                {
                    gl->DeleteFramebuffers(1, &glHandle); _SglThrowErrorOccuried();
                    glHandle = NIL;
                }

                AfxAssert(NIL == glHandle);
                gl->GenFramebuffers(1, &(glHandle)); _SglThrowErrorOccuried();
                gl->BindFramebuffer(target, glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsFramebuffer(glHandle));
                afxNat const surfCnt = AfxCanvasGetSurfaceCount(canv);
                afxNat rasterIdx = 0;

                for (afxNat i = 0; i < surfCnt; i++)
                {
                    afxSurface surf = AfxCanvasGetSurface(canv, i);
                    AfxAssertObject(surf, AFX_FCC_SURF);
                    GLenum glAttachment;

                    if (AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_RASTER))
                    {
                        glAttachment = GL_COLOR_ATTACHMENT0 + rasterIdx;
                        ++rasterIdx;
                    }
                    else if (AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH))
                    {
                        if (canv->depthIdx == i)
                        {
                            glAttachment = (canv->stencilIdx == i) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
                        }
                        else if (canv->stencilIdx == i)
                        {
                            glAttachment = GL_STENCIL_ATTACHMENT;
                        }
                        else glAttachment = 0;
                    }
                    else glAttachment = 0;

                    _SglDqueBindAndSyncTex(dque, 0, &surf->tex);
                    _SglDqueBindAndSyncTex(dque, 0, 0);
                    AfxAssert(surf->tex.glHandle);
                    AfxAssert(gl->IsTexture(surf->tex.glHandle));

                    _SglDqueSurfSync(dque, surf);
                    AfxAssert(surf->glHandle);
                    AfxAssert(gl->IsRenderBuffer(surf->glHandle));
                    gl->FramebufferRenderbuffer(target, glAttachment, GL_RENDERBUFFER, surf->glHandle); _SglThrowErrorOccuried();
                    
                    switch (surf->tex.glTarget)
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
                        gl->FramebufferTexture(target, glAttachment, surf->tex.glHandle, 0); _SglThrowErrorOccuried();
                        break;
                    };
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

                canv->glHandle[dque->queueIdx] = glHandle;
            }
            else if ((canv->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(glHandle);
                AfxThrowError(); // can't be modified
            }
        }
        else
        {
            AfxAssert(glHandle);
            gl->BindFramebuffer(target, glHandle); _SglThrowErrorOccuried();
        }
    }
    else
    {
        if (dque->state.renderPass.canv)
        {
            gl->BindFramebuffer(target, 0); _SglThrowErrorOccuried();
        }
    }
    return err;
}

_SGL afxSurface _AfxCanvasGetStencil(afxCanvas canv)
{
    afxError err = NIL;
    AfxAssertObject(canv, AFX_FCC_CANV);
    afxSurface surf = canv->stencilIdx != AFX_INVALID_INDEX ? canv->surfaces[canv->stencilIdx] : NIL;
    AfxAssertObject(surf, AFX_FCC_SURF);
    return surf;
}

_SGL afxSurface _AfxCanvasGetDepth(afxCanvas canv)
{
    afxError err = NIL;
    AfxAssertObject(canv, AFX_FCC_CANV);
    afxSurface surf = canv->depthIdx != AFX_INVALID_INDEX ? canv->surfaces[canv->depthIdx] : NIL;
    AfxAssertObject(surf, AFX_FCC_SURF);
    return surf;
}

_SGL afxSurface _AfxCanvasGetRaster(afxCanvas canv, afxNat rasIdx)
{
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);
	AfxAssert(rasIdx < canv->rasterCnt);
	afxSurface surf = canv->surfaces[rasIdx];
	AfxAssertObject(surf, AFX_FCC_SURF);
	return surf;
}

_SGL afxSurface _AfxCanvasGetSurface(afxCanvas canv, afxNat surfIdx)
{
    afxError err = NIL;
    AfxAssertObject(canv, AFX_FCC_CANV);
    AfxAssert(surfIdx < canv->surfCnt);
    afxSurface surf = canv->surfaces[surfIdx];
    AfxAssertObject(surf, AFX_FCC_SURF);
    return surf;
}

_SGL afxNat _AfxCanvasGetRasterCount(afxCanvas canv)
{
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);
	return canv->rasterCnt;
}

_SGL afxNat _AfxCanvasGetSurfaceCount(afxCanvas canv)
{
    afxError err = NIL;
    AfxAssertObject(canv, AFX_FCC_CANV);
    return canv->surfCnt;
}

_SGL afxError _AfxCanvasSetExtent(afxCanvas canv, afxWhd const extent)
{
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);
	AfxAssert(extent);
	AfxAssert(extent[0]);
	AfxAssert(extent[1]);
	AfxAssert(extent[2]);
    AfxAssert(extent[0] != canv->whd[0] || extent[1] != canv->whd[1] || extent[2] != canv->whd[2]);

	if (canv->whd[0] != extent[0] || canv->whd[1] != extent[1] || canv->whd[2] != extent[2])
	{
		canv->whd[0] = extent[0];
		canv->whd[1] = extent[1];
		canv->whd[2] = extent[2];

        afxSurface surf;

		for (afxNat i = 0; i < canv->surfCnt; i++)
		{
			surf = canv->surfaces[i];
			AfxAssertObject(surf, AFX_FCC_SURF);

			if (AfxTextureSetExtent(&surf->tex, surf->tex.layerCnt, canv->whd))
				AfxThrowError();
		}

        canv->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
	}
	return err;
}

_SGL afxNat* _AfxCanvasGetExtent(afxCanvas canv, afxWhd extent)
{
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);
	AfxAssert(extent);
    extent[0] = canv->whd[0];
    extent[1] = canv->whd[1];
    extent[2] = canv->whd[2];
	return extent;
}

_SGL afxResult _AfxDrawContextBuildCanvases(afxDrawContext dctx, afxNat cnt, afxCanvasBlueprint const blueprint[], afxCanvas canv[])
{
	afxError err = NIL;
	AfxAssertObject(dctx, AFX_FCC_DCTX);
	AfxAssert(cnt);
	AfxAssert(blueprint);
	AfxAssert(canv);
	AfxEntry("dctx=%p,cnt=%u,blueprint=%p,canv=%p", dctx, cnt, blueprint, canv);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        _afxCanvCtorArgs args =
        {
            dctx,
            &blueprint[i]
        };

        if (!(canv[i] = AfxObjectAcquire(AfxDrawContextGetCanvasClass(dctx), &args, AfxSpawnHint()))) AfxThrowError();
        else
        {
            AfxAssertObject(canv[i], AFX_FCC_CANV);
            rslt++;
        }
    }
	return rslt;
};

_SGL void _AfxCanvTexHandler(afxObject *obj, afxEvent *ev)
{
	afxError err = NIL;
	afxCanvas canv = (void*)obj;
	AfxAssertObject(canv, AFX_FCC_CANV);
	AfxAssertObject(ev->obj, AFX_FCC_TEX);
	//AfxAssertConnection(&dout->din);
	//AfxAssert(AfxConnectionGetObject(&dout->din) == ev->obj);

	if (ev->type == AFX_EVENT_OBJ_DESTROYED)
	{
		AfxError("Texture %p attached to draw buffer %p was destroyed.", ev->obj, obj);
		while (0 < AfxObjectRelease(&canv->obj));
	}
}

_SGL afxError _AfxCanvDropAllSurfaces(afxCanvas canv)
{
	AfxEntry("canv=%p", canv);
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);

	for (afxNat i = 0; i < canv->surfCnt; i++)
	{
		afxSurface surf = canv->surfaces[i];
		AfxTryAssertObject(surf, AFX_FCC_SURF);
		AfxObjectRelease(&surf->tex.res.obj);
	}

	canv->rasterCnt = 0;
    canv->surfCnt = 0;
	return err;
}

_SGL afxCanvImpl const _AfxStdCanvImpl;
afxCanvImpl const _AfxStdCanvImpl =
{
    _AfxCanvasGetExtent,
    _AfxCanvasSetExtent,
    _AfxCanvasGetSurfaceCount,
    _AfxCanvasGetSurface,
    _AfxCanvasGetRasterCount,
    _AfxCanvasGetRaster,
    _AfxCanvasGetDepth,
    _AfxCanvasGetStencil,
};

_SGL afxError _AfxCanvDtor(afxCanvas canv)
{
	AfxEntry("canv=%p", canv);
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);

    afxDrawContext dctx = AfxCanvasGetContext(canv);

    for (afxNat i = 0; i < dctx->queueCnt; i++)
    {
        if (canv->glHandle[i])
        {
            _SglEnqueueGlResourceDeletion(dctx, i, 3, canv->glHandle[i]);
            canv->glHandle[i] = 0;
        }
    }

	_AfxCanvDropAllSurfaces(canv);

	return err;
}

_SGL afxError _AfxCanvCtor(afxCanvas canv, _afxCanvCtorArgs *args)
{
	AfxEntry("canv=%p", canv);
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);
    afxCanvasBlueprint const *blueprint = args->blueprint;

	AfxAssert(blueprint->extent[0]);
	AfxAssert(blueprint->extent[1]);
	AfxAssert(blueprint->extent[2]);
	canv->whd[0] = blueprint->extent[0];
	canv->whd[1] = blueprint->extent[1];
	canv->whd[2] = blueprint->extent[2];

	AfxAssert(blueprint->rasterCnt);
	afxNat rasterCnt = blueprint->rasterCnt;
	AfxAssert(rasterCnt <= _SGL_MAX_RASTER_SURF_PER_CANV);
	//afxSurfaceSpecification const *specs = args->specs;
	//AfxAssert(specs);

    canv->surfCnt = 0;
    canv->rasterCnt = 0;

	for (afxNat i = 0; i < rasterCnt; i++)
	{
        afxSurface raster;

		if ((raster = blueprint->rasterExistingObj[i]))
		{
            AfxAssert(raster->tex.whd[0] >= canv->whd[0]);
            AfxAssert(raster->tex.whd[1] >= canv->whd[1]);
            AfxAssert(raster->tex.whd[2] >= canv->whd[2]);

			if (!(AfxObjectReacquire(&(blueprint->rasterExistingObj[i]->tex.res.obj), &canv->obj, NIL, NIL, NIL)))
                AfxThrowError();
		}
		else
		{
			AfxAssert(blueprint->rasterFmt[i]);
			afxTextureUsage usage = blueprint->rasterUsage[i] & ~(AFX_TEX_USAGE_SURFACE);
            usage |= AFX_TEX_USAGE_SURFACE_RASTER;

			if (!(raster = AfxDrawContextAcquireSurface(args->dctx, blueprint->rasterFmt[i], canv->whd, usage)))
                AfxThrowError();
		}

        if (err) break;
        else
        {
            if (raster)
            {
                AfxAssertObject(raster, AFX_FCC_SURF);
                AfxAssert(AfxTextureTestUsageFlags(&(raster->tex), AFX_TEX_USAGE_SURFACE_RASTER));

                canv->surfaces[canv->surfCnt] = raster;
                canv->surfCnt++;
                canv->rasterCnt++;
            }
        }
	}

    if (!err)
    {
        afxPixelFormat depthFmt = blueprint->depthFmt, stencilFmt = blueprint->stencilFmt;

        if ((depthFmt == AFX_PIXEL_FMT_D24 && stencilFmt == AFX_PIXEL_FMT_S8) || (depthFmt == AFX_PIXEL_FMT_D24S8 && !stencilFmt))
        {
            depthFmt = AFX_PIXEL_FMT_D24S8;
            stencilFmt = AFX_PIXEL_FMT_D24S8;
        }

        if (!depthFmt) canv->depthIdx = AFX_INVALID_INDEX;
        else
        {
            if (!(canv->surfaces[canv->surfCnt] = AfxDrawContextAcquireSurface(args->dctx, depthFmt, canv->whd, AFX_TEX_USAGE_SURFACE_DEPTH))) AfxThrowError();
            else
            {
                canv->depthIdx = canv->surfCnt;
                canv->surfCnt++;
            }
        }

        if (!stencilFmt) canv->stencilIdx = AFX_INVALID_INDEX;
        else
        {
            if (stencilFmt == depthFmt)
                canv->stencilIdx = canv->depthIdx;
            else if (!(canv->surfaces[canv->surfCnt] = AfxDrawContextAcquireSurface(args->dctx, stencilFmt, canv->whd, AFX_TEX_USAGE_SURFACE_DEPTH)))
                AfxThrowError();
            else
            {
                canv->stencilIdx = canv->surfCnt;
                canv->surfCnt++;
            }
        }

        if (!err)
        {
            AfxZero(canv->glHandle, sizeof(canv->glHandle));
            canv->updFlags = SGL_UPD_FLAG_DEVICE_INST;
        }
    }

	if (err && canv->rasterCnt)
	{
		_AfxCanvDropAllSurfaces(canv);
	}
	return err;
}

_SGL afxClassSpecification const _AfxCanvClassSpec;

afxClassSpecification const _AfxCanvClassSpec =
{
    AFX_FCC_CANV,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxCanvas)),
    NIL,
    (void*)_AfxCanvCtor,
    (void*)_AfxCanvDtor,
    "Canvas",
    &_AfxStdCanvImpl
};
