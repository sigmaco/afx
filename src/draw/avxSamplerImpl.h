/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // The Qwadro Video Graphics Infrastructure                                 //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_SAMPLER_IMPL_H
#define AVX_SAMPLER_IMPL_H

#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxIddSamp);
AFX_DECLARE_STRUCT(_avxDdiSamp);

#ifdef _AVX_SAMPLER_C
#ifdef _AVX_SAMPLER_IMPL
AFX_OBJECT(_avxSampler)
#else
AFX_OBJECT(avxSampler)
#endif
{
    _avxIddSamp*        idd;
    _avxDdiSamp const*  ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxUnit32           crc;
    afxFlags            flags;
    avxSamplerConfig    cfg;
};
#endif

AVX afxClassConfig const _AVX_SAMP_CLASS_CONFIG;

#endif//AVX_SAMPLER_IMPL_H
