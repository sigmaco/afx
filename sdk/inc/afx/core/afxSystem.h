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
#include "afx/core/io/afxArchive.h"
#include "afx/core/io/afxResource.h"
#include "afx/core/io/afxStream.h"
#include "afx/core/io/afxUri.h"
#include "afx/core/mem/afxAllocator.h"
#include "afx/core/time/afxTime.h"
#include "afxModule.h"
#include "afx/draw/afxDrawSystem.h"

enum // opcodes used for primitive communication bethween engine and modules.
{
    AFX_OPCODE_CONTINUE,
    AFX_OPCODE_SUSPEND,
    AFX_OPCODE_BREAK, // if system returns BREAK, the engine must shutdown and exit to operating system.
    AFX_OPCODE_REBOOT, // if system returns REBOOT, the engine must shutdown then reinitialize the system.

    AFX_OPCODE_DETACH = 0, // informa a um módulo que ele será desacoplado do sistema, dando-lhe uma chance de liberar recursos.
    AFX_OPCODE_ATTACH, // informa a um módulo que ele será acoplado ao sistema, dando-lhe uma chance de realizar sua devida initialização de recursos e operações.
};

#if 0
A implentação atual de sistema de arquivos é mentalizada de forma simples.
A operação requerida é tentada para cada volume montado, volume este de prefixo correspondente,
entretando, começando a partir do mais recentemente mountado.Um prefixo vazio é considerado
a sempre corresponder, assim frequentemente serão usados como um 'fallback' final.

Uma configuração típica para um dispositivo móvel deveria prover muitos volumes, dentre - os:
um volume somente - leitura para o pacote da aplicação, então um volume modificiável para o
uma localização dentro do mmcard.E finalmente um volume capture - tudo para manejar caminhos
absolutos.

1. ("Resources", "/path/to/app/bundle/Resources", writable = False)
2. ("Resources", "/mnt/sdcard/Havok/Resources", writable = True)
3. ("", "", writable = False)

There is no support for whiteouts so if a file exists in more than
one mount, it will still be present after removing the first one.
Similarly when iterating, a file with the same name but different
contents may be returned.
#endif

#define AFX_FS_SYM_LEN  (32)
#define AFX_FS_DIR_LEN  (1024 + 1 + AFX_FS_SYM_LEN + 1)

// A mount point is a directory on a file system that is logically linked to another file system.
// Mount points are used to make the data on a different physical storage drive easily available in a folder structure.
// Mount points are fundamental to Unix, Linux and macOS. Windows can use mount points, but it is not common.

// Mounting is a process by which the operating system makes files and directories on a storage device available for users to access via the computer's file system.

AFX_DEFINE_STRUCT(afxMountPointSpecification)
{
    afxString const *name;
    afxString const *path;
    afxBool         rwx[3];
    afxBool         isZip;
};

typedef struct afxUriResolver
{
    afxChar         *access; // "rwx"
    afxNat          _dropParts;
    afxUri const    *in;
    afxUri          *out;
    afxTime         *lastUpdTime;
} afxUriResolver;

typedef struct afxResourceInfo
{
    // virtual file
    // blob
    // file
    // directory
    afxBool isFile; // else it is a directory
    
} afxResourceInfo;

AFX_DEFINE_STRUCT(afxSystemSpecification)
{
    afxChar const                       *root;
    afxNat                              mntCnt;
    afxMountPointSpecification const    *mntSpecs;
    afxSize                             maxMemUsage;
};

AFX afxAllocator       AfxSystemAcquireAllocator(afxSystem sys, afxAllocationStrategy const *strategy, afxHint const hint);
AFX afxArchive          AfxSystemAcquireArchive(afxSystem sys, afxUri const *uri, afxChar const *access);
AFX afxApplication      AfxSystemAcquireApplication(afxSystem sys, afxApplicationSpecification const *spec);
AFX afxDrawSystem       AfxSystemAcquireDrawSystem(afxSystem sys);
AFX afxHid              AfxSystemAcquireHid(afxSystem sys, afxNat port);
AFX afxKeyboard         AfxSystemAcquireKeyboard(afxSystem sys, afxNat port);
AFX afxModule           AfxSystemAcquireModule(afxSystem sys, afxUri const *uri);
AFX afxMouse            AfxSystemAcquireMouse(afxSystem sys, afxNat port);
AFX afxResource         AfxSystemAcquireResource(afxSystem sys, afxResourceSpecification const *spec);
AFX afxStream           AfxSystemAcquireStream(afxSystem sys, afxUri const *uri, afxChar const *access);
//AFX afxUrd              AfxSystemBuildUrd(afxSystem sys, afxChar const *src);
AFX afxUrd              AfxSystemUploadUrd(afxSystem sys, afxUri const *uri);
AFX afxThread           AfxSystemAcquireThread(afxSystem sys, void (*start)(afxThread thr, void *udd), void *udd, void (*exec)(afxThread thr), afxHint const hint);

AFX afxHid              AfxSystemFindHid(afxSystem sys, afxNat port);
AFX afxKeyboard         AfxSystemFindKeyboard(afxSystem sys, afxNat port);
AFX afxModule           AfxSystemFindModule(afxSystem sys, afxUri const *uri);
AFX afxMouse            AfxSystemFindMouse(afxSystem sys, afxNat port);

AFX afxClass*           AfxSystemGetAllocatorClass(afxSystem sys);
AFX afxClass*           AfxSystemGetArchiveClass(afxSystem sys);
AFX afxClass*           AfxSystemGetApplicationClass(afxSystem sys);
AFX afxClass*           AfxSystemGetDrawSystemClass(afxSystem sys);
AFX afxClass*           AfxSystemGetHidClass(afxSystem sys);
AFX afxClass*           AfxSystemGetKeyboardClass(afxSystem sys);
AFX afxClass*           AfxSystemGetModuleClass(afxSystem sys);
AFX afxClass*           AfxSystemGetMouseClass(afxSystem sys);
AFX afxClass*           AfxSystemGetResourceClass(afxSystem sys);
AFX afxClass*           AfxSystemGetStreamClass(afxSystem sys);
AFX afxClass*           AfxSystemGetThreadClass(afxSystem sys);
AFX afxClass*           AfxSystemGetUrdClass(afxSystem sys);

AFX afxAllocator        AfxSystemGetAllocator(afxSystem sys);
AFX afxUri const*       AfxSystemGetRootUri(afxSystem sys, afxUri *copy);
AFX afxString const*    AfxSystemGetRootUriString(afxSystem sys, afxString *copy);

AFX afxResult           AfxSystemForEachAllocator(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachArchive(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachApplication(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachDrawSystem(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachHid(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachKeyboard(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachModule(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachMountPoint(afxSystem sys, void(*f)(afxIterator *iter), void* data);
AFX afxResult           AfxSystemForEachMouse(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachResource(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachStream(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachThread(afxSystem sys, void(*f)(afxIterator *iter), void *data);
AFX afxResult           AfxSystemForEachUrd(afxSystem sys, void(*f)(afxIterator *iter), void *data);

AFX afxResult           AfxSystemMountPoints(afxSystem sys, afxNat cnt, afxMountPointSpecification const query[]);
AFX afxResult           AfxSystemDismountPoints(afxSystem sys, afxNat cnt, afxMountPointSpecification const query[]);

AFX afxError            AfxSystemResolveUri(afxSystem sys, afxUriResolver const *resolver);
AFX afxResult           AfxSystemResolveUris(afxSystem sys, afxNat cnt, afxUriResolver const resolver[]);

AFX afxBool             AfxSystemNotify(afxSystem sys, afxObject *receiver, afxEvent *ev);
AFX afxBool             AfxSystemEmitEvent(afxSystem sys, afxObject *receiver, afxEvent *ev);
AFX afxBool             AfxSystemEmitEvents(afxSystem sys, afxNat cnt, afxObject *receiver[], afxEvent ev[]);
AFX afxBool             AfxSystemReemitEvent(afxSystem sys, afxNat cnt, afxObject *receiver[], afxEvent *ev);
AFX afxError            AfxSystemPostEvent(afxSystem sys, afxObject *receiver, afxEvent *ev);
AFX afxError            AfxSystemPostEvents(afxSystem sys, afxNat cnt, afxObject *receiver[], afxEvent ev[]);

AFX afxSystem           AfxSystemGet(void);
AFX afxSystem           AfxSystemBootUp(afxSystemSpecification const *spec);
AFX afxError            AfxSystemProcess(afxSystem sys);

#define _AFX_SYS_IMPL
#ifdef _AFX_SYS_IMPL

AFX_OBJECT(afxSystem)
{
    afxObject               obj;
    afxChain                provisions;

    afxUri4096              rootDir;
    afxList                 mountPoints;
    afxChain                aliveRsrcs;

    afxClass                allClass;
    afxClass                thrClass;
    afxClass                mdleClass;
    afxClass                iosClass;
    afxClass                archClass;
    afxClass                urdClass;
    afxClass                resClass;
    afxClass                hidClass;
    afxClass                kbdClass;
    afxClass                mseClass;
    afxClass                dsysClass;
    afxClass                appClass; // can use everything

    afxNat                  nofProcessors;
    afxNat                  memPageSize; // The page size and the granularity of page protection and commitment.
    afxAllocator            genrlAll;
    afxModule               e2coree;
    afxThread               deusExMachina;
    afxKeyboard             stdKbd;
};

AFX_DEFINE_STRUCT(afxMountPoint)
{
    afxFcc                  fcc;
    afxLink                 biosLink;
    afxSystem               sys;

    afxString32     name; // AFX_FS_SYM_LEN // canv't have control chars. Actually works like a variable without $().
    afxString512    path;
    afxBool                 rwx[3];
    afxBool                 isZip;
};

#endif

AFX afxNat mainThreadId;

#endif//AFX_SYSTEM_H