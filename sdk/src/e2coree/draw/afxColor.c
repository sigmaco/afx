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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "afx/draw/afxColor.h"

_AFX afxColorSwizzling const AFX_STD_COLOR_SWIZZLING =
{
    AFX_COLOR_SIZZLE_R,
    AFX_COLOR_SIZZLE_G,
    AFX_COLOR_SIZZLE_B,
    AFX_COLOR_SIZZLE_A,
    
};

#if 0
struct
{
    afxNat stride; // bytes per pixel
    afxNat bits[4]; // bits per component
    afxNat shift[4];
    afxBool real;
    afxBool normalized;
    afxBool drawable;
    afxBool sampleable;
    afxBool depth;
    afxBool stencil;
    afxBool compressed;
}
lays[] =
{
    { },

    // sampleable & drawable
    GL_R8, // AFX_PFD_R8
    GL_RG8, // AFX_PFD_RG8
    GL_RGB8, // AFX_PFD_RGB8
    GL_RGBA8, // AFX_PFD_RGBA8
    GL_RGB565, // AFX_PFD_R5G6B5
    GL_SRGB8_ALPHA8, // AFX_PFD_RGBA8_SRGB
    GL_RGB5_A1, // AFX_PFD_RGB5A1
    GL_RGBA4, // AFX_PFD_RGBA4
    GL_RGB10_A2, // AFX_PFD_RGB10A2

    
    // sampleable
    GL_R8_SNORM, // AFX_PFD_R8_SN
    GL_RG8_SNORM, // AFX_PFD_RG8_SN
    GL_RGB8_SNORM, // AFX_PFD_RGB8_SN
    GL_RGBA8_SNORM, // AFX_PFD_RGBA8_SN

    GL_R16F, // AFX_PFD_R16R
    GL_RG16F, // AFX_PFD_RG16R
    GL_RGB16F, // AFX_PFD_RGB16R
    GL_RGBA16F, // AFX_PFD_RGBA16R

    GL_SRGB8, // AFX_PFD_RGB8_SRGB
    GL_RGB9_E5, // AFX_PFD_RGB9E5
    GL_R11F_G11F_B10F, // AFX_PFD_RG11B10R

    // drawable
    GL_R8UI, // AFX_PFD_R8U
    GL_RG8UI, // AFX_PFD_RG8U
    GL_RGBA8UI, // AFX_PFD_RGBA8U

    GL_R8I, // AFX_PFD_R8I
    GL_RG8I, // AFX_PFD_RG8I
    GL_RGBA8I, // AFX_PFD_RGBA8I

    GL_R16UI, // AFX_PFD_R16_UN
    GL_RG16UI, // AFX_PFD_RG16_UN
    GL_RGBA16UI, // AFX_PFD_RGBA16_UN
    
    GL_R16I, // AFX_PFD_R16I
    GL_RG16I, // AFX_PFD_RG16I
    GL_RGBA16I, // AFX_PFD_RGBA16I

    GL_R32UI, // AFX_PFD_R32U
    GL_RG32UI, // AFX_PFD_RG32U
    GL_RGBA32UI, // AFX_PFD_RGBA32U

    GL_R32I, // AFX_PFD_R32I
    GL_RG32I, // AFX_PFD_RG32I
    GL_RGBA32I, // AFX_PFD_RGBA32I

    GL_RGB10_A2UI, // AFX_PFD_RGB10A2U

    // drawable special
    GL_DEPTH_COMPONENT16, // AFX_PFD_D16
    GL_DEPTH_COMPONENT24, // AFX_PFD_D24
    GL_DEPTH_COMPONENT32F, // AFX_PFD_D32R
    GL_DEPTH24_STENCIL8, // AFX_PFD_D24S8
    GL_DEPTH32F_STENCIL8, // AFX_PFD_D32RS8

    // ?
    GL_R32F, // AFX_PFD_R32R
    GL_RG32F, // AFX_PFD_RG32R
    GL_RGB32F, // AFX_PFD_RGB32R
    GL_RGBA32F, // AFX_PFD_RGBA32R

    GL_RGB8UI,
    GL_RGB8I,

    GL_RGB16UI,
    GL_RGB16I,

    GL_RGB32UI,
    GL_RGB32I,

};
#endif

afxPixelLayout layouts[] =
{
    {   0, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // reserved for NIL argumentation giving system a chance to a best choise when user doesn't care about pixel format robustness.

    {   8, {  8,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_R8,
    {  16, {  8,  8,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RG8,
    {  24, {  8,  8,  8,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGB8,
    {  32, {  8,  8,  8,  8 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGBA8,

    {  16, {  8,  8,  0,  0 }, {  0,  8,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_GR8,
    {  24, {  8,  8,  8,  0 }, {  0,  8, 16,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_BGR8,
    {  32, {  8,  8,  8,  8 }, {  0,  8, 16, 24 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_BGRA8,

    {  32, { 32,  0,  0,  0 }, {  0,  0,  0,  0 }, 1, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_R32R,
    {  64, { 32, 32,  0,  0 }, {  0,  0,  0,  0 }, 1, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RG32R,
    {  96, { 32, 32, 32,  0 }, {  0,  0,  0,  0 }, 1, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGB32R,
    { 128, { 32, 32, 32, 32 }, {  0,  0,  0,  0 }, 1, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGBA32R,

    {  24, {  8,  8,  8,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGB8_SRGB,
    {  32, {  8,  8,  8,  8 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGBA8_SRGB,
    {  32, {  9,  9,  9,  5 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGB9E5,

    {  16, {  4,  4,  4,  4 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGBA4,
    {  16, {  5,  5,  5,  1 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGB5A1,
    {  16, {  5,  6,  5,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_R5G6B5,
    {  32, { 10, 10, 10,  2 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // AFX_PFD_RGB10A2,

    {   8, {  8,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 1, 0, 0, 0, 0 }, // AFX_PFD_S8,
    {  16, { 16,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 1, 0, 0, 0, 0, 0 }, // AFX_PFD_D16,
    {  24, { 24,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 1, 0, 0, 0, 0, 0 }, // AFX_PFD_D24,
    {  32, { 32,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 1, 0, 0, 0, 0, 0 }, // AFX_PFD_D32,
    {  32, { 32,  0,  0,  0 }, {  0,  0,  0,  0 }, 1, 1, 0, 0, 0, 0, 0 }, // AFX_PFD_D32F,
    {  32, { 24,  8,  0,  0 }, {  0,  0,  0,  0 }, 0, 1, 1, 0, 0, 0, 0 }, // AFX_PFD_D24S8,
    {  40, { 32,  8,  0,  0 }, {  0,  0,  0,  0 }, 1, 1, 1, 0, 0, 0, 0 }, // AFX_PFD_D32FS8,
    
    {  64, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 1, 0, 0, 0 }, // AFX_PFD_RGB_DXT1,
    {  64, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 1, 0, 0, 0 }, // AFX_PFD_RGBA_DXT1,
    { 128, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 1, 0, 0, 0 }, // AFX_PFD_RGBA_DXT3,
    { 128, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 1, 0, 0, 0 }, // AFX_PFD_RGBA_DXT5,
};

_AFXINL void AfxDescribePixelFormat(afxPixelFormat fmt, afxPixelLayout *layout)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(layout);
    AfxAssert(AFX_PFD_TOTAL > fmt);
    *layout = layouts[fmt];
}

_AFXINL afxBool AfxPixelFormatIsCombinedDepthStencil(afxPixelFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(AFX_PFD_TOTAL > fmt);
    return layouts[fmt].depth && layouts[fmt].stencil;
}

_AFXINL afxBool AfxPixelFormatIsDepth(afxPixelFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(AFX_PFD_TOTAL > fmt);
    return layouts[fmt].depth;
}
_AFXINL afxBool AfxPixelFormatIsStencil(afxPixelFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(AFX_PFD_TOTAL > fmt);
    return layouts[fmt].stencil;
}

_AFXINL afxBool AfxPixelFormatIsReal(afxPixelFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(AFX_PFD_TOTAL > fmt);
    return layouts[fmt].real;
}

_AFXINL afxBool AfxPixelFormatIsSrgb(afxPixelFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(AFX_PFD_TOTAL > fmt);
    return fmt == AFX_PFD_RGB8_SRGB || fmt == AFX_PFD_RGBA8_SRGB ;
}

_AFXINL afxBool AfxPixelFormatIsCompressed(afxPixelFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(AFX_PFD_TOTAL > fmt);
    return layouts[fmt].block;
}

_AFX afxReal AfxLinearToSrgba(afxReal theLinearValue)
{
    return theLinearValue <= 0.0031308f ? theLinearValue * 12.92f : powf(theLinearValue, 1.0f / 2.4f) * 1.055f - 0.055f;
}

_AFX afxReal AfxSrgbToLinear(afxReal thesRGBValue)
{
    return thesRGBValue <= 0.04045f ? thesRGBValue / 12.92f : powf((thesRGBValue + 0.055f) / 1.055f, 2.4f);
}
