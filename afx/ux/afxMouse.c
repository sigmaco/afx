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

#define _AUX_UX_C
#define _AUX_HID_C
#define _AFX_CONTEXT_C
#define _AUX_MOUSE_C
#define _AUX_CONTROLLER_C
#define _AUX_SESSION_C
#include "../dev/AuxImplKit.h"

_AUX void AfxGetMouseMotion(afxSession ses, afxNat seat, afxReal motion[2])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        motion[0] = ses->seats[seat].motion[0][0];
        motion[1] = ses->seats[seat].motion[0][1];
    }
}

_AUX afxReal AfxGetMouseWheelDelta(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
        return ses->seats[seat].wheelDelta[0];

    return 0;
}

_AUX afxBool AfxIsLmbPressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_LMB]);
    
    return rslt;
}

_AUX afxBool AfxIsRmbPressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_RMB]);
    
    return rslt;
}

_AUX afxBool AfxIsMmbPressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_MMB]);
    
    return rslt;
}

_AUX afxBool AfxIsXmb1Pressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_XMB1]);
    
    return rslt;
}

_AUX afxBool AfxIsXmb2Pressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_XMB2]);
    
    return rslt;
}

_AUX afxBool AfxWasLmbPressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_LMB] && !(ses->seats[seat].mbState[1][AFX_LMB]));
    
    return rslt;
}

_AUX afxBool AfxWasRmbPressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_RMB] && !(ses->seats[seat].mbState[1][AFX_RMB]));
    
    return rslt;
}

_AUX afxBool AfxWasMmbPressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_MMB] && !(ses->seats[seat].mbState[1][AFX_MMB]));
    
    return rslt;
}

_AUX afxBool AfxWasXmb1Pressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_XMB1] && !(ses->seats[seat].mbState[1][AFX_XMB1]));
    
    return rslt;
}

_AUX afxBool AfxWasXmb2Pressed(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_XMB2] && !(ses->seats[seat].mbState[1][AFX_XMB2]));
    
    return rslt;
}

_AUX afxBool AfxWasLmbReleased(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_LMB] && !(ses->seats[seat].mbState[0][AFX_LMB]));
    
    return rslt;
}

_AUX afxBool AfxWasRmbReleased(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_RMB] && !(ses->seats[seat].mbState[0][AFX_RMB]));
    
    return rslt;
}

_AUX afxBool AfxWasMmbReleased(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_MMB] && !(ses->seats[seat].mbState[0][AFX_MMB]));
    
    return rslt;
}

_AUX afxBool AfxWasXmb1Released(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_XMB1] && !(ses->seats[seat].mbState[0][AFX_XMB1]));
    
    return rslt;
}

_AUX afxBool AfxWasXmb2Released(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_XMB2] && !(ses->seats[seat].mbState[0][AFX_XMB2]));
    
    return rslt;
}

_AUX afxBool AfxMouseHasHorizontalChanged(afxSession ses, afxNat seat, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (((afxReal)AfxAbs(ses->seats[seat].motion[0][0] - ses->seats[seat].motion[1][0])) >= (afxReal)tolerance);
    
    return rslt;
}

_AUX afxBool AfxMouseHasVerticalChanged(afxSession ses, afxNat seat, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);
    afxBool rslt = FALSE;

    if (seat < ses->seatCnt)
        rslt = (((afxReal)AfxAbs(ses->seats[seat].motion[0][1] - ses->seats[seat].motion[1][1])) >= (afxReal)tolerance);
    
    return rslt;
}

_AUX afxError AfxEmulateMouseMotion(afxSession ses, afxNat seat, afxReal const motion[2], afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        ses->seats[seat].motion[1][0] = ses->seats[seat].motion[0][0];
        ses->seats[seat].motion[1][1] = ses->seats[seat].motion[0][1];
        ses->seats[seat].motion[0][0] = motion[0];
        ses->seats[seat].motion[0][1] = motion[1];

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.ev.siz = sizeof(ev);
        ev.id = auxEventId_AXIS;
        ev.wnd = wnd;
        ev.seat = seat;
        AfxAssertObjects(1, &wnd, afxFcc_WND);
        AfxEmitEvent(wnd, (void*)&ev);
    }
    return err;
}

_AUX afxError AfxEmulateMouseWheelAction(afxSession ses, afxNat seat, afxReal delta, afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        ses->seats[seat].wheelDelta[1] = ses->seats[seat].wheelDelta[0];
        ses->seats[seat].wheelDelta[0] = delta;

        auxEvent ev = { 0 };
        ev.ev.id = afxEvent_UX;
        ev.ev.siz = sizeof(ev);
        ev.id = auxEventId_WHEEL;
        ev.wnd = wnd;
        ev.seat = seat;
        AfxEmitEvent(wnd, (void*)&ev);
    }
    return err;
}

_AUX afxError AfxEmulateMouseButtonActions(afxSession ses, afxNat seat, afxNat cnt, afxMouseButton const butt[], afxBool const pressed[], afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        AfxAssert(cnt);
        AfxAssert(butt);
        AfxAssert(pressed);

        for (afxNat i = 0; i < cnt; i++)
        {
            auxEventId evtype;

            switch (butt[i])
            {
            case AFX_LMB:
            {
                evtype = auxEventId_LMB;
                break;
            }
            case AFX_RMB:
            {
                evtype = auxEventId_RMB;
                break;
            }
            case AFX_MMB:
            {
                evtype = auxEventId_MMB;
                break;
            }
            case AFX_XMB1:
            {
                evtype = auxEventId_XMB1;
                break;
            }
            case AFX_XMB2:
            {
                evtype = auxEventId_XMB2;
                break;
            }
            default:
            {
                AfxThrowError();
                evtype = NIL;
                break;
            }
            }

            ses->seats[seat].mbState[1][butt[i]] = ses->seats[seat].mbState[0][butt[i]];
            ses->seats[seat].mbState[0][butt[i]] = !!pressed[i];

            if (evtype)
            {
                auxEvent ev = { 0 };
                ev.ev.id = afxEvent_UX;
                ev.ev.siz = sizeof(ev);
                ev.id = evtype;
                ev.wnd = wnd;
                ev.seat = seat;
                AfxEmitEvent(wnd, (void*)&ev);
            }
        }
    }
    return err;
}

_AUX afxError AfxReleaseMouseButtons(afxSession ses, afxNat seat, afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &ses, afxFcc_SES);

    if (seat < ses->seatCnt)
    {
        afxNat cnt = ses->seats[seat].buttonCnt;
        AfxAssert(cnt);

        for (afxNat i = 0; i < cnt; i++)
        {
            auxEventId evtype = auxEventId_LMB + i;
            
            ses->seats[seat].mbState[1][i] = ses->seats[seat].mbState[0][i];
            ses->seats[seat].mbState[0][i] = FALSE;

            auxEvent ev = { 0 };
            ev.ev.id = afxEvent_UX;
            ev.ev.siz = sizeof(ev);
            ev.id = evtype;
            ev.wnd = wnd;
            ev.seat = seat;
            AfxEmitEvent(wnd, (void*)&ev);
        }
    }
    return err;
}

_AUX afxError _AuxMseDtorCb(afxMouse mse)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mse, afxFcc_MSE);



    return err;
}

_AUX afxResult _AuxMseCtorCb(afxMouse mse, void** args, afxNat invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &mse, afxFcc_MSE);

    afxNat const *ports = args[0];
    afxNat seat = (afxNat)ports[invokeNo];

    mse->flags = NIL;

    // mouse
    AfxZero(mse->lastMbState, sizeof(mse->lastMbState));
    AfxZero(mse->prevMbState, sizeof(mse->prevMbState));
    mse->buttonCnt = AFX_MB_TOTAL;
    mse->lastMotion[0] = 0;
    mse->lastMotion[1] = 0;
    mse->lastWheelDelta = 0;
    mse->prevWheelDelta = 0;
    mse->sampleRate = 1;

    return err;
}

_AUX afxClassConfig const _AuxMseStdImplementation =
{
    .fcc = afxFcc_MSE,
    .name = "Mouse",
    .desc = "HID/Mouse Interface",
    .fixedSiz = sizeof(AFX_OBJ(afxMouse)),
    .ctor = (void*)_AuxMseCtorCb,
    .dtor = (void*)_AuxMseDtorCb
};

////////////////////////////////////////////////////////////////////////////////

#if 0
_AUX afxError AfxAcquireControllers(afxNat cnt, afxNat const seat[], afxController controllers[])
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetControllerClass();
    AfxAssertClass(cls, afxFcc_CTRL);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxController tmp;
        
        if ((tmp = AfxGetController(seat ? seat[i] : 0)))
        {
            if (AfxReacquireObjects(1, (afxObject*)&tmp)) AfxThrowError();
            else
            {
                controllers[i] = tmp;
            }
        }
        else
        {
            if (AfxAcquireObjects(cls, 1, (afxObject*)&controllers[i], (void const*[]) { seat ? (void*)&seat[i] : 0 }))
                AfxThrowError();
        }

        if (err)
        {
            AfxReleaseObjects(i, (afxObject*)controllers);
            break;
        }
    }
    return err;
}

_AUX afxController AfxGetController(afxSession ses, afxNat seat)
{
    afxError err = AFX_ERR_NONE;
    afxNat i = 0;
    afxController ctrl;
    while (AfxEnumerateControllers(i, 1, &ctrl))
    {
        AfxAssertObjects(1, &ctrl, afxFcc_CTRL);

        if (ctrl->hid.seat == seat)
            return ctrl;

        ++i;
    }
    return NIL;
}
#endif
