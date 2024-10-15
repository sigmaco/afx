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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// No QWADRO, uma fila é dividida em duas partes, afxDrawInput na vanguarda e afxDrawBridge na retaguarda.
/// Ao criar um afxDrawContext, necessariamente ao menos uma afxDrawBridge foi criada consequentemente e associado ao mesmo.
/// Isto porque, no QWADRO, você não executa as operações de desenho. Ao invés, você submete elas a serem processadas através de um afxDrawInput.
/// Uma afxDrawBridge livre, por sua vez, e que pode realizar determinada operação submetida, então puxará a submissão.

/// Uma afxDrawBridge é um objeto que mapeia afxDrawQueue para uma porta de execução de cargas de trabalho.
/// A afxDrawQueue é uma unidade de gestão de transporte, onde se armazena a carga de trabalho até sua consumação.

/*
    A draw device bridge is a term used in the context of technology and digital communications to refer to a network bridge that includes or supports drawing or graphical device functionality. 
    This type of bridge typically integrates with or connects different devices or systems that are involved in graphical or visual processes.

    A draw device bridge handles the transmission of graphical data between devices and applications. 
    This includes managing data streaming, communication protocols, and synchronization.
*/

/*
    A draw queue is a data structure or system used in graphics programming and rendering to manage and organize the tasks related to drawing or rendering operations. 
    It helps to efficiently handle the sequence and execution of draw commands, particularly in environments where multiple rendering tasks are required.

    A draw queue is a list or queue of rendering commands or tasks that need to be executed by the graphics system. 
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

#ifndef AVX_DRAW_BRIDGE_H
#define AVX_DRAW_BRIDGE_H

#include "qwadro/inc/draw/pipe/avxPipeline.h"
#include "qwadro/inc/draw/io/afxRaster.h"
#include "qwadro/inc/draw/io/afxBuffer.h"

typedef enum afxDrawBridgeFlag
{
    afxDrawBridgeFlag_DRAW      = AFX_BIT(0), // supports draw ops
    afxDrawBridgeFlag_DRAW_AUX  = AFX_BIT(1), // supports auxiliary draw ops (but can't perform a entire draw pipeline execution)
    afxDrawBridgeFlag_COMPUTE   = AFX_BIT(2), // supports compute ops
    afxDrawBridgeFlag_TRANSFER  = AFX_BIT(3), // supports transfer ops
    afxDrawBridgeFlag_VHS       = AFX_BIT(4), // supports VHS enc/dec
} afxDrawBridgeFlags;

AFX_DEFINE_STRUCT(afxDrawPortCaps)
{
    afxDrawBridgeFlags  flags;
    afxNat              minQueCnt; // usually 3
    afxNat              maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
    afxAcceleration     acceleration;

    afxBool8            draw;
    afxBool8            compute;
    afxBool8            transfer;
    afxBool8            present;
    afxBool8            encode;
    afxBool8            decode;
};

AFX_DEFINE_STRUCT(afxDrawBridgeConfig)
{
    afxNat              portId;
    afxNat              minQueCnt;
    afxReal const*      queuePriority;
};

typedef enum
{
    avxQueueOp_WORK = 1,
    avxQueueOp_PRESENT,
    avxQueueOp_TRANSFER,
    avxQueueOp_MAPPING
} avxQueueOpType;

AFX_DEFINE_STRUCT(avxSubmission)
{
    afxNat              portId;
    afxNat              exuIdx;
    afxNat              exuCnt;
    afxNat              queIdx;
    afxNat              queCnt;

    afxFlags            flags;
    afxSemaphore        waitSems;
    afxNat64            waitValues;
    avxPipelineStage    waitStageMasks;
    afxNat32            waitReserveds;
    afxSemaphore        signalSems;
    afxNat64            signalValues;
    avxPipelineStage    signalStageMasks;
    afxNat32            signalReserveds;
    afxFence            fence;
};

AFX_DEFINE_STRUCT(avxPresentation)
{
    afxNat              portId;
    afxNat              exuIdx;
    afxNat              exuCnt;
    afxNat              queIdx;
    afxNat              queCnt;
    afxSemaphore        wait;
};

AFX_DEFINE_STRUCT(avxTransference)
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
    afxNat              encSiz;
    afxNat              decSiz;
    union
    {
        struct
        {
            afxRaster       ras;
            //afxRasterIo     op;
            union
            {
                void*       dst;
                void const* src;
                afxBuffer   buf;
                afxStream   iob;
            };
        }                   img;
        struct
        {
            afxBuffer       buf;
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

AFX_DEFINE_UNION(avxQueueOpData)
{
    struct
    {
        afxFlags            flags;
        afxNat              waitCnt;
        afxSemaphore        waitSems[4];
        afxNat64            waitValues[4];
        avxPipelineStage    waitStageMasks[4];
        afxNat32            waitReserveds[4];
        afxNat              cmdbCnt;
        avxCmdb             cmdbs[4];
        afxNat              signalCnt;
        afxSemaphore        signalSems[4];
        afxNat64            signalValues[4];
        avxPipelineStage    signalStageMasks[4];
        afxNat32            signalReserveds[4];
        afxFence            fence;
    }                       work;
    struct
    {
        afxNat          waitCnt;
        afxSemaphore    waits[4];
        afxNat          bufCnt;
        afxDrawOutput   outputs[4];
        afxNat          buffers[4];
        afxResult       results[4];
    }                   present;
    struct
    {
        union
        {
            struct
            {
                afxRaster       ras;
                afxRasterIo     op;
                union
                {
                    void*       dst;
                    void const* src;
                    afxBuffer   buf;
                    afxStream   iob;
                };
            }                   img;
            struct
            {
                afxBuffer       buf;
                afxBufferIo     op;
                union
                {
                    void*       dst;
                    void const* src;
                    afxStream   iob;
                };
            }                   data;
        };
        afxFcc              srcFcc;
        afxFcc              dstFcc;
        afxCodec            codec;
        afxNat              decompressedSiz;
        afxNat              waitCnt;
        afxSemaphore        waits[4];
        afxNat              signalCnt;
        afxSemaphore        signals[4];
        afxFence            fence;
    } transfer;
    struct
    {
        afxBuffer           buf;
        afxSize             off;
        afxNat              ran;
        afxFlags            flags;
    } map;
};

AFX_DEFINE_STRUCT(avxQueueOp)
{
    afxNat              submType;
    afxError            (*f)(void*, void*, afxNat, void*);
    void*               udd;
    afxNat              dataSiz;
    avxQueueOpData*     data;
};

AVX afxDrawDevice       AfxGetDrawBridgeDevice(afxDrawBridge dexu);
AVX afxDrawContext      AfxGetDrawBridgeContext(afxDrawBridge dexu);

AVX afxNat              AfxGetDrawBridgePort(afxDrawBridge dexu);

AVX void                AfxQueryDrawQueues(afxDrawBridge dexu, afxNat* baseQueIdx, afxNat* queCnt);

AVX afxError            _AvxSubmitDrawCommands(afxDrawBridge dexu, avxSubmission const* ctrl, afxNat cnt, avxCmdb cmdbs[]);
AVX afxError            _AvxSubmitTransferences(afxDrawBridge dexu, avxTransference const* ctrl, afxNat opCnt, void const* ops);
AVX afxError            _AvxSubmitDrawWorkRequest(afxDrawBridge dexu, afxNat cnt, avxQueueOp const subm[]);

AVX void                _AvxBeginDrawQueueDebugScope(afxDrawQueue dque, afxString const* name, afxColor const color);
AVX void                _AvxPushDrawQueueDebugLabel(afxDrawQueue dque, afxString const* name, afxColor const color);
AVX void                _AvxEndDrawQueueDebugScope(afxDrawQueue dque);

AVX afxDrawDevice       AfxGetDrawQueueDevice(afxDrawQueue dque);
AVX afxDrawContext      AfxGetDrawQueueContext(afxDrawQueue dque);
AVX afxNat              AfxGetDrawQueuePort(afxDrawQueue dque);

#endif//AVX_DRAW_BRIDGE_H
