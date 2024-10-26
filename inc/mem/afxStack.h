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

/**
    A Stack is a linear data structure in which elements can be inserted and deleted only from one side of the list, called the top. 
    A stack follows the LIFO (Last In First Out) principle. Example: the element inserted at the last is the first element to come out. 
    The insertion of an element into the stack is called push operation, and the deletion of an element from the stack is called pop operation. 
    In stack, we always keep track of the last element present in the list with a pointer called top.
*/

#ifndef AFX_STACK_H
#define AFX_STACK_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/mem/afxMemory.h"

AFX_DEFINE_STRUCT(afxStackPage)
{
    afxUnit          usedUnitCnt;
    afxByte*        units;
    afxUnit          firstIdx;
    afxStackPage*   prev;
};

AFX_DEFINE_STRUCT(afxStack)
{
    afxUnit              unitSiz;
    afxUnit              unitsPerBlock;
    afxUnit              totalUsedUnitCnt;
    afxStackPage*       lastBlock;
    afxUnit              maxUnits;
    afxUnit              activeBlocks;
    afxUnit              maxActiveBlocks;
    afxStackPage**      blockDir;
    afxMmu              ctx;
};

AFX void        AfxAllocateStack(afxStack* stak, afxUnit unitSiz, afxUnit unitsPerBlock);
AFX void        AfxAllocatePagedStack(afxStack* stak, afxUnit unitSiz, afxUnit unitsPerBlock, afxUnit maxUnits);
AFX void        AfxDeallocateStack(afxStack* stak);

AFX void        AfxEmptyStack(afxStack* stak);

AFX void*       AfxGetStackUnit(afxStack* stak, afxUnit idx);
AFX void        AfxPopStackUnits(afxStack* stak, afxUnit cnt);
AFX void*       AfxPushStackUnit(afxStack* stak, afxUnit *idx);
AFX char        AfxPushStackUnits(afxStack* stak, afxUnit cnt, afxUnit *firstIdx, void const *initialVal);

AFX void        AfxDumpStackElement(afxStack* stak, afxUnit idx, void *dst);
AFX void        AfxDumpStackElements(afxStack* stak, afxUnit first, afxUnit cnt, void *dst);

AFX void        AfxUpdateStackElement(afxStack* stak, afxUnit idx, void const* src);
AFX void        AfxUpdateStackElements(afxStack* stak, afxUnit first, afxUnit cnt, void const* src);

AFX void        AfxDumpStack(afxStack* stak, void *dst);

#endif//AFX_STACK_H
