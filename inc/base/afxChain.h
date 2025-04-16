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

#ifndef AFX_CHAIN_H
#define AFX_CHAIN_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/exec/afxAtomic.h"

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

AFXINL void             AfxDeployChain(afxChain *ch, void *holder);

AFXINL afxUnit          AfxGetChainLength(afxChain const *ch);
AFXINL void*            AfxGetChainHolder(afxChain const *ch);

AFXINL void             AfxSwapChains(afxChain *ch, afxChain *other);
AFXINL afxLink*         AfxGetLastLink(afxChain const *ch); // last in (B2F)
AFXINL afxLink*         AfxGetFirstLink(afxChain const *ch); // first in (B2F)
AFXINL afxLink*         AfxGetAnchor(afxChain *ch);
AFXINL afxLink const*   AfxGetAnchorConst(afxChain const *ch);
AFXINL afxBool          AfxIsChainEmpty(afxChain const *ch);

AFXINL afxBool          AfxFindLinkIndex(afxChain const *ch, afxLink *lnk, afxUnit *idx);
AFXINL afxBool          AfxFindLinkIndexB2F(afxChain const *ch, afxLink *lnk, afxUnit *idx);
AFXINL afxLink*         AfxFindLastLink(afxChain const *ch, afxUnit idx);
AFXINL afxLink*         AfxFindFirstLink(afxChain const *ch, afxUnit idx);
AFXINL afxError         AfxPushLinkAfter(afxChain *ch, afxUnit idx);
AFXINL afxError         AfxChainPopLinkage(afxChain *ch, afxUnit idx);


AFXINL afxResult        AfxChainEnumerateLinkages(afxChain *ch, afxBool reverse, afxUnit base, afxUnit cnt, afxLink *lnk[]);

AFXINL afxUnit          AfxInvokeLinkages(afxChain *ch, afxBool fromLast, afxUnit first, afxUnit cnt, afxBool(*f)(afxLink *lnk, void *udd), void *udd);

AFXINL void             AfxResetLink(afxLink *lnk);
AFXINL afxUnit          AfxPushLink(afxLink *lnk, afxChain *ch);
AFXINL afxUnit          AfxPushBackLink(afxLink *lnk, afxChain *ch);
AFXINL void             AfxPopLink(afxLink *lnk);


AFXINL afxUnit          AfxLinkBehind(afxLink* lnk, afxLink* next);
AFXINL afxUnit          AfxLinkAhead(afxLink* lnk, afxLink* prev);

AFXINL void*            AfxGetLinker(afxLink const *lnk); // linker is the chain holder

AFXINL afxChain*        AfxGetChain(afxLink const *lnk);

AFXINL afxLink*         AfxGetNextLink(afxLink const *lnk);

AFXINL afxLink*         AfxGetPrevLink(afxLink const *lnk);

/*
    AFX_ITERATE_CHAIN is a powerful macro for iterating over intrusive linked lists (where links are embedded within larger structures). 
    It's efficient and avoids allocating wrapper nodes. It's similar in spirit to Linux kernel macros like list_for_each_entry.
    It iterates through a circular linked list that is anchored by ch_. It allows access to each element of type type_ through the pointer lnk_.
    It's built for a custom data structure defined with an afxLink struct for linking nodes.
*/

#define AFX_ITERATE_CHAIN(type_, lnk_, offset_, ch_) \
    for (afxLink const* _next##lnk_ = (ch_)->anchor.next, \
                      * _curr##lnk_ = _next##lnk_; \
         (_curr##lnk_ != &(ch_)->anchor) && \
         ((lnk_) = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), \
          _next##lnk_ = _curr##lnk_->next, 1); \
         _curr##lnk_ = _next##lnk_)

#define AFX_ITERATE_CHAIN2(type_, lnk_, offset_, ch_) \
    for (afxLink const* _next##lnk_2 = (ch_)->anchor.next, \
                      * _curr##lnk_2 = _next##lnk_2; \
         (_curr##lnk_2 != &(ch_)->anchor) && \
         ((lnk_) = (type_*)AFX_REBASE(_curr##lnk_2, type_, offset_), \
          _next##lnk_2 = _curr##lnk_2->next, 1); \
         _curr##lnk_2 = _next##lnk_2)

/*
    AFX_ITERATE_CHAIN_B2F is the reverse counterpart of AFX_ITERATE_CHAIN.
    This will iterate backwards through the list.
    This works by following the prev pointer instead of next.
    Like the forward iteration, the prev pointer is saved before anything else happens, 
    ensuring that the loop will continue safely even if nodes are removed.
    This version stays compact, keeping all logic inside the for condition.
*/

#define AFX_ITERATE_CHAIN_B2F(type_, lnk_, offset_, ch_) \
    for (afxLink const* _prev##lnk_ = (ch_)->anchor.prev, \
                      * _curr##lnk_ = _prev##lnk_; \
         (_curr##lnk_ != &(ch_)->anchor) && \
         ((lnk_) = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), \
          _prev##lnk_ = _curr##lnk_->prev, 1); \
         _curr##lnk_ = _prev##lnk_)

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

#define AFX_ITERATE_CHAIN_ATOMIC(type_, lnk_, offset_, ch_, mem_order_) \
    for (afxLink const* _curr##lnk_ = atomic_load_explicit(&(ch_)->anchor.nextA, mem_order_), \
                      * _next##lnk_; \
         (_curr##lnk_ != &(ch_)->anchor) && \
         ((lnk_) = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), \
          _next##lnk_ = atomic_load_explicit(&_curr##lnk_->nextA, mem_order_), 1); \
         _curr##lnk_ = _next##lnk_)

#define AFX_ITERATE_CHAIN_ATOMIC_RELAXED(type_, lnk_, offset_, ch_) \
    AFX_ITERATE_CHAIN_ATOMIC(type_, lnk_, offset_, ch_, memory_order_relaxed)

#define AFX_ITERATE_CHAIN_ATOMIC_B2F(type_, lnk_, offset_, ch_, mem_order_) \
    for (afxLink const* _curr##lnk_ = atomic_load_explicit(&(ch_)->anchor.prevA, mem_order_), \
                      * _prev##lnk_; \
         (_curr##lnk_ != &(ch_)->anchor) && \
         ((lnk_) = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), \
          _prev##lnk_ = atomic_load_explicit(&_curr##lnk_->prevA, mem_order_), 1); \
         _curr##lnk_ = _prev##lnk_)


#endif//AFX_CHAIN_H
