/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_OBJECT_H
#define AFX_OBJECT_H

#include <stdarg.h>
#include "afx/core/afxDebug.h"
#include "afx/core/afxFcc.h"
#include "afx/core/afxChain.h"
#include "afx/core/afxString.h"
#include "afx/core/afxEvent.h"
//#include "afx/core/afxObjack.h"

// HANDLES AND OBJECTS
//
// An object is a data structure that represents a system resource, such as a file, thread, or graphic image.
// Your application canv't directly access object data, nor the system resource that an object represents.
// Instead, your application must obtain an object handle, which it canv use to examine or modify the system resource.
// Each handle has an entry in an internally maintained table.
// Those entries contain the addresses of the resources, and the means to identify the resource type.
//

#define AFX_OBJ_NAME_LEN 32
#define AFX_OBJ_EVENT_LAYER 0

AFX_DECLARE_STRUCT(afxInstance);
AFX_DECLARE_STRUCT(afxClass);

//#include "../mem/afxArray.h"

typedef enum afxInstanceFlag
{
    AFX_OBJ_FLAG_READY  = (1 << 0),
    AFX_OBJ_FLAG_LOCKED = (1 << 2)
} afxInstanceFlag;

typedef afxBool (*afxEventFilterFn)(afxInstance *obj, afxInstance *watched, afxEvent *ev);

AFX_DEFINE_STRUCT(afxInstance)
{
    afxFcc              fcc; // OBJ
    afxClass*           cls;
    afxNat              instIdx;
    afxInt32            refCnt;
    afxList             signaling; // slot holders
    afxList             handling; // slot helds
    
    afxChain            *watchers;
    afxChain            *watching;
    afxNat32            tid;
    afxInstanceFlag       flags;
};

AFX_DEFINE_STRUCT(afxEventFilter)
{
    afxLinkage          holder; // obj->watching
    afxLinkage          watched; // obj->watchers
    //afxEventFilterFn    fn;
};

AFX afxError AfxObjectInstallEventFilter(afxHandle obj, afxHandle filter); // if filter is already filtering this object, callback will be replaced.
AFX afxError AfxObjectRemoveEventFilter(afxHandle obj, afxHandle filter);
AFX afxBool AfxObjectEmitEvent(afxHandle obj, afxEvent *ev);

AFX_DEFINE_STRUCT(afxConnection)
{
    afxFcc              fcc; // objc
    afxLink             holderLink;
    afxHandle           holder; // observing
    void                (*handler)(afxHandle, afxEvent*);
    afxNat32            filter;
    afxLink             objLink;
    union
    {
        afxHandle       obj; // signaling
        void            *voidObj;
    };
    afxBool             reacquired;
    afxBool             alloced;
    afxInt32            tenacity; // reference counter
};

AFX afxResult           AfxObjectTestFcc(afxHandle obj, afxFcc fcc);
AFX afxFcc              AfxObjectGetFcc(afxHandle obj);
AFX afxChar const*      AfxObjectGetFccAsString(afxHandle obj);

AFX afxClass*           AfxObjectGetClass(afxHandle obj);
AFX void*               AfxGetObjectProvider(afxHandle obj);
AFX afxNat              AfxObjectGetIndex(afxHandle obj, afxBool b2f);
AFX void const*         AfxObjectGetVmt(afxHandle obj);
AFXINL afxBool          AfxObjectInherits(afxHandle obj, afxClass const* cls);

AFX afxInt32            AfxObjectGetRefCount(afxHandle obj);

AFX afxNat32            AfxObjectGetTid(afxHandle obj);

AFX afxResult           AfxObjectSignalConnections(afxHandle obj, afxEventType evtype, void *data);
AFX afxConnection*      AfxObjectConnect(afxHandle obj, afxHandle holder, void(*handler)(afxHandle,afxEvent*), afxNat32 filter, afxConnection *objc);
AFX afxResult           AfxObjectDisconnect(afxHandle obj, afxHandle holder, void(*handler)(afxHandle,afxEvent*));

AFX afxInstanceFlag       AfxObjectSetFlags(afxHandle obj, afxInstanceFlag flags);
AFX afxInstanceFlag       AfxObjectClearFlags(afxHandle obj, afxInstanceFlag flags);
AFX afxBool             AfxObjectTestFlags(afxHandle obj, afxInstanceFlag flags);

// SLOT UTILS

AFXINL afxError         AfxConnectionReset(afxConnection *objc); // initializes a connection slot.
AFXINL afxError         AfxConnectionDrop(afxConnection *objc);

AFXINL afxInt32         AfxConnectionIncTenacity(afxConnection *objc);
AFXINL afxInt32         AfxConnectionDecTenacity(afxConnection *objc);

AFXINL void*            AfxConnectionGetObject(afxConnection const *objc);
AFXINL void*            AfxConnectionGetHolder(afxConnection const *objc);

AFXINL afxError         AfxConnectionSetHandler(afxConnection *objc, void(*handler)(afxHandle, afxEvent*));
AFXINL afxError         AfxConnectionSetFilter(afxConnection *objc, afxNat32 filter);


// %p?%.4s#%i
#define AfxPushObject(obj_) 0,0,0//(obj_), (obj_) ? AfxObjectGetFccAsString((afxInstance*)obj_) : NIL, (obj_) ? ((afxInstance*)obj_)->refCnt : 0


#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))

#   define AfxAssertConnection(var_) ((!!((var_) && ((var_)->fcc == afxFcc_OBJC)))||(err = (afxError)__LINE__,AfxLogError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[AFXERR_INVALID]),0))

#   define AfxAssertObject(obj_, fcc_) (err = AfxObjectAssert(((afxHandle)obj_), (fcc_), AfxSpawnHint(), AfxStr((obj_))));
#   define AfxTryAssertObject(obj_, fcc_) ((!obj_) || ((obj_) && (err = AfxObjectAssert(((afxHandle)obj_), (fcc_), AfxSpawnHint(), AfxStr((obj_))))));

#else

#   define AfxAssertConnection(var_) ((void)(err))

#   define AfxAssertObject(obj_, fcc_) ((void)(err))
#   define AfxTryAssertObject(obj_, fcc_) ((void)(err))

#endif



typedef struct
{
    _AFX_DBG_FCC
    afxSize     data[2];
    afxNat      refCnt;
    afxFlags    flags;
    char const* name;
}
afxObjectData;

//AFX_DEFINE_HANDLE(afxHandle);

AFX_DEFINE_STRUCT(afxObjectBus)
{
//    afxSlock    lock;
    afxNat      lockReqCnt;
    afxNat      objCnt;
};

AFX_OBJECT(afxHandlse)
{
    afxObjectBus*   bus;
    afxChain        aggregations;
    void*           bindings;
    afxNat          bindCnt;
    afxNat          refCnt;
    afxFlags        flags;
    char const*     name;
};

AFX_DEFINE_STRUCT(afxAggregation)
{
    afxFcc      fcc;
    afxLinkage  aggr;
    afxLinkage  obj;
};

AFX_DEFINE_STRUCT(afxAggregator)
{
    afxLinkage  provider;
    afxChain    aggregations;
    afxChain    unused;
    afxNat      chainLockReqCnt;
//    afxSlock    chainLock;
    afxNat      maxInstCnt; // limits existence of certain amount of items.
    afxNat      minInstCnt; // limits existence of unused
    afxError    (*deploy)(afxNat, afxHandle[], afxAggregator*, void*);
    afxError    (*drop)(afxNat, afxHandle[], afxAggregator*, void*);
};

AFX afxError    AfxAcquireObjects(afxClass* cls, afxNat cnt, afxHandle obj[], void** udd);
AFX afxError    AfxReacquireObjects(afxNat cnt, afxHandle obj[]);
AFX afxBool     AfxReleaseObjects(afxNat cnt, void* obj[]);

AFX afxResult   _AfxAssertObjects(afxNat cnt, afxHandle const obj[], afxFcc fcc);
#define         AfxAssertObjects(cnt_, objs_, fcc_) AfxAssert(((afxResult)(cnt_)) == _AfxAssertObjects((cnt_), (afxHandle const*)(objs_),(fcc_)));
#define         AfxTryAssertObjects(cnt_, objs_, fcc_) AfxAssert((!(((afxHandle const*)objs_)[0])) || (((afxResult)(cnt_)) == _AfxAssertObjects((cnt_), (afxHandle const*)(objs_),(fcc_))));

AFX afxNat      AfxIdentifyObject(afxHandle obj);

#if 0
AFX afxError    AfxAggregateObjects(afxNat cnt, afxHandle obj[], afxAggregator* aggr, void *udd);
AFX afxError    AfxDisaggregateObjects(afxNat cnt, afxHandle obj[], afxAggregator* aggr, void *udd);

AFX afxError    AfxAcquireAggregator(afxAggregator* aggr, afxChain* provider);

AFX afxError AfxAcquireObjectBus(afxObjectBus* bus);
AFX afxBool AfxLockObjectBus(afxObjectBus* bus, afxBool modify, afxBool attemp);

AFX afxError AfxRegroupObjects(afxNat cnt, afxHandle obj[], afxObjectBus* bus);
AFX afxError AfxUngroupObjects(afxNat cnt, afxHandle obj[]);

AFX afxError AfxAcquireAggregator(afxAggregator* aggr, afxChain* provider);

AFX afxHandle AfxGetObject(afxAggregation* aggn);

AFX afxAggregator* AfxGetAggregator(afxAggregation* aggn);

AFX afxNat AfxCurateAggregations(afxNat cnt, afxHandle obj[], afxBool(*f)(afxAggregation*,void*), void *udd);
AFX afxNat AfxCurateSpecificAggregations(afxNat cnt, afxHandle obj[], afxFcc fcc, afxBool(*f)(afxAggregation*,void*), void *udd);

AFX afxNat AfxCurateInactiveAggregations(afxAggregator* aggr, afxBool(*f)(afxAggregation*, void*), void *udd);
AFX afxNat AfxCurateActiveAggregations(afxAggregator* aggr, afxBool(*f)(afxAggregation*, void*), void *udd);


AFX afxNat AfxCurateAggregations(afxNat cnt, afxHandle obj[], afxBool(*f)(afxAggregation*, void*), void *udd)
{
    afxError err = NIL;
    AfxAssert(cnt);
    AfxAssert(obj);
    AfxAssert(f);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxAggregation* aggn;
        AfxForEachListElement(&obj[i]->aggregations, afxAggregation, obj, aggn)
        {
            if (f(aggn, udd))
                break;
        }
    }

    return rslt;
}

AFX afxNat AfxCurateSpecificAggregations(afxNat cnt, afxHandle obj[], afxFcc fcc, afxBool(*f)(afxAggregation*, void*), void *udd)
{
    afxError err = NIL;
    AfxAssert(cnt);
    AfxAssert(obj);
    AfxAssert(f);
    afxNat rslt = 0;

    for (afxNat i = 0; i < cnt; i++)
    {
        afxAggregation* aggn;
        AfxForEachListElement(&obj[i]->aggregations, afxAggregation, obj, aggn)
        {
            if ((!fcc || fcc == aggn->fcc) && f(aggn, udd))
                break;
        }
    }
    return rslt;
}

AFX afxNat AfxCurateInactiveAggregations(afxAggregator* aggr, afxBool(*f)(afxAggregation*, void*), void *udd)
{
    afxError err = NIL;
    AfxAssert(aggr);
    AfxAssert(f);
    afxNat rslt = 0;

    afxAggregation* aggn;
    AfxForEachListElement(&aggr->unused, afxAggregation, aggr, aggn)
    {
        if (f(aggn, udd))
            break;
    }
    return rslt;
}

AFX afxNat AfxCurateActiveAggregations(afxAggregator* aggr, afxBool(*f)(afxAggregation*, void*), void *udd)
{
    afxError err = NIL;
    AfxAssert(aggr);
    AfxAssert(f);
    afxNat rslt = 0;

    afxAggregation* aggn;
    AfxForEachListElement(&aggr->aggregations, afxAggregation, aggr, aggn)
    {
        if (f(aggn, udd))
            break;
    }
    return rslt;
}

#endif

#endif//AFX_OBJECT_H