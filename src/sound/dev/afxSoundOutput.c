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
#include "../../dev/AsxImplKit.h"

_ASX afxSoundDevice AfxGetSoundOutputDevice(afxSoundOutput sout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);
    afxSoundDevice sdev = AfxGetProvider(sout);
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev;
}

_ASX afxError AfxGetSoundOutputIdd(afxSoundOutput sout, afxUnit code, void* dst)
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
    AFX_ASSERT(context);
    *context = sctx;
    return !!sctx;
}

_ASX afxError AfxDisconnectSoundOutput(afxSoundOutput sout)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sout, afxFcc_SOUT);

    if (!AfxReconnectSoundOutput(sout, NIL))
        AfxThrowError();

    AFX_ASSERT(!AfxGetLinker(&sout->sctx));
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

        if (!sdev->relinkSoutCb)
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
        else if (sdev->relinkSoutCb(sdev, sctx, 1, &sout))
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

    if (sdev->closeSoutCb && sdev->closeSoutCb(sdev, sout))
        AfxThrowError();

    AFX_ASSERT(!sout->idd);

    return err;
}

_ASX afxError _AsxSoutCtorCb(afxSoundOutput sout, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;

    AfxZero(sout, sizeof(sout[0]));

    afxSoundDevice sdev = args[0];
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    afxSoundOutputConfig const* cfg = ((afxSoundOutputConfig const *)args[1]) + invokeNo;
    AFX_ASSERT(cfg);
    
    AfxPushLinkage(&sout->sctx, NIL);

    sout->udd = cfg->udd;

    sout->reconnecting = FALSE;

    if (sdev->openSoutCb(sdev, sout, cfg)) AfxThrowError();
    else
    {

    }
    return err;
}

_ASX afxClassConfig const _AsxSoutStdImplementation =
{
    .fcc = afxFcc_SOUT,
    .name = "SoundOutput",
    .desc = "Sound Output Mechanism",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundOutput)),
    .ctor = (void*)_AsxSoutCtorCb,
    .dtor = (void*)_AsxSoutDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AfxConfigureSoundOutput(afxUnit sdevId, afxSoundOutputConfig* cfg)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(sdevId != AFX_INVALID_INDEX);
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        AFX_ASSERT(cfg);
        *cfg = (afxSoundOutputConfig) { 0 };
    }
    return err;
}

_ASX afxError AfxOpenSoundOutput(afxUnit sdevId, afxSoundOutputConfig const* cfg, afxSoundOutput* output)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cfg);
    afxSoundDevice sdev;

    if (!(AfxGetSoundDevice(sdevId, &sdev))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &sdev, afxFcc_SDEV);
        afxClass* cls = (afxClass*)AfxGetSoundOutputClass(sdev);
        AfxAssertClass(cls, afxFcc_SOUT);
        afxSoundOutput sout;

        if (AfxAcquireObjects(cls, 1, (afxObject*)&sout, (void const*[]) { sdev, cfg })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &sout, afxFcc_SOUT);
            AFX_ASSERT(output);
            *output = sout;
        }
    }
    return err;
}

_ASX afxUnit AfxInvokeSoundOutputs(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxBool(*f)(afxSoundOutput, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = AfxGetSoundOutputClass(sdev);
    AfxAssertClass(cls, afxFcc_SOUT);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_ASX afxUnit AfxEvokeSoundOutputs(afxSoundDevice sdev, afxBool(*flt)(afxSoundOutput, void*), void* fdd, afxUnit first, afxUnit cnt, afxSoundOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AFX_ASSERT(cnt);
    AFX_ASSERT(outputs);
    afxClass const* cls = AfxGetSoundOutputClass(sdev);
    AfxAssertClass(cls, afxFcc_SOUT);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)outputs);
}

_ASX afxUnit AfxEnumerateSoundOutputs(afxSoundDevice sdev, afxUnit first, afxUnit cnt, afxSoundOutput outputs[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    AFX_ASSERT(cnt);
    AFX_ASSERT(outputs);
    afxClass const* cls = AfxGetSoundOutputClass(sdev);
    AfxAssertClass(cls, afxFcc_SOUT);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)outputs);
}
