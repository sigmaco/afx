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

#ifndef AFX_DEVICE_H
#define AFX_DEVICE_H

#include "qwadro/inc/base/afxString.h"
#include "qwadro/inc/base/afxFixedString.h"
#include "qwadro/inc/mem/afxArray.h"
#include "qwadro/inc/io/afxUri.h"
#include "qwadro/inc/base/afxManifest.h"
#include "qwadro/inc/base/afxClass.h"
#include "qwadro/inc/exec/afxFence.h"
#include "qwadro/inc/exec/afxSemaphore.h"

typedef enum afxDeviceType
{
    afxDeviceType_HID = 1,
    afxDeviceType_SIM,
    afxDeviceType_PRAGMA,
    afxDeviceType_DRAW,
    afxDeviceType_DISPLAY,
    afxDeviceType_SOUND,
    afxDeviceType_TELECOM,
    afxDeviceType_SHELL,
} afxDeviceType;

typedef enum afxAcceleration
{
    afxAcceleration_CPU     = AFX_BIT(0), // CPU with no graphic support --- ex.: Ryzen 7
    afxAcceleration_GPU     = AFX_BIT(1), // abstract GPU
    afxAcceleration_IGPU    = afxAcceleration_GPU | AFX_BIT(2), // integrated GPU --- ex.: Intel HD Graphics
    afxAcceleration_DGPU    = afxAcceleration_GPU | AFX_BIT(3), // dedicated GPU --- ex.: AMD/NVidia PCIE adapter
    afxAcceleration_VGPU    = afxAcceleration_GPU | AFX_BIT(4), // virtual GPU --- ex.: VM/sandboxed instantiation.
    afxAcceleration_APU     = afxAcceleration_CPU | afxAcceleration_IGPU, // accelerated processing unit; combined CPU-IGPU --- ex.: AMD A-series
    afxAcceleration_PPU     = AFX_BIT(5), // physics processing unit --- ex.: Ageia PhysX card
    afxAcceleration_SPU     = AFX_BIT(6), // synergistic processing unit --- ex.: SIMD processors
    afxAcceleration_TPU     = AFX_BIT(7), // tensor processing unit
    afxAcceleration_DSP     = AFX_BIT(8), // digital signal processor --- ex.: waveform processors
    
    afxAcceleration_TOTAL   = 8
} afxAcceleration;

typedef enum afxDeviceStatus
{
    // NIL /// The device state is unknown or invalid.
    afxDeviceStatus_ACTIVE      = AFX_BIT(0), /// The device is active. That is, the adapter that connects to the endpoint device is present and enabled.
    afxDeviceStatus_DISABLED    = AFX_BIT(1), /// The device is disabled.
    afxDeviceStatus_AUSENT      = AFX_BIT(2), /// The device is not present because the adapter that connects to the endpoint device has been removed from the system.
    afxDeviceStatus_UNPLUGGED   = AFX_BIT(3), /// The device is unplugged.
    
    afxDeviceStatus_ALL         = afxDeviceStatus_ACTIVE | afxDeviceStatus_DISABLED | afxDeviceStatus_AUSENT | afxDeviceStatus_UNPLUGGED, /// Includes devices in all states.
} afxDeviceStatus;

AFX_DEFINE_STRUCT(afxDeviceInfo)
{
    afxString           urn; // device URN
    afxDeviceType       type;
    afxAcceleration     acceleration;
    afxUnit              manifestPagNo;
    afxError            (*procCb)(afxDevice,afxThread,void*);
    afxResult           (*ioctl)(afxDevice dev, afxUnit reqCode, va_list args);
};

AFX_DEFINE_STRUCT(afxDeviceDescription)
{
    afxUnit32            devId; /// Device ID for Qwadro. This is the same as what is returned from GetDeviceId and GetDeviceIdFromName.
    afxString128        name; /// The user-friendly name for the device.
    afxDeviceStatus     stateMask; /// Bitmask used to filter the device based on their state.
    afxBool             isDefault; /// Identify default device. Always false when not supported.
};

AFX afxModule           AfxGetDeviceDriver(afxDevice dev);

AFX afxDeviceType       AfxGetDeviceType(afxDevice dev);
AFX afxDeviceStatus     AfxGetDeviceStatus(afxDevice dev);
AFX afxAcceleration     AfxGetDeviceAcceleration(afxDevice dev);

AFX afxResult           AfxCallDevice(afxDevice dev, afxUnit reqCode, ...);
AFX afxError            AfxDoDeviceService(afxDevice dev);

AFX afxClass*           AfxGetFenceClass(afxContext ctx);
AFX afxClass*           AfxGetSemaphoreClass(afxContext ctx);

////////////////////////////////////////////////////////////////////////////////

AFX afxBool             AfxFindDevice(afxDeviceType order, afxUri const* urn, afxDevice* device);

////////////////////////////////////////////////////////////////////////////////

AFX afxClassConfig const _AfxSemStdImplementation;
AFX afxClassConfig const _AfxFencStdImplementation;

#endif//AFX_DEVICE_H
