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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define _AFX_ARENA_C
#include "../impl/afxExecImplKit.h"

AFX_DEFINE_STRUCT(afxArenaCleanup)
{
    void(*action)(void *data,void*extra);
    void *data;
    void *extra;
};

AFX_DEFINE_STRUCT(afxArenaRecycleItem)
{
    afxArenaRecycleItem* next;
};

AFX_DEFINE_STRUCT(afxArenaLargeItem)
{
    afxArenaLargeItem* next;
    afxArenaLargeItem* prev;
    //afxUnit siz; // afx
};
#if 0
AFX_OBJECT(afxArena)
{
    AFX_OBJECT(afxMmu)    all;
    afxMmu                   mem;
    afxSize                     totalAllocated;
    afxSize                     smallItems;
    afxSize                     largeItems;
    afxSize                     chunkCnt;
    afxSize                     unusedSpace; /* Unused space due to alignment, etc. */

    afxSize                     allocated;
    char                        *initialData;
    char                        *data;

    void*                       (*alloc)(afxSize);
    void                        (*dealloc)(void *);

    afxSize                     maxCleanupCnt;
    afxSize                     cleanupCnt;
    afxArenaCleanup*            cleanups;
    afxArenaLargeItem*          largeList;

    afxSize                     chunkSiz;
    afxSize                     largeItemSiz;

     // se não for NIL, reciclagem está habilitada.
    // Isto é um array de linked list de partes mantidas para reciclagem.
    // As partes são todas ponteiros para dentro dos chunks alocados.
    // Array[i] aponta para itens de tamanho 'i'.
    afxArenaRecycleItem**       recycleBin;
    // E aqui o montante de memória armazenada na reciclagem.
    afxSize		                recycleSiz;
};
#endif
#define DEFAULT_CHUNK_SIZE         4096
#define DEFAULT_LARGE_OBJECT_SIZE  (DEFAULT_CHUNK_SIZE / 8)
#define DEFAULT_INITIAL_CLEANUP_SIZE 16


/*
 * mmap allocator constants
 *
 */
#ifdef USE_MMAP_ALLOC

 /* header starts with afxSize containing allocated size info and has at least 16 bytes to align the returned memory */
#define MMAP_ALLOC_HEADER_SIZE (sizeof(afxSize) >= 16 ? (sizeof(afxSize)) : 16)

/* mmap allocator uses chunks of 32 4kB pages */
#define MMAP_ALLOC_CHUNK_SIZE		((32 * 4096) - MMAP_ALLOC_HEADER_SIZE)
#define MMAP_ALLOC_LARGE_OBJECT_SIZE	(MMAP_ALLOC_CHUNK_SIZE / 8)
#define MMAP_ALLOC_INITIAL_CLEANUP_SIZE	16

#endif /* USE_MMAP_ALLOC */

/*
 * Create a new aren.
 */
_AFX void* AfxArenaCtor(afxArena* result);


/*
 * Create a new aren, with chunk size and large object size.
 * Note that largeItemSiz must be <= chunkSiz.
 * Anything larger than the large object size is individually alloced.
 * largeItemSiz = chunkSiz/8 is reasonable;
 * initial_cleanup_size is the number of preallocated ptrs for cleanups.
 * The cleanups are in a growing array, and it must start larger than zero.
 * If recycle is true, environmentally friendly memory recycling is be enabled.
 */
_AFX void* AfxArenaCtor2(afxArena* result, afxSize chunkSiz, afxSize largeItemSiz, afxSize initialCleanupSiz, int recycle);


/*
 * Destroy REGION.  All memory associated with REGION is freed as if
 * AfxExhaustArena was called.
 */
_AFX void AfxArenaDtor(afxArena* aren);


/*
 * Add a cleanup to REGION.  ACTION will be called with DATA as
 * parameter when the aren is freed or destroyed.
 *
 * Returns 0 on failure.
 */
_AFX afxSize AfxPushArenaCleanup(afxArena* aren, void(*action)(void *data,void*extra), void *data,void*extra);

/*
 * Remove cleanup, both action and data must match exactly.
 */
_AFX afxBool AfxPopArenaCleanup(afxArena* aren, void(*action)(void *,void*), void *data);

/*
 * Allocate SIZE bytes of memory inside REGION.  The memory is
 * deallocated when AfxExhaustArena is called for this aren.
 */
//_AFX void *AfxRequestArenaUnit(afxArena* aren, afxSize size);

/** Allocate array with integer overflow checks, in aren */
_AFX void *AfxRequestArenaUnits(afxArena* aren, afxSize num, afxSize size);

/*
 * Allocate SIZE bytes of memory inside REGION and copy INIT into it.
 * The memory is deallocated when AfxExhaustArena is called for this
 * aren.
 */
_AFX void *AfxRequestArenaCopy(afxArena* aren, afxSize size, const void *init);

/**
 * Allocate array (with integer overflow check on sizes), and init with
 * the given array copied into it.  Allocated in the aren
 */
_AFX void *AfxRequestArenaCopies(afxArena* aren, afxSize num, afxSize size, const void *init);

/*
 * Allocate SIZE bytes of memory inside REGION that are initialized to
 * 0.  The memory is deallocated when AfxExhaustArena is called for
 * this aren.
 */
_AFX void *AfxRequestArenaZeroedUnit(afxArena* aren, afxSize size);

/**
 * Allocate array (with integer overflow check on sizes), and zero it.
 * Allocated in the aren.
 */
_AFX void *AfxRequestArenaZeroedUnits(afxArena* aren, afxSize num, afxSize size);

/*
 * Run the cleanup actions and free all memory associated with REGION.
 */
_AFX void AfxExhaustArena(afxArena* aren);


/*
 * Duplicate STRING and allocate the result in REGION.
 */
_AFX char *AfxArenaDuplicateString(afxArena* aren, const char *string);

/*
 * Recycle an allocated memory block. Pass size used to alloc it.
 * Does nothing if recycling is not enabled for the aren.
 */
_AFX void AfxRecycleArenaUnit(afxArena* aren, void *block, afxSize size);

/*
 * Print some REGION statistics to OUT.
 */
_AFX void AfxDumpArenaStats(afxArena* aren, afxStream out);

/* get size of recyclebin */
_AFX afxSize AfxGetArenaRecycleSize(afxArena* aren);
/* get size of aren memory in use */
_AFX afxSize AfxGetArenaTotalAllocated(afxArena* aren);
/* get size of aren memory unused */
_AFX afxSize AfxGetArenaUnusedSpace(afxArena* aren);

/* Debug print REGION statistics to LOG. */
_AFX void AfxLogArenaStats(afxArena* aren);

/** This value is enough so that x*y does not overflow if both < than this */
#define _AFX_ARENA_NO_OVERFLOW ((afxSize)1 << (sizeof(afxSize) * 4))

#ifdef ALIGNMENT
#undef ALIGNMENT
#endif
#ifndef PACKED_STRUCTS
#define _AFX_ARENA_ALIGN_UP(x, s)     (((x) + s - 1) & (~(s - 1)))
#if SIZEOF_OFF_T > SIZEOF_VOIDP
#define ALIGNMENT	(sizeof(off_t))
#else
#define ALIGNMENT	(sizeof(void *))
#endif
#else
#define _AFX_ARENA_ALIGN_UP(x, s) ((x)<SIZEOF_VOIDP?SIZEOF_VOIDP:(x))
#define ALIGNMENT 1
#endif /* PACKED_STRUCTS */
/* #define CHECK_DOUBLE_FREE 0 */ /* set to 1 to perform expensive check for double recycle() */

_AFX void* AfxArenaCtor2(afxArena* result, afxSize chunkSiz, afxSize largeItemSiz, afxSize initial_cleanupCnt, int recycle)
{
    afxError err = AFX_ERR_NONE;

    result->totalAllocated = 0;
    result->smallItems = 0;
    result->largeItems = 0;
    result->chunkCnt = 1;
    result->unusedSpace = 0;
    result->recycleEnabled = !!recycle;
    result->recycleBin = NIL;
    result->recycleSiz = 0;
    result->largeList = NIL;

    result->allocated = 0;
    result->data = NIL;
    result->initialData = NIL;

    //result->alloc = allocator;
    //result->dealloc = deallocator;

    AFX_ASSERT(initial_cleanupCnt > 0);
    result->maxCleanupCnt = initial_cleanupCnt;
    result->cleanupCnt = 0;
    result->cleanups = NIL;

    result->chunkSiz = DEFAULT_CHUNK_SIZE;
    result->largeItemSiz = DEFAULT_LARGE_OBJECT_SIZE;

    AFX_ASSERT(largeItemSiz <= chunkSiz);
    result->chunkSiz = chunkSiz;
    result->largeItemSiz = largeItemSiz;

    if (result->chunkSiz > 0)
    {
#if 0
        result->data = (char *)AfxAllocate(1, result->chunkSiz, 0, AfxHere());

        if (!result->data)
        {
            AfxDeallocate(result->cleanups);
            //AfxDeallocate(result->mem, result);
            return NIL;
        }
        result->initialData = result->data;
#endif
    }
    return result;
}

_AFX void AfxArenaDtor(afxArena* aren)
{
    if (!aren)
        return;

    afxMmu mmu = aren->mmu;

    AfxExhaustArena(aren);

    if (aren->cleanups)
        AfxDeallocate((void**)&aren->cleanups, AfxHere());

    if (aren->initialData)
        AfxDeallocate((void**)&aren->initialData, AfxHere());

    if (aren->recycleBin)
        AfxDeallocate((void**)&aren->recycleBin, AfxHere());

    if (aren->largeList)
    {
        afxArenaLargeItem* p = aren->largeList, *np;

        while (p)
        {
            np = p->next;
            AfxDeallocate((void**)&p, AfxHere());
            p = np;
        }
    }
    //deallocator(aren);
}

_AFX afxSize AfxPushArenaCleanup(afxArena* aren, void(*action)(void *data, void*extra), void *data, void *extra)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(action);
    afxArenaCleanup *cleanups = aren->cleanups;

    if (!cleanups || (aren->cleanupCnt >= aren->maxCleanupCnt))
    {
        afxUnit maxCleanupCnt = (aren->cleanupCnt >= aren->maxCleanupCnt) ? 2 * aren->maxCleanupCnt : aren->maxCleanupCnt;

        AfxAllocate(maxCleanupCnt * sizeof(afxArenaCleanup), 0, AfxHere(), (void**)&cleanups);

        if (!cleanups) AfxThrowError();
        else
        {
            if (aren->cleanups)
            {
                AfxCopy(cleanups, aren->cleanups, sizeof(afxArenaCleanup) * aren->cleanupCnt);
                AfxDeallocate((void**)&aren->cleanups, AfxHere());
            }
            aren->cleanups = cleanups;
            aren->maxCleanupCnt = maxCleanupCnt;
        }
    }

    if (!err)
    {
        cleanups = aren->cleanups;
        cleanups[aren->cleanupCnt].action = action;
        cleanups[aren->cleanupCnt].data = data;
        cleanups[aren->cleanupCnt].extra = extra;

        ++aren->cleanupCnt;
        return aren->cleanupCnt;
    }
    return 0;
}

_AFX afxBool AfxPopArenaCleanup(afxArena* aren, void(*action)(void *,void*), void *data)
{
    afxSize i;

    for (i = 0; i < aren->cleanupCnt; i++)
    {
        afxArenaCleanup *cleanups = aren->cleanups;

        if (cleanups[i].action == action && cleanups[i].data == data)
        {
            aren->cleanupCnt--;
            cleanups[i] = cleanups[aren->cleanupCnt];
            break;
        }
    }
}

_AFX void _AfxArenDeallocBlockCleanupAction(void *data, void*extra)
{
    AfxDeallocate((void**)&data, AfxHere());
}

_AFX void* AfxRequestArenaUnit(afxArena* aren, afxSize size, afxUnit cnt, void const* src, afxUnit stride)
{
    afxError err = NIL;
    AFX_ASSERT(aren);

    if (!size)
    {
        AfxThrowError();
        return NIL;
    }

    // TODO: To be a parameter
    afxUnit align = AFX_SIMD_ALIGNMENT;

    afxSize aligned_size;
    
    void *result = NIL;

    aligned_size = _AFX_ARENA_ALIGN_UP(size, align) * cnt;

    if (aligned_size >= aren->largeItemSiz)
    {
        AfxAllocate(sizeof(afxArenaLargeItem) + (size * cnt), align, AfxHere(), (void**)&result);

        if (!result) AfxThrowError();
        else
        {
            ((afxArenaLargeItem*)result)->prev = NIL;
            ((afxArenaLargeItem*)result)->next = aren->largeList;
            //((afxArenaLargeItem*)result)->siz = size; // afx

            afxArenaLargeItem *largeList = aren->largeList;

            if (largeList)
                largeList->prev = (afxArenaLargeItem*)result;

            aren->largeList = (afxArenaLargeItem*)result;

            aren->totalAllocated += (size * cnt);
            ++aren->largeItems;

            void* ptr = (char *)result + sizeof(afxArenaLargeItem);

            if (src)
                AfxStream2(cnt, src, stride, ptr, size);
            else if (stride)
                AfxZero(ptr, cnt * size);

            return ptr;
        }
        return 0;
    }

    afxArenaRecycleItem** recycleBin = (afxArenaRecycleItem**)aren->recycleBin;

    if (recycleBin && recycleBin[aligned_size])
    {
        result = (void*)recycleBin[aligned_size];
        aren->recycleBin[aligned_size] = recycleBin[aligned_size]->next;
        aren->recycleSiz -= aligned_size;
        aren->unusedSpace += aligned_size - (size * cnt);

        if (src)
            AfxStream2(cnt, src, stride, result, size);
        else if (stride)
            AfxZero(result, cnt * size);

        return result;
    }

    if (!aren->initialData)
    {
        AfxAllocate(aren->chunkSiz, align, AfxHere(), (void**)&aren->initialData);

        if (!aren->initialData)
        {
            AfxThrowError();
            return 0;
        }
        aren->data = aren->initialData;
    }

    if (aren->allocated + aligned_size > aren->chunkSiz)
    {
        void *chunk;
        AfxAllocate(aren->chunkSiz, align, AfxHere(), (void**)&chunk);
        afxSize wasted;

        if (!chunk) AfxThrowError();
        else
        {
            wasted = (aren->chunkSiz - aren->allocated) & (~(align - 1));

            if (
#ifndef PACKED_STRUCTS
                wasted >= align
#else
                wasted >= SIZEOF_VOIDP
#endif
                )
            {
                /* put wasted part in recycle bin for later use */
                aren->totalAllocated += wasted;
                ++aren->smallItems;
                AfxRecycleArenaUnit(aren, aren->data + aren->allocated, wasted);
                aren->allocated += wasted;
            }

            ++aren->chunkCnt;
            aren->unusedSpace += aren->chunkSiz - aren->allocated;

            if (!AfxPushArenaCleanup(aren, _AfxArenDeallocBlockCleanupAction, chunk, aren))
            {
                AfxThrowError();
                AfxDeallocate((void**)&chunk, AfxHere());
                aren->chunkCnt--;
                aren->unusedSpace -= aren->chunkSiz - aren->allocated;
                return NIL;
            }

            if (!err)
            {
                aren->allocated = 0;
                aren->data = (char *)chunk;
            }
        }
    }

    if (!err)
    {
        result = aren->data + aren->allocated;
        aren->allocated += aligned_size;

        aren->totalAllocated += aligned_size;
        aren->unusedSpace += aligned_size - size;
        ++aren->smallItems;

        if (src)
            AfxStream2(cnt, src, stride, result, size);
        else if (stride)
            AfxZero(result, cnt * size);
    }
    return result;
}

_AFX void AfxExhaustArena(afxArena* aren)
{
    afxError err = AFX_ERR_NONE;
    afxSize i;
    AFX_ASSERT(aren);

    if (aren->cleanups)
    {
        i = aren->cleanupCnt;
        afxArenaCleanup *cleanups = aren->cleanups;

        while (i > 0)
        {
            --i;
            AFX_ASSERT(cleanups[i].action);
            cleanups[i].action(cleanups[i].data, cleanups[i].extra);
        }
    }

    if (aren->recycleBin)
    {
        AfxZero(aren->recycleBin, sizeof(afxArenaRecycleItem*) * aren->largeItemSiz);
        aren->recycleSiz = 0;
    }

    if (aren->largeList)
    {
        afxArenaLargeItem* p = aren->largeList, *np;
        //void(*deallocator)(void *) = aren->dealloc;

        while (p)
        {
            np = p->next;
            AfxDeallocate((void**)&p, AfxHere());
            p = np;
        }
        aren->largeList = NIL;
    }

    aren->data = aren->initialData;
    aren->cleanupCnt = 0;
    aren->allocated = 0;

    aren->totalAllocated = 0;
    aren->smallItems = 0;
    aren->largeItems = 0;
    aren->chunkCnt = 1;
    aren->unusedSpace = 0;
}

_AFX char* AfxArenaDuplicateString(afxArena* aren, const char *string)
{
    afxUnit len = strlen(string) + 1;
    return (char *)AfxRequestArenaUnit(aren, sizeof(char), len, string, len);
}

_AFX void AfxRecycleArenaUnit(afxArena* aren, void *block, afxSize size)
{
    afxError err = AFX_ERR_NONE;
    afxSize aligned_size;
    AFX_ASSERT(size);

    if (!aren->recycleBin && aren->recycleEnabled)
    {
        AfxAllocate(aren->largeItemSiz * sizeof(afxArenaRecycleItem*), 0, AfxHere(), (void**)&aren->recycleBin);

        if (!aren->recycleBin)
        {
            AfxThrowError();
            return;
        }
        AfxZero(aren->recycleBin, sizeof(afxArenaRecycleItem*) * aren->largeItemSiz);
    }

    if (!block || !aren->recycleBin)
        return;

    afxUnit align = AFX_SIMD_ALIGNMENT;
    aligned_size = _AFX_ARENA_ALIGN_UP(size, align);

    if (aligned_size < aren->largeItemSiz)
    {
        afxArenaRecycleItem* elem = (afxArenaRecycleItem*)block;
        /* we rely on the fact that ALIGNMENT is void* so the next will fit */
        AFX_ASSERT(aligned_size >= sizeof(afxArenaRecycleItem));

#ifdef CHECK_DOUBLE_FREE
        if (CHECK_DOUBLE_FREE)
        {
            /* make sure the same ptr is not freed twice. */
            afxArenaRecycleItem *p = aren->recycleBin[aligned_size];
            while (p) {
                AFX_ASSERT(p != elem);
                p = p->next;
            }
        }
#endif

        elem->next = aren->recycleBin[aligned_size];
        aren->recycleBin[aligned_size] = elem;
        aren->recycleSiz += aligned_size;
        aren->unusedSpace -= aligned_size - size;
        return;
    }
    else
    {
        afxArenaLargeItem* l;

        /* a large allocation */
        aren->totalAllocated -= size;
        --aren->largeItems;

        l = (afxArenaLargeItem*)((char*)block - sizeof(afxArenaLargeItem));

        if (l->prev)
            l->prev->next = l->next;
        else
            aren->largeList = l->next;

        if (l->next)
            l->next->prev = l->prev;

        AfxDeallocate((void**)&l, AfxHere());
    }
}

_AFX void AfxDumpArenaStats(afxArena* aren, afxStream out)
{
    afxString128 str;
    AfxMakeString128(&str, NIL);
    AfxFormatString(&str.s, "%lu objects (%lu small/%lu large), %lu bytes allocated (%lu wasted) in %lu chunks, %lu cleanups, %lu in recyclebin",
        (unsigned long)(aren->smallItems + aren->largeItems),
        (unsigned long)aren->smallItems,
        (unsigned long)aren->largeItems,
        (unsigned long)aren->totalAllocated,
        (unsigned long)aren->unusedSpace,
        (unsigned long)aren->chunkCnt,
        (unsigned long)aren->cleanupCnt,
        (unsigned long)aren->recycleSiz);
    AfxWriteStream(out, AfxGetStringLength(&str.s), 0, AfxGetStringData(&str.s, 0));

    if (aren->recycleBin)
    {
        /* print details of the recycle bin */
        afxSize i;
        for (i = 0; i < aren->largeItemSiz; i++)
        {
            afxSize count = 0;
            afxArenaRecycleItem* el = aren->recycleBin[i];
            while (el)
            {
                count++;
                el = el->next;
            }

            if (i%ALIGNMENT == 0 && i != 0)
            {
                AfxFormatString(&str.s, " %lu", (unsigned long)count);
                AfxWriteStream(out, AfxGetStringLength(&str.s), 0, AfxGetStringData(&str.s, 0));
            }
        }
    }
}

_AFX void AfxGetArenaInfo(afxArena* aren, afxUnit* totalAllocated, afxUnit* unusedSpace, afxUnit* recycleSiz)
{
    afxError err = NIL;
    AFX_ASSERT(aren);
    AFX_ASSERT(totalAllocated || unusedSpace || recycleSiz);
    *totalAllocated = aren->totalAllocated;
    *unusedSpace = aren->unusedSpace;
    *recycleSiz = aren->recycleSiz;
}

/* debug routine */
_AFX void AfxLogArenaStats(afxArena* aren)
{
    char buf[10240], *str = buf;
    int strl = sizeof(buf);
    int len;
    snprintf(str, strl, "%lu objects (%lu small/%lu large), %lu bytes allocated (%lu wasted) in %lu chunks, %lu cleanups, %lu in recyclebin",
        (unsigned long)(aren->smallItems + aren->largeItems),
        (unsigned long)aren->smallItems,
        (unsigned long)aren->largeItems,
        (unsigned long)aren->totalAllocated,
        (unsigned long)aren->unusedSpace,
        (unsigned long)aren->chunkCnt,
        (unsigned long)aren->cleanupCnt,
        (unsigned long)aren->recycleSiz);
    len = strlen(str);
    str += len;
    strl -= len;
    if (aren->recycleBin) {
        /* print details of the recycle bin */
        afxSize i;
        for (i = 0; i < aren->largeItemSiz; i++) {
            afxSize count = 0;
            afxArenaRecycleItem* el = aren->recycleBin[i];
            while (el) {
                count++;
                el = el->next;
            }
            if (i%ALIGNMENT == 0 && i != 0) {
                snprintf(str, strl, " %lu", (unsigned long)count);
                len = strlen(str);
                str += len;
                strl -= len;
            }
        }
    }
    AfxReportWarn("memory: %s", buf);
}

_AFX void AfxDismantleArena(afxArena* aren)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(aren, afxFcc_AREN);
    AfxExhaustArena(aren);
    AfxArenaDtor(aren);   
}

_AFX afxError AfxMakeArena(afxArena* aren, afxArenaInfo const *spec, afxHere const hint)
{
    afxError err = AFX_ERR_NONE;
    //AFX_ASSERT(spec);

    afxUnit sysPagSiz = AfxGetMemoryPageSize();
    afxUnit pageSiz = spec && spec->chunkSiz ? spec->chunkSiz : sysPagSiz;
    afxUnit initCleanupSiz = spec && spec->initialCleanupSiz ? spec->initialCleanupSiz : 16;
    afxUnit largeItemSiz = spec && spec->largeItemSiz ? spec->largeItemSiz : pageSiz / 8;
    afxBool recycle = spec && spec->recycle ? spec->recycle : TRUE;

    AfxZero(aren, sizeof(*aren));
#ifdef _AFX_ARENA_VALIDATION_ENABLED
    aren->fcc = afxFcc_AREN;
#endif
    aren->mmu = NIL;

    aren->hint[0] = hint[0];
    aren->hint[1] = hint[1];
    aren->hint[2] = hint[2];

    if (!AfxArenaCtor2(aren, pageSiz, largeItemSiz, initCleanupSiz, recycle))
        AfxThrowError();
    
    return err;
}
