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

#ifndef AMX_ENCODER_H
#define AMX_ENCODER_H

#include "qwadro/mix/afxMixDefs.h"
#include "qwadro/mix/amxVideo.h"

AFX_DEFINE_STRUCT(amxEncoderConfig)
{
    amxFormat   fmt; // type and bits per sample

};

AFX_DEFINE_STRUCT(amxEncoderDesc)
{
    int a;
};

////////////////////////////////////////////////////////////////////////////////

AMX afxError AmxAcquireEncoders
(
    afxMixSystem msys, 
    afxUnit cnt, 
    amxEncoderConfig const cfgs[],
    amxEncoder encoders[]
);

#endif//AMX_ENCODER_H
