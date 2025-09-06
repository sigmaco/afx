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

// This software is part of Advanced Video Graphics Extensions & Experiments.

// QWADRO is an open-source coalition of libraries that supports rapid development of software that deals with 4D data.
// The QWADRO frontend exposes a set of carefully selected data structures and algorithms in C.
// The backend is highly optimized and is set up for parallelization.
// We welcome contributions from the open-source community.

// AVX is designed to be a Vulkan-flavored OpenGL. It is not a graphics engine; it will not engage in optimizations.

#ifndef AVX_DRAW_SYSTEM_H
#define AVX_DRAW_SYSTEM_H

#include "qwadro/draw/afxDrawDevice.h"
#include "qwadro/draw/afxDrawContext.h"
#include "qwadro/draw/avxBuffer.h"
#include "qwadro/draw/avxBuffered.h"
#include "qwadro/draw/avxRaster.h"
#include "qwadro/draw/avxRasterFile.h"
#include "qwadro/draw/avxCanvas.h"
#include "qwadro/draw/avxLigature.h"
#include "qwadro/draw/avxPipeline.h"
#include "qwadro/draw/avxQueryPool.h"
#include "qwadro/draw/avxPipeline.h"
#include "qwadro/draw/avxSampler.h"
#include "qwadro/draw/avxShader.h"
#include "qwadro/draw/avxVertexInput.h"
#include "qwadro/draw/avxDrawing.h"
#include "qwadro/draw/avxFence.h"
#include "qwadro/draw/avxMatrix.h"
#include "qwadro/draw/avxTxd.h"
#include "qwadro/draw/avxTransformationEXT.h"
#include "qwadro/draw/avxRasterizationEXT.h"

#define AVX_MAX_BRIDGES_PER_SYSTEM (32)

typedef enum avxEventId
{
    avxEventId_FENCE,
    avxEventId_EXECUTE,
    avxEventId_PRESENT,
    avxEventId_UPLOAD,
    avxEventId_DOWNLOAD,
    avxEventId_PREFETCH,
    avxEventId_REFRESH,
    avxEventId_RECONNECT,
    avxEventId_EXTENT,
} avxEventId;

AFX_DEFINE_STRUCT(avxEvent)
{
    avxEventId  id;
    afxBool     posted, accepted;
    void*       udd[1];
};

AFX_DEFINE_STRUCT(afxDrawSystemConfig)
// The system-wide settings and parameters prefered/required for acquisition.
{
    afxUnit             verMajor;
    afxUnit             verMinor;
    // The functions to be capable on bridged devices.
    afxDrawFn           caps;
    // The acceleration to be available on bridged devices.
    afxAcceleration     accel;
    // The features to be enabled.
    afxDrawFeatures     features;
    // The number of system extensions to be enabled.
    afxUnit             extCnt;
    // An array of Qwadro strings containing the names of extensions to enable for the desired system.
    afxString const*    exts;
    // The depth mapping used by projection to map depth values into the clip space. 
    avxClipSpaceDepth   clipSpcDepth;
    // The number of bridged devices' execution ports.
    afxUnit             exuCnt;
    // An array of configurations for each bridged device.
    afxDrawBridgeConfig exus[AVX_MAX_BRIDGES_PER_SYSTEM];
    // User-defined data attached to the system.
    void*               udd;
    // Debugging string attached to the system.
    afxString           tag;
};

AVX afxError AvxConfigureDrawSystem
(
    // The Id of the installable client driver.
    afxUnit icd,
    // A pointer to a system configuration structure.
    afxDrawSystemConfig* cfg
);

/*
    The AvxEstablishDrawSystem() function establishes a new drawing system based on a specific driver and configuration.
    It provides the ability to establish a system for rendering and drawing operations, allowing for further interaction
    with the graphics pipeline (such as rendering, resource management, and more). This function is often used during the
    initialization phase of an application to prepare the system for graphical tasks.
*/

AVX afxError AvxEstablishDrawSystem
(
    // The installable client driver (ICD) identifier. 
    // This is an integer that uniquely identifies the driver
    afxUnit icd,

    // A configuration structure that holds the parameters required to establish and configure the drawing system.
    afxDrawSystemConfig const* cfg,

    // A pointer to an afxDrawSystem where the created drawing system will be stored. 
    // The function will populate this pointer with the reference to the newly established drawing system.
    afxDrawSystem* system
);

/*
    The AvxEnumerateDrawSystems() function enumerates drawing systems established by a given installable client driver (ICD).
    By specifying a starting index and a count of systems to retrieve, you can query and retrieve information about the
    established drawing systems. This functionality is useful when your application needs to detect and interact with
    multiple systems, providing a way to select the best system for rendering.
*/

AVX afxUnit AvxEnumerateDrawSystems
(
    // The installable client driver (ICD) module identifier.
    afxUnit icd,

    // The starting index for the enumeration. 
    // If you want to start enumerating from the first available drawing system, you would set this value to 0. 
    // If you wish to start from a later point, you can provide an index specifying where to begin.
    afxUnit first,

    // The number of draw system to retrieve.
    afxUnit cnt,

    // An array where the enumerated drawing systems will be stored. 
    // After the function call, the array will contain the drawing systems up to the requested count @cnt.
    afxDrawSystem systems[]
);

/*
    The AvxInvokeDrawSystems() function performs custom actions on a set of drawing systems established by a given ICD.
    By specifying a callback function, you can iterate over multiple drawing systems and apply specific logic to each system.
    This is useful when you need to perform system-specific operations, such as querying, configuring, or logging properties
    for each drawing system. The function provides a robust mechanism for handling multiple systems in a streamlined way.
*/

AVX afxUnit AvxInvokeDrawSystems
(
    // The installable client driver (ICD) module identifier.
    afxUnit icd,

    // The starting index for the enumeration of drawing systems. 
    // It specifies which system to start processing from.
    afxUnit first,

    // The user-defined data that will be passed to the callback function.
    void *udd,

    // A callback function that will be invoked for each drawing system being enumerated.
    afxBool(*f)(void*, afxDrawSystem),

    // The number of drawing systems to process starting from the first index. 
    // The function will invoke the callback for each of these drawing systems.
    afxUnit cnt
);

/*
    The AvxEvokeDrawSystems() function retrieves and process drawing systems in one go.
    It not only retrieves the systems but also invokes a callback function on each one to determine when push it to the @systems array,
    enabling you to perform custom logic while working with the retrieved systems.
    This function is useful when you need to both enumerate drawing systems and apply specific actions or checks to each system.
*/

AVX afxUnit AvxEvokeDrawSystems
(
    // The installable client driver (ICD) module identifier.
    afxUnit icd,

    // The index of the first drawing system to retrieve. 
    // This allows you to start processing from a specific drawing system rather than always starting from the first.
    afxUnit first,

    // The user-defined data that will be passed to the callback function.
    void* udd,

    // The callback function that will be invoked for each drawing system.
    afxBool(*f)(void*, afxDrawSystem),

    // The number of drawing systems to process starting from the first index. 
    // This specifies how many systems the function should attempt to retrieve.
    afxUnit cnt,

    // An array where the retrieved drawing systems will be stored. 
    // The function will fill this array with the actual drawing systems that were retrieved, up to the number @cnt.
    afxDrawSystem systems[]
);

////////////////////////////////////////////////////////////////////////////////

/*
    The AvxGetDrawSystemIcd() function retrieves the ICD (Installable Client Driver) running a specific drawing system. 
    This allows the application to determine which implementation is being used for the drawing system and to perform 
    further actions with the corresponding driver or module.
*/

AVX afxModule       AvxGetDrawSystemIcd
(
    // The drawing system for which the ICD is being queried.
    afxDrawSystem   dsys
);

/*
    The AvxResolveDrawSymbols() function retrieves the address of a device-specific function that allows you to 
    dynamically load Vulkan function pointers for a device object at runtime. It is part of Qwadro's extensible and 
    dynamic approach, enabling developers to load Qwadro functions based on their needs, instead of hardcoding everything.

    Returns the number of procedures found and retrieved.
*/

AVX afxUnit         AvxResolveDrawSymbols
(
    // The established drawing system.
    afxDrawSystem   dsys, 

    // The number of requested procedures.
    afxUnit         cnt, 

    // An array of strings representing procedure names or identifiers. 
    // The function looks up procedures based on these names.
    afxString const names[], 

    // An array of function pointers to store the procedure addresses.
    void*           addresses[]
);

/*
    The AvxGetDrawBridges() function retrieves drawing bridges for a established drawing system. 
    Drawing bridges are components that link and provide communication between an established system and its working devices. 
    This function allows applications to query and retrieve information about the established bridges, 
    which can be useful when dealing with systems that involve multiple hardware/software interfaces.

    Returns the number of arranged bridges. If @bridges is NIL, it returns the total number of bridges from the base index.
*/

AVX afxUnit         AvxGetDrawBridges
(
    // The established drawing system.
    afxDrawSystem   dsys, 

    // The base index to begin the enumeration of the bridges.
    afxUnit         baseIdx, 

    // The number of bridges to be retrieved.
    afxUnit         cnt, 

    // An array where the function will store the retrieved drawing bridges.
    afxDrawBridge   bridges[]
);

/*
    The AvxChooseDrawBridges() function provides a way to select specific drawing bridges in an established drawing system, 
    filtered by device ID. The function returns the selected bridges in an array and allows the application 
    to filter the available bridges based on the provided indices. This is useful for applications that need to work 
    with multiple bridges or interfaces between components in a drawing system, such as managing communication between 
    the CPU and GPU or between different parts of the graphics pipeline.

    Returns the number of arranged draw bridges.
*/

AVX afxUnit         AvxChooseDrawBridges
(
    // The established drawing system.
    afxDrawSystem   dsys, 

    // An optional device ID for which the bridges must be linked against.
    afxUnit         ddevId, 

    // An optional bitmask describing the drawing device's port capabilities for which bridges must be linked against. 
    afxDrawFn     caps,

    afxMask         exuMask,

    // The first index of the bridges to begin selection from.
    afxUnit         first, 
    
    // The maximum number of bridges to be selected and returned.
    afxUnit         maxCnt, 

    // An array that will hold the selected drawing bridges.
    afxDrawBridge   bridges[]
);

/*
    The AvxWaitForDrawSystem() function waits for a drawing system to become ready, ensuring synchronization between the 
    application and the graphics context. It is particularly useful for managing asynchronous tasks or ensuring the system 
    is in a stable state before proceeding with further operations. It provides a way to wait for completion or readiness 
    while managing timeouts for better control over the execution flow.

    If the system is not ready within the given @timeout, the function may return an error or a timeout code.
*/

AVX afxError        AvxWaitForDrawSystem
(
    // The drawing system that you want to wait for.
    afxDrawSystem   dsys, 
    
    // The timeout value that defines how long the function should wait before returning. 
    // It is expressed in microseconds and defines how long to wait for the drawing system 
    // to be ready or for the drawing operation to complete.
    afxUnit64       timeout
);

/*
    The AvxWaitForDrawBridges() function waits for a specific bridge in a drawing system to become ready or finish its operation. 
    It is useful for synchronizing tasks in graphics pipelines or handling communication between multiple devices. 
    By providing a timeout, it ensures that the function does not block indefinitely and allows you to proceed with other 
    operations if the bridge does not become ready in time. This function is useful in contexts where bridges or execution 
    units handle asynchronous tasks, requiring careful synchronization to ensure that operations proceed in the correct order.

    If the bridge does not reach the ready state within the timeout, the function might return an error.
*/

AVX afxError        AvxWaitForDrawBridges
(
    // The drawing system to which the specific bridge belongs.
    afxDrawSystem   dsys, 

    // The timeout period that the function should wait for the bridge to become ready or to complete its operation. 
    // The time is expressed in microseconds, and the function will stop waiting once this period has elapsed.
    afxUnit64       timeout,

    // The index of the execution unit (bridge) that needs to be waited on. 
    // If the drawing system has multiple bridges or execution units (e.g., for communication between different components or devices), 
    // this index identifies the particular bridge you're concerned with.
    afxMask         exuMask
);

/*
    The AvxWaitForDrawQueue() function waits for a specific queue in a drawing system to become ready or finish its tasks. 
    It is useful for managing synchronization in systems that utilize multiple command queues, ensuring that one queue's 
    operations are complete before continuing with the next phase of processing. The timeout parameter provides control 
    over how long to wait, preventing the system from hanging indefinitely.

    If the queue does not become ready within this time frame, the function will return an error.
*/

AVX afxError        AvxWaitForDrawQueue
(
    // The drawing system that contains the queue.
    afxDrawSystem   dsys,

    // The timeout period defines how long the function will wait for the queue to become ready. 
    // The time is specified in microseconds.
    afxUnit64       timeout,

    // The execution unit index, which likely refers to a specific queue or processing unit within the drawing system.
    // This helps to identify which execution unit's queue you are waiting for, especially if there are multiple queues in the system.
    afxUnit         exuIdx,

    // The queue you want to wait for. 
    // Drawing systems often have multiple command queues for various tasks like graphics, compute, or transfer operations. 
    // The queId specifies which queue's state should be checked.
    afxUnit         queId
);

AVX void AvxGetEnabledDrawFeatures
(
    afxDrawSystem dsys, 
    afxDrawFeatures* features
);

AVX afxUnit AvxTestForDrawExtensionsEnabled
(
    afxDrawSystem dsys, 
    afxUnit cnt, 
    afxString const* exts, 
    afxBool enabled[]
);

#endif//AVX_DRAW_SYSTEM_H
