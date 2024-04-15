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

#ifndef AFX_SYSTEM_H
#define AFX_SYSTEM_H

#include "qwadro/io/afxUri.h"
#include "qwadro/mem/afxArena.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/core/afxTime.h"
#include "qwadro/io/afxResource.h"
#include "qwadro/mem/afxPool.h"
// provided classes
#include "qwadro/mem/afxMmu.h"
#include "qwadro/mem/afxMappedString.h"
//#include "qwadro/core/afxModule.h"

// core
#include "qwadro/core/afxService.h"
#include "qwadro/core/afxTerminal.h"
#include "qwadro/core/afxThread.h"
#include "qwadro/core/afxVersion.h"
// env
#include "qwadro/env/afxShell.h"
// io
#include "qwadro/io/afxData.h"
#include "qwadro/io/afxSource.h"
#include "qwadro/io/afxStorage.h"
// math
#include "qwadro/math/afxOpticalMatrix.h"
#include "qwadro/math/afxTransform.h"
// mem
#include "qwadro/mem/afxArena.h"
#include "qwadro/mem/afxFifo.h"
#include "qwadro/mem/afxQueue.h"
#include "qwadro/mem/afxStack.h"
// ux
#include "qwadro/ux/afxKeyboard.h"
#include "qwadro/ux/afxMouse.h"


enum // opcodes used for primitive communication bethween engine and executables.
{
    AFX_OPCODE_CONTINUE,
    AFX_OPCODE_SUSPEND,
    AFX_OPCODE_BREAK, // if system returns BREAK, the engine must shutdown and exit to operating system.
    AFX_OPCODE_REBOOT, // if system returns REBOOT, the engine must shutdown then reinitialize the system.

    AFX_OPCODE_DETACH = 0, // informa a um módulo que ele será desacoplado do sistema, dando-lhe uma chance de liberar recursos.
    AFX_OPCODE_ATTACH, // informa a um módulo que ele será acoplado ao sistema, dando-lhe uma chance de realizar sua devida initialização de recursos e operações.
};

AFX_CALLBACK(void, afxProfilerPushTimerFn)(aaxPluginId in_uPluginID, const char* in_pszZoneName);
AFX_CALLBACK(void, afxProfilerPopTimerFn)(void);
AFX_CALLBACK(void, afxProfilerPostMarkerFn)(aaxPluginId in_uPluginID, const char* in_pszMarkerName );

typedef struct afxDrawSystemConfig afxDrawSystemConfig;
typedef struct afxSoundSystemConfig afxSoundSystemConfig;

AFX_DEFINE_STRUCT(afxPlatformConfig)
{
#ifdef AFX_PLATFORM_W32
    /*HINSTANCE*/void*          instance;
    /*HWND*/void*               hWnd;
#endif
};

AFX_DEFINE_STRUCT(afxSystemConfig)
{
    afxChar const*              root;
    /*
    afxSize                     maxMemUsage;
    afxNat                      memPageSiz;
    afxNat                      allocGranularity;
    afxNat                      genrlArenaSpace;

    afxNat                      ioBufSiz;
    afxNat                      ioArenaSpace;
    afxNat                      maxHwThreads; // max amount of hardware threads managed by Qwadro.
    afxReal                     unitsToMeter; // the number of units in a meter.
    
    afxNat                      handlePageSiz;
    */

    afxAssertHook               assertHook; // external assertion handling function (optional)

    afxProfilerPushTimerFn      profilerPushTimer; // external (optional) function for tracking performance of the system that is called when a timer starts. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPopTimerFn       profilerPopTimer; // external (optional) function for tracking performance of the system that is called when a timer stops. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPostMarkerFn     profilerPostMarker; // external (optional) function for tracking significant events in the system, to act as a marker or bookmark. (only called in Debug and Profile binaries; this is not called in Release)

    afxSize const*              attrs[2];
    
    struct
    {
        afxChar const*          key;
        void const*             value;
    };

    afxPlatformConfig const*    platform;
    afxDrawSystemConfig const*  draw;
    afxSoundSystemConfig const* sound;
};

#ifdef _AFX_CORE_C
#ifdef _AFX_SYSTEM_C
AFX_OBJECT(afxSystem)
{
    afxNat                  ptrSiz;
    afxBool                 bigEndian;
    afxNat                  memPageSize; // The page size and the granularity of page protection and commitment.
    afxNat                  allocGranularity;
    afxNat                  hwConcurrencyCap; // # of logical proc units (hardware threads)
    afxReal                 unitsPerMeter; // the number of units in a meter.
    afxNat                  ioBufSiz;
    afxNat                  primeTid;
    afxThread               primeThr;

    afxUri2048              pwd; // process working dir (usually abs/path/to/qwadro/system/$(host)/)
    afxUri2048              qwd; // root dir for Qwadro forked from pwd (usually qwadro/system/$(host)/../../)

    afxChain                mgrChn;
    afxManager              mmuMgr;
    afxManager              strbMgr;
    afxManager              exeMgr;
    afxManager              hidMgr;
    afxManager              thrMgr;
    afxManager              devMgr;
    afxManager              svcMgr;
    afxManager              iosMgr;
    afxManager              cdcMgr;
    afxManager              fileMgr;
    afxManager              archMgr;
    afxManager              fsysMgr;
    afxManager              envMgr;

    afxStorage              defStops[9]; // [ ., system/$(host)d, system/$(host), system, code, sound, data, art, tmp ]

    afxModule               e2coree;
    afxModule               e2sound;
    afxResult               (*ssysctl)(afxSystem,afxInt,...);
    afxModule               e2draw;
    afxResult               (*dsysctl)(afxSystem,afxInt,...);
    afxModule               e2sim;
    afxModule               e2ux;
    afxKeyboard             stdKbd;
    afxMouse                stdMse;


    afxSize                 maxMemUsage;
    
    afxAssertHook           assertHook; // external assertion handling function (optional)

    afxProfilerPushTimerFn  profilerPushTimer; // external (optional) function for tracking performance of the system that is called when a timer starts. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPopTimerFn   profilerPopTimer; // external (optional) function for tracking performance of the system that is called when a timer stops. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPostMarkerFn profilerPostMarker; // external (optional) function for tracking significant events in the system, to act as a marker or bookmark. (only called in Debug and Profile binaries; this is not called in Release)

    afxBool                 isInBootUp;
    afxBool                 isInShutdown;
    afxBool                 operating;
    afxBool                 interruptionRequested;

    struct
    {
        struct
        {
            afxFcc          type;
            afxChain        resources;
        }                   supplyChain[1];
    }                       resourcing;
    afxInt                  exitCode;

    afxShell          env;
};
#endif//_AFX_SYSTEM_C
#endif//_AFX_CORE_C

AFX void                AfxChooseSystemConfiguration(afxSystemConfig* cfg, afxPlatformConfig* plaf);
AFX afxError            AfxDoSystemBootUp(afxSystemConfig const *config);
AFX void                AfxDoSystemShutdown(afxInt exitCode);
AFX afxBool             AfxGetSystem(afxSystem* system);

AFX afxBool             AfxSystemIsExecuting(void);
AFX afxResult           AfxDoSystemExecution(afxTime timeout);
AFX void                AfxRequestSystemShutdown(afxInt exitCode);

AFX afxNat              AfxGetIoBufferSize(void);

AFX afxBool             AfxGetShell(afxShell* shell);

AFX afxNat              AfxGetMemoryPageSize(void);

/// Returns the ideal number of threads that this process can run in parallel. 
/// This is done by querying the number of logical processors available to this process (if supported by this OS) or the total number of logical processors in the system. 
/// This function returns 1 if neither value could be determined.
AFX afxNat              AfxGetThreadingCapacity(void);

AFX afxUri const*       AfxGetSystemDirectory(afxUri *dst);
AFX afxString const*    AfxGetSystemDirectoryString(afxRestring *dst);

AFX afxUri const*       AfxGetPwd(afxUri *dst);
AFX afxString const*    AfxGetPwdString(afxRestring *dst);

// Sends event event directly to receiver receiver, using the notify() function. Returns the value that was returned from the event handler.
// Adds the event event, with the object receiver as the receiver of the event, to an event queue and returns immediately.

AFX afxBool             AfxEmitEvent(afxObject receiver, afxEvent* ev);


AFX afxManager*         AfxGetArchiveClass(void);
AFX afxManager*         AfxGetDeviceClass(void);
AFX afxManager*         AfxGetEnvironmentClass(void);
AFX afxManager*         AfxGetFileClass(void);
AFX afxManager*         AfxGetStorageClass(void);
AFX afxManager*         AfxGetHidManager(void);
AFX afxManager*         AfxGetMmuClass(void);
AFX afxManager*         AfxGetModuleClass(void);
AFX afxManager*         AfxGetServiceClass(void);
AFX afxManager*         AfxGetStreamClass(void);
AFX afxManager*         AfxGetStringBaseClass(void);
AFX afxManager*         AfxGetThreadClass(void);

AFX afxNat              AfxCountArchives(void);
AFX afxNat              AfxCountDevices(afxDeviceType type);
AFX afxNat              AfxCountFiles(void);
AFX afxNat              AfxCountHids(void);
AFX afxNat              AfxCountMmus(void);
AFX afxNat              AfxCountModules(void);
AFX afxNat              AfxCountServices(void);
AFX afxNat              AfxCountStorages(void);
AFX afxNat              AfxCountStreams(void);
AFX afxNat              AfxCountThreads(void);

AFX afxNat              AfxEnumerateArchives(afxNat first, afxNat cnt, afxArchive archives[]);
AFX afxNat              AfxEnumerateDevices(afxDeviceType type, afxNat first, afxNat cnt, afxDevice devices[]);
AFX afxNat              AfxEnumerateFiles(afxNat first, afxNat cnt, afxFile files[]);
AFX afxNat              AfxEnumerateStorages(afxNat first, afxNat cnt, afxStorage systems[]);
AFX afxNat              AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hids[]);
AFX afxNat              AfxEnumerateMmus(afxNat first, afxNat cnt, afxMmu mmus[]);
AFX afxNat              AfxEnumerateModules(afxNat first, afxNat cnt, afxModule executables[]);
AFX afxNat              AfxEnumerateServices(afxNat first, afxNat cnt, afxService services[]);
AFX afxNat              AfxEnumerateStreams(afxNat first, afxNat cnt, afxStream streams[]);
AFX afxNat              AfxEnumerateThreads(afxNat first, afxNat cnt, afxThread threads[]);

AFX afxNat              AfxInvokeArchives(afxNat first, afxNat cnt, afxBool(*f)(afxArchive, void*), void *udd);
AFX afxNat              AfxInvokeDevices(afxDeviceType type, afxNat first, afxNat cnt, afxBool(*f)(afxDevice, void*), void *udd);
AFX afxNat              AfxInvokeFiles(afxNat first, afxNat cnt, afxBool(*f)(afxFile, void*), void *udd);
AFX afxNat              AfxInvokeStorages(afxNat first, afxNat cnt, afxBool(*f)(afxStorage, void*), void *udd);
AFX afxNat              AfxInvokeHids(afxNat first, afxNat cnt, afxBool(*f)(afxHid, void*), void *udd);
AFX afxNat              AfxInvokeMmus(afxNat first, afxNat cnt, afxBool(*f)(afxMmu, void*), void *udd);
AFX afxNat              AfxInvokeModules(afxNat first, afxNat cnt, afxBool(*f)(afxModule, void*), void *udd);
AFX afxNat              AfxInvokeServices(afxNat first, afxNat cnt, afxBool(*f)(afxService, void*), void *udd);
AFX afxNat              AfxInvokeStreams(afxNat first, afxNat cnt, afxBool(*f)(afxStream, void*), void *udd);
AFX afxNat              AfxInvokeThreads(afxNat first, afxNat cnt, afxBool(*f)(afxThread, void*), void *udd);

#if 0
////////////////////////////////////////////////////////////////////////////////
// QWADRO PLUGIN SYSTEM
////////////////////////////////////////////////////////////////////////////////

typedef void afxPlugin;
typedef void afxPluginParam;

/// Registered plugin creation function prototype.

AFX_CALLBACK(afxPlugin*, afxCreatePluginCallback)(afxAllocator * in_pAllocator);

// Registered plugin parameter node creation function prototype.

AFX_CALLBACK(afxPluginParam*, afxCreateParamCallback)(afxAllocator * in_pAllocator);

// Registered plugin device enumeration function prototype, used for providing lists of devices by plug-ins.

AFX_CALLBACK(afxResult, afxGetDeviceListCallback)
(
    afxNat32    *io_maxNumDevices,                 ///< In: The length of the out_deviceDescriptions array, or zero is out_deviceDescriptions is null. Out: If out_deviceDescriptions is not-null, this should be set to the number of entries in out_deviceDescriptions that was populated (and should be less-than-or-equal to the initial value). If out_deviceDescriptions is null, this should be set to the maximum number of devices that may be returned by this callback.
    afxDeviceDescription* out_deviceDescriptions ///< The output array of device descriptions. If this is not-null, there will be a number of entries equal to the input value of io_maxNumDevices.
);

/// Plug-in type.

typedef enum afxPluginType
{
    afxPluginTypeNone = 0,   ///< Unknown/invalid plug-in type.
    afxPluginTypeCodec = 1,  ///< Compressor/decompressor plug-in (allows support for custom audio file types).
    afxPluginTypeSource = 2, ///< Source plug-in: creates sound by synthesis method (no input, just output).
    afxPluginTypeEffect = 3, ///< Effect plug-in: applies processing to audio data.
    afxPluginTypeMixer = 6,  ///< Mixer plug-in: mix voices at the bus level.
    afxPluginTypeSink = 7,   ///< Sink plug-in: implement custom sound engine end point.
    afxPluginTypeGlobalExtension = 8,    ///< Global Extension plug-in: (e.g. Spatial Audio, Interactive Music)
    afxPluginTypeMetadata = 9,   ///< Metadata plug-in: applies object-based processing to audio data
    afxPluginTypeMask = 0xf  ///< Plug-in type mask is 4 bits.
} afxPluginType;

/// Registers a plug-in with the sound engine and sets the callback functions to create the plug-in and its parameter node.  
/// This function is deprecated. Registration is now automatic if you link plug-ins statically. If plug-ins are dynamic libraries (such as DLLs or SOs), use \c RegisterPluginDLL.
/// Codecs and plug-ins must be registered before loading banks that use them.
/// Loading a bank referencing an unregistered plug-in or codec will result in a load bank success, but the plug-ins will not be used. More specifically, playing a sound that uses an unregistered effect plug-in will result in audio playback without applying the said effect. If an unregistered source plug-in is used by an event's audio objects, posting the event will fail.

AFX_EXTERN_API_FUNC(afxError, RegisterPlugin)
(
    afxPluginType in_eType, // Plug-in type (for example, source or effect)
    afxNat32 in_ulCompanyID, // Company identifier (as declared in the plug-in description XML file)
    afxNat32 in_ulPluginID, // Plug-in identifier (as declared in the plug-in description XML file)
    afxCreatePluginCallback in_pCreateFunc, // Pointer to the plug-in's creation function
    afxCreateParamCallback in_pCreateParamFunc, // Pointer to the plug-in's parameter node creation function
    afxGetDeviceListCallback in_pGetDeviceList // Optional pointer to the plug-in's device enumeration function. Specify for a sink plug-in to support \ref AK::SoundEngine::GetDeviceList.
);



#endif

#endif//AFX_SYSTEM_H
