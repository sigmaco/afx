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

// This software is part of Advanced User Experiences Extensions & Experiments.

#define _AUX_UX_C
#define _AUX_FONT_C
#include "src/ux/impl/auxImplementation.h"
#include "dep/stb_truetype.h"

// Unicode code point: U+FFFD
// UTF-8 encoding: EF BF BD
// The official name is "Replacement Character".
// It is used to represent an unknown, unrecognized, or unrepresentable character in Unicode.
#define AFX_UTF_REPLACEMENT_CHAR 0xFFFD // is the Unicode replacement character.

// Unicode: U+FFFE
// Meaning: Not a character.
// It is used in combination with U+FEFF to detect byte order in UTF-16 encodings.
// For example, if you read two bytes FF FE, it usually indicates UTF-16LE (little-endian).
// If you read FE FF, it indicates UTF-16BE (big-endian).
// Because of that, 0xFFFE is never a valid character in normal Unicode text; it helps detect encoding issues.
#define AFX_UTF_NACHAR 0xFFFE

// Unicode: U + FFFF
// Also a noncharacter.
// It's never assigned to a character and is not valid for text interchange.
// Like U + FFFE, it can be used internally in software (for example, as a sentinel value).
#define AFX_UTF_INVALID 0xFFFF

AFX_DEFINE_STRUCT(BakedFont)
{
    // height of the font.
    afxReal height;
    // font glyphs ascent and descent.
    afxReal ascent;
    // font glyphs ascent and descent.
    afxReal descent;
    // glyph array offset inside the font glyph baking output array.
    afxUnit glyph_offset;
    // number of glyphs of this font inside the glyph baking array output.
    afxUnit glyph_count;
    // font codepoint ranges as pairs of (from/to) and 0 as last element.
    afxUnit const* ranges;
};

AFX_DEFINE_STRUCT(afxGlyph)
{
    afxUnit codepoint;
    float xAdvance;
    float x0, y0, x1, y1, w, h;
    float u0, v0, u1, v1;
};

AFX_DEFINE_STRUCT(auxGlyph)
{
    // Texture coordinates.
    afxV2d uv[2];
    // Offset between top left and glyph.
    afxV2d offset;
    // Size of the glyph.
    afxReal width, height;
    // Offset to the next glyph.
    afxReal xadvance;
};

AFX_DEFINE_STRUCT(afxFontAtlas)
{
    avxRange    whd;
    avxRaster   ras;
};

AFX_DEFINE_STRUCT(GlyphInfo) {
    float ax; // advance.x
    float x0, y0, x1, y1; // screen quad
    float s0, t0, s1, t1; // texture UVs
};

AFX_DEFINE_STRUCT(GlyphPage) {
    avxRaster texture_id;
    GlyphInfo glyphs[96]; // 96 = strip size
    afxBool loaded;
};

#if 0
GlyphPage* get_glyph_page(GlyphPage* glyph_pages, int codepoint, stbtt_fontinfo* font, float scale) {
    int base = (codepoint / 96) * 96;
    int page_index = codepoint / 96;

    if (!glyph_pages[page_index].loaded) {
        // Rasterize 96 glyphs into texture
        unsigned char bitmap[512 * 128] = { 0 }; // tweak size based on font
        stbtt_packedchar packed[96];

        stbtt_pack_context pc;
        stbtt_PackBegin(&pc, bitmap, 512, 128, 0, 1, NULL);
        stbtt_PackSetOversampling(&pc, 2, 2);

        stbtt_PackFontRange(&pc, font_data, 0, scale * 72.0f, base, 96, packed);
        stbtt_PackEnd(&pc);

        // Upload to GPU
        GLuint tex;
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 128, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store texture and metadata
        glyph_pages[page_index].texture_id = tex;
        for (int i = 0; i < 96; i++) {
            glyph_pages[page_index].glyphs[i] = convert_to_GlyphInfo(packed[i]);
        }

        glyph_pages[page_index].loaded = true;
    }

    return &glyph_pages[page_index];
}
#endif

_AUX afxReal AfxMeasureTextWidth(afxFont fnt, afxReal fntHeight, afxString const* text)
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FNT, 1, &fnt);


}

_AUX void AfxQueryFontGlyph(afxFont fnt, afxReal fntHeight, afxUnit cnt, afxUnit codepointPairs[][2], auxGlyph glyphes[])
{
    afxError err = AFX_ERR_NONE;
    AFX_ASSERT_OBJECTS(afxFcc_FNT, 1, &fnt);
    AFX_ASSERT(codepointPairs);
    AFX_ASSERT(glyphes);

    for (afxUnit i = 0; i < cnt; i++)
    {
        auxGlyph* glyph = &glyphes[i];


    }
}

// We can better handle it by binding the font to a surface, where the binding state would hold the dynamic data.
// And, we could draw it during presentation calls.

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

    AvxCmdBindPipeline(dctx, fnt->fntPip, NIL, NIL);

    afxM4d p;
    AfxM4dReset(p);
    AfxComputeOffcenterOrthographicMatrix(p, 0, vp->extent[0], 0, vp->extent[1], -1.f, 1.f, FALSE, avxClipSpaceDepth_NEG_ONE_TO_ONE);
    //AfxComputeBasicOrthographicMatrix(p, vp.extent[0] / vp.extent[1], 1.0, 3.0, &AVX_CLIP_SPACE_OPENGL);
    AvxCmdPushConstants(dctx, 0, sizeof(p), p);

    //AvxCmdBindRasters(dctx, avxBus_GFX, 0, 0, 1, &dout->fntRas);
    AvxCmdBindSamplers(dctx, avxBus_GFX, 0, 0, 1, &fnt->fntSamp);

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

    afxEnvironment env = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
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
        AvxAcquireVertexInputs(dsys, 1, &vlay, &vin);
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

        AvxAssembleGfxPipelines(dsys, 1, &pipb, &pip);
        AFX_ASSERT_OBJECTS(afxFcc_PIP, 1, &pip);
        AfxDisposeObjects(1, &vin);

        avxCodebase codb;
        AvxGetPipelineCodebase(pip, &codb);
        AFX_ASSERT_OBJECTS(afxFcc_SHD, 1, &codb);
        avxShaderSpecialization specs[2] = { 0 };
        specs[0].stage = avxShaderType_VERTEX;
        specs[0].prog = AFX_STRING("fontVsh");
        specs[1].stage = avxShaderType_FRAGMENT;
        specs[1].prog = AFX_STRING("fontFsh");
        AvxCompileShader(codb, &specs[0].prog, &fontVsh);
        AvxCompileShader(codb, &specs[1].prog, &fontFsh);
        AvxReprogramPipeline(pip, 2, specs);

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
        AvxAcquireSamplers(dsys, 1, &smpCnf, &fntSamp);
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

    afxEnvironment env;
    if (!AfxGetEnvironment(&env))
    {
        AfxThrowError();
        return err;
    }
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);

    if (!cfg)
    {
        AfxThrowError();
        return err;
    }

    afxClass* cls = (afxClass*)_AuxEnvGetFntClass(env);
    AFX_ASSERT_CLASS(cls, afxFcc_FNT);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)fonts, (void const*[]) { env, cfg }))
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
