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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ASX_BANK_H
#define ASX_BANK_H

#include "qwadro/sound/afxSoundDefs.h"

AFX_OBJECT(asxBank)
{
    afxBool loaded;
};

typedef afxNat32 asxBankId;
typedef afxNat32 asxBankType;
typedef afxInt8 asxPriority;
typedef afxInt32 asxMemPoolId;

AFX_CALLBACK(void, asxBankCallbackFunc)(afxNat32 in_bankID, void const* in_pInMemoryBankPtr, afxResult in_eLoadResult, void* in_pCookie);

typedef enum asxPreparationType
{
    Preparation_Load,   ///< \c PrepareEvent() will load required information to play the specified event.
    Preparation_Unload, ///< \c PrepareEvent() will unload required information to play the specified event.
    Preparation_LoadAndDecode ///< Vorbis media is decoded when loading, and an uncompressed PCM version is used for playback.
} asxPreparationType;

typedef enum asxBankContent
{
    AkBankContent_StructureOnly,    ///< Use AkBankContent_StructureOnly to load only the structural content, including Events, and then later use the PrepareEvent() functions to load media on demand from loose files on the disk.
    AkBankContent_All               ///< Use AkBankContent_All to load both the media and structural content.
} asxBankContent;



#endif//ASX_BANK_H
