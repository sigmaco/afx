/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_CLASS_H
#define AFX_CLASS_H

#include "afx/core/afxFcc.h"
#include "afx/core/afxChain.h"
#include "afx/core/async/afxSlock.h"
#include "afx/core/afxInstance.h"

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
    AFX_OBJB_FLAG_DONT_DELETE   = AFX_FLAG(0)
} afxBlueprintFlags;

AFX_DEFINE_STRUCT(afxBlueprint)
{
    afxFcc              fcc;
    afxBlueprintFlags   flags;
};

AFX_DEFINE_STRUCT(afxClassSpecification)
{
    afxFcc          fcc;
    afxContext       all;
    afxNat          maxCnt;
    afxNat          size;
    afxClass*       base;
    afxError        (*ctor)(void *cache, afxNat idx, afxInstance* obj, void const *blueprint);
    afxError        (*dtor)(afxInstance* obj);
    afxBool         (*event)(afxInstance *obj, afxEvent *ev);
    afxBool         (*eventFilter)(afxInstance *obj, afxInstance *watched, afxEvent *ev);
    afxChar const*  name;
    void const*     vmt;
};

AFX_DEFINE_STRUCT(afxClass)
{
    afxFcc          fcc; // AFX_FCC_CLS
    afxFcc          objFcc;
    afxLinkage      base; // inherit this object.
    afxChain        deriveds;
    afxNat          level;
    afxNat          levelMask;

    afxLinkage      provider; // the object that installed this type on Qwadro. Usually a system and/or module.
    void*           all; // afxContext
    afxSize         siz;

    afxBlueprint*   (*formulate)(afxBlueprint *blueprint);
    afxError        (*discard)(afxBlueprint *blueprint);

    afxError        (*ctor)(void *cache, afxNat idx, afxInstance* obj, void const *blueprint); // void to avoid warnings
    afxError        (*dtor)(afxInstance* obj);
    afxError        (*output)(afxInstance* obj, void* ios); // afxStream
    afxError        (*input)(afxInstance* obj, void* ios);
    
    afxBool         (*event)(afxInstance *obj, afxEvent *ev);
    afxBool         (*eventFilter)(afxInstance *obj, afxInstance *watched, afxEvent *ev);
    
    afxSlock        slock;
    afxChain        instances;

    afxChar         name[32];

    afxNat          maxInstCnt;
    afxNat          instBaseSiz;
    void const      *vmt;
    void*           userData[4];
};

AFX afxError        AfxMountClass(afxClass *cls, afxChain* provider, afxClassSpecification const *spec);
AFX afxError        AfxMountClassInherited(afxClass *cls, afxClass *base, afxChain* provider, afxClassSpecification const *spec);
AFX afxError        AfxClassDismount(afxClass *cls);

AFX afxError        AfxClassAllocateObjects(afxClass *cls, afxNat cnt, afxInstance *obj[], afxHint const hint);
AFX afxError        AfxClassDeallocateObjects(afxClass *cls, afxNat cnt, afxInstance *obj[]);

AFX afxError        AfxClassRequestBlueprints(afxClass *cls, afxNat cnt, afxBlueprint *blueprint[]);
AFX afxError        AfxClassDiscardBlueprints(afxClass *cls, afxNat cnt, afxBlueprint *blueprint[]);

AFX afxError        AfxClassBuildObjects(afxClass *cls, void *cache, afxNat cnt, void const *blueprint, afxInstance *obj[]);
//AFX afxError        AfxClassRebuildObjects(afxClass *cls, afxNat cnt, afxInstance *obj[], afxBlueprint const blueprint[]); // Concept: should be used only to reuse allocation for already ready objects.
AFX afxError        AfxClassDismantleObjects(afxClass *cls, afxNat cnt, afxInstance *obj[]);

AFX afxError        AfxClassAcquireObjects(afxClass *cls, void *cache, afxNat cnt, void const *blueprint, afxInstance *obj[], afxHint const hint);

AFXINL afxNat       AfxGetClassInstanceCount(afxClass const* cls);
AFXINL afxNat       AfxClassGetSizeOfObject(afxClass const* cls);
AFXINL afxNat       AfxClassGetSizeOfSubset(afxClass const* cls);

AFXINL afxClass*    AfxClassGetBase(afxClass const* cls);
AFXINL void const*  AfxClassGetVmt(afxClass const* cls);
AFXINL afxInstance*   AfxClassGetObject(afxClass const* cls, afxBool b2f, afxNat idx);
AFXINL afxBool      AfxClassFindObjectIndex(afxClass const* cls, afxBool b2f, afxInstance const* obj, afxNat *idx);

AFXINL afxNat       AfxEnumerateFirstClassInstances(afxClass* cls, afxNat first, afxNat cnt, afxInstance *obj[]);
AFXINL afxNat       AfxEnumerateLastClassInstances(afxClass* cls, afxNat first, afxNat cnt, afxInstance *obj[]);

AFXINL afxNat       AfxCurateFirstClassInstances(afxClass* cls, afxNat first, afxNat cnt, afxBool(*f)(afxInstance *obj, void *udd), void *udd);
AFXINL afxNat       AfxCurateLastClassInstances(afxClass* cls, afxNat first, afxNat cnt, afxBool(*f)(afxInstance *obj, void *udd), void *udd);

AFX afxResult       AfxClassForEveryInstance(afxClass *cls, afxBool exclusive, void(*f)(afxIterator *obji), void *data);
AFX afxResult       AfxClassForEveryDerivedInstance(afxClass *cls, afxFcc superset, afxBool exclusive, void(*f)(afxIterator *obji), void *data);

AFX void    AfxClassLockInclusive(afxClass *cls);
AFX void    AfxClassLockExclusive(afxClass *cls);

AFX void    AfxClassUnlockInclusive(afxClass *cls);
AFX void    AfxClassUnlockExclusive(afxClass *cls);

AFX afxBool AfxClassTryLockInclusive(afxClass *cls);
AFX afxBool AfxClassTryLockExclusive(afxClass *cls);

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))
#   define AfxAssertClass(cls_, objFcc_)    ((!!((cls_) && ((cls_)->fcc == AFX_FCC_CLS) && ((cls_)->objFcc == (objFcc_)))) || (AfxThrowError(), AfxLogError(AfxSpawnHint(), "%s\n    %s", AfxStr((var_)), errorMsg[AFXERR_INVALID]), 0))
#else
#   define AfxAssertClass(cls_, fcc_) ((void)(err))
#endif

AFX afxResult _AfxShutdownOrphanClasses(void);
AFX afxResult _AfxDropClassChain(afxChain *ch);




AFX afxError        AfxClass_AllocateInstances(afxClass *cls, afxNat cnt, void *inst[], void const *substance, afxHint const hint);
AFX afxError        AfxClass_DeallocateInstances(afxClass *cls, afxNat cnt, void *inst[]);
AFX afxError        AfxClass_BuildInstances(afxClass *cls, afxNat cnt, void *inst[], void const *substance, void *cache);
AFX afxError        AfxClass_RuinInstances(afxClass *cls, afxNat cnt, void *inst[]);

#endif//AFX_CLASS_H