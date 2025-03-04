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

#ifndef AFX_SYSTEM_H
#define AFX_SYSTEM_H

#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/mem/afxArena.h"
#include "qwadro/inc/base/afxClass.h"
#include "qwadro/inc/exec/afxTime.h"
#include "qwadro/inc/io/afxResource.h"
#include "qwadro/inc/mem/afxPool.h"
// provided classes
#include "qwadro/inc/mem/afxMemory.h"
#include "qwadro/inc/mem/afxMappedString.h"
//#include "qwadro/inc/exec/afxModule.h"

// core
#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/exec/afxService.h"
#include "qwadro/inc/exec/afxTerminal.h"
#include "qwadro/inc/exec/afxThread.h"
#include "qwadro/inc/base/afxVersion.h"
// io
#include "qwadro/inc/io/afxData.h"
#include "qwadro/inc/io/afxIoBridge.h"
#include "qwadro/inc/io/afxSource.h"
#include "qwadro/inc/io/afxStorage.h"
// math
#include "qwadro/inc/math/afxTransform.h"
// mem
#include "qwadro/inc/mem/afxArena.h"
#include "qwadro/inc/mem/afxInterlockedQueue.h"
#include "qwadro/inc/mem/afxQueue.h"
#include "qwadro/inc/mem/afxSlabAllocator.h"
#include "qwadro/inc/mem/afxStack.h"

enum // opcodes used for primitive communication bethween engine and executables.
{
    AFX_OPCODE_CONTINUE,
    AFX_OPCODE_SUSPEND,
    AFX_OPCODE_BREAK, // if system returns BREAK, the engine must shutdown and exit to operating system.
    AFX_OPCODE_REBOOT, // if system returns REBOOT, the engine must shutdown then reinitialize the system.

    AFX_OPCODE_DETACH = 0, // informa a um módulo que ele será desacoplado do sistema, dando-lhe uma chance de liberar recursos.
    AFX_OPCODE_ATTACH, // informa a um módulo que ele será acoplado ao sistema, dando-lhe uma chance de realizar sua devida initialização de recursos e operações.
};

/// All internal memory allocations go through a central allocator.
/// You can get complete control over memory allocations overriding it.
AFX_CALLBACK(void*, afxReallocatorFn)(void*, afxSize align, afxSize siz, afxChar const* __file__, afxUnit __line__);

AFX_DEFINE_STRUCT(afxSystemConfig)
// The system-wide settings and parameters.
{
    afxUnit                 verMajor;
    afxUnit                 verMinor;
    afxUnit                 verPatch;

    afxIoBridgeConfig       mainIoBridge;
    afxSize                 maxMemUsage;
    afxUnit                 memPageSiz;
    afxUnit                 allocGranularity;
    afxUnit                 genrlArenaSpace;

    afxUnit                 ioBufSiz;
    afxUnit                 ioArenaSpace;
    afxUnit                 hwThreadingCap; // max amount of hardware threads managed by Qwadro.
    afxReal                 unitsToMeter; // the number of units in a meter.
    
    afxAssertHook           assertHook; // external assertion handling function (optional)
    afxReallocatorFn        reallocatorFn;

    afxProfilerPushTimerFn  profilerPushTimer; // external (optional) function for tracking performance of the system that is called when a timer starts. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPopTimerFn   profilerPopTimer; // external (optional) function for tracking performance of the system that is called when a timer stops. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPostMarkerFn profilerPostMarker; // external (optional) function for tracking significant events in the system, to act as a marker or bookmark. (only called in Debug and Profile binaries; this is not called in Release)

    afxChar const*          root;

    afxBool                 avxDisabled;
    afxBool                 asxDisabled;
    afxBool                 auxDisabled;

    afxChar const*          appId; // a 32-byte long unique identifier for the application.
    union
    {
#ifdef AFX_OS_WIN
        struct
        {
            /*HINSTANCE*/void* hInst;
            /*HWND*/void* hWnd;
        } w32;
#else
        struct
        {
        } lnx;
#endif
    };
};

// Perform a Qwadro bootstrap.

AFX void                AfxConfigureSystem(afxSystemConfig* cfg);
AFX afxError            AfxBootstrapSystem(afxSystemConfig const *config);

AFX void                AfxDoSystemShutdown(afxInt exitCode);
AFX void                AfxRequestShutdown(afxInt exitCode);

AFX afxBool             AfxGetSystem(afxSystem* system);

AFX afxBool             AfxSystemIsExecuting(void);
//AFX afxTime             AfxDoSystemThreading(afxTime timeout);

AFX afxUnit             AfxGetIoBufferSize(void);

AFX afxUnit             AfxGetMemoryPageSize(void);

/// Returns the ideal number of threads that this process can run in parallel. 
/// This is done by querying the number of logical processors available to this process (if supported by this OS) or the total number of logical processors in the system. 
/// This function returns 1 if neither value could be determined.
AFX afxUnit             AfxGetThreadingCapacity(void);

AFX afxUri const*       AfxGetSystemDirectory(afxUri *dst);
AFX afxString const*    AfxGetSystemDirectoryString(afxString *dst);

AFX afxUri const*       AfxGetPwd(afxUri *dst);
AFX afxString const*    AfxGetPwdString(afxString *dst);

AFX afxUnit32           AfxGetPrimeTid(void);

// Sends event event directly to receiver receiver, using the notify() function. Returns the value that was returned from the event handler.
// Adds the event event, with the object receiver as the receiver of the event, to an event queue and returns immediately.

AFX afxBool             AfxEmitEvent(afxObject receiver, afxEvent* ev);


#endif//AFX_SYSTEM_H
