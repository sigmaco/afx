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
#include "qwadro/afxQwadro.h"
#include "qwadro/draw/pipe/avxDrawOps.h"

_SGL void _SglCopyTexSubImage(sglDpu* dpu, GLenum glDstTarget, GLenum glSrcTarget, GLuint glSrcHandle, afxNat opCnt, afxRasterCopy const ops[])
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    gl->BindFramebuffer(GL_READ_FRAMEBUFFER, dpu->fboOpSrc);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterCopy const* op = &ops[i];
        afxNat const* origin = op->srcOffset;
        afxRasterRegion const* dstRgn = &op->dst;

        _SglBindFboAttachment(gl, GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, glSrcTarget, glSrcHandle, op->srcLodIdx, op->srcBaseLayer, op->srcOffset[2]);
        gl->ReadBuffer(GL_COLOR_ATTACHMENT0);

        switch (glDstTarget)
        {
        case GL_TEXTURE_2D:
        {
            gl->CopyTexSubImage2D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->origin[1], origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
            break;
        }
        case GL_TEXTURE_1D_ARRAY:
        {
            for (afxNat i = dstRgn->whd[2]; i < dstRgn->whd[2]; i++)
            {
                gl->CopyTexSubImage2D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], i, origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_1D:
        {
            gl->CopyTexSubImage1D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], origin[0], origin[1], dstRgn->whd[0]);
            break;
        }
        case GL_TEXTURE_3D:
        {
            for (afxNat i = dstRgn->origin[2]; i < dstRgn->whd[2]; i++)
            {
                gl->CopyTexSubImage3D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->origin[1], i, origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_2D_ARRAY:
        {
            for (afxNat i = dstRgn->origin[2]; i < dstRgn->whd[2]; i++)
            {
                gl->CopyTexSubImage3D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->origin[1], i, origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_CUBE_MAP_ARRAY:
        {
            for (afxNat i = dstRgn->origin[2]; i < dstRgn->whd[2]; i++)
            {
                gl->CopyTexSubImage3D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->origin[1], i, origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_CUBE_MAP:
        {
            for (afxNat i = dstRgn->origin[2]; i < dstRgn->whd[2]; i++)
            {
                gl->CopyTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->origin[1], origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_RECTANGLE:
        {
            AfxAssert(dstRgn->lodIdx == 0);
            gl->CopyTexSubImage2D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->origin[1], origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
        };
        default:
            AfxThrowError();
            break;
        }
    }
}

_SGL void _DecodeCmdRasCopy(sglDpu* dpu, _sglCmdCopyRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;
    _DpuCopyRas(dpu, cmd->src, cmd->dst, cmd->opCnt, cmd->ops);
}

_SGL void _DecodeCmdRasPack(sglDpu* dpu, _sglCmdPackRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;

    if (cmd->unpack)
    {
        for (afxNat i = 0; i < cmd->opCnt; i++)
            _DpuUnpackRas(dpu, cmd->ras, &cmd->ops[i], cmd->buf);
    }
    else
    {
        for (afxNat i = 0; i < cmd->opCnt; i++)
            _DpuPackRas(dpu, cmd->ras, &cmd->ops[i], cmd->buf);
    }
}

_SGL void _DecodeCmdRasSubsample(sglDpu* dpu, _sglCmdRegenerateMipmaps const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxRaster ras = cmd->ras;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxNat lodCnt = cmd->lodCnt;
    afxNat baseLod = cmd->baseLod;
    AfxAssert(lodCnt == 1);
    AfxAssert(baseLod == 0);

    afxNat rasLodCnt = AfxCountRasterLods(ras);

    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);
    gl->GenerateMipmap(ras->glTarget); _SglThrowErrorOccuried();
    DpuBindAndSyncRas(dpu, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, NIL);

    AfxAssertRange(rasLodCnt, baseLod, lodCnt);
    AfxThrowError();
}
