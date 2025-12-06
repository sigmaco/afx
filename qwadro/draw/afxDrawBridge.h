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
 // DRAW DEVICE COMMAND, OPERATION AND COMMUNICATION BRIDGE                  //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#ifndef AVX_DRAW_BRIDGE_H
#define AVX_DRAW_BRIDGE_H

/*
    In Qwadro, a draw bridge is a context of communications to refer to a network bridge that includes or supports drawing or graphical device functionality. 
    This type of bridge typically integrates with or connects different devices or systems that are involved in graphical or visual processes.

    In Qwadro, a draw bridge is context of transmission of graphical data between draw devices and a draw system.
    This includes managing data streaming, communication protocols, and synchronization.
*/

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

#include "qwadro/draw/afxDrawQueue.h"

#define AVX_MAX_QUEUES_PER_BRIDGE (32)

typedef enum avxAptitude
// Flags specifying the facilities on a draw device for performing a specified task.
{
    // Supports transfer ops via direct memory access.
    avxAptitude_DMA      = AFX_BITMASK(0),
    // Supports graphics ops, including transformation, tesselation, rasterization and resampling.
    avxAptitude_GFX      = AFX_BITMASK(1),
    // Support deferred presentation.
    avxAptitude_PRESENT  = AFX_BITMASK(2),
    // Supports programmable general purpose parallel computation.
    avxAptitude_PCX     = AFX_BITMASK(3),
    // Supports fixed-function YUV video encoding/decoding.
    avxAptitude_VCX     = AFX_BITMASK(4),
    // Supports raytracing ops
    avxAptitude_RTX      = AFX_BITMASK(5),
} avxAptitude;

AFX_DEFINE_STRUCT(avxPortInfo)
{
    avxAptitude         capabilities;
    afxAcceleration     acceleration;
    afxUnit             minQueCnt; // usually 3
    afxUnit             maxQueCnt; // the count of queues in this port. Each port must support at least one queue.
};

AFX_DEFINE_STRUCT(avxBridgeConfig)
{
    afxUnit             ddevId;
    afxAcceleration     acceleration;
    avxAptitude         capabilities; // specifies capabilities of queues in a port.
    afxUnit             minQueCnt;
    afxReal const*      queuePriority;
};

AVX afxDrawSystem AvxGetBridgedDrawSystem
(
    afxDrawBridge dexu, 
    afxUnit* bridgeId
);

AVX afxDrawDevice AvxGetBridgedDrawDevice
(
    afxDrawBridge dexu, 
    afxUnit* ddevId
);

AVX afxUnit AvxGetDrawQueues
(
    afxDrawBridge dexu, 
    afxUnit baseQueIdx, 
    afxUnit cnt, 
    afxDrawQueue queues[]
);

AVX afxError AvxWaitForIdleDrawBridge
(
    afxDrawBridge dexu, 
    afxUnit64 timeout
);

#endif//AVX_DRAW_BRIDGE_H
