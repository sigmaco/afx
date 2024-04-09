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

#define _AFX_SOUND_C
#define _AFX_SOUND_CONTEXT_C
#define _AFX_SOUND_SYSTEM_C
#include "qwadro/mem/afxArena.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/sound/afxSoundSystem.h"
#include "qwadro/core/afxSystem.h"

_AAX afxMmu AfxGetSoundContextMmu(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxMmu mmu = sctx->mmu;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AAX afxSoundDevice AfxGetSoundContextDevice(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundDevice sdev = AfxGetObjectProvider(sctx);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

////////////////////////////////////////////////////////////////////////////////

_AAX afxError AfxAcquireSoundContexts(afxNat sdevId, afxNat cnt, afxSoundContextConfig const config[], afxSoundContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev;

    if (!(sdev = AfxGetSoundDevice(sdevId))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxManager* cls = AfxGetSoundContextClass(sdev);
        AfxAssertClass(cls, afxFcc_SCTX);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)contexts, (void const*[]) { &sdevId, (void*)config }))
            AfxThrowError();

        AfxAssertObjects(cnt, contexts, afxFcc_SCTX);
    }
    return err;
}

_AAX afxNat AfxInvokeSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AAX afxNat AfxEnumerateSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundContext contexts[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AfxAssert(contexts);
    AfxAssert(cnt);
    afxManager* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)contexts);
}

_AAX afxNat AfxCountSoundContexts(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxManager* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxCountObjects(cls);
}
