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

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_FILE_SYSTEM_H
#define AFX_FILE_SYSTEM_H

#include "afx/core/io/afxFile.h"
#include "afx/core/io/afxArchive.h"
#include "afx/core/io/afxResource.h"
#include "afx/core/io/afxUrd.h"

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

typedef struct afxUriResolver
{
    afxIoFlags      permissions;
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

AFX_DEFINE_STRUCT(afxFileSystemSpecification)
{
    afxMemory                        genrlMem;
    afxNat                              mpCnt;
    afxStoragePointSpecification const*   mpSpec;
};

AFX_DEFINE_HANDLE(afxFileSystem);

AFX_OBJECT(afxFileSystem)
{
    afxObject       obj;
#ifdef _AFX_FILE_SYSTEM_C
    afxSystem       sys;
    afxChain        provisions;
    afxMemory       genrlMem;
    afxClass        iosClass;
    afxClass        fileClass;
    afxClass        archClass;
    afxClass        xmlClass;
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

AFX afxSystem       AfxFileSystemGetSystem(afxFileSystem fsys);
AFX afxMemory       AfxFileSystemGetMemory(afxFileSystem fsys);

AFX afxStream       AfxFileSystemAcquireStream(afxFileSystem fsys, afxIoFlags flags, void *buf, afxNat bufCap);
AFX afxFile         AfxFileSystemOpenFile(afxFileSystem fsys, afxIoFlags flags, afxUri const *uri);
AFX afxFile         AfxFileSystemOpenReadableFile(afxFileSystem fsys, afxUri const *uri);
AFX afxFile         AfxFileSystemOpenWritableFile(afxFileSystem fsys, afxUri const *uri);
AFX afxStream       AfxFileSystemLoadFile(afxFileSystem fsys, afxIoFlags flags, afxUri const *uri); // will fully load the opened file then close it.

AFX afxArchive      AfxFileSystemAcquireArchive(afxFileSystem fsys, afxIoFlags flags, afxUri const *uri);
AFX afxResource     AfxFileSystemAcquireResource(afxFileSystem fsys, afxResourceSpecification const *spec);
AFX afxXml          AfxFileSystemLoadXml(afxFileSystem fsys, afxUri const *uri);
AFX afxUrd          AfxFileSystemLoadUrd(afxFileSystem fsys, afxUri const *uri);

AFX afxNat          AfxFileSystemGetStoragePointCount(afxFileSystem fsys);
AFX afxResult       AfxFileSystemMountStoragePoints(afxFileSystem fsys, afxNat cnt, afxStoragePointSpecification const spec[]);
AFX afxResult       AfxFileSystemDismountStoragePoints(afxFileSystem fsys, afxNat cnt, afxStoragePointSpecification const spec[]);
AFX afxResult       AfxFileSystemDescribeStoragePoints(afxFileSystem fsys, afxNat base, afxNat cnt, afxStoragePointSpecification spec[]);

AFX afxResult       AfxFileSystemEnumerateArchives(afxFileSystem fsys, afxNat base, afxNat cnt, afxArchive arch[]);
AFX afxResult       AfxFileSystemEnumerateFiles(afxFileSystem fsys, afxNat base, afxNat cnt, afxFile file[]);
AFX afxResult       AfxFileSystemEnumerateResources(afxFileSystem fsys, afxNat base, afxNat cnt, afxResource res[]);
AFX afxResult       AfxFileSystemEnumerateStreams(afxFileSystem fsys, afxNat base, afxNat cnt, afxStream ios[]);
AFX afxResult       AfxFileSystemEnumerateXmls(afxFileSystem fsys, afxNat base, afxNat cnt, afxXml xml[]);
AFX afxResult       AfxFileSystemEnumerateUrds(afxFileSystem fsys, afxNat base, afxNat cnt, afxUrd urd[]);

AFX afxError        AfxFileSystemResolveUri(afxFileSystem fsys, afxUriResolver const *resolver);
AFX afxError        AfxFileSystemResolveUris(afxFileSystem fsys, afxNat cnt, afxUriResolver const resolver[]);

AFX afxError        AfxFileSystemEnableResourceMonitoring(afxFileSystem fsys, afxResource res, afxBool enable);

AFX afxClass*       AfxFileSystemGetArchiveClass(afxFileSystem fsys);
AFX afxClass*       AfxFileSystemGetFileClass(afxFileSystem fsys);
AFX afxClass*       AfxFileSystemGetResourceClass(afxFileSystem fsys);
AFX afxClass*       AfxFileSystemGetStreamClass(afxFileSystem fsys);
AFX afxClass*       AfxFileSystemGetXmlClass(afxFileSystem fsys);
AFX afxClass*       AfxFileSystemGetUrdClass(afxFileSystem fsys);

AFX afxError        _AfxFileSystemProcess(afxFileSystem fsys);

#endif//AFX_FILE_SYSTEM_H