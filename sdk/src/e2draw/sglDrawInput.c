/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_INPUT_C
#define _AFX_DRAW_CONTEXT_C
#include "sgl.h"
#include "afx/afxQwadro.h"
#include "afx/draw/afxDrawInput.h"



_SGL afxError _AfxDinVmtDctxCb(afxDrawInput din, afxDrawContext from, afxDrawContext to)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObject(din, AFX_FCC_DIN);
    // TODO discard pull request submissions too.
    (void)from;
    (void)to;

    //AfxDiscardAllDrawInputSubmissions(din);

    if (from && from->vmt->cin(from, din, FALSE)) AfxThrowError(); // ask dctx to unlink this dout
    else
    {
        if (to && to->vmt->cin(to, din, TRUE)) AfxThrowError(); // ask dctx to unlink this dout
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
    AfxAssertObject(din, AFX_FCC_DIN);

    if (din->prefetchEnabled)
    {
        //if (AfxTryEnterSlockExclusive(&din->prefetchSlock))
        {
            if (din->userPrefetchProc)
                din->userPrefetchProc(din, dthr);

            //AfxExitSlockExclusive(&din->prefetchSlock);
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

_SGL afxError _SglDdrvVmtDinCb(afxDrawInput din, afxUri const* endpoint, afxDrawInputSpecification const *spec)
{
    //AfxEntry("din=%p,uri=%.*s", din, endpoint ? AfxPushString(AfxUriGetStringConst(endpoint)) : &AFX_STR_EMPTY);
    afxError err = AFX_ERR_NONE;

    din->vmt = &_SglDinVmt;

    return err;
}
