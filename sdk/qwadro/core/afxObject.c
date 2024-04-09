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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

//#define _AFX_SYSTEM_C
#include "qwadro/core/afxSystem.h"

// SLOTS incorporados diretamente no espaço de array estavam crashando e gerando anomalias de corrupção de leitura/escrita e alocação.
// Quando optado por operar por slots alocados individualmente e arranjados como pointeiros, o problema foi resolvido.
// O problema ocorria após a necessidade de mover os elementos. Aparentemente a mudança com função especial de cópia não funcionou.

afxSlock refCntSlock;
afxBool refCntSlockInited = FALSE;

_AFX void _AfxInitObjRefCntSlck(void)
{
    if (!refCntSlockInited)
    {
        AfxSetUpSlock(&refCntSlock);
        refCntSlockInited = TRUE;
    }
}

_AFX afxInt32 _AfxObjIncRefCntr(afxInt32 *refCntr)
{
    if (!refCntSlockInited)
        _AfxInitObjRefCntSlck();

    AfxEnterSlockExclusive(&refCntSlock);
    afxInt32 cnt = (*refCntr = ++(*refCntr));
    AfxExitSlockExclusive(&refCntSlock);
    return cnt;
}

_AFX afxInt32 _AfxObjDecRefCntr(afxInt32 *refCntr)
{
    if (!refCntSlockInited)
        _AfxInitObjRefCntSlck();

    AfxEnterSlockExclusive(&refCntSlock);
    afxInt32 cnt = (*refCntr = --(*refCntr));
    AfxExitSlockExclusive(&refCntSlock);
    return cnt;
}

// AUXILIAR FUNCTIONS /////////////////////////////////////////////////////////

_AFXINL afxError _AfxConnectionDeploy(afxConnection *objc, afxObject obj, afxObject holder, void(*handler)(afxObject,afxEvent*), afxNat32 filter)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(objc);
    AfxAssertType(obj, afxFcc_OBJ);
    //AfxAssertAbstractObject(holder);

    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxObjectBase* holderInst = (afxObjectBase*)holder;
    --holderInst;
    AfxAssertType(holderInst, afxFcc_OBJ);

    afxList *handling = holder ? &holderInst->handling : NIL;

    objc->fcc = afxFcc_OBJC;

    if (handling)
        AfxPrependLink(&objc->holderLink, handling);
    else
        AfxDeployLink(&objc->holderLink);

    objc->holder = holder;
    AfxPrependLink(&objc->objLink, &inst->signaling);
    objc->obj = obj;
    objc->handler = handler;
    objc->filter = filter;
    objc->reacquired = FALSE;
    objc->alloced = FALSE;
    objc->tenacity = 1;

    AfxLogEcho("Taking connection %p connecting %p?%.4s#%i to holder %p?%.4s#%i.", objc, AfxPushObject(objc->obj), AfxPushObject(objc->holder));

    return err;
}

_AFXINL afxInt32 AfxConnectionIncTenacity(afxConnection *objc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertConnection(objc);
    return ++objc->tenacity;
}

_AFXINL afxInt32 AfxConnectionDecTenacity(afxConnection *objc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertConnection(objc);
    return --objc->tenacity;
}

_AFXINL afxError AfxConnectionSetFilter(afxConnection *objc, afxNat32 filter)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertConnection(objc);
    objc->filter = filter;
    return err;
}

_AFXINL afxError AfxConnectionSetHandler(afxConnection *objc, void(*handler)(afxObject, afxEvent*))
{
    afxError err = AFX_ERR_NONE;
    AfxAssertConnection(objc);
    objc->handler = handler;
    return err;
}

_AFXINL afxError AfxConnectionReset(afxConnection *objc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(objc);
    
    objc->fcc = afxFcc_OBJC;
    AfxDeployLink(&objc->holderLink);
    objc->holder = NIL;
    AfxDeployLink(&objc->objLink);
    objc->obj = NIL;
    objc->handler = NIL;
    objc->filter = NIL; // doesn't avoid any event; handle all them.
    objc->reacquired = FALSE;

    return err;
}

_AFXINL afxError AfxConnectionDrop(afxConnection *objc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(objc);

    if (objc->fcc == afxFcc_OBJC && objc->obj && AfxGetClass(objc->obj)->fcc == afxFcc_CLS)
    {
        AfxAssertConnection(objc);

        AfxLogEcho("Dropping connection %p from %p?%.4s#%i to holder %p?%.4s#%i.", objc, AfxPushObject(objc->obj), AfxPushObject(objc->holder));

        AfxDropLink(&objc->objLink);

        if (objc->reacquired)
        {
            AfxReleaseObjects(1, &objc->obj);
            objc->reacquired = FALSE;
        }

        objc->obj = NIL;
        objc->handler = NIL;

        if (objc->holder)
        {
            AfxDropLink(&objc->holderLink);
            objc->holder = NIL;
        }
        objc->tenacity = 0;
    }
    return err;
}

_AFXINL void* AfxConnectionGetObject(afxConnection const *objc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(objc);
    //AfxAssertConnection(objc);
    return objc->fcc == afxFcc_OBJC ? objc->obj : NIL;
}

_AFXINL void* AfxConnectionGetHolder(afxConnection const *objc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(objc);
    //AfxAssertConnection(objc);
    return objc->fcc == afxFcc_OBJC ? objc->holder : NIL;
}

_AFXINL afxResult _AfxClsTestObjFccRecursive(afxManager const *mgr, afxFcc fcc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mgr, afxFcc_CLS);

    if (mgr->objFcc == fcc) return 1;
    else if (mgr->subset.chain)
        return _AfxClsTestObjFccRecursive(AfxGetLinker(&mgr->subset), fcc);

    return 0;
}

_AFXINL afxResult AfxTestObjectFcc(afxObject obj, afxFcc fcc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    AfxAssert(fcc);
    afxManager* mgr = AfxGetClass(obj);

    if (!mgr) return 1;
    else
    {
        AfxAssertType(mgr, afxFcc_CLS);

        if (mgr->objFcc == fcc) return 1;
        else if (mgr->subset.chain)
            return _AfxClsTestObjFccRecursive(AfxGetLinker(&mgr->subset), fcc);

    }
    return 0;
}

_AFXINL afxFcc AfxObjectGetFcc(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    afxManager* mgr = AfxGetClass(obj);
    AfxAssertType(mgr, afxFcc_CLS);
    return mgr->objFcc;
}

_AFXINL afxChar const* AfxObjectGetFccAsString(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    afxManager* mgr = AfxGetClass(obj);
    AfxAssertType(mgr, afxFcc_CLS);
    return (afxChar const *)&mgr->objFcc;
}

_AFXINL afxInt32 AfxGetRefCount(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    return inst->refCnt;
}

_AFXINL afxNat32 AfxGetObjectTid(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    return inst->tid;
}

_AFXINL afxManager* AfxGetClass(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxManager *mgr = inst->mgr;
    AfxAssertType(mgr, afxFcc_CLS);
    return mgr;
}

_AFXINL afxBool AfxObjectInherits(afxObject obj, afxManager const* mgr)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    AfxAssertType(mgr, afxFcc_CLS);

    afxManager* objCls = inst->mgr;
    AfxAssertType(objCls, afxFcc_CLS);

    while (1)
    {
        if (mgr)
        {
            AfxAssertType(mgr, afxFcc_CLS);

            if (objCls == mgr)
                return TRUE;

            mgr = AfxGetSubmanager(mgr);
        }
    }
    return FALSE;
}

_AFXINL void* AfxGetObjectProvider(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxManager const *mgr = inst->mgr;
    AfxAssertType(mgr, afxFcc_CLS);
    return AfxGetLinker(&mgr->host);
}

_AFXINL afxBool AfxObjectTestFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    AfxAssert(flags);
    return (inst->flags & flags);
}

_AFXINL afxError AfxObjectAssert(afxObject obj, afxFcc fcc, afxHere const hint, afxChar const *exp)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
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
        afxObjectBase* inst = (afxObjectBase*)obj;
        --inst;
        AfxAssert(inst->fcc == afxFcc_OBJ);
        AfxAssert(fcc);

        if (!AfxTestObjectFcc(obj, fcc))
            ((err) = (-((afxError)__LINE__)), AfxLogError(exp));
    }
    return err;
}

_AFXINL afxObjectFlags AfxObjectSetFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    return (inst->flags |= flags);
}

_AFXINL afxObjectFlags AfxObjectClearFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    return (inst->flags &= ~flags);
}

_AFX afxError AfxObjectInstallEventFilter(afxObject obj, afxObject filter)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxObjectBase* filterInst = (afxObjectBase*)filter;
    --filterInst;
    AfxAssertType(filterInst, afxFcc_OBJ);
    AfxLogEcho("afxHandle<%p>::InstallEventFilter(%p)", obj, filter);
    //AfxAssert(fn);
    
    afxBool(*fn)(afxObject obj, afxObject watched, afxEvent *ev) = AfxGetClass(filter)->eventFilter;

    while (fn)
    {
        afxEventFilter *flt;

        if (inst->watchers)
        {
            AfxChainForEveryLinkage(inst->watchers, afxEventFilter, watched, flt)
            {
                AfxAssert(AfxGetLinker(&flt->watched) == obj);

                if (AfxGetLinker(&flt->holder) == filter)
                    return err;
            }
        }
        else
        {
            if (!(inst->watchers = AfxAllocate(1, sizeof(*inst->watchers), 0, AfxHere()))) AfxThrowError();
            else
            {
                AfxSetUpChain(inst->watchers, obj);
            }
        }

        if (!err)
        {
            if (!filterInst->watching)
            {
                if (!(filterInst->watching = AfxAllocate(1, sizeof(*filterInst->watching), 0, AfxHere()))) AfxThrowError();
                else
                {
                    AfxSetUpChain(filterInst->watching, filter);
                }
            }

            if (!err)
            {
                if (!(flt = AfxAllocate(1, sizeof(*flt), 0, AfxHere()))) AfxThrowError();
                else
                {
                    AfxPushLinkage(&flt->holder, filterInst->watching);
                    AfxPushLinkage(&flt->watched, inst->watchers);
                }
            }
        }

        if (err)
        {
            if (inst->watchers && inst->watchers->cnt == 0)
            {
                AfxDeallocate(inst->watchers);
                inst->watchers = NIL;
            }

            if (filterInst->watching && filterInst->watching->cnt == 0)
            {
                AfxDeallocate(filterInst->watching);
                filterInst->watching = NIL;
            }
        }
        break;
    }
    return err;
}

_AFX afxError AfxObjectRemoveEventFilter(afxObject obj, afxObject filter)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxObjectBase* filterInst = (afxObjectBase*)filter;
    --filterInst;
    AfxAssertType(filterInst, afxFcc_OBJ);
    AfxLogEcho("afxHandle<%p>::RemoveEventFilter(%p)", obj, filter);

    while (inst->watchers)
    {
        afxEventFilter *flt;
        AfxChainForEveryLinkage(inst->watchers, afxEventFilter, watched, flt)
        {
            AfxAssert(AfxGetLinker(&flt->watched) == obj);

            if (AfxGetLinker(&flt->holder) == filter)
            {
                AfxPopLinkage(&flt->watched);
                AfxPopLinkage(&flt->holder);
                AfxDeallocate(flt);

                if (inst->watchers && inst->watchers->cnt == 0)
                {
                    AfxDeallocate(inst->watchers);
                    inst->watchers = NIL;
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
    return err;
}

_AFX afxBool AfxNotifyObject(afxObject obj, afxEvent* ev)
{
    afxError err = AFX_ERR_NONE;

    // Sends event to receiver: receiver->event(event). 
    // Returns the value that is returned from the receiver's event handler. 
    // Note that this function is called for all events sent to any object in any thread.

    // This virtual function receives events to an object and should return true if the event e was recognized and processed.

    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    //AfxAssertType(ev, afxFcc_EVNT);

    // An event filter is an object that receives all events that are sent to this object. 
    // The filter can either stop the event or forward it to this object. 
    // The event filter filterObj receives events via its eventFilter() function. 
    // The eventFilter() function must return true if the event should be filtered, (i.e. stopped); otherwise it must return false.

    // If multiple event filters are installed on a single object, the filter that was installed last is activated first.

    afxBool intercept = FALSE;

    if (inst->watchers)
    {
        afxEventFilter *flt;
        AfxChainForEveryLinkage(inst->watchers, afxEventFilter, watched, flt)
        {
            AfxAssert(AfxGetLinker(&flt->watched) == obj);
            afxBool(*eventFilter)(afxObject obj, afxObject watched, afxEvent *ev) = AfxGetClass(AfxGetLinker(&flt->holder))->eventFilter;

            if (eventFilter)
            {
                AfxAssert(eventFilter);
                intercept |= eventFilter(AfxGetLinker(&flt->holder), obj, ev);
            }
        }
    }

    if (!intercept)
    {
        afxBool(*event)(afxObject obj, afxEvent *ev) = AfxGetClass(obj)->event;

        if (event)
            return event(obj, ev);
    }
    return FALSE;
}

_AFXINL afxResult AfxObjectSignalConnections(afxObject obj, afxEventId evtype, void *data)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    AfxAssert(evtype && evtype < AFX_EVENT_TOTAL);

    afxEvent ev = {0};
    ev.id = evtype;
    afxResult cnt = 0;

    afxConnection *objc;
    AfxForEachListElement(&inst->signaling, afxConnection, objLink, objc)
    {
        AfxAssertType(objc, afxFcc_OBJC);
        AfxAssertType(objc->obj, afxFcc_OBJ);
        AfxAssert(AfxConnectionGetObject(objc) == obj);
        AfxAssertType(objc->holder, afxFcc_OBJ);

        if ((evtype == AFX_EVENT_OBJ_DESTROYED || (objc->filter & evtype)) && objc->handler) // destroy event is always emitted.
        {
            objc->handler(objc->holder, &ev);
            cnt++;
        }
    }
    return cnt;
}

_AFXINL afxResult AfxObjectDisconnect(afxObject obj, afxObject holder, void(*handler)(afxObject,afxEvent*)) // if handler is null, any objc held by holder will be dropped.
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* inst = (afxObjectBase*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxObjectBase* holderInst = (afxObjectBase*)holder;
    --holderInst;
    AfxAssertType(holderInst, afxFcc_OBJ);

    afxResult cnt = 0;

    afxConnection *objc;
    AfxForEachListElement(&inst->signaling, afxConnection, objLink, objc)
    {
        AfxAssertType(objc, afxFcc_OBJC);
        AfxAssertType(objc->obj, afxFcc_OBJ);
        AfxAssert(AfxConnectionGetObject(objc) == obj);
        AfxAssertType(objc->holder, afxFcc_OBJ);

        if (holder == objc->holder)
        {
            if (!objc->handler || objc->handler == handler)
            {
                cnt++;
                 AfxConnectionDrop(objc);
            }
        }
    }
    return cnt;
}

_AFXINL afxConnection* AfxObjectConnect(afxObject obj, afxObject holder, void (*handler)(afxObject,afxEvent*), afxNat32 filter, afxConnection *slot)
{
    afxError err = AFX_ERR_NONE;

    if (!obj) AfxThrowError(); // Diferentemente de outras funções, vamos adicionar lógica de proteção aqui para podermos usar derivados de AfxObjectAcquire() diretamente dentro de AfxObjectReacquire(). Para facilitar o código. Para evitar AfxReleaseObject() conseguinte, use AfxObjectConnect().
    else
    {
        afxSystem sys;
        afxBool alloced = (!slot && !!(slot = AfxAllocate(1, sizeof(*slot), 0, AfxHere())));
        
        if (!slot) AfxThrowError();
        else
        {
            afxObjectBase* inst = (afxObjectBase*)obj;
            --inst;
            AfxAssertType(inst, afxFcc_OBJ);
            
            if (holder)
            {
                afxObjectBase* holderInst = (afxObjectBase*)holder;
                --holderInst;
                AfxAssertType(holderInst, afxFcc_OBJ);
            }

            if (_AfxConnectionDeploy(slot, obj, holder, handler, filter))
            {
                AfxThrowError();

                if (alloced)
                {
                    AfxDeallocate(slot);
                }
                slot = NIL;
            }
            else
            {
                slot->alloced = alloced;
            }
        }
    }
    return slot;
}
