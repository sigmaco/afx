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

#ifndef AFX_LINKED_LIST_H
#define AFX_LINKED_LIST_H

#include "afx/core/diag/afxDebug.h"

// std::list is a container that supports constant time insertion and removal of elements from anywhere in the container.
// Fast random access is not supported. It is usually implemented as a doubly-linked list.
// Compared to std::forward_list this container provides bidirectional iteration capability while being less space efficient.

// Adding, removing and moving the elements within the list or across several lists does not invalidate the iterators or references.
// An iterator is invalidated only when the corresponding element is deleted.

// std::list meets the requirements of Container, MemoryAwareContainer, SequenceContainer and ReversibleContainer.

typedef struct afxChain afxChain;
typedef struct afxLinkage afxLinkage;

struct afxLinkage
{
    afxLinkage  *next, *prev;
    afxChain    *chain;
};

struct afxChain
{
    afxLinkage      anchor;
    union
    {
        void        *owner;
        afxFcc      *ownerFcc;
    };
    afxNat          cnt;
};

// GRANT CHANGE MONITORING

AFXINL void                 AfxChainDeploy(afxChain *ch, void *owner);

AFXINL afxNat               AfxChainGetLength(afxChain const *ch);
AFXINL void*                AfxChainGetOwner(afxChain const *ch);

AFXINL afxLinkage*          AfxChainGetBegin(afxChain const *ch);
AFXINL afxLinkage*          AfxChainGetEnd(afxChain const *ch);
AFXINL afxLinkage*          AfxChainGetAnchor(afxChain *ch);
AFXINL afxLinkage const*    AfxChainGetAnchorConst(afxChain const *ch);

AFXINL afxNat               AfxChainFindLinkage(afxChain const *ch, afxLinkage *lnk);
AFXINL afxLinkage*          AfxChainGetLinkage(afxChain *ch, afxNat idx);
AFXINL afxError             AfxChainPushLinkageAfter(afxChain *ch, afxNat idx);
AFXINL afxError             AfxChainPopLinkage(afxChain *ch, afxNat idx);


AFXINL afxResult            AfxChainEnumerateLinkages(afxChain *ch, afxBool reverse, afxNat base, afxNat cnt, afxLinkage *lnk[]);



AFXINL void AfxLinkageDeploy(afxLinkage *lnk, afxChain *ch);

AFXINL void AfxLinkageDrop(afxLinkage *lnk);

AFXINL void* AfxLinkageGetOwner(afxLinkage const *lnk);

AFXINL afxLinkage* AfxLinkageGetNext(afxLinkage const *lnk);

AFXINL afxLinkage* AfxLinkageGetPrev(afxLinkage const *lnk);

#define AfxChainForEveryLinkage2(ch_, type_, offset_, lnk_) for (afxLinkage *_next##lnk2_ = (afxLinkage*)NIL, *_curr##lnk2_ = (ch_)->anchor.next; _next##lnk2_ = (_curr##lnk2_)->next, lnk_ = (type_*)AfxContainerOf(_curr##lnk2_, type_, offset_), (_curr##lnk2_) != &(ch_)->anchor; _curr##lnk2_ = _next##lnk2_)
#define AfxChainForEveryLinkage(ch_, type_, offset_, lnk_) for (afxLinkage *_next##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.next; _next##lnk_ = (_curr##lnk_)->next, lnk_ = (type_*)AfxContainerOf(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _next##lnk_)
#define AfxChainForEveryLinkageB2F(ch_, type_, offset_, lnk_) for (afxLinkage *_last##lnk_ = (afxLinkage*)NIL, *_curr##lnk_ = (ch_)->anchor.prev; _last##lnk_ = (_curr##lnk_)->prev, lnk_ = (type_*)AfxContainerOf(_curr##lnk_, type_, offset_), (_curr##lnk_) != &(ch_)->anchor; _curr##lnk_ = _last##lnk_)

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

#define         AfxForEachListElement(list_, type_, offset_, elem_) for (afxLink *_next##elem_ = (afxLink*)NIL, *_curr##elem_ = AfxGetFirstLink(list_); _next##elem_ = AfxGetNextLink(_curr##elem_), elem_ = (type_*)AfxContainerOf(_curr##elem_, type_, offset_), (_curr##elem_) != AfxGetListHead(list_); _curr##elem_ = _next##elem_)
#define         AfxForEachListElement_B(listB_, typeB_, offsetB_, elemB_) for (afxLink *_next##elemB_ = (afxLink*)NIL, *_curr##elemB_ = AfxGetFirstLink(listB_); _next##elemB_ = AfxGetNextLink(_curr##elemB_), elemB_ = (typeB_*)AfxContainerOf(_curr##elemB_, typeB_, offsetB_), (_curr##elemB_) != AfxGetListHead(listB_); _curr##elemB_ = _next##elemB_)
#define         AfxForEachListElement_C(listC_, typeC_, offsetC_, elemC_) for (afxLink *_next##elemC_ = (afxLink*)NIL, *_curr##elemC_ = AfxGetFirstLink(listC_); _next##elemC_ = AfxGetNextLink(_curr##elemC_), elemC_ = (typeC_*)AfxContainerOf(_curr##elemC_, typeC_, offsetC_), (_curr##elemC_) != AfxGetListHead(listC_); _curr##elemC_ = _next##elemC_)
#define         AfxForEachListElementB2F(list__, type__, offset__, elem__) for (afxLink *_prev##elem__ = (afxLink*)NIL, *_curr##elem__ = AfxGetLastLink(list__); _prev##elem__ = AfxGetPrevLink(_curr##elem__), elem__ = (type__*)AfxContainerOf(_curr##elem__, type__, offset__), (_curr##elem__) != AfxGetListHead(list__); _curr##elem__ = _prev##elem__)
#define         AfxForEachListElementB2F_B(listB__, typeB__, offsetB__, elemB__) for (afxLink *_prev##elemB__ = (afxLink*)NIL, *_curr##elemB__ = AfxGetLastLink(listB__); _prev##elemB__ = AfxGetPrevLink(_curr##elemB__), elemB__ = (typeB__*)AfxContainerOf(_curr##elemB__, typeB__, offsetB__), (_curr##elemB__) != AfxGetListHead(listB__); _curr##elemB__ = _prev##elemB__)

AFXINL afxLink* AfxGetFirstListElement_(afxList const *lst);

AFXINL afxLink* AfxGetLastListElement_(afxList const *lst);

#define AfxCastFirstListElement(lst_,typ_,off_) AfxContainerOf(AfxGetFirstListElement_(lst_),typ_,off_)
#define AfxCastLastListElement(lst_,typ_,off_) AfxContainerOf(AfxGetLastListElement_(lst_),typ_,off_)

#endif//AFX_LINKED_LIST_H