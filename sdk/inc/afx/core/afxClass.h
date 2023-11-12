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
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_CLASS_H
#define AFX_CLASS_H

#include "afx/core/afxFcc.h"
#include "afx/core/afxChain.h"
#include "afx/core/afxSlock.h"
#include "afx/core/afxInstance.h"
#include "afx/core/afxPool.h"

// RTTI of Qwadro

// The Qt Meta-Object System in Qt is responsible for the signals and slots inter-object communication mechanism, runtime class information, and the Qt property system.
// A single QMetaObject instance is created for each QObject subclass that is used in an application, and this instance stores all the class-information for the QObject subclass.
// This object is available as QObject::typeObject().

AFX_DECLARE_STRUCT(afxClassExtension);
AFX_DECLARE_STRUCT(afxInstance);
AFX_DECLARE_STRUCT(afxClass);
AFX_DECLARE_STRUCT(afxEvent);

#define _AFX_CLASS_LEVEL_MASK 0xF
#define _AFX_CLASS_BASE_LEVEL 1 // 0 is reserved for afxInstance only stuff

// A afxClass is a model for something which explains it or shows how it canv be produced.

AFX_DEFINE_STRUCT(afxIterator)
{
    afxNat          idx;
    afxBool         abort;
    void(*f)(afxIterator *itr);
    void            *udd;
    union
    {
        afxInstance   *objItem;
        void        *voidItem;
    };
};

typedef enum afxBlueprintFlags
{
    AFX_OBJB_FLAG_DONT_DELETE   = AfxGetBitOffset(0)
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

AFX_DEFINE_STRUCT(afxClassConfig)
{
    afxFcc          fcc;
    afxContext      ctx;
    afxNat          maxCnt;
    afxNat          size;
    afxClass*       base;
    afxError        (*ctor)(afxHandle obj, afxCookie const* cookie);
    afxError        (*dtor)(afxHandle obj);
    afxBool         (*event)(afxHandle obj, afxEvent *ev);
    afxBool         (*eventFilter)(afxHandle obj, afxHandle watched, afxEvent *ev);
    afxChar const*  name;
    void const*     vmt;
    afxNat          unitsPerPage;
};

AFX_DEFINE_STRUCT(afxClass)
{
    afxFcc          fcc; // afxFcc_CLS
    afxFcc          objFcc;
    afxLinkage      base; // inherit this object.
    afxChain        deriveds;
    afxNat          level;
    afxNat          levelMask;

    afxLinkage      provider; // the object that installed this type on Qwadro. Usually a system and/or module.
    afxContext      ctx; // afxContext
    afxSize         siz;

    afxError        (*ctor)(afxHandle obj, afxCookie const* cookie); // void to avoid warnings
    afxError        (*dtor)(afxHandle obj);
    afxError        (*output)(afxHandle obj, void* ios); // afxStream
    afxError        (*input)(afxHandle obj, void* ios);
    
    afxBool         (*event)(afxHandle obj, afxEvent *ev);
    afxBool         (*eventFilter)(afxHandle obj, afxHandle watched, afxEvent *ev);
    
    afxSlock        slock;
    afxPool         pool;
    afxNat          nextPreallocCnt; // when pool gets empty, the class will try to resizes storage pages to this value.

    afxChar         name[32];

    afxNat          maxInstCnt;
    afxNat          instBaseSiz;
    void const      *vmt;
    void*           userData[4];
};

AFX afxError        AfxMountClass(afxClass *cls, afxChain* provider, afxClassConfig const *spec);
AFX afxError        AfxInstallClassExtension(afxClass *cls, afxClass *base, afxChain* provider, afxClassConfig const *spec);
AFX afxError        AfxDismountClass(afxClass *cls);

//AFX afxError        AfxClassAllocateObjects(afxClass *cls, afxNat cnt, afxHandle obj[]);
//AFX afxError        AfxClassDeallocateObjects(afxClass *cls, afxNat cnt, afxHandle obj[]);

AFX afxError        AfxClassBuildObjects(afxClass *cls, afxNat cnt, afxHandle obj[], void* udd);
//AFX afxError        AfxClassRebuildObjects(afxClass *cls, afxNat cnt, afxInstance *obj[], afxBlueprint const blueprint[]); // Concept: should be used only to reuse allocation for already ready objects.
AFX afxError        AfxClassDismantleObjects(afxClass *cls, afxNat cnt, afxHandle obj[]);

AFXINL afxNat       AfxCountInstances(afxClass const* cls);
AFXINL afxNat       AfxGetInstanceSize(afxClass const* cls);
AFXINL afxNat       AfxGetInstanceAddedSsize(afxClass const* cls);

AFXINL afxClass*    AfxGetBaseClass(afxClass const* cls);
AFXINL void const*  AfxClassGetVmt(afxClass const* cls);
AFXINL afxBool      AfxGetInstance(afxClass const* cls, afxNat32 uniqueId, afxHandle* obj);
AFXINL afxBool      AfxClassFindObjectIndex(afxClass const* cls, afxBool b2f, afxHandle obj, afxNat *idx);

AFXINL afxNat       AfxEnumerateInstances(afxClass const* cls, afxNat first, afxNat cnt, afxHandle obj[]);

AFXINL afxNat       AfxCurateInstances(afxClass const* cls, afxNat first, afxNat cnt, afxBool(*f)(afxHandle obj, void *udd), void *udd);

AFX afxResult       AfxClassForEveryInstance(afxClass *cls, afxBool exclusive, void(*f)(afxIterator *obji), void *data);
AFX afxResult       AfxClassForEveryDerivedInstance(afxClass *cls, afxFcc superset, afxBool exclusive, void(*f)(afxIterator *obji), void *data);

AFX void    AfxClassLockInclusive(afxClass *cls);
AFX void    AfxClassLockExclusive(afxClass *cls);

AFX void    AfxClassUnlockInclusive(afxClass *cls);
AFX void    AfxClassUnlockExclusive(afxClass *cls);

AFX afxBool AfxClassTryLockInclusive(afxClass *cls);
AFX afxBool AfxClassTryLockExclusive(afxClass *cls);

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))
#   define AfxAssertClass(cls_, objFcc_)    ((!!((cls_) && ((cls_)->fcc == afxFcc_CLS) && ((cls_)->objFcc == (objFcc_)))) || (AfxThrowError(), AfxLogError(AfxSpawnHint(), "%s\n    %s", AFX_STRINGIFY((var_)), errorMsg[AFXERR_INVALID]), 0))
#else
#   define AfxAssertClass(cls_, fcc_) ((void)(err))
#endif

AFX afxResult _AfxShutdownOrphanClasses(void);
AFX afxResult _AfxUninstallChainedClasses(afxChain *ch);

#endif//AFX_CLASS_H