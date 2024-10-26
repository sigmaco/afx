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

#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/io/afxFile.h"

typedef enum afxIoBridgeFlag
{
    afxIoBridgeFlag_      = AFX_BIT(0),
} afxIoBridgeFlags;

AFX_DEFINE_STRUCT(afxIoPortCaps)
{
    afxIoBridgeFlags    flags;
    afxUnit              minQueCnt; // usually 3
    afxUnit              maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxAcceleration     acceleration;

    afxBool8            transfer;
    afxBool8            encode;
    afxBool8            decode;
};

AFX_DEFINE_STRUCT(afxIoBridgeConfig)
{
    afxUnit              portId;
    afxUnit              minQueCnt;
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
    afxUnit              portId;
    afxUnit              exuIdx;
    afxUnit              exuCnt;
    afxUnit              queIdx;
    afxUnit              queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxUnit64            waitValues;
    afxUnit32            waitReserveds;
    afxSemaphore        signalSems;
    afxUnit64            signalValues;
    afxUnit32            signalReserveds;
    afxFence            fence;
};

AFX_DEFINE_STRUCT(afxTransference)
{
    afxUnit              portIdx;
    afxUnit              baseQueIdx;
    afxUnit              queCnt;

    afxSemaphore        wait;
    afxSemaphore        signal;
    afxFence            fence;

    afxFcc              srcFcc;
    afxFcc              dstFcc;
    afxCodec            codec;
    afxUnit              decSiz;

    union
    {
        struct
        {
            //afxRaster       ras;
            //afxRasterIo     op;
            union
            {
                void*       dst;
                void const* src;
                //afxBuffer   buf;
                afxStream   iob;
            };
        }                   img;
        struct
        {
            //afxBuffer       buf;
            //afxBufferIo     op;
            union
            {
                void*       dst;
                void const* src;
                afxStream   iob;
            };
        }                   data;
    };
};

AFX afxUnit              AfxGetIoBridgePort(afxIoBridge xexu);

AFX afxUnit              AfxGetIoQueues(afxIoBridge xexu, afxUnit first, afxUnit cnt, afxIoQueue queues[]);

AFX afxError            _AfxSubmitIoCommands(afxIoBridge xexu, afxSubmission const* ctrl, afxUnit cnt, afxObject cmdbs[]);
AFX afxError            _AfxSubmitTransferences(afxIoBridge xexu, afxTransference const* ctrl, afxUnit opCnt, void const* ops);

AFX afxUnit              AfxGetIoQueuePort(afxIoQueue xque);

////////////////////////////////////////////////////////////////////////////////

AFX afxError AfxAcquireIoBridge(afxIoBridgeConfig const* cfg, afxIoBridge* bridge);

#endif//AFX_IO_BRIDGE_H
