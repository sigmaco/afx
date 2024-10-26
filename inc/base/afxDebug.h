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

#ifndef AFX_DEBUG_H
#define AFX_DEBUG_H

#include "qwadro/inc/base/afxCoreDefs.h"
#include "qwadro/inc/base/afxString.h"
#include "qwadro/inc/base/afxFcc.h"

#if _DEBUG
#define _AFX_DEBUG 1
#define _AFX_EXPECT 1
#define _AFX_TRACE 1
#else
//#define _AFX_DISABLE_DEBUGGER 1
#endif

#ifdef _AFX_DEBUG // after afxDebug!!!
#   define VLD_FORCE_ENABLE
#endif

#if 0
#if (defined(_WIN64) || defined(_WIN32))
#   ifdef VLD_FORCE_ENABLE
#       include <vld.h>
#   endif
#endif
#endif

typedef enum
{
    // NIL // end of
    AFX_HINT_FNAME = 1,
    AFX_HINT_FLINE,
    AFX_HINT_FUNC,
} afxHintAttr;

AFX_DEFINE_STRUCT(afxHere2)
{
    afxString   file;
    afxUnit      line;
    afxString   func;
};

typedef afxSize afxHere[3];

#ifndef AFX_ASSERT_HOOK // Function called on assert handling, optional
AFX_CALLBACK(void, afxAssertHook)(afxChar const* expr, afxChar const* file, afxInt line);
#define AFX_ASSERT_HOOK
#endif

AFX_CALLBACK(void, afxProfilerPopTimerFn)(void);
AFX_CALLBACK(void, afxProfilerPushTimerFn)(aaxPluginId pluginId, afxChar const* zoneName);
AFX_CALLBACK(void, afxProfilerPostMarkerFn)(aaxPluginId pluginId, afxChar const* markerName);

#define AfxHere() ((afxHere const){ (afxSize)(__FILE__), (afxSize)(__LINE__), (afxSize)(__func__) })
//#define AfxHint2() (){ AFX_STRINGIFY(), }

AFX afxResult       _AfxDbgAttach(afxChar const* file);
AFX afxResult       _AfxDbgDetach(void);
AFX afxBool         _AfxDbgIsAttached(void);

AFX afxResult       _AfxDbgLock(void);
AFX afxResult       _AfxDbgUnlock(void);

AFX afxResult       AfxDbgLog(afxUnit ch, afxChar const* msg, afxUnit len);
AFX afxResult       AfxDbgLogf(afxUnit ch, afxHere const hint, afxChar const* msg, ...);

#if 0
AFX void            AfxLogCall(afxHere const hint, afxChar const* args, ...); // green
AFX void            AfxLogEcho(afxHere const hint, afxChar const* msg, ...); // green
AFX void            AfxLogError(afxHere const hint, afxChar const* msg, ...); // red
AFX void            AfxLogComment(afxHere const hint, afxChar const* msg, ...); // grey
AFX void            AfxLogAdvertence(afxHere const hint, afxChar const* msg, ...); // yellow
AFX void            AfxLogAssistence(afxHere const hint, afxChar const* msg, ...); // purple
#endif

AFX afxChar const* _AfxDbgTrimFilename(afxChar const* path);

#define __AFX_FILE__ _AfxDbgTrimFilename(__FILE__)

#ifdef TRUE// ((defined(_AFX_DEBUG)))

#   define AfxLogEcho(msg_, ...)         AfxDbgLogf(2,(AfxHere()),(msg_),__VA_ARGS__)
#   define AfxLogError(msg_, ...)        AfxDbgLogf(9,(AfxHere()),(msg_),__VA_ARGS__)
#   define AfxLogAssertionFailure(msg_, ...)        AfxDbgLogf(8,(AfxHere()),(msg_),__VA_ARGS__)
#   define AfxLogComment(msg_, ...)      AfxDbgLogf(0,(AfxHere()),(msg_),__VA_ARGS__)
#   define AfxLogAssistence(msg_, ...)   AfxDbgLogf(4,(AfxHere()),(msg_),__VA_ARGS__)
#   define AfxLogAdvertence(msg_, ...)   AfxDbgLogf(7,(AfxHere()),(msg_),__VA_ARGS__)
#   define AfxLogY(msg_, ...)   AfxDbgLogf(6,(AfxHere()),(msg_),__VA_ARGS__)

#else

#   define AfxLogEcho(msg, ...)
#   define AfxLogError(msg, ...)
#   define AfxLogComment(msg, ...)
#   define AfxLogAssistence(msg, ...)
#   define AfxLogAdvertence(msg, ...)

#endif

//#define AFX_SUCCESS ((afxResult)NIL)
#define AFX_TEST(_exp_) if(_exp_) ((err) = (afxError)(-((afxUnit16)__LINE__)), AfxLogError(AFX_STRINGIFY((_exp_))))
#define AfxThrowError() ((err) = (afxError)(-((afxUnit16)__LINE__)), AfxLogError(""))
#define AfxThrowSoftError() ((err) = (afxError)(-((afxUnit16)__LINE__))) // does not echo
#define AfxResetResult(rslt) (rslt = AFX_SUCCESS)

#if ((defined(_AFX_DEBUG) || defined(_AFX_EXPECT)))

//#   define AFX_ASSERT_RANGE(total_, base_, range_) ((!!((total_ >= base_ + range_)))||(AfxThrowError(),AfxLogError(AfxHere(),"%s(%u) + %s(%u) is out of range [0, %s(%u)]",AFX_STRINGIFY(base_),(base_),AFX_STRINGIFY(range_),(range_),AFX_STRINGIFY(total_),(total_)),0))
//#   define AfxAssertRangei(total_, base_, range_) ((!!((total_ >= base_ + range_)))||(AfxThrowError(),AfxLogError(AfxHere(),"%s(%i) + %s(%i) is out of range [0, %s(%i)]",AFX_STRINGIFY(base_),(base_),AFX_STRINGIFY(range_),(range_),AFX_STRINGIFY(total_),(total_)),0))
//#   define AfxAssertRangef(total_, base_, range_) ((!!((total_ >= base_ + range_)))||(AfxThrowError(),AfxLogError(AfxHere(),"%s(%f) + %s(%f) is out of range [0, %s(%f)]",AFX_STRINGIFY(base_),(base_),AFX_STRINGIFY(range_),(range_),AFX_STRINGIFY(total_),(total_)),0))

// TODO override old ones
#   define AFX_ASSERT_RANGE(capacity_, first_, count_)  ((!!((capacity_ >= first_ + count_)))||(AfxThrowError(),AfxLogAssertionFailure("<%s>%u + <%s>%u is out of capacity <%s>%u.",AFX_STRINGIFY(first_),(first_),AFX_STRINGIFY(count_),(count_),AFX_STRINGIFY(capacity_),(capacity_)),0))
#   define AfxAssertRangei(capacity_, first_, count_) ((!!((capacity_ >= first_ + count_)))||(AfxThrowError(),AfxLogAssertionFailure("<%s>%i + <%s>%i is out of capacity <%s>%i.",AFX_STRINGIFY(first_),(first_),AFX_STRINGIFY(count_),(count_),AFX_STRINGIFY(capacity_),(capacity_)),0))
#   define AfxAssertRangef(capacity_, first_, count_) ((!!((capacity_ >= first_ + count_)))||(AfxThrowError(),AfxLogAssertionFailure("<%s>%f + <%s>%f is out of capacity <%s>%f.",AFX_STRINGIFY(first_),(first_),AFX_STRINGIFY(count_),(count_),AFX_STRINGIFY(capacity_),(capacity_)),0))
#   define AfxAssertCapacity(capacity_, unit_)        ((!!((capacity_ >= unit_))))||(AfxThrowError(),AfxLogAssertionFailure("%s<%u> is out of capacity %s<%u>.",AFX_STRINGIFY(unit_),(unit_),AFX_STRINGIFY(capacity_),(capacity_)),0))
#   define AFX_ASSERT_BOUNDS(value_, min_, max_)  ((!!((value_ >= min_ && value_ <= max_)))||(AfxThrowError(),AfxLogAssertionFailure("<%s>%u is out of bounds <%s>[%u, %u]<%s>.",AFX_STRINGIFY(value_),(value_),AFX_STRINGIFY(min_),(min_),(max_),AFX_STRINGIFY(max_)),0))

#   define AfxAssertBool(value_) ((!!((value_!=FALSE)||(value_!=TRUE)))||(AfxThrowError(),AfxLogAssertionFailure("<%s>%u is a illegal bool.",AFX_STRINGIFY(value_),(value_)),0))
#   define AfxAssertAbs(value_) ((!!((value_>=0)))||(AfxThrowError(),AfxLogAssertionFailure("<%s>%u is not absolute.",AFX_STRINGIFY(value_),(value_)),0))

//#   define AfxAssertRangeWhd(total_, offset_, range_) ((!!(((total_) && (offset_) && (range_))&&(((total_).w >= (offset_).x + (range_).w)&&((total_).h >= (offset_).y + (range_).h)&&((total_).d >= (offset_).z + (range_).d))))||(AfxThrowError(),AfxLogError("<%s>[ %i, %i, %i ] + <%s>[ %i, %i, %i ] is out of capacity <%s>[ %i, %i, %i ].",AFX_STRINGIFY(offset_),((offset_) ? (offset_).x : 0),((offset_) ? (offset_).y : 0),((offset_) ? (offset_).z : 0),AFX_STRINGIFY(range_),((range_) ? (range_).w : 0),((range_) ? (range_).h : 0),((range_) ? (range_).d : 0),AFX_STRINGIFY(total_),((total_) ? (total_).w : 0),((total_) ? (total_).h : 0),((total_) ? (total_).d : 0)),0))
#   define AfxAssertRangeWhd(total_, offset_, range_) ((!!((((total_).w >= (offset_).x + (range_).w)&&((total_).h >= (offset_).y + (range_).h)&&((total_).d >= (offset_).z + (range_).d))))||(AfxThrowError(),AfxLogError("<%s>[ %i, %i, %i ] + <%s>[ %i, %i, %i ] is out of capacity <%s>[ %i, %i, %i ].",AFX_STRINGIFY(offset_),((offset_).x),((offset_).y),((offset_).z),AFX_STRINGIFY(range_),((range_).w),((range_).h),((range_).d),AFX_STRINGIFY(total_),((total_).w),(total_).h),((total_).d),0))

// diferente de um range, um extent sempre há um valor mínimo. Algo não pode ter largura igual a zero e existir.
//#   define AfxAssertExtent(total_, range_) (((!!(range_))&&(!!((total_ >= range_))))||(AfxThrowError(),AfxLogAssertionFailure("%s(%u) is out of range [1, %u]",AFX_STRINGIFY((range_)),(range_),AFX_STRINGIFY((total_)),(total_)),0))
#   define AfxAssertExtent(total_, range_) (((((total_ >= range_))))||(AfxThrowError(),AfxLogAssertionFailure("%s(%u) is out of range [1, %u]",AFX_STRINGIFY((range_)),(range_),(total_)),0))

#   define AFX_ASSERT(cond_)  /*assert(cond_)*/           ((!!((cond_)))||(AfxThrowError(),AfxLogAssertionFailure("%s\n    %s",AFX_STRINGIFY((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssert2(a_, b_) AFX_ASSERT((a_)), AFX_ASSERT((b_))
#   define AfxAssert3(a_, b_, c_) AfxAssert2((a_),(b_)), AFX_ASSERT((c_))
#   define AfxAssert4(a_, b_, c_, d_) AfxAssert3((a_),(b_),(c_)), AFX_ASSERT((d_))
#   define AfxAssert5(a_, b_, c_, d_, e_) AfxAssert4((a_),(b_),(c_),(d_)), AFX_ASSERT((e_))
#   define AfxAssert6(a_, b_, c_, d_, e_, f_) AfxAssert5((a_),(b_),(c_),(d_),(e_)), AFX_ASSERT((f_))
#   define AfxAssert7(a_, b_, c_, d_, e_, f_, g_) AfxAssert6((a_),(b_),(c_),(d_),(e_),(f_)), AFX_ASSERT((g_))
#   define AfxAssert8(a_, b_, c_, d_, e_, f_, g_, h_) AfxAssert7((a_),(b_),(c_),(d_),(e_),(f_),(g_)), AFX_ASSERT((h_))
#   define AfxAssert9(a_, b_, c_, d_, e_, f_, g_, h_, i_) AfxAssert8((a_),(b_),(c_),(d_),(e_),(f_),(g_),(h_)), AFX_ASSERT((i_))

#   define AfxAssertAND(condA_,condB_) ((!!((condA_) && (condB_)))||(AfxThrowError(),AfxLogAssertionFailure("%s\n    %s",AFX_STRINGIFY((condA_) && (condB_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertXOR(condA_,condB_) ((!!((condA_) || (condB_)))||(AfxThrowError(),AfxLogAssertionFailure("%s\n    %s",AFX_STRINGIFY((condA_) || (condB_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertOR(condA_,condB_)  ((!!(((condA_) && !(condB_)) || ((condB_) && !(condA_))))||(AfxThrowError(),AfxLogAssertionFailure("%s\n    %s",AFX_STRINGIFY(((condA_) && !(condB_)) || ((condB_) && !(condA_))),errorMsg[AFXERR_INVALID]),0))

#   define AfxAssertSoft(cond_)         ((!!((cond_)))||(AfxThrowError(),AfxLogAdvertence("%s\n    %s",AFX_STRINGIFY((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertDiff(a_,b_)         ((!!(((void const*)(a_) != (void const*)(b_))))||(AfxThrowError(),AfxLogAssertionFailure("%s\n    %s",AFX_STRINGIFY((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertDiffSoft(a_,b_)         ((!!(((void const*)(a_) != (void const*)(b_))))||(AfxThrowError(),AfxLogAdvertence("%s\n    %s",AFX_STRINGIFY((cond_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertType(var_, fcc_)    ((!!((var_) && (*((afxFcc const*)var_) == (fcc_))))||(AfxThrowError(),AfxLogAssertionFailure("%s\n    %s",AFX_STRINGIFY((var_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxTryAssertType(var_, fcc_) ((!!(!(var_) || (*((afxFcc const*)var_) == (fcc_))))||(AfxThrowError(),AfxLogAssertionFailure("%s\n    %s",AFX_STRINGIFY((var_)),errorMsg[AFXERR_INVALID]),0))
#   define AfxAssertAlign(addr_,align_) ((!!((AFX_IS_ALIGNED((addr_),(align_)))))||(AfxThrowError(),AfxLogAssertionFailure("%s(%u) is not aligned to %u bytes",AFX_STRINGIFY((addr_)), (addr_), (align_),errorMsg[AFXERR_INVALID]),0))

#   define AfxAssignFcc(obj_,fcc_) (obj_)->fcc = (fcc_)
#   define AfxAssignTypeFcc(type_,fcc_) *((afxFcc*)type_) = (fcc_)
#else

#   define AFX_ASSERT(cond_) ((void)(err))
#   define AfxAssert2(a_, b_) ((void)(err))
#   define AfxAssert3(a_, b_, c_) ((void)(err))
#   define AfxAssert4(a_, b_, c_, d_) ((void)(err))
#   define AfxAssert5(a_, b_, c_, d_, e_) ((void)(err))
#   define AfxAssert6(a_, b_, c_, d_, e_, f_) ((void)(err))
#   define AfxAssert7(a_, b_, c_, d_, e_, f_, g_) ((void)(err))
#   define AfxAssert8(a_, b_, c_, d_, e_, f_, g_, h_) ((void)(err))
#   define AfxAssert9(a_, b_, c_, d_, e_, f_, g_, h_, i_) ((void)(err))

#   define AfxAssertAND(condA_,condB_) 
#   define AfxAssertXOR(condA_,condB_) 
#   define AfxAssertOR(condA_,condB_) 

#   define AfxAssertDiff(a_,b_) ((void)(err))
#   define AfxAssertDiffSoft(a_,b_) ((void)(err)) // when input causes just computational power loss
#   define AfxAssertType(var_, fcc_) ((void)(err))
#   define AfxTryAssertType(var_, fcc_) ((void)(err))

#   define AfxAssertBool(value_)
#   define AfxAssertAbs(value_)

#   define AFX_ASSERT_RANGE(total_, base_, range_)
#   define AfxAssertRangei(total_, base_, range_)
#   define AfxAssertRangef(total_, base_, range_)
#   define AfxAssertCapacity(capacity_, unit_)
#   define AFX_ASSERT_BOUNDS(value_, min_, max_)

#   define AfxAssertRangeWhd(total_, offset_, range_)

#   define AfxAssertAlign(addr_,align_)

// diferente de um range, um extent sempre há um valor mínimo. Algo não pode ter largura igual a zero e existir.
#   define AfxAssertExtent(total_, range_)

#   define AfxAssignFcc(obj_,fcc_)
#   define AfxAssignTypeFcc(type_,fcc_)
#endif

#if ((defined(_AFX_DEBUG) || defined(_AFX_TRACE)))
#   define AfxEntry(args, ...) AfxLogCall(AfxHere(),args,__VA_ARGS__)
#else
#   define AfxEntry(args, ...)
#endif


#ifdef _AFX_DEBUG
#   define _AFX_DBG_FCC    afxFcc    fcc
#   define _AFX_DBG_FCC16   afxUnit16  fcc
#else
#   define _AFX_DBG_FCC 
#   define _AFX_DBG_FCC16 
#endif

AFX void AfxCatchError_(afxError err, afxHere const hint);
#define AfxCatchError(err_) ((!((err_)))||(AfxCatchError_((err_), AfxHere()),0))

#endif//AFX_DEBUG_H
