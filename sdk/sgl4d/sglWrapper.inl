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
#include "sglDdrv.h"
#include "qwadro/core/afxSystem.h"

SGLINL void SglBindTextureUnit(sglDpu* dpu, GLuint unit, GLenum target, GLuint texture)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    if (gl->BindTextureUnit)
    {
        gl->BindTextureUnit(unit, texture);
    }
    else
    {
        gl->ActiveTexture(GL_TEXTURE0 + unit); _SglThrowErrorOccuried();
        gl->BindTexture(target, texture); _SglThrowErrorOccuried();
    }
}

SGLINL void SglUsePipeline(sglDpu* dpu, GLuint program)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    gl->UseProgram(program); _SglThrowErrorOccuried();
}

SGLINL void SglBindFramebuffer(sglDpu* dpu, GLenum target, GLuint fbo)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    gl->BindFramebuffer(target, fbo); _SglThrowErrorOccuried();
}
