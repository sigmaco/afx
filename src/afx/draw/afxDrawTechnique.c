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

#include "../dev/AvxDevKit.h"

////////////////////////////////////////////////////////////////////////////////
// DRAW TECHNIQUE                                                             //
////////////////////////////////////////////////////////////////////////////////

_AVX afxError _AfxDtecDtor(afxDrawTechnique dtec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dtec, afxFcc_DTEC);
    


    return err;
}

_AVX afxError _AfxDtecCtor(afxDrawTechnique dtec, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dtec, afxFcc_DTEC);
    


    return err;
}

_AVX afxClassConfig const _AfxDtecClassSpec =
{
    .fcc = afxFcc_DTEC,
    .fixedSiz = sizeof(AFX_OBJECT(afxDrawTechnique)),
    .ctor = (void*)_AfxDtecCtor,
    .dtor = (void*)_AfxDtecDtor,
    .name = "afxDrawTechnique"
};