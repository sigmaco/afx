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

/// Qwadro uses a combined object-handle paradigm to manage mechanism and shared resources across the system.

/// HANDLES AND OBJECTS
/// An object is a data structure that represents a system resource, such as a file, thread, or graphic image.
/// Your application canv't directly access object data, nor the system resource that an object represents.
/// Instead, your application must obtain an object handle, which it canv use to examine or modify the system resource.
/// Each handle has an entry in an internally maintained table.
/// Those entries contain the addresses of the resources, and the means to identify the resource type.

#ifndef AFX_OBJECT_H
#define AFX_OBJECT_H

#include "qwadro/core/afxDebug.h"
#include "qwadro/core/afxAtomic.h"
#include "qwadro/core/afxFcc.h"
#include "qwadro/core/afxChain.h"
#include "qwadro/core/afxString.h"
#include "qwadro/core/afxEvent.h"
#include "qwadro/core/afxTime.h"
//#include "qwadro/core/afxObjack.h"

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
    afxAtom32           refCnt;
    afxNat              instIdx; // read-only

    afxChain            *watchers;
    afxChain            *watching;
    afxBool             (*event)(afxObject obj, afxEvent *ev);
    afxBool             (*eventFilter)(afxObject obj, afxObject watched, afxEvent *ev);
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

AFX void                AfxInstallEventHandler(afxObject obj, afxBool(*handler)(afxObject obj,afxEvent*));
AFX void                AfxInstallEventFilter(afxObject obj, afxBool(*filter)(afxObject obj, afxObject watched, afxEvent*));
AFX afxError            AfxInstallWatcher(afxObject obj, afxObject watcher); // if filter is already filtering this object, callback will be replaced.
AFX afxBool             AfxDeinstallWatcher(afxObject obj, afxObject watcher);
AFX afxBool             AfxNotifyObject(afxObject obj, afxEvent *ev);

AFX afxResult           AfxTestObjectFcc(afxObject obj, afxFcc fcc);
AFX afxFcc              AfxGetObjectFcc(afxObject obj);
AFX afxChar const*      AfxGetObjectFccAsString(afxObject obj);

AFX afxManager*         AfxGetManager(afxObject obj);
AFX void*               AfxGetObjectProvider(afxObject obj);
AFXINL afxBool          AfxObjectInherits(afxObject obj, afxManager const* mgr);

AFX afxInt32            AfxGetRefCount(afxObject obj);

AFX afxNat32            AfxGetObjectTid(afxObject obj);

AFX afxObjectFlags      AfxSetObjectFlags(afxObject obj, afxObjectFlags flags);
AFX afxObjectFlags      AfxClearObjectFlags(afxObject obj, afxObjectFlags flags);
AFX afxBool             AfxTestObjectFlags(afxObject obj, afxObjectFlags flags);

// %p?%.4s#%i
#define AfxPushObject(obj_) 0,0,0//(obj_), (obj_) ? AfxGetObjectFccAsString((afxHandle*)obj_) : NIL, (obj_) ? ((afxHandle*)obj_)->refCnt : 0


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

#ifndef _AFX_MANAGER_C
#   define AfxReleaseObjects(cnt_, objects_) AfxReleaseObjects((cnt_), (afxObject*)(objects_))
#endif

AFX afxBool     _AfxAssertObjects(afxNat cnt, afxObject const handles[], afxFcc fcc);
#define         AfxAssertObjects(cnt_, objs_, fcc_) AfxAssert(((afxResult)(cnt_)) == _AfxAssertObjects((cnt_), (afxObject const*)(objs_),(fcc_)))
#define         AfxTryAssertObjects(cnt_, objs_, fcc_) AfxAssert((!(((afxObject const*)objs_)[0])) || (((afxResult)(cnt_)) == _AfxAssertObjects((cnt_), (afxObject const*)(objs_),(fcc_))))

AFX afxNat      AfxGetObjectId(afxObject obj);

AFX afxResult   AfxWaitForObject(afxObject obj, afxTime timeout);

#endif//AFX_OBJECT_H
