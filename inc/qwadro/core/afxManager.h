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

#include "qwadro/core/afxFcc.h"
#include "qwadro/core/afxChain.h"
#include "qwadro/core/afxSlock.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/mem/afxPool.h"

// RTTI of Qwadro

// The Qt Meta-Object System in Qt is responsible for the signals and slots inter-object communication mechanism, runtime class information, and the Qt property system.
// A single QMetaObject instance is created for each QObject subclass that is used in an application, and this instance stores all the class-information for the QObject subclass.
// This object is available as QObject::typeObject().

AFX_DECLARE_STRUCT(afxClassExtension);
//AFX_DECLARE_STRUCT(afxHandle);
AFX_DECLARE_STRUCT(afxManager);
AFX_DECLARE_STRUCT(afxEvent);

#define _AFX_CLASS_LEVEL_MASK 0xF
#define _AFX_CLASS_BASE_LEVEL 1 // 0 is reserved for afxHandle only stuff

// A afxManager is a model for something which explains it or shows how it canv be produced.

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
    afxNat          size;
    afxManager*     subset;
    afxError        (*ctor)(afxObject obj, afxCookie const* cookie);
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

AFX_DEFINE_STRUCT(afxManagedExtension)
{
    afxLinkage  lnk;
    afxNat      off;
    afxNat      siz;
    afxFcc      fcc;
    afxError    (*dtor)(afxObject obj, afxNat off, afxNat siz);
    afxError    (*ctor)(afxObject obj, afxNat off, afxNat siz, afxCookie const* cookie); // void to avoid warnings
    afxError    (*cpy)(afxObject dst, afxObject src, afxNat off, afxNat siz);
    afxError    (*ioAlways)(afxStream iob, afxObject obj, afxNat off, afxNat siz); // called after the reading of plugin stream data is finished(useful to set up plugin data for plugins that found no data in the stream, but that cannot set up the data during the ctor callback)
    afxError    (*ioRights)(afxStream iob, afxObject obj, afxNat off, afxNat siz, void* data); // called after the reading of plugin stream data is finished, and the object finalised, if and only if the object's rights id was equal to that of the plugin registering the call.
    afxError    (*ioWrite)(afxStream iob, afxObject obj, afxNat off, afxNat siz); // writes extension data to a binary stream.
    afxError    (*ioRead)(afxStream iob, afxObject obj, afxNat off, afxNat siz); // reads extension data from a binary stream.
    afxSize     (*ioSiz)(afxStream iob, afxObject obj, afxNat off, afxNat siz); // determines the binary size of the extension data.
};

AFX_DEFINE_STRUCT(afxClassPlugin)
{
    afxLinkage  cls;
    afxFcc      pluginId;
    afxNat      objOff;
    afxNat      objSiz;
    afxError    (*dtor)(afxObject obj, afxNat off, afxNat siz);
    afxError    (*ctor)(afxObject obj, afxNat off, afxNat siz, afxCookie const* cookie); // void to avoid warnings
    afxError    (*cpy)(afxObject dst, afxObject src, afxNat off, afxNat siz);
    afxError    (*ioAlways)(afxStream iob, afxObject obj, afxNat off, afxNat siz); // called after the reading of plugin stream data is finished(useful to set up plugin data for plugins that found no data in the stream, but that cannot set up the data during the ctor callback)
    afxError    (*ioRights)(afxStream iob, afxObject obj, afxNat off, afxNat siz, void* data); // called after the reading of plugin stream data is finished, and the object finalised, if and only if the object's rights id was equal to that of the plugin registering the call.
    afxError    (*ioWrite)(afxStream iob, afxObject obj, afxNat off, afxNat siz); // writes extension data to a binary stream.
    afxError    (*ioRead)(afxStream iob, afxObject obj, afxNat off, afxNat siz); // reads extension data from a binary stream.
    afxSize     (*ioSiz)(afxStream iob, afxObject obj, afxNat off, afxNat siz); // determines the binary size of the extension data.
};

AFX_DEFINE_STRUCT(afxManager)
{
    afxFcc          fcc; // afxFcc_CLS
    afxFcc          objFcc;
    afxChar         name[32];
    afxLinkage      host; // the object that installed this type on Qwadro. Usually a system and/or module.
    afxLinkage      subset; // inherit this object.
    afxChain        supersets;
    afxNat          level;
    afxNat          levelMask;

    afxNat          instCnt; // constructed, not allocated as from pool. We can't rely on pool due to it not being used for static instances.
    afxNat          maxInstCnt; // 1 == singleton
    afxPool         pool;
    afxSlock        poolLock;
    afxNat          unitsPerPage; // when pool gets empty, the class will try to resizes storage pages to this value.

    afxError        (*ctor)(afxObject obj, afxCookie const* cookie); // void to avoid warnings
    afxError        (*dtor)(afxObject obj);
    afxError        (*ioAlways)(afxStream iob, afxObject obj); // called after the reading of plugin stream data is finished(useful to set up plugin data for plugins that found no data in the stream, but that cannot set up the data during the ctor callback)
    afxError        (*ioRights)(afxStream iob, afxObject obj, void* data); // called after the reading of plugin stream data is finished, and the object finalised, if and only if the object's rights id was equal to that of the plugin registering the call.
    afxError        (*ioWrite)(afxStream iob, afxObject obj); // writes extension data to a binary stream.
    afxError        (*ioRead)(afxStream iob, afxObject obj); // reads extension data from a binary stream.
    afxSize         (*ioSiz)(afxStream iob, afxObject obj); // determines the binary size of the extension data.
    
    afxManagedExtension anchor;

    afxBool         (*defEvent)(afxObject obj, afxEvent *ev);
    afxBool         (*defEventFilter)(afxObject obj, afxObject watched, afxEvent *ev);
    
    afxMmu          mmu; // afxMmu
    afxNat          siz;

    afxNat          instBaseSiz;
    void const**    vmt;
    afxString const*vmtNames;
    void*           userData[4];
};

AFX afxError        AfxEstablishManager(afxManager* mgr, afxManager *base, afxChain* provider, afxClassConfig const *spec);
AFX afxError        AfxAbolishManager(afxManager* mgr);
AFX afxNat          AfxExhaustManager(afxManager* mgr);

AFX afxNat          AfxCountObjects(afxManager const* mgr);
AFX afxNat          AfxGetObjectSize(afxManager const* mgr);
AFX afxNat          AfxGetObjectStrictSize(afxManager const* mgr);

AFX afxManager*     AfxGetSubmanager(afxManager const* mgr);
AFX afxObject       AfxGetObject(afxManager const* mgr, afxNat32 uniqueId);

AFX afxNat          AfxEnumerateObjects(afxManager const* mgr, afxNat first, afxNat cnt, afxObject objects[]);
AFX afxNat          AfxInvokeObjects(afxManager const* mgr, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void *udd);

AFX afxError        _AfxDeallocateObjects(afxManager* mgr, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxAllocateObjects(afxManager* mgr, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxAllocateObjectsAt(afxManager* mgr, afxNat base, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxDestructObjects(afxManager* mgr, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxConstructObjects(afxManager* mgr, afxNat cnt, afxObject objects[], void** udd);

AFX afxResult       AfxCleanUpChainedManagers(afxChain* ch);
AFX afxResult       AfxExhaustChainedManagers(afxChain* ch);
AFX afxChain*       _AfxGetOrphanClasses(void);

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))
#   define AfxAssertClass(cls_, objFcc_)    ((!!((cls_) && ((cls_)->fcc == afxFcc_CLS) && ((cls_)->objFcc == (objFcc_)))) || (AfxThrowError(), AfxLogError("%s\n    %s", AFX_STRINGIFY((var_)), errorMsg[AFXERR_INVALID]), 0))
#else
#   define AfxAssertClass(cls_, fcc_) ((void)(err))
#endif

#endif//AFX_CLASS_H
