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

#include "salSdev.h"


_A4D afxError _SalCurDtor(afxSoundBuffer sbuf)
{
    afxError err = AFX_ERR_NONE;

    afxSoundContext sctx = AfxGetObjectProvider(sbuf);
    afxMmu mmu = AfxGetSoundContextMmu(sctx);
    AfxAssertObjects(1, &mmu, afxFcc_MMU);

    //if (sbuf->glHandle)
    {
        //_SalSctxDeleteGlRes(dctx, 0, sbuf->glHandle);
        //sbuf->glHandle = 0;
    }

    if (sbuf->base.bytemap)
    {
        AfxDeallocate(mmu, sbuf->base.bytemap);
    }
    return err;
}

_A4D afxError _SalCurCtor(afxSoundBuffer sbuf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &sbuf, afxFcc_SBUF);

    return err;
}

_A4D afxClassConfig _SalCurMgrCfg =
{
    .fcc = afxFcc_SBUF,
    .name = "Curve",
    .desc = "Hardware-accelerated Curve",
    .fixedSiz = sizeof(AFX_OBJECT(afxCurve)),
    .ctor = (void*)_SalCurCtor,
    .dtor = (void*)_SalCurDtor
};
