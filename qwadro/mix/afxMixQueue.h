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

AFX_DEFINE_STRUCT(amxSubmission)
{
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxSemaphore        signalSems;
    avxFence            fence;

    afxMixContext mctx;
    afxUnit batchId;
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
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxSemaphore        wait;
    afxSemaphore        signal;
    avxFence            fence;

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
