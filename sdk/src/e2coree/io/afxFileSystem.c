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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#include <sys/stat.h>
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_FILE_SYSTEM_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"

/******************************************************************************

 Unix-like systems implement three specific permissions that apply to each class:

 The read permission grants the ability to read a file. When set for a directory,
 this permission grants the ability to read the names of files in the directory,
 but not to find out any further information about them such as contents, file class,
 size, ownership, permissions.

 The write permission grants the ability to modify a file. When set for a directory,
 this permission grants the ability to modify entries in the directory, which includes
 creating files, deleting files, and renaming files. Note that this requires that
 execute is also set; without it, the write permission is meaningless for directories.

 The execute permission grants the ability to execute a file. This permission must be
 set for executable programs, in order to allow the operating system to run them. When
 set for a directory, the execute permission is interpreted as the search permission:
 it grants the ability to access file contents and meta-information if its name is
 known, but not list files inside the directory, unless read is set also.

 The effect of setting the permissions on a directory, rather than a file, is "one of
 the most frequently misunderstood file permission issues".

 When a permission is not set, the corresponding rights are denied. Unlike ACL-based
 systems, permissions on Unix-like systems are not inherited. Files created within a
 directory do not necessarily have the same permissions as that directory.

 ******************************************************************************/

_AFX afxError AfxResolveUris(afxFileFlags const permissions, afxNat cnt, afxUri const in[], afxUri out[])
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys = AfxGetSystem();
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxAssert(in);
    AfxAssert(out);
    AfxAssert(permissions);

    for (afxNat i = 0; i < cnt; i++)
    {
        AfxAssert(!AfxUriIsBlank(&in[i]));

        afxFileFlags ioPerms = permissions ? permissions & AFX_FILE_PERM_MASK : afxFileFlag_RX;
        afxBool resolved = FALSE;
        struct stat st;

        unsigned short flagsToTest = NIL;

        if (AfxTestFlags(ioPerms, afxFileFlag_R))
            flagsToTest |= _S_IREAD;

        if (AfxTestFlags(ioPerms, afxFileFlag_W))
            flagsToTest |= _S_IWRITE;

        //if (AfxTestFlags(ioPerms, afxIoFlag_X))
            //flagsToTest |= _S_IEXEC;

#if 0
        if (fileStr)
            flagsToTest |= _S_IFREG;
        else if (dirStr)
            flagsToTest |= _S_IFDIR;
#endif

        afxString const *srcStr = AfxGetUriString(&in[i]);
        afxString const *dstStr = AfxGetUriString(&out[i]);
        afxChar const* srcData = AfxGetStringData(srcStr, 0);
        afxChar const* dstData = AfxGetStringData(dstStr, 0);

        if (('a' > srcData[0] && 'z' < srcData[0]) || ('A' > srcData[0] && 'Z' < srcData[0]))
        {
            AfxCopyUri(&out[i], &in[i]);

            if (stat(dstData, &(st))) AfxThrowError();
            else
            {
                if (!((st.st_mode & flagsToTest) == flagsToTest))
                    AfxThrowError();
            }
            resolved = TRUE;
        }
        else
        {
            afxUri path, dir, file;
            AfxExcerptUriPath(&in[i], &path);
            AfxExcerptUriDirectory(&in[i], &dir);
            AfxExcerptUriFile(&in[i], &file);
            afxString const *pathStr = AfxGetUriString(&path);
            afxString const *dirStr = AfxGetUriString(&dir);
            afxString const *fileStr = AfxGetUriString(&file);

            srcData = AfxGetStringData(pathStr, 0);

            afxChar const *pointStart = srcData;

            afxChar const *pointEnd = AfxStrchr(pointStart, '/');

            afxString point;
            AfxMakeString(&point, pointStart, pointEnd ? pointEnd - pointStart : AfxGetStringLength(pathStr));
            afxString const *pointStr = &point;

            if (pointEnd && *pointEnd == '/')
                pointEnd++; // skip bar

            afxNat pointEndRange = pointEnd ? AfxGetStringLength(pathStr) - (pointEnd - pointStart) : AfxGetStringLength(pathStr);

            afxBool hasWildcard = (AFX_INVALID_INDEX != AfxFindFirstChar(pathStr, 0, '*'));

#if 0
#define _S_IFMT   0xF000 // File class mask
#define _S_IFDIR  0x4000 // Directory
#define _S_IFCHR  0x2000 // Character special
#define _S_IFIFO  0x1000 // Pipe
#define _S_IFREG  0x8000 // Regular
#define _S_IREAD  0x0100 // Read permission, owner
#define _S_IWRITE 0x0080 // Write permission, owner
#define _S_IEXEC  0x0040 // Execute/search permission, owner
#endif
            AfxCopyUri(&out[i], &in[i]);

            if (!pointEnd)
            {
                AfxFormatUri(&out[i], "%.*s", AfxGetStringLength(pathStr), AfxGetStringData(pathStr, 0));

                if (stat(dstData, &(st))) AfxThrowError();
                else
                {
                    if (!((st.st_mode & flagsToTest) == flagsToTest))
                        AfxThrowError();

                }

                resolved = TRUE;
            }
            else
            {
                afxNat j = AfxCountFileSystems();
                afxFileSystem fsys;
                while (AfxEnumerateFileSystems(--j, 1, &fsys))
                {
                    AfxAssertObjects(1, &fsys, afxFcc_FSYS);

                    if ((fsys->flags & ioPerms) == ioPerms)
                    {
                        if (0 == AfxCompareString(&point, AfxGetUriString(&fsys->name.uri)))
                        {
                            AfxFormatUri(&out[i], "%.*s/%.*s\0", AfxPushString(AfxGetUriString(&fsys->path)), pointEndRange, pointEnd);
                            AfxEcho(dstData);

                            if (!stat(dstData, &(st)) || (ioPerms & afxFileFlag_W) || hasWildcard)
                            {
                                if (((st.st_mode & flagsToTest) == flagsToTest) || ioPerms & afxFileFlag_W || hasWildcard)
                                {
                                    resolved = TRUE;
                                    break;
                                }
                            }
                        }
                    }
                }

                if (!resolved)
                    AfxThrowError(); // unresolved.
            }

            //AfxAdvertise("%x %x %x %x %x", st.st_mode & _S_IFREG, st.st_mode & _S_IFDIR, st.st_mode & _S_IREAD, st.st_mode & _S_IWRITE, st.st_mode & _S_IEXEC);
        }

  //      if (!resolved)
//            AfxCopyUri(&out[i], &in[i]);
    }
    return err;
}

_AFX afxError AfxResolveUri(afxFileFlags permissions, afxUri const *in, afxUri *out)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(out);
    AfxAssert(!AfxUriIsBlank(in));

    if (AfxResolveUris(permissions, 1, in, out))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxFsysCtor(afxFileSystem fsys, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);

    afxString const* point = cookie->udd[0];
    afxString const* location = cookie->udd[1];
    afxIoFlags perm = *(afxIoFlags const*)cookie->udd[2];

    AfxAssert(perm);
    AfxAssert(perm & AFX_IO_PERM_MASK);

    afxUri path, hostPath;
    AfxUriFromString(&hostPath, location);
    AfxExcerptUriPath(&hostPath, &path);

    afxUri2048 bufUri;
    AfxMakeUri2048(&bufUri, NIL);

    if (AfxPathIsRelative(&path))
    {
        afxUri const *rootUri = AfxGetSystemDirectory(NIL);
        afxString const *rootUriString = AfxGetSystemDirectoryString(NIL);
        AfxFormatString(&bufUri.uri.str, "%.*s"
#ifdef AFX_PLATFORM_WIN
            "\\"
#else
            "/"
#endif
            "%.*s"
#ifdef AFX_PLATFORM_WIN
            "\\"
#else
            "/"
#endif
            , AfxPushString(rootUriString), AfxPushString(AfxGetUriString(&path)));
        AfxReparseUri(&bufUri.uri);
    }
    else
    {
        AfxCopyUri(&bufUri.uri, &path);
    }

    afxChar const *pathRaw = AfxGetUriData(&bufUri.uri, 0);
    struct stat st;

    if (stat(pathRaw, &(st))) AfxThrowError();
    else
    {
        unsigned short flagsToTest = NIL;
        afxIoFlags ioPerms = perm & AFX_IO_PERM_MASK;

        if (AfxTestFlags(ioPerms, afxIoFlag_R))
            flagsToTest |= _S_IREAD;

        if (AfxTestFlags(ioPerms, afxIoFlag_W))
            flagsToTest |= _S_IWRITE;

        if (AfxTestFlags(ioPerms, afxIoFlag_X))
            flagsToTest |= _S_IEXEC;

        if (!((st.st_mode & flagsToTest) == flagsToTest)) AfxThrowError();
        else
        {
            afxBool isDir = !!(st.st_mode & _S_IFDIR);
            afxBool isFile = !!(st.st_mode & _S_IFREG);
            afxArchive arc = NIL;

            if (!isDir && (isFile && AfxAcquireArchives(1, &arc, &bufUri.uri, (afxFileFlags[]) { ioPerms | afxFileFlag_D }))) AfxThrowError();
            else
            {
                afxUri uri;
                AfxUriFromString(&uri, point);
                AfxMakeUri8(&fsys->name, &uri);

                //if (!(fssp->name)) AfxThrowError();
                //else
                {
                    AfxCloneUriPath(&fsys->path, &bufUri.uri);

                    //if (!(fssp->path)) AfxThrowError();
                    //else
                    {
                        fsys->flags = ioPerms;
                        fsys->arc = arc;

                        AfxEcho("Storage point \"%.*s\" %x mounted -> \"%.*s\".", AfxPushString(AfxGetUriString(&fsys->name.uri)), fsys->flags, AfxPushString(AfxGetUriString(&fsys->path)));

                        if (err)
                            AfxDeallocateUri(&fsys->path);
                    }
                }

                if (err && arc)
                    AfxReleaseObjects(1, (void*[]) { arc });
            }
        }
    }

    return err;
}

_AFX afxError _AfxFsysDtor(afxFileSystem fsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);

    AfxEcho("Storage point \"%.*s\" %x unmounted -> \"%.*s\".", AfxPushString(AfxGetUriString(&fsys->name.uri)), fsys->flags, AfxPushString(AfxGetUriString(&fsys->path)));
    
    if (fsys->arc)
        AfxReleaseObjects(1, (void*[]) { fsys->arc });

    AfxDeallocateUri(&fsys->path);

    return err;
}

_AFX afxClassConfig const _AfxFsysClsConfig =
{
    .fcc = afxFcc_FSYS,
    .name = "File System",
    .unitsPerPage = 6,
    .size = sizeof(AFX_OBJECT(afxFileSystem)),
    .ctor = (void*)_AfxFsysCtor,
    .dtor = (void*)_AfxFsysDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxMountFileSystem(afxString const* point, afxString const* location, afxIoFlags ioFlags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(point);
    AfxAssert(location);
    AfxAssert(ioFlags);
    afxFileSystem fsys;

    afxClass* cls = AfxGetFileSystemClass();
    AfxAssertClass(cls, afxFcc_FSYS);

    if (AfxAcquireObjects(cls, 1, (afxObject*)&fsys, (void const*[]) { point, location, &ioFlags }))
        AfxThrowError();

    return err;
}

_AFX afxNat AfxInvokeFileSystems(afxNat first, afxNat cnt, afxBool(*f)(afxFileSystem, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetFileSystemClass();
    AfxAssertClass(cls, afxFcc_FSYS);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateFileSystems(afxNat first, afxNat cnt, afxFileSystem systems[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(systems);
    afxClass* cls = AfxGetFileSystemClass();
    AfxAssertClass(cls, afxFcc_FSYS);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)systems);
}

_AFX afxNat AfxCountFileSystems(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetFileSystemClass();
    AfxAssertClass(cls, afxFcc_FSYS);
    return AfxCountInstances(cls);
}
