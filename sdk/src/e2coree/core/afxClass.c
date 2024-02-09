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

#include "qwadro/core/afxClass.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/async/afxTxu.h"
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
            afxClass *cls = AFX_REBASE(first, afxClass, provider);
            AfxAssertType(cls, afxFcc_CLS);
            ++cnt;

            if (cls->objFcc == afxFcc_SYS)
                break; // has only the SYS. We can't drop it or will shit the entire platform. Break.
            else
                AfxDismountClass(cls);
        }
    }
    return cnt;
}

_AFX afxResult _AfxShutdownOrphanClasses(void)
{
    return _AfxUninstallChainedClasses(&orphanClassChain);
}

_AFX void AfxClassLockInclusive(afxClass *cls)
{
    //AfxEnterSlockShared(&cls->slock);
}

_AFX void AfxClassLockExclusive(afxClass *cls)
{
    //AfxEnterSlockExclusive(&cls->slock);
}

_AFX void AfxClassUnlockInclusive(afxClass *cls)
{
    //AfxExitSlockShared(&cls->slock);
}

_AFX void AfxClassUnlockExclusive(afxClass *cls)
{
    //AfxExitSlockExclusive(&cls->slock);
}

_AFX afxBool AfxClassTryLockInclusive(afxClass *cls)
{
    return 1;// AfxTryEnterSlockShared(&cls->slock);
}

_AFX afxBool AfxClassTryLockExclusive(afxClass *cls)
{
    return 1;// AfxTryEnterSlockExclusive(&cls->slock);
}

_AFXINL afxClass* AfxGetBaseClass(afxClass const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    return cls->base.chain ? cls->base.chain->owner : NIL;
}

_AFXINL void const* AfxClassGetVmt(afxClass const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    return cls->vmt;
}

_AFXINL afxNat AfxGetInstanceSize(afxClass const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    return cls->siz;
}

_AFXINL afxNat AfxGetInstanceAddedSsize(afxClass const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    afxClass *base = AfxGetBaseClass(cls);
    return (base ? cls->siz - base->siz : cls->siz);
}

_AFXINL afxNat AfxCountInstances(afxClass const *cls)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    return cls->pool.totalUsedCnt;
}

_AFXINL afxBool AfxGetInstance(afxClass const* cls, afxNat32 uniqueId, afxObject* obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    //return AfxGetPoolUnit(&cls->pool, uniqueId, obj);
    return AfxEnumerateInstances(cls, uniqueId, 1, obj);
}

_AFXINL afxNat _AfxClsEnumerateInstances(afxClass const* cls, afxBool fromLast, afxNat first, afxNat cnt, afxObject obj[])
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
        rslt = cnt ? AfxEnumeratePoolItems(&cls->pool, first, cnt, (void**)obj) : 0;

        if (rslt)
        {
            AfxAssert(cnt >= rslt);

            for (afxNat i = 0; i < rslt; i++)
            {
                afxHandle* inst = obj[i];
                AfxAssertType(inst, afxFcc_OBJ);
                //afxModule inst2 = (inst + 1);
                //AfxAssertObjects(1, &inst2, cls->objFcc);
                obj[i] = (inst + 1);
                AfxAssertObjects(1, &obj[i], cls->objFcc);
            }

            if (first && (first >= rslt))
                first -= rslt;

            cnt = cnt - rslt;
        }
    }

    if (cnt)
    {
        afxClass *superset;
        afxChain const *supersets = &cls->deriveds;
        AfxChainForEveryLinkageB2F(supersets, afxClass, base, superset)
        {
            AfxAssertType(superset, afxFcc_CLS);

            if (first && (first >= superset->pool.totalUsedCnt))
            {
                first -= superset->pool.totalUsedCnt;
            }
            else
            {
#if !0
                afxNat rslt2 = _AfxClsEnumerateInstances(superset, fromLast, first, cnt, &obj[rslt]);

                AfxAssert(cnt >= rslt2); // cant be greater

                if (first && (first >= rslt2))
                    first -= rslt2;

                cnt = cnt - rslt2;
                rslt += rslt2;

#else
                afxNat rslt2 = _AfxClsEnumerateInstances(superset, fromLast, first2, cnt2, &obj[rslt]);

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

_AFXINL afxNat AfxEnumerateInstances(afxClass const* cls, afxNat first, afxNat cnt, afxObject obj[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    afxNat rslt = 0;

    AfxClassLockInclusive((afxClass*)cls);
    rslt = _AfxClsEnumerateInstances(cls, FALSE, first, cnt, obj);
    AfxClassUnlockInclusive((afxClass*)cls);
    return rslt;
}

_AFXINL afxNat _AfxClsCurateInstances(afxClass const* cls, afxBool fromLast, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(f);
    afxNat rslt = 0;

    afxNat i = 0;
    afxObject obj;
    while (_AfxClsEnumerateInstances(cls, fromLast, first + i, 1, &obj))
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

AFXINL afxNat AfxInvokeInstances(afxClass const* cls, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void *udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(f);
    afxNat rslt = 0;
    AfxClassLockInclusive((afxClass*)cls);
    rslt = _AfxClsCurateInstances(cls, FALSE, first, cnt, f, udd);
    AfxClassUnlockInclusive((afxClass*)cls);
    return rslt;
}

#if 0
_AFXINL afxError AfxClassDeallocateObjects(afxClass *cls, afxNat cnt, afxObject obj[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(obj);
    AfxEcho("afxClass<%s>::DeallocateObjects(%d, %p)", cls->name, cnt, obj);
    
    for (afxNat i = 0; i < cnt; i++)
    {
        afxHandle *item = (afxHandle *)obj[i];
        AfxAssert(item);
        --item;
        AfxDeallocatePoolUnit(&cls->pool, item->instIdx);
    }
    return err;
}

_AFXINL afxError AfxClassAllocateObjects(afxClass *cls, afxNat cnt, afxObject obj[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxEcho("afxClass<%s>::AllocateObjects(%d, %p, %p)", cls->name, cnt, obj);
    
    if (cls->maxInstCnt && ((cls->maxInstCnt - cls->pool.totalUsedCnt) == 0)) AfxThrowError();
    else
    {
        for (afxNat i = 0; i < cnt; i++)
        {
            afxHandle* ptr;
            afxSize idx;
            AfxAllocatePoolUnit(&cls->pool, &idx);
            AfxGetPoolUnit(&cls->pool, idx, (void**)&ptr);
            ptr->instIdx = idx;
            obj[i] = (afxObject)(ptr + 1);

            if (err)
            {
                if (i > 0)
                    AfxClassDeallocateObjects(cls, i, obj);

                break;
            }
        }
    }
    return err;
}
#endif

_AFXINL afxError _AfxClsRunSubsetDtor(afxClass *cls, afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);

    AfxEcho("Destructing instance %p of %s class.", obj, cls->name);

    if (cls->dtor && cls->dtor(obj))
        AfxThrowError();

    if (cls->base.chain)
    {
        AfxEcho("Destructing instance %p of %s : %s class.", obj, cls->name, ((afxClass*)cls->base.chain->owner)->name);

        if (_AfxClsRunSubsetDtor(cls->base.chain->owner, obj))
            AfxThrowError();
    }

    return err;
}

_AFX afxError AfxClassDismantleObjects(afxClass *cls, afxNat cnt, afxObject obj[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(obj);
 
    for (afxNat i = cnt; i-- > 0;)
    {
        afxObject item = obj[i];
        AfxEcho("Dismantling %s object (#%d) %p...", cls->name, i, obj[i]);
        afxHandle *itemD = (afxHandle *)item;
        --itemD;
        AfxAssertType(itemD, afxFcc_OBJ);
        AfxAssert(AfxObjectGetClass(item) == cls);

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
                    AfxAssert(flt->holder.chain->owner == item);
                    //AfxAssertType((afxHandle*)flt->watched.chain->owner, afxFcc_OBJ);
                    AfxObjectRemoveEventFilter(flt->watched.chain->owner, item);
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
                    AfxAssert(flt->watched.chain->owner == item);
                    //AfxAssertType((afxHandle*)flt->holder.chain->owner, afxFcc_OBJ);
                    AfxObjectRemoveEventFilter(item, flt->holder.chain->owner);
                }
            }
        }

        if (_AfxClsRunSubsetDtor(cls, item))
            AfxThrowError();

        AfxZero(1, sizeof(*itemD), itemD);
        //AfxDeallocate(NIL, cls->all, item);

        //AfxEcho("Instance %p[%.4s]^%i destroyed.", item, (afxChar const*)&(fcc), refCnt);
    }
    return err;
}

_AFXINL afxError _AfxClsRunSubsetCtor(afxClass *cls, afxObject obj, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);

    if (cls->base.chain)
    {
        AfxEcho("afxClass<%s, %s>ConstructingObject(%p)", cls->name, ((afxClass*)cls->base.chain->owner)->name, obj);
        
        if (_AfxClsRunSubsetCtor(cls->base.chain->owner, obj, cookie))
            AfxThrowError();
    }

    if (!err)
    {
        AfxEcho("afxClass<%s>::ConstructObject(%p)", cls->name, obj);

        if (cls->ctor && cls->ctor(obj, cookie))
        {
            AfxThrowError();

            if (cls->base.chain)
                _AfxClsRunSubsetDtor(cls, obj);
        }
    }
    return err;
}

_AFX afxError AfxClassBuildObjects(afxClass *cls, afxNat cnt, afxObject obj[], void* udd)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(obj);
    //AfxAssert(blueprint);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxObject item = obj[i];
        AfxEcho("Building %s object (#%d*) %p...", cls->name, i, obj[i]);
        afxHandle *itemD = (afxHandle *)item;
        AfxAssert(item);
        --itemD;
        //AfxAssertType(item, afxFcc_OBJ);
        AfxZero(1, sizeof(*itemD), itemD);

        itemD->fcc = afxFcc_OBJ;
        itemD->cls = cls;

        itemD->refCnt = 1;
        AfxDeployList(&itemD->signaling);
        AfxDeployList(&itemD->handling);

        itemD->tid;
        AfxGetThreadingId(&itemD->tid);

        itemD->watchers = NIL;
        itemD->watching = NIL;

        AfxClassLockExclusive(cls);
        
        if (_AfxClsRunSubsetCtor(cls, item, udd))
            AfxThrowError();
        
        AfxClassUnlockExclusive(cls);

        if (!err)
        {
            //afxFcc fcc = AfxObjectGetFcc(item);
            //AfxEcho("Instance %p[%.4s]^%i built.", item, (afxChar const*)&(fcc), item->refCnt);
            AfxAssert(itemD->refCnt == 1);
        }
        else
        {
            AfxClassDismantleObjects(cls, i, obj);

            break;
        }
    }
    return err;
}

_AFX afxError AfxDismountClass(afxClass *cls)
{
    afxError err = AFX_ERR_NONE;

    AfxAssert(cls);
    AfxAssertType(cls, afxFcc_CLS);
    AfxEcho("Dismounting %s : %s class... (%p)", cls->name, cls->base.chain ? ((afxClass*)cls->base.chain)->name : "", cls);

    //AfxEnterSlockExclusive(&cls->slock);

    _AfxUninstallChainedClasses(&cls->deriveds);

    AfxAssert(0 == AfxGetChainLength(&cls->deriveds));

    afxObject obj;
    while (_AfxClsEnumerateInstances(cls, TRUE, 0, 1, &obj))
    {
        while (TRUE != AfxReleaseObjects(1, &obj));
    }

    AfxAssert(0 == cls->pool.totalUsedCnt);
    AfxReleasePool(&cls->pool);

    if (cls->base.chain)
        AfxPopLinkage(&cls->base);

    if (cls->provider.chain)
        AfxPopLinkage(&cls->provider);

    if (cls->mmu)
        AfxReleaseObjects(1, (void*[]) { cls->mmu });

    //AfxExitSlockExclusive(&cls->slock);
    //AfxReleaseSlock(&cls->slock);
    return err;
}

_AFXINL afxError _AfxClsDummyDtor(afxObject obj)
{
    (void)obj;
    return 0;
}

_AFXINL afxError _AfxClsDummyCtor(afxObject obj, afxCookie const* cookie)
{
    (void)obj;
    (void)cookie;
    return 0;
}

_AFX afxError AfxMountClass(afxClass *cls, afxClass *base, afxChain* provider, afxClassConfig const *config)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(cls);
    AfxAssert(config);

    if (provider)
    {
        if (base)
        {
            AfxEcho("Installing %s at %p by %p over %p...", config->name, cls, provider, base);
        }
        else
        {
            AfxEcho("Installing %s at %p by %p...", config->name, cls, provider);
        }
    }
    else
    {
        if (base)
        {
            AfxEcho("Installing %s at %p over %p...", config->name, cls, base);
        }
        else
        {
            AfxEcho("Installing %s at %p...", config->name, cls);
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
    cls->maxInstCnt = config->maxCnt;
    cls->ctor = config->ctor ? config->ctor : _AfxClsDummyCtor;
    cls->dtor = config->dtor ? config->dtor : _AfxClsDummyDtor;
    AfxAssert(config->name && config->name[0]);
    AfxStrcpy(cls->name, config->name ? config->name : "");
    cls->input = NIL;
    cls->output = NIL;
    cls->event = config->event;
    cls->eventFilter = config->eventFilter;
    cls->vmt = config->vmt;

    AfxTakePool(&cls->pool, AFX_ALIGN(cls->siz + sizeof(afxHandle), sizeof(void*)), config->unitsPerPage);

    if ((cls->mmu = config->mmu))
    {
        AfxAssertObjects(1, &cls->mmu, afxFcc_MMU);

        if (AfxReacquireObjects(1, (void*[]) { cls->mmu }))
            AfxThrowError();

        if (err)
            cls->mmu = NIL;
    }

    AfxTakeChain(&cls->deriveds, cls);
    AfxPushLinkage(&cls->provider, provider ? provider : &orphanClassChain);

    //AfxExitSlockExclusive(&cls->slock);
    return err;
}

_AFX afxNat AfxIdentifyObject(afxObject obj)
{
    afxError err = AFX_ERR_NONE;
    afxHandle* inst = (afxHandle*)obj;
    --inst;
    AfxAssertType(inst, afxFcc_OBJ);
    
    afxPool* pool = &(AfxObjectGetClass(obj)->pool);
    AfxAssertType(pool, afxFcc_POOL);
    afxNat idx = AFX_INVALID_INDEX;
    AfxFindPoolUnitIndex(pool, (afxByte*)inst, &idx, NIL);
    return idx;
}

_AFX afxError AfxAcquireObjects(afxClass *cls, afxNat cnt, afxObject obj[], void const* udd[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertType(cls, afxFcc_CLS);
    AfxAssert(cnt);
    AfxAssert(obj);

    if (cls->maxInstCnt && ((cls->maxInstCnt - cls->pool.totalUsedCnt) == 0)) AfxThrowError();
    else
    {
        AfxClassLockExclusive(cls);
        AfxClassUnlockExclusive(cls);
        AfxClassLockExclusive(cls);
        afxHandle* ptr, *ptr2;

        for (afxNat i = 0; i < cnt; i++)
        {
            afxSize idx;
            
            if (!(ptr = (void*)AfxAllocatePoolUnit(&cls->pool, &idx))) AfxThrowError();
            else
            {
                AfxAssert(AfxIsAligned(ptr, sizeof(void*)));

                if (!(AfxGetPoolUnit(&cls->pool, idx, (void**)&ptr2))) AfxThrowError();
                else
                {
                    AfxAssert(AfxIsAligned(ptr2, sizeof(void*)));
                    AfxAssert(ptr == ptr2);
                    ptr->instIdx = (afxNat)idx;
                    ptr->fcc = afxFcc_OBJ;
                    ptr->cls = cls;

                    ptr->refCnt = 1;
                    AfxDeployList(&ptr->signaling);
                    AfxDeployList(&ptr->handling);

                    ptr->tid;
                    AfxGetThreadingId(&ptr->tid);

                    ptr->watchers = NIL;
                    ptr->watching = NIL;

                    obj[i] = ptr + 1;
                    AfxAssert(AfxIsAligned(obj[i], sizeof(void*)));
                    AfxAssert(ptr == (((afxHandle*)(obj[i])) - 1));
                    afxNat idxTmp = AfxIdentifyObject(obj[i]);
                    AfxAssert(idx == idxTmp);
                }

                if (err)
                    AfxDeallocatePoolUnit(&cls->pool, (afxByte*)ptr);
            }

            if (err)
            {
                for (afxNat j = i; j--> 0;)
                {
                    ptr = obj[j];
                    --ptr;
                    AfxAssertType(ptr, afxFcc_OBJ);
                    obj[j] = ptr;
                }
                
                if (i)
                    AfxDeallocatePoolUnits(&cls->pool, i, (afxByte**)obj);

                break;
            }
        }

        if (!err) // initialize
        {
            afxCookie cookie = { 0 };
            cookie.udd = (void**)udd;

            for (afxNat i = 0; i < cnt; i++)
            {
                cookie.no = i;

                AfxAssertObjects(1, &obj[i], cls->objFcc);

                if (_AfxClsRunSubsetCtor(cls, obj[i], &cookie))
                    AfxThrowError();

                AfxAssertObjects(1, &obj[i], cls->objFcc);

                if (err)
                {
                    if (i)
                        AfxClassDismantleObjects(cls, i, obj);

                    break;
                }
            }

            if (err)
            {
                for (afxNat j = cnt; j-- > 0;)
                {
                    ptr = obj[j];
                    --ptr;
                    AfxAssertType(ptr, afxFcc_OBJ);
                    obj[j] = ptr;
                }

                AfxDeallocatePoolUnits(&cls->pool, cnt, (afxByte**)obj);
            }
        }

        AfxClassUnlockExclusive(cls);
        int a = 0;
    }

    if (!err)
    {
        AfxAssertObjects(cnt, obj, cls->objFcc);
    }
    return err;
}

_AFX afxError AfxReacquireObjects(afxNat cnt, afxObject obj[])
{
    afxError err = NIL;
    AfxAssert(cnt);
    AfxAssert(obj);

    for (afxNat i = 0; i < cnt; i++)
    {
        afxHandle* inst = (afxHandle*)(obj[i]);
        --inst;
        AfxAssertType(inst, afxFcc_OBJ);
        ++inst->refCnt;
    }
    return err;
}

_AFX afxBool AfxReleaseObjects(afxNat cnt, void* obj[])
{
    afxError err = NIL;
    AfxAssert(cnt);
    AfxAssert(obj);
    afxBool rslt = 0;

    for (afxNat i = cnt; i--> 0;)
    {
        afxHandle* inst = (afxHandle*)(obj[i]);
        --inst;
        AfxAssertType(inst, afxFcc_OBJ);
        
        if (inst->refCnt && (0 == --inst->refCnt))
        {
            ++rslt;
            afxClass* cls = inst->cls;
            AfxAssertType(cls, afxFcc_CLS);
            AfxClassLockExclusive(cls);

            AfxAssert(inst->instIdx == AfxIdentifyObject(obj[i]));

            // se der erro aqui, é porque você provavelmente está passando uma array como elemento aninhado (ex.: AfxReleaseObjects(n, (void*[]){ array })) ao invés de passar a array diretamente (ex.: AfxReleaseObjects(n, array));

            if (err)
            {
                int a = 0;
            }


            AfxClassDismantleObjects(cls, 1, &obj[i]);


            AfxDeallocatePoolUnit(&cls->pool, (void*)inst);
            AfxClassUnlockExclusive(cls);
        }
    };

    return rslt;
}

_AFX afxResult _AfxAssertObjects(afxNat cnt, afxObject const obj[], afxFcc fcc)
{
    afxError err = NIL;
    AfxAssert(cnt);
    AfxAssert(obj);
    afxResult rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        if (!obj[i]) err = -1;
        else
        {
            afxHandle* inst = obj[i];
            --inst;
            AfxAssertType(inst, afxFcc_OBJ);
            afxClass* cls = inst->cls;
            AfxAssertType(cls, afxFcc_CLS);

            do if (cls->objFcc == fcc) break;
            while ((cls = AfxGetBaseClass(cls)));

            AfxAssertClass(cls, fcc);
            AfxAssert(inst->instIdx == AfxIdentifyObject(obj[i]));

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
