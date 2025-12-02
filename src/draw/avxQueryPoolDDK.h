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

#ifndef AVX_QUERY_POOL_DDK_H
#define AVX_QUERY_POOL_DDK_H

#include "../exec/afxSystemDDK.h"
#include "qwadro/draw/afxDrawSystem.h"

AFX_DECLARE_STRUCT(_avxDdiQryp);
AFX_DECLARE_STRUCT(_avxIddQryp);

#ifdef _AVX_QUERY_POOL_C
#ifdef _AVX_QUERY_POOL_IMPL
AFX_OBJECT(_avxQueryPool)
#else
AFX_OBJECT(avxQueryPool)
#endif
{
    _avxDdiQryp const*  ddi;
    _avxIddQryp*        idd;
    // User-defined data.
    void*               udd;
    // Debugging tag.
    afxString           tag;

    avxQueryType        type;
    afxUnit             slotCnt;
};
#endif

AVX afxClassConfig const _AVX_CLASS_CONFIG_QRYP;

AVX afxError _AvxDsysResetQrypCb_SW(afxDrawSystem dsys, avxQueryPool qryp, afxUnit base, afxUnit cnt);
AVX afxResult _AvxDsysGetQrypRsltCb_SW(afxDrawSystem dsys, avxQueryPool qryp, avxQueryResultFlags flags, afxUnit base, afxUnit cnt, afxSize dstCap, void* dst, afxSize stride);

#endif//AVX_QUERY_POOL_DDK_H
