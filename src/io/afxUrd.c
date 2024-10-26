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
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define _AFX_URD_C
#include "../dev/afxIoImplKit.h"
#include "qwadro/inc/io/afxUrd.h"

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
    afxUnit      segCnt;
    afxByte**   segBufs;
    afxUnit*     segRefCnt;
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
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOB);
    AFX_ASSERT(hdr);
    return !(AfxReadStream(in, sizeof(*hdr), 0, hdr));
}

#define RWLIBRARYIDPACK( _version, _buildNum ) ( ( ( ( (_version)-0x30000U ) & 0x3ff00U ) << 14 ) | ( ( (_version)&0x0003fU ) << 16 ) | ( (_buildNum)&0xffffU ) )
#define RWLIBRARYIDUNPACKVERSION( _libraryID ) ( ( ( ( ( _libraryID ) >> 14 ) & 0x3ff00U ) + 0x30000U ) | ( ( ( _libraryID ) >> 16 ) & 0x0003fU ) )
#define RWLIBRARYIDUNPACKBUILDNUM( _libraryID ) ( (_libraryID)&0xffffU )

_AFX afxBool AfxFetchNextStreamChunk(afxStream in, afxUnit32 id, urdMark* hdr)
{
    // compatible with RwStreamFindChunk()
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOB);
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
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOB);
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
    afxError err = NIL;
    AFX_ASSERT(obj);
    afxUnit size = 0;

    afxClass const* cls = AfxGetClass(obj);
    afxInstanceExtension* entry = AFX_REBASE(AfxGetFirstLinkage(&cls->plugins), afxInstanceExtension, cls);
    AFX_ASSERT(entry->cls.chain->holder == cls);

    while (entry)
    {
        if (entry->ioSizCb)
        {
            afxUnit thisSize;

            if (thisSize = (entry->ioSizCb(obj, ((afxByte*)obj) + entry->objOff, entry->objSiz)))
                size += (thisSize + sizeof(urdMark));
        }
        entry = AFX_REBASE(AfxGetNextLinkage(&entry->cls), afxInstanceExtension, cls);
        AFX_ASSERT(entry->cls.chain->holder == cls);
    }
    return size;
}

_AFX afxError AfxStoreSerializableExtensions(afxStream out, afxObject obj)
{
    afxError err = NIL;
    AfxAssertObjects(1, &out, afxFcc_IOB);
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
    afxInstanceExtension* entry = AFX_REBASE(AfxGetFirstLinkage(&cls->plugins), afxInstanceExtension, cls);
    AFX_ASSERT(entry->cls.chain->holder == cls);

    while (entry)
    {
        if (entry->ioSizCb && entry->ioWriteCb)
        {
            afxUnit size;

            if ((size = entry->ioSizCb(obj, ((afxByte*)obj) + entry->objOff, entry->objSiz)))
            {
                hdr.fcc = entry->pluginId;
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
        entry = AFX_REBASE(AfxGetNextLinkage(&entry->cls), afxInstanceExtension, cls);
        AFX_ASSERT(entry->cls.chain->holder == cls);
    }
    return TRUE;
}

_AFX afxError AfxLoadSerializedExtensions(afxStream in, urdMark const* hdr, afxObject obj)
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOB);
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

        afxInstanceExtension* entry = AFX_REBASE(AfxGetFirstLinkage(&cls->plugins), afxInstanceExtension, cls);
        AFX_ASSERT(entry->cls.chain->holder == cls);
        afxBool found = FALSE;

        while (entry)
        {
            if (entry->pluginId == sub.fcc)
            {
                found = TRUE;
                break;
            }
            entry = AFX_REBASE(AfxGetNextLinkage(&entry->cls), afxInstanceExtension, cls);
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
        afxInstanceExtension* entry = AFX_REBASE(AfxGetFirstLinkage(&cls->plugins), afxInstanceExtension, cls);
        AFX_ASSERT(entry->cls.chain->holder == cls);

        while (entry)
        {
            if (entry->ioAlwaysCb && entry->ioAlwaysCb(in, obj, ((afxByte*)obj) + entry->objOff, entry->objSiz))
            {
                AfxThrowError();
                return FALSE;
            }
            entry = AFX_REBASE(AfxGetNextLinkage(&entry->cls), afxInstanceExtension, cls);
            AFX_ASSERT(entry->cls.chain->holder == cls);
        }
    }
    return TRUE;
}

_AFX afxError AfxSkipSerializedExtensions(afxStream in, urdMark const* hdr)
{
    afxError err = NIL;
    AfxAssertObjects(1, &in, afxFcc_IOB);
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

_AFX afxBool AfxTrackFileSegment(afxUrd urd, void const* object, afxUnit* segIdx)
{
    afxError err = NIL;
    afxBool rslt = FALSE;
    afxByte** maps = urd->segBufs;

    for (afxUnit i = 0; i < urd->segCnt; i++)
    {
        if ((object >= maps[i]) && (object <= maps[i] + urd->fsegs[i].decSiz))
        {
            AFX_ASSERT(segIdx);
            *segIdx = i;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFX void* AfxResolveUrdReference(afxUrd urd, afxUrdOrigin const* ref)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    AFX_ASSERT(ref);
    AFX_ASSERT_RANGE(urd->segCnt, ref->segIdx, 1);
    AFX_ASSERT_RANGE(urd->fsegs[ref->segIdx].decSiz, ref->offset, 1);
    return (ref->segIdx < urd->segCnt) && (ref->offset < urd->fsegs[ref->segIdx].decSiz) ? urd->segBufs[ref->segIdx] + ref->offset : NIL;
}

_AFX afxBool AfxOpenUrdSegments(afxUrd urd, afxUnit firstSeg, afxUnit secCnt, afxStream in)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    AFX_ASSERT_RANGE(urd->segCnt, firstSeg, secCnt);
    afxBool ret = TRUE;

    for (afxUnit i = 0; i < secCnt; i++)
    {
        afxUnit segIdx = firstSeg + i;
        urdSegment* seg = &urd->fsegs[segIdx];

        if (!(!seg->decSiz || urd->segBufs[segIdx]))
            ret = FALSE;

        if (urd->segRefCnt[segIdx])
        {
            AFX_ASSERT(urd->segBufs[segIdx]);
            ++urd->segRefCnt[segIdx];
        }
        else if (seg->decSiz)
        {
            afxUnit decSizAligned = (seg->decSiz + 3) & 0xFFFFFFFC;

            if (!(urd->segBufs[segIdx] = AfxAllocate(1, decSizAligned, seg->decAlign, AfxHere()))) AfxThrowError();
            else
            {
                if (!seg->codec)
                {
                    AfxReadStreamAt(in, seg->start, seg->encSiz, 0, urd->segBufs[segIdx]);
                }
                else
                {
                    void* buf;

                    if (!(buf = AfxAllocate(1, seg->encSiz, seg->encAlign, AfxHere()))) AfxThrowError();
                    else
                    {
                        if (AfxReadStreamAt(in, seg->start, seg->encSiz, 0, buf)) AfxThrowError();
                        else
                        {
                            //AfxDecodeStream(in, seg->start, seg->decAlign, seg->encSiz / seg->encAlign);
                        }
                    }
                }

                urd->segRefCnt[segIdx] = 1;

                if (err && urd->segBufs[segIdx])
                {
                    AfxDeallocate(urd->segBufs[segIdx]);
                    urd->segBufs[segIdx] = NIL;
                    urd->segRefCnt[segIdx] = 0;
                }
            }
        }
    }
    return ret;
}

_AFX void AfxCloseUrdSegments(afxUrd urd, afxUnit firstSeg, afxUnit segCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    AFX_ASSERT_RANGE(urd->segCnt, firstSeg, segCnt);

    for (afxUnit i = 0; i < segCnt; i++)
    {
        afxUnit segIdx = firstSeg + i;

        if (urd->segRefCnt[segIdx])
        {
            AFX_ASSERT(urd->segBufs[segIdx]);

            if (--urd->segRefCnt[segIdx] == 0)
            {
                AfxDeallocate(urd->segBufs[segIdx]);
                urd->segBufs[segIdx] = NIL;
                urd->segRefCnt[segIdx] = 0;
            }
        }
    }
}

_AFX afxError _AfxUrdDtorCb(afxUrd urd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);

    AfxCloseUrdSegments(urd, 0, urd->segCnt);

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
        AfxDeallocate(urd->convBuf);

    return err;
}

_AFX afxError _AfxUrdCtorCb(afxUrd urd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);

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
    afxError err = AFX_ERR_NONE;
    afxUrd urd = NIL;

    afxStream file;

    //if (AfxOpenFile(path, afxIoFlag_R, &file)) AfxThrowError();
    //else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);

        //if (AfxAcquireObjects(AfxGetUrdClass(), NIL, 1, NIL, (afxHandle**)&urd, AfxHere()))
        AfxThrowError();

        AfxReleaseObjects(1, (void*) { file });
    }

    return urd;
}

_AFX afxError AfxAcquireUrd(afxUnit segCnt, afxFcc trunkId, void* resvd, afxUrd* urd)
{
    afxError err = AFX_ERR_NONE;
    afxUrd urd2 = NIL;

    afxClass* cls = 0;// (afxClass*)AfxGetUrdClass();
    AfxAssertClass(cls, afxFcc_URD);

    if (AfxAcquireObjects(cls, 1, (afxObject*)&urd2, (void const*[]) { NIL, &segCnt, &trunkId })) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &urd2, afxFcc_URD);


    }
    return err;
}

_AFX afxError AfxLoadUrd(afxStream in, afxFcc trunkId, afxUrd* urd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &in, afxFcc_IOB);
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
    afxError err = AFX_ERR_NONE;

    afxStream iob;
    afxStreamInfo iobi = { 0 };
    iobi.usage = afxStreamUsage_FILE;
    iobi.flags = afxStreamFlag_READABLE;
    AfxAcquireStream(1, &iobi, &iob);

    if (AfxReopenFile(iob, uri, afxFileFlag_RX)) AfxThrowError();
    else
    {
        if (AfxLoadUrd(iob, trunkId, urd))
            AfxThrowError();

        AfxReleaseObjects(1, &iob);
    }
    return err;
}
