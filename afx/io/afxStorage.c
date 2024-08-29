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

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <sys/stat.h>
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_STORAGE_C
#include "../dev/afxDevIoBase.h"

extern afxClassConfig const _AfxIosClsCfg;
//extern afxClassConfig const _AfxFileClsCfg;
extern afxClassConfig const _AfxArcClsCfg;
extern afxClassConfig const _AfxUrdClsCfg;

_AFX afxClass const* AfxGetStreamClass(afxStorage fsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);
    afxClass const* cls = &fsys->iosCls;
    AfxAssertClass(cls, afxFcc_IOB);
    return cls;
}

_AFX afxClass const* AfxGetFileClass(afxStorage fsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);
    afxClass const* cls = &fsys->fileCls;
    AfxAssertClass(cls, afxFcc_FILE);
    return cls;
}

_AFX afxClass const* AfxGetArchiveClass(afxStorage fsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);
    afxClass const* cls = &fsys->archCls;
    AfxAssertClass(cls, afxFcc_ARC);
    return cls;
}

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

_AFX afxError AfxResolveUris2(afxFileFlags const permissions, afxNat cnt, afxUri const in[], afxUri out[], afxNat diskIds[])
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

        diskIds[i] = AFX_INVALID_INDEX;

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
        afxChar const* dstData = AfxGetStringStorage(dstStr, 0);

        afxUri dev, path;
        AfxSplitPath(&in[i], &dev, &path);

        if (!dev.root)
        {
            AfxCopyUri(&out[i], &in[i]);

            if (stat(dstData, &(st)))
                AfxThrowError();
            else
            {
                if (!((st.st_mode & flagsToTest) == flagsToTest))
                    AfxThrowError();
            }
            resolved = TRUE;
        }
        else
        {
            afxString const *pathStr = AfxGetUriString(&path);

            srcData = AfxGetStringData(pathStr, 0);

            afxBool hasWildcard = (AFX_INVALID_INDEX != AfxFindFirstChar(pathStr, 0, '*'));

            afxStorage fsys;
                
            if ((fsys = AfxFindStorage2(&dev)))
            {
                AfxAssertObjects(1, &fsys, afxFcc_FSYS);

                afxStorageUnit const* fsto;
                AfxIterateLinkage(afxStorageUnit, fsto, &fsys->storages, fsys)
                {
                    if ((fsto->flags & ioPerms) == ioPerms)
                    {
                        AfxFormatUri(&out[i], "%.*s/%.*s\0", AfxPushString(AfxGetUriString(&fsto->rootPath)), AfxPushString(pathStr));
                        AfxCanonicalizePath(&out[i], AFX_OS_WIN);
                        //AfxLogEcho(dstData);

                        if (!stat(dstData, &(st)) || (ioPerms & afxFileFlag_W) || hasWildcard)
                        {
                            if (((st.st_mode & flagsToTest) == flagsToTest) || ioPerms & afxFileFlag_W || hasWildcard)
                            {
                                resolved = TRUE;
                                diskIds[i] = AfxGetObjectId(fsys);
                                break;
                            }
                        }
                    }
                }
            }

            if (!resolved)
                AfxThrowError(); // unresolved.

            //AfxLogAdvertence("%x %x %x %x %x", st.st_mode & _S_IFREG, st.st_mode & _S_IFDIR, st.st_mode & _S_IREAD, st.st_mode & _S_IWRITE, st.st_mode & _S_IEXEC);
        }

  //      if (!resolved)
//            AfxCopyUri(&out[i], &in[i]);
    }
    return err;
}

_AFX afxError AfxResolveUris(afxFileFlags const permissions, afxNat cnt, afxUri const in[], afxUri out[])
{
    afxNat diskId = AFX_INVALID_INDEX;
    afxNat diskIds[256];
    return AfxResolveUris2(permissions, cnt, in, out, diskIds);
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

_AFX afxError AfxResolveUri2(afxFileFlags permissions, afxUri const *in, afxUri *out, afxNat* diskId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(in);
    AfxAssert(out);
    AfxAssert(!AfxUriIsBlank(in));

    if (AfxResolveUris2(permissions, 1, in, out, diskId))
        AfxThrowError();

    return err;
}

_AFX afxError AfxFindFiles(afxUri const* pattern, afxBool(*callback)(afxUri const*, void*), void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(pattern);
    AfxAssert(callback);
    afxUri2048 pathBuf;
    AfxMakeUri2048(&pathBuf, NIL);
    AfxResolveUri(afxFileFlag_RX, pattern, &pathBuf.uri);
    /*
    "art://./actor/";
    "//./art/actor/";
    "system://./e2draw.dll";
    "//./system/e2draw.dll";
    */
    HANDLE fh;
    WIN32_FIND_DATAA wfd;

    if ((fh = FindFirstFileA(AfxGetUriData(&pathBuf.uri, 0), &(wfd))))
    {
        do
        {
            afxUri found;
            AfxMakeUri(&found, 0, wfd.cFileName, 0);

            if (!callback(&found, udd))
                break;

        } while (FindNextFileA(fh, &wfd));
        FindClose(fh);
    }
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

    AfxLogEcho("Dismounting... <%.*s>('%.*s'),%x @u", AfxPushString(AfxGetUriString(&fsys->baseUrl.uri)), AfxPushString(AfxGetUriString(&fsto->rootPath)), fsto->flags, AfxGetObjectId(fsys));

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
        //AfxClipUriPath(&endpoint, &location2);
        afxUri archiveNameExtUri;

        if (AfxClipUriExtension(&archiveNameExtUri, endpoint, FALSE))
        {
            fileFlags |= afxFileFlag_D;

            if (!(arc = AfxOpenArchive(endpoint, fileFlags, NIL))) AfxThrowError();
            else
            {
                afxUri path;
                AfxClipUriPath(&path, endpoint);
                AfxCopyUri(&endpoint2.uri, &path);
            }
        }
        else
        {
            if (AfxUriIsBlank(endpoint))
            {

            }
            else
            {
                afxUri const *rootUri = AfxGetSystemDirectory(NIL);
                afxString const *rootUriString = AfxGetSystemDirectoryString(NIL);
                //AfxFormatString(&location2.uri.str, "%.*s"
#if 0
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
#else
                AfxFormatUri(&endpoint2.uri, "%.*s%.*s"
#endif
                    , AfxPushString(rootUriString), AfxPushString(AfxGetUriString(endpoint)));
                AfxReparseUri(&endpoint2.uri);
                AfxCanonicalizePath(&endpoint2.uri, AFX_OS_WIN);
            }
        }
    }
    else
    {
        AfxMakeUri2048(&endpoint2, endpoint);
        AfxTransformPathString(&endpoint2.uri.str, AFX_OS_WIN);
        AfxReparseUri(&endpoint2.uri);

        afxChar const *pathRaw = AfxGetUriData(&endpoint2.uri, 0);
        struct stat st;

        if (stat(pathRaw, &(st)))
            AfxThrowError();
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
        AfxLogEcho("Mounting... <%.*s>('%.*s'),%x", AfxPushString(AfxGetUriString(&fsys->baseUrl.uri)), AfxPushString(AfxGetUriString(&endpoint2.uri)), fileFlags);

        afxStorageUnit* fsto = AfxAllocate(1, sizeof(*fsto), 0, AfxHere());

        if (!fsto) AfxThrowError();
        else
        {
            AfxPushLinkage(&fsto->fsys, &fsys->storages);

            AfxDuplicateUri(&fsto->rootPath, &endpoint2.uri);

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

_AFX afxError _AfxFsysCtorCb(afxStorage fsys, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);

    afxChar diskId = *(afxChar const*)args[0];
    diskId = (afxChar)AfxTolower(diskId);

    afxUri const* endpoint = args[1];
    afxFileFlags ioFlags = *(afxFileFlags const*)args[2];
    AfxAssert(ioFlags);
    afxArchive arc = args[3];
    
    AfxAssert(ioFlags);
    AfxAssert(ioFlags & AFX_IO_PERM_MASK);

    AfxMakeUri8(&fsys->baseUrl, NIL);
    AfxFormatUri(&fsys->baseUrl.uri, "//./%c/", diskId);
    fsys->diskId = diskId;

    AfxDeployChain(&fsys->storages, fsys);
    AfxDeployChain(&fsys->classes, fsys);

    if (_MountStorageUnit(fsys, endpoint, ioFlags)) AfxThrowError();
    else
    {
        AfxRegisterClass(&fsys->iosCls, NIL, &fsys->classes, &_AfxIosClsCfg);
        AfxRegisterClass(&fsys->archCls, NIL, &fsys->classes, &_AfxArcClsCfg); // require iob
        AfxRegisterClass(&fsys->urdCls, NIL, &fsys->classes, &_AfxUrdClsCfg);

    }

    return err;
}

_AFX afxError _AfxFsysDtorCb(afxStorage fsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);

    AfxCleanUpChainedClasses(&fsys->classes);

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
    .fixedSiz = sizeof(AFX_OBJECT(afxStorage)),
    .ctor = (void*)_AfxFsysCtorCb,
    .dtor = (void*)_AfxFsysDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxMountStorageUnit(afxChar diskId, afxUri const* endpoint, afxFileFlags ioFlags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert((diskId >= 'a' && diskId <= 'z') || (diskId >= 'A' && diskId <= 'Z'));
    AfxAssert(endpoint);
    AfxAssert(ioFlags);
    afxStorage fsys = NIL;

    if (!(fsys = AfxFindStorage(diskId)))
    {
        afxClass* cls = AfxGetStorageClass();
        AfxAssertClass(cls, afxFcc_FSYS);

        if (AfxAcquireObjects(cls, 1, (afxObject*)&fsys, (void const*[]) { &diskId, endpoint, &ioFlags, NIL }))
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

_AFX afxError AfxDismountStorageUnit(afxChar diskId, afxUri const* endpoint, afxFileFlags ioFlags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert((diskId >= 'a' && diskId <= 'z') || (diskId >= 'A' && diskId <= 'Z'));
    AfxAssert(endpoint);
    AfxAssert(ioFlags);
    afxStorage fsys;
    
    if (!(fsys = AfxFindStorage(diskId)))
        AfxThrowError();
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

_AFX afxBool AfxGetStorage(afxNat diskNo, afxStorage* disk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(diskNo != AFX_INVALID_INDEX);
    afxStorage fsys = NIL;
    while (AfxEnumerateStorages(diskNo, 1, &fsys))
    {
        AfxAssertObjects(1, &fsys, afxFcc_FSYS);
        AfxAssert(disk);
        *disk = fsys;
        break;
    }
    return !!fsys;
}

_AFX afxStorage AfxFindStorage(afxChar diskId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert((diskId >= 'a' && diskId <= 'z') || (diskId >= 'A' && diskId <= 'Z'));
    afxStorage fsys = NIL;

    diskId = (afxChar)AfxTolower(diskId);

    afxNat i = 0;
    while (AfxEnumerateStorages(i, 1, &fsys))
    {
        if (fsys->diskId == diskId)
            break;

        ++i;
        fsys = NIL;
    }
    AfxTryAssertObjects(1, &fsys, afxFcc_FSYS);
    return fsys;
}

_AFX afxStorage AfxFindStorage2(afxUri const* dev)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(dev);
    afxStorage fsys = NIL;

    afxNat i = 0;
    while (AfxEnumerateStorages(i, 1, &fsys))
    {
        if (0 == AfxCompareStringRangeCi(&dev->str, 0, &fsys->baseUrl.uri.str, 5))
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
    afxClass* cls = AfxGetStorageClass();
    AfxAssertClass(cls, afxFcc_FSYS);
    return AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateStorages(afxNat first, afxNat cnt, afxStorage systems[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(systems);
    afxClass* cls = AfxGetStorageClass();
    AfxAssertClass(cls, afxFcc_FSYS);
    return AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)systems);
}
