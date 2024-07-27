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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_CONTEXT_C
#define _AFX_SYSTEM_C

#define _ASX_SOUND_C
#define _ASX_SOUND_DEVICE_C
#define _ASX_SOUND_INPUT_C
#define _ASX_SOUND_OUTPUT_C
#define _ASX_SOUND_SYSTEM_C
#define _ASX_SOUND_CONTEXT_C
#include "AsxDevKit.h"

_ASX afxBool AfxSoundDeviceIsRunning(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev->dev.serving;
}

_ASX afxNat AfxCountSoundPorts(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return sdev->portCnt;
}

_ASX afxClass* AfxGetSoundContextClass(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);
    return &sdev->sctxCls;
}

_ASX afxError _AsxSdevDtorCb(afxSoundDevice sdev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sdev, afxFcc_SDEV);

    AfxAssert(sdev->stopCb);

    if (sdev->stopCb(sdev))
        AfxThrowError();

    AfxAssert(!sdev->idd);
    AfxCleanUpChainedClasses(&sdev->dev.classes);

    AfxCleanUpMutex(&sdev->relinkedCndMtx);
    AfxCleanUpCondition(&sdev->relinkedCnd);

    return err;
}

_ASX afxError _AsxSdevCtorCb(afxSoundDevice sdev, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;

    //afxSoundSystem ssys = cookie->udd[0];
    //AfxAssertObjects(1, &ssys, afxFcc_SSYS);
    //afxSoundDeviceInfo const* info = ((afxSoundDeviceInfo const *)cookie->udd[1]) + cookie->no;
    //AfxAssert(info);

    sdev->dev.serving = FALSE;

    AfxDeployChain(&sdev->openedSoutChain, sdev);
    AfxDeployChain(&sdev->openedSinChain, sdev);

    AfxSetUpCondition(&sdev->relinkedCnd);
    AfxSetUpMutex(&sdev->relinkedCndMtx, AFX_MTX_PLAIN);

    sdev->dev.procCb = NIL;
    sdev->idd = NIL;
    sdev->startCb = NIL;
    sdev->stopCb = NIL;
    sdev->openCb = NIL;
    sdev->closeCb = NIL;
    sdev->sinOpenCb = NIL;
    sdev->sinCloseCb = NIL;
    sdev->sinRelinkCb = NIL;
    sdev->soutCloseCb = NIL;
    sdev->soutOpenCb = NIL;
    sdev->soutRelinkCb = NIL;

    if (AfxCallDevice(&sdev->dev, afxFcc_SSYS, NIL)) AfxThrowError();
    else
    {
        if (!sdev->portCnt) AfxThrowError();
        else
        {
            AfxAssert(sdev->portCaps);
        }

        if (err)
        {
            AfxCleanUpChainedClasses(&sdev->dev.classes);
        }
    }
    return err;
}

_ASX afxClassConfig const _AsxSdevClsCfg =
{
    .fcc = afxFcc_SDEV,
    .name = "SoundDevice",
    .desc = "Sound Device Driver Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxSoundDevice)),
    .ctor = (void*)_AsxSdevCtorCb,
    .dtor = (void*)_AsxSdevDtorCb
};
