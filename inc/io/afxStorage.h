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

// This code is part of SIGMA Future Storage.

/**
    A mount point is a directory on a file system that is logically linked to another file system.
    Mount points are used to make the data on a different physical storage drive easily available in a folder structure.
    Mount points are fundamental to Unix, Linux and macOS. Windows can use mount points, but it is not common.

    Mounting is a process by which the operating system makes files and directories on a storage device available for users to access via the computer's file system.

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

    The mount command instructs the Qwadro to make a file system available for use at a specified location (the mount point).
    The mount command mounts a file system that is expressed as a directory by using the Node:Directory parameter on the directory specified by the Directory parameter.
    After the mount command has finished, the directory that is specified becomes the root directory of the newly mounted file system.
*/

#ifndef AFX_STORAGE_H
#define AFX_STORAGE_H

#include "qwadro/inc/io/afxArchive.h"
#include "qwadro/inc/io/afxFile.h"
#include "qwadro/inc/io/afxStream.h"
#include "qwadro/inc/io/afxUrd.h"
#include "qwadro/inc/io/afxUri.h"

#define AFX_FS_SYM_LEN  (8)
#define AFX_FS_DIR_LEN  (1024 + 1 + AFX_FS_SYM_LEN + 1)

typedef enum afxStorageUsage
{
    afxStorageUsage_SYSTEM  = AFX_BIT(0),
    afxStorageUsage_SOUND   = AFX_BIT(1),
    afxStorageUsage_CODE    = AFX_BIT(2),
    afxStorageUsage_DATA    = AFX_BIT(3),
    afxStorageUsage_TEMP    = AFX_BIT(4),
    afxStorageUsage_ART     = AFX_BIT(5),
}
afxStorageUsage;

AFX_DEFINE_STRUCT(afxStoragePointSpecification)
{
    afxUri const*   point; // exchange point
    afxString const*hostPath;
    afxIoFlags      perm;
    afxBool         isZip;

    afxStorageUsage usage;
};

AFX_DEFINE_STRUCT(afxFileInfo)
{
    afxFileFlags perm;
    // Device ID of device containing file.
    afxUnit      devId;
    // File serial number. Aka inode number (unused on Windows)
    afxUnit16    inodeNum;
    // Mode of file
    afxUnit16    mode;
    // Number of hard links to the file.
    afxInt16    nofLinks;
    // User ID of file.
    afxInt16    uid;
    // Group ID of file.
    afxInt16    gid;
    // Device ID (if file is character or block special).
    afxUnit     rdevId;
    // For regular files, the file size in bytes. For symbolic links, the length in bytes of the pathname contained in the symbolic link. For a shared memory object, the length in bytes. For a typed memory object, the length in bytes. For other file types, the use of this field is unspecified.
    afxSize     siz;
    // Time of last access.
    afxTime     lastAccess;
    // Time of last data modification.
    afxTime     lastMod;
    // Time of last status change.
    afxTime     creationTime;
};

AFX afxBool             AfxGetStorage(afxChar diskId, afxStorage* disk);
AFX afxBool             AfxFindStorage(afxUri const* uri, afxStorage* disk);

AFX afxError            AfxMountStorageUnit(afxChar diskId, afxUri const* endpoint, afxFileFlags ioFlags);
AFX afxError            AfxDismountStorageUnit(afxChar diskId, afxUri const* endpoint, afxFileFlags ioFlags);

AFX afxUnit             AfxFindStorageUnit(afxStorage fsys, afxUri const* endpoint, afxFileFlags ioFlags);

// TODO: Directories starting with "//./" will be resolved as devices instead of paths. Ex.: "//./e/dir/file.ext" -> "E:\\dir\\file.ext"

AFX afxError            AfxResolveUri(afxFileFlags permissions, afxUri const *in, afxUri *out);
AFX afxError            AfxResolveUris(afxFileFlags const permissions, afxUnit cnt, afxUri const in[], afxUri out[]);

AFX afxError            AfxResolveUri2(afxFileFlags permissions, afxUri const *in, afxUri *out, afxUnit* diskId);

AFX afxUnit             AfxFindFiles(afxUri const* pattern, afxFileFlags flags, afxBool(*proc)(void* udd, afxUri const* path, afxUri const* osPath), void* udd);
AFX afxError            AfxForEachUriResolution(afxUri const* pattern, afxFileFlags flags, afxBool(*proc)(void* udd, afxUri const* uri), void* udd);

AFX afxClass const*     AfxGetFileClass(afxStorage fsys);
AFX afxClass const*     AfxGetArchiveClass(afxStorage fsys);

#endif//AFX_STORAGE_H
