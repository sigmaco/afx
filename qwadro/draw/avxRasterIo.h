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

// This code is part of AVX and SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AVX_RASTER_IO_H
#define AVX_RASTER_IO_H

#include "qwadro/draw/avxRaster.h"
#include "qwadro/draw/avxFormat.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/draw/avxSampler.h"
#include "qwadro/base/afxWarp.h"
//#include "qwadro/draw/avxTransference.h"
#include "qwadro/draw/avxViewport.h"

/*
    The AvxLoadSegmentedRaster() function loads a cubemap (six textures for the six sides of a cube) 
    into the drawing system from the given URIs.
*/

AVX afxError            AvxLoadSegmentedRaster
(
    // The system which will host the new acquired raster.
    afxDrawSystem       dsys, 
    // The structure prescribing the new raster.
    avxRasterInfo const*info, 
    // The directory containing the files for each faces.
    afxUri const*       dir, 
    // An array of URIs locating each face in the specified directory.
    afxUri const        faces[],
    // An placeholder for the acquired avxRaster.
    avxRaster*          raster
);

////////////////////////////////////////////////////////////////////////////////

AFX_DEFINE_STRUCT(avxRasteredCopy)
// Especificação de operação de cópia de avxRaster.
{
    // The destination avxRaster.
    avxRaster           dst;
    afxUnit             dstLodIdx;
    // select the initial x, y, and z offsets in texels of the sub-regions of the source and destination avxRaster data.
    avxOrigin           dstOrigin;
    // The source avxRaster.
    avxRaster           src;
    avxRasterRegion     rgn;
};

AFX_DEFINE_STRUCT(avxRasterCopy)
// Especificação de operação de cópia de avxRaster.
{
    avxRasterRegion src;
    afxUnit         dstLodIdx;
    // select the initial x, y, and z offsets in texels of the sub-regions of the source and destination avxRaster data.
    avxOrigin       dstOrigin;
};

/*
    The AvxCopyRaster() function copies raster data into another raster, like does AvxCmdCopyRaster but in AVX.
*/

AVX afxError            AvxCopyRaster
(
    // The destination avxRaster.
    avxRaster           ras,
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterCopy const rgns[],
    // The source avxRaster.
    avxRaster           src,
    // Special flags.
    afxFlags            flags,
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

AFX_DEFINE_STRUCT(avxRasterIo)
// Especificação de operação de transferência arbitrária de avxRaster.
{
    avxRasterRegion rgn;
    // The offset in bytes from the start of the buffer object where the avxRaster data is copied from or to.
    afxSize         offset;
    // (aka bytes per row) specify in texels a subregion of an image, and control the addressing calculations. 
    // If either of these values is zero, that aspect of the image is considered to be tightly packed according to the raster's extent.
    afxUnit         rowStride;
    // (aka rows per layer) specify in texels a subregion of an image, and control the addressing calculations. 
    // If either of these values is zero, that aspect of the image is considered to be tightly packed according to the raster's extent.
    afxUnit         rowsPerImg;
    //  rowsPerImage * rowStride is the stride, in bytes, between the beginning of each texel image of data and the subsequent texel image.
    afxCodec        codec;
    afxUnit         decSiz;
    afxUnit         encSiz;
};

/*
    The AvxPackRaster() function packs raster data into a buffer for efficient memory transfer to device-local memory.
*/

AVX afxError            AvxPackRaster
(
    // The destination buffer.
    avxRaster           ras,
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const   ops[],
    // The source avxRaster.
    avxBuffer           buf,
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
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const   ops[],
    // The source buffer.
    avxBuffer           buf, 
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
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const   ops[],
    // The destination memory.
    void*               dst,
    // Special flags.
    afxFlags            flags,
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
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to copy.
    avxRasterIo const   ops[],
    // The source memory.
    void const*         src,
    // Special flags.
    afxFlags            flags,
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
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to upload.
    avxRasterIo const   ops[],
    // The source stream.
    afxStream           in,
    // Special flags.
    afxFlags            flags,
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
    // The number of regions to copy.
    afxUnit             opCnt,
    // An array of structures specifying the regions to download.
    avxRasterIo const   ops[],
    // The destination stream.
    afxStream           out,
    // Special flags.
    afxFlags            flags,
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
    // The structure specifying the region to be printed.
    avxRasterIo const*  iop,
    // The number of LODs to print.
    afxUnit             lodCnt,
    // The location to where the file will be stored.
    afxUri const*       uri,
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

/*
    The AvxReloadRaster() function reloads or fetches raster data from a specified URI (possibly fetching a texture or other raster asset).
*/

AVX afxError            AvxReloadRaster
(
    // The destination avxRaster.
    avxRaster           ras,
    // The number of uploads.
    afxUnit             opCnt, 
    // An array of structure specifying the regions to be updated.
    avxRasterIo const   ops[],
    // The number of LOD to be updated.
    afxUnit             lodCnt,
    // The location for the resource to be uploaded.
    afxUri const*       uri,
    // A bitmask specifying which bridges can assume this operation.
    afxMask             exuMask
);

#endif//AVX_RASTER_IO_H
