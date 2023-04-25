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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_SYSTEM_H
#define AFX_SYSTEM_H

#include "afx/core/io/afxKeyboard.h"
#include "afx/core/io/afxMouse.h"
#include "afx/core/io/afxStream.h"
#include "afx/core/io/afxUri.h"
#include "afx/core/mem/afxAllocator.h"
#include "afx/core/time/afxTime.h"
#include "afxModule.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/io/afxFileSystem.h"

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
    afxMountPointSpecification const    *mntSpecs;
    afxSize                             maxMemUsage;
};

#ifndef AFX_SYSTEM_C

AFX_OBJECT(afxSystem) { afxObject obj; };

#endif

AFX afxAllocator        AfxSystemAcquireAllocator(afxSystem sys, afxAllocationStrategy const *strategy, afxHint const hint);
AFX afxApplication      AfxSystemAcquireApplication(afxSystem sys, afxApplicationSpecification const *spec);
AFX afxDrawSystem       AfxSystemAcquireDrawSystem(afxSystem sys);
AFX afxFileSystem       AfxSystemAcquireFileSystem(afxSystem sys, afxNat mnptCnt, afxMountPointSpecification const spec[]);
AFX afxHid              AfxSystemAcquireHid(afxSystem sys, afxNat port);
AFX afxKeyboard         AfxSystemAcquireKeyboard(afxSystem sys, afxNat port);
AFX afxModule           AfxSystemAcquireModule(afxSystem sys, afxUri const *uri);
AFX afxMouse            AfxSystemAcquireMouse(afxSystem sys, afxNat port);
AFX afxThread           AfxSystemAcquireThread(afxSystem sys, void (*start)(afxThread thr, void *udd), void *udd, void (*exec)(afxThread thr), afxHint const hint);

AFX afxHid              AfxSystemFindHid(afxSystem sys, afxNat port);
AFX afxKeyboard         AfxSystemFindKeyboard(afxSystem sys, afxNat port);
AFX afxModule           AfxSystemFindModule(afxSystem sys, afxUri const *uri);
AFX afxMouse            AfxSystemFindMouse(afxSystem sys, afxNat port);

AFX afxClass*           AfxSystemGetAllocatorClass(afxSystem sys);
AFX afxClass*           AfxSystemGetApplicationClass(afxSystem sys);
AFX afxClass*           AfxSystemGetDrawSystemClass(afxSystem sys);
AFX afxClass*           AfxSystemGetFileSystemClass(afxSystem sys);
AFX afxClass*           AfxSystemGetHidClass(afxSystem sys);
AFX afxClass*           AfxSystemGetKeyboardClass(afxSystem sys);
AFX afxClass*           AfxSystemGetModuleClass(afxSystem sys);
AFX afxClass*           AfxSystemGetMouseClass(afxSystem sys);
AFX afxClass*           AfxSystemGetThreadClass(afxSystem sys);

AFX afxNat              AfxSystemGetMemPageSize(afxSystem sys);
AFX afxNat              AfxSystemGetProcessorCount(afxSystem sys);
AFX afxAllocator        AfxSystemGetAllocator(afxSystem sys);
AFX afxFileSystem       AfxSystemGetFileSystem(afxSystem sys);
AFX afxUri const*       AfxSystemGetRootUri(afxSystem sys, afxUri *copy);
AFX afxString const*    AfxSystemGetRootUriString(afxSystem sys, afxString *copy);

AFX afxResult           AfxSystemEnumerateAllocators(afxSystem sys, afxNat base, afxNat cnt, afxAllocator all[]);
AFX afxResult           AfxSystemEnumerateApplications(afxSystem sys, afxNat base, afxNat cnt, afxApplication app[]);
AFX afxResult           AfxSystemEnumerateDrawSystems(afxSystem sys, afxNat base, afxNat cnt, afxDrawSystem dsys[]);
AFX afxResult           AfxSystemEnumerateFileSystems(afxSystem sys, afxNat base, afxNat cnt, afxFileSystem fsys[]);
AFX afxResult           AfxSystemEnumerateHids(afxSystem sys, afxNat base, afxNat cnt, afxHid hid[]);
AFX afxResult           AfxSystemEnumerateKeyboards(afxSystem sys, afxNat base, afxNat cnt, afxKeyboard kbd[]);
AFX afxResult           AfxSystemEnumerateModules(afxSystem sys, afxNat base, afxNat cnt, afxModule mdle[]);
AFX afxResult           AfxSystemEnumerateMouses(afxSystem sys, afxNat base, afxNat cnt, afxMouse mse[]);
AFX afxResult           AfxSystemEnumerateThreads(afxSystem sys, afxNat base, afxNat cnt, afxThread thr[]);

AFX afxBool             AfxSystemNotify(afxSystem sys, afxObject *receiver, afxEvent *ev);
AFX afxBool             AfxSystemEmitEvent(afxSystem sys, afxObject *receiver, afxEvent *ev);
AFX afxBool             AfxSystemEmitEvents(afxSystem sys, afxNat cnt, afxObject *receiver[], afxEvent ev[]);
AFX afxBool             AfxSystemReemitEvent(afxSystem sys, afxNat cnt, afxObject *receiver[], afxEvent *ev);
AFX afxError            AfxSystemPostEvent(afxSystem sys, afxObject *receiver, afxEvent *ev);
AFX afxError            AfxSystemPostEvents(afxSystem sys, afxNat cnt, afxObject *receiver[], afxEvent ev[]);

AFX afxSystem           AfxSystemGet(void);
AFX afxSystem           AfxSystemBootUp(afxSystemSpecification const *spec);
AFX afxError            AfxSystemProcess(afxSystem sys);

AFX afxNat mainThreadId;

#endif//AFX_SYSTEM_H