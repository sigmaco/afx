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

#include "qwadro/inc/draw/avxPipeline.h"
#include "qwadro/inc/draw/avxRaster.h"
#include "qwadro/inc/draw/avxBuffer.h"

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

    // An array of draw contexts that represent the individual drawing commands or operations to be executed. 
    // Each afxDrawContext could contain specific information about a set of drawing commands, such as commands 
    // for rendering objects, setting up shaders, and managing resources.
    afxDrawContext      dctx;
    afxUnit             batchId;

    // A fence which will be signaled when the operation have completed execution.
    avxFence            fence;
};

AFX_DEFINE_STRUCT(avxPresentation)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask         exuMask;
    afxUnit         baseQueIdx;
    afxUnit         queCnt;

    // The frame identifier.
    afxUnit64       frameId;
    // A application-defined flag combo for the frame.
    afxFlags        frameFlags;
    // A application-defined bitmask for the frame.
    afxMask         frameMask;
    // A non-zero value specifying the present id to be associated with the presentation of the swapchain.
    afxUnit64       presentId;

    // A fence that the function will wait on before proceeding with the presentation. 
    // Fences are used to ensure synchronization, especially when working with GPUs. 
    // The function will wait for this fence to be signaled before beginning the presentation. 
    // This ensures that drawing commands are completed before the output is presented.
    avxFence        waitOnDpu;
    // The semaphore to wait for before issuing the present request.
    afxSemaphore    wait;

    // An handle to a afxDrawOuput object to be presented.
    afxSurface   dout;

    // An index to a swapchain buffer specifying which buffers to present. 
    // This indicate which buffer is to be shown.
    afxUnit         bufIdx;

    // The number of hint regions.
    afxUnit         hintCnt;
    // Four regions that has changed since the last present to the swapchain.
    afxRect         hintRcs[4];

    // An array of fences to signal after the presentation is completed. 
    // Once the presentation is done, these fences are signaled, allowing the system to continue processing other tasks 
    // that depend on the completion of the presentation. This is useful for synchronization with other parts of the 
    // rendering pipeline or application logic.
    avxFence        signal;
};

AFX_DEFINE_STRUCT(avxTransference)
{
    // A bitmask specifying which bridges can assume this operation.
    // If NIL, any bridge is allowed to assume this operation.
    afxMask         exuMask;
    afxUnit         baseQueIdx;
    afxUnit         queCnt;

    // A semaphore upon which to wait on before the operation begin execution.
    afxSemaphore    wait;
    // A semaphore which will be signaled when the operation have completed execution.
    afxSemaphore    signal;
    // A fence which will be signaled when the operation have completed execution.
    avxFence        fence;

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

AVX afxError        AvxWaitForEmptyDrawQueue(afxDrawQueue dque, afxUnit64 timeout);

/// Open a queue debug label region.
/// Close a queue debug label region.
/// Insert a label into a queue.

AVX void            AvxBeginDrawQueueDebugScope(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx, afxString const* name, avxColor const color);
AVX void            AvxPushDrawQueueDebugLabel(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx, afxString const* name, avxColor const color);
AVX void            AvxEndDrawQueueDebugScope(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx);

////////////////////////////////////////////////////////////////////////////////

#endif//AVX_DRAW_QUEUE_H
