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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AFX_DEVICE_H
#define AFX_DEVICE_H

#include "qwadro/core/afxString.h"
#include "qwadro/core/afxFixedString.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/core/afxManifest.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/core/afxFence.h"
#include "qwadro/core/afxSemaphore.h"

typedef enum afxDeviceType
{
    afxDeviceType_HID = 1,
    afxDeviceType_SIM,
    afxDeviceType_DRAW,
    afxDeviceType_SOUND,
    afxDeviceType_TELECOM
} afxDeviceType;

typedef enum afxDeviceAccelor
{
    afxDeviceAccelor_CPU        = AFX_BIT_OFFSET(0), // CPU with no graphic support --- ex.: Ryzen 7
    afxDeviceAccelor_GPU        = AFX_BIT_OFFSET(1), // dedicated GPU --- ex.: AMD/NVidia PCIE adapter
    afxDeviceAccelor_APU        = AFX_BIT_OFFSET(2), // combined CPU-GPU --- ex.: AMD A-series
    afxDeviceAccelor_IGPU       = AFX_BIT_OFFSET(3), // integrated GPU --- ex.: Intel HD Graphics
    afxDeviceAccelor_VGPU       = AFX_BIT_OFFSET(4), // virtual GPU --- ex.: VM/sandboxed instantiation.
    afxDeviceAccelor_PPU        = AFX_BIT_OFFSET(5), // physics processing unit --- ex.: Ageia PhysX card
    afxDeviceAccelor_SPU        = AFX_BIT_OFFSET(6), // synergistic processing unit
    afxDeviceAccelor_DSP        = AFX_BIT_OFFSET(7),
    
    afxDeviceAccelor_TOTAL = 8
} afxDeviceAccelor;

typedef enum afxDeviceStatus
{
    // NIL /// The device state is unknown or invalid.
    afxDeviceStatus_ACTIVE      = AFX_BIT_OFFSET(0), /// The device is active That is, the adapter that connects to the endpoint device is present and enabled.
    afxDeviceStatus_DISABLED    = AFX_BIT_OFFSET(1), /// The device is disabled.
    afxDeviceStatus_AUSENT      = AFX_BIT_OFFSET(2), /// The device is not present because the adapter that connects to the endpoint device has been removed from the system.
    afxDeviceStatus_UNPLUGGED   = AFX_BIT_OFFSET(3), /// The device is unplugged.
    
    afxDeviceStatus_ALL         = afxDeviceStatus_ACTIVE | afxDeviceStatus_DISABLED | afxDeviceStatus_AUSENT | afxDeviceStatus_UNPLUGGED, /// Includes devices in all states.
} afxDeviceStatus;

AFX_DEFINE_STRUCT(afxDeviceInfo)
{
    afxDeviceType       type;
    afxUri              manifestUri;
    afxString           manifestPag;
    afxNat              manifestPagNo;
    //afxManifest*        manifest;
    afxResult           (*ioctl)(afxDevice dev, afxNat reqCode, va_list args);
};

AFX_DEFINE_STRUCT(afxDeviceDescription)
{
    afxNat32            devId; /// Device ID for Qwadro. This is the same as what is returned from GetDeviceId and GetDeviceIdFromName.
    afxString128        name; /// The user-friendly name for the device.
    afxDeviceStatus      stateMask; /// Bitmask used to filter the device based on their state.
    afxBool             isDefault; /// Identify default device. Always false when not supported.
};

#ifdef _AFX_CORE_C
#ifdef _AFX_CONTEXT_C
AFX_OBJECT(afxContext)
{
    afxChain            classes;
    afxManager          semaphores;
    afxManager          fences;
    afxError            (*waitFenc)(afxBool, afxNat64, afxNat, const afxFence[]);
    afxError            (*resetFenc)(afxNat, afxFence const[]);
};
#endif//_AFX_CONTEXT_C
#ifdef _AFX_DEVICE_C
AFX_OBJECT(afxDevice)
{
    afxDeviceType       type;
    afxDeviceAccelor    accelor;
    afxDeviceStatus     status;
    afxBool             serving;

    afxChain            classes;

    afxUri32            manifestUri;
    afxNat              manifestPagNo;
    afxManifest         manifest;
    afxModule           mdle;
    afxError            (*procCb)(afxDevice,afxThread);
    afxResult           (*ioctlCb)(afxDevice,afxNat,va_list);
};
#endif//_AFX_DEVICE_C
#endif//_AFX_CORE_C

AFX afxDeviceType       AfxGetDeviceType(afxDevice dev);
AFX afxDeviceStatus     AfxGetDeviceStatus(afxDevice dev);
AFX afxDeviceAccelor    AfxGetDeviceAcceleration(afxDevice dev);

AFX afxModule           AfxGetDeviceModule(afxDevice dev);
AFX afxManifest const*  AfxGetDeviceManifest(afxDevice dev, afxNat* devPagIdx);
AFX afxUri const*       AfxGetDeviceManifestUri(afxDevice dev);

AFX void                AfxGetDeviceDriverInfo(afxDevice dev, afxString* name, afxString* desc, afxString* vendor, afxString* website);
AFX void                AfxGetDeviceDriverVersion(afxDevice dev, afxNat* major, afxNat* minor, afxNat* patch);

AFX afxResult           AfxCallDevice(afxDevice dev, afxNat reqCode, ...);
AFX afxError            AfxDoDeviceService(afxDevice dev);

AFX afxManager*         AfxGetFenceClass(afxContext ctx);
AFX afxManager*         AfxGetSemaphoreClass(afxContext ctx);

////////////////////////////////////////////////////////////////////////////////

AFX afxDevice           AfxOpenDevice(afxUri const* manifest, ...);

AFX afxBool             AfxFindDevice(afxDeviceType type, afxUri const* manifest, afxDevice* device);

AFX afxError            AfxInstallClientDriver(afxDeviceType type, afxUri const* manifest, afxDevice* device);

////////////////////////////////////////////////////////////////////////////////

AFX afxClassConfig const _AfxSemStdImplementation;
AFX afxClassConfig const _AfxFencStdImplementation;

#endif//AFX_DEVICE_H
