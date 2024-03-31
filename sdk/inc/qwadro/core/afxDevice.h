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

#ifndef AFX_DEVICE_H
#define AFX_DEVICE_H

#include "qwadro/core/afxString.h"
#include "qwadro/core/afxFixedString.h"
#include "qwadro/mem/afxArray.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/core/afxManifest.h"

typedef enum afxDeviceType
{
    afxDeviceType_HID = 1,
    afxDeviceType_SIM,
    afxDeviceType_DRAW,
    afxDeviceType_SOUND,
    afxDeviceType_TELECOM
} afxDeviceType;

typedef enum afxDeviceAcceleration
{
    afxDeviceAcceleration_CPU   = AFX_BIT_OFFSET(0), // CPU with no graphic support --- ex.: Ryzen 7
    afxDeviceAcceleration_GPU   = AFX_BIT_OFFSET(1), // dedicated GPU --- ex.: AMD/NVidia PCIE adapter
    afxDeviceAcceleration_APU   = AFX_BIT_OFFSET(2), // combined CPU-GPU --- ex.: AMD A-series
    afxDeviceAcceleration_IGPU  = AFX_BIT_OFFSET(3), // integrated GPU --- ex.: Intel HD Graphics
    afxDeviceAcceleration_VGPU  = AFX_BIT_OFFSET(4), // virtual GPU --- ex.: VM/sandboxed instantiation.
    afxDeviceAcceleration_PPU   = AFX_BIT_OFFSET(5), // physics processing unit --- ex.: Ageia PhysX card
    afxDeviceAcceleration_SPU   = AFX_BIT_OFFSET(6), // synergistic processing unit
    afxDeviceAcceleration_DSP   = AFX_BIT_OFFSET(7),
    afxDeviceAcceleration_TOTAL
} afxDeviceAcceleration;

typedef enum afxDeviceState
{
    // NIL /// The device state is unknown or invalid.
    afxDeviceState_ACTIVE       = AFX_BIT_OFFSET(0), /// The device is active That is, the adapter that connects to the endpoint device is present and enabled.
    afxDeviceState_DISABLED     = AFX_BIT_OFFSET(1), /// The device is disabled.
    afxDeviceState_AUSENT       = AFX_BIT_OFFSET(2), /// The device is not present because the adapter that connects to the endpoint device has been removed from the system.
    afxDeviceState_UNPLUGGED    = AFX_BIT_OFFSET(3), /// The device is unplugged.
    
    afxDeviceState_ALL          = afxDeviceState_ACTIVE | afxDeviceState_DISABLED | afxDeviceState_AUSENT | afxDeviceState_UNPLUGGED, /// Includes devices in all states.
} afxDeviceState;

AFX_DEFINE_STRUCT(afxDeviceInfo)
{
    afxDeviceType       type;
    afxUri              uri;
    afxManifest*        manifest;
    afxResult           (*ioctl)(afxDevice dev, afxNat reqCode, va_list args);
};

AFX_DEFINE_STRUCT(afxDeviceDescription)
{
    afxNat32            devId; /// Device ID for Qwadro. This is the same as what is returned from GetDeviceId and GetDeviceIdFromName.
    afxString128        name; /// The user-friendly name for the device.
    afxDeviceState      devStateMask; /// Bitmask used to filter the device based on their state.
    afxBool             isDefaultDev; /// Identify default device. Always false when not supported.
};

#ifdef _AFX_CORE_C
#ifdef _AFX_DEVICE_C
AFX_OBJECT(afxDevice)
{
    afxChain            classes;

    afxString           domain;
    afxString           name;

    afxDeviceType       type;
  afxDeviceAcceleration accel;
    afxDeviceState      state;

    afxError            (*proc)(afxDevice,afxThread);
    afxResult           (*ioctl)(afxDevice,afxNat,va_list);

    afxBool             serving;
    afxArray            threads;
    
    afxUri32            uri;
    afxManifest         manifest;
    afxModule           mdle;
};
#endif//_AFX_DEVICE_C
#endif//_AFX_CORE_C


AFX afxModule           AfxGetDeviceModule(afxDevice dev);
AFX afxManifest const*  AfxGetDeviceManifest(afxDevice dev);
AFX afxUri const*       AfxGetDeviceManifestUri(afxDevice dev);

AFX void                AfxGetDeviceDriverInfo(afxDevice dev, afxString* name, afxString* desc, afxString* vendor, afxString* website);
AFX void                AfxGetDeviceDriverVersion(afxDevice dev, afxNat* major, afxNat* minor, afxNat* patch);

AFX afxResult           AfxCallDevice(afxDevice dev, afxNat reqCode, ...);


////////////////////////////////////////////////////////////////////////////////

AFX afxDevice           AfxOpenDevice(afxUri const* manifest, ...);

AFX afxBool             AfxFindDevice(afxDeviceType type, afxUri const* manifest, afxDevice* dev);

AFX afxError            AfxInstallClientDriver(afxDeviceType type, afxUri const* manifest, afxDevice* dev);

#endif//AFX_DEVICE_H
