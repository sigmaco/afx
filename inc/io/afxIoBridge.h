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

#include "qwadro/inc/io/afxFile.h"

typedef enum afxIoBridgeFlag
{
    afxIoBridgeFlag_      = AFX_BIT(0),
} afxIoBridgeFlags;

AFX_DEFINE_STRUCT(afxIoPortCaps)
{
    afxIoBridgeFlags    flags;
    afxNat              minQueCnt; // usually 3
    afxNat              maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxAcceleration     acceleration;

    afxBool8            transfer;
    afxBool8            encode;
    afxBool8            decode;
};

AFX_DEFINE_STRUCT(afxIoBridgeConfig)
{
    afxNat              portId;
    afxNat              minQueCnt;
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
    afxNat              portId;
    afxNat              exuIdx;
    afxNat              exuCnt;
    afxNat              queIdx;
    afxNat              queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxNat64            waitValues;
    afxNat32            waitReserveds;
    afxSemaphore        signalSems;
    afxNat64            signalValues;
    afxNat32            signalReserveds;
    afxFence            fence;
};

AFX_DEFINE_STRUCT(afxTransference)
{
    afxNat              portIdx;
    afxNat              baseQueIdx;
    afxNat              queCnt;

    afxSemaphore        wait;
    afxSemaphore        signal;
    afxFence            fence;

    afxFcc              srcFcc;
    afxFcc              dstFcc;
    afxCodec            codec;
    afxNat              decSiz;

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

AFX afxNat              AfxGetIoBridgePort(afxIoBridge xexu);

AFX afxNat              AfxGetIoQueues(afxIoBridge xexu, afxNat first, afxNat cnt, afxIoQueue queues[]);

AFX afxError            _AfxSubmitIoCommands(afxIoBridge xexu, afxSubmission const* ctrl, afxNat cnt, afxObject cmdbs[]);
AFX afxError            _AfxSubmitTransferences(afxIoBridge xexu, afxTransference const* ctrl, afxNat opCnt, void const* ops);

AFX afxNat              AfxGetIoQueuePort(afxIoQueue xque);

////////////////////////////////////////////////////////////////////////////////

AFX afxError AfxAcquireIoBridge(afxIoBridgeConfig const* cfg, afxIoBridge* bridge);

#endif//AFX_IO_BRIDGE_H
