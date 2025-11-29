/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _CRT_SECURE_NO_WARNINGS 1
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _AFX_URD_C
#include "../io/afxIoDDK.h"
#include "qwadro/etc/afxUrd.h"

#define LOWORD(l) ((afxUnit16)(l))
#define HIWORD(l) ((afxUnit16)(((afxUnit32)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((afxByte)(w))
#define HIBYTE(w) ((afxByte)(((afxUnit16)(w) >> 8) & 0xFF))

////////////////////////////////////////////////////////////////////////////////

AFX_OBJECT(afxUrd)
{
    afxUri128   uri;
    urdRoot     froot;
    urdSegment* fsegs;
    afxUnit     segCnt;
    afxByte**   segBufs;
    afxUnit*    segRefCnt;
    void*       convBuf;
};

AFX_DEFINE_STRUCT(urdBuilder)
{
    afxFcc      trunkFcc;
    afxUnit      segCnt;
    afxChain    segments;
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxBool AfxWriteNextSeriesHeader(afxStream out, urdMark const* hdr)
{
    return !(AfxWriteStream(out, 1, sizeof(*hdr), hdr));
}

_AFX afxBool AfxReadNextSeriesHeader(afxStream in, urdMark* hdr)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(hdr);
    return !(AfxReadStream(in, sizeof(*hdr), 0, hdr));
}

#define RWLIBRARYIDPACK( _version, _buildNum ) ( ( ( ( (_version)-0x30000U ) & 0x3ff00U ) << 14 ) | ( ( (_version)&0x0003fU ) << 16 ) | ( (_buildNum)&0xffffU ) )
#define RWLIBRARYIDUNPACKVERSION( _libraryID ) ( ( ( ( ( _libraryID ) >> 14 ) & 0x3ff00U ) + 0x30000U ) | ( ( ( _libraryID ) >> 16 ) & 0x0003fU ) )
#define RWLIBRARYIDUNPACKBUILDNUM( _libraryID ) ( (_libraryID)&0xffffU )

_AFX afxBool AfxFetchNextStreamChunk(afxStream in, afxUnit32 id, urdMark* hdr)
{
    // compatible with RwStreamFindChunk()
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(hdr);
    AFX_ASSERT(id);
    afxBool rslt = FALSE;

#pragma pack(push, 1)
    struct
    {
        afxUnit32 type;
        afxUnit32 length;
        afxUnit32 libraryID;
    } mark = { 0 };

    struct
    {
        afxUnit32 type; // chunk ID - see \ref RwStreamFindChunk.
        afxUnit32 length; // length of the chunk data in bytes.
        afxUnit32 version; // version of the chunk data. See RwEngineGetVersion.
        afxUnit32 buildNum; // build number of the RenderWare libraries previously used to stream out the data.
        afxBool32 isComplex;
    } chunkHdrInfo = { 0 };
#pragma pack(pop)

    while (!AfxHasStreamReachedEnd(in))
    {
        if (AfxReadStream(in, sizeof(mark), 0, &mark))
        {
            AfxThrowError();
            break;
        }

        chunkHdrInfo.type = mark.type;
        chunkHdrInfo.length = mark.length;
        chunkHdrInfo.isComplex = TRUE;

        if (!(mark.libraryID & 0xffff0000)) // Check for old RW library ID
        {
            // Just contains old-style version number
            chunkHdrInfo.version = mark.libraryID << 8;
            chunkHdrInfo.buildNum = 0;
        }
        else
        {
            // Unpack new RW library ID
            chunkHdrInfo.version = (((((mark.libraryID) >> 14) & 0x3ff00U) + 0x30000U) | (((mark.libraryID) >> 16) & 0x0003fU));
            chunkHdrInfo.buildNum = ((mark.libraryID) & 0xffffU);
        }

        if (chunkHdrInfo.type == id)
        {
            hdr->siz = chunkHdrInfo.length;
            hdr->ver = chunkHdrInfo.version;
            rslt = TRUE;
            break;
        }

        if (chunkHdrInfo.length && AfxAdvanceStream(in, chunkHdrInfo.length))
        {
            AfxThrowError();
            break;
        }
    }
    return rslt;
}

_AFX afxUnit32 AfxPullStreamChunk(afxStream in, urdMark* hdr)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(hdr);
    afxUnit32 id = AFX_INVALID_INDEX;

#pragma pack(push, 1)
    struct
    {
        afxUnit32 type;
        afxUnit32 length;
        afxUnit32 libraryID;
    } mark = { 0 };

    struct
    {
        afxUnit32 type; // chunk ID - see \ref RwStreamFindChunk.
        afxUnit32 length; // length of the chunk data in bytes.
        afxUnit32 version; // version of the chunk data. See RwEngineGetVersion.
        afxUnit32 buildNum; // build number of the RenderWare libraries previously used to stream out the data.
        afxBool32 isComplex;
    } chunkHdrInfo = { 0 };
#pragma pack(pop)

    while (!AfxHasStreamReachedEnd(in))
    {
        if (AfxReadStream(in, sizeof(mark), 0, &mark))
        {
            AfxThrowError();
            break;
        }

        chunkHdrInfo.type = mark.type;
        chunkHdrInfo.length = mark.length;
        chunkHdrInfo.isComplex = TRUE;

        if (!(mark.libraryID & 0xffff0000)) // Check for old RW library ID
        {
            // Just contains old-style version number
            chunkHdrInfo.version = mark.libraryID << 8;
            chunkHdrInfo.buildNum = 0;
        }
        else
        {
            // Unpack new RW library ID
            chunkHdrInfo.version = (((((mark.libraryID) >> 14) & 0x3ff00U) + 0x30000U) | (((mark.libraryID) >> 16) & 0x0003fU));
            chunkHdrInfo.buildNum = ((mark.libraryID) & 0xffffU);
        }

        //if (chunkHdrInfo.type == id)
        {
            hdr->fcc = chunkHdrInfo.type;
            hdr->siz = chunkHdrInfo.length;
            hdr->ver = chunkHdrInfo.version;
            id = chunkHdrInfo.type;
            break;
        }

        if (chunkHdrInfo.length && AfxAdvanceStream(in, chunkHdrInfo.length))
        {
            AfxThrowError();
            break;
        }
        break;
    }
    return id;
}

_AFX afxUnit AfxMeasureSerializableExtensions(afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT(obj);
    afxUnit size = 0;

    afxClass const* cls = AfxGetClass(obj);
    afxClassExtension* entry = AFX_REBASE(AfxGetFirstLink(&cls->extensions), afxClassExtension, cls);
    AFX_ASSERT(entry->cls.chain->holder == cls);

    while (entry)
    {
        if (entry->ioSizCb)
        {
            afxUnit thisSize;

            if (thisSize = (entry->ioSizCb(obj, ((afxByte*)obj) + entry->objOff, entry->objSiz)))
                size += (thisSize + sizeof(urdMark));
        }
        entry = AFX_REBASE(AfxGetNextLink(&entry->cls), afxClassExtension, cls);
        AFX_ASSERT(entry->cls.chain->holder == cls);
    }
    return size;
}

_AFX afxError AfxStoreSerializableExtensions(afxStream out, afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &out);
    AFX_ASSERT(obj);

    urdMark hdr =
    {
        .fcc = afxChunkId_EXTENSIONS,
        .siz = AfxMeasureSerializableExtensions(obj),
        .ver = NIL
    };

    if (!AfxWriteNextSeriesHeader(out, &hdr))
    {
        AfxThrowError();
        return err;
    }

    afxClass const* cls = AfxGetClass(obj);
    afxClassExtension* entry = AFX_REBASE(AfxGetFirstLink(&cls->extensions), afxClassExtension, cls);
    AFX_ASSERT(entry->cls.chain->holder == cls);

    while (entry)
    {
        if (entry->ioSizCb && entry->ioWriteCb)
        {
            afxUnit size;

            if ((size = entry->ioSizCb(obj, ((afxByte*)obj) + entry->objOff, entry->objSiz)))
            {
                hdr.fcc = entry->extId;
                hdr.siz = size;

                if (!AfxWriteNextSeriesHeader(out, &hdr))
                {
                    AfxThrowError();
                    return FALSE;
                }
                else if (entry->ioWriteCb(out, obj, ((afxByte*)obj) + entry->objOff, entry->objSiz))
                {
                    AfxThrowError();
                    return FALSE;
                }
            }
        }
        entry = AFX_REBASE(AfxGetNextLink(&entry->cls), afxClassExtension, cls);
        AFX_ASSERT(entry->cls.chain->holder == cls);
    }
    return TRUE;
}

_AFX afxError AfxLoadSerializedExtensions(afxStream in, urdMark const* hdr, afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(hdr);
    AFX_ASSERT(obj);
    AFX_ASSERT(hdr->fcc == afxChunkId_EXTENSIONS);

    afxUnit totalExtLen = hdr->siz;

    afxClass const* cls = AfxGetClass(obj);

    urdMark sub;

    while (totalExtLen)
    {
        if (!AfxReadNextSeriesHeader(in, &sub))
        {
            AfxThrowError();
            break;
        }

        afxClassExtension* entry = AFX_REBASE(AfxGetFirstLink(&cls->extensions), afxClassExtension, cls);
        AFX_ASSERT(entry->cls.chain->holder == cls);
        afxBool found = FALSE;

        while (entry)
        {
            if (entry->extId == sub.fcc)
            {
                found = TRUE;
                break;
            }
            entry = AFX_REBASE(AfxGetNextLink(&entry->cls), afxClassExtension, cls);
            AFX_ASSERT(entry->cls.chain->holder == cls);
        }

        if (found && entry->ioReadCb)
        {
            if (entry->ioReadCb(in, obj, ((afxByte*)obj) + entry->objOff, entry->objSiz))
            {
                AfxThrowError();
                return FALSE;
            }
        }
        else if (AfxAdvanceStream(in, sub.siz))
        {
            AfxThrowError();
            return FALSE;
        }

        totalExtLen -= (sub.siz + sizeof(urdMark));
    }

    if (!err)
    {
        afxClassExtension* entry = AFX_REBASE(AfxGetFirstLink(&cls->extensions), afxClassExtension, cls);
        AFX_ASSERT(entry->cls.chain->holder == cls);

        while (entry)
        {
            if (entry->ioAlwaysCb && entry->ioAlwaysCb(in, obj, ((afxByte*)obj) + entry->objOff, entry->objSiz))
            {
                AfxThrowError();
                return FALSE;
            }
            entry = AFX_REBASE(AfxGetNextLink(&entry->cls), afxClassExtension, cls);
            AFX_ASSERT(entry->cls.chain->holder == cls);
        }
    }
    return TRUE;
}

_AFX afxError AfxSkipSerializedExtensions(afxStream in, urdMark const* hdr)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(hdr);
    AFX_ASSERT(hdr->fcc == afxChunkId_EXTENSIONS);

    afxUnit len = hdr->siz;

    while (len > 0)
    {
        urdMark sub;

        if (!AfxReadNextSeriesHeader(in, &sub))
        {
            AfxThrowError();
            return FALSE;
        }
        else if (AfxAdvanceStream(in, sub.siz))
        {
            AfxThrowError();
            return FALSE;
        }
        else
            len -= (sub.siz + sizeof(urdMark));
    }
    return err;
}

_AFX afxError _AfxUrdDtorCb(afxUrd urd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_URD, 1, &urd);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = urd->segCnt,
            .var = (void*)&urd->segRefCnt
        },
        {
            .cnt = urd->segCnt,
            .var = (void*)&urd->segBufs
        }
    };
    AfxDeallocateInstanceData(urd, ARRAY_SIZE(stashes), stashes);

    if (urd->convBuf)
        AfxDeallocate((void**)&urd->convBuf, AfxHere());

    return err;
}

_AFX afxError _AfxUrdCtorCb(afxUrd urd, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_URD, 1, &urd);

    afxStorage disk = args[0];
    afxUnit segCnt = *(afxUnit*)(args[1]);
    afxUnit trunkId = *(afxUnit*)(args[2]);

    urd->segCnt = segCnt;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = urd->segCnt,
            .var = (void*)&urd->segRefCnt
        },
        {
            .cnt = urd->segCnt,
            .var = (void*)&urd->segBufs
        }
    };

    if (AfxAllocateInstanceData(urd, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        for (afxUnit i = 0; i < segCnt; i++)
        {
            urd->segBufs[i] = NIL;
            urd->segRefCnt[i] = 0;
        }

        if (err)
            AfxDeallocateInstanceData(urd, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_AFX afxClassConfig _AfxUrdClsCfg =
{
    .fcc = afxFcc_URD,
    .name = "URD",
    .desc = "Uniform Resource Dictionary",
    .fixedSiz = sizeof(AFX_OBJECT(afxUrd)),
    .ctor = (void*)_AfxUrdCtorCb,
    .dtor = (void*)_AfxUrdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxUrd AfxBuildUrd(afxUri const *path)
{
    afxError err = { 0 };
    afxUrd urd = NIL;

    afxStream file;

    //if (AfxOpenFile(path, afxIoFlag_R, &file)) AfxThrowError();
    //else
    {
        AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &file);

        //if (AfxAcquireObjects(AfxGetUrdClass(), NIL, 1, NIL, (afxHandle**)&urd, AfxHere()))
        AfxThrowError();

        AfxDisposeObjects(1, (void*) { file });
    }

    return urd;
}

_AFX afxError AfxAcquireUrd(afxUnit segCnt, afxFcc trunkId, void* resvd, afxUrd* urd)
{
    afxError err = { 0 };
    afxUrd urd2 = NIL;

    afxClass* cls = 0;// (afxClass*)AfxGetUrdClass();
    AFX_ASSERT_CLASS(cls, afxFcc_URD);

    if (AfxAcquireObjects(cls, 1, (afxObject*)&urd2, (void const*[]) { NIL, &segCnt, &trunkId })) AfxThrowError();
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_URD, 1, &urd2);


    }
    return err;
}

_AFX afxError AfxLoadUrd(afxStream in, afxFcc trunkId, afxUrd* urd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_IOB, 1, &in);
    AFX_ASSERT(AfxIsStreamReadable(in));
    afxResource rs;
    
    urdRoot root;
    if (AfxSeekStream(in, sizeof(root), afxSeekOrigin_END)) AfxThrowError();
    else
    {
        urdTrunk trunk;
        if (AfxReadStream(in, sizeof(root), 0, &root)) AfxThrowError();
        else if (root.hdr.fcc != afxFcc_URD) AfxThrowError();
        else if (AfxSeekStream(in, root.trunkOff, afxSeekOrigin_BEGIN)) AfxThrowError();
        else if (AfxReadStream(in, sizeof(trunk), 0, &trunk)) AfxThrowError();
        {
            if (trunkId && trunk.hdr.fcc != (afxUnit32)trunkId) AfxThrowError();
            else
            {
                urdSegment segs[256];
                AFX_ASSERT(trunk.hdr.siz > trunk.segCnt * sizeof(segs[0]));

                if (AfxReadStreamAt(in, trunk.baseSegOff, trunk.segCnt * sizeof(segs[0]), 0, &segs[0])) AfxThrowError();
                else
                {
                    if (AfxAcquireUrd(trunk.segCnt, trunkId, NIL, urd)) AfxThrowError();
                    else
                    {

                    }
                }
            }
        }
    }
    return err;
}

_AFX afxError AfxOpenUrd(afxUri const* uri, afxFcc trunkId, afxUrd* urd)
{
    afxError err = { 0 };

    afxStream iob;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxIoUsage_FILE;
    iobi.flags = afxIoFlag_READABLE;
    AfxAcquireStream(1, &iobi, &iob);

    if (AfxReopenFile(iob, uri, afxFileFlag_RX)) AfxThrowError();
    else
    {
        if (AfxLoadUrd(iob, trunkId, urd))
            AfxThrowError();

        AfxDisposeObjects(1, &iob);
    }
    return err;
}
