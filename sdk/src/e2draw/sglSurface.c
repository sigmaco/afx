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

#define _AFX_SURFACE_C
#define _AFX_TEXTURE_C
#define _AFX_DRAW_QUEUE_C
#include "sgl.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/res/afxSurface.h"
#include "../src/e2coree/draw/afxDrawParadigms.h"

_SGL afxError _SglDqueSurfSync(afxDrawQueue dque, afxSurface surf, glVmt const* gl)
{
    afxError err = AFX_ERR_NONE;
    sglDqueIdd *dqueIdd = dque->idd;

    if (surf)
    {
        sglSurfIdd *idd = surf->idd;

        if ((idd->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (idd->glHandle)
                {
                    gl->DeleteRenderbuffers(1, &(idd->glHandle)); _SglThrowErrorOccuried();
                    idd->glHandle = NIL;
                }

                AfxAssert(NIL == idd->glHandle);
                gl->GenRenderbuffers(1, &(idd->glHandle)); _SglThrowErrorOccuried();
                gl->BindRenderbuffer(GL_RENDERBUFFER, idd->glHandle); _SglThrowErrorOccuried();
                AfxAssert(gl->IsRenderBuffer(idd->glHandle));
                AfxAssert(1 < surf->tex.whd[0]);
                AfxAssert(1 < surf->tex.whd[1]);

                sglTexIdd*texIdd = surf->tex.idd;

                if (!texIdd->glIntFmt)
                    SglDetermineGlTargetInternalFormatType(&surf->tex, &texIdd->glTarget, &texIdd->glIntFmt, &texIdd->glFmt, &texIdd->glType);

                gl->RenderbufferStorage(GL_RENDERBUFFER, texIdd->glIntFmt, surf->tex.whd[0], surf->tex.whd[1]); _SglThrowErrorOccuried();
                gl->BindRenderbuffer(GL_RENDERBUFFER, 0); _SglThrowErrorOccuried();

                idd->updFlags &= ~(SGL_UPD_FLAG_DEVICE);
                AfxEcho("afxSurface %p hardware-side data instanced.", surf);
            }
            else if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
            {
                AfxAssert(idd->glHandle);
                //gl->BindRenderbuffer(GL_RENDERBUFFER, surf->glHandle); _SglThrowErrorOccuried();
                AfxThrowError(); // cant't be modified by host
            }
        }
        else
        {
            AfxAssert(idd->glHandle);
            gl->BindRenderbuffer(GL_RENDERBUFFER, idd->glHandle); _SglThrowErrorOccuried();
        }
    }
    else
    {
        gl->BindRenderbuffer(GL_RENDERBUFFER, 0); _SglThrowErrorOccuried();
    }
    return err;
}

_SGL afxError _AfxSurfDtor(afxSurface surf)
{
    AfxEntry("surf=%p", surf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(surf, AFX_FCC_SURF);
    afxDrawContext dctx = AfxGetTextureContext(&surf->tex);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);

    sglSurfIdd *idd = surf->idd;

    if (idd)
    {
        if (idd->glHandle)
        {
            _SglDeleteGlRes(dctx, 2, idd->glHandle);
            idd->glHandle = 0;
        }
        AfxDeallocate(mem, idd);
        surf->idd = NIL;
    }

    return err;
}

_SGL _afxSurfVmt _SglSurfVmt =
{
    _AfxSurfDtor
};

_SGL afxError _AfxSurfCtor(afxSurface surf)
{
    AfxEntry("surf=%p", surf);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(surf, AFX_FCC_SURF);
    afxDrawContext dctx = AfxGetTextureContext(&surf->tex);
    afxContext mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObjects(1, &mem, AFX_FCC_MEM);

    surf->vmt = &_SglSurfVmt;
    sglSurfIdd *idd = AfxAllocate(mem, sizeof(*idd), 0, AfxSpawnHint());
    surf->idd = idd;
    idd->glHandle = 0;
    idd->updFlags = SGL_UPD_FLAG_DEVICE_INST;

    return err;
}
