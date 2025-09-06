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

// This software is part of Advanced Multimedia UX Extensions & Experiments.

#define _AUX_UX_C
#define _AUX_FONT_C
#include "src/ux/impl/auxImplementation.h"

_AFX afxError AfxTranscribe(afxFont fnt, afxRect const* area, afxUnit col, afxUnit row, afxString const* text)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FNT, 1, &fnt);

    /*
        CEA-608 captions should appear in a title-safe region, typically 80% of screen width and 90% of screen height.
        This gives you the rectangular box (top-left x/y and size) where captions are drawn.

        caption_width  = 0.80 * 1920 = 1536 px
        caption_height = 0.90 * 1080 = 972 px

        caption_x = (1920 - 1536) / 2 = 192 px
        caption_y = (1080 - 972) / 2 = 54 px

        So the caption area is a 1536×972 box starting at position (192, 54).

        Now, to map the 32×15 grid, each character cell then becomes:

        cell_width  = caption_width  / 32
        cell_height = caption_height / 15

        Using the example above:

        cell_width  = 1536 / 32 = 48 px
        cell_height = 972 / 15 = 64.8 px

        Each caption character is drawn inside a 48×65 px box (rounded), monospaced.

        Aspect Ratio
        The CEA-608 grid (32×15) has a character aspect ratio of:

        width  = 32 units
        height = 15 units

        aspect = 32 / 15 ~= 2.13

        To maintain this aspect ratio on any screen, your rendering should preserve this relative width and height per character.

        That means: if we're rendering captions in a pixel-based UI, we should stretch or scale text to match this ratio,
        or we'll get distortion (too tall or too wide).
    */

    afxReal screen_width = area->w;
    afxReal screen_height = area->h;
    afxReal caption_width = 0.80 * screen_width;
    afxReal caption_height = 0.90 * screen_height;
    afxReal caption_x = (screen_width - caption_width) / 2;
    afxReal caption_y = (screen_height - caption_height) / 2;
    afxReal cell_width = caption_width / 32;
    afxReal cell_height = caption_height / 15;

    afxReal x = caption_x + col * cell_width;
    afxReal y = caption_y + row * cell_height;
    afxUnit numchar = text->len;
    afxReal r = 1, g = 1, b = 1;
    afxReal x2 = x;

    afxSize vboOffset;
    afxUnit vboRange;
    afxReal* verts = (afxReal*)&fnt->fntVboPtr[20 * ((col * 32) + row)];

    //afxReal* verts = AvxGetBufferMap(dout->fntVbo, 0, 20 * text->len);
    void const* bufStart = verts;

    for (char const *ptr = text->start, i = 0; *ptr != '\0'; ptr++)
    {
        // Decrement 'y' for any CR's
        if (*ptr == '\n')
        {
            x = x2;
            y -= 16;
            continue;
        }

#if !0
        // Just advance spaces instead of rendering empty quads
        if (*ptr == ' ')
        {
            x += 16;
            numchar--;
            continue;
        }
#endif
        // Emit position, atlas offset, and color for this character
        *verts++ = x;
        *verts++ = y;
        *verts++ = (float)(*ptr % 16)*0.0625f; // *verts++ = (float)(*ptr % 16)*0.0625f;
        *verts++ = 1.0f - (float)(*ptr / 16)*0.0625f; // *verts++ = 1.0f - (float)(*ptr / 16)*0.0625f;
        //*verts++ = r;
        //*verts++ = g;
        //*verts++ = b;
        *verts++ = 1.f;

        // Advance one character
        x += 16;
    }

    fnt->fntStoreLen += numchar;

    return err;
}

_AFX afxError AfxFlushDeviceFont(afxFont fnt, afxDrawContext dctx, avxViewport const* vp)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FNT, 1, &fnt);
    AFX_ASSERT_OBJECTS(afxFcc_DCTX, 1, &dctx);

    AvxCmdBindPipeline(dctx, 0, fnt->fntPip, NIL, NIL);

    afxM4d p;
    AfxM4dReset(p);
    AfxComputeOffcenterOrthographicMatrix(p, 0, vp->extent[0], 0, vp->extent[1], -1.f, 1.f, FALSE, avxClipSpaceDepth_NEG_ONE_TO_ONE);
    //AfxComputeBasicOrthographicMatrix(p, vp.extent[0] / vp.extent[1], 1.0, 3.0, &AVX_CLIP_SPACE_OPENGL);
    AvxCmdPushConstants(dctx, 0, sizeof(p), p);

    //AvxCmdBindRasters(dctx, avxBus_DRAW, 0, 0, 1, &dout->fntRas);
    AvxCmdBindSamplers(dctx, avxBus_DRAW, 0, 0, 1, &fnt->fntSamp);

    avxBufferedStream bufi = { 0 };
    bufi.buf = fnt->fntVbo;
    bufi.offset = 0;
    bufi.range = 32 * 15 * 20;
    bufi.stride = sizeof(afxV2d) + sizeof(afxV2d) + sizeof(afxByte[4]);
    AvxCmdBindVertexBuffers(dctx, 0, 1, &bufi);
    AvxCmdDraw(dctx, 4, fnt->fntStoreLen, 0, 0);

    fnt->fntStoreLen = 0;

    return err;
}

_AUX afxError _AuxFntDtorCb(afxFont fnt)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FNT, 1, &fnt);

    if (fnt->fntEnabled)
    {
        AfxDisposeObjects(1, &fnt->fntPip);
        AfxDisposeObjects(1, &fnt->fntSamp);
        AfxDisposeObjects(1, &fnt->fntVbo);
    }

    return err;
}

_AUX afxError _AuxFntCtorCb(afxFont fnt, void** args, afxUnit invokeNo)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FNT, 1, &fnt);

    afxSession ses = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    AFX_ASSERT(args[1]);
    afxFontConfig const* cfg = AFX_CAST(afxFontConfig const*, args[1]) + invokeNo;

    if (cfg->dsys && cfg->isDevFont)
    {
        afxDrawSystem dsys = cfg->dsys;

        // DEVICE FONT STUFF

        avxVertexInput vin;
        avxVertexLayout vlay = { 0 };
        vlay.binCnt = 1;
        vlay.bins[0].attrCnt = 3;
        vlay.bins[0].instRate = 1;
        vlay.attrs[0].location = 0;
        vlay.attrs[0].fmt = avxFormat_RG32f;
        vlay.attrs[1].location = 1;
        vlay.attrs[1].offset = 8;
        vlay.attrs[1].fmt = avxFormat_RG32f;
        vlay.attrs[2].location = 2;
        vlay.attrs[2].offset = 16;
        vlay.attrs[2].fmt = avxFormat_RGBA8un;
        AvxDeclareVertexInputs(dsys, 1, &vlay, &vin);
        AFX_ASSERT_OBJECTS(afxFcc_VIN, 1, &vin);

        avxPipeline pip;
        avxPipelineConfig pipb = { 0 };
        pipb.fillMode = avxFillMode_FACE;
        pipb.cullMode = avxCullMode_BACK;
        pipb.primTop = avxTopology_TRI_LIST; // avxTopology_TRI_STRIP
        pipb.vin = vin;

        afxString const fontVsh = AFX_STRING("\
        IN(0, vec2, in_instPos); \
        IN(1, vec2, in_instUv); \
        IN(2, vec4, in_instRgba); \
        PUSH(pushable) {\
        mat4 p;\
        };\
        \
        const vec2 pos[4] = { vec2(0, 16), vec2(0, 0), vec2(16, 16), vec2(16, 0) };\
        const vec2 uv[4] = { vec2(0.00625, -0.00625), vec2(0.00625, -0.05625), vec2(0.05625, -0.00625), vec2(0.05652, -0.05625) };\
        out block { vec2 uv; vec4 rgba;} sgl_v;\
        void main(){\
            gl_Position = p * vec4((pos[gl_VertexID] + in_instPos), 0.0, 1.0);\
            sgl_v.uv = uv[gl_VertexID] + in_instUv;\
            sgl_v.rgba = vec4(1, 1, 1, 1);//in_instRgba;\
        }\
        ");

        afxString const fontFsh = AFX_STRING("\
        TEXTURE(0, 0, sampler2D, atlas);\
        in block { vec2 uv; vec4 rgba; } sgl_v;\
        OUT(0, vec4, sgl_rgba);\
        void main(){\
            vec4 sampled = texture(atlas, sgl_v.uv); //vec4(1.0, 1.0, 1.0, texture(samp, sgl_v.uv0).a);\
            sgl_rgba = sgl_v.rgba * sampled;\
            //sgl_rgba = vec4(/*sgl_v.rgba.rgb*/vec3(1.0, 1.0, 1.0), texture(atlas, sgl_v.uv).r);\
        }\
        ");

        AvxAssemblePipelines(dsys, 1, &pipb, &pip);
        AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
        AfxRecompilePipelineFunction(pip, avxShaderType_VERTEX, &fontVsh, NIL, NIL, NIL);
        AfxRecompilePipelineFunction(pip, avxShaderType_FRAGMENT, &fontFsh, NIL, NIL, NIL);
        AfxDisposeObjects(1, &vin);

        avxBuffer fntDataBuf;
        avxBufferInfo vboSpec = { 0 };
        vboSpec.flags = avxBufferFlag_W;
        vboSpec.usage = avxBufferUsage_VERTEX;
        vboSpec.size = AFX_MAX(2048, cfg->typeBufCap);
        vboSpec.mapped = TRUE;
        AvxAcquireBuffers(dsys, 1, &vboSpec, &fntDataBuf);
        AFX_ASSERT_OBJECTS(afxFcc_BUF, 1, &fntDataBuf);

        avxSampler fntSamp;
        avxSamplerConfig smpCnf = { 0 };
        smpCnf.minify = avxTexelFilter_LINEAR;
        smpCnf.uvw[0] = avxTexelWrap_EDGE;
        smpCnf.uvw[1] = avxTexelWrap_EDGE;
        smpCnf.uvw[2] = avxTexelWrap_EDGE;
        AvxConfigureSampler(dsys, &smpCnf);
        AvxDeclareSamplers(dsys, 1, &smpCnf, &fntSamp);
        AFX_ASSERT_OBJECTS(afxFcc_SAMP, 1, &fntSamp);

        fnt->fntVbo = fntDataBuf;
        fnt->fntVboPtr = AvxGetBufferMap(fntDataBuf, 0, vboSpec.size);
        fnt->fntSamp = fntSamp;
        fnt->fntPip = pip;

        AvxMakeBufferedRing(&fnt->fntVboRng, 2, 20, AVX_BUFFER_ALIGNMENT, fnt->fntVbo, 32 * 15, NIL);

        fnt->fntEnabled = TRUE;
    }

    return err;
}

_AUX afxClassConfig const _AUX_FNT_CLASS_CONFIG =
{
    .fcc = afxFcc_FNT,
    .name = "Font",
    .desc = "Typographic Font",
    .fixedSiz = sizeof(AFX_OBJECT(afxFont)),
    .ctor = (void*)_AuxFntCtorCb,
    .dtor = (void*)_AuxFntDtorCb
};

////////////////////////////////////////////////////////////////////////////////

_AUX afxError AfxAcquireFonts(afxUnit cnt, afxFontConfig const cfg[], afxFont fonts[])
{
    afxError err = AFX_ERR_NONE;

    afxSession ses;
    if (!AfxGetSession(&ses))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AuxSesGetFntClass(ses);
    AFX_ASSERT_CLASS(cls, afxFcc_FNT);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)fonts, (void const*[]) { ses, cfg }))
    {
        AfxThrowError();
        return err;
    }

    return err;
}

_AUX afxError AfxLoadFonts(afxUnit cnt, afxUri const uri[], afxFont fonts[])
{

    afxUri uri2;
    AfxMakeUri(&uri2, 0, "//./z/video/font-256.tga", 0);
    //AvxLoadRasters(dsys, avxRasterUsage_TEXTURE, NIL, 1, &uri2, &dsys->fntRas);

}
