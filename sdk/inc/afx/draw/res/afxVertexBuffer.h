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

    AFX_VTX_USAGE_RASTERIZATION = AFX_FLAG(16),
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
    afxNat streamIdx;
    afxNat baseAttr; // component of stream (aka set of attributes)
    afxNat attrCnt;
    afxNat baseElem;
    afxNat elemCnt;
};

AFX_DEFINE_STRUCT(afxVertexRowBlueprint)
{
    afxString8              semantic; // 8
    afxVertexFormat         fmt;
    afxVertexUsage          usage;
    void const              *src;
    afxVertexFormat         srcFmt;
};

AFX_DEFINE_STRUCT(afxVertexBufferBlueprint)
{
    afxFcc                  fcc; // VBUB
    afxNat                  cap;
    afxNat                  rowCnt;
    afxVertexRowBlueprint   row[8];
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
    afxString           semantic;
    afxNat              offset;
    afxNat              stride;
    afxVertexUsage      usage;
    afxVertexFormat     fmt;
};

#ifdef _AFX_VERTEX_BUFFER_C
AFX_OBJECT(afxVertexBuffer)
#else
struct afxBaseVertexBuffer
#endif
{
    afxBuffer               buf;
    afxNat                  cap;
    afxNat                  rowCnt;
    afxVertexDataRow        *row;
#if !0
    // NEW
    afxNat                  streamCnt;
    struct
    {
        afxNat              rowCnt;
        struct
        {
            afxNat          stride;
        }*                  rows;
    }*                      streams;
    afxNat                  attrCnt;
    struct
    {
        afxNat              streamIdx;
        afxNat              cachedElemStride; // aside fmt because it could be padded/aligned.
        afxNat              cachedElemCnt; // aside fmt because it could be a arbitrary array.
        afxVertexUsage      usage;
        afxVertexFormat     fmt;
        afxString           semantic;
    }*                      attrs;
#endif
};

AFX afxNat                  AfxEnumerateVertexBuffers(afxDrawContext dctx, afxNat first, afxNat cnt, afxVertexBuffer vbuf[]);

AFX afxError                AfxBuildVertexBuffers(afxDrawContext dctx, afxNat cnt, afxVertexBuffer vbuf[], afxVertexBufferBlueprint const blueprint[]);

AFX afxError                AfxVertexBufferDescribeRow(afxVertexBuffer vbuf, afxNat rowIdx, afxVertexRowSpecification *spec);
AFX afxError                AfxVertexBufferDump(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void *dst, afxNat dstVtxStride); // copy out
AFX afxNat                  AfxVertexBufferFindArrange(afxVertexBuffer vbuf, afxString const *name);
AFX afxNat                  AfxVertexBufferGetRowCount(afxVertexBuffer vbuf);
//AFX void const*             AfxVertexBufferGetData(afxVertexBuffer vbuf, afxNat rowIdx, afxNat vtxIdx);
AFX afxVertexFormat         AfxVertexBufferGetFormat(afxVertexBuffer vbuf, afxNat rowIdx);
AFX afxString const*        AfxVertexBufferGetSemantic(afxVertexBuffer vbuf, afxNat rowIdx);
AFX afxNat                  AfxVertexBufferGetRange(afxVertexBuffer vbuf, afxNat baseRow, afxNat rowCnt); // get whole memory range used by a arrange
AFX afxNat                  AfxVertexBufferGetOffset(afxVertexBuffer vbuf, afxNat vtxIdx, afxNat rowIdx); // get whole memory range used by a arrange
AFX afxNat                  AfxVertexBufferGetStride(afxVertexBuffer vbuf, afxNat rowIdx); // get element stride
AFX afxNat                  AfxVertexBufferGetCap(afxVertexBuffer vbuf);
AFX afxNat                  AfxVertexBufferGetPerVertexSize(afxVertexBuffer vbuf);
AFX afxError                AfxVertexBufferForEachVertex(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void (*f)(void const *vtx,void*data), void *data);
AFX afxError                AfxVertexBufferOptimize(afxVertexBuffer vbuf, afxNat rowIdx, afxBool favorSpeedOverSize);
AFX afxError                AfxVertexBufferUpdate(afxVertexBuffer vbuf, afxNat rowIdx, afxNat baseVtx, afxNat vtxCnt, void const *src, afxVertexFormat srcFmt); // copy into




AFX afxNat                  AfxGetVertexStreamCount(afxVertexBuffer vbuf);

AFX afxError                AfxBufferizeVertexBuffer(afxVertexBuffer vbuf);
AFX afxNat                  AfxMeasureVertexBufferRegion(afxVertexBuffer vbuf, afxVertexBufferRegion const *rgn);
AFX afxNat                  AfxLocateVertexBufferRegion(afxVertexBuffer vbuf, afxNat streamIdx, afxNat attrIdx, afxNat elemIdx);

AFX afxError                AfxUpdateVertexBufferRegion(afxVertexBuffer vbuf, afxVertexBufferRegion const *rgn, void const *src, afxVertexFormat const fmt[]);


#endif//AFX_VERTEX_BUFFER_H