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

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _AFX_CORE_C
#define _AFX_CODEC_C
#include "../dev/afxDevIoBase.h"

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

_AFX void _AfxDecodeRle(afxStream stream, afxNat len, afxNat byteCnt, afxByte *dst)
{
    afxNat siz = len * byteCnt;
    afxNat currByte = 0;

    while (currByte < siz)
    {
        afxByte hdr;
        AfxReadStream(stream, sizeof(hdr), 0, &hdr);

        afxNat i, runLen = (hdr & 0x7F) + 1;

        if (hdr & 0x80)
        {
            afxByte buf[4];
            AfxReadStream(stream, sizeof(afxByte) * byteCnt, 0, buf);

            for (i = 0; i < runLen; i++)
            {
                AfxCopy2(&dst[currByte], buf, sizeof(afxByte), byteCnt);
                currByte += byteCnt;
            }
        }
        else
        {
            for (i = 0; i < runLen; i++)
            {
                AfxReadStream(stream, sizeof(afxByte) * byteCnt, 0, &dst[currByte]);
                currByte += byteCnt;
            }
        }
    }
}

_AFX afxError _AfxCdcCtor(afxCodec cdc, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cdc, afxFcc_CDC);

    afxSystem sys = args[0];

    return err;
}

_AFX afxError _AfxCdcDtor(afxCodec cdc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &cdc, afxFcc_CDC);

    //afxMmu mmu = AfxGetDrawSystemMmu();

    //AfxReleaseObjects(1, (void*[]) { ddrv->mdle });

    return err;
}

_AFX afxClassConfig _AfxCdcMgrCfg =
{
    .fcc = afxFcc_CDC,
    .name = "Codec",
    .fixedSiz = sizeof(AFX_OBJECT(afxCodec)),
    .ctor = (void*)_AfxCdcCtor,
    .dtor = (void*)_AfxCdcDtor
};

////////////////////////////////////////////////////////////////////////////////
