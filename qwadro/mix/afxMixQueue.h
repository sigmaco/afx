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

#ifndef AMX_MIX_QUEUE_H
#define AMX_MIX_QUEUE_H

#include "qwadro/mix/afxMixDefs.h"

typedef enum amxBusStage
// Bitmask-based value specifying pipeline stages.
{
    amxBusStage_HOST = AFX_BITMASK(0),
    amxBusStage_COMMANDS = AFX_BITMASK(1),
    amxBusStage_COPY = AFX_BITMASK(4),
    amxBusStage_RESOLVE = AFX_BITMASK(5),
    amxBusStage_TRANSFER = (amxBusStage_COPY | amxBusStage_RESOLVE),
    amxBusStage_COMPUTE = AFX_BITMASK(8),
} amxBusStage;

AFX_DEFINE_STRUCT(amxSubmission)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;
    afxFlags            flags;

    // An array of mix contexts that represent the individual mixing commands or operations to be executed. 
    // Each afxMixContext could contain specific information about a set of mixing commands, such as commands 
    // for synthetizing objects, setting up filters, and managing resources.
    afxMixContext       mctx;

    amxFence            wait;
    afxUnit64           waitValue;
    amxBusStage         waitStageMask;
    afxUnit32           waitReserved;
    // A fence which will be signaled when the operation have completed execution.
    amxFence            signal;
    afxUnit64           signalValue;
    amxBusStage         signalStageMask;
    afxUnit32           signalReserved;
};

AFX_DEFINE_STRUCT(amxFlush)
{
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;
    afxSemaphore        wait;
};

AFX_DEFINE_STRUCT(amxTransference)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    // A semaphore upon which to wait on before the operation begin execution.
    amxFence    wait;
    afxUnit64   waitValue;
    amxBusStage waitStageMask;
    afxUnit32   waitReserved;
    // A semaphore which will be signaled when the operation have completed execution.
    // A fence which will be signaled when the operation have completed execution.
    amxFence    signal;
    afxUnit64   signalValue;
    amxBusStage signalStageMask;
    afxUnit32   signalReserved;

    union
    {
        amxAudio    aud;
        amxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               src;
    afxFcc          srcFcc;
    union
    {
        amxAudio    aud;
        amxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               dst;
    afxFcc          dstFcc;

    afxCodec        codec;
    afxUnit         encSiz;
    afxUnit         decSiz;
};

AMX afxMixDevice    AmxGetMixQueueDevice(afxMixQueue mque);
AMX afxMixSystem    AmxGetMixQueueSystem(afxMixQueue mque);

#endif//AMX_MIX_QUEUE_H
