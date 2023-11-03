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
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/afxSurface.h"
#include "../src/e2coree/draw/afxDrawParadigms.h"

_SGL afxError _SglDpuSurfSync(sglDpuIdd* dpu, afxSurface surf, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    (void)dpu;

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
                AfxAssert(1 < surf->base.tex->base.whd[0]);
                AfxAssert(1 < surf->base.tex->base.whd[1]);

                afxTexture texIdd = surf->base.tex;

                if (!texIdd->glIntFmt)
                    SglDetermineGlTargetInternalFormatType(surf->base.tex, &texIdd->glTarget, &texIdd->glIntFmt, &texIdd->glFmt, &texIdd->glType);

                gl->RenderbufferStorage(GL_RENDERBUFFER, texIdd->glIntFmt, surf->base.tex->base.whd[0], surf->base.tex->base.whd[1]); _SglThrowErrorOccuried();
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

_SGL afxError _SglSurfDtor(afxSurface surf)
{
    AfxEntry("surf=%p", surf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &surf, afxFcc_SURF);
    afxDrawContext dctx = AfxGetObjectProvider(surf);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, afxFcc_CTX);

    if (surf->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 2, surf->glHandle);
        surf->glHandle = 0;
    }

    AfxReleaseObjects(1, (void*[]) { surf->base.tex });

    return err;
}

_SGL afxError _SglSurfCtor(afxSurface surf, afxCookie const* cookie)
{
    AfxEntry("surf=%p", surf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &surf, afxFcc_SURF);

    afxDrawContext dctx = AfxGetObjectProvider(surf);

    afxTextureInfo const *texi = ((afxTextureInfo const *)cookie->udd[0]) + cookie->no;

    if (AfxAcquireTextures(dctx, 1, texi, &surf->base.tex)) AfxThrowError();
    else
    {
        surf->glHandle = 0;
        surf->updFlags = SGL_UPD_FLAG_DEVICE_INST;
    }
    return err;
}

_SGL afxClassConfig _SglSurfClsConfig =
{
    .fcc = afxFcc_SURF,
    .name = "Surface",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSurface)),
    .ctx = NIL,
    .ctor = (void*)_SglSurfCtor,
    .dtor = (void*)_SglSurfDtor
};
