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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/core/afxSystem.h"


_AFXINL void AfxSetUpChain(afxChain *ch, void *holder)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    ch->holder = holder;
    ch->cnt = 0;
    afxLinkage* anchor = &(ch->anchor);
    anchor->next = (anchor->prev = anchor);
    anchor->chain = ch;
}

_AFXINL void AfxSwapChains(afxChain *ch, afxChain *other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    void *holder = ch->holder;
    ch->holder = other->holder;
    other->holder = holder;
    afxNat cnt = ch->cnt;
    ch->cnt = other->cnt;
    other->cnt = ch->cnt;

    afxLinkage *a = AfxGetLastLinkage(ch);
    afxLinkage *b = AfxGetLastLinkage(other);
    
    if (a)
        AfxPushLinkage(a, other);

    if (b)
        AfxPushLinkage(b, ch);
}

_AFXINL afxNat AfxGetChainLength(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    return ch->cnt;
}

_AFXINL void* AfxGetChainHolder(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    return ch->holder;
}

_AFXINL afxLinkage* AfxGetLastLinkage(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    return ch->anchor.next != &ch->anchor ? ch->anchor.next : NIL;
}

_AFXINL afxLinkage* AfxGetFirstLinkage(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    return ch->anchor.prev != &ch->anchor ? ch->anchor.prev : NIL;
}

_AFXINL afxLinkage* AfxGetAnchor(afxChain *ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    return &ch->anchor;
}

_AFXINL afxLinkage const* AfxGetAnchorConst(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    return &ch->anchor;
}

_AFXINL afxBool AfxChainIsEmpty(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    return (ch->anchor.next == &(ch->anchor));
}

_AFXINL void AfxResetLinkage(afxLinkage *lnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    lnk->next = lnk;
    lnk->prev = lnk;
    lnk->chain = NIL;
}

_AFXINL afxNat AfxPushLinkage(afxLinkage *lnk, afxChain *ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    afxNat lnkIdx;

    if (!(lnk->chain = ch))
    {
        lnk->next = (lnk->prev = lnk);
        lnkIdx = AFX_INVALID_INDEX;
    }
    else
    {
        afxInt cnt = ++ch->cnt;
        ch->cnt = 0;
        afxLinkage *anchor = &(ch)->anchor;
        lnk->next = (anchor)->next;
        lnk->prev = anchor;
        (anchor)->next->prev = lnk;
        anchor->next = lnk;
        lnkIdx = (ch->cnt = cnt);
    }
    return lnkIdx;
}

_AFXINL afxNat AfxPushBackLinkage(afxLinkage *lnk, afxChain *ch)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    afxNat lnkIdx;

    if (!(lnk->chain = ch))
    {
        lnk->prev = (lnk->next = lnk);
        lnkIdx = AFX_INVALID_INDEX;
    }
    else
    {
        afxInt cnt = ++ch->cnt;
        ch->cnt = 0;
        afxLinkage *anchor = &(ch)->anchor;
        lnk->prev = (anchor)->prev;
        lnk->next = anchor;
        (anchor)->prev->next = lnk;
        anchor->prev = lnk;
        lnkIdx = (ch->cnt = cnt);
    }
    return lnkIdx;
}

_AFXINL void AfxPopLinkage(afxLinkage *lnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);

    afxChain* ch = lnk->chain;
    afxInt cnt = ch ? cnt = --ch->cnt, ch->cnt = 0 : 0;

    afxLinkage* prev = (lnk)->prev;
    afxLinkage* next = (lnk)->next;
    prev->next = next;
    next->prev = prev;
    lnk->next = (lnk->prev = lnk);

    if (ch)
    {
        ch->cnt = cnt;
        lnk->chain = NIL;
    }
}

_AFXINL afxLinkage* AfxFindLastLinkage(afxChain const *ch, afxNat idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    //AfxAssert(idx < ch->cnt);

    afxLinkage const *lnk = &ch->anchor;
    lnk = lnk->next;

    for (afxInt i = 0; i < ch->cnt; i++)
    {
        AfxAssert(lnk);

        if (idx == (afxNat)i)
            return (void*)lnk;

        lnk = lnk->next;
    }
    return (void*)lnk;
}

_AFXINL afxLinkage* AfxFindFirstLinkage(afxChain const *ch, afxNat idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    //AfxAssert(idx < ch->cnt);

    afxLinkage const *lnk = &ch->anchor;
    lnk = lnk->prev;

    for (afxInt i = 0; i < ch->cnt; i++)
    {
        AfxAssert(lnk);

        if (idx == (afxNat)i)
            return (void*)lnk;

        lnk = lnk->prev;
    }
    return (void*)lnk;
}

_AFXINL afxBool AfxFindChainLinkageIndex(afxChain const *ch, afxLinkage *lnk, afxNat *idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);

    afxLinkage const *lnk2 = &ch->anchor;

    for (afxInt i = 0; i < ch->cnt; i++)
    {
        lnk2 = lnk2->next;
        AfxAssert(lnk2);

        if (lnk == lnk2)
        {
            AfxAssert(idx);
            *idx = i;
            return TRUE;
        }
    }
    return FALSE;
}

_AFXINL afxBool AfxFindChainLinkageIndexB2F(afxChain const *ch, afxLinkage *lnk, afxNat *idx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);

    afxLinkage const *lnk2 = &ch->anchor;

    for (afxInt i = 0; i < ch->cnt; i++)
    {
        lnk2 = lnk2->prev;
        AfxAssert(lnk2);

        if (lnk == lnk2)
        {
            AfxAssert(idx);
            *idx = i;
            return TRUE;
        }
    }
    return FALSE;
}

_AFXINL void* AfxGetLinker(afxLinkage const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    return lnk->chain ? lnk->chain->holder : NIL;
}

_AFXINL afxChain* AfxGetChain(afxLinkage const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    return lnk->chain;
}

_AFXINL afxLinkage* AfxGetNextLinkage(afxLinkage const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    afxLinkage* n = lnk->next;
    return lnk->chain && &lnk->chain->anchor != n ? n : NIL;
}

_AFXINL afxLinkage* AfxGetPrevLinkage(afxLinkage const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    afxLinkage* p = lnk->prev;
    return lnk->chain && &lnk->chain->anchor != p ? p : NIL;
}

_AFXINL afxNat AfxInvokeLinkages(afxChain *ch, afxBool fromLast, afxNat first, afxNat cnt, afxBool(*f)(afxLinkage *lnk, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    afxNat rslt = 0;

    afxLinkage *lnk = &ch->anchor;

    for (afxInt i = 0; i < ch->cnt; i++)
    {
        lnk = fromLast ? lnk->next : lnk->prev;
        
        if (lnk == &ch->anchor)
            break;

        if (i >= (afxInt)first)
        {
            ++rslt;

            if (f(lnk, udd))
                break;

            if (rslt == cnt)
                break;
        }
    }

    return rslt;
}

_AFXINL afxResult AfxChainEnumerateLinkages(afxChain *ch, afxBool reverse, afxNat base, afxNat cnt, afxLinkage *lnk[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(ch);
    AfxAssert(cnt);
    AfxAssert(lnk);
    afxResult rslt = 0;

    afxNat posn = 0;
    afxLinkage *lnk2;

    if (reverse)  // from front (last in) to back (first in) of chain
    {
        AfxChainForEveryLinkage(ch, afxLinkage, next, lnk2)
        {
            if (posn >= base)
            {
                lnk[rslt] = lnk2;
                ++rslt;
            }

            ++posn;

            if (posn >= base + cnt)
                break;
        }
    }
    else  // from back (first in) to front (last in) of chain
    {
        AfxChainForEveryLinkageB2F(ch, afxLinkage, next, lnk2)
        {
            if (posn >= base)
            {
                lnk[rslt] = lnk2;
                ++rslt;
            }

            ++posn;

            if (posn >= base + cnt)
                break;
        }
    }

    return rslt;
}


_AFXINL afxLink* AfxGetListHead(afxList *lst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lst);
    return &(lst->head);
}

_AFXINL afxLink const* AfxGetConstListHead(afxList const *lst) { afxError err = AFX_ERR_NONE; AfxAssert(lst); return (&(lst->head)); }

_AFXINL afxLink* AfxGetNextLink(afxLink const *lnk) { afxError err = AFX_ERR_NONE; AfxAssert(lnk); return lnk->next; }

_AFXINL afxLink* AfxGetPrevLink(afxLink const *lnk) { afxError err = AFX_ERR_NONE; AfxAssert(lnk); return lnk->prev; }

_AFXINL void AfxDeployLink(afxLink *lnk) { afxError err = AFX_ERR_NONE; AfxAssert(lnk); lnk->next = (afxLink*)NIL; lnk->prev = (afxLink*)NIL; }

_AFXINL afxBool AfxIsLinkUnused(afxLink const *lnk) { afxError err = AFX_ERR_NONE; AfxAssert(lnk); afxBool b = (NIL == AfxGetNextLink(lnk)); if (b && !(b = (NIL == AfxGetPrevLink(lnk)))) AfxLogError("Object chain broken."); return b; }


_AFXINL void AfxPrependLink(afxLink *lnk, afxList *lst)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    AfxAssert(lst);
    afxLink *head = AfxGetListHead(lst);
    lnk->next = AfxGetNextLink(head);
    lnk->prev = head;
    AfxGetNextLink(head)->prev = lnk;
    head->next = lnk;
}

_AFXINL void AfxDropLink(afxLink *lnk)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    afxLink* prev = AfxGetPrevLink(lnk), *next = AfxGetNextLink(lnk);
    prev->next = next;
    next->prev = prev;
    lnk->next = (lnk->prev = NIL);
}

_AFXINL void AfxRelink(afxLink *lnk, afxLink *to)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    AfxAssert(to);
    afxLink* prev = AfxGetPrevLink(lnk), *next = AfxGetNextLink(lnk);
    prev->next = to;
    to->prev = prev;
    next->prev = to;
    to->next = next;
    lnk->next = NIL;
    lnk->prev = NIL;
}

_AFXINL void AfxSwapLinks(afxLink *lnk, afxLink *other)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(lnk);
    AfxAssert(other);

    afxLink* prevA = AfxGetPrevLink(lnk), *nextA = AfxGetNextLink(lnk);
    afxLink* prevB = AfxGetPrevLink(other), *nextB = AfxGetNextLink(other);

    prevA->next = other;
    other->prev = prevA;
    nextA->prev = other;
    other->next = nextA;

    prevB->next = lnk;
    lnk->prev = prevB;
    nextB->prev = lnk;
    lnk->next = nextB;
}

// *** afxLinkedList ***

_AFXINL afxLink* AfxGetFirstLink(afxList const *lst) { afxError err = AFX_ERR_NONE; AfxAssert(lst); return AfxGetNextLink(AfxGetConstListHead(lst)); }

_AFXINL afxLink* AfxGetLastLink(afxList const *lst) { afxError err = AFX_ERR_NONE; AfxAssert(lst); return AfxGetPrevLink(AfxGetConstListHead(lst)); }

_AFXINL afxBool AfxIsListEmpty(afxList const *lst) { afxError err = AFX_ERR_NONE; AfxAssert(lst); return (AfxGetFirstLink(lst) == AfxGetConstListHead(lst)); }

_AFXINL void AfxDeployList(afxList *lst) { afxError err = AFX_ERR_NONE; AfxAssert(lst); afxLink* head = AfxGetListHead(lst); head->next = (head->prev = head); }

_AFXINL afxLink* AfxGetFirstListElement_(afxList const *lst) { afxError err = AFX_ERR_NONE; AfxAssert(lst); return AfxIsListEmpty(lst) ? NIL : AfxGetFirstLink(lst); }

_AFXINL afxLink* AfxGetLastListElement_(afxList const *lst) { afxError err = AFX_ERR_NONE; AfxAssert(lst); return AfxIsListEmpty(lst) ? NIL : AfxGetLastLink(lst); }
