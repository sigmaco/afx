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

#ifndef AFX_CLASS_H
#define AFX_CLASS_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/base/afxChain.h"
#include "qwadro/inc/exec/afxSlock.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/mem/afxPool.h"
#include "qwadro/inc/mem/afxArena.h"

// RTTI of Qwadro

// The Qt Meta-Object System in Qt is responsible for the signals and slots inter-object communication mechanism, runtime class information, and the Qt property system.
// A single QMetaObject instance is created for each QObject subclass that is used in an application, and this instance stores all the class-information for the QObject subclass.
// This object is available as QObject::typeObject().

AFX_DECLARE_STRUCT(afxClassExtension);
//AFX_DECLARE_STRUCT(afxHandle);
AFX_DECLARE_STRUCT(afxClass);
AFX_DECLARE_STRUCT(afxEvent);

#define _AFX_CLASS_LEVEL_MASK 0xF
#define _AFX_CLASS_BASE_LEVEL 1 // 0 is reserved for afxHandle only stuff

// A afxClass is a model for something which explains it or shows how it canv be produced.

AFX_DEFINE_STRUCT(afxIterator)
{
    afxNat          idx;
    afxBool         abort;
    void(*f)(afxIterator *itr);
    void            *udd;
    union
    {
        afxObjectBase*objItem;
        void        *voidItem;
    };
};

typedef enum afxBlueprintFlags
{
    AFX_OBJB_FLAG_DONT_DELETE   = AFX_BIT(0)
} afxBlueprintFlags;

AFX_DEFINE_STRUCT(afxBlueprint)
{
    afxFcc              fcc;
    afxBlueprintFlags   flags;
};

AFX_DEFINE_STRUCT(afxCookie)
{
    afxNat no;
    void** udd;
};

#define AfxGetCookieData(cookie_,_idx_) 

AFX_DEFINE_STRUCT(afxClassSuballocation)
{
    afxNat          siz;
    afxNat          align;
    afxNat          cntOff;
    afxNat          ptrOff;
};

AFX_DEFINE_STRUCT(afxClassConfig)
{
    afxFcc          fcc;
    afxMmu          mmu;
    afxNat          maxCnt;
    afxNat          fixedSiz;
    afxClass*       subset;
    afxError        (*ctor)(afxObject obj, void** args, afxNat invokeNo);
    afxError        (*dtor)(afxObject obj);
    afxBool         (*event)(afxObject obj, afxEvent *ev);
    afxBool         (*eventFilter)(afxObject obj, afxObject watched, afxEvent *ev);
    afxChar const*  name;
    afxChar const*  desc;
    void const**    vmt;
    afxNat          unitsPerPage;
    afxNat          suballocCnt;
    afxClassSuballocation const* suballocs;
};

AFX_DEFINE_STRUCT(afxObjectChunk)
{
    afxNat      off;
    afxNat      siz;
};

AFX_DEFINE_STRUCT(afxInstanceExtension)
{
    afxLinkage      cls;
    afxNat          pluginId;
    afxNat          objOff;
    afxNat          objSiz;
    afxError        (*dtorCb)(afxObject obj, void* ext, afxNat siz);
    afxError        (*ctorCb)(afxObject obj, void* ext, afxNat siz); // void to avoid warnings
    afxError        (*cpyCb)(afxObject dst, afxObject src, void* ext, afxNat siz);
    afxError        (*ioAlwaysCb)(afxStream iob, afxObject obj, void* ext, afxNat siz); // called after the reading of plugin stream data is finished(useful to set up plugin data for plugins that found no data in the stream, but that cannot set up the data during the ctor callback)
    afxError        (*ioRightsCb)(afxStream iob, afxObject obj, void* ext, afxNat siz, void* data); // called after the reading of plugin stream data is finished, and the object finalised, if and only if the object's rights id was equal to that of the plugin registering the call.
    afxError        (*ioWriteCb)(afxStream iob, afxObject obj, void* ext, afxNat siz); // writes extension data to a binary stream.
    afxError        (*ioReadCb)(afxStream iob, afxObject obj, void* ext, afxNat siz); // reads extension data from a binary stream.
    afxSize         (*ioSizCb)(afxObject obj, void* ext, afxNat siz); // determines the binary size of the extension data.
};

AFX_DEFINE_STRUCT(afxClassVmt)
{
    afxBool(*release)(afxNat cnt, afxObject objects[]);
    afxError(*reacquire)(afxNat cnt, afxObject objects[]);
};

AFX_DEFINE_STRUCT(afxClass)
{
    afxFcc          fcc; // afxFcc_CLS
    afxFcc          objFcc;
    afxChar         name[32];
    afxLinkage      host; // provided by this object.
    afxLinkage      subset; // inherit this object.
    afxChain        supersets; // inherited by these objects.
    afxNat          level;
    afxNat          levelMask;

    afxMmu          mmu;
    afxNat          fixedSiz;
    afxNat          instCnt; // constructed, not allocated as from pool. We can't rely on pool due to it not being used for static instances.
    afxNat          maxInstCnt; // 1 == singleton
    afxPool         pool;
    afxSlock        poolLock;
    afxNat          unitsPerPage; // when pool gets empty, the class will try to resizes storage pages to this value. If zero, the new page will be set to the size of the first batch allocation when pool was zero.
    
    afxArena        arena; // used to allocate class/struct suballocations for members.

    afxError        (*ctor)(afxObject obj, void** args, afxNat invokeNo); // void to avoid warnings
    afxError        (*dtor)(afxObject obj);
    afxError        (*ioAlways)(afxStream iob, afxObject obj); // called after the reading of plugin stream data is finished(useful to set up plugin data for plugins that found no data in the stream, but that cannot set up the data during the ctor callback)
    afxError        (*ioRights)(afxStream iob, afxObject obj, void* data); // called after the reading of plugin stream data is finished, and the object finalised, if and only if the object's rights id was equal to that of the plugin registering the call.
    afxError        (*ioWrite)(afxStream iob, afxObject obj); // writes extension data to a binary stream.
    afxError        (*ioRead)(afxStream iob, afxObject obj); // reads extension data from a binary stream.
    afxSize         (*ioSiz)(afxStream iob, afxObject obj); // determines the binary size of the extension data.
    
    afxChain        plugins;
    afxNat          extraSiz; // extra size contributed by each plugin.
    afxArena        extraAlloc; // plugin space allocation.

    afxBool         (*defEvent)(afxObject obj, afxEvent *ev);
    afxBool         (*defEventFilter)(afxObject obj, afxObject watched, afxEvent *ev);
    
    afxNat          instBaseSiz;
    void const**    vmt;
    afxString const*vmtNames;
    void*           userData[4];
};

AFX afxError        AfxRegisterClass(afxClass* cls, afxClass *base, afxChain* provider, afxClassConfig const *spec);
AFX afxError        AfxDeregisterClass(afxClass* cls);
AFX afxNat          AfxExhaustClass(afxClass* cls);

AFX afxNat          AfxCountClassInstances(afxClass const* cls);
AFX afxNat          AfxGetClassInstanceFixedSize(afxClass const* cls);
AFX afxNat          AfxGetClassInstanceStrictFixedSize(afxClass const* cls);

AFXINL afxArena*    AfxGetClassArena(afxClass *cls);

AFXINL afxClass*    AfxGetSubClass(afxClass const* cls);
AFXINL afxObject    AfxGetClassInstance(afxClass const* cls, afxNat32 uniqueId);

AFX afxNat          AfxEnumerateClassInstances(afxClass const* cls, afxNat first, afxNat cnt, afxObject objects[]);

AFX afxNat          AfxEvokeClassInstances(afxClass const* cls, afxBool(*f)(afxObject,void*), void* udd, afxNat first, afxNat cnt, afxObject objects[]);

/// The AfxInvokeClassInstances2() function is used to apply the given callback function to all objects in the specified class.
/// If any invocation of the callback function returns a failure status the interation is terminated. However, AfxInvokeClassInstances2 will still return successfully.
AFX afxNat          AfxInvokeClassInstances(afxClass const* cls, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void* udd);

/// The AfxInvokeClassInstances2() function is used to apply the given callback function to all objects in the specified class using another callback as filter.
/// If any invocation of the exec() callback function returns a failure status the iteration is terminated.
/// If a invocation of the flt() callback function returns non-zero the object is passed to the exec() callback.
/// However, AfxInvokeClassInstances2 will return count of objects that passed in flt() callback.
AFX afxNat          AfxInvokeClassInstances2(afxClass const* cls, afxNat first, afxNat cnt, afxBool(*f)(afxObject,void*), void* udd, afxBool(*f2)(afxObject,void*), void* udd2);

AFX afxError        _AfxDeallocateObjects(afxClass* cls, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxAllocateObjects(afxClass* cls, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxAllocateClassInstancesAt(afxClass* cls, afxNat base, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxDestructObjects(afxClass* cls, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxConstructObjects(afxClass* cls, afxNat cnt, afxObject objects[], void** udd);

AFX afxResult       AfxDeregisterChainedClasses(afxChain* ch);
AFX afxResult       AfxExhaustChainedClasses(afxChain* ch);
AFX afxChain*       _AfxGetOrphanClasses(void);

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))
#   define AfxAssertClass(cls_, objFcc_)    ((!!((cls_) && ((cls_)->fcc == afxFcc_CLS) && ((cls_)->objFcc == (objFcc_)))) || (AfxThrowError(), AfxLogError("%s\n    %s", AFX_STRINGIFY((var_)), errorMsg[AFXERR_INVALID]), 0))
#else
#   define AfxAssertClass(cls_, fcc_) ((void)(err))
#endif

AFXINL void* AfxGetObjectExtra(afxObject obj, afxNat pluginId);

#endif//AFX_CLASS_H
