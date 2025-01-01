/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Foundation Math <https://sigmaco.org/math>

#include "../draw/impl/avxImplementation.h"

_AVXINL void AfxZeroRect(afxRect* rc)
{
    afxError err = NIL;
    AFX_ASSERT(rc);
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

_AVXINL void AfxSetRect(afxRect* rc, afxInt x, afxInt y, afxUnit w, afxUnit h)
{
    afxError err = NIL;
    AfxAssert3(rc, w, h);
    rc->x = x;
    rc->y = y;
    rc->w = w;
    rc->h = h;
}

_AVXINL void AfxResetViewport(afxViewport* vp, afxReal x, afxReal y, afxReal w, afxReal h, afxReal minDepth, afxReal maxDepth)
{
    afxError err = NIL;
    AfxAssert3(vp, w, h);
    vp->origin[0] = x;
    vp->origin[1] = y;
    vp->extent[0] = w;
    vp->extent[1] = h;
    vp->minDepth = minDepth;
    vp->maxDepth = maxDepth;
}
