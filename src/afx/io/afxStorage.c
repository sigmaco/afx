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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#include <sys/stat.h>
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_STORAGE_C
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
    afxSystem sys;
    AfxGetSystem(&sys);
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
            AfxPickUriPath(&in[i], &path);
            AfxPickUriDirectory(&in[i], &dir);
            AfxPickUriFile(&in[i], &file);
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
                AfxCanonicalizePath(&out[i], AFX_OS_WIN);

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
                afxUri uri3;
                AfxUriFromString(&uri3, &point);
                afxStorage fsys;
                
                if ((fsys = AfxFindStorage(&uri3)))
                {
                    AfxAssertObjects(1, &fsys, afxFcc_FSYS);

                    afxStorageUnit const* fsto;
                    AfxIterateLinkage(afxStorageUnit, fsto, &fsys->storages, fsys)
                    {
                        if ((fsto->flags & ioPerms) == ioPerms)
                        {
                            AfxFormatUri(&out[i], "%.*s/%.*s\0", AfxPushString(AfxGetUriString(&fsto->rootPath)), pointEndRange, pointEnd);
                            AfxCanonicalizePath(&out[i], AFX_OS_WIN);
                            //AfxLogEcho(dstData);

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

            //AfxLogAdvertence("%x %x %x %x %x", st.st_mode & _S_IFREG, st.st_mode & _S_IFDIR, st.st_mode & _S_IREAD, st.st_mode & _S_IWRITE, st.st_mode & _S_IEXEC);
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

_AFX afxNat AfxFindStorageUnit(afxStorage fsys, afxUri const* endpoint, afxFileFlags ioFlags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);
    AfxAssert(endpoint);
    AfxAssert(ioFlags);
    afxNat fstoIdx = AFX_INVALID_INDEX;

    afxNat i = 0;
    afxStorageUnit const* fsto;
    AfxIterateLinkage(afxStorageUnit, fsto, &fsys->storages, fsys)
    {
        if (0 == AfxCompareUri(endpoint, &fsto->rootPath))
        {
            if ((fsto->flags & ioFlags) == ioFlags)
            {
                fstoIdx = i;
                break;
            }
        }
        ++i;
        fsys = NIL;
    }
    return fstoIdx;
}

_AFX afxError _DismountStorageUnit(afxStorage fsys, afxStorageUnit* fsto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(fsto);

    //AfxLogEcho("Dismounting storage unit #u... <%.*s>('%.*s'),%x", AfxGetObjectId(fsys), 2,"  "/*AfxPushString(AfxGetUriString(&fsys->point.uri))*/, 2, "  "/*AfxPushString(AfxGetUriString(&fsto->rootPath))*/, fsto->flags);

    AfxPopLinkage(&fsto->fsys);

    if (fsto->arc)
        AfxReleaseObjects(1, (void*[]) { fsto->arc });

    AfxDeallocateUri(&fsto->rootPath);
    AfxDeallocate(fsto);
    return err;
}

_AFX afxError _MountStorageUnit(afxStorage fsys, afxUri const* endpoint, afxFileFlags fileFlags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);
    AfxAssert(fileFlags);

    afxUri2048 endpoint2;
    AfxMakeUri2048(&endpoint2, NIL);

    afxArchive arc = NIL;

    if (AfxPathIsRelative(endpoint))
    {
        //AfxPickUriPath(&location2, &endpoint);
        afxUri archiveNameExtUri;

        if (AfxPickUriExtension(endpoint, FALSE, &archiveNameExtUri))
        {
            fileFlags |= afxFileFlag_D;

            if (!(arc = AfxOpenArchive(endpoint, fileFlags, NIL))) AfxThrowError();
            else
            {
                AfxCopyUriPath(&endpoint2.uri, endpoint);
            }
        }
        else
        {
            afxUri const *rootUri = AfxGetSystemDirectory(NIL);
            afxString const *rootUriString = AfxGetSystemDirectoryString(NIL);
            //AfxFormatString(&location2.uri.str, "%.*s"
            AfxFormatUri(&endpoint2.uri, "%.*s"
#ifdef AFX_OS_WIN
                "\\"
#else
                "/"
#endif
                "%.*s"
#ifdef AFX_OS_WIN
                "\\"
#else
                "/"
#endif
                , AfxPushString(rootUriString), AfxPushString(AfxGetUriString(endpoint)));
            AfxReparseUri(&endpoint2.uri);
            AfxCanonicalizePath(&endpoint2.uri, AFX_OS_WIN);
        }
    }
    else
    {
        AfxCanonicalizePath(&endpoint2.uri, AFX_OS_WIN);

        afxChar const *pathRaw = AfxGetUriData(&endpoint2.uri, 0);
        struct stat st;

        if (stat(pathRaw, &(st))) AfxThrowError();
        else
        {
            unsigned short flagsToTest = NIL;
            afxIoFlags ioPerms = fileFlags & AFX_IO_PERM_MASK;

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

                if (!isDir) AfxThrowError();
                else
                {

                }
            }
        }
    }

    if (!err)
    {
        AfxLogEcho("Mounting storage unit... <%.*s>('%.*s'),%x", AfxPushString(AfxGetUriString(&fsys->point.uri)), AfxPushString(AfxGetUriString(&endpoint2.uri)), fileFlags);

        afxStorageUnit* fsto = AfxAllocate(1, sizeof(*fsto), 0, AfxHere());

        if (!fsto) AfxThrowError();
        else
        {
            AfxPushLinkage(&fsto->fsys, &fsys->storages);

            AfxDuplicateUriPath(&fsto->rootPath, &endpoint2.uri);

            fsto->flags = fileFlags;
            fsto->arc = arc;

            if (err)
                AfxDeallocateUri(&fsto->rootPath);
        }
    }


    if (err && arc)
        AfxReleaseObjects(1, (void*[]) { arc });

    return err;
}

_AFX afxError _AfxFsysCtor(afxStorage fsys, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);

    afxUri const* point = cookie->udd[0];
    afxUri const* endpoint = cookie->udd[1];
    afxFileFlags ioFlags = *(afxFileFlags const*)cookie->udd[2];
    AfxAssert(ioFlags);
    afxArchive arc = cookie->udd[3];
    
    AfxAssert(ioFlags);
    AfxAssert(ioFlags & AFX_IO_PERM_MASK);

    AfxMakeUri8(&fsys->point, point);
    AfxSetUpChain(&fsys->storages, fsys);

    if (_MountStorageUnit(fsys, endpoint, ioFlags)) AfxThrowError();
    else
    {
        
    }

    return err;
}

_AFX afxError _AfxFsysDtor(afxStorage fsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);
    
    afxStorageUnit* fsto;
    AfxIterateLinkage(afxStorageUnit, fsto, &fsys->storages, fsys)
    {
        _DismountStorageUnit(fsys, fsto);
    }
    
    AfxAssert(AfxChainIsEmpty(&fsys->storages));

    return err;
}

_AFX afxClassConfig const _AfxFsysMgrCfg =
{
    .fcc = afxFcc_FSYS,
    .name = "Storage",
    .desc = "I/O Storage System",
    .unitsPerPage = 6,
    .size = sizeof(AFX_OBJECT(afxStorage)),
    .ctor = (void*)_AfxFsysCtor,
    .dtor = (void*)_AfxFsysDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxMountStorageUnit(afxUri const* point, afxUri const* endpoint, afxFileFlags ioFlags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(point);
    AfxAssert(endpoint);
    AfxAssert(ioFlags);
    afxStorage fsys = NIL;

    afxUri point2;

    if (point->dir)
        AfxClipUriDirectory(&point2, point);
    else
        AfxClipUriObject(&point2, point);

    if (!(fsys = AfxFindStorage(&point2)))
    {
        afxManager* cls = AfxGetStorageClass();
        AfxAssertClass(cls, afxFcc_FSYS);

        if (AfxAcquireObjects(cls, 1, (afxObject*)&fsys, (void const*[]) { &point2, endpoint, &ioFlags, NIL }))
            AfxThrowError();
    }
    else
    {
        AfxAssert(!err);

        if (_MountStorageUnit(fsys, endpoint, ioFlags))
            AfxThrowError();
    }

    return err;
}

_AFX afxError AfxDismountStorageUnit(afxUri const* point, afxUri const* endpoint, afxFileFlags ioFlags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(point);
    AfxAssert(endpoint);
    AfxAssert(ioFlags);
    afxStorage fsys;

    if (!(fsys = AfxFindStorage(point))) AfxThrowError();
    else
    {
        afxStorageUnit* fsto;
        AfxIterateLinkage(afxStorageUnit, fsto, &fsys->storages, fsys)
        {
            if (0 == AfxCompareUri(endpoint, &fsto->rootPath))
            {
                if ((fsto->flags & ioFlags) == ioFlags)
                {
                    if (_DismountStorageUnit(fsys, fsto))
                        AfxThrowError();

                    break;
                }
            }
        }
    }
    return err;
}

_AFX afxStorage AfxFindStorage(afxUri const* point)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(point);
    afxStorage fsys = NIL;

    afxNat i = 0;
    while (AfxEnumerateStorages(i, 1, &fsys))
    {
        if (0 == AfxCompareUri(point, &fsys->point.uri))
            break;

        ++i;
        fsys = NIL;
    }
    AfxTryAssertObjects(1, &fsys, afxFcc_FSYS);
    return fsys;
}

_AFX afxNat AfxInvokeStorages(afxNat first, afxNat cnt, afxBool(*f)(afxStorage, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxManager* cls = AfxGetStorageClass();
    AfxAssertClass(cls, afxFcc_FSYS);
    return AfxInvokeObjects(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateStorages(afxNat first, afxNat cnt, afxStorage systems[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(systems);
    afxManager* cls = AfxGetStorageClass();
    AfxAssertClass(cls, afxFcc_FSYS);
    return AfxEnumerateObjects(cls, first, cnt, (afxObject*)systems);
}

_AFX afxNat AfxCountStorages(void)
{
    afxError err = AFX_ERR_NONE;
    afxManager* cls = AfxGetStorageClass();
    AfxAssertClass(cls, afxFcc_FSYS);
    return AfxCountObjects(cls);
}
