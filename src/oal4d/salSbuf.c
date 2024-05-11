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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#include "salSdev.h"


_A4D afxError _SalSbufDtor(afxSoundBuffer sbuf)
{
    afxError err = AFX_ERR_NONE;

    afxSoundContext sctx = AfxGetObjectProvider(sbuf);
    
    //if (sbuf->glHandle)
    {
        //_SalSctxDeleteGlRes(dctx, 0, sbuf->glHandle);
        //sbuf->glHandle = 0;
    }

    return err;
}

_A4D afxError _SalSbufCtor(afxSoundBuffer sbuf, afxCookie const* cookie)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &sbuf, afxFcc_SBUF);

    return err;
}

_A4D afxClassConfig _SalSbufMgrCfg =
{
    .fcc = afxFcc_SBUF,
    .name = "SoundBuffer",
    .desc = "Sound Buffer",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSoundBuffer)),
    .mmu = NIL,
    .ctor = (void*)_SalSbufCtor,
    .dtor = (void*)_SalSbufDtor
};
