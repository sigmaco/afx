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

#include "../dev/afxExecImplKit.h"


_AFXINL void AfxDeployChain(afxChain *ch, void *holder)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    ch->holder = holder;
    ch->cnt = 0;
    afxLink* anchor = &(ch->anchor);
    anchor->next = anchor;
    anchor->prev = anchor;
    anchor->chain = ch;
}

_AFXINL void AfxSwapChains(afxChain *ch, afxChain *other)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    void *holder = ch->holder;
    ch->holder = other->holder;
    other->holder = holder;
    ch->cnt = other->cnt;
    other->cnt = ch->cnt;

    afxLink *a = AfxGetLastLink(ch);
    afxLink *b = AfxGetLastLink(other);
    
    if (a)
        AfxPushLink(a, other);

    if (b)
        AfxPushLink(b, ch);
}

_AFXINL afxUnit AfxGetChainLength(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return ch->cnt;
}

_AFXINL void* AfxGetChainHolder(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return ch->holder;
}

_AFXINL afxLink* AfxGetLastLink(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return ch->anchor.next != &ch->anchor ? ch->anchor.next : NIL;
}

_AFXINL afxLink* AfxGetFirstLink(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return ch->anchor.prev != &ch->anchor ? ch->anchor.prev : NIL;
}

_AFXINL afxLink* AfxGetAnchor(afxChain *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return &ch->anchor;
}

_AFXINL afxLink const* AfxGetAnchorConst(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return &ch->anchor;
}

_AFXINL afxBool AfxIsChainEmpty(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return (ch->anchor.next == &(ch->anchor));
}

_AFXINL void AfxResetLink(afxLink *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    lnk->next = lnk;
    lnk->prev = lnk;
    lnk->chain = NIL;
}

_AFXINL afxUnit AfxLinkAhead(afxLink* lnk, afxLink* prev)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    afxUnit lnkIdx;

    if (!prev)
    {
        lnk->next = lnk;
        lnk->prev = lnk;
        lnk->chain = NIL;
        lnkIdx = AFX_INVALID_INDEX;
    }
    else
    {
        lnk->next = prev->next;
        lnk->prev = prev;
        prev->next->prev = lnk;
        prev->next = lnk;
        lnk->chain = prev->chain;

        lnkIdx = (lnk->chain) ? lnk->chain->cnt++ : AFX_INVALID_INDEX;
    }
    return lnkIdx;
}

_AFXINL afxUnit AfxLinkBehind(afxLink* lnk, afxLink* next)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    afxUnit lnkIdx;

    if (!next)
    {
        lnk->next = lnk;
        lnk->prev = lnk;
        lnk->chain = NIL;
        lnkIdx = AFX_INVALID_INDEX;
    }
    else
    {
        lnk->prev = next->prev;
        lnk->next = next;
        next->prev->next = lnk;
        next->prev = lnk;
        lnk->chain = next->chain;

        lnkIdx = (lnk->chain) ? lnk->chain->cnt++ : AFX_INVALID_INDEX;
    }
    return lnkIdx;
}

_AFXINL afxUnit AfxPushLink(afxLink *lnk, afxChain *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    afxUnit lnkIdx;

    if (!(lnk->chain = ch))
    {
        lnk->next = lnk;
        lnk->prev = lnk;
        lnkIdx = AFX_INVALID_INDEX;
    }
    else
    {
        afxLink *anchor = &ch->anchor;
        lnk->next = anchor->next;
        lnk->prev = anchor;
        anchor->next->prev = lnk;
        anchor->next = lnk;
        lnkIdx = ch->cnt++;
    }
    return lnkIdx;
}

_AFXINL afxUnit AfxPushBackLink(afxLink *lnk, afxChain *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    afxUnit lnkIdx;

    if (!(lnk->chain = ch))
    {
        lnk->prev = (lnk->next = lnk);
        lnkIdx = AFX_INVALID_INDEX;
    }
    else
    {
        afxLink *anchor = &ch->anchor;
        lnk->prev = anchor->prev;
        lnk->next = anchor;
        anchor->prev->next = lnk;
        anchor->prev = lnk;
        lnkIdx = ch->cnt++;
    }
    return lnkIdx;
}

_AFXINL void AfxPopLink(afxLink *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);

    if (lnk->chain)
    {
        --lnk->chain->cnt;
        lnk->chain = NIL;
    }
    afxLink* prev = lnk->prev;
    afxLink* next = lnk->next;

    if (!prev || !next)
    {
        int a = 1;
    }

    prev->next = next;
    next->prev = prev;
    lnk->next = (lnk->prev = lnk);
}

_AFXINL afxLink* AfxFindLastLink(afxChain const *ch, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    //AFX_ASSERT(idx < ch->cnt);

    afxLink const *lnk = &ch->anchor;

    for (afxUnit i = 0; i < ch->cnt; i++)
    {
        lnk = lnk->next;
        AFX_ASSERT(lnk);

        if (idx == (afxUnit)i)
            return (void*)lnk;

        lnk = lnk->next;
    }
    return NIL;
}

_AFXINL afxLink* AfxFindFirstLink(afxChain const *ch, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    //AFX_ASSERT(idx < ch->cnt);

    afxLink const *lnk = &ch->anchor;

    for (afxUnit i = 0; i < ch->cnt; i++)
    {
        lnk = lnk->prev;
        AFX_ASSERT(lnk);

        if (idx == (afxUnit)i)
            return (void*)lnk;

        lnk = lnk->prev;
    }
    return NIL;
}

_AFXINL afxBool AfxFindLinkIndex(afxChain const *ch, afxLink *lnk, afxUnit *idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);

    afxLink const *lnk2 = &ch->anchor;

    for (afxUnit i = 0; i < ch->cnt; i++)
    {
        lnk2 = lnk2->next;
        AFX_ASSERT(lnk2);

        if (lnk == lnk2)
        {
            AFX_ASSERT(idx);
            *idx = i;
            return TRUE;
        }
    }
    return FALSE;
}

_AFXINL afxBool AfxFindLinkIndexB2F(afxChain const *ch, afxLink *lnk, afxUnit *idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);

    afxLink const *lnk2 = &ch->anchor;

    for (afxUnit i = 0; i < ch->cnt; i++)
    {
        lnk2 = lnk2->prev;
        AFX_ASSERT(lnk2);

        if (lnk == lnk2)
        {
            AFX_ASSERT(idx);
            *idx = i;
            return TRUE;
        }
    }
    return FALSE;
}

_AFXINL void* AfxGetLinker(afxLink const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    return lnk->chain ? lnk->chain->holder : NIL;
}

_AFXINL afxChain* AfxGetChain(afxLink const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    return lnk->chain;
}

_AFXINL afxLink* AfxGetNextLink(afxLink const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    afxLink* n = lnk->next;
    return lnk->chain && &lnk->chain->anchor != n ? n : NIL;
}

_AFXINL afxLink* AfxGetPrevLink(afxLink const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    afxLink* p = lnk->prev;
    return lnk->chain && &lnk->chain->anchor != p ? p : NIL;
}

_AFXINL afxUnit AfxInvokeLinkages(afxChain *ch, afxBool fromLast, afxUnit first, afxUnit cnt, afxBool(*f)(afxLink *lnk, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    afxUnit rslt = 0;

    afxLink *lnk = &ch->anchor;

    for (afxUnit i = 0; i < ch->cnt; i++)
    {
        lnk = fromLast ? lnk->next : lnk->prev;
        
        if (lnk == &ch->anchor)
            break;

        if (i >= (afxUnit)first)
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

_AFXINL afxResult AfxChainEnumerateLinkages(afxChain *ch, afxBool reverse, afxUnit base, afxUnit cnt, afxLink *lnk[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    AFX_ASSERT(cnt);
    AFX_ASSERT(lnk);
    afxResult rslt = 0;

    afxUnit posn = 0;
    afxLink *lnk2;

    if (reverse)  // from front (last in) to back (first in) of chain
    {
        AfxChainForEveryLinkage(ch, afxLink, next, lnk2)
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
        AfxChainForEveryLinkageB2F(ch, afxLink, next, lnk2)
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

