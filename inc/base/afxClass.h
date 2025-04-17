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

/*
    The afxClass is a rich and powerful type meta-system, forming the core of a reflective object system 
    and component-based architecture across the whole Qwadro ecosystem.
    
    It's a runtime type info (RTTI), memory management mechanism and plugin IO glue.
    It supports a flexible single inheritance model with tracking of subtypes and RTTI depth filtering.

    afxClass describes a runtime type, essentially a class descriptor or type object that:
        Defines how to construct/destroy objects;
        Supports object pooling;
        Manages inheritance and composition;
        Handles plugin extension data;
        Provides memory management via arenas and pools;
        Encodes object-level behavior (like events and IO serialization);

    This is a very complete runtime class system with:
        Inheritance support
        Memory pooling
        Plugin extension points
        RTTI & serialization
        Custom construction logic
        Instance count limits

    It has beeen designed for both performance (custom allocators, pools, alignment) and flexibility (plugin IO, VMT, events, extensions).
*/

#ifndef AFX_CLASS_H
#define AFX_CLASS_H

#include "qwadro/inc/base/afxDebug.h"
#include "qwadro/inc/base/afxFcc.h"
#include "qwadro/inc/base/afxChain.h"
#include "qwadro/inc/exec/afxFutex.h"
#include "qwadro/inc/base/afxObject.h"
#include "qwadro/inc/mem/afxPool.h"
#include "qwadro/inc/mem/afxArena.h"

//AFX_DECLARE_STRUCT(afxHandle);
AFX_DECLARE_STRUCT(afxClass);
AFX_DECLARE_STRUCT(afxEvent);

#define _AFX_CLASS_LEVEL_MASK 0xF
#define _AFX_CLASS_BASE_LEVEL 1 // 0 is reserved for afxHandle only stuff

// A afxClass is a model for something which explains it or shows how it can be produced.

AFX_DEFINE_STRUCT(afxIterator)
{
    afxUnit          idx;
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
    afxUnit no;
    void** udd;
};

#define AfxGetCookieData(cookie_,_idx_) 

AFX_DEFINE_STRUCT(afxClassSuballocation)
{
    afxUnit         siz;
    afxUnit         align;
    afxUnit         cntOff;
    afxUnit         ptrOff;
};

AFX_DEFINE_STRUCT(afxClassConfig)
{
    afxFcc          fcc;
    afxMmu          mmu;
    afxUnit         maxCnt;
    afxUnit         fixedSiz;
    afxClass*       subset;
    afxError        (*ctor)(afxObject obj, void** args, afxUnit invokeNo);
    afxError        (*dtor)(afxObject obj);
    afxBool         (*event)(afxObject obj, afxEvent *ev);
    afxBool         (*eventFilter)(afxObject obj, afxObject watched, afxEvent *ev);
    afxChar const*  name;
    afxChar const*  desc;
    void const**    vmt;
    afxUnit         unitsPerPage;
    afxUnit         suballocCnt;
    afxClassSuballocation const* suballocs;
};

AFX_DEFINE_STRUCT(afxObjectChunk)
{
    afxUnit     off;
    afxUnit     siz;
};

AFX_DEFINE_STRUCT(afxClassExtension)
{
    afxLink      cls;
    afxUnit         extId;
    afxUnit         objOff;
    afxUnit         objSiz;
    afxError        (*dtorCb)(afxObject obj, void* ext, afxUnit siz);
    afxError        (*ctorCb)(afxObject obj, void* ext, afxUnit siz); // void to avoid warnings
    afxError        (*cpyCb)(afxObject dst, afxObject src, void* ext, afxUnit siz);
    afxError        (*ioAlwaysCb)(afxStream iob, afxObject obj, void* ext, afxUnit siz); // called after the reading of plugin stream data is finished(useful to set up plugin data for plugins that found no data in the stream, but that cannot set up the data during the ctor callback)
    afxError        (*ioRightsCb)(afxStream iob, afxObject obj, void* ext, afxUnit siz, void* data); // called after the reading of plugin stream data is finished, and the object finalised, if and only if the object's rights id was equal to that of the plugin registering the call.
    afxError        (*ioWriteCb)(afxStream iob, afxObject obj, void* ext, afxUnit siz); // writes extension data to a binary stream.
    afxError        (*ioReadCb)(afxStream iob, afxObject obj, void* ext, afxUnit siz); // reads extension data from a binary stream.
    afxSize         (*ioSizCb)(afxObject obj, void* ext, afxUnit siz); // determines the binary size of the extension data.
};

AFX_DEFINE_STRUCT(afxClassVmt)
{
    afxBool(*release)(afxUnit cnt, afxObject objects[]);
    afxError(*reacquire)(afxUnit cnt, afxObject objects[]);
};

AFX_DEFINE_STRUCT(afxClass)
{
    // Should be afxFcc_CLS (for class validation).
    afxFcc          fcc; // afxFcc_CLS
    // The FourCC of the objects it instantiates.
    afxFcc          objFcc;
    // Human-readable name.
    afxChar         name[32];
    // The owner or host of this class definition.
    afxLink         host; // provided by this object.
    // Parent/superclass (inherits from).
    afxLink         subset; // inherit this object.
    // Children/subclasses.
    afxChain        supersets; // inherited by these objects.
    // Depth in hierarchy.
    afxUnit         level;
    // Bitmask for level filtering (e.g., permissions or RTTI filtering).
    afxUnit         levelMask;

    // MEMORY MANAGEMENT
    // Memory allocator for fixed allocations.
    afxMmu          mmu;
    // Static size of the object.
    afxUnit         fixedSiz;
    // Active instance count (constructed).
    // Object count is tracked separately from pooled memory. 
    // This is important for static or embedded instances that don't come from the pool.
    afxUnit         instCnt; // constructed, not allocated as from pool. We can't rely on pool due to it not being used for static instances.
    // Limit (singleton = 1).
    afxUnit         maxInstCnt; // 1 == singleton
    // Memory pool for dynamic allocations.
    afxPool         pool;
    // Thread-safe lock around pool usage.
    afxFutex        poolLock;
    // Controls pool page growth strategy.
    // When pool gets empty, the class will try to resizes storage pages to this value. 
    // If zero, the new page will be set to the size of the first batch allocation when pool was zero.
    afxUnit         unitsPerPage;
    // Counter for unique IDs, etc.
    afxUnit         uniqueInc;

    afxArena        arena; // used to allocate class/struct suballocations for members.

    // Custom constructor/destructor logic.
    afxError        (*ctor)(afxObject obj, void** args, afxUnit invokeNo); // void to avoid warnings
    afxError        (*dtor)(afxObject obj);
    // Security/permission checks with ioRights.
    afxError        (*ioAlways)(afxStream iob, afxObject obj); // called after the reading of plugin stream data is finished(useful to set up plugin data for plugins that found no data in the stream, but that cannot set up the data during the ctor callback)
    afxError        (*ioRights)(afxStream iob, afxObject obj, void* data); // called after the reading of plugin stream data is finished, and the object finalised, if and only if the object's rights id was equal to that of the plugin registering the call.
    // Binary stream support with optional plugin-specific data.
    afxError        (*ioWrite)(afxStream iob, afxObject obj); // writes extension data to a binary stream.
    afxError        (*ioRead)(afxStream iob, afxObject obj); // reads extension data from a binary stream.
    afxSize         (*ioSiz)(afxStream iob, afxObject obj); // determines the binary size of the extension data.
    
    // Plugin System and Extensions.
    afxChain        extensions;
    afxUnit         extraSiz; // extra size contributed by each plugin.
    afxArena        extraAlloc; // plugin space allocation.

    // Event Handling.
    // This lets classes hook into an event system, e.g., Qt-style event filtering and overrides.
    afxBool         (*defEvent)(afxObject obj, afxEvent *ev);
    afxBool         (*defEventFilter)(afxObject obj, afxObject watched, afxEvent *ev);
    

    afxUnit         instBaseSiz;
    // A virtual method table (manual polymorphism).
    void const**    vmt;
    afxString const*vmtNames;
    // A extra per-class storage - useful for internal hooks, user metadata, or binding to scripting systems.
    void*           userData[4];
};

AFX afxError        AfxMountClass(afxClass* cls, afxClass *base, afxChain* provider, afxClassConfig const *spec);
AFX afxError        AfxDismountClass(afxClass* cls);

AFX afxError        AfxInstallClassExtension(afxClass* cls, afxClassExtension* const ext);

AFX afxUnit         AfxExhaustClass(afxClass* cls);

AFX afxUnit         AfxGetSizeOfObject(afxClass const* cls, afxUnit* strictSize);

AFXINL afxArena*    AfxGetClassArena(afxClass *cls);

AFXINL afxClass*    AfxGetSubClass(afxClass const* cls);
AFXINL afxObject    AfxGetClassInstance(afxClass const* cls, afxUnit32 uniqueId);

// Pass NIL into objects to AfxEnumerateObjects() to return a total number of instances.

AFX afxUnit         AfxEnumerateObjects(afxClass const* cls, afxUnit first, afxUnit cnt, afxObject objects[]);
AFX afxUnit         _AfxEnumerateObjectsUnlocked(afxClass const* cls, afxBool fromLast, afxUnit first, afxUnit cnt, afxObject objects[]);

AFX afxUnit         AfxEvokeObjects(afxClass const* cls, afxBool(*f)(afxObject,void*), void* udd, afxUnit first, afxUnit cnt, afxObject objects[]);

/// The AfxInvokeClassInstances2() function is used to apply the given callback function to all objects in the specified class.
/// If any invocation of the callback function returns a failure status the interation is terminated. However, AfxInvokeClassInstances2 will still return successfully.
AFX afxUnit         AfxInvokeObjects(afxClass const* cls, afxUnit first, afxUnit cnt, afxBool(*f)(afxObject obj, void *udd), void* udd);

/// The AfxInvokeClassInstances2() function is used to apply the given callback function to all objects in the specified class using another callback as filter.
/// If any invocation of the exec() callback function returns a failure status the iteration is terminated.
/// If a invocation of the flt() callback function returns non-zero the object is passed to the exec() callback.
/// However, AfxInvokeClassInstances2 will return count of objects that passed in flt() callback.
AFX afxUnit         AfxInvokeClassInstances2(afxClass const* cls, afxUnit first, afxUnit cnt, afxBool(*f)(afxObject,void*), void* udd, afxBool(*f2)(afxObject,void*), void* udd2);

AFX afxError        _AfxDeallocateObjects(afxClass* cls, afxUnit cnt, afxObject objects[]);
AFX afxError        _AfxAllocateObjects(afxClass* cls, afxUnit cnt, afxObject objects[]);
AFX afxError        _AfxAllocateClassInstancesAt(afxClass* cls, afxUnit base, afxUnit cnt, afxObject objects[]);
AFX afxError        _AfxDestructObjects(afxClass* cls, afxUnit cnt, afxObject objects[]);
AFX afxError        _AfxConstructObjects(afxClass* cls, afxUnit cnt, afxObject objects[], void** udd);

AFX afxResult       AfxDeregisterChainedClasses(afxChain* ch);
AFX afxResult       AfxExhaustChainedClasses(afxChain* ch);
AFX afxChain*       _AfxGetOrphanClasses(void);

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))
#   define AFX_ASSERT_CLASS(cls_, objFcc_)    ((!!((cls_) && ((cls_)->fcc == afxFcc_CLS) && ((cls_)->objFcc == (objFcc_)))) || (AfxThrowError(), AfxReportError("%s\n    %s", AFX_STRINGIFY((var_)), errorMsg[afxError_INVALID]), 0))
#else
#   define AFX_ASSERT_CLASS(cls_, fcc_) ((void)(err))
#endif

AFXINL void* AfxGetObjectExtra(afxObject obj, afxUnit pluginId);

AFXINL void* AfxGetObjectIdd(afxObject obj, afxUnit offset);

#endif//AFX_CLASS_H
