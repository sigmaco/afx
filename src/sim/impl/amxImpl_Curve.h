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

#ifndef AMX_IMPL___CURVE_H
#define AMX_IMPL___CURVE_H

#include "qwadro/inc/sim/afxSimulation.h"

#ifdef _AMX_CURVE_C
#ifdef _AMX_CURVE_IMPL
AFX_OBJECT(_amxCurve)
#else
AFX_OBJECT(afxCurve)
#endif
{
    afxCurveFormat  fmt;
    afxUnit         degree;
    afxUnit         dimens;
    afxCurveFlags   flags;
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
#endif//_AMX_CURVE_C

AMX afxClassConfig const _AMX_CUR_CLASS_CONFIG;

#endif//AMX_IMPL___CURVE_H
