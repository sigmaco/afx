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

#define _AUX_UX_C
#define _AUX_HID_C
#define _AFX_CONTEXT_C
#define _AUX_MOUSE_C
#define _AUX_CONTROLLER_C
#define _AUX_SESSION_C
#include "../impl/auxImplementation.h"

_AUX void AfxGetMouseMotion(afxUnit seat, afxReal motion[2])
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        motion[0] = ses->seats[seat].motion[0][0];
        motion[1] = ses->seats[seat].motion[0][1];
    }
}

_AUX afxReal AfxGetMouseWheelDelta(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return 0;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        return ses->seats[seat].wheelDelta[0];

    return 0;
}

_AUX afxBool AfxIsLmbPressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_LMB]);
    
    return rslt;
}

_AUX afxBool AfxIsRmbPressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_RMB]);
    
    return rslt;
}

_AUX afxBool AfxIsMmbPressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_MMB]);
    
    return rslt;
}

_AUX afxBool AfxIsXmb1Pressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_XMB1]);
    
    return rslt;
}

_AUX afxBool AfxIsXmb2Pressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_XMB2]);
    
    return rslt;
}

_AUX afxBool AfxWasLmbPressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_LMB] && !(ses->seats[seat].mbState[1][AFX_LMB]));
    
    return rslt;
}

_AUX afxBool AfxWasRmbPressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_RMB] && !(ses->seats[seat].mbState[1][AFX_RMB]));
    
    return rslt;
}

_AUX afxBool AfxWasMmbPressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_MMB] && !(ses->seats[seat].mbState[1][AFX_MMB]));
    
    return rslt;
}

_AUX afxBool AfxWasXmb1Pressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_XMB1] && !(ses->seats[seat].mbState[1][AFX_XMB1]));
    
    return rslt;
}

_AUX afxBool AfxWasXmb2Pressed(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[0][AFX_XMB2] && !(ses->seats[seat].mbState[1][AFX_XMB2]));
    
    return rslt;
}

_AUX afxBool AfxWasLmbReleased(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_LMB] && !(ses->seats[seat].mbState[0][AFX_LMB]));
    
    return rslt;
}

_AUX afxBool AfxWasRmbReleased(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_RMB] && !(ses->seats[seat].mbState[0][AFX_RMB]));
    
    return rslt;
}

_AUX afxBool AfxWasMmbReleased(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_MMB] && !(ses->seats[seat].mbState[0][AFX_MMB]));
    
    return rslt;
}

_AUX afxBool AfxWasXmb1Released(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_XMB1] && !(ses->seats[seat].mbState[0][AFX_XMB1]));
    
    return rslt;
}

_AUX afxBool AfxWasXmb2Released(afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (ses->seats[seat].mbState[1][AFX_XMB2] && !(ses->seats[seat].mbState[0][AFX_XMB2]));
    
    return rslt;
}

_AUX afxMask AfxIsLmbPressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_LMB])
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxIsRmbPressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_RMB])
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxIsMmbPressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_MMB])
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxIsXmb1Pressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_XMB1])
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxIsXmb2Pressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_XMB2])
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxWasLmbPressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_LMB] &&
            !(ses->seats[seatIdx].mbState[1][AFX_LMB]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxWasRmbPressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_RMB] &&
            !(ses->seats[seatIdx].mbState[1][AFX_RMB]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxWasMmbPressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_MMB] &&
            !(ses->seats[seatIdx].mbState[1][AFX_MMB]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxWasXmb1Pressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_XMB1] &&
            !(ses->seats[seatIdx].mbState[1][AFX_XMB1]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxWasXmb2Pressed2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[0][AFX_XMB2] &&
            !(ses->seats[seatIdx].mbState[1][AFX_XMB2]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxWasLmbReleased2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[1][AFX_LMB] &&
            !(ses->seats[seatIdx].mbState[0][AFX_LMB]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxWasRmbReleased2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[1][AFX_RMB] &&
            !(ses->seats[seatIdx].mbState[0][AFX_RMB]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxWasMmbReleased2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[1][AFX_MMB] &&
            !(ses->seats[seatIdx].mbState[0][AFX_MMB]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxWasXmb1Released2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[1][AFX_XMB1] &&
            !(ses->seats[seatIdx].mbState[0][AFX_XMB1]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxMask AfxWasXmb2Released2(afxMask seats)
{
    afxError err = AFX_ERR_NONE;
    afxMask rslt = NIL;

    afxSession ses;
    if (!AfxGetSession(&ses)) return rslt;
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    for (afxUnit seatIdx = 0, seatCnt = ses->seatCnt; seatIdx < seatCnt; seatIdx++)
    {
        if (seats && !(seats & AFX_BITMASK(seatIdx)))
            continue; // skip if a mask is specified but it is not covered on such mask.

        if (ses->seats[seatIdx].mbState[1][AFX_XMB2] &&
            !(ses->seats[seatIdx].mbState[0][AFX_XMB2]))
        {
            rslt |= AFX_BITMASK(seatIdx);
        }
    }
    return rslt;
}

_AUX afxBool AfxMouseHasHorizontalChanged(afxUnit seat, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (((afxReal)AfxAbs(ses->seats[seat].motion[0][0] - ses->seats[seat].motion[1][0])) >= (afxReal)tolerance);
    
    return rslt;
}

_AUX afxBool AfxMouseHasVerticalChanged(afxUnit seat, afxInt tolerance)
{
    afxError err = AFX_ERR_NONE;
    afxBool rslt = FALSE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return rslt;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
        rslt = (((afxReal)AfxAbs(ses->seats[seat].motion[0][1] - ses->seats[seat].motion[1][1])) >= (afxReal)tolerance);
    
    return rslt;
}

_AUX afxError AfxEmulateMouseMotion(afxUnit seat, afxReal const motion[2], afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

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
        AFX_ASSERT_OBJECTS(afxFcc_WND, 1, &wnd);
        AfxEmitEvent(wnd, (void*)&ev);
    }
    return err;
}

_AUX afxError AfxEmulateMouseWheelAction(afxUnit seat, afxReal delta, afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

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

_AUX afxError AfxEmulateMouseButtonActions(afxUnit seat, afxUnit cnt, afxMouseButton const butt[], afxBool const pressed[], afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        AFX_ASSERT(cnt);
        AFX_ASSERT(butt);
        AFX_ASSERT(pressed);

        for (afxUnit i = 0; i < cnt; i++)
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

_AUX afxError AfxReleaseMouseButtons(afxUnit seat, afxWindow wnd)
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (seat < ses->seatCnt)
    {
        afxUnit cnt = ses->seats[seat].buttonCnt;
        AFX_ASSERT(cnt);

        for (afxUnit i = 0; i < cnt; i++)
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
    AFX_ASSERT_OBJECTS(afxFcc_MSE, 1, &mse);


    return err;
}

_AUX afxResult _AuxMseCtorCb(afxMouse mse, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_MSE, 1, &mse);

    afxSession ses = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    afxUnit const *ports = args[1];
    afxUnit seat = (afxUnit)ports[invokeNo];

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
_AUX afxError AfxAcquireControllers(afxUnit cnt, afxUnit const seat[], afxController controllers[])
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetControllerClass();
    AFX_ASSERT_CLASS(cls, afxFcc_CTRL);

    for (afxUnit i = 0; i < cnt; i++)
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
            AfxDisposeObjects(i, (afxObject*)controllers);
            break;
        }
    }
    return err;
}

_AUX afxController AfxGetController(afxSession ses, afxUnit seat)
{
    afxError err = AFX_ERR_NONE;
    afxUnit i = 0;
    afxController ctrl;
    while (AfxEnumerateControllers(i, 1, &ctrl))
    {
        AFX_ASSERT_OBJECTS(afxFcc_CTRL, 1, &ctrl)

        if (ctrl->hid.seat == seat)
            return ctrl;

        ++i;
    }
    return NIL;
}
#endif
