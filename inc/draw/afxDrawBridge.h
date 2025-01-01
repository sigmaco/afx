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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/// No QWADRO, uma fila é dividida em duas partes, afxDrawInput na vanguarda e afxDrawBridge na retaguarda.
/// Ao criar um afxDrawSystem, necessariamente ao menos uma afxDrawBridge foi criada consequentemente e associado ao mesmo.
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

typedef enum afxDrawPortFlag
{
    afxDrawPortFlag_DRAW      = AFX_BIT(0), // supports draw ops
    afxDrawPortFlag_COMPUTE   = AFX_BIT(1), // supports compute ops
    afxDrawPortFlag_TRANSFER  = AFX_BIT(2), // supports transfer ops
    afxDrawPortFlag_DRAW_AUX  = AFX_BIT(3), // supports auxiliary draw ops (but can't perform a entire draw pipeline execution)
    afxDrawPortFlag_VHS       = AFX_BIT(4), // supports VHS enc/dec
} afxDrawPortFlags;

AFX_DEFINE_STRUCT(afxDrawPortCaps)
{
    afxDrawPortFlags    capabilities;
    afxAcceleration     acceleration;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
};

AFX_DEFINE_STRUCT(afxDrawBridgeConfig)
{
    afxUnit             ddevId;
    afxDrawPortFlags    capabilities; // specifies capabilities of queues in a port.
    afxAcceleration     acceleration;
    afxUnit             minQueCnt;
    afxReal const*      queuePriority;
};

AFX_DEFINE_STRUCT(avxSubmission)
{
    afxUnit             exuIdx;
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
    afxFence            fence;
};

AFX_DEFINE_STRUCT(avxPresentation)
{
    afxUnit         exuIdx;
    afxUnit         baseQueIdx;
    afxUnit         queCnt;
    afxSemaphore    wait;
};

AFX_DEFINE_STRUCT(avxTransference)
{
    afxUnit         exuIdx;
    afxUnit         baseQueIdx;
    afxUnit         queCnt;

    afxSemaphore    wait;
    afxSemaphore    signal;
    afxFence        fence;

    union
    {
        afxRaster   ras;
        afxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               src;
    afxFcc          srcFcc;
    union
    {
        afxRaster   ras;
        afxBuffer   buf;
        void*       dst;
        void const* src;
        afxStream   iob;
    }               dst;
    afxFcc          dstFcc;

    afxCodec        codec;
    afxUnit         encSiz;
    afxUnit         decSiz;
};

AVX afxDrawDevice   AfxGetDrawBridgeDevice(afxDrawBridge dexu);
AVX afxDrawSystem   AfxGetDrawBridgeContext(afxDrawBridge dexu);

AVX afxUnit         AfxQueryDrawBridgePort(afxDrawBridge dexu, afxDrawDevice* device);

AVX afxUnit         AfxGetDrawQueues(afxDrawBridge dexu, afxUnit baseQueIdx, afxUnit cnt, afxDrawQueue queues[]);

AVX afxDrawSystem   AfxGetDrawQueueContext(afxDrawQueue dque);
AVX afxUnit         AfxGetDrawQueuePort(afxDrawQueue dque);


/// Open a queue debug label region.
/// Close a queue debug label region.
/// Insert a label into a queue.

AVX void            AfxBeginDrawQueueDebugScope(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx, afxString const* name, afxColor const color);
AVX void            AfxPushDrawQueueDebugLabel(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx, afxString const* name, afxColor const color);
AVX void            AfxEndDrawQueueDebugScope(afxDrawSystem dsys, afxUnit exuIdx, afxUnit queIdx);

#endif//AVX_DRAW_BRIDGE_H
