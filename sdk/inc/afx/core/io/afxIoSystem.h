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

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_FILE_SYSTEM_H
#define AFX_FILE_SYSTEM_H

#include "afx/core/io/afxFile.h"
#include "afx/core/io/afxArchive.h"
#include "afx/core/io/afxResource.h"
#include "afx/core/io/afxUrd.h"
#include "afx/core/mem/afxArena.h"

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

typedef struct afxResourceInfo
{
    // virtual file
    // blob
    // file
    // directory
    afxBool isFile; // else it is a directory

} afxResourceInfo;

#define AFX_FS_SYM_LEN  (32)
#define AFX_FS_DIR_LEN  (1024 + 1 + AFX_FS_SYM_LEN + 1)

// A mount point is a directory on a file system that is logically linked to another file system.
// Mount points are used to make the data on a different physical storage drive easily available in a folder structure.
// Mount points are fundamental to Unix, Linux and macOS. Windows can use mount points, but it is not common.

// Mounting is a process by which the operating system makes files and directories on a storage device available for users to access via the computer's file system.

AFX_DEFINE_STRUCT(afxStoragePointSpecification)
{
    afxUri const    *namespace; // exchange point
    afxUri const    *hostPath;
    afxIoFlags      perm;
    afxBool         isZip;
};

AFX_DEFINE_STRUCT(afxIoSystemSpecification)
{
    afxMemory                           genrlMem;
    afxNat                              bufSiz;
    afxNat                              mpCnt;
    afxStoragePointSpecification const* mpSpec;
};

AFX_DEFINE_STRUCT(afxFileInfo)
{
    afxFileFlags perm;
    afxNat      devId; // Device ID of device containing file.
    afxNat16    inodeNum; // File serial number. Aka inode number (unused on Windows)
    afxNat16    mode; // Mode of file
    afxInt16    nofLinks; // Number of hard links to the file.
    afxInt16    uid; // User ID of file.
    afxInt16    gid; // Group ID of file.
    afxNat      rdevId; // Device ID (if file is character or block special).
    afxSize     siz; // For regular files, the file size in bytes. For symbolic links, the length in bytes of the pathname contained in the symbolic link. For a shared memory object, the length in bytes. For a typed memory object, the length in bytes. For other file types, the use of this field is unspecified.
    afxTime     lastAccess; // Time of last access.
    afxTime     lastMod; // Time of last data modification.
    afxTime     creationTime; // Time of last status change.
};

AFX_OBJECT(afxIoSystem)
{
    afxObject       obj;
#ifdef _AFX_FILE_SYSTEM_C
    afxChain        provisions;
    afxMemory       mem;
    afxArena        aren;
    afxNat          bufSiz;
    afxClass        fileClass;
    afxClass        arcClass;
    afxClass        urdClass;
    afxClass        resClass;
    afxChain        mountPoints;
    afxChain        aliveRsrcs;
    struct
    {
        struct
        {
            afxFcc      type;
            afxChain    resources;
        }           supplyChain[1];
    }               resourcing;
#endif
};

AFX afxBool         AfxGetIoSystem(afxIoSystem *io);
AFX void            AfxShutdownIoSystem(void);
AFX afxError        AfxBootUpIoSystem(afxIoSystemSpecification const *spec, afxIoSystem *io);

AFX afxMemory       AfxGetIoMemory(void);
AFX afxArena*       AfxGetIoArena(void);
AFX afxNat          AfxGetIoBufferSize(void);

AFX afxArchive      AfxAcquireArchive(afxFileFlags flags, afxUri const *uri);
AFX afxResource     AfxAcquireResource(afxResourceSpecification const *spec);
AFX afxUrd          AfxAcquireUrd(afxUri const *path);

AFX afxNat          AfxGetStoragePointCount(void);
AFX afxResult       AfxMountStoragePoints(afxNat cnt, afxStoragePointSpecification const spec[]);
AFX afxResult       AfxDismountStoragePoints(afxNat cnt, afxStoragePointSpecification const spec[]);
AFX afxResult       AfxDescribeStoragePoints(afxNat first, afxNat cnt, afxStoragePointSpecification spec[]);

AFX afxNat          AfxEnumerateArchives(afxNat first, afxNat cnt, afxArchive arc[]);
AFX afxNat          AfxEnumerateFiles(afxNat first, afxNat cnt, afxFile file[]);
AFX afxNat          AfxEnumerateResources(afxNat first, afxNat cnt, afxResource res[]);
AFX afxNat          AfxEnumerateUrds(afxNat first, afxNat cnt, afxUrd urd[]);

AFX afxError        AfxEnableResourceMonitoring(afxResource res, afxBool enable);

AFX afxClass*       AfxGetArchiveClass(void);
AFX afxClass*       AfxGetFileClass(void);
AFX afxClass*       AfxGetResourceClass(void);
AFX afxClass*       AfxGetUrdClass(void);

AFX afxError            AfxResolveUri(afxUri const *in, afxFileFlags permissions, afxUri *out);
AFX afxError            AfxResolveUris(afxNat cnt, afxUri const in[], afxFileFlags const permissions[], afxUri out[]);

#endif//AFX_FILE_SYSTEM_H