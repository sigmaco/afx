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
    afxLinkage* anchor = &(ch->anchor);
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
    afxUnit cnt = ch->cnt;
    ch->cnt = other->cnt;
    other->cnt = ch->cnt;

    afxLinkage *a = AfxGetLastLinkage(ch);
    afxLinkage *b = AfxGetLastLinkage(other);
    
    if (a)
        AfxPushLinkage(a, other);

    if (b)
        AfxPushLinkage(b, ch);
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

_AFXINL afxLinkage* AfxGetLastLinkage(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return ch->anchor.next != &ch->anchor ? ch->anchor.next : NIL;
}

_AFXINL afxLinkage* AfxGetFirstLinkage(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return ch->anchor.prev != &ch->anchor ? ch->anchor.prev : NIL;
}

_AFXINL afxLinkage* AfxGetAnchor(afxChain *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return &ch->anchor;
}

_AFXINL afxLinkage const* AfxGetAnchorConst(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return &ch->anchor;
}

_AFXINL afxBool AfxChainIsEmpty(afxChain const *ch)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    return (ch->anchor.next == &(ch->anchor));
}

_AFXINL void AfxResetLinkage(afxLinkage *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    lnk->next = lnk;
    lnk->prev = lnk;
    lnk->chain = NIL;
}

_AFXINL afxUnit AfxLinkAhead(afxLinkage* lnk, afxLinkage* prev)
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

_AFXINL afxUnit AfxLinkBehind(afxLinkage* lnk, afxLinkage* next)
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

_AFXINL afxUnit AfxPushLinkage(afxLinkage *lnk, afxChain *ch)
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
        afxLinkage *anchor = &ch->anchor;
        lnk->next = anchor->next;
        lnk->prev = anchor;
        anchor->next->prev = lnk;
        anchor->next = lnk;
        lnkIdx = ch->cnt++;
    }
    return lnkIdx;
}

_AFXINL afxUnit AfxPushBackLinkage(afxLinkage *lnk, afxChain *ch)
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
        afxLinkage *anchor = &ch->anchor;
        lnk->prev = anchor->prev;
        lnk->next = anchor;
        anchor->prev->next = lnk;
        anchor->prev = lnk;
        lnkIdx = ch->cnt++;
    }
    return lnkIdx;
}

_AFXINL void AfxPopLinkage(afxLinkage *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    afxChain* ch = lnk->chain;

    if (lnk->chain)
    {
        --lnk->chain->cnt;
        lnk->chain = NIL;
    }
    afxLinkage* prev = lnk->prev;
    afxLinkage* next = lnk->next;
    prev->next = next;
    next->prev = prev;
    lnk->next = (lnk->prev = lnk);
}

_AFXINL afxLinkage* AfxFindLastLinkage(afxChain const *ch, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    //AFX_ASSERT(idx < ch->cnt);

    afxLinkage const *lnk = &ch->anchor;
    lnk = lnk->next;

    for (afxInt i = 0; i < ch->cnt; i++)
    {
        AFX_ASSERT(lnk);

        if (idx == (afxUnit)i)
            return (void*)lnk;

        lnk = lnk->next;
    }
    return (void*)lnk;
}

_AFXINL afxLinkage* AfxFindFirstLinkage(afxChain const *ch, afxUnit idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    //AFX_ASSERT(idx < ch->cnt);

    afxLinkage const *lnk = &ch->anchor;
    lnk = lnk->prev;

    for (afxInt i = 0; i < ch->cnt; i++)
    {
        AFX_ASSERT(lnk);

        if (idx == (afxUnit)i)
            return (void*)lnk;

        lnk = lnk->prev;
    }
    return (void*)lnk;
}

_AFXINL afxBool AfxFindChainLinkageIndex(afxChain const *ch, afxLinkage *lnk, afxUnit *idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);

    afxLinkage const *lnk2 = &ch->anchor;

    for (afxInt i = 0; i < ch->cnt; i++)
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

_AFXINL afxBool AfxFindChainLinkageIndexB2F(afxChain const *ch, afxLinkage *lnk, afxUnit *idx)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);

    afxLinkage const *lnk2 = &ch->anchor;

    for (afxInt i = 0; i < ch->cnt; i++)
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

_AFXINL void* AfxGetLinker(afxLinkage const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    return lnk->chain ? lnk->chain->holder : NIL;
}

_AFXINL afxChain* AfxGetChain(afxLinkage const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    return lnk->chain;
}

_AFXINL afxLinkage* AfxGetNextLinkage(afxLinkage const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    afxLinkage* n = lnk->next;
    return lnk->chain && &lnk->chain->anchor != n ? n : NIL;
}

_AFXINL afxLinkage* AfxGetPrevLinkage(afxLinkage const *lnk)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(lnk);
    afxLinkage* p = lnk->prev;
    return lnk->chain && &lnk->chain->anchor != p ? p : NIL;
}

_AFXINL afxUnit AfxInvokeLinkages(afxChain *ch, afxBool fromLast, afxUnit first, afxUnit cnt, afxBool(*f)(afxLinkage *lnk, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    afxUnit rslt = 0;

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

_AFXINL afxResult AfxChainEnumerateLinkages(afxChain *ch, afxBool reverse, afxUnit base, afxUnit cnt, afxLinkage *lnk[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT(ch);
    AFX_ASSERT(cnt);
    AFX_ASSERT(lnk);
    afxResult rslt = 0;

    afxUnit posn = 0;
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

