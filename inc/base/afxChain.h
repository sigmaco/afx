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
{
    union
    {
        afxLink* next;
        afxAtomPtr nextA;
    };
    union
    {
        afxLink* prev;
        afxAtomPtr prevA;
    };
    union
    {
        afxChain*   chain;
        afxAtomPtr  chainA;
    };
    void*           held;
};

struct afxChain 
{
    afxLink     anchor;
    void*       holder;
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

#define AFX_ITERATE_CHAIN2(ch_, type_, offset_, lnk_) for (afxLink const*_next##lnk2_ = (afxLink*)NIL, *_curr##lnk2_ = (ch_)->anchor.next; _next##lnk2_ = (_curr##lnk2_)->next, lnk_ = (type_*)AFX_REBASE(_curr##lnk2_, type_, offset_), (_curr##lnk2_) != &(ch_)->anchor; _curr##lnk2_ = _next##lnk2_)
#define AFX_ITERATE_CHAIN(ch_, type_, offset_, lnk_) for (afxLink const*_next##lnk_ = (afxLink*)NIL, *_curr##lnk_ = (ch_)->anchor.next; _next##lnk_ = (_curr##lnk_)->next, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _next##lnk_)
#define AFX_ITERATE_CHAIN_B2F(ch_, type_, offset_, lnk_) for (afxLink const*_last##lnk_ = (afxLink*)NIL, *_curr##lnk_ = (ch_)->anchor.prev; _last##lnk_ = (_curr##lnk_)->prev, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _last##lnk_)

#define AfxIterateLinkage(type_, lnk_, ch_, offset_) for (afxLink const*_next##lnk_ = (afxLink*)NIL, *_curr##lnk_ = (ch_)->anchor.next; _next##lnk_ = (_curr##lnk_)->next, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _next##lnk_)
#define AfxIterateLinkageB2F(type_, lnk_, ch_, offset_) for (afxLink const*_last##lnk_ = (afxLink*)NIL, *_curr##lnk_ = (ch_)->anchor.prev; _last##lnk_ = (_curr##lnk_)->prev, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _last##lnk_)

#endif//AFX_CHAIN_H
