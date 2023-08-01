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

#define _AFX_SHADER_C
#define _AFX_DRAW_QUEUE_C
#include "sgl.h"

#include "../e2coree/draw/afxDrawParadigms.h"
#include "afx/draw/res/afxShader.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxSystem.h"
#include "afx/core/io/afxUri.h"
#include "afx/draw/res/afxShaderBlueprint.h"

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _SglDqueSyncShd(afxDrawQueue dque, afxShader shd, afxShaderStage stage, sglVmt const* gl)
{
    //AfxEntry("shd=%p", shd);
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);

    sglShdIdd *idd = shd->idd;
    sglDqueIdd *dqueIdd = dque->idd;

    if ((idd->updFlags & SGL_UPD_FLAG_DEVICE))
    {
        if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (idd->glHandle)
            {
                gl->DeleteShader(idd->glHandle); _SglThrowErrorOccuried();
                idd->glHandle = NIL;
            }

            AfxAssert(NIL == idd->glHandle);

            if (!(idd->glHandle = gl->CreateShader(AfxToGlShaderStage(stage))))
            {
                AfxThrowError();
                _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(shd);
                AfxAssert(gl->IsShader(idd->glHandle));
                const GLint codeLens[] = { shd->codeLen };
                GLchar const* const codes[] = { (GLchar const*)shd->code };
                gl->ShaderSource(idd->glHandle, 1, codes, codeLens); _SglThrowErrorOccuried();
                gl->CompileShader(idd->glHandle); _SglThrowErrorOccuried();
                GLint status = 0;
                gl->GetShaderiv(idd->glHandle, GL_COMPILE_STATUS, &status); _SglThrowErrorOccuried();

                if (status == GL_FALSE)
                {
                    AfxThrowError();
                    afxChar str[1024];
                    gl->GetShaderInfoLog(idd->glHandle, sizeof(str), NIL, (GLchar*)str); _SglThrowErrorOccuried();
                    AfxError(str);
                    gl->DeleteShader(idd->glHandle); _SglThrowErrorOccuried();
                    idd->glHandle = NIL;
                }
                else
                {
                    idd->glProgHandle = 0;
                    idd->compiled = TRUE;
                    idd->updFlags &= ~(SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH);
                    AfxEcho("afxShader %p hardware-side data instanced.", shd);
                }
            }
        }
        else if ((idd->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
        {
            AfxAssert(idd->glHandle);
            
            AfxThrowError(); // can't be modified
        }
    }
    return err;
}

_SGL afxError _AfxShdDtor(afxShader shd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(shd, AFX_FCC_SHD);

    afxDrawContext dctx = AfxGetShaderContext(shd);
    afxMemory mem = AfxGetDrawContextMemory(dctx);
    sglShdIdd *idd = shd->idd;

    if (idd)
    {
        if (idd->glHandle)
        {
            _SglDeleteGlRes(dctx, 6, idd->glHandle);
            idd->glHandle = 0;
        }

        if (idd->glProgHandle)
        {
            _SglDeleteGlRes(dctx, 7, idd->glProgHandle);
            idd->glProgHandle = 0;
        }

        AfxDeallocate(mem, idd);
        shd->idd = NIL;
    }

    return err;
}

_SGL _afxShdVmt _SglShdVmt =
{
    _AfxShdDtor
};

_SGL afxError _AfxShdCtor(afxShader shd)
{
    AfxEntry("shd=%p", shd);
    afxError err = AFX_ERR_NONE;
    afxDrawContext dctx = AfxObjectGetProvider(&shd->obj);
    AfxAssertObject(dctx, AFX_FCC_DCTX);

    afxMemory mem = AfxGetDrawContextMemory(dctx);
    AfxAssertObject(mem, AFX_FCC_MEM);

    shd->vmt = &_SglShdVmt;
    sglShdIdd *idd = AfxAllocate(mem, sizeof(*idd), 0, AfxSpawnHint());
    shd->idd = idd;
    idd->glHandle = NIL;
    idd->glProgHandle = NIL;
    idd->updFlags = SGL_UPD_FLAG_DEVICE_INST;
    idd->compiled = FALSE;

    return err;
}

