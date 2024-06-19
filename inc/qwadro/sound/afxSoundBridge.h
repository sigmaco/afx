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

#ifndef ASX_SOUND_BRIDGE_H
#define ASX_SOUND_BRIDGE_H

#include "qwadro/sound/afxSoundContext.h"

AFX_DEFINE_STRUCT(afxExecutionRequest2)
{
    afxSemaphore        wait;
    //afxSoundStream      cmdb;
    afxSemaphore        signal;
};

AFX_DEFINE_STRUCT(afxTransferRequest2)
{
    afxFcc              srcFcc;
    afxFcc              dstFcc;

    afxCodec            codec;
    afxNat              decompressedSiz;

    afxSemaphore        wait;
    afxSemaphore        signal;
};

AFX_DEFINE_STRUCT(afxSoundBridgeConfig)
{
    afxFlags            flags;
    afxNat              portIdx;
    afxNat              queueCnt;
    afxReal const*      queuePriority;
};

ASX afxSoundDevice      AfxGetSoundBridgeDevice(afxSoundBridge sdge);
ASX afxSoundContext     AfxGetSoundBridgeContext(afxSoundBridge sdge);

#endif//ASX_SOUND_BRIDGE_H
