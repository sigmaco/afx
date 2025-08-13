/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AMX_CODEC_H
#define AMX_CODEC_H

#include "qwadro/inc/mix/io/amxAudio.h"

typedef enum amxMediaType
{
    amxMediaType_DATA,
    amxMediaType_VIDEO,
    amxMediaType_AUDIO,
    amxMediaType_CAPTION,
    amxMediaType_EXTRA,
} amxMediaType;

typedef enum amxCodecId
{
    amxCodecId_THEORA,
    amxCodecId_VORBIS,
    amxCodecId_ROQ,
    amxCodecId_BINK,
    amxCodecId_BINKA,
    amxCodecId_FLAC,
} amxCodecId;

AFX_DEFINE_STRUCT(amxPacket)
{
    afxFlags    flags;
    afxUnit     srcIdx; // index of source stream
    afxInt64    posn; // in stream
    afxInt64    dts; // time at which the packet is decompressed
    afxInt64    pts; // time at which the decompressed packet will be presented
    amxBuffer   buf; // packet data storage
    afxByte*    data;
    afxUnit     siz;
    afxUnit64   dur; // duration
    void*       udd;
};

AFX_DEFINE_STRUCT(amxFrame)
{
    afxByte*    data;
    void*       udd;
    afxUnit     w, h;
    afxUnit     sampCnt;
    amxFormat   fmt;
    afxUnit     keyFrame;
    afxReal     sampAspRatio;
    afxInt64    dts;
    afxInt64    pts;
    afxUnit     picNum;
    afxUnit     dispPicNum;
    afxInt      quality;
    afxUnit     sampRate;
    amxBuffer   buf;
};

AFX_OBJECT(amxCodec)
{
    int a;
};

AFX_OBJECT(amxEncoder)
{
    int a;
};

AFX_OBJECT(amxDecoder)
{
    int a;
};

AFX_DEFINE_STRUCT(amxCodecFeatures)
{
    afxBool encode;
    afxBool decode;
};

AFX_DEFINE_STRUCT(amxCodecLimits)
{
    afxUnit sampleRate;
    afxUnit bitsPerSample;
};

AMX afxUnit AmxEnumerateCodecs(afxUnit icd, afxUnit first, afxUnit cnt, amxCodec codecs[]);

AMX afxUnit AmxInvokeCodecs(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, amxCodec), afxUnit cnt);

AMX afxUnit AmxEvokeCodecs(afxUnit icd, afxUnit first, void* udd, afxBool(*f)(void*, amxCodec), afxUnit cnt, amxCodec codecs[]);

AMX afxUnit AmxChooseCodecs(afxUnit icd, amxCodecFeatures const* features, amxCodecLimits const* limits, void const* caps, afxUnit maxCnt, afxUnit mcdcId[]);

#endif//AMX_CODEC_H
