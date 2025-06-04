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

  //////////////////////////////////////////////////////////////////////////////
 // Qwadro Data Access And Storage Infrastructure                            //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA Future Storage.

#ifndef AFX_FILE_H
#define AFX_FILE_H

#include "qwadro/inc/io/afxStream.h"
#include "qwadro/inc/io/afxUri.h"

typedef enum afxFileFlag
{
    // permissions
    afxFileFlag_R       = AFX_BITMASK(0), // Readable
    afxFileFlag_W       = AFX_BITMASK(1), // Writeable
    afxFileFlag_X       = AFX_BITMASK(2), // Executable (seekable)
    afxFileFlag_RW      = (afxFileFlag_R | afxFileFlag_W),
    afxFileFlag_RX      = (afxFileFlag_R | afxFileFlag_X),
    afxFileFlag_WX      = (afxFileFlag_W | afxFileFlag_X),
    afxFileFlag_RWX     = (afxFileFlag_R | afxFileFlag_WX),
    AFX_FILE_PERM_MASK  = afxFileFlag_RWX,

    // attributes
    afxFileFlag_U       = AFX_BITMASK(3), // Unbuffered.
    afxFileFlag_L       = AFX_BITMASK(4), // Literal
    afxFileFlag_V       = AFX_BITMASK(5), // Virtual. This flag is reserved for system use.
    afxFileFlag_Q       = AFX_BITMASK(6), // Qwadro. This flag is reserved for system use.
    afxFileFlag_D       = AFX_BITMASK(7),  // Device. This flag is reserved for system use.
    afxFileFlag_H       = AFX_BITMASK(8),  // Hidden. The stream is hidden. It shouldn't be included in an ordinary listing.
    afxFileFlag_A       = AFX_BITMASK(9),  // Archived. The stream is an archive-derived stream.
    afxFileFlag_T       = AFX_BITMASK(10), // Temporary. Used for temporary storage. File systems avoid writing data back to mass storage if sufficient cache memory is available, because typically, an application deletes a temporary file after the handle is closed. In that scenario, the system can entirely avoid writing the data. Otherwise, the data is written after the handle is closed. This is used by SIGMA Future Storage.
    afxFileFlag_Z       = AFX_BITMASK(11), // Compressed. The data is compressed. For a file, all of the data in the file is compressed. This is used by SIGMA Future Storage.
    afxFileFlag_E       = AFX_BITMASK(12), // Encrypted. The data is encrypted. For a file, all data streams in the file are encrypted. This is used by SIGMA Future Storage.
    afxFileFlag_O       = AFX_BITMASK(13), // Offline. The data isn't available immediately. This flag indicates that the data is physically moved to remote/offline storage. This is used by SIGMA Future Storage.
    afxFileFlag_S       = AFX_BITMASK(14), // Sparse. The data is a sparse file. Empty ranges (zeroed ranges) are generated dynamically by some algorithm. This is used by SIGMA Future Storage.
} afxFileFlags;

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(afxFileSegment)
{
    afxUnit16   encodedAlign;
    afxUnit16   decodedAlign;
    afxUnit32   decodedSiz; // decoded/uncompressed size
    afxUnit32   encodedSiz; // encoded/compressed size
    afxUnit32   codec; // codec used to compress this section
    afxUnit32   start; // where starts the data belonging to this segment.
};

#pragma pack(pop)

AFX_DEFINE_STRUCT(afxFileBlock)
{
    afxUnit32   segIdx; // index of a file segment.
    afxUnit32   offset; // relative to start of a file segment.
    afxUnit32   range; // length of data from offset.
};

AFX afxError    AfxReadFileLine(afxStream file, afxString* str);

AFX afxResult   AfxFlushFileCache(afxStream file);

AFX afxBool     AfxTestFileFlags(afxStream file, afxFileFlags flags);

AFX afxBool     AfxGetFileUri(afxStream file, afxUri* location);
AFX afxBool     AfxGetResolvedFileUri(afxStream file, afxUri* location);

// FILE SEGMENT

AFX afxBool     AfxTrackFileSegment(afxStream file, void const* at, afxUnit* segIdx);

AFX void*       AfxResolveFileReference(afxStream file, afxFileBlock const* ref);

AFX afxBool     AfxOpenFileSegments(afxStream file, afxUnit baseSegIdx, afxUnit segCnt, afxStream in);

AFX void        AfxCloseFileSegments(afxStream file, afxUnit baseSegIdx, afxUnit segCnt);

AFX afxError    AfxRealizeFileSegments(afxStream file, afxUnit32 baseSegOffset, afxUnit segCnt);

// FILE REUSE

/**
    The AfxReopenFile() method reuses a stream to either open the file specified by uri or to change its access mode.

    If a new uri is specified, the function first attempts to close any file already associated with the stream 
    and disassociates it. Then, independently of whether that stream was successfuly closed or not, this method opens the 
    file specified by uri and associates it with the stream just as AfxOpenFile() would do using the specified mode.

    If uri is nil, the function attempts to change the mode of the stream.

    This function is especially useful for redirecting predefined streams like stdin, stdout and stderr to specific files.
*/

AFX afxError    AfxReopenFile(afxStream file, afxUri const* uri, afxFileFlags flags);

AFX afxError    AfxReloadFile(afxStream file, afxUri const* uri); // will upload the entire file data into RAM and close the file.

////////////////////////////////////////////////////////////////////////////////

/**
    The AfxOpenFile() method opens a file stream.

    Opens the file whose name is specified in the parameter filename and associates it with a stream that can be identified in future operations by the FILE pointer returned.

    The operations that are allowed on the stream and how these are performed are defined by the mode parameter.
*/

AFX afxError    AfxOpenFile(afxUri const* uri, afxFileFlags flags, afxStream* file);

AFX afxError    AfxLoadFile(afxUri const* uri, afxStream* file); // will upload the entire file data into RAM and close the file.

#endif//AFX_FILE_H
