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

#ifndef AFX_VERTEX_BUFFER_H
#define AFX_VERTEX_BUFFER_H

#include "afx/draw/afxDrawDefs.h"

#include "afx/draw/res/afxBuffer.h"

typedef enum afxVertexUsage
{
    AFX_VTX_USAGE_DYNAMIC       = AFX_FLAG(0), // The data store contents will be modified repeatedly and used many times.
    AFX_VTX_USAGE_STREAM        = AFX_FLAG(1), // The data store contents will be modified once and used at most a few times.
    //AFX_VTX_USAGE_STATIC = AFX_FLAG(2), // The data store contents will be modified once and used many times.

    AFX_VTX_USAGE_POS           = AFX_FLAG( 9),
    AFX_VTX_USAGE_NRM           = AFX_FLAG(10),
    AFX_VTX_USAGE_TAN           = AFX_FLAG(11),
    AFX_VTX_USAGE_BTN           = AFX_FLAG(12),
    AFX_VTX_USAGE_UV            = AFX_FLAG(13),

    AFX_VTX_HINT_POSITIONAL     = AFX_FLAG(14),
    AFX_VTX_HINT_SPATIAL        = AFX_FLAG(15),

    AFX_VTX_USAGE_NORMALIZED    = AFX_FLAG(17),
    AFX_VTX_USAGE_RASTERIZATION = AFX_FLAG(18),
} afxVertexUsage;

AFX_DEFINE_STRUCT(afxVertexRowSpecification)
{
    afxString const         *semantic;
    afxVertexFormat         fmt;
    afxVertexUsage          usage;
    void const              *src;
    afxVertexFormat         srcFmt;
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
    }*sections;
    struct
    {
        afxNat          secIdx;
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

AFX afxNat                  AfxEnumerateVertexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxVertexBuffer vbuf[]);

AFX afxError                AfxBuildVertexBuffers(afxDrawContext dctx, afxNat cnt, afxVertexBuffer vbuf[], afxVertexBufferBlueprint const blueprint[]);

AFX afxError                AfxVertexBufferDump(afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void *dst, afxNat dstStride); // copy out

AFX afxNat                  AfxGetVertexCapacity(afxVertexBuffer vbuf);
AFX afxNat                  AfxCountVertexSections(afxVertexBuffer vbuf);
AFX afxNat                  AfxCountVertexAttributes(afxVertexBuffer vbuf);
AFX afxNat                  AfxGetVertexSize(afxVertexBuffer vbuf);
//AFX void const*             AfxVertexBufferGetData(afxVertexBuffer vbuf, afxNat attrIdx, afxNat vtxIdx);

AFX afxBool                 AfxFindVertexAttribute(afxVertexBuffer vbuf, afxString const *name, afxNat* attrIdx);
AFX afxError                AfxDescribeVertexAttribute(afxVertexBuffer vbuf, afxNat attrIdx, afxNat* secIdx, afxVertexFormat* fmt, afxNat32* offset, afxVertexUsage* usage);
AFX afxNat                  AfxGetVertexAttributeSection(afxVertexBuffer vbuf, afxNat attrIdx);
AFX afxVertexFormat         AfxGetVertexAttributeFormat(afxVertexBuffer vbuf, afxNat attrIdx);
AFX afxNat                  AfxGetVertexAttributeSize(afxVertexBuffer vbuf, afxNat attrIdx); // get element stride
AFX afxNat                  AfxGetVertexAttributeOffset(afxVertexBuffer vbuf, afxNat attrIdx);
AFX afxVertexUsage          AfxGetVertexAttributeUsage(afxVertexBuffer vbuf, afxNat attrIdx);
AFX afxString const*        AfxGetVertexAttributeName(afxVertexBuffer vbuf, afxNat attrIdx);

AFX afxNat                  AfxDescribeVertexSection(afxVertexBuffer vbuf, afxNat secIdx, afxNat32* base, afxNat32* range, afxNat32* stride);
AFX afxError                AfxVertexBufferForEachVertex(afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void (*f)(void const *vtx,void*data), void *data);

AFX afxError                AfxOptimizeVertexBuffer(afxVertexBuffer vbuf, afxNat attrIdx, afxBool favorSpeedOverSize);
AFX afxError                AfxUpdateVertexBuffer(afxVertexBuffer vbuf, afxNat attrIdx, afxNat baseVtx, afxNat vtxCnt, void const *src, afxVertexFormat srcFmt, afxNat srcStride); // copy into


AFX afxError                AfxBufferizeVertexBuffer(afxVertexBuffer vbuf);
AFX afxNat                  AfxMeasureVertexBufferRegion(afxVertexBuffer vbuf, afxVertexBufferRegion const *rgn);
AFX afxNat                  AfxLocateVertexBufferRegion(afxVertexBuffer vbuf, afxNat streamIdx, afxNat attrIdx, afxNat elemIdx);

AFX afxError                AfxUpdateVertexBufferRegion(afxVertexBuffer vbuf, afxVertexBufferRegion const *rgn, void const *src, afxVertexFormat const fmt[]);


#endif//AFX_VERTEX_BUFFER_H