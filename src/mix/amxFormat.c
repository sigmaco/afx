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

// This software is part of Advanced Multimedia Extensions & Experiments.

#define _AMX_MIX_C
//#define _AMX_MIX_SYSTEM_C
#include "amxIcd.h"


_AMX amxFormatDescription const _AmxStdPfds[amxFormat_TOTAL] =
{
    {
        .bps = 0,
        .stride = 0,
        .compCnt = 0,
        .type = { amxFormatType_UNK, amxFormatType_UNK, amxFormatType_UNK, amxFormatType_UNK },
        .swizzle = { 0, 0, 0, 0 },
        .defaults = { 0, 0, 0, 0 },
        .isNormalized = { 0, 0, 0, 0 },
        .tof = { 0, 0, 0, 0 },
        .bpc = { 0, 0, 0, 0 },
        .bcWh = { 0, 0 },
        .flags = NIL
    },

    ////////////////////////////////////////////////////////////////////////////

    [amxFormat_S16i] = // stereo 16-bit signed int
    {
        .tag = AFX_STRING("S16i"),
        .type = {amxFormatType_UN, amxFormatType_UN, amxFormatType_UN, amxFormatType_UN},
        .defaults = { 0, 0, 0, 0x3f800000 },
        .swizzle = { 0, 1, 2, 3 },
        .bpc = { 16, 0, 0, 0 },
        .bps = 16,
        .stride = 2,
        .compCnt = 1,
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = { 1.0f / 32767.0f, 0.f, 0.f, 0.f },
        .bcWh = { 1, 1 },
        .flags = amxFormatFlag_TANGLED | amxFormatFlag_STEREO,
        .audCaps = amxFormatUsage_COPY | amxFormatUsage_BLIT | amxFormatUsage_MIX | amxFormatUsage_BLEND
    },
    [amxFormat_S32f] = // stereo 32-bit floating
    {
        .tag = AFX_STRING("S32f"),
        .type = {amxFormatType_UN, amxFormatType_UN, amxFormatType_UN, amxFormatType_UN},
        .defaults = { 0, 0, 0, 0x3f800000 },
        .swizzle = { 0, 1, 2, 3 },
        .bpc = { 32, 0, 0, 0 },
        .bps = 32,
        .stride = 4,
        .compCnt = 1,
        .isNormalized = { TRUE, TRUE, TRUE, TRUE },
        .tof = { 1.f, 0.f, 0.f, 0.f },
        .bcWh = { 1, 1 },
        .flags = amxFormatFlag_TANGLED | amxFormatFlag_STEREO,
        .audCaps = amxFormatUsage_COPY | amxFormatUsage_BLIT | amxFormatUsage_MIX | amxFormatUsage_BLEND
    },

};

_AMXINL void AmxDescribeFormats(afxUnit cnt, amxFormat const formats[], amxFormatDescription pfd[])
{
    afxError err = { 0 };
    AFX_ASSERT(formats);
    AFX_ASSERT(pfd);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        amxFormat fmt = formats[i];
        AFX_ASSERT(amxFormat_TOTAL > fmt);
        fmt = AFX_MIN(fmt, amxFormat_TOTAL - 1);
        pfd[i] = _AmxStdPfds[fmt];
    }
}

