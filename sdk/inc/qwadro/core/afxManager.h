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
    AFX_OBJB_FLAG_DONT_DELETE   = AFX_BIT_OFFSET(0)
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
    afxManager*     base;
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

AFX_DEFINE_STRUCT(afxManager)
{
    afxFcc          fcc; // afxFcc_CLS
    afxFcc          objFcc;
    afxLinkage      base; // inherit this object.
    afxChain        deriveds;
    afxNat          level;
    afxNat          levelMask;

    afxLinkage      provider; // the object that installed this type on Qwadro. Usually a system and/or module.
    afxMmu          mmu; // afxMmu
    afxNat          siz;

    afxError        (*ctor)(afxObject obj, afxCookie const* cookie); // void to avoid warnings
    afxError        (*dtor)(afxObject obj);
    afxError        (*output)(afxObject obj, void* ios); // afxStream
    afxError        (*input)(afxObject obj, void* ios);
    
    afxBool         (*event)(afxObject obj, afxEvent *ev);
    afxBool         (*eventFilter)(afxObject obj, afxObject watched, afxEvent *ev);
    
    afxSlock        slock;
    afxPool         pool;
    afxNat          nextPreallocCnt; // when pool gets empty, the class will try to resizes storage pages to this value.

    afxChar         name[32];

    afxNat          maxInstCnt;
    afxNat          instBaseSiz;
    void const**    vmt;
    afxString const*vmtNames;
    void*           userData[4];
};

AFX afxError        AfxSetUpManager(afxManager *cls, afxManager *base, afxChain* provider, afxClassConfig const *spec);
AFX afxError        AfxCleanUpManager(afxManager *cls);
AFX afxNat          AfxExhaustManager(afxManager *cls);

AFXINL afxNat       AfxCountObjects(afxManager const* cls);
AFXINL afxNat       AfxGetObjectSize(afxManager const* cls);
AFXINL afxNat       AfxGetObjectStrictSize(afxManager const* cls);

AFXINL afxManager*    AfxGetBaseClass(afxManager const* cls);
AFXINL void const*  AfxClassGetVmt(afxManager const* cls);
AFXINL afxObject    AfxGetObjectAt(afxManager const* cls, afxNat32 uniqueId);

AFXINL afxNat       AfxEnumerateObjects(afxManager const* cls, afxNat first, afxNat cnt, afxObject objects[]);
AFXINL afxNat       AfxInvokeObjects(afxManager const* cls, afxNat first, afxNat cnt, afxBool(*f)(afxObject obj, void *udd), void *udd);

AFX afxError        _AfxDeallocateObjects(afxManager *cls, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxAllocateObjects(afxManager *cls, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxAllocateObjectsAt(afxManager *cls, afxNat base, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxDestructObjects(afxManager *cls, afxNat cnt, afxObject objects[]);
AFX afxError        _AfxConstructObjects(afxManager *cls, afxNat cnt, afxObject objects[], void** udd);

AFX void    AfxClassLockInclusive(afxManager *cls);
AFX void    AfxClassLockExclusive(afxManager *cls);

AFX void    AfxClassUnlockInclusive(afxManager *cls);
AFX void    AfxClassUnlockExclusive(afxManager *cls);

AFX afxBool AfxClassTryLockInclusive(afxManager *cls);
AFX afxBool AfxClassTryLockExclusive(afxManager *cls);

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))
#   define AfxAssertClass(cls_, objFcc_)    ((!!((cls_) && ((cls_)->fcc == afxFcc_CLS) && ((cls_)->objFcc == (objFcc_)))) || (AfxThrowError(), AfxLogError("%s\n    %s", AFX_STRINGIFY((var_)), errorMsg[AFXERR_INVALID]), 0))
#else
#   define AfxAssertClass(cls_, fcc_) ((void)(err))
#endif

AFX afxResult _AfxShutdownOrphanClasses(void);
AFX afxResult _AfxUninstallChainedClasses(afxChain *ch);
AFX afxResult _AfxExhaustChainedClasses(afxChain *provisions);
AFX afxChain* _AfxGetOrphanClasses(void);

#endif//AFX_CLASS_H
