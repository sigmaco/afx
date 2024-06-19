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

#ifndef AVX_TO_GL_H
#define AVX_TO_GL_H

#include "WglLoader.h"
#include "../src/afx/draw/dev/AvxDevKit.h"

SGL GLenum AfxToGlFrontFace(avxFrontFace ff);
SGL GLenum AfxToGlCullMode(avxCullMode cm);
SGL GLenum AfxToGlFillMode(avxFillMode pm);
SGL GLenum SglToGlCompareOp(avxCompareOp co);
SGL GLuint AfxToGlShaderStage(avxShaderStage s);
SGL GLuint AfxToGlShaderStageBit(avxShaderStage s);
SGL GLenum AfxToGlTopology(avxTopology pm);
SGL GLenum AfxToGlBufferUsage(afxInt bt);
SGL GLenum AfxToGlStencilOp(avxStencilOp so);
SGL GLenum AfxToGlBlendOp(avxBlendOp f);
SGL GLenum AfxToGlBlendFactor(avxBlendFactor f);
SGL void AfxToGlVertexFormat(afxVertexFormat fmt, GLint* siz, GLenum* typ, afxNat32* stride);
SGL GLenum AfxToGlLogicOp(avxLogicOp f);
SGL void SglToGlFormat(afxPixelFormat pf, GLint *fmt, GLenum *layout, GLenum *type);
SGL GLenum SglToGlImageDimensions(afxNat d);
SGL GLenum SglToGlTexelFilterMode(avxTexelFilter tfm);
SGL GLenum SglToGlTexelFilterModeMipmapped(avxTexelFilter min, avxTexelFilter mip);
SGL GLenum SglToGlTexWrapMode(avxTexelAddress twm);
SGL GLenum SglToGlColorSwizzle(avxColorSwizzle swizzle);
SGL void SglToGlColorSwizzling(avxColorSwizzling const *swizzling, GLenum *arrayedSwizzles);
SGL void SglDetermineGlTargetInternalFormatType(afxRaster tex, GLenum *target, GLint *intFmt, GLenum *fmt, GLenum *type);

#endif//AVX_TO_GL_H
