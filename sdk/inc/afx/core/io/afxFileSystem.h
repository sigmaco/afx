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

// SIGMA Future Storage

#ifndef AFX_FILE_SYSTEM_H
#define AFX_FILE_SYSTEM_H

#include "afx/core/io/afxFile.h"
#include "afx/core/io/afxArchive.h"
#include "afx/core/io/afxResource.h"

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

#define AFX_FS_SYM_LEN  (32)
#define AFX_FS_DIR_LEN  (1024 + 1 + AFX_FS_SYM_LEN + 1)

// A mount point is a directory on a file system that is logically linked to another file system.
// Mount points are used to make the data on a different physical storage drive easily available in a folder structure.
// Mount points are fundamental to Unix, Linux and macOS. Windows can use mount points, but it is not common.

// Mounting is a process by which the operating system makes files and directories on a storage device available for users to access via the computer's file system.

AFX_DEFINE_STRUCT(afxMountPointSpecification)
{
    afxUri const    *namespace;
    afxUri const    *hostPath;
    afxBool         rwx[3];
    afxBool         isZip;
};

AFX_DEFINE_HANDLE(afxFileSystem);

#ifndef AFX_FILE_SYSTEM_C

AFX_OBJECT(afxFileSystem) { afxObject obj; };

#endif

AFX void*           AfxFileSystemGetSystem(afxFileSystem fsys);
AFX afxAllocator    AfxFileSystemGetAllocator(afxFileSystem fsys);

AFX afxStream       AfxFileSystemAcquireStream(afxFileSystem fsys, afxRwx const rwx, void *buf, afxNat bufCap);
AFX afxFile         AfxFileSystemOpenFile(afxFileSystem fsys, afxRwx const rwx, afxUri const *uri);
AFX afxFile         AfxFileSystemLoadFile(afxFileSystem fsys, afxRwx const rwx, afxUri const *uri);

AFX afxArchive      AfxFileSystemAcquireArchive(afxFileSystem fsys, afxUri const *uri, afxChar const *access);
AFX afxResource     AfxFileSystemAcquireResource(afxFileSystem fsys, afxResourceSpecification const *spec);
AFX afxUrd          AfxFileSystemLoadUrd(afxFileSystem fsys, afxUri const *uri);

AFX afxResult       AfxFileSystemMountPoints(afxFileSystem fsys, afxNat cnt, afxMountPointSpecification const spec[]);
AFX afxResult       AfxFileSystemDismountPoints(afxFileSystem fsys, afxNat cnt, afxMountPointSpecification const spec[]);
AFX afxResult       AfxFileSystemDescribeMountPoints(afxFileSystem fsys, afxNat base, afxNat cnt, afxMountPointSpecification spec[]);
AFX afxNat          AfxFileSystemGetMountPointCount(afxFileSystem fsys);

AFX afxResult       AfxFileSystemEnumerateArchives(afxFileSystem fsys, afxNat base, afxNat cnt, afxArchive arch[]);
AFX afxResult       AfxFileSystemEnumerateResources(afxFileSystem fsys, afxNat base, afxNat cnt, afxResource res[]);
AFX afxResult       AfxFileSystemEnumerateStreams(afxFileSystem fsys, afxNat base, afxNat cnt, afxStream ios[]);
AFX afxResult       AfxFileSystemEnumerateUrds(afxFileSystem fsys, afxNat base, afxNat cnt, afxUrd urd[]);

AFX afxError        AfxFileSystemResolveUri(afxFileSystem fsys, afxUriResolver const *resolver);
AFX afxError        AfxFileSystemResolveUris(afxFileSystem fsys, afxNat cnt, afxUriResolver const resolver[]);

AFX afxError        AfxFileSystemEnableResourceMonitoring(afxFileSystem fsys, afxResource res, afxBool enable);

AFX afxClass*       AfxFileSystemGetFileClass(afxFileSystem fsys);
AFX afxClass*       AfxFileSystemGetArchiveClass(afxFileSystem fsys);
AFX afxClass*       AfxFileSystemGetResourceClass(afxFileSystem fsys);
AFX afxClass*       AfxFileSystemGetStreamClass(afxFileSystem fsys);
AFX afxClass*       AfxFileSystemGetUrdClass(afxFileSystem fsys);

AFX afxError        _AfxFileSystemProcess(afxFileSystem fsys);

#endif//AFX_FILE_SYSTEM_H