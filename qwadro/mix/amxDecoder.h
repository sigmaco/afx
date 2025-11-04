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

#ifndef AMX_DECODER_H
#define AMX_DECODER_H

#include "qwadro/mix/afxMixDefs.h"
#include "qwadro/mix/amxVideo.h"

AFX_DEFINE_STRUCT(amxDecoderConfig)
{
    amxFormat   fmt; // type and bits per sample

};

AFX_DEFINE_STRUCT(amxDecoderDesc)
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

AMX afxError AmxAcquireDecoders
(
    afxMixSystem msys, 
    afxUnit cnt, 
    amxDecoderConfig const cfgs[],
    amxDecoder decoders[]
);

#endif//AMX_DECODER_H
