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

#ifndef AFX_MEMORY_H
#define AFX_MEMORY_H

//#define VLD_FORCE_ENABLE

#include <stdlib.h>
#if (defined(_WIN64) || defined(_WIN32))
#   ifdef VLD_FORCE_ENABLE
#       include <vld.h>
#   endif
#endif

#include "../base/afxLinkedList.h"
#include "../base/afxFcc.h"
#include "../diag/afxDebug.h"

//#define _AFX_TRY_ASSERT_ALLOCATION_AREA 1

// MEMORY LAYOUT
// size is the raw lenght of a type in bytes.
// alignment is an additional size used to pad elements in memory.
// stride is the sum of size with alignment.

AFX_DEFINE_HANDLE(afxMemContext);
AFX_DECLARE_STRUCT(afxSegment);
AFX_DECLARE_STRUCT(afxMemory);

struct afxSegment
{
    afxSize off, siz, stride;
};

#define AfxSpawnSegment(off_,siz_,stride_) (afxSegment){ .off = (afxSize)(off_), .siz = (afxSize)(siz_), .stride = (afxSize)(stride_) }

AFXINL void AfxMakeSegment(afxSegment *seg, afxSize off, afxSize siz, afxSize stride)
{
    afxError err = NIL;
    AfxAssert(seg); AfxAssert(siz);
    seg->off = off, seg->siz = siz;
    seg->stride = stride ? stride : siz;
};

struct afxMemory
{
    afxChar const       *func;
    afxSize             line;
    afxChar const       *file;
    
    afxSize             siz;
    afxLinkage          mctx;
    afxFcc              fcc;
};

AFX void*       AfxAllocate(afxMemContext mctx, afxSize siz, afxHint const hint);

AFX void*       AfxCoallocate(afxMemContext mctx, afxSize cnt, afxSize siz, afxHint const hint);

AFX void*       AfxReallocate(void *p, afxSize siz, afxHint const hint);

AFX afxResult   AfxDeallocate(void *p);

AFX afxResult   AfxCopy(void *dst, void const *src, afxSize range);

AFX afxResult   AfxFill(void *p, afxSize range, afxInt value);

AFX afxResult   AfxZero(void *p, afxSize range);

AFX afxResult   AfxWriteSegmented(void *p, afxNat cnt, afxSegment const *seg, void const *from);

AFX afxMemContext    AfxGetMemContext(void *p);

AFX afxBool         AfxIsAnValidAllocation(void const *p);

#define AFX_ALIGN(operand_,alignment_) ((operand_ + (alignment_ - 1)) & ~(alignment_ - 1))

#define AFX_ZERO(chnk_) AfxZero((chnk_), sizeof(*(chnk_)))
#define AFX_FILL(chnk_, val_) AfxFill((chnk_), sizeof(*(chnk_)), (val_))
#define AFX_COPY(chnkDst_, chnkSrc_) AfxCopy((chnkDst_), (chnkSrc_), sizeof(*(chnkDst_)))

#endif//AFX_MEMORY_H
