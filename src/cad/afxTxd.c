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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#define _AVX_TXD_C
#include "../dev/AvxImplKit.h"
#include "qwadro/inc/cad/afxTxd.h"
#include <d3d9.h>

#ifdef _AVX_TXD_C

AFX_OBJECT(afxTxd)
{
    afxUri128           url;
    afxStringBase       strb;
    afxUnit              texCnt;
    afxString*          texIds; // nested names to speed up searches.
    afxUnit*             texRefCnt;
    afxRaster*          texRasters;
};

#endif//_AVX_TXD_C

_AVX afxError AfxRenameTextures(afxTxd txd, afxUnit first, afxUnit cnt, afxString const* name)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txd, afxFcc_TXD);

    return err;
}

_AVX afxError AfxRequestTextures(afxTxd txd, afxUnit cnt, afxUnit texIdxes[], afxRaster rasters[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txd, afxFcc_TXD);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxUnit texIdx = texIdxes[i];

        if (!txd->texRasters[texIdx])
        {
            afxDrawContext dctx;
            afxDrawInput din = AfxGetProvider(txd);
            AfxAssertObjects(1, &din, afxFcc_DIN);
            AfxGetDrawInputContext(din, &dctx);

            afxUri2048 urib;
            AfxMakeUri2048(&urib, NIL);
            AfxFormatUri(&urib.uri, "%.*s/%.*s.tga", AfxPushString(&txd->url.uri.str), AfxPushString(&txd->texIds[texIdx]));

            afxRasterInfo rasi = { 0 };

            if (AfxLoadRasters(dctx, 1, &rasi, &urib.uri, &txd->texRasters[texIdx]))
                AfxThrowError();
        }

        if (!err)
            txd->texRefCnt[texIdx] += 1;
    }
    return err;
}

_AVX afxError _AvxTxdStdDtorCb(afxTxd txd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txd, afxFcc_TXD);

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

_AVX afxError _AvxTxdStdCtorCb(afxTxd txd, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &txd, afxFcc_TXD);

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
        afxRaster rasters[256];
        avxSampler samplers[256];
#if 0
        if (AfxAcquireRasters(dctx, texCnt, txdi->rasters, rasters))
            AfxThrowError();

        if (AfxAcquireSamplers(dctx, texCnt, txdi->samplers, samplers))
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

_AVX afxClassConfig const _AvxTexClsCfg =
{
    .fcc = afxFcc_TEX,
    .name = "Texture",
    .desc = "Texture"
};

_AVX afxClassConfig const _AvxTxdClsCfg =
{
    .fcc = afxFcc_TXD,
    .name = "Txd",
    .desc = "Texture Dictionary",
    .fixedSiz = sizeof(AFX_OBJECT(afxTxd)),
    .ctor = (void*)_AvxTxdStdCtorCb,
    .dtor = (void*)_AvxTxdStdDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AVX afxError AfxAcquireTxd(afxDrawInput din, avxTxdInfo const* info, afxTxd* txd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    afxClass* cls = (afxClass*)AfxGetTxdClass(din);
    AfxAssertClass(cls, afxFcc_TXD);

    if (AfxAcquireObjects(cls, 1, (afxObject*)txd, (void const*[]) { din, info }))
        AfxThrowError();

    AfxAssertObjects(1, txd, afxFcc_TXD);

    return err;
}

_AVX afxBool GetDirTexNamesCb(afxArray* files, afxUri const* path, afxUri const* osPath)
{
    afxUri fname;
    AfxExcerptPathSegments(path, NIL, NIL, &fname, NIL);

    if (AFX_MAX_TEX_ID_LEN > fname.str.len) // can't be greater than 32
    {
        afxUnit arrel;
        afxString* name = AfxInsertArrayUnit(files, &arrel);
        AfxAllocateString(name, fname.str.len, fname.str.start, fname.str.len);
    }
    return TRUE;
}

_AVX afxError AfxOpenTxd(afxDrawInput din, afxUri const* url, afxTxd* dict)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxUri urlBase;
    
    if (AfxIsUriBlank(url) || 0 == AfxExcerptPathBase(url, &urlBase)) AfxThrowError();
    else
    {
        afxUri2048 urib;
        AfxMakeUri2048(&urib, NIL);
        AfxFormatUri(&urib.uri, "%.*s/*.tga", AfxPushString(&urlBase.str));

        afxArray files;
        AfxMakeArray(&files, 8, sizeof(afxString), NIL);

        afxUnit rasCnt = AfxFindFiles(&urib.uri, afxFileFlag_R, (void*)GetDirTexNamesCb, &files);

        avxTxdInfo txdi = { 0 };
        txdi.url = &urlBase;
        txdi.texCnt = files.pop;

        afxTxd txd;
        AfxAcquireTxd(din, &txdi, &txd);

        afxDrawContext dctx;
        AfxGetDrawInputContext(din, &dctx);

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
