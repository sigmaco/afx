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

 /// This structure allows the game to provide audio files to fill the external sources.
 /// You can specify a streaming file or a file in-memory, regardless of the "Stream" option.
 /// The only file format accepted is a fully formed WEM file, as converted by Wwise.
 /// Make sure that only one of szFile, pInMemory or idFile is non-null. if both idFile and szFile are set, idFile is passed to low-level IO and szFile is used as stream name (for profiling purposes).
 /// When using the in-memory file (pInMemory & uiMemorySize), it is the responsibility of the game to ensure the memory stays valid for the entire duration of the playback. 
 /// You can achieve this by using the EndOfEvent callback to track when the Event ends.

AFX_DEFINE_STRUCT(afxSource)
{
    afxNat32    iExternalSrcCookie;    ///< Cookie identifying the source, given by hashing the name of the source given in the project.  See \ref AK::SoundEngine::GetIDFromString. \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures. \endaknote
    afxNat32    idCodec;              ///< Codec ID for the file.  One of the audio formats defined in AkTypes.h (AKCODECID_XXX)
    afxUri      uri;              ///< File path for the source.  If not NULL, the source will be streaming from disk. Set pInMemory to NULL. If idFile is set, this field is used as stream name (for profiling purposes). /// The only file format accepted is a fully formed WEM file, as converted by Wwise.
    void*       buf;                ///< Pointer to the in-memory file.  If not NULL, the source will be read from memory. Set szFile and idFile to NULL. The only file format accepted is a fully formed WEM file, as converted by Wwise.
    afxNat32    siz;          ///< Size of the data pointed by pInMemory
    afxFile     file;                ///< File ID.  If not zero, the source will be streaming from disk.  This ID can be anything.  Note that you must override the low-level IO to resolve this ID to a real file.  See \ref streamingmanager_lowlevel for more information on overriding the Low Level IO.
    afxStream   iob;
    afxNat32    offset;
};

AFXINL void AfxMakeSource(afxSource* src);

AFXINL void AfxMakeSourceFromMemory(afxSource* src, void* mem, afxNat32 siz, afxNat32 extlSrcCookie, afxNat32 idCodec);

AFXINL void AfxMakeSourceFromUri(afxSource* src, afxUri const* uri, afxNat extlSrcCookie, afxNat32 idCodec);

AFXINL void AfxMakeSourceFromFile(afxSource* src, afxFile file, afxNat32 extlSrcCookie, afxNat32 idCodec);

AFXINL void AfxMakeSourceFromStream(afxSource* src, afxStream iob, afxNat32 offset, afxNat32 range);

#endif//AFX_SOURCE_H
