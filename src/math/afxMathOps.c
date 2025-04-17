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

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/inc/math/afxTransform.h"
#include "qwadro/inc/math/afxMatrix.h"

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxV2dNdc(afxV2d v, afxV2d const from, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(from);
    AFX_ASSERT(total);
    v[0] = AfxNdcf(from[0], total[0]);
    v[1] = AfxNdcf(from[1], total[1]);
}

_AFXINL void AfxV2dUnndc(afxV2d v, afxV2d const from, afxV2d const total)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(v);
    AFX_ASSERT(from);
    AFX_ASSERT(total);
    v[0] = AfxUnndcf(from[0], total[0]);
    v[1] = AfxUnndcf(from[1], total[1]);
}
