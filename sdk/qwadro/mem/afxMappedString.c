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
#include "qwadro/core/afxSystem.h"

_AFX afxNat AfxResolveStrings2(afxStringBase strc, afxNat cnt, afxString const in[], afxString out[])
{
    afxError err = NIL;
    AfxTryAssertObjects(1, &strc, afxFcc_STRB);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (!in || !in[i].len)
            AfxResetString(&out[i]);
        else
            AfxMakeString(&out[i], &strc->buf.bytemap[in[i].offset], in[i].len), ++rslt;
    }
    return rslt;
}

_AFX afxNat AfxCatalogStrings2(afxStringBase strc, afxNat cnt, afxString const in[], afxString out[])
{
    afxError err = NIL;
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxString const* src = &in[i];
        afxNat srcLen = src->len;

        if (srcLen)
        {
            void const* dataStart = NIL;

            if (strc->buf.cnt)
                dataStart = AfxStrnstr(strc->buf.data, strc->buf.cnt, src->start, srcLen);

            if (!dataStart)
            {
                afxNat firstCh;

                if (!(dataStart = AfxInsertArrayUnits(&strc->buf, srcLen, &firstCh, NIL))) AfxThrowError();
                else
                {
                    AfxCopy2(srcLen, sizeof(afxChar), src->start, (void*)dataStart);
                }
            }

            if (!err)
            {
                AfxMakeString(&out[i], (void*)((afxSize)dataStart - (afxSize)strc->buf.data), srcLen);
                out[i].offset = (afxSize)dataStart - (afxSize)strc->buf.data;
                out[i].len = srcLen;
                ++strc->cnt;
                ++rslt;
            }
        }
    }
    return rslt;
}

_AFX afxError _AfxStrcCtor(afxStringBase strc, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &strc, afxFcc_STRB);
    
    strc->cnt = 0;
    strc->strings = NIL;
    AfxAllocateArray(&strc->buf, 1, sizeof(afxChar), NIL);

    return err;
}

_AFX afxError _AfxStrcDtor(afxStringBase strc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &strc, afxFcc_STRB);

    if (strc->strings)
        AfxDeallocate(strc->strings);

    AfxDeallocateArray(&strc->buf);

    return err;
}

_AFX afxClassConfig _AfxStrbMgrCfg =
{
    .fcc = afxFcc_STRB,
    .name = "String Base",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxStringBase)),
    .mmu = NIL,
    .ctor = (void*)_AfxStrcCtor,
    .dtor = (void*)_AfxStrcDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireStringCatalogs(afxNat cnt, afxStringBase catalogs[])
{
    afxError err = NIL;
    AfxAssert(catalogs);
    AfxAssert(cnt);

    if (AfxAcquireObjects(AfxGetStringBaseClass(), cnt, (afxObject*)catalogs, (void const*[]) {  }))
        AfxThrowError();

    return err;
}
