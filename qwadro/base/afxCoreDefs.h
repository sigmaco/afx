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

  //////////////////////////////////////////////////////////////////////////////
 // ACCELERATION FRAMEWORK EXPERIMENTS FROM QWADRO                           //
//////////////////////////////////////////////////////////////////////////////

#ifndef AFX_CORE_DEFS_H
#define AFX_CORE_DEFS_H

#include "qwadro/base/afxPlatformDefs.h"

#ifndef __e2coree__
#   ifdef _DEBUG
#       define AFX DLLIMPORT extern 
#       define AFXINL DLLIMPORT EMBED
#   else
#       define AFX DLLIMPORT extern 
#       define AFXINL DLLIMPORT EMBED
#   endif
#else
#   ifdef _DEBUG
#       define _AFX DLLEXPORT
#       define AFX DLLEXPORT extern 
#       define _AFXINL _AFX /*DLLEXPORT INLINE*/
#       define AFXINL AFX /*DLLEXPORT EMBED*/
#   else
#       define _AFX DLLEXPORT
#       define AFX DLLEXPORT extern 
#       define _AFXINL DLLEXPORT INLINE
#       define AFXINL DLLEXPORT EMBED
#   endif
#endif//__e2coree__

 // used to access or embed the struct of an handled object.
#define AFX_OBJECT(handle_) struct handle_##_T
#define AFX_OBJ(handle_) struct handle_##_T

#ifndef AFX_PTR_ALIGNMENT
#   define AFX_PTR_ALIGNMENT sizeof(void*) 
#endif

#ifndef AFX_SIMD_ALIGNMENT
#   define AFX_SIMD_ALIGNMENT 16u 
#endif

#ifndef AFX_DEFAULT_ALIGNMENT
#   define AFX_DEFAULT_ALIGNMENT AFX_SIMD_ALIGNMENT 
#endif

#define AFX_ALIGN_SIZE(operand_,alignment_) (((operand_) + ((alignment_) - 1)) & ~((alignment_) - 1))

#define AFX_DEFINE_HANDLE(object) typedef struct object##_T* object
#define AFX_DEFINE_STRUCT(struct_) typedef struct struct_ struct_; struct struct_ 
#define AFX_DEFINE_STRUCT_ALIGNED(align_, struct_) typedef struct AFX_ALIGNED(align_) struct_ AFX_ALIGNED(align_) struct_; struct AFX_ALIGNED(align_) struct_ 
#define AFX_DEFINE_UNION(union_) typedef union union_ union_; union union_ 
#define AFX_DEFINE_UNION_ALIGNED(align_, union_) typedef  AFX_ALIGNED(align_)  union union_ union_ ; union union_ 

#define AFX_DECLARE_STRUCT(struct_) typedef struct struct_ struct_ 
#define AFX_DECLARE_STRUCT_ALIGN(align_, struct_) typedef struct AFX_ALIGNED(align_) struct_ AFX_ALIGNED(align_) struct_ 
#define AFX_DECLARE_UNION(union_) typedef union union_ union_ 
#define AFX_DECLARE_UNION_ALIGNED(align_, union_) typedef union AFX_ALIGNED(align_) union_ AFX_ALIGNED(align_) union_ 

#define _AFX_STR(X) #X
#define AFX_STRINGIFY(X) _AFX_STR(X)

#ifndef offsetof
#   define offsetof(s,m) ((afxSize)&(((s*)0)->m))
#endif
#ifndef ARRAY_SIZE
#   define ARRAY_SIZE(static_array_) (sizeof((static_array_)) / sizeof(((static_array_))[0]))
#endif
#ifndef countof
#   define countof ARRAY_SIZE
#endif

#define AFX_CAST(_type_, _var_) ((_type_)(_var_))

#define AFX_REBASE(p_, type_, field_) ((type_*)((void const*)(((afxByte const*)(p_)) - offsetof(type_, field_))))
#define AFX_REBASE2(type_, p_, field_) ((type_*)((void const*)(((afxByte const*)(p_)) - offsetof(type_, field_))))
#define AfxRebase(p_,s_,m_) ((s_)(void*)(((afxByte*)p_) - offsetof(s_, m_)))

#ifndef container_of
#   define container_of AFX_REBASE
#endif

#define AFX_SIMD AFX_ALIGNED(AFX_SIMD_ALIGNMENT) // make SIMD vector alignment
#define AFX_ADDR  AFX_ALIGNED(8) // make machine-dependent pointer alignment
#define AFX_VLA(type, name, align) type __declspec(align(align)) name

#define NIL (0)

typedef afxInt8     afxBool8;
typedef afxInt16    afxBool16;
typedef afxInt32    afxBool32;
typedef afxInt32    afxBool;
typedef afxInt32    afxResult;

typedef afxBool8    afxB8;
typedef afxBool16   afxB16;
typedef afxBool32   afxB32;

#ifdef TRUE
#   undef TRUE
#   undef FALSE
#endif
#define FALSE (0)
#define TRUE (!FALSE)

typedef afxUnit8    afxIndex8;
typedef afxUnit16   afxIndex16;
typedef afxUnit32   afxIndex32;
typedef afxIndex32  afxIndex;

// Bitwise NOT of 0 = all bits set (e.g. 0xFFFF or 0xFFFFFFFF).
#ifdef _MSC_VER
#define AFX_INVALID_INDEX64 AFX_U64_MAX
#define AFX_INVALID_INDEX32 AFX_U32_MAX
#define AFX_INVALID_INDEX16 AFX_U16_MAX
#define AFX_INVALID_INDEX8 AFX_U8_MAX
#define AFX_INVALID_INDEX AFX_INVALID_INDEX32
#else
#define AFX_INVALID_INDEX64 ((afxUnit64)(~((afxUnit64)0)))
#define AFX_INVALID_INDEX32 ((afxUnit32)(~((afxUnit32)0)))
#define AFX_INVALID_INDEX16 ((afxUnit16)(~((afxUnit16)0)))
#define AFX_INVALID_INDEX8 ((afxUnit8)(~((afxUnit8)0)))
#define AFX_INVALID_INDEX ((afxUnit)(~((afxUnit)0)))
#endif

AFX_STATIC_ASSERT(AFX_INVALID_INDEX64 == AFX_U64_MAX, "");
AFX_STATIC_ASSERT(AFX_INVALID_INDEX32 == AFX_U32_MAX, "");
AFX_STATIC_ASSERT(AFX_INVALID_INDEX16 == AFX_U16_MAX, "");
AFX_STATIC_ASSERT(AFX_INVALID_INDEX8 == AFX_U8_MAX, "");
AFX_STATIC_ASSERT(AFX_INVALID_INDEX == AFX_U32_MAX, "");

#define AFX_IS_INDEX_INVALID(_var_) (((typeof(_var_))~(typeof(_var_))0) == (_var_))
#define AFX_IS_INDEX_VALID(_var_) (((typeof(_var_))~(typeof(_var_))0) != (_var_))

typedef afxUnit32   afxFlags;
typedef afxUnit32   afxMask32;
typedef afxUnit64   afxMask64;
typedef afxMask32   afxMask;

// Computes the bitmask for a given bit index.
// Returns a non-zero bitmask value (the actual bit value, e.g., 8 for bit 3) for the bit.
#define AFX_BITMASK(bit_) (1 << (bit_)) // get bit offset

// Computes 1 << bit_ (i.e., a mask with only that bit set), and bitwise-ANDs it with mask_.
// Returns a non-zero value (the actual bit value, e.g., 8 for bit 3) if the bit is set; otherwise returns 0.
#define AFX_BITMASK_IF_SET(mask_,bit_) ((mask_) &  (1 << (bit_))) // Return bit position or 0 depending on if the bit is actually enabled.

// Shifts mask_ right by bit_ places and isolates the least significant bit using & 1.
// Returns 1 if the bit is set, 0 if it's not.
#define AFX_TEST_BIT_SET(mask_,bit_) (((mask_) >> (bit_)) & 1) // Return 1 or 0 if bit is enabled and not the position;

// For each used unit in a bitmask
#define AFX_ITERATE_BITMASK(iter_, usage) for (afxMask _u##__LINE__ = (usage), (iter_) = 0; _u##__LINE__; ++(iter_), _u##__LINE__ >>= 1) if (_u##__LINE__ & 1)

#define AFX_MASK64_NONE (0x0000000000000000)
#define AFX_MASK64_ALL (0xffffffffffffffff)
#define AFX_MASK32_NONE (0x00000000)
#define AFX_MASK32_ALL (0xffffffff)
#define AFX_MASK_ALL AFX_MASK32_ALL
#define AFX_MASK_NONE AFX_MASK32_NONE
#define AFX_MASK_LAST AFX_BITMASK(31)
#define AFX_MASK_FIRST AFX_BITMASK(0)
#define AFX_FORCE_ENUM_N32 (0x7fffffff)

typedef afxInt8     afxI8;
typedef afxInt16    afxI16;
typedef afxInt32    afxI32;
typedef afxInt64    afxI64;

typedef afxUnit8    afxN8;
typedef afxUnit16   afxN16;
typedef afxUnit32   afxN32;
typedef afxUnit64   afxN64;

typedef afxReal16   afxR16;
typedef afxReal32   afxR32;
typedef afxReal64   afxR64;

typedef afxChar8    afxC8;
typedef afxChar16   afxC16;
typedef afxChar32   afxC32;

// 4 bytes interpreted as 16:16 fixed.
typedef afxInt32 afxFixed;

AFXINL afxFixed AfxFixedFromFloat(afxReal f);

AFXINL afxReal AfxRealFromFixed(afxFixed f);

AFXINL afxFixed AfxFixedFromInt(afxInt i);

AFXINL afxInt AfxIntFromFixed(afxFixed f);

////////////////////////////////////////////////////////////////////////////////

//typedef afxInt /*__attribute__((unused))*/ afxError;
#define AFX_ERR_NONE /*(afxError)*/0

typedef enum afxError
// The afxError type allows for clear and consistent error handling and success status reporting in your code.
{
    // Zero indicates a successful operation.
    afxError_NONE,
    afxError_SUCCESS = afxError_NONE,

    // Positive codes
    // These represent status. Isn't necessarily a direct success or error (for example, 
    // a state code indicating progress or partial success).
    afxError_INITIALIZED,

    afxError_TIMEOUT,
    afxError_INCOMPLETE,

    // Negative error codes.
    // These represent runtime error conditions and are negative values.
    afxError_UNKNOWN = AFX_I32_MIN, // all following declarations will decrease one unit.

    afxError_FILE_NOT_FOUND, // The system cannot find the file specified.
    afxError_PATH_NOT_FOUND, // The system cannot find the path specified.
    afxError_TOO_MANY_OPEN_FILES, // The system cannot open the file.
    afxError_ACCESS_DENIED, // Access is denied.
    afxError_INVALID_HANDLE, // The handle is invalid.
    afxError_ARENA_TRASHED, // The storage control blocks were destroyed.
    afxError_NOT_ENOUGH_MEMORY, // Not enough memory resources are available to process this command.
    afxError_INVALID_BLOCK, // The storage control block address is invalid.
    afxError_BAD_ENVIRONMENT, // The environment is incorrect.
    afxError_BAD_FORMAT, // An attempt was made to load a program with an incorrect format.
    afxError_INVALID_ACCESS, // The access code is invalid.
    afxError_INVALID_DATA, // The data is invalid.
    afxError_OUT_OF_MEMORY, // Not enough storage is available to complete this operation.
    afxError_INVALID_DRIVE, // The system cannot find the drive specified.
    afxError_CURRENT_DIRECTORY, // The directory cannot be removed.
    afxError_NOT_SAME_DEVICE, // The system cannot move the file to a different disk drive.
    afxError_NO_MORE_FILES, // There are no more files.
    afxError_WRITE_PROTECT, // The media is write protected.
    afxError_BAD_UNIT, // The system cannot find the device specified.
    afxError_NOT_READY, // The device is not ready.
    afxError_BAD_COMMAND, // The device does not recognize the command.
    afxError_CRC, // Data error(cyclic redundancy check).
    afxError_BAD_LENGTH, // The program issued a command but the command length is incorrect.
    afxError_SEEK, // The drive cannot locate a specific area or track on the disk.
    afxError_NOT_DOS_DISK, // The specified disk or diskette cannot be accessed.
    afxError_SECTOR_NOT_FOUND, // The drive cannot find the sector requested.
    afxError_OUT_OF_PAPER, // The printer is out of paper.
    afxError_WRITE_FAULT, // The system cannot write to the specified device.
    afxError_READ_FAULT, // The system cannot read from the specified device.
    afxError_GEN_FAILURE, // A device attached to the system is not functioning.
    afxError_SHARING_VIOLATION, // The process cannot access the file because it is being used by another process.
    afxError_LOCK_VIOLATION, // The process cannot access the file because another process has locked a portion of the file.
    afxError_INVALID_ADDRESS, // Attempt to access invalid address.
    afxError_INVALID,
    afxError_EMPTY,
    afxError_NOT_FOUND,
    afxError_OUT_OF_RANGE,
    afxError_INSUFFICIENT,
    afxError_DENIED,
    afxError_BUSY,
    afxError_VIOLATION,
    afxError_UNSUPPORTED,
    afxError_UNINITIALIZED,
    afxError_OUT_OF_DED_MEMORY,
    afxError_INIT_FAILED,
    afxError_DEVICE_LOST,
    afxError_MEMORY_MAP_FAILED,
    afxError_LAYER_NOT_PRESENT,
    afxError_EXTENSION_NOT_PRESENT,
    afxError_FEATURE_NOT_PRESENT,
    afxError_INCOMPATIBLE_DRIVER,
    afxError_TOO_MANY_OBJECTS,
    afxError_FMT_NOT_SUPPORTED,
    afxError_FRAGMENTED_POOL,
} afxError;

typedef enum afxCriterion
{
    afxCriterion_PROHIBIT, // if available can't be procceeded.
    afxCriterion_PERMIT, // if available can be used. Don't care. Same as allow?
    afxCriterion_PREFER, // if available should be used. Same as recommend?
    afxCriterion_REQUIRE, // if not available shouldn't procceed.
}
afxCriterion;

typedef enum afxProfileFlag
{
    afxProfileFlag_ROBUSTNESS   = AFX_BITMASK(0),
    afxProfileFlag_PERFORMANCE  = AFX_BITMASK(1),
    afxProfileFlag_QUALITY      = AFX_BITMASK(2),
} afxProfileFlag;

// Object handles defined by Core Execution System

AFX_DEFINE_HANDLE(afxSystem);
AFX_DEFINE_HANDLE(afxIoSystem);
AFX_DEFINE_HANDLE(afxThread);
AFX_DEFINE_HANDLE(afxThreadPool);
AFX_DEFINE_HANDLE(afxTxu);
AFX_DEFINE_HANDLE(afxMmu);
AFX_DEFINE_HANDLE(afxMemory);
AFX_DEFINE_HANDLE(afxDatabase);
AFX_DEFINE_HANDLE(afxModule);
AFX_DEFINE_HANDLE(afxDriver);
AFX_DEFINE_HANDLE(afxUrd);
AFX_DEFINE_HANDLE(afxArchive);
AFX_DEFINE_HANDLE(afxCodec);
AFX_DEFINE_HANDLE(afxStream);
AFX_DEFINE_HANDLE(afxIoContext);
AFX_DEFINE_HANDLE(afxFile);
AFX_DEFINE_HANDLE(afxDevice);
AFX_DEFINE_HANDLE(afxDevLink);
AFX_DEFINE_HANDLE(afxService);
AFX_DEFINE_HANDLE(afxSemaphore);
AFX_DEFINE_HANDLE(avxFence);
AFX_DEFINE_HANDLE(afxIoSystem);
AFX_DEFINE_HANDLE(afxHid);
AFX_DEFINE_HANDLE(afxKeyboard);
AFX_DEFINE_HANDLE(afxMouse);
AFX_DEFINE_HANDLE(afxGamepad);
AFX_DEFINE_HANDLE(afxController);
AFX_DEFINE_HANDLE(afxResource);
AFX_DEFINE_HANDLE(afxStorage);
AFX_DEFINE_HANDLE(afxStringBase);
AFX_DEFINE_HANDLE(afxIoBridge);
AFX_DEFINE_HANDLE(afxIoQueue);
AFX_DEFINE_HANDLE(afxShell);
AFX_DEFINE_HANDLE(afxDrawSystem);
AFX_DEFINE_HANDLE(afxMixSystem);
AFX_DEFINE_HANDLE(arxSimulation);
AFX_DEFINE_HANDLE(afxWindow);

AFX_DEFINE_UNION(afxUdd)
/// Specification for user-defined data
{
    void*       p;
    void const* cp;
    afxInt      i;
    afxInt32    i32;
    afxInt16    i16;
    afxInt8     i8;
    afxUnit     u;
    afxUnit32   u32;
    afxUnit16   u16;
    afxUnit8    u8;
    afxReal     f;
    afxReal32   f32;
    afxReal64   f64;
};

#define AFX_FUNC( _type, _name ) _type _CALL _name // Declare a function
#define AFX_EXTERN_FUNC( _type, _name ) extern _type  _name // // Declare an extern function
#define AFX_EXTERN_API_FUNC( _type, _name ) extern DLLIMPORT _type  _name // Declare an extern function that is exported/imported
#define AFX_CALLBACK( _type, _name ) typedef _type (*_name ) // Declare a callback function type


typedef afxUnit aaxGameObjectId;

AFX_DEFINE_STRUCT(afxCallbackInfo) // Callback information structure used as base for all notifications handled by CallbackFunc.
{
    void*           pCookie;        ///< User data, passed to PostEvent()
    aaxGameObjectId gameObjID;      ///< Game object ID
};

AFX_CALLBACK(void, afxCallbackFunc) // Function called on completion of an event, or when a marker is reached.
(
    int in_eType,                            ///< Callback type.
    afxCallbackInfo* in_pCallbackInfo                    ///< Structure containing desired information. You can cast it to the proper sub-type, depending on the callback type.
);

#define AFX_NUM_JOB_TYPES 1

// Function that the host runtime must call to allow for jobs to execute.
// in_jobType is the type originally provided by FuncRequestJobWorker.
// in_uExecutionTimeUsec is the number of microseconds that the function should execute for before terminating.
// Note that the deadline is only checked after each individual job completes execution, so the function may run slightly
// longer than intended. The "in_uExecutionTimeUsec" should be considered a suggestion or guideline, not a strict rule.
// A value of 0 means that the function will run until there are no more jobs ready to be immediately executed.

typedef afxUnit aaxPluginId;
typedef afxUnit aaxPlayingId;
typedef afxUnit aaxUniqueId;



AFX afxUnit AfxFlagsFindLsb(afxFlags mask);
AFX afxUnit AfxFlagsFindMsb(afxFlags mask);

#define AfxTestFlags(_var_,_mask_) ((((afxFlags)(_var_)) & ((afxFlags)(_mask_))) == (afxFlags)(_mask_))
#define AfxFlagsSet(_var_,_mask_) (((afxFlags)(_var_)) = ((afxFlags)(_mask_)))
#define AfxFlagsMark(_var_,_mask_) (((afxFlags)(_var_)) |= ((afxFlags)(_mask_)))
#define AfxFlagsClear(_var_,_mask_) (((afxFlags)(_var_)) &= ~((afxFlags)(_mask_)))

#define AFX_TEST_ALIGNMENT(ptr_, alignment_) (((uintptr_t)(ptr_) % (alignment_)) == 0)

#define AFX_ABS(x_) ((0 > (x_)) ? -(x_) : (x_))
#define AFX_ABSF(x_) ((0 > (afxReal)(x_)) ? -(afxReal)(x_) : (afxReal)(x_))
#define AFX_MIN(a_,b_) (((a_) < (b_)) ? (a_) : (b_))
#define AFX_MAX(a_,b_) (((a_) > (b_)) ? (a_) : (b_))
#define AFX_MIN_OR(a_,b_) ((a_) && (a_) < (b_)) ? (a_) : ((b_) ? (b_) : (a_)) // minor non-zero
#define AFX_OR(a_,b_) ((a_) ? (a_) : (b_))
#define AFX_CLAMP(_value_, _min_, _max_) ((_value_) < (_min_) ? (_min_) : ((_value_) > (_max_) ? (_max_) : (_value_)))

#define AFX_MIN_CASTED(a_, b_, _TYPE_) AFX_MIN((_TYPE_)(a_),(_TYPE_)(b_))
#define AFX_MAX_CASTED(a_, b_, _TYPE_) AFX_MAX((_TYPE_)(a_),(_TYPE_)(b_))

#define AfxAtLeast(var_,min_) 

#define AFX_UN8_TO_R32(by_) (((afxReal)(by_)) * (1.0 / 255.0))

AFX afxChar const* errorMsg[];


AFXINL afxInt   AfxRandom(void);
AFXINL afxInt   AfxRandom2(afxInt mini, afxInt maxi);

AFXINL afxReal  AfxRandomReal(void);
AFXINL afxReal  AfxRandomReal2(afxReal mini, afxReal maxi);


AFX void AfxAccumulateCrc32(afxUnit32 *crc, void const* data, afxSize len);

#define AFX_ALIGN_DOWN_VALUE(val, align) (typeof(val))((val) & (~((typeof(val))((align) - 1))))
#define AFX_ALIGN_VALUE(val, align) AFX_ALIGN_DOWN_VALUE(((val) + ((typeof(val)) (align) - 1)), align)

#define AFX_SIZEOF_FIELD(_TYPE_, field_) (sizeof(((_TYPE_*)0)->field_))
#define	AFX_DIM(a_) (sizeof(a_) / sizeof((a_)[0]))

// assertion for test power of two condition
#define AFX_ASSERT_P2(val_) AFX_ASSERT(((uintptr_t)val_ & (val_ - 1)) == 0)

#define AFX_ADD_PTR(ptr, x) ((void*)((uintptr_t)(ptr) + (x)))
#define AFX_SUB_PTR(ptr, x) ((void *)((uintptr_t)(ptr) - (x)))
#define AFX_DIFF_PTR(ptr1, ptr2) ((uintptr_t)(ptr1) - (uintptr_t)(ptr2))
#define AFX_ALIGN_DOWN_PTR(ptr, align) ((typeof(ptr))AFX_ALIGN_DOWN_VALUE((uintptr_t)(ptr), align))
#define AFX_ALIGN_UP_PTR(ptr, align) AFX_ALIGN_DOWN_PTR((typeof(ptr))AFX_ADD_PTR(ptr, (align) - 1), align)

AFX afxUnit32 AfxPowerOfTwo32(afxUnit32 v);

#endif//AFX_CORE_DEFS_H
