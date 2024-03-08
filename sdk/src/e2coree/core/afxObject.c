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
#include "qwadro/core/afxObject.h"
#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxEvent.h"
#include "qwadro/core/afxString.h"
#include "qwadro/mem/afxMmu.h"
#include "qwadro/core/afxDebug.h"
#include "qwadro/async/afxThread.h"
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
        AfxTakeSlock(&refCntSlock);
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

    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxHandle* holderInst = (afxHandle*)holder;
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

    AfxEcho("Taking connection %p connecting %p?%.4s#%i to holder %p?%.4s#%i.", objc, AfxPushObject(objc->obj), AfxPushObject(objc->holder));

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

    if (objc->fcc == afxFcc_OBJC && objc->obj && AfxObjectGetClass(objc->obj)->fcc == afxFcc_CLS)
    {
        AfxAssertConnection(objc);

        AfxEcho("Dropping connection %p from %p?%.4s#%i to holder %p?%.4s#%i.", objc, AfxPushObject(objc->obj), AfxPushObject(objc->holder));

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

_AFXINL afxResult _AfxClsTestObjFccRecursive(afxClass const *cls, afxFcc fcc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(cls, afxFcc_CLS);

    if (cls->objFcc == fcc) return 1;
    else if (cls->base.chain)
        return _AfxClsTestObjFccRecursive(cls->base.chain->owner, fcc);

    return 0;
}

_AFXINL afxResult AfxObjectTestFcc(afxObject obj, afxFcc fcc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    AfxAssert(fcc);
    afxClass* cls = AfxObjectGetClass(obj);

    if (!cls) return 1;
    else
    {
        AfxAssertType(cls, afxFcc_CLS);

        if (cls->objFcc == fcc) return 1;
        else if (cls->base.chain)
            return _AfxClsTestObjFccRecursive(cls->base.chain->owner, fcc);

    }
    return 0;
}

_AFXINL afxFcc AfxObjectGetFcc(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    afxClass* cls = AfxObjectGetClass(obj);
    AfxAssertType(cls, afxFcc_CLS);
    return cls->objFcc;
}

_AFXINL afxChar const* AfxObjectGetFccAsString(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    afxClass* cls = AfxObjectGetClass(obj);
    AfxAssertType(cls, afxFcc_CLS);
    return (afxChar const *)&cls->objFcc;
}

_AFXINL afxInt32 AfxGetRefCount(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    return inst->refCnt;
}

_AFXINL afxNat32 AfxObjectGetTid(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    return inst->tid;
}

_AFXINL afxClass* AfxObjectGetClass(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxClass *cls = inst->cls;
    AfxAssertType(cls, afxFcc_CLS);
    return cls;
}

_AFXINL afxBool AfxObjectInherits(afxObject obj, afxClass const* cls)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    AfxAssertType(cls, afxFcc_CLS);

    afxClass* objCls = inst->cls;
    AfxAssertType(objCls, afxFcc_CLS);

    while (1)
    {
        if (cls)
        {
            AfxAssertType(cls, afxFcc_CLS);

            if (objCls == cls)
                return TRUE;

            cls = AfxGetBaseClass(cls);
        }
    }
    return FALSE;
}

_AFXINL void* AfxGetObjectProvider(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxClass const *cls = inst->cls;
    AfxAssertType(cls, afxFcc_CLS);
    return cls->provider.chain->owner;
}

_AFXINL afxBool AfxObjectTestFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    AfxAssert(flags);
    return (inst->flags & flags);
}

_AFXINL afxError AfxObjectAssert(afxObject obj, afxFcc fcc, afxHint const hint, afxChar const *exp)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    AfxAssert(fcc);
    AfxAssert(hint);
    AfxAssert(exp);
    
    if (!AfxObjectTestFcc(obj, fcc))
    {
        ((err) = (-((afxError)__LINE__)), AfxLogError(hint, exp));
    }
    return err;
}

_AFXINL afxError AfxObjectTryAssert(afxObject obj, afxFcc fcc, afxHint const hint, afxChar const *exp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(hint);
    AfxAssert(exp);

    if (!obj) ((err) = (-((afxError)__LINE__)), AfxLogError(hint, exp));
    else
    {
        afxHandle* inst = (afxHandle*)obj;
        --inst;
        AfxAssert(inst->fcc == afxFcc_OBJ);
        AfxAssert(fcc);

        if (!AfxObjectTestFcc(obj, fcc))
            ((err) = (-((afxError)__LINE__)), AfxLogError(hint, exp));
    }
    return err;
}

_AFXINL afxObjectFlags AfxObjectSetFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    return (inst->flags |= flags);
}

_AFXINL afxObjectFlags AfxObjectClearFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    return (inst->flags &= ~flags);
}

_AFX afxError AfxObjectInstallEventFilter(afxObject obj, afxObject filter)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxHandle* filterInst = (afxHandle*)filter;
    --filterInst;
    AfxAssertType(filterInst, afxFcc_OBJ);
    AfxEcho("afxHandle<%p>::InstallEventFilter(%p)", obj, filter);
    //AfxAssert(fn);
    
    afxBool(*fn)(afxObject obj, afxObject watched, afxEvent *ev) = AfxObjectGetClass(filter)->eventFilter;

    while (fn)
    {
        afxEventFilter *flt;

        if (inst->watchers)
        {
            AfxChainForEveryLinkage(inst->watchers, afxEventFilter, watched, flt)
            {
                AfxAssert(flt->watched.chain->owner == obj);

                if (flt->holder.chain->owner == filter)
                    return err;
            }
        }
        else
        {
            if (!(inst->watchers = AfxAllocate(NIL, 1, sizeof(*inst->watchers), 0, AfxHint()))) AfxThrowError();
            else
            {
                AfxSetUpChain(inst->watchers, obj);
            }
        }

        if (!err)
        {
            if (!filterInst->watching)
            {
                if (!(filterInst->watching = AfxAllocate(NIL, 1, sizeof(*filterInst->watching), 0, AfxHint()))) AfxThrowError();
                else
                {
                    AfxSetUpChain(filterInst->watching, filter);
                }
            }

            if (!err)
            {
                if (!(flt = AfxAllocate(NIL, 1, sizeof(*flt), 0, AfxHint()))) AfxThrowError();
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
                AfxDeallocate(NIL, inst->watchers);
                inst->watchers = NIL;
            }

            if (filterInst->watching && filterInst->watching->cnt == 0)
            {
                AfxDeallocate(NIL, filterInst->watching);
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
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxHandle* filterInst = (afxHandle*)filter;
    --filterInst;
    AfxAssertType(filterInst, afxFcc_OBJ);
    AfxEcho("afxHandle<%p>::RemoveEventFilter(%p)", obj, filter);

    while (inst->watchers)
    {
        afxEventFilter *flt;
        AfxChainForEveryLinkage(inst->watchers, afxEventFilter, watched, flt)
        {
            AfxAssert(flt->watched.chain->owner == obj);

            if (flt->holder.chain->owner == filter)
            {
                AfxPopLinkage(&flt->watched);
                AfxPopLinkage(&flt->holder);
                AfxDeallocate(NIL, flt);

                if (inst->watchers && inst->watchers->cnt == 0)
                {
                    AfxDeallocate(NIL, inst->watchers);
                    inst->watchers = NIL;
                }

                if (filterInst->watching && filterInst->watching->cnt == 0)
                {
                    AfxDeallocate(NIL, filterInst->watching);
                    filterInst->watching = NIL;
                }
                break;
            }
        }
        break;
    }
    return err;
}

_AFX afxBool AfxObjectEmitEvent(afxObject obj, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    AfxAssertType(ev, afxFcc_EVNT);

    afxBool intercept = FALSE;

    while (inst->watchers)
    {
        afxEventFilter *flt;
        AfxChainForEveryLinkage(inst->watchers, afxEventFilter, watched, flt)
        {
            AfxAssert(flt->watched.chain->owner == obj);
            afxBool(*eventFilter)(afxObject obj, afxObject watched, afxEvent *ev) = AfxObjectGetClass(flt->holder.chain->owner)->eventFilter;

            if (eventFilter)
            {
                AfxAssert(eventFilter);
                intercept |= eventFilter(flt->holder.chain->owner, obj, ev);
            }
        }
        break;
    }

    afxBool(*event)(afxObject obj, afxEvent *ev) = AfxObjectGetClass(obj)->event;

    if (!intercept && event)
    {
        return event(obj, ev);
    }
    return TRUE;
}

_AFXINL afxResult AfxObjectSignalConnections(afxObject obj, afxEventId evtype, void *data)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    AfxAssert(evtype && evtype < AFX_EVENT_TOTAL);

    afxEvent ev;
    AfxEventDeploy(&ev, evtype, obj, data);
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
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    afxHandle* holderInst = (afxHandle*)holder;
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
        afxMmu mmu = AfxGetSystemContext();

        afxBool alloced = (!slot && !!(slot = AfxAllocate(mmu, 1, sizeof(*slot), 0, AfxHint())));
        
        if (!slot) AfxThrowError();
        else
        {
            afxHandle* inst = (afxHandle*)obj;
            --inst;
            AfxAssertType(inst, afxFcc_OBJ);
            
            if (holder)
            {
                afxHandle* holderInst = (afxHandle*)holder;
                --holderInst;
                AfxAssertType(holderInst, afxFcc_OBJ);
            }

            if (_AfxConnectionDeploy(slot, obj, holder, handler, filter))
            {
                AfxThrowError();

                if (alloced)
                {
                    AfxDeallocate(mmu, slot);
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
