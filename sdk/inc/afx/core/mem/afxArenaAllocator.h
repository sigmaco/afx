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

/*

In computer science, region-based memory management is a type of memory management in which each allocated object is assigned to a region.
A region, also called a zone, arena, area, or memory context, is a collection of allocated objects that can be efficiently reallocated or deallocated all at once.
Like stack allocation, regions facilitate allocation and deallocation of memory with low overhead; but they are more flexible, allowing objects to live longer than the stack frame in which they were allocated.
In typical implementations, all objects in a region are allocated in a single contiguous range of memory addresses, similarly to how stack frames are typically allocated.

*/

#ifndef AFX_ARENA_ALLOCATOR_H
#define AFX_ARENA_ALLOCATOR_H

#include "afxMemContext.h"

AFX_DEFINE_HANDLE(afxArenaMemContext);

AFX_OBJECT(afxArenaMemContext)
{
    AFX_OBJECT(afxMemContext)    all;
};

#endif//AFX_ARENA_ALLOCATOR_H