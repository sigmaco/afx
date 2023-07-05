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
 *                               (c) 2017 Federação SIGMA
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
#define AfxThrowError() ((err) = (afxError)(-((afxNat16)__LINE__)), AfxOutputError(AfxSpawnHint(),""))
#define AfxResetResult(rslt) (rslt = AFX_SUCCESS)
#define AfxCatchError(rslt) if (rslt) { AfxOutputError(((afxHint const){ (afxSize)(__FILE__), (afxSize)(rslt), (afxSize)(__func__) }),""); }

AFX afxResult       AfxAttachDebugger(afxChar const* file);
AFX afxResult       AfxDetachDebugger(void);
AFX afxBool         AfxIsDebuggerAttached(void);

AFX afxResult       AfxLockDebugger(void);
AFX afxResult       AfxUnlockDebugger(void);

AFX afxResult       AfxOutputDebugString(afxNat32 color, afxChar const* msg);
AFX afxResult       AfxOutputDebugFormattedString(afxNat32 color, afxChar const* msg, ...);

AFX void            AfxOutputEntry(afxHint const hint, afxChar const* args, ...); // green
AFX void            AfxOutputEcho(afxHint const hint, afxChar const* msg, ...); // green
AFX void            AfxOutputError(afxHint const hint, afxChar const* msg, ...); // red
AFX void            AfxOutputComment(afxHint const hint, afxChar const* msg, ...); // grey
AFX void            AfxOutputAdvertence(afxHint const hint, afxChar const* msg, ...); // yellow
AFX void            AfxOutputAssistence(afxHint const hint, afxChar const* msg, ...); // purple

AFXINL afxChar* AfxFindPathTarget(afxChar const* path)
{
    afxChar* start = (afxChar*)path, *p = (afxChar*)path;
    while (*(p)++);
    while (--(p) != start && *(p) != (afxChar)'/' && *(p) != (afxChar)'\\' && *(p) != (afxChar)':');
    return((*(p) == (afxChar)'/' || *(p) == (afxChar)'\\' || *(p) == (afxChar)':') ? (afxChar*)++p : NIL);
}

#define __AFX_FILE__ AfxFindPathTarget(__FILE__)

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))

#   define AfxAssert(cond_)             ((!!((cond_)))||(AfxThrowError(),AfxOutputError(AfxSpawnHint(),"%s\n    %s",AfxStr((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertSoft(cond_)         ((!!((cond_)))||(AfxThrowError(),AfxOutputAdvertence(AfxSpawnHint(),"%s\n    %s",AfxStr((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertDiff(a_,b_)         ((!!(((void*)(a_) != (void*)(b_))))||(AfxThrowError(),AfxOutputError(AfxSpawnHint(),"%s\n    %s",AfxStr((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertDiffSoft(a_,b_)         ((!!(((void*)(a_) != (void*)(b_))))||(AfxThrowError(),AfxOutputAdvertence(AfxSpawnHint(),"%s\n    %s",AfxStr((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertType(var_, fcc_)    ((!!((var_) && ((var_)->fcc == (fcc_))))||(AfxThrowError(),AfxOutputError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxTryAssertType(var_, fcc_) ((!!(!(var_) || ((var_)->fcc == (fcc_))))||(AfxThrowError(),AfxOutputError(AfxSpawnHint(),"%s\n    %s",AfxStr((var_)),errorMsg[AFXERR_INVALID]),0))

#else

#   define AfxAssert(cond_) ((void)(err))
#   define AfxAssertDiff(a_,b_) ((void)(err))
#   define AfxAssertDiffSoft(a_,b_) ((void)(err)) // when input causes just computational power loss
#   define AfxAssertType(var_, fcc_) ((void)(err))
#   define AfxTryAssertType(var_, fcc_) ((void)(err))

#endif

#if ((defined(_AFX_DEBUG) || defined(_AFX_TRACE)))
#   define AfxEntry(args, ...) AfxOutputEntry(AfxSpawnHint(),args,__VA_ARGS__)
#else
#   define AfxEntry(args, ...)
#endif


#ifdef TRUE// ((defined(_AFX_DEBUG)))

#   define AfxEcho(msg, ...)        AfxOutputEcho(AfxSpawnHint(),msg,__VA_ARGS__)
#   define AfxError(msg, ...)       AfxOutputError(AfxSpawnHint(),msg,__VA_ARGS__)
#   define AfxComment(msg, ...)     AfxOutputComment(AfxSpawnHint(),msg,__VA_ARGS__)
#   define AfxAssist(msg, ...)      AfxOutputAssistence(AfxSpawnHint(),msg,__VA_ARGS__)
#   define AfxAdvertise(msg, ...)   AfxOutputAdvertence(AfxSpawnHint(),msg,__VA_ARGS__)

#else

#   define AfxEcho(msg, ...)
#   define AfxError(msg, ...)
#   define AfxComment(msg, ...)
#   define AfxAssist(msg, ...)
#   define AfxAdvertise(msg, ...)

#endif

#ifdef _AFX_DEBUG
#   define _AFX_DBG_FCC     afxFcc    fcc
#   define _AFX_DBG_FCC16   afxNat16  fcc
#else
#   define _AFX_DBG_FCC 
#   define _AFX_DBG_FCC16 
#endif

#endif//AFX_DEBUG_H