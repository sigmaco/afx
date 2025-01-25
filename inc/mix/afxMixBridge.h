/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>

#ifndef AMX_MIX_BRIDGE_H
#define AMX_MIX_BRIDGE_H

#include "qwadro/inc/mix/afxMixDefs.h"

typedef enum afxMixPortFlag
{
    afxMixPortFlag_MIX = AFX_BIT(0), // supports mixing
    afxMixPortFlag_SIM = AFX_BIT(1)
} afxMixPortFlags;

AFX_DEFINE_STRUCT(afxMixPortCaps)
{
    afxMixPortFlags     capabilities;
    afxAcceleration     acceleration;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
};

AFX_DEFINE_STRUCT(afxMixBridgeConfig)
{
    afxUnit             sdevId;
    afxMixPortFlags     capabilities; // specifies capabilities of queues in a port.
    afxAcceleration     acceleration;
    afxUnit             minQueCnt;
    afxReal const*      queuePriority;
};

// the submission thing

AFX_DEFINE_STRUCT(amxSubmission)
{
    afxUnit             portIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxSemaphore        signalSems;
    avxFence            fence;
};

AFX_DEFINE_STRUCT(amxFlush)
{
    afxUnit             portIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;
    afxSemaphore        wait;
};

AFX_DEFINE_STRUCT(amxTransference)
{
    afxUnit             portIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxSemaphore        wait;
    afxSemaphore        signal;
    avxFence            fence;

    union
    {
        afxAudio    wav;
        afxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               src;
    afxFcc          srcFcc;
    union
    {
        afxAudio    wav;
        afxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               dst;
    afxFcc          dstFcc;

    afxCodec        codec;
    afxUnit         encSiz;
    afxUnit         decSiz;
};

AMX afxMixDevice    AfxGetMixBridgeDevice(afxMixBridge mexu);
AMX afxMixSystem    AfxGetMixBridgeSystem(afxMixBridge mexu);

AMX afxUnit         AfxQueryMixBridgePort(afxMixBridge mexu, afxMixDevice* device);

AMX afxUnit         AfxGetMixQueues(afxMixBridge mexu, afxUnit first, afxUnit cnt, afxMixQueue queues[]);

AMX afxError        AfxWaitForIdleMixBridge(afxMixBridge mexu, afxTime timeout);
AMX afxError        AfxWaitForEmptyMixQueue(afxMixBridge mexu, afxUnit queIdx, afxTime timeout);

// MIX QUEUE METHODS

AMX afxMixDevice    AfxGetMixQueueDevice(afxMixQueue mque);
AMX afxMixSystem    AfxGetMixQueueContext(afxMixQueue mque);
AMX afxUnit         AfxGetMixQueuePort(afxMixQueue mque);

#endif//AMX_MIX_BRIDGE_H
