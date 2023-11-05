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

#ifndef AFX_DATA_H
#define AFX_DATA_H

#include "afx/core/afxStream.h"

typedef enum afxCompressionType
{
    //NONE // no compressed
    AFX_COMPRESSION_TYPE_RLE = 1,
} afxCompressionType;

AFX afxNat  AfxGetCompressionPaddingSize(afxCompressionType type);

AFX afxBool AfxDecompressData(afxCompressionType type, afxBool fileIsByteReversed, afxNat compressedBytesSiz, void *compressedBytes, afxInt stop0, afxInt stop1, afxInt stop2, void *decompressedBytes);

#endif//AFX_DATA_H