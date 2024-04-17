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

#define _AFX_MANAGER_C
#include "qwadro/core/afxSystem.h"

_AFX afxBool metaObjectMgrInited = FALSE;
_AFX afxChain orphanClassChain =
{
    {
        &(orphanClassChain.anchor),
        &(orphanClassChain.anchor),
        &(orphanClassChain)
    },
    { NIL },
    0
};

_AFX afxChain* _AfxGetOrphanClasses(void)
{
    return &orphanClassChain;
}

_AFX afxResult AfxExhaustChainedManagers(afxChain *provisions)
{
    afxError err = AFX_ERR_NONE;
    afxResult cnt = 0;

    afxManager* mgr;
    AfxIterateLinkage(afxManager, mgr, provisions, host)
    {
        AfxAssertType(mgr, afxFcc_CLS);
        
        if (mgr->objFcc == afxFcc_SYS)
            break; // We can't drop it, it would fuck the entire platform. Break.
            
        cnt += AfxExhaustManager(mgr);
    }
    return cnt;
}

_AFX afxResult AfxCleanUpChainedManagers(afxChain *provisions)
{
    afxError err = AFX_ERR_NONE;
    afxResult cnt = 0;

    while (1)
    {
        afxLinkage *first = AfxGetLastLinkage(provisions);

        if (!first) break;
        else
        {
            afxManager *mgr = AFX_REBASE(first, afxManager, host);
            AfxAssertType(mgr, afxFcc_CLS);
            ++cnt;

            if (mgr->objFcc != afxFcc_SYS) AfxAbolishManager(mgr);
            else
            {
                AfxAssert(mgr->maxInstCnt == 1);
                //AfxAssert(mgr->pool.totalUsedCnt == 1);
                break; // has only the SYS. We can't drop it else we will fuck the entire platform. Break.
            }                
        }
    }
    return cnt;
}

_AFXINL afxManager* AfxGetSubmanager(afxManager const *mgr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    return AfxGetLinker(&mgr->subset);
}

_AFXINL afxNat AfxGetObjectSize(afxManager const *mgr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    return mgr->siz;
}

_AFXINL afxNat AfxGetObjectStrictSize(afxManager const *mgr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    afxManager *subset = AfxGetSubmanager(mgr);
    return (subset ? mgr->siz - subset->siz : mgr->siz);
}

_AFXINL afxNat AfxCountObjects(afxManager const *mgr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    afxNat instCnt = mgr->instCnt;
    afxChain const *supersets = &mgr->supersets;
    AfxChainForEveryLinkageB2F(supersets, afxManager, subset, mgr)
    {
        AfxAssertType(mgr, afxFcc_CLS);
        instCnt += mgr->instCnt;
    }
    return instCnt;
}

_AFXINL afxObject AfxGetObject(afxManager const* mgr, afxNat32 uniqueId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);    
    afxObject obj;
    AfxEnumerateObjects(mgr, uniqueId, 1, &obj);
    return obj;
}

_AFX afxNat _AfxClsEnumerateObjects(afxManager const* mgr, afxBool fromLast, afxNat first, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    afxNat rslt = 0;

    if (first && (first >= mgr->pool.totalUsedCnt))
    {
        first -= mgr->pool.totalUsedCnt;
    }
    else
    {
        rslt = cnt ? AfxEnumeratePoolItems(&mgr->pool, first, cnt, (void**)objects) : 0;

        if (rslt)
        {
            AfxAssert(cnt >= rslt);

            for (afxNat i = 0; i < rslt; i++)
            {
                afxObjectBase* subset = objects[i];
                AfxAssertType(subset, afxFcc_OBJ);
                //afxModule inst2 = (inst + 1);
                //AfxAssertObjects(1, &inst2, mgr->objFcc);
                objects[i] = (subset + 1);
                AfxAssertObjects(1, &objects[i], mgr->objFcc);
            }

            if (first && (first >= rslt))
                first -= rslt;

            cnt = cnt - rslt;
        }
    }

    if (cnt)
    {
        afxManager *superset;
        afxChain const *supersets = &mgr->supersets;
        AfxChainForEveryLinkageB2F(supersets, afxManager, subset, superset)
        {
            AfxAssertType(superset, afxFcc_CLS);

            if (first && (first >= superset->pool.totalUsedCnt))
            {
                first -= superset->pool.totalUsedCnt;
            }
            else
            {
#if !0
                afxNat rslt2 = _AfxClsEnumerateObjects(superset, fromLast, first, cnt, &objects[rslt]);

                AfxAssert(cnt >= rslt2); // cant be greater

                if (first && (first >= rslt2))
                    first -= rslt2;

                cnt = cnt - rslt2;
                rslt += rslt2;

#else
                afxNat rslt2 = _AfxClsEnumerateObjects(superset, fromLast, first2, cnt2, &objects[rslt]);

                if (rslt2)
                {
                    AfxAssert(cnt2 >= rslt2);
                    cnt2 = cnt2 - rslt2;

                    if (cnt2)
                        first2 = (first2 > cnt2 - 1) ? first2 - cnt2 : 0;

                    rslt += rslt2;
                }
#endif
            }

            if (rslt == cnt)
                break;
        }
    }
    return rslt;
}

_AFX afxNat AfxEnumerateObjects(afxManager const* mgr, afxNat first, afxNat cnt, afxObject obj[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    AfxAssert(cnt);
    afxNat rslt = 0;

    if (mgr->instCnt)
    {
        AfxEnterSlockShared((void*)&mgr->poolLock);
        rslt = _AfxClsEnumerateObjects(mgr, FALSE, first, cnt, obj);
        AfxExitSlockShared((void*)&mgr->poolLock);
    }
    return rslt;
}

_AFX afxNat _AfxClsCurateObjects(afxManager const* mgr, afxBool fromLast, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    AfxAssert(f);
    afxNat rslt = 0;
    afxNat i = 0;
    afxObject obj;
    while (_AfxClsEnumerateObjects(mgr, fromLast, first + i, 1, &obj))
    {
        AfxAssertObjects(1, &obj, mgr->objFcc);
        ++rslt;

        if (f(obj, udd))
            break;
        else if (++i == cnt)
            break;
    }
    return rslt;
}

AFX afxNat AfxInvokeObjects(afxManager const* mgr, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mgr, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(f);
    afxNat rslt = 0;
    
    if (mgr->instCnt)
    {
        AfxEnterSlockShared((void*)&mgr->poolLock);
        rslt = _AfxClsCurateObjects(mgr, FALSE, first, cnt, f, udd);
        AfxExitSlockShared((void*)&mgr->poolLock);
    }
    return rslt;
}

_AFX afxError _AfxDeallocateObjects(afxManager *mgr, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    AfxAssert(objects);
    AfxAssert(cnt);
    
#if !0
    AfxDeallocatePoolUnits(&mgr->pool, cnt, (void**)objects);
#else
    for (afxNat i = 0; i < cnt; i++)
    {
        afxObjectBase *item = (afxObjectBase *)obj[i];
        AfxAssert(item);
        --item;
        AfxDeallocatePoolUnit(&mgr->pool, item);
    }
#endif
    return err;
}

_AFX afxError _AfxAllocateObjects(afxManager *mgr, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    AfxAssert(cnt);

    afxPool* pool = &mgr->pool;
    afxNat room = (mgr->maxInstCnt - pool->totalUsedCnt);

    if ((room == 0) || (cnt > room))
        AfxThrowError();
    else
    {
        if (AfxAllocatePoolUnits(pool, cnt, (void**)objects))
            AfxThrowError();
    }
    return err;
}

_AFX afxError _AfxAllocateObjectsAt(afxManager *mgr, afxNat subset, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    AfxAssert(cnt);

    afxPool* pool = &mgr->pool;
    afxNat room = (mgr->maxInstCnt - pool->totalUsedCnt);

    if ((room == 0) || (cnt > room)) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            objects[i] = NIL;

            if (AfxGetPoolUnit(pool, subset + i, NIL)) AfxThrowError();
            else
            {
                if (AfxOccupyPoolUnit(pool, subset + i, NIL)) AfxThrowError();
                else
                {
                    AfxAssert(AfxGetPoolUnit(pool, subset + i, (void**)&objects[i]));
                }
            }

            if (err)
            {
                _AfxDeallocateObjects(mgr, i, objects);
            }
        }
    }
    return err;
}

_AFX afxError _AfxObjDtor(afxManager *mgr, afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);

    if (mgr->dtor && mgr->dtor(obj))
        AfxThrowError();

    if (err)
    {
        AfxLogEcho("Destructing instance %p of %s class.", obj, mgr->name);
    }

    afxManager* subset = AfxGetSubmanager(mgr);

    if (subset)
    {
        if (err)
        {
            AfxLogEcho("Destructing instance %p of %s : %s class.", obj, mgr->name, subset->name);
        }

        if (_AfxObjDtor(subset, obj))
            AfxThrowError();
    }

    return err;
}

_AFX afxError _AfxDestructObjects(afxManager *mgr, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    AfxAssert(objects);
    AfxAssert(cnt);
 
    for (afxNat i = cnt; i-- > 0;)
    {
        afxObject obj = objects[i];

        if (obj)
        {
            --mgr->instCnt;
            //AfxLogEcho("Dismantling %s object (#%d) %p...", mgr->name, i, obj[i]);
            afxObjectBase *base = (afxObjectBase *)obj;
            --base;
            AfxAssertType(base, afxFcc_OBJ);
            AfxAssert(AfxGetClass(obj) == mgr);

            //afxFcc fcc = AfxObjectGetFcc(item);
            //afxNat refCnt = AfxGetRefCount(item);
            //AfxLogEcho("Destroying %s... %p[%.4s]^%i", mgr->name, item, (afxChar*)&fcc, item->refCnt);

            {
                AfxObjectSignalConnections(obj, AFX_EVENT_OBJ_DESTROYED, NIL);

                afxConnection *objc;
                afxList *connections = &base->handling;

                while (1)
                {
                    afxLink *first = AfxGetFirstLink(connections), *head = AfxGetListHead(connections);

                    if (first == head) break;
                    else
                    {
                        objc = AFX_REBASE(first, afxConnection, holderLink);
                        AfxAssertConnection(objc);
                        AfxConnectionDrop(objc);
                    }
                }

                connections = &base->signaling;

                while (1)
                {
                    afxLink *first = AfxGetFirstLink(connections), *head = AfxGetListHead(connections);

                    if (first == head) break;
                    else
                    {
                        objc = AFX_REBASE(first, afxConnection, objLink);
                        AfxAssertConnection(objc);
                        AfxConnectionDrop(objc);
                    }
                }
            }

            {
                afxEvent ev = { 0 };
                ev.id = AFX_EVENT_OBJ_DESTROYED;
                AfxNotifyObject(obj, &ev);

                afxEventFilter *flt;

                while (base->watching)
                {
                    afxLinkage *first = AfxGetLastLinkage(base->watching);

                    if (!first)
                    {
                        AfxAssert(!base->watching);
                        break;
                    }
                    else
                    {
                        flt = AFX_REBASE(first, afxEventFilter, holder);
                        AfxAssert(AfxGetLinker(&flt->holder) == obj);
                        //AfxAssertType((afxHandle*)flt->watched.chain->owner, afxFcc_OBJ);
                        AfxObjectRemoveEventFilter(AfxGetLinker(&flt->watched), obj);
                    }
                }

                while (base->watchers)
                {
                    afxLinkage *first = AfxGetLastLinkage(base->watchers);

                    if (!first)
                    {
                        AfxAssert(!base->watchers);
                        break;
                    }
                    else
                    {
                        flt = AFX_REBASE(first, afxEventFilter, watched);
                        AfxAssert(AfxGetLinker(&flt->watched) == obj);
                        //AfxAssertType((afxHandle*)flt->holder.chain->owner, afxFcc_OBJ);
                        AfxObjectRemoveEventFilter(obj, AfxGetLinker(&flt->holder));
                    }
                }
            }

            if (_AfxObjDtor(mgr, obj))
                AfxThrowError();

            AfxZero2(1, sizeof(*base), base);
            objects[i] = base;

            //AfxDeallocate(mgr->all, item);

            //AfxLogEcho("Instance %p[%.4s]^%i destroyed.", item, (afxChar const*)&(fcc), refCnt);
        }
    }
    return err;
}

_AFX afxError _AfxObjCtor(afxManager *mgr, afxObject obj, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);

    afxManager* subset = AfxGetSubmanager(mgr);

    if (subset)
    {
        if (_AfxObjCtor(subset, obj, cookie))
            AfxThrowError();

        if (err)
        {
            AfxLogEcho("afxManager<%s, %s>ConstructingObject(%p)", mgr->name, subset->name, obj);
        }
    }

    if (err)
    {
        AfxLogEcho("afxManager<%s>::ConstructObject(%p)", mgr->name, obj);
    }

    if (!err)
    {
        if (mgr->ctor && mgr->ctor(obj, cookie))
        {
            AfxThrowError();

            if (mgr->subset.chain)
                _AfxObjDtor(mgr, obj);
        }
    }
    return err;
}

_AFX afxError _AfxConstructObjects(afxManager *mgr, afxNat cnt, afxObject objects[], void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(objects);
    //AfxAssert(blueprint);

    afxCookie cookie = { 0 };
    cookie.udd = (void**)udd;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxObjectBase* base = objects[i];
        AfxAssert(AfxIsAligned(base, sizeof(void*)));
        base->fcc = afxFcc_OBJ;
        base->mgr = mgr;

        base->refCnt = 1;
        AfxDeployList(&base->signaling);
        AfxDeployList(&base->handling);

        base->tid;
        AfxGetTid(&base->tid);

        base->watchers = NIL;
        base->watching = NIL;

        objects[i] = base + 1; // move the pointer forward

        AfxAssert(AfxIsAligned(objects[i], sizeof(void*)));
        AfxAssert(base == (((afxObjectBase*)(objects[i])) - 1));

        cookie.no = i;

        if (_AfxObjCtor(mgr, objects[i], &cookie))
        {
            AfxThrowError();

            for (afxNat j = i; j-- > 0;)
            {
                _AfxObjDtor(mgr, objects[j]);
                afxObjectBase* subset = objects[j];
                objects[j] = subset - 1; // move the pointer backward
            }
            objects[i] = base - 1; // move the pointer backward
        }
        else
        {
            ++mgr->instCnt;
        }
    }
    return err;
}

_AFX afxNat AfxExhaustManager(afxManager *mgr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);
    afxNat rslt = 0;

    if (mgr->siz)
    {
        afxObject obj;
        while (_AfxClsEnumerateObjects(mgr, TRUE, 0, 1, &obj))
        {
            while (TRUE != AfxReleaseObjects(1, &obj));
            ++rslt;
        }
    }
    return rslt;
}

_AFX afxError AfxAbolishManager(afxManager *mgr)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssertType(mgr, afxFcc_CLS);

    afxNat objCnt = AfxCountObjects(mgr);
    afxManager* subset = AfxGetSubmanager(mgr);

    if (subset)
        AfxLogEcho(objCnt ? "Cleaning up %s[%s]. %u lost objects." : "Cleaning up %s[%s].", mgr->name, subset->name, objCnt);
    else
        AfxLogEcho(objCnt ? "Cleaning up %s. %u lost objects." : "Cleaning up %s.", mgr->name, objCnt);

    //AfxEnterSlockExclusive(&mgr->slock);

    AfxCleanUpChainedManagers(&mgr->supersets);

    AfxAssert(0 == AfxGetChainLength(&mgr->supersets));

    AfxExhaustManager(mgr);

    if (mgr->siz) // only if dynamically allocated
    {
        AfxAssert(0 == mgr->pool.totalUsedCnt);        
        AfxEnterSlockExclusive(&mgr->poolLock);
        AfxCleanUpPool(&mgr->pool);
        AfxExitSlockExclusive(&mgr->poolLock);
        AfxCleanUpSlock(&mgr->poolLock);

        if (mgr->mmu)
            AfxReleaseObjects(1, (void*[]) { mgr->mmu });
    }

    if (mgr->subset.chain)
        AfxPopLinkage(&mgr->subset);

    if (mgr->host.chain)
        AfxPopLinkage(&mgr->host);

    AfxZero(mgr, sizeof(*mgr));

    return err;
}

_AFXINL afxError _AfxDummyObjDtor(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    (void)obj;
    return err;
}

_AFXINL afxError _AfxDummyObjCtor(afxObject obj, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    (void)obj;
    (void)cookie;
    return err;
}

_AFX afxError AfxEstablishManager(afxManager* mgr, afxManager* subset, afxChain* host, afxClassConfig const* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(mgr);
    AfxAssert(cfg);

    AfxZero(mgr, sizeof(*mgr));

    if (host)
    {
        if (subset)
        {
            AfxLogEcho("Setting up %s[%s]", cfg->name, subset->name);
        }
        else
        {
            AfxLogEcho("Setting up %s", cfg->name);
        }
    }
    else
    {
        if (subset)
        {
            AfxLogEcho("Setting up %s[%s]", cfg->name, subset->name);
        }
        else
        {
            AfxLogEcho("Setting up %s", cfg->name);
        }
    }

    //AfxSetUpSlock(&mgr->slock);
    //AfxEnterSlockExclusive(&mgr->slock);

    mgr->fcc = afxFcc_CLS;
    AfxAssert(cfg->fcc);
    mgr->objFcc = cfg->fcc;
    //AfxAssert(cfg->size);
    mgr->siz = cfg->size;

    if (!subset)
    {
        AfxPushLinkage(&mgr->subset, NIL);
        mgr->level = _AFX_CLASS_BASE_LEVEL;
    }
    else
    {
        AfxAssert(subset != mgr);
        AfxAssertType(subset, afxFcc_CLS);
        AfxAssert(cfg->size >= subset->siz);
        AfxPushLinkage(&mgr->subset, &subset->supersets);
        mgr->level = subset->level + 1;
    }

    mgr->levelMask = (_AFX_CLASS_LEVEL_MASK << mgr->level);
    mgr->maxInstCnt = cfg->maxCnt ? cfg->maxCnt : AFX_N32_MAX;
    mgr->ctor = cfg->ctor ? cfg->ctor : _AfxDummyObjCtor;
    mgr->dtor = cfg->dtor ? cfg->dtor : _AfxDummyObjDtor;
    AfxAssert(cfg->name && cfg->name[0]);
    AfxStrcpy(mgr->name, cfg->name ? cfg->name : "");
    mgr->input = NIL;
    mgr->output = NIL;
    mgr->event = cfg->event;
    mgr->eventFilter = cfg->eventFilter;
    mgr->vmt = cfg->vmt;
    mgr->unitsPerPage = cfg->unitsPerPage;

    if (mgr->siz) // only if dynamically allocated
    {
        AfxSetUpSlock(&mgr->poolLock);
        AfxEnterSlockExclusive(&mgr->poolLock);
        AfxSetUpPool(&mgr->pool, mgr->siz ? AFX_ALIGN(mgr->siz + sizeof(afxObjectBase), sizeof(void*)) : 0, AfxMax(1, mgr->unitsPerPage));
        AfxExitSlockExclusive(&mgr->poolLock);

        if ((mgr->mmu = cfg->mmu))
        {
            AfxAssertObjects(1, &mgr->mmu, afxFcc_MMU);

            if (AfxReacquireObjects(1, (void*[]) { mgr->mmu }))
                AfxThrowError();
        }
    }

    AfxSetUpChain(&mgr->supersets, mgr);
    AfxPushLinkage(&mgr->host, host ? host : &orphanClassChain);

    //AfxExitSlockExclusive(&mgr->slock);
    return err;
}

_AFX afxNat AfxGetObjectId(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* base = (afxObjectBase*)obj;
    --base;
    AfxAssertType(base, afxFcc_OBJ);
    
    afxPool* pool = &(AfxGetClass(obj)->pool);
    AfxAssertType(pool, afxFcc_POOL);
    afxNat idx = AFX_INVALID_INDEX;
    AfxFindPoolUnitIndex(pool, (afxByte*)base, &idx, NIL);
    return idx;
}

_AFX afxError AfxAcquireObjects(afxManager *mgr, afxNat cnt, afxObject objects[], void const* udd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(mgr, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(objects);

    AfxEnterSlockExclusive(&mgr->poolLock);

    if (mgr->pool.totalUsedCnt >= mgr->maxInstCnt)
        AfxThrowError();
    else
    {
        afxObjectBase* ptr, *ptr2;

        if (_AfxAllocateObjects(mgr, cnt, objects))
            AfxThrowError();
        else
        {
            if (_AfxConstructObjects(mgr, cnt, objects, (void**)udd))
            {
                AfxThrowError();
                _AfxDeallocateObjects(mgr, cnt, objects);
            }
            else
            {
                AfxAssertObjects(cnt, objects, mgr->objFcc);
            }
        }
    }
    
    AfxExitSlockExclusive(&mgr->poolLock);

    return err;
}

_AFX afxError AfxReacquireObjects(afxNat cnt, afxObject objects[])
{
    afxError err = NIL;
    AfxAssert(objects);
    AfxAssert(cnt);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxObject obj = objects[i];

        if (obj)
        {
            afxObjectBase* base = (afxObjectBase*)(obj);
            --base;
            AfxAssertType(base, afxFcc_OBJ);
            ++base->refCnt;
        }
    }
    return err;
}

_AFX afxBool AfxReleaseObjects(afxNat cnt, afxObject objects[])
{
    afxError err = NIL;
    AfxAssert(objects);
    AfxAssert(cnt);
    afxBool rslt = 0;

    for (afxNat i = cnt; i-- > 0;)
    {
        afxObject obj = objects[i];

        if (obj)
        {
            afxObjectBase* base = (afxObjectBase*)(obj);
            --base;
            AfxAssertType(base, afxFcc_OBJ);

            AfxCatchError(err);

            afxInt refCnt;

            if (0 == (refCnt = --base->refCnt))
            {
                ++rslt;
                afxManager* mgr = base->mgr;
                AfxAssertType(mgr, afxFcc_CLS);

                // se der erro aqui, é porque você provavelmente está passando uma array como elemento aninhado (ex.: AfxReleaseObjects(n, (void*[]){ array })) ao invés de passar a array diretamente (ex.: AfxReleaseObjects(n, array));

                AfxEnterSlockExclusive(&mgr->poolLock);

                _AfxDestructObjects(mgr, 1, &obj);
                _AfxDeallocateObjects(mgr, 1, &obj);

                AfxExitSlockExclusive(&mgr->poolLock);
                
                objects[i] = NIL;
            }
        }
    }
    return rslt;
}

_AFX afxBool _AfxAssertObjects(afxNat cnt, afxObject const objects[], afxFcc fcc)
{
    afxError err = NIL;
    AfxAssert(objects);
    AfxAssert(cnt);
    afxBool rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxObject obj = objects[i];

        if (obj)
        {
            afxObjectBase* base = obj;
            --base;
            AfxAssertType(base, afxFcc_OBJ);
            afxManager* mgr = base->mgr;
            AfxAssertType(mgr, afxFcc_CLS);

            do if (mgr->objFcc == fcc)
            {
                ++rslt;
                break;
            }
            while ((mgr = AfxGetSubmanager(mgr)));

            AfxAssertClass(mgr, fcc);
            AfxCatchError(err);
        }
    }
    return rslt;
}

_AFX afxResult AfxWaitForObject(afxObject obj, afxTime timeout)
{
    afxError err = NIL;
    AfxAssert(obj);
    return 0;
}
