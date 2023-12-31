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

#include "qwadro/core/afxData.h"

_AFX afxNat AfxGetCompressionPaddingSize(afxCompressionType type)
{
    afxError err = NIL;
    AfxAssert(type == NIL);

    switch (type)
    {
    case (afxCompressionType)NIL:
    {
        break;
    }
    default: AfxThrowError(); break;
    }

    return 0;
}

_AFX afxBool AfxDecompressData(afxCompressionType type, afxBool fileIsByteReversed, afxNat compressedBytesSiz, void *compressedBytes, afxInt stop0, afxInt stop1, afxInt stop2, void *decompressedBytes)
{
    afxError err = NIL;
    AfxAssert(type == NIL);

    switch (type)
    {
    case (afxCompressionType)NIL:
    {
        afxInt v10 = stop2;

        for (afxByte *i = compressedBytes; v10; --v10)
        {
            i[(afxByte*)decompressedBytes - (afxByte*)compressedBytes] = *i;
            ++i;
        }
        return TRUE;
        break;
    }
    default: AfxThrowError(); break;
    }

    return FALSE;
}
