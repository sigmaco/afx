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

typedef enum afxVideoUsage
{
    afxVideoUsage_SAMPLED, // used as a texture by DSYS
    afxVideoUsage_MIX,
    afxVideoUsage_SINK,
} afxVideoUsage;

AFX_DEFINE_STRUCT(afxVideoInfo)
{
    amxFormat   fmt; // type and bits per sample

};

AFX_DEFINE_STRUCT(afxVideoDesc)
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

AMX afxError AfxBindVideoSamples(afxVideoDecoder vidd, afxUnit cnt, avxBufferedMap const maps[]);
AMX afxError AfxBindVideoBuffer(afxVideoDecoder vidd, avxRaster ras);

AMX afxError AfxBlitVideo(afxVideoDecoder vidd, afxUnit opCnt, afxRect ops[]);

////////////////////////////////////////////////////////////////////////////////

AMX afxError AfxAcquireVideoDecoders(afxMixSystem msys, afxUnit cnt, afxVideoInfo const info[], afxVideoDecoder decoders[]);

#endif//AMX_VIDEO_H
