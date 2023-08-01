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

#ifndef AFX_STACK_H
#define AFX_STACK_H

#include "afx/core/diag/afxDebug.h"
#include "afx/core/afxFcc.h"
#include "afx/core/mem/afxMemory.h"

// A Stack is a linear data structure in which elements can be inserted and deleted only from one side of the list, called the top. 
// A stack follows the LIFO (Last In First Out) principle, i.e., the element inserted at the last is the first element to come out. 
// The insertion of an element into the stack is called push operation, and the deletion of an element from the stack is called pop operation. 
// In stack, we always keep track of the last element present in the list with a pointer called top.

AFX_DEFINE_STRUCT(afxStack)
{
    _AFX_DBG_FCC;
    afxNat              unitSiz;
    afxNat              unitsPerBlock;
    afxNat              totalUsedUnitCnt;
    afxAllocationBlock  *lastBlock;
    afxNat              maxUnits;
    afxNat              activeBlocks;
    afxNat              maxActiveBlocks;
    afxAllocationBlock  **blockDir;

    afxMemory           ctx;
};

AFX void        AfxAcquireStack(afxMemory mem, afxStack *stck, afxNat unitSiz, afxNat unitsPerBlock);
AFX void        AfxAcquirePagedStack(afxMemory mem, afxStack *stck, afxNat unitSiz, afxNat unitsPerBlock, afxNat maxUnits);
AFX void        AfxReleaseStack(afxStack *stck);

AFX void        AfxEmptyStack(afxStack *stck);

AFX void*       AfxPushStackUnit(afxStack *stck, afxNat *idx);
AFX char        AfxPushStackUnits(afxStack *stck, afxNat cnt, afxNat *firstIdx, void const *initialVal);
AFX void        AfxPopStackUnits(afxStack *stck, afxNat cnt);
AFX void*       AfxGetStackUnit(afxStack *stck, afxNat idx);

AFX void        AfxSerializeStack(afxStack *stck, void *dst);

#endif//AFX_STACK_H