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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_SOUND_C
#define _AFX_SOUND_THREAD_C
#define _AFX_SOUND_DEVICE_C
#define _AFX_THREAD_C
#include "qwadro/sound/afxSoundSystem.h"

AFXINL afxSoundSystem _AfxGetSsysData(void);

_AFX afxBool AfxGetSoundThreadActiveQueue(afxSoundThread sthr, afxSoundQueue* sque)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sthr, afxFcc_STHR);
    afxSoundQueue sque2;
    afxBool rslt;

    if ((rslt = !!(sque2 = sthr->sque)))
    {
        AfxAssertObjects(1, &sque2, afxFcc_SQUE);

        if (sque)
            *sque = sque2;
    }
    return rslt;
}

_AFX afxBool AfxGetSoundThreadActiveContext(afxSoundThread sthr, afxSoundContext* sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sthr, afxFcc_STHR);
    afxSoundContext sctx2;
    afxBool rslt;

    if ((rslt = !!(sctx2 = sthr->sctx)))
    {
        AfxAssertObjects(1, &sctx2, afxFcc_DCTX);

        if (sctx)
            *sctx = sctx2;
    }
    return rslt;
}

_AFX afxSoundDevice AfxGetSoundThreadDevice(afxSoundThread sthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sthr, afxFcc_STHR);
    afxSoundDevice sdev = sthr->sdev;
    AfxAssertObjects(1, &sdev, afxFcc_DDEV);
    return sdev;
}

_AFX void* AfxGetSoundThreadUdd(afxSoundThread sthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sthr, afxFcc_STHR);
    return sthr->udd;
}

_AFX afxThread AfxGetSoundThreadBase(afxSoundThread sthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sthr, afxFcc_STHR);
    afxThread thr = &sthr->thr;
    AfxAssertObjects(1, &thr, afxFcc_THR);
    return thr;
}

_AFX afxBool _AfxProcessSdevCb(afxSoundDevice sdev, void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxSoundThread sthr = (afxSoundThread)udd;
    AfxAssertObjects(1, &sthr, afxFcc_STHR);

    //AfxAssert(sthr == (afxSoundThread)AfxGetThreadUdd(&sthr->thr));

    if (AfxSoundDeviceIsRunning(sdev))
    {
        sthr->sdev = sdev;
        sthr->sctx = NIL;
        sthr->sque = NIL;

        if (sdev->procCb(sdev, sthr))
            AfxThrowError();

        sthr->sdev = NIL;
        sthr->sctx = NIL;
        sthr->sque = NIL;
    }
    return FALSE; // dont interrupt curation
}

_AFX afxError _AfxThrProcSthrCb(afxThread thr, void *udd, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;
    afxSoundThread sthr = (afxSoundThread)thr;// AfxRebase(thr, afxSoundThread, thr);
    //AfxAssert(sthr == (afxSoundThread)AfxGetThreadUdd(thr));
    AfxAssertObjects(1, &sthr, afxFcc_STHR);
    (void)opcode;

    //AfxAssert(sthr == (afxSoundThread)AfxGetThreadUdd(&sthr->thr));

    AfxCurateSoundDevices(0, AFX_N32_MAX, _AfxProcessSdevCb, sthr);

    return err;
}

_AFX afxError _AfxSthrCtor(afxSoundThread sthr, afxCookie const* cookie)
{
    AfxEntry("sthr=%p", sthr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sthr, afxFcc_STHR);

    afxSoundThreadConfig const *config = ((afxSoundThreadConfig const*)cookie->udd[0]) + cookie->no;

    sthr->thr.proc = _AfxThrProcSthrCb;

    sthr->sdev = NIL;
    sthr->sctx = NIL;
    sthr->sque = NIL;
    sthr->queueIdx = 0;

    sthr->udd = NIL;

    return err;
}

_AFX afxError _AfxSthrDtor(afxSoundThread sthr)
{
    AfxEntry("sthr=%p", sthr);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sthr, afxFcc_STHR);

    return err;
}

_AFX afxClassConfig const _sthrClsConfig =
{
    .fcc = afxFcc_STHR,
    .name = "Sound Thread",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxSoundThread)),
    .mmu = NIL,
    .ctor = (void*)_AfxSthrCtor,
    .dtor = (void*)_AfxSthrDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireSoundThreads(afxNat cnt, afxSoundThreadConfig const config[], afxSoundThread sthr[])
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cnt=%u,config=%p,sthr=%p", cnt, config, sthr);
    
    afxClass* cls = AfxGetSoundThreadClass();
    AfxAssertClass(cls, afxFcc_STHR);


    if (AfxAcquireObjects(cls, cnt, (afxObject*)sthr, (void const*[]) { config }))
        AfxThrowError();

    AfxAssertObjects(cnt, sthr, afxFcc_STHR);

    return err;
};

_AFX afxNat AfxCurateSoundThreads(afxNat first, afxNat cnt, afxBool(*f)(afxSoundThread, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetSoundThreadClass();
    AfxAssertClass(cls, afxFcc_STHR);
    return AfxCurateInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateSoundThreads(afxNat first, afxNat cnt, afxSoundThread sthr[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(sthr);
    afxClass* cls = AfxGetSoundThreadClass();
    AfxAssertClass(cls, afxFcc_STHR);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)sthr);
}

_AFX afxNat AfxCountSoundThreads(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetSoundThreadClass();
    AfxAssertClass(cls, afxFcc_STHR);
    return AfxCountInstances(cls);
}
