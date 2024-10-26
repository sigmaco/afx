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
typedef struct afxLinkage afxLinkage;
typedef struct afxLinkage2 afxLinkage2;

struct afxLinkage
{
    union
    {
        afxLinkage* next;
        afxAtomPtr nextA;
    };
    union
    {
        afxLinkage* prev;
        afxAtomPtr prevA;
    };
    union
    {
        afxChain*   chain;
        afxAtomPtr  chainA;
    };
};

struct afxChain 
{
    afxLinkage      anchor;
    union
    {
        void*       holder;
        afxFcc*     holderFcc;
    };
    /*afxAtomic*/afxInt cnt;
};

AFXINL void                 AfxDeployChain(afxChain *ch, void *holder);

AFXINL afxUnit              AfxGetChainLength(afxChain const *ch);
AFXINL void*                AfxGetChainHolder(afxChain const *ch);

AFXINL void                 AfxSwapChains(afxChain *ch, afxChain *other);
AFXINL afxLinkage*          AfxGetLastLinkage(afxChain const *ch); // last in (B2F)
AFXINL afxLinkage*          AfxGetFirstLinkage(afxChain const *ch); // first in (B2F)
AFXINL afxLinkage*          AfxGetAnchor(afxChain *ch);
AFXINL afxLinkage const*    AfxGetAnchorConst(afxChain const *ch);
AFXINL afxBool              AfxChainIsEmpty(afxChain const *ch);

AFXINL afxBool              AfxFindChainLinkageIndex(afxChain const *ch, afxLinkage *lnk, afxUnit *idx);
AFXINL afxBool              AfxFindChainLinkageIndexB2F(afxChain const *ch, afxLinkage *lnk, afxUnit *idx);
AFXINL afxLinkage*          AfxFindLastLinkage(afxChain const *ch, afxUnit idx);
AFXINL afxLinkage*          AfxFindFirstLinkage(afxChain const *ch, afxUnit idx);
AFXINL afxError             AfxChainPushLinkageAfter(afxChain *ch, afxUnit idx);
AFXINL afxError             AfxChainPopLinkage(afxChain *ch, afxUnit idx);


AFXINL afxResult            AfxChainEnumerateLinkages(afxChain *ch, afxBool reverse, afxUnit base, afxUnit cnt, afxLinkage *lnk[]);

AFXINL afxUnit               AfxInvokeLinkages(afxChain *ch, afxBool fromLast, afxUnit first, afxUnit cnt, afxBool(*f)(afxLinkage *lnk, void *udd), void *udd);

AFXINL void     AfxResetLinkage(afxLinkage *lnk);
AFXINL afxUnit   AfxPushLinkage(afxLinkage *lnk, afxChain *ch);
AFXINL afxUnit   AfxPushBackLinkage(afxLinkage *lnk, afxChain *ch);
AFXINL void     AfxPopLinkage(afxLinkage *lnk);


AFXINL afxUnit AfxLinkBehind(afxLinkage* lnk, afxLinkage* next);
AFXINL afxUnit AfxLinkAhead(afxLinkage* lnk, afxLinkage* prev);

AFXINL void* AfxGetLinker(afxLinkage const *lnk); // linker is the chain holder

AFXINL afxChain* AfxGetChain(afxLinkage const *lnk);

AFXINL afxLinkage* AfxGetNextLinkage(afxLinkage const *lnk);

AFXINL afxLinkage* AfxGetPrevLinkage(afxLinkage const *lnk);

#define AfxChainForEveryLinkage2(ch_, type_, offset_, lnk_) for (afxLinkage const*_next##lnk2_ = (afxLinkage*)NIL, *_curr##lnk2_ = (ch_)->anchor.next; _next##lnk2_ = (_curr##lnk2_)->next, lnk_ = (type_*)AFX_REBASE(_curr##lnk2_, type_, offset_), (_curr##lnk2_) != &(ch_)->anchor; _curr##lnk2_ = _next##lnk2_)
#define AfxChainForEveryLinkage(ch_, type_, offset_, lnk_) for (afxLinkage const*_next##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.next; _next##lnk_ = (_curr##lnk_)->next, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _next##lnk_)
#define AfxChainForEveryLinkageB2F(ch_, type_, offset_, lnk_) for (afxLinkage const*_last##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.prev; _last##lnk_ = (_curr##lnk_)->prev, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _last##lnk_)

#define AfxIterateLinkage(type_, lnk_, ch_, offset_) for (afxLinkage const*_next##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.next; _next##lnk_ = (_curr##lnk_)->next, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _next##lnk_)
#define AfxIterateLinkageB2F(type_, lnk_, ch_, offset_) for (afxLinkage const*_last##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.prev; _last##lnk_ = (_curr##lnk_)->prev, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _last##lnk_)

#endif//AFX_CHAIN_H
