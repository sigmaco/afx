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

#include "qwadro/core/afxManager.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/core/afxTxu.h"
#include "qwadro/mem/afxMmu.h"

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

_AFX afxResult _AfxUninstallChainedClasses(afxChain *provisions)
{
    afxError err = AFX_ERR_NONE;
    afxResult cnt = 0;

    while (1)
    {
        afxLinkage *first = AfxGetLastLinkage(provisions);

        if (!first) break;
        else
        {
            afxManager *cls = AFX_REBASE(first, afxManager, provider);
            AfxAssertType(cls, afxFcc_CLS);
            ++cnt;

            if (cls->objFcc == afxFcc_SYS)
                break; // has only the SYS. We can't drop it else we will fuck the entire platform. Break.
            else
                AfxCleanUpManager(cls);
        }
    }
    return cnt;
}

_AFX afxResult _AfxShutdownOrphanClasses(void)
{
    return _AfxUninstallChainedClasses(&orphanClassChain);
}

_AFX void AfxClassLockInclusive(afxManager *cls)
{
    //AfxEnterSlockShared(&cls->slock);
}

_AFX void AfxClassLockExclusive(afxManager *cls)
{
    //AfxEnterSlockExclusive(&cls->slock);
}

_AFX void AfxClassUnlockInclusive(afxManager *cls)
{
    //AfxExitSlockShared(&cls->slock);
}

_AFX void AfxClassUnlockExclusive(afxManager *cls)
{
    //AfxExitSlockExclusive(&cls->slock);
}

_AFX afxBool AfxClassTryLockInclusive(afxManager *cls)
{
    return 1;// AfxTryEnterSlockShared(&cls->slock);
}

_AFX afxBool AfxClassTryLockExclusive(afxManager *cls)
{
    return 1;// AfxTryEnterSlockExclusive(&cls->slock);
}

_AFXINL afxManager* AfxGetBaseClass(afxManager const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    return AfxGetLinker(&cls->base);
}

_AFXINL void const* AfxClassGetVmt(afxManager const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    return cls->vmt;
}

_AFXINL afxNat AfxGetObjectSize(afxManager const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    return cls->siz;
}

_AFXINL afxNat AfxGetObjectStrictSize(afxManager const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    afxManager *base = AfxGetBaseClass(cls);
    return (base ? cls->siz - base->siz : cls->siz);
}

_AFXINL afxNat AfxCountObjects(afxManager const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    return cls->pool.totalUsedCnt;
}

_AFXINL afxObject AfxGetObjectAt(afxManager const* cls, afxNat32 uniqueId)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);    
    afxObject obj;
    AfxEnumerateObjects(cls, uniqueId, 1, &obj);
    return obj;
}

_AFX afxNat _AfxClsEnumerateObjects(afxManager const* cls, afxBool fromLast, afxNat first, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
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
                afxObjectBase* base = objects[i];
                AfxAssertType(base, afxFcc_OBJ);
                //afxModule inst2 = (inst + 1);
                //AfxAssertObjects(1, &inst2, cls->objFcc);
                objects[i] = (base + 1);
                AfxAssertObjects(1, &objects[i], cls->objFcc);
            }

            if (first && (first >= rslt))
                first -= rslt;

            cnt = cnt - rslt;
        }
    }

    if (cnt)
    {
        afxManager *superset;
        afxChain const *supersets = &cls->deriveds;
        AfxChainForEveryLinkageB2F(supersets, afxManager, base, superset)
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

_AFX afxNat AfxEnumerateObjects(afxManager const* cls, afxNat first, afxNat cnt, afxObject obj[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    afxNat rslt = 0;

    AfxClassLockInclusive((afxManager*)cls);
    rslt = _AfxClsEnumerateObjects(cls, FALSE, first, cnt, obj);
    AfxClassUnlockInclusive((afxManager*)cls);
    return rslt;
}

_AFX afxNat _AfxClsCurateObjects(afxManager const* cls, afxBool fromLast, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(f);
    afxNat rslt = 0;

    afxNat i = 0;
    afxObject obj;
    while (_AfxClsEnumerateObjects(cls, fromLast, first + i, 1, &obj))
    {
        AfxAssertObjects(1, &obj, cls->objFcc);
        ++rslt;

        if (f(obj, udd))
            break;
        
        ++i;

        if (i == cnt)
            break;
    }
    return rslt;
}

AFX afxNat AfxInvokeObjects(afxManager const* cls, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(f);
    afxNat rslt = 0;
    AfxClassLockInclusive((afxManager*)cls);
    rslt = _AfxClsCurateObjects(cls, FALSE, first, cnt, f, udd);
    AfxClassUnlockInclusive((afxManager*)cls);
    return rslt;
}

_AFX afxError _AfxDeallocateObjects(afxManager *cls, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
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

_AFX afxError _AfxAllocateObjectsAt(afxManager *cls, afxNat base, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    
    afxPool* pool = &cls->pool;
    afxNat room = (cls->maxInstCnt - pool->totalUsedCnt);

    if ((room == 0) || (cnt > room)) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            objects[i] = NIL;

            if (AfxGetPoolUnit(pool, base + i, NIL)) AfxThrowError();
            else
            {
                if (AfxOccupyPoolUnit(pool, base + i, NIL)) AfxThrowError();
                else
                {
                    AfxAssert(AfxGetPoolUnit(pool, base + i, (void**)&objects[i]));
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

_AFX afxError _AfxAllocateObjects(afxManager *cls, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);

    afxPool* pool = &cls->pool;
    afxNat room = (cls->maxInstCnt - pool->totalUsedCnt);

    if ((room == 0) || (cnt > room))
        AfxThrowError();
    else
    {
        if (AfxAllocatePoolUnits(pool, cnt, (void**)objects))
            AfxThrowError();
    }
    return err;
}

_AFX afxError _AfxObjDtor(afxManager *cls, afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);

    if (cls->dtor && cls->dtor(obj))
        AfxThrowError();

    if (err)
    {
        AfxEcho("Destructing instance %p of %s class.", obj, cls->name);
    }

    afxManager* base = AfxGetBaseClass(cls);

    if (base)
    {
        if (err)
        {
            AfxEcho("Destructing instance %p of %s : %s class.", obj, cls->name, base->name);
        }

        if (_AfxObjDtor(base, obj))
            AfxThrowError();
    }

    return err;
}

_AFX afxError _AfxDestructObjects(afxManager *cls, afxNat cnt, afxObject objects[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(objects);
    AfxAssert(cnt);
 
    for (afxNat i = cnt; i-- > 0;)
    {
        afxObject item = objects[i];
        //AfxEcho("Dismantling %s object (#%d) %p...", cls->name, i, obj[i]);
        afxObjectBase *itemD = (afxObjectBase *)item;
        --itemD;
        AfxAssertType(itemD, afxFcc_OBJ);
        AfxAssert(AfxGetClass(item) == cls);

        //afxFcc fcc = AfxObjectGetFcc(item);
        //afxNat refCnt = AfxGetRefCount(item);
        //AfxEcho("Destroying %s... %p[%.4s]^%i", cls->name, item, (afxChar*)&fcc, item->refCnt);

        {
            AfxObjectSignalConnections(item, AFX_EVENT_OBJ_DESTROYED, NIL);

            afxConnection *objc;
            afxList *connections = &itemD->handling;

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

            connections = &itemD->signaling;

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
            afxEvent ev;
            AfxEventDeploy(&ev, AFX_EVENT_OBJ_DESTROYED, item, NIL);
            AfxObjectEmitEvent(item, &ev);

            afxEventFilter *flt;

            while (itemD->watching)
            {
                afxLinkage *first = AfxGetLastLinkage(itemD->watching);

                if (!first)
                {
                    AfxAssert(!itemD->watching);
                    break;
                }
                else
                {
                    flt = AFX_REBASE(first, afxEventFilter, holder);
                    AfxAssert(AfxGetLinker(&flt->holder) == item);
                    //AfxAssertType((afxHandle*)flt->watched.chain->owner, afxFcc_OBJ);
                    AfxObjectRemoveEventFilter(AfxGetLinker(&flt->watched), item);
                }
            }

            while (itemD->watchers)
            {
                afxLinkage *first = AfxGetLastLinkage(itemD->watchers);

                if (!first)
                {
                    AfxAssert(!itemD->watchers);
                    break;
                }
                else
                {
                    flt = AFX_REBASE(first, afxEventFilter, watched);
                    AfxAssert(AfxGetLinker(&flt->watched) == item);
                    //AfxAssertType((afxHandle*)flt->holder.chain->owner, afxFcc_OBJ);
                    AfxObjectRemoveEventFilter(item, AfxGetLinker(&flt->holder));
                }
            }
        }

        if (_AfxObjDtor(cls, item))
            AfxThrowError();

        AfxZero(1, sizeof(*itemD), itemD);
        objects[i] = itemD;
        //AfxDeallocate(cls->all, item);

        //AfxEcho("Instance %p[%.4s]^%i destroyed.", item, (afxChar const*)&(fcc), refCnt);
    }
    return err;
}

_AFX afxError _AfxObjCtor(afxManager *cls, afxObject obj, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);

    afxManager* base = AfxGetBaseClass(cls);

    if (base)
    {
        if (_AfxObjCtor(base, obj, cookie))
            AfxThrowError();

        if (err)
        {
            AfxEcho("afxManager<%s, %s>ConstructingObject(%p)", cls->name, base->name, obj);
        }
    }

    if (err)
    {
        AfxEcho("afxManager<%s>::ConstructObject(%p)", cls->name, obj);
    }

    if (!err)
    {
        if (cls->ctor && cls->ctor(obj, cookie))
        {
            AfxThrowError();

            if (cls->base.chain)
                _AfxObjDtor(cls, obj);
        }
    }
    return err;
}

_AFX afxError _AfxConstructObjects(afxManager *cls, afxNat cnt, afxObject objects[], void** udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
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
        base->cls = cls;

        base->refCnt = 1;
        AfxDeployList(&base->signaling);
        AfxDeployList(&base->handling);

        base->tid;
        AfxGetThreadingId(&base->tid);

        base->watchers = NIL;
        base->watching = NIL;

        objects[i] = base + 1; // move the pointer forward

        AfxAssert(AfxIsAligned(objects[i], sizeof(void*)));
        AfxAssert(base == (((afxObjectBase*)(objects[i])) - 1));

        cookie.no = i;

        if (_AfxObjCtor(cls, objects[i], &cookie))
        {
            AfxThrowError();

            for (afxNat j = i; j-- > 0;)
            {
                _AfxObjDtor(cls, objects[j]);
                afxObjectBase* base = objects[j];
                objects[j] = base - 1; // move the pointer backward
            }

            objects[i] = base - 1; // move the pointer backward
        }
    }

    return err;
}

_AFX afxError AfxCleanUpManager(afxManager *cls)
{
    afxError err = AFX_ERR_NONE;

    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);

    afxManager* base = AfxGetBaseClass(cls);

    if (base)
        AfxEcho("Cleaning up %s[%s]", cls->name, base->name);
    else
        AfxEcho("Cleaning up %s", cls->name);

    //AfxEnterSlockExclusive(&cls->slock);

    _AfxUninstallChainedClasses(&cls->deriveds);

    AfxAssert(0 == AfxGetChainLength(&cls->deriveds));

    afxObject obj;
    while (_AfxClsEnumerateObjects(cls, TRUE, 0, 1, &obj))
    {
        while (TRUE != AfxReleaseObjects(1, &obj));
    }

    AfxAssert(0 == cls->pool.totalUsedCnt);
    AfxCleanUpPool(&cls->pool);

    if (cls->base.chain)
        AfxPopLinkage(&cls->base);

    if (cls->provider.chain)
        AfxPopLinkage(&cls->provider);

    if (cls->mmu)
        AfxReleaseObjects(1, (void*[]) { cls->mmu });

    //AfxExitSlockExclusive(&cls->slock);
    //AfxReleaseSlock(&cls->slock);
    
    cls->fcc = NIL;

    return err;
}

_AFXINL afxError _AfxDummyObjDtor(afxObject obj)
{
    (void)obj;
    return 0;
}

_AFXINL afxError _AfxDummyObjCtor(afxObject obj, afxCookie const* cookie)
{
    (void)obj;
    (void)cookie;
    return 0;
}

_AFX afxError AfxSetUpManager(afxManager *cls, afxManager *base, afxChain* provider, afxClassConfig const *config)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssert(config);

    if (provider)
    {
        if (base)
        {
            AfxEcho("Setting up %s[%s]", config->name, base->name);
        }
        else
        {
            AfxEcho("Setting up %s", config->name);
        }
    }
    else
    {
        if (base)
        {
            AfxEcho("Setting up %s[%s]", config->name, base->name);
        }
        else
        {
            AfxEcho("Setting up %s", config->name);
        }
    }

    //AfxTakeSlock(&cls->slock);
    //AfxEnterSlockExclusive(&cls->slock);

    cls->fcc = afxFcc_CLS;
    AfxAssert(config->fcc);
    cls->objFcc = config->fcc;
    AfxAssert(config->size);
    cls->siz = config->size;

    if (!base)
    {
        AfxPushLinkage(&cls->base, NIL);
        cls->level = _AFX_CLASS_BASE_LEVEL;
    }
    else
    {
        AfxAssert(base != cls);
        AfxAssertType(base, afxFcc_CLS);
        AfxAssert(config->size >= base->siz);
        AfxClassLockExclusive(base);
        AfxPushLinkage(&cls->base, &base->deriveds);
        cls->level = base->level + 1;
        AfxClassUnlockExclusive(base);
    }

    cls->levelMask = (_AFX_CLASS_LEVEL_MASK << cls->level);
    cls->maxInstCnt = AfxMinorNonZero(config->maxCnt, AFX_N32_MAX);
    cls->ctor = config->ctor ? config->ctor : _AfxDummyObjCtor;
    cls->dtor = config->dtor ? config->dtor : _AfxDummyObjDtor;
    AfxAssert(config->name && config->name[0]);
    AfxStrcpy(cls->name, config->name ? config->name : "");
    cls->input = NIL;
    cls->output = NIL;
    cls->event = config->event;
    cls->eventFilter = config->eventFilter;
    cls->vmt = config->vmt;

    AfxSetUpPool(&cls->pool, AFX_ALIGN(cls->siz + sizeof(afxObjectBase), sizeof(void*)), config->unitsPerPage);

    if ((cls->mmu = config->mmu))
    {
        AfxAssertObjects(1, &cls->mmu, afxFcc_MMU);

        if (AfxReacquireObjects(1, (void*[]) { cls->mmu }))
            AfxThrowError();

        if (err)
            cls->mmu = NIL;
    }

    AfxSetUpChain(&cls->deriveds, cls);
    AfxPushLinkage(&cls->provider, provider ? provider : &orphanClassChain);

    //AfxExitSlockExclusive(&cls->slock);
    return err;
}

_AFX afxNat AfxIdentifyObject(afxObject obj)
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

_AFX afxError AfxAcquireObjects(afxManager *cls, afxNat cnt, afxObject objects[], void const* udd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(objects);

    if (cls->maxInstCnt && ((cls->maxInstCnt - cls->pool.totalUsedCnt) == 0)) AfxThrowError();
    else
    {
        afxObjectBase* ptr, *ptr2;

        if (_AfxAllocateObjects(cls, cnt, objects))
            AfxThrowError();
        else
        {
            if (_AfxConstructObjects(cls, cnt, objects, (void**)udd))
            {
                AfxThrowError();
                _AfxDeallocateObjects(cls, cnt, objects);
            }
            else
            {
                AfxAssertObjects(cnt, objects, cls->objFcc);
            }
        }
    }
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
            afxObjectBase* inst = (afxObjectBase*)(obj);
            --inst;
            AfxAssertType(inst, afxFcc_OBJ);
            ++inst->refCnt;
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

            afxInt refCnt = --base->refCnt;

            if (refCnt == 0)
            {
                ++rslt;
                afxManager* cls = base->cls;
                AfxAssertType(cls, afxFcc_CLS);

                // se der erro aqui, é porque você provavelmente está passando uma array como elemento aninhado (ex.: AfxReleaseObjects(n, (void*[]){ array })) ao invés de passar a array diretamente (ex.: AfxReleaseObjects(n, array));

                _AfxDestructObjects(cls, 1, &obj);
                _AfxDeallocateObjects(cls, 1, &obj);
            }
        }
    }
    return rslt;
}

_AFX afxResult _AfxAssertObjects(afxNat cnt, afxObject const objects[], afxFcc fcc)
{
    afxError err = NIL;
    AfxAssert(objects);
    AfxAssert(cnt);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxObject obj = objects[i];

        if (!obj) err = -1;
        else
        {
            afxObjectBase* base = obj;
            --base;
            AfxAssertType(base, afxFcc_OBJ);
            afxManager* cls = base->cls;
            AfxAssertType(cls, afxFcc_CLS);

            do if (cls->objFcc == fcc) break;
            while ((cls = AfxGetBaseClass(cls)));

            AfxAssertClass(cls, fcc);

            if (err)
            {
                int a = 0;
            }
        }

        if (!err)
            ++rslt;
    }
    return rslt;
}

_AFX afxResult AfxWaitForObject(afxObject obj, afxTime timeout)
{
    afxError err = NIL;
    AfxAssert(obj);
    return 0;
}
