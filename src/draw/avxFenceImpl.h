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

#ifndef AVX_FENCE_IMPL_H
#define AVX_FENCE_IMPL_H

#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxIddFenc);

#ifdef _AVX_FENCE_C

AFX_DEFINE_STRUCT(_avxDdiFenc)
{
    afxError(*waitCb)(avxFence, afxUnit64 value, afxUnit64 timeout);
    afxError(*resetCb)(avxFence);
    afxError(*signalCb)(avxFence);
};

#ifdef _AVX_FENCE_IMPL
AFX_OBJECT(_avxFence)
#else
AFX_OBJECT(avxFence)
#endif
{
    _avxIddFenc*        idd;
    _avxDdiFenc const*  ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    avxFenceFlags       flags;
    afxAtom32           signaled;
    afxAtom64           value;
};
#endif//_AVX_FENCE_C

AVX afxClassConfig const _AVX_FENC_CLASS_CONFIG;

#endif//AVX_FENCE_IMPL_H
