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
#include "qwadro/core/afxClass.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/sound/afxSoundSystem.h"
#include "qwadro/core/afxSystem.h"
AFXINL afxSoundSystem _AfxGetSsysData(void);


_AFX afxMmu AfxGetSoundContextMmu(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxMmu mmu = sctx->mmu;
    AfxAssertObjects(1, &mmu, afxFcc_MMU);
    return mmu;
}

_AFX afxSoundDevice AfxGetSoundContextDevice(afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    afxSoundDevice sdev = AfxGetObjectProvider(sctx);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireSoundContexts(afxNat sdevId, afxNat cnt, afxSoundContextConfig const config[], afxSoundContext sctx[])
{
    AfxEntry("cnt=%u,config=%p", cnt, config);
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev;

    if (!(sdev = AfxGetSoundDevice(sdevId))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxClass* cls = AfxGetSoundContextClass(sdev);
        AfxAssertClass(cls, afxFcc_SCTX);

        if (AfxAcquireObjects(cls, cnt, (afxObject*)sctx, (void const*[]) { &sdevId, (void*)config }))
            AfxThrowError();

        AfxAssertObjects(cnt, sctx, afxFcc_SCTX);
    }
    return err;
}

_AFX afxNat AfxInvokeSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundContext, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls;

    if (sdev)
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        cls = AfxGetSoundContextClass(sdev);            
    }
    else
    {
        afxSoundSystem ssys = _AfxGetSsysData();
        AfxAssertType(ssys, afxFcc_SSYS);
        cls = &ssys->scontexts;
    }
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateSoundContexts(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundContext sctx[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(sctx);
    afxClass* cls;

    if (sdev)
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        cls = AfxGetSoundContextClass(sdev);            
    }
    else
    {
        afxSoundSystem ssys = _AfxGetSsysData();
        AfxAssertType(ssys, afxFcc_SSYS);
        cls = &ssys->scontexts;
    }
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)sctx);
}

_AFX afxNat AfxCountSoundContexts(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxClass* cls = AfxGetSoundContextClass(sdev);
    AfxAssertClass(cls, afxFcc_SCTX);
    return AfxCountInstances(cls);
}
