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
#include "../dev/afxIoImplKit.h"

extern afxClassConfig const _AfxFileClsCfg;
extern afxClassConfig const _AfxArcClsCfg;
extern afxClassConfig const _AfxUrdClsCfg;

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

_AFX afxError AfxForEachUriResolution(afxUri const* pattern, afxFileFlags flags, afxBool(*proc)(void* udd, afxUri const* uri), void* udd)
{
    afxError err = AFX_ERR_NONE;

    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);

    AfxAssert(pattern);
    AfxAssert(flags);
    AfxAssert(proc);

    AfxAssert(!AfxUriIsBlank(pattern));

    // diskIds[i] = AFX_INVALID_INDEX;

    afxFileFlags ioPerms = flags ? flags & AFX_FILE_PERM_MASK : afxFileFlag_RX;
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

    afxString const *srcStr = AfxGetUriString(pattern);
    afxUri2048 out;
    AfxMakeUri2048(&out, NIL);
    afxString const *dstStr = AfxGetUriString(&out.uri);
    afxChar const* srcData = AfxGetStringData(srcStr, 0);
    afxChar const* dstData = AfxGetStringStorage(dstStr, 0);

    afxUri dev, path;
    AfxSplitPath(pattern, &dev, &path);

    if (!dev.root)
    {
        AfxCopyUri(&out.uri, pattern);

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

        if (hasWildcard)
        {
            afxStorage fsys;

            if (AfxFindStorage(&dev, &fsys))
            {
                AfxAssertObjects(1, &fsys, afxFcc_FSYS);

                afxStorageUnit const* fsto;
                AfxIterateLinkage(afxStorageUnit, fsto, &fsys->storages, fsys)
                {
                    if ((fsto->flags & ioPerms) == ioPerms)
                    {
                        HANDLE fh;
                        WIN32_FIND_DATAA wfd;

                        AfxFormatUri(&out.uri, "%.*s/%.*s\0", AfxPushString(AfxGetUriString(&fsto->rootPath)), AfxPushString(pathStr));
                        AfxCanonicalizePath(&out.uri, AFX_OS_WIN);

                        if (INVALID_HANDLE_VALUE != (fh = FindFirstFileA(AfxGetUriData(&out.uri, 0), &(wfd))))
                        {
                            do
                            {
                                afxUri found;
                                AfxMakeUri(&found, 0, wfd.cFileName, 0);

                                if (!proc(udd, &found))
                                    break;

                            } while (FindNextFileA(fh, &wfd));
                            FindClose(fh);
                        }
                    }
                }
            }
        }
        else
        {
            afxStorage fsys;

            if (AfxFindStorage(&dev, &fsys))
            {
                AfxAssertObjects(1, &fsys, afxFcc_FSYS);

                afxStorageUnit const* fsto;
                AfxIterateLinkage(afxStorageUnit, fsto, &fsys->storages, fsys)
                {
                    if ((fsto->flags & ioPerms) == ioPerms)
                    {
                        AfxFormatUri(&out.uri, "%.*s/%.*s\0", AfxPushString(AfxGetUriString(&fsto->rootPath)), AfxPushString(pathStr));
                        AfxCanonicalizePath(&out.uri, AFX_OS_WIN);
                        //AfxLogEcho(dstData);

                        if (!stat(dstData, &(st)) || (ioPerms & afxFileFlag_W) || hasWildcard)
                        {
                            if (((st.st_mode & flagsToTest) == flagsToTest) || ioPerms & afxFileFlag_W || hasWildcard)
                            {
                                resolved = TRUE;
                                //diskIds[i] = AfxGetObjectId(fsys);



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
    
    return err;
}

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
                
            if (AfxFindStorage(&dev, &fsys))
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

_AFX afxNat AfxFindFiles(afxUri const* pattern, afxFileFlags flags, afxBool(*callback)(void* udd, afxUri const* path, afxUri const* osPath), void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(pattern);
    AfxAssert(callback);
    afxNat rslt = 0;

    /*
    "art://./actor/";
    "//./art/actor/";
    "system://./e2draw.dll";
    "//./system/e2draw.dll";
    */

    afxFileFlags ioPerms = flags ? flags & AFX_FILE_PERM_MASK : afxFileFlag_RX;
    afxBool resolved = FALSE;

    unsigned short flagsToTest = NIL;

    if (AfxTestFlags(ioPerms, afxFileFlag_R))
        flagsToTest |= _S_IREAD;

    if (AfxTestFlags(ioPerms, afxFileFlag_W))
        flagsToTest |= _S_IWRITE;

    afxUri dev, path;
    AfxSplitPath(pattern, &dev, &path);
    afxStorage disk;

    if (AfxUriIsBlank(&dev) || !(AfxFindStorage(&dev, &disk)))
        disk = NIL;

    afxBool hasWildcard = (AFX_INVALID_INDEX != AfxFindFirstChar(&path.str, 0, '*'));

    afxUri2048 resolvedUrl, baseUrl;
    AfxMakeUri2048(&resolvedUrl, NIL);
    AfxMakeUri2048(&baseUrl, NIL);

    afxUri dir;
    AfxClipPathDirectory(&dir, &path);

    if (disk)
    {
        afxStorageUnit const* fsto;
        AfxIterateLinkage(afxStorageUnit, fsto, &disk->storages, fsys)
        {
            if ((fsto->flags & ioPerms) == ioPerms)
            {
                AfxFormatUri(&resolvedUrl.uri, "%.*s/%.*s", AfxPushString(AfxGetUriString(&fsto->rootPath)), AfxPushString(&path.str));
                AfxCanonicalizePath(&resolvedUrl.uri, AFX_OS_WIN);

                if (hasWildcard)
                {
                    HANDLE fh;
                    WIN32_FIND_DATAA wfd;

                    if (INVALID_HANDLE_VALUE != (fh = FindFirstFileA(AfxGetUriData(&resolvedUrl.uri, 0), &(wfd))))
                    {
                        do
                        {
                            afxUri found;
                            AfxMakeUri(&found, 0, wfd.cFileName, 0);

                            if (dev.str.len && dir.str.len)
                                AfxFormatUri(&baseUrl.uri, "%.*s/%.*s/%.*s", AfxPushString(&dev.str), AfxPushString(&dir.str), AfxPushString(&found.str));
                            else if (dev.str.len)
                                AfxFormatUri(&baseUrl.uri, "%.*s/%.*s", AfxPushString(&dev.str), AfxPushString(&found.str));
                            else if (dir.str.len)
                                AfxFormatUri(&baseUrl.uri, "%.*s/%.*s", AfxPushString(&dir.str), AfxPushString(&found.str));
                            else
                                AfxFormatUri(&baseUrl.uri, "%.*s", AfxPushString(&found.str));

                            AfxFormatUri(&resolvedUrl.uri, "%.*s/%.*s/%.*s", AfxPushString(AfxGetUriString(&fsto->rootPath)), AfxPushString(&dir.str), AfxPushString(&found.str));
                            AfxCanonicalizePath(&resolvedUrl.uri, AFX_OS_WIN);

                            resolved = TRUE;
                            rslt++;

                            if (!callback(udd, &baseUrl.uri, &resolvedUrl.uri))
                                break;

                        } while (FindNextFileA(fh, &wfd));
                        FindClose(fh);
                    }
                }
                else
                {
                    struct stat st;

                    if (!stat(resolvedUrl.uri.str.start, &(st)) || (ioPerms & afxFileFlag_W) || hasWildcard)
                    {
                        if (((st.st_mode & flagsToTest) == flagsToTest) || ioPerms & afxFileFlag_W || hasWildcard)
                        {
                            if (dev.str.len && path.str.len)
                                AfxFormatUri(&baseUrl.uri, "%.*s/%.*s", AfxPushString(&dev.str), AfxPushString(&path.str));
                            else
                                AfxFormatUri(&baseUrl.uri, "%.*s", AfxPushString(&path.str));

                            resolved = TRUE;
                            rslt++;

                            if (!callback(udd, &baseUrl.uri, &resolvedUrl.uri))
                                break;
                        }
                    }
                }
            }

            if (resolved)
                break;
        }
    }
    else
    {
        afxUri2048 szPattern;
        AfxMakeUri2048(&szPattern, pattern);
        AfxCanonicalizePath(&szPattern.uri, AFX_OS_WIN);

        HANDLE fh;
        WIN32_FIND_DATAA wfd;

        if (INVALID_HANDLE_VALUE != (fh = FindFirstFileA(AfxGetUriData(&szPattern.uri, 0), &wfd)))
        {
            do
            {
                afxUri found;
                AfxMakeUri(&found, 0, wfd.cFileName, 0);

                if (dev.str.len && dir.str.len)
                    AfxFormatUri(&baseUrl.uri, "%.*s/%.*s/%.*s", AfxPushString(&dev.str), AfxPushString(&dir.str), AfxPushString(&found.str));
                else if (dev.str.len)
                    AfxFormatUri(&baseUrl.uri, "%.*s/%.*s", AfxPushString(&dev.str), AfxPushString(&found.str));
                else if (dir.str.len)
                    AfxFormatUri(&baseUrl.uri, "%.*s/%.*s", AfxPushString(&dir.str), AfxPushString(&found.str));
                else
                    AfxFormatUri(&baseUrl.uri, "%.*s", AfxPushString(&found.str));

                if (dev.str.len && dir.str.len)
                    AfxFormatUri(&resolvedUrl.uri, "%.*s/%.*s/%.*s", AfxPushString(&dev.str), AfxPushString(&dir.str), AfxPushString(&found.str));
                else if (dev.str.len)
                    AfxFormatUri(&resolvedUrl.uri, "%.*s/%.*s", AfxPushString(&dev.str), AfxPushString(&found.str));
                else if (dir.str.len)
                    AfxFormatUri(&resolvedUrl.uri, "%.*s/%.*s", AfxPushString(&dir.str), AfxPushString(&found.str));
                else
                    AfxFormatUri(&resolvedUrl.uri, "%.*s", AfxPushString(&found.str));

                AfxCanonicalizePath(&resolvedUrl.uri, AFX_OS_WIN);

                rslt++;

                if (!callback(udd, &found, &resolvedUrl.uri))
                    break;

            } while (FindNextFileA(fh, &wfd));
            FindClose(fh);
        }
    }
    return rslt;
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
    AfxDeployChain(&fsys->fileChain, fsys);

    if (_MountStorageUnit(fsys, endpoint, ioFlags)) AfxThrowError();
    else
    {
        //AfxRegisterClass(&fsys->fileCls, (afxClass*)AfxGetStreamClass(), &fsys->classes, &_AfxFileClsCfg);
        AfxRegisterClass(&fsys->archCls, NIL, &fsys->classes, &_AfxArcClsCfg); // require iob
        AfxRegisterClass(&fsys->urdCls, NIL, &fsys->classes, &_AfxUrdClsCfg);

    }

    return err;
}

_AFX afxError _AfxFsysDtorCb(afxStorage fsys)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fsys, afxFcc_FSYS);

    AfxDeregisterChainedClasses(&fsys->classes);

    afxStorageUnit* fsto;
    AfxIterateLinkage(afxStorageUnit, fsto, &fsys->storages, fsys)
    {
        _DismountStorageUnit(fsys, fsto);
    }
    
    AfxAssert(AfxChainIsEmpty(&fsys->storages));
    AfxAssert(AfxChainIsEmpty(&fsys->fileChain));

    return err;
}

_AFX afxClassConfig const _AfxFsysMgrCfg =
{
    .fcc = afxFcc_FSYS,
    .name = "Storage",
    .desc = "Data Storage Device",
    .fixedSiz = sizeof(AFX_OBJECT(afxStorage)),
    .ctor = (void*)_AfxFsysCtorCb,
    .dtor = (void*)_AfxFsysDtorCb
};

////////////////////////////////////////////////////////////////////////////////

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

_AFX afxError AfxMountStorageUnit(afxChar diskId, afxUri const* endpoint, afxFileFlags ioFlags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert((diskId >= 'a' && diskId <= 'z') || (diskId >= 'A' && diskId <= 'Z'));
    AfxAssert(endpoint);
    AfxAssert(ioFlags);
    afxStorage fsys = NIL;

    if (!AfxGetStorage(diskId, &fsys))
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
    
    if (!AfxGetStorage(diskId, &fsys)) AfxThrowError();
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

_AFX afxBool AfxGetStorage(afxChar diskId, afxStorage* disk)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    AfxAssert((diskId >= 'a' && diskId <= 'z') || (diskId >= 'A' && diskId <= 'Z'));
    diskId = (afxChar)AfxTolower(diskId);

    afxNat i = 0;
    afxStorage fsys;
    while (AfxEnumerateStorages(i, 1, &fsys))
    {
        AfxAssertObjects(1, &fsys, afxFcc_FSYS);

        if (fsys->diskId == diskId)
        {
            AfxAssert(disk);
            *disk = fsys;
            rslt = TRUE;
            break;
        }
        ++i;
    }
    return rslt;
}

_AFX afxBool AfxFindStorage(afxUri const* uri, afxStorage* disk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(uri);
    afxBool rslt = FALSE;

    afxUri dev, path;
    AfxSplitPath(uri, &dev, &path);

    if (!AfxUriIsBlank(&dev))
    {
        afxNat i = 0;
        afxStorage fsys;
        while (AfxEnumerateStorages(i, 1, &fsys))
        {
            AfxAssertObjects(1, &fsys, afxFcc_FSYS);

            if (0 == AfxCompareStringRangeCi(&dev.str, 0, &fsys->baseUrl.uri.str, 5))
            {
                AfxAssert(disk);
                *disk = fsys;
                rslt = TRUE;
                break;
            }
            ++i;
        }
    }
    return rslt;
}
