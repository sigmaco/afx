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

#include "qwadro/math/afxTrigonometry.h"

// Returns the hyperbolic sine function; (e^x - e^(-x)) / 2.

_AFXINL afxReal64 AfxSinh(afxReal64 x)
{
    return sinh(x);
}

_AFXINL afxReal AfxSinhf(afxReal x)
{
    return sinhf(x);
}

// Returns the hyperbolic cosine function; (e^x + e^(-x)) / 2.

_AFXINL afxReal64 AfxCosh(afxReal64 x)
{
    return cosh(x);
}

_AFXINL afxReal AfxCoshf(afxReal x)
{
    return coshf(x);
}

// Returns the hyperbolic tangent function; sinh(x) / cosh(x).

_AFXINL afxReal64 AfxTanh(afxReal64 x)
{
    return tanh(x);
}

_AFXINL afxReal AfxTanhf(afxReal x)
{
    return tanhf(x);
}

// Arc hyperbolic sine; returns the inverse of sinh.

_AFXINL afxReal64 AfxAsinh(afxReal64 x)
{
    return asinh(x);
}

_AFXINL afxReal AfxAsinhf(afxReal x)
{
    return asinhf(x);
}

// Arc hyperbolic cosine; returns the non-negative inverse of cosh.
// Results are undefined if x < 1.

_AFXINL afxReal64 AfxAcosh(afxReal64 x)
{
    return acosh(x);
}

_AFXINL afxReal AfxAcoshf(afxReal x)
{
    return acoshf(x);
}

// Arc hyperbolic tangent; returns the inverse of tanh.
// Results are undefined if x >= 1.

_AFXINL afxReal64 AfxAtanh(afxReal64 y_over_x)
{
    return atanh(y_over_x);
}

_AFXINL afxReal AfxAtanhf(afxReal y_over_x)
{
    return atanhf(y_over_x);
}
