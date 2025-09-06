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

#ifndef AFX_ITERATOR_H
#define AFX_ITERATOR_H

#include "qwadro/base/afxObject.h"

AFX_DEFINE_STRUCT(afxIter)
{
    union
    {
        void        *item;
        afxHandle   *itemObj;
    };
    void*           udd[3];
};

AFX_DEFINE_STRUCT(_afxIter)
{
    void    *f;
    afxUnit  idx;
    afxBool quit;
    afxError quitCode;
    afxIter iter;
};

AFXINL afxUnit AfxIteratorGetIndex(afxIter const *i)
{
    _afxIter *ip = AFX_REBASE(i, _afxIter, iter);
    return ip->idx;
}

AFXINL void AfxIteratorQuit(afxIter const *i, afxError code)
{
    _afxIter *ip = AFX_REBASE(i, _afxIter, iter);
    ip->quit = TRUE;
    ip->quitCode = code;
}

#endif//AFX_ITERATOR_H