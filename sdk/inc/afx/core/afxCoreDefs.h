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

#ifndef AFX_CORE_DEFS_H
#define AFX_CORE_DEFS_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <float.h>
#include <assert.h>
#include <intrin.h>

#ifdef _MSC_VER
//#   pragma warning (disable : 4054) // 'type cast': from function pointer '' to data pointer ''
#   pragma warning (disable : 4152) // nonstandard extension, function/data pointer conversion in expression
#   pragma warning (disable : 4324) // '': structure was padded due to alignment specifier
#   pragma warning (disable : 4204) // nonstandard extension used : non-constant aggregate initializer
#   pragma warning (disable : 4201) // nonstandard extension used: nameless struct/union
#   pragma warning (disable : 4706) // assignment within conditional expression
#endif

#if 0
#if _AFX_CLASSIFIED_VBUF
#define AFX_DEFINE_HANDLE2(object) typedef struct object##_T* object;
#else
#define AFX_DEFINE_HANDLE2(object) typedef afxInstance* object;
#endif
#define AFX_OBJECT2(handle_) struct handle_##_T
#endif

// used to access or embed the struct of an handled object.
#define AFX_OBJECT(handle_) struct handle_##_T
#define AFX_INTERFACE(object, symbol) object##Interface const symbol

#define AFX_DEFINE_HANDLE(object) typedef struct object##_T* object;
#define AFX_DEFINE_STRUCT(struct_) typedef struct struct_ struct_; struct struct_
#define AFX_DEFINE_UNION(union_) typedef union union_ union_; union union_
#define AFX_DECLARE_STRUCT(struct_) typedef struct struct_ struct_
#define AFX_DECLARE_UNION(union_) typedef union union_ union_

#define AfxRebase(p_,s_,m_) ((s_)(((afxByte*)p_) - ((size_t)&(((s_)0)->m_))))
#define AfxGetSuperset(obj_,type_,member_) ((type_)(((afxByte*)obj_) - ((size_t)&(((type_)0)->member_))))

#ifdef _WIN64
#   define AFX_PLATFORM_X64
#   define AFX_PLATFORM_WIN32
#elif _WIN32
#   define AFX_PLATFORM_X86
#   define AFX_PLATFORM_WIN32
#else
#   error ""
#endif//_WIN64

#if !defined(AFX_ENDIANNESS_LITTLE) && !defined(AFX_ENDIANNESS_BIG)
#   define AFX_ENDIANNESS_LITTLE
#endif

#define _AfxStr(X) #X
#define AfxStr(X) _AfxStr(X)

#define _AFXIMPORT __declspec(dllimport)
#define _AFXEXPORT __declspec(dllexport)
#define _AFXINLINE __forceinline
#define AFXCALL __cdecl


#ifndef __e2coree__
#   ifdef _DEBUG
#       define AFX _AFXIMPORT extern 
#       define AFXINL _AFXIMPORT extern inline
#   else
#       define AFX _AFXIMPORT extern 
#       define AFXINL _AFXIMPORT extern inline
#   endif
#else
#   ifdef _DEBUG
#       define _AFX _AFXEXPORT
#       define AFX _AFXEXPORT extern 
#       define _AFXINL _AFXEXPORT inline
#       define AFXINL _AFXEXPORT extern inline
#   else
#       define _AFX _AFXEXPORT
#       define AFX _AFXEXPORT extern 
#       define _AFXINL _AFXEXPORT inline
#       define AFXINL _AFXEXPORT extern inline
#   endif
#endif//__e2coree__

#ifdef TRUE
#   undef TRUE
#   undef FALSE
#endif
#define FALSE 0
#define TRUE (!FALSE)

#define NIL 0

#define AFX_SIMD_ALIGN 16
#define afxSimd(x) __declspec(align(AFX_SIMD_ALIGN)) x

typedef int8_t      afxInt8;
typedef int16_t     afxInt16;
typedef int32_t     afxInt32;
typedef int64_t     afxInt64;

typedef uint8_t     afxNat8;
typedef uint16_t    afxNat16;
typedef uint32_t    afxNat32;
typedef uint64_t    afxNat64;

typedef afxNat16    afxReal16;
typedef float       afxReal32;
typedef double      afxReal64;

typedef afxInt8     afxBool8;
typedef afxInt32    afxBool32;

typedef char        afxChar8;
typedef afxInt16    afxChar16;
typedef afxInt32    afxChar32;

typedef afxChar8    afxChar;
typedef afxNat8     afxByte;
typedef afxInt32    afxInt; // signed
typedef afxNat32    afxNat; // unsigned
typedef afxReal32   afxSimd(afxReal);
typedef afxBool32   afxBool;
typedef afxInt32    afxResult;

typedef intptr_t    afxOffset;
typedef uintptr_t   afxAddress;
typedef size_t      afxSize;
typedef afxNat32    afxFlags;
typedef afxNat32    afxMask;

#define AFX_I8_MIN  ((afxInt8)INT8_MIN)
#define AFX_I8_MAX  ((afxInt8)INT8_MAX)
#define AFX_N8_MAX  ((afxNat8)UINT8_MAX)

#define AFX_I16_MIN ((afxInt16)INT16_MIN)
#define AFX_I16_MAX ((afxInt16)INT16_MAX)
#define AFX_N16_MAX ((afxNat16)UINT16_MAX)

#define AFX_I32_MIN ((afxInt32)INT32_MIN)
#define AFX_I32_MAX ((afxInt32)INT32_MAX)
#define AFX_N32_MAX ((afxNat32)UINT32_MAX)

#define AFX_I64_MIN ((afxInt64)INT64_MIN)
#define AFX_I64_MAX ((afxInt64)INT64_MAX)
#define AFX_N64_MAX ((afxNat64)UINT64_MAX)

#define AFX_R32_MIN ((afxReal32)FLT_MIN)
#define AFX_R32_MAX ((afxReal32)FLT_MAX)
#define AFX_R64_MIN ((afxReal64)DBL_MIN)
#define AFX_R64_MAX ((afxReal64)DBL_MAX)

#define AFX_BYTE_SIZE 8
#define AFX_CHAR_SIZE 8

AFX afxNat AfxFlagsFindLsb(afxFlags mask);
AFX afxNat AfxFlagsFindMsb(afxFlags mask);

#define AFX_FLAG_MIN ((afxNat32)1 << (afxNat32)0)
#define AFX_FLAG_MAX ((afxNat32)1 << (afxNat32)31)
#define AFX_BIT_OFFSET(bit_) ((afxNat32)1 << (afxNat32)(bit_))
//#define AFX_UNFLAG(_hex_) AfxFlagsFindLsb(_hex_)


#define AfxTestBitPosition(var_,bit_) ((var_) &  (1 << (bit_))) // Return bit position or 0 depending on if the bit is actually enabled.
#define AfxTestBitEnabled(var_,bit_) (((var_)>>(bit_)) & 1) // Return 1 or 0 if bit is enabled and not the position;

#define AfxFlagsTest(_var_,_mask_) ((((afxFlags)(_var_)) & ((afxFlags)(_mask_))) == (afxFlags)(_mask_))
#define AfxFlagsSet(_var_,_mask_) (((afxFlags)(_var_)) = ((afxFlags)(_mask_)))
#define AfxFlagsMark(_var_,_mask_) (((afxFlags)(_var_)) |= ((afxFlags)(_mask_)))
#define AfxFlagsClear(_var_,_mask_) (((afxFlags)(_var_)) &= ~((afxFlags)(_mask_)))

#define AFX_INVALID_INDEX (afxNat)(~((afxNat)0))
#define AFX_INVALID_INDEX8 (afxNat8)(~((afxNat8)0))
#define AFX_INVALID_INDEX16 (afxNat16)(~((afxNat16)0))
#define AFX_INVALID_INDEX32 (afxNat32)(~((afxNat32)0))
#define AFX_INVALID_INDEX64 (afxNat64)(~((afxNat64)0))

static_assert(AFX_INVALID_INDEX == AFX_N32_MAX, "");
static_assert(AFX_INVALID_INDEX8 == AFX_N8_MAX, "");
static_assert(AFX_INVALID_INDEX16 == AFX_N16_MAX, "");
static_assert(AFX_INVALID_INDEX32 == AFX_N32_MAX, "");

#define AFX_REBASE(link_, type_, entry_) ((type_ *)(((afxByte *)(link_)) - offsetof(type_, entry_)))

#define AfxAbs(x_) ((0 > ((afxReal)x_)) ? -((afxReal)x_) : ((afxReal)x_))

#define AfxMini(a_,b_) (((a_) < (b_)) ? (a_) : (b_))
#define AfxMaxi(a_,b_) (((a_) > (b_)) ? (a_) : (b_))
#define AfxMinor(a_,b_) ((a_) && (a_) < (b_)) ? (a_) : ((b_) ? (b_) : (a_)); // minor non-zero

#define AfxElse(a_,b_) (((a_) ? (a_) : (b_))

#define AfxAtLeast(var_,min_) 

AFXINL afxInt    AfxRandom(void);
AFXINL afxInt    AfxRandom2(afxInt mini, afxInt maxi);

AFXINL afxReal      AfxRandomReal(void);
AFXINL afxReal      AfxRandomReal2(afxReal mini, afxReal maxi);

AFX afxChar const* errorMsg[];

typedef enum afxError3
{
    // NIL, // The operation completed successfully.    
    AFXERR_UNKNOWN = 1,
    AFXERR_NIL, // Pointer or reference was nil.
    AFXERR_ZERO, // Zero was found when some value was expected for such parameter.
    AFXERR_EMPTY, // Nothing was found when function expected anything other than zero.
    AFXERR_NOT_FOUND, // Resource wasn't found at an given address.
    AFXERR_OUT_OF_RANGE, // Index or offset exceeded the totality of the seekable space.
    AFXERR_INVALID, // The given argument exists but was invalid for such parameter.
    AFXERR_NOT_ENOUGHT, // Supplied memory or data was insufficient for such operation.
    AFXERR_DENIED, // Such operation was denied on given object or resource.
    AFXERR_VIOLATION, // Potential violation on data integrity was detected.
    AFXERR_INCOMPATIBLE, // Object or resource incompatible.
    AFXERR_CORRUPTED, // Corruption in data integrity was detected for the given object.
    AFXERR_UNSUPPORTED, // Operation unsupported.
    AFXERR_UNINITIALIZED, // Object or resource wasn't initialized yet.

    FORCE_ERROR_SIZE = AFX_N64_MAX
} afxError3;

typedef afxInt afxError;
#define AFX_ERR_NONE (afxError)0;

enum
{
    AFX_SUCCESS, // The operation completed successfully.

    AFX_UNKNOWN = -1,
    AFX_FILE_NOT_FOUND = -2, // The system cannot find the file specified.
    AFX_PATH_NOT_FOUND = -3, // The system cannot find the path specified.
    AFX_TOO_MANY_OPEN_FILES = -4, // The system cannot open the file.
    AFX_ACCESS_DENIED = -5, // Access is denied.
    AFX_INVALID_HANDLE = -6, // The handle is invalid.
    AFX_ARENA_TRASHED = -7, // The storage control blocks were destroyed.
    AFX_NOT_ENOUGH_MEMORY = -8, // Not enough memory resources are available to process this command.
    AFX_INVALID_BLOCK = -9, // The storage control block address is invalid.
    AFX_BAD_ENVIRONMENT = -10, // The environment is incorrect.
    AFX_BAD_FORMAT = -11, // An attempt was made to load a program with an incorrect format.
    AFX_INVALID_ACCESS = -12, // The access code is invalid.
    AFX_INVALID_DATA = -13, // The data is invalid.
    AFX_OUTOFMEMORY = -14, // Not enough storage is available to complete this operation.
    AFX_INVALID_DRIVE = -15, // The system cannot find the drive specified.
    AFX_CURRENT_DIRECTORY = -16, // The directory cannot be removed.
    AFX_NOT_SAME_DEVICE = -17, // The system cannot move the file to a different disk drive.
    AFX_NO_MORE_FILES = -18, // There are no more files.
    AFX_WRITE_PROTECT = -19, // The media is write protected.
    AFX_BAD_UNIT = -20, // The system cannot find the device specified.
    AFX_NOT_READY = -21, // The device is not ready.
    AFX_BAD_COMMAND = -22, // The device does not recognize the command.
    AFX_CRC = -23, // Data error(cyclic redundancy check).
    AFX_BAD_LENGTH = -24, // The program issued a command but the command length is incorrect.
    AFX_SEEK = -25, // The drive cannot locate a specific area or track on the disk.
    AFX_NOT_DOS_DISK = -26, // The specified disk or diskette cannot be accessed.
    AFX_SECTOR_NOT_FOUND = -27, // The drive cannot find the sector requested.
    AFX_OUT_OF_PAPER = -28, // The printer is out of paper.
    AFX_WRITE_FAULT = -29, // The system cannot write to the specified device.
    AFX_READ_FAULT = -30, // The system cannot read from the specified device.
    AFX_GEN_FAILURE = -31, // A device attached to the system is not functioning.
    AFX_SHARING_VIOLATION = -32, // The process cannot access the file because it is being used by another process.
    AFX_LOCK_VIOLATION = -33, // The process cannot access the file because another process has locked a portion of the file.
    AFX_INVALID_ADDRESS = -487, // Attempt to access invalid address.
};

typedef enum afxCriterion
{
    AFX_PROHIBIT, // if available can't be procceeded.
    AFX_PERMIT, // if available can be used. Don't care. Same as allow?
    AFX_PREFER, // if available should be used. Same as recommend?
    AFX_REQUIRE, // if not available shouldn't procceed.
}
afxCriterion;

typedef enum afxProfileFlag
{
    afxProfileFlag_ROBUSTNESS   = AFX_BIT_OFFSET(0),
    afxProfileFlag_PERFORMANCE  = AFX_BIT_OFFSET(1),
    afxProfileFlag_QUALITY      = AFX_BIT_OFFSET(2),
} afxProfileFlag;

// Object handles defined by Core Execution System

typedef void* afxHandle;
//typedef afxHandle afxObject;

AFX_DEFINE_HANDLE(afxSystem);
//typedef afxHandle afxSystem;
AFX_DEFINE_HANDLE(afxIoSystem);
AFX_DEFINE_HANDLE(afxThread);
//typedef afxHandle afxThread;
AFX_DEFINE_HANDLE(afxContext);
//typedef afxHandle afxContext;
AFX_DEFINE_HANDLE(afxDatabase);
AFX_DEFINE_HANDLE(afxModule);
//typedef afxHandle afxModule;
AFX_DEFINE_HANDLE(afxUrd);
AFX_DEFINE_HANDLE(afxArchive);
//typedef afxHandle afxArchive;
AFX_DEFINE_HANDLE(afxFile);
//typedef afxHandle afxFile;
AFX_DEFINE_HANDLE(afxApplication);
//typedef afxHandle afxApplication;
AFX_DEFINE_HANDLE(afxIoSystem);
//AFX_DEFINE_HANDLE(afxHid);
AFX_DEFINE_HANDLE(afxKeyboard);
//typedef afxHandle afxKeyboard;
AFX_DEFINE_HANDLE(afxMouse);
//typedef afxHandle afxMouse;
AFX_DEFINE_HANDLE(afxResource);
AFX_DEFINE_HANDLE(afxWidget);


struct _afxSysD;

AFX void AfxCrc32(afxNat32 *crc, void const* data, afxSize len);


#define AFX_FUNC( _type, _name ) _type AKSOUNDENGINE_CALL _name // Declare a function
#define AFX_EXTERN_FUNC( _type, _name ) extern _type  _name // // Declare an extern function
#define AFX_EXTERN_API_FUNC( _type, _name ) extern _AFXIMPORT _type  _name // Declare an extern function that is exported/imported
#define AFX_CALLBACK( _type, _name ) typedef _type (*_name ) // Declare a callback function type


typedef afxNat afxGameObjectID;

AFX_DEFINE_STRUCT(afxCallbackInfo) // Callback information structure used as base for all notifications handled by \ref AkCallbackFunc.
{
    void *          pCookie;        ///< User data, passed to PostEvent()
    afxGameObjectID  gameObjID;      ///< Game object ID
};

AFX_CALLBACK(void, afxCallbackFunc) // Function called on completion of an event, or when a marker is reached.
(
    int in_eType,                            ///< Callback type.
    afxCallbackInfo* in_pCallbackInfo                    ///< Structure containing desired information. You can cast it to the proper sub-type, depending on the callback type.
);

#ifndef AFX_ASSERT_HOOK // Function called on assert handling, optional

AFX_CALLBACK(void, afxAssertHook)(const char * in_pszExpression, const char * in_pszFileName, int in_lineNumber);
#define AFX_ASSERT_HOOK
#endif

typedef afxNat afxJobType;
#define AFX_NUM_JOB_TYPES 1

// Function that the host runtime must call to allow for jobs to execute.
// in_jobType is the type originally provided by JobMgrSettings::FuncRequestJobWorker.
// in_uExecutionTimeUsec is the number of microseconds that the function should execute for before terminating.
// Note that the deadline is only checked after each individual job completes execution, so the function may run slightly
// longer than intended. The "in_uExecutionTimeUsec" should be considered a suggestion or guideline, not a strict rule.
// A value of 0 means that the function will run until there are no more jobs ready to be immediately executed.

AFX_CALLBACK(void, afxJobWorkerFunc)(afxJobType in_jobType, afxNat32 in_uExecutionTimeUsec);

/// Settings for the Sound Engine's internal job manager

AFX_CALLBACK(void, FuncRequestJobWorker)(
afxJobWorkerFunc in_fnJobWorker, ///< Function passed to host runtime that should be executed. Note that the function provided will exist for as long as the soundengine code is loaded, and will always be the same.
afxJobType in_jobType,           ///< The type of job worker that has been requested. This should be passed forward to in_fnJobWorker
afxNat32 in_uNumWorkers,        ///< Number of workers requested
void * in_pClientData           ///< Data provided by client in AkJobMgrSettings
);

AFX_DEFINE_STRUCT(afxJobMgrSettings)
{ // Callback function prototype definition used for handling requests from JobMgr for new workers to perform work.

FuncRequestJobWorker fnRequestJobWorker; ///< Function called by the job manager when a new worker needs to be requested. When null, all jobs will be executed on the same thread that calls RenderAudio().
afxNat32 uMaxActiveWorkers[AFX_NUM_JOB_TYPES]; ///< The maximum number of concurrent workers that will be requested. Must be >= 1 for each jobType.
afxNat32 uNumMemorySlabs; ///< Number of memory slabs to pre-allocate for job manager memory. At least one slab per worker thread should be pre-allocated. Default is 1.
afxNat32 uMemorySlabSize; ///< Size of each memory slab used for job manager memory. Must be a power of two. Default is 8K.
    void* pClientData; ///< Arbitrary data that will be passed back to the client when calling FuncRequestJobWorker
};

typedef afxNat afxPluginID;
typedef afxNat afxPlayingID;
typedef afxNat afxUniqueID;


#endif//AFX_CORE_DEFS_H