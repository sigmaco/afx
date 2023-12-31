#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/core/afxSystem.h"
#include "qwadro/core/afxDebug.h"

#include "qwadro/sim/awxNode.h"

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"
#include "qwadro/afxQwadro.h"
#include "../src/e2bink/afxBinkVideo.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/font/afxTypography.h"
#include "qwadro/draw/io/afxVertexStream.h"
#include "qwadro/core/afxBitmap.h"
#include "qwadro/math/afxOpticalMatrix.h"

#define ENABLE_DIN1 // 
#define ENABLE_DOUT1

//#define ENABLE_DIN2 // yuv
//#define ENABLE_DOUT2

//#define ENABLE_DIN3 // rgb
//#define ENABLE_DOUT3

afxSimulation sim = NIL;
afxRaster dumpImg = NIL;

afxDrawOutput dout[3] = { NIL, NIL, NIL };
afxDrawInput din[3] = { NIL, NIL, NIL };
afxDrawContext dctx = NIL;

//afxCanvas canv[3][60] = { NIL };

afxFixedUri2048 uri, uri2;

afxBuffer ubo = NIL;
afxPipeline dpip[2] = { NIL, NIL };
afxRaster tex[4] = { NIL, NIL, NIL, NIL };

afxBinkVideo bnk = { 0 };
afxRaster fontRas = NIL;
afxRaster fontRas2 = NIL;
afxSampler fontSmp = NIL;
afxPipeline fontPip = NIL;
afxBuffer textVbo = NIL;
afxBuffer viewConstantsBuffer = NIL;
afxTypography typ = NIL;

void AfxCmdDrawText(afxDrawScript dscr, afxNat fontIdx, afxWhd origin, afxReal scale, afxString const* text)
{
    afxReal x = origin[0];
    afxReal y = origin[1];
    afxNat numchar = text->len;
    afxReal r = 1, g = 1, b = 1;
    afxReal x2 = x;

    afxReal* verts = AfxMapBufferRange(textVbo, 0, ((16 * 16) * 16), afxBufferAccess_W);

    for (char const *ptr = text->start, i = 0; *ptr != '\0'; ptr++)
    {
        // Decrement 'y' for any CR's
        if (*ptr == '\n')
        {
            x = x2;
            y -= 16;
            continue;
        }

#if 0
        // Just advance spaces instead of rendering empty quads
        if (*ptr == ' ')
        {
            x += 16;
            numchar--;
            continue;
        }
#endif

        // ANSI color escape codes
        // I'm sure there's a better way to do this!
        // But it works, so whatever.
        if (*ptr == '\x1B')
        {
            ptr++;
            if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '0'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.0f;	b = 0.0f; }	// BLACK
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '1'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.0f;	b = 0.0f; }	// DARK RED
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '2'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.5f;	b = 0.0f; }	// DARK GREEN
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '3'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.5f;	b = 0.0f; }	// DARK YELLOW
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '4'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.0f;	b = 0.5f; }	// DARK BLUE
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '5'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.0f;	b = 0.5f; }	// DARK MAGENTA
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '6'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.5f;	b = 0.5f; }	// DARK CYAN
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '3'&&*(ptr + 2) == '7'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.5f;	b = 0.5f; }	// GREY
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '0'&&*(ptr + 3) == 'm') { r = 0.5f;	g = 0.5f;	b = 0.5f; }	// GREY
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '1'&&*(ptr + 3) == 'm') { r = 1.0f;	g = 0.0f;	b = 0.0f; }	// RED
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '2'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 1.0f;	b = 0.0f; }	// GREEN
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '3'&&*(ptr + 3) == 'm') { r = 1.0f;	g = 1.0f;	b = 0.0f; }	// YELLOW
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '4'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 0.0f;	b = 1.0f; }	// BLUE
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '5'&&*(ptr + 3) == 'm') { r = 1.0f;	g = 0.0f;	b = 1.0f; }	// MAGENTA
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '6'&&*(ptr + 3) == 'm') { r = 0.0f;	g = 1.0f;	b = 1.0f; }	// CYAN
            else if (*(ptr + 0) == '['&&*(ptr + 1) == '9'&&*(ptr + 2) == '7'&&*(ptr + 3) == 'm') { r = 1.0f;	g = 1.0f;	b = 1.0f; }	// WHITE
            ptr += 4;
        }

        // Emit position, atlas offset, and color for this character
        *verts++ = x;
        *verts++ = y;
        *verts++ = (float)(*ptr % 16)*0.0625f; // *verts++ = (float)(*ptr % 16)*0.0625f;
        *verts++ = 1.0f - (float)(*ptr / 16)*0.0625f; // *verts++ = 1.0f - (float)(*ptr / 16)*0.0625f;
        *verts++ = r;
        *verts++ = g;
        *verts++ = b;
        *verts++ = 1.f;

        // Advance one character
        x += 16;
    }

    AfxUnmapBufferRange(textVbo);

    AfxCmdBindPipeline(dscr, 0, fontPip);

    AfxCmdBindBuffers(dscr, 0, 0, 1, &viewConstantsBuffer, NIL, NIL);
    
    if (fontIdx)
    {
        AfxCmdBindRasters(dscr, 0, 1, 1, &fontSmp, &fontRas2);
    }
    else
    {
        AfxCmdBindRasters(dscr, 0, 1, 1, &fontSmp, &fontRas);
    }

    //AfxCmdResetVertexStreams(dscr, 1, NIL, (afxNat32[]) { sizeof(afxV4d) * 2 }, (afxBool const[]) { 1 });
    AfxCmdBindVertexSources(dscr, 0, 1, &textVbo, NIL, NIL, NIL);
    AfxCmdDraw(dscr, 4, numchar, 0, 0);
}

_AFXEXPORT afxError DrawInputProc(afxDrawInput din, afxNat thrUnitIdx) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxBinkVideo *bnk = AfxGetDrawInputUdd(din);

    if (bnk->running)
    {
        afxDrawContext dctx = AfxGetDrawInputContext(din);
        afxNat unitIdx;
        AfxGetThreadingUnit(&unitIdx);

        afxDrawScript dscr;

        if (AfxAcquireDrawScripts(din, 0, 1, &dscr)) AfxThrowError();
        else
        {
            if (AfxRecordDrawScript(dscr, afxDrawScriptUsage_ONCE)) AfxThrowError();
            else
            {
                afxNat outBufIdx = 0;
                AfxRequestDrawOutputBuffer(dout[0], 0, &outBufIdx);
                afxCanvas canv;
                AfxGetDrawOutputCanvas(dout[0], outBufIdx, 1, &canv);
                //afxRaster surf = AfxGetDrawOutputSurface(dout[0], outBufIdx);
                AfxAssertObjects(1, &canv, afxFcc_CANV);
                //AfxAssertObjects(1, &surf, afxFcc_RAS);

                afxBool readjust = TRUE;
                afxBool upscale = FALSE;
                afxWhd extent;

                extent[0] = bnk->whd[0];
                extent[1] = bnk->whd[1];
                extent[2] = bnk->whd[2];

                if (readjust)
                    AfxGetCanvasExtent(canv, extent);

                if (!upscale)
                {
                    if (extent[0] > bnk->whd[0])
                        extent[0] = bnk->whd[0];

                    if (extent[1] > bnk->whd[1])
                        extent[1] = bnk->whd[1];

                    if (extent[2] > bnk->whd[2])
                        extent[2] = bnk->whd[2];
                }

                afxNat annexCnt;

                afxDrawTarget dpt = { 0 };
                dpt.clearValue.color[0] = 0.3;
                dpt.clearValue.color[1] = 0.1;
                dpt.clearValue.color[2] = 0.3;
                dpt.clearValue.color[3] = 1;
                dpt.loadOp = afxSurfaceLoadOp_CLEAR;
                dpt.storeOp = afxSurfaceStoreOp_STORE;

                afxSynthesisConfig dps = { 0 };
                dps.canv = canv;
                dps.layerCnt = 1;
                dps.rasterCnt = 1;
                dps.rasters = &dpt;
                dps.depth = NIL;
                dps.stencil = NIL;
                dps.area.extent[0] = extent[0];
                dps.area.extent[1] = extent[1];
                AfxCmdBeginSynthesis(dscr, &dps);

                afxViewport vp = { 0 };
                vp.extent[0] = extent[0];
                vp.extent[1] = extent[1];
                vp.depth[0] = (afxReal)0;
                vp.depth[1] = (afxReal)1;
                AfxCmdResetViewports(dscr, 1, &vp);

#if 0
                afxPipelineRasterizerState ras = { 0 };
                ras.cullMode = afxCullMode_BACK;
                ras.fillMode = afxFillMode_SOLID;
                ras.frontFace = afxFrontFace_CCW;
                ras.lineWidth = 1.f;
                AfxCmdSetRasterizerState(dscr, &ras);
#endif
                // turn off Z buffering, culling, and projection (since we are drawing orthographically)
                //afxPipelineDepthState const depth = { 0 };
                //AfxCmdSetDepthState(dscr, &depth);

                //AfxBinkDoFrame(&bnk, TRUE, TRUE, outBufIdx, dscr, canv, NIL);
                //AfxBinkDoFrame(bnk, TRUE, TRUE);
                //AfxBinkBlitFrame(bnk2, dscr, canv[0][outBufIdx], NIL);
                //AfxBinkDoFrame(bnk, TRUE, TRUE, 0, 0, NIL);
                AfxBinkBlitFrame(bnk, dscr);

#if 0
                if (AfxRandom2(0, 60) == 60)
                {
                    afxFixedUri2048 uri;
                    AfxMakeFixedUri2048(&uri);
                    AfxFormatUri(&uri.uri, "tmp/bink_frame_%u.tga", bnk->set.bink_buffers.FrameNum);
                    AfxPrintRasterToTarga(surf, 0, 0, 1, &uri);
                }
#endif

#if !0
                awxViewConstants vc = { 0 };

                //afxWhd extent;
                AfxGetDrawOutputExtent(dout[0], extent);

                //afxViewport vp = { 0 };
                vp.extent[0] = bnk->whd[0];
                vp.extent[1] = bnk->whd[1];
                vp.depth[1] = 1;

                vc.viewExtent[0] = vp.extent[0];
                vc.viewExtent[1] = vp.extent[1];

                //AfxComputeRenderWareProjectionM4d(vc.p, &vp, FALSE);
                //AfxResetOrthographicMatrix(vc.p, vp.extent[0] / vp.extent[1], 1, FALSE, afxClipDepthRange_NEG_ONE_TO_ONE);
                //AfxResetPerspectiveMatrix(vc.p, vp.extent[0] / vp.extent[1], FALSE, afxClipDepthRange_NEG_ONE_TO_ONE);

                // glOrtho( 0, c.right - c.left, c.bottom - c.top, 0, -1.0F, 1.0F );
                AfxComputeOffcenterOrthographicMatrix(vc.p, 0, vp.extent[0], 0, vp.extent[1], -1.f, 1.f, FALSE, afxClipDepthRange_NEG_ONE_TO_ONE);

                AfxResetM4d(vc.v);

                //afxDrawTarget dpt = { 0 };
                dpt.clearValue.color[0] = 0.3;
                dpt.clearValue.color[1] = 0.1;
                dpt.clearValue.color[2] = 0.3;
                dpt.clearValue.color[3] = 1;
                dpt.loadOp = afxSurfaceLoadOp_LOAD;
                dpt.storeOp = afxSurfaceStoreOp_STORE;

                //afxSynthesisConfig dps = { 0 };
                dps.canv = canv;
                dps.layerCnt = 1;
                dps.rasterCnt = 1;
                dps.rasters = &dpt;
                dps.depth = NIL;
                dps.stencil = NIL;
                dps.area.extent[0] = vp.extent[0];
                dps.area.extent[1] = vp.extent[1];
                AfxCmdBeginSynthesis(dscr, &dps);

                AfxCmdResetViewports(dscr, 1, &vp);

                //AfxCmdBindFontSIG(dscr, 0, 1, &typ, &fontPip, &fontSmp, &fontRas);

                //AfxCmdResetVertexStreams(dscr, 1, NIL, (afxNat32[]) { sizeof(afxV4d) }, NIL);
                AfxCmdBindVertexSources(dscr, 0, 1, &textVbo, NIL, NIL, NIL);

                AfxUpdateBuffer(viewConstantsBuffer, 0, sizeof(vc), &vc);

                AfxCmdBindBuffers(dscr, 0, 0, 1, &viewConstantsBuffer, NIL, NIL);
                AfxCmdBindRasters(dscr, 0, 1, 1, &fontSmp, &fontRas);

                AfxCmdDrawText(dscr, AfxKeyIsPressed(0, AFX_KEY_A), (afxNat[]) { 100, 100, 1 }, 1, &AfxString("Qwadro Execution Ecosystem\nSIGMA_GL/2"));
                
                //render_text(dscr, "W", 10, 50, 1, 1);
                //render_text(dscr, "Qwadro Execution Ecosystem", 10, 50, sx, sy);
                //render_text(dscr, "SIGMA Technology Group", 40, 10, sx, sy);

                AfxCmdFinishSynthesis(dscr);
#endif

                if (AfxCompileDrawScript(dscr)) AfxThrowError();
                else if (AfxSubmitDrawScripts(din, 1, &dscr))
                    AfxThrowError();

                if (AfxPresentDrawOutputBuffers(din, 1, &dout[0], &outBufIdx))
                    AfxThrowError();
            }
        }
    }
    return err;
}

_AFXEXPORT afxResult Once(afxApplication app)
{
    afxError err = AFX_ERR_NONE;

    AfxEntry("app=%p", app);
    AfxMakeFixedUri2048(&uri, NIL);

    AfxFormatUri(&uri.uri, "art/world.tga");
    
    if (AfxLoadRastersFromTarga(dctx, NIL, NIL, 1, &uri.uri, &dumpImg))
        AfxThrowError();

    AfxAssert(dumpImg);
    AfxFormatUri(&uri.uri, "tmp/world2.tga");
    AfxPrintRasterToTarga(dumpImg, 0, 0, 1, &uri.uri);
    AfxReleaseObjects(1, (void*[]) { dumpImg });

    AfxFormatUri(&uri.uri, "desktop");
    afxDrawOutputConfig doutConfig = { 0 };
    //doutConfig.endpoint = &uri.uri;
#if 0
    doutConfig.whd[0] = 1280;
    doutConfig.whd[1] = 720;
    doutConfig.whd[2] = 1;
    doutConfig.bufCnt = 2;
    doutConfig.clipped = TRUE;
    doutConfig.colorSpc = NIL;
    doutConfig.presentAlpha = FALSE;
    doutConfig.pixelFmt = afxPixelFormat_RGBA8;
    doutConfig.presentMode = afxPresentMode_LIFO;
    doutConfig.presentTransform = NIL;
    doutConfig.bufUsage = AFX_TEX_FLAG_SURFACE_RASTER;
#endif
    doutConfig.pixelFmt = afxPixelFormat_RGB8;
    //doutConfig.colorSpc = afxColorSpace_SRGB;
    
#ifdef ENABLE_DOUT1
    AfxOpenDrawOutputs(0, 1, &doutConfig, &dout[0]);
    AfxAssert(dout[0]);
    AfxReconnectDrawOutput(dout[0], dctx);
#endif
#ifdef ENABLE_DOUT2
    AfxOpenDrawOutputs(0, 1, &doutConfig, &dout[1]);
    AfxAssert(dout[1]);
    AfxReconnectDrawOutput(dout[1], dctx);
#endif
#ifdef ENABLE_DOUT3
    AfxOpenDrawOutputs(0, 1, &doutConfig, &dout[2]);
    AfxAssert(dout[2]);
    AfxReconnectDrawOutput(dout[2], dctx);
#endif

    AfxClearUri(&uri.uri);

    afxDrawInputConfig dinConfig = { 0 };
    dinConfig.prefetch = (void*)DrawInputProc;
    dinConfig.udd = &bnk;
    dinConfig.cmdPoolMemStock = 4096;
    dinConfig.estimatedSubmissionCnt = 2;

#ifdef ENABLE_DIN1
    AfxOpenDrawInputs(0, 1, &dinConfig, &din[0]);
    AfxAssert(din[0]);
    AfxReconnectDrawInput(din[0], dctx);
#endif
#ifdef ENABLE_DIN2
    AfxOpenDrawInputs(0, 1, &dinConfig, &din[1]);
    AfxAssert(din[1]);
    AfxReconnectDrawInput(din[1], dctx);
#endif
#ifdef ENABLE_DIN3
    AfxOpenDrawInputs(0, 1, &dinConfig, &din[2]);
    AfxAssert(din[2]);
    AfxReconnectDrawInput(din[2], dctx);
#endif

    //BinkSoundUseDirectSound(0);
    err = AfxBinkDeploy(&bnk, dctx);
    AfxAssert(!err);
#if 0
    AfxFormatUri(&uri.uri, "art/fmv/t2.bik");
#else
    AfxFormatUri(&uri.uri, "art/fmv/ubi.bik");
    //AfxFormatUri(&uri.uri, "art/fmv/disco.bik");
#endif
    
    err = AfxBinkOpen(&bnk, &uri.uri);
    AfxAssert(!err);

#ifdef ENABLE_DOUT1
    AfxReadjustDrawOutput(dout[0], bnk.whd);
#endif
#ifdef ENABLE_DOUT2
    AfxReadjustDrawOutput(dout[1], bnk.whd);
#endif
#ifdef ENABLE_DOUT3
    AfxReadjustDrawOutput(dout[2], bnk.whd);
#endif

    AfxEnableDrawInputPrefetching(din[0], TRUE);



    afxUri uri2;
    AfxMakeUri(&uri2, "art/vcr-mono-font-256.tga", 0);
    AfxLoadRastersFromTarga(dctx, afxRasterUsage_SAMPLING, NIL, 1, &uri2, &fontRas);
    AfxMakeUri(&uri2, "art/vcr-mono-font-256-left.tga", 0);
    AfxLoadRastersFromTarga(dctx, afxRasterUsage_SAMPLING, NIL, 1, &uri2, &fontRas2);
   
    afxNat visCnt = 1;
    afxVertexInputStream vis[1];
    vis[0].instanceRate = 1;
    vis[0].slotIdx = 0;
    vis[0].stride = 0;
    AfxMakeUri(&uri2, "data/pipeline/font.xsh.xml?instanced", 0);
    fontPip = AfxLoadPipelineFromXsh(dctx, &uri2);

    afxBufferSpecification vboSpec = { 0 };
    vboSpec.access = afxBufferAccess_W;
    vboSpec.usage = afxBufferUsage_VERTEX;
    vboSpec.src = NIL;
    vboSpec.siz = ((16 * 16) * 16);
    AfxAcquireBuffers(dctx, 1, &vboSpec, &textVbo);

    afxSamplerConfig smpCnf;
    AfxDescribeDefaultSampler(&smpCnf);
    smpCnf.minFilter = afxTexelFilter_LINEAR;
    smpCnf.uvw[0] = afxTexelAddress_CLAMP;
    smpCnf.uvw[1] = afxTexelAddress_CLAMP;
    smpCnf.uvw[2] = afxTexelAddress_CLAMP;
    AfxAcquireSamplers(dctx, 1, &smpCnf, &fontSmp);

    afxBufferSpecification bufSpec;
    bufSpec.siz = sizeof(awxViewConstants);
    bufSpec.usage = afxBufferUsage_UNIFORM;
    bufSpec.access = afxBufferAccess_W;
    bufSpec.src = NIL;
    AfxAcquireBuffers(dctx, 1, &bufSpec, &viewConstantsBuffer);

    bnk.running |= TRUE;
    return AFX_SUCCESS;
}

_AFXEXPORT afxError AppProc(afxApplication app, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;

    switch (opcode)
    {
    case AFX_THR_OPCODE_RUN:
    {
        Once(app);
        break;
    }
    case AFX_THR_OPCODE_QUIT:
    {
        AfxEntry("app=%p", app);

        AfxEcho("aaaaaaaaaaaa");

#ifdef ENABLE_DIN1
        AfxReleaseObjects(1, (void*[]) { din[0] });
#endif
#ifdef ENABLE_DIN2
        din[1]->process = NIL;
#endif
#ifdef ENABLE_DIN3
        din[2]->process = NIL;
#endif

        AfxBinkDrop(&bnk);
        break;
    }
    default:
    {
        afxError err = AFX_ERR_NONE;
        //AfxFormatUri(&uri, "?text=timer %u, timer^2 %u", AfxGetTimer(), AfxGetTimer() * AfxGetTimer());

        AfxBinkDoFrame(&bnk, TRUE, TRUE);
        //AfxBinkDoFrame(&bnk, TRUE, TRUE);
        //AfxBinkBlitFrame(&bnk);
        //AfxBinkDoFrame(&bnk, TRUE, TRUE);
        break;
    }
    }
    return err;
}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxFixedUri2048 romUri;
    AfxMakeFixedUri2048(&romUri, NIL);
    AfxFormatUri(&romUri.uri, "%s", argv[0]); // hardcoded name

    afxBool reboot = 1;
    while (reboot)
    {
        afxSystemConfig sysCfg;
        afxPlatformConfig winCfg;
        afxDrawSystemConfig dsysCfg;
        AfxChooseSystemConfiguration(&sysCfg, &winCfg);
        AfxChooseDrawSystemConfiguration(&dsysCfg);
        sysCfg.platform = &winCfg;
        sysCfg.draw = &dsysCfg;
        sysCfg.sound = TRUE;
        AfxBootUpSystem(&sysCfg);
        AfxAssertObjects(1, (void*[]) { AfxGetSystem() }, afxFcc_SYS);

        afxSoundContext sctx;
        afxSoundContextConfig sctxCfg = { 0 };
        AfxAcquireSoundContexts(0, 1, &sctxCfg, &sctx);
        AfxAssertObjects(1, &sctx, afxFcc_SCTX);

        afxDrawContextConfig dctxCfg = { 0 };        
        AfxAcquireDrawContexts(0, 1, &dctxCfg, &dctx);
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxApplication TheApp;
        afxApplicationConfig appCfg = { 0 };
        appCfg.argc = argc;
        appCfg.argv = argv;
        appCfg.dctx = dctx;
        appCfg.sctx = sctx;
        appCfg.proc = AppProc;
        AfxAcquireApplications(1, &appCfg, &TheApp);
        AfxAssertObjects(1, &TheApp, afxFcc_APP);
        AfxRunApplication(TheApp);

        while (AfxSystemIsExecuting())
            AfxDoSystemExecution(0);

        AfxReleaseObjects(1, (void*[]) { TheApp });
        
        AfxReleaseObjects(1, (void*[]) { dctx });

        AfxShutdownSystem(0);
    }
    Sleep(3000);
    return 0;
}
