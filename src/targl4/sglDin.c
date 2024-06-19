/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "sgl.h"
#include "qwadro/afxQwadro.h"
#include "qwadro/draw/afxDrawInput.h"

extern afxClassConfig _SglCmdbMgrCfg;

afxError _SglActivateDin(sglDpu* dpu, afxDrawInput din)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    return err;
}

_SGL afxError _SglDinRelinkCb(afxDrawDevice ddev, afxDrawContext dctx, afxNat cnt, afxDrawInput din[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    return err;
}

_SGL afxError _SglDdevOpenDinCb(afxDrawDevice ddev, afxDrawInput din, afxDrawInputConfig const* cfg, afxUri const* endpoint)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ddev, afxFcc_DDEV);
    AfxAssertObjects(1, &din, afxFcc_DIN);

    for (afxNat i = 0; i < din->poolCnt; i++)
    {
        AfxDeregisterClass(&din->pools[i].streams);
        afxClassConfig tmpClsCfg = _SglCmdbMgrCfg;
        AfxRegisterClass(&din->pools[i].streams, NIL, &din->classes, &tmpClsCfg);
    }

    return err;
}
