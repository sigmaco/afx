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

#ifndef AVX_IMPLEMENTATION_H
#define AVX_IMPLEMENTATION_H

#include "../impl/afxExecImplKit.h"
#include "../impl/afxIoImplKit.h"

#ifdef _AFX_DEBUG
// hardcoded validation controls
#define AVX_VALIDATION_ENABLED TRUE
#define AVX_LIMIT_VALIDATION_ENABLED TRUE
#define AVX_FEATURE_VALIDATION_ENABLED TRUE
#endif

#include "avxSystemImpl.h"
#include "avxDeviceImpl.h"
#include "avxContextImpl.h"
#include "dpu/avxDpuExecutor.h"
#include "avxPipelineImpl.h"
#include "avxQueryPoolImpl.h"
#include "avxCodebaseImpl.h"
#include "avxLigatureImpl.h"
#include "avxVertexInputImpl.h"
#include "avxSamplerImpl.h"
#include "avxCanvasImpl.h"
#include "avxSurfaceImpl.h"
#include "avxRasterImpl.h"
#include "avxBufferImpl.h"
#include "dpu/avxDpuTransference.h"

AFX_DEFINE_STRUCT(_avxDsysImp)
{
    afxClassConfig ddevCls;
    afxClassConfig dsysCls;
};

AVX afxClass const* _AvxIcdGetDdevClass(afxModule icd);
AVX afxClass const* _AvxIcdGetDsysClass(afxModule icd);

AVX afxError _AvxRegisterDisplays(afxModule icd, afxUnit cnt, avxDisplayInfo const infos[], afxDisplay displays[]);
AVX afxError _AvxRegisterDrawDevices(afxModule icd, afxUnit cnt, _avxDdevReg const infos[], afxDrawDevice devices[]);
AVX afxError _AvxImplementDrawSystem(afxModule icd, _avxDsysImp const* cfg);

AVX afxBool _AvxGetIcd(afxUnit icdIdx, afxModule* driver);

AVX avxFormatDescription const _AvxStdPfds[avxFormat_TOTAL];

#endif//AVX_IMPLEMENTATION_H
