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
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_CANVAS_C
#define _AFX_SURFACE_C
#define _AFX_TEXTURE_C
#define _AFX_DRAW_CONTEXT_C
#define _AFX_DRAW_QUEUE_C
#include "sgl.h"
#include "afx/draw/pipelining/afxCanvas.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/draw/afxDrawSystem.h"

////////////////////////////////////////////////////////////////////////////////
// FRAME BUFFER                                                               //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglCanvDtor(afxCanvas canv, afxDrawQueue dque, sglVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("canv=%p", canv);
    AfxAssertObject(canv, AFX_FCC_CANV);
    afxDrawContext dctx = AfxGetCanvasContext(canv);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    sglDqueIdd*dqueIdd = dque->idd;

    sglCanvIdd*idd = canv->idd;

    if (idd)
    {
        if (idd->glHandle)
        {
            gl->DeleteFramebuffers(1, &(idd->glHandle)); _SglThrowErrorOccuried();
            idd->glHandle = NIL;
        }
    }
    return err;
}

_SGL afxError _SglCanvReinstantiateIdd(afxDrawQueue dque, afxCanvas canv, GLenum target, sglVmt const* gl)
{
    AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = AfxGetCanvasContext(canv);
    AfxAssertObject(dctx, AFX_FCC_DCTX);
    afxMemory mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);
    sglCanvIdd *idd;

    if (!(idd = canv->idd))
    {
        if (!(canv->idd = (idd = AfxAllocate(mem, sizeof(*idd), 0, AfxSpawnHint())))) AfxThrowError();
        else
        {
            *idd = (sglCanvIdd) { 0 };

            idd->updFlags = SGL_UPD_FLAG_DEVICE_INST;
        }
    }
    
    if (idd)
    {
        if (idd->glHandle)
        {
            gl->DeleteFramebuffers(1, &(idd->glHandle)); _SglThrowErrorOccuried();
            idd->glHandle = NIL;
        }
        *idd = (sglCanvIdd) { 0 };
        idd->updFlags = SGL_UPD_FLAG_DEVICE_INST;
    }

    if (!err && idd && !idd->glHandle)
    {
        gl->GenFramebuffers(1, &(idd->glHandle)); _SglThrowErrorOccuried();
        gl->BindFramebuffer(target, idd->glHandle); _SglThrowErrorOccuried();
        AfxAssert(gl->IsFramebuffer(idd->glHandle));
        afxNat const surfCnt = AfxGetAnnexedSurfaceCount(canv);
        afxNat rasterIdx = 0;

        for (afxNat i = 0; i < surfCnt; i++)
        {
            afxSurface surf = AfxGetAnnexedSurface(canv, i);
            AfxAssertObject(surf, AFX_FCC_SURF);

            if (!surf) AfxThrowError();
            else
            {
                GLenum glAttachment;

                if (AfxTestTexture(&surf->tex, AFX_TEX_FLAG_SURFACE_RASTER))
                {
                    glAttachment = GL_COLOR_ATTACHMENT0 + rasterIdx;
                    ++rasterIdx;
                }
                else if (AfxTestTexture(&surf->tex, AFX_TEX_FLAG_SURFACE_DEPTH))
                {
                    if (canv->dsIdx[0] == i)
                    {
                        glAttachment = (canv->dsIdx[1] == i) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
                    }
                    else if (canv->dsIdx[1] == i)
                    {
                        glAttachment = GL_STENCIL_ATTACHMENT;
                    }
                    else glAttachment = 0;
                }
                else glAttachment = 0;

                _SglDqueBindAndSyncTex(dque, 0, &surf->tex, gl);
                _SglDqueBindAndSyncTex(dque, 0, 0, gl);
                sglTexIdd *texIdd = surf->tex.idd;

                if (!texIdd) AfxThrowError();
                else
                {
                    AfxAssert(texIdd->glHandle);
                    AfxAssert(gl->IsTexture(texIdd->glHandle));

                    _SglDqueSurfSync(dque, surf, gl);
                    sglSurfIdd *surfIdd = surf->idd;

                    if (!surfIdd) AfxThrowError();
                    else
                    {
                        AfxAssert(surfIdd->glHandle);
                        AfxAssert(gl->IsRenderBuffer(surfIdd->glHandle));
                        gl->FramebufferRenderbuffer(target, glAttachment, GL_RENDERBUFFER, surfIdd->glHandle); _SglThrowErrorOccuried();
                    }

                    switch (texIdd->glTarget)
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
                        gl->FramebufferTexture(target, glAttachment, texIdd->glHandle, 0); _SglThrowErrorOccuried();
                        break;
                    };
                }
            }
        }

        switch (gl->CheckFramebufferStatus(target))
        {
        case GL_FRAMEBUFFER_COMPLETE:
            idd->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
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

_SGL afxError _SglDqueBindAndSyncCanv(afxDrawQueue dque, afxCanvas canv, GLenum target, sglVmt const* gl)
{
    //AfxEntry("canv=%p", canv);
    afxError err = AFX_ERR_NONE;
    sglDqueIdd*dqueIdd = dque->idd;

    if (canv)
    {
        AfxAssertObject(canv, AFX_FCC_CANV);

        sglCanvIdd*idd = canv->idd;

        if (!canv->idd || idd->updFlags & SGL_UPD_FLAG_DEVICE_INST || AfxTestCanvas(canv, AFX_FLAG(AFX_CANV_FLAG_REVALIDATE)))
        {
            if (_SglCanvReinstantiateIdd(dque, canv, target, gl))
                AfxThrowError();

            idd = canv->idd;

            if (!err)
                AfxUnflagCanvas(canv, AFX_FLAG(AFX_CANV_FLAG_REVALIDATE));
        }

        if (!canv->idd) AfxThrowError();
        else
        {
            idd = canv->idd;

            {
                AfxAssert(idd->glHandle);
                gl->BindFramebuffer(target, idd->glHandle); _SglThrowErrorOccuried();
            }
        }
    }
    else
    {
        if (dqueIdd->state.renderPass.canv)
        {
            gl->BindFramebuffer(target, 0); _SglThrowErrorOccuried();
        }
    }
    return err;
}

_SGL afxError _AfxReadjustCanvas(afxCanvas canv, afxWhd const from, afxWhd const extent)
{
	afxError err = AFX_ERR_NONE;
	AfxAssertObject(canv, AFX_FCC_CANV);
    sglCanvIdd *idd = canv->idd;
	AfxAssert(extent);
	AfxAssert(extent[0]);
	AfxAssert(extent[1]);
	AfxAssert(extent[2]);
    AfxAssert(extent[0] != canv->whd[0] || extent[1] != canv->whd[1] || extent[2] != canv->whd[2]);

	if (canv->whd[0] != extent[0] || canv->whd[1] != extent[1] || canv->whd[2] != extent[2])
	{
        idd->updFlags |= SGL_UPD_FLAG_DEVICE_INST;
	}
	return err;
}

_SGL afxError _AfxCanvDropAllSurfaces(afxCanvas canv)
{
	AfxEntry("canv=%p", canv);
	afxError err = AFX_ERR_NONE;
	AfxAssertObject(canv, AFX_FCC_CANV);

	for (afxNat i = 0; i < canv->annexCnt; i++)
	{
		afxSurface surf = canv->annexes[i].surf;

        if (surf)
        {
            AfxTryAssertObject(surf, AFX_FCC_SURF);
            AfxReleaseObject(&surf->tex.obj);
            canv->annexes[i].surf = NIL;
        }
	}

	//canv->rasterCnt = 0;
    //canv->surfCnt = 0;
	return err;
}

_SGL afxError _AfxCanvDtor(afxCanvas canv)
{
	AfxEntry("canv=%p", canv);
	afxError err = AFX_ERR_NONE;
	AfxAssertObject(canv, AFX_FCC_CANV);

    _AfxCanvDropAllSurfaces(canv);

    sglCanvIdd *idd = canv->idd;

    if (idd)
    {
        afxDrawContext dctx = AfxGetCanvasContext(canv);
        AfxAssertObject(dctx, AFX_FCC_DCTX);
        afxMemory mem = AfxGetDrawContextMemory(dctx);
        AfxAssertObject(mem, AFX_FCC_MEM);

        if (idd->glHandle)
        {
            _SglDeleteGlRes(dctx, 3, idd->glHandle);
            idd->glHandle = 0;
        }
        AfxDeallocate(mem, idd);
        canv->idd = NIL;
    }

	return err;
}

_SGL _afxCanvVmt _SglCanvVmt=
{
    _AfxCanvDtor,
    _AfxReadjustCanvas
};

_SGL afxError _AfxCanvCtor(afxCanvas canv)
{
	AfxEntry("canv=%p", canv);
	afxError err = AFX_ERR_NONE;
	AfxAssertObject(canv, AFX_FCC_CANV);
    afxDrawContext dctx = AfxGetCanvasContext(canv);
    afxMemory mem = AfxGetDrawContextMemory(dctx);

    canv->vmt = &_SglCanvVmt;
    canv->idd = NIL;
    
	return err;
}
