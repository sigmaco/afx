/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *        Q W A D R O   V I D E O   G R A P H I C S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

  //////////////////////////////////////////////////////////////////////////////
 // Qwadro Unified Video Graphics Infrastructure                             //
//////////////////////////////////////////////////////////////////////////////

/*
    The Qwadro draw I/O system refers to the overall framework or set of components involved in creating, managing, and rendering graphical content. 
    This can include both hardware and software elements that work together to produce and display visual data.

    In the draw system, graphics hardware refers to the physical components that handle the rendering of images and video.

    Graphics Processing Unit (GPU) is a specialized chip designed to accelerate rendering tasks by processing complex graphical computations.
    Graphics Card is a hardware component that includes the GPU, memory, and video output interfaces to drive displays and manage graphical output.

    Software components of the draw system include the tools and libraries used to create and manage graphical content.

    APIs and libraries like OpenGL, DirectX, Vulkan, or Metal that provide the functions and tools for rendering graphics.

    The rendering pipeline is a series of stages and processes that graphical data goes through to be rendered onto a screen.

    The integration and management aspect of the draw system involves coordinating between different components and ensuring efficient rendering and display.
    Driver Software manages communication between the system and graphics hardware.

    Draw I/O mechanisms are software systems that integrate various components to render complex scenes.
*/

/*
    A draw device typically refers to a hardware or software component used to render or display graphical content. 
    It is responsible for processing and presenting visual data, whether it's through direct output to a screen or through intermediate rendering processes.

    In graphics hardware, a draw device refers to a physical component that handles rendering tasks. It processes graphical instructions and outputs them to a display.
    In software, a draw device can refer to a virtual or logical component used to handle drawing operations. It is often part of a graphics library or API.
    In VR and AR systems, draw devices are components that render virtual environments and overlay information onto the real world.

*/

// In Qwadro Video Graphics Infrastructure, a draw device installs and operates a whole draw I/O system. This is why the draw device is who provides draw I/O mechanisms.

#ifndef AVX_DRAW_SYSTEM_H
#define AVX_DRAW_SYSTEM_H

#include "qwadro/inc/exec/afxDevice.h"
#include "qwadro/inc/draw/afxDrawDefs.h"
#include "qwadro/inc/math/afxColor.h"
#include "qwadro/inc/draw/afxPixel.h"
#include "qwadro/inc/math/avxMatrix.h"
// provided classes.
#include "qwadro/inc/draw/afxDrawBridge.h"
//#include "qwadro/inc/draw/afxDrawDevice.h"
#include "qwadro/inc/draw/io/avxCmdb.h"
#include "qwadro/inc/draw/pipe/avxDrawOps.h"
#include "qwadro/inc/draw/afxDrawContext.h"
#include "qwadro/inc/draw/afxDrawOutput.h"
#include "qwadro/inc/draw/afxDrawInput.h"
#include "qwadro/inc/draw/io/afxTarga.h"
// provided classes.
#include "qwadro/inc/cad/afxCamera.h"

AFX_DEFINE_STRUCT(afxDrawLimits)
// Draw limits are various constraints or maximum values that can impact how objects are drawn or rendered. 
// These limits are typically defined by the graphics hardware, drivers, or the graphics API itself. 
// Understanding these limits is important for optimizing performance and ensuring compatibility across different hardware and platforms.
{
    afxNat      maxRasterDim1D;
    afxNat      maxRasterDim2D;
    afxNat      maxRasterDim3D;
    afxNat      maxRasterDimCube;
    afxNat      maxRasterArrayLayers;
    afxNat      maxTexelBufElements;
    afxNat      maxUniformBufRange;
    afxNat      maxStorageBufRange;
    afxNat      maxPushConstantsSiz;
    afxNat      maxMemAllocCnt;
    afxNat      maxSamplerAllocCnt;
    afxSize     bufferRasterGranularity;
    afxSize     sparseAddrSpaceSiz;
    afxNat      maxBoundDescriptorSets;
    afxNat      maxPerStageDescriptorSamplers;
    afxNat      maxPerStageDescriptorUniformBuffers;
    afxNat      maxPerStageDescriptorStorageBuffers;
    afxNat      maxPerStageDescriptorSampledImages;
    afxNat      maxPerStageDescriptorStorageImages;
    afxNat      maxPerStageDescriptorInputAttachments;
    afxNat      maxPerStageResources;
    afxNat      maxDescriptorSetSamplers;
    afxNat      maxDescriptorSetUniformBuffers;
    afxNat      maxDescriptorSetUniformBuffersDynamic;
    afxNat      maxDescriptorSetStorageBuffers;
    afxNat      maxDescriptorSetStorageBuffersDynamic;
    afxNat      maxDescriptorSetSampledImages;
    afxNat      maxDescriptorSetStorageImages;
    afxNat      maxDescriptorSetInputAttachments;
    afxNat      maxVtxInputAttributes;
    afxNat      maxVtxInputBindings;
    afxNat      maxVtxInputAttributeOffset;
    afxNat      maxVtxInputBindingStride;
    afxNat      maxVtxOutputCompos;
    afxNat      maxTessGenLvl;
    afxNat      maxTessPatchSiz;
    afxNat      maxTessCtrlPerVtxInComps;
    afxNat      maxTessCtrlPerVtxOutComps;
    afxNat      maxTessCtrlPerPatchOutComps;
    afxNat      maxTessCtrlTotalOutComps;
    afxNat      maxTessEvalInComps;
    afxNat      maxTessEvalOutComps;
    afxNat      maxPrimShaderInvocations;
    afxNat      maxPrimInComps;
    afxNat      maxPrimOutComps;
    afxNat      maxPrimOutVertices;
    afxNat      maxPrimTotalOutComps;
    afxNat      maxFragInComps;
    afxNat      maxFragOutAttachments;
    afxNat      maxFragDualSrcAttachments;
    afxNat      maxFragCombinedOutputResources;
    afxNat      maxComputeSharedMemorySiz;
    afxWhd      maxComputeWorkGroupCnt;
    afxNat      maxComputeWorkGroupInvocations;
    afxWhd      maxComputeWorkGroupSiz;
    afxNat      subPixelPrecisionBits;
    afxNat      subTexelPrecisionBits;
    afxNat      mipmapPrecisionBits;
    afxNat      maxDrawIndexedIdxValue;
    afxNat      maxDrawIndirectCnt;
    afxReal     maxSamplerLodBias;
    afxReal     maxSamplerAnisotropy;
    afxNat      maxVpCnt;
    afxNat      maxVpDimensions[2];
    afxV2d      vpBoundsRange;
    afxNat      vpSubPixelBits;
    afxSize     minMemMapAlign;
    afxSize     minTexelBufOffsetAlign;
    afxSize     minUniformBufOffsetAlign;
    afxSize     minStorageBufOffsetAlign;
    afxInt      minTexelOffset;
    afxNat      maxTexelOffset;
    afxInt      minTexelGatherOffset;
    afxNat      maxTexelGatherOffset;
    afxReal     minInterpolationOffset;
    afxReal     maxInterpolationOffset;
    afxNat      subPixelInterpolationOffsetBits;
    afxWhd      maxCanvasWhd;
    afxFlags    canvasColorSampleCnts;
    afxFlags    canvasDepthSampleCnts;
    afxFlags    canvasStencilSampleCnts;
    afxFlags    canvasNoAttachmentsSampleCnts;
    afxNat      maxColorAttachments;
    afxFlags    sampledRasterColorSampleCnts;
    afxFlags    sampledRasterIntegerSampleCnts;
    afxFlags    sampledRasterDepthSampleCnts;
    afxFlags    sampledRasterStencilSampleCnts;
    afxFlags    storageRasterSampleCnts;
    afxNat      maxSampleMaskWords;
    afxBool     timestampComputeAndGraphics;
    afxReal     timestampPeriod;
    afxNat      maxClipDistances;
    afxNat      maxCullDistances;
    afxNat      maxCombinedClipAndCullDistances;
    afxNat      discreteQueuePriorities;
    afxV2d      pointSizRange;
    afxV2d      lineWidthRange;
    afxReal     pointSizGranularity;
    afxReal     lineWidthGranularity;
    afxBool     strictLines;
    afxBool     standardSampleLocations;
    afxSize     optimalBufCopyOffsetAlign;
    afxSize     optimalBufCopyRowPitchAlign;
    afxSize     nonCoherentAtomSiz;
};


/// If you don't know what the physical aspect ratio is of the device you're using 
/// (for example, if you're using a standard PC, there is no way to determine for sure what kind of monitor is attached), 
/// you can either assume square pixels (pass the width of the screen divided by the height), or you can use Qwadro's "best guess": 

AVX afxReal64       AfxFindPhysicalAspectRatio(afxNat screenWidth, afxNat screenHeight);

AVX afxReal64       AfxFindAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam);

AVX afxClass const* AvxGetDrawDeviceClass(void);

// DRAW DEVICE HANDLING ////////////////////////////////////////////////////////

AVX afxBool         AfxIsDrawDevicePrompt(afxDrawDevice ddev);

/// Test the features and limits of a draw device.
AVX afxBool         AfxIsDrawDeviceAcceptable(afxDrawDevice ddev, afxDrawFeatures const* features, afxDrawLimits const* limits);
AVX void            AfxGetDrawDeviceFeatures(afxDrawDevice ddev, afxDrawFeatures* features);
AVX void            AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawLimits* limits);

AVX afxNat          AfxCountDrawPorts(afxDrawDevice ddev);
AVX void            AfxGetDrawPortCapabilities(afxDrawDevice ddev, afxNat portId, afxDrawPortCaps* caps);

// Query draw port support for presentation.
AVX afxBool         AfxDoesDrawPortSupportPresentation(afxDrawDevice ddev, afxNat portId, afxDrawOutput dout);
AVX afxNat          AfxEnumeratePresentationModes(afxDrawOutput dout, afxNat first, afxNat cnt, avxPresentMode modes[]);
AVX afxNat          AfxEnumerateDrawOutputFormats(afxDrawOutput dout, afxNat first, afxNat cnt, avxFormat formats[]);

AVX afxClass const* AfxGetDrawContextClass(afxDrawDevice ddev);
AVX afxClass const* AvxGetDrawOutputClass(afxDrawDevice ddev);
AVX afxClass const* AvxGetDrawInputClass(afxDrawDevice ddev);

AVX afxNat          AfxInvokeDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd);
AVX afxNat          AfxInvokeDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AVX afxNat          AfxInvokeDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);

AVX afxNat          AfxEvokeDrawContexts(afxDrawDevice ddev, afxBool(*f)(afxDrawContext, void*), void* udd, afxNat first, afxNat cnt, afxDrawContext contexts[]);
AVX afxNat          AfxEvokeDrawOutputs(afxDrawDevice ddev, afxBool(*f)(afxDrawOutput, void*), void* udd, afxNat first, afxNat cnt, afxDrawOutput outputs[]);
AVX afxNat          AfxEvokeDrawInputs(afxDrawDevice ddev, afxBool(*f)(afxDrawInput, void*), void* udd, afxNat first, afxNat cnt, afxDrawInput inputs[]);

AVX afxNat          AfxEnumerateDrawContexts(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawContext contexts[]);
AVX afxNat          AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawOutput outputs[]);
AVX afxNat          AfxEnumerateDrawInputs(afxDrawDevice ddev, afxNat first, afxNat cnt, afxDrawInput inputs[]);

AVX void*           AfxGetDrawDeviceIdd(afxDrawDevice ddev);

// DRAW DEVICE DISCOVERY ///////////////////////////////////////////////////////

AVX afxNat          AfxChooseDrawDevices(afxDrawFeatures const* features, afxDrawLimits const* limits, afxNat maxCnt, afxNat ddevId[]); // return count of found devices

/// Enumerates all draw devices by passing the handle to each device, in turn, to an application-defined callback function. 
/// AfxEvokeDrawDevices() continues until the last device is enumerated or the callback function returns FALSE.
AVX afxNat          AfxInvokeDrawDevices(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDevice, void*), void* udd);

AVX afxNat          AfxEvokeDrawDevices(afxBool(*f)(afxDrawDevice, void*), void* udd, afxNat first, afxNat cnt, afxDrawDevice devices[]);

/// The AfxEnumerateDrawDevices() function lets you obtain the handle to each draw device in the current session.
AVX afxNat          AfxEnumerateDrawDevices(afxNat first, afxNat cnt, afxDrawDevice devices[]);

AVX afxBool         AfxGetDrawDevice(afxNat ddevId, afxDrawDevice* device);

// DRAW DEVICE CONTEXTUALIZATION ///////////////////////////////////////////////

AVX afxError        AfxConfigureDrawDevice(afxNat ddevId, afxDrawContextConfig* cfg);
AVX afxError        AfxConfigureDrawOutput(afxNat ddevId, afxDrawOutputConfig* cfg);
AVX afxError        AfxConfigureDrawInput(afxNat ddevId, afxDrawInputConfig* cfg);

AVX afxError        AfxOpenDrawDevice(afxNat ddevId, afxDrawContextConfig const* cfg, afxDrawContext* context);
AVX afxError        AfxOpenDrawOutput(afxNat ddevId, afxDrawOutputConfig const* cfg, afxDrawOutput* output); // file, window, desktop, widget, frameserver, etc; physical or virtual VDUs.
AVX afxError        AfxOpenDrawInput(afxNat ddevId, afxDrawInputConfig const* cfg, afxDrawInput* input);

/*
    AfxOpenDrawDevice("//./targa/", &cfg, &dctx);
    AfxOpenDrawOutput("//./targa/dwm", &cfg, &dout);
    AfxOpenDrawOutput("targa://gdi", &cfg, &dout);
*/

#endif//AVX_DRAW_SYSTEM_H
