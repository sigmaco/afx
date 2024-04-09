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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#include "salSdev.h"


_A4D afxError _SalSctxDtor(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    AfxCleanUpChainedManagers(&sctx->base.classes);

    AfxDeallocateArena(&sctx->base.aren);
    AfxReleaseObjects(1, (void*[]) { sctx->base.mmu });

    return err;
}

_A4D afxError _SalSctxCtor(afxSoundContext sctx, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    //AfxAssert(spec);

    afxNat devId = *(afxNat const *)(cookie->udd[0]);
    afxSoundContextConfig const *config = ((afxSoundContextConfig const *)cookie->udd[1]) + cookie->no;

    afxSoundDevice sdev = AfxGetObjectProvider(sctx);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    afxMmu mmu = sctx->base.mmu;

    AfxAllocateArena(mmu, &sctx->base.aren, NIL, AfxHere());

    //if (!(AfxObjectReacquire(&sctx->base.memD->obj, &sctx->base.obj, NIL, NIL, NIL))) AfxThrowError();
    //else
    {
        afxChain *classes = &sctx->base.classes;
        AfxSetUpChain(classes, (void*)sctx);

        afxClassConfig tmpClsConf = {0};

        //tmpClsConf = _SalBufMgrCfg;
        tmpClsConf.mmu = mmu;
        AfxEstablishManager(&sctx->base.buffers, NIL, classes, &tmpClsConf);

#if 0
        tmpClsConf = _AfxVbufMgrCfg;
        tmpClsConf.mmu = mmu;
        AfxEstablishManager(&sctx->base.vbuffers, classes, &tmpClsConf);
        tmpClsConf = _AfxIbufMgrCfg;
        tmpClsConf.mmu = mmu;
        AfxEstablishManager(&sctx->base.ibuffers, classes, &tmpClsConf);
#endif
        {
            {

                if (!err)
                {
                    sctx->base.running = TRUE;
                }
            }

        }

        if (err)
        {
            AfxCleanUpChainedManagers(&sctx->base.classes);

            AfxReleaseObjects(1, (void*[]) { sctx->base.mmu });
        }
    }

    return err;
}

_A4D afxClassConfig const _SalSctxMgrCfg =
{
    .fcc = afxFcc_SCTX,
    .name = "Sound Context",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSoundContext)),
    .mmu = NIL,
    .ctor = (void*)_SalSctxCtor,
    .dtor = (void*)_SalSctxDtor
};
