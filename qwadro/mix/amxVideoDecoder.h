/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

#ifndef AMX_VIDEO_DEC_H
#define AMX_VIDEO_DEC_H

#include "qwadro/mix/afxMixDefs.h"
#include "qwadro/mix/amxVideo.h"

AFX_DEFINE_STRUCT(amxVideoDecInfo)
{
    amxFormat   fmt; // type and bits per sample

};

AFX_DEFINE_STRUCT(amxVideoDesc)
{
    amxFormat fmt;
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

////////////////////////////////////////////////////////////////////////////////

AMX afxError AmxAcquireVideoDecoders(afxMixSystem msys, afxUnit cnt, amxVideoDecInfo const info[], amxVideoDecoder decoders[]);

#endif//AMX_VIDEO_DEC_H
