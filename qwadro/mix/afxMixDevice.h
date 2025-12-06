/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *            Q W A D R O   M U L T I M E D I A   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This software is part of Advanced Multimedia Extensions & Experiments.

  //////////////////////////////////////////////////////////////////////////////
 // Advanced Sound Experience on Qwadro                                      //
//////////////////////////////////////////////////////////////////////////////
// This section is part of SIGMA A4D; Advanced Audio Extensions for Qwadro.

// Qwadro Sound Synthesis Infrastructure

#ifndef AMX_MIX_DEVICE_H
#define AMX_MIX_DEVICE_H

#include "qwadro/exec/afxDevice.h"
#include "qwadro/mix/afxMixBridge.h"
#include "qwadro/mix/afxSink.h"

#define AFX_MAX_AUDITION_HEADS 8
#define AFX_MAX_AUDIO_CHANNELS 32

AFX_DEFINE_STRUCT(amxFeatures)
{
    afxBool8 eax;
    afxBool8 hrtf;
    afxBool8 ambisonics;
};

AFX_DEFINE_STRUCT(amxLimits)
{
    afxUnit maxHeadCnt; // audition heads are listeners in Qwadro parlance. At least AFX_MAX_AUDIO_HEADS.
    afxUnit maxSrcCnt;
    afxUnit maxVoxCnt;
    afxUnit maxSampCnt; // AFX_U32_MAX
    afxUnit maxChanCnt; // AFX_U32_MAX
    afxUnit minFreq; // 4
    afxUnit maxFreq; // 192000 --- 192kHz
    afxUnit maxAmbisOrder; // 2 --- 9 channels
};

AFX_DEFINE_STRUCT(afxMixDeviceInfo)
{
    afxString32     urn;
    afxString128    name;
    afxDeviceType   type;
    afxAcceleration accel;
    afxDeviceStatus status;
    afxUnit         hwVndId;
    afxUnit         hwVndDevId;
    afxUnit32       swVndId;
    afxUnit32       swVndDrvId;
    afxUnit         drvVer;
    afxUnit         apiVer;
};

  //////////////////////////////////////////////////////////////////////////////
 // MIX DEVICE HANDLING ///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

AMX afxError AmxDescribeMixDevice
(
    afxMixDevice mdev, 
    afxMixDeviceInfo* desc
);

AMX afxBool AmxIsMixDevicePrompt
(
    afxMixDevice mdev
);

/*
    The AmxQueryMixDeviceFeatures() function is a way to query and retrieve detailed information about
    features of a specific mixing device. The amxFeatures structure holds the queried features, and after calling this function,
    you can access the capabilities of the device to make informed decisions about rendering or utilizing the device's features.
*/

AMX void AmxQueryMixDeviceFeatures
(
    // A handle for a mixing device to query for its features.
    afxMixDevice mdev, 

    // A pointer to a amxFeatures structure where the results will be stored.
    amxFeatures* features
);

/*
    The AmxQueryMixDeviceLimits() function queries the hardware limits of a specific mixing device.
    The device's capabilities are often constrained by the physical hardware, and knowing these limits
    is crucial when developing applications that need to operate efficiently within the device's capabilities.
    These limits are often critical for tasks such as optimizing memory usage, setting rendering parameters,
    or determining which rendering techniques can be supported.
*/

AMX void AmxQueryMixDeviceLimits
(
    // The device to query for its limits.
    afxMixDevice mdev, 

    // A amxLimits structure where the device's limits will be stored.
    amxLimits* limits
);

/*
    The AmxIsMixDeviceAcceptable() function determines if a given mixing device is suitable for use based on both the
    required features and hardware limits. If the device meets both the feature requirements and hardware constraints, it
    would be considered acceptable for the application. If it fails to meet either one, the device would not be considered acceptable.
*/

AMX afxBool AmxIsMixDeviceAcceptable
(
    // The device you are considering for use in your application.
    afxMixDevice mdev,

    // A amxFeatures structure containing the required features that the mixing device must support.
    amxFeatures const* features,

    // A amxLimits structure containing the hardware limits that the mixing device should meet or exceed.
    amxLimits const* limits
);

/*
    The AmxQueryMixCapabilities() function retrieves the capabilities of a synthesis device across multiple ports.
    It will return the mixing capabilities in the caps[] array. Each element in caps[] corresponds to the mixing
    capabilities of a specific port, and this information will help the application understand what features and mixing
    methods are supported by each port on the device.

    Returns the number of elements in caps[] array or the number of ports from specified base index.
*/

AMX void AmxQueryMixCapabilities
(
    // The mixing device to query.
    afxMixDevice mdev, 

    amxPortInfo* caps
);

  //////////////////////////////////////////////////////////////////////////////
 // IMPLEMENTATION DISCOVERY //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

/**
    The AmxEnumerateMixDevices() function enumerates the available mixing devices associated with an ICD.
    It allows you to discover which devices are available for rendering tasks on a system, which is important in
    scenarios where multiple devices (e.g., integrated and discrete GPUs) exist. The function provides the number
    of devices found and populates an array with the relevant devices for further processing. This is useful for
    applications that need to choose a specific device for rendering or computational tasks.

    Returns the number of mix devices inserted in the @devices.
*/

AMX afxUnit AmxEnumerateMixDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // The number ordinal for the first mix device to start the iteration from.
    afxUnit first, 

    // The maximum number of mix devices to be retrieved.
    afxUnit cnt, 

    // An array that will be populated with the mixing devices found during the enumeration.
    afxMixDevice devices[]
);

/*
    The AmxInvokeMixDevices() function provides an iterative mechanism to enumerate available mixing devices (e.g., GPUs)
    and invoke a callback function for each device. This approach is useful when you want to perform operations or checks
    on multiple devices without having to manually loop through them.

    Returns the count of found devices.
*/

AMX afxUnit AmxInvokeMixDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd, 

    // The ordinal number for the first mixing device to start the iteration from.
    afxUnit first, 

    // A pointer to user-defined data that will be passed to the callback function for each device.
    void* udd, 

    // The callback function that will be invoked for each mixing device during enumeration.
    afxBool(*f)(void*,afxMixDevice), 

    // The maximum number of mixing devices to be considered during the invocation.
    afxUnit cnt
);

/*
    The AmxEvokeMixDevices() function provides an iterative mechanism to enumerate mixing devices and
    process each device through a callback function. It also stores the enumerated devices in the devices[] array,
    allowing further operations or checks to be performed after enumeration. This function offers flexibility in
    both processing devices and retrieving them for further use, making it useful for applications that need to
    interact with multiple available GPUs or mixing devices in a system.

    Returns the count of found devices.
*/

AMX afxUnit AmxEvokeMixDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd, 

    // The ordinal number for the first mixing device to start the iteration from.
    afxUnit first, 

    // A pointer to user-defined data that will be passed to the callback function for each device.
    void* udd, 

    // The callback function that will be invoked for each mixing device during enumeration.
    afxBool(*f)(void*,afxMixDevice), 

    // The maximum number of mixing devices to be considered during the invocation.
    afxUnit cnt, 

    // An array that will be populated with the mixing devices found during the enumeration process.
    afxMixDevice devices[]
);

/*
    The AmxChooseMixDevices() function provides a way to select mixing devices that match specified feature and limit requirements.
    It returns the number of selected devices and populates an array with their device IDs. This function is helpful in scenarios
    where an application needs to filter and choose devices based on certain hardware capabilities or constraints, such as selecting
    GPUs that support specific rendering features or fall within particular performance limits.

    Returns the count of found devices.
*/

AVX afxUnit AmxChooseMixDevices
(
    // The ordinal identifier for the installable client driver (ICD).
    afxUnit icd,

    // A structure that specifies the features that the mixing devices must support.
    amxFeatures const* features,

    // A structure that defines the limits that the mixing devices should meet.
    amxLimits const* limits,

    // The operations or features that the device must supports.
    amxPortInfo const* caps,

    // The maximum number of devices to be selected and returned in the @ddevIds array.
    afxUnit maxCnt,

    // An array that will be populated with the device IDs of the selected mixing devices.
    afxUnit mdevIds[]
);

#endif//AMX_MIX_DEVICE_H
