/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>
// This software is part of Advanced Video Graphics Extensions & Experiments.

#define _AVX_DRAW_C
#define _AVX_BUFFER_C
#define _AVX_VERTEX_INPUT_C
#include "../avxIcd.h"

afxError AfxReadBuffer(avxBuffer buf, afxSize offset, afxUnit range, afxUnit stride, void* data)
{
    AfxCopy(data, &(AFX_CAST(afxByte*, buf->storage[0].host.addr)[offset]), stride);
}

void DoVertex(afxUnit vtxId, void* data)
{

}

typedef struct _avxVertexIn
{
    union
    {
        afxV4d vec4;
        afxV3d vec3;
        afxV2d vec2;
        afxReal f;
        afxInt iv4[4];
        afxInt iv3[3];
        afxInt iv2[2];
        afxInt i;
        afxUnit uv4[4];
        afxUnit uv3[3];
        afxUnit uv2[2];
        afxUnit u;
    } va[AVX_MAX_VERTEX_ATTRIBS];
} _avxVertexIn;

void _AvxFetchVertices(avxVertexInput vtxd, afxUnit vtxCnt, afxUnit instCnt, avxBufferedStream const streams[], afxByte* data)
{
    avxBuffer srcBuf[AVX_MAX_VERTEX_SOURCES];
    afxByte* srcPtr[AVX_MAX_VERTEX_SOURCES];
    afxUnit srcStride[AVX_MAX_VERTEX_SOURCES];
    avxFormatDescription attrFd[AVX_MAX_VERTEX_ATTRIBS];

    // bind buffers

    for (afxUnit fIdx = 0; fIdx < vtxd->binCnt; fIdx++)
    {
        avxVertexStream const* vtxf = &vtxd->bins[fIdx];
        avxBufferedStream const* vtxs = &streams[vtxf->pin];
        AfxReadBuffer(vtxs->buf, vtxs->offset, vtxs->range, vtxs->stride, data);

        srcBuf[vtxf->pin] = vtxs->buf;
        srcPtr[vtxf->pin] = &(AFX_CAST(afxByte*, vtxs->buf->storage[0].host.addr)[vtxs->offset]);
        srcStride[vtxf->pin] = vtxs->stride;
    }

    // describe attributes

    for (afxUnit aIdx = 0; aIdx < vtxd->totalAttrCnt; aIdx++)
    {
        avxVertexAttr const* vtxa = &vtxd->attrs[aIdx];
        AvxDescribeFormats(1, &vtxa->fmt, &attrFd[aIdx]);
    }

    for (afxUnit instIdx = 0; instIdx < instCnt; instIdx++)
    {
        for (afxUnit vtxIdx = 0; vtxIdx < vtxCnt; vtxIdx++)
        {
            // step per-vertex stream fetch

            for (afxUnit fIdx = 0; fIdx < vtxd->binCnt; fIdx++)
            {
                avxVertexStream const* vs = &vtxd->bins[fIdx];

                for (afxUnit aIdx = 0; aIdx < vtxd->totalAttrCnt; aIdx++)
                {
                    avxVertexAttr const* va = &vtxd->attrs[aIdx];
                    AfxCopy(data, &srcPtr[fIdx][va->offset], attrFd[aIdx].stride);
                    data += attrFd[aIdx].stride;
                }

                DoVertex(vtxIdx, data);

                if (!vs->instRate)
                    srcPtr[vs->pin] += srcStride[vs->pin];
            }
        }

        // step per-instance stream fetch

        for (afxUnit fIdx = 0; fIdx < vtxd->binCnt; fIdx++)
        {
            avxVertexStream const* vtxf = &vtxd->bins[fIdx];

            if (vtxf->instRate)
                srcPtr[vtxf->pin] += srcStride[vtxf->pin];
        }
    }
}

typedef struct {
    float position[4];
    float normal[4];
    float color[4];
    float texcoord[2];
    // Add more as needed...
} MyVertexInput; // Custom format expected by your T&L logic

void fetch_vertex(MyVertexInput* out, int vertexIdx, const avxVertexInput layout, const avxBufferedStream* streams)
{
    for (int attrIdx = 0; attrIdx < layout->totalAttrCnt; ++attrIdx)
    {
        avxVertexAttr* attr = &layout->attrs[attrIdx];
        avxVertexStream* fetch = NULL;

        // Find the fetch config for this attribute
        for (int srcIdx = 0; srcIdx < layout->binCnt; ++srcIdx)
        {
            if (layout->bins[srcIdx].baseAttrIdx <= attrIdx &&
                attrIdx < layout->bins[srcIdx].baseAttrIdx + layout->bins[srcIdx].attrCnt)
            {
                fetch = &layout->bins[srcIdx];
                break;
            }
        }

        if (!fetch) continue;

        // Stream from the source binding
        const avxBufferedStream* stream = &streams[fetch->pin];

        uint8_t* base = (uint8_t*)stream->buf->storage[0].host.bytemap; // You probably have a mapped pointer
        uint32_t stride = fetch->minStride ? fetch->minStride : stream->stride;

        // Adjust for instancing here if needed
        uint32_t finalIdx = vertexIdx; // or instanceIdx depending on fetch->instRate

        uint8_t* vertexData = base + stream->offset + finalIdx * stride + attr->offset;

        // Now decode based on attr->fmt (avxFormat)
        switch (attr->location)
        {
        case 0: // POSITION
            //decode_format(attr->fmt, vertexData, out->position);
            AvxConvertFormat(1, 1, vertexData, 0, attr->fmt, avxFormat_RGBA32f, out->position, 0);
            break;
        case 1: // NORMAL
            //decode_format(attr->fmt, vertexData, out->normal);
            AvxConvertFormat(1, 1, vertexData, 0, attr->fmt, avxFormat_RGBA32f, out->normal, 0);
            break;
        case 2: // COLOR
            //decode_format(attr->fmt, vertexData, out->color);
            AvxConvertFormat(1, 1, vertexData, 0, attr->fmt, avxFormat_RGBA32f, out->color, 0);
            break;
        case 3: // TEXCOORD
            //decode_format(attr->fmt, vertexData, out->texcoord);
            AvxConvertFormat(1, 1, vertexData, 0, attr->fmt, avxFormat_RG32f, out->texcoord, 0);
            break;
        }
    }
}

typedef struct {
    float clipPos[4];
    float color[4];
    float texcoord[2];
} MyTransformedVertex;

void transform_vertex(MyVertexInput* in, MyTransformedVertex* out, afxM4d mvp)
{
    afxV4d pos;
    AfxV4dSet(pos, in->position[0], in->position[1], in->position[2], 1.0f);
    AfxV4dPostMultiplyM4d(out->clipPos, mvp, pos);
    AfxV4dCopy(out->color, in->color);
    AfxV2dCopy(out->texcoord, in->texcoord);
}

void PerspectiveDivideToNdc(MyTransformedVertex* triangle)
{
    // Perspective divide to NDC
    // After clip space, divide by W to go to Normalized Device Coordinates (NDC).

    for (int i = 0; i < 3; ++i)
    {
        MyTransformedVertex* v = &triangle[i];
        float invW = 1.0f / v->clipPos[3];
        v->clipPos[0] *= invW;
        v->clipPos[1] *= invW;
        v->clipPos[2] *= invW;
        // clipPos[3] becomes 1.0 (or keep it if you want for depth)
    }

    // Now clipPos holds X/Y/Z in NDC space: [-1, +1].
}

void ndc_to_screen(float screenPos[2], const float clipPos[4], int screenW, int screenH)
{
    // Viewport Transform -> Screen Coordinate
    // Map from NDC to screen

    screenPos[0] = (clipPos[0] * 0.5f + 0.5f) * screenW;
    screenPos[1] = (1.0f - (clipPos[1] * 0.5f + 0.5f)) * screenH; // Y-flip

    // Call this for all 3 triangle vertices.
}

void AvxViewportTransform0(avxViewport const*vp, afxUnit cnt, afxV4d const clipPos[], afxV3d screenPos[])
{
    // From NDC to Screen Coordinates
    // You have vertices in NDC space after perspective divide :
    //  x, y in range[-1, 1]
    //  z in range[-1, 1](OpenGL) or [0, 1](Vulkan / D3D)
    //  Your avxViewport defines the destination window area and depth range

    /*  USAGE:

        MyVertexOut vertex; // already transformed and divided
        float screenPos[3];
        avxViewport_Transform(&viewport, vertex.clipPos, screenPos);
    */

    // Assume clipPos is already perspective-divided (i.e. NDC)

    for (afxUnit i = 0; i < cnt; i++)
    {
        // X and Y: [-1, 1] -> screen coordinates
        screenPos[i][0] = vp->origin[0] + (clipPos[i][0] * 0.5f + 0.5f) * vp->extent[0];
        screenPos[i][1] = vp->origin[1] + (clipPos[i][1] * 0.5f + 0.5f) * vp->extent[1];

        // Z (depth): [-1, 1] -> [minDepth, maxDepth] (assuming OpenGL-style NDC)
        afxReal ndcZ = clipPos[i][2];
        afxReal depth = ndcZ * 0.5f + 0.5f; // Map [-1, 1] -> [0, 1]

        //  If you're following Vulkan-style NDC where Z is already in [0, 1], skip the ndcZ * 0.5 + 0.5 step.

        screenPos[i][2] = vp->minDepth + depth * (vp->maxDepth - vp->minDepth);
    }

    /*
        Now screenPos holds:
            screenPos[0]: screen X
            screenPos[1]: screen Y
            screenPos[2]: depth (between minDepth and maxDepth)
        You can now use this to:
            Rasterize triangles in pixel coordinates
            Interpolate and write depth buffer values

        Y-flip Consideration
        Your avxViewport.origin is bottom-left (like OpenGL), so the transform above preserves that.
        If your framebuffer or rasterizer assumes top-left origin (like Vulkan or D3D), flip Y like this:
        screenPos[1] = vp->origin[1] + (1.0f - (clipPos[1] * 0.5f + 0.5f)) * vp->extent[1];
    */
}

#if 0
void draw_triangle(const MyTransformedVertex v[3], uint32_t* framebuffer, int screenW, int screenH)
{
    // Rasterize Triangle (Simplified).
    // After you have screenPos[2] for each vertex:
    //  Compute triangle bounding box
    //  Iterate over pixels in box
    //  Use barycentric coordinates or edge functions to determine coverage
    //  Shade pixel with interpolated color(or flat color for now)

    float sx[3], sy[3];
    for (int i = 0; i < 3; ++i)
        ndc_to_screen((float[]) { sx[i], sy[i] }, v[i].clipPos, screenW, screenH);

    // Compute bounding box (clamped)
    int minX = max(0, floorf(min3(sx[0], sx[1], sx[2])));
    int maxX = min(screenW - 1, ceilf(max3(sx[0], sx[1], sx[2])));
    int minY = max(0, floorf(min3(sy[0], sy[1], sy[2])));
    int maxY = min(screenH - 1, ceilf(max3(sy[0], sy[1], sy[2])));

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            // Simple coverage test (e.g. using barycentric or edge function)
            if (point_in_triangle(x + 0.5f, y + 0.5f, sx, sy)) {
                framebuffer[y * screenW + x] = 0xFFFF00FF; // magenta pixel
            }
        }
    }
}
#endif

float edge_fn(float x0, float y0, float x1, float y1, float x, float y)
{
    // Point-In-Triangle (Edge Function).
    return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
}
void perspective_divide(float v[4]) {
    float w = v[3];
    if (w != 0.0f) {
        v[0] /= w; v[1] /= w; v[2] /= w; v[3] = 1.0f;
    }
}
void rasterize_triangle(MyTransformedVertex tri[3], const avxViewport* vp, uint32_t* framebuffer, int fbWidth, int fbHeight)
{
    // Triangle Rasterizer.

    float screen[3][3]; // 3 vertices, each with x, y, z
    for (int i = 0; i < 3; ++i) {
        perspective_divide(tri[i].clipPos);
        AvxViewportTransform0(vp, 1, &tri[i].clipPos, &screen[i]);
    }

    float* v0 = screen[0];
    float* v1 = screen[1];
    float* v2 = screen[2];

    // Bounding box
    int minX = (int)fmaxf(0, floorf(fminf(fminf(v0[0], v1[0]), v2[0])));
    int minY = (int)fmaxf(0, floorf(fminf(fminf(v0[1], v1[1]), v2[1])));
    int maxX = (int)fminf(fbWidth - 1, ceilf(fmaxf(fmaxf(v0[0], v1[0]), v2[0])));
    int maxY = (int)fminf(fbHeight - 1, ceilf(fmaxf(fmaxf(v0[1], v1[1]), v2[1])));

    float area = edge_fn(v0[0], v0[1], v1[0], v1[1], v2[0], v2[1]);
    if (area == 0.0f) return; // Degenerate triangle

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {
            float px = (float)x + 0.5f;
            float py = (float)y + 0.5f;

            float w0 = edge_fn(v1[0], v1[1], v2[0], v2[1], px, py);
            float w1 = edge_fn(v2[0], v2[1], v0[0], v0[1], px, py);
            float w2 = edge_fn(v0[0], v0[1], v1[0], v1[1], px, py);

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                framebuffer[y * fbWidth + x] = 0xFF00FFFF; // flat magenta
            }
        }
    }
}

