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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "../dev/AvxImplKit.h"

afxNat32 const sRgb8ToRealLut[256] =
// lookup table for unorm8 srgb -> float conversion
{
    0x00000000, 0x399f22b4, 0x3a1f22b4, 0x3a6eb40f, 0x3a9f22b4, 0x3ac6eb61, 0x3aeeb40f, 0x3b0b3e5e, 0x3b1f22b4, 0x3b33070b, 0x3b46eb61, 0x3b5b518d, 0x3b70f18d, 0x3b83e1c6, 0x3b8fe616, 0x3b9c87fd,
    0x3ba9c9b5, 0x3bb7ad6f, 0x3bc63549, 0x3bd5635f, 0x3be539c1, 0x3bf5ba70, 0x3c0373b5, 0x3c0c6152, 0x3c15a703, 0x3c1f45be, 0x3c293e6b, 0x3c3391f7, 0x3c3e4149, 0x3c494d43, 0x3c54b6c7, 0x3c607eb1,
    0x3c6ca5dc, 0x3c792d22, 0x3c830aa8, 0x3c89af9f, 0x3c9085db, 0x3c978dc5, 0x3c9ec7c0, 0x3ca63431, 0x3cadd37d, 0x3cb5a601, 0x3cbdac20, 0x3cc5e639, 0x3cce54ab, 0x3cd6f7d3, 0x3cdfd00e, 0x3ce8ddb9,
    0x3cf22131, 0x3cfb9ac6, 0x3d02a56c, 0x3d0798df, 0x3d0ca7e7, 0x3d11d2b0, 0x3d171965, 0x3d1c7c31, 0x3d21fb3c, 0x3d2796b2, 0x3d2d4ebe, 0x3d332384, 0x3d39152e, 0x3d3f23e6, 0x3d454fd4, 0x3d4b991f,
    0x3d51ffef, 0x3d58846a, 0x3d5f26b7, 0x3d65e6fe, 0x3d6cc564, 0x3d73c20f, 0x3d7add25, 0x3d810b66, 0x3d84b795, 0x3d887330, 0x3d8c3e4a, 0x3d9018f6, 0x3d940345, 0x3d97fd4a, 0x3d9c0716, 0x3da020bb,
    0x3da44a4b, 0x3da883d7, 0x3daccd70, 0x3db12728, 0x3db59110, 0x3dba0b38, 0x3dbe95b5, 0x3dc33092, 0x3dc7dbe2, 0x3dcc97b6, 0x3dd1641f, 0x3dd6412c, 0x3ddb2eef, 0x3de02d77, 0x3de53cd5, 0x3dea5d19,
    0x3def8e55, 0x3df4d093, 0x3dfa23e8, 0x3dff8861, 0x3e027f07, 0x3e054282, 0x3e080ea5, 0x3e0ae379, 0x3e0dc107, 0x3e10a755, 0x3e13966c, 0x3e168e53, 0x3e198f11, 0x3e1c98ae, 0x3e1fab32, 0x3e22c6a3,
    0x3e25eb09, 0x3e29186c, 0x3e2c4ed2, 0x3e2f8e45, 0x3e32d6c8, 0x3e362865, 0x3e398322, 0x3e3ce706, 0x3e405419, 0x3e43ca62, 0x3e4749e8, 0x3e4ad2b1, 0x3e4e64c6, 0x3e52002b, 0x3e55a4e9, 0x3e595307,
    0x3e5d0a8b, 0x3e60cb7c, 0x3e6495e0, 0x3e6869bf, 0x3e6c4720, 0x3e702e08, 0x3e741e7f, 0x3e78188c, 0x3e7c1c38, 0x3e8014c2, 0x3e82203c, 0x3e84308d, 0x3e8645ba, 0x3e885fc5, 0x3e8a7eb2, 0x3e8ca283,
    0x3e8ecb3d, 0x3e90f8e1, 0x3e932b74, 0x3e9562f8, 0x3e979f71, 0x3e99e0e2, 0x3e9c274e, 0x3e9e72b7, 0x3ea0c322, 0x3ea31892, 0x3ea57308, 0x3ea7d289, 0x3eaa3718, 0x3eaca0b7, 0x3eaf0f69, 0x3eb18333,
    0x3eb3fc16, 0x3eb67a15, 0x3eb8fd34, 0x3ebb8576, 0x3ebe12e1, 0x3ec0a571, 0x3ec33d2d, 0x3ec5da17, 0x3ec87c33, 0x3ecb2383, 0x3ecdd00b, 0x3ed081cd, 0x3ed338cc, 0x3ed5f50b, 0x3ed8b68d, 0x3edb7d54,
    0x3ede4965, 0x3ee11ac1, 0x3ee3f16b, 0x3ee6cd67, 0x3ee9aeb6, 0x3eec955d, 0x3eef815d, 0x3ef272ba, 0x3ef56976, 0x3ef86594, 0x3efb6717, 0x3efe6e02, 0x3f00bd2b, 0x3f02460c, 0x3f03d1a5, 0x3f055ff8,
    0x3f06f106, 0x3f0884cf, 0x3f0a1b57, 0x3f0bb49d, 0x3f0d50a2, 0x3f0eef69, 0x3f1090f2, 0x3f123540, 0x3f13dc53, 0x3f15862d, 0x3f1732cf, 0x3f18e23b, 0x3f1a9471, 0x3f1c4973, 0x3f1e0143, 0x3f1fbbe1,
    0x3f217950, 0x3f23398f, 0x3f24fca2, 0x3f26c288, 0x3f288b43, 0x3f2a56d5, 0x3f2c253f, 0x3f2df681, 0x3f2fca9e, 0x3f31a197, 0x3f337b6c, 0x3f355820, 0x3f3737b3, 0x3f391a26, 0x3f3aff7e, 0x3f3ce7b7,
    0x3f3ed2d4, 0x3f40c0d6, 0x3f42b1c0, 0x3f44a592, 0x3f469c4d, 0x3f4895f3, 0x3f4a9284, 0x3f4c9203, 0x3f4e9470, 0x3f5099cd, 0x3f52a21a, 0x3f54ad59, 0x3f56bb8c, 0x3f58ccb3, 0x3f5ae0cf, 0x3f5cf7e2,
    0x3f5f11ee, 0x3f612ef2, 0x3f634eef, 0x3f6571ec, 0x3f6797e1, 0x3f69c0d8, 0x3f6beccb, 0x3f6e1bc2, 0x3f704db6, 0x3f7282b1, 0x3f74baae, 0x3f76f5b3, 0x3f7933b9, 0x3f7b74cb, 0x3f7db8e0, 0x3f800000,
};

#define PFD(fmt_, bpp_, stride_, compCnt_, bpcR_, bpcG_, bpcB_, bpcA_, flags_) pfds[fmt_] = (avxFormatDescription){ .bpp = bpp_, .stride = stride_, .compCnt = compCnt_, .bpc[0] = bpcR_, .bpc[1] = bpcG_, .bpc[2] = bpcB_, .bpc[3] = bpcA_, .flags = flags_ }
#define PFD_COMPRESSED(fmt_, bpp_, stride_, compCnt_, bpc, swizzle, defaults, normalized, tof, features) pfds[fmt_] = (avxFormatDescription){ .bpp = bpp_, .stride = stride_, .compCnt = compCnt_ }

_AVX avxFormatDescription pfds[avxFormat_TOTAL] =
{
    {
        .bpp = 0,
        .stride = 0,
        .compCnt = 0,
        .type = { avxFormatComponent_UNK, avxFormatComponent_UNK, avxFormatComponent_UNK, avxFormatComponent_UNK },
        .swizzle = { 0, 0, 0, 0 },
        .defaults = { 0, 0, 0, 0 },
        .isNormalized = { 0, 0, 0, 0 },
        .tof = { 0, 0, 0, 0 },
        .bpc = { 0, 0, 0, 0 },
        .bcWh = { 0, 0 },
        .flags = NIL
    },
};

#if 0
avxFormatDescription const pfds[] =
{
    {
        .type = { afxPixelElementType_FLOAT, afxPixelElementType_FLOAT, afxPixelElementType_FLOAT, afxPixelElementType_FLOAT },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .swizzle = { 0, 1, 2, 3 },
        .bpc = { 32, 32, 32, 32 },
        .bpp = 128,
        .bypp = 16,
        .compCnt = 4,
        .isSrgb = FALSE,
        .isBc = FALSE,
        .isSubsampled = FALSE,
        .isLuminance = FALSE,
        .isNormalized = { FALSE, FALSE, FALSE, FALSE },
        .tof = { 1.f, 1.f, 1.f, 1.f },
        .bcWh = { 1, 1 }
    },
    {
        .type = { afxPixelElementType_SINT, afxPixelElementType_SINT, afxPixelElementType_SINT, afxPixelElementType_SINT },
        .defaults = { 0, 0, 0, 0x1 },
        .swizzle = { 0, 1, 2, 3 },
        .bpc = { 32, 32, 32, 32 },
        .bpp = 128,
        .bypp = 16,
        .compCnt = 4,
        .isSrgb = FALSE,
        .isBc = FALSE,
        .isSubsampled = FALSE,
        .isLuminance = FALSE,
        .isNormalized = { FALSE, FALSE, FALSE, FALSE },
        .tof = { 1.f, 1.f, 1.f, 1.f },
        .bcWh = { 1, 1 }
    },
    {
        .type = { afxPixelElementType_UINT, afxPixelElementType_UINT, afxPixelElementType_UINT, afxPixelElementType_UINT },
        .defaults = { 0, 0, 0, 0x1 },
        .swizzle = { 0, 1, 2, 3 },
        .bpc = { 32, 32, 32, 32 },
        .bpp = 128,
        .bypp = 16,
        .compCnt = 4,
        .isSrgb = FALSE,
        .isBc = FALSE,
        .isSubsampled = FALSE,
        .isLuminance = FALSE,
        .isNormalized = { FALSE, FALSE, FALSE, FALSE },
        .tof = { 1.f, 1.f, 1.f, 1.f },
        .bcWh = { 1, 1 }
    }
};
#endif

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
    GL_R8, // avxFormat_R8
    GL_RG8, // avxFormat_RG8
    GL_RGB8, // avxFormat_RGB8
    GL_RGBA8, // avxFormat_RGBA8
    GL_RGB565, // avxFormat_R5G6B5
    GL_SRGB8_ALPHA8, // avxFormat_RGBA8_SRGB
    GL_RGB5_A1, // avxFormat_RGB5A1
    GL_RGBA4, // avxFormat_RGBA4
    GL_RGB10_A2, // avxFormat_RGB10A2


    // sampleable
    GL_R8_SNORM, // avxFormat_R8_SN
    GL_RG8_SNORM, // avxFormat_RG8_SN
    GL_RGB8_SNORM, // avxFormat_RGB8_SN
    GL_RGBA8_SNORM, // avxFormat_RGBA8_SN

    GL_R16F, // avxFormat_R16R
    GL_RG16F, // avxFormat_RG16R
    GL_RGB16F, // avxFormat_RGB16R
    GL_RGBA16F, // avxFormat_RGBA16R

    GL_SRGB8, // avxFormat_RGB8_SRGB
    GL_RGB9_E5, // avxFormat_RGB9E5
    GL_R11F_G11F_B10F, // avxFormat_RG11B10R

    // drawable
    GL_R8UI, // avxFormat_R8U
    GL_RG8UI, // avxFormat_RG8U
    GL_RGBA8UI, // avxFormat_RGBA8U

    GL_R8I, // avxFormat_R8I
    GL_RG8I, // avxFormat_RG8I
    GL_RGBA8I, // avxFormat_RGBA8I

    GL_R16UI, // avxFormat_R16_UN
    GL_RG16UI, // avxFormat_RG16_UN
    GL_RGBA16UI, // avxFormat_RGBA16_UN

    GL_R16I, // avxFormat_R16I
    GL_RG16I, // avxFormat_RG16I
    GL_RGBA16I, // avxFormat_RGBA16I

    GL_R32UI, // avxFormat_R32U
    GL_RG32UI, // avxFormat_RG32U
    GL_RGBA32UI, // avxFormat_RGBA32U

    GL_R32I, // avxFormat_R32I
    GL_RG32I, // avxFormat_RG32I
    GL_RGBA32I, // avxFormat_RGBA32I

    GL_RGB10_A2UI, // avxFormat_RGB10A2U

    // drawable special
    GL_DEPTH_COMPONENT16, // avxFormat_D16
    GL_DEPTH_COMPONENT24, // avxFormat_D24
    GL_DEPTH_COMPONENT32F, // avxFormat_D32R
    GL_DEPTH24_STENCIL8, // avxFormat_D24S8
    GL_DEPTH32F_STENCIL8, // avxFormat_D32RS8

    // ?
    GL_R32F, // avxFormat_R32R
    GL_RG32F, // avxFormat_RG32R
    GL_RGB32F, // avxFormat_RGB32R
    GL_RGBA32F, // avxFormat_RGBA32R

    GL_RGB8UI,
    GL_RGB8I,

    GL_RGB16UI,
    GL_RGB16I,

    GL_RGB32UI,
    GL_RGB32I,

};
#endif

#if 0
afxPixelLayout layouts[] =
{
    {   0, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // reserved for NIL argumentation giving system a chance to a best choise when user doesn't care about pixel format robustness.

    {   8, {  8,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_R8,
    {  16, {  8,  8,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_RG8,
    {  24, {  8,  8,  8,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGB8,
    {  32, {  8,  8,  8,  8 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGBA8,

    {  16, {  8,  8,  0,  0 }, {  0,  8,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_GR8,
    {  24, {  8,  8,  8,  0 }, {  0,  8, 16,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_BGR8,
    {  32, {  8,  8,  8,  8 }, {  0,  8, 16, 24 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_BGRA8,

    {  32, { 32,  0,  0,  0 }, {  0,  0,  0,  0 }, 1, 0, 0, 0, 0, 0, 0 }, // avxFormat_R32R,
    {  64, { 32, 32,  0,  0 }, {  0,  0,  0,  0 }, 1, 0, 0, 0, 0, 0, 0 }, // avxFormat_RG32R,
    {  96, { 32, 32, 32,  0 }, {  0,  0,  0,  0 }, 1, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGB32R,
    { 128, { 32, 32, 32, 32 }, {  0,  0,  0,  0 }, 1, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGBA32R,

    {  24, {  8,  8,  8,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGB8_SRGB,
    {  32, {  8,  8,  8,  8 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGBA8_SRGB,
    {  32, {  9,  9,  9,  5 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGB9E5,

    {  16, {  4,  4,  4,  4 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGBA4,
    {  16, {  5,  5,  5,  1 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGB5A1,
    {  16, {  5,  6,  5,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_R5G6B5,
    {  32, { 10, 10, 10,  2 }, {  0,  0,  0,  0 }, 0, 0, 0, 0, 0, 0, 0 }, // avxFormat_RGB10A2,

    {   8, {  8,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 1, 0, 0, 0, 0 }, // avxFormat_S8,
    {  16, { 16,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 1, 0, 0, 0, 0, 0 }, // avxFormat_D16,
    {  24, { 24,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 1, 0, 0, 0, 0, 0 }, // avxFormat_D24,
    {  32, { 32,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 1, 0, 0, 0, 0, 0 }, // avxFormat_D32,
    {  32, { 32,  0,  0,  0 }, {  0,  0,  0,  0 }, 1, 1, 0, 0, 0, 0, 0 }, // avxFormat_D32F,
    {  32, { 24,  8,  0,  0 }, {  0,  0,  0,  0 }, 0, 1, 1, 0, 0, 0, 0 }, // avxFormat_D24S8,
    {  40, { 32,  8,  0,  0 }, {  0,  0,  0,  0 }, 1, 1, 1, 0, 0, 0, 0 }, // avxFormat_D32FS8,

    {  64, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 1, 0, 0, 0 }, // avxFormat_RGB_DXT1,
    {  64, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 1, 0, 0, 0 }, // avxFormat_RGBA_DXT1,
    { 128, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 1, 0, 0, 0 }, // avxFormat_RGBA_DXT3,
    { 128, {  0,  0,  0,  0 }, {  0,  0,  0,  0 }, 0, 0, 0, 1, 0, 0, 0 }, // avxFormat_RGBA_DXT5,
};
#endif

_AVXINL void AfxDescribePixelFormat(avxFormat fmt, avxFormatDescription* pfd)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(pfd);
    AfxAssert(avxFormat_TOTAL > fmt);

    PFD(avxFormat_RGB8un, 24, 3, 3, 8, 8, 8, 0, avxFormatFlag_DRAW);
    PFD(avxFormat_BGR8un, 24, 3, 3, 8, 8, 8, 0, avxFormatFlag_DRAW);

    PFD(avxFormat_RGBA8un, 32, 4, 4, 8, 8, 8, 8, avxFormatFlag_DRAW);
    PFD(avxFormat_BGRA8un, 32, 4, 4, 8, 8, 8, 8, avxFormatFlag_DRAW);
    PFD(avxFormat_ABGR8un, 32, 4, 4, 8, 8, 8, 8, avxFormatFlag_DRAW);

    PFD(avxFormat_RGBA4un, 16, 2, 4, 4, 4, 4, 4, avxFormatFlag_DRAW);

    PFD_COMPRESSED(avxFormat_DXT1un, 16, 8, 3, 0, 0, 0, 0, 0, 0);


    *pfd = pfds[fmt];
}

_AVXINL afxNat AfxGetBpp(avxFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(avxFormat_TOTAL > fmt);
    
    PFD(avxFormat_RGB8un, 24, 3, 3, 8, 8, 8, 0, avxFormatFlag_DRAW);
    PFD(avxFormat_BGR8un, 24, 3, 3, 8, 8, 8, 0, avxFormatFlag_DRAW);

    PFD(avxFormat_RGBA8un, 32, 4, 4, 8, 8, 8, 8, avxFormatFlag_DRAW);
    PFD(avxFormat_BGRA8un, 32, 4, 4, 8, 8, 8, 8, avxFormatFlag_DRAW);
    PFD(avxFormat_ABGR8un, 32, 4, 4, 8, 8, 8, 8, avxFormatFlag_DRAW);

    PFD(avxFormat_RGBA4un, 16, 2, 4, 4, 4, 4, 4, avxFormatFlag_DRAW);
    

    return pfds[fmt].bpp;
}

#define PFD_D(fmt_,bpp_,stride_, bpc_) pfds[fmt_] = (avxFormatDescription){ .bpp = bpp_, .stride = stride_, .compCnt = 1, .bpc[0] = bpc_, .flags = avxFormatFlag_DEPTH }
#define PFD_S(fmt_,bpp_,stride_, bpc_) pfds[fmt_] = (avxFormatDescription){ .bpp = bpp_, .stride = stride_, .compCnt = 1, .bpc[0] = bpc_, .flags = avxFormatFlag_STENCIL }
#define PFD_DS(fmt_,bpp_,stride_, bpcD_, bpcS_) pfds[fmt_] = (avxFormatDescription){ .bpp = bpp_, .stride = stride_, .compCnt = 2, .bpc[0] = bpcD_, .bpc[1] = bpcS_, .flags = avxFormatFlag_DEPTH | avxFormatFlag_STENCIL }

_AVXINL afxBool AfxIsPixelFormatCombinedDepthStencil(avxFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(avxFormat_TOTAL > fmt);

    PFD_D(avxFormat_D16un, 16, 2, 16);
    PFD_D(avxFormat_X8D24un, 32, 4, 24);
    PFD_D(avxFormat_D32f, 32, 4, 32);
    PFD_S(avxFormat_S8u, 8, 1, 8);
    PFD_DS(avxFormat_D16unS8u, 24, 3, 16, 8);
    PFD_DS(avxFormat_D24unS8u, 32, 4, 24, 8);
    PFD_DS(avxFormat_D32fS8u, 64, 8, 32, 8);

    switch (fmt)
    {
    case avxFormat_D16unS8u:
    case avxFormat_D24unS8u:
    case avxFormat_D32fS8u:
        return TRUE;
    default: break;
    }
    return FALSE;
}

_AVXINL afxBool AfxIsPixelFormatDepth(avxFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(avxFormat_TOTAL > fmt);

    switch (fmt)
    {
    case avxFormat_D16un:
    case avxFormat_X8D24un:
    case avxFormat_D32f:
    case avxFormat_D16unS8u:
    case avxFormat_D24unS8u:
    case avxFormat_D32fS8u:
        return TRUE;
    default: break;
    }
    return FALSE;
}

_AVXINL afxBool AfxIsPixelFormatStencil(avxFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(avxFormat_TOTAL > fmt);

    switch (fmt)
    {
    case avxFormat_S8u:
    case avxFormat_D16unS8u:
    case avxFormat_D24unS8u:
    case avxFormat_D32fS8u:
        return TRUE;
    default: break;
    }
    return FALSE;
}

_AVXINL afxBool AfxIsSrgb(avxFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(avxFormat_TOTAL > fmt);
    return pfds[fmt].flags & avxFormatFlag_sRGB;
}

_AVXINL afxBool AfxIsPixelFormatCompressed(avxFormat fmt)
{
    afxError err = NIL;
    AfxAssert(fmt);
    AfxAssert(avxFormat_TOTAL > fmt);
    return fmt >= avxFormat_DXT1un && fmt <= avxFormat_ASTC_12x12_sRGB;
}
