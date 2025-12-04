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

#define _AFX_MANAGER_C
#include "../exec/afxSystemDDK.h"

#define OBJ_HDR_SIZ     AFX_ALIGN_SIZE(sizeof(afxObjectBase), AFX_SIMD_ALIGNMENT)
#define GET_OBJ_HDR(obj_) ((void*)(((afxByte*)obj_) - OBJ_HDR_SIZ))
#define GET_OBJ_ADDR(hdr_) ((void*)(((afxByte*)hdr_) + OBJ_HDR_SIZ))

_AFX afxBool metaObjectMgrInited = FALSE;
_AFX afxChain orphanClassChain =
{
    {
        .next = &(orphanClassChain.anchor),
        .prev = &(orphanClassChain.anchor),
        .chain = &(orphanClassChain)
    },
    { NIL },
    0
};

_AFX afxChain* _AfxGetOrphanClasses(void)
{
    return &orphanClassChain;
}

_AFX afxResult AfxExhaustChainedClasses(afxChain *provisions)
{
    afxError err = { 0 };
    afxResult cnt = 0;

    afxClass* cls;
    AFX_ITERATE_CHAIN(cls, host, provisions)
    {
        AFX_ASSERT(cls->fcc == afxFcc_CLS);
        
        if (cls->objFcc == afxFcc_SYS)
            break; // We can't drop it, it would fuck the entire platform. Break.
            
        cnt += AfxExhaustClass(cls);
    }
    return cnt;
}

_AFX afxResult AfxDeregisterChainedClasses(afxChain *provisions)
{
    afxError err = { 0 };
    afxResult cnt = 0;

#if !0
    afxClass* cls;
    AFX_ITERATE_CHAIN(cls, host, provisions)
    {
        AFX_ASSERT(cls->fcc == afxFcc_CLS);
        ++cnt;

        if (cls->objFcc != afxFcc_SYS) AfxDismountClass(cls);
        else
        {
            AFX_ASSERT(cls->maxInstCnt == 1);
            //AFX_ASSERT(cls->pool.totalUsedCnt == 1);
            break; // has only the SYS. We can't drop it else we will fuck the entire platform. Break.
        }
    }
#else
    while (1)
    {
        afxLink *first = AfxGetLastLink(provisions);

        if (!first || first == &provisions->anchor) break;
        else
        {
            afxClass *cls = AFX_REBASE(first, afxClass, host);
            AFX_ASSERT(cls->fcc == afxFcc_CLS);
            ++cnt;

            if (cls->objFcc != afxFcc_SYS) AfxDismountClass(cls);
            else
            {
                AFX_ASSERT(cls->maxInstCnt == 1);
                //AFX_ASSERT(cls->pool.totalUsedCnt == 1);
                break; // has only the SYS. We can't drop it else we will fuck the entire platform. Break.
            }                
        }
    }
#endif
    return cnt;
}

_AFX afxError _AfxUnlockClass(afxClass* cls, afxClassLock mode)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    afxBool shared = (mode & afxClassLock_SHARED);
    AfxUnlockFutex((void*)&cls->poolLock, shared);
    return err;
}

_AFX afxError _AfxLockClass(afxClass* cls, afxClassLock mode)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);

    afxBool shared = (mode & afxClassLock_SHARED);

    if ((mode & afxClassLock_TRY))
        AfxTryLockFutex((void*)&cls->poolLock, shared);
    else
        AfxLockFutex((void*)&cls->poolLock, shared);

    return err;
}

_AFX afxError AfxUnlockClass(afxClass* cls, afxClassLock mode)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    return _AfxUnlockClass(cls, mode);
}

_AFX afxError AfxLockClass(afxClass* cls, afxClassLock mode)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    return _AfxLockClass(cls, mode);
}

_AFXINL afxArena* AfxGetClassArena(afxClass *cls)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    return &cls->arena;
}

_AFXINL afxClass* AfxGetSubclass(afxClass const *cls)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    return AfxGetLinker(&cls->subset);
}

_AFX afxUnit AfxGetSizeOfObject(afxClass const* cls, afxUnit* strictSize)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    
    if (strictSize)
    {
        afxClass const* subset = AfxGetSubclass(cls);
        *strictSize = (subset ? cls->fixedSiz - subset->fixedSiz : cls->fixedSiz);
    }
    return cls->fixedSiz;
}

_AFX afxUnit _AfxEnumerateObjectsUnlocked(afxClass const* cls, afxBool fromLast, afxUnit first, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    afxUnit rslt = 0;

    if (first && (first >= cls->pool.totalUsedCnt))
    {
        first -= cls->pool.totalUsedCnt;
    }
    else
    {
        rslt = cnt ? AfxEnumeratePoolItems(&cls->pool, first, cnt, (void**)objects) : 0;

        if (rslt)
        {
            AFX_ASSERT(cnt >= rslt);

            for (afxUnit i = 0; i < rslt; i++)
            {
                afxObjectBase* hdr = objects[i];
                AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
                objects[i] = GET_OBJ_ADDR(hdr);
                AFX_ASSERT_OBJECTS(cls->objFcc, 1, &objects[i]);
            }

            if (first && (first >= rslt))
                first -= rslt;

            cnt = cnt - rslt;
        }
    }

    if (cnt)
    {
        afxClass *superset;
        afxChain const *supersets = &cls->supersets;
        AFX_ITERATE_CHAIN_B2F(superset, subset, supersets)
        {
            AFX_ASSERT(superset->fcc == afxFcc_CLS);

            if (first && (first >= superset->pool.totalUsedCnt))
            {
                first -= superset->pool.totalUsedCnt;
            }
            else
            {
#if !0
                afxUnit rslt2 = _AfxEnumerateObjectsUnlocked(superset, fromLast, first, cnt, &objects[rslt]);

                AFX_ASSERT(cnt >= rslt2); // cant be greater

                if (first && (first >= rslt2))
                    first -= rslt2;

                cnt = cnt - rslt2;
                rslt += rslt2;

#else
                afxUnit rslt2 = _AfxEnumerateObjectsUnlocked(superset, fromLast, first2, cnt2, &objects[rslt]);

                if (rslt2)
                {
                    AFX_ASSERT(cnt2 >= rslt2);
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

_AFX afxUnit _AfxEvokeObjectsUnlocked(afxClass const* cls, afxBool fromLast, afxBool(*flt)(afxObject, void*), void* fdd, afxUnit first, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    afxUnit rslt = 0;

    if (first && (first >= cls->pool.totalUsedCnt))
    {
        first -= cls->pool.totalUsedCnt;
    }
    else
    {
        afxFcc objFcc = cls->objFcc;
        afxObjectBase* baseObj;
        afxUnit j = first;

        while (AfxEnumeratePoolItems(&cls->pool, j++, 1, (void**)&baseObj))
        {
            AFX_ASSERT(baseObj->fcc == afxFcc_OBJ);
            afxObject obj = GET_OBJ_ADDR(baseObj);
            AFX_ASSERT_OBJECTS(objFcc, 1, &obj);

            if (flt(obj, fdd))
            {
                objects[rslt] = obj;
                ++rslt;

                if (rslt == cnt)
                    break;
            }
        }

        if (first && (first >= rslt))
            first -= rslt;

        cnt = cnt - rslt;
    }

    if (cnt)
    {
        afxClass *superset;
        afxChain const *supersets = &cls->supersets;
        AFX_ITERATE_CHAIN_B2F(superset, subset, supersets)
        {
            AFX_ASSERT(superset->fcc == afxFcc_CLS);

            if (first && (first >= superset->pool.totalUsedCnt))
            {
                first -= superset->pool.totalUsedCnt;
            }
            else
            {
#if !0
                afxUnit rslt2 = _AfxEvokeObjectsUnlocked(superset, fromLast, flt, fdd, first, cnt, &objects[rslt]);

                AFX_ASSERT(cnt >= rslt2); // cant be greater

                if (first && (first >= rslt2))
                    first -= rslt2;

                cnt = cnt - rslt2;
                rslt += rslt2;

#else
                afxUnit rslt2 = _AfxEnumerateObjectsUnlocked(superset, fromLast, first2, cnt2, &objects[rslt]);

                if (rslt2)
                {
                    AFX_ASSERT(cnt2 >= rslt2);
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

_AFX afxUnit AfxEnumerateObjects(afxClass const* cls, afxUnit first, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(!cnt || objects);
    afxUnit instCnt = 0;

    if (!objects)
    {
        afxChain const *supersets = &cls->supersets;
        AFX_ITERATE_CHAIN_B2F(cls, subset, supersets)
        {
            AFX_ASSERT(cls->fcc == afxFcc_CLS);
            instCnt += cls->instCnt;
        }
    }
    else if (cls->instCnt)
    {
        AfxLockClass(cls, afxClassLock_SHARED);
        instCnt = _AfxEnumerateObjectsUnlocked(cls, FALSE, first, cnt ? cnt : cls->instCnt, objects);
        AfxUnlockClass(cls, afxClassLock_SHARED);
    }
    return instCnt;
}

_AFX afxUnit AfxEvokeObjects(afxClass const* cls, afxBool(*f)(afxObject, void*), void* udd, afxUnit first, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    //AFX_ASSERT(cnt);
    afxUnit rslt = 0;

    if (cls->instCnt)
    {
        AfxLockClass(cls, afxClassLock_SHARED);
        rslt = _AfxEvokeObjectsUnlocked(cls, FALSE, f, udd, first, cnt ? cnt : cls->instCnt, objects);
        AfxUnlockClass(cls, afxClassLock_SHARED);
    }
    return rslt;
}

_AFX afxUnit _AfxInvokeObjectsUnlocked(afxClass const* cls, afxBool fromLast, afxBool(*f)(afxObject,void*), void* udd, afxBool(*f2)(afxObject,void*), void* udd2, afxUnit first, afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(f2);
    afxUnit rslt = 0;
    afxUnit i = first;
    afxObject obj;
    
    if (!f)
    {
        while (_AfxEnumerateObjectsUnlocked(cls, fromLast, i++, 1, &obj))
        {
            AFX_ASSERT_OBJECTS(cls->objFcc, 1, &obj);
            ++rslt;

            if (!f2(obj, udd2)) // if secondary callback doesn't feed back, break the iteration.
                break;

            if (++rslt == cnt) // if cnt has been reached, break the iteration.
                break;
        }
    }
    else
    {
        while (_AfxEnumerateObjectsUnlocked(cls, fromLast, i++, 1, &obj))
        {
            AFX_ASSERT_OBJECTS(cls->objFcc, 1, &obj);

            if (f(obj, udd)) // pass only if filter has fed.
            {
                ++rslt;

                if (!f2(obj, udd2)) // if secondary callback don't feed back, break the iteration.
                    break;

                if (++rslt == cnt) // if cnt has been reached, break the iteration.
                    break;
            }
        }
    }
    return rslt;
}

_AFX afxUnit AfxInvokeObjects2(afxClass const* cls, afxBool(*f)(afxObject, void*), void* udd, afxBool(*f2)(afxObject, void*), void* udd2, afxUnit first, afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    //AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    AFX_ASSERT(f2);
    afxUnit rslt = 0;

    if (cls->instCnt)
    {
        AfxLockClass(cls, afxClassLock_SHARED);
        rslt = _AfxInvokeObjectsUnlocked(cls, FALSE, f, udd, f2, udd2, first, cnt ? cnt : cls->instCnt);
        AfxUnlockClass(cls, afxClassLock_SHARED);
    }
    return rslt;
}

_AFX afxUnit AfxInvokeObjects(afxClass const* cls, afxBool(*f)(afxObject obj, void *udd), void *udd, afxUnit first, afxUnit cnt)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    //AFX_ASSERT(cnt);
    AFX_ASSERT(f);
    afxUnit rslt = 0;
    
    if (cls->instCnt)
    {
        AfxLockClass(cls, afxClassLock_SHARED);
        rslt = _AfxInvokeObjectsUnlocked(cls, FALSE, NIL, NIL, f, udd, first, cnt ? cnt : cls->instCnt);
        AfxUnlockClass(cls, afxClassLock_SHARED);
    }
    return rslt;
}

_AFXINL afxBool AfxFindClassPluginSegment(afxClass const* cls, afxUnit extId, afxUnit* off, afxUnit* siz)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    afxBool rslt = FALSE;

    afxClassExtension* ext;
    afxChain const* extensions = &cls->extensions;
    AFX_ITERATE_CHAIN_B2F(ext, cls, extensions)
    {
        if (ext->extId == extId)
        {
            *off = ext->objOff;
            *siz = ext->objSiz;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFXINL void* AfxGetObjectExtra(afxObject obj, afxUnit pluginId)
{
    afxError err = { 0 };
    afxObjectBase* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    afxClass* cls = hdr->cls;
    AFX_ASSERT(cls->fcc == afxFcc_CLS);

    afxUnit off = 0, siz = 0;
    void* p = NIL;

    if (hdr->extra && AfxFindClassPluginSegment(cls, pluginId, &off, &siz))
    {
        p = &hdr->extra[off];
    }
    return p;
}

_AFXINL void* AfxGetObjectIdd(afxObject obj, afxUnit offset)
{
    afxError err = { 0 };
    afxObjectBase const* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    afxClass const* cls = hdr->cls;
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    return hdr->extra ? &hdr->extra[offset] : NIL;
}

_AFX afxError _AfxDeallocateObjects(afxClass *cls, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(objects);
    AFX_ASSERT(cnt);
    
    if (cls->pool.unitSiz == 0)
        return err;

#if !0
    AfxReclaimPoolUnits(&cls->pool, AfxHere(), cnt, (void**)objects);
#else
    for (afxUnit i = 0; i < cnt; i++)
    {
        afxObjectBase *item = (afxObjectBase *)obj[i];
        AFX_ASSERT(item);
        --item;
        AfxPopPoolUnit(&cls->pool, item);
    }
#endif
    return err;
}

_AFX afxError _AfxAllocateObjects(afxClass *cls, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(cnt);

    afxPool* pool = &cls->pool;

    if (pool->unitSiz == 0)
    {
        AfxThrowError();
        return err;
    }

    afxUnit room = (cls->maxInstCnt - pool->totalUsedCnt);

    if ((room == 0) || (cnt > room))
    {
        AfxThrowError();
        return err;
    }

#if 0 // We should make a pool method for it.
    if (pool->totalUsedCnt == 0)
    {
        AfxExhaustPool(pool, FALSE);
        pool->unitsPerPage = cls->unitsPerPage ? cls->unitsPerPage : cnt;
    }
#endif

    if (AfxRequestPoolUnits(pool, AfxHere(), cnt, NIL, (void**)objects))
        AfxThrowError();

    return err;
}

_AFX afxError _AfxAllocateClassInstancesAt(afxClass *cls, afxUnit subset, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(cnt);

    afxPool* pool = &cls->pool;
    afxUnit room = (cls->maxInstCnt - pool->totalUsedCnt);

    if ((room == 0) || (cnt > room)) AfxThrowError();
    else
    {
#if 0 // We should make a pool method for it.
        if (pool->totalUsedCnt == 0)
        {
            AfxExhaustPool(pool, FALSE);
            pool->unitsPerPage = cls->unitsPerPage ? cls->unitsPerPage : cnt;
        }
#endif

        for (afxUnit i = 0; i < cnt; i++)
        {
            objects[i] = NIL;

            if (AfxGetPoolUnit(pool, subset + i, NIL)) AfxThrowError();
            else
            {
                if (AfxTakePoolUnit(pool, subset + i, NIL)) AfxThrowError();
                else
                {
                    AFX_ASSERT(AfxGetPoolUnit(pool, subset + i, (void**)&objects[i]));
                }
            }

            if (err)
            {
                _AfxDeallocateObjects(cls, i, objects);
            }
        }
    }
    return err;
}

_AFX afxError _AfxAllocateClassInstancesAt2(afxClass *cls, afxUnit first, afxUnit cnt, afxUnit const idMap[], afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(cnt);

    afxPool* pool = &cls->pool;
    afxUnit room = (cls->maxInstCnt - pool->totalUsedCnt);

    if ((room == 0) || (cnt > room)) AfxThrowError();
    else
    {

#if 0 // We should make a pool method for it.
        if (pool->totalUsedCnt == 0)
        {
            AfxExhaustPool(pool, FALSE);
            pool->unitsPerPage = cls->unitsPerPage ? cls->unitsPerPage : cnt;
        }
#endif

        for (afxUnit i = 0; i < cnt; i++)
        {
            objects[i] = NIL;

            afxUnit atId = idMap ? first + idMap[i] : first + i;

            if (AfxGetPoolUnit(pool, atId, NIL)) AfxThrowError();
            else
            {
                if (AfxTakePoolUnit(pool, atId, NIL)) AfxThrowError();
                else
                {
                    AFX_ASSERT(AfxGetPoolUnit(pool, atId, (void**)&objects[i]));
                }
            }

            if (err)
            {
                _AfxDeallocateObjects(cls, i, objects);
            }
        }
    }
    return err;
}

_AFX afxError _AfxClsObjDtor(afxClass *cls, afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    afxClass* subset = AfxGetSubclass(cls);

    if (cls->dtor && cls->dtor(obj))
    {
        AfxThrowError();

        if (subset)
            AfxReportError("afxClass<%s, %s>(%p)", cls->name, subset->name, obj);
        else
            AfxReportError("afxClass<%s>(%p)", cls->name, obj);
    }

    if (subset && _AfxClsObjDtor(subset, obj))
    {
        AfxThrowError();
        AfxReportError("afxClass<%s, %s>(%p)", cls->name, subset->name, obj);
    }

    return err;
}

_AFX afxError _AfxClsObjCtor(afxClass *cls, afxObject obj, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    afxClass* subset = AfxGetSubclass(cls);

    if (subset && _AfxClsObjCtor(subset, obj, args, invokeNo)) AfxThrowError();
    else
    {
        afxError err2;

        if (cls->ctor && (err2 = cls->ctor(obj, args, invokeNo)))
        {
            AfxThrowError();

            if (subset)
            {
                AfxReportError("afxClass<%s, %s>(%p)", cls->name, subset->name, obj);
                _AfxClsObjDtor(subset, obj);
            }
            else AfxReportError("afxClass<%s>(%p)", cls->name, obj);
        }
    }
    return err;
}

_AFX afxError _AfxCleanUpObjects(afxClass *cls, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(objects);
    AFX_ASSERT(cnt);
 
    for (afxUnit i = cnt; i-- > 0;)
    {
        afxObject obj = objects[i];

        if (!obj)
            continue;

        --cls->instCnt;
        //AfxReportMessage("Dismantling %s object (#%d) %p...", cls->name, i, obj[i]);
        afxObjectBase *hdr = GET_OBJ_HDR(obj);
        AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
        AFX_ASSERT(AfxGetClass(obj) == cls);

        //afxFcc fcc = AfxGetObjectFcc(item);
        //afxUnit refCnt = AfxGetRefCount(item);
        //AfxReportMessage("Destroying %s... %p[%.4s]^%i", cls->name, item, (afxChar*)&fcc, item->refCnt);

        {
            afxEvent ev = { 0 };
            ev.id = AFX_EVENT_OBJ_DESTROYED;
            AfxNotifyObject(obj, &ev);

            afxEventFilter *flt;

            while (hdr->watching)
            {
                afxLink *first = AfxGetLastLink(hdr->watching);

                if (!first)
                {
                    AFX_ASSERT(!hdr->watching);
                    break;
                }
                else
                {
                    flt = AFX_REBASE(first, afxEventFilter, holder);
                    AFX_ASSERT(AfxGetLinker(&flt->holder) == obj);
                    //AfxAssertType((afxHandle*)flt->watched.chain->owner, afxFcc_OBJ);
                    AfxDisconnectObjects(obj, flt->fn, 1, (afxObject[]) { AfxGetLinker(&flt->watched) });
                }
            }

            while (hdr->watchers)
            {
                afxLink *first = AfxGetLastLink(hdr->watchers);

                if (!first)
                {
                    AFX_ASSERT(!hdr->watchers);
                    break;
                }
                else
                {
                    flt = AFX_REBASE(first, afxEventFilter, watched);
                    AFX_ASSERT(AfxGetLinker(&flt->watched) == obj);
                    //AfxAssertType((afxHandle*)flt->holder.chain->owner, afxFcc_OBJ);
                    AfxDisconnectObjects(AfxGetLinker(&flt->holder), flt->fn, 1, (afxObject[]) { obj });
                }
            }
        }

        afxClassExtension* ext;
        afxChain const* extensions = &cls->extensions;
        AFX_ITERATE_CHAIN(ext, cls, extensions)
        {
            if (ext->dtorCb && ext->dtorCb(obj, AfxGetObjectExtra(obj, ext->extId), ext->objSiz))
                AfxThrowError();
        }

        if (_AfxClsObjDtor(cls, obj))
            AfxThrowError();

        AfxZero(hdr, AFX_ALIGN_SIZE(sizeof(afxObjectBase), AFX_SIMD_ALIGNMENT));
        objects[i] = hdr;

        //AfxDeallocate(cls->all, item);

        //AfxReportMessage("Instance %p[%.4s]^%i destroyed.", item, (afxChar const*)&(fcc), refCnt);
    }
    return err;
}

_AFX afxError _AfxSetUpObjects(afxClass *cls, afxUnit cnt, afxObject objects[], void** udd)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(cnt);
    AFX_ASSERT(objects);
    //AFX_ASSERT(blueprint);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxObject obj = objects[i];

        if (!obj)
            continue;

        afxObjectBase* hdr = (void*)obj;
        AFX_ASSERT(AFX_TEST_ALIGNMENT(hdr, AFX_SIMD_ALIGNMENT));
        
        hdr->fcc = afxFcc_OBJ;
        hdr->cls = cls;

        AfxStoreAtom32(&hdr->refCnt, 1);
        //hdr->uniqueInc = ++cls->uniqueInc;

        hdr->tid = AfxGetTid();

        hdr->watchers = NIL;
        hdr->watching = NIL;
        hdr->event = cls->defEvent;

        // Debugging
        if (cls->pool.totalUsedCnt)
        {
            AfxFindPoolUnit(&cls->pool, hdr, &hdr->instIdx, NIL);
        }

        if (err)
        {
            int a = 1;
        }

        obj = GET_OBJ_ADDR(hdr);
        AFX_ASSERT(AFX_TEST_ALIGNMENT(obj, AFX_SIMD_ALIGNMENT));
        objects[i] = obj;// hdr + 1; // move the pointer forward

        AFX_ASSERT(AFX_TEST_ALIGNMENT(objects[i], AFX_SIMD_ALIGNMENT));
        
        if (err)
        {
            int a = 1;
        }

        hdr->extra = NIL;

        if (cls->extraSiz)
            if (!(hdr->extra = AfxRequestArena(&cls->extraAlloc, cls->extraSiz, 1, NIL, 0)))
                AfxThrowError();

        if (_AfxClsObjCtor(cls, objects[i], udd, i))
        {
            AfxThrowError();
        }
        else
        {
            afxClassExtension* ext;
            afxChain const* extensions = &cls->extensions;
            AFX_ITERATE_CHAIN_B2F(ext, cls, extensions)
            {
                if (ext->ctorCb && ext->ctorCb(objects[i], AfxGetObjectExtra(objects[i], ext->extId), ext->objSiz))
                    AfxThrowError();
            }

            ++cls->instCnt;
        }

        if (err && hdr->extra)
            AfxReclaimArena(&cls->extraAlloc, hdr->extra, cls->extraSiz);

        if (err)
        {
            for (afxUnit j = i; j-- > 0;)
            {
                afxObject obj2 = objects[j];
                _AfxClsObjDtor(cls, obj2);
                afxObjectBase* subset = GET_OBJ_HDR(obj2);
                objects[j] = subset; // move the pointer backward
            }
            objects[i] = /*hdr - 1*/GET_OBJ_HDR(obj); // move the pointer backward
        }

        if (err)
            break;
    }
    return err;
}

_AFX afxUnit AfxExhaustClass(afxClass *cls)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    afxUnit rslt = 0;

    if (cls->fixedSiz)
    {
        afxObject obj;
        while (_AfxEnumerateObjectsUnlocked(cls, TRUE, 0, 1, &obj))
        {
            while (TRUE != AfxDisposeObjects(AfxHere(), 1, &obj));
            ++rslt;
        }
    }
    return rslt;
}

_AFX afxError AfxDismountClass(afxClass *cls)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);

    if (!cls->fcc)
        return err;

    AFX_ASSERT(cls->fcc == afxFcc_CLS);

    afxUnit objCnt = AfxEnumerateObjects(cls, 0, 0, NIL);
    afxClass* subset = AfxGetSubclass(cls);

    if (cls->fixedSiz)
    {
        if (objCnt)
        {
            AfxReportMessage("Annihilating %u lost <%s> objects.", objCnt, cls->name);
        }
    }
    else
    {
        if (objCnt)
        {
            AfxReportMessage("%u lost <%s> objects not reachable.", objCnt, cls->name);
        }
    }
    //AfxLockFutex(&cls->slock);

    AfxDeregisterChainedClasses(&cls->supersets);
    AFX_ASSERT(0 == AfxGetChainLength(&cls->supersets));

    AfxExhaustClass(cls);

    if (cls->fixedSiz) // only if dynamically allocated
    {
        AFX_ASSERT(0 == cls->pool.totalUsedCnt);        
        AfxLockClass(cls, NIL);
        AfxExhaustPool(&cls->pool, FALSE);
        AfxUnlockClass(cls, NIL);
        AfxCleanUpFutex(&cls->poolLock);

        if (cls->mmu)
            AfxDisposeObjects(AfxHere(), 1, (void*[]) { cls->mmu });
    }

    AfxDismantleArena(&cls->arena);

    afxClassExtension* ext;
    afxChain const* extensions = &cls->extensions;
    AFX_ITERATE_CHAIN_B2F(ext, cls, extensions)
    {
        AfxPopLink(&ext->cls);
    }

    if (cls->extraSiz)
    {
        AfxDismantleArena(&cls->extraAlloc);
        cls->extraSiz = 0;
    }

    if (cls->subset.chain)
        AfxPopLink(&cls->subset);

    if (cls->host.chain)
        AfxPopLink(&cls->host);


    cls->maxInstCnt = 0; // block new acquisitions.
    //AfxZero(cls, sizeof(*cls));

    //AfxReportMessage("");

    return err;
}

_AFXINL afxError _AfxDummyObjDtor(afxObject obj)
{
    afxError err = { 0 };
    (void)obj;
    return err;
}

_AFXINL afxError _AfxDummyObjCtor(afxObject obj, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    (void)obj;
    (void)args;
    return err;
}

_AFX afxError AfxMountClass(afxClass* cls, afxClass* subset, afxChain* host, afxClassConfig const* cfg)
{
    afxError err = { 0 };
    AFX_ASSERT(cls);
    AFX_ASSERT(cfg);


    if (cls->fcc == afxFcc_CLS)
    {
        AfxDismountClass(cls);
    }

    AfxZero(cls, sizeof(*cls));

    if (host)
    {
        if (subset)
        {
            //AfxReportMessage("Registering... %s[%s]", cfg->name, subset->name);
        }
        else
        {
            //AfxReportMessage("Registering... %s", cfg->name);
        }
    }
    else
    {
        if (subset)
        {
            //AfxReportMessage("Registering... %s[%s]", cfg->name, subset->name);
        }
        else
        {
            //AfxReportMessage("Registering... %s", cfg->name);
        }
    }

    //AfxSetUpFutex(&cls->slock);
    //AfxLockFutex(&cls->slock);

    cls->fcc = afxFcc_CLS;
    AFX_ASSERT(cfg->fcc);
    cls->objFcc = cfg->fcc;
    //AFX_ASSERT(cfg->size);
    cls->fixedSiz = cfg->fixedSiz;

    if (!subset)
    {
        AfxPushLink(&cls->subset, NIL);
        cls->level = _AFX_CLASS_BASE_LEVEL;
    }
    else
    {
        AFX_ASSERT(subset != cls);
        AFX_ASSERT(subset->fcc == afxFcc_CLS);
        AFX_ASSERT(cfg->fixedSiz >= subset->fixedSiz);
        AfxPushLink(&cls->subset, &subset->supersets);
        cls->level = subset->level + 1;
    }

    cls->levelMask = (_AFX_CLASS_LEVEL_MASK << cls->level);
    cls->maxInstCnt = cfg->maxCnt ? cfg->maxCnt : AFX_U32_MAX;
    cls->ctor = cfg->ctor ? cfg->ctor : _AfxDummyObjCtor;
    cls->dtor = cfg->dtor ? cfg->dtor : _AfxDummyObjDtor;
    AFX_ASSERT(cfg->name && cfg->name[0]);
    AfxStrcpy(cls->name, cfg->name ? cfg->name : "");
    cls->defEvent = cfg->event;
    cls->defEventFilter = cfg->eventFilter;
    cls->vmt = cfg->vmt;
    cls->unitsPerPage = AFX_MIN(cfg->unitsPerPage, 32);
    cls->uniqueInc = 0;

    if (cls->fixedSiz) // only if dynamically allocated
    {
        AFX_ASSERT(cls->fixedSiz > sizeof(afxObject)); // just to avoid non-open structure being passed in.

        afxUnit unitSizAligned = AFX_ALIGN_SIZE(cls->fixedSiz + OBJ_HDR_SIZ, AFX_POOL_ALIGNMENT);

        AfxSetUpFutex(&cls->poolLock);
        AfxLockFutex(&cls->poolLock, FALSE);
        AfxSetUpPool(&cls->pool, cls->fixedSiz ? unitSizAligned : 0, AFX_MAX(1, cls->unitsPerPage), AFX_POOL_ALIGNMENT);
        AfxUnlockFutex(&cls->poolLock, FALSE);

        AFX_ASSERT(cls->pool.unitSiz >= unitSizAligned);

        if ((cls->mmu = cfg->mmu))
        {
            AFX_ASSERT_OBJECTS(afxFcc_MMU, 1, &cfg->mmu);

            if (AfxReacquireObjects(AfxHere(), 1, (void*[]) { cls->mmu }))
                AfxThrowError();
        }
    }
    
    if (AfxMakeArena(&cls->arena, NIL, AfxHere()))
        AfxThrowError();

    AfxMakeChain(&cls->supersets, cls);
    //AfxPushLink(&cls->host, host ? host : &orphanClassChain);
    AfxLinkAhead(&cls->host, host ? &host->anchor : &orphanClassChain.anchor);

    AfxMakeChain(&cls->extensions, cls);
    cls->extraSiz = 0;

    //AfxUnlockFutex(&cls->slock);
    return err;
}

_AFX afxUnit AfxGetObjectId(afxObject obj)
{
    afxError err = { 0 };
    afxObjectBase* hdr = GET_OBJ_HDR(obj);
    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
    
    afxPool* pool = &(AfxGetClass(obj)->pool);
    //AfxAssertType(pool, afxFcc_POOL);
    afxUnit idx = AFX_INVALID_INDEX;
    afxBool found = AfxFindPoolUnit(pool, (afxByte*)hdr, &idx, NIL);
    AFX_ASSERT(hdr->instIdx == idx);
    return found ? idx : AFX_INVALID_INDEX;
}

_AFX afxError AfxAcquireObjects(afxClass *cls, afxUnit cnt, afxObject objects[], void const* udd[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(cnt);
    AFX_ASSERT(objects);

    AfxLockClass(cls, NIL);

    afxPool* pool = &cls->pool;

    if (pool->totalUsedCnt >= cls->maxInstCnt)
    {
        AfxThrowError();
    }
    else
    {
        // If an object is statically allocated, check for the each container presence and if it is SIMD aligned.
        if (pool->unitSiz == 0)
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                if (!objects[i])
                    AfxThrowError();

                if (!AFX_TEST_ALIGNMENT(objects[i], AFX_SIMD_ALIGNMENT))
                    AfxThrowError();
            }
        }
        else
        {
            if (_AfxAllocateObjects(cls, cnt, objects))
                AfxThrowError();
        }
        
        if (!err)
        {
            if (_AfxSetUpObjects(cls, cnt, objects, (void**)udd))
            {
                AfxThrowError();
                _AfxDeallocateObjects(cls, cnt, objects);
            }
            else
            {
                AFX_ASSERT_OBJECTS(cls->objFcc, cnt, objects);

                for (afxUnit i = 0; i < cnt; i++)
                {
                    afxObject obj = objects[i];

                    if (!obj)
                        continue;

                    afxObjectBase* hdr = GET_OBJ_HDR(obj);
                    AFX_ASSERT(hdr->fcc == afxFcc_OBJ);

                    if (cls->dbgAcq)
                    {
                        // Debugging
                        AfxReportComment("Acquired &%p#%u,%.4s", obj, hdr->instIdx, &hdr->cls->objFcc);
                    }
                }
            }
        }
    }
    
    AfxUnlockClass(cls, NIL);

    return err;
}

_AFX afxError AfxAcquireObjects2(afxClass *cls, afxUnit first, afxUnit cnt, afxUnit const IdMap[], afxObject objects[], void const* udd[])
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(cnt);
    AFX_ASSERT(objects);

    AfxLockClass(cls, NIL);

    afxPool* pool = &cls->pool;

    if (pool->totalUsedCnt >= cls->maxInstCnt)
    {
        AfxThrowError();
    }
    else
    {
        // If an object is statically allocated, check for the each container presence and if it is SIMD aligned.
        if (pool->unitSiz == 0)
        {
            for (afxUnit i = 0; i < cnt; i++)
            {
                if (!objects[i])
                    AfxThrowError();

                if (!AFX_TEST_ALIGNMENT(objects[i], AFX_SIMD_ALIGNMENT))
                    AfxThrowError();
            }
        }
        else
        {
            if (_AfxAllocateObjects(cls, cnt, objects))
                AfxThrowError();
        }

        if (!err)
        {
            if (_AfxSetUpObjects(cls, cnt, objects, (void**)udd))
            {
                AfxThrowError();
                _AfxDeallocateObjects(cls, cnt, objects);
            }
            else
            {
                AFX_ASSERT_OBJECTS(cls->objFcc, cnt, objects);
            }
        }
    }

    AfxUnlockClass(cls, NIL);

    return err;
}

_AFX afxError AfxReacquireObjects(afxHere const dbg, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(objects);
    AFX_ASSERT(cnt);

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxObject obj = objects[i];

        if (!obj)
            continue;

        afxObjectBase* hdr = GET_OBJ_HDR(obj);
        AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
        AfxIncAtom32(&hdr->refCnt);

        if (hdr->cls->dbgAcq)
        {
            // Debugging
            AfxReportComment("Reacquired &%p#%u,%.4s", obj, hdr->instIdx, &hdr->cls->objFcc);
        }
    }
    return err;
}

_AFX afxBool AfxDisposeObjects(afxHere const dbg, afxUnit cnt, afxObject objects[])
{
    afxError err = { 0 };
    AFX_ASSERT(objects);
    AFX_ASSERT(cnt);
    afxBool rslt = 0;

    for (afxUnit i = cnt; i-- > 0;)
    {
        afxObject obj = objects[i];

        if (!obj)
            continue;

        afxObjectBase* hdr = GET_OBJ_HDR(obj);
        AFX_ASSERT(hdr->fcc == afxFcc_OBJ);

        AfxCatchError(err);

        afxInt refCnt = AfxDecAtom32(&hdr->refCnt);

        if (0 == refCnt)
        {
            afxClass* cls = hdr->cls;
            AFX_ASSERT(cls->fcc == afxFcc_CLS);

            // se der erro aqui, é porque você provavelmente está passando uma array como elemento aninhado (ex.: AfxDisposeObjects(n, (void*[]){ array })) ao invés de passar a array diretamente (ex.: AfxDisposeObjects(n, array));

            if (cls->dbgAcq)
            {
                // Debugging
                AfxReportComment("Disposed &%p#%u,%.4s", obj, hdr->instIdx, &hdr->cls->objFcc);
            }

            AfxLockClass(cls, NIL);

#ifdef _AFX_DEBUG
            // Debugging
            afxUnit poolUsedCnt = cls->pool.totalUsedCnt;
#endif

            _AfxCleanUpObjects(cls, 1, &obj);
            AFX_ASSERT(obj == hdr);

            // Skip deallocation if it is statically allocated.
            if (cls->pool.unitSiz != 0)
                _AfxDeallocateObjects(cls, 1, &obj);

#ifdef _AFX_DEBUG
            // Debugging
            AFX_ASSERT(!poolUsedCnt || (cls->pool.totalUsedCnt == (poolUsedCnt - 1)));
#endif

            AfxUnlockClass(cls, NIL);

            ++rslt;
        }
        objects[i] = NIL;
    }
    return rslt;
}

_AFX afxUnit _AfxAssertObjects(afxUnit cnt, afxObject const objects[], afxFcc fcc)
{
    afxError err = { 0 };
    AFX_ASSERT(objects);
    AFX_ASSERT(cnt);
    afxUnit exceptions = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxObject obj = objects[i];

        if (!obj)
        {
            ++exceptions;
            continue;
        }

        afxObjectBase* hdr = GET_OBJ_HDR(obj);
        AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
        afxClass const* cls = hdr->cls;

        if (!cls)
        {
            AfxThrowError();
            AfxCatchError(err);
            continue;
        }
        AFX_ASSERT(cls->fcc == afxFcc_CLS);
        afxBool found = FALSE;

        do if (cls->objFcc == fcc)
        {
#if 0
            // Must be the class which registers the size.
            //if (cls->fixedSiz)
            if (hdr->cls->fixedSiz)
            {
                void* ptr;
                afxBool booked = AfxGetPoolUnit(&hdr->cls->pool, hdr->instIdx, &ptr);
                AFX_ASSERT(booked);
                if (!booked)
                    break;
            }
#endif
            found = TRUE;
            break;
        }
        while ((cls = AfxGetSubclass(cls)));

        if (!found) ++exceptions;
        else
        {
            AFX_ASSERT_CLASS(cls, fcc);
        }
        AfxCatchError(err);
    }
    return exceptions;
}

_AFX afxUnit _AfxTryAssertObjects(afxUnit cnt, afxObject const objects[], afxFcc fcc)
{
    afxError err = { 0 };
    AFX_ASSERT(objects);
    AFX_ASSERT(cnt);
    afxUnit exceptions = 0;

    for (afxUnit i = 0; i < cnt; i++)
    {
        afxObject obj = objects[i];

        if (!obj)
            continue;

        afxObjectBase* hdr = GET_OBJ_HDR(obj);
        AFX_ASSERT(hdr->fcc == afxFcc_OBJ);
        afxClass const* cls = hdr->cls;

        if (!cls)
        {
            AfxThrowError();
            AfxCatchError(err);
            continue;
        }

        AFX_ASSERT(cls->fcc == afxFcc_CLS);
        afxBool found = FALSE;

        do if (cls->objFcc == fcc) { found = TRUE;  break; }
        while ((cls = AfxGetSubclass(cls)));

        if (!found) ++exceptions;
        else
        {
            AFX_ASSERT_CLASS(cls, fcc);
        }
        AfxCatchError(err);
    }
    return exceptions;
}

_AFX afxResult AfxWaitForObject(afxTime timeout, afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT(obj);
    return 0;
}

_AFX afxError AfxInstallClassExtension(afxClass* cls, afxClassExtension* const ext)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);

    // Make sure that the library has not been opened yet.

    if (cls->instCnt)
    {
        AfxThrowError();
        return err;
    }

    // Make sure it's not in the list yet.
    afxUnit off, siz = ext->objSiz;

    //if (AfxFindClassPluginSegment(cls, ext->extId, &off, &siz))
    {
        //AfxThrowError();
        // Can't register twice, return the already registered offset.
    }
    //else
    {
        afxUnit newExtraSiz;
        // Increase structure size, but keep longword alignment.
#ifdef _AFX_DEBUG
    // Allow for a longword front and back for stomp detection.
        newExtraSiz = cls->extraSiz + (ext->objSiz = ((siz + (sizeof(afxUnit32) * 2) + 3) & ~3));
        ext->objOff = cls->extraSiz + sizeof(afxUnit32);
#else
        newExtraSiz = cls->extraSiz + ((siz + 3) & ~3);
        ext->objOff = cls->extraSiz;
#endif
        cls->extraSiz = newExtraSiz;
        AfxPushLink(&ext->cls, &cls->extensions);

        //if (AfxMakeArena(&cls->extraAlloc, NIL, AfxHere()))
            //AfxThrowError();
    }
    return err;
}

#define rwTKMEMORYMAGIC 0x12345678

afxResult _AfxObjAggrValidation(afxClass* cls, afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(obj);
    afxResult invalid = 0;

    afxClassExtension* ext;
    afxChain const* extensions = &cls->extensions;
    AFX_ITERATE_CHAIN_B2F(ext, cls, extensions)
    {
        afxUnit32 frontOfMemOff = ext->objOff - 4;
        afxUnit32 backOfMemOff = (ext->objOff + ext->objSiz + 3) & ~3;
        afxUnit32 const* frontOfMem = (afxUnit32 const*)(((afxUnit8 const*)obj) + frontOfMemOff);
        afxUnit32 const* backOfMem = (afxUnit32 const*)(((afxUnit8 const*)obj) + backOfMemOff);

        if ((*frontOfMem != rwTKMEMORYMAGIC) || (*backOfMem != rwTKMEMORYMAGIC))
        {
            ++invalid;
        }
    }
    return invalid;
}

afxError _AfxObjAggrCtor(afxClass* cls, afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(obj);

#ifdef RWDEBUG
    _rwPluginRegistryPlaceValidationCodes(reg, object);
#endif

    // Run through the list forwards.

    afxClassExtension* ext;
    afxChain const* extensions = &cls->extensions;
    AFX_ITERATE_CHAIN_B2F(ext, cls, extensions)
    {
        if (ext->ctorCb(obj, AfxGetObjectExtra(obj, ext->extId), ext->objSiz))
        {
            AfxThrowError();

            // Failed, go back along the constructor list.
            
            while ((ext = AFX_REBASE(AfxGetPrevLink(&ext->cls), afxClassExtension, cls)))
                if (ext->dtorCb(obj, AfxGetObjectExtra(obj, ext->extId), ext->objSiz))
                    AfxThrowError();

            break;
        }
    }

#ifdef RWDEBUG
    if (_AfxDoObjectValidation(reg, object))
    {
        AfxThrowError();
        // Object plugin memory has become corrupt during construction.
    }
#endif
    return err;
}

afxError _AfxObjAggrDtor(afxClass* cls, afxObject obj)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(obj);

#ifdef RWDEBUG
    if (_AfxDoObjectValidation(reg, object))
    {
        AfxThrowError();
        // Object plugin memory is corrupt at destruction.
    }
#endif

    // Run through the list backwards.

    afxClassExtension* ext;
    afxChain const* extensions = &cls->extensions;
    AFX_ITERATE_CHAIN(ext, cls, extensions)
    {
        if (ext->dtorCb && ext->dtorCb(obj, AfxGetObjectExtra(obj, ext->extId), ext->objSiz))
            AfxThrowError();
    }
    return err;
}

afxError _AfxObjAggrCpy(afxClass* cls, void* dst, void const* src)
{
    afxError err = { 0 };
    AFX_ASSERT(cls->fcc == afxFcc_CLS);
    AFX_ASSERT(dst);
    AFX_ASSERT(src);

#ifdef RWDEBUG
    if (_AfxDoObjectValidation(cls, src))
    {
        AfxThrowError();
        // Source object plugin memory is corrupt at copy.;
    }

    if (_AfxDoObjectValidation(cls, dst))
    {
        AfxThrowError();
        // Destination object plugin memory is corrupt at copy.;
    }
#endif

    // Run through the list forwards.

    afxClassExtension* ext;
    afxChain const* extensions = &cls->extensions;
    AFX_ITERATE_CHAIN_B2F(ext, cls, extensions)
    {
        if (ext->cpyCb(dst, (void*)src, AfxGetObjectExtra(dst, ext->extId), ext->objSiz))
            AfxThrowError();
    }

#ifdef RWDEBUG
    if (_AfxDoObjectValidation(cls, src))
    {
        AfxThrowError();
        // Source object plugin memory has become corrupt during copy.;
    }

    if (_AfxDoObjectValidation(cls, dst))
    {
        AfxThrowError();
        // Destination object plugin memory has become corrupt during copy.
    }
#endif
    return err;
}
