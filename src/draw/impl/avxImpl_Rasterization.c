/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      Q W A D R O   D R A W   I / O   S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#define _AVX_DRAW_C
#include "avxImplementation.h"

void FillRectangle(afxByte* p, afxUnit pitch, afxUnit sx, afxUnit sy, afxUnit ex, afxUnit ey, afxUnit32 color)
{
    p += pitch * sy;

    for (afxUnit y = sy; y < ey; y++, p += pitch)
    {
        for (afxUnit x = sx; x < ex; x++)
        {
            ((afxUnit32*)p)[x] = color;
        }
    }
}

afxReal calcBlendFactor(afxReal src, afxReal srcAlpha, afxReal dst, afxReal dstAlpha, avxBlendFactor factor)
{
    switch (factor)
    {
    case avxBlendFactor_ZERO: return (0.f);
    case avxBlendFactor_ONE: return (1.f);
    case avxBlendFactor_SRC_RGB: return src;
    case avxBlendFactor_SRC_A: return (srcAlpha);
    case avxBlendFactor_DST_RGB: return dst;
    case avxBlendFactor_DST_A: return (dstAlpha);
    case avxBlendFactor_ONE_MINUS_SRC_RGB: return ((1.f) - src);
    case avxBlendFactor_ONE_MINUS_SRC_A: return (1.f - srcAlpha);
    case avxBlendFactor_ONE_MINUS_DST_RGB: return ((1.f) - dst);
    case avxBlendFactor_ONE_MINUS_DST_A: return (1.f - dstAlpha);
    }
    return (0.f);
}

void calcBlendFactorV4d(afxV4d src, afxV4d dst, avxBlendFactor factor, afxV4d ret)
{
    for (afxUnit i = 0; i < 4; i++)
    {
        switch (factor)
        {
        case avxBlendFactor_ZERO: ret[i] = (0.f);
        case avxBlendFactor_ONE: ret[i] = (1.f);
        case avxBlendFactor_SRC_RGB: ret[i] = src[i];
        case avxBlendFactor_SRC_A: ret[i] = (src[3]);
        case avxBlendFactor_DST_RGB: ret[i] = dst[i];
        case avxBlendFactor_DST_A: ret[i] = (dst[3]);
        case avxBlendFactor_ONE_MINUS_SRC_RGB: ret[i] = ((1.f) - src[i]);
        case avxBlendFactor_ONE_MINUS_SRC_A: ret[i] = (1.f - src[3]);
        case avxBlendFactor_ONE_MINUS_DST_RGB: ret[i] = ((1.f) - dst[i]);
        case avxBlendFactor_ONE_MINUS_DST_A: ret[i] = (1.f - dst[3]);
        }
    }
}

afxReal calcBlendFunc(afxReal src, afxReal dst, avxBlendOp op)
{
    switch (op)
    {
    case avxBlendOp_ADD: return src + dst;
    case avxBlendOp_SUB: return src - dst;
    case avxBlendOp_REV_SUB:  return dst - src;
    case avxBlendOp_MIN: return AfxMin(src, dst);
    case avxBlendOp_MAX: return AfxMax(src, dst);
    }
    return src + dst;
}

void calcBlendFuncV4d(afxV4d src, afxV4d dst, avxBlendOp op, afxV4d ret)
{
    for (afxUnit i = 0; i < 4; i++)
    {
        switch (op)
        {
        case avxBlendOp_ADD: ret[i] = src[i] + dst[i];
        case avxBlendOp_SUB: ret[i] = src[i] - dst[i];
        case avxBlendOp_REV_SUB: ret[i] = dst[i] - src[i];
        case avxBlendOp_MIN: ret[i] = AfxMin(src[i], dst[i]);
        case avxBlendOp_MAX: ret[i] = AfxMax(src[i], dst[i]);
        }
    }
}

void calcBlendColorV4d(afxV4d const src, afxV4d const dst, const avxColorBlend *params, afxV4d ret)
{
    afxV4d srcF =
    {
        calcBlendFactor(src[0], src[3], dst[0], dst[3], params->rgbSrcFactor),
        calcBlendFactor(src[1], src[3], dst[1], dst[3], params->rgbSrcFactor),
        calcBlendFactor(src[2], src[3], dst[2], dst[3], params->rgbSrcFactor),
        calcBlendFactor(src[3], src[3], dst[3], dst[3], params->aSrcFactor)
    };

    afxV4d dstF =
    {
        calcBlendFactor(dst[0], dst[3], dst[0], dst[3], params->rgbDstFactor),
        calcBlendFactor(dst[1], dst[3], dst[1], dst[3], params->rgbDstFactor),
        calcBlendFactor(dst[2], dst[3], dst[2], dst[3], params->rgbDstFactor),
        calcBlendFactor(dst[3], dst[3], dst[3], dst[3], params->aDstFactor)
    };

    ret[0] = calcBlendFunc(src[0] * srcF[0], dst[0] * dstF[0], params->rgbBlendOp);
    ret[1] = calcBlendFunc(src[1] * srcF[1], dst[1] * dstF[1], params->rgbBlendOp);
    ret[2] = calcBlendFunc(src[2] * srcF[2], dst[2] * dstF[2], params->rgbBlendOp);
    ret[3] = calcBlendFunc(src[3] * srcF[3], dst[3] * dstF[3], params->aBlendOp);
}

afxBool DepthTest(afxReal a, afxReal b, avxCompareOp op)
{
    switch (op) {
    case avxCompareOp_NEVER:     return FALSE;
    case avxCompareOp_LESS:      return a < b;
    case avxCompareOp_EQUAL:     return AfxAbsf(a - b) <= AFX_EPSILON32;
    case avxCompareOp_LEQUAL:    return a <= b;
    case avxCompareOp_GREATER:   return a > b;
    case avxCompareOp_NOTEQUAL:  return AfxAbsf(a - b) > AFX_EPSILON32;
    case avxCompareOp_GEQUAL:    return a >= b;
    case avxCompareOp_ALWAYS:    return TRUE;
    }
    return a < b;
}

AFX_DEFINE_STRUCT(_avxVtxCache)
{
    afxBool discard; // FALSE
    afxSize index; // 0

    void* vertex; // NIL
    afxReal* varyings; // NIL

    afxInt clipMask; // 0
    afxV4d clipPos; // 0.f // clip space position
    afxV4d fragPos; // 0.f // screen space position

    afxUnit8* vertexHolder; // NIL
    afxReal* varyingsHolder; // NIL
};

AFX_DEFINE_STRUCT(_avxPrimCache)
{
    afxBool discard; // FALSE
    afxBool frontFacing; // TRUE
    afxSize indices[3]; // { 0, 0, 0 }
};

AFX_DEFINE_STRUCT(_avxSampleCtx)
{
    afxBool inside; // FALSE
    afxUnit fboCoord[2]; // 0
    afxV4d position; // 0.f
    afxV4d barycentric; // 0.f
};

AFX_DEFINE_STRUCT(_avxPixelCtx)
{
    afxBool inside; // FALSE
    afxReal* varyingsFrag; // NIL
    afxUnit samplesPop;
    _avxSampleCtx samples[32];
    _avxSampleCtx* sampleShading; // NIL
    afxInt sampleCnt; // 0
    afxInt coverage; // 0
};

void GetSampleLocation4X(_avxPixelCtx* ctx, afxV2d loc[4])
{
    static afxV2d location_4x[4] =
    {
            {0.375f, 0.875f},
            {0.875f, 0.625f},
            {0.125f, 0.375f},
            {0.625f, 0.125f},
    };
    //return location_4x;

    for (afxUnit i = 0; i < 4; i++)
        AfxV2dCopy(loc[i], location_4x[i]);
}

void PixelCtxInit(_avxPixelCtx* ctx, afxReal x, afxReal y, afxInt sampleCnt)
{
    afxError err;
    AFX_ASSERT(sampleCnt);

    ctx->inside = FALSE;
    ctx->sampleCnt = sampleCnt;
    ctx->coverage = 0;

    if (sampleCnt > 1)
    {
        ctx->samplesPop = ctx->sampleCnt + 1;  // store center sample at end

        if (ctx->sampleCnt == 4)
        {
            static afxV2d location_4x[4] =
            {
                    {0.375f, 0.875f},
                    {0.875f, 0.625f},
                    {0.125f, 0.375f},
                    {0.625f, 0.125f},
            };

            for (int i = 0; i < ctx->sampleCnt; i++)
            {
                ctx->samples[i].fboCoord[0] = x;
                ctx->samples[i].fboCoord[1] = y;

                AfxV2dAdd(ctx->samples[i].position, location_4x[i], AFX_V2D(x, y));
                ctx->samples[i].position[2] = 0.f;
                ctx->samples[i].position[3] = 0.f;
            }
            // pixel center
            ctx->samples[4].fboCoord[0] = x;
            ctx->samples[4].fboCoord[1] = y;
            ctx->samples[4].position[0] = x + 0.5f;
            ctx->samples[4].position[1] = y + 0.5f;
            ctx->samples[4].position[2] = 0.f;
            ctx->samples[4].position[3] = 0.f;
            ctx->sampleShading = &ctx->samples[4];
        }
        else
        {
            // not support
        }
    }
    else
    {
        ctx->samplesPop = 1;
        ctx->samples[0].fboCoord[0] = x;
        ctx->samples[0].fboCoord[1] = y;
        ctx->samples[0].position[0] = x + 0.5f;
        ctx->samples[0].position[1] = y + 0.5f;
        ctx->samples[0].position[2] = 0.f;
        ctx->samples[0].position[3] = 0.f;
        ctx->sampleShading = &ctx->samples[0];
    }
}

afxBool PixelCtxInitCoverage(_avxPixelCtx* ctx)
{
    if (ctx->sampleCnt > 1)
    {
        ctx->coverage = 0;
        ctx->inside = FALSE;

        for (int i = 0; i < ctx->samplesPop - 1; i++)
        {
            if (ctx->samples[i].inside) {
                ctx->coverage++;
            }
        }
        ctx->inside = ctx->coverage > 0;
    }
    else
    {
        ctx->coverage = 1;
        ctx->inside = ctx->samples[0].inside;
    }
    return ctx->inside;
}

void PixelCtxInitShadingSample(_avxPixelCtx* ctx)
{
    if (ctx->sampleShading->inside)
    {
        return;
    }

    for (afxUnit i = 0; i < ctx->samplesPop; i++)
    {
        _avxSampleCtx* sample = &ctx->samples[i];

        if (sample->inside)
        {
            ctx->sampleShading = &sample;
            break;
        }
    }
}

AFX_DEFINE_STRUCT(_avxPixelQuadCtx)
{
    /*
        p2--p3
        |   |
        p0--p1
     */
    _avxPixelCtx pixels[4];

    // triangle vertex screen space position
    afxV4d vertPos[3];
    afxV4d vertPosFlat[4];

    // triangle barycentric correct
    afxReal const* vertZ[3]; // NIL, NIL, NIL
    afxV4d vertW; // { 0, 0, 0, 1 }

    // triangle vertex shader varyings
    afxReal const* vertVaryings[3]; // NIL, NIL, NIL

    // triangle Facing
    afxBool frontFacing; // TRUE

    // shader program
    void* shaderProgram; // NIL

    afxSize varyingsAlignedCnt_; // 0
    afxReal* varyingsPool_; // NIL
};

void PixelQuadCtxSetVaryingsSize(_avxPixelQuadCtx* ctx, afxSize size)
{
    if (ctx->varyingsAlignedCnt_ != size)
    {
        ctx->varyingsAlignedCnt_ = size;
        ctx->varyingsPool_ = NIL;
        AfxAllocate(sizeof(float) * (4 * ctx->varyingsAlignedCnt_), NIL, AfxHere(), &ctx->varyingsPool_);

        for (int i = 0; i < 4; i++)
        {
            ctx->pixels[i].varyingsFrag = ctx->varyingsPool_ + i * ctx->varyingsAlignedCnt_;
        }
    }
}

void PixelQuadCtxInit(_avxPixelQuadCtx* ctx, afxReal x, afxReal y, afxInt sample_cnt)
{
    afxError err;
    AFX_ASSERT(sample_cnt);

    PixelCtxInit(&ctx->pixels[0], x, y, sample_cnt);
    PixelCtxInit(&ctx->pixels[0], x, y, sample_cnt);
    PixelCtxInit(&ctx->pixels[1], x + 1, y, sample_cnt);
    PixelCtxInit(&ctx->pixels[2], x, y + 1, sample_cnt);
    PixelCtxInit(&ctx->pixels[3], x + 1, y + 1, sample_cnt);
}

afxBool PixelQuadCtxCheckInside(_avxPixelQuadCtx* ctx)
{
    return ctx->pixels[0].inside || ctx->pixels[1].inside || ctx->pixels[2].inside || ctx->pixels[3].inside;
}


void WrapTexel2(avxTexelWrap mode, afxV2d uv)
{
    switch (mode)
    {
    case avxTexelWrap_BORDER:
        uv[0] = AfxClamp(uv[0], 0.0f, 1.0f);
        uv[1] = AfxClamp(uv[1], 0.0f, 1.0f);
        break;
    case avxTexelWrap_EDGE:
        uv[0] = AfxClamp(uv[0], 0.0f, 1.0f);
        uv[1] = AfxClamp(uv[1], 0.0f, 1.0f);
        break;
    case avxTexelWrap_REPEAT:
        if (uv[0] < 0.0f) uv[0] += 1.0f;
        if (uv[0] > 1.0f) uv[0] -= 1.0f;
        if (uv[1] < 0.0f) uv[1] += 1.0f;
        if (uv[1] > 1.0f) uv[1] -= 1.0f;
        break;
    }
}

void WrapTexel3(avxTexelWrap mode, afxV3d uvw)
{
    switch (mode)
    {
    case avxTexelWrap_BORDER:
        uvw[0] = AfxClamp(uvw[0], 0.0f, 1.0f);
        uvw[1] = AfxClamp(uvw[1], 0.0f, 1.0f);
        uvw[2] = AfxClamp(uvw[2], 0.0f, 1.0f);
        break;
    case avxTexelWrap_EDGE:
        uvw[0] = AfxClamp(uvw[0], 0.0f, 1.0f);
        uvw[1] = AfxClamp(uvw[1], 0.0f, 1.0f);
        uvw[2] = AfxClamp(uvw[2], 0.0f, 1.0f);
        break;
    case avxTexelWrap_REPEAT:
        if (uvw[0] < 0.0f) uvw[0] += 1.0f;
        if (uvw[0] > 1.0f) uvw[0] -= 1.0f;
        if (uvw[1] < 0.0f) uvw[1] += 1.0f;
        if (uvw[1] > 1.0f) uvw[1] -= 1.0f;
        if (uvw[2] < 0.0f) uvw[2] += 1.0f;
        if (uvw[2] > 1.0f) uvw[2] -= 1.0f;
        break;
    }
}
