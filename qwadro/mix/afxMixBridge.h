/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA A4D <https://sigmaco.org/a4d>
// This software is part of Advanced Multimedia Extensions & Experiments.

/*
    A mix bridge, as well as other bridges and queues in Qwadro, is highly based on Commondore Amiga-way 
    of handling device connections, here specially AHI connection. Despite the main enginner never having touched on in his life.
*/

#ifndef AMX_MIX_BRIDGE_H
#define AMX_MIX_BRIDGE_H

#include "qwadro/mix/afxMixQueue.h"

#define AMX_MAX_QUEUES_PER_BRIDGE (32)

typedef enum afxMixCaps
{
    afxMixCaps_MIX      = AFX_BITMASK(0), // supports mixing
    afxMixCaps_COMPUTE  = AFX_BITMASK(1),
    afxMixCaps_TRANSFER = AFX_BITMASK(2),
    afxMixCaps_SINK     = AFX_BITMASK(3)
} afxMixCaps;

AFX_DEFINE_STRUCT(afxMixPortInfo)
{
    afxMixCaps      capabilities;
    afxAcceleration acceleration;
    afxUnit         minQueCnt; // usually 3
    afxUnit         maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
};

AFX_DEFINE_STRUCT(afxMixBridgeConfig)
{
    afxUnit         mdevId;
    afxAcceleration acceleration;
    afxMixCaps      capabilities; // specifies capabilities of queues in a port.
    afxUnit         minQueCnt;
    afxReal const*  queuePriority;
};

AMX afxMixSystem    AmxGetBridgedMixSystem(afxMixBridge mexu, afxUnit* bridgeId);
AMX afxMixDevice    AmxGetBridgedMixDevice(afxMixBridge mexu, afxUnit* mdevId);

AMX afxUnit         AmxGetMixQueues(afxMixBridge mexu, afxUnit first, afxUnit cnt, afxMixQueue queues[]);

AMX afxError        AmxWaitForIdleMixBridge(afxMixBridge mexu, afxUnit64 timeout);
AMX afxError        AmxWaitForEmptyMixQueue(afxMixBridge mexu, afxUnit queIdx, afxUnit64 timeout);

#endif//AMX_MIX_BRIDGE_H
