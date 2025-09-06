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

#ifndef AFX_IO_BRIDGE_H
#define AFX_IO_BRIDGE_H

#include "qwadro/exec/afxDevice.h"
#include "qwadro/io/afxFile.h"

typedef enum afxIoCap
{
    afxIoCap_           = AFX_BITMASK(0),
} afxIoCaps;

AFX_DEFINE_STRUCT(afxIoPortCaps)
{
    afxIoCaps           capabilites;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxAcceleration     acceleration;

    afxBool8            transfer;
    afxBool8            encode;
    afxBool8            decode;
};

AFX_DEFINE_STRUCT(afxIoBridgeConfig)
{
    afxUnit             devId;
    afxIoCaps           capabilities;
    afxUnit             minQueCnt;
    afxReal const*      queuePriority;
};

typedef enum
{
    afxIoQueueOp_WORK = 1,
    afxIoQueueOp_TRANSFER,
    afxIoQueueOp_MAPPING
} afxIoQueueOpType;

AFX_DEFINE_STRUCT(afxSubmission)
{
    afxUnit             exuIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxUnit64           waitValues;
    afxUnit32           waitReserveds;
    afxSemaphore        signalSems;
    afxUnit64           signalValues;
    afxUnit32           signalReserveds;
    avxFence            fence;
};

AFX_DEFINE_STRUCT(afxTransference)
{
    afxUnit             portIdx;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxSemaphore        wait;
    afxSemaphore        signal;
    avxFence            fence;

    afxFcc              srcFcc;
    afxFcc              dstFcc;
    afxCodec            codec;
    afxUnit             decSiz;

    union
    {
        struct
        {
            //avxRaster       ras;
            //avxRasterIo     op;
            union
            {
                void*       dst;
                void const* src;
                //avxBuffer   buf;
                afxStream   iob;
            };
        }                   img;
        struct
        {
            //avxBuffer       buf;
            //avxBufferIo     op;
            union
            {
                void*       dst;
                void const* src;
                afxStream   iob;
            };
        }                   data;
    };
};

AFX afxDevice       AfxGetIoBridgeDevice(afxIoBridge exu);
AFX afxDevLink      AfxGetIoBridgeContext(afxIoBridge exu);

AFX afxUnit         AfxGetIoBridgePort(afxIoBridge xexu);

AFX afxUnit         AfxGetIoQueues(afxIoBridge xexu, afxUnit baseQueIdx, afxUnit cnt, afxIoQueue queues[]);

AFX afxError        _AfxWaitForEmptyIoQueue(afxIoBridge exu, afxUnit queIdx, afxTime timeout);
AFX afxError        _AfxWaitForIdleIoBridge(afxIoBridge exu, afxTime timeout);

AFX afxError        _AfxSubmitIoCommands(afxIoBridge xexu, afxSubmission const* ctrl, afxUnit cnt, afxObject cmdbs[]);


AFX afxUnit         AfxGetIoQueuePort(afxIoQueue xque);
AFX afxDevLink      AfxGetIoQueueContext(afxIoQueue xque);

////////////////////////////////////////////////////////////////////////////////

AFX afxError AfxAcquireIoBridge(afxIoBridgeConfig const* cfg, afxIoBridge* bridge);

#endif//AFX_IO_BRIDGE_H
