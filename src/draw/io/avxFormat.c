/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "../impl/avxImplementation.h"

afxUnit32 const sRgb8ToRealLut[256] =
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

_AVX avxFormatDescription const _AvxStdPfds[avxFormat_TOTAL] =
{
    {
        .bpp = 0,
        .stride = 0,
        .compCnt = 0,
        .type = { avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 0, 0, 0 },
        .defaults = { 0, 0, 0, 0 },
        .isNormalized = { 0, 0, 0, 0 },
        .tof = { 0, 0, 0, 0 },
        .bpc = { 0, 0, 0, 0 },
        .bcWh = { 0, 0 },
        .flags = NIL
    },

    ////////////////////////////////////////////////////////////////////////////
    // 8-bit precise sRGB

    [avxFormat_RGBA8v] = // R8G8B8A8_UNORM_SRGB (0xC8)
    {
        .name = AFX_STRING("RGBA8v"),
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UN},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 2, 3},
        .bpc = {8, 8, 8, 8},
        .bpp = 32,
        .stride = 4,
        .compCnt = 4,
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = {1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR | avxFormatFlag_sRGB,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_BGRA8v] =
    {
        .name = AFX_STRING("BGRA8v"),
        .bpp = 32,
        .stride = 4,
        .compCnt = 4,
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UN},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {2, 1, 0, 3},
        .bpc = {8, 8, 8, 8},
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = {1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR | avxFormatFlag_sRGB,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_BGR8v] = // B8G8R8X8_UNORM_SRGB (0xEA)
    {
        .name = AFX_STRING("BGR8v"),
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UNUSED},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {2, 1, 0, 3},
        .bpc = {8, 8, 8, 8},
        .bpp = 24,
        .stride = 4,
        .compCnt = 3,
        .isNormalized = { TRUE, TRUE, TRUE, FALSE },
        .tof = {1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR | avxFormatFlag_sRGB
    },

    ////////////////////////////////////////////////////////////////////////////
    // 8-bit precise unsigned normalized integer

    [avxFormat_R8un] = // R8_UNORM (0x140)
    {
        .name = AFX_STRING("R8un"),
        .type = {avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 0, 0, 0},
        .bpc = {8, 0, 0, 0},
        .bpp = 8,
        .stride = 1,
        .compCnt = 1,
        .isNormalized = { TRUE, TRUE, TRUE, FALSE },
        .tof = {1.0f / 255.0f, 0, 0, 0},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_BLIT_SRC
    },
    [avxFormat_RG8un] = // R8G8_UNORM (0x106)
    {
        .name = AFX_STRING("RG8un"),
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UN},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 0, 0},
        .bpc = {8, 8, 8, 8},
        .bpp = 16,
        .stride = 2,
        .compCnt = 2,
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = {1.0f / 255.0f, 1.0f / 255.0f, 0, 0 },
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_RGBA8un] = // R8G8B8A8_UNORM (0xC7)
    {
        .name = AFX_STRING("RGBA8un"),
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UN},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 2, 3},
        .bpc = {8, 8, 8, 8},
        .bpp = 32,
        .stride = 4,
        .compCnt = 4,
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = {1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_BGRA8un] = // B8G8R8A8_UNORM (0xC0)
    {
        .name = AFX_STRING("BGRA8un"),
        .bpp = 32,
        .stride = 4,
        .compCnt = 4,
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UN},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {2, 1, 0, 3},
        .bpc = {8, 8, 8, 8},
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = {1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_BGR8un] = // B8G8R8X8_UNORM (0xE9)
    {
        .name = AFX_STRING("BGR8un"),
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UNUSED},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {2, 1, 0, 3},
        .bpc = {8, 8, 8, 8},
        .bpp = 24,
        .stride = 4,
        .compCnt = 3,
        .isNormalized = { TRUE, TRUE, TRUE, FALSE },
        .tof = {1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR
    },

    ////////////////////////////////////////////////////////////////////////////
    // 8-bit precise signed normalized integer

    [avxFormat_R8sn] = // R8_SNORM (0x141)
    {
        .name = AFX_STRING("R8sn"),
        .type = {avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 0, 0, 0},
        .bpc = {8, 0, 0, 0},
        .bpp = 8,
        .stride = 1,
        .compCnt = 1,
        .isNormalized = { TRUE, TRUE, TRUE, FALSE },
        .tof = {1.0f / 127.f, 0, 0, 0},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT_SRC
    },
    [avxFormat_RG8sn] = // R8G8_SNORM (0x107)
    {
        .name = AFX_STRING("RG8sn"),
        .type = {avxFormatType_SN, avxFormatType_SN, avxFormatType_SN, avxFormatType_SN},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 0, 0},
        .bpc = {8, 8, 0, 0},
        .bpp = 16,
        .stride = 2,
        .compCnt = 2,
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = {1.0f / 127.0f, 1.0f / 127.0f, 0, 0 },
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_RGBA8sn] = // R8G8B8A8_SNORM (0xc9)
    {
        .name = AFX_STRING("RGBA8sn"),
        .type = {avxFormatType_SN, avxFormatType_SN, avxFormatType_SN, avxFormatType_SN},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 2, 3},
        .bpc = {8, 8, 8, 8},
        .bpp = 32,
        .stride = 4,
        .compCnt = 4,
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = {1.0f / 127.0f, 1.0f / 127.0f, 1.0f / 127.0f, 1.0f / 127.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    
    ////////////////////////////////////////////////////////////////////////////
    // 16-bit precise unsigned normalized integer

    [avxFormat_R16un] = // R16_UNORM (0x10a)
    {
        .name = AFX_STRING("R16un"),
        .type = {avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 0, 0, 0},
        .bpc = {16, 0, 0, 0},
        .bpp = 16,
        .stride = 2,
        .compCnt = 1,
        .isNormalized = {TRUE, TRUE, TRUE, TRUE},
        .tof = {1.0f / 65535.0f, 0, 0, 0 },
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT_SRC
    },
    [avxFormat_RG16un] = // R16G16_UNORM (0xcc)
    {
        .name = AFX_STRING("RG16un"),
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 0, 0, 0},
        .bpc = {16, 16, 0, 0},
        .bpp = 32,
        .stride = 4,
        .compCnt = 2,
        .isNormalized = {TRUE, TRUE, TRUE, TRUE},
        .tof = {1.0f / 65535.0f, 1.0f / 65535.0f, 0, 0 },
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT_SRC
    },

    ////////////////////////////////////////////////////////////////////////////
    // 16-bit precise signed normalized integer

    [avxFormat_R16sn] = // R16_SNORM (0x10b)
    {
        .name = AFX_STRING("R16sn"),
        .type = {avxFormatType_SN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 0, 0, 0},
        .bpc = {16, 0, 0, 0},
        .bpp = 16,
        .stride = 2,
        .compCnt = 1,
        .isNormalized = {TRUE, TRUE, TRUE, TRUE},
        .tof = {1.0f / 32767.0f, 0, 0, 0 },
        .bcWh = { 1, 1 },
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT_SRC
    },
    [avxFormat_RG16sn] = // R16G16_SNORM (0xcd)
    {
        .name = AFX_STRING("RG16sn"),
        .type = {avxFormatType_SN, avxFormatType_SN, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 0, 0, 0},
        .bpc = {16, 16, 0, 0},
        .bpp = 32,
        .stride = 4,
        .compCnt = 2,
        .isNormalized = {TRUE, TRUE, TRUE, TRUE},
        .tof = {1.0f / 32767.0f, 1.0f / 32767.0f, 0, 0 },
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT_SRC | avxFormatCap_DRAW | avxFormatCap_BLEND
    },

    ////////////////////////////////////////////////////////////////////////////
    // 16-bit precise floating point    

    [avxFormat_RG16f] = // R16G16_FLOAT (0xD0)
    {
        .name = AFX_STRING("RG16f"),
        .type = {avxFormatType_F, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 0, 0},
        .bpc = {16, 16, 0, 0},
        .bpp = 32,
        .stride = 4,
        .compCnt = 2,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.f, 0.f, 0.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_RGBA16f] = // R16G16B16A16_FLOAT (0x84)
    {
        .name = AFX_STRING("RGBA16f"),
        .type = {avxFormatType_F, avxFormatType_F, avxFormatType_F, avxFormatType_F},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 2, 3},
        .bpc = {16, 16, 16, 16},
        .bpp = 64,
        .stride = 8,
        .compCnt = 4,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.f, 1.f, 1.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },

    ////////////////////////////////////////////////////////////////////////////
    // 32-bit precise floating point

    [avxFormat_R32f] = // R32_FLOAT (0xD8)
    {
        .name = AFX_STRING("R32f"),
        .type = {avxFormatType_F, avxFormatType_F, avxFormatType_F, avxFormatType_F},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 0, 0, 0},
        .bpc = {32, 0, 0, 0},
        .bpp = 32,
        .stride = 4,
        .compCnt = 1,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 0.f, 0.f, 0.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_RG32f] = // R32G32_FLOAT (0x85)
    {
        .name = AFX_STRING("RG32f"),
        .type = {avxFormatType_F, avxFormatType_F, avxFormatType_F, avxFormatType_F},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 0, 0},
        .bpc = {32, 32, 0, 0},
        .bpp = 64,
        .stride = 8,
        .compCnt = 2,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.0f, 0.f, 0.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_RGB32f] = // R32G32B32_FLOAT (0x40)
    {
        .name = AFX_STRING("RGB32f"),
        .type = {avxFormatType_F, avxFormatType_F, avxFormatType_F, avxFormatType_F},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 2, 0},
        .bpc = {32, 32, 32, 0},
        .bpp = 96,
        .stride = 12,
        .compCnt = 3,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.0f, 1.0f, 0.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },
    [avxFormat_RGBA32f] =// R32G32B32A32_FLOAT (0x0)
    {
        .name = AFX_STRING("RGBA32f"),
        .type = {avxFormatType_F, avxFormatType_F, avxFormatType_F, avxFormatType_F},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 1, 2, 3},
        .bpc = {32, 32, 32, 32},
        .bpp = 128,
        .stride = 16,
        .compCnt = 4,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.0f, 1.0f, 1.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW | avxFormatCap_BLEND
    },

    ////////////////////////////////////////////////////////////////////////////
    // 32-bit precise unsigned integer

    [avxFormat_R32u] = // R32_UINT (0xD7)
    {
        .name = AFX_STRING("R32u"),
        .type = {avxFormatType_U, avxFormatType_U, avxFormatType_U, avxFormatType_U},
        .defaults = {0, 0, 0, 0x1},
        .swizzle = {0, 0, 0, 0},
        .bpc = {32, 0, 0, 0},
        .bpp = 32,
        .stride = 4,
        .compCnt = 1,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 0.f, 0.f, 0.f},
        .bcWh = { 1, 1 },
        .rasterCaps = avxFormatCap_COPY
    },
    [avxFormat_RG32u] = // R32G32_UINT (0x87)
    {
        .name = AFX_STRING("RG32u"),
        .type = {avxFormatType_U, avxFormatType_U, avxFormatType_U, avxFormatType_U},
        .defaults = {0, 0, 0, 0x1},
        .swizzle = {0, 1, 0, 0},
        .bpc = {32, 32, 0, 0},
        .bpp = 64,
        .stride = 8,
        .compCnt = 2,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.0f, 0.f, 0.f},
        .bcWh = { 1, 1 },
        .rasterCaps = avxFormatCap_COPY
    },
    [avxFormat_RGB32u] = // R32G32B32_UINT (0x42)
    {
        .name = AFX_STRING("RGB32u"),
        .type = {avxFormatType_U, avxFormatType_U, avxFormatType_U, avxFormatType_U},
        .defaults = {0, 0, 0, 0x1},
        .swizzle = {0, 1, 2, 0},
        .bpc = {32, 32, 32, 0},
        .bpp = 96,
        .stride = 12,
        .compCnt = 3,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.0f, 1.0f, 0.f},
        .bcWh = { 1, 1 },
        .rasterCaps = avxFormatCap_COPY
    },
    [avxFormat_RGBA32u] = // R32G32B32A32_UINT (0x2)
    {
        .name = AFX_STRING("RGBA32u"),
        .type = {avxFormatType_U, avxFormatType_U, avxFormatType_U, avxFormatType_U},
        .defaults = {0, 0, 0, 0x1},
        .swizzle = {0, 1, 2, 3},
        .bpc = {32, 32, 32, 32},
        .bpp = 128,
        .stride = 16,
        .compCnt = 4,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.0f, 1.0f, 1.0f},
        .bcWh = { 1, 1 },
        .rasterCaps = avxFormatCap_COPY
    },

    ////////////////////////////////////////////////////////////////////////////
    // 32-bit precise signed integer

    [avxFormat_R32i] = // R32_SINT (0xD6)
    {
        .name = AFX_STRING("R32i"),
        .type = {avxFormatType_I, avxFormatType_I, avxFormatType_I, avxFormatType_I},
        .defaults = {0, 0, 0, 0x1},
        .swizzle = {0, 0, 0, 0},
        .bpc = {32, 0, 0, 0},
        .bpp = 32,
        .stride = 4,
        .compCnt = 1,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 0.f, 0.f, 0.f},
        .bcWh = { 1, 1 },
        .rasterCaps = avxFormatCap_COPY
    },
    [avxFormat_RG32i] = // R32G32_SINT (0x86)
    {
        .name = AFX_STRING("RG32i"),
        .type = {avxFormatType_I, avxFormatType_I, avxFormatType_I, avxFormatType_I},
        .defaults = {0, 0, 0, 0x1},
        .swizzle = {0, 1, 0, 0},
        .bpc = {32, 32, 0, 0},
        .bpp = 64,
        .stride = 8,
        .compCnt = 2,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.0f, 0.f, 0.f},
        .bcWh = { 1, 1 },
        .rasterCaps = avxFormatCap_COPY
    },
    [avxFormat_RGB32i] = // R32G32B32_SINT (0x41)
    {
        .name = AFX_STRING("RGB32i"),
        .type = {avxFormatType_I, avxFormatType_I, avxFormatType_I, avxFormatType_I},
        .defaults = {0, 0, 0, 0x1},
        .swizzle = {0, 1, 2, 0},
        .bpc = {32, 32, 32, 0},
        .bpp = 96,
        .stride = 12,
        .compCnt = 3,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.0f, 1.0f, 0.f},
        .bcWh = { 1, 1 },
        .rasterCaps = avxFormatCap_COPY
    },
    [avxFormat_RGBA32i] = // R32G32B32A32_SINT (0x1)
    {
        .name = AFX_STRING("RGBA32i"),
        .type = {avxFormatType_I, avxFormatType_I, avxFormatType_I, avxFormatType_I},
        .defaults = {0, 0, 0, 0x1},
        .swizzle = {0, 1, 2, 3},
        .bpc = {32, 32, 32, 32},
        .bpp = 128,
        .stride = 16,
        .compCnt = 4,
        .isNormalized = {FALSE, FALSE, FALSE, FALSE},
        .tof = {1.0f, 1.0f, 1.0f, 1.0f},
        .bcWh = { 1, 1 },
        .rasterCaps = avxFormatCap_COPY
    },

    

    ////////////////////////////////////////////////////////////////////////////
    // 4-bit precise unsigned normalized integer

    [avxFormat_BGRA4un] = // B4G4R4A4_UNORM (0x104)
    {
        .name = AFX_STRING("BGRA4un"),
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UN},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {2, 1, 0, 3},
        .bpc = {4, 4, 4, 4},
        .bpp = 16,
        .stride = 2,
        .compCnt = 4,
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = {1.0f / 15.0f, 1.0f / 15.0f, 1.0f / 15.0f, 1.0f / 15.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_BLIT | avxFormatCap_DRAW
    },
            
    ////////////////////////////////////////////////////////////////////////////
    // 5/6-bit precise unsigned normalized integer

    [avxFormat_B5G6R5un] = // B5G6R5_UNORM (0x100)
    {
        .name = AFX_STRING("B5G6R5un"),
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},                         // Defaults for missing components
        .swizzle = {2, 1, 0, 0},
        .bpc = {5, 6, 5, 0},
        .bpp = 16,
        .stride = 2,
        .compCnt = 3,
        .isNormalized = { TRUE, TRUE, TRUE, FALSE },
        .tof = {1.0f / 31.0f, 1.0f / 63.0f, 1.0f / 31.0f, 0},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_BLIT | avxFormatCap_DRAW
    },

    ////////////////////////////////////////////////////////////////////////////
    // 5/1-bit precise unsigned normalized integer

    [avxFormat_BGR5A1un] = // B5G5R5A1_UNORM (0x102)
    {
        .name = AFX_STRING("BGR5A1un"),
        .type = {avxFormatType_UN, avxFormatType_UN, avxFormatType_UN, avxFormatType_UN},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {2, 1, 0, 3},
        .bpc = {5, 5, 5, 1},
        .bpp = 16,
        .stride = 2,
        .compCnt = 4,
        .isNormalized = {TRUE, TRUE, TRUE, TRUE },
        .tof = {1.0f / 31.0f, 1.0f / 31.0f, 1.0f / 31.0f, 1.0f / 1.0f},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_COLOR,
        .rasterCaps = avxFormatCap_BLIT | avxFormatCap_DRAW
    },
    
    ///////////////////////////////////////////////////////////////////////////
    // DEPTH/STENCIL CAPABLE FORMATS                                         //
    ///////////////////////////////////////////////////////////////////////////
    
    [avxFormat_S8u] =
    {
        .name = AFX_STRING("S8u"),
        .stride = 1,
        .compCnt = 1,
        .bpc = {8, 0, 0, 0},
        .bpp = 8,
        .type = {avxFormatType_U, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x1},
        .swizzle = {0, 0, 0, 0},
        .isNormalized = { FALSE, FALSE, FALSE, FALSE },
        .tof = {1.0f, 0, 0, 0},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_STENCIL | avxFormatFlag_LUMINANCE,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_DRAW
    },
    [avxFormat_D16un] =
    {
        .name = AFX_STRING("D16un"),
        .stride = 2,
        .compCnt = 1,
        .bpc = {16, 0, 0, 0},
        .bpp = 16,
        .type = {avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 0, 0, 0},
        .isNormalized = { TRUE, FALSE, FALSE, FALSE },
        .tof = {1.0f / 65535.0f, 0, 0, 0},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_DEPTH,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW
    },
    [avxFormat_D16unS8u] =
    {
        .name = AFX_STRING("D16unS8u"),
        .stride = 3,
        .compCnt = 2,
        .bpc = {16, 8, 0, 0},
        .bpp = 24,
        .type = { avxFormatType_UN, avxFormatType_U, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = {0, 1, 2, 3},
        .defaults = {0, 0, 0x1, 0x3f800000},
        .isNormalized = { TRUE, FALSE, FALSE, FALSE },
        .tof = {1.0f / 65535.0f, 1.f, 0, 0},
        .bcWh = { 1, 1 },
        .flags = 0, // UNSUPPORTED IN GL // avxFormatFlag_DEPTH | avxFormatFlag_STENCIL,
        .rasterCaps = 0, // UNSUPPORTED IN GL // avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW
    },
    [avxFormat_X8D24un] = // R24_UNORM_X8_TYPELESS (0xD9)
    {
        .name = AFX_STRING("X8D24un"),
        .stride = 4,
        .compCnt = 1,
        .bpc = {24, 0, 0, 0},
        .bpp = 32,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = {0, 1, 2, 3},
        .defaults = {0, 0, 0, 0x3f800000},
        .isNormalized = { TRUE, FALSE, FALSE, FALSE },
        .tof = {1.0f / 16777215.0f, 0, 0, 0},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_DEPTH,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_DRAW
    },
    [avxFormat_D24unS8u] =
    {
        .name = AFX_STRING("D24unS8u"),
        .stride = 4,
        .compCnt = 2,
        .bpc = {24, 8, 0, 0},
        .bpp = 32,
        .type = { avxFormatType_UN, avxFormatType_U, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = {0, 1, 2, 3},
        .defaults = {0, 0, 0x1, 0x3f800000},
        .isNormalized = { TRUE, FALSE, FALSE, FALSE },
        .tof = {1.0f / 16777215.0f, 1.f, 0, 0},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_DEPTH | avxFormatFlag_STENCIL,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW
    },
    [avxFormat_D32f] =
    {
        .name = AFX_STRING("D32f"),
        .stride = 4,
        .compCnt = 1,
        .bpc = {32, 0, 0, 0},
        .bpp = 32,
        .type = {avxFormatType_F, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = {0, 0, 0, 0x3f800000},
        .swizzle = {0, 0, 0, 0},
        .isNormalized = { FALSE, FALSE, FALSE, FALSE },
        .tof = {1.0f, 0, 0, 0},
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_DEPTH,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW
    },
    [avxFormat_D32fS8u] =
    {
        .name = AFX_STRING("D32fS8u"),
        .stride = 8,
        .compCnt = 2,
        .bpc = { 32, 8, 0, 0 },
        .bpp = 64,
        .type = {avxFormatType_F, avxFormatType_U, avxFormatType_UNK, avxFormatType_UNK},
        .defaults = { 0, 0, 0x1, 0x3f800000},
        .swizzle = { 0, 0, 0, 0 },
        .isNormalized = { TRUE, FALSE, FALSE, FALSE },
        .tof = { 1.0f, 1.0f, 0, 0 },
        .bcWh = { 1, 1 },
        .flags = avxFormatFlag_DEPTH | avxFormatFlag_STENCIL,
        .rasterCaps = avxFormatCap_COPY | avxFormatCap_BLIT | avxFormatCap_DRAW
    },
    
    ///////////////////////////////////////////////////////////////////////////
    // BLOCK-COMPRESSED FORMATS                                              //
    ///////////////////////////////////////////////////////////////////////////

    [avxFormat_DXT1un] =
    {
        .name = AFX_STRING("DXT1"),
        .bpp = 64,
        .stride = 8,
        .compCnt = 1,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 1, 2, 3 },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .isNormalized = { TRUE, 0, 0, 0 },
        .tof = { 1.0f / 255.0f, 0, 0, 0 },
        .bpc = { 8, 8, 8, 8 },
        .bcWh = { 4, 4 },
        .flags = avxFormatFlag_COMPRESSED
    },
    [avxFormat_DXT1v] =
    {
        .name = AFX_STRING("DXT1v"),
        .bpp = 64,
        .stride = 8,
        .compCnt = 1,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 1, 2, 3 },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .isNormalized = { TRUE, 0, 0, 0 },
        .tof = { 1.0f / 255.0f, 0, 0, 0 },
        .bpc = { 8, 8, 8, 8 },
        .bcWh = { 4, 4 },
        .flags = avxFormatFlag_COMPRESSED | avxFormatFlag_sRGB
    },
    [avxFormat_DXT1A] =
    {
        .name = AFX_STRING("DXT1A"),
        .bpp = 64,
        .stride = 8,
        .compCnt = 1,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 1, 2, 3 },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .isNormalized = { TRUE, 0, 0, 0 },
        .tof = { 1.0f / 255.0f, 0, 0, 0 },
        .bpc = { 8, 8, 8, 8 },
        .bcWh = { 4, 4 },
        .flags = avxFormatFlag_COMPRESSED
    },
    [avxFormat_DXT3un] =
    {
        .name = AFX_STRING("DXT3"),
        .bpp = 128,
        .stride = 16,
        .compCnt = 1,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 1, 2, 3 },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .isNormalized = { TRUE, 0, 0, 0 },
        .tof = { 1.0f / 255.0f, 0, 0, 0 },
        .bpc = { 8, 8, 8, 8 },
        .bcWh = { 4, 4 },
        .flags = avxFormatFlag_COMPRESSED
    },
    [avxFormat_DXT3v] =
    {
        .name = AFX_STRING("DXT3v"),
        .bpp = 128,
        .stride = 16,
        .compCnt = 1,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 1, 2, 3 },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .isNormalized = { TRUE, 0, 0, 0 },
        .tof = { 1.0f / 255.0f, 0, 0, 0 },
        .bpc = { 8, 8, 8, 8 },
        .bcWh = { 4, 4 },
        .flags = avxFormatFlag_COMPRESSED | avxFormatFlag_sRGB
    },
    [avxFormat_DXT5un] =
    {
        .name = AFX_STRING("DXT5"),
        .bpp = 128,
        .stride = 16,
        .compCnt = 1,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 1, 2, 3 },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .isNormalized = { TRUE, 0, 0, 0 },
        .tof = { 1.0f / 255.0f, 0, 0, 0 },
        .bpc = { 8, 8, 8, 8 },
        .bcWh = { 4, 4 },
        .flags = avxFormatFlag_COMPRESSED
    },
    [avxFormat_DXT5v] =
    {
        .name = AFX_STRING("DXT5v"),
        .bpp = 128,
        .stride = 16,
        .compCnt = 1,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 1, 2, 3 },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .isNormalized = { TRUE, 0, 0, 0 },
        .tof = { 1.0f / 255.0f, 0, 0, 0 },
        .bpc = { 8, 8, 8, 8 },
        .bcWh = { 4, 4 },
        .flags = avxFormatFlag_COMPRESSED | avxFormatFlag_sRGB
    },
    [avxFormat_RGTC1un] =
    {
        .name = AFX_STRING("RGTC1un"),
        .bpp = 64,
        .stride = 8,
        .compCnt = 1,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 1, 2, 3 },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .isNormalized = { TRUE, 0, 0, 0 },
        .tof = { 1.0f / 255.0f, 0, 0, 0 },
        .bpc = { 8, 8, 8, 8 },
        .bcWh = { 4, 4 },
        .flags = avxFormatFlag_COMPRESSED
    },
    [avxFormat_RGTC2un] =
    {
        .name = AFX_STRING("RGTC2un"),
        .bpp = 128,
        .stride = 16,
        .compCnt = 1,
        .type = { avxFormatType_UN, avxFormatType_UNK, avxFormatType_UNK, avxFormatType_UNK },
        .swizzle = { 0, 1, 2, 3 },
        .defaults = { 0, 0, 0, 0x3f800000 },
        .isNormalized = { TRUE, 0, 0, 0 },
        .tof = { 1.0f / 255.0f, 0, 0, 0 },
        .bpc = { 8, 8, 8, 8 },
        .bcWh = { 4, 4 },
        .flags = avxFormatFlag_COMPRESSED
    },
};

_AVXINL afxUnit AvxChooseFormats(avxFormatDescription const* pfd, afxUnit maxCnt, avxFormat formats[])
{
    afxError err = NIL;
    AFX_ASSERT(formats);
    AFX_ASSERT(maxCnt);
    AFX_ASSERT(pfd);
    afxUnit rslt = 0;

    afxUnit bpp = (!pfd->bpp && pfd->stride) ? pfd->stride / AFX_BYTE_SIZE : pfd->bpp;
    afxUnit stride = (!pfd->stride && bpp) ? bpp / AFX_BYTE_SIZE : pfd->stride;

    for (avxFormat i = 0; i < avxFormat_TOTAL; i++)
    {
        if (maxCnt == rslt)
            break;

        avxFormatDescription const* ref = &_AvxStdPfds[i];

        if ((bpp && bpp != ref->bpp) ||
            (stride && stride != ref->stride))
            continue;

        if (((ref->flags & pfd->flags) != pfd->flags) ||
            ((ref->rasterCaps & pfd->rasterCaps) != pfd->rasterCaps) ||
            ((ref->bufferCaps & pfd->bufferCaps) != pfd->bufferCaps))
            continue;

#if 0
        if ((pfd->compCnt && pfd->compCnt != ref->compCnt) ||
            (pfd->bpc[0] && pfd->bpc[0] != ref->bpc[0]) ||
            (pfd->bpc[1] && pfd->bpc[1] != ref->bpc[1]) ||
            (pfd->bpc[2] && pfd->bpc[2] != ref->bpc[2]) ||
            (pfd->bpc[3] && pfd->bpc[3] != ref->bpc[3]) ||
            (pfd->swizzle[0] && pfd->swizzle[0] != ref->swizzle[0]) ||
            (pfd->swizzle[1] && pfd->swizzle[1] != ref->swizzle[1]) ||
            (pfd->swizzle[2] && pfd->swizzle[2] != ref->swizzle[2]) ||
            (pfd->swizzle[3] && pfd->swizzle[3] != ref->swizzle[3]) ||
            (pfd->type[0] && pfd->type[0] != ref->type[0]) ||
            (pfd->type[1] && pfd->type[1] != ref->type[1]) ||
            (pfd->type[2] && pfd->type[2] != ref->type[2]) ||
            (pfd->type[3] && pfd->type[3] != ref->type[3]))
            continue;
#else
        if ((pfd->compCnt && pfd->compCnt > ref->compCnt))
            continue;
        else
        {
            afxBool skip = FALSE;

            for (afxUnit j = 0; j < 4; j++)
            {
                if (pfd->compCnt && j == pfd->compCnt)
                    break;

                if (/*pfd->swizzle[j] && */(pfd->swizzle[j] != ref->swizzle[j]))
                {
                    skip = TRUE;
                    break;
                }

                if (pfd->bpc[j] && (pfd->bpc[j] != ref->bpc[j]))
                {
                    skip = TRUE;
                    break;
                }

                if (pfd->type[j] && (pfd->type[j] != ref->type[j]))
                {
                    skip = TRUE;
                    break;
                }
            }

            if (skip)
                continue;
        }
#endif
        if ((pfd->bcWh[0] && pfd->bcWh[0] != ref->bcWh[0]) ||
            (pfd->bcWh[1] && pfd->bcWh[1] != ref->bcWh[1]))
            continue;

        formats[rslt] = i;
        ++rslt;

    }
    return rslt;
}

_AVXINL void AvxDescribeFormats(afxUnit cnt, avxFormat const formats[], avxFormatDescription pfd[])
{
    afxError err = NIL;
    AFX_ASSERT(formats);
    AFX_ASSERT(pfd);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        avxFormat fmt = formats[i];
        AFX_ASSERT(avxFormat_TOTAL > fmt);
        fmt = AFX_MIN(fmt, avxFormat_TOTAL - 1);
        pfd[i] = _AvxStdPfds[fmt];
    }
}

_AVXINL afxUnit AfxGetBpp(avxFormat fmt)
{
    afxError err = NIL;
    AFX_ASSERT(fmt);
    AFX_ASSERT(avxFormat_TOTAL > fmt);
    
    return _AvxStdPfds[fmt].bpp;
}

_AVXINL afxBool AfxIsPixelFormatCombinedDepthStencil(avxFormat fmt)
{
    afxError err = NIL;
    AFX_ASSERT(fmt);
    AFX_ASSERT(avxFormat_TOTAL > fmt);

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
    AFX_ASSERT(fmt);
    AFX_ASSERT(avxFormat_TOTAL > fmt);

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
    return _AvxStdPfds[fmt].flags & avxFormatFlag_DEPTH;
}

_AVXINL afxBool AfxIsPixelFormatStencil(avxFormat fmt)
{
    afxError err = NIL;
    AFX_ASSERT(fmt);
    AFX_ASSERT(avxFormat_TOTAL > fmt);

    switch (fmt)
    {
    case avxFormat_S8u:
    case avxFormat_D16unS8u:
    case avxFormat_D24unS8u:
    case avxFormat_D32fS8u:
        return TRUE;
    default: break;
    }
    return _AvxStdPfds[fmt].flags & avxFormatFlag_STENCIL;
}

_AVXINL afxBool AfxIsSrgb(avxFormat fmt)
{
    afxError err = NIL;
    AFX_ASSERT(fmt);
    AFX_ASSERT(avxFormat_TOTAL > fmt);
    return _AvxStdPfds[fmt].flags & avxFormatFlag_sRGB;
}

_AVXINL afxBool AfxIsPixelFormatCompressed(avxFormat fmt)
{
    afxError err = NIL;
    AFX_ASSERT(fmt);
    AFX_ASSERT(avxFormat_TOTAL > fmt);
    return _AvxStdPfds[fmt].flags & avxFormatFlag_COMPRESSED || (fmt >= avxFormat_DXT1un && fmt <= avxFormat_ASTC12v);
}

_AVXINL afxUnit _AvxGetFormatRowStride(avxFormat fmt, afxUnit width)
{
    avxFormatDescription const* pfd = &_AvxStdPfds[fmt];
    
    // Test if it is a block-compressed format.
    if (pfd->bcWh[0] > 1 || pfd->bcWh[1] > 1)
    {
        afxUnit bw = pfd->bcWh[0];
        afxUnit wblocks = (width + bw - 1) / bw;
        return wblocks * pfd->stride;
    }
    return width * pfd->stride;
}
