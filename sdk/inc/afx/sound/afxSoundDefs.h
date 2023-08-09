/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_SOUND_DEFS_H
#define AFX_SOUND_DEFS_H

#include "afx/core/diag/afxDebug.h"
#include "afx/core/afxSimd.h"
#include "afx/math/afxMathDefs.h"

typedef enum afxCurveInterpolation // curve interpolation types
{
    // curves from 0 to LastFadeCurve NEED TO BE A MIRROR IMAGE AROUND LINEAR (ex. Log3 is the inverse of Exp3)
    afxCurveInterpolation_Log3 = 0, // log3
    afxCurveInterpolation_Sine = 1, // sine
    afxCurveInterpolation_Log1 = 2, // log1
    afxCurveInterpolation_InvSCurve = 3, // inversed S curve
    afxCurveInterpolation_Linear = 4, // linear (default)
    afxCurveInterpolation_SCurve = 5, // S curve
    afxCurveInterpolation_Exp1 = 6, // exp1
    afxCurveInterpolation_SineRecip = 7, // reciprocal of sine curve
    afxCurveInterpolation_Exp3 = 8, // exp3
    afxCurveInterpolation_LastFadeCurve = 8, // update this value to reflect last curve available for fades
    afxCurveInterpolation_Constant = 9  // constant ( not valid for fading values )
} afxCurveInterpolation;

AFX_DEFINE_STRUCT(afxListener) // Listener information.
{
    afxTransform        pos; /// Listener position (see AK::SoundEngine::SetPosition()).

    afxReal32           scalingFactor; /// Listener scaling factor (see AK::SoundEngine::SetListenerScalingFactor()).
    afxBool             spatialized; /// Whether listener is spatialized or not (see AK::SoundEngine::SetListenerSpatialization()).
};

AFXINL void AfxAcquireListener(afxListener *list)
{
    afxError err = NIL;
    AfxAssert(list);
    list->scalingFactor = (afxReal32)1;
    list->spatialized = TRUE;
}

AFX_DEFINE_STRUCT(afxRamp) // volume ramp specified by end points "previous" and "next".
{
    afxReal32 prev;
    afxReal32 next;
};

AFXINL void AfxAcquireRamp(afxRamp *ramp, afxReal prev, afxReal next)
{
    afxError err = NIL;
    AfxAssert(ramp);
    ramp->prev = (afxReal32)prev;
    ramp->next = (afxReal32)next;
}

AFXINL void AfxResetRamp(afxRamp *ramp)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAcquireRamp(ramp, 1, 1);
}

AFXINL void AfxCopyRamp(afxRamp *ramp, afxRamp const* in)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(in);
    ramp->prev = in->prev;
    ramp->next = in->next;
}

AFXINL void AfxScaleRamp(afxRamp *ramp, afxRamp const *rhs)
{
    afxError err = NIL;
    AfxAssert(ramp);
    AfxAssert(rhs);
    ramp->prev *= rhs->prev;
    ramp->next *= rhs->next;
}

AFXINL afxRamp AfxMergeRamps(afxRamp const* lhs, afxRamp const* rhs)
{
    afxRamp rslt;
    AfxCopyRamp(&rslt, lhs);
    AfxScaleRamp(&rslt, rhs);
    return rslt;
}

AFX_DEFINE_STRUCT(afxChannelConfig)
{
    afxNat32 uNumChannels : 8;   // the number of channels, identified (deduced from channel mask) or anonymous (set directly). 
    afxNat32 eConfigType : 4;    // a code that completes the identification of channels by uChannelMask.
    afxNat32 uChannelMask : 20; // a bit field, whose channel identifiers depend on afxChannelConfigType (up to 20). Channel bits are defined in Speaker Config.
};



/// This structure allows the game to provide audio files to fill the external sources.
/// You can specify a streaming file or a file in-memory, regardless of the "Stream" option.
/// The only file format accepted is a fully formed WEM file, as converted by Wwise.
/// Make sure that only one of szFile, pInMemory or idFile is non-null. if both idFile and szFile are set, idFile is passed to low-level IO and szFile is used as stream name (for profiling purposes).
/// When using the in-memory file (pInMemory & uiMemorySize), it is the responsibility of the game to ensure the memory stays valid for the entire duration of the playback. 
/// You can achieve this by using the AK_EndOfEvent callback to track when the Event ends.

typedef afxNat32 afxCodecID;
typedef afxNat32 afxFileID;

AFX_DEFINE_STRUCT(afxExternalSourceInfo)
{
    afxNat32 iExternalSrcCookie;    ///< Cookie identifying the source, given by hashing the name of the source given in the project.  See \ref AK::SoundEngine::GetIDFromString. \aknote If an event triggers the playback of more than one external source, they must be named uniquely in the project therefore have a unique cookie) in order to tell them apart when filling the AkExternalSourceInfo structures. \endaknote
    afxCodecID idCodec;              ///< Codec ID for the file.  One of the audio formats defined in AkTypes.h (AKCODECID_XXX)
    char* szFile;              ///< File path for the source.  If not NULL, the source will be streaming from disk. Set pInMemory to NULL. If idFile is set, this field is used as stream name (for profiling purposes). /// The only file format accepted is a fully formed WEM file, as converted by Wwise.
    void* pInMemory;                ///< Pointer to the in-memory file.  If not NULL, the source will be read from memory. Set szFile and idFile to NULL. The only file format accepted is a fully formed WEM file, as converted by Wwise.
    afxNat32 uiMemorySize;          ///< Size of the data pointed by pInMemory
    afxFileID idFile;                ///< File ID.  If not zero, the source will be streaming from disk.  This ID can be anything.  Note that you must override the low-level IO to resolve this ID to a real file.  See \ref streamingmanager_lowlevel for more information on overriding the Low Level IO.
};

AFXINL void AfxResetExternalSourceInfo(afxExternalSourceInfo *src)
{
    src->iExternalSrcCookie;
    src->idCodec;
    src->szFile;
    src->pInMemory;
    src->uiMemorySize;
    src->idFile;
}

AFXINL void AfxAcquireExternalSourceInfoFromMemory(afxExternalSourceInfo *src, void* mem, afxNat32 siz, afxNat32 extlSrcCookie, afxCodecID idCodec)
{
    // specify source by memory.
    src->iExternalSrcCookie = extlSrcCookie;
    src->idCodec = idCodec;
    src->szFile = NIL;
    src->pInMemory = mem;
    src->uiMemorySize = siz;
    src->idFile = 0;
}


AFXINL void AfxAcquireExternalSourceInfoFromUri(afxExternalSourceInfo *src, char *fname, afxNat extlSrcCookie, afxCodecID idCodec)
{
    // specify source by streaming file name.

    src->iExternalSrcCookie = extlSrcCookie;
    src->idCodec = idCodec;
    src->szFile = fname;
    src->pInMemory = NIL;
    src->uiMemorySize = 0;
    src->idFile = 0;
}


AFXINL void AfxAcquireExternalSourceInfoFromFile(afxExternalSourceInfo *src, afxFileID idFile, afxNat32 extlSrcCookie, afxCodecID idCodec)
{
    // specify source by streaming file ID.

    src->iExternalSrcCookie = extlSrcCookie;
    src->idCodec = idCodec;
    src->idFile = 0;
    src->pInMemory = 0;
    src->uiMemorySize = 0;
    src->idFile = idFile;
}

#endif//AFX_SOUND_DEFS_H