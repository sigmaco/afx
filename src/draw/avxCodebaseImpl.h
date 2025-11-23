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

#ifndef AVX_CODEBASE_IMPL_H
#define AVX_CODEBASE_IMPL_H

#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxIddCodb);
AFX_DECLARE_STRUCT(_avxDdiCodb);

#ifdef _AVX_SHADER_C

AFX_DEFINE_STRUCT(_avxCodeBlock)
{
    afxString           tag;
    void*               udd;
    afxString32         name;
    avxShaderType       stage;

    afxUnit             verMajor;
    afxUnit             verMinor;
    afxBool             extended;
    afxByte*            code;
    afxUnit             codeLen;
    afxString           entry;

    afxString32         pushConstName;
    afxUnit             resDeclCnt;
    avxShaderResource*  resDecls;

    afxUnit             ioDeclCnt;
    avxShaderIoChannel* ioDecls;
    avxTopology         topology;
};

#ifdef _AVX_SHADER_IMPL
AFX_OBJECT(_avxCodebase)
#else
AFX_OBJECT(avxCodebase)
#endif
{
    _avxIddCodb*        idd;
    _avxDdiCodb const*  ddi;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    // Optional base URL for file lookup.
    afxUri128           url;
    // A pool of code units.
    afxPool             codes;
};
#endif//_AVX_SHADER_C

AVX afxClassConfig const _AVX_SHD_CLASS_CONFIG;

AVX afxError    _AvxLoadGlScript(afxStream file, afxArray* fCode);

#endif//AVX_CODEBASE_IMPL_H
