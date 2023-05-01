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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_ITERATOR_H
#define AFX_ITERATOR_H

#include "afx/core/afxObject.h"

AFX_DEFINE_STRUCT(afxIter)
{
    union
    {
        void        *item;
        afxObject   *itemObj;
    };
    void*           udd[3];
};

AFX_DEFINE_STRUCT(_afxIter)
{
    void    *f;
    afxNat  idx;
    afxBool quit;
    afxError quitCode;
    afxIter iter;
};

AFXINL afxNat AfxIteratorGetIndex(afxIter const *i)
{
    _afxIter *ip = AfxContainerOf(i, _afxIter, iter);
    return ip->idx;
}

AFXINL void AfxIteratorQuit(afxIter const *i, afxError code)
{
    _afxIter *ip = AfxContainerOf(i, _afxIter, iter);
    ip->quit = TRUE;
    ip->quitCode = code;
}

#endif//AFX_ITERATOR_H