/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // DRAW DEVICE COMMAND, OPERATION AND COMMUNICATION QUUEUE                  //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/*
    In Qwadro, a draw queue is an entity used in graphics programming and rendering to manage and organize the tasks related to drawing or rendering operations. 
    It efficiently handles the sequence and execution of draw commands, particularly in environments where multiple rendering tasks are required.

    A draw queue is a queue of tasks that need to be executed by the graphics system. 
    It organizes these tasks in the order they should be processed.

    The primary purpose of a draw queue is to manage the sequence and execution of drawing operations efficiently. 
    It ensures that rendering commands are executed in a controlled manner, optimizing performance and maintaining consistency in graphical output.

    The commands are processed in the order they are added to the queue. 
    This order is crucial for achieving the desired visual results, especially in complex scenes where the order of rendering affects the final output.

    The draw queue helps in managing synchronization between different rendering tasks, ensuring that operations are performed in the correct sequence and avoiding issues such as race conditions or visual artifacts.

    By batching and organizing draw commands, a draw queue can reduce the overhead associated with frequent state changes or direct hardware interactions.
    It ensures that rendering commands are executed in the correct order, maintaining visual consistency and accuracy.
    And it also helps manage complex scenes and multiple rendering tasks efficiently, which is crucial for high-performance graphics applications.
*/

#ifndef AVX_DRAW_QUEUE_H
#define AVX_DRAW_QUEUE_H

#include "qwadro/inc/draw/op/avxPipeline.h"
#include "qwadro/inc/draw/io/avxRaster.h"
#include "qwadro/inc/draw/io/avxBuffer.h"

AFX_DEFINE_STRUCT(avxSubmission)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxUnit64           waitValues;
    avxPipelineStage    waitStageMasks;
    afxUnit32           waitReserveds;
    afxSemaphore        signalSems;
    afxUnit64           signalValues;
    avxPipelineStage    signalStageMasks;
    afxUnit32           signalReserveds;

    // A fence which will be signaled when the operation have completed execution.
    avxFence            fence;
};

AFX_DEFINE_STRUCT(avxPresentation)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    // The semaphore to wait for before issuing the present request.
    afxSemaphore        wait;
};

AFX_DEFINE_STRUCT(avxTransference)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask             exuMask;
    afxUnit             baseQueIdx;
    afxUnit             queCnt;

    // A semaphore upon which to wait on before the operation begin execution.
    afxSemaphore        wait;
    // A semaphore which will be signaled when the operation have completed execution.
    afxSemaphore        signal;
    // A fence which will be signaled when the operation have completed execution.
    avxFence            fence;

    union
    {
        avxRaster   ras;
        avxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               src;
    afxFcc          srcFcc;
    union
    {
        avxRaster   ras;
        avxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               dst;
    afxFcc          dstFcc;

    afxCodec        codec;
    afxUnit         encSiz;
    afxUnit         decSiz;
};

AVX afxDrawSystem   AvxGetDrawQueueDock(afxDrawQueue dque);
AVX afxUnit         AvxGetDrawQueuePort(afxDrawQueue dque);

AVX afxError        AvxWaitForEmptyDrawQueue(afxDrawQueue dque, afxUnit64 timeout);

/// Open a queue debug label region.
/// Close a queue debug label region.
/// Insert a label into a queue.

AVX void            AfxBeginDrawQueueDebugScope(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx, afxString const* name, avxColor const color);
AVX void            AfxPushDrawQueueDebugLabel(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx, afxString const* name, avxColor const color);
AVX void            AfxEndDrawQueueDebugScope(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx);

#endif//AVX_DRAW_QUEUE_H
