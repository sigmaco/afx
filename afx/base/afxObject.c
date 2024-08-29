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

//#define _AFX_SYSTEM_C
#include "../dev/afxDevCoreBase.h"

// SLOTS incorporados diretamente no espaço de array estavam crashando e gerando anomalias de corrupção de leitura/escrita e alocação.
// Quando optado por operar por slots alocados individualmente e arranjados como pointeiros, o problema foi resolvido.
// O problema ocorria após a necessidade de mover os elementos. Aparentemente a mudança com função especial de cópia não funcionou.

// AUXILIAR FUNCTIONS /////////////////////////////////////////////////////////

_AFXINL afxResult _AfxClsTestObjFccRecursive(afxClass const *cls, afxFcc fcc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(cls, afxFcc_CLS);

    if (cls->objFcc == fcc) return 1;
    else if (cls->subset.chain)
        return _AfxClsTestObjFccRecursive(AfxGetLinker(&cls->subset), fcc);

    return 0;
}

_AFXINL afxResult AfxTestObjectFcc(afxObject obj, afxFcc fcc)
{
    afxError err = AFX_ERR_NONE;
    //AfxAssertType(obj, afxFcc_OBJ);
    AfxAssert(fcc);
    afxClass const* cls = AfxGetClass(obj);

    if (!cls) return 1;
    else
    {
        AfxAssertType(cls, afxFcc_CLS);

        if (cls->objFcc == fcc) return 1;
        else if (cls->subset.chain)
            return _AfxClsTestObjFccRecursive(AfxGetLinker(&cls->subset), fcc);

    }
    return 0;
}

_AFXINL afxFcc AfxGetObjectFcc(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    afxClass const* cls = AfxGetClass(obj);
    AfxAssertType(cls, afxFcc_CLS);
    return cls->objFcc;
}

_AFXINL afxChar const* AfxGetObjectFccAsString(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    afxClass const* cls = AfxGetClass(obj);
    AfxAssertType(cls, afxFcc_CLS);
    return (afxChar const *)&cls->objFcc;
}

_AFXINL afxInt32 AfxGetRefCount(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    return AfxLoadAtom32(&hdr->refCnt);
}

_AFXINL afxNat32 AfxGetObjectTid(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase const* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    return hdr->tid;
}

_AFXINL afxClass* AfxGetClass(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase const* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    afxClass *cls = hdr->cls;
    AfxAssertType(cls, afxFcc_CLS);
    return cls;
}

_AFXINL afxBool AfxDoesObjectInherit(afxObject obj, afxClass const* cls)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase const* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    AfxAssertType(cls, afxFcc_CLS);

    afxClass const* objCls = hdr->cls;
    AfxAssertType(objCls, afxFcc_CLS);

    while (1)
    {
        if (cls)
        {
            AfxAssertType(cls, afxFcc_CLS);

            if (objCls == cls)
                return TRUE;

            cls = AfxGetSubClass(cls);
        }
    }
    return FALSE;
}

_AFXINL void* AfxGetParent(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase const* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    afxClass const *cls = hdr->cls;
    AfxAssertType(cls, afxFcc_CLS);
    return AfxGetLinker(&cls->host);
}

_AFXINL afxBool AfxTestObjectFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase const* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    AfxAssert(flags);
    return (hdr->flags & flags);
}

_AFXINL afxError AfxObjectAssert(afxObject obj, afxFcc fcc, afxHere const hint, afxChar const *exp)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase const* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    AfxAssert(fcc);
    AfxAssert(hint);
    AfxAssert(exp);
    
    if (!AfxTestObjectFcc(obj, fcc))
    {
        ((err) = (-((afxError)__LINE__)), AfxLogError(exp));
    }
    return err;
}

_AFXINL afxError AfxObjectTryAssert(afxObject obj, afxFcc fcc, afxHere const hint, afxChar const *exp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(hint);
    AfxAssert(exp);

    if (!obj) ((err) = (-((afxError)__LINE__)), AfxLogError(exp));
    else
    {
        afxObjectBase const* hdr = (afxObjectBase*)obj;
        --hdr;
        AfxAssert(hdr->fcc == afxFcc_OBJ);
        AfxAssert(fcc);

        if (!AfxTestObjectFcc(obj, fcc))
            ((err) = (-((afxError)__LINE__)), AfxLogError(exp));
    }
    return err;
}

_AFXINL afxObjectFlags AfxSetObjectFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    return (hdr->flags |= flags);
}

_AFXINL afxObjectFlags AfxClearObjectFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    return (hdr->flags &= ~flags);
}

_AFXINL afxError AfxAllocateInstanceData(afxObject obj, afxNat cnt, afxObjectStash const stashes[])
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetClass(obj);
    afxArena* aren = AfxGetClassArena(cls);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxObjectStash const* stash = &stashes[i];
        afxNat alignedSiz = AFX_ALIGNED_SIZEOF(stash->siz, AfxMax(sizeof(void*), stash->align));
        void* p = stash->cnt ? AfxAllocateArena(aren, AfxMax(1, stash->cnt) * alignedSiz) : NIL;

        if (stash->cnt && !p)
        {
            AfxThrowError();

            for (afxNat j = i; j-- > 0;)
            {
                afxObjectStash const* stash2 = &stashes[j];
                afxNat alignedSiz = AFX_ALIGNED_SIZEOF(stash2->siz, AfxMax(sizeof(void*), stash2->align));
                AfxRecycleArena(aren, *stash2->var, AfxMax(1, stash2->cnt) * alignedSiz);
            }
        }
        *stash->var = p;
    }
    return err;
}

_AFXINL afxError AfxDeallocateInstanceData(afxObject obj, afxNat cnt, afxObjectStash const stashes[])
{
    afxError err = AFX_ERR_NONE;
    afxClass* cls = AfxGetClass(obj);
    afxArena* aren = AfxGetClassArena(cls);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxObjectStash const* stash = &stashes[i];

        if (stash->cnt)
        {
            AfxAssert(*stash->var);
            void* p = *stash->var;

            if (p)
            {
                afxNat alignedSiz = AFX_ALIGNED_SIZEOF(stash->siz, AfxMax(sizeof(void*), stash->align));
                AfxRecycleArena(aren, p, AfxMax(1, stash->cnt) * alignedSiz);
            }
            *stash->var = NIL;
        }
    }
    return err;
}
_AFX void AfxResetEventFilter(afxObject obj, afxBool(*filter)(afxObject,afxObject,afxEvent*))
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);

    if (filter) hdr->eventFilter = filter;
    else
    {
        afxClass const* cls = hdr->cls;
        AfxAssert(cls);
        hdr->eventFilter = cls->defEventFilter;
    }
}

_AFX void AfxResetEventHandler(afxObject obj, afxBool(*handler)(afxObject,afxEvent*))
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);

    if (handler) hdr->event = handler;
    else
    {
        afxClass const* cls = hdr->cls;
        AfxAssert(cls);
        hdr->event = cls->defEvent;
    }
}

_AFX afxError AfxDisconnectObjects(afxObject obj, afxNat cnt, afxObject watcheds[])
{
    afxError err = AFX_ERR_NONE;

    for (afxNat objIdx = 0; objIdx < cnt; objIdx++)
    {
        afxObject watched = watcheds[objIdx];
        afxObjectBase* hdr = (afxObjectBase*)watched;
        --hdr;
        AfxAssertType(hdr, afxFcc_OBJ);
        afxObjectBase* filterInst = (afxObjectBase*)obj;
        --filterInst;
        AfxLogEcho("Deinstalling watcher <%p> for %p", obj, watched);
        
        while (hdr->watchers)
        {
            afxEventFilter *flt;
            AfxChainForEveryLinkage(hdr->watchers, afxEventFilter, watched, flt)
            {
                AfxAssert(AfxGetLinker(&flt->watched) == watched);

                if (AfxGetLinker(&flt->holder) == obj)
                {
                    AfxPopLinkage(&flt->watched);
                    AfxPopLinkage(&flt->holder);
                    AfxDeallocate(flt);

                    if (hdr->watchers && hdr->watchers->cnt == 0)
                    {
                        AfxDeallocate(hdr->watchers);
                        hdr->watchers = NIL;
                    }

                    if (filterInst->watching && filterInst->watching->cnt == 0)
                    {
                        AfxDeallocate(filterInst->watching);
                        filterInst->watching = NIL;
                    }
                    break;
                }
            }
            break;
        }
    }
    return err;
}

_AFX afxError AfxConnectObjects(afxObject obj, afxNat cnt, afxObject watcheds[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(watcheds);
    AfxAssert(cnt);

    for (afxNat objIdx = 0; objIdx < cnt; objIdx++)
    {
        afxObject watched = watcheds[objIdx];
        afxObjectBase* hdr = (afxObjectBase*)watched;
        --hdr;
        AfxAssertType(hdr, afxFcc_OBJ);
        afxObjectBase* filterInst = (afxObjectBase*)obj;
        --filterInst;
        AfxAssertType(filterInst, afxFcc_OBJ);
        AfxLogEcho("Installing watcher <%p> for %p...", obj, watched);
        //AfxAssert(fn);

        afxBool(*fn)(afxObject obj, afxObject watched, afxEvent *ev) = filterInst->eventFilter;

        while (fn)
        {
            afxEventFilter *flt;

            if (hdr->watchers)
            {
                AfxChainForEveryLinkage(hdr->watchers, afxEventFilter, watched, flt)
                {
                    AfxAssert(AfxGetLinker(&flt->watched) == watched);

                    if (AfxGetLinker(&flt->holder) == obj)
                        break; // already exists
                }
            }
            else
            {
                if (!(hdr->watchers = AfxAllocate(1, sizeof(*hdr->watchers), 0, AfxHere()))) AfxThrowError();
                else
                {
                    AfxDeployChain(hdr->watchers, watched);
                }
            }

            if (!err)
            {
                if (!filterInst->watching)
                {
                    if (!(filterInst->watching = AfxAllocate(1, sizeof(*filterInst->watching), 0, AfxHere()))) AfxThrowError();
                    else
                    {
                        AfxDeployChain(filterInst->watching, obj);
                    }
                }

                if (!err)
                {
                    if (!(flt = AfxAllocate(1, sizeof(*flt), 0, AfxHere()))) AfxThrowError();
                    else
                    {
                        AfxPushLinkage(&flt->holder, filterInst->watching);
                        AfxPushLinkage(&flt->watched, hdr->watchers);
                    }
                }
            }

            if (err)
            {
                if (hdr->watchers && hdr->watchers->cnt == 0)
                {
                    AfxDeallocate(hdr->watchers);
                    hdr->watchers = NIL;
                }

                if (filterInst->watching && filterInst->watching->cnt == 0)
                {
                    AfxDeallocate(filterInst->watching);
                    filterInst->watching = NIL;
                }
            }
            break;
        }

        if (err)
        {
            AfxDisconnectObjects(obj, objIdx, watcheds);
            break;
        }
    }
    return err;
}

_AFX afxBool AfxNotifyObject(afxObject obj, afxEvent* ev)
{
    afxError err = AFX_ERR_NONE;

    // Sends event to receiver: receiver->event(event). 
    // Returns the value that is returned from the receiver's event handler. 
    // Note that this function is called for all events sent to any object in any thread.

    // This virtual function receives events to an object and should return true if the event e was recognized and processed.

    afxObjectBase const* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    //AfxAssertType(ev, afxFcc_EVNT);

    // An event filter is an object that receives all events that are sent to this object. 
    // The filter can either stop the event or forward it to this object. 
    // The event filter filterObj receives events via its eventFilter() function. 
    // The eventFilter() function must return true if the event should be filtered, (i.e. stopped); otherwise it must return false.

    // If multiple event filters are installed on a single object, the filter that was installed last is activated first.

    afxBool intercept = FALSE;

    if (hdr->watchers)
    {
        afxEventFilter *flt;
        AfxChainForEveryLinkage(hdr->watchers, afxEventFilter, watched, flt)
        {
            AfxAssert(AfxGetLinker(&flt->watched) == obj);
            afxObjectBase* fltHdr = (afxObjectBase*)AfxGetLinker(&flt->holder);
            --fltHdr;
            AfxAssertType(fltHdr, afxFcc_OBJ);
            afxBool(*eventFilter)(afxObject obj, afxObject watched, afxEvent *ev) = fltHdr->eventFilter;

            if (eventFilter)
            {
                AfxAssert(eventFilter);
                intercept |= eventFilter(AfxGetLinker(&flt->holder), obj, ev);
            }
        }
    }

    if (!intercept)
    {
        afxBool(*event)(afxObject obj, afxEvent *ev) = hdr->event;

        if (event)
            return event(obj, ev);
    }
    return FALSE;
}
