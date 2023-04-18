#include "sgl.h"
#include "afx/draw/res/afxCanvas.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/draw/afxDrawSystem.h"

typedef struct
{
	afxDrawContext dctx;
	afxNat const *whd;
	afxNat surfaceCnt;
	afxSurfaceSpecification const *specs;
} _afxCanvCtorArgs;

////////////////////////////////////////////////////////////////////////////////
// FRAME BUFFER                                                               //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _AfxStdCanvImplInit(afxCanvas canv, afxDrawEngine deng)
{
    afxError err = NIL;
    AfxAssertObject(canv, AFX_FCC_CANV);
    sglVmt const* gl = &deng->wglVmt;

    AfxAssert(0 == canv->gpuHandle[deng->queueIdx]);
    gl->GenFramebuffers(1, &(canv->gpuHandle[deng->queueIdx])); _SglThrowErrorOccuried();
    gl->BindFramebuffer(GL_FRAMEBUFFER, canv->gpuHandle[deng->queueIdx]); _SglThrowErrorOccuried();
    AfxAssert(gl->IsFramebuffer(canv->gpuHandle[deng->queueIdx]));
    afxNat const surCnt = AfxCanvasGetSurfaceCount(canv);
    afxNat rasterIdx = 0;

    for (afxNat i = 0; i < surCnt; i++)
    {
        afxSurface surf = AfxCanvasGetSurface(canv, i);
        AfxAssertObject(surf, AFX_FCC_SURF);

        GLenum glAttachment;
        afxBool isRaster = AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_RASTER);
        afxBool isDepth = AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_USAGE_SURFACE_DEPTH);

        if (isRaster)
        {
            glAttachment = GL_COLOR_ATTACHMENT0 + rasterIdx;
            ++rasterIdx;
        }
        else if (isDepth) glAttachment = GL_DEPTH_STENCIL_ATTACHMENT;
        else glAttachment = 0;

        _SglTexUpdate(&surf->tex, deng);
        AfxAssert(isRaster || isDepth && !(isRaster && isDepth)); // is render buffer
        AfxAssert(gl->IsTexture(surf->tex.gpuTexHandle));

        afxBool const isCubemap = AfxTextureTestUsageFlags(&surf->tex, AFX_TEX_CUBEMAP);
        afxBool const isLayered = surf->tex.layerCnt > 1;
        //gl->BindTexture(surf->tex.glTarget, surf->tex.gpuTexHandle); _SglThrowErrorOccuried();

        if (surf->gpuRboHandle)
        {
            AfxAssert(gl->IsRenderBuffer(surf->gpuRboHandle));
            //gl->BindRenderbuffer(GL_RENDERBUFFER, surf->gpuRboHandle); _SglThrowErrorOccuried();
        }

        afxWhd const extent = { surf->tex.whd[0], surf->tex.whd[1], surf->tex.whd[2] };
        AfxAssert(1 < extent[0]);
        
        switch (surf->tex.glTarget)
        {
        case GL_TEXTURE_CUBE_MAP:
        {
            AfxAssert(isCubemap);
            AfxAssert(extent[2] == 6);
            gl->FramebufferTexture(GL_FRAMEBUFFER, glAttachment, surf->tex.gpuTexHandle, 0); _SglThrowErrorOccuried();
            break;
        }
        case GL_TEXTURE_3D:
        {
            gl->FramebufferTexture3D(GL_FRAMEBUFFER, glAttachment, surf->tex.glTarget, surf->tex.gpuTexHandle, 0, 0); _SglThrowErrorOccuried();
            break;
        }
        case GL_TEXTURE_2D_ARRAY:
        {
            AfxAssert(isLayered);
            gl->FramebufferTexture3D(GL_FRAMEBUFFER, glAttachment, surf->tex.glTarget, surf->tex.gpuTexHandle, 0, 0); _SglThrowErrorOccuried();
            break;
        }
        case GL_TEXTURE_2D:
        {
            gl->FramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, surf->tex.glTarget, surf->tex.gpuTexHandle, 0); _SglThrowErrorOccuried();
            break;
        }
        case GL_TEXTURE_1D_ARRAY:
        {
            AfxAssert(isLayered);
            gl->FramebufferTexture2D(GL_FRAMEBUFFER, glAttachment, surf->tex.glTarget, surf->tex.gpuTexHandle, 0); _SglThrowErrorOccuried();
            break;
        }
        case GL_TEXTURE_1D:
        {
            gl->FramebufferTexture1D(GL_FRAMEBUFFER, glAttachment, surf->tex.glTarget, surf->tex.gpuTexHandle, 0); _SglThrowErrorOccuried();
            break;
        }
        default:
            AfxThrowError();
            break;
        };

        if (surf->gpuRboHandle)
        {
            gl->FramebufferRenderbuffer(GL_FRAMEBUFFER, glAttachment, GL_RENDERBUFFER, surf->gpuRboHandle); _SglThrowErrorOccuried();
        }
        //gl->BindRenderbuffer(GL_RENDERBUFFER, 0); _SglThrowErrorOccuried();
        //gl->BindTexture(surf->tex.glTarget, 0); _SglThrowErrorOccuried();
    }

    switch (gl->CheckFramebufferStatus(GL_FRAMEBUFFER))
    {
    case GL_FRAMEBUFFER_COMPLETE:
        AfxEcho("canv %p, reinstanced.", canv);
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

    gl->BindFramebuffer(GL_FRAMEBUFFER, 0); _SglThrowErrorOccuried();
    canv->layoutAltered = FALSE;
    return err;
}

_SGL afxError _AfxStdCanvImplDtor(afxCanvas canv, afxDrawEngine deng)
{
    afxError err = NIL;
    AfxEntry("canv=%p", canv);
    AfxAssertObject(canv, AFX_FCC_CANV);
    afxDrawContext dctx = AfxObjectGetProvider(&canv->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    sglVmt const* gl = &(deng->wglVmt);

    if (canv->gpuHandle[deng->queueIdx])
    {
        gl->DeleteFramebuffers(1, &(canv->gpuHandle[deng->queueIdx])); _SglThrowErrorOccuried();
        canv->gpuHandle[deng->queueIdx] = NIL;
    }
    return err;
}

_SGL afxError _AfxStdCanvUpdate(afxCanvas canv, afxDrawEngine deng)
{
    //AfxEntry("canv=%p", canv);
    afxError err = NIL;
    AfxAssertObject(canv, AFX_FCC_CANV);
    sglVmt const* gl = &deng->wglVmt;

    if (canv->layoutAltered || !canv->gpuHandle[deng->queueIdx])
    {
        _AfxStdCanvImplDtor(canv, deng);
        _AfxStdCanvImplInit(canv, deng); // rebuild
    }
    return err;
}

_SGL afxSurface _AfxCanvasGetSurface(afxCanvas canv, afxNat idx)
{
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);
	AfxAssert(idx < canv->surfaceCnt);
	afxSurface surf = canv->surfaces[idx];
	AfxAssertObject(surf, AFX_FCC_SURF);
	return surf;
}

_SGL afxNat _AfxCanvasGetSurfaceCount(afxCanvas canv)
{
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);
	return canv->surfaceCnt;
}

_SGL afxError _AfxCanvasSetExtent(afxCanvas canv, afxWhd const extent)
{
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);
	AfxAssert(extent);
	AfxAssert(extent[0]);
	AfxAssert(extent[1]);
	AfxAssert(extent[2]);

	if (canv->whd[0] != extent[0] || canv->whd[1] != extent[1] || canv->whd[2] != extent[2])
	{
		canv->whd[0] = extent[0];
		canv->whd[1] = extent[1];
		canv->whd[2] = extent[2];

		for (afxNat i = 0; i < canv->surfaceCnt; i++)
		{
			afxSurface surf = canv->surfaces[i];
			AfxAssertObject(surf, AFX_FCC_SURF);

			if (AfxTextureSetExtent(&surf->tex, surf->tex.layerCnt, canv->whd))
				AfxThrowError();
		}

		canv->layoutAltered = TRUE;
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

_SGL afxCanvas _AfxDrawContextAcquireCanvas(afxDrawContext dctx, afxWhd const extent, afxNat surfaceCnt, afxSurfaceSpecification const *specs)
{
	afxError err = NIL;
	AfxAssertObject(dctx, AFX_FCC_DCTX);
	AfxAssert(extent);
	AfxAssert(extent[0]);
	AfxAssert(extent[1]);
	AfxAssert(extent[2]);
	AfxAssert(specs);
	AfxAssert(surfaceCnt < 4);
	AfxEntry("dctx=%p,extent=[%u,%u,%u],surfaceCnt=%u", dctx, extent[0], extent[1], extent[2], surfaceCnt);
	afxCanvas canv = NIL;

	_afxCanvCtorArgs args =
	{
		dctx,
        extent,
		surfaceCnt,
		specs
	};

	if (!(canv = AfxObjectAcquire(AfxDrawContextGetCanvasClass(dctx), &args, AfxSpawnHint())))
		AfxThrowError();

	return canv;
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

	for (afxNat i = 0; i < canv->surfaceCnt; i++)
	{
		afxSurface surf = canv->surfaces[i];
		AfxTryAssertObject(surf, AFX_FCC_SURF);
		AfxObjectRelease(&surf->tex.res.obj);
	}

	canv->surfaceCnt = 0;
	return err;
}

_SGL afxCanvImpl const _AfxStdCanvImpl;
afxCanvImpl const _AfxStdCanvImpl =
{
    _AfxCanvasGetExtent,
    _AfxCanvasSetExtent,
    _AfxCanvasGetSurfaceCount,
    _AfxCanvasGetSurface
};

_SGL afxError _AfxCanvDtor(afxCanvas canv)
{
	AfxEntry("canv=%p", canv);
	afxError err = NIL;
	AfxAssertObject(canv, AFX_FCC_CANV);

    afxDrawContext dctx = AfxCanvasGetContext(canv);

    for (afxNat i = 0; i < dctx->queueCnt; i++)
    {
        if (canv->gpuHandle[i])
        {
            _SglEnqueueGlResourceDeletion(dctx, i, 3, canv->gpuHandle[i]);
            canv->gpuHandle[i] = 0;
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

	AfxAssert(args->whd[0]);
	AfxAssert(args->whd[1]);
	AfxAssert(args->whd[2]);

	canv->whd[0] = args->whd[0];
	canv->whd[1] = args->whd[1];
	canv->whd[2] = args->whd[2];

	AfxAssert(args->surfaceCnt);
	afxNat surfaceCnt = args->surfaceCnt;
	AfxAssert(surfaceCnt <= _SGL_MAX_SURF_PER_CANV);
	afxSurfaceSpecification const *specs = args->specs;
	AfxAssert(specs);

    canv->surfaceCnt = 0;

	for (afxNat i = 0; i < surfaceCnt; i++)
	{
		if (specs[i].surf)
		{
			if (!(AfxObjectReacquire(&(specs[i].surf->tex.res.obj), &canv->obj, NIL, NIL, NIL))) AfxThrowError();
			else
			{
				AfxAssertObject(specs[i].surf, AFX_FCC_SURF);
				AfxAssert(AfxTextureTestUsageFlags(&(specs[i].surf->tex), AFX_TEX_USAGE_SURFACE_RASTER | AFX_TEX_USAGE_SURFACE_DEPTH));
				canv->surfaces[i] = specs[i].surf;

				canv->surfaceCnt++;
			}
		}
		else
		{
			AfxAssert(specs[i].fmt);
			AfxAssert(specs[i].usage & AFX_TEX_USAGE_SURFACE_RASTER || specs[i].usage & AFX_TEX_USAGE_SURFACE_DEPTH);

			if (!(canv->surfaces[i] = AfxDrawContextAcquireSurface(args->dctx, specs[i].fmt, canv->whd, specs[i].usage))) AfxThrowError();
			else
			{
				canv->surfaceCnt++;
			}
		}
	}

	if (!err)
	{
		canv->layoutAltered = TRUE;
        
        canv->gpuHandle[0] = canv->gpuHandle[1] = canv->gpuHandle[2] = canv->gpuHandle[3] = 0;
	}

	if (err && canv->surfaceCnt)
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
