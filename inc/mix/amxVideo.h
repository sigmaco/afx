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

#ifndef AMX_VIDEO_H
#define AMX_VIDEO_H

#include "qwadro/inc/mix/afxMixDefs.h"

typedef enum amxVideoUsage
{
    amxVideoUsage_SAMPLED, // used as a texture by DSYS
    amxVideoUsage_MIX,
    amxVideoUsage_SINK,
} amxVideoUsage;

AFX_DEFINE_STRUCT(amxVideoInfo)
{
    amxFormat   fmt; // type and bits per sample

};

AFX_DEFINE_STRUCT(amxVideoDesc)
{
    avxFormat fmt;
    //DXVA2_ExtendedFormat SampleFormat;
    struct
    {
        afxUnit SampleFormat : 8;
        afxUnit VideoChromaSubsampling : 4;
        afxUnit NominalRange : 3;
        afxUnit VideoTransferMatrix : 3;
        afxUnit VideoLighting : 4;
        afxUnit VideoPrimaries : 5;
        afxUnit VideoTransferFunction : 5;
    };
    afxUnit sampleWidth;
    afxUnit sampleHeight;
    afxReal inSampleFreq;
    afxReal outFrameFreq;
};

AMX afxError AmxBindVideoSamples(amxVideoDecoder vidd, afxUnit cnt, avxBufferedMap const maps[]);
AMX afxError AmxBindVideoBuffer(amxVideoDecoder vidd, avxRaster ras);

AMX afxError AmxBlitVideo(amxVideoDecoder vidd, afxUnit opCnt, afxRect ops[]);

////////////////////////////////////////////////////////////////////////////////

AMX afxError AmxAcquireVideoDecoders(afxMixSystem msys, afxUnit cnt, amxVideoInfo const info[], amxVideoDecoder decoders[]);

#endif//AMX_VIDEO_H
