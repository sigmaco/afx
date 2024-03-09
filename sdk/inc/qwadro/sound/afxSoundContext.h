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

#ifndef AFX_SOUND_CONTEXT_H
#define AFX_SOUND_CONTEXT_H

#include "qwadro/sound/afxSoundBuffer.h"
#include "qwadro/sound/afxListener.h"

#ifdef _AFX_SOUND_C
#ifdef _AFX_SOUND_CONTEXT_C
#ifndef _AFX_SOUND_CONTEXT_IMPL
AFX_OBJECT(afxSoundContext)
#else
struct _afxBaseSoundContext
#endif
{
    afxError            (*procCb)(afxSoundContext, afxSoundThread);
    afxBool             running;

    afxChain            classes;

    afxMmu              mmu;
    afxArena            aren;

    afxNat              lastReqPortIdx;
    afxNat              openPortCnt;
    struct
    {

        //afxSlock            recyclSubmChainSlock;
        //afxChain            recyclSubmChain;
        //afxNat              minRecyclSubmCnt;
        afxNat          lastReqQueIdx;
        //afxClass        queues;
        //afxClass        scripts;
        afxNat          dqueCnt;
        afxSoundQueue*  queues;
    }*openPorts;

    afxChain            inlinks;
    afxChain            outlinks;

    afxClass            buffers;
};
#endif//_AFX_SOUND_CONTEXT_C
#endif//_AFX_SOUND_C

AAX afxSoundDevice      AfxGetSoundContextDevice(afxSoundContext sctx);
AAX afxMmu              AfxGetSoundContextMmu(afxSoundContext sctx);

#endif//AFX_SOUND_CONTEXT_H
