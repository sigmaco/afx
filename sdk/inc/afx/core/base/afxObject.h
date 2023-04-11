/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_OBJECT_H
#define AFX_OBJECT_H

#include <stdarg.h>
#include "../diag/afxDebug.h"
#include "afxFcc.h"
#include "afxLinkedList.h"
#include "afxString.h"
#include "afxEvent.h"
#include "afxClass.h"

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

AFX_DECLARE_STRUCT(afxClass);

#include "../mem/afxArray.h"

typedef enum afxObjectFlag
{
    AFX_OBJ_FLAG_READY  = (1 << 0),
    AFX_OBJ_FLAG_LOCKED = (1 << 2)
} afxObjectFlag;

AFX_DEFINE_STRUCT(afxObject)
{
    afxFcc              fcc;
    afxLinkage          cls;
    afxInt32            refCnt;
    afxList             signaling; // slot holders
    afxList             handling; // slot helds
    afxBool             (*event)(afxObject *obj, afxEvent *ev);
    afxNat32            tid;
    afxObjectFlag       flags;
};

AFX_DEFINE_STRUCT(afxConnection)
{
    afxFcc              fcc; // objc
    afxLink             holderLink;
    afxObject           *holder; // observing
    void                (*handler)(afxObject*, afxEvent*);
    afxNat32            filter;
    afxLink             objLink;
    union
    {
        afxObject       *obj; // signaling
        void            *voidObj;
    };
    afxBool             reacquired;
    afxBool             alloced;
    afxInt32            tenacity; // reference counter
};

AFX void*               AfxObjectAcquire(afxClass *cls, void const *paradigm, afxHint const hint);
AFX afxResult           AfxObjectCoacquire(afxClass *cls, afxNat cnt, void const **paradigms, void **objs, afxHint const hint);
AFX afxResult           AfxObjectReacquire(afxObject *obj, afxObject *holder, void(*handler)(afxObject*, afxEvent*), afxNat32 filter, afxConnection *objc);
AFX afxResult           AfxObjectRelease(afxObject *obj);

AFX afxResult           AfxObjectTestFcc(afxObject const *obj, afxFcc fcc);
AFX afxFcc              AfxObjectGetFcc(afxObject const *obj);
AFX afxChar const*      AfxObjectGetFccAsString(afxObject const *obj);

AFX afxError            AfxObjectAssert(afxObject const *obj, afxFcc fcc, afxHint const hint, afxChar const *exp);
AFX afxClass*           AfxObjectGetClass(afxObject const *obj);
AFX void*               AfxObjectGetProvider(afxObject const *obj);
AFX afxNat              AfxObjectGetIndex(afxObject const *obj, afxBool b2f);
AFX void const*         AfxObjectGetVmt(afxObject const *obj);
AFXINL afxBool          AfxObjectInherits(afxObject const *obj, afxClass const* cls);

AFX afxInt32            AfxObjectGetRefCount(afxObject const *obj);

AFX afxNat32            AfxObjectGetTid(afxObject const *obj);

AFX afxResult           AfxObjectSignal(afxObject *obj, afxEventType evtype, void *data);
AFX afxConnection*      AfxObjectConnect(afxObject *obj, afxObject *holder, void(*handler)(afxObject*,afxEvent*), afxNat32 filter, afxConnection *objc);
AFX afxResult           AfxObjectDisconnect(afxObject *obj, afxObject *holder, void(*handler)(afxObject*,afxEvent*));

AFX afxObjectFlag       AfxObjectSetFlags(afxObject *obj, afxObjectFlag flags);
AFX afxObjectFlag       AfxObjectClearFlags(afxObject *obj, afxObjectFlag flags);
AFX afxBool             AfxObjectTestFlags(afxObject const *obj, afxObjectFlag flags);

// SLOT UTILS

AFXINL afxError         AfxConnectionReset(afxConnection *objc); // initializes a connection slot.
AFXINL afxError         AfxConnectionDrop(afxConnection *objc);

AFXINL afxInt32         AfxConnectionIncTenacity(afxConnection *objc);
AFXINL afxInt32         AfxConnectionDecTenacity(afxConnection *objc);

AFXINL void*            AfxConnectionGetObject(afxConnection const *objc);
AFXINL void*            AfxConnectionGetHolder(afxConnection const *objc);

AFXINL afxError         AfxConnectionSetHandler(afxConnection *objc, void(*handler)(afxObject*, afxEvent*));
AFXINL afxError         AfxConnectionSetFilter(afxConnection *objc, afxNat32 filter);


// %p?%.4s#%i
#define AfxPushObject(obj_) (obj_), (obj_) ? AfxObjectGetFccAsString((afxObject*)obj_) : NIL, (obj_) ? ((afxObject*)obj_)->refCnt : 0


#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))

#   define AfxAssertConnection(var_) ((!!((var_) && ((var_)->fcc == AFX_FCC_OBJC)))||(err = (afxResult)__LINE__,AfxOutputError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[AFXERR_INVALID]),0))

#   define AfxAssertObject(obj_, fcc_) (err = AfxObjectAssert(((afxObject const*)obj_), (fcc_), AfxSpawnHint(), AfxStr((obj_))));
#   define AfxTryAssertObject(obj_, fcc_) ((!obj_) || (err = AfxObjectAssert(((afxObject const*)obj_), (fcc_), AfxSpawnHint(), AfxStr((obj_)))));

#else

#   define AfxAssertConnection(var_) ((void)(err))

#   define AfxAssertObject(obj_, fcc_) ((void)(err))
#   define AfxTryAssertObject(obj_, fcc_) ((void)(err))

#endif

#endif//AFX_OBJECT_H