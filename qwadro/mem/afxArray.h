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
 // FIXED-SIZE DYNAMICALLY ALLOCATED ARRAY                                   //
//////////////////////////////////////////////////////////////////////////////

/**
    The elements are stored contiguously, which means that elements can be accessed not only 
    through iterators, but also using offsets to regular pointers to elements.
    This means that a pointer to an element of a vector may be passed to any function that 
    expects a pointer to an element of an array.

    The storage of the vector is handled automatically, being expanded and contracted as needed.
    Vectors usually occupy more space than static arrays, because more memory is allocated to handle 
    future growth.
    This way a vector does not need to reallocate each time an element is inserted, but only when 
    the additional memory is exhausted.
    The total amount of allocated memory can be queried using capacity() function. Extra memory can 
    be returned to the system via a call to shrink_to_fit().

    Reallocations are usually costly operations in terms of performance. The reserve() function can 
    be used to eliminate reallocations if the number of elements is known beforehand.

    The complexity(efficiency) of common operations on vectors is as follows:
    - Random access - constant O(1)
    - Insertion or removal of elements at the end - amortized constant O(1)
    - Insertion or removal of elements - linear in the distance to the end of the vector O(n)

    std::vector(for T other than bool) meets the requirements of Container, MemoryAwareContainer, SequenceContainer, ContiguousContainer(since C++17) and ReversibleContainer.
*/

#ifndef AFX_ARRAY_H
#define AFX_ARRAY_H

#include "qwadro/exec/afxDebug.h"
#include "qwadro/base/afxFcc.h"
#include "qwadro/mem/afxMemory.h"

#if (defined _AFX_DEBUG) && !(defined(_AFX_ARRAY_VALIDATION_ENABLED))
#   define _AFX_ARRAY_VALIDATION_ENABLED TRUE
#endif

#define _AFX_USE_TYPEOF TRUE

typedef enum afxArrayFlag
{
    afxArrayFlag_STATIC = AFX_BITMASK(0),
    afxArrayFlag_TRIM   = AFX_BITMASK(1),
} afxArrayFlags;

AFX_DEFINE_STRUCT(afxArray)
// A lightweight dynamic array with alignment control, runtime validation (optional), 
// and access to underlying data in multiple formats. It supports:
//  Typed and byte-level data access.
//  Manual control over memory allocation and alignment.
//  Runtime capacity tracking for efficient appends.
{
#ifdef _AFX_ARRAY_VALIDATION_ENABLED
    // A type signature used for runtime validation/debugging.
    afxFcc      fcc; // afxFcc_ARR;
#endif
    // Size (in bytes) of each item in the array.
    afxUnit     unitSiz;
    // Alignment requirement of each item, typically a power of two.
    // Useful for SIMD or architecture-specific performance optimizations.
    afxUnit     align;
    // Capacity; how many items the array can hold before needing to reallocate memory.
    afxUnit     cap;
    // Population; how many elements are currently in use (i.e. how many valid items exist).
    afxUnit     pop;
    // A union giving access to the underlying data in multiple ways:
    //  items: Points to the data (typed via casting later).
    //  bytemap: Byte-level access (useful for serialization or memory scanning).
    //  addr: The raw address, possibly used for alignment checks or pointer arithmetic.
    union
    {
        void*   items;
        afxByte*bytemap;
        uintptr_t addr;
    };
    // A boolean flag indicating whether the memory has been dynamically allocated (versus statically assigned or externally managed).
    afxBool     alloced;
};

#define AfxArray(type_, cap_, initialVal_) AfxMakeArray((cap_), sizeof(type_), (initialVal_) ? ((type_ const[]){(initialVal_)}) : NIL)


/*
    The AfxMakeArray() function initializes an array directly using externally provided memory (or a null buffer for dynamic allocation).
    This is ideal for embedding an array in-place, or wrapping an existing memory block into an afxArray interface.

    Its design enables buffer ownership flexibility (external or internal).
    It can be useful in embedded, realtime, or GPU-interfacing code.
    It can be useful for temporary or fixed-size workspaces.
    Its design is essential when memory layout must be externally controlled.
*/

AFXINL afxError     AfxMakeArray
(
    // The array to initialize.
    afxArray*       arr, 
    // Size of each unit (element).
    afxUnit         unitSiz, 
    // How many units the array can hold.
    afxUnit         cap, 
    // Optional buffer (can be NIL).
    void*           buf, 
    // Initial unit count (<= cap) for user-provided buffer.
    afxUnit         pop
);

/*
    The AfxEmptyArray() clears or resets the array, optionally freeing its memory, depending on the flag.
    Optionally, it also frees the memory allocation, thus zeroing its capacity.
    This function is used to properly discommission an array.
    
    Reusing array structures with or without releasing memory, depending on whether you plan to push more data in again soon.
*/

AFXINL afxError     AfxEmptyArray
(
    // The dynamic array to exhaust.
    afxArray*       arr, 
    // If TRUE, it keeps memory allocated for reuse.
    afxBool         dontFree,
    // If TRUE, it zeroes out the array's memory.
    afxBool         zeroOut
);

/*
    The AfxReserveArraySpace() function ensures the array has at least @cap units of capacity allocated.
    This function does not change the count of items. It may allocate or reallocate internal storage.
    Useful for preallocation if you know you'll push a bunch of elements soon, avoiding repeated 
    reallocations during a large push sequence (like pushing 1000 things at once).
*/

AFXINL afxError     AfxReserveArraySpace
(
    // The array to be inflated.
    afxArray*       arr, 
    // The desired capacity.
    afxUnit         cap
);

/*
    The AfxIsArrayFull() function returns true if the current count of elements has reached the array's capacity.
    You'll probably want to reserve or grow the array if this returns true before pushing more items.
*/

AFXINL afxBool      AfxIsArrayFull
(
    // The array to be checked.
    afxArray const* arr
);

/*
    The AfxIsArrayEmpty() function returns true if the array contains no elements (count == 0), 
    regardless of whether memory is allocated or not.

*/

AFXINL afxBool      AfxIsArrayEmpty
(
    // The array to be checked.
    afxArray const* arr
);

/*
    The AfxPushArrayUnits() function adds @cnt new units/elements to the end of an array.
    @baseUnitIdx, if not NULL, the function sets this to the index in the array where the newly pushed units start.
    @data, if not NULL, it copies @cnt units from this source into the array.
    @stride is the byte offset between elements in the source data (allows support for non-contiguous source buffers).

    The is the insertion function here, but it's optimized for appending to the end of the array. So it's a push-back model.

    Returns a pointer to the beginning of the memory block within the array where the new units were added.
    If no memory was added (e.g., due to allocation failure), it returns NULL.
*/

AFXINL void*        AfxPushArrayUnits
(
    // The dynamic array to push data into.
    afxArray*       arr, 
    // Number of units (elements) to push.
    afxUnit         cnt, 
    // [out] Optional. Returns the base index where new units were added.
    afxUnit*        baseUnitIdx, 
    // Optional. Pointer to source data to copy from (optional).
    void const*     src, 
    // Stride (in bytes) between each source unit in @data.
    afxUnit         srcStride
);

/*
    The AfxInsertArrayUnits() lets you insert a batch of units/elements into the array at a given base index, 
    shifting the existing elements after that index to make space.
    It's the batched, shifting-aware version of AfxPushArrayUnits().
*/

AFXINL afxError     AfxInsertArrayUnits
(
    afxArray*       arr,
    afxUnit         baseIdx,
    afxUnit         cnt,
    void const*     src,
    afxUnit         srcStride
);

AFXINL afxError     AfxRemoveArrayUnits
(
    afxArray*       arr, 
    afxUnit         idx, 
    afxUnit         cnt
);

/*
    The AfxSwapArray() function swaps the contents and metadata between two arrays.
    All done without copying the actual array contents, just the struct fields. Fast and zero allocations.

    Stride should match if you're going to use the arrays interchangeably after swap. If they're different types, this may lead to bugs.
    If the arrays are tied to a memory context or allocator, make sure that context is swappable too (if it's part of afxArray).
    This only swaps metadata and pointers, not the actual memory blocks.

    Use cases:
    Swapping two arrays of the same type for double buffering.
    Returning large arrays from a function by swapping into an output parameter.
    Temporarily stashing data without reallocation.
*/

AFXINL afxError     AfxSwapArray
(
    afxArray*       arr, 
    afxArray*       other
);

/*
    The AfxCloneArray() deeply copies the contents of one array into another, allocating new memory as needed.
*/

AFXINL afxError     AfxCloneArray
(
    afxArray*       arr, 
    afxArray const* src,
    afxUnit         baseIdx,
    afxUnit         cnt
);

/*
    The AfxAppendArray() function appends the contents of one array to another.
*/

AFXINL afxError     AfxAppendArray
(
    afxArray*       arr, 
    afxArray const* src
);

/*
    The AfxCopyArray() function copies a range of units or an entire array into another.
    This is the ultimate generic array memory copier for tightly packed data blobs, structured fields, or memory overlays.
    This function isn't just about moving values - it allows cross-field, cross-structure, even partial-type transfers 
    between arrays of the same or different types.
*/

AFXINL afxError AfxCopyArray
(
    // How many units to copy.
    afxUnit         unitCnt,
    // Number of bytes to copy per unit.
    afxUnit         size,
    // Source array.
    afxArray const* src,
    // Index in source array to start reading.
    afxUnit         srcUnitIdx,
    // Byte offset into each source unit.
    afxUnit         srcOffset,
    // Destination array.
    afxArray*       dst,
    // Index in destination array to start writing.
    afxUnit         toUnitIdx,
    // Byte offset into each destination unit.
    afxUnit         toOffset
);

/*
    The AfxDumpArray() function copies a range of units from the internal array into a destination buffer, 
    possibly skipping bytes via stride, and optionally starting from a byte offset inside each unit.

    @firstUnit + @unitCnt defines the range of elements to dump.
    @offset defines how far into each unit we read from (e.g., to grab just a member field).
    @stride defines how far apart each destination element is in memory.
    @dst can be a tightly packed buffer, a struct array, a GPU upload buffer, etc.
*/

AFXINL void         AfxDumpArray
(
    // source array.
    afxArray const* arr, 
    // first unit to dump.
    afxUnit         firstUnit, 
    // how many units to dump.
    afxUnit         unitCnt, 
    // byte offset inside each unit (to extract a field).
    afxUnit         offset, 
    // destination buffer.
    void*           dst, 
    // spacing between dst units in bytes.
    afxUnit         stride
);

/*
    The AfxUpdateArray() function updates a specific region of the internal array with data from an external source, 
    supporting partial field updates via offset, range-based updates (firstUnit, unitCnt), 
    strided input (e.g., tightly-packed, interleaved, or padded source data).
    It's essentially a field-aware, batched memcpy with offset into the array.
*/

AFXINL void         AfxUpdateArray
(
    // destination array (to be modified).
    afxArray*       arr, 
    // index of the first unit to update.
    afxUnit         firstUnit, 
    // number of units to update.
    afxUnit         unitCnt, 
    // byte offset into each unit.
    afxUnit         offset, 
    // source buffer to copy from.
    void const*     src, 
    // spacing between source units in bytes.
    afxUnit         stride
);

AFXINL void*        AfxGetArrayUnit
(
    afxArray const* arr, 
    afxUnit         unitIdx
);

AFX afxBool         AfxLookUpArray
(
    afxArray*       arr, 
    void*           data, 
    afxUnit*        elemIdx
);

AFX afxError        AfxPushArrayUnique
(
    afxArray*       arr, 
    void*           data, 
    afxUnit*        elemIdx
);

// ELEMENT

#define AFX_COPY_ARRAY(cnt_, src_, srcIdx_, srcType_, srcField_, dst_, dstIdx_, dstType_, dstField_) \
    AfxCopyArray((cnt_), sizeof(((srcType_*)0)->srcField, (src_), (srcIdx_), offsetof(srcType_, srcField_), (dst_), (dstIdx_), offsetof(dstType_, dstField_)))

#define AFX_ITERATE_ARRAY_TYPED(TYPE_, varName_, arr_) \
    for (afxUnit varName_##Idx = 0; \
         (arr_) && (varName_##Idx < (arr_)->pop) && \
         (((varName_) = (TYPE_ *)((afxByte *)(arr_)->bytemap + (varName_##Idx * (arr_)->unitSiz))), TRUE); \
         ++varName_##Iter)

#if _AFX_USE_TYPEOF
#define AFX_ITERATE_ARRAY(var_, arr_) \
    for (afxUnit var_##Iter = 0; \
         (var_##Iter < (arr_)->pop) && \
         (((var_) = (AFX_TYPEOF(var_))((afxByte*)(arr_)->bytemap + (var_##Iter * (arr_)->unitSiz))), TRUE); \
         ++var_##Iter)
#else
// Pure-C99 version that doesn’t rely on __typeof__ at all.
#define AFX_ITERATE_ARRAY(var_, arr_) \
    for (afxUnit var_##Iter = 0; \
         (arr_) && (var_##Iter < (arr_)->pop) && \
         (((var_) = (void *)((afxByte*)(arr_)->bytemap + (var_##Iter * (arr_)->unitSiz))), TRUE); \
         ++var_##Idx)
#endif

#if defined(_AFX_USE_TYPEOF)
#  define AFX_GET_ARRAY_ELEMENT(var_, arr_, idx_) \
      ((var_) = ((AFX_TYPEOF(var_))((afxByte *)(arr_)->bytemap + ((idx_) * (arr_)->unitSiz))))
#else
#  define AFX_GET_ARRAY_ELEMENT(var_, arr_, idx_) \
      ((var_) = ((void*)((afxByte *)(arr_)->bytemap + ((idx_) * (arr_)->unitSiz)))
#endif

#endif//AFX_ARRAY_H
