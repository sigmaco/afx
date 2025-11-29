/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA Future Storage <https://sigmaco.org/future-storage>

#define _AFX_CORE_C
#define _AFX_CODEC_C
#include "afxIoDDK.h"

_AFX afxUnit AfxGetCompressionPaddingSize(afxCompressionType type)
{
    afxError err = { 0 };
    AFX_ASSERT(type == NIL);

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

_AFX afxBool AfxDecompressData(afxCompressionType type, afxBool fileIsByteReversed, afxUnit compressedBytesSiz, void *compressedBytes, afxInt stop0, afxInt stop1, afxInt stop2, void *decompressedBytes)
{
    afxError err = { 0 };
    AFX_ASSERT(type == NIL);

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

_AFX void _AfxDecodeRle(afxStream stream, afxUnit len, afxUnit byteCnt, afxByte *dst)
{
    afxUnit siz = len * byteCnt;
    afxUnit currByte = 0;

    while (currByte < siz)
    {
        afxByte hdr;
        AfxReadStream(stream, sizeof(hdr), 0, &hdr);

        afxUnit i, runLen = (hdr & 0x7F) + 1;

        if (hdr & 0x80)
        {
            afxByte buf[4];
            AfxReadStream(stream, sizeof(afxByte) * byteCnt, 0, buf);

            for (i = 0; i < runLen; i++)
            {
                AfxCopy(&dst[currByte], buf,  byteCnt);
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

_AFX afxError _AfxCdcCtor(afxCodec cdc, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CDC, 1, &cdc);

    afxSystem sys = args[0];

    return err;
}

_AFX afxError _AfxCdcDtor(afxCodec cdc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_CDC, 1, &cdc);

    //afxMmu mmu = AfxGetDrawSystemMmu();

    //AfxDisposeObjects(1, (void*[]) { ddrv->mdle });

    return err;
}

_AFX afxClassConfig const _AFX_CDC_CLASS_CONFIG =
{
    .fcc = afxFcc_CDC,
    .name = "Codec",
    .fixedSiz = sizeof(AFX_OBJECT(afxCodec)),
    .ctor = (void*)_AfxCdcCtor,
    .dtor = (void*)_AfxCdcDtor
};

////////////////////////////////////////////////////////////////////////////////
