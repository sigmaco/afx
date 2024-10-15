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

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/exec/afxAtomic.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/base/afxChain.h"
#include "qwadro/inc/base/afxString.h"
#include "qwadro/inc/base/afxEvent.h"
#include "qwadro/inc/exec/afxTime.h"
//#include "qwadro/inc/exec/afxObjack.h"

#define AFX_OBJ_NAME_LEN 32
#define AFX_OBJ_EVENT_LAYER 0

//AFX_DECLARE_STRUCT(afxHandle);
AFX_DECLARE_STRUCT(afxClass);

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
    afxClass*           cls;
    afxAtom32           refCnt;
    afxNat              instIdx; // read-only

    afxChain            *watchers;
    afxChain            *watching;
    afxBool             (*event)(afxObject obj, afxEvent *ev);
    afxBool             (*eventFilter)(afxObject obj, afxObject watched, afxEvent *ev);
    afxNat32            tid;
    afxObjectFlags      flags;
    
    afxByte*            extra; // plugin data
    afxAddress AFX_SIMD data[];
};

AFX_DEFINE_STRUCT(afxEventFilter)
{
    afxLinkage          holder; // obj->watching
    afxLinkage          watched; // obj->watchers
    //afxEventFilterFn    fn;
};

AFX_DEFINE_STRUCT(afxObjectStash)
{
    afxNat  cnt;
    afxNat  siz;
    afxNat  align;
    void**  var;
};

AFX void                AfxResetEventHandler(afxObject obj, afxBool(*handler)(afxObject obj,afxEvent*));
AFX void                AfxResetEventFilter(afxObject obj, afxBool(*filter)(afxObject obj, afxObject watched, afxEvent*));

AFX afxBool             AfxNotifyObject(afxObject obj, afxEvent *ev);
AFX afxError            AfxConnectObjects(afxObject obj, afxNat cnt, afxObject watcheds[]);
AFX afxError            AfxDisconnectObjects(afxObject obj, afxNat cnt, afxObject watcheds[]);

AFX afxResult           AfxTestObjectFcc(afxObject obj, afxFcc fcc);
AFX afxFcc              AfxGetObjectFcc(afxObject obj);
AFX afxChar const*      AfxGetObjectFccAsString(afxObject obj);

AFX afxClass*           AfxGetClass(afxObject obj);
AFX void*               AfxGetProvider(afxObject obj);
AFXINL afxBool          AfxDoesObjectInherit(afxObject obj, afxClass const* cls);

AFX afxInt32            AfxGetRefCount(afxObject obj);

AFX afxNat32            AfxGetObjectTid(afxObject obj);

AFX afxObjectFlags      AfxSetObjectFlags(afxObject obj, afxObjectFlags flags);
AFX afxObjectFlags      AfxClearObjectFlags(afxObject obj, afxObjectFlags flags);
AFX afxBool             AfxTestObjectFlags(afxObject obj, afxObjectFlags flags);

AFXINL afxError         AfxAllocateInstanceData(afxObject obj, afxNat cnt, afxObjectStash const stashes[]);
AFXINL afxError         AfxDeallocateInstanceData(afxObject obj, afxNat cnt, afxObjectStash const stashes[]);

// %p?%.4s#%i
#define AfxPushObject(obj_) 0,0,0//(obj_), (obj_) ? AfxGetObjectFccAsString((afxHandle*)obj_) : NIL, (obj_) ? ((afxHandle*)obj_)->refCnt : 0

AFX afxError    AfxAcquireObjects(afxClass* cls, afxNat cnt, afxObject objects[], void const* udd[]);
AFX afxError    AfxReacquireObjects(afxNat cnt, afxObject objects[]);
AFX afxBool     AfxReleaseObjects(afxNat cnt, afxObject objects[]);

#ifndef _AFX_MANAGER_C
//#   define AfxAcquireObjects(_mgr_,_cnt_,_objects_,_udd_) AfxAcquireObjects((_mgr_),(_cnt_),(afxObject*)(_objects_), ((_udd_)) )
#   define AfxReacquireObjects(_cnt_,_objects_) AfxReacquireObjects((_cnt_),(afxObject*)(_objects_))
#   define AfxReleaseObjects(_cnt_,_objects_) AfxReleaseObjects((_cnt_),(afxObject*)(_objects_))
#endif

AFX afxNat      _AfxAssertObjects(afxNat cnt, afxObject const objects[], afxFcc fcc); // returns the number of exceptions (if any); expects valid handles only.
AFX afxNat      _AfxTryAssertObjects(afxNat cnt, afxObject const objects[], afxFcc fcc); // returns the number of exceptions (if any); ignore nil handles.
//#define         AfxAssertObjects(_cnt_,_objects_,_fcc_) AfxAssert(((afxResult)(_cnt_)) == _AfxAssertObjects((_cnt_), (afxObject const*)(_objects_),(_fcc_)))
#define         AfxTryAssertObjects(_cnt_,_objects_,_fcc_) AfxAssert((!_cnt_) || ((_objects_) && !(_AfxTryAssertObjects((_cnt_), (afxObject const*)(_objects_),(_fcc_)))))
#define         AfxAssertObjects(_cnt_,_objects_,_fcc_)    AfxAssert((!_cnt_) || ((_objects_) && !(_AfxAssertObjects((_cnt_), (afxObject const*)(_objects_),(_fcc_)))))

AFX afxNat      AfxGetObjectId(afxObject obj);

AFX afxResult   AfxWaitForObject(afxTime timeout, afxObject obj);

#endif//AFX_OBJECT_H
