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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

// afxDrawInput devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

// No QWADRO, h� um conceito de uma fila de submiss�o de trabalho para a GPU. Por�m, diferentemente do Vulkan, no QWADRO esta "fila" � separada em dois.
// A primeira parte cont�m a demanda e a segunda parte o estado da fila junto a ponte.
// Isto porque assim podemos usar a mesma demanda para dois contextos diferentes, cada qual contendo a outra parte, e logo, seu estado particular.

/*
    Draw input refers to the data or instructions provided to a system or application to create or render graphical content. 
    It involves the input that specifies what should be drawn, how it should be drawn, and where it should appear.

    In graphics programming, draw input consists of the commands or data used to generate visual content on the screen.
    In 3D graphics, vertex data includes coordinates, colors, and texture coordinates that define the geometry of objects.
    In game development, draw input refers to the data and instructions used to render game graphics and scenes.
    In vector graphics, draw input consists of commands and parameters that define shapes and lines in a vector-based drawing application.

*/

#ifndef AVX_DRAW_INPUT_H
#define AVX_DRAW_INPUT_H

#include "qwadro/inc/draw/afxDrawContext.h"

#include "qwadro/inc/draw/video/avxCamera.h"
//#include "qwadro/inc/cad/afxGeometry.h"
#include "qwadro/inc/draw/io/avxTxd.h"
#include "qwadro/inc/draw/io/afxVertexStream.h"
#include "qwadro/inc/draw/kit/afxDrawTechnique.h"
#include "qwadro/inc/draw/io/avxTargaFile.h"

typedef enum avxEventId
{
    avxEventId_FENCE,
    avxEventId_EXECUTE,
    avxEventId_PRESENT,
    avxEventId_UPLOAD,
    avxEventId_DOWNLOAD,
    avxEventId_PREFETCH,
    avxEventId_REFRESH,
    avxEventId_RECONNECT,
    avxEventId_EXTENT,    
} avxEventId;

AFX_DEFINE_STRUCT(avxEvent)
{
    avxEventId  id;
    afxBool         posted, accepted;
    void*           udd[1];
};

typedef afxBool(*afxDrawInputProc)(afxDrawInput din, avxEvent const* ev);

AFX_DEFINE_STRUCT(afxDrawInputConfig)
{
    afxUri              endpoint;
    afxDrawInputProc    proc;
    afxUnit             dragTxuBase;
    afxUnit             dragTxuCnt;

    afxUnit             cmdPoolMemStock;
    afxUnit             estimatedSubmissionCnt;
    afxUnit             minVtxBufCap;
    afxUnit             maxVtxBufCap;
    afxUnit             minIdxBufCap;
    afxUnit             maxIdxBufCap;
    void*               udd;
};

AVX afxDrawDevice   AvxGetDrawInputDevice(afxDrawInput din);
AVX afxDrawSystem   AvxGetDrawInputContext(afxDrawInput din);

AVX void*           AvxGetDrawInputUdd(afxDrawInput din);

// OPTICAL MATRIX UTILITY

AVX void            AvxGetClipSpaceInfo(afxDrawInput din, avxClipSpaceDepth* depth, afxBool* leftHanded);

AVX void            AvxComputeLookToMatrices(afxDrawInput din, afxV3d const eye, afxV3d const dir, afxM4d v, afxM4d iv);
AVX void            AvxComputeLookAtMatrices(afxDrawInput din, afxV3d const eye, afxV3d const target, afxM4d v, afxM4d iv);

AVX void            AvxComputeBasicOrthographicMatrices(afxDrawInput din, afxReal aspectRatio, afxReal scale, afxReal range, afxM4d p, afxM4d ip);
AVX void            AvxComputeOrthographicMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AvxComputeOffcenterOrthographicMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AvxComputeBoundingOrthographicMatrices(afxDrawInput din, afxBox const aabb, afxM4d p, afxM4d ip);

AVX void            AvxComputeFovMatrices(afxDrawInput din, afxReal fovY, afxReal aspectRatio, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AvxComputeFrustrumMatrices(afxDrawInput din, afxReal left, afxReal right, afxReal bottom, afxReal top, afxReal near, afxReal far, afxM4d p, afxM4d ip);
AVX void            AvxComputeBasicPerspectiveMatrices(afxDrawInput din, afxReal aspectRatio, afxReal range, afxM4d p, afxM4d ip);
AVX void            AvxComputePerspectiveMatrices(afxDrawInput din, afxV2d const extent, afxReal near, afxReal far, afxM4d p, afxM4d ip);

AVX afxError        AvxUplinkTxds(afxDrawInput din, afxUnit baseSlot, afxUnit slotCnt, afxUri const uris[]);

////////////////////////////////////////////////////////////////////////////////

AVX afxUnit AvxEnumerateDrawInputs
(
    afxDrawSystem dsys, 
    afxUnit first, 
    afxUnit cnt, 
    afxDrawInput inputs[]
);

AVX afxUnit AvxInvokeDrawInputs
(
    afxDrawSystem dsys, 
    afxUnit first, 
    afxUnit cnt, 
    afxBool(*f)(afxDrawInput, void*), 
    void *udd
);

AVX afxUnit AvxEvokeDrawInputs
(
    afxDrawSystem dsys, 
    afxBool(*f)(afxDrawInput, void*), 
    void* udd, 
    afxUnit first, 
    afxUnit cnt, 
    afxDrawInput inputs[]
);


AVX afxError AvxConfigureDrawInput(afxDrawSystem dsys, afxDrawInputConfig* cfg);

AVX afxError AvxOpenDrawInput
(
    afxDrawSystem dsys, 
    afxDrawInputConfig const* cfg, 
    afxDrawInput* input
);

#endif//AVX_DRAW_INPUT_H
