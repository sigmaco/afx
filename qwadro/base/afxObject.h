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

/// Qwadro uses a combined object-handle paradigm to manage mechanism and shared resources across the system.

/// HANDLES AND OBJECTS
/// An object is a data structure that represents a system resource, such as a file, thread, or graphic image.
/// Your application canv't directly access object data, nor the system resource that an object represents.
/// Instead, your application must obtain an object handle, which it canv use to examine or modify the system resource.
/// Each handle has an entry in an internally maintained table.
/// Those entries contain the addresses of the resources, and the means to identify the resource type.

#ifndef AFX_OBJECT_H
#define AFX_OBJECT_H

#include "qwadro/exec/afxDebug.h"
#include "qwadro/exec/afxAtomic.h"
#include "qwadro/base/afxFcc.h"
#include "qwadro/base/afxChain.h"
#include "qwadro/base/afxString.h"
#include "qwadro/base/afxEvent.h"
#include "qwadro/exec/afxTime.h"
//#include "qwadro/exec/afxObjack.h"

#define AFX_OBJ_NAME_LEN 32
#define AFX_OBJ_EVENT_LAYER 0

//AFX_DECLARE_STRUCT(afxHandle);
AFX_DECLARE_STRUCT(afxClass);

//#include "qwadro/mem/afxArray.h"

typedef enum afxEventId
{
    AFX_EVENT_NONE,

    afxThreadEvent_RUN = 1,
    afxThreadEvent_QUIT,

    AFX_EVENT_OBJ_DESTROYED, // before deletion
    AFX_EVENT_OBJ_RENAMED, // after rename

    AFX_EVENT_STRM_UPDATED,

    afxEvent_DRAW,
    afxEvent_SOUND,
    afxEvent_UX,

    AFX_EVENT_TOTAL
} afxEventId;

typedef enum afxObjectFlag
{
    afxObjectFlag_READY  = (1 << 0),
    afxObjectFlag_LOCKED = (1 << 2),

    afxObjectFlag_
} afxObjectFlags;

typedef void* afxObject;
#define AfxObjects(_objets_) ((afxObject*)_objets_)

AFX_DECLARE_STRUCT(afxEvent);

AFX_DEFINE_STRUCT(afxEvent)
{
    afxEventId      id;
    afxUnit          siz;
    afxUnit32        tid; // poster
    //afxBool         posted;
    afxBool         accepted;
    void*           udd[4];
};

typedef afxBool(*afxEventFilterFn)(afxObject obj, afxObject watched, afxEvent *ev);

AFX_DEFINE_STRUCT(afxPostedEvent)
{
    afxObject       receiver;
    afxUnit          priority;
    afxUnit          siz;
    afxByte         ev[];
};

AFX_DEFINE_STRUCT(afxObjectBase)
{
    afxFcc              fcc; // OBJ
    afxUnit             instIdx; // read-only
    afxObjectFlags      flags;
    afxAtom32           refCnt;
    afxUnit32           tid;
    afxClass*           cls;    
    afxChain            *watchers;
    afxChain            *watching;
    afxBool             (*event)(afxObject obj, afxEvent *ev);
    afxByte*            extra; // plugin data
    afxAddress AFX_SIMD data[];
};

AFX_STATIC_ASSERT(offsetof(afxObjectBase, data) % 16 == 0, "");

AFX_DEFINE_STRUCT(afxEventFilter)
{
    afxLink holder; // holder->watching
    afxLink watched; // watched->watchers
    afxBool (*fn)(afxObject obj, afxObject watched, afxEvent*);
};

AFX_DEFINE_STRUCT(afxObjectStash)
{
    afxUnit cnt;
    afxUnit siz;
    afxUnit align;
    void**  var;
};

AFX void                AfxInstallEventHandler(afxObject obj, afxBool(*handler)(afxObject obj,afxEvent*));

AFX afxBool             AfxNotifyObject(afxObject obj, afxEvent *ev);
AFX afxError            AfxConnectObjects(afxObject obj, afxBool(*handler)(afxObject obj, afxObject watched, afxEvent *ev), afxUnit cnt, afxObject watcheds[]);
AFX afxError            AfxDisconnectObjects(afxObject obj, afxBool(*handler)(afxObject obj, afxObject watched, afxEvent *ev), afxUnit cnt, afxObject watcheds[]);

AFX afxResult           AfxTestObjectFcc(afxObject obj, afxFcc fcc);
AFX afxFcc              AfxGetObjectFcc(afxObject obj);
AFX afxChar const*      AfxGetObjectFccAsString(afxObject obj);

AFX afxClass*           AfxGetClass(afxObject obj);
AFX void*               AfxGetHost(afxObject obj);
AFXINL afxBool          AfxDoesObjectInherit(afxObject obj, afxClass const* cls);

AFX afxInt32            AfxGetRefCount(afxObject obj);

AFX afxUnit32            AfxGetObjectTid(afxObject obj);

AFX afxObjectFlags      AfxSetObjectFlags(afxObject obj, afxObjectFlags flags);
AFX afxObjectFlags      AfxClearObjectFlags(afxObject obj, afxObjectFlags flags);
AFX afxBool             AfxTestObjectFlags(afxObject obj, afxObjectFlags flags);

AFXINL afxError         AfxAllocateInstanceData(afxObject obj, afxUnit cnt, afxObjectStash const stashes[]);
AFXINL afxError         AfxDeallocateInstanceData(afxObject obj, afxUnit cnt, afxObjectStash const stashes[]);

// %p?%.4s#%i
#define AfxPushObject(obj_) 0,0,0//(obj_), (obj_) ? AfxGetObjectFccAsString((afxHandle*)obj_) : NIL, (obj_) ? ((afxHandle*)obj_)->refCnt : 0

/*
    In Qwadro, we do not "create" things. This may be confuse but remember that the Qwadro is literally an alchemist canvas.
    This mean that we declare, proclaim, promulgate and/or publicize things using classes, we never create them. 
    That is why you "acquire" things, via AfxAcquireObjects(), where everything existing is already existing before you but has not been yet "classified".

    The result of an acquisition is a handle to an entity. Despite it being a combined object-handle given the afxObject component, 
    the object is idealized to be just a state of your contract of acquision. Thus, the object acquired is firstly symbolic.
*/

AFX afxError    AfxAcquireObjects(afxClass* cls, afxUnit cnt, afxObject objects[], void const* udd[]);

/*
    As a whole system, handles are shareable, where they can be reacquired across the system.
*/

AFX afxError    AfxReacquireObjects(afxUnit cnt, afxObject objects[]);

/*
    As every handle is shareable and things not being created, the function to get rid of it is the AfxDisposeObjects().
    This function can destroy the object when the reference counter is decremented to zero, and deallocate its storage if the pool is ready to free the page backing such object.
*/

AFX afxBool     AfxDisposeObjects(afxUnit cnt, afxObject objects[]);

#ifndef _AFX_MANAGER_C
//#   define AfxAcquireObjects(_mgr_,_cnt_,_objects_,_udd_) AfxAcquireObjects((_mgr_),(_cnt_),(afxObject*)(_objects_), ((_udd_)) )
#   define AfxReacquireObjects(_cnt_,_objects_) AfxReacquireObjects((_cnt_),(afxObject*)(_objects_))
#   define AfxDisposeObjects(_cnt_,_objects_) AfxDisposeObjects((_cnt_),(afxObject*)(_objects_))
#endif

AFX afxUnit      _AfxAssertObjects(afxUnit cnt, afxObject const objects[], afxFcc fcc); // returns the number of exceptions (if any); expects valid handles only.
AFX afxUnit      _AfxTryAssertObjects(afxUnit cnt, afxObject const objects[], afxFcc fcc); // returns the number of exceptions (if any); ignore nil handles.
//#define         AfxAssertObjects(_cnt_,_objects_,_fcc_) AFX_ASSERT(((afxResult)(_cnt_)) == _AfxAssertObjects((_cnt_), (afxObject const*)(_objects_),(_fcc_)))
//#define         AfxTryAssertObjects(_cnt_,_objects_,_fcc_) AFX_ASSERT((!_cnt_) || ((_objects_) && !(_AfxTryAssertObjects((_cnt_), (afxObject const*)(_objects_),(_fcc_)))))
//#define         AfxAssertObjects(_cnt_,_objects_,_fcc_)    AFX_ASSERT((!_cnt_) || ((_objects_) && !(_AfxAssertObjects((_cnt_), (afxObject const*)(_objects_),(_fcc_)))))

#ifdef _AFX_DEBUG
#   define  AFX_TRY_ASSERT_OBJECTS(_fcc_,_cnt_,_objects_) AFX_ASSERT((!_cnt_) || ((_objects_) && !(_AfxTryAssertObjects((_cnt_), (afxObject const*)(_objects_),(_fcc_)))))
#   define  AFX_ASSERT_OBJECTS(_fcc_,_cnt_,_objects_)    AFX_ASSERT((!_cnt_) || ((_objects_) && !(_AfxAssertObjects((_cnt_), (afxObject const*)(_objects_),(_fcc_)))))
#else
#   define  AFX_TRY_ASSERT_OBJECTS(_fcc_,_cnt_,_objects_) 
#   define  AFX_ASSERT_OBJECTS(_fcc_,_cnt_,_objects_) 
#endif

AFX afxUnit      AfxGetObjectId(afxObject obj);

AFX afxResult   AfxWaitForObject(afxTime timeout, afxObject obj);

#endif//AFX_OBJECT_H
