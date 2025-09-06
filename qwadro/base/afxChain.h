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
 // INTRUSIVE DOUBLY-LINKED CIRCULAR LIST                                    //
//////////////////////////////////////////////////////////////////////////////

/*
    afxChain is a powerful example of an intrusive doubly-linked list implementation that makes use of manual memory management, 
    type punning via unions, and circular list design. Intrusive design means the node is embedded in a larger object (e.g., a user-defined struct), 
    not allocated separately. This is super efficient and great for real-time systems.
    
    This is a goldmine of low-level software architecture and engineering. You will work with a robust, high-performance, 
    custom intrusive list system with both conventional and atomic-safe variants, forward and backward traversal, 
    and rich tooling for embedded container access and flexible manipulation.

    DESIGN:
    Iteration. Walk through from chain->anchor.next until you loop back to &anchor.
    Insertions. Use pointer manipulations to splice afxLinks into the chain before/after others.
    Removals. Remove a node by re-linking its neighbors.
    Intrusive List. Node metadata (afxLink) is embedded in the user structure. Zero-allocation, fast, cache-local, low overhead.
    Circular Anchor. Avoids null checks; simplifies iteration and boundary cases. Sentinel pattern removes null checks, which is very CPU branch-friendly.
    Unions. Support for generic pointer types or abstraction (afxAtomPtr).
    Back-Reference. Links know their chain (chain field). Enables metadata tracking, reverse lookups, O(1) removals or validation from links to the list.
    Held Pointer. Can store additional node-specific payloads or metadata.
    Flexible macros. Iteration macros handle most access. Mimics Linux kernel macros like list_for_each_entry.
    Customizability. Macros for both forward, reverse, and atomic. Control over traversal, even under concurrent modification.
    Atomic pointer variants. afxAtomPtr, *_A fields and macros. Optional lock-free traversal for SPSC or MPMC patterns.

    RISKS:
    Memory Safety: You must ensure nodes aren’t freed while in use.
    Thread Safety: Only atomic traversal is safe in concurrent access scenarios; insertion/removal is not lock-free unless extended.
    Error-Prone: Like list_entry() in Linux kernel, this is "raw power" and requires careful use.
*/

#ifndef AFX_CHAIN_H
#define AFX_CHAIN_H

#include "qwadro/base/afxDebug.h"
#include "qwadro/exec/afxAtomic.h"

// std::list is a container that supports constant time insertion and removal of elements from anywhere in the container.
// Fast random access is not supported. It is usually implemented as a doubly-linked list.
// Compared to std::forward_list this container provides bidirectional iteration capability while being less space efficient.

// Adding, removing and moving the elements within the list or across several lists does not invalidate the iterators or references.
// An iterator is invalidated only when the corresponding element is deleted.

// std::list meets the requirements of Container, MemoryAwareContainer, SequenceContainer and ReversibleContainer.

typedef struct afxChain afxChain;
typedef struct afxChain2 afxChain2;
typedef struct afxLink afxLink;
typedef struct afxLink2 afxLink2;

struct afxLink
// Structure specifying a node in a intrusive list, which embeds an afxLink.
{
    // next and prev point to neighboring afxLink nodes.
    union { afxLink* next; afxAtomPtr nextA; };
    union { afxLink* prev; afxAtomPtr prevA; };
    // Back-references to the parent afxChain, useful for validation or bookkeeping.
    // The anchor in afxChain is an afxLink, acting as a sentinel.
    union { afxChain* chain; afxAtomPtr chainA; };
    // Arbitrary per-node payload pointer, maybe for additional metadata or ownership tracking.
    void* held;
};

struct afxChain 
// Structure specifying a "intrusive" doubly-linked list.
// It is intrusive, meaning each node embeds an afxLink somewhere inside it.
{
    // The sentinel node, part of a circular doubly-linked list.
    // anchor.next and anchor.prev always point to the actual first and last nodes 
    // (or to &anchor itself if the list is empty), making it always circular.
    afxLink     anchor;
    // A back-reference or context.
    void*       holder;
    // Count of nodes in the list.
    afxUnit     cnt;
};


// INITIALIZATION AND METADATA

/*
    The AfxDeplyChain() function initializes a chain. Basically a constructor; makes the list safe for use.
*/

AFXINL void             AfxDeployChain
(
    afxChain*           ch,
    void*               holder
);

AFXINL void             AfxSwapChains
(
    afxChain*           ch, 
    afxChain*           other
);

/*
    The AfxAppendChain() function appends all links from @other to the end of @ch.
    It is an efficient list splicer. Efficient because it's intrusive (no copying).
    The @other is emptied in the process (like a "move").
    The 'chain' pointer in each link is updated for ownership tracking.
*/

AFXINL void AfxAppendChain(afxChain* ch, afxChain* other);

/*
    The AfxPrependChain() function prepends all links from @other to the start of @ch.
    It is an efficient list splicer. Efficient because it's intrusive (no copying).
    The @other is emptied in the process (like a "move").
    The 'chain' pointer in each link is updated for ownership tracking.
*/

AFXINL void AfxPrependChain(afxChain* ch, afxChain* other);

/*
    The AfxGetChainLength() function retrieves the length (the number of links) of a chain.
    Optimized for fast O(1) access without iterating.
*/

AFXINL afxUnit          AfxGetChainLength(afxChain const *ch);

/*
    The AfxGetChainHolder() function retrieves the holder specified in chain deployment.
    Optimized for fast O(1) access without iterating.
*/

AFXINL void*            AfxGetChainHolder(afxChain const *ch);

// ACCESS AND NAVIGATION

/*
    The AfxIsChainEmpty() function checks if does not have any link in chain.
*/

AFXINL afxBool          AfxIsChainEmpty(afxChain const *ch);

/*
    The AfxGetLastLink() function
*/

AFXINL afxLink*         AfxGetLastLink(afxChain const *ch); // last in (B2F)
AFXINL afxLink*         AfxGetFirstLink(afxChain const *ch); // first in (B2F)

/*
    The AfxGetAnchor() function retrieves the chain sentinel.
*/

AFXINL afxLink*         AfxGetAnchor(afxChain *ch);

/*
    The AfxGetAnchorConst() function retrieves the chain sentinel as constant.
*/

AFXINL afxLink const*   AfxGetAnchorConst(afxChain const *ch);

// INSERTION AND REMOVAL

AFXINL void             AfxResetLink(afxLink *lnk);

/*
    The AfxPushLink() relinks it in front of the chain.
    This do O(1) pointer rewiring to insert nodes at either front.
*/

AFXINL afxUnit          AfxPushLink(afxLink *lnk, afxChain *ch);

/*
    The AfxPushBackLink() relinks it in back of the chain.
    This do O(1) pointer rewiring to insert nodes at either back.
*/

AFXINL afxUnit          AfxPushBackLink(afxLink *lnk, afxChain *ch);

/*
    The AfxPopLink() function detaches a node. Resets lnk's internal fields to avoid dangling pointers.
*/

AFXINL void             AfxPopLink(afxLink *lnk);

/*
    The AfxLinkBehind() function insert lnk after a target node.
    This gives full control (not just ends) supports sorting, reordering.
*/

AFXINL afxUnit          AfxLinkBehind(afxLink* lnk, afxLink* next);

/*
    The AfxLinkAhead() function insert lnk before a target node.
    This gives full control (not just ends) supports sorting, reordering.
*/

AFXINL afxUnit          AfxLinkAhead(afxLink* lnk, afxLink* prev);

// FINDING AND INDEXING

/*
    The AfxFindLinkIndex() function iterates through the list until it finds lnk, returns its index.
    Used for debugging, sorting, consistency checking.
*/

AFXINL afxBool          AfxFindLinkIndex(afxChain const *ch, afxLink *lnk, afxUnit *idx);

/*
    The AfxFindLinkIndexB2F() function iterates through the list until it finds lnk, returns its index.
    Used for debugging, sorting, consistency checking.
*/

AFXINL afxBool          AfxFindLinkIndexB2F(afxChain const *ch, afxLink *lnk, afxUnit *idx);

/*
    The AfxFindLastLink() function does indexed access into list nodes. 
    This is O(n), not O(1), but helps traverse specific elements.
*/

AFXINL afxLink*         AfxFindLastLink(afxChain const *ch, afxUnit idx);

/*
    The AfxFindFirstLink() function does indexed access into list nodes. 
    This is O(n), not O(1), but helps traverse specific elements.
*/

AFXINL afxLink*         AfxFindFirstLink(afxChain const *ch, afxUnit idx);

/*
    The () function .
*/

AFXINL void*            AfxGetLinker(afxLink const *lnk); // linker is the chain holder

AFXINL afxChain*        AfxGetChain(afxLink const *lnk);

AFXINL afxLink*         AfxGetNextLink(afxLink const *lnk);

AFXINL afxLink*         AfxGetPrevLink(afxLink const *lnk);

// ITERATION MACROS --- KERNEL-STYLE POWER

/*
    AFX_GET_LINKED_OBJECT is a macro for safe container lookup (container_of pattern).
*/

#define AFX_GET_LINKED_OBJECT(TYPE_, link_, offset_) \
    ((TYPE_*)AFX_REBASE(link_, TYPE_, offset_))

/*
    AFX_ITERATE_CHAIN is a powerful macro for iterating over intrusive linked lists (where links are embedded within larger structures). 
    It's efficient and similar in spirit to Linux kernel macros like list_for_each_entry.
    It iterates through a circular linked list that is anchored by pChain_. It allows access to each element of type TYPE_ through the pointer lnk_.
    It's built for a custom data structure defined with an afxLink struct for linking nodes.
*/

#define AFX_ITERATE_CHAIN(TYPE_, iterator_, link_, pChain_) \
    for (afxLink const* _next##iterator_ = (pChain_)->anchor.next, \
                      * _curr##iterator_ = _next##iterator_; \
         (_curr##iterator_ != &(pChain_)->anchor) && \
         ((iterator_) = (TYPE_*)AFX_REBASE(_curr##iterator_, TYPE_, link_), \
          _next##iterator_ = _curr##iterator_->next, 1); \
         _curr##iterator_ = _next##iterator_)

#define AFX_ITERATE_CHAIN2(TYPE_, iterator2_, link_, pChain_) \
    for (afxLink const* _next##iterator2_ = (pChain_)->anchor.next, \
                      * _curr##iterator2_ = _next##iterator2_; \
         (_curr##iterator2_ != &(pChain_)->anchor) && \
         ((iterator2_) = (TYPE_*)AFX_REBASE(_curr##iterator2_, TYPE_, link_), \
          _next##iterator2_ = _curr##iterator2_->next, 1); \
         _curr##iterator2_ = _next##iterator2_)

/*
    AFX_ITERATE_CHAIN_B2F is the reverse counterpart of AFX_ITERATE_CHAIN.
    This will iterate backwards through the list.
    This works by following the prev pointer instead of next.
    Like the forward iteration, the prev pointer is saved before anything else happens, 
    ensuring that the loop will continue safely even if nodes are removed.
    This version stays compact, keeping all logic inside the for condition.
*/

#define AFX_ITERATE_CHAIN_B2F(TYPE_, iterator_, link_, pChain_) \
    for (afxLink const* _prev##iterator_ = (pChain_)->anchor.prev, \
                      * _curr##iterator_ = _prev##iterator_; \
         (_curr##iterator_ != &(pChain_)->anchor) && \
         ((iterator_) = (TYPE_*)AFX_REBASE(_curr##iterator_, TYPE_, link_), \
          _prev##iterator_ = _curr##iterator_->prev, 1); \
         _curr##iterator_ = _prev##iterator_)

/*
    Important Notes on Atomic Iteration.
    These macros only ensure that pointer reads are atomic - they don't protect against the structure being mutated 
    concurrently unless the entire structure is designed for lock-free access.
    If you're going to be modifying links concurrently (e.g., popping, inserting), you’ll need compare-and-swap (CAS) 
    logic or hazard pointers to be truly safe.
    For lock-free consumption, e.g., one writer and one reader (SPSC), this is usually okay.
    For full MPMC lock-free structures, iteration macros may be unsafe unless they're combined with safe 
    traversal techniques (version counters, epoch-based GC, etc.).
*/

#define AFX_ITERATE_CHAIN_ATOMIC(TYPE_, iterator_, link_, pChain_, mem_order_) \
    for (afxLink const* _curr##iterator_ = atomic_load_explicit(&(pChain_)->anchor.nextA, mem_order_), \
                      * _next##iterator_; \
         (_curr##iterator_ != &(pChain_)->anchor) && \
         ((iterator_) = (TYPE_*)AFX_REBASE(_curr##iterator_, TYPE_, link_), \
          _next##iterator_ = atomic_load_explicit(&_curr##iterator_->nextA, mem_order_), 1); \
         _curr##iterator_ = _next##iterator_)

#define AFX_ITERATE_CHAIN_ATOMIC_RELAXED(TYPE_, iterator_, link_, pChain_) \
    AFX_ITERATE_CHAIN_ATOMIC(TYPE_, iterator_, link_, pChain_, memory_order_relaxed)

#define AFX_ITERATE_CHAIN_ATOMIC_B2F(TYPE_, iterator_, link_, pChain_, mem_order_) \
    for (afxLink const* _curr##iterator_ = atomic_load_explicit(&(pChain_)->anchor.prevA, mem_order_), \
                      * _prev##iterator_; \
         (_curr##iterator_ != &(pChain_)->anchor) && \
         ((iterator_) = (TYPE_*)AFX_REBASE(_curr##iterator_, TYPE_, link_), \
          _prev##iterator_ = atomic_load_explicit(&_curr##iterator_->prevA, mem_order_), 1); \
         _curr##iterator_ = _prev##iterator_)


#endif//AFX_CHAIN_H
