/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_TXD_C
#include "../impl/avxImplementation.h"
#include "qwadro/inc/draw/kit/afxTxd.h"
#include <d3d9.h>

#ifdef _AVX_TXD_C

AFX_OBJECT(afxTxd)
{
    afxUri128           url;
    afxStringBase       strb;
    afxUnit              texCnt;
    afxString*          texIds; // nested names to speed up searches.
    afxUnit*             texRefCnt;
    avxRaster*          texRasters;
};

#endif//_AVX_TXD_C

_AVX afxError AfxRenameTextures(afxTxd txd, afxUnit first, afxUnit cnt, afxString const* name)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, &txd);

    return err;
}

_AVX afxError AfxRequestTextures(afxTxd txd, afxUnit cnt, afxUnit texIdxes[], avxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, &txd);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit texIdx = texIdxes[i];

        if (!txd->texRasters[texIdx])
        {
            afxDrawInput din = AfxGetProvider(txd);
            AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
            afxDrawSystem dsys = AfxGetDrawInputContext(din);

            afxUri2048 urib;
            AfxMakeUri2048(&urib, NIL);
            AfxFormatUri(&urib.uri, "%.*s/%.*s.tga", AfxPushString(&txd->url.uri.str), AfxPushString(&txd->texIds[texIdx]));

            avxRasterInfo rasi = { 0 };

            if (AvxLoadRasters(dsys, 1, &rasi, &urib.uri, &txd->texRasters[texIdx]))
                AfxThrowError();
        }

        if (!err)
            txd->texRefCnt[texIdx] += 1;
    }
    return err;
}

_AVX afxError _AvxTxdDtorCb(afxTxd txd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, &txd);

    afxObjectStash const stashes[] =
    {
        {
            .cnt = txd->texCnt,
            .siz = sizeof(txd->texRasters[0]),
            .var = (void**)&txd->texRasters
        },
        {
            .cnt = txd->texCnt,
            .siz = sizeof(txd->texIds[0]),
            .var = (void**)&txd->texIds
        },
        {
            .cnt = txd->texCnt,
            .siz = sizeof(txd->texRefCnt[0]),
            .var = (void**)&txd->texRefCnt
        }
    };
    AfxDeallocateInstanceData(txd, ARRAY_SIZE(stashes), stashes);

    return err;
}

_AVX afxError _AvxTxdCtorCb(afxTxd txd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, &txd);

    avxTxdInfo const *txdi = ((avxTxdInfo const *)args[1]) + invokeNo;

    AfxMakeUri128(&txd->url, txdi->url);

    afxUnit texCnt = txdi->texCnt;
    txd->texCnt = texCnt;

    afxObjectStash const stashes[] =
    {
        {
            .cnt = texCnt,
            .siz = sizeof(txd->texRasters[0]),
            .var = (void**)&txd->texRasters
        },
        {
            .cnt = texCnt,
            .siz = sizeof(txd->texIds[0]),
            .var = (void**)&txd->texIds
        },
        {
            .cnt = texCnt,
            .siz = sizeof(txd->texRefCnt[0]),
            .var = (void**)&txd->texRefCnt
        }
    };

    if (AfxAllocateInstanceData(txd, ARRAY_SIZE(stashes), stashes)) AfxThrowError();
    else
    {
        avxRaster rasters[256];
        avxSampler samplers[256];
#if 0
        if (AvxAcquireRasters(dsys, texCnt, txdi->rasters, rasters))
            AfxThrowError();

        if (AvxDeclareSamplers(dsys, texCnt, txdi->samplers, samplers))
            AfxThrowError();
#endif
        for (afxUnit i = 0; i < texCnt; i++)
        {
            AfxResetStrings(1, &txd->texIds[i]);
            txd->texRasters[i] = NIL;
            txd->texRefCnt = 0;
        }

        AfxAcquireStringCatalogs(1, &txd->strb);

        if (err)
            AfxDeallocateInstanceData(txd, ARRAY_SIZE(stashes), stashes);
    }
    return err;
}

_AVX afxClassConfig const _AVX_TEX_CLASS_CONFIG =
{
    .fcc = afxFcc_TEX,
    .name = "Texture",
    .desc = "Texture"
};

_AVX afxClassConfig const _AVX_TXD_CLASS_CONFIG =
{
    .fcc = afxFcc_TXD,
    .name = "Txd",
    .desc = "Texture Dictionary",
    .fixedSiz = sizeof(AFX_OBJECT(afxTxd)),
    .ctor = (void*)_AvxTxdCtorCb,
    .dtor = (void*)_AvxTxdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireTxd(afxDrawInput din, avxTxdInfo const* info, afxTxd* txd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    afxClass* cls = (afxClass*)AvxGetTxdClass(din);
    AFX_ASSERT_CLASS(cls, afxFcc_TXD);

    if (AfxAcquireObjects(cls, 1, (afxObject*)txd, (void const*[]) { din, info }))
        AfxThrowError();

    AFX_ASSERT_OBJECTS(afxFcc_TXD, 1, txd);

    return err;
}

_AVX afxBool GetDirTexNamesCb(afxArray* files, afxUri const* path, afxUri const* osPath)
{
    afxUri fname;
    AfxExcerptPathSegments(path, NIL, NIL, &fname, NIL);

    if (AFX_MAX_TEX_ID_LEN > fname.str.len) // can't be greater than 32
    {
        afxUnit arrel;
        afxString* name = AfxPushArrayUnit(files, &arrel);
        AfxAllocateString(name, fname.str.len, fname.str.start, fname.str.len);
    }
    return TRUE;
}

_AVX afxError AfxOpenTxd(afxDrawInput din, afxUri const* url, afxTxd* dict)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);
    afxUri urlBase;
    
    if (AfxIsUriBlank(url) || 0 == AfxExcerptPathBase(url, &urlBase)) AfxThrowError();
    else
    {
        afxUri2048 urib;
        AfxMakeUri2048(&urib, NIL);
        AfxFormatUri(&urib.uri, "%.*s/*.tga", AfxPushString(&urlBase.str));

        afxArray files;
        AfxMakeArray(&files, sizeof(afxString), 8, NIL, 0);

        afxUnit rasCnt = AfxFindFiles(&urib.uri, afxFileFlag_R, (void*)GetDirTexNamesCb, &files);

        avxTxdInfo txdi = { 0 };
        txdi.url = &urlBase;
        txdi.texCnt = files.pop;

        afxTxd txd;
        AfxAcquireTxd(din, &txdi, &txd);

        afxDrawSystem dsys = AfxGetDrawInputContext(din);

        AfxCatalogStrings(txd->strb, files.pop, files.data, txd->texIds);

        for (afxUnit i = 0; i < files.pop; i++)
        {
            afxString* str = AfxGetArrayUnit(&files, i);
            AfxDeallocateString(str);
        }

        AfxCleanUpArray(&files);

    }
    return err;
}
