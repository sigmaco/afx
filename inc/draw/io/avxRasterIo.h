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
    The AvxCopyRaster() function copies raster data into another raster, like does AvxCmdCopyRaster but in AVX.
*/

AVX afxError            AvxCopyRaster
(
    // The destination avxRaster.
    avxRaster           ras,
    // The source avxRaster.
    avxRaster           src,
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterCopy const rgns[],
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask,
    // Special flags.
    afxFlags            flags
);

/*
    The AvxPackRaster() function packs raster data into a buffer for efficient memory transfer to device-local memory.
*/

AVX afxError            AvxPackRaster
(
    // The destination buffer.
    avxRaster           ras,
    // The source avxRaster.
    avxBuffer           buf,
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const   ops[],
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

/*
    The AvxUnpackRaster() function unpacks raster data from a device-local memory buffer back into the appropriate format for use.
*/

AVX afxError            AvxUnpackRaster
(
    // The destination avxRaster.
    avxRaster           ras,
    // The source buffer.
    avxBuffer           buf,
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const   ops[], 
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

/*
    The AvxDumpRaster() function dumps raster data into a raw destination (possibly a host memory region), typically for copying purposes.
*/

AVX afxError            AvxDumpRaster
(
    // The source avxRaster.
    avxRaster           ras,
    // The destination memory.
    void*               dst,
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const   ops[],
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

/*
    The AvxUpdateRaster() function updates raster data from a source buffer or memory location.
*/

AVX afxError            AvxUpdateRaster
(
    // The destination avxRaster.
    avxRaster           ras,
    // The source memory.
    void const*         src,
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const   ops[],
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

/*
    The AvxUploadRaster() function streams raster data into the system or device from an external stream (likely for loading).
*/

AVX afxError            AvxUploadRaster
(
    // The destination avxRaster.
    avxRaster           ras,
    // The source stream.
    afxStream           in,
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to upload.
    avxRasterIo const   ops[],
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

/*
    The AvxDownloadRaster() function downloads raster data from the system or device to an external stream.
*/

AVX afxError            AvxDownloadRaster
(
    // The source avxRaster.
    avxRaster           ras,
    // The destination stream.
    afxStream           out,
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to download.
    avxRasterIo const   ops[],
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

/*
    The AvxPrintRaster() function outputs raster data to a file or URI, likely for saving or exporting the raster.
*/

AVX afxError            AvxPrintRaster
(
    // The source avxRaster.
    avxRaster           ras,
    // The number of LODs to print.
    afxUnit             lodCnt,
    // The structure specifying the region to be printed.
    avxRasterIo const*  iop,
    // The location to where the file will be stored.
    afxUri const*       uri,
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

/*
    The AvxFetchRaster() function reloads or fetches raster data from a specified URI (possibly fetching a texture or other raster asset).
*/

AVX afxError            AvxFetchRaster
(
    // The destination avxRaster.
    avxRaster           ras,
    // The location for the resource to be uploaded.
    afxUri const*       uri,
    // The number of uploads.
    afxUnit             opCnt, 
    // An array of structure specifying the regions to be updated.
    avxRasterIo const   ops[],
    // The number of LOD to be updated.
    afxUnit             lodCnt,
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

////////////////////////////////////////////////////////////////////////////////

/*
    The AvxLoadRasters() function loads multiple rasters from specified URIs into a drawing system.
*/

AVX afxError            AvxLoadRasters
(
    // The system which will host the new acquired rasters.
    afxDrawSystem       dsys, 
    // The number of rasters to be acquired and loaded.
    afxUnit             cnt, 
    // An array of structures prescribing the rasters to be acquired.
    avxRasterInfo const info[], 
    // An array of URIs locating the source files.
    afxUri const        uri[], 
    // An array of handles for each acquired raster.
    avxRaster           rasters[]
);

/*
    The AvxLoadCubemapRaster() function loads a cubemap (six textures for the six sides of a cube) into the drawing system from the given URIs.
*/

AVX avxRaster           AvxLoadCubemapRaster
(
    // The system which will host the new acquired raster.
    afxDrawSystem       dsys, 
    // The structure prescribing the new raster.
    avxRasterInfo const*info, 
    // The directory containing the files for each faces.
    afxUri const*       dir, 
    // An array of URIs locating each face in the specified directory.
    afxUri const        faces[6]
);

#endif//AVX_RASTER_IO_H
