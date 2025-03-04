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

// This code is part of SIGMA Future Storage.

#ifndef AFX_SOURCE_H
#define AFX_SOURCE_H

#include "qwadro/inc/io/afxUri.h"

/**
    The afxSource structure allows the game to provide audio files to fill the external sources.
    You can specify a streaming file or a file in-memory, regardless of the "Stream" option.
    The only file format accepted is a fully formed WEM file, as converted by Wwise.
    Make sure that only one of szFile, pInMemory or idFile is non-null. if both idFile and szFile are set, idFile is passed to low-level IO and szFile is used as stream name (for profiling purposes).
    When using the in-memory file (pInMemory & uiMemorySize), it is the responsibility of the game to ensure the memory stays valid for the entire duration of the playback. 
    You can achieve this by using the EndOfEvent callback to track when the Event ends.
*/

AFX_DEFINE_STRUCT(afxSource)
{
    // Cookie identifying the source, given by hashing the name of the source given in the project.
    afxUnit32   iExternalSrcCookie;
    // Codec ID for the file.
    afxUnit32   idCodec;
    // File path for the source. If not NULL, the source will be streaming from disk. Set buf to NULL. If file is set, this field is used as stream name (for profiling purposes).
    afxUri      uri; 
    // Pointer to the in-memory file.  If not NULL, the source will be read from memory. Set szFile and idFile to NULL.
    void*       buf;
    // Size of the data pointed by buf.
    afxUnit32   siz;
    // File ID.  If not zero, the source will be streaming from disk.  This ID can be anything.  Note that you must override the low-level IO to resolve this ID to a real file.  See \ref streamingmanager_lowlevel for more information on overriding the Low Level IO.
    afxFile     file;
    afxStream   iob;
    afxUnit32   offset;
};

AFXINL void AfxMakeSource(afxSource* src);

AFXINL void AfxMakeSourceFromMemory(afxSource* src, void* mem, afxUnit32 siz, afxUnit32 extlSrcCookie, afxUnit32 idCodec);

AFXINL void AfxMakeSourceFromUri(afxSource* src, afxUri const* uri, afxUnit extlSrcCookie, afxUnit32 idCodec);

AFXINL void AfxMakeSourceFromFile(afxSource* src, afxFile file, afxUnit32 extlSrcCookie, afxUnit32 idCodec);

AFXINL void AfxMakeSourceFromStream(afxSource* src, afxStream iob, afxUnit32 offset, afxUnit32 range);

#endif//AFX_SOURCE_H
