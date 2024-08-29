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

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_SYSTEM_C
#define _AUX_UX_C
#define _AUX_HID_C
#include "../dev/AuxImplKit.h"

_AUX afxClass const* AfxGetHidClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AfxAssertObjects(1, &sys, afxFcc_SYS);
    afxClass const* cls = &sys->aux.hidCls;
    AfxAssertClass(cls, afxFcc_HID);
    return cls;
}

////////////////////////////////////////////////////////////////////////////////
// HID HANDLING                                                               //
////////////////////////////////////////////////////////////////////////////////

_AUXINL void* AfxGetHidIdd(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &hid, afxFcc_HID);
    return hid->idd;
}

_AUXINL afxBool AfxIsHidPrompt(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &hid, afxFcc_HID);
    return hid->dev.serving;
}

_AUXINL afxNat AfxGetHidPort(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    return hid->port;
}

_AUXINL afxResult AfxTestHidFlags(afxHid hid, afxHidFlag flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(hid, afxFcc_HID);
    AfxAssert(flags);
    return (hid->flags & flags);
}

_AUX afxError _AuxHidDtorCb(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &hid, afxFcc_HID);

    AfxCleanUpChainedClasses(&hid->dev.classes);

    return err;
}

_AUX afxResult _AuxHidCtorCb(afxHid hid, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &hid, afxFcc_HID);

    afxDriver icd = args[0];
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    afxHidInfo const* info = (afxHidInfo const*)(args[1]) + invokeNo;

    if (_AfxDevBaseImplementation.ctor(&hid->dev, (void*[]) { icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        afxNat port = 0;
        hid->port = port;
        hid->flags = NIL;
    }
    return err;
}

_AUX afxClassConfig const _AuxHidStdImplementation =
{
    .fcc = afxFcc_HID,
    .name = "HID",
    .desc = "Human Input Device Interface",
    .fixedSiz = sizeof(AFX_OBJECT(afxHid)),
    .ctor = (void*)_AuxHidCtorCb,
    .dtor = (void*)_AuxHidDtorCb
};

////////////////////////////////////////////////////////////////////////////////
// HID DISCOVERY                                                              //
////////////////////////////////////////////////////////////////////////////////

_AUX afxNat AfxInvokeHids(afxNat first, afxNat cnt, afxBool(*f)(afxHid, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass const* cls = AfxGetHidClass();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxInvokeClassInstances(cls, first, cnt, (void*)f, udd) : 0;
}

_AUX afxNat AfxEvokeHids(afxBool(*flt)(afxHid, void*), void* fdd, afxNat first, afxNat cnt, afxHid hids[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(hids);
    AfxAssert(flt);
    AfxAssert(cnt);
    afxClass const* cls = AfxGetHidClass();
    AfxAssertClass(cls, afxFcc_HID);
    return AfxEvokeClassInstances(cls, (void*)flt, fdd, first, cnt, (afxObject*)hids);
}

_AUX afxNat AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hids[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(hids);
    afxClass const* cls = AfxGetHidClass();
    AfxAssertClass(cls, afxFcc_HID);
    return cnt ? AfxEnumerateClassInstances(cls, first, cnt, (afxObject*)hids) : 0;
}

_AUX afxBool AfxGetHid(afxNat port, afxHid* hid)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;
    afxNat i = 0;
    afxHid hid2;
    while (AfxEnumerateHids(i, 1, &hid2))
    {
        AfxAssertObjects(1, &hid2, afxFcc_HID);

        if (hid2->port == port)
        {
            found = TRUE;
            break;
        }
        ++i;
    }
    AfxAssert(hid);
    *hid = found ? hid2 : NIL;
    return found;
}

_AUX afxError _AuxRegisterHids(afxDriver icd, afxNat cnt, afxHidInfo const infos[], afxHid hids[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &icd, afxFcc_ICD);
    AfxAssert(hids);

    afxClass* cls = (afxClass*)AfxGetHidClass();
    AfxAssertClass(cls, afxFcc_HID);

    AfxEnterSlockExclusive(&cls->poolLock);

    if (cls->pool.totalUsedCnt >= cls->maxInstCnt) AfxThrowError();
    else
    {
        if (_AfxAllocateClassInstances(cls, cnt, (afxObject*)hids)) AfxThrowError();
        else
        {
            if (_AfxConstructClassInstances(cls, cnt, (afxObject*)hids, (void*[]) { icd, (void*)infos }))
            {
                AfxThrowError();
            }

            if (!err)
            {
                AfxAssertObjects(cnt, hids, afxFcc_HID);
            }
            else
            {
                _AfxDeallocateClassInstances(cls, cnt, (afxObject*)hids);
            }
        }
    }

    AfxExitSlockExclusive(&cls->poolLock);

    return err;
}
