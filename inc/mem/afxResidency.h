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

#ifndef AFX_HANDLING_H
#define AFX_HANDLING_H

#include "qwadro/inc/mem/afxPool.h"
#include "qwadro/inc/exec/afxSlock.h"

AFX_DEFINE_STRUCT(afxResident)
{
    afxUnit      unitIdx;
    afxObject   obj;
    void*       data;
};

AFX_DEFINE_STRUCT(afxResidency)
{
    _AFX_DBG_FCC
    afxSlock    lock;
    afxBool     locked;
    afxUnit      unitsPerPage;
    afxUnit      dataUnitSiz;
    afxUnit      totalUsedCnt;
    afxUnit      pageCnt;
    afxByte**   pages;
    afxMmu   mem;
    void        (*dtor)(afxResident *resident);
    void        (*ctor)(afxResident *resident);
    void*       udd;
};

AFX void        AfxAcquireResidency(afxResidency* hdlr, afxUnit dataUnitSiz, afxUnit unitsPerPage);
AFX void        AfxReleaseResidency(afxResidency* hdlr);

AFX afxUnit      AfxGetObjectsResidency(afxUnit cnt, afxObject obj[], void *data[], afxResidency* hdlr);
AFX afxError    AfxAllocateObjects(afxUnit cnt, afxObject obj[], afxResidency* hndl);
AFX afxError    AfxDeallocateObjects(afxUnit cnt, afxObject obj[], afxResidency* hndl);
AFX afxUnit      AfxFindObjectsResidency(afxUnit cnt, afxObject obj[], afxUnit unitIdx[], afxResidency* hndl);

AFX afxUnit      AfxEnumerateResidentObjects(afxResidency* hdlr, afxUnit first, afxUnit cnt, afxObject obj[]);
AFX afxUnit      AfxInvokeResidentObjects(afxResidency* hndl, afxUnit first, afxUnit cnt, afxBool(*f)(afxObject obj, void* udd), void *udd);

AFX afxUnit      AfxGetResidencyCount(afxResidency* hndl);

#endif//AFX_HANDLING_H