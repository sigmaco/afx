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
#include "../src/afx/dev/afxDevCoreBase.h"

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

_AFX afxResult AfxExhaustChainedClasses(afxChain *provisions)
{
    afxError err = AFX_ERR_NONE;
    afxResult cnt = 0;

    afxClass* cls;
    AfxIterateLinkage(afxClass, cls, provisions, host)
    {
        AfxAssertType(cls, afxFcc_CLS);
        
        if (cls->objFcc == afxFcc_SYS)
            break; // We can't drop it, it would fuck the entire platform. Break.
            
        cnt += AfxExhaustClass(cls);
    }
    return cnt;
}

_AFX afxResult AfxCleanUpChainedClasses(afxChain *provisions)
{
    afxError err = AFX_ERR_NONE;
    afxResult cnt = 0;

    while (1)
    {
        afxLinkage *first = AfxGetLastLinkage(provisions);

        if (!first) break;
        else
        {
            afxClass *cls = AFX_REBASE(first, afxClass, host);
            AfxAssertType(cls, afxFcc_CLS);
            ++cnt;

            if (cls->objFcc != afxFcc_SYS) AfxDeregisterClass(cls);
            else
            {
                AfxAssert(cls->maxInstCnt == 1);
                //AfxAssert(cls->pool.totalUsedCnt == 1);
                break; // has only the SYS. We can't drop it else we will fuck the entire platform. Break.
            }                
        }
    }
    return cnt;
}

_AFXINL afxClass* AfxGetSubClass(afxClass const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    return AfxGetLinker(&cls->subset);
}

_AFXINL afxNat AfxGetClassInstanceFixedSize(afxClass const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    return cls->fixedSiz;
}

_AFXINL afxNat AfxGetClassInstanceStrictFixedSize(afxClass const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    afxClass *subset = AfxGetSubClass(cls);
    return (subset ? cls->fixedSiz - subset->fixedSiz : cls->fixedSiz);
}

_AFXINL afxNat AfxCountClassInstances(afxClass const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    afxNat instCnt = cls->instCnt;
    afxChain const *supersets = &cls->supersets;
    AfxChainForEveryLinkageB2F(supersets, afxClass, subset, cls)
    {
        AfxAssertType(cls, afxFcc_CLS);
        instCnt += cls->instCnt;
    }
    return instCnt;
}

_AFXINL afxObject AfxGetClassInstance(afxClass const* cls, afxNat32 uniqueId)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    afxObject obj;
    AfxEnumerateClassInstances(cls, uniqueId, 1, &obj);
    return obj;
}

_AFX afxNat _AfxClsEnumInstances(afxClass const* cls, afxBool fromLast, afxNat first, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    afxNat rslt = 0;

    if (first && (first >= cls->pool.totalUsedCnt))
    {
        first -= cls->pool.totalUsedCnt;
    }
    else
    {
        rslt = cnt ? AfxEnumeratePoolItems(&cls->pool, first, cnt, (void**)objects) : 0;

        if (rslt)
        {
            AfxAssert(cnt >= rslt);

            for (afxNat i = 0; i < rslt; i++)
            {
                afxObjectBase* subset = objects[i];
                AfxAssertType(subset, afxFcc_OBJ);
                //afxModule inst2 = (inst + 1);
                //AfxAssertObjects(1, &inst2, cls->objFcc);
                objects[i] = (subset + 1);
                AfxAssertObjects(1, &objects[i], cls->objFcc);
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
        AfxChainForEveryLinkageB2F(supersets, afxClass, subset, superset)
        {
            AfxAssertType(superset, afxFcc_CLS);

            if (first && (first >= superset->pool.totalUsedCnt))
            {
                first -= superset->pool.totalUsedCnt;
            }
            else
            {
#if !0
                afxNat rslt2 = _AfxClsEnumInstances(superset, fromLast, first, cnt, &objects[rslt]);

                AfxAssert(cnt >= rslt2); // cant be greater

                if (first && (first >= rslt2))
                    first -= rslt2;

                cnt = cnt - rslt2;
                rslt += rslt2;

#else
                afxNat rslt2 = _AfxClsEnumInstances(superset, fromLast, first2, cnt2, &objects[rslt]);

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

_AFX afxNat _AfxClsEnumInstances2(afxClass const* cls, afxBool fromLast, afxBool(*flt)(afxObject, void*), void* fdd, afxNat first, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    afxNat rslt = 0;

    if (first && (first >= cls->pool.totalUsedCnt))
    {
        first -= cls->pool.totalUsedCnt;
    }
    else
    {
        afxFcc objFcc = cls->objFcc;
        afxObjectBase* baseObj;
        afxNat j = first;

        while (AfxEnumeratePoolItems(&cls->pool, j++, 1, (void**)&baseObj))
        {
            AfxAssertType(baseObj, afxFcc_OBJ);
            afxObject obj = (baseObj + 1);
            AfxAssertObjects(1, &obj, objFcc);

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
        AfxChainForEveryLinkageB2F(supersets, afxClass, subset, superset)
        {
            AfxAssertType(superset, afxFcc_CLS);

            if (first && (first >= superset->pool.totalUsedCnt))
            {
                first -= superset->pool.totalUsedCnt;
            }
            else
            {
#if !0
                afxNat rslt2 = _AfxClsEnumInstances2(superset, fromLast, flt, fdd, first, cnt, &objects[rslt]);

                AfxAssert(cnt >= rslt2); // cant be greater

                if (first && (first >= rslt2))
                    first -= rslt2;

                cnt = cnt - rslt2;
                rslt += rslt2;

#else
                afxNat rslt2 = _AfxClsEnumInstances(superset, fromLast, first2, cnt2, &objects[rslt]);

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

_AFX afxNat AfxEnumerateClassInstances(afxClass const* cls, afxNat first, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    afxNat rslt = 0;

    if (cls->instCnt)
    {
        AfxEnterSlockShared((void*)&cls->poolLock);
        rslt = _AfxClsEnumInstances(cls, FALSE, first, cnt, objects);
        AfxExitSlockShared((void*)&cls->poolLock);
    }
    return rslt;
}

_AFX afxNat AfxEvokeClassInstances(afxClass const* cls, afxBool(*flt)(afxObject, void*), void* fdd, afxNat first, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    afxNat rslt = 0;

    if (cls->instCnt)
    {
        AfxEnterSlockShared((void*)&cls->poolLock);
        rslt = _AfxClsEnumInstances2(cls, FALSE, flt, fdd, first, cnt, objects);
        AfxExitSlockShared((void*)&cls->poolLock);
    }
    return rslt;
}

_AFX afxNat _AfxClsInvokeInstances2(afxClass const* cls, afxBool fromLast, afxNat first, afxNat cnt, afxBool(*flt)(afxObject,void*), void* fdd, afxBool(*exec)(afxObject,void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(exec);
    afxNat rslt = 0;
    afxNat i = first;
    afxObject obj;
    
    if (!flt)
    {
        while (_AfxClsEnumInstances(cls, fromLast, i++, 1, &obj))
        {
            AfxAssertObjects(1, &obj, cls->objFcc);
            ++rslt;

            if (!exec(obj, udd)) // if secondary callback doesn't feed back, break the iteration.
                break;

            if (++rslt == cnt) // if cnt has been reached, break the iteration.
                break;
        }
    }
    else
    {
        while (_AfxClsEnumInstances(cls, fromLast, i++, 1, &obj))
        {
            AfxAssertObjects(1, &obj, cls->objFcc);

            if (flt(obj, fdd)) // pass only if filter has fed.
            {
                ++rslt;

                if (!exec(obj, udd)) // if secondary callback don't feed back, break the iteration.
                    break;

                if (++rslt == cnt) // if cnt has been reached, break the iteration.
                    break;
            }
        }
    }
    return rslt;
}

_AFX afxNat AfxInvokeClassInstances2(afxClass const* cls, afxNat first, afxNat cnt, afxBool(*flt)(afxObject, void*), void* fdd, afxBool(*exec)(afxObject, void*), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(flt);
    AfxAssert(exec);
    afxNat rslt = 0;

    if (cls->instCnt)
    {
        AfxEnterSlockShared((void*)&cls->poolLock);
        rslt = _AfxClsInvokeInstances2(cls, FALSE, first, cnt, flt, fdd, exec, udd);
        AfxExitSlockShared((void*)&cls->poolLock);
    }
    return rslt;
}

_AFX afxNat AfxInvokeClassInstances(afxClass const* cls, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(f);
    afxNat rslt = 0;
    
    if (cls->instCnt)
    {
        AfxEnterSlockShared((void*)&cls->poolLock);
        rslt = _AfxClsInvokeInstances2(cls, FALSE, first, cnt, NIL, NIL, f, udd);
        AfxExitSlockShared((void*)&cls->poolLock);
    }
    return rslt;
}

_AFXINL afxBool AfxFindClassPluginSegment(afxClass const* cls, afxNat pluginId, afxNat* off, afxNat* siz)
{
    afxError err = NIL;
    AfxTryAssertType(cls, afxFcc_CLS);
    afxBool rslt = FALSE;

    afxClassPlugin* ext;
    afxChain const* plugins = &cls->plugins;
    AfxChainForEveryLinkageB2F(plugins, afxClassPlugin, cls, ext)
    {
        if (ext->pluginId == pluginId)
        {
            *off = ext->objOff;
            *siz = ext->objSiz;
            rslt = TRUE;
            break;
        }
    }
    return rslt;
}

_AFXINL void* AfxGetObjectExtra(afxObject obj, afxNat pluginId)
{
    afxError err = NIL;
    afxObjectBase* hdr = obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    afxClass* cls = hdr->cls;
    AfxAssertType(cls, afxFcc_CLS);

    afxNat off = 0, siz = 0;
    void* p = NIL;

    if (hdr->extra && AfxFindClassPluginSegment(cls, pluginId, &off, &siz))
    {
        p = &hdr->extra[off];
    }
    return p;
}

_AFX afxError _AfxDeallocateClassInstances(afxClass *cls, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(objects);
    AfxAssert(cnt);
    
#if !0
    AfxDeallocatePoolUnits(&cls->pool, cnt, (void**)objects);
#else
    for (afxNat i = 0; i < cnt; i++)
    {
        afxObjectBase *item = (afxObjectBase *)obj[i];
        AfxAssert(item);
        --item;
        AfxDeallocatePoolUnit(&cls->pool, item);
    }
#endif
    return err;
}

_AFX afxError _AfxAllocateClassInstances(afxClass *cls, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);

    afxPool* pool = &cls->pool;
    afxNat room = (cls->maxInstCnt - pool->totalUsedCnt);

    if ((room == 0) || (cnt > room))
        AfxThrowError();
    else
    {
        if (pool->totalUsedCnt == 0)
            pool->unitsPerPage = cls->unitsPerPage ? cls->unitsPerPage : cnt;

        if (AfxAllocatePoolUnits(pool, cnt, (void**)objects))
            AfxThrowError();
    }
    return err;
}

_AFX afxError _AfxAllocateClassInstancesAt(afxClass *cls, afxNat subset, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);

    afxPool* pool = &cls->pool;
    afxNat room = (cls->maxInstCnt - pool->totalUsedCnt);

    if ((room == 0) || (cnt > room)) AfxThrowError();
    else
    {
        if (pool->totalUsedCnt == 0)
            pool->unitsPerPage = cls->unitsPerPage ? cls->unitsPerPage : cnt;

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
                _AfxDeallocateClassInstances(cls, i, objects);
            }
        }
    }
    return err;
}

_AFX afxError _AfxClsObjDtor(afxClass *cls, afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);

    afxClassPlugin* ext;
    afxChain const* plugins = &cls->plugins;
    AfxChainForEveryLinkage(plugins, afxClassPlugin, cls, ext)
    {
        if (ext->dtorCb && ext->dtorCb(obj, AfxGetObjectExtra(obj, ext->pluginId), ext->objSiz))
            AfxThrowError();
    }

    if (cls->dtor && cls->dtor(obj))
        AfxThrowError();

    if (err)
    {
        AfxLogEcho("Destructing instance %p of %s class.", obj, cls->name);
    }

    afxClass* subset = AfxGetSubClass(cls);

    if (subset)
    {
        if (err)
        {
            AfxLogEcho("Destructing instance %p of %s : %s class.", obj, cls->name, subset->name);
        }

        if (_AfxClsObjDtor(subset, obj))
            AfxThrowError();
    }

    return err;
}

_AFX afxError _AfxDestructClassInstances(afxClass *cls, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(objects);
    AfxAssert(cnt);
 
    for (afxNat i = cnt; i-- > 0;)
    {
        afxObject obj = objects[i];

        if (obj)
        {
            --cls->instCnt;
            //AfxLogEcho("Dismantling %s object (#%d) %p...", cls->name, i, obj[i]);
            afxObjectBase *hdr = (afxObjectBase *)obj;
            --hdr;
            AfxAssertType(hdr, afxFcc_OBJ);
            AfxAssert(AfxGetObjectClass(obj) == cls);

            //afxFcc fcc = AfxGetObjectFcc(item);
            //afxNat refCnt = AfxGetRefCount(item);
            //AfxLogEcho("Destroying %s... %p[%.4s]^%i", cls->name, item, (afxChar*)&fcc, item->refCnt);

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

            if (_AfxClsObjDtor(cls, obj))
                AfxThrowError();

            AfxZero2(1, sizeof(*hdr), hdr);
            objects[i] = hdr;

            //AfxDeallocate(cls->all, item);

            //AfxLogEcho("Instance %p[%.4s]^%i destroyed.", item, (afxChar const*)&(fcc), refCnt);
        }
    }
    return err;
}

_AFX afxError _AfxClsObjCtor(afxClass *cls, afxObject obj, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);

    afxClass* subset = AfxGetSubClass(cls);

    if (subset)
    {
        if (_AfxClsObjCtor(subset, obj, cookie))
            AfxThrowError();

        if (err)
        {
            AfxLogEcho("afxClass<%s, %s>ConstructingObject(%p)", cls->name, subset->name, obj);
        }
    }

    if (err)
    {
        AfxLogEcho("afxClass<%s>::ConstructObject(%p)", cls->name, obj);
    }

    if (!err)
    {
        if (cls->ctor && cls->ctor(obj, cookie))
        {
            AfxThrowError();
        }
        else
        {
            afxClassPlugin* ext;
            afxChain const* plugins = &cls->plugins;
            AfxChainForEveryLinkageB2F(plugins, afxClassPlugin, cls, ext)
            {
                if (ext->ctorCb && ext->ctorCb(obj, AfxGetObjectExtra(obj, ext->pluginId), ext->objSiz))
                    AfxThrowError();
            }
        }

        if (err)
        {
            if (cls->subset.chain)
                _AfxClsObjDtor(cls, obj);
        }
    }
    return err;
}

_AFX afxError _AfxConstructClassInstances(afxClass *cls, afxNat cnt, afxObject objects[], void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
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
        hdr->cls = cls;

        AfxStoreAtom32(&hdr->refCnt, 1);

        hdr->tid = AfxGetTid();

        hdr->watchers = NIL;
        hdr->watching = NIL;
        hdr->event = cls->defEvent;

        objects[i] = hdr + 1; // move the pointer forward

        AfxAssert(AfxIsAligned(objects[i], sizeof(void*)));
        AfxAssert(hdr == (((afxObjectBase*)(objects[i])) - 1));

        hdr->extra = NIL;

        if (cls->extraSiz)
            if (!(hdr->extra = AfxRequestArenaUnit(&cls->extraAlloc, cls->extraSiz)))
                AfxThrowError();

        cookie.no = i;

        if (_AfxClsObjCtor(cls, objects[i], &cookie))
        {
            AfxThrowError();

            for (afxNat j = i; j-- > 0;)
            {
                _AfxClsObjDtor(cls, objects[j]);
                afxObjectBase* subset = objects[j];
                objects[j] = subset - 1; // move the pointer backward
            }
            objects[i] = hdr - 1; // move the pointer backward
        }
        else
        {
            ++cls->instCnt;
        }

        if (err && hdr->extra)
            AfxRecycleArenaUnit(&cls->extraAlloc, hdr->extra, cls->extraSiz);

    }
    return err;
}

_AFX afxNat AfxExhaustClass(afxClass *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    afxNat rslt = 0;

    if (cls->fixedSiz)
    {
        afxObject obj;
        while (_AfxClsEnumInstances(cls, TRUE, 0, 1, &obj))
        {
            while (TRUE != AfxReleaseObjects(1, &obj));
            ++rslt;
        }
    }
    return rslt;
}

_AFX afxError AfxDeregisterClass(afxClass *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);

    afxNat objCnt = AfxCountClassInstances(cls);
    afxClass* subset = AfxGetSubClass(cls);

    if (cls->fixedSiz)
    {
        if (objCnt)
        {
            AfxLogEcho("Anihilating %u lost <%s> objects.", objCnt, cls->name);
        }
    }
    else
    {
        if (objCnt)
        {
            AfxLogEcho("%u lost <%s> objects not reachable.", objCnt, cls->name);
        }
    }
    //AfxEnterSlockExclusive(&cls->slock);

    AfxCleanUpChainedClasses(&cls->supersets);
    AfxAssert(0 == AfxGetChainLength(&cls->supersets));

    AfxExhaustClass(cls);

    if (cls->fixedSiz) // only if dynamically allocated
    {
        AfxAssert(0 == cls->pool.totalUsedCnt);        
        AfxEnterSlockExclusive(&cls->poolLock);
        AfxCleanUpPool(&cls->pool);
        AfxExitSlockExclusive(&cls->poolLock);
        AfxCleanUpSlock(&cls->poolLock);

        if (cls->mmu)
            AfxReleaseObjects(1, (void*[]) { cls->mmu });
    }

    afxClassPlugin* ext;
    afxChain const* plugins = &cls->plugins;
    AfxChainForEveryLinkageB2F(plugins, afxClassPlugin, cls, ext)
    {
        AfxPopLinkage(&ext->cls);
    }

    if (cls->extraSiz)
    {
        AfxDeallocateArena(&cls->extraAlloc);
        cls->extraSiz = 0;
    }

    if (cls->subset.chain)
        AfxPopLinkage(&cls->subset);

    if (cls->host.chain)
        AfxPopLinkage(&cls->host);


    cls->maxInstCnt = 0; // block new acquisitions.
    //AfxZero(cls, sizeof(*cls));

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

_AFX afxError AfxRegisterClass(afxClass* cls, afxClass* subset, afxChain* host, afxClassConfig const* cfg)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssert(cfg);

    AfxZero(cls, sizeof(*cls));

    if (host)
    {
        if (subset)
        {
            //AfxLogEcho("Registering... %s[%s]", cfg->name, subset->name);
        }
        else
        {
            //AfxLogEcho("Registering... %s", cfg->name);
        }
    }
    else
    {
        if (subset)
        {
            //AfxLogEcho("Registering... %s[%s]", cfg->name, subset->name);
        }
        else
        {
            //AfxLogEcho("Registering... %s", cfg->name);
        }
    }

    //AfxSetUpSlock(&cls->slock);
    //AfxEnterSlockExclusive(&cls->slock);

    cls->fcc = afxFcc_CLS;
    AfxAssert(cfg->fcc);
    cls->objFcc = cfg->fcc;
    //AfxAssert(cfg->size);
    cls->fixedSiz = cfg->fixedSiz;

    if (!subset)
    {
        AfxPushLinkage(&cls->subset, NIL);
        cls->level = _AFX_CLASS_BASE_LEVEL;
    }
    else
    {
        AfxAssert(subset != cls);
        AfxAssertType(subset, afxFcc_CLS);
        AfxAssert(cfg->fixedSiz >= subset->fixedSiz);
        AfxPushLinkage(&cls->subset, &subset->supersets);
        cls->level = subset->level + 1;
    }

    cls->levelMask = (_AFX_CLASS_LEVEL_MASK << cls->level);
    cls->maxInstCnt = cfg->maxCnt ? cfg->maxCnt : AFX_N32_MAX;
    cls->ctor = cfg->ctor ? cfg->ctor : _AfxDummyObjCtor;
    cls->dtor = cfg->dtor ? cfg->dtor : _AfxDummyObjDtor;
    AfxAssert(cfg->name && cfg->name[0]);
    AfxStrcpy(cls->name, cfg->name ? cfg->name : "");
    cls->defEvent = cfg->event;
    cls->defEventFilter = cfg->eventFilter;
    cls->vmt = cfg->vmt;
    cls->unitsPerPage = AfxMin(cfg->unitsPerPage, 32);
    
    if (cls->fixedSiz) // only if dynamically allocated
    {
        AfxSetUpSlock(&cls->poolLock);
        AfxEnterSlockExclusive(&cls->poolLock);
        AfxSetUpPool(&cls->pool, cls->fixedSiz ? AFX_ALIGN(cls->fixedSiz + sizeof(afxObjectBase), sizeof(void*)) : 0, AfxMax(1, cls->unitsPerPage));
        AfxExitSlockExclusive(&cls->poolLock);

        if ((cls->mmu = cfg->mmu))
        {
            AfxAssertObjects(1, &cls->mmu, afxFcc_MMU);

            if (AfxReacquireObjects(1, (void*[]) { cls->mmu }))
                AfxThrowError();
        }
    }

    AfxSetUpChain(&cls->supersets, cls);
    AfxPushLinkage(&cls->host, host ? host : &orphanClassChain);

    AfxSetUpChain(&cls->plugins, cls);
    cls->extraSiz = 0;

    //AfxExitSlockExclusive(&cls->slock);
    return err;
}

_AFX afxNat AfxGetObjectId(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxObjectBase* hdr = (afxObjectBase*)obj;
    --hdr;
    AfxAssertType(hdr, afxFcc_OBJ);
    
    afxPool* pool = &(AfxGetObjectClass(obj)->pool);
    //AfxAssertType(pool, afxFcc_POOL);
    afxNat idx = AFX_INVALID_INDEX;
    AfxFindPoolUnitIndex(pool, (afxByte*)hdr, &idx, NIL);
    return idx;
}

_AFX afxError AfxAcquireObjects(afxClass *cls, afxNat cnt, afxObject objects[], void const* udd[])
{
    afxError err = AFX_ERR_NONE;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(objects);

    AfxEnterSlockExclusive(&cls->poolLock);

    if (cls->pool.totalUsedCnt >= cls->maxInstCnt)
        AfxThrowError();
    else
    {
        afxObjectBase* ptr, *ptr2;

        if (_AfxAllocateClassInstances(cls, cnt, objects))
            AfxThrowError();
        else
        {
            if (_AfxConstructClassInstances(cls, cnt, objects, (void**)udd))
            {
                AfxThrowError();
                _AfxDeallocateClassInstances(cls, cnt, objects);
            }
            else
            {
                AfxAssertObjects(cnt, objects, cls->objFcc);
            }
        }
    }
    
    AfxExitSlockExclusive(&cls->poolLock);

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
                afxClass* cls = hdr->cls;
                AfxAssertType(cls, afxFcc_CLS);

                // se der erro aqui, é porque você provavelmente está passando uma array como elemento aninhado (ex.: AfxReleaseObjects(n, (void*[]){ array })) ao invés de passar a array diretamente (ex.: AfxReleaseObjects(n, array));

                AfxEnterSlockExclusive(&cls->poolLock);

                _AfxDestructClassInstances(cls, 1, &obj);
                AfxAssert(obj == hdr);
                _AfxDeallocateClassInstances(cls, 1, &obj);

                AfxExitSlockExclusive(&cls->poolLock);
                
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
            afxClass const* cls = hdr->cls;
            AfxAssertType(cls, afxFcc_CLS);

            do if (cls->objFcc == fcc) { ++rslt; break; }
            while ((cls = AfxGetSubClass(cls)));

            AfxAssertClass(cls, fcc);
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

afxNat AfxRegisterClassExtension(afxClass* cls, afxClassPlugin* const ext)
{
    afxError err = NIL;
    AfxTryAssertType(cls, afxFcc_CLS);

    // Make sure that the library has not been opened yet.

    if (cls->instCnt) AfxThrowError();
    else
    {
        // Make sure it's not in the list yet.
        afxNat off, siz;

        if (AfxFindClassPluginSegment(cls, ext->pluginId, &off, &siz))
        {
            AfxThrowError();
            // Can't register twice, return the already registered offset.
        }
        else
        {
            afxNat newExtraSiz;
            // Increase structure size, but keep longword alignment.
#ifdef _AFX_DEBUG
        // Allow for a longword front and back for stomp detection.
            newExtraSiz = cls->extraSiz + ((siz + (sizeof(afxNat32) * 2) + 3) & ~3);
            ext->objOff = cls->extraSiz + sizeof(afxNat32);
#else
            newExtraSiz = cls->extraSiz + ((siz + 3) & ~3);
            ext->objOff = cls->extraSiz;
#endif
            cls->extraSiz = newExtraSiz;
            AfxPushLinkage(&ext->cls, &cls->plugins);

            if (AfxAllocateArena(NIL, &cls->extraAlloc, NIL, AfxHere()))
                AfxThrowError();
        }
    }
    return ext->objOff;
}

#define rwTKMEMORYMAGIC 0x12345678

afxResult _AfxObjAggrValidation(afxClass* cls, afxObject obj)
{
    afxError err = NIL;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(obj);
    afxResult invalid = 0;

    afxClassPlugin* ext;
    afxChain const* plugins = &cls->plugins;
    AfxChainForEveryLinkageB2F(plugins, afxClassPlugin, cls, ext)
    {
        afxNat32 frontOfMemOff = ext->objOff - 4;
        afxNat32 backOfMemOff = (ext->objOff + ext->objSiz + 3) & ~3;
        afxNat32 const* frontOfMem = (afxNat32 const*)(((afxNat8 const*)obj) + frontOfMemOff);
        afxNat32 const* backOfMem = (afxNat32 const*)(((afxNat8 const*)obj) + backOfMemOff);

        if ((*frontOfMem != rwTKMEMORYMAGIC) || (*backOfMem != rwTKMEMORYMAGIC))
        {
            ++invalid;
        }
    }
    return invalid;
}

afxError _AfxObjAggrCtor(afxClass* cls, afxObject obj)
{
    afxError err = NIL;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(obj);

#ifdef RWDEBUG
    _rwPluginRegistryPlaceValidationCodes(reg, object);
#endif

    // Run through the list forwards.

    afxClassPlugin* ext;
    afxChain const* plugins = &cls->plugins;
    AfxChainForEveryLinkageB2F(plugins, afxClassPlugin, cls, ext)
    {
        if (ext->ctorCb(obj, AfxGetObjectExtra(obj, ext->pluginId), ext->objSiz))
        {
            AfxThrowError();

            // Failed, go back along the constructor list.
            
            while ((ext = AfxRebase(AfxGetPrevLinkage(&ext->cls), afxClassPlugin*, cls)))
                if (ext->dtorCb(obj, AfxGetObjectExtra(obj, ext->pluginId), ext->objSiz))
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
    afxError err = NIL;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(obj);

#ifdef RWDEBUG
    if (_AfxDoObjectValidation(reg, object))
    {
        AfxThrowError();
        // Object plugin memory is corrupt at destruction.
    }
#endif

    // Run through the list backwards.

    afxClassPlugin* ext;
    afxChain const* plugins = &cls->plugins;
    AfxChainForEveryLinkage(plugins, afxClassPlugin, cls, ext)
    {
        if (ext->dtorCb && ext->dtorCb(obj, AfxGetObjectExtra(obj, ext->pluginId), ext->objSiz))
            AfxThrowError();
    }
    return err;
}

afxError _AfxObjAggrCpy(afxClass* cls, void* dst, void const* src)
{
    afxError err = NIL;
    AfxTryAssertType(cls, afxFcc_CLS);
    AfxAssert(dst);
    AfxAssert(src);

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

    afxClassPlugin* ext;
    afxChain const* plugins = &cls->plugins;
    AfxChainForEveryLinkageB2F(plugins, afxClassPlugin, cls, ext)
    {
        if (ext->cpyCb(dst, (void*)src, AfxGetObjectExtra(dst, ext->pluginId), ext->objSiz))
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
