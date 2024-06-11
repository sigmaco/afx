/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_DEV_KIT_H
#define AVX_DEV_KIT_H

#include "qwadro/draw/afxDrawSystem.h"

AFX_DEFINE_STRUCT(afxDrawDeviceInfo)
{
    afxDeviceInfo       dev;
    afxNat              portCnt;
    afxDrawPortCaps const*portCaps;
    afxDrawDeviceCaps   caps;
    afxDrawDeviceLimits limits;
    afxClipSpace        clipSpace;
    void*               idd;
    afxError            (*iddCtorCb)(afxDrawDevice);
    afxError            (*iddDtorCb)(afxDrawDevice);
    afxError            (*procCb)(afxDrawDevice,afxThread); // call their draw threads.    
    afxError            (*dinIddDtorCb)(afxDrawDevice,afxDrawInput);
    afxError            (*dinIddCtorCb)(afxDrawDevice,afxDrawInput,afxDrawInputConfig const*,afxUri const*);
    afxError            (*dinRelinkCb)(afxDrawDevice,afxDrawContext,afxNat,afxDrawInput[]);
    afxError            (*doutIddDtorCb)(afxDrawDevice,afxDrawOutput);
    afxError            (*doutIddCtorCb)(afxDrawDevice,afxDrawOutput,afxDrawOutputConfig const*,afxUri const*);
    afxError            (*doutRelinkCb)(afxDrawDevice,afxDrawContext,afxNat,afxDrawOutput[]);
    afxClassConfig const*dctxClsCfg;
    afxClassConfig const*ddgeClsCfg;
    afxClassConfig const*dqueClsCfg;
};

typedef enum _ddevReqCode
{
    _ddevReqCode_0,
    _ddevReqCode_1,
    _ddevReqCode_DIN_DTOR,
    _ddevReqCode_DIN_CTOR,
    _ddevReqCode_DIN_RLNK,
    _ddevReqCode_DOUT_RLNK,
    _ddevReqCode_DOUT_DTOR,
    _ddevReqCode_DOUT_CTOR,
    _ddevReqCode_DCTX_DTOR,
    _ddevReqCode_DCTX_CTOR,
}
_ddevReqCode;

#ifdef _AVX_DRAW_C
AVX afxClassConfig const _AvxDqueStdImplementation;
AVX afxClassConfig const _AvxDdgeStdImplementation;
AVX afxClassConfig const _AvxDctxStdImplementation;
AVX afxClassConfig const _AvxCmdbStdImplementation;
AVX afxClassConfig const _AvxCanvStdImplementation;
AVX afxClassConfig const _AvxPipStdImplementation;
AVX afxClassConfig const _AvxSampStdImplementation;
AVX afxClassConfig const _AvxShdStdImplementation;
AVX afxClassConfig const _AvxRasStdImplementation;
AVX afxClassConfig const _AvxBufStdImplementation;
AVX afxClassConfig const _AvxRazrStdImplementation;
AVX afxClassConfig const _AvxBschStdImplementation;
AVX afxClassConfig const _AvxVinStdImplementation;
#endif//_AVX_DRAW_C


#endif//AVX_DEV_KIT_H
