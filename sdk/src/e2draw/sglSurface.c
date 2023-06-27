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
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/res/afxSurface.h"
#include "../src/e2coree/draw/afxDrawParadigms.h"

_SGL afxError _SglDqueSurfSync(afxDrawQueue dque, afxSurface surf)
{
    afxError err = AFX_ERR_NONE;

    sglVmt const* gl = &dque->wglVmt;

    if (surf)
    {
        if ((surf->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((surf->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (surf->glHandle)
                {
                    gl->DeleteRenderbuffers(1, &(surf->glHandle)); _SglThrowErrorOccuried();
                    surf->glHandle = NIL;
                }

                AfxAssert(NIL == surf->glHandle);
                gl->GenRenderbuffers(1, &(surf->glHandle)); _SglThrowErrorOccuried();
                gl->BindRenderbuffer(GL_RENDERBUFFER, surf->glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsRenderBuffer(surf->glHandle));
                AfxAssert(1 < surf->tex.whd[0]);
                AfxAssert(1 < surf->tex.whd[1]);

                if (!surf->tex.glIntFmt)
                    SglDetermineGlTargetInternalFormatType(&surf->tex, &surf->tex.glTarget, &surf->tex.glIntFmt, &surf->tex.glFmt, &surf->tex.glType);

                gl->RenderbufferStorage(GL_RENDERBUFFER, surf->tex.glIntFmt, surf->tex.whd[0], surf->tex.whd[1]); _SglThrowErrorOccuried();
                gl->BindRenderbuffer(GL_RENDERBUFFER, 0); _SglThrowErrorOccuried();

                surf->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("afxSurface %p hardware-side data instanced.", surf);
            }
            else if ((surf->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(surf->glHandle);
                //gl->BindRenderbuffer(GL_RENDERBUFFER, surf->glHandle); _SglThrowErrorOccuried();
                AfxThrowError(); // cant't be modified by host
            }
        }
        else
        {
            AfxAssert(surf->glHandle);
            gl->BindRenderbuffer(GL_RENDERBUFFER, surf->glHandle); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->BindRenderbuffer(GL_RENDERBUFFER, 0); _SglThrowErrorOccuried();
    }
    return err;
}


_SGL afxBool _SglSurfEventHandler(afxObject *obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxSurface surf = (void*)obj;
    AfxAssertObject(surf, AFX_FCC_SURF);
    (void)ev;
    return FALSE;
}

_SGL afxBool _SglSurfEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxSurface surf = (void*)obj;
    AfxAssertObject(surf, AFX_FCC_SURF);
    (void)watched;
    (void)ev;
    return FALSE;
}

_SGL afxError _AfxSurfDtor(afxSurface surf)
{
    AfxEntry("surf=%p", surf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(surf, AFX_FCC_SURF);

    while (1)
    {
        if (!surf->swapchain.chain) break;
        else
        {
            AfxAssert(surf->state == AFX_SURF_STATE_PENDING);

            // AfxLinkageDrop(&canv->queue); // we can't do it here. We need wait for draw context to liberate it.

            afxDrawOutput dout = AfxLinkageGetOwner(&surf->swapchain);
            AfxAssertObject(dout, AFX_FCC_DOUT);
            //_SglDoutProcess(dout); // process until draw output ends its works and unlock this canvas.
            AfxYield();
        }
    }

    if (surf->glHandle)
    {
        afxDrawContext dctx = AfxTextureGetContext(&surf->tex);
        _SglEnqueueGlResourceDeletion(dctx, 0, 2, surf->glHandle);
        surf->glHandle = 0;
    }

    return err;
}

_SGL afxError _AfxSurfCtor(void *cache, afxNat idx, afxSurface surf, void *args)
{
    (void)args;
    AfxEntry("surf=%p", surf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(surf, AFX_FCC_SURF);

    surf->state = AFX_SURF_STATE_IDLE;

    AfxLinkageDeploy(&surf->swapchain, NIL);

    //AfxInterlinkDeployAsB(&surf->interlinkSentinel, surf);

    surf->glHandle = 0;
    surf->updFlags = SGL_UPD_FLAG_DEVICE_INST;

    return err;
}

_SGL afxSurfImpl const _AfxStdSurfImpl;
afxSurfImpl const _AfxStdSurfImpl =
{
    {
        _AfxTextureGetColorSwizzling,
        _AfxTextureGetData,
        //_AfxTextureMeasure,
        _AfxTextureGetExtent,
        _AfxTextureGenerateLods,
        _AfxTextureGetUri,
        _AfxTextureGetFormat,
        _AfxTextureGetLodCount,
        _AfxTextureGetLayerCount,
        _AfxTextureGetSampleCount,
        _AfxTextureMap,
        _AfxTextureSetExtent,
        _AfxTextureTestFlags,
        _AfxTextureTestUsageFlags,
        _AfxTextureUnmap,
        _AfxTextureUpdateRegions
    }
};

_SGL afxClassSpecification const _AfxSurfClassSpec;

afxClassSpecification const _AfxSurfClassSpec =
{
    AFX_FCC_SURF,
    NIL,
    0,
    sizeof(AFX_OBJECT(afxSurface)),
    NIL,
    (void*)_AfxSurfCtor,
    (void*)_AfxSurfDtor,
    .event = _SglSurfEventHandler,
    .eventFilter = _SglSurfEventFilter,
    "afxSurface",
    &_AfxStdSurfImpl
};
