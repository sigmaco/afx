/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

// afxStream, data I/O stream, is the base object for objects representing files in Qwadro.

#ifndef AFX_STREAM_H
#define AFX_STREAM_H

#include "../base/afxObject.h"
#include "afxUri.h"

typedef enum afxType
{
    AFX_DATA_INT,
    AFX_DATA_INT8,
    AFX_DATA_INT16,
    AFX_DATA_INT32,
    AFX_DATA_INT64,
    AFX_DATA_INT128,
    
    AFX_DATA_NAT,
    AFX_DATA_NAT8,
    AFX_DATA_NAT16,
    AFX_DATA_NAT32,
    AFX_DATA_NAT64,
    AFX_DATA_NAT128,

    AFX_DATA_REAL,
    AFX_DATA_REAL16,
    AFX_DATA_REAL32,
    AFX_DATA_REAL64,
    AFX_DATA_REAL128,

    AFX_DATA_BOOL,
    AFX_DATA_BOOL8,
    AFX_DATA_BOOL32,

    AFX_DATA_CHAR,
    AFX_DATA_CHAR8,
    AFX_DATA_CHAR16,
    AFX_DATA_CHAR32,

    AFX_DATA_SIZE,
    AFX_DATA_PTR,

    // should know data to do proper alignment for pass in directly into math functions, and optimizations when doing batch copy.
    AFX_DATA_V2D,
    AFX_DATA_V3D,
    AFX_DATA_V4D,
    AFX_DATA_M2D,
    AFX_DATA_M3D,
    AFX_DATA_M4D,

    AFX_DATA_
} afxType;

typedef struct
{
    afxNat32    type;
    afxNat32    size; // vector size or 1.
    afxNat32    cnt; // array size or 1.
    afxNat32    align; // if must be corrected or zero.
} afxSerializedDataHeader;

typedef enum
{
    AFX_IO_FLAG_R           = (1 << 0), // readable
    AFX_IO_FLAG_W           = (1 << 1), // writeable
    AFX_IO_FLAG_X           = (1 << 2),
    AFX_IO_FLAG_RW          = (AFX_IO_FLAG_R | AFX_IO_FLAG_W),
    AFX_IO_FLAG_RX          = (AFX_IO_FLAG_R | AFX_IO_FLAG_X),
    AFX_IO_FLAG_WX          = (AFX_IO_FLAG_W | AFX_IO_FLAG_X),
    AFX_IO_FLAG_RWX         = (AFX_IO_FLAG_R | AFX_IO_FLAG_W | AFX_IO_FLAG_X),
    AFX_IO_FLAG_UNBUFFERED  = (1 << 3),
    AFX_IO_FLAG_LITERAL     = (1 << 4),
    AFX_IO_FLAG_VIRTUAL     = (1 << 5),
}
afxIoFlags;

typedef enum
{
    AFX_STRM_ORIGIN_BEGIN,
    AFX_STRM_ORIGIN_CURRENT,
    AFX_STRM_ORIGIN_END
}
afxStreamOrigin;

typedef afxNat afxRwx[3];

AFX_DEFINE_HANDLE(afxStream);

AFX void*               AfxStreamGetSystem(afxStream ios);

AFX afxResult           AfxStreamAdvance(afxStream ios, afxInt range);
AFX afxNat              AfxStreamAskCursor(afxStream ios);
AFX afxResult           AfxStreamCopyUri(afxStream ios, afxString *str);
AFX afxResult           AfxStreamEnd(afxStream ios);
AFX afxResult           AfxStreamFlush(afxStream ios);
AFX afxUri const*       AfxStreamGetUri(afxStream ios);
AFX afxString const*    AfxStreamGetUriString(afxStream ios);
AFX afxBool             AfxStreamHasReachedEnd(afxStream ios);
AFX afxBool             AfxStreamIsBuffered(afxStream ios);
AFX afxBool             AfxStreamIsExecutable(afxStream ios);
AFX afxBool             AfxStreamIsReadable(afxStream ios);
AFX afxBool             AfxStreamIsWriteable(afxStream ios);
AFX afxNat              AfxStreamMeasure(afxStream ios);
AFX afxResult           AfxStreamRead(afxStream ios, afxNat cnt, afxNat const siz[], void *dst[]);
AFX afxResult           AfxStreamReadSegmentsNEW(afxStream ios, afxNat cnt, afxSegment const segments[], void *dst[]);
AFX afxResult           AfxStreamRecede(afxStream ios, afxInt range);
AFX afxResult           AfxStreamResizeBuffer(afxStream ios, afxNat siz);
AFX afxResult           AfxStreamRewind(afxStream ios);
AFX afxResult           AfxStreamSeek(afxStream ios, afxInt offset, afxStreamOrigin origin);
AFX afxResult           AfxStreamWrite(afxStream ios, afxNat cnt, afxNat const siz[], void const *src[]);
AFX afxResult           AfxStreamWriteSegmentsNEW(afxStream ios, afxNat cnt, afxSegment const segments[], void const* src[]);

typedef struct afxStreamDataBatch
{
    afxType     type;
    afxNat      cnt;
    void const  *data;
} afxStreamDataBatch;


typedef struct afxStreamWriteOp
{
    afxNat32    siz;
    afxInt32    cnt;
    void const  *src;
}afxStreamWriteOp;

typedef struct afxStreamReadOp
{
    afxNat32    siz;
    afxInt32    cnt;
    void        *dst;
}afxStreamReadOp;


AFX afxError            AfxStreamReadBatch(afxStream ios, afxNat cnt, afxStreamReadOp ops[]);
AFX afxError            AfxStreamWriteBatch(afxStream ios, afxNat cnt, afxStreamWriteOp const ops[]);

AFX afxError            AfxStreamWriteChunk(afxStream ios, afxNat id, afxNat cnt, afxStreamWriteOp const ops[]);

//Bufferize?

typedef struct
{
    afxResult(*free)(afxStream);
    afxNat(*read)(afxStream, afxNat cnt, afxNat const siz[], void *dst[]);
    afxNat(*readProgressFb)(afxStream, afxNat32, void*);
    afxNat(*write)(afxStream, afxNat cnt, afxNat const siz[], void const * const src[]);
    afxNat(*writeProgressFb)(afxStream, afxNat32, void*);
    afxResult(*flush)(afxStream);
    afxNat(*tell)(afxStream);
    afxResult(*seek)(afxStream, afxInt, afxStreamOrigin);
    afxBool(*eos)(afxStream);
} afxStreamImplementation;

AFX afxStreamImplementation const _AfxStdStreamImplFile;

AFX_OBJECT(afxStream)
{
    afxObject                       obj;
    afxStreamImplementation const   *impl;

    afxBool                         rwx[3];

    afxNat32                        bufCap;
    afxByte                         *buffer;
    afxNat32                        currPosn;

    // file
    void                            *fd;
    afxUri4096                      uri;
    afxBool                         shouldBeFlushed;
};

#endif//AFX_STREAM_H