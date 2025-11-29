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

#ifndef AVX_VERTEX_INPUT_DDK_H
#define AVX_VERTEX_INPUT_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxIddVin);
AFX_DECLARE_STRUCT(_avxDdiVin);

#ifdef _AVX_VERTEX_INPUT_C
#ifdef _AVX_VERTEX_DECL_IMPL
AFX_OBJECT(_avxVertexInput)
#else
AFX_OBJECT(avxVertexInput)
#endif
{
    _avxDdiVin const*   ddi;
    _avxIddVin*         idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    afxFlags            flags;
    afxUnit             totalAttrCnt;
    avxVertexAttr*      attrs;
    afxUnit             binCnt;
    avxVertexStream*    bins;
};
#endif

AVX afxClassConfig const _AVX_CLASS_CONFIG_VIN;

#endif//AVX_VERTEX_INPUT_DDK_H
