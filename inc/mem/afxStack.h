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

/// A Stack is a linear data structure in which elements can be inserted and deleted only from one side of the list, called the top. 
/// A stack follows the LIFO (Last In First Out) principle. Example: the element inserted at the last is the first element to come out. 
/// The insertion of an element into the stack is called push operation, and the deletion of an element from the stack is called pop operation. 
/// In stack, we always keep track of the last element present in the list with a pointer called top.

#ifndef AFX_STACK_H
#define AFX_STACK_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/mem/afxMemory.h"

AFX_DEFINE_STRUCT(afxStackPage)
{
    afxNat          usedUnitCnt;
    afxByte*        units;
    afxNat          firstIdx;
    afxStackPage*   prev;
};

AFX_DEFINE_STRUCT(afxStack)
{
    afxNat              unitSiz;
    afxNat              unitsPerBlock;
    afxNat              totalUsedUnitCnt;
    afxStackPage*       lastBlock;
    afxNat              maxUnits;
    afxNat              activeBlocks;
    afxNat              maxActiveBlocks;
    afxStackPage**      blockDir;
    afxMmu              ctx;
};

AFX void        AfxAllocateStack(afxStack* stak, afxNat unitSiz, afxNat unitsPerBlock);
AFX void        AfxAllocatePagedStack(afxStack* stak, afxNat unitSiz, afxNat unitsPerBlock, afxNat maxUnits);
AFX void        AfxDeallocateStack(afxStack* stak);

AFX void        AfxEmptyStack(afxStack* stak);

AFX void*       AfxGetStackUnit(afxStack* stak, afxNat idx);
AFX void        AfxPopStackUnits(afxStack* stak, afxNat cnt);
AFX void*       AfxPushStackUnit(afxStack* stak, afxNat *idx);
AFX char        AfxPushStackUnits(afxStack* stak, afxNat cnt, afxNat *firstIdx, void const *initialVal);

AFX void        AfxDumpStackElement(afxStack* stak, afxNat idx, void *dst);
AFX void        AfxDumpStackElements(afxStack* stak, afxNat first, afxNat cnt, void *dst);

AFX void        AfxUpdateStackElement(afxStack* stak, afxNat idx, void const* src);
AFX void        AfxUpdateStackElements(afxStack* stak, afxNat first, afxNat cnt, void const* src);

AFX void        AfxDumpStack(afxStack* stak, void *dst);

#endif//AFX_STACK_H
