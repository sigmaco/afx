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

 // QWADRO is an open-source coalition of libraries that supports rapid development of software that deals with 4D data.
 // The QWADRO frontend exposes a set of carefully selected data structures and algorithms in C.
 // The backend is highly optimized and is set up for parallelization.
 // We welcome contributions from the open-source community.

/*
    Copyright (c) 2017 SIGMA FEDERATION.

    SIGMA FEDERATION ("SIGMA") supplies this software to you in
    consideration of your agreement to the following terms, and your use,
    installation, modification or redistribution of this SIGMA software
    constitutes acceptance of these terms.  If you do not agree with these
    terms, please do not use, install, modify or redistribute this SIGMA
    software.

    In consideration of your agreement to abide by the following terms, and
    subject to these terms, SIGMA grants you a personal, non-exclusive
    license, under SIGMA's copyrights in this original SIGMA software (the
    SIGMA Software), to use, reproduce, modify and redistribute the
    SIGMA Software, with or without modifications, in source and/or binary
    forms; provided that if you redistribute the SIGMA Software, you must
    retain the copyright notice of SIGMA, this notice and the following
    text and disclaimers in all such redistributions of the SIGMA Software.
    Neither the name, trademarks, service marks nor logos of SIGMA
    FEDERATION may be used to endorse or promote products derived from the
    SIGMA Software without specific prior written permission from SIGMA.
    Except as expressly stated in this notice, no other rights or licenses
    express or implied, are granted by SIGMA herein, including but not
    limited to any patent rights that may be infringed by your derivative
    works or by other works in which the SIGMA Software may be
    incorporated. No hardware is licensed hereunder.

    THE SIGMA SOFTWARE IS BEING PROVIDED ON AN "AS IS" BASIS, WITHOUT
    WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
    INCLUDING WITHOUT LIMITATION, WARRANTIES OR CONDITIONS OF TITLE,
    NON-INFRINGEMENT, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
    ITS USE AND OPERATION EITHER ALONE OR IN COMBINATION WITH OTHER
    PRODUCTS.

    IN NO EVENT SHALL SIGMA BE LIABLE FOR ANY SPECIAL, INDIRECT,
    INCIDENTAL, EXEMPLARY, CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
    TO, LOST PROFITS; PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
    USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) OR ARISING IN ANY WAY
    OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE
    SIGMA SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT,
    TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF
    SIGMA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    afxUnit     maxRasterDim1D;
    afxUnit     maxRasterDim2D;
    afxUnit     maxRasterDim3D;
    afxUnit     maxRasterDimCube;
    afxUnit     maxRasterArrayLayers;
    afxUnit     maxTexelBufElements;
    afxUnit     maxUboRange;
    afxUnit     maxSsboRange;
    afxUnit     maxPushConstsSiz;
    afxUnit     maxMemAllocCnt;
    afxUnit     maxSamplerAllocCnt;
    afxSize     bufferRasterGranularity;
    afxSize     sparseAddrSpaceSiz;
    afxUnit     maxBoundLigaSets;
    afxUnit     maxPerStageSamplers;
    afxUnit     maxPerStageUbos;
    afxUnit     maxPerStageSsbos;
    afxUnit     maxPerStageSampledImages;
    afxUnit     maxPerStageStorageImages;
    afxUnit     maxPerStageInputAttachments;
    afxUnit     maxPerStageResources;
    afxUnit     maxLigaSetSamplers;
    afxUnit     maxLigaSetUbos;
    afxUnit     maxLigaSetUbosDynamic;
    afxUnit     maxLigaSetSsbos;
    afxUnit     maxLigaSetSsbosDynamic;
    afxUnit     maxLigaSetSampledImages;
    afxUnit     maxLigaSetStorageImages;
    afxUnit     maxLigaSetInputAttachments;
    afxUnit     maxVtxIns;
    afxUnit     maxVtxInSrcs;
    afxUnit     maxVtxInOffset;
    afxUnit     maxVtxInSrcStride;
    afxUnit     maxVtxOutCompos;
    afxUnit     maxTessGenLvl;
    afxUnit     maxTessPatchSiz;
    afxUnit     maxTessCtrlPerVtxInComps;
    afxUnit     maxTessCtrlPerVtxOutComps;
    afxUnit     maxTessCtrlPerPatchOutComps;
    afxUnit     maxTessCtrlTotalOutComps;
    afxUnit     maxTessEvalInComps;
    afxUnit     maxTessEvalOutComps;
    afxUnit     maxPrimShadInvocations;
    afxUnit     maxPrimInComps;
    afxUnit     maxPrimOutComps;
    afxUnit     maxPrimOutVertices;
    afxUnit     maxPrimTotalOutComps;
    afxUnit     maxFragInComps;
    afxUnit     maxFragOutAttachments;
    afxUnit     maxFragDualSrcAttachments;
    afxUnit     maxFragCombinedOutputResources;
    afxUnit     maxComputeSharedMemSiz;
    afxWhd      maxComputeWarpCnt;
    afxUnit     maxComputeWarpInvocations;
    afxWhd      maxComputeWarpSiz;
    afxUnit     subPixelPreciBits;
    afxUnit     subTexelPreciBits;
    afxUnit     mipmapPrecisionBits;
    afxUnit     maxDrawIndexedIdxValue;
    afxUnit     maxDrawIndirectCnt;
    afxReal     maxSamplerLodBias;
    afxReal     maxSamplerAnisotropy;
    afxUnit     maxVpCnt;
    afxUnit     maxVpDimensions[2];
    afxV2d      vpBoundsRange;
    afxUnit     vpSubPixelBits;
    afxSize     minMemMapAlign;
    afxSize     minTexelBufOffsetAlign;
    afxSize     minUboOffsetAlign;
    afxSize     minSsboOffsetAlign;
    afxInt      minTexelOffset;
    afxUnit     maxTexelOffset;
    afxInt      minTexelGatherOffset;
    afxUnit     maxTexelGatherOffset;
    afxReal     minInterpolationOffset;
    afxReal     maxInterpolationOffset;
    afxUnit     subPixelInterpolationOffsetBits;
    afxWhd      maxCanvasWhd;
    afxFlags    canvasColorSampleCnts;
    afxFlags    canvasDepthSampleCnts;
    afxFlags    canvasStencilSampleCnts;
    afxFlags    canvasNoAttachmentsSampleCnts;
    afxUnit     maxColorAttachments;
    afxFlags    sampledRasterColorSampleCnts;
    afxFlags    sampledRasterIntegerSampleCnts;
    afxFlags    sampledRasterDepthSampleCnts;
    afxFlags    sampledRasterStencilSampleCnts;
    afxFlags    storageRasterSampleCnts;
    afxUnit     maxSampleMaskWords;
    afxBool     timestampComputeAndGraphics;
    afxReal     timestampPeriod;
    afxUnit     maxClipDistances;
    afxUnit     maxCullDistances;
    afxUnit     maxCombinedClipAndCullDistances;
    afxUnit     discreteQueuePriorities;
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

AVX afxReal64       AfxFindPhysicalAspectRatio(afxUnit screenWidth, afxUnit screenHeight);

AVX afxReal64       AfxFindAllowedCameraLodError(afxReal64 errInPixels, afxInt vpHeightInPixels, afxReal64 fovY, afxReal64 distanceFromCam);

AVX afxClass const* AvxGetDrawDeviceClass(void);

// DRAW DEVICE HANDLING ////////////////////////////////////////////////////////

AVX afxBool         AfxIsDrawDevicePrompt(afxDrawDevice ddev);

/// Test the features and limits of a draw device.
AVX afxBool         AfxIsDrawDeviceAcceptable(afxDrawDevice ddev, afxDrawFeatures const* features, afxDrawLimits const* limits);
AVX void            AfxGetDrawDeviceFeatures(afxDrawDevice ddev, afxDrawFeatures* features);
AVX void            AfxGetDrawDeviceLimits(afxDrawDevice ddev, afxDrawLimits* limits);

AVX afxUnit         AfxCountDrawPorts(afxDrawDevice ddev);
AVX void            AfxGetDrawPortCapabilities(afxDrawDevice ddev, afxUnit portId, afxDrawPortCaps* caps);

// Query draw port support for presentation.
AVX afxBool         AfxDoesDrawPortSupportPresentation(afxDrawDevice ddev, afxUnit portId, afxDrawOutput dout);
AVX afxUnit         AfxEnumeratePresentationModes(afxDrawOutput dout, afxUnit first, afxUnit cnt, avxPresentMode modes[]);
AVX afxUnit         AfxEnumerateDrawOutputFormats(afxDrawOutput dout, afxUnit first, afxUnit cnt, avxFormat formats[]);

AVX afxClass const* AfxGetDrawContextClass(afxDrawDevice ddev);
AVX afxClass const* AvxGetDrawOutputClass(afxDrawDevice ddev);
AVX afxClass const* AvxGetDrawInputClass(afxDrawDevice ddev);

AVX afxUnit         AfxInvokeDrawContexts(afxDrawDevice ddev, afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawContext, void*), void *udd);
AVX afxUnit         AfxInvokeDrawOutputs(afxDrawDevice ddev, afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AVX afxUnit         AfxInvokeDrawInputs(afxDrawDevice ddev, afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawInput, void*), void *udd);

AVX afxUnit         AfxEvokeDrawContexts(afxDrawDevice ddev, afxBool(*f)(afxDrawContext, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawContext contexts[]);
AVX afxUnit         AfxEvokeDrawOutputs(afxDrawDevice ddev, afxBool(*f)(afxDrawOutput, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawOutput outputs[]);
AVX afxUnit         AfxEvokeDrawInputs(afxDrawDevice ddev, afxBool(*f)(afxDrawInput, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawInput inputs[]);

AVX afxUnit         AfxEnumerateDrawContexts(afxDrawDevice ddev, afxUnit first, afxUnit cnt, afxDrawContext contexts[]);
AVX afxUnit         AfxEnumerateDrawOutputs(afxDrawDevice ddev, afxUnit first, afxUnit cnt, afxDrawOutput outputs[]);
AVX afxUnit         AfxEnumerateDrawInputs(afxDrawDevice ddev, afxUnit first, afxUnit cnt, afxDrawInput inputs[]);

AVX void*           AfxGetDrawDeviceIdd(afxDrawDevice ddev);

// DRAW DEVICE DISCOVERY ///////////////////////////////////////////////////////

AVX afxUnit         AfxChooseDrawDevices(afxDrawFeatures const* features, afxDrawLimits const* limits, afxUnit maxCnt, afxUnit ddevId[]); // return count of found devices

/// Enumerates all draw devices by passing the handle to each device, in turn, to an application-defined callback function. 
/// AfxEvokeDrawDevices() continues until the last device is enumerated or the callback function returns FALSE.
AVX afxUnit         AfxInvokeDrawDevices(afxUnit first, afxUnit cnt, afxBool(*f)(afxDrawDevice, void*), void* udd);

AVX afxUnit         AfxEvokeDrawDevices(afxBool(*f)(afxDrawDevice, void*), void* udd, afxUnit first, afxUnit cnt, afxDrawDevice devices[]);

/// The AfxEnumerateDrawDevices() function lets you obtain the handle to each draw device in the current session.
AVX afxUnit         AfxEnumerateDrawDevices(afxUnit first, afxUnit cnt, afxDrawDevice devices[]);

AVX afxBool         AfxGetDrawDevice(afxUnit ddevId, afxDrawDevice* device);

// DRAW DEVICE CONTEXTUALIZATION ///////////////////////////////////////////////

AVX afxError        AfxConfigureDrawDevice(afxUnit ddevId, afxDrawContextConfig* cfg);
AVX afxError        AfxConfigureDrawOutput(afxUnit ddevId, afxDrawOutputConfig* cfg);
AVX afxError        AfxConfigureDrawInput(afxUnit ddevId, afxDrawInputConfig* cfg);

AVX afxError        AfxOpenDrawDevice(afxUnit ddevId, afxDrawContextConfig const* cfg, afxDrawContext* context);
AVX afxError        AfxOpenDrawOutput(afxUnit ddevId, afxDrawOutputConfig const* cfg, afxDrawOutput* output); // file, window, desktop, widget, frameserver, etc; physical or virtual VDUs.
AVX afxError        AfxOpenDrawInput(afxUnit ddevId, afxDrawInputConfig const* cfg, afxDrawInput* input);

/*
    AfxOpenDrawDevice("//./targa/", &cfg, &dctx);
    AfxOpenDrawOutput("//./targa/dwm", &cfg, &dout);
    AfxOpenDrawOutput("targa://gdi", &cfg, &dout);
*/

#endif//AVX_DRAW_SYSTEM_H
