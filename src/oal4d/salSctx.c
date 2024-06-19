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

extern afxClassConfig const _SalSbufMgrCfg;
extern afxClassConfig const _SalCurMgrCfg;

_A4D afxError _SalSdevCloseCb(afxSoundDevice sdev, afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    AfxDisconnectSoundContext(sctx, TRUE, TRUE);
    AfxWaitForSoundContext(sctx);

    AfxCleanUpChainedClasses(&sctx->m.ctx.classes);

    sctx->m.ownedBridgeCnt = 0;

    return err;
}

_A4D afxError _SalSdevOpenCb(afxSoundDevice sdev, afxSoundContext sctx, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    //AfxAssert(spec);

    afxSoundContextConfig const *config = ((afxSoundContextConfig const *)cookie->udd[1]) + cookie->no;

    afxChain *classes = &sctx->m.ctx.classes;
        
    afxClassConfig tmpClsConf;
    tmpClsConf = _AfxSemStdImplementation;
    AfxRegisterClass(&sctx->m.ctx.semaphores, NIL, classes, &tmpClsConf);

    tmpClsConf = _AfxFencStdImplementation;
    AfxRegisterClass(&sctx->m.ctx.fences, NIL, classes, &tmpClsConf);

    tmpClsConf = _SalSbufMgrCfg;
    AfxRegisterClass(&sctx->m.buffers, NIL, classes, &tmpClsConf);

    //tmpClsConf = _SalCurMgrCfg;
    //AfxRegisterClass(&sctx->m.curves, NIL, classes, &tmpClsConf);

    if (!err)
    {
        sctx->m.running = TRUE;
    }

    if (err)
    {
        AfxCleanUpChainedClasses(&sctx->m.ctx.classes);
    }

    return err;
}
