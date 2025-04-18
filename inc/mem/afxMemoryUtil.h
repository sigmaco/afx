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

  //////////////////////////////////////////////////////////////////////////////
 // MEMORY MANAGEMENT UTILITY                                                //
//////////////////////////////////////////////////////////////////////////////

#ifndef AFX_MEMORY_UTIL_H
#define AFX_MEMORY_UTIL_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/base/afxChain.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/exec/afxFutex.h"
#include "qwadro/inc/io/afxUri.h"

AFX void        AfxZero(void* start, afxSize range);
AFX void*       AfxFind(void const* start, afxSize range, afxInt c);
AFX void        AfxFill(void* start, afxSize range, void const* pattern, afxSize patternSiz);
AFX void        AfxMove(void* dst, void const* src, afxSize size);
AFX void        AfxCopy(void* dst, void const* src, afxSize size);
AFX afxInt      AfxCompare(void const* a, void const* b, afxSize siz);

AFX void*       AfxMemset(void* start, afxInt val, afxSize siz);
AFX void*       AfxMemcpy(void* dst, void const* src, afxSize siz);
AFX void const* AfxMemchr(void const* buf, afxInt val, afxUnit cap);
AFX afxInt      AfxMemcmp(void const* buf1, void const* buf2, afxSize siz);
AFX void*       AfxMemmove(void* dst, void const* src, afxSize siz);

#endif//AFX_MEMORY_UTIL_H
