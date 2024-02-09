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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AAX_BANK_H
#define AAX_BANK_H

#include "qwadro/sound/afxSoundDefs.h"

AFX_OBJECT(aaxBank)
{
    afxBool loaded;
};

typedef afxNat32 aaxBankId;
typedef afxNat32 aaxBankType;
typedef afxInt8 aaxPriority;
typedef afxInt32 aaxMemPoolId;

AFX_CALLBACK(void, aaxBankCallbackFunc)(afxNat32 in_bankID, void const* in_pInMemoryBankPtr, afxResult in_eLoadResult, void* in_pCookie);

typedef enum aaxPreparationType
{
    Preparation_Load,   ///< \c PrepareEvent() will load required information to play the specified event.
    Preparation_Unload, ///< \c PrepareEvent() will unload required information to play the specified event.
    Preparation_LoadAndDecode ///< Vorbis media is decoded when loading, and an uncompressed PCM version is used for playback.
} aaxPreparationType;

typedef enum aaxBankContent
{
    AkBankContent_StructureOnly,    ///< Use AkBankContent_StructureOnly to load only the structural content, including Events, and then later use the PrepareEvent() functions to load media on demand from loose files on the disk.
    AkBankContent_All               ///< Use AkBankContent_All to load both the media and structural content.
} aaxBankContent;



#endif//AAX_BANK_H
