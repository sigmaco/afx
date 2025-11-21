/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Advanced Math Extensions for Qwadro

#include "qwadro/math/afxTransform.h"
#include "qwadro/math/afxMatrix.h"

////////////////////////////////////////////////////////////////////////////////

_AFXINL void AfxV2dNdc(afxV2d v, afxV2d const from, afxV2d const total)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(from);
    AFX_ASSERT(total);
    v[0] = AfxNdcf(from[0], total[0]);
    v[1] = AfxNdcf(from[1], total[1]);
}

_AFXINL void AfxV2dUnndc(afxV2d v, afxV2d const from, afxV2d const total)
{
    afxError err = { 0 };
    AFX_ASSERT(v);
    AFX_ASSERT(from);
    AFX_ASSERT(total);
    v[0] = AfxUnndcf(from[0], total[0]);
    v[1] = AfxUnndcf(from[1], total[1]);
}
