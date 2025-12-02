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

#ifndef AVX_CODEBASE_DDK_H
#define AVX_CODEBASE_DDK_H

#include "../exec/afxSystemDDK.h"
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

    afxUnit             inCnt;
    avxShaderIoChannel* ins;
    afxUnit             outCnt;
    avxShaderIoChannel* outs;
    avxTopology         topology;
};

#ifdef _AVX_SHADER_IMPL
AFX_OBJECT(_avxCodebase)
#else
AFX_OBJECT(avxCodebase)
#endif
{
    _avxDdiCodb const*  ddi;
    _avxIddCodb*        idd;
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

AVX afxClassConfig const _AVX_CLASS_CONFIG_CODB;

AVX afxError _AvxLoadGlScript(afxStream file, afxArray* fCode);
AVX afxError _AvxScanGlScript(afxString const* code, afxArray* fIns, afxArray* fOuts, afxArray* fResources, afxString* pushConstsName);

AVX afxError _AvxConvertToGlsl(afxString const* src, afxArray* fCode);

#endif//AVX_CODEBASE_DDK_H
