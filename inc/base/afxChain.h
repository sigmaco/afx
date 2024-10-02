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
    afxLinkage  *next, *prev;
    afxChain    *chain;
};

struct afxLinkage2
{
    afxAtomPtr next, prev;
    afxAtomPtr chain;
};

struct afxChain 
{
    afxLinkage      anchor;
    union
    {
        void*       holder;
        afxFcc*     holderFcc;
    };
    /*afxAtomic*/afxInt      cnt;
};

struct afxChain2
{
    afxLinkage2     anchor;
    union
    {
        afxAtomPtr  holder;
        afxFcc*     holderFcc;
    };
    afxAtom32       cnt;
};

// GRANT CHANGE MONITORING

AFXINL void                 AfxDeployChain(afxChain *ch, void *holder);

AFXINL afxNat               AfxGetChainLength(afxChain const *ch);
AFXINL void*                AfxGetChainHolder(afxChain const *ch);

AFXINL void                 AfxSwapChains(afxChain *ch, afxChain *other);
AFXINL afxLinkage*          AfxGetLastLinkage(afxChain const *ch); // last in (B2F)
AFXINL afxLinkage*          AfxGetFirstLinkage(afxChain const *ch); // first in (B2F)
AFXINL afxLinkage*          AfxGetAnchor(afxChain *ch);
AFXINL afxLinkage const*    AfxGetAnchorConst(afxChain const *ch);
AFXINL afxBool              AfxChainIsEmpty(afxChain const *ch);

AFXINL afxBool              AfxFindChainLinkageIndex(afxChain const *ch, afxLinkage *lnk, afxNat *idx);
AFXINL afxBool              AfxFindChainLinkageIndexB2F(afxChain const *ch, afxLinkage *lnk, afxNat *idx);
AFXINL afxLinkage*          AfxFindLastLinkage(afxChain const *ch, afxNat idx);
AFXINL afxLinkage*          AfxFindFirstLinkage(afxChain const *ch, afxNat idx);
AFXINL afxError             AfxChainPushLinkageAfter(afxChain *ch, afxNat idx);
AFXINL afxError             AfxChainPopLinkage(afxChain *ch, afxNat idx);


AFXINL afxResult            AfxChainEnumerateLinkages(afxChain *ch, afxBool reverse, afxNat base, afxNat cnt, afxLinkage *lnk[]);

AFXINL afxNat               AfxInvokeLinkages(afxChain *ch, afxBool fromLast, afxNat first, afxNat cnt, afxBool(*f)(afxLinkage *lnk, void *udd), void *udd);

AFXINL void     AfxResetLinkage(afxLinkage *lnk);
AFXINL afxNat   AfxPushLinkage(afxLinkage *lnk, afxChain *ch);
AFXINL afxNat   AfxPushBackLinkage(afxLinkage *lnk, afxChain *ch);
AFXINL void     AfxPopLinkage(afxLinkage *lnk);


AFXINL afxNat AfxLinkBehind(afxLinkage* lnk, afxLinkage* next);
AFXINL afxNat AfxLinkAhead(afxLinkage* lnk, afxLinkage* prev);

AFXINL void* AfxGetLinker(afxLinkage const *lnk); // linker is the chain holder

AFXINL afxChain* AfxGetChain(afxLinkage const *lnk);

AFXINL afxLinkage* AfxGetNextLinkage(afxLinkage const *lnk);

AFXINL afxLinkage* AfxGetPrevLinkage(afxLinkage const *lnk);

#define AfxChainForEveryLinkage2(ch_, type_, offset_, lnk_) for (afxLinkage const*_next##lnk2_ = (afxLinkage*)NIL, *_curr##lnk2_ = (ch_)->anchor.next; _next##lnk2_ = (_curr##lnk2_)->next, lnk_ = (type_*)AFX_REBASE(_curr##lnk2_, type_, offset_), (_curr##lnk2_) != &(ch_)->anchor; _curr##lnk2_ = _next##lnk2_)
#define AfxChainForEveryLinkage(ch_, type_, offset_, lnk_) for (afxLinkage const*_next##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.next; _next##lnk_ = (_curr##lnk_)->next, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _next##lnk_)
#define AfxChainForEveryLinkageB2F(ch_, type_, offset_, lnk_) for (afxLinkage const*_last##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.prev; _last##lnk_ = (_curr##lnk_)->prev, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _last##lnk_)

#define AfxIterateLinkage(type_, lnk_, ch_, offset_) for (afxLinkage const*_next##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.next; _next##lnk_ = (_curr##lnk_)->next, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _next##lnk_)
#define AfxIterateLinkageB2F(type_, lnk_, ch_, offset_) for (afxLinkage const*_last##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.prev; _last##lnk_ = (_curr##lnk_)->prev, lnk_ = (type_*)AFX_REBASE(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _last##lnk_)

//#define AfxGetFirstLinked(ch_, t_, m_) AfxRebase((1 * AfxGetFirstLinkage((ch_))), t_, m_)

AFX_DEFINE_STRUCT(afxLink)
{
    afxLink *next, *prev;
};

AFX_DEFINE_STRUCT(afxList)
{
    afxLink head;
};

AFXINL afxLink* AfxGetListHead(afxList *lst);

AFXINL afxLink const* AfxGetConstListHead(afxList const *lst);

// *** afxLink ***

AFXINL afxLink* AfxGetNextLink(afxLink const *lnk);

AFXINL afxLink* AfxGetPrevLink(afxLink const *lnk);

AFXINL void AfxDeployLink(afxLink *lnk);

AFXINL afxBool AfxIsLinkUnused(afxLink const *lnk);

AFXINL void AfxPrependLink(afxLink *lnk, afxList *lst);

AFXINL void AfxDropLink(afxLink *lnk);

AFXINL void AfxRelink(afxLink *lnk, afxLink *to);

AFXINL void AfxSwapLinks(afxLink *lnk, afxLink *other);

// *** afxLinkedList ***

AFXINL afxLink* AfxGetFirstLink(afxList const *lst);

AFXINL afxLink* AfxGetLastLink(afxList const *lst);

AFXINL afxBool AfxIsListEmpty(afxList const *lst);

AFXINL void AfxDeployList(afxList *lst);

#define         AfxForEachListLink(lstA_, elemA_)  for (afxLink *(nextElemA_) = (afxLink*)NIL, *(elemA_) = AfxGetFirstLink((lstA_)); (nextElemA_) = AfxGetNextLink((elemA_)), (elemA_) != AfxGetListHead((lstA_)); (elemA_) = (nextElemA_))
#define         AfxForEachListLinkB(lstB_, elemB_) for (afxLink *(nextElemB_) = (afxLink*)NIL, *(elemB_) = AfxGetFirstLink((lstB_)); (nextElemB_) = AfxGetNextLink((elemB_)), (elemB_) != AfxGetListHead((lstB_)); (elemB_) = (nextElemB_))
#define         AfxForEachListLinkC(lstC_, elemC_) for (afxLink *(nextElemC_) = (afxLink*)NIL, *(elemC_) = AfxGetFirstLink((lstC_)); (nextElemC_) = AfxGetNextLink((elemC_)), (elemC_) != AfxGetListHead((lstC_)); (elemC_) = (nextElemC_))
#define         AfxForEachListLinkD(lstD_, elemD_) for (afxLink *(nextElemD_) = (afxLink*)NIL, *(elemD_) = AfxGetFirstLink((lstD_)); (nextElemD_) = AfxGetNextLink((elemD_)), (elemD_) != AfxGetListHead((lstD_)); (elemD_) = (nextElemD_))

#define         AfxForEachConstListLink(lstA_, elemA_)  for (afxLink *(nextElemA_) = (afxLink*)NIL, *(elemA_) = AfxGetFirstLink((lstA_)); (nextElemA_) = AfxGetNextLink((elemA_)), (elemA_) != AfxGetConstListHead((lstA_)); (elemA_) = (nextElemA_))

#define         AfxForEachListLink_B2F(lstA_, elemA_)  for (afxLink *(prevElemA_) = (afxLink*)NIL, *(elemA_) = AfxGetLastLink((lstA_)); (prevElemA_) = AfxGetPrevLink((elemA_)), (elemA_) != AfxGetListHead((lstA_)); (elemA_) = (prevElemA_))
#define         AfxForEachListLinkB_B2F(lstB_, elemB_)  for (afxLink *(prevElemB_) = (afxLink*)NIL, *(elemB_) = AfxGetLastLink((lstB_)); (prevElemB_) = AfxGetPrevLink((elemB_)), (elemB_) != AfxGetListHead((lstB_)); (elemB_) = (prevElemB_))

// ATENÇÃO! Observe se não há uma ponto-vírgula no final desses macros.

#define         AfxForEachListElement(list_, type_, offset_, elem_) for (afxLink *_next##elem_ = (afxLink*)NIL, *_curr##elem_ = AfxGetFirstLink(list_); _next##elem_ = AfxGetNextLink(_curr##elem_), elem_ = (type_*)AFX_REBASE(_curr##elem_, type_, offset_), (_curr##elem_) != AfxGetListHead(list_); _curr##elem_ = _next##elem_)
#define         AfxForEachListElement_B(listB_, typeB_, offsetB_, elemB_) for (afxLink *_next##elemB_ = (afxLink*)NIL, *_curr##elemB_ = AfxGetFirstLink(listB_); _next##elemB_ = AfxGetNextLink(_curr##elemB_), elemB_ = (typeB_*)AFX_REBASE(_curr##elemB_, typeB_, offsetB_), (_curr##elemB_) != AfxGetListHead(listB_); _curr##elemB_ = _next##elemB_)
#define         AfxForEachListElement_C(listC_, typeC_, offsetC_, elemC_) for (afxLink *_next##elemC_ = (afxLink*)NIL, *_curr##elemC_ = AfxGetFirstLink(listC_); _next##elemC_ = AfxGetNextLink(_curr##elemC_), elemC_ = (typeC_*)AFX_REBASE(_curr##elemC_, typeC_, offsetC_), (_curr##elemC_) != AfxGetListHead(listC_); _curr##elemC_ = _next##elemC_)
#define         AfxForEachListElementB2F(list__, type__, offset__, elem__) for (afxLink *_prev##elem__ = (afxLink*)NIL, *_curr##elem__ = AfxGetLastLink(list__); _prev##elem__ = AfxGetPrevLink(_curr##elem__), elem__ = (type__*)AFX_REBASE(_curr##elem__, type__, offset__), (_curr##elem__) != AfxGetListHead(list__); _curr##elem__ = _prev##elem__)
#define         AfxForEachListElementB2F_B(listB__, typeB__, offsetB__, elemB__) for (afxLink *_prev##elemB__ = (afxLink*)NIL, *_curr##elemB__ = AfxGetLastLink(listB__); _prev##elemB__ = AfxGetPrevLink(_curr##elemB__), elemB__ = (typeB__*)AFX_REBASE(_curr##elemB__, typeB__, offsetB__), (_curr##elemB__) != AfxGetListHead(listB__); _curr##elemB__ = _prev##elemB__)

AFXINL afxLink* AfxGetFirstListElement_(afxList const *lst);

AFXINL afxLink* AfxGetLastListElement_(afxList const *lst);

#define AfxCastFirstListElement(lst_,typ_,off_) AFX_REBASE(AfxGetFirstListElement_(lst_),typ_,off_)
#define AfxCastLastListElement(lst_,typ_,off_) AFX_REBASE(AfxGetLastListElement_(lst_),typ_,off_)

#endif//AFX_CHAIN_H
