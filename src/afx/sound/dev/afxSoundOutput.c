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

 // This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _ASX_SOUND_C
#define _ASX_SOUND_SYSTEM_C
#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _ASX_SOUND_DEVICE_C
#define _ASX_SOUND_CONTEXT_C
#define _ASX_SOUND_OUTPUT_C
#include "AsxDevKit.h"

_ASX afxSoundDevice AfxGetSoundOutputDevice(afxSoundOutput sout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);
    afxSoundDevice sdev = AfxGetLinker(&sout->sdev);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

_ASX afxError AfxGetSoundOutputIdd(afxSoundOutput sout, afxNat code, void* dst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);
    sout->getIddCb(sout, code, dst);
    return err;
}

_ASX afxBool AfxGetSoundOutputContext(afxSoundOutput sout, afxSoundContext* context)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);
    afxSoundContext sctx = AfxGetLinker(&sout->sctx);
    AfxTryAssertObjects(1, &sctx, afxFcc_SCTX);
    AfxAssert(context);
    *context = sctx;
    return !!sctx;
}

_ASX afxError AfxDisconnectSoundOutput(afxSoundOutput sout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);

    if (!AfxReconnectSoundOutput(sout, NIL))
        AfxThrowError();

    AfxAssert(!AfxGetLinker(&sout->sctx));
    return err;
}

_ASX afxBool AfxReconnectSoundOutput(afxSoundOutput sout, afxSoundContext sctx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);
    afxSoundDevice sdev = AfxGetSoundOutputDevice(sout);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    AfxLockMutex(&sdev->relinkedCndMtx);

    while (sdev->relinking)
        AfxWaitCondition(&sdev->relinkedCnd, &sdev->relinkedCndMtx);

    afxSoundContext from;
    AfxGetSoundOutputContext(sout, &from);

    if (sctx != from)
    {
        sdev->relinking = TRUE;
        sout->reconnecting = TRUE;

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

        if (!sdev->soutRelinkCb)
        {
            if (from)
            {
                afxSoundOutput sout2;
                AfxIterateLinkageB2F(AFX_OBJECT(afxSoundOutput), sout2, &from->outputs, sctx)
                {
                    AfxAssertObjects(1, &sout2, afxFcc_SOUT);

                    if (sout2 == sout)
                    {
                        AfxPopLinkage(&sout->sctx);
                        //AfxRedoSoundOutputBuffers(sout);
                        AfxReleaseObjects(1, &from);
                        break;
                    }
                }
            }

            if (sctx)
            {
                AfxReacquireObjects(1, &sctx);
                AfxPushLinkage(&sout->sctx, &sctx->outputs);
                //AfxRedoSoundOutputBuffers(sout);
            }
        }
        else if (sdev->soutRelinkCb(sdev, sctx, 1, &sout))
            AfxThrowError();

        sout->reconnecting = FALSE;
        sdev->relinking = FALSE;
    }

    AfxUnlockMutex(&sdev->relinkedCndMtx);
    AfxSignalCondition(&sdev->relinkedCnd);
    return !err;
}

_ASX afxError _AsxSoutDtorCb(afxSoundOutput sout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);
    afxSoundDevice sdev = AfxGetSoundOutputDevice(sout);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    AfxDisconnectSoundOutput(sout);
    //sout->disabled;

    if (sdev->soutCloseCb && sdev->soutCloseCb(sdev, sout))
        AfxThrowError();

    AfxAssert(!sout->idd);

    return err;
}

_ASX afxError _AsxSoutCtorCb(afxSoundOutput sout, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    AfxZero(sout, sizeof(sout[0]));

    afxSoundDevice sdev = cookie->udd[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxUri const* endpoint = cookie->udd[1];
    afxSoundOutputConfig const* cfg = ((afxSoundOutputConfig const *)cookie->udd[2]) + cookie->no;
    AfxAssert(cfg);
    
    AfxPushLinkage(&sout->sdev, &sdev->openedSoutChain);
    AfxPushLinkage(&sout->sctx, NIL);

    sout->udd = cfg->udd;

    sout->reconnecting = FALSE;

    if (sdev->soutOpenCb(sdev, sout, cfg, endpoint)) AfxThrowError();
    else
    {

    }
    return err;
}

_ASX afxClassConfig const _AsxSoutClsCfg =
{
    .fcc = afxFcc_SOUT,
    .name = "SoundOutput",
    .desc = "Sound Output Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundOutput)),
    .ctor = (void*)_AsxSoutCtorCb,
    .dtor = (void*)_AsxSoutDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxOpenSoundOutput(afxNat sdevId, afxUri const* endpoint, afxSoundOutputConfig const* cfg, afxSoundOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxClass* cls = (afxClass*)AfxGetSoundOutputClass();
        AfxAssertClass(cls, afxFcc_SOUT);
        afxSoundOutput sout;

        if (AfxAcquireObjects(cls, 1, (afxObject*)&sout, (void const*[]) { sdev, endpoint, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &sout, afxFcc_SOUT);
            AfxAssert(output);
            *output = sout;
        }
    }
    return err;
}

_ASX afxError AfxAcquireSoundOutput(afxNat sdevId, afxSoundOutputConfig const* cfg, afxSoundOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cfg);
    afxSoundOutput sout;

    if (AfxOpenSoundOutput(sdevId, NIL, cfg, &sout)) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sout, afxFcc_SOUT);
        AfxAssert(output);
        *output = sout;
    }
    return err;
}

_ASXINL afxBool _AvxTestSoutIsFromSdevFltCb(afxSoundOutput sout, afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return (sdev == AfxGetSoundOutputDevice(sout));
}

_ASXINL afxBool _AvxTestSoutIsFromSdevFlt2Cb(afxSoundOutput sout, void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);
    afxSoundDevice sdev = udd[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return (sdev == AfxGetSoundOutputDevice(sout)) && ((afxBool(*)(afxSoundOutput, void*))udd[1])(sout, udd[2]);
}

_ASX afxNat AfxEnumerateSoundOutputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxSoundOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(outputs);
    afxClass const* cls = AfxGetSoundOutputClass();
    AfxAssertClass(cls, afxFcc_SOUT);
    afxNat rslt = 0;

    if (!sdev) rslt = AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)outputs);
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        rslt = AfxEvokeClassInstances(cls, (void*)_AvxTestSoutIsFromSdevFltCb, sdev, first, cnt, (afxObject*)outputs);
    }
    return rslt;
}

_ASX afxNat AfxEvokeSoundOutputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundOutput, void*), void* fdd, afxNat first, afxNat cnt, afxSoundOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(outputs);
    afxClass const* cls = AfxGetSoundOutputClass();
    AfxAssertClass(cls, afxFcc_SOUT);
    afxNat rslt = 0;

    if (!sdev) rslt = AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)outputs);
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        rslt = AfxEvokeClassInstances(cls, (void*)_AvxTestSoutIsFromSdevFlt2Cb, (void*[]) { sdev, flt, fdd }, first, cnt, (afxObject*)outputs);
    }
    return rslt;
}

_ASX afxNat AfxInvokeSoundOutputs(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*f)(afxSoundOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetSoundOutputClass();
    AfxAssertClass(cls, afxFcc_SOUT);
    afxNat rslt = 0;

    if (!sdev) rslt = AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)_AvxTestSoutIsFromSdevFltCb, sdev, (void*)f, udd);
    }
    return rslt;
}

_ASX afxNat AfxInvokeSoundOutputs2(afxSoundDevice sdev, afxNat first, afxNat cnt, afxBool(*flt)(afxSoundOutput, void*), void *fdd, afxBool(*f)(afxSoundOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetSoundOutputClass();
    AfxAssertClass(cls, afxFcc_SOUT);
    afxNat rslt = 0;

    if (!sdev) rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)flt, fdd, (void*)f, udd);
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        rslt = AfxInvokeClassInstances2(cls, first, cnt, (void*)_AvxTestSoutIsFromSdevFlt2Cb, (void*[]) { sdev, flt, fdd }, (void*)f, udd);
    }
    return rslt;
}
