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

#ifndef AFX_CORE_DEFS_H
#define AFX_CORE_DEFS_H

#include "qwadro/inc/base/afxPlatformDefs.h"

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

#define AFX_SIMD_ALIGNMENT 16u
#define AFX_PTR_ALIGNMENT sizeof(void*)
#define AFX_DEFAULT_ALIGNMENT AFX_PTR_ALIGNMENT

#define AFX_DEFINE_HANDLE(object) typedef struct object##_T* object
#define AFX_DEFINE_STRUCT(struct_) typedef struct struct_ struct_; struct struct_ 
#define AFX_DEFINE_STRUCT_ALIGNED(align_, struct_) typedef struct AFX_ALIGN(align_) struct_ AFX_ALIGN(align_) struct_; struct AFX_ALIGN(align_) struct_ 
#define AFX_DEFINE_UNION(union_) typedef union union_ union_; union union_ 
#define AFX_DEFINE_UNION_ALIGNED(align_, union_) typedef  AFX_ALIGN(align_)  union union_ union_ ; union union_ 

#define AFX_DECLARE_STRUCT(struct_) typedef struct struct_ struct_ 
#define AFX_DECLARE_STRUCT_ALIGN(align_, struct_) typedef struct AFX_ALIGN(align_) struct_ AFX_ALIGN(align_) struct_ 
#define AFX_DECLARE_UNION(union_) typedef union union_ union_ 
#define AFX_DECLARE_UNION_ALIGNED(align_, union_) typedef union AFX_ALIGN(align_) union_ AFX_ALIGN(align_) union_ 

#define _AFX_STR(X) #X
#define AFX_STRINGIFY(X) _AFX_STR(X)

#ifndef offsetof
#   define offsetof(s,m) ((afxSize)&(((s*)0)->m))
#endif
#ifndef ARRAY_SIZE
#   define ARRAY_SIZE(static_array_) (sizeof(static_array_) / sizeof((static_array_)[0]))
#endif
#ifndef countof
#   define countof ARRAY_SIZE
#endif

#define AFX_CAST(_type_, _var_) ((_type_)(_var_))

#define AFX_REBASE(link_, type_, entry_) ((type_ *)((void const*)(((afxByte const*)(link_)) - offsetof(type_, entry_))))
#define AfxRebase(p_,s_,m_) ((s_)(void*)(((afxByte*)p_) - offsetof(s_, m_)))

#ifndef container_of
#   define container_of AFX_REBASE
#endif

#define AFX_SIMD AFX_ALIGN(AFX_SIMD_ALIGNMENT) // make SIMD vector alignment
#define AFX_ADDR  AFX_ALIGN(8) // make machine-dependent pointer alignment
#define AFX_VLA(type, name, align) type __declspec(align(align)) name

#ifdef TRUE
#   undef TRUE
#   undef FALSE
#endif
#define FALSE 0
#define TRUE (!FALSE)

#define NIL 0

#define AFX_INVALID_INDEX ((afxUnit)(~((afxUnit)0)))
#define AFX_INVALID_INDEX8 ((afxUnit8)(~((afxUnit8)0)))
#define AFX_INVALID_INDEX16 ((afxUnit16)(~((afxUnit16)0)))
#define AFX_INVALID_INDEX32 ((afxUnit32)(~((afxUnit32)0)))
#define AFX_INVALID_INDEX64 ((afxUnit64)(~((afxUnit64)0)))

AFX_STATIC_ASSERT(AFX_INVALID_INDEX == AFX_N32_MAX, "");
AFX_STATIC_ASSERT(AFX_INVALID_INDEX8 == AFX_N8_MAX, "");
AFX_STATIC_ASSERT(AFX_INVALID_INDEX16 == AFX_N16_MAX, "");
AFX_STATIC_ASSERT(AFX_INVALID_INDEX32 == AFX_N32_MAX, "");

#define AFX_BIT_OFFSET(bit_) ((afxUnit32)1 << (afxUnit32)(bit_)) // get bit offset
#define AFX_BIT AFX_BIT_OFFSET  // get bit offset

#define AFX_MASK_MIN AFX_BIT(0)
#define AFX_MASK_MAX AFX_BIT(31)
#define AFX_MASK_ALL 0xFFFFFFFF
#define AFX_MASK_NONE 0xFFFFFFFF
#define AFX_FORCE_ENUM_N32 0x7fffffff

////////////////////////////////////////////////////////////////////////////////

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
} afxError3;

typedef afxInt /*__attribute__((unused))*/ afxError;
#define AFX_ERR_NONE /*(afxError)*/0

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
    afxProfileFlag_ROBUSTNESS   = AFX_BIT(0),
    afxProfileFlag_PERFORMANCE  = AFX_BIT(1),
    afxProfileFlag_QUALITY      = AFX_BIT(2),
} afxProfileFlag;

// Object handles defined by Core Execution System

typedef void* afxObject;
#define AfxObjects(_objets_) ((afxObject*)_objets_)

AFX_DEFINE_HANDLE(afxSystem);
AFX_DEFINE_HANDLE(afxIoSystem);
AFX_DEFINE_HANDLE(afxThread);
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
AFX_DEFINE_HANDLE(afxFile);
AFX_DEFINE_HANDLE(afxDevice);
AFX_DEFINE_HANDLE(afxContext);
AFX_DEFINE_HANDLE(afxService);
AFX_DEFINE_HANDLE(afxSemaphore);
AFX_DEFINE_HANDLE(afxFence);
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
AFX_DEFINE_HANDLE(afxSoundSystem);
AFX_DEFINE_HANDLE(afxSimulation);
AFX_DEFINE_HANDLE(afxWindow);

AFX_DEFINE_UNION(afxUdd)
/// Specification for user-defined data
{
    void*       p;
    void const* cp;
    afxUnit8     i8;
    afxUnit16    i16;
    afxUnit32    i32;
    afxUnit      i;
    afxUnit8     n8;
    afxUnit16    n16;
    afxUnit32    n32;
    afxUnit      n;
    afxReal32   f32;
    afxReal64   f64;
    afxReal     f;
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

#define AfxTestBitPosition(mask_,bit_) ((mask_) &  (1 << (bit_))) // Return bit position or 0 depending on if the bit is actually enabled.
#define AfxTestBitEnabled(mask_,bit_) (((mask_)>>(bit_)) & 1) // Return 1 or 0 if bit is enabled and not the position;

#define AfxTestFlags(_var_,_mask_) ((((afxFlags)(_var_)) & ((afxFlags)(_mask_))) == (afxFlags)(_mask_))
#define AfxFlagsSet(_var_,_mask_) (((afxFlags)(_var_)) = ((afxFlags)(_mask_)))
#define AfxFlagsMark(_var_,_mask_) (((afxFlags)(_var_)) |= ((afxFlags)(_mask_)))
#define AfxFlagsClear(_var_,_mask_) (((afxFlags)(_var_)) &= ~((afxFlags)(_mask_)))

#define AFX_IS_ALIGNED(ptr_, alignment_) (((uintptr_t)(ptr_) % (alignment_)) == 0)

#define AfxIsIndexInvalid(_var_) (((afxIndex)(_value_)) == AFX_INVALID_INDEX) 
#define AfxIsAnValidIndex(_value_) (((afxIndex)(_value_)) != AFX_INVALID_INDEX) 

#define AfxAbs(x_) ((0 > (x_)) ? -(x_) : (x_))
#define AfxAbsf(x_) ((0 > (afxReal)(x_)) ? -(afxReal)(x_) : (afxReal)(x_))
#define AfxMin(a_,b_) (((a_) < (b_)) ? (a_) : (b_))
#define AfxMax(a_,b_) (((a_) > (b_)) ? (a_) : (b_))
#define AfxMinorNonZero(a_,b_) ((a_) && (a_) < (b_)) ? (a_) : ((b_) ? (b_) : (a_)) // minor non-zero
#define AfxElse(a_,b_) ((a_) ? (a_) : (b_))


#define AfxClamp(_value_, _min_, _max_) ((_value_) < (_min_) ? (_min_) : ((_value_) > (_max_) ? (_max_) : (_value_)))


#define AfxMinf(a_,b_) AfxMin((afxReal)(a_),(afxReal)(b_))
#define AfxMinf64(a_,b_) AfxMin((afxReal64)(a_),(afxReal64)(b_))
#define AfxMini(a_,b_) AfxMin((afxInt)(a_),(afxInt)(b_))
#define AfxMinu(a_,b_) AfxMin((afxUnit)(a_),(afxUnit)(b_))

#define AfxMaxf(a_,b_) AfxMax((afxReal)(a_),(afxReal)(b_))
#define AfxMaxf64(a_,b_) AfxMax((afxReal64)(a_),(afxReal64)(b_))
#define AfxMaxi(a_,b_) AfxMax((afxInt)(a_),(afxInt)(b_))
#define AfxMaxu(a_,b_) AfxMax((afxUnit)(a_),(afxUnit)(b_))

#define AfxAtLeast(var_,min_) 

#define AfxRealFromByte(by_) (((afxReal)(by_)) * (1.0 / 255.0))

AFX afxChar const* errorMsg[];


AFXINL afxInt   AfxRandom(void);
AFXINL afxInt   AfxRandom2(afxInt mini, afxInt maxi);

AFXINL afxReal  AfxRandomReal(void);
AFXINL afxReal  AfxRandomReal2(afxReal mini, afxReal maxi);


#define AfxFind(first_,last_,val_) _AfxFind(first_, last_, sizeof(val_), &val_)
AFX void const* _AfxFind(void const* first, void const* last, afxSize unitSiz, void const* val);

AFX void AfxAccumulateCrc32(afxUnit32 *crc, void const* data, afxSize len);

#endif//AFX_CORE_DEFS_H
