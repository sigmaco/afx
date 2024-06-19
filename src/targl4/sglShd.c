/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"

#include "qwadro/draw/io/avxShader.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/exec/afxSystem.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/draw/io/avxShaderBlueprint.h"

////////////////////////////////////////////////////////////////////////////////
// SHADER                                                                     //
////////////////////////////////////////////////////////////////////////////////

_SGL afxError _DpuSyncShd(sglDpu* dpu, avxShader shd, avxShaderStage stage, glVmt const* gl)
{
    //AfxEntry("shd=%p", shd);
    afxError err = AFX_ERR_NONE;
    (void)dpu;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    if ((shd->updFlags & SGL_UPD_FLAG_DEVICE))
    {
        if (!shd->glHandle || (shd->updFlags & SGL_UPD_FLAG_DEVICE_INST))
        {
            if (shd->glHandle)
            {
                gl->DeleteShader(shd->glHandle); _SglThrowErrorOccuried();
                shd->glHandle = NIL;
            }

            AfxAssert(NIL == shd->glHandle);

            if (!(shd->glHandle = gl->CreateShader(AfxToGlShaderStage(stage))))
            {
                AfxThrowError();
                _SglThrowErrorOccuried();
            }
            else
            {
                AfxAssert(shd);
                AfxAssert(gl->IsShader(shd->glHandle));
                const GLint codeLens[] = { shd->m.codeLen };
                GLchar const* const codes[] = { (GLchar const*)shd->m.code };
                gl->ShaderSource(shd->glHandle, 1, codes, codeLens); _SglThrowErrorOccuried();
                gl->CompileShader(shd->glHandle); _SglThrowErrorOccuried();
                GLint status = 0;
                gl->GetShaderiv(shd->glHandle, GL_COMPILE_STATUS, &status); _SglThrowErrorOccuried();

                if (status == GL_FALSE)
                {
                    AfxThrowError();
                    afxChar str[1024];
                    gl->GetShaderInfoLog(shd->glHandle, sizeof(str), NIL, (GLchar*)str); _SglThrowErrorOccuried();
                    AfxLogError(str);
                    gl->DeleteShader(shd->glHandle); _SglThrowErrorOccuried();
                    shd->glHandle = NIL;
                }
                else
                {
                    shd->glProgHandle = 0;
                    shd->compiled = TRUE;
                    shd->updFlags &= ~(SGL_UPD_FLAG_DEVICE_INST | SGL_UPD_FLAG_DEVICE_FLUSH);
                    AfxLogEcho("avxShader %p hardware-side data instanced.", shd);
                }
            }
        }
        else if ((shd->updFlags & SGL_UPD_FLAG_DEVICE_FLUSH))
        {
            AfxAssert(shd->glHandle);
            
            AfxThrowError(); // can't be modified
        }
    }
    return err;
}

_SGL afxError _SglShdDtor(avxShader shd)
{
    afxError err = AFX_ERR_NONE;

    afxDrawContext dctx = AfxGetObjectProvider(shd);
    
    if (shd->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 6, (void*)shd->glHandle);
        shd->glHandle = 0;
    }

    if (shd->glProgHandle)
    {
        _SglDctxDeleteGlRes(dctx, 7, (void*)shd->glProgHandle);
        shd->glProgHandle = 0;
    }

    if (_AvxShdStdImplementation.dtor(shd))
        AfxThrowError();

    return err;
}

_SGL afxError _SglShdCtor(avxShader shd, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &shd, afxFcc_SHD);

    if (_AvxShdStdImplementation.ctor(shd, cookie)) AfxThrowError();
    else
    {
        shd->glHandle = NIL;
        shd->glProgHandle = NIL;
        shd->updFlags = SGL_UPD_FLAG_DEVICE_INST;
        shd->compiled = FALSE;

        if (err && _AvxShdStdImplementation.dtor(shd))
            AfxThrowError();
    }
    AfxAssertObjects(1, &shd, afxFcc_SHD);
    return err;
}

_SGL afxClassConfig const _SglShdMgrCfg =
{
    .fcc = afxFcc_SHD,
    .name = "Shader",
    .desc = "Programmable Pipeline Module",
    .fixedSiz = sizeof(AFX_OBJECT(avxShader)),
    .ctor = (void*)_SglShdCtor,
    .dtor = (void*)_SglShdDtor
};
