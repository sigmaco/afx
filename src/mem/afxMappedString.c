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

#include "../impl/afxExecImplKit.h"

_AFX afxUnit AfxDecatalogStrings(afxStringBase strc, afxUnit cnt, afxString const strings[])
{
    afxError err = NIL;
    afxUnit rslt = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        if (strings[i].len)
        {
            afxReferencedString* ref;
            AFX_ITERATE_CHAIN(afxReferencedString, ref, strb, &strc->strings)
            {
                if (AfxCompareStrings(&ref->str, 0, TRUE, 1, &strings[i], NIL))
                {
                    if (0 == (--ref->refCnt))
                    {
                        AfxPopLink(&ref->strb);
                        AfxDeallocate((void**)&ref, AfxHere());
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
            AFX_ITERATE_CHAIN(afxReferencedString, ref, strb, &strc->strings)
            {
                if (AfxCompareStrings(&ref->str, 0, FALSE, 1, &in[i], NIL))
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
                AfxAllocate(sizeof(*ref) + (sizeof(ref->data[0]) * in[i].len), 0, AfxHere(), (void**)&ref);

                if (!ref)
                {
                    AfxThrowError();
                    AfxResetStrings(1, &out[i]);
                }
                else
                {
                    AfxPushLink(&ref->strb, &strc->strings);
                    AfxMakeString(&ref->str, in[i].len, ref->data, in[i].len);
                    AfxCopyString(&ref->str, 0, &in[i], 0);
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
    AFX_ASSERT_OBJECTS(afxFcc_STRB, 1, &strc);
    
    AfxDeployChain(&strc->strings, strc);
    strc->first = NIL;

    //AfxDeployPool(&strc, sizeof(afxMappedString), 32);

    return err;
}

_AFX afxError _AfxStrcDtor(afxStringBase strc)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_STRB, 1, &strc);

    afxUnit rslt = 0;
    afxReferencedString* ref;
    AFX_ITERATE_CHAIN(afxReferencedString, ref, strb, &strc->strings)
    {
        AfxPopLink(&ref->strb);
        AfxDeallocate((void**)&ref, AfxHere());
        ++rslt;
    }

    return err;
}

_AFX afxClassConfig const _AFX_STRB_CLASS_CONFIG =
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

    afxSystem sys;
    AfxGetSystem(&sys);

    if (AfxAcquireObjects(_AfxSysGetStrbClass(sys), cnt, (afxObject*)catalogs, (void const*[]) { 0 }))
        AfxThrowError();

    return err;
}
