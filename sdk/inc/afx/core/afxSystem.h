/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SYSTEM_H
#define AFX_SYSTEM_H

#include "afx/core/io/afxKeyboard.h"
#include "afx/core/io/afxMouse.h"
#include "afx/core/io/afxStream.h"
#include "afx/core/io/afxUri.h"
#include "afx/core/mem/afxArena.h"
#include "afx/core/time/afxTime.h"
#include "afx/core/afxModule.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/io/afxIoSystem.h"
#include "afx/core/time/afxClock.h"
#include "afx/core/async/afxThread.h"

enum // opcodes used for primitive communication bethween engine and modules.
{
    AFX_OPCODE_CONTINUE,
    AFX_OPCODE_SUSPEND,
    AFX_OPCODE_BREAK, // if system returns BREAK, the engine must shutdown and exit to operating system.
    AFX_OPCODE_REBOOT, // if system returns REBOOT, the engine must shutdown then reinitialize the system.

    AFX_OPCODE_DETACH = 0, // informa a um módulo que ele será desacoplado do sistema, dando-lhe uma chance de liberar recursos.
    AFX_OPCODE_ATTACH, // informa a um módulo que ele será acoplado ao sistema, dando-lhe uma chance de realizar sua devida initialização de recursos e operações.
};

AFX_DEFINE_STRUCT(afxSystemSpecification)
{
    afxChar const                       *root;
    afxNat                              mntCnt;
    afxStoragePointSpecification const  *mntSpecs;
    afxSize                             maxMemUsage;
};

AFX_OBJECT(afxSystem)
{
    afxObject           obj;
#ifdef _AFX_SYSTEM_C
    afxChain            provisions;

    afxUri*             rootDir;

    afxClass            memClass;
    afxClass            thrClass;
    afxClass            fsysClass;
    afxClass            mdleClass;
    afxClass            hidClass;
    afxClass            kbdClass;
    afxClass            mseClass;
    afxClass            dsysClass;
    afxClass            appClass; // can use everything

    afxMemory           genrlMem;
    afxNat              memPageSize; // The page size and the granularity of page protection and commitment.
    afxNat              hwConcurrencyCap; // # of logical proc units (hardware threads)
    afxProcessor        procUnits[2];
    afxNat              procUnitCnt;
    afxThread           deusExMachina;
    afxModule           e2coree;
    afxKeyboard         stdKbd;

    afxBool             isInBootUp;
    afxBool             isInShutdown;
    afxBool             operating;
    afxBool             interruptionRequested;
#endif
};

AFX afxSystem           AfxGetSystem(void);
AFX void                AfxShutdownSystem(void);
AFX afxSystem           AfxBootUpSystem(afxSystemSpecification const *spec);
AFX afxResult           AfxDoSystemThreading(afxTime timeout);

AFX afxMemory           AfxAcquireMemory(afxAllocationStrategy const *strategy, afxHint const hint);
AFX afxApplication      AfxAcquireApplication(afxApplicationSpecification const *spec);
AFX afxHid              AfxAcquireHid(afxNat port);
AFX afxKeyboard         AfxAcquireKeyboard(afxNat port);
AFX afxModule           AfxAcquireModule(afxUri const *uri);
AFX afxMouse            AfxAcquireMouse(afxNat port);

AFX afxHid              AfxFindHid(afxNat port);
AFX afxKeyboard         AfxFindKeyboard(afxNat port);
AFX afxModule           AfxFindModule(afxUri const *uri);
AFX afxMouse            AfxFindMouse(afxNat port);

AFX afxClass*           AfxGetMemoryClass(void);
AFX afxClass*           AfxGetApplicationClass(void);
AFX afxClass*           AfxGetDrawSystemClass(void);
AFX afxClass*           AfxGetIoSystemClass(void);
AFX afxClass*           AfxGetHidClass(void);
AFX afxClass*           AfxGetKeyboardClass(void);
AFX afxClass*           AfxGetModuleClass(void);
AFX afxClass*           AfxGetMouseClass(void);
AFX afxClass*           AfxGetThreadClass(void);

AFX afxNat              AfxGetMemoryPageSize(void);

/// Returns the ideal number of threads that this process can run in parallel. 
/// This is done by querying the number of logical processors available to this process (if supported by this OS) or the total number of logical processors in the system. 
/// This function returns 1 if neither value could be determined.
AFX afxNat              AfxGetSystemConcurrencyCapacity(void);

AFX afxMemory           AfxGetMainMemory(void);
AFX afxUri const*       AfxGetSystemRootPath(afxUri *copy);
AFX afxString const*    AfxGetSystemRootPathString(afxString *copy);

AFX afxNat              AfxEnumerateMemories(afxNat first, afxNat cnt, afxMemory mem[]);
AFX afxNat              AfxEnumerateApplications(afxNat first, afxNat cnt, afxApplication app[]);
AFX afxNat              AfxEnumerateDrawSystems(afxNat first, afxNat cnt, afxDrawSystem dsys[]);
AFX afxNat              AfxEnumerateIoSystems(afxNat first, afxNat cnt, afxIoSystem fsys[]);
AFX afxNat              AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hid[]);
AFX afxNat              AfxEnumerateKeyboards(afxNat first, afxNat cnt, afxKeyboard kbd[]);
AFX afxNat              AfxEnumerateModules(afxNat first, afxNat cnt, afxModule mdle[]);
AFX afxNat              AfxEnumerateMouses(afxNat first, afxNat cnt, afxMouse mse[]);
AFX afxNat              AfxEnumerateThreads(afxNat first, afxNat cnt, afxThread thr[]);

AFX afxNat              AfxCurateThreads(afxNat first, afxNat cnt, afxBool(*f)(afxThread, void*), void *udd);

// Sends event event directly to receiver receiver, using the notify() function. Returns the value that was returned from the event handler.
AFX afxBool             AfxEmitEvent(afxObject *receiver, afxEvent *ev);
AFX afxBool             AfxEmitEvents(afxNat cnt, afxObject *receiver[], afxEvent ev[]);
AFX afxBool             AfxReemitEvent(afxNat cnt, afxObject *receiver[], afxEvent *ev);


AFX afxError            AfxPostEvent(afxObject *receiver, afxEvent *ev);
AFX afxError            AfxPostEvents(afxNat cnt, afxObject *receiver[], afxEvent ev[]);

AFX afxNat mainThreadId;

#endif//AFX_SYSTEM_H