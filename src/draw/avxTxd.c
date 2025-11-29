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

// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_TXD_C
#include "avxIcd.h"

#ifdef _AVX_TXD_C

AFX_DEFINE_STRUCT(_avxTxdTex)
{
    afxLink     txd;
    afxString32 name;
    afxUnit     reqCnt;
    avxRaster   ras;
    afxUnit     pagId;
};

AFX_DEFINE_STRUCT(_avxTxdPage)
{
    int a;
};

AFX_OBJECT(avxTxd)
{
    afxUnit         maxPagSizX; // AFX_I16_MAX / 2;
    afxUnit         maxPagSizY; // AFX_I16_MAX / 2;
    afxUnit         maxPagSizZ;
    
    afxUnit         urlCnt;
    afxUri128       url;

    afxChain        texs;
};

#endif//_AVX_TXD_C

_AVX afxCmdId AvxCmdUseTxds(afxDrawContext dctx, afxUnit baseSlotIdx, afxUnit cnt, avxTxd txds[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    return 0;
}

_AVX afxCmdId AvxCmdBindTextures(afxDrawContext dctx, afxUnit txdIdx, afxUnit cnt, afxUnit const indices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);
    AFX_ASSERT(indices);

    return 0;
}

_AVX afxError AvxAddTextures(avxTxd txd, afxUnit cnt, afxString const names[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, &txd);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxString const* n = &names[i];

        if (AfxIsStringEmpty(n))
            continue;

        afxBool found = FALSE;
        _avxTxdTex* tex;
        AFX_ITERATE_CHAIN(tex, txd, &txd->texs)
        {
            if (AfxCompareStrings(n, 0, FALSE, 1, &tex->name.s, NIL))
            {
                found = TRUE;
                break;
            }
        }

        // Skip if already exist one present with such name
        if (found)
            continue;

        if (AfxAllocate(sizeof(*tex), 0, AfxHere(), (void**)&tex))
        {
            AfxThrowError();
            break;
        }

        *tex = (_avxTxdTex) { 0 };
        AfxPushLink(&tex->txd, &txd->texs);
        AfxMakeString32(&tex->name, n);
    }
    return err;
}

_AVX afxError AvxFindTextures(avxTxd txd, afxUnit cnt, afxString const names[], afxUnit indices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, &txd);

    afxUnit foundCnt = 0;
    afxUnit i = 0;
    _avxTxdTex* tex;
    AFX_ITERATE_CHAIN(tex, txd, &txd->texs)
    {
        afxUnit sIdx;
        if (AfxCompareStrings(&tex->name.s, 0, FALSE, cnt, names, &sIdx))
        {
            indices[sIdx] = i;
            ++foundCnt;

            if (foundCnt >= cnt)
                break;
        }
        ++i;
    }
    return err;
}

_AVX afxError AvxRequestTextures(avxTxd txd, afxUnit cnt, afxUnit indices[])
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, &txd);

    afxUnit foundCnt = 0;
    afxUnit i = 0;
    _avxTxdTex* tex;
    AFX_ITERATE_CHAIN(tex, txd, &txd->texs)
    {
        afxBool found = FALSE;
        for (afxUnit j = 0; j < cnt; j++)
        {
            if (indices[j] == i)
            {
                found = TRUE;
                ++foundCnt;
            }
        }

        if (found)
        {
            if (!tex->ras)
            {
                afxUri2048 urib;
                AfxMakeUri2048(&urib, NIL);
                AfxFormatUri(&urib.uri, "%.*s/%.*s.tga", AfxPushString(&txd->url.uri.s), AfxPushString(&tex->name.s));

                avxRasterInfo rasi = { 0 };

                if (AvxLoadRasters(AfxGetHost(txd), 1, &rasi, &urib.uri, NIL, &tex->ras))
                    AfxThrowError();
            }
            ++tex->reqCnt;
        }

        ++i;
        if (foundCnt >= cnt) break;
    }
    return err;
}

_AVX afxError _AvxTxdDtorCb(avxTxd txd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, &txd);

    return err;
}

_AVX afxError _AvxTxdCtorCb(avxTxd txd, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, &txd);

    afxDrawSystem dsys = args[0];
    avxTxdInfo const* txdi = AFX_CAST(avxTxdInfo const*, args[1]) + invokeNo;

    AfxMakeUri128(&txd->url, txdi->url);



    return err;
}

_AVX afxClassConfig const _AVX_CLASS_CONFIG_TXD =
{
    .fcc = afxFcc_TXD,
    .name = "Txd",
    .desc = "Texture Exchange Dictionary",
    .fixedSiz = sizeof(AFX_OBJECT(avxTxd)),
    .ctor = (void*)_AvxTxdCtorCb,
    .dtor = (void*)_AvxTxdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AvxAcquireTxd(afxDrawSystem dsys, avxTxdInfo const* info, avxTxd* txd)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    afxClass* cls = (afxClass*)_AvxDsysGetDdi(dsys)->txdCls(dsys);
    AFX_ASSERT_CLASS(cls, afxFcc_TXD);

    if (AfxAcquireObjects(cls, 1, (afxObject*)txd, (void const*[]) { dsys, info }))
    {
        AfxThrowError();
        return err;
    }

    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, txd);

    return err;
}

_AVX afxBool GetDirTexNamesCb(afxArray* files, afxUri const* path, afxUri const* osPath)
{
    afxUri fname;
    AfxExcerptPathSegments(path, NIL, NIL, &fname, NIL);

    if (AFX_MAX_TEXTURE_NAME_LENGTH > fname.s.len) // can't be greater than 32
    {
        afxUnit arrel;
        afxString32* name = AfxPushArrayUnits(files, 1, &arrel, NIL, 0);
        AfxMakeString32(name, (afxString[]) { { .len = fname.s.len, .start = fname.s.start } } );
    }
    return TRUE;
}

_AVX afxError AvxOpenTxd(afxDrawSystem dsys, afxUri const* url, avxTxd* dict)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);
    afxUri urlBase;
    
    if (AfxIsUriBlank(url) || (0 == AfxExcerptPathBase(url, &urlBase))) AfxThrowError();
    else
    {
        afxUri2048 urib;
        AfxMakeUri2048(&urib, NIL);
        AfxFormatUri(&urib.uri, "%.*s/*.tga", AfxPushString(&urlBase.s));

        afxArray files;
        AfxMakeArray(&files, sizeof(afxString32), 8, NIL, 0);

        afxUnit rasCnt = AfxFindFiles(&urib.uri, afxFileFlag_R, (void*)GetDirTexNamesCb, &files);

        avxTxdInfo txdi = { 0 };
        txdi.url = &urlBase;
        txdi.texCnt = files.pop;

        avxTxd txd;
        AvxAcquireTxd(dsys, &txdi, &txd);

        for (afxUnit i = 0; i < files.pop; i++)
        {
            afxString32* str = AfxGetArrayUnit(&files, i);
            AvxAddTextures(txd, 1, &str->s);
        }

        AfxEmptyArray(&files, FALSE, FALSE);
        *dict = txd;
    }
    return err;
}
