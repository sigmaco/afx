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
#include "qwadro/core/afxClass.h"
#include "qwadro/async/afxTime.h"
#include "qwadro/io/afxResource.h"
#include "qwadro/mem/afxPool.h"
#include "qwadro/core/afxMap.h"
// provided classes
#include "qwadro/mem/afxMmu.h"
#include "qwadro/core/afxIndexedString.h"
//#include "qwadro/core/afxModule.h"
#include "qwadro/core/afxIcd.h"
#include "qwadro/io/afxKeyboard.h"
#include "qwadro/io/afxMouse.h"
#include "qwadro/async/afxThread.h"
#include "qwadro/async/afxTxu.h"
#include "qwadro/core/afxDevice.h"
#include "qwadro/core/afxService.h"
#include "qwadro/io/afxStorage.h"
//#include "qwadro/io/afxStream.h"
//#include "qwadro/io/afxFile.h"
#include "qwadro/io/afxArchive.h"
#include "qwadro/sim/afxSimulation.h"
#include "qwadro/ux/afxApplication.h"
// system extension
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"

enum // opcodes used for primitive communication bethween engine and modules.
{
    AFX_OPCODE_CONTINUE,
    AFX_OPCODE_SUSPEND,
    AFX_OPCODE_BREAK, // if system returns BREAK, the engine must shutdown and exit to operating system.
    AFX_OPCODE_REBOOT, // if system returns REBOOT, the engine must shutdown then reinitialize the system.

    AFX_OPCODE_DETACH = 0, // informa a um módulo que ele será desacoplado do sistema, dando-lhe uma chance de liberar recursos.
    AFX_OPCODE_ATTACH, // informa a um módulo que ele será acoplado ao sistema, dando-lhe uma chance de realizar sua devida initialização de recursos e operações.
};

AFX_CALLBACK(void, afxProfilerPushTimerFunc)(aaxPluginId in_uPluginID, const char* in_pszZoneName);
AFX_CALLBACK(void, afxProfilerPopTimerFunc)(void);
AFX_CALLBACK(void, afxProfilerPostMarkerFunc)(aaxPluginId in_uPluginID, const char* in_pszMarkerName );

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
    afxSize                     maxMemUsage;
    afxNat                      memPageSiz;
    afxNat                      allocGranularity;
    afxNat                      genrlArenaSpace;

    afxNat                      ioBufSiz;
    afxNat                      ioArenaSpace;
    afxAssertHook               assertHook; // external assertion handling function (optional)
    afxNat                      maxHwThreads; // max amount of hardware threads managed by Qwadro.
    afxReal                     unitsToMeter; // the number of units in a meter.
    
    afxNat                      handlePageSiz;

    afxProfilerPushTimerFunc    profilerPushTimer; // external (optional) function for tracking performance of the system that is called when a timer starts. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPopTimerFunc     profilerPopTimer; // external (optional) function for tracking performance of the system that is called when a timer stops. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPostMarkerFunc   profilerPostMarker; // external (optional) function for tracking significant events in the system, to act as a marker or bookmark. (only called in Debug and Profile binaries; this is not called in Release)

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
    afxUri                      pwd; // process working dir (usually abs/path/to/qwadro/system/$(host)/)
    afxUri                      qwd; // root dir for Qwadro forked from pwd (usually qwadro/system/$(host)/../../)

    afxChain                    classes;
    afxClass                    mmus;
    afxClass                    strcats;
    afxClass                    modules;
    afxClass                    icds;
    afxClass                    hids;
    afxClass                    keyboards;
    afxClass                    mouses;
    afxClass                    threads;
    afxClass                    txus;
    afxClass                    devices;
    afxClass                    services;
    afxClass                    streams;
    afxClass                    codecs;
    afxClass                    files;
    afxClass                    archives;
    afxClass                    storages;
    afxClass                    simulations;
    afxClass                    applications;

    afxMmu                      mmu;
    afxNat                      memPageSize; // The page size and the granularity of page protection and commitment.
    afxNat                      allocGranularity;
    afxNat                      hwConcurrencyCap; // # of logical proc units (hardware threads)
    afxNat                      ptrSiz;
    afxBool                     nonLe;
    
    afxStorage                  defStops[8]; // [ ., system/$(host), system, code, sound, data, art, tmp ]

    afxModule                   e2coree;
    afxKeyboard                 stdKbd;

    afxSize                     maxMemUsage;
    
    afxAssertHook               assertHook; // external assertion handling function (optional)
    afxReal                     unitsToMeter; // the number of units in a meter.

    afxProfilerPushTimerFunc    profilerPushTimer; // external (optional) function for tracking performance of the system that is called when a timer starts. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPopTimerFunc     profilerPopTimer; // external (optional) function for tracking performance of the system that is called when a timer stops. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPostMarkerFunc   profilerPostMarker; // external (optional) function for tracking significant events in the system, to act as a marker or bookmark. (only called in Debug and Profile binaries; this is not called in Release)

    afxBool                     isInBootUp;
    afxBool                     isInShutdown;
    afxBool                     operating;
    afxBool                     interruptionRequested;

    afxArena                    ioArena;
    afxNat                      ioBufSiz;    
    struct
    {
        struct
        {
            afxFcc              type;
            afxChain            resources;
        }                       supplyChain[1];
    }                           resourcing;
    afxInt                      exitCode;

    afxDrawSystem               dsys;
    afxSoundSystem              ssys;
};
#endif//_AFX_SYSTEM_C
#endif//_AFX_CORE_C

AFX void                AfxChooseSystemConfiguration(afxSystemConfig* cfg, afxPlatformConfig* plaf);
AFX afxError            AfxBootUpSystem(afxSystemConfig const *config);
AFX void                AfxShutdownSystem(afxInt exitCode);
AFX afxSystem           AfxGetSystem(void);

AFX afxBool             AfxSystemIsExecuting(void);
AFX afxResult           AfxDoSystemExecution(afxTime timeout);
AFX void                AfxRequestSystemShutdown(afxInt exitCode);

AFX afxMmu              AfxGetIoContext(void);
AFX afxArena*           AfxGetIoArena(void);
AFX afxNat              AfxGetIoBufferSize(void);


AFX afxNat              AfxGetMemoryPageSize(void);

/// Returns the ideal number of threads that this process can run in parallel. 
/// This is done by querying the number of logical processors available to this process (if supported by this OS) or the total number of logical processors in the system. 
/// This function returns 1 if neither value could be determined.
AFX afxNat              AfxGetThreadingCapacity(void);

AFX afxMmu              AfxGetSystemContext(void);

AFX afxUri const*       AfxGetSystemDirectory(afxUri *dst);
AFX afxString const*    AfxGetSystemDirectoryString(afxBufferedString *dst);

AFX afxUri const*       AfxGetPwd(afxUri *dst);
AFX afxString const*    AfxGetPwdString(afxBufferedString *dst);

// Sends event event directly to receiver receiver, using the notify() function. Returns the value that was returned from the event handler.
AFX afxBool             AfxEmitEvent(afxHandle *receiver, afxEvent *ev);
AFX afxBool             AfxEmitEvents(afxNat cnt, afxHandle *receiver[], afxEvent ev[]);
AFX afxBool             AfxReemitEvent(afxNat cnt, afxHandle *receiver[], afxEvent *ev);

AFX afxError            AfxPostEvent(afxHandle *receiver, afxEvent *ev);
AFX afxError            AfxPostEvents(afxNat cnt, afxHandle *receiver[], afxEvent ev[]);

AFX afxClass*           AfxGetApplicationClass(void);
AFX afxClass*           AfxGetArchiveClass(void);
AFX afxClass*           AfxGetDeviceClass(void);
AFX afxClass*           AfxGetFileClass(void);
AFX afxClass*           AfxGetIcdClass(void);
AFX afxClass*           AfxGetHidClass(void);
AFX afxClass*           AfxGetKeyboardClass(void);
AFX afxClass*           AfxGetMmuClass(void);
AFX afxClass*           AfxGetModuleClass(void);
AFX afxClass*           AfxGetMouseClass(void);
AFX afxClass*           AfxGetServiceClass(void);
AFX afxClass*           AfxGetSimulationClass(void);
AFX afxClass*           AfxGetStorageClass(void);
AFX afxClass*           AfxGetStreamClass(void);
AFX afxClass*           AfxGetStringCatalogClass(void);
AFX afxClass*           AfxGetThreadClass(void);
AFX afxClass*           AfxGetTxuClass(void);

AFX afxNat              AfxCountApplications(void);
AFX afxNat              AfxCountArchives(void);
AFX afxNat              AfxCountDevices(void);
AFX afxNat              AfxCountFiles(void);
AFX afxNat              AfxCountHids(void);
AFX afxNat              AfxCountIcds(void);
AFX afxNat              AfxCountKeyboards(void);
AFX afxNat              AfxCountMmus(void);
AFX afxNat              AfxCountModules(void);
AFX afxNat              AfxCountMouses(void);
AFX afxNat              AfxCountServices(void);
AFX afxNat              AfxCountSimulations(void);
AFX afxNat              AfxCountStorages(void);
AFX afxNat              AfxCountStreams(void);
AFX afxNat              AfxCountThreads(void);
AFX afxNat              AfxCountTxus(void);

AFX afxNat              AfxEnumerateApplications(afxNat first, afxNat cnt, afxApplication app[]);
AFX afxNat              AfxEnumerateArchives(afxNat first, afxNat cnt, afxArchive arc[]);
AFX afxNat              AfxEnumerateDevices(afxNat first, afxNat cnt, afxDevice dev[]);
AFX afxNat              AfxEnumerateFiles(afxNat first, afxNat cnt, afxFile file[]);
AFX afxNat              AfxEnumerateIcds(afxNat first, afxNat cnt, afxIcd icd[]);
AFX afxNat              AfxEnumerateHids(afxNat first, afxNat cnt, afxHid hid[]);
AFX afxNat              AfxEnumerateKeyboards(afxNat first, afxNat cnt, afxKeyboard kbd[]);
AFX afxNat              AfxEnumerateMmus(afxNat first, afxNat cnt, afxMmu mmu[]);
AFX afxNat              AfxEnumerateModules(afxNat first, afxNat cnt, afxModule mdle[]);
AFX afxNat              AfxEnumerateMouses(afxNat first, afxNat cnt, afxMouse mse[]);
AFX afxNat              AfxEnumerateServices(afxNat first, afxNat cnt, afxService svc[]);
AFX afxNat              AfxEnumerateSimulations(afxNat first, afxNat cnt, afxSimulation sim[]);
AFX afxNat              AfxEnumerateStorages(afxNat first, afxNat cnt, afxStorage sto[]);
AFX afxNat              AfxEnumerateStreams(afxNat first, afxNat cnt, afxStream ios[]);
AFX afxNat              AfxEnumerateThreads(afxNat first, afxNat cnt, afxThread thr[]);
AFX afxNat              AfxEnumerateTxus(afxNat first, afxNat cnt, afxTxu txu[]);

AFX afxNat              AfxInvokeApplications(afxNat first, afxNat cnt, afxBool(*f)(afxApplication, void*), void *udd);
AFX afxNat              AfxInvokeArchives(afxNat first, afxNat cnt, afxBool(*f)(afxArchive, void*), void *udd);
AFX afxNat              AfxInvokeDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDevice, void*), void *udd);
AFX afxNat              AfxInvokeFiles(afxNat first, afxNat cnt, afxBool(*f)(afxFile, void*), void *udd);
AFX afxNat              AfxInvokeIcds(afxNat first, afxNat cnt, afxBool(*f)(afxIcd, void*), void *udd);
AFX afxNat              AfxInvokeHids(afxNat first, afxNat cnt, afxBool(*f)(afxHid, void*), void *udd);
AFX afxNat              AfxInvokeKeyboards(afxNat first, afxNat cnt, afxBool(*f)(afxKeyboard, void*), void *udd);
AFX afxNat              AfxInvokeMmus(afxNat first, afxNat cnt, afxBool(*f)(afxMmu, void*), void *udd);
AFX afxNat              AfxInvokeModules(afxNat first, afxNat cnt, afxBool(*f)(afxModule, void*), void *udd);
AFX afxNat              AfxInvokeMouses(afxNat first, afxNat cnt, afxBool(*f)(afxMouse, void*), void *udd);
AFX afxNat              AfxInvokeServices(afxNat first, afxNat cnt, afxBool(*f)(afxService, void*), void *udd);
AFX afxNat              AfxInvokeSimulations(afxNat first, afxNat cnt, afxBool(*f)(afxSimulation, void*), void *udd);
AFX afxNat              AfxInvokeStorages(afxNat first, afxNat cnt, afxBool(*f)(afxStorage, void*), void *udd);
AFX afxNat              AfxInvokeStreams(afxNat first, afxNat cnt, afxBool(*f)(afxStream, void*), void *udd);
AFX afxNat              AfxInvokeThreads(afxNat first, afxNat cnt, afxBool(*f)(afxThread, void*), void *udd);
AFX afxNat              AfxInvokeTxus(afxNat first, afxNat cnt, afxBool(*f)(afxTxu, void*), void *udd);

AFX afxModule           AfxFindModule(afxUri const *uri);
AFX afxIcd              AfxFindIcd(afxUri const *uri);

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
