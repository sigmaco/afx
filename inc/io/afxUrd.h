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

// This code is part of SIGMA Future Storage.

// URD --- UNIFORM RESOURCE DICTIONARY

#ifndef AFX_URD_H
#define AFX_URD_H

#include "qwadro/inc/io/afxStream.h"
#include "qwadro/inc/io/afxResource.h"

typedef enum afxChunkId
{
    afxChunkId_NIL,
    afxChunkId_STRUCT,
    afxChunkId_STRING,
    afxChunkId_EXTENSION,
    afxChunkId_RW_CAM = 0x05,
    afxChunkId_RW_TEX = 0x06, // RwTexture
    afxChunkId_RW_MTL = 0x07, // RpMaterial
    afxChunkId_RW_MTLL = 0x08,
    afxChunkId_RW_ATOMSEC = 0x09,
    afxChunkId_RW_PLANESEC = 0x0A,
    afxChunkId_RW_WORLD = 0x0B, // RpWorld
    afxChunkId_RW_SPLINE = 0x0C, // RpSpline
    afxChunkId_RW_MATRIX = 0x0D, // RwMatrix
    afxChunkId_RW_FRAMELIST = 0x0E,
    afxChunkId_RW_GEOM = 0x0F, // RpGeometry
    afxChunkId_RW_CLUMP = 0x10, // RpClump
    afxChunkId_RW_LIGHT = 0x12, // RpLight
    afxChunkId_RW_UNISTRING = 0x13,
    afxChunkId_RW_ATOMIC = 0x14, // RpAtomic
    afxChunkId_RW_RASTER = 0x15,
    afxChunkId_RW_TXD = 0x16, // RwTexDictionary (device-dependent)
    afxChunkId_RW_ANIMDB = 0x17,
    afxChunkId_RW_IMAGE = 0x18, // RwImage
    afxChunkId_RW_SKANIM = 0x19,
    afxChunkId_RW_GEOLIST = 0x1A,
    afxChunkId_RW_ANIMANIM = 0x1B, // RtAnimAnimation
    afxChunkId_RW_HANIMANIM = afxChunkId_RW_ANIMANIM, // backward compatibility
    afxChunkId_RW_TEAM = 0x1C, // RpTeam
    afxChunkId_RW_CROWD = 0x1D,
    afxChunkId_RW_DMORPHANIM = 0x1E, // RpDMorphAnimation
    afxChunkId_RW_RIGHT2RENDER = 0x1F,
    afxChunkId_RW_MTFXNAT = 0x20, // RpMTEffect (multi-texture effect chunk)
    afxChunkId_RW_MTFXDIC = 0x21, // RpMTEffectDict (multi-texture effect dictionary chunk)
    afxChunkId_RW_TEAMDIC = 0x22, // RpTeamDictionary
    afxChunkId_RW_PITXD = 0x23, // RwTexDictionary (device-independent)
    afxChunkId_RW_TOC = 0x24, // RtTOC
    afxChunkId_RW_PRTSTDGLOBALDATA = 0x25, // RpPrtStdEmitterClass, RpPrtStdParticleClass, RpPrtStdPropertyTable.
    afxChunkId_RW_ALTPIPE = 0x26,
    afxChunkId_RW_PIPEDS = 0x27,
    afxChunkId_RW_PATCHMESH = 0x28, // RpPatchMesh
    afxChunkId_RW_BEGIN = 0x29, // chunk group start.
    afxChunkId_RW_END = 0x2A, // chunk group end.
    afxChunkId_RW_UVANIMDIC = 0x2B, // UV anim dictionary
    afxChunkId_RW_COLLTREE = 0x2C,
    afxChunkId_RW_ENVIRONMENT = 0x2D, // RpEnvironment chunk is used internally to convey art package background settings to the Visualizer.
    // reserved to RenderWare middlewares up to 0x50
    afxChunkId_QW_SKL = 0x51, // like the afxChunkId_RW_FRAMELIST for Qwadro
    afxChunkId_QW_GEO,
    afxChunkId_QW_MSHT,
    afxChunkId_QW_MSH,
    afxChunkId_QW_MDL,
    afxChunkId_QW_MOT,
    afxChunkId_QW_ANI,
} afxChunkId;

#pragma pack(push, 1)

AFX_DEFINE_STRUCT(afxUrdOrigin)
{
    afxNat32    segIdx;
    afxNat32    offset; // relative to start of segIdx
};

AFX_DEFINE_STRUCT(urdMark)
{
    // must be afxNat32[3] to be compatible with RenderWare stream allowing Qwadro features to be skipped as unknown/unsupported chunks.
    afxNat32        fcc;
    afxNat32        siz;
    afxNat32        ver;
};

AFX_DEFINE_STRUCT(urdTrunk)
{
    // .hdr.fcc = [ 't', 'x', 'd', '\0' ], [ 'a', 'n', 'd', '\0' ], [ 'm', 'd', 'd', '\0' ], [ 't', 'g', 'a', '\0' ]
    // .hdr.siz = must include all segments.
    // .hdr.ver = still undefined.
    urdMark         hdr;
    afxNat32        segCnt;
    afxNat32        baseSegOff; // where is urdSegment[] inside trunk scope.
    afxNat32        baseStrOff;
    afxNat32        strLen;
};

AFX_DEFINE_STRUCT(urdSegment)
{
    afxNat32        decSiz; // decoded/uncompressed size
    afxNat32        decAlign;
    afxNat32        encSiz; // encoded/compressed size
    afxNat32        encAlign;
    afxNat32        codec; // codec used to compress this section
    afxNat32        start; // where starts the data belonging to this segment.
};

AFX_DEFINE_STRUCT(urdRoot)
{
    // .hdr.fcc = [ 'u', 'r', 'd, '/0' ]
    // .hdr.siz = must be exactly sizeof(urdRoot) due its usage as being a file footer hence not allowed to be extended.
    // .hdr.ver = still undefined (Qwadro has not versions)
    urdMark         hdr;
    afxNat32        trunkFcc;
    afxNat32        trunkOff;
    afxNat32        trunkSiz;
    afxNat32        segBaseOff; // where is urdSegment[] inside trunk scope.
    afxNat32        segCnt;
    afxNat32        strBaseOff;
    afxNat32        strLen;
};

#pragma pack(pop)

AFX afxBool     AfxFetchNextStreamChunk(afxStream in, afxNat32 id, urdMark* hdr);

AFX afxBool     AfxReadNextSeriesHeader(afxStream in, urdMark* hdr);
AFX afxBool     AfxWriteNextSeriesHeader(afxStream out, urdMark const* hdr);

AFX afxNat      AfxMeasureSerializableExtensions(afxObject obj);
AFX afxError    AfxStoreSerializableExtensions(afxStream out, afxObject obj);
AFX afxError    AfxLoadSerializedExtensions(afxStream in, urdMark const* hdr, afxObject obj);
AFX afxError    AfxSkipSerializedExtensions(afxStream in, urdMark const* hdr);



AFX afxBool AfxOpenUrdSegments(afxUrd urd, afxNat firstSeg, afxNat segCnt, afxStream in);

AFX void    AfxCloseUrdSegments(afxUrd urd, afxNat firstSeg, afxNat segCnt);

AFX void*   AfxResolveUrdReference(afxUrd urd, afxUrdOrigin const* ref);

#endif//AFX_URD_H
