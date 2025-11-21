/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Video Graphics Extensions & Experiments.

#include "avxIcd.h"

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
        .tag = AFX_STRING("RGBA8v"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_BGRA8v] =
    {
        .tag = AFX_STRING("BGRA8v"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_BGR8v] = // B8G8R8X8_UNORM_SRGB (0xEA)
    {
        .tag = AFX_STRING("BGR8v"),
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
        .tag = AFX_STRING("R8un"),
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
        .rasCaps = avxFormatUsage_BLIT_SRC
    },
    [avxFormat_RG8un] = // R8G8_UNORM (0x106)
    {
        .tag = AFX_STRING("RG8un"),
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
        .rasCaps = avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_RGBA8un] = // R8G8B8A8_UNORM (0xC7)
    {
        .tag = AFX_STRING("RGBA8un"),
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
        .rasCaps = avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_BGRA8un] = // B8G8R8A8_UNORM (0xC0)
    {
        .tag = AFX_STRING("BGRA8un"),
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
        .rasCaps = avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_BGR8un] = // B8G8R8X8_UNORM (0xE9)
    {
        .tag = AFX_STRING("BGR8un"),
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
        .tag = AFX_STRING("R8sn"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT_SRC
    },
    [avxFormat_RG8sn] = // R8G8_SNORM (0x107)
    {
        .tag = AFX_STRING("RG8sn"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_RGBA8sn] = // R8G8B8A8_SNORM (0xc9)
    {
        .tag = AFX_STRING("RGBA8sn"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    
    ////////////////////////////////////////////////////////////////////////////
    // 16-bit precise unsigned normalized integer

    [avxFormat_R16un] = // R16_UNORM (0x10a)
    {
        .tag = AFX_STRING("R16un"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT_SRC
    },
    [avxFormat_RG16un] = // R16G16_UNORM (0xcc)
    {
        .tag = AFX_STRING("RG16un"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT_SRC
    },

    ////////////////////////////////////////////////////////////////////////////
    // 16-bit precise signed normalized integer

    [avxFormat_R16sn] = // R16_SNORM (0x10b)
    {
        .tag = AFX_STRING("R16sn"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT_SRC
    },
    [avxFormat_RG16sn] = // R16G16_SNORM (0xcd)
    {
        .tag = AFX_STRING("RG16sn"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT_SRC | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },

    ////////////////////////////////////////////////////////////////////////////
    // 16-bit precise floating point    

    [avxFormat_RG16f] = // R16G16_FLOAT (0xD0)
    {
        .tag = AFX_STRING("RG16f"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_RGBA16f] = // R16G16B16A16_FLOAT (0x84)
    {
        .tag = AFX_STRING("RGBA16f"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },

    ////////////////////////////////////////////////////////////////////////////
    // 32-bit precise floating point

    [avxFormat_R32f] = // R32_FLOAT (0xD8)
    {
        .tag = AFX_STRING("R32f"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_RG32f] = // R32G32_FLOAT (0x85)
    {
        .tag = AFX_STRING("RG32f"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_RGB32f] = // R32G32B32_FLOAT (0x40)
    {
        .tag = AFX_STRING("RGB32f"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },
    [avxFormat_RGBA32f] =// R32G32B32A32_FLOAT (0x0)
    {
        .tag = AFX_STRING("RGBA32f"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW | avxFormatUsage_BLEND
    },

    ////////////////////////////////////////////////////////////////////////////
    // 32-bit precise unsigned integer

    [avxFormat_R32u] = // R32_UINT (0xD7)
    {
        .tag = AFX_STRING("R32u"),
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
        .rasCaps = avxFormatUsage_COPY
    },
    [avxFormat_RG32u] = // R32G32_UINT (0x87)
    {
        .tag = AFX_STRING("RG32u"),
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
        .rasCaps = avxFormatUsage_COPY
    },
    [avxFormat_RGB32u] = // R32G32B32_UINT (0x42)
    {
        .tag = AFX_STRING("RGB32u"),
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
        .rasCaps = avxFormatUsage_COPY
    },
    [avxFormat_RGBA32u] = // R32G32B32A32_UINT (0x2)
    {
        .tag = AFX_STRING("RGBA32u"),
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
        .rasCaps = avxFormatUsage_COPY
    },

    ////////////////////////////////////////////////////////////////////////////
    // 32-bit precise signed integer

    [avxFormat_R32i] = // R32_SINT (0xD6)
    {
        .tag = AFX_STRING("R32i"),
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
        .rasCaps = avxFormatUsage_COPY
    },
    [avxFormat_RG32i] = // R32G32_SINT (0x86)
    {
        .tag = AFX_STRING("RG32i"),
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
        .rasCaps = avxFormatUsage_COPY
    },
    [avxFormat_RGB32i] = // R32G32B32_SINT (0x41)
    {
        .tag = AFX_STRING("RGB32i"),
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
        .rasCaps = avxFormatUsage_COPY
    },
    [avxFormat_RGBA32i] = // R32G32B32A32_SINT (0x1)
    {
        .tag = AFX_STRING("RGBA32i"),
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
        .rasCaps = avxFormatUsage_COPY
    },

    

    ////////////////////////////////////////////////////////////////////////////
    // 4-bit precise unsigned normalized integer

    [avxFormat_BGRA4un] = // B4G4R4A4_UNORM (0x104)
    {
        .tag = AFX_STRING("BGRA4un"),
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
        .rasCaps = avxFormatUsage_BLIT | avxFormatUsage_DRAW
    },
            
    ////////////////////////////////////////////////////////////////////////////
    // 5/6-bit precise unsigned normalized integer

    [avxFormat_B5G6R5un] = // B5G6R5_UNORM (0x100)
    {
        .tag = AFX_STRING("B5G6R5un"),
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
        .rasCaps = avxFormatUsage_BLIT | avxFormatUsage_DRAW
    },

    ////////////////////////////////////////////////////////////////////////////
    // 5/1-bit precise unsigned normalized integer

    [avxFormat_BGR5A1un] = // B5G5R5A1_UNORM (0x102)
    {
        .tag = AFX_STRING("BGR5A1un"),
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
        .rasCaps = avxFormatUsage_BLIT | avxFormatUsage_DRAW
    },
    
    ///////////////////////////////////////////////////////////////////////////
    // DEPTH/STENCIL CAPABLE FORMATS                                         //
    ///////////////////////////////////////////////////////////////////////////
    
    [avxFormat_S8u] =
    {
        .tag = AFX_STRING("S8u"),
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
        .flags = avxFormatFlag_STENCIL | avxFormatFlag_LUMA,
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_DRAW
    },
    [avxFormat_D16un] =
    {
        .tag = AFX_STRING("D16un"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW
    },
    [avxFormat_D16unS8u] =
    {
        .tag = AFX_STRING("D16unS8u"),
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
        .rasCaps = 0, // UNSUPPORTED IN GL // avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW
    },
    [avxFormat_X8D24un] = // R24_UNORM_X8_TYPELESS (0xD9)
    {
        .tag = AFX_STRING("X8D24un"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_DRAW
    },
    [avxFormat_D24unS8u] =
    {
        .tag = AFX_STRING("D24unS8u"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW
    },
    [avxFormat_D32f] =
    {
        .tag = AFX_STRING("D32f"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW
    },
    [avxFormat_D32fS8u] =
    {
        .tag = AFX_STRING("D32fS8u"),
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
        .rasCaps = avxFormatUsage_COPY | avxFormatUsage_BLIT | avxFormatUsage_DRAW
    },
    
    ///////////////////////////////////////////////////////////////////////////
    // BLOCK-COMPRESSED FORMATS                                              //
    ///////////////////////////////////////////////////////////////////////////

    [avxFormat_DXT1un] =
    {
        .tag = AFX_STRING("DXT1"),
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
        .flags = avxFormatFlag_BC
    },
    [avxFormat_DXT1v] =
    {
        .tag = AFX_STRING("DXT1v"),
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
        .flags = avxFormatFlag_BC | avxFormatFlag_sRGB
    },
    [avxFormat_DXT1A] =
    {
        .tag = AFX_STRING("DXT1A"),
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
        .flags = avxFormatFlag_BC
    },
    [avxFormat_DXT3un] =
    {
        .tag = AFX_STRING("DXT3"),
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
        .flags = avxFormatFlag_BC
    },
    [avxFormat_DXT3v] =
    {
        .tag = AFX_STRING("DXT3v"),
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
        .flags = avxFormatFlag_BC | avxFormatFlag_sRGB
    },
    [avxFormat_DXT5un] =
    {
        .tag = AFX_STRING("DXT5"),
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
        .flags = avxFormatFlag_BC
    },
    [avxFormat_DXT5v] =
    {
        .tag = AFX_STRING("DXT5v"),
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
        .flags = avxFormatFlag_BC | avxFormatFlag_sRGB
    },
    [avxFormat_RGTC1un] =
    {
        .tag = AFX_STRING("RGTC1un"),
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
        .flags = avxFormatFlag_BC
    },
    [avxFormat_RGTC2un] =
    {
        .tag = AFX_STRING("RGTC2un"),
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
        .flags = avxFormatFlag_BC
    },
};

_AVXINL void AvxDescribeFormats(afxUnit cnt, avxFormat const formats[], avxFormatDescription pfd[])
{
    afxError err = { 0 };
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

_AVXINL afxUnit AvxChooseFormats(avxFormatDescription const* pfd, afxUnit maxCnt, avxFormat formats[])
{
    afxError err = { 0 };
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
            ((ref->rasCaps & pfd->rasCaps) != pfd->rasCaps) ||
            ((ref->bufCaps & pfd->bufCaps) != pfd->bufCaps))
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

_AVXINL afxUnit AfxGetBpp(avxFormat fmt)
{
    afxError err = { 0 };
    AFX_ASSERT(fmt);
    AFX_ASSERT(avxFormat_TOTAL > fmt);
    
    return _AvxStdPfds[fmt].bpp;
}

_AVXINL afxBool AvxTestCombinedDsFormat(avxFormat fmt)
{
    afxError err = { 0 };
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

_AVXINL afxBool AvxTestDepthFormat(avxFormat fmt)
{
    afxError err = { 0 };
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

_AVXINL afxBool AvxTestStencilFormat(avxFormat fmt)
{
    afxError err = { 0 };
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

_AVXINL afxBool AvxTestSrgbFormat(avxFormat fmt)
{
    afxError err = { 0 };
    AFX_ASSERT(fmt);
    AFX_ASSERT(avxFormat_TOTAL > fmt);
    return _AvxStdPfds[fmt].flags & avxFormatFlag_sRGB;
}

_AVXINL afxBool AvxTestCompressedFormat(avxFormat fmt)
{
    afxError err = { 0 };
    AFX_ASSERT(fmt);
    AFX_ASSERT(avxFormat_TOTAL > fmt);
    return _AvxStdPfds[fmt].flags & avxFormatFlag_BC || (fmt >= avxFormat_DXT1un && fmt <= avxFormat_ASTC12v);
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

void transcode_UN8x4_to_F32x4(afxUnit cnt, const void* srcBuf, void* dstBuf)
{
    const uint8_t* src = (const uint8_t*)srcBuf;
    float* dst = (float*)dstBuf;

    for (size_t i = 0; i < cnt; ++i)
    {
        dst[0] = src[0] / 255.0f;
        dst[1] = src[1] / 255.0f;
        dst[2] = src[2] / 255.0f;
        dst[3] = src[3] / 255.0f;

        src += 4;
        dst += 4;
    }
}

#define AFX_MAX_COMPONENTS 4

typedef struct avxTypeScheme {
    avxFormatType type[AFX_MAX_COMPONENTS];
    afxUnit32     bpc[AFX_MAX_COMPONENTS]; // Bits per component
    afxBool       normalized[AFX_MAX_COMPONENTS];
    afxUnit32     compCnt;
} avxTypeScheme;

uint64_t encode_scheme_key(const avxTypeScheme* scheme)
{
    /*
        To make lookups fast (no full struct comparison), we build a packed 64-bit key.
        This can represent up to 4 components, with:
            4 bits for type
            6 bits for bpc
            1 bit for normalization
    */

    uint64_t key = 0;

    for (int i = 0; i < scheme->compCnt; ++i)
    {
        key |= ((uint64_t)(scheme->type[i] & 0xF) << (i * 12 + 0));
        key |= ((uint64_t)(scheme->bpc[i] & 0x3F) << (i * 12 + 4));
        key |= ((uint64_t)(scheme->normalized[i] ? 1 : 0) << (i * 12 + 10));
    }

    key |= ((uint64_t)(scheme->compCnt & 0x7) << 60);
    return key;
}

typedef void(*avxTranscodeFn)(
    const void* src,
    void* dst,
    size_t count
    );


typedef struct avxTranscodeEntry {
    uint64_t fromKey;
    uint64_t toKey;
    avxTranscodeFn fn;
} avxTranscodeEntry;

#define MAX_TRANSCODE_ENTRIES 1024
static avxTranscodeEntry g_transcodeTable[MAX_TRANSCODE_ENTRIES];
static size_t g_transcodeTableSize = 0;

void register_transcoder(const avxTypeScheme* from, const avxTypeScheme* to, avxTranscodeFn fn)
{
    g_transcodeTable[g_transcodeTableSize++] = (avxTranscodeEntry) {
        .fromKey = encode_scheme_key(from),
            .toKey = encode_scheme_key(to),
            .fn = fn
    };
}

avxTranscodeFn find_transcoder(const avxTypeScheme* from, const avxTypeScheme* to)
{
    uint64_t fromKey = encode_scheme_key(from);
    uint64_t toKey = encode_scheme_key(to);

    for (size_t i = 0; i < g_transcodeTableSize; ++i)
    {
        if (g_transcodeTable[i].fromKey == fromKey &&
            g_transcodeTable[i].toKey == toKey)
        {
            return g_transcodeTable[i].fn;
        }
    }

    return NULL; // fallback
}

void transcode_8UN_to_16UN(const void* src, void* dst, afxUnit32 srcBpc, afxUnit32 dstBpc)
{
    uint32_t in = (srcBpc == 8) ? *(uint8_t*)src :
        (srcBpc == 16) ? *(uint16_t*)src :
        *(uint32_t*)src;

    uint32_t out;
    if (srcBpc == dstBpc) {
        out = in; // Same size, no conversion
    }
    else {
        uint32_t maxIn = (1u << srcBpc) - 1;
        uint32_t maxOut = (1u << dstBpc) - 1;
        out = (in * maxOut + maxIn / 2) / maxIn;
    }

    if (dstBpc == 8)      *(uint8_t*)dst = (uint8_t)out;
    else if (dstBpc == 16)*(uint16_t*)dst = (uint16_t)out;
    else                  *(uint32_t*)dst = out;
}

void transcode_UN_to_UN(const void* src, void* dst, afxUnit32 srcBpc, afxUnit32 dstBpc)
{
    uint32_t in = (srcBpc == 8) ? *(uint8_t*)src :
        (srcBpc == 16) ? *(uint16_t*)src :
        *(uint32_t*)src;

    uint32_t out;
    if (srcBpc == dstBpc) {
        out = in; // Same size, no conversion
    }
    else {
        uint32_t maxIn = (1u << srcBpc) - 1;
        uint32_t maxOut = (1u << dstBpc) - 1;
        out = (in * maxOut + maxIn / 2) / maxIn;
    }

    if (dstBpc == 8)      *(uint8_t*)dst = (uint8_t)out;
    else if (dstBpc == 16)*(uint16_t*)dst = (uint16_t)out;
    else                  *(uint32_t*)dst = out;
}

static inline avxClearValue decode_component(avxFormatDescription const* fmtd, const afxByte* ptr)
{
    avxClearValue out = { 0 };

    afxUnit compCnt = fmtd->compCnt;

    size_t offset = 0;

    // Step 1: Decode source components
    for (int i = 0; i < compCnt; ++i)
    {
        avxFormatType type = fmtd->type[i];
        afxBool norm = fmtd->isNormalized[i];
        afxUnit32 bpc = fmtd->bpc[i];
        offset += bpc / 8;

        switch (type)
        {
        case avxFormatType_UN:
        {
            uint32_t v = (bpc == 8) ? *(uint8_t*)&ptr[offset] : (bpc == 16) ? *(uint16_t*)&ptr[offset] : *(uint32_t*)&ptr[offset];
            out.rgban[i] = norm ? ((float)v) / ((1u << bpc) - 1) : (float)v;
            break;
        }
        case avxFormatType_SN:
        {
            int32_t v = (bpc == 8) ? *(int8_t*)&ptr[offset] : (bpc == 16) ? *(int16_t*)&ptr[offset] : *(int32_t*)&ptr[offset];
            out.rgbai[i] = norm ? fmaxf(-1.0f, (float)v / ((1 << (bpc - 1)) - 1)) : (float)v;
            break;
        }
        case avxFormatType_F:
            out.rgba[i] = *(float*)&ptr[offset];
            break;
        case avxFormatType_U:
            out.rgban[i] = (bpc == 8) ? *(uint8_t*)&ptr[offset] : (bpc == 16) ? *(uint16_t*)&ptr[offset] : *(uint32_t*)&ptr[offset];
            break;
        case avxFormatType_I:
            out.rgbai[i] = (bpc == 8) ? *(int8_t*)&ptr[offset] : (bpc == 16) ? *(int16_t*)&ptr[offset] : *(int32_t*)&ptr[offset];
            break;
        default:
            out.rgban[i] = 0;
            break;
        }
    }
    return out;
}

static void encode_component(avxFormatDescription const* fmtd, afxByte* dst, avxClearValue val)
{

    afxUnit compCnt = fmtd->compCnt;

    size_t offset = 0;

    for (int i = 0; i < compCnt; ++i)
    {
        avxFormatType type = fmtd->type[i];
        afxBool norm = fmtd->isNormalized[i];
        afxUnit32 bpc = fmtd->bpc[i];
        offset += bpc / 8;

        switch (type)
        {
        case avxFormatType_UN:
        {
            float f = norm ? fminf(fmaxf(val.rgba[i], 0.0f), 1.0f) : val.rgba[i];
            uint32_t u = (uint32_t)(f * ((1u << bpc) - 1) + 0.5f);

            if (bpc == 8)      *(uint8_t*)&dst[offset] = (uint8_t)u;
            else if (bpc == 16)*(uint16_t*)&dst[offset] = (uint16_t)u;
            else               *(uint32_t*)&dst[offset] = u;
            break;
        }
        case avxFormatType_SN:
        {
            float f = norm ? fminf(fmaxf(val.rgba[i], -1.0f), 1.0f) : val.rgba[i];
            int32_t i = (int32_t)(f * ((1 << (bpc - 1)) - 1) + 0.5f);

            if (bpc == 8)      *(int8_t*)&dst[offset] = (int8_t)i;
            else if (bpc == 16)*(int16_t*)&dst[offset] = (int16_t)i;
            else               *(int32_t*)&dst[offset] = i;
            break;
        }
        case avxFormatType_F:
            *(float*)dst = val.rgba[i];
            break;
        case avxFormatType_U:
            if (bpc == 8)      *(uint8_t*)&dst[offset] = (uint8_t)val.rgban[i];
            else if (bpc == 16)*(uint16_t*)&dst[offset] = (uint16_t)val.rgban[i];
            else               *(uint32_t*)&dst[offset] = val.rgban[i];
            break;
        case avxFormatType_I:
            if (bpc == 8)      *(int8_t*)&dst[offset] = (int8_t)val.rgbai[i];
            else if (bpc == 16)*(int16_t*)&dst[offset] = (int16_t)val.rgbai[i];
            else               *(int32_t*)&dst[offset] = val.rgbai[i];
            break;
        default:
            break;
        }
    }
}

inline void transcode_pixel(const uint8_t* src, const avxFormatDescription* srcFmt, uint8_t* dst, const avxFormatDescription* dstFmt)
{
    avxClearValue val = decode_component(srcFmt, src);
    encode_component(dstFmt, dst, val);
}

_AVX void AvxTranscodeFormat(avxFormat srcFmt, avxFormat dstFmt, afxUnit cnt, void const* srcBuf, void* dstBuf, afxUnit srcStride, afxUnit dstStride)
{
    afxError err = { 0 };
    avxFormatDescription const* srcDesc = &_AvxStdPfds[srcFmt];
    avxFormatDescription const* dstDesc = &_AvxStdPfds[dstFmt];

    const uint8_t* src = (const uint8_t*)srcBuf;
    uint8_t* dst = (uint8_t*)dstBuf;

    for (size_t i = 0; i < cnt; ++i)
    {
        transcode_pixel(src, srcDesc, dst, dstDesc);
        //src += srcDesc->stride;
        src += srcStride;
        //dst += dstDesc->stride;
        dst += dstStride;
    }
}

_AVX void AvxConvertFormat(afxUnit32 rowSize, afxUnit32 rowCnt,
    void const* srcBuffer, afxUnit32 rowStrideSrc, avxFormat srcFmt, avxFormat dstFmt,
    void* dstBuffer, afxUnit32 rowStrideDst)
{
    afxError err = { 0 };
    avxFormatDescription const* srcDesc = &_AvxStdPfds[srcFmt];
    avxFormatDescription const* dstDesc = &_AvxStdPfds[dstFmt];

    afxUnit32 srcPixelSize = srcDesc->stride;
    afxUnit32 dstPixelSize = dstDesc->stride;

    for (afxUnit32 y = 0; y < rowCnt; ++y)
    {
        uint8_t const* srcRow = ((uint8_t const*)srcBuffer) + y * rowStrideSrc;
        uint8_t* dstRow = ((uint8_t*)dstBuffer) + y * rowStrideDst;

        for (afxUnit32 x = 0; x < rowSize; ++x)
        {
            uint8_t const* srcPixel = srcRow + x * srcPixelSize;
            uint8_t* dstPixel = dstRow + x * dstPixelSize;

            float comp[4] = { 0 };

            // Load from source as float
            for (afxUnit32 i = 0; i < srcDesc->compCnt; ++i)
            {
                switch (srcDesc->type[i])
                {
                case avxFormatType_UN:
                {
                    afxUnit32 maxVal = (1u << srcDesc->bpc[i]) - 1;
                    comp[i] = ((float)((uint8_t*)srcPixel)[i]) / (float)maxVal;
                    break;
                }
                case avxFormatType_F:
                    comp[i] = ((float*)srcPixel)[i];
                    break;

                    // TODO: Handle U, I, SN, US, SS, etc.
                default:
                    comp[i] = 0.f;
                    break;
                }
            }

            // Fill missing components
            for (afxUnit32 i = srcDesc->compCnt; i < 4; ++i)
            {
                comp[i] = dstDesc->defaults[i] / 255.0f;
            }

            // Swizzle to destination order
            float reordered[4] = { 0 };
            for (afxUnit32 i = 0; i < dstDesc->compCnt; ++i)
            {
                reordered[i] = comp[dstDesc->swizzle[i]];
            }

            // Write to destination
            for (afxUnit32 i = 0; i < dstDesc->compCnt; ++i)
            {
                switch (dstDesc->type[i])
                {
                case avxFormatType_UN:
                {
                    afxUnit32 maxVal = (1u << dstDesc->bpc[i]) - 1;
                    ((uint8_t*)dstPixel)[i] = (uint8_t)(reordered[i] * maxVal + 0.5f);
                    break;
                }
                case avxFormatType_F:
                    ((float*)dstPixel)[i] = reordered[i];
                    break;

                    // TODO: Add rest of cases
                default:
                    ((uint8_t*)dstPixel)[i] = 0;
                    break;
                }
            }
        }
    }
}

_AVXINL afxBool AvxTestCompatibleFormats(avxFormat base, avxFormat other)
{
    afxError err;

    /*
        According to Vulkan, two formats are compatible if:
         - They have the same number of components
         - Each component has the same size (in bits)
         - The total bit size (BPP) matches
         - Component layout matches (e.g., same order/swizzle)
         - Same block size if compressed
         - (For most cases) only the type differs (UNORM vs SNORM vs UINT, etc.)
    */

    avxFormatDescription const* a = &_AvxStdPfds[base];
    avxFormatDescription const* b = &_AvxStdPfds[other];

    // Compressed formats are only compatible with themselves.
    if ((a->flags & avxFormatFlag_BC) || (b->flags & avxFormatFlag_BC))
    {
        return a == b; // Pointers must be equal, or you can check name/equivalence.
    }

    // Depth/stencil formats generally are not compatible with color formats.
    afxBool aIsDepthStencil = (a->flags & (avxFormatFlag_DEPTH | avxFormatFlag_STENCIL));
    afxBool bIsDepthStencil = (b->flags & (avxFormatFlag_DEPTH | avxFormatFlag_STENCIL));
    if (aIsDepthStencil != bIsDepthStencil)
    {
        return FALSE;
    }

    // Check component count.
    if (a->compCnt != b->compCnt)
        return FALSE;

    // Check each component: type and bit size must match.
    for (afxIndex i = 0; i < a->compCnt; ++i)
    {
        if (a->bpc[i] != b->bpc[i])
            return FALSE;

        // Component *type* may differ (UNORM <> SRGB <> SNORM <> UINT <> FLOAT)
        // But all types must be the same *class* — up to you how strict you want to be
        // We skip that for now (or loosen it based on type class below)
    }

    // Check total bit size
    if (a->bpp != b->bpp)
        return FALSE;

    // Check stride/padding — optional, can be relaxed if tightly packed.
    if (a->stride != b->stride)
        return FALSE;

    // Check swizzle/channel layout (must match).
    for (afxIndex i = 0; i < 4; ++i)
    {
        if (a->swizzle[i] != b->swizzle[i])
            return FALSE;
    }

    // Same block size for compressed formats.
    if ((a->bcWh[0] != b->bcWh[0]) || (a->bcWh[1] != b->bcWh[1]))
        return FALSE;

    return TRUE;
}

_AVX afxUnit AvxChooseCompatibleFormats(avxFormat base, afxUnit maxCnt, avxFormat compatibles[])
// Preallocated buffer, or NULL to count only
{
    maxCnt = AFX_MIN(maxCnt, avxFormat_TOTAL);
    afxIndex matchCnt = 0;

    for (afxIndex i = 0; i < maxCnt; ++i)
    {
        if (AvxTestCompatibleFormats(base, i))
        {
            if (compatibles)
            {
                compatibles[matchCnt] = i;
            }
            ++matchCnt;
        }
    }
    return matchCnt;
}
