/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_CONTEXT_C
#include "sgl.h"
#include "afx/afxQwadro.h"
#include "afx/draw/afxDrawInput.h"



_SGL afxError _AfxDinVmtDctxCb(afxDrawInput din, afxDrawContext from, afxDrawContext to, afxNat *slotIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);
    struct _afxDinD *dinD;
    _AfxGetDinD(din, &dinD,dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);
    // TODO discard pull request submissions too.

    struct _afxDctxD *fromD;
    
    if (from)
    {
        AfxAssertObjects(1, &from, AFX_FCC_DCTX);
        _AfxGetDctxD(from, &fromD,dsysD);
    }

    struct _afxDctxD *toD;

    if (to)
    {
        AfxAssertObjects(1, &to, AFX_FCC_DCTX);
        _AfxGetDctxD(to, &toD,dsysD);
    }

    //AfxDiscardAllDrawInputSubmissions(din);

    if (from && fromD->vmt->cin(from, din, FALSE, slotIdx)) AfxThrowError(); // ask dctx to unlink this dout
    else
    {
        if (to && toD->vmt->cin(to, din, TRUE, slotIdx)) AfxThrowError(); // ask dctx to unlink this dout
        else
        {
            //AfxDiscardAllDrawInputSubmissions(din);
        }
    }

    //AfxDiscardAllDrawInputSubmissions(din);

    return err;
}

_SGL afxError _AfxDinVmtProcCb(afxDrawInput din, afxDrawThread dthr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    struct _afxDinD *dinD;
    _AfxGetDinD(din, &dinD, dsysD);
    AfxAssertType(dinD, AFX_FCC_DIN);

    if (dinD->prefetchEnabled)
    {
        //if (AfxTryEnterSlockExclusive(&dinD->prefetchSlock))
        {
            if (dinD->userPrefetchProc)
                dinD->userPrefetchProc(din, dthr);

            //AfxExitSlockExclusive(&dinD->prefetchSlock);
        }
    }
    return err;
}

_SGL _afxDinVmt const _SglDinVmt =
{
    NIL,
    _AfxDinVmtProcCb,
    _AfxDinVmtDctxCb,
};

_SGL afxError _SglDdrvVmtDinCb(afxDrawInput din, afxUri const* endpoint, afxDrawInputConfig const *spec)
{
    //AfxEntry("din=%p,uri=%.*s", din, endpoint ? AfxPushString(AfxUriGetStringConst(endpoint)) : &AFX_STR_EMPTY);
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, AFX_FCC_DIN);

    afxDrawSystem dsys;
    AfxGetDrawSystem(&dsys);
    AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);
    struct _afxDsysD* dsysD;
    _AfxGetDsysD(dsys, &dsysD);
    AfxAssertType(dsysD, AFX_FCC_DSYS);

    struct _afxDinD *dinD;
    _AfxGetDinD(din, &dinD,dsysD);
    dinD->vmt = &_SglDinVmt;

    return err;
}
