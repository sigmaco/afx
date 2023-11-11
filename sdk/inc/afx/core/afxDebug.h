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

#ifndef AFX_DEBUG_H
#define AFX_DEBUG_H

#include "afx/core/afxCoreDefs.h"
#include "afx/core/afxFcc.h"

#if _DEBUG
#define _AFX_DEBUG 1
#define _AFX_EXPECT 1
#define _AFX_TRACE 1
#else
//#define _AFX_DISABLE_DEBUGGER 1
#endif

typedef enum
{
    // NIL // end of
    AFX_HINT_FNAME = 1,
    AFX_HINT_FLINE,
    AFX_HINT_FUNC,
} afxHintAttr;

typedef afxSize afxHint[3];

#define AfxSpawnHint() ((afxHint const){ (afxSize)(__FILE__), (afxSize)(__LINE__), (afxSize)(__func__) })

//#define AFX_SUCCESS ((afxResult)NIL)
#define AfxThrowError() ((err) = (afxError)(-((afxNat16)__LINE__)), AfxLogError(AfxSpawnHint(),""))
#define AfxResetResult(rslt) (rslt = AFX_SUCCESS)
#define AfxCatchError(rslt) if (rslt) { AfxLogError(((afxHint const){ (afxSize)(__FILE__), (afxSize)(rslt), (afxSize)(__func__) }),""); }

AFX afxResult       AfxAttachDebugger(afxChar const* file);
AFX afxResult       AfxDetachDebugger(void);
AFX afxBool         AfxDebuggerIsAttached(void);

AFX afxResult       AfxLockDebugger(void);
AFX afxResult       AfxUnlockDebugger(void);

AFX afxResult       AfxLogMessage(afxNat32 color, afxChar const* msg);
AFX afxResult       AfxLogMessageFormatted(afxNat32 color, afxChar const* msg, ...);

AFX void            AfxLogCall(afxHint const hint, afxChar const* args, ...); // green
AFX void            AfxLogEcho(afxHint const hint, afxChar const* msg, ...); // green
AFX void            AfxLogError(afxHint const hint, afxChar const* msg, ...); // red
AFX void            AfxLogComment(afxHint const hint, afxChar const* msg, ...); // grey
AFX void            AfxLogAdvertence(afxHint const hint, afxChar const* msg, ...); // yellow
AFX void            AfxLogAssistence(afxHint const hint, afxChar const* msg, ...); // purple

AFXINL afxChar* AfxFindPathTarget(afxChar const* path)
{
    afxChar* start = (afxChar*)path, *p = (afxChar*)path;
    while (*(p)++);
    while (--(p) != start && *(p) != (afxChar)'/' && *(p) != (afxChar)'\\' && *(p) != (afxChar)':');
    return((*(p) == (afxChar)'/' || *(p) == (afxChar)'\\' || *(p) == (afxChar)':') ? (afxChar*)++p : NIL);
}

#define __AFX_FILE__ AfxFindPathTarget(__FILE__)

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))

//#   define AfxAssertRange(total_, base_, range_) ((!!((total_ >= base_ + range_)))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"%s(%u) + %s(%u) is out of range [0, %s(%u)]",AFX_STRINGIFY(base_),(base_),AFX_STRINGIFY(range_),(range_),AFX_STRINGIFY(total_),(total_)),0))
//#   define AfxAssertRangei(total_, base_, range_) ((!!((total_ >= base_ + range_)))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"%s(%i) + %s(%i) is out of range [0, %s(%i)]",AFX_STRINGIFY(base_),(base_),AFX_STRINGIFY(range_),(range_),AFX_STRINGIFY(total_),(total_)),0))
//#   define AfxAssertRangef(total_, base_, range_) ((!!((total_ >= base_ + range_)))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"%s(%f) + %s(%f) is out of range [0, %s(%f)]",AFX_STRINGIFY(base_),(base_),AFX_STRINGIFY(range_),(range_),AFX_STRINGIFY(total_),(total_)),0))

// TODO override old ones
#   define AfxAssertRange(capacity_, first_, count_)  ((!!((capacity_ >= first_ + count_)))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"<%s>%u + <%s>%u is out of capacity <%s>%u.",AFX_STRINGIFY(capacity_),(capacity_),AFX_STRINGIFY(first_),(first_),AFX_STRINGIFY(count_),(count_)),0))
#   define AfxAssertRangei(capacity_, first_, count_) ((!!((capacity_ >= first_ + count_)))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"<%s>%i + <%s>%i is out of capacity <%s>%i.",AFX_STRINGIFY(capacity_),(capacity_),AFX_STRINGIFY(first_),(first_),AFX_STRINGIFY(count_),(count_)),0))
#   define AfxAssertRangef(capacity_, first_, count_) ((!!((capacity_ >= first_ + count_)))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"<%s>%f + <%s>%f is out of capacity <%s>%f.",AFX_STRINGIFY(capacity_),(capacity_),AFX_STRINGIFY(first_),(first_),AFX_STRINGIFY(count_),(count_)),0))
#   define AfxAssertCapacity(capacity_, unit_)        ((!!((capacity_ >= unit_))))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"%s<%u> is out of capacity %s<%u>.",AFX_STRINGIFY(unit_),(unit_),AFX_STRINGIFY(capacity_),(capacity_)),0))

#   define AfxAssertBool(value_) ((!!((value_!=FALSE)||(value_!=TRUE)))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"<%s>%u is a illegal bool.",AFX_STRINGIFY(value_),(value_)),0))
#   define AfxAssertAbs(value_) ((!!((value_>=0)))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"<%s>%u is not absolute.",AFX_STRINGIFY(value_),(value_)),0))

// diferente de um range, um extent sempre há um valor mínimo. Algo não pode ter largura igual a zero e existir.
#   define AfxAssertExtent(total_, range_) (((!!(range_))&&(!!((total_ >= range_))))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"%s(%u) is out of range [1, %u]",AFX_STRINGIFY((range_)),(range_),AFX_STRINGIFY((total_)),(total_)),0))

#   define AfxAssert(cond_)  /*assert(cond_)*/           ((!!((cond_)))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"%s\n    %s",AFX_STRINGIFY((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertSoft(cond_)         ((!!((cond_)))||(AfxThrowError(),AfxLogAdvertence(AfxSpawnHint(),"%s\n    %s",AFX_STRINGIFY((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertDiff(a_,b_)         ((!!(((void*)(a_) != (void*)(b_))))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"%s\n    %s",AFX_STRINGIFY((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertDiffSoft(a_,b_)         ((!!(((void*)(a_) != (void*)(b_))))||(AfxThrowError(),AfxLogAdvertence(AfxSpawnHint(),"%s\n    %s",AFX_STRINGIFY((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertType(var_, fcc_)    ((!!((var_) && (*((afxFcc*)var_) == (fcc_))))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"%s\n    %s",AFX_STRINGIFY((var_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxTryAssertType(var_, fcc_) ((!!(!(var_) || (*((afxFcc*)var_) == (fcc_))))||(AfxThrowError(),AfxLogError(AfxSpawnHint(),"%s\n    %s",AFX_STRINGIFY((var_)),errorMsg[AFXERR_INVALID]),0))

#   define AfxAssignFcc(obj_,fcc_) (obj_)->fcc = (fcc_)
#   define AfxAssignTypeFcc(type_,fcc_) *((afxFcc*)type_) = (fcc_)
#else

#   define AfxAssert(cond_) ((void)(err))
#   define AfxAssertDiff(a_,b_) ((void)(err))
#   define AfxAssertDiffSoft(a_,b_) ((void)(err)) // when input causes just computational power loss
#   define AfxAssertType(var_, fcc_) ((void)(err))
#   define AfxTryAssertType(var_, fcc_) ((void)(err))

#   define AfxAssertRange(total_, base_, range_)
#   define AfxAssertRangei(total_, base_, range_)
#   define AfxAssertRangef(total_, base_, range_)

// diferente de um range, um extent sempre há um valor mínimo. Algo não pode ter largura igual a zero e existir.
#   define AfxAssertExtent(total_, range_)

#   define AfxAssignTypeFcc(type_,fcc_)
#endif

#if ((defined(_AFX_DEBUG) || defined(_AFX_TRACE)))
#   define AfxEntry(args, ...) AfxLogCall(AfxSpawnHint(),args,__VA_ARGS__)
#else
#   define AfxEntry(args, ...)
#endif


#ifdef TRUE// ((defined(_AFX_DEBUG)))

#   define AfxEcho(msg, ...)        AfxLogEcho(AfxSpawnHint(),msg,__VA_ARGS__)
#   define AfxError(msg, ...)       AfxLogError(AfxSpawnHint(),msg,__VA_ARGS__)
#   define AfxComment(msg, ...)     AfxLogComment(AfxSpawnHint(),msg,__VA_ARGS__)
#   define AfxAssist(msg, ...)      AfxLogAssistence(AfxSpawnHint(),msg,__VA_ARGS__)
#   define AfxAdvertise(msg, ...)   AfxLogAdvertence(AfxSpawnHint(),msg,__VA_ARGS__)

#else

#   define AfxEcho(msg, ...)
#   define AfxError(msg, ...)
#   define AfxComment(msg, ...)
#   define AfxAssist(msg, ...)
#   define AfxAdvertise(msg, ...)

#endif

#ifdef _AFX_DEBUG
#   define _AFX_DBG_FCC     afxFcc    fcc;
#   define _AFX_DBG_FCC16   afxNat16  fcc;
#else
#   define _AFX_DBG_FCC 
#   define _AFX_DBG_FCC16 
#endif

#endif//AFX_DEBUG_H