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

#define _AFX_CORE_C
#define _AFX_MODULE_C
#define _AFX_ICD_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/draw/afxDrawSystem.h"


_AFX afxError _AfxIcdCtor(afxIcd icd, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);

    afxSystem sys = cookie->udd[0];

    afxModule mdle = &icd->mdle;
    afxError (*IcdHookPoint)(afxIcd icd) = AfxFindModuleSymbol(mdle, "AfxIcdHookPoint");

    if (!IcdHookPoint) AfxThrowError();
    else
    {
        if (IcdHookPoint(icd)) AfxThrowError();
        else
        {


        }
    }
    return err;
}

_AFX afxError _AfxIcdDtor(afxIcd icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);

    //afxMmu mmu = AfxGetDrawSystemMmu();

    _AfxUninstallChainedClasses(&icd->classes);

    //AfxReleaseObjects(1, (void*[]) { ddrv->mdle });

    return err;
}

_AFX afxClassConfig _AfxIcdClsConfig =
{
    .fcc = afxFcc_ICD,
    .name = "Installable Client Driver",
    .unitsPerPage = 2,
    .size = sizeof(AFX_OBJECT(afxIcd)),
    .mmu = NIL,
    .ctor = (void*)_AfxIcdCtor,
    .dtor = (void*)_AfxIcdDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError _AfxSysLoadIcd(afxSystem sys, afxUri const* file, afxIcd *icd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    AfxAssertType(file, afxFcc_URI);
    AfxComment("Loading draw ICD %.*s", AfxPushString(AfxGetUriString(file)));

    afxUri file2;
    AfxGetUriObject(&file2, file);
    afxIcd icd2;

    if ((icd2 = AfxFindIcd(&file2))) AfxReacquireObjects(1, (void*[]) { icd2 });
    else
    {
        afxClass* cls = AfxGetIcdClass();
        AfxAssertClass(cls, afxFcc_ICD);

        if (AfxAcquireObjects(cls, 1, (afxObject*)&icd2, (void const*[]) { sys, file })) AfxThrowError();
        else
        {
            AfxAssertObjects(1, &icd2, afxFcc_ICD);

            if (err)
                AfxReleaseObjects(1, (void*[]) { icd2 });
        }
    }

    if (!err)
        *icd = icd2;

    return err;
}

_AFX afxNat AfxInvokeIcds(afxNat first, afxNat cnt, afxBool(*f)(afxIcd, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetIcdClass();
    AfxAssertClass(cls, afxFcc_ICD);
    return cnt ? AfxInvokeInstances(cls, first, cnt, (void*)f, udd) : 0;
}

_AFX afxNat AfxEnumerateIcds(afxNat first, afxNat cnt, afxIcd icd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(icd);
    afxClass* cls = AfxGetIcdClass();
    AfxAssertClass(cls, afxFcc_ICD);
    return cnt ? AfxEnumerateInstances(cls, first, cnt, (afxObject*)icd) : 0;
}

_AFX afxNat AfxCountIcds(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetIcdClass();
    AfxAssertClass(cls, afxFcc_ICD);
    return AfxCountInstances(cls);
}

_AFX afxIcd AfxFindIcd(afxUri const *uri)
{
    afxError err = AFX_ERR_NONE;

    afxUri target;
    AfxAssertType(uri, afxFcc_URI);
    AfxGetUriObject(&target, uri);

    afxIcd icd;
    afxNat i = 0;
    while (AfxEnumerateIcds(i, 1, &icd))
    {
        AfxAssertObjects(1, &icd, afxFcc_ICD);

        afxUri tmp;
        AfxGetUriObject(&tmp, AfxGetModulePath(&icd->mdle));

        if (0 == AfxCompareUri(&target, &tmp))
            return icd;

        ++i;
    }
    return NIL;
}
