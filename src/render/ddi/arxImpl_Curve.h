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

// This section is part of SIGMA Simulation Infrastructure.

#ifndef ARX_IMPL___CURVE_H
#define ARX_IMPL___CURVE_H

#include "qwadro/inc/render/arxRenderware.h"

AFX_DEFINE_STRUCT(_arxCurveDdi)
{
    void(*evalAtKnot)(arxCurve c, afxUnit dimens, afxBool normalize, afxBool bwdsLoop, afxBool fwdsLoop, afxReal curveDur, afxUnit knotIdx, afxReal t, afxReal* rslt, afxReal const* identityVec);
};

#ifdef _ARX_CURVE_C
#ifdef _ARX_CURVE_IMPL
AFX_OBJECT(_arxCurve)
#else
AFX_OBJECT(arxCurve)
#endif
{
    _arxCurveDdi const*pimpl;
    arxCurveFormat  fmt;
    afxUnit         degree;
    afxUnit         dimens;
    arxCurveFlags   flags;
    afxUnit         knotCnt;
    union
    {
        afxReal*    knots; // 32f
        afxReal     knot0;
    };
    afxUnit         ctrlCnt;
    union
    {
        afxV4d      ctrls4; // D4, Constant32f
        afxV3d      ctrls3;
        afxReal*    ctrls; // 32f
    };
};
#endif//_ARX_CURVE_C

ARX afxClassConfig const _ARX_CUR_CLASS_CONFIG;

#endif//ARX_IMPL___CURVE_H
