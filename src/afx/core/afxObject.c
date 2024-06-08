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
    afxManager* mgr = AfxGetManager(obj);

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

_AFXINL afxFcc AfxGetObjectFcc(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    afxManager* mgr = AfxGetManager(obj);
    AfxAssertType(mgr, afxFcc_CLS);
    return mgr->objFcc;
}

_AFXINL afxChar const* AfxGetObjectFccAsString(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(obj, afxFcc_OBJ);
    afxManager* mgr = AfxGetManager(obj);
    AfxAssertType(mgr, afxFcc_CLS);
    return (afxChar const *)&mgr->objFcc;
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
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    return hdr->tid;
}

_AFXINL afxManager* AfxGetManager(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    afxManager *mgr = hdr->mgr;
    AfxAssertType(mgr, afxFcc_CLS);
    return mgr;
}

_AFXINL afxBool AfxObjectInherits(afxObject obj, afxManager const* mgr)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    AfxAssertType(mgr, afxFcc_CLS);

    afxManager* objCls = hdr->mgr;
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
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    afxManager const *mgr = hdr->mgr;
    AfxAssertType(mgr, afxFcc_CLS);
    return AfxGetLinker(&mgr->host);
}

_AFXINL afxBool AfxTestObjectFlags(afxObject obj, afxObjectFlags flags)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    AfxAssert(flags);
    return (hdr->flags & flags);
}

_AFXINL afxError AfxObjectAssert(afxObject obj, afxFcc fcc, afxHere const hint, afxChar const *exp)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
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
        afxObjectBase* hdr = (afxObjectBase*)obj;
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

_AFX void AfxInstallEventFilter(afxObject obj, afxBool(*filter)(afxObject,afxObject,afxEvent*))
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    hdr->eventFilter = filter;
}

_AFX void AfxInstallEventHandler(afxObject obj, afxBool(*handler)(afxObject,afxEvent*))
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    hdr->event = handler;
}

_AFX afxError AfxInstallWatcher(afxObject obj, afxObject watcher)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    afxObjectBase* filterInst = (afxObjectBase*)watcher;
    --filterInst;
    AfxAssertType(filterInst, afxFcc_OBJ);
    AfxLogEcho("Installing watcher <%p> for %p...", watcher, obj);
    //AfxAssert(fn);
    
    afxBool(*fn)(afxObject obj, afxObject watched, afxEvent *ev) = filterInst->eventFilter;

    while (fn)
    {
        afxEventFilter *flt;

        if (hdr->watchers)
        {
            AfxChainForEveryLinkage(hdr->watchers, afxEventFilter, watched, flt)
            {
                AfxAssert(AfxGetLinker(&flt->watched) == obj);

                if (AfxGetLinker(&flt->holder) == watcher)
                    return err;
            }
        }
        else
        {
            if (!(hdr->watchers = AfxAllocate(1, sizeof(*hdr->watchers), 0, AfxHere()))) AfxThrowError();
            else
            {
                AfxSetUpChain(hdr->watchers, obj);
            }
        }

        if (!err)
        {
            if (!filterInst->watching)
            {
                if (!(filterInst->watching = AfxAllocate(1, sizeof(*filterInst->watching), 0, AfxHere()))) AfxThrowError();
                else
                {
                    AfxSetUpChain(filterInst->watching, watcher);
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
    return err;
}

_AFX afxBool AfxDeinstallWatcher(afxObject obj, afxObject watcher)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    afxObjectBase* filterInst = (afxObjectBase*)watcher;
    --filterInst;
    AfxAssertType(filterInst, afxFcc_OBJ);
    AfxLogEcho("Deinstalling watcher <%p> for %p", watcher, obj);
    afxBool rslt = FALSE;

    while (hdr->watchers)
    {
        afxEventFilter *flt;
        AfxChainForEveryLinkage(hdr->watchers, afxEventFilter, watched, flt)
        {
            AfxAssert(AfxGetLinker(&flt->watched) == obj);

            if (AfxGetLinker(&flt->holder) == watcher)
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
                rslt = TRUE;
                break;
            }
        }
        break;
    }
    return rslt;
}

_AFX afxBool AfxNotifyObject(afxObject obj, afxEvent* ev)
{
    afxError err = AFX_ERR_NONE;

    // Sends event to receiver: receiver->event(event). 
    // Returns the value that is returned from the receiver's event handler. 
    // Note that this function is called for all events sent to any object in any thread.

    // This virtual function receives events to an object and should return true if the event e was recognized and processed.

    afxObjectBase* hdr = (afxObjectBase*)obj;
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
