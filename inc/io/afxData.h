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

#ifndef AFX_DATA_H
#define AFX_DATA_H

#include "qwadro/inc/io/afxStream.h"

typedef enum afxCompressionType
{
    //NONE // no compressed
    AFX_COMPRESSION_TYPE_RLE = 1,
} afxCompressionType;

typedef enum afxCodecId
{
    afxCodecId_RAW,
    afxCodecId_RLE,
} afxCodecId;

#ifdef _AFX_CORE_C
#ifdef _AFX_CODEC_C
AFX_OBJECT(afxCodec)
{
    afxError(*enc)(afxStream stream, afxNat len, afxNat byteCnt, afxByte const* src);
    afxError(*dec)(afxStream stream, afxNat len, afxNat byteCnt, afxByte* dst);
};
#endif//_AFX_CODEC_C
#endif//_AFX_CORE_C

AFX afxNat  AfxGetCompressionPaddingSize(afxCompressionType type);

AFX afxBool AfxDecompressData(afxCompressionType type, afxBool fileIsByteReversed, afxNat compressedBytesSiz, void *compressedBytes, afxInt stop0, afxInt stop1, afxInt stop2, void *decompressedBytes);

AFX afxError    AfxEncode(afxCodec cdc, void const* src, afxNat srcLen, void* dst, afxNat dstLen);
AFX afxError    AfxDecode(afxCodec cdc, void const* src, afxNat srcLen, void* dst, afxNat dstLen);

//AFX void        AfxDecodeStream(afxStream stream, afxNat len, afxNat byteCnt, afxByte *dst);

#endif//AFX_DATA_H
