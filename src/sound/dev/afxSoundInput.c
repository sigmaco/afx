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

 // This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#define _ASX_SOUND_C
#define _ASX_SOUND_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _ASX_SOUND_DEVICE_C
#define _ASX_SOUND_CONTEXT_C
#define _ASX_SOUND_INPUT_C
#include "../../dev/AsxImplKit.h"

_ASX afxSoundDevice AfxGetSoundInputDevice(afxSoundInput sin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);
    afxSoundDevice sdev = AfxGetProvider(sin);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

_ASX void* AfxGetSoundInputUdd(afxSoundInput sin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);
    return sin->udd;
}

_ASX afxBool AfxGetSoundInputContext(afxSoundInput sin, afxSoundContext* context)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);
    afxSoundContext sctx = AfxGetLinker(&sin->sctx);
    AfxTryAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT(context);
    *context = sctx;
    return !!sctx;
}

_ASX afxBool AfxReconnectSoundInput(afxSoundInput sin, afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);
    afxSoundDevice sdev = AfxGetSoundInputDevice(sin);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    AfxLockMutex(&sdev->relinkedCndMtx);

    while (sdev->relinking)
        AfxWaitCondition(&sdev->relinkedCnd, &sdev->relinkedCndMtx);

    afxSoundContext from;
    AfxGetSoundInputContext(sin, &from);

    if (sctx != from)
    {
        sdev->relinking = TRUE;
        sin->reconnecting = TRUE;

        if (sctx)
        {
            AfxAssertObjects(1, &sctx, afxFcc_SCTX);
            afxSoundDevice sdrv2 = AfxGetSoundContextDevice(sctx);
            AfxAssertObjects(1, &sdrv2, afxFcc_SDEV);

            if (sdev != sdrv2) // can not connect to context acquired of another device
            {
                AfxThrowError();
                sctx = NIL; // so just disconnect
            }
        }

        if (!sdev->relinkSinCb)
        {
            if (from)
            {
                afxSoundInput sin2;
                AfxIterateLinkageB2F(AFX_OBJECT(afxSoundInput), sin2, &from->inputs, sctx)
                {
                    AfxAssertObjects(1, &sin2, afxFcc_SIN);

                    if (sin2 == sin)
                    {
                        AfxPopLinkage(&sin->sctx);
                        AfxExhaustChainedClasses(&sin->classes);
                        AfxReleaseObjects(1, &from);
                        break;
                    }
                }
            }

            if (sctx)
            {
                AfxReacquireObjects(1, &sctx);
                AfxPushLinkage(&sin->sctx, &sctx->inputs);

            }
        }
        else if (sdev->relinkSinCb(sdev, sctx, 1, &sin))
            AfxThrowError();

        sin->reconnecting = FALSE;
        sdev->relinking = FALSE;
    }

    AfxUnlockMutex(&sdev->relinkedCndMtx);
    AfxSignalCondition(&sdev->relinkedCnd);
    return !err;
}

_ASX afxError AfxDisconnectSoundInput(afxSoundInput sin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);

    if (!AfxReconnectSoundInput(sin, NIL))
        AfxThrowError();

    AFX_ASSERT(!AfxGetLinker(&sin->sctx));
    return err;
}

_ASX afxError _AsxSinDtorCb(afxSoundInput sin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);

    AfxDisconnectSoundInput(sin);

    afxSoundDevice sdev = AfxGetSoundInputDevice(sin);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    if (sdev->closeSinCb && sdev->closeSinCb(sdev, sin))
        AfxThrowError();

    AFX_ASSERT(!sin->idd);

    AfxDeregisterChainedClasses(&sin->classes);

    return err;
}

_ASX afxError _AsxSinCtorCb(afxSoundInput sin, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);

    AfxZero(sin, sizeof(sin[0]));

    afxSoundDevice sdev = args[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxSoundInputConfig const* cfg = ((afxSoundInputConfig const *)args[1]) + invokeNo;

    AfxPushLinkage(&sin->sctx, NIL);

    afxChain *classes = &sin->classes;
    AfxDeployChain(classes, (void*)sin);

    afxUnit defStreamSiz = 32000000; // 32MB (I think it is the total of RAM in PlayStation 2)

    sin->udd = cfg && cfg->udd ? cfg->udd : NIL;

    sin->procCb = cfg ? cfg->proc : NIL;

    sin->udd = NIL;

    afxClassConfig tmpClsConf;

    if (sdev->openSinCb && sdev->openSinCb(sdev, sin, cfg)) AfxThrowError();
    else
    {

    }
    return err;
}

_ASX afxClassConfig const _AsxSinStdImplementation =
{
    .fcc = afxFcc_SIN,
    .name = "SoundInput",
    .desc = "Sound Input Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundInput)),
    .ctor = (void*)_AsxSinCtorCb,
    .dtor = (void*)_AsxSinDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxConfigureSoundInput(afxUnit sdevId, afxSoundInputConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(sdevId != AFX_INVALID_INDEX);
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        AFX_ASSERT(cfg);
        *cfg = (afxSoundInputConfig) { 0 };
    }
    return err;
}

_ASX afxError AfxOpenSoundInput(afxUnit sdevId, afxSoundInputConfig const* cfg, afxSoundInput* input)
{
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxClass* cls = (afxClass*)AfxGetSoundInputClass(sdev);
        AfxAssertClass(cls, afxFcc_SIN);
        afxSoundInput sin;

        if (AfxAcquireObjects(cls, 1, (afxObject*)&sin, (void const*[]) { sdev, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &sin, afxFcc_SIN);
            AFX_ASSERT(input);
            *input = sin;
        }
    }
    return err;
}

_ASX afxUnit AfxInvokeSoundInputs(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxBool(*f)(afxSoundInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = AfxGetSoundInputClass(sdev);
    AfxAssertClass(cls, afxFcc_SIN);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_ASX afxUnit AfxEvokeSoundInputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundInput, void*), void* fdd, afxUnit first, afxUnit cnt, afxSoundInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AFX_ASSERT(inputs);
    AFX_ASSERT(flt);
    AFX_ASSERT(cnt);
    afxClass const* cls = AfxGetSoundInputClass(sdev);
    AfxAssertClass(cls, afxFcc_SIN);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)inputs);
}

_ASX afxUnit AfxEnumerateSoundInputs(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxSoundInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AFX_ASSERT(inputs);
    AFX_ASSERT(cnt);
    afxClass const* cls = AfxGetSoundInputClass(sdev);
    AfxAssertClass(cls, afxFcc_SIN);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)inputs);
}
