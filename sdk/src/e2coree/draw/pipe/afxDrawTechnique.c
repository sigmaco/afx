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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/afxDrawTechnique.h"
#include "qwadro/draw/afxXsh.h"

////////////////////////////////////////////////////////////////////////////////
// DRAW TECHNIQUE                                                             //
////////////////////////////////////////////////////////////////////////////////

_AFX afxError _AfxDtecDtor(afxDrawTechnique dtec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dtec, afxFcc_DTEC);
    


    return err;
}

_AFX afxError _AfxDtecCtor(afxDrawTechnique dtec, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &dtec, afxFcc_DTEC);
    


    return err;
}

_AFX afxClassConfig const _AfxDtecClassSpec =
{
    .fcc = afxFcc_DTEC,
    .size = sizeof(AFX_OBJECT(afxDrawTechnique)),
    .ctor = (void*)_AfxDtecCtor,
    .dtor = (void*)_AfxDtecDtor,
    .name = "afxDrawTechnique"
};
