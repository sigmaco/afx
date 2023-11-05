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

// This content is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#ifndef AFX_URD_H
#define AFX_URD_H

#include "afx/core/afxStream.h"
#include "afx/core/afxResource.h"

// URD --- UNIFORM RESOURCE DICTIONARY

AFX_DEFINE_STRUCT(afxResourceHandler)
{
    afxFcc              type;
    afxError            (*upload)(afxResource res);
    afxError            (*download)(afxResource res);
};

AFX_DEFINE_STRUCT(afxResourceAllocation)
{
    afxBool             permanent;
    void                *meta;
    void                *data;
};

AFX_DEFINE_STRUCT(afxResourceLinkage)
{
    afxLinkage          urd; // afxUrd
    afxString           name; // 32
    //afxResourceState    status; // initially idle
    afxNat              reqCnt; // initially zero
    afxTime             lastReqTime;
    afxTime             lastUpdTime;
    afxError            (*request)(afxResource res, afxNat cnt, void *udd);
    afxError            (*dispense)(afxResource res);
    afxChain            allocations;
};

AFX_DEFINE_STRUCT(afxUrdEntry)
{
    afxUri*             name; // 32
    afxUrd              urd; // link back to file backing this resource
    afxInt              reqCnt;
    //afxResourceState    status;
    afxTime             lastReqTime;
};

AFX_DEFINE_STRUCT(afxUrdReference)
{
    afxNat              secIdx;
    afxSize             offset;
};

AFX_DEFINE_STRUCT(afxUrdHeader)
{
    afxNat              ver;
    afxNat              totalSiz;
    afxNat32            crc;
    afxNat              secOffset;
    afxNat              secCnt;
    afxUrdReference     rootObjectTypeDef;
    afxUrdReference     rootObject;
    afxNat              typeTag;
    afxNat              extraTags[4];
    afxNat              stringDatabaseCrc;
    afxNat              reserved[3];
};

AFX_DEFINE_STRUCT(afxUrdMagicValue)
{
    afxNat              magicVal[4];
    afxNat              hdrSiz;
    afxNat              hdrFmt;
    afxNat              reserved[2];
};

AFX_DEFINE_STRUCT(afxUrdSection)
{
    afxNat              fmt; // codec
    afxNat              dataOffset;
    afxNat              dataSiz;
    afxNat              expandedDataSiz;
    afxNat              internalAlignment;

    afxInt              reqCnt;
    //afxResourceState    status;
    afxTime             lastReqTime;
};

AFX_OBJECT(afxUrd)
{
//#ifdef _AFX_URD_C
    afxUrdHeader*       hdr;
    afxUrdMagicValue*   srcMagicVal;
    afxNat              secCnt;
    afxUrdSection**     sections;
    afxBool8*           marshalled;
    afxBool8*           isUserMem;
    void*               conversionBuf;

    afxFile             file;
    afxUri*             path; // 128
    afxTime             lastUpdTime;
    afxNat              entryCnt;
    afxUrdEntry*        entries;
//#endif
};

AFX afxBool     AfxOpenUrdSection(afxUrd urd, afxNat secIdx, void *buf);
AFX afxBool     AfxOpenUrdSections(afxUrd urd, afxNat baseIdx, afxNat secCnt, void *buf[]);

AFX void        AfxCloseUrdSection(afxUrd urd, afxNat secIdx);
AFX void        AfxCloseUrdSections(afxUrd urd, afxNat baseIdx, afxNat secCnt);
AFX void        AfxCloseAllUrdSections(afxUrd urd);

/// In addition to the basic ReadEntireFile and FreeFile calls, Granny provides a number of more flexible alternatives that can be used to fine-tune the way your app loads data.

/// Instead of reading an entire Granny file from disk, you can easily read one from your own memory buffer instead :

//  granny_file *File = ReadEntireFileFromMemory(BufferSize, Buffer);

/// Note that GrannyReadEntireFileFromMemory unpacks the file into its own memory buffers, so you are free to recycle your memory buffer after GrannyReadEntireFileFromMemory returns.

/// You can also opt to read directly from a granny_file_reader of your own creation, which allows you to provide the data in any way you want :

//  granny_file *File = ReadEntireFileFromReader(Reader);

/// and, you also have the option of only reading the header of the file, and deferring loads of the rest of the portions until you actually need them :

// granny_file *File = ReadPartialFileFromReader(Reader);

/// This will fill in just the header section of the granny_file object, and will leave the section buffers empty(and unallocated).You can then choose when and which sections to load by calling ReadFileSection and FixupFileSectionPhase1/FixupFileSectionPhase2.

/// You can control which sections of a file are memory - resident and which are not at any time like this:

// Load section 5 into memory
// ReadFileSection(FileReader, File, 5);

// Load section 6 into your memory
// ReadFileSectionInPlace(FileReader, File, 6, MyMemory);

// Now free section 5
// FreeFileSection(File, 5);

// Free any sections currently loaded
// (marks section 6 as free, but does not touch the memory itself)
// FreeAllFileSections(File);

AFX void* AfxDecodeUrdReference(void const **sections, afxUrdReference const* ref);

#endif//AFX_URD_H