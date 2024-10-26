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

#define _ASX_SOUND_C
#define _ASX_TRACKER_C
#include "../dev/AsxImplKit.h"
#include "qwadro/inc/sound/asxTracker.h"

_ASXINL afxSoundContext AsxGetTrackerContext(asxTracker trax)
{
    afxError err;
    AfxAssertObjects(1, &trax, afxFcc_TRAX);
    afxSoundContext sctx = AfxGetProvider(trax);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    return sctx;
}

_ASXINL void* AsxGetTrackerUdd(asxTracker trax)
{
    afxError err;
    AfxAssertObjects(1, &trax, afxFcc_TRAX);
    return trax->udd;
}


_ASX afxError _AsxTraxStdDtorCb(asxTracker trax)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &trax, afxFcc_TRAX);

    afxSoundContext sctx = AsxGetTrackerContext(trax);

    return err;
}

_ASX afxError _AsxTraxStdCtorCb(afxWaveform trax, void** args, afxUnit invokeNo)
{
    afxResult err = NIL;
    AfxAssertObjects(1, &trax, afxFcc_TRAX);

    afxSoundContext sctx = args[0];
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);



    return err;
}

_ASX afxClassConfig const _AsxTraxStdImplementation =
{
    .fcc = afxFcc_TRAX,
    .name = "Tracker",
    .desc = "Tracker",
    .fixedSiz = sizeof(AFX_OBJECT(asxTracker)),
    .ctor = (void*)_AsxTraxStdCtorCb,
    .dtor = (void*)_AsxTraxStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_ASX afxError AsxAcquireTrackers(afxSoundContext sctx, afxUnit cnt, asxTracker trackers[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
    AFX_ASSERT(trackers);
    AFX_ASSERT(cnt);

    afxClass* cls = (afxClass*)AfxGetTrackerClass(sctx);
    AfxAssertClass(cls, afxFcc_TRAX);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)trackers, (void const*[]) { sctx }))
        AfxThrowError();

    return err;
}
