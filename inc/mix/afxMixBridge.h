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

/*
    A mix bridge, as well as other bridges and queues in Qwadro, is highly based on Commondore Amiga-way 
    of handling device connections, here specially AHI connection. Despite the main enginner never having touched on in his life.
*/

#ifndef AMX_MIX_BRIDGE_H
#define AMX_MIX_BRIDGE_H

#include "qwadro/inc/mix/afxMixQueue.h"

#define AMX_MAX_QUEUES_PER_BRIDGE (32)

typedef enum afxMixPortFlag
{
    afxMixPortFlag_MIX = AFX_BIT(0), // supports mixing
    afxMixPortFlag_SIM = AFX_BIT(1)
} afxMixPortFlags;

AFX_DEFINE_STRUCT(afxMixCapabilities)
{
    afxMixPortFlags capabilities;
    afxAcceleration acceleration;
    afxUnit         minQueCnt; // usually 3
    afxUnit         maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
};

AFX_DEFINE_STRUCT(afxMixBridgeConfig)
{
    afxUnit         mdevId;
    afxMixPortFlags capabilities; // specifies capabilities of queues in a port.
    afxAcceleration acceleration;
    afxUnit         minQueCnt;
    afxReal const*  queuePriority;
};

AMX afxMixSystem    AmxGetBridgedMixSystem(afxMixBridge mexu);
AMX afxMixDevice    AmxGetBridgedMixDevice(afxMixBridge mexu, afxUnit* portId);

AMX afxUnit         AmxGetMixQueues(afxMixBridge mexu, afxUnit first, afxUnit cnt, afxMixQueue queues[]);

AMX afxError        AmxWaitForIdleMixBridge(afxMixBridge mexu, afxUnit64 timeout);
AMX afxError        AmxWaitForEmptyMixQueue(afxMixBridge mexu, afxUnit queIdx, afxUnit64 timeout);

#endif//AMX_MIX_BRIDGE_H
