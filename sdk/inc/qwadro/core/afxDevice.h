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

typedef enum afxProcessorType
{
    afxProcessorType_CPU, // CPU with no graphic support --- ex.: Ryzen 7
    afxProcessorType_GPU, // dedicated GPU --- ex.: AMD/NVidia PCIE adapter
    afxProcessorType_APU, // combined CPU-GPU --- ex.: AMD A-series
    afxProcessorType_IGPU, // integrated GPU --- ex.: Intel HD Graphics
    afxProcessorType_VGPU, // virtual GPU --- ex.: VM/sandboxed instantiation.
    afxProcessorType_PPU, // physics processing unit --- ex.: Ageia PhysX card
    afxProcessorType_SPU, // synergistic processing unit
    afxProcessorType_DSP,
    afxProcessorType_TOTAL
} afxProcessorType;

typedef enum afxDeviceState
{
    // NIL /// The audio device state is unknown or invalid.
    afxDeviceState_ACTIVE       = AfxGetBitOffset(0), /// The device is active That is, the adapter that connects to the endpoint device is present and enabled.
    afxDeviceState_DISABLED     = AfxGetBitOffset(1), /// The device is disabled.
    afxDeviceState_AUSENT       = AfxGetBitOffset(2), /// The device is not present because the adapter that connects to the endpoint device has been removed from the system.
    afxDeviceState_UNPLUGGED    = AfxGetBitOffset(3), /// The device is unplugged.
    
    afxDeviceState_ALL          = afxDeviceState_ACTIVE | afxDeviceState_DISABLED | afxDeviceState_AUSENT | afxDeviceState_UNPLUGGED, /// Includes devices in all states.
} afxDeviceState;

AFX_DEFINE_STRUCT(afxDeviceDescription)
{
    afxNat32                    devId; /// Device ID for Qwadro. This is the same as what is returned from GetDeviceId and GetDeviceIdFromName.
    afxString128           name; /// The user-friendly name for the device.
    afxDeviceState              devStateMask; /// Bitmask used to filter the device based on their state.
    afxBool                     isDefaultDev; /// Identify default device. Always false when not supported.
};

#ifdef _AFX_CORE_C
#ifdef _AFX_DEVICE_C
AFX_OBJECT(afxDevice)
{
    afxChain                classes;
    afxClass                buffers;
    afxLinkage              icd;
    afxString               domain;
    afxString               name;

    afxBool                 serving;
    afxArray                threads;
    afxArray                txus;

};
#endif//_AFX_DEVICE_C
#endif//_AFX_CORE_C

#endif//AFX_DEVICE_H
