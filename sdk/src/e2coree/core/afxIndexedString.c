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


#define _CRT_SECURE_NO_WARNINGS 1
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "qwadro/core/afxIndexedString.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/io/afxStream.h"
#include "qwadro/core/afxSystem.h"

_AFX afxError AfxResolveStrings(afxStringCatalog strc, afxNat cnt, afxNat strIdx[], afxString dst[])
{
    afxError err = NIL;

    for (afxNat i = 0; i < cnt; i++)
    {
        dst[i] = strc->strings[strIdx[i]];
        dst[i].start = strc->buf.bytemap + (afxSize)dst[i].start;
    }

    return err;
}

_AFX afxError AfxCatalogStrings(afxStringCatalog strc, afxNat cnt, afxString const src[], afxNat strIdx[], afxNat strIdxStride)
{
    afxError err = NIL;
    
    afxByte* dst = strIdx;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxNat strIdx2 = AFX_INVALID_INDEX;

        if (src->len)
        {
            for (afxNat j = 0; j < strc->cnt; j++)
            {
                afxString tmp;
                AfxResolveStrings(strc, 1, &j, &tmp);

                if (0 == AfxTestStringEquality(src, &tmp))
                {
                    strIdx2 = j;
                    break;
                }
            }

            if (strIdx2 == AFX_INVALID_INDEX)
            {
                void* dataStart = NIL;

                if (strc->buf.cnt)
                    dataStart = AfxStrnstr(strc->buf.data, strc->buf.cnt, src->start, src->len);

                if (!dataStart)
                {
                    afxNat firstCh;

                    if (!(dataStart = AfxInsertArrayUnits(&strc->buf, src->len, &firstCh, NIL))) AfxThrowError();
                    else
                    {
                        AfxCopy(src->len, sizeof(afxChar), src->start, dataStart);
                    }
                }

                if (!err)
                {
                    afxString* strings = AfxReallocate(NIL, strc->strings, sizeof(strc->strings[0]), strc->cnt + 1, 0, AfxHint());

                    if (!strings) AfxThrowError();
                    else
                    {
                        strIdx2 = strc->cnt;
                        AfxMakeString(&strings[strIdx2], (afxSize)dataStart - (afxSize)strc->buf.data, src->len);
                        strc->strings = strings;
                        ++strc->cnt;
                    }
                }
            }
        }

        afxNat* dst2 = dst;
        dst2[0] = strIdx2;
        dst += strIdxStride;

        if (err)
            break;
    }
    return err;
}

_AFX afxError _AfxStrcCtor(afxStringCatalog strc, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &strc, afxFcc_STRC);
    
    strc->cnt = 0;
    strc->strings = NIL;
    AfxAllocateArray(&strc->buf, 1, sizeof(afxChar), NIL);

    return err;
}

_AFX afxError _AfxStrcDtor(afxStringCatalog strc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &strc, afxFcc_STRC);

    if (strc->strings)
        AfxDeallocate(NIL, strc->strings);

    AfxDeallocateArray(&strc->buf);

    return err;
}

_AFX afxClassConfig _AfxStrcClsConfig =
{
    .fcc = afxFcc_STRC,
    .name = "String Catalog",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxStringCatalog)),
    .mmu = NIL,
    .ctor = (void*)_AfxStrcCtor,
    .dtor = (void*)_AfxStrcDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireStringCatalogs(afxNat cnt, afxStringCatalog catalogs[])
{
    afxError err = NIL;
    AfxAssert(catalogs);
    AfxAssert(cnt);

    if (AfxAcquireObjects(AfxGetStringCatalogClass(), cnt, (afxObject*)catalogs, (void const*[]) {  }))
        AfxThrowError();

    return err;
}