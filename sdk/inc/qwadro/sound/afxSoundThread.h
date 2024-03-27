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

// This section is part of SIGMA A4D.

#ifndef AFX_SOUND_THREAD_H
#define AFX_SOUND_THREAD_H

#include "qwadro/core/afxThread.h"
#include "qwadro/sound/afxSoundDefs.h"

AFX_DEFINE_STRUCT(afxSoundThreadConfig)
{
    afxThreadConfig base;
    afxSoundDevice  sdev;
    void*           udd;
};

#ifdef _AFX_SOUND_C
#ifdef _AFX_SOUND_THREAD_C
AFX_OBJECT(afxSoundThread)
{
    AFX_OBJECT(afxThread)   thr;
    afxSoundDevice          sdev;
    afxSoundContext         sctx;
    afxNat                  portIdx;
    afxSoundQueue           sque;
    afxNat                  queueIdx;

    void*                   udd;
};
#endif
#endif

AAX afxError            AfxAcquireSoundThreads(afxSoundThreadConfig const* cfg, afxHint const hint, afxNat cnt, afxSoundThread sthr[]);

AAX afxSoundDevice      AfxGetSoundThreadDevice(afxSoundThread sthr);
AAX afxThread           AfxGetSoundThreadBase(afxSoundThread sthr);
AAX void*               AfxGetSoundThreadUdd(afxSoundThread sthr);

AAX afxBool             AfxGetSoundThreadActiveContext(afxSoundThread sthr, afxSoundContext* sctx);
AAX afxBool             AfxGetSoundThreadActiveQueue(afxSoundThread sthr, afxSoundQueue* sque);

#endif//AFX_SOUND_THREAD_H
