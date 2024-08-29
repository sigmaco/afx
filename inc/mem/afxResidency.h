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
    afxNat      unitIdx;
    afxObject   obj;
    void*       data;
};

AFX_DEFINE_STRUCT(afxResidency)
{
    _AFX_DBG_FCC
    afxSlock    lock;
    afxBool     locked;
    afxNat      unitsPerPage;
    afxNat      dataUnitSiz;
    afxNat      totalUsedCnt;
    afxNat      pageCnt;
    afxByte**   pages;
    afxMmu   mem;
    void        (*dtor)(afxResident *resident);
    void        (*ctor)(afxResident *resident);
    void*       udd;
};

AFX void        AfxAcquireResidency(afxResidency* hdlr, afxNat dataUnitSiz, afxNat unitsPerPage);
AFX void        AfxReleaseResidency(afxResidency* hdlr);

AFX afxNat      AfxGetObjectsResidency(afxNat cnt, afxObject obj[], void *data[], afxResidency* hdlr);
AFX afxError    AfxAllocateObjects(afxNat cnt, afxObject obj[], afxResidency* hndl);
AFX afxError    AfxDeallocateObjects(afxNat cnt, afxObject obj[], afxResidency* hndl);
AFX afxNat      AfxFindObjectsResidency(afxNat cnt, afxObject obj[], afxNat unitIdx[], afxResidency* hndl);

AFX afxNat      AfxEnumerateResidentObjects(afxResidency* hdlr, afxNat first, afxNat cnt, afxObject obj[]);
AFX afxNat      AfxInvokeResidentObjects(afxResidency* hndl, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void* udd), void *udd);

AFX afxNat      AfxGetResidencyCount(afxResidency* hndl);

#endif//AFX_HANDLING_H