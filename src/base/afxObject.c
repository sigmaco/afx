/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

//#define _AFX_SYSTEM_C
#include "../exec/afxSystemDDK.h"

#define OBJ_HDR_SIZ     AFX_ALIGN_SIZE(sizeof(afxObjectBase), AFX_SIMD_ALIGNMENT)
#define GET_OBJ_HDR(obj_) ((void*)(((afxByte*)obj_) - OBJ_HDR_SIZ))
#define GET_OBJ_ADDR(hdr_) ((void*)(((afxByte*)hdr_) + OBJ_HDR_SIZ))

// SLOTS incorporados diretamente no espaço de array estavam crashando e gerando anomalias de corrupção de leitura/escrita e alocação.
// Quando optado por operar por slots alocados individualmente e arranjados como pointeiros, o problema foi resolvido.
// O problema ocorria após a necessidade de mover os elementos. Aparentemente a mudança com função especial de cópia não funcionou.

// AUXILIAR FUNCTIONS /////////////////////////////////////////////////////////

_AFXINL afxResult _AfxClsTestObjFccRecursive(afxClass const *cls, afxFcc fcc)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);

    if (cls->objFcc == fcc) return 1;
    else if (cls->subset.chain)
        return _AfxClsTestObjFccRecursive(AfxGetLinker(&cls->subset), fcc);

    return 0;
}

_AFXINL afxResult AfxTestObjectFcc(afxObject obj, afxFcc fcc)
{
    afxError err = { 0 };
    //AFX_ASSERT(obj->fcc == afxFcc_OBJ);
    AFX_ASSERT(fcc);
    afxClass const* cls = AfxGetClass(obj);

    if (cls)
    {
        AFX_ASSERT(cls->fcc == afxFcc_CLS);

        if (cls->objFcc == fcc) return 1;
        else if (cls->subset.chain)
            return _AfxClsTestObjFccRecursive(AfxGetLinker(&cls->subset), fcc);

    }
    return 0;
}

_AFXINL afxFcc AfxGetObjectFcc(afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT(obj);
    afxClass const* cls = AfxGetClass(obj);
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    return cls->objFcc;
}

_AFXINL afxChar const* AfxGetObjectFccAsString(afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT(obj);
    afxClass const* cls = AfxGetClass(obj);
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    return (afxChar const *)&cls->objFcc;
}

_AFXINL afxInt32 AfxGetRefCount(afxObject obj)
{
    afxError err = { 0 };
    afxObjectBase* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    return AfxLoadAtom32(&hdr->refCnt);
}

_AFXINL afxUnit32 AfxGetObjectTid(afxObject obj)
{
    afxError err = { 0 };
    afxObjectBase const* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    return hdr->tid;
}

_AFXINL afxClass* AfxGetClass(afxObject obj)
{
    afxError err = { 0 };
    afxObjectBase const* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    afxClass *cls = hdr->cls;
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    return cls;
}

_AFXINL afxBool AfxDoesObjectInherit(afxObject obj, afxClass const* cls)
{
    afxError err = { 0 };
    afxObjectBase const* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    AFX_ASSERT(cls->fcc == afxFcc_CLS);

    afxClass const* objCls = hdr->cls;
    AFX_ASSERT(objCls->fcc == afxFcc_CLS);

    while (1)
    {
        if (cls)
        {
            AFX_ASSERT(cls->fcc == afxFcc_CLS);

            if (objCls == cls)
                return TRUE;

            cls = AfxGetSubclass(cls);
        }
    }
    return FALSE;
}

_AFXINL void* AfxGetHost(afxObject obj)
{
    afxError err = { 0 };
    afxObjectBase const* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    afxClass const *cls = hdr->cls;
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    return AfxGetLinker(&cls->host);
}

_AFXINL afxBool AfxTestObjectFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = { 0 };
    afxObjectBase const* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    AFX_ASSERT(flags);
    return (hdr->flags & flags);
}

_AFXINL afxError AfxObjectAssert(afxObject obj, afxFcc fcc, afxHere const hint, afxChar const *exp)
{
    afxError err = { 0 };
    afxObjectBase const* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    AFX_ASSERT(fcc);
    AFX_ASSERT(hint);
    AFX_ASSERT(exp);
    
    if (!AfxTestObjectFcc(obj, fcc))
    {
        ((err) = (-((afxError)__LINE__)), AfxReportError(exp));
    }
    return err;
}

_AFXINL afxError AfxObjectTryAssert(afxObject obj, afxFcc fcc, afxHere const hint, afxChar const *exp)
{
    afxError err = { 0 };
    AFX_ASSERT(hint);
    AFX_ASSERT(exp);

    if (!obj) ((err) = (-((afxError)__LINE__)), AfxReportError(exp));
    else
    {
        afxObjectBase const* hdr = GET_OBJ_HDR(obj);
        AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
        AFX_ASSERT(fcc);

        if (!AfxTestObjectFcc(obj, fcc))
            ((err) = (-((afxError)__LINE__)), AfxReportError(exp));
    }
    return err;
}

_AFXINL afxObjectFlags AfxSetObjectFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = { 0 };
    afxObjectBase* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    return (hdr->flags |= flags);
}

_AFXINL afxObjectFlags AfxClearObjectFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = { 0 };
    afxObjectBase* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    return (hdr->flags &= ~flags);
}

_AFXINL afxError AfxAllocateInstanceData(afxObject obj, afxUnit cnt, afxObjectStash const stashes[])
{
    afxError err = { 0 };
    afxClass* cls = AfxGetClass(obj);
    afxArena* aren = AfxGetClassArena(cls);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxObjectStash const* stash = &stashes[i];
        afxUnit alignedSiz = AFX_ALIGN_SIZE(stash->siz, AFX_MAX(AFX_SIMD_ALIGNMENT, stash->align));
        void* p = stash->cnt ? AfxRequestArena(aren, alignedSiz, AFX_MAX(1, stash->cnt), NIL, 0) : NIL;

        if (stash->cnt && !p)
        {
            AfxThrowError();

            for (afxUnit j = i; j-- > 0;)
            {
                afxObjectStash const* stash2 = &stashes[j];
                afxUnit alignedSiz = AFX_ALIGN_SIZE(stash2->siz, AFX_MAX(sizeof(void*), stash2->align));
                AfxReclaimArena(aren, *stash2->var, AFX_MAX(1, stash2->cnt) * alignedSiz);
            }
        }
        *stash->var = p;
    }
    return err;
}

_AFXINL afxError AfxDeallocateInstanceData(afxObject obj, afxUnit cnt, afxObjectStash const stashes[])
{
    afxError err = { 0 };
    afxClass* cls = AfxGetClass(obj);
    afxArena* aren = AfxGetClassArena(cls);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxObjectStash const* stash = &stashes[i];

        if (stash->cnt)
        {
            AFX_ASSERT(*stash->var);
            void* p = *stash->var;

            if (p)
            {
                afxUnit alignedSiz = AFX_ALIGN_SIZE(stash->siz, AFX_MAX(AFX_SIMD_ALIGNMENT, stash->align));
                AfxReclaimArena(aren, p, AFX_MAX(1, stash->cnt) * alignedSiz);
            }
            *stash->var = NIL;
        }
    }
    return err;
}

_AFX void AfxInstallEventHandler(afxObject obj, afxBool(*handler)(afxObject,afxEvent*))
{
    afxError err = { 0 };
    afxObjectBase* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);

    if (handler) hdr->event = handler;
    else
    {
        afxClass const* cls = hdr->cls;
        AFX_ASSERT(cls);
        hdr->event = cls->defEvent;
    }
}

_AFX afxError AfxDisconnectObjects(afxObject obj, afxBool(*handler)(afxObject obj, afxObject watched, afxEvent *ev), afxUnit cnt, afxObject watcheds[])
{
    afxError err = { 0 };

    for (afxUnit objIdx = 0; objIdx < cnt; objIdx++)
    {
        afxObject watched = watcheds[objIdx];

        if (!watched)
            continue;

        afxObjectBase* hdr = GET_OBJ_HDR(watched);
        AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
        afxObjectBase* filterInst = GET_OBJ_HDR(obj);
        AfxReportMessage("Deinstalling watcher <%p> for %p", obj, watched);
        
        while (hdr->watchers)
        {
            afxEventFilter *flt;
            AFX_ITERATE_CHAIN(flt, watched, hdr->watchers)
            {
                AFX_ASSERT(AfxGetLinker(&flt->watched) == watched);

                if ((AfxGetLinker(&flt->holder) == obj) && (!handler || (flt->fn == handler)))
                {
                    AfxPopLink(&flt->watched);
                    AfxPopLink(&flt->holder);
                    AfxDeallocate((void**)&flt, AfxHere());

                    if (hdr->watchers && hdr->watchers->cnt == 0)
                    {
                        AfxDeallocate((void**)&hdr->watchers, AfxHere());
                        hdr->watchers = NIL;
                    }

                    if (filterInst->watching && filterInst->watching->cnt == 0)
                    {
                        AfxDeallocate((void**)&filterInst->watching, AfxHere());
                        filterInst->watching = NIL;
                    }

                    if (handler)
                        break;
                }
            }
            break;
        }

        // dealloc filter chains if there is no more filter.

        if (hdr->watchers && hdr->watchers->cnt == 0)
        {
            AfxDeallocate((void**)&hdr->watchers, AfxHere());
            hdr->watchers = NIL;
        }

        if (filterInst->watching && filterInst->watching->cnt == 0)
        {
            AfxDeallocate((void**)&filterInst->watching, AfxHere());
            filterInst->watching = NIL;
        }
    }
    return err;
}

_AFX afxError AfxConnectObjects(afxObject obj, afxBool(*handler)(afxObject obj, afxObject watched, afxEvent *ev), afxUnit cnt, afxObject watcheds[])
{
    afxError err = { 0 };
    AFX_ASSERT(watcheds);
    AFX_ASSERT(cnt);

    for (afxUnit objIdx = 0; objIdx < cnt; objIdx++)
    {
        afxObject watched = watcheds[objIdx];

        if (!watched)
            continue;

        afxObjectBase* hdr = GET_OBJ_HDR(watched);
        AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
        afxObjectBase* filterInst = GET_OBJ_HDR(obj);
        AFX_ASSERT(filterInst->fcc == afxFcc_OBJ);
        AfxReportMessage("Installing watcher <%p> for %p...", obj, watched);
        //AFX_ASSERT(fn);

        while (handler)
        {
            afxEventFilter *flt;

            if (hdr->watchers)
            {
                AFX_ITERATE_CHAIN(flt, watched, hdr->watchers)
                {
                    AFX_ASSERT(AfxGetLinker(&flt->watched) == watched);

                    if ((AfxGetLinker(&flt->holder) == obj) && (flt->fn == handler))
                        break; // already exists
                }
            }
            else
            {
                if (AfxAllocate(sizeof(*hdr->watchers), 0, AfxHere(), (void**)&hdr->watchers)) AfxThrowError();
                else
                {
                    AfxMakeChain(hdr->watchers, watched);
                }
            }

            if (!err)
            {
                if (!filterInst->watching)
                {
                    if (AfxAllocate(sizeof(*filterInst->watching), 0, AfxHere(), (void**)&filterInst->watching)) AfxThrowError();
                    else
                    {
                        AfxMakeChain(filterInst->watching, obj);
                    }
                }

                if (!err)
                {
                    if (AfxAllocate(sizeof(*flt), 0, AfxHere(), (void**)&flt)) AfxThrowError();
                    else
                    {
                        AfxPushLink(&flt->holder, filterInst->watching);
                        AfxPushLink(&flt->watched, hdr->watchers);
                        flt->fn = handler;
                    }
                }
            }

            if (err)
            {
                // dealloc filter chains if there is no more filter.

                if (hdr->watchers && hdr->watchers->cnt == 0)
                {
                    AfxDeallocate((void**)&hdr->watchers, AfxHere());
                    hdr->watchers = NIL;
                }

                if (filterInst->watching && filterInst->watching->cnt == 0)
                {
                    AfxDeallocate((void**)&filterInst->watching, AfxHere());
                    filterInst->watching = NIL;
                }
            }
            break;
        }

        if (err)
        {
            AfxDisconnectObjects(obj, handler, objIdx, watcheds);
            break;
        }
    }
    return err;
}

_AFX afxBool AfxNotifyObject(afxObject obj, afxEvent* ev)
{
    afxError err = { 0 };

    // Sends event to receiver: receiver->event(event). 
    // Returns the value that is returned from the receiver's event handler. 
    // Note that this function is called for all events sent to any object in any thread.

    // This virtual function receives events to an object and should return true if the event e was recognized and processed.

    afxObjectBase const* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
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
        AFX_ITERATE_CHAIN(flt, watched, hdr->watchers)
        {
            AFX_ASSERT(AfxGetLinker(&flt->watched) == obj);
            afxObjectBase* watcherHdr = GET_OBJ_HDR(AfxGetLinker(&flt->holder));
            AFX_ASSERT(watcherHdr->fcc == afxFcc_OBJ);
            AFX_ASSERT(flt->fn);
            intercept |= flt->fn(AfxGetLinker(&flt->holder), obj, ev);
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
