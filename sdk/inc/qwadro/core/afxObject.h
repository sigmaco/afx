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

#ifndef AFX_OBJECT_H
#define AFX_OBJECT_H

#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxFcc.h"
#include "qwadro/core/afxChain.h"
#include "qwadro/core/afxString.h"
#include "qwadro/core/afxEvent.h"
#include "qwadro/core/afxTime.h"
//#include "qwadro/core/afxObjack.h"

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

//AFX_DECLARE_STRUCT(afxHandle);
AFX_DECLARE_STRUCT(afxManager);

//#include "../mem/afxArray.h"

typedef enum afxObjectFlag
{
    afxObjectFlag_READY  = (1 << 0),
    afxObjectFlag_LOCKED = (1 << 2),

    afxObjectFlag_
} afxObjectFlags;

typedef afxBool (*afxEventFilterFn)(afxObject obj, afxObject watched, afxEvent *ev);

AFX_DEFINE_STRUCT(afxObjectBase)
{
    afxFcc              fcc; // OBJ
    afxManager*         mgr;
    afxAtomic           refCnt;
    afxNat              instIdx;
    afxList             signaling; // slot holders
    afxList             handling; // slot helds
    
    afxChain            *watchers;
    afxChain            *watching;
    afxNat32            tid;
    afxObjectFlags      flags;

    afxAddress          afxSimd(data[1]);
};

AFX_DEFINE_STRUCT(afxEventFilter)
{
    afxLinkage          holder; // obj->watching
    afxLinkage          watched; // obj->watchers
    //afxEventFilterFn    fn;
};

AFX afxError AfxObjectInstallEventFilter(afxObject obj, afxObject filter); // if filter is already filtering this object, callback will be replaced.
AFX afxError AfxObjectRemoveEventFilter(afxObject obj, afxObject filter);
AFX afxBool AfxNotifyObject(afxObject obj, afxEvent *ev);

AFX_DEFINE_STRUCT(afxConnection)
{
    afxFcc              fcc; // objc
    afxLink             holderLink;
    afxObject           holder; // observing
    void                (*handler)(afxObject, afxEvent*);
    afxNat32            filter;
    afxLink             objLink;
    union
    {
        afxObject       obj; // signaling
        void            *voidObj;
    };
    afxBool             reacquired;
    afxBool             alloced;
    afxInt32            tenacity; // reference counter
};

AFX afxResult           AfxTestObjectFcc(afxObject obj, afxFcc fcc);
AFX afxFcc              AfxObjectGetFcc(afxObject obj);
AFX afxChar const*      AfxObjectGetFccAsString(afxObject obj);

AFX afxManager*         AfxGetClass(afxObject obj);
AFX void*               AfxGetObjectProvider(afxObject obj);
AFX afxNat              AfxObjectGetIndex(afxObject obj, afxBool b2f);
AFX void const*         AfxObjectGetVmt(afxObject obj);
AFXINL afxBool          AfxObjectInherits(afxObject obj, afxManager const* cls);

AFX afxInt32            AfxGetRefCount(afxObject obj);

AFX afxNat32            AfxGetObjectTid(afxObject obj);

AFX afxResult           AfxObjectSignalConnections(afxObject obj, afxEventId evtype, void *data);
AFX afxConnection*      AfxObjectConnect(afxObject obj, afxObject holder, void(*handler)(afxObject,afxEvent*), afxNat32 filter, afxConnection *objc);
AFX afxResult           AfxObjectDisconnect(afxObject obj, afxObject holder, void(*handler)(afxObject,afxEvent*));

AFX afxObjectFlags      AfxObjectSetFlags(afxObject obj, afxObjectFlags flags);
AFX afxObjectFlags      AfxObjectClearFlags(afxObject obj, afxObjectFlags flags);
AFX afxBool             AfxObjectTestFlags(afxObject obj, afxObjectFlags flags);

// SLOT UTILS

AFXINL afxError         AfxConnectionReset(afxConnection *objc); // initializes a connection slot.
AFXINL afxError         AfxConnectionDrop(afxConnection *objc);

AFXINL afxInt32         AfxConnectionIncTenacity(afxConnection *objc);
AFXINL afxInt32         AfxConnectionDecTenacity(afxConnection *objc);

AFXINL void*            AfxConnectionGetObject(afxConnection const *objc);
AFXINL void*            AfxConnectionGetHolder(afxConnection const *objc);

AFXINL afxError         AfxConnectionSetHandler(afxConnection *objc, void(*handler)(afxObject, afxEvent*));
AFXINL afxError         AfxConnectionSetFilter(afxConnection *objc, afxNat32 filter);


// %p?%.4s#%i
#define AfxPushObject(obj_) 0,0,0//(obj_), (obj_) ? AfxObjectGetFccAsString((afxHandle*)obj_) : NIL, (obj_) ? ((afxHandle*)obj_)->refCnt : 0


#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))

#   define AfxAssertConnection(var_) ((!!((var_) && ((var_)->fcc == afxFcc_OBJC)))||(err = (afxError)__LINE__,AfxLogError("%s\n    %s",AFX_STRINGIFY((var_)),errorMsg[AFXERR_INVALID]),0))

#   define AfxAssertObject(obj_, fcc_) (err = AfxObjectAssert(((afxObject)obj_), (fcc_), AfxHere(), AFX_STRINGIFY((obj_))));
#   define AfxTryAssertObject(obj_, fcc_) ((!obj_) || ((obj_) && (err = AfxObjectAssert(((afxObject)obj_), (fcc_), AfxHere(), AFX_STRINGIFY((obj_))))));

#else

#   define AfxAssertConnection(var_) ((void)(err))

#   define AfxAssertObject(obj_, fcc_) ((void)(err))
#   define AfxTryAssertObject(obj_, fcc_) ((void)(err))

#endif

AFX afxError    AfxAcquireObjects(afxManager* cls, afxNat cnt, afxObject handles[], void const* udd[]);
AFX afxError    AfxReacquireObjects(afxNat cnt, afxObject handles[]);
AFX afxBool     AfxReleaseObjects(afxNat cnt, afxObject handles[]);

AFX afxBool     _AfxAssertObjects(afxNat cnt, afxObject const handles[], afxFcc fcc);
#define         AfxAssertObjects(cnt_, objs_, fcc_) AfxAssert(((afxResult)(cnt_)) == _AfxAssertObjects((cnt_), (afxObject const*)(objs_),(fcc_)))
#define         AfxTryAssertObjects(cnt_, objs_, fcc_) AfxAssert((!(((afxObject const*)objs_)[0])) || (((afxResult)(cnt_)) == _AfxAssertObjects((cnt_), (afxObject const*)(objs_),(fcc_))))

AFX afxNat      AfxGetObjectId(afxObject obj);

AFX afxResult   AfxWaitForObject(afxObject obj, afxTime timeout);

#endif//AFX_OBJECT_H
