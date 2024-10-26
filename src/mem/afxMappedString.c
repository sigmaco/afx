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

#include "../dev/afxExecImplKit.h"

_AFX afxUnit AfxDecatalogStrings(afxStringBase strc, afxUnit cnt, afxString const strings[])
{
    afxError err = NIL;
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (strings[i].len)
        {
            afxReferencedString* ref;
            AfxChainForEveryLinkage(&strc->strings, afxReferencedString, strb, ref)
            {
                if (0 == AfxCompareStrings(&ref->str, TRUE, 1, &strings[i]))
                {
                    if (0 == (--ref->refCnt))
                    {
                        AfxPopLinkage(&ref->strb);
                        AfxDeallocate(ref);
                    }
                    ++rslt;
                    break;
                }
            }
        }
    }
    return rslt;
}

_AFX afxUnit AfxCatalogStrings(afxStringBase strc, afxUnit cnt, afxString const in[], afxString out[])
{
    afxError err = NIL;
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (in[i].len)
        {
            afxBool found = FALSE;

            afxReferencedString* ref;
            AfxChainForEveryLinkage(&strc->strings, afxReferencedString, strb, ref)
            {
                if (0 == AfxCompareStrings(&ref->str, FALSE, 1, &in[i]))
                {
                    out[i] = ref->str;
                    out[i].cap = 0;
                    found = TRUE;
                    ++ref->refCnt;
                    ++rslt;
                    break;
                }
            }

            if (!found)
            {
                ref = AfxAllocate(1, sizeof(*ref) + (sizeof(ref->data[0]) * in[i].len), 0, AfxHere());

                if (!ref)
                {
                    AfxThrowError();
                    AfxResetStrings(1, &out[i]);
                }
                else
                {
                    AfxPushLinkage(&ref->strb, &strc->strings);
                    AfxMakeString(&ref->str, in[i].len, ref->data, in[i].len);
                    AfxCopyString(&ref->str, &in[i]);
                    ref->refCnt = 1;
                    out[i] = ref->str;
                    out[i].cap = 0;
                    ++rslt;
                }
            }
        }
        else
        {
            AfxResetStrings(1, &out[i]);
            ++rslt;
        }
    }
    return rslt;
}

_AFX afxError _AfxStrcCtor(afxStringBase strc, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &strc, afxFcc_STRB);
    
    AfxDeployChain(&strc->strings, strc);
    strc->first = NIL;

    //AfxSetUpPool(&strc, sizeof(afxMappedString), 32);

    return err;
}

_AFX afxError _AfxStrcDtor(afxStringBase strc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &strc, afxFcc_STRB);

    afxUnit rslt = 0;
    afxReferencedString* ref;
    AfxChainForEveryLinkage(&strc->strings, afxReferencedString, strb, ref)
    {
        AfxPopLinkage(&ref->strb);
        AfxDeallocate(ref);
        ++rslt;
    }

    return err;
}

_AFX afxClassConfig _AfxStrbMgrCfg =
{
    .fcc = afxFcc_STRB,
    .name = "String Base",
    .fixedSiz = sizeof(AFX_OBJECT(afxStringBase)),
    .ctor = (void*)_AfxStrcCtor,
    .dtor = (void*)_AfxStrcDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireStringCatalogs(afxUnit cnt, afxStringBase catalogs[])
{
    afxError err = NIL;
    AFX_ASSERT(catalogs);
    AFX_ASSERT(cnt);

    if (AfxAcquireObjects(AfxGetStringBaseClass(), cnt, (afxObject*)catalogs, (void const*[]) { 0 }))
        AfxThrowError();

    return err;
}
