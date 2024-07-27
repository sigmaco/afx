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
#include "../src/afx/dev/afxDevIoBase.h"
#include "qwadro/io/afxUrd.h"

#define LOWORD(l) ((afxNat16)(l))
#define HIWORD(l) ((afxNat16)(((afxNat32)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((afxByte)(w))
#define HIBYTE(w) ((afxByte)(((afxNat16)(w) >> 8) & 0xFF))

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(_afxUrdHdr)
{
    afxNat32        fcc; // 'u', 'r', 'd', '\0'
    afxNat32        len;
    afxNat32        ver;
    afxNat32        mach; // 'l', 'e', '3', '2'
    afxNat32        crc32;
    afxNat32        baseSegOffset;
    afxNat32        segCnt;
    afxUrdReference rootObj; // 't', 'x', 'd', '\0'; 'a', 'n', 'd', '\0'; 'm', 'd', 'd', '\0'
};

AFX_DEFINE_STRUCT(_afxUrdChunkTxd)
{
    afxNat32        fcc; // 't', 'x', 'd', '\0'
    afxNat32        len;
    afxNat32        ver;
    afxNat          texCnt;
    afxNat          rasCnt;
    // afxString    texUrns[texCnt];
    // ---
};

AFX_DEFINE_STRUCT(_afxUrdChunkTex)
{
    afxNat      rasCnt;
    //afxNat      rasRasIdx[rasCnt];
    //afxNat      rasUsage[rasCnt];
};

AFX_DEFINE_STRUCT(_afxUrdChunkRas)
{
    afxNat          fmt;
    afxNat32        whd[3];
    afxNat          lodCnt;
    afxNat          flags;
    afxNat          usage;
};

#pragma pack(pop)

AFX_DEFINE_STRUCT(afxUrdSegment)
{
    afxNat          decSiz; // uncompressed size
    afxNat          encSiz; // compressed size
    afxNat          codec; // codec used to compress this section
    afxNat          intAlign;
    afxNat32        baseDataOffset;
};

AFX_OBJECT(afxUrd)
{
    _afxUrdHdr      hdr;
    afxNat          openSegCnt;
    afxUrdSegment** openSegs;
    afxBool         revBytes;
    afxBool8*       marshalled;
    afxBool8*       userBuffered;
    void*           convBuf;
};

////////////////////////////////////////////////////////////////////////////////

_AFX void* LoadFileSection2(afxUrdSegment const *Section, void *DestinationMemory, void *Reader, afxBool fileIsByteReversed)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(Section);
    AfxAssert(DestinationMemory);
    AfxAssert(Reader);

    void *result = 0;
    void *Result = 0;

    if (Section->decSiz)
    {
        afxNat alignedSiz = (Section->decSiz + 3) & 0xFFFFFFFC;

        if (DestinationMemory)
        {
            result = DestinationMemory;
            Result = DestinationMemory;
        }
        else
        {
            //use Section->InternalAlignment
            Result = AfxAllocate(1, alignedSiz, 0, AfxHere());
            result = Result;
        }

        if (result)
        {
            if (Section->codec)
            {
                afxNat v8 = AfxGetCompressionPaddingSize(Section->codec);
                void *v9 = AfxAllocate(1, Section->encSiz + v8, 0, AfxHere());

                if (v9)
                {
                    AfxReadStreamAt(Reader, Section->baseDataOffset, Section->encSiz, 0, v9);

                    if (!AfxDecompressData(Section->codec, fileIsByteReversed, Section->encSiz, v9, 0, 0, Section->decSiz, Result))
                    {
                        AfxDeallocate(Result);
                        Result = 0;
                    }
                }

                AfxDeallocate(v9);
                result = Result;
            }
            else
            {
                AfxReadStreamAt(Reader, Section->baseDataOffset, Section->encSiz, 0, result);
                result = Result;
            }
        }
    }
    return result;
}

_AFX void* AfxDecodeUrdReference(void const **sections, afxUrdReference const* ref)
{
    return (afxByte*)sections[ref->segIdx] + ref->offset;
}

_AFX afxBool AfxOpenUrdSegments(afxUrd urd, afxNat firstSeg, afxNat secCnt, void *buf[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    AfxAssertRange(urd->hdr.segCnt, firstSeg, secCnt);
    afxBool ret = TRUE;

    afxUrdSegment* segs = ((afxUrdSegment*)((afxByte*)urd->hdr.baseSegOffset));

    for (afxNat i = 0; i < secCnt; i++)
    {
        afxUrdSegment* seg = &segs[firstSeg + i];

        urd->userBuffered[firstSeg + i] = !!buf;

        if (!(!seg->decSiz || urd->openSegs[firstSeg + i]))
            ret = FALSE;
    }
    return ret;
}

_AFX afxBool AfxOpenUrdSegment(afxUrd urd, afxNat segIdx, void* buf)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    return AfxOpenUrdSegments(urd, segIdx, 1, buf ? &buf : NIL);
}

_AFX void AfxCloseUrdSegments(afxUrd urd, afxNat firstSeg, afxNat segCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    AfxAssertRange(urd->hdr.segCnt, firstSeg, segCnt);

    for (afxNat i = 0; i < segCnt; i++)
    {
        if (urd->openSegs[firstSeg + i])
        {
            if (!urd->userBuffered[firstSeg + i])
                AfxDeallocate(urd->openSegs[firstSeg + i]);

            urd->userBuffered[firstSeg + i] = FALSE;
            urd->marshalled[firstSeg + i] = FALSE;
            urd->openSegs[firstSeg + i] = NIL;
        }
    }
}

_AFX void AfxCloseAllUrdSegments(afxUrd urd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);
    AfxCloseUrdSegments(urd, 0, urd->openSegCnt);
}

_AFX afxError _AfxUrdDtorCb(afxUrd urd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &urd, afxFcc_URD);

    AfxCloseAllUrdSegments(urd);

    if (urd->openSegs)
        AfxDeallocate(urd->openSegs);
    
    if (urd->convBuf)
        AfxDeallocate(urd->convBuf);

    return err;
}

_AFX afxError _AfxUrdCtorCb(void *cache, afxNat idx, afxUrd urd, void const *paradigms)
{
    afxError err = AFX_ERR_NONE;



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

    if (!(file = AfxOpenFile(path, afxIoFlag_R))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);

        //if (AfxAcquireObjects(AfxGetUrdClass(), NIL, 1, NIL, (afxHandle**)&urd, AfxHere()))
        AfxThrowError();

        AfxReleaseObjects(1, (void*) { file });
    }

    return urd;
}

_AFX afxUrd AfxAcquireUrd(afxUri const *path)
{
    //AfxEntry("uri=%.*s", AfxPushString(AfxGetUriString(path)));
    afxError err = AFX_ERR_NONE;
    afxUrd urd = NIL;

    afxStream file;

    if (!(file = AfxOpenFile(path, afxIoFlag_R))) AfxThrowError();
    else
    {
        AfxAssertObjects(1, &file, afxFcc_FILE);

        //if (AfxAcquireObjects(AfxGetUrdClass(), NIL, 1, NIL, (afxHandle**)&urd, AfxHere()))
        AfxThrowError();

        AfxReleaseObjects(1, (void*) { file });
    }

    return urd;
}
