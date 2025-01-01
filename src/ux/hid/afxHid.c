/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *         Q W A D R O   M U L T I M E D I A   U X   I N F R A S T R U C T U R E
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
#include "../impl/auxImplementation.h"

_AUX afxClass const* AfxGetHidClass(void)
{
    afxError err = AFX_ERR_NONE;
    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass const* cls = &sys->aux.hidCls;
    AFX_ASSERT_CLASS(cls, afxFcc_HID);
    return cls;
}

////////////////////////////////////////////////////////////////////////////////
// HID HANDLING                                                               //
////////////////////////////////////////////////////////////////////////////////

_AUXINL void* AfxGetHidIdd(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_HID, 1, &hid);
    return hid->idd;
}

_AUXINL afxBool AfxIsHidPrompt(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_HID, 1, &hid);
    return hid->dev.serving;
}

_AUXINL afxUnit AfxGetHidPort(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    return hid->port;
}

_AUXINL afxResult AfxTestHidFlags(afxHid hid, afxHidFlag flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(hid, afxFcc_HID);
    AFX_ASSERT(flags);
    return (hid->flags & flags);
}

_AUX afxError _AuxHidDtorCb(afxHid hid)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_HID, 1, &hid);

    AfxDeregisterChainedClasses(&hid->dev.classes);

    return err;
}

_AUX afxResult _AuxHidCtorCb(afxHid hid, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_HID, 1, &hid);

    afxModule icd = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    afxHidInfo const* info = (afxHidInfo const*)(args[1]) + invokeNo;

    if (_AfxDevBaseImplementation.ctor(&hid->dev, (void*[]) { icd, (void*)&info->dev }, 0)) AfxThrowError();
    else
    {
        afxUnit port = 0;
        hid->port = port;
        hid->flags = NIL;
    }
    return err;
}

_AUX afxClassConfig const _AUX_HID_CLASS_CONFIG =
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

_AUX afxUnit AfxInvokeHids(afxUnit first, afxUnit cnt, afxBool(*f)(afxHid, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxClass const* cls = AfxGetHidClass();
    AFX_ASSERT_CLASS(cls, afxFcc_HID);
    return cnt ? AfxInvokeObjects(cls, first, cnt, (void*)f, udd) : 0;
}

_AUX afxUnit AfxEvokeHids(afxBool(*flt)(afxHid, void*), void* fdd, afxUnit first, afxUnit cnt, afxHid hids[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(hids);
    AFX_ASSERT(flt);
    AFX_ASSERT(cnt);
    afxClass const* cls = AfxGetHidClass();
    AFX_ASSERT_CLASS(cls, afxFcc_HID);
    return AfxEvokeObjects(cls, (void*)flt, fdd, first, cnt, (afxObject*)hids);
}

_AUX afxUnit AfxEnumerateHids(afxUnit first, afxUnit cnt, afxHid hids[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(cnt);
    AFX_ASSERT(hids);
    afxClass const* cls = AfxGetHidClass();
    AFX_ASSERT_CLASS(cls, afxFcc_HID);
    return cnt ? AfxEnumerateObjects(cls, first, cnt, (afxObject*)hids) : 0;
}

_AUX afxBool AfxGetHid(afxUnit port, afxHid* hid)
{
    afxError err = AFX_ERR_NONE;
    afxBool found = FALSE;
    afxUnit i = 0;
    afxHid hid2;
    while (AfxEnumerateHids(i, 1, &hid2))
    {
        AFX_ASSERT_OBJECTS(afxFcc_HID, 1, &hid2);

        if (hid2->port == port)
        {
            found = TRUE;
            break;
        }
        ++i;
    }
    AFX_ASSERT(hid);
    *hid = found ? hid2 : NIL;
    return found;
}

_AUX afxError _AuxRegisterHids(afxModule icd, afxUnit cnt, afxHidInfo const infos[], afxHid hids[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MDLE, 1, &icd);
    AFX_ASSERT(hids);

    afxClass* cls = (afxClass*)AfxGetHidClass();
    AFX_ASSERT_CLASS(cls, afxFcc_HID);

    AfxEnterSlockExclusive(&cls->poolLock);

    if (cls->pool.totalUsedCnt >= cls->maxInstCnt) AfxThrowError();
    else
    {
        if (_AfxAllocateObjects(cls, cnt, (afxObject*)hids)) AfxThrowError();
        else
        {
            if (_AfxConstructObjects(cls, cnt, (afxObject*)hids, (void*[]) { icd, (void*)infos }))
            {
                AfxThrowError();
            }

            if (!err)
            {
                AFX_ASSERT_OBJECTS(afxFcc_HID, cnt, hids);
            }
            else
            {
                _AfxDeallocateObjects(cls, cnt, (afxObject*)hids);
            }
        }
    }

    AfxExitSlockExclusive(&cls->poolLock);

    return err;
}
