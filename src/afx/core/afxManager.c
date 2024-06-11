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
            afxObjectBase *hdr = (afxObjectBase *)obj;
            --hdr;
            AfxAssertType(hdr, afxFcc_OBJ);
            AfxAssert(AfxGetManager(obj) == mgr);

            //afxFcc fcc = AfxGetObjectFcc(item);
            //afxNat refCnt = AfxGetRefCount(item);
            //AfxLogEcho("Destroying %s... %p[%.4s]^%i", mgr->name, item, (afxChar*)&fcc, item->refCnt);

            {
                afxEvent ev = { 0 };
                ev.id = AFX_EVENT_OBJ_DESTROYED;
                AfxNotifyObject(obj, &ev);

                afxEventFilter *flt;

                while (hdr->watching)
                {
                    afxLinkage *first = AfxGetLastLinkage(hdr->watching);

                    if (!first)
                    {
                        AfxAssert(!hdr->watching);
                        break;
                    }
                    else
                    {
                        flt = AFX_REBASE(first, afxEventFilter, holder);
                        AfxAssert(AfxGetLinker(&flt->holder) == obj);
                        //AfxAssertType((afxHandle*)flt->watched.chain->owner, afxFcc_OBJ);
                        AfxDeinstallWatcher(AfxGetLinker(&flt->watched), obj);
                    }
                }

                while (hdr->watchers)
                {
                    afxLinkage *first = AfxGetLastLinkage(hdr->watchers);

                    if (!first)
                    {
                        AfxAssert(!hdr->watchers);
                        break;
                    }
                    else
                    {
                        flt = AFX_REBASE(first, afxEventFilter, watched);
                        AfxAssert(AfxGetLinker(&flt->watched) == obj);
                        //AfxAssertType((afxHandle*)flt->holder.chain->owner, afxFcc_OBJ);
                        AfxDeinstallWatcher(obj, AfxGetLinker(&flt->holder));
                    }
                }
            }

            if (_AfxObjDtor(mgr, obj))
                AfxThrowError();

            AfxZero2(1, sizeof(*hdr), hdr);
            objects[i] = hdr;

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
        afxObjectBase* hdr = objects[i];
        AfxAssert(AfxIsAligned(hdr, sizeof(void*)));
        hdr->fcc = afxFcc_OBJ;
        hdr->mgr = mgr;

        AfxStoreAtom32(&hdr->refCnt, 1);

        hdr->tid = AfxGetTid();

        hdr->watchers = NIL;
        hdr->watching = NIL;
        hdr->event = mgr->defEvent;

        objects[i] = hdr + 1; // move the pointer forward

        AfxAssert(AfxIsAligned(objects[i], sizeof(void*)));
        AfxAssert(hdr == (((afxObjectBase*)(objects[i])) - 1));

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
            objects[i] = hdr - 1; // move the pointer backward
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

    if (mgr->siz)
    {
        if (objCnt)
        {
            AfxLogEcho("Anihilating %u lost <%s> objects.", objCnt, mgr->name);
        }
    }
    else
    {
        if (objCnt)
        {
            AfxLogEcho("%u lost <%s> objects not reachable.", objCnt, mgr->name);
        }
    }
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


    mgr->maxInstCnt = 0; // block new acquisitions.
    //AfxZero(mgr, sizeof(*mgr));

    //AfxLogEcho("");

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
    mgr->defEvent = cfg->event;
    mgr->defEventFilter = cfg->eventFilter;
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
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    
    afxPool* pool = &(AfxGetManager(obj)->pool);
    //AfxAssertType(pool, afxFcc_POOL);
    afxNat idx = AFX_INVALID_INDEX;
    AfxFindPoolUnitIndex(pool, (afxByte*)hdr, &idx, NIL);
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
            afxObjectBase* hdr = (afxObjectBase*)(obj);
            --hdr;
            AfxAssertType(hdr, afxFcc_OBJ);
            AfxIncAtom32(&hdr->refCnt);
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
            afxObjectBase* hdr = (afxObjectBase*)(obj);
            --hdr;
            AfxAssertType(hdr, afxFcc_OBJ);

            AfxCatchError(err);
            afxInt refCnt;

            if (0 == (refCnt = AfxDecAtom32(&hdr->refCnt)))
            {
                ++rslt;
                afxManager* mgr = hdr->mgr;
                AfxAssertType(mgr, afxFcc_CLS);

                // se der erro aqui, é porque você provavelmente está passando uma array como elemento aninhado (ex.: AfxReleaseObjects(n, (void*[]){ array })) ao invés de passar a array diretamente (ex.: AfxReleaseObjects(n, array));

                AfxEnterSlockExclusive(&mgr->poolLock);

                _AfxDestructObjects(mgr, 1, &obj);
                AfxAssert(obj == hdr);
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
            afxObjectBase* hdr = obj;
            --hdr;
            AfxAssertType(hdr, afxFcc_OBJ);
            afxManager* mgr = hdr->mgr;
            AfxAssertType(mgr, afxFcc_CLS);

            do if (mgr->objFcc == fcc) { ++rslt; break; }
            while ((mgr = AfxGetSubmanager(mgr)));

            AfxAssertClass(mgr, fcc);
            AfxCatchError(err);
        }
    }
    return rslt;
}

_AFX afxResult AfxWaitForObject(afxTime timeout, afxObject obj)
{
    afxError err = NIL;
    AfxAssert(obj);
    return 0;
}

#if 0
afxBool AfxFindManagedExtension(afxManager const* mgr, afxNat pluginID, afxNat* off, afxNat* siz)
{
    afxError err = NIL;
    AfxAssert(mgr);
    afxBool rslt = FALSE;

    afxManagedExtension* entry;
    afxChain const* entries = (afxChain const *)&mgr->anchor.lnk;
    AfxChainForEveryLinkageB2F(entries, afxManagedExtension, lnk, entry)
    {
        if (entry->fcc == pluginID)
        {
            *off = entry->off;
            *siz = entry->siz;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

RwInt32 AfxAddManagedExtension(afxManager const* mgr, afxNat pluginID, afxNat siz, void* ctor, void* dtor)
{
    RwPluginRegEntry *entry;
    RwInt32 newStructSize;

    AfxAssert(mgr);
    AfxAssert(mgr->instCnt == 0);

    /* add the registry to the list if freelists aren't used */
    if (RWSRCGLOBAL(memoryAlloc) != _rwFreeListAllocReal)
    {
        RwUInt32    i;

        for (i = 0; i < numRegToolkits; i += 1)
        {
            RWASSERT(NULL != toolkitNonFLRegList);

            if (reg == toolkitNonFLRegList[i])
            {
                break;
            }
        }

        if (numRegToolkits == i)
        {
            RwPluginRegistry    **newRegistryList;

            RwUInt32            j;


            newRegistryList = (RwPluginRegistry **)RwMalloc(sizeof(RwPluginRegistry *) * (numRegToolkits + 1),
                rwMEMHINTDUR_GLOBAL);

            j = 0;
            if (NULL != toolkitNonFLRegList)
            {
                for (; j < numRegToolkits; j += 1)
                {
                    newRegistryList[j] = toolkitNonFLRegList[j];
                }

                RwFree(toolkitNonFLRegList);
                toolkitNonFLRegList = (RwPluginRegistry **)NULL;
            }

            newRegistryList[j] = reg;

            numRegToolkits += 1;

            toolkitNonFLRegList = newRegistryList;
        }
    }

    /* Make sure it's not in the list yet */
    entry = reg->firstRegEntry;

    while (entry)
    {
        if (entry->pluginID == pluginID)
        {
            /* Can't register twice, return the already registered offset */

#ifdef RWDEBUG
            {
                RwChar  msg[80];

                sprintf(msg, "Plugin ,Vendor ID = 0x%02x, Object ID = 0x%02x, already registered.\n",
                    GETVENDORID(pluginID), GETOBJECTID(pluginID));

                RWMESSAGE(("%s", msg));
            }
#endif /* RWDEBUG */

            RWERROR((E_RW_PLUGININIT));
            RWRETURN(entry->offset);
        }
        entry = entry->nextRegEntry;
    }

    /* Increase structure size, but keep longword alignment */
#ifdef RWDEBUG
    /* Allow for a longword front and back for stomp detection */
    newStructSize = reg->sizeOfStruct + ((siz + (sizeof(afxNat32) * 2) + 3) & ~3);
#else /* RWDEBUG */
    newStructSize = reg->sizeOfStruct + ((siz + 3) & ~3);
#endif /* RWDEBUG */

    if (reg->maxSizeOfStruct && (newStructSize > reg->maxSizeOfStruct))
    {
        /* We've exceeded our limit, so fail */
        RWRETURN(-1);
    }

    entry = (RwPluginRegEntry *)RwFreeListAlloc(toolkitRegEntries, rwMEMHINTDUR_GLOBAL);
    RWASSERT(RWFREELISTALIGNED(entry, toolkitRegEntries));

    if (entry)
    {
        /* Fill it the entry */

#ifdef RWDEBUG
        /* Allow for a longword front and back for stomp detection */
        entry->offset = reg->sizeOfStruct + sizeof(afxNat32);
#else /* RWDEBUG */
        entry->offset = reg->sizeOfStruct;
#endif /* RWDEBUG */

        reg->sizeOfStruct = newStructSize;

        entry->size = siz;
        entry->pluginID = pluginID;
        entry->readCB = (RwPluginDataChunkReadCallBack)NULL;
        entry->writeCB = (RwPluginDataChunkWriteCallBack)NULL;
        entry->getSizeCB = (RwPluginDataChunkGetSizeCallBack)NULL;
        entry->alwaysCB = (RwPluginDataChunkAlwaysCallBack)NULL;
        entry->rightsCB = (RwPluginDataChunkRightsCallBack)NULL;
        entry->constructCB = constructCB ? constructCB : PluginDefaultConstructor;
        entry->destructCB = destructCB ? destructCB : PluginDefaultDestructor;
        entry->copyCB = copyCB ? copyCB : PluginDefaultCopy;
        entry->errStrCB = (RwPluginErrorStrCallBack)NULL;
        entry->nextRegEntry = (RwPluginRegEntry *)NULL;
        entry->prevRegEntry = (RwPluginRegEntry *)NULL;

        /* Point back to the parent */
        entry->parentRegistry = reg;

        /* Append it to the list */
        if (reg->firstRegEntry == NULL)
        {
            /* First in the list, just shove it in */
            reg->firstRegEntry = entry;
            reg->lastRegEntry = entry;
        }
        else
        {
            /* Whack it on the end */
            reg->lastRegEntry->nextRegEntry = entry;
            entry->prevRegEntry = reg->lastRegEntry;

            /* New end of the list */
            reg->lastRegEntry = entry;
        }

        RWRETURN(entry->offset);
    }

    /* Failed to allocate a freelist entry */
    RWRETURN(-1);
}
#endif
