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
#include "../src/afx/sound/dev/asxDevKit.h"

_ASX void* AfxGetSoundInputUdd(afxSoundInput sin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);
    return sin->udd;
}

_ASX afxSoundDevice AfxGetSoundInputDevice(afxSoundInput sin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);
    afxSoundDevice sdev = AfxGetLinker(&sin->sdev);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

_ASX afxBool AfxGetSoundInputContext(afxSoundInput sin, afxSoundContext* context)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);
    afxSoundContext sctx = AfxGetLinker(&sin->sctx);
    AfxTryAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssert(context);
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

        if (!sdev->sinRelinkCb)
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
        else if (sdev->sinRelinkCb(sdev, sctx, 1, &sin))
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

    AfxAssert(!AfxGetLinker(&sin->sctx));
    return err;
}

_ASX afxError _AsxSinDtorCb(afxSoundInput sin)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);

    AfxDisconnectSoundInput(sin);

    afxSoundDevice sdev = AfxGetSoundInputDevice(sin);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    if (sdev->sinCloseCb && sdev->sinCloseCb(sdev, sin))
        AfxThrowError();

    AfxAssert(!sin->idd);

    AfxCleanUpChainedClasses(&sin->classes);

    return err;
}

_ASX afxError _AsxSinCtorCb(afxSoundInput sin, afxCookie const *cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);

    AfxZero(sin, sizeof(sin[0]));

    afxSoundDevice sdev = cookie->udd[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxUri const* endpoint = cookie->udd[1];
    afxSoundInputConfig const* cfg = ((afxSoundInputConfig const *)cookie->udd[2]) + cookie->no;

    AfxPushLinkage(&sin->sdev, &sdev->openedSinChain);
    AfxPushLinkage(&sin->sctx, NIL);

    afxChain *classes = &sin->classes;
    AfxSetUpChain(classes, (void*)sin);

    afxNat defStreamSiz = 32000000; // 32MB (I think it is the total of RAM in PlayStation 2)

    sin->udd = cfg && cfg->udd ? cfg->udd : NIL;

    sin->procCb = cfg ? cfg->proc : NIL;

    sin->udd = NIL;

    afxClassConfig tmpClsConf;

    if (sdev->sinOpenCb && sdev->sinOpenCb(sdev, sin, cfg, endpoint)) AfxThrowError();
    else
    {

    }
    return err;
}

_ASX afxClassConfig const _AsxSinClsCfg =
{
    .fcc = afxFcc_SIN,
    .name = "SoundInput",
    .desc = "Sound Input Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundInput)),
    .ctor = (void*)_AsxSinCtorCb,
    .dtor = (void*)_AsxSinDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxOpenSoundInput(afxNat sdevId, afxUri const* endpoint, afxSoundInputConfig const* cfg, afxSoundInput* input)
{
    afxError err = AFX_ERR_NONE;
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxClass* cls = (afxClass*)AfxGetSoundInputClass();
        AfxAssertClass(cls, afxFcc_SIN);
        afxSoundInput sin;

        if (AfxAcquireObjects(cls, 1, (afxObject*)&sin, (void const*[]) { sdev, endpoint, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &sin, afxFcc_SIN);
            AfxAssert(input);
            *input = sin;
        }
    }
    return err;
}

_ASX afxError AfxAcquireSoundInput(afxNat sdevId, afxSoundInputConfig const* cfg, afxSoundInput* input)
{
    afxError err = AFX_ERR_NONE;
    afxSoundInput sin = NIL;

    if (AfxOpenSoundInput(sdevId, NIL, cfg, &sin)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sin, afxFcc_SIN);
        AfxAssert(input);
        *input = sin;
    }
    return err;
}

_ASXINL afxBool _AvxTestSinIsFromSdevFltCb(afxSoundInput sin, afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return (sdev == AfxGetSoundInputDevice(sin));
}

_ASXINL afxBool _AvxTestSinIsFromSdevFlt2Cb(afxSoundInput sin, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sin, afxFcc_SIN);
    afxSoundDevice sdev = udd[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return (sdev == AfxGetSoundInputDevice(sin)) && ((afxBool(*)(afxSoundInput, void*))udd[1])(sin, udd[2]);
}

_ASX afxNat AfxEnumerateSoundInputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(inputs);
    AfxAssert(cnt);
    afxClass const* cls = AfxGetSoundInputClass();
    AfxAssertClass(cls, afxFcc_SIN);
    afxNat rslt = 0;

    if (!sdev) rslt = AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)inputs);
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        rslt = AfxEvokeClassInstances(cls, (void*)_AvxTestSinIsFromSdevFltCb, sdev, first, cnt, (afxObject*)inputs);
    }
    return rslt;
}

_ASX afxNat AfxEvokeSoundInputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundInput, void*), void* fdd, afxNat first, afxNat cnt, afxSoundInput inputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(inputs);
    AfxAssert(flt);
    AfxAssert(cnt);
    afxClass const* cls = AfxGetSoundInputClass();
    AfxAssertClass(cls, afxFcc_SIN);
    afxNat rslt = 0;

    if (!sdev) rslt = AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)inputs);
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        rslt = AfxEvokeClassInstances(cls, (void*)_AvxTestSinIsFromSdevFlt2Cb, (void*[]) { sdev, flt, fdd }, first, cnt, (afxObject*)inputs);
    }
    return rslt;
}

_ASX afxNat AfxInvokeSoundInputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetSoundInputClass();
    AfxAssertClass(cls, afxFcc_SIN);
    afxNat rslt = 0;

    if (!sdev) rslt = AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)_AvxTestSinIsFromSdevFltCb, sdev, (void*)f, udd);
    }
    return rslt;
}

_ASX afxNat AfxInvokeSoundInputs2(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*flt)(afxSoundInput, void*), void *fdd, afxBool(*f)(afxSoundInput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetSoundInputClass();
    AfxAssertClass(cls, afxFcc_SIN);
    afxNat rslt = 0;

    if (!sdev) rslt = AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)_AvxTestSinIsFromSdevFlt2Cb, (void*[]) { sdev, flt, fdd }, (void*)f, udd);
    }
    return rslt;
}
