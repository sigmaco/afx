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

  ////////////////////////////////////////////////////////////////////////////////
 // FIXED-SIZE PAGED MEMORY STACK                                              //
////////////////////////////////////////////////////////////////////////////////

/**
    A Stack is a linear data structure in which elements can be inserted and deleted only from one side of the list, called the top. 
    A stack follows the LIFO (Last In First Out) principle. Example: the element inserted at the last is the first element to come out. 
    The insertion of an element into the stack is called push operation, and the deletion of an element from the stack is called pop operation. 
    In stack, we always keep track of the last element present in the list with a pointer called top.
*/

#ifndef AFX_STACK_H
#define AFX_STACK_H

#include "qwadro/exec/afxDebug.h"
#include "qwadro/base/afxFcc.h"
#include "qwadro/mem/afxMemory.h"

#if (defined _AFX_DEBUG) && !(defined(_AFX_STACK_VALIDATION_ENABLED))
#   define _AFX_STACK_VALIDATION_ENABLED TRUE
#   define _AFX_STACK_METRICS_ENABLED TRUE
#endif

AFX_DEFINE_STRUCT(afxStackPage)
{
    afxUnit         usedUnitCnt;
    afxByte*        units;
    afxUnit         firstIdx;
    afxStackPage*   prev;
};

AFX_DEFINE_STRUCT(afxStack)
// Structure establishing a arena-style memory stack for efficient and reusable memory allocations in fixed-size units.
{
#ifdef _AFX_STACK_VALIDATION_ENABLED
    // (Optional) FourCC to validate the struct at runtime. Helps catch corruption or miscasts.
    afxFcc          fcc; // afxFcc_STAK;
#endif
    // Size in bytes of a single allocation unit in this stack.
    afxUnit         unitSiz;
    // Number of allocation units per block (aka a page).
    afxUnit         unitsPerBlock;
    // Total number of units currently in use across all blocks.
    afxUnit         totalUsedUnitCnt;
    // Pointer to the most recently used (and probably still active) block/page.
    afxStackPage*   lastBlock;
    // Optional cap on how many units the whole stack can hold.
    afxUnit         maxUnits;
    // How many memory blocks are currently allocated and in use.
    afxUnit         activeBlocks;
    // Optional cap on the number of blocks the stack is allowed to grow to.
    afxUnit         maxActiveBlocks;
    // Directory/array of pointers to all blocks used by this stack.
    afxStackPage**  blockDir;
    // Memory context or allocator from which memory is acquired (like malloc replacement).
    afxMmu          ctx;
#ifdef _AFX_STACK_METRICS_ENABLED
    afxUnit         peakUsedUnitCnt;
    afxUnit         peakActiveBlocks;
    afxUnit         totalAllocatedBlocks;  // Cumulative over time
    afxUnit         totalAllocatedBytes;   // Cumulative bytes ever allocated
#endif
};

AFX_DEFINE_STRUCT(afxStackStats)
{
    afxUnit         unitSiz;
    afxUnit         unitsPerBlock;

    afxUnit         totalUsedUnitCnt;
    afxUnit         totalAllocatedUnitCnt;

    afxUnit         activeBlocks;
    afxUnit         maxActiveBlocks;

    afxUnit         usedBytes;
    afxUnit         allocatedBytes;
    afxUnit         slackBytes;

#ifdef _AFX_STACK_METRICS_ENABLED
    afxUnit         peakUsedUnitCnt;
    afxUnit         peakActiveBlocks;
    afxUnit         totalAllocatedBlocks;  // Cumulative over time
    afxUnit         totalAllocatedBytes;   // Cumulative bytes ever allocated
#endif
};



AFX void            AfxInitStack
(
    afxStack*       stak, 
    afxUnit         unitSiz, 
    afxUnit         unitsPerBlock, 
    afxUnit         maxUnits
);

AFX void            AfxExhaustStack
(
    afxStack*       stak, 
    afxBool         fully
);

AFX void*           AfxPushStack
(
    afxStack*       stak, 
    afxUnit         cnt, 
    afxUnit*        first, 
    void const*     src, 
    afxUnit         stride
);

AFX void            AfxPopStack
(
    afxStack*       stak, 
    afxUnit         cnt
);

AFX void*           AfxPeekStack
(
    afxStack*       stak, 
    afxUnit         idx
);

AFX void            AfxDumpStack
(
    afxStack*       stak, 
    afxUnit         first, 
    afxUnit         cnt, 
    void*           dst, 
    afxUnit         stride
);

AFX void            AfxUpdateStack
(
    afxStack*       stak, 
    afxUnit         first, 
    afxUnit         cnt, 
    void const*     src, 
    afxUnit         srcStride
);

AFX void            AfxQueryStackStatistics
(
    afxStack const* stak, 
    afxStackStats*  stats
);

AFX void            AfxPrintStackStats
(
    afxStack const* stak
);

#endif//AFX_STACK_H
