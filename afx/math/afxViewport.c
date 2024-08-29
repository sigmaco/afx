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

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "../dev/AvxImplKit.h"

_AVXINL void AfxZeroRect(afxRect* rc)
{
    afxError err = NIL;
    AfxAssert(rc);
    rc->x = 0;
    rc->y = 0;
    rc->w = 1;
    rc->h = 1;
}

_AVXINL void AfxCopyRect(afxRect* rc, afxRect const* src)
{
    afxError err = NIL;
    AfxAssert2(rc, src);
    rc->x = src->x;
    rc->y = src->y;
    rc->w = src->w;
    rc->h = src->h;
}

_AVXINL void AfxSetRect(afxRect* rc, afxInt x, afxInt y, afxNat w, afxNat h)
{
    afxError err = NIL;
    AfxAssert3(rc, x, y);
    rc->x = x;
    rc->y = y;
    rc->w = w;
    rc->h = h;
}
