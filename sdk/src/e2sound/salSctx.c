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


_A4D afxError _SalSctxProcCb(afxSoundContext sctx, afxSoundThread sthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssertObjects(1, &sthr, afxFcc_STHR);


    return err;
}

_A4D afxError _SalSctxDtor(afxSoundContext sctx)
{
    AfxEntry("sctx=%p", sctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    _AfxUninstallChainedClasses(&sctx->base.classes);

    AfxDeallocateArena(&sctx->base.aren);
    AfxReleaseObjects(1, (void*[]) { sctx->base.mmu });

    return err;
}

_A4D afxError _SalSctxCtor(afxSoundContext sctx, afxCookie const* cookie)
{
    AfxEntry("sctx=%p", sctx);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    //AfxAssert(spec);

    afxNat devId = *(afxNat const *)(cookie->udd[0]);
    afxSoundContextConfig const *config = ((afxSoundContextConfig const *)cookie->udd[1]) + cookie->no;

    afxSoundDevice sdev = AfxGetObjectProvider(sctx);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    sctx->base.running = FALSE;

    sctx->base.mmu = AfxGetSoundSystemMmu();


    AfxAssertObjects(1, &sctx->base.mmu, afxFcc_MMU);

    afxMmu mmu = sctx->base.mmu;

    AfxAllocateArena(mmu, &sctx->base.aren, NIL, AfxHint());

    //if (!(AfxObjectReacquire(&sctx->base.memD->obj, &sctx->base.obj, NIL, NIL, NIL))) AfxThrowError();
    //else
    {
        afxChain *classes = &sctx->base.classes;
        AfxSetUpChain(classes, (void*)sctx);

        afxClassConfig tmpClsConf = {0};

        //tmpClsConf = _SalBufClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&sctx->base.buffers, NIL, classes, &tmpClsConf);

#if 0
        tmpClsConf = _AfxVbufClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&sctx->base.vbuffers, classes, &tmpClsConf);
        tmpClsConf = _AfxIbufClsConfig;
        tmpClsConf.mmu = mmu;
        AfxMountClass(&sctx->base.ibuffers, classes, &tmpClsConf);
#endif
        {
            {
                sctx->base.procCb = _SalSctxProcCb;

                if (!err)
                {
                    sctx->base.running = TRUE;
                }
            }

        }

        if (err)
        {
            _AfxUninstallChainedClasses(&sctx->base.classes);

            AfxReleaseObjects(1, (void*[]) { sctx->base.mmu });
        }
    }

    return err;
}

_A4D afxClassConfig const _SalSctxClsConfig =
{
    .fcc = afxFcc_SCTX,
    .name = "Sound Context",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxSoundContext)),
    .mmu = NIL,
    .ctor = (void*)_SalSctxCtor,
    .dtor = (void*)_SalSctxDtor
};
