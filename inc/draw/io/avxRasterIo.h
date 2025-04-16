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
 // RASTER INPUT/OUTPUT MECHANISM                                            //
//////////////////////////////////////////////////////////////////////////////

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

/*
    A list of function declarations related to raster operations in a graphics or rendering API, 
    related to the management and manipulation of raster data (such as textures or images) in a drawing system.

    These functions are used to interact with raster data in a graphics system, likely within a GPU or rendering context. 
    They include operations for testing usage and flags, querying properties, manipulating textures (or images), 
    and transferring data between the system and the GPU or storage.
*/

#ifndef AVX_RASTER_IO_H
#define AVX_RASTER_IO_H

#include "qwadro/inc/draw/io/avxRaster.h"

/*
    The AvxPackRaster() function packs raster data into a buffer for efficient memory transfer to device-local memory.
*/

AVX afxError            AvxPackRaster
(
    avxRaster           ras, 
    avxBuffer           buf, 
    afxUnit             opCnt, 
    avxRasterIo const   ops[], 
    afxMask             exuMask
);

/*
    The AvxUnpackRaster() function unpacks raster data from a device-local memory buffer back into the appropriate format for use.
*/

AVX afxError            AvxUnpackRaster
(
    avxRaster           ras, 
    avxBuffer           buf, 
    afxUnit             opCnt, 
    avxRasterIo const   ops[], 
    afxMask             exuMask
);

/*
    The AvxDumpRaster() function dumps raster data into a raw destination (possibly a host memory region), typically for copying purposes.
*/

AVX afxError            AvxDumpRaster
(
    avxRaster           ras, 
    void*               dst, 
    afxUnit             opCnt, 
    avxRasterIo const   ops[], 
    afxMask             exuMask
);

/*
    The AvxUpdateRaster() function updates raster data from a source buffer or memory location.
*/

AVX afxError            AvxUpdateRaster
(
    avxRaster           ras, 
    void const*         src, 
    afxUnit             opCnt, 
    avxRasterIo const   ops[], 
    afxMask             exuMask
);

/*
    The AvxUploadRaster() function streams raster data into the system or device from an external stream (likely for loading).
*/

AVX afxError            AvxUploadRaster
(
    avxRaster           ras, 
    afxStream           in, 
    afxUnit             opCnt, 
    avxRasterIo const   ops[], 
    afxMask             exuMask
);

/*
    The AvxDownloadRaster() function downloads raster data from the system or device to an external stream.
*/

AVX afxError            AvxDownloadRaster
(
    avxRaster           ras, 
    afxStream           out, 
    afxUnit             opCnt, 
    avxRasterIo const   ops[], 
    afxMask             exuMask
);

/*
    The AvxPrintRaster() function outputs raster data to a file or URI, likely for saving or exporting the raster.
*/

AVX afxError            AvxPrintRaster
(
    avxRaster           ras,
    afxUnit             lodCnt,
    avxRasterIo const*  iop,
    afxUri const*       uri,
    afxMask             exuMask
);

/*
    The AvxFetchRaster() function reloads or fetches raster data from a specified URI (possibly fetching a texture or other raster asset).
*/

AVX afxError            AvxFetchRaster
(
    avxRaster           ras,
    afxUri const*       uri,
    afxUnit             opCnt, 
    avxRasterIo const   ops[],
    afxUnit             lodCnt,
    afxMask             exuMask
);

////////////////////////////////////////////////////////////////////////////////

/*
    The AvxLoadRasters() function loads multiple rasters from specified URIs into a drawing system.
*/

AVX afxError            AvxLoadRasters
(
    afxDrawSystem       dsys, 
    afxUnit             cnt, 
    avxRasterInfo const info[], 
    afxUri const        uri[], 
    avxRaster           rasters[]
);

/*
    The AvxLoadCubemapRaster() function loads a cubemap (six textures for the six sides of a cube) into the drawing system from the given URIs.
*/

AVX avxRaster           AvxLoadCubemapRaster
(
    afxDrawSystem       dsys, 
    avxRasterInfo const*info, 
    afxUri const*       dir, 
    afxUri const        faces[6]
);

#endif//AVX_RASTER_IO_H
