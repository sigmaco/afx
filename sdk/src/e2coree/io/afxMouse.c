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
#include <stdio.h>
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <hidusage.h>

#define _AFX_CORE_C
#define _AFX_DEVICE_C
#define _AFX_HID_C
#define _AFX_MOUSE_C
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxSystem.h"
#include "qwadro/io/afxKeyboard.h"
#include "qwadro/io/afxMouse.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/core/afxString.h"

AFX void AfxGetLastMouseMotion(afxNat port, afxReal motion[2])
{
    afxError err = AFX_ERR_NONE;
    afxMouse mse;
    
    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        motion[0] = mse->lastMotion[0];
        motion[1] = mse->lastMotion[1];
    }
}

AFX afxReal AfxGetLastMouseWheelData(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        return mse->lastWheelDelta;
    }
    return 0;
}

_AFX afxBool AfxLmbIsPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_LMB]);
    }
    return rslt;
}

_AFX afxBool AfxRmbIsPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_RMB]);
    }
    return rslt;
}

_AFX afxBool AfxMmbIsPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_MMB]);
    }
    return rslt;
}

_AFX afxBool AfxXmbIs1Pressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_XMB1]);
    }
    return rslt;
}

_AFX afxBool AfxXmbIs2Pressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_XMB2]);
    }
    return rslt;
}

_AFX afxBool AfxLmbWasPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_LMB] && !(mse->prevState[AFX_LMB]));
    }
    return rslt;
}

_AFX afxBool AfxRmbWasPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_RMB] && !(mse->prevState[AFX_RMB]));
    }
    return rslt;
}

_AFX afxBool AfxMmbWasPressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_MMB] && !(mse->prevState[AFX_MMB]));
    }
    return rslt;
}

_AFX afxBool AfxXmbWas1Pressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_XMB1] && !(mse->prevState[AFX_XMB1]));
    }
    return rslt;
}

_AFX afxBool AfxXmbWas2Pressed(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->currState[AFX_XMB2] && !(mse->prevState[AFX_XMB2]));
    }
    return rslt;
}

_AFX afxBool AfxLmbWasReleased(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->prevState[AFX_LMB] && !(mse->currState[AFX_LMB]));
    }
    return rslt;
}

_AFX afxBool AfxRmbWasReleased(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->prevState[AFX_RMB] && !(mse->currState[AFX_RMB]));
    }
    return rslt;
}

_AFX afxBool AfxMmbWasReleased(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->prevState[AFX_MMB] && !(mse->currState[AFX_MMB]));
    }
    return rslt;
}

_AFX afxBool AfxXmbWas1Released(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->prevState[AFX_XMB1] && !(mse->currState[AFX_XMB1]));
    }
    return rslt;
}

_AFX afxBool AfxXmbWas2Released(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (mse->prevState[AFX_XMB2] && !(mse->currState[AFX_XMB2]));
    }
    return rslt;
}

_AFX afxBool AfxMouseHasHorizontalChanged(afxNat port, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (((afxReal)AfxAbs(mse->lastMotion[0] - mse->prevMotion[0])) >= (afxReal)tolerance);
    }
    return rslt;
}

_AFX afxBool AfxMouseHasVerticalChanged(afxNat port, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        rslt = (((afxReal)AfxAbs(mse->lastMotion[1] - mse->prevMotion[1])) >= (afxReal)tolerance);
    }
    return rslt;
}

_AFX afxError AfxEmulateMouseMotion(afxNat port, afxReal const motion[2])
{
    afxError err = AFX_ERR_NONE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        mse->prevMotion[0] = mse->lastMotion[0];
        mse->prevMotion[1] = mse->lastMotion[1];
        mse->lastMotion[0] = motion[0];
        mse->lastMotion[1] = motion[1];

        afxEvent ev;
        AfxEventDeploy(&ev, AFX_EVENT_MSE_AXIS_UPDATED, (void*)mse, mse->lastMotion);
        AfxObjectEmitEvent(mse, &ev);
    }
    return err;
}

_AFX afxError AfxEmulateMouseWheelAction(afxNat port, afxReal delta)
{
    afxError err = AFX_ERR_NONE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);

        mse->prevWheelDelta = mse->lastWheelDelta;
        mse->lastWheelDelta = delta;

        afxEvent ev;
        AfxEventDeploy(&ev, AFX_EVENT_MSE_WHEEL_UPDATED, (void*)mse, &mse->lastWheelDelta);
        AfxObjectEmitEvent(mse, &ev);
    }
    return err;
}

_AFX afxError AfxEmulateMouseButtonActions(afxNat port, afxNat cnt, afxMouseButton const butt[], afxBool const pressed[])
{
    afxError err = AFX_ERR_NONE;
    afxMouse mse;

    if ((mse = AfxGetMouse(port)))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);
        AfxAssert(cnt);
        AfxAssert(butt);
        AfxAssert(pressed);

        for (afxNat i = 0; i < cnt; i++)
        {
            afxEventId evtype;

            switch (butt[i])
            {
            case AFX_LMB:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_LMB_PRESSED : AFX_EVENT_MSE_LMB_RELEASED;
                break;
            }
            case AFX_RMB:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_RMB_PRESSED : AFX_EVENT_MSE_RMB_RELEASED;
                break;
            }
            case AFX_MMB:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_MMB_PRESSED : AFX_EVENT_MSE_MMB_RELEASED;
                break;
            }
            case AFX_XMB1:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_XMB1_PRESSED : AFX_EVENT_MSE_XMB1_RELEASED;
                break;
            }
            case AFX_XMB2:
            {
                evtype = (pressed[i]) ? AFX_EVENT_MSE_XMB2_PRESSED : AFX_EVENT_MSE_XMB2_RELEASED;
                break;
            }
            default:
            {
                AfxThrowError();
                evtype = NIL;
                break;
            }
            }

            if (evtype)
            {
                mse->prevState[butt[i]] = mse->currState[butt[i]];
                mse->currState[butt[i]] = !!pressed[i];

                afxEvent ev;
                AfxEventDeploy(&ev, evtype, (void*)mse, (void*)&butt[i]);
                AfxObjectEmitEvent(mse, &ev);
            }
        }
    }
    return err;
}

_AFX afxError _AfxMseDtor(afxMouse mse)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mse, afxFcc_MSE);
    return err;
}

_AFX afxError _AfxMseCtor(afxMouse mse, afxCookie const *cookie)
{
    AfxEntry("mse=%p", mse);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mse, afxFcc_MSE);

    afxNat const *ports = cookie->udd[0];
    afxNat port = (afxNat)ports[cookie->no];

    mse->hid.port = port;
    mse->hid.flags = NIL;

    AfxZero(1, sizeof(mse->currState), mse->currState);
    AfxZero(1, sizeof(mse->prevState), mse->prevState);
    mse->buttonCnt = AFX_MB_TOTAL;
    mse->lastMotion[0] = 0;
    mse->lastMotion[1] = 0;
    mse->lastWheelDelta = 0;
    mse->prevWheelDelta = 0;
    mse->sampleRate = 1;

    static afxBool hasRidMseBeenRegistered = FALSE;

    if (hasRidMseBeenRegistered == FALSE)
    {
        RAWINPUTDEVICE rid;
        rid.usUsagePage = HID_USAGE_PAGE_GENERIC; // ((USAGE)0x01)
        rid.usUsage = HID_USAGE_GENERIC_MOUSE; // ((USAGE) 0x02)
        rid.dwFlags = /*RIDEV_NOLEGACY |*/ RIDEV_DEVNOTIFY; // ignores legacy mouse messages
        rid.hwndTarget = NIL;
        //hid->onRelease = (afxResult(*)(afxHid hid))AfxReleaseMouse;

        if (!(hasRidMseBeenRegistered = RegisterRawInputDevices(&(rid), sizeof(rid) / sizeof(rid), sizeof(rid)))) AfxThrowError();
        else
        {

        }
    }
    return err;
}

_AFX afxClassConfig const _AfxMseClsConfig =
{
    .fcc = afxFcc_MSE,
    .name = "Mouse",
    .unitsPerPage = 1,
    .size = sizeof(AFX_OBJECT(afxMouse)),
    .mmu = NIL,
    .ctor = (void*)_AfxMseCtor,
    .dtor = (void*)_AfxMseDtor
};

////////////////////////////////////////////////////////////////////////////////

_AFX afxError AfxAcquireMouses(afxNat cnt, afxNat const port[], afxMouse mse[])
{
    AfxEntry("cnt=%u,port=%p,mse=%p", cnt, port, mse);
    afxError err = AFX_ERR_NONE;

    afxClass* cls = AfxGetMouseClass();
    AfxAssertClass(cls, afxFcc_MSE);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxMouse tmp;

        if ((tmp = AfxGetMouse(port ? port[i] : 0)))
        {
            if (AfxReacquireObjects(1, (afxObject*)&tmp)) AfxThrowError();
            else
            {
                mse[i] = tmp;
            }
        }
        else
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&mse[i], (void const*[]) { port ? (void*)&port[i] : 0 }))
                AfxThrowError();
        }

        if (err)
        {
            AfxReleaseObjects(i, (afxObject*)mse);
            break;
        }
    }
    return err;
}

_AFX afxNat AfxInvokeMouses(afxNat first, afxNat cnt, afxBool(*f)(afxMouse, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(f);
    afxClass* cls = AfxGetMouseClass();
    AfxAssertClass(cls, afxFcc_MSE);
    return AfxInvokeInstances(cls, first, cnt, (void*)f, udd);
}

_AFX afxNat AfxEnumerateMouses(afxNat first, afxNat cnt, afxMouse mse[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cnt);
    AfxAssert(mse);
    afxClass* cls = AfxGetMouseClass();
    AfxAssertClass(cls, afxFcc_MSE);
    return AfxEnumerateInstances(cls, first, cnt, (afxObject*)mse);
}

_AFX afxNat AfxCountMouses(void)
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetMouseClass();
    AfxAssertClass(cls, afxFcc_MSE);
    return AfxCountInstances(cls);
}

_AFX afxMouse AfxGetMouse(afxNat port)
{
    afxError err = AFX_ERR_NONE;
    afxNat i = 0;
    afxMouse mse;
    while (AfxEnumerateMouses(i, 1, &mse))
    {
        AfxAssertObjects(1, &mse, afxFcc_MSE);
        
        if (mse->hid.port == port)
            return mse;

        ++i;
    }
    return NIL;
}
