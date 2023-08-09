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
#include "afx/core/diag/afxDebug.h"
#include "afx/core/afxFcc.h"
#include "afx/core/afxChain.h"
#include "afx/core/afxString.h"
#include "afx/core/afxEvent.h"

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
    afxFcc              fcc;
    afxLinkage          cls;
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

AFX afxError AfxObjectInstallEventFilter(afxInstance *obj, afxInstance *filter); // if filter is already filtering this object, callback will be replaced.
AFX afxError AfxObjectRemoveEventFilter(afxInstance *obj, afxInstance *filter);
AFX afxBool AfxObjectEmitEvent(afxInstance *obj, afxEvent *ev);

AFX_DEFINE_STRUCT(afxConnection)
{
    afxFcc              fcc; // objc
    afxLink             holderLink;
    afxInstance           *holder; // observing
    void                (*handler)(afxInstance*, afxEvent*);
    afxNat32            filter;
    afxLink             objLink;
    union
    {
        afxInstance       *obj; // signaling
        void            *voidObj;
    };
    afxBool             reacquired;
    afxBool             alloced;
    afxInt32            tenacity; // reference counter
};

AFX afxResult           AfxObjectReacquire(afxInstance *obj, afxInstance *holder, void(*handler)(afxInstance*, afxEvent*), afxNat32 filter, afxConnection *objc);
AFX afxResult           AfxReleaseObject(afxInstance *obj);

AFX afxResult           AfxObjectTestFcc(afxInstance const *obj, afxFcc fcc);
AFX afxFcc              AfxObjectGetFcc(afxInstance const *obj);
AFX afxChar const*      AfxObjectGetFccAsString(afxInstance const *obj);

AFX afxError            AfxObjectAssert(afxInstance const *obj, afxFcc fcc, afxHint const hint, afxChar const *exp);
AFX afxClass*           AfxObjectGetClass(afxInstance const *obj);
AFX void*               AfxObjectGetProvider(afxInstance const *obj);
AFX afxNat              AfxObjectGetIndex(afxInstance const *obj, afxBool b2f);
AFX void const*         AfxObjectGetVmt(afxInstance const *obj);
AFXINL afxBool          AfxObjectInherits(afxInstance const *obj, afxClass const* cls);

AFX afxInt32            AfxObjectGetRefCount(afxInstance const *obj);

AFX afxNat32            AfxObjectGetTid(afxInstance const *obj);

AFX afxResult           AfxObjectSignalConnections(afxInstance *obj, afxEventType evtype, void *data);
AFX afxConnection*      AfxObjectConnect(afxInstance *obj, afxInstance *holder, void(*handler)(afxInstance*,afxEvent*), afxNat32 filter, afxConnection *objc);
AFX afxResult           AfxObjectDisconnect(afxInstance *obj, afxInstance *holder, void(*handler)(afxInstance*,afxEvent*));

AFX afxInstanceFlag       AfxObjectSetFlags(afxInstance *obj, afxInstanceFlag flags);
AFX afxInstanceFlag       AfxObjectClearFlags(afxInstance *obj, afxInstanceFlag flags);
AFX afxBool             AfxObjectTestFlags(afxInstance const *obj, afxInstanceFlag flags);

// SLOT UTILS

AFXINL afxError         AfxConnectionReset(afxConnection *objc); // initializes a connection slot.
AFXINL afxError         AfxConnectionDrop(afxConnection *objc);

AFXINL afxInt32         AfxConnectionIncTenacity(afxConnection *objc);
AFXINL afxInt32         AfxConnectionDecTenacity(afxConnection *objc);

AFXINL void*            AfxConnectionGetObject(afxConnection const *objc);
AFXINL void*            AfxConnectionGetHolder(afxConnection const *objc);

AFXINL afxError         AfxConnectionSetHandler(afxConnection *objc, void(*handler)(afxInstance*, afxEvent*));
AFXINL afxError         AfxConnectionSetFilter(afxConnection *objc, afxNat32 filter);


// %p?%.4s#%i
#define AfxPushObject(obj_) (obj_), (obj_) ? AfxObjectGetFccAsString((afxInstance*)obj_) : NIL, (obj_) ? ((afxInstance*)obj_)->refCnt : 0


#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))

#   define AfxAssertConnection(var_) ((!!((var_) && ((var_)->fcc == AFX_FCC_OBJC)))||(err = (afxError)__LINE__,AfxLogError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[AFXERR_INVALID]),0))

#   define AfxAssertObject(obj_, fcc_) (err = AfxObjectAssert(((afxInstance const*)obj_), (fcc_), AfxSpawnHint(), AfxStr((obj_))));
#   define AfxTryAssertObject(obj_, fcc_) ((!obj_) || ((obj_) && (err = AfxObjectAssert(((afxInstance const*)obj_), (fcc_), AfxSpawnHint(), AfxStr((obj_))))));

#else

#   define AfxAssertConnection(var_) ((void)(err))

#   define AfxAssertObject(obj_, fcc_) ((void)(err))
#   define AfxTryAssertObject(obj_, fcc_) ((void)(err))

#endif



typedef struct
{
    _AFX_DBG_FCC
    afxFlags    flags;
    char const* name;
    afxNat      refCnt;
}
afxObjectData;

//AFX_DEFINE_HANDLE(afxObject);

AFX_DEFINE_STRUCT(afxObjectBus)
{
//    afxSlock    lock;
    afxNat      lockReqCnt;
    afxNat      objCnt;
};

AFX_OBJECT(afxObject)
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
    afxError    (*deploy)(afxNat, afxObject[], afxAggregator*, void*);
    afxError    (*drop)(afxNat, afxObject[], afxAggregator*, void*);
};

AFX afxError    AfxAcquireObjects(afxNat cnt, afxObject obj[], afxFcc fcc);
AFX afxError    AfxAcquireNamedObjects(afxNat cnt, afxObject obj[], afxFcc fcc, char const* name);
AFX afxError    AfxReacquireObjects(afxNat cnt, afxObject obj[]);
AFX afxBool     AfxReleaseObjects(afxNat cnt, afxObject obj[], void(*f)(afxObject));
AFX afxResult   _AfxAssertObjects(afxNat cnt, afxObject const obj[], afxFcc fcc);
#define         AfxAssertObjects(cnt_, objs_, fcc_) AfxAssert(((afxResult)(cnt_)) == _AfxAssertObjects((cnt_), (objs_),(fcc_)));

#if 0
AFX afxError    AfxAggregateObjects(afxNat cnt, afxObject obj[], afxAggregator* aggr, void *udd);
AFX afxError    AfxDisaggregateObjects(afxNat cnt, afxObject obj[], afxAggregator* aggr, void *udd);

AFX afxError    AfxAcquireAggregator(afxAggregator* aggr, afxChain* provider);

AFX afxError AfxAcquireObjectBus(afxObjectBus* bus);
AFX afxBool AfxLockObjectBus(afxObjectBus* bus, afxBool modify, afxBool attemp);

AFX afxError AfxRegroupObjects(afxNat cnt, afxObject obj[], afxObjectBus* bus);
AFX afxError AfxUngroupObjects(afxNat cnt, afxObject obj[]);

AFX afxError AfxAcquireAggregator(afxAggregator* aggr, afxChain* provider);

AFX afxObject AfxGetObject(afxAggregation* aggn);

AFX afxAggregator* AfxGetAggregator(afxAggregation* aggn);

AFX afxNat AfxCurateAggregations(afxNat cnt, afxObject obj[], afxBool(*f)(afxAggregation*,void*), void *udd);
AFX afxNat AfxCurateSpecificAggregations(afxNat cnt, afxObject obj[], afxFcc fcc, afxBool(*f)(afxAggregation*,void*), void *udd);

AFX afxNat AfxCurateInactiveAggregations(afxAggregator* aggr, afxBool(*f)(afxAggregation*, void*), void *udd);
AFX afxNat AfxCurateActiveAggregations(afxAggregator* aggr, afxBool(*f)(afxAggregation*, void*), void *udd);


AFX afxNat AfxCurateAggregations(afxNat cnt, afxObject obj[], afxBool(*f)(afxAggregation*, void*), void *udd)
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

AFX afxNat AfxCurateSpecificAggregations(afxNat cnt, afxObject obj[], afxFcc fcc, afxBool(*f)(afxAggregation*, void*), void *udd)
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