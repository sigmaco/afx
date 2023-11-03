/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

// This section is part of SIGMA GL/2.

#ifndef AFX_VERTEX_BUFFER_H
#define AFX_VERTEX_BUFFER_H

#include "afx/draw/afxDrawDefs.h"

#include "afx/draw/afxBuffer.h"

typedef enum afxVertexFlag
{
    afxVertexFlag_DYNAMIC = AFX_BIT_OFFSET(0), // The data store contents will be modified repeatedly and used many times.
    afxVertexFlag_STREAM = AFX_BIT_OFFSET(1), // The data store contents will be modified once and used at most a few times.
    
    afxVertexFlag_POSITIONAL = AFX_BIT_OFFSET(2),
    afxVertexFlag_SPATIAL = AFX_BIT_OFFSET(3),

    afxVertexFlag_NORMALIZED = AFX_BIT_OFFSET(4),
    afxVertexFlag_RASTERIZATION = AFX_BIT_OFFSET(5),

    afxVertexFlag_AFFINE = AFX_BIT_OFFSET(10),
    /// affected by affine transformations (ex.: position). Non-delta spatial attributes should receive affine transformations.

    afxVertexFlag_LINEAR = AFX_BIT_OFFSET(11),
    /// affected by linear transformations (ex.: tangent, binormal). Delta spatial attributes should receive linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_LINEAR_INV = AFX_BIT_OFFSET(12),
    /// affected by inverse linear transformations. Non-delta spatial attributes should receive inverse linear transformations (ex.: normal, tangent/binormal cross).

    afxVertexFlag_DELTA = AFX_BIT_OFFSET(13), // treat as delta
} afxVertexFlags;

typedef enum afxVertexUsage
{
    afxVertexUsage_POS          = AFX_BIT_OFFSET(0),
    afxVertexUsage_JNT          = AFX_BIT_OFFSET(1),
    afxVertexUsage_WGT          = AFX_BIT_OFFSET(2),
    afxVertexUsage_BLENDING     = afxVertexUsage_JNT | afxVertexUsage_WGT,
    afxVertexUsage_POSITIONAL   = afxVertexUsage_POS | afxVertexUsage_BLENDING,

    afxVertexUsage_NRM          = AFX_BIT_OFFSET(3),
    afxVertexUsage_TAN          = AFX_BIT_OFFSET(4),
    afxVertexUsage_BTN          = AFX_BIT_OFFSET(5),
    afxVertexUsage_LIGHTING     = afxVertexUsage_NRM | afxVertexUsage_TAN | afxVertexUsage_BTN,
    afxVertexUsage_SPATIAL      = afxVertexUsage_POSITIONAL | afxVertexUsage_LIGHTING,

    afxVertexUsage_UV           = AFX_BIT_OFFSET(6),
    afxVertexUsage_VISUAL       = afxVertexUsage_UV,
} afxVertexUsage;

AFX_DEFINE_STRUCT(afxVertexRowSpecification)
{
    afxString const         *semantic;
    afxVertexFormat         fmt;
    afxVertexUsage          usage;
    void const              *src;
    afxVertexFormat         srcFmt;
    afxNat                  srcStride;
};

/*
    Microsoft DirectX 8.0 introduced the notion of a stream to bind data to input registers. 
    A stream is a uniform array of component data. 
    Each component consists of one or more elements that represent a single entity, such as position, normal, color, texture coordinate set, and so on. 
    With streams, GPUs are able to perform a direct memory access (DMA) from multiple vertex buffers in parallel.
*/

AFX_DEFINE_STRUCT(afxVertexBufferRegion)
{
    afxNat attrIdx;
    afxNat baseVtx;
    afxNat vtxCnt;
};

AFX_DEFINE_STRUCT(afxVertexSpec2222)
{
    afxBuffer               buf;
    afxNat                  stride;

    afxVertexFormat         fmt;
    afxVertexUsage          usage;
    afxString               semantic;
};

AFX_DEFINE_STRUCT(afxVertexSpec)
{
    afxNat                  secIdx;
    afxVertexFormat         fmt;
    afxVertexUsage          usage;
    afxString const*        semantic;
    void const*             src;
    afxVertexFormat         srcFmt;
    afxNat                  srcStride;
};

AFX_DEFINE_STRUCT(afxVertexBufferBlueprintAttr)
{
    afxNat                  secIdx;
    afxVertexFormat         fmt;
    afxNat                  relOffset;
    afxVertexUsage          usage;
    afxString               semantic;
};

AFX_DEFINE_STRUCT(afxVertexRowBlueprint)
{
    afxNat                  secIdx;
    afxVertexFormat         fmt;
    afxVertexUsage          usage;
    afxString8              semantic; // 8
    void const              *src;
    afxVertexFormat         srcFmt;
    afxNat                  srcStride;
};

AFX_DEFINE_STRUCT(afxVertexBufferBlueprint)
{
    afxFcc                  fcc; // VBUB
    afxNat                  cap;
    afxNat                  attrCnt;
    afxVertexRowBlueprint   spec[8];

    //afxVertexSpec           spec[8];
};

/*
{ // mem
    { // lod0
        { // stream0
            { // pos
                x, y, z
            },
            { // wgt

            },
            { // idx

            }
        },
        { // stream1

        }
    },
    { // lod1
        { // stream0

        },
        { // stream1

        }
    }
}

struct // lod
{
    struct
    {
        afxV4d pos;
        afxNat8 wgt[4];
        afxNat8 idx[4];
    } stream0;
    struct
    {
        afxV3d bit;
        afxV3d tan;
        afxV3d nrm;
    } stream1;
    struct
    {
        afxV3d rgb;
        afxV2d uv;
    } stream2;
};
*/

AFX_DECLARE_STRUCT(afxVertexDataRow);

AFX_DEFINE_STRUCT(afxVertexDataRow)
{
    afxNat          cnt;
    afxVertexFormat fmt;
    afxNat          relOffset;
    afxVertexUsage  usage;
    afxString       semantic;

    afxNat          streamIdx;
    afxNat          streamOffset;
    afxNat          streamRange;
};

#ifdef _AFX_VERTEX_BUFFER_C
AFX_OBJECT(afxVertexBuffer)
#else
struct afxBaseVertexBuffer
#endif
{
    afxBuffer               buf;
    afxNat                  cap;
    //afxNat                  rowCnt;
    //afxVertexDataRow        *row;
#if !0
    // NEW
    afxNat              attrCnt;
    afxNat              secCnt;
    struct
    {
        afxNat          base;
        afxNat          range;
        afxNat          stride;
    }*storages;
    struct
    {
        afxNat          srcIdx;
        afxVertexFormat fmt;
        afxNat          offset;
        afxVertexUsage  usage;
        afxString       semantic;
    }*attrs;
    // 0   0,0,0   1,1,1
#if 0
    afxNat                  attrCnt;
    struct
    {
        afxNat              streamIdx; // vertex data stream index
        afxNat              cachedElemStride; // aside fmt because it could be padded/aligned.
        afxNat              cachedElemCnt; // aside fmt because it could be a arbitrary array.
        afxVertexUsage      usage;
        afxVertexFormat     fmt;
        afxString           semantic;
    }*                      attrs;
#endif
#endif
};

#if 0
AFX afxError                AfxBuildVertexBuffers(afxDrawContext dctx, afxNat cnt, afxVertexBuffer vbuf[], afxVertexBufferBlueprint const blueprint[]);

AFX afxError                AfxVertexBufferDump(afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void *dst, afxNat dstStride); // copy out

AFX afxNat                  AfxGetVertexCapacity(afxVertexBuffer vbuf);
AFX afxNat                  AfxCountVertexSections(afxVertexBuffer vbuf);
AFX afxNat                  AfxCountVertexAttributes(afxVertexBuffer vbuf);
AFX afxNat                  AfxGetVertexSize(afxVertexBuffer vbuf);
//AFX void const*             AfxVertexBufferGetData(afxVertexBuffer vbuf, afxNat attrIdx, afxNat vtxIdx);

AFX afxBool                 AfxFindVertexAttribute(afxVertexBuffer vbuf, afxString const *name, afxNat* attrIdx);
AFX afxError                AfxDescribeVertexAttribute(afxVertexBuffer vbuf, afxNat attrIdx, afxNat* secIdx, afxVertexFormat* fmt, afxNat32* offset, afxVertexUsage* usage);
AFX afxNat                  AfxGetVertexAttributeStorage(afxVertexBuffer vbuf, afxNat attrIdx);
AFX afxVertexFormat         AfxGetVertexAttributeFormat(afxVertexBuffer vbuf, afxNat attrIdx);
AFX afxNat                  AfxGetVertexAttributeSize(afxVertexBuffer vbuf, afxNat attrIdx); // get element stride
AFX afxNat                  AfxGetVertexAttributeOffset(afxVertexBuffer vbuf, afxNat attrIdx);
AFX afxVertexUsage          AfxGetVertexAttributeUsage(afxVertexBuffer vbuf, afxNat attrIdx);
AFX afxString const*        AfxGetVertexAttributeName(afxVertexBuffer vbuf, afxNat attrIdx);

AFX afxNat                  AfxDescribeVertexStorage(afxVertexBuffer vbuf, afxNat secIdx, afxNat32* base, afxNat32* range, afxNat32* stride);
AFX afxError                AfxVertexBufferForEachVertex(afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void (*f)(void const *vtx,void*data), void *data);

AFX afxError                AfxOptimizeVertexBuffer(afxVertexBuffer vbuf, afxNat attrIdx, afxBool favorSpeedOverSize);
AFX afxError                AfxUpdateVertexBuffer(afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const *src, afxVertexFormat srcFmt, afxNat srcStride); // copy into


AFX afxError                AfxBufferizeVertexBuffer(afxVertexBuffer vbuf);
AFX afxNat                  AfxMeasureVertexBufferRegion(afxVertexBuffer vbuf, afxVertexBufferRegion const *rgn);
AFX afxNat                  AfxLocateVertexBufferRegion(afxVertexBuffer vbuf, afxNat streamIdx, afxNat attrIdx, afxNat elemIdx);

AFX afxError                AfxUpdateVertexBufferRegion(afxVertexBuffer vbuf, afxVertexBufferRegion const *rgn, void const *src, afxVertexFormat const fmt[]);
#endif

#endif//AFX_VERTEX_BUFFER_H