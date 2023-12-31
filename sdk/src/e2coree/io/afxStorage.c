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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#include <sys/stat.h>
#include <stdio.h>

#define _AFX_CORE_C
#define _AFX_STORAGE_C
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
        AfxAssertType(&in[i], afxFcc_URI);
        AfxAssertType(&out[i], afxFcc_URI);
        AfxAssert(!AfxUriIsBlank(&in[i]));

        afxFileFlags ioPerms = permissions ? permissions & AFX_FILE_PERM_MASK : AFX_FILE_FLAG_RX;
        afxBool resolved = FALSE;
        struct stat st;

        unsigned short flagsToTest = NIL;

        if (AfxFlagsTest(ioPerms, AFX_FILE_FLAG_R))
            flagsToTest |= _S_IREAD;

        if (AfxFlagsTest(ioPerms, AFX_FILE_FLAG_W))
            flagsToTest |= _S_IWRITE;

        //if (AfxFlagsTest(ioPerms, afxIoFlag_X))
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
            AfxGetUriPath(&path, &in[i]);
            AfxGetUriDirectory(&dir, &in[i]);
            AfxGetUriObject(&file, &in[i]);
            afxString const *pathStr = AfxGetUriString(&path);
            afxString const *dirStr = AfxGetUriString(&dir);
            afxString const *fileStr = AfxGetUriString(&file);

            srcData = AfxGetStringData(pathStr, 0);

            afxChar const *pointStart = srcData;

            afxChar const *pointEnd = AfxFindCharRaw(pointStart, '/');

            afxString point;
            AfxMakeString(&point, pointStart, pointEnd ? pointEnd - pointStart : AfxGetStringLength(pathStr));
            afxString const *pointStr = &point;

            if (pointEnd && *pointEnd == '/')
                pointEnd++; // skip bar

            afxNat pointEndRange = pointEnd ? AfxGetStringLength(pathStr) - (pointEnd - pointStart) : AfxGetStringLength(pathStr);


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
                afxNat j = 0;
                afxStorage sto;
                while (AfxEnumerateStorages(j, 1, &sto))
                {
                    AfxAssertObjects(1, &sto, afxFcc_STO);

                    if ((sto->flags & ioPerms) == ioPerms)
                    {
                        if (0 == AfxCompareString(&point, AfxGetUriString(&sto->name.uri)))
                        {
                            AfxFormatUri(&out[i], "%.*s/%.*s\0", AfxPushString(AfxGetUriString(&sto->path)), pointEndRange, pointEnd);
                            AfxEcho(dstData);

                            if (!stat(dstData, &(st)) || (ioPerms & AFX_FILE_FLAG_W))
                            {
                                if (((st.st_mode & flagsToTest) == flagsToTest) || ioPerms & AFX_FILE_FLAG_W)
                                {
                                    resolved = TRUE;
                                    break;
                                }
                            }
                        }
                    }
                    ++j;
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
    AfxAssertType(in, afxFcc_URI);
    AfxAssertType(out, afxFcc_URI);
    AfxAssert(!AfxUriIsBlank(in));

    if (AfxResolveUris(permissions, 1, in, out))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxStoCtor(afxStorage sto, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sto, afxFcc_STO);

    afxStoragePointSpecification const* spec = ((afxStoragePointSpecification const*)cookie->udd[0]) + cookie->no;
    AfxAssert(spec);
    AfxAssert(spec->perm & AFX_IO_PERM_MASK);

    afxUri path;
    AfxGetUriPath(&path, spec->hostPath);

    afxFixedUri2048 bufUri;
    AfxMakeFixedUri2048(&bufUri, NIL);

    if (AfxPathIsRelative(&path))
    {
        afxUri const *rootUri = AfxGetSystemRootPath(NIL);
        afxString const *rootUriString = AfxGetSystemRootPathString(NIL);
        AfxFormatUri(&bufUri.uri, "%.*s/%.*s", AfxPushString(rootUriString), AfxPushString(AfxGetUriString(&path)));
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
        afxIoFlags ioPerms = spec->perm & AFX_IO_PERM_MASK;

        if (AfxFlagsTest(ioPerms, afxIoFlag_R))
            flagsToTest |= _S_IREAD;

        if (AfxFlagsTest(ioPerms, afxIoFlag_W))
            flagsToTest |= _S_IWRITE;

        if (AfxFlagsTest(ioPerms, afxIoFlag_X))
            flagsToTest |= _S_IEXEC;

        if (!((st.st_mode & flagsToTest) == flagsToTest)) AfxThrowError();
        else
        {
            afxBool isDir = !!(st.st_mode & _S_IFDIR);
            afxBool isFile = !!(st.st_mode & _S_IFREG);
            afxArchive arc = NIL;

            if (!isDir && (isFile && AfxAcquireArchives(1, &arc, &bufUri.uri, (afxFileFlags[]) { ioPerms | AFX_FILE_FLAG_D }))) AfxThrowError();
            else
            {
                AfxMakeFixedUri8(&sto->name, spec->namespace);

                //if (!(fssp->name)) AfxThrowError();
                //else
                {
                    AfxCloneUriPath(&sto->path, &bufUri.uri);

                    //if (!(fssp->path)) AfxThrowError();
                    //else
                    {
                        sto->flags = ioPerms;
                        sto->arc = arc;

                        AfxEcho("Storage point \"%.*s\" %x mounted -> \"%.*s\".", AfxPushString(AfxGetUriString(&sto->name.uri)), sto->flags, AfxPushString(AfxGetUriString(&sto->path)));

                        if (err)
                            AfxDeallocateUri(&sto->path);
                    }
                }

                if (err && arc)
                    AfxReleaseObjects(1, (void*[]) { arc });
            }
        }
    }

    return err;
}

_AFX afxError _AfxStoDtor(afxStorage sto)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sto, afxFcc_STO);

    AfxEcho("Storage point \"%.*s\" %x unmounted -> \"%.*s\".", AfxPushString(AfxGetUriString(&sto->name.uri)), sto->flags, AfxPushString(AfxGetUriString(&sto->path)));
    
    if (sto->arc)
        AfxReleaseObjects(1, (void*[]) { sto->arc });

    AfxDeallocateUri(&sto->path);

    return err;
}

_AFX afxClassConfig const _AfxStoClsConfig =
{
    .fcc = afxFcc_STO,
    .name = "Storage Point",
    .unitsPerPage = 6,
    .size = sizeof(AFX_OBJECT(afxStorage)),
    .ctor = (void*)_AfxStoCtor,
    .dtor = (void*)_AfxStoDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxMountStoragePoints(afxNat cnt, afxStoragePointSpecification const spec[], afxStorage sto[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(spec);
    AfxAssert(sto);

    afxClass* cls = AfxGetStorageClass();
    AfxAssertClass(cls, afxFcc_STO);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)sto, (void const*[]) { spec }))
        AfxThrowError();

    return err;
}

_AFX afxNat AfxCurateStorages(afxNat first, afxNat cnt, afxBool(*f)(afxStorage, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetStorageClass();
    AfxAssertClass(cls, afxFcc_STO);
    return AfxCurateInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateStorages(afxNat first, afxNat cnt, afxStorage sto[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(sto);
    afxClass* cls = AfxGetStorageClass();
    AfxAssertClass(cls, afxFcc_STO);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)sto);
}

_AFX afxNat AfxCountStorages(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetStorageClass();
    AfxAssertClass(cls, afxFcc_STO);
    return AfxCountInstances(cls);
}
