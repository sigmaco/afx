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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"
#include "qwadro/afxQwadro.h"
#include "qwadro/draw/pipe/afxDrawOps.h"

_SGL void _SglUnpackTexSubImage(sglDpu* dpu, GLenum glTarget, afxNat bpp, GLenum glFmt, GLenum glType, afxNat opCnt, afxRasterIoOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    //DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_PIXEL_UNPACK_BUFFER, cmd->buf, 0, 0, 0, GL_STREAM_COPY);
    //DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterIoOp const *op = &ops[i];
        afxRasterRegion const* rgn = &op->rgn;

        /*
            GL_UNPACK_ALIGNMENT
            Specifies the alignment requirements for the start of each pixel row in memory.
            The allowable values are 1 (byte-alignment), 2 (rows aligned to even-numbered bytes), 4 (word-alignment), and 8 (rows start on double-word boundaries).

            GL_UNPACK_SKIP_PIXELS and GL_UNPACK_SKIP_ROWS
            These values are provided as a convenience to the programmer; they provide no functionality that cannot be duplicated by incrementing the pointer passed to glTexImage1D, glTexImage2D, glTexSubImage1D or glTexSubImage2D.
            Setting GL_UNPACK_SKIP_PIXELS to i is equivalent to incrementing the pointer by in components or indices, where n is the number of components or indices in each pixel.
            Setting GL_UNPACK_SKIP_ROWS to j is equivalent to incrementing the pointer by jk components or indices, where k is the number of components or indices per row, as just computed in the GL_UNPACK_ROW_LENGTH section.

            GL_UNPACK_IMAGE_HEIGHT
            If greater than 0, GL_UNPACK_IMAGE_HEIGHT defines the number of pixels in an image of a three-dimensional texture volume.
            Where "image" is defined by all pixel sharing the same third dimension index.

            The word component in this description refers to the nonindex values red, green, blue, alpha, and depth. Storage format GL_RGB, for example, has three components per pixel: first red, then green, and finally blue.

            GL_UNPACK_ROW_LENGTH
            If greater than 0, GL_UNPACK_ROW_LENGTH defines the number of pixels in a row.

            The word component in this description refers to the nonindex values red, green, blue, alpha, and depth.
            Storage format GL_RGB, for example, has three components per pixel: first red, then green, and finally blue.

            GL_UNPACK_LSB_FIRST
            If true, bits are ordered within a byte from least significant to most significant; otherwise, the first bit in each byte is the most significant one.

            GL_UNPACK_SWAP_BYTES
            If true, byte ordering for multibyte color components, depth components, or stencil indices is reversed.
            That is, if a four-byte component consists of bytes b0, b1, b2, b3, it is taken from memory as b3, b2, b1, b0 if GL_UNPACK_SWAP_BYTES is true.
            GL_UNPACK_SWAP_BYTES has no effect on the memory order of components within a pixel, only on the order of bytes within components or indices.
            For example, the three components of a GL_RGB format pixel are always stored with red first, green second, and blue third, regardless of the value of GL_UNPACK_SWAP_BYTES.
        */

        afxNat bypp = AFX_ALIGN(bpp, AFX_BYTE_SIZE) / AFX_BYTE_SIZE;
        afxNat rowLen = (op->bufRowStride / bypp);

        gl->PixelStorei(GL_UNPACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
        gl->PixelStorei(GL_UNPACK_ROW_LENGTH, rowLen); _SglThrowErrorOccuried();
        gl->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, op->bufRowCnt); _SglThrowErrorOccuried();
        //gl->PixelStorei(GL_UNPACK_LSB_FIRST, ); _SglThrowErrorOccuried();
        //gl->PixelStorei(GL_UNPACK_SWAP_BYTES, ); _SglThrowErrorOccuried();

        if (glTarget == GL_TEXTURE_CUBE_MAP)
        {
            for (afxNat j = 0; j < rgn->layerCnt; j++)
                _SglTexSubImage(gl, GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, rgn->lodIdx, rgn->baseLayer, 1, rgn->origin, rgn->whd, glFmt, glType, op->bufOffset);
        }
        else
        {
            _SglTexSubImage(gl, glTarget, rgn->lodIdx, rgn->baseLayer, rgn->layerCnt, rgn->origin, rgn->whd, glFmt, glType, op->bufOffset);
        }
    }

    //DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, NIL);
    //DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_PIXEL_UNPACK_BUFFER, NIL, 0, 0, 0, GL_STREAM_COPY);
}

_SGL void _SglCopyTexSubImage(sglDpu* dpu, GLenum glDstTarget, GLenum glSrcTarget, GLuint glSrcHandle, afxNat opCnt, afxRasterCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    gl->BindFramebuffer(GL_READ_FRAMEBUFFER, dpu->fboOpSrc);

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterCopyOp const* op = &ops[i];
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
            for (afxNat i = 0; i < dstRgn->layerCnt; i++)
            {
                gl->CopyTexSubImage2D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->baseLayer + i, origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
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
            for (afxNat i = 0; i < dstRgn->whd[2]; i++)
            {
                gl->CopyTexSubImage3D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->origin[1], dstRgn->origin[2] + i, origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_2D_ARRAY:
        {
            for (afxNat i = 0; i < dstRgn->layerCnt; i++)
            {
                gl->CopyTexSubImage3D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->origin[1], dstRgn->baseLayer + i, origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_CUBE_MAP_ARRAY:
        {
            for (afxNat i = 0; i < dstRgn->layerCnt; i++)
            {
                gl->CopyTexSubImage3D(glDstTarget, dstRgn->lodIdx, dstRgn->origin[0], dstRgn->origin[1], dstRgn->baseLayer + i, origin[0], origin[1], dstRgn->whd[0], dstRgn->whd[1]); _SglThrowErrorOccuried();
            }
            break;
        }
        case GL_TEXTURE_CUBE_MAP:
        {
            for (afxNat i = 0; i < dstRgn->layerCnt; i++)
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

_SGL void _DpuRasCopy(sglDpu* dpu, _sglCmdCopyRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxRaster dst = cmd->dst;
    afxRaster src = cmd->src;
    AfxAssertObjects(1, &dst, afxFcc_RAS);
    AfxAssertObjects(1, &src, afxFcc_RAS);
    afxNat opCnt = cmd->opCnt;
    AfxAssert(opCnt);
    afxRasterCopyOp const* ops = cmd->ops;
    AfxAssert(ops);

    DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, src);
    DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, dst);
    _SglCopyTexSubImage(dpu, dst->glTarget, src->glTarget, src->glHandle, opCnt, ops);
}

_SGL afxCmdId _SglEncodeCmdRasCopy(afxDrawStream diob, afxRaster src, afxRaster dst, afxNat opCnt, afxRasterCopyOp const ops[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdCopyRasterRegions *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + (opCnt * sizeof(cmd->ops[0])));
    AfxAssert(cmd);
    cmd->src = src;
    cmd->dst = dst;
    cmd->opCnt = opCnt;
    AfxAssert(8 > opCnt);

    for (afxNat i = 0; i < opCnt; i++)
        cmd->ops[i] = ops[i];

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, ras.cpy) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL void _DpuRasPack(sglDpu* dpu, _sglCmdPackRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxRaster ras = cmd->ras;
    afxBuffer buf = cmd->buf;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    afxNat opCnt = cmd->opCnt;
    AfxAssert(opCnt);
    afxRasterIoOp const* ops = cmd->ops;
    AfxAssert(ops);

    afxRasterInfo rasCap;
    AfxDescribeRaster(ras, &rasCap);

    afxPixelLayout pdf;
    AfxDescribePixelFormat(rasCap.fmt, &pdf);
    afxNat bpp = pdf.bpp;
    afxNat rasMaxRowSiz, rasMaxLayerSiz;
    AfxDetermineRasterStride(ras, 0, &rasMaxRowSiz, &rasMaxLayerSiz);
    afxNat rasMaxRowCnt = rasCap.whd[1];

    if (cmd->unpack)
    {
        DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_PIXEL_UNPACK_BUFFER, buf, 0, 0, 0, 0);
        DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);
        _SglUnpackTexSubImage(dpu, ras->glTarget, bpp, ras->glFmt, ras->glType, opCnt, ops);
    }

    for (afxNat i = 0; i < opCnt; i++)
    {
        afxRasterIoOp const *op = &ops[i];
        afxRasterRegion rgn = op->rgn;
        AfxAssertRange(rasCap.lodCnt, rgn.lodIdx, 1);
        AfxAssertRange(rasCap.layerCnt, rgn.baseLayer, rgn.layerCnt);
        AfxAssertRange(rasCap.whd[0], rgn.origin[0], rgn.whd[0]);
        AfxAssertRange(rasCap.whd[1], rgn.origin[1], rgn.whd[1]);
        AfxAssertRange(rasCap.whd[2], rgn.origin[2], rgn.whd[2]);

        afxNat bufOffset = op->bufOffset;
        afxNat bufRowStride = op->bufRowStride;
        afxNat bufRowCnt = op->bufRowCnt;

        AfxAssert(bufRowStride >= rasMaxRowSiz);
        AfxAssert(bufRowCnt >= rasMaxRowCnt);

        if (!cmd->unpack)
        {
            /*
                GL_PACK_ALIGNMENT
                Specifies the alignment requirements for the start of each pixel row in memory.
                The allowable values are 1 (byte-alignment), 2 (rows aligned to even-numbered bytes), 4 (word-alignment), and 8 (rows start on double-word boundaries).

                GL_PACK_SKIP_PIXELS and GL_PACK_SKIP_ROWS
                These values are provided as a convenience to the programmer; they provide no functionality that cannot be duplicated simply by incrementing the pointer passed to glReadPixels.
                Setting GL_PACK_SKIP_PIXELS to i is equivalent to incrementing the pointer by in components or indices, where n is the number of components or indices in each pixel.
                Setting GL_PACK_SKIP_ROWS to j is equivalent to incrementing the pointer by jm components or indices, where m is the number of components or indices per row, as just computed in the GL_PACK_ROW_LENGTH section.
                Setting GL_PACK_SKIP_IMAGES to k is equivalent to incrementing the pointer by kp, where p is the number of components or indices per image, as computed in the GL_PACK_IMAGE_HEIGHT section.

                GL_PACK_IMAGE_HEIGHT
                If greater than 0, GL_PACK_IMAGE_HEIGHT defines the number of pixels in an image three-dimensional texture volume, where "image" is defined by all pixels sharing the same third dimension index.

                The word component in this description refers to the nonindex values red, green, blue, alpha, and depth.
                Storage format GL_RGB, for example, has three components per pixel: first red, then green, and finally blue.

                GL_PACK_ROW_LENGTH
                If greater than 0, GL_PACK_ROW_LENGTH defines the number of pixels in a row.

                The word component in this description refers to the nonindex values red, green, blue, alpha, and depth.
                Storage format GL_RGB, for example, has three components per pixel: first red, then green, and finally blue.

                GL_PACK_LSB_FIRST
                If true, bits are ordered within a byte from least significant to most significant; otherwise, the first bit in each byte is the most significant one.

                GL_PACK_SWAP_BYTES
                If true, byte ordering for multibyte color components, depth components, or stencil indices is reversed.
                That is, if a four-byte component consists of bytes b0, b1, b2, b3, it is stored in memory as b3, b2, b1, b0 if GL_PACK_SWAP_BYTES is true.
                GL_PACK_SWAP_BYTES has no effect on the memory order of components within a pixel, only on the order of bytes within components or indices.
                For example, the three components of a GL_RGB format pixel are always stored with red first, green second, and blue third, regardless of the value of GL_PACK_SWAP_BYTES.
            */

            //afxNat rowSiz = dstRowSiz ? dstRowSiz : srcRowSiz;
            //afxNat rowCnt = dstRowCnt ? dstRowCnt : srcRowCnt;
            //afxNat dataSiz = rowSiz * rowCnt * rgn.whd[2] * rgn.layerCnt;
            //AfxAssert(rgnSiz >= dataSiz);

            DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_PIXEL_PACK_BUFFER, cmd->buf, 0, 0, 0, GL_STREAM_COPY);
            DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);

            afxNat bypp = AFX_ALIGN(AfxGetRasterBpp(ras), AFX_BYTE_SIZE) / AFX_BYTE_SIZE;
            afxNat rowLen = (bufRowStride / bypp);

            gl->PixelStorei(GL_PACK_ALIGNMENT, 1); _SglThrowErrorOccuried();
            gl->PixelStorei(GL_PACK_ROW_LENGTH, rowLen); _SglThrowErrorOccuried();
            gl->PixelStorei(GL_PACK_IMAGE_HEIGHT, bufRowCnt); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_PACK_LSB_FIRST, ); _SglThrowErrorOccuried();
            //gl->PixelStorei(GL_PACK_SWAP_BYTES, ); _SglThrowErrorOccuried();

            if (ras->glTarget == GL_TEXTURE_CUBE_MAP)
            {
                for (afxNat j = 0; j < rgn.layerCnt; j++)
                {
                    gl->GetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, rgn.lodIdx, ras->glFmt, ras->glType, (void*)bufOffset); _SglThrowErrorOccuried();
                }
            }
            else
            {
                gl->GetTexImage(ras->glTarget, rgn.lodIdx, ras->glFmt, ras->glType, (void*)bufOffset); _SglThrowErrorOccuried();
            }

            DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, NIL);
            DpuBindAndSyncBuf(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, GL_PIXEL_PACK_BUFFER, NIL, 0, 0, 0, GL_STREAM_COPY);
        }
    }
}

_SGL afxCmdId _SglEncodeCmdRasPack(afxDrawStream diob, afxRaster ras, afxBuffer buf, afxNat opCnt, afxRasterIoOp const ops[], afxBool unpack)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &diob, afxFcc_DIOB);
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssert(opCnt);
    AfxAssert(ops);

    _sglCmdPackRasterRegions *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + (opCnt * sizeof(cmd->ops[0])));
    AfxAssert(cmd);
    cmd->unpack = !!unpack;
    cmd->ras = ras;
    cmd->buf = buf;
    cmd->opCnt = opCnt;
    AfxAssert(8 > opCnt);

    for (afxNat i = 0; i < opCnt; i++)
        cmd->ops[i] = ops[i];

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, ras.pak) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL void _DpuRasSubsample(sglDpu* dpu, _sglCmdRegenerateMipmaps const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxRaster ras = cmd->ras;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxNat lodCnt = cmd->lodCnt;
    afxNat baseLod = cmd->baseLod;
    AfxAssert(lodCnt == 1);
    AfxAssert(baseLod == 0);

    afxWhd rasWhd;
    AfxGetRasterExtent(ras, 0, rasWhd);
    afxNat rasLayerCnt = AfxCountRasterLayers(ras);
    afxNat rasLodCnt = AfxCountRasterLods(ras);

    DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP | sglBindFlag_SYNC, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, ras);
    gl->GenerateMipmap(ras->glTarget); _SglThrowErrorOccuried();
    DpuBindAndSyncRas(dpu, sglBindFlag_BIND | sglBindFlag_KEEP, SGL_LAST_COMBINED_TEXTURE_IMAGE_UNIT, NIL);

    AfxAssertRange(rasLodCnt, baseLod, lodCnt);
    AfxThrowError();
}

_SGL afxCmdId _SglEncodeCmdRasSubsample(afxDrawStream diob, afxRaster ras, afxNat baseLod, afxNat lodCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdRegenerateMipmaps *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd));
    AfxAssert(cmd);
    cmd->ras = ras;
    cmd->baseLod = baseLod;
    cmd->lodCnt = lodCnt;

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, ras.mip) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL void _DpuRasSwizzle(sglDpu* dpu, _sglCmdSwizzleRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;
    afxRaster ras = cmd->ras;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxNat rgnCnt = cmd->rgnCnt;
    afxRasterRegion const* rgns = cmd->rgn;
    AfxAssert(rgnCnt);
    afxColorSwizzle a = cmd->a;
    afxColorSwizzle b = cmd->b;

    afxWhd rasWhd;
    AfxGetRasterExtent(ras, 0, rasWhd);
    afxNat rasLayerCnt = AfxCountRasterLayers(ras);
    afxNat rasLodCnt = AfxCountRasterLods(ras);

    for (afxNat j = 0; j < rgnCnt; j++)
    {
        afxRasterRegion const* rgn = &rgns[j];
        AfxAssert(rgn);
        AfxAssert(rasLodCnt > rgn->lodIdx);
        AfxAssertRange(rasLayerCnt, rgn->baseLayer, rgn->layerCnt);
        AfxAssertRange(rasWhd[0], rgn->origin[0], rgn->whd[0]);
        AfxAssertRange(rasWhd[1], rgn->origin[1], rgn->whd[1]);
        AfxAssertRange(rasWhd[2], rgn->origin[2], rgn->whd[2]);

        afxPixelFormat fmt = AfxGetRasterFormat(ras);
        AfxAssert(fmt == afxPixelFormat_BGR8 || fmt == afxPixelFormat_BGRA8 || fmt == afxPixelFormat_RGB8 || fmt == afxPixelFormat_RGBA8);

        afxPixelLayout pfd;
        AfxDescribePixelFormat(fmt, &pfd);
        afxNat pixelSiz = AFX_ALIGN(pfd.bpp, AFX_BYTE_SIZE) / AFX_BYTE_SIZE;

        AfxAssert(a < afxColorSwizzle_ZERO);

        afxNat rgnSize;
        afxNat rowLen = 0;
        afxByte *bytemap = NIL;// AfxMapRaster(ras, rgn, afxRasterAccess_RW, &rgnSize, &rowLen);

        // swap the R and B values to get RGB since the bitmap color format is in BGR
        for (afxNat k = 0; k < rgn->whd[0] * rgn->whd[1] * pixelSiz; k += pixelSiz)
        {
            afxByte temp = (b != afxColorSwizzle_ZERO ? (b != afxColorSwizzle_ONE ? bytemap[k + b] : 1) : 0);
            bytemap[k + b] = bytemap[k + a];
            bytemap[k + a] = temp;
        }

        //AfxUnmapRaster(ras, rgn);
    }
}

_SGL afxCmdId _SglEncodeCmdRasSwizzle(afxDrawStream diob, afxRaster ras, afxColorSwizzle a, afxColorSwizzle b, afxNat rgnCnt, afxRasterRegion const rgn[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdSwizzleRasterRegions *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + (rgnCnt * sizeof(cmd->rgn[0])));
    AfxAssert(cmd);
    cmd->ras = ras;
    cmd->a = a;
    cmd->b = b;
    cmd->rgnCnt = rgnCnt;
    AfxAssert(8 > rgnCnt);

    for (afxNat i = 0; i < rgnCnt; i++)
        cmd->rgn[i] = rgn[i];

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, ras.swizzle) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL void _DpuRasXform(sglDpu* dpu, _sglCmdFlipRasterRegions const* cmd)
{
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;
    afxRaster ras = cmd->ras;
    AfxAssertObjects(1, &ras, afxFcc_RAS);
    afxNat rgnCnt = cmd->rgnCnt;
    afxRasterRegion const* rgns = cmd->rgn;

    afxWhd rasWhd;
    AfxGetRasterExtent(ras, 0, rasWhd);
    afxNat rasLayerCnt = AfxCountRasterLayers(ras);
    afxNat rasLodCnt = AfxCountRasterLods(ras);

    for (afxNat j = 0; j < rgnCnt; j++)
    {
        afxRasterRegion const* rgn = &rgns[j];
        afxRasterRegion srcRgn = *rgn;
        srcRgn.layerCnt = 1;
        AfxGetRasterExtent(ras, 0, srcRgn.whd);

        afxNat size;
        afxNat rowLen = 0;

        afxNat bpl;
        AfxDetermineRasterStride(ras, 0, &size, &bpl);
        //afxNat bpr = AfxDetermineRasterStride(ras);
        afxNat rowCnt = (rasWhd[1] >> 1);
        afxByte tmp[2048]; // used to be max texture size supported in old days

        for (afxNat layer = 0; layer < srcRgn.layerCnt; ++layer)
        {
            srcRgn.baseLayer = layer;
            afxByte* bytemap = NIL;// AfxMapRaster(ras, &srcRgn, afxRasterAccess_RW, &size, &rowLen);

            for (afxNat row = 0, row2 = rowCnt; row < rowCnt; row++)
            {
                afxByte *first = &bytemap[row * rowLen];
                afxByte *next = &bytemap[--row2 * rowLen];
                // swap row0 with row1
                afxNat bytesLeft = rowLen;

                while (bytesLeft)
                {
                    afxNat bytesCpy = (bytesLeft < sizeof(tmp)) ? bytesLeft : sizeof(tmp);
                    AfxCopy2(bytesCpy, 1, first, tmp);
                    AfxCopy2(bytesCpy, 1, next, first);
                    AfxCopy2(bytesCpy, 1, tmp, next);
                    first += bytesCpy;
                    next += bytesCpy;
                    bytesLeft -= bytesCpy;
                }
            }
            //AfxUnmapRaster(ras, &srcRgn);
            //bytemap += bpl;
        }
    }

#if 0

    afxNat bpr = AfxDetermineRasterStride(ras);
    AfxAssert(bpr);
    afxByte tmp[2048]; // used to be max texture size supported in old days
    afxByte *bytes = ras->maps;

    afxNat layerCnt = ras->layerCnt;
    afxNat bpl = AfxMeasureRasterLayer(ras);
    AfxAssert(bpl);
    afxNat rowCnt = (ras->whd[1] >> 1);

    for (afxNat layer = 0; layer < layerCnt; ++layer)
    {
        for (afxNat row = 0; row < rowCnt; row++)
        {
            afxByte *first = bytes + (row * bpr);
            afxByte *next = bytes + (ras->whd[1] - row - 1) * bpr;
            // swap row0 with row1
            afxNat bytesLeft = bpr;

            while (bytesLeft)
            {
                afxSize bytesCpy = (bytesLeft < sizeof(tmp)) ? bytesLeft : sizeof(tmp);
                AfxCopy2(bytesCpy, 1, first, tmp);
                AfxCopy2(bytesCpy, 1, next, first);
                AfxCopy2(bytesCpy, 1, tmp, next);
                first += bytesCpy;
                next += bytesCpy;
                bytesLeft -= bytesCpy;
            }
        }
        bytes += bpl;
    }
#endif
}

_SGL afxCmdId _SglEncodeCmdRasXform(afxDrawStream diob, afxRaster ras, afxM4d const m, afxNat rgnCnt, afxRasterRegion const rgn[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diob->base.state == afxDrawStreamState_RECORDING);

    _sglCmdFlipRasterRegions *cmd = AfxRequestArenaUnit(&diob->base.cmdArena, sizeof(*cmd) + (rgnCnt * sizeof(cmd->rgn[0])));
    AfxAssert(cmd);
    cmd->ras = ras;
    AfxCopyM4d(cmd->m, m);
    cmd->rgnCnt = rgnCnt;
    AfxAssert(8 > rgnCnt);

    for (afxNat i = 0; i < rgnCnt; i++)
        cmd->rgn[i] = rgn[i];

    return _SglEncodeCmdCommand(diob, (offsetof(afxCmd, ras.xform) / sizeof(void*)), sizeof(cmd), &cmd->cmd);
}

_SGL afxCmdRaster const _SglEncodeCmdRasterVmt =
{
    .swizzle = _SglEncodeCmdRasSwizzle,
    .xform = _SglEncodeCmdRasXform,
    .mip = _SglEncodeCmdRasSubsample,
    .cpy = _SglEncodeCmdRasCopy,
    .pak = _SglEncodeCmdRasPack,
};