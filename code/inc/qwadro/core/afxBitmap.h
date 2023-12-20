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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_BITMAP_H
#define AFX_BITMAP_H

#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxFcc.h"
#include "qwadro/core/afxMmu.h"

AFX_DEFINE_STRUCT(afxBitmapPage)
{
    afxNat          markedBitCnt;
    afxNat32*       sets;
};

AFX_DEFINE_STRUCT(afxBitmap)
{
    _AFX_DBG_FCC;
    afxNat          setsPerPage;
    afxNat          totalMarkedBitCnt;
    afxNat          pageCnt;
    afxBitmapPage*  pages;
    afxMmu       mem;
};

AFX void        AfxAcquireBitmap(afxBitmap* bmp, afxNat setsPerPage);
AFX void        AfxReleaseBitmap(afxBitmap* bmp);

AFX afxError    AfxSetBit(afxBitmap* bmp, afxNat bit);
AFX void        AfxClearBit(afxBitmap* bmp, afxNat bit);
AFX afxResult   AfxTestBit(afxBitmap const* bmp, afxNat bit);
AFX afxError    AfxPullBit(afxBitmap* bmp, afxNat* bit);

#endif//AFX_BITMAP_H
