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

#ifndef AFX_CLASS_H
#define AFX_CLASS_H

#include "afxFcc.h"
#include "afxLinkedList.h"
#include "../async/afxTransistor.h"

// RTTI of Qwadro

// The Qt Meta-Object System in Qt is responsible for the signals and slots inter-object communication mechanism, runtime cls information, and the Qt property system.
// A single QMetaObject instance is created for each QObject subclass that is used in an application, and this instance stores all the cls-information for the QObject subclass.
// This object is available as QObject::typeObject().

AFX_DEFINE_HANDLE(afxAllocator);

AFX_DECLARE_STRUCT(afxClassExtension);
AFX_DECLARE_STRUCT(afxObject);
AFX_DECLARE_STRUCT(afxClass);
AFX_DECLARE_STRUCT(afxEvent);

// A afxClass is a model for something which explains it or shows how it canv be produced.

AFX_DEFINE_STRUCT(afxClass)
{
    afxFcc          fcc; // AFX_FCC_CLS
    afxFcc          objFcc;
    afxLinkage      base; // inherit this object.
    afxChain        deriveds;

    afxLinkage      provider; // the object that installed this type on Qwadro. Usually a system and/or module.
    void*           all; // afxAllocator
    afxSize         siz;
    afxError        (*ctor)(afxObject* obj, void const *paradigm); // void to avoid warnings
    afxError        (*dtor)(afxObject* obj);
    afxError        (*output)(afxObject* obj, void* ios); // afxStream
    afxError        (*input)(afxObject* obj, void* ios);
    
    afxBool         (*event)(afxObject *obj, afxEvent *ev);

    afxTransistor   transitor;
    afxChain        instances;

    afxChar         name[32];

    afxNat          maxInstCnt;
    afxNat          instBaseSiz;
    void const      *vmt;
};

AFX_DEFINE_STRUCT(afxIterator)
{
    afxNat          idx;
    afxBool         abort;
    void            (*f)(afxIterator *itr);
    void            *udd;
    union
    {
        afxObject   *objItem;
        void        *voidItem;
    };
};

AFX_DEFINE_STRUCT(afxClassSpecification)
{
    afxFcc          fcc;
    afxAllocator   all;
    afxNat          maxCnt;
    afxNat          size;
    afxClass*       base;
    afxError        (*ctor)(afxObject* obj, void const *paradigm);
    afxError        (*dtor)(afxObject* obj);
    afxChar const*  name;
    void const*     vmt;
};

AFX afxError AfxClassRegister(afxClass *cls, afxChain* provider, afxClass *base, afxClassSpecification const *spec);
AFX afxError AfxClassDeregister(afxClass *cls);

AFXINL afxClass* AfxClassGetBase(afxClass const *cls);
AFXINL void const* AfxClassGetVmt(afxClass const *cls);
AFXINL afxObject* AfxClassGetInstance(afxClass const *cls, afxBool b2f, afxNat idx);
AFXINL afxNat AfxClassGetInstanceIndex(afxClass const *cls, afxBool b2f, afxObject const *obj);

AFXINL afxNat AfxClassGetInstanceCount(afxClass *cls);
AFXINL afxNat AfxClassGetInstanceSize(afxClass *cls);
AFXINL afxNat AfxClassGetSubsetSize(afxClass *cls);

AFXINL afxResult AfxClassAllocateInstances(afxClass *cls, afxNat cnt, afxObject *obj[], afxHint const hint);
AFXINL afxResult AfxClassDeallocateInstances(afxClass *cls, afxNat cnt, afxObject *obj[]);

AFX afxResult AfxClassDestructInstances(afxClass *cls, afxNat cnt, afxObject *obj[]);
AFX afxResult AfxClassConstructInstances(afxClass *cls, afxNat cnt, afxObject *obj[], void const *args[]);

AFXINL afxResult AfxClassEnumerateInstances(afxClass *cls, afxBool reverse, afxNat base, afxNat cnt, afxObject *obj[]);

AFX afxResult AfxClassForEveryInstance(afxClass *cls, afxBool exclusive, void(*f)(afxIterator *obji), void *data);
AFX afxResult AfxClassForEveryDerivedInstance(afxClass *cls, afxFcc superset, afxBool exclusive, void(*f)(afxIterator *obji), void *data);

AFX void    AfxClassLockInclusive(afxClass *cls);
AFX void    AfxClassLockExclusive(afxClass *cls);

AFX void    AfxClassUnlockInclusive(afxClass *cls);
AFX void    AfxClassUnlockExclusive(afxClass *cls);

AFX afxBool AfxClassTryLockInclusive(afxClass *cls);
AFX afxBool AfxClassTryLockExclusive(afxClass *cls);

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))
#   define AfxAssertClass(cls_, fcc_)    ((!!((cls_) && ((cls_)->fcc == AFX_FCC_CLS) && ((cls_)->objFcc == (fcc_)))) || (AfxThrowError(), AfxOutputError(AfxSpawnHint(), "%s\n    %s", AfxStr((var_)), errorMsg[AFXERR_INVALID]), 0))
#else
#   define AfxAssertClass(cls_, fcc_) ((void)(err))
#endif

AFX afxResult _AfxShutdownOrphanClasses(void);
AFX afxResult _AfxDropClassChain(afxChain *ch);

#endif//AFX_CLASS_H