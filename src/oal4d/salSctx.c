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
extern afxClassConfig const _SalSbufMgrCfg;
extern afxClassConfig const _SalCurMgrCfg;

_A4D afxError _SalSctxDtor(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    AfxDisconnectSoundContext(sctx, TRUE, TRUE);
    AfxWaitForIdleSoundContext(sctx);

    AfxCleanUpChainedManagers(&sctx->base.ctx.classes);

    if (_AsxSctxStdImplementation.dtor(sctx))
        AfxThrowError();

    return err;
}

_A4D afxError _SalSctxCtor(afxSoundContext sctx, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    //AfxAssert(spec);

    if (_AsxSctxStdImplementation.ctor(sctx, cookie)) AfxThrowError();
    else
    {
        afxSoundDevice sdev = cookie->udd[0];
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxSoundContextConfig const *config = ((afxSoundContextConfig const *)cookie->udd[1]) + cookie->no;

        afxChain *classes = &sctx->base.ctx.classes;
        
        afxClassConfig tmpClsConf;
        tmpClsConf = _AfxSemStdImplementation;
        AfxEstablishManager(&sctx->base.ctx.semaphores, NIL, classes, &tmpClsConf);

        tmpClsConf = _AfxFencStdImplementation;
        AfxEstablishManager(&sctx->base.ctx.fences, NIL, classes, &tmpClsConf);

        tmpClsConf = _SalSbufMgrCfg;
        AfxEstablishManager(&sctx->base.buffers, NIL, classes, &tmpClsConf);

        //tmpClsConf = _SalCurMgrCfg;
        //AfxEstablishManager(&sctx->base.curves, NIL, classes, &tmpClsConf);

        if (!err)
        {
            sctx->base.running = TRUE;
        }

        if (err)
        {
            AfxCleanUpChainedManagers(&sctx->base.ctx.classes);
        }

        if (err && _AsxSctxStdImplementation.dtor(sctx))
            AfxThrowError();
    }
    return err;
}

_A4D afxClassConfig const _SalSctxMgrCfg =
{
    .fcc = afxFcc_SCTX,
    .name = "SoundContext",
    .desc = "Sound Device Context Management",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSoundContext)),
    .ctor = (void*)_SalSctxCtor,
    .dtor = (void*)_SalSctxDtor
};
