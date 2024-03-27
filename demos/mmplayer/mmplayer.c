#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/core/afxSystem.h"
#include "qwadro/core/afxDebug.h"

#include "qwadro/sim/awxNode.h"

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/sound/afxSoundSystem.h"
#include "qwadro/afxQwadro.h"
#include "../src/bink1/afxBinkVideo.h"
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/font/afxTypography.h"
#include "qwadro/draw/io/afxVertexStream.h"
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

afxUri2048 uri, uri2;

afxBuffer ubo = NIL;
afxPipeline dpip[2] = { NIL, NIL };
afxRaster tex[4] = { NIL, NIL, NIL, NIL };

afxBinkVideo bnk = { 0 };
afxBuffer viewConstantsBuffer = NIL;

afxBool DrawInputProc(afxDrawInput din, afxDrawEvent const* ev) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    switch (ev->id)
    {
    default:
    {
        afxBinkVideo *bnk = AfxGetDrawInputUdd(din);

        if (bnk->running)
        {
            afxDrawContext dctx = AfxGetDrawInputContext(din);
            afxNat unitIdx;
            AfxGetThreadingUnit(&unitIdx);

            afxDrawStream dscr;

            if (AfxAcquireDrawStreams(din, 0, 1, &dscr)) AfxThrowError();
            else
            {
                if (AfxRecordDrawStream(dscr, afxDrawStreamUsage_ONCE)) AfxThrowError();
                else
                {
                    afxNat outBufIdx = 0;
                    AfxRequestDrawOutputBuffer(dout[0], 0, &outBufIdx);
                    afxCanvas canv;
                    AfxGetDrawOutputCanvas(dout[0], outBufIdx, 1, &canv);
                    //afxRaster surf = AfxGetDrawOutputBuffer(dout[0], outBufIdx);
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
                        afxUri2048 uri;
                        AfxMakeUri2048(&uri);
                        AfxFormatUri(&uri.uri, "tmp/bink_frame_%u.tga", bnk->set.bink_buffers.FrameNum);
                        AfxPrintRasterToTarga(surf, 0, 0, 1, &uri);
                    }
#endif

                    afxSemaphore dscrCompleteSem = NIL;

                    if (AfxCompileDrawStream(dscr)) AfxThrowError();
                    else
                    {
                        afxExecutionRequest execReq = { 0 };
                        execReq.dscr = dscr;
                        execReq.signal = dscrCompleteSem;

                        if (AfxExecuteDrawStreams(din, 1, &execReq, NIL))
                            AfxThrowError();
                    }

                    afxPresentationRequest req = { 0 };
                    req.dout = dout[0];
                    req.bufIdx = outBufIdx;
                    req.wait = dscrCompleteSem;

                    //AfxStampDrawOutputBuffers(1, &req, AfxV2d(100, 100), &AfxString("Qwadro Execution Ecosystem\nSIGMA_GL/2"), 738);

                    if (AfxPresentDrawOutputBuffers(1, &req))
                        AfxThrowError();
                }
            }
        }
        break;
    }
    }
    return FALSE;
}

_AFXEXPORT afxResult Once(afxApplication app)
{
    afxError err = AFX_ERR_NONE;

    AfxEntry("app=%p", app);
    AfxMakeUri2048(&uri, NIL);

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
    dinConfig.proc = DrawInputProc;
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
    AfxAdjustDrawOutput(dout[0], bnk.whd);
#endif
#ifdef ENABLE_DOUT2
    AfxAdjustDrawOutput(dout[1], bnk.whd);
#endif
#ifdef ENABLE_DOUT3
    AfxAdjustDrawOutput(dout[2], bnk.whd);
#endif

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

    afxUri2048 romUri;
    AfxMakeUri2048(&romUri, NIL);
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
        sysCfg.sound = NIL;
        AfxDoSystemBootUp(&sysCfg);
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

        afxUri uri;
        AfxMakeUri(&uri, "system/mmplayer.xs", 0);
        AfxRunApplication(TheApp, &uri);

        while (AfxSystemIsExecuting())
            AfxDoSystemExecution(0);

        AfxReleaseObjects(1, (void*[]) { TheApp });
        
        AfxReleaseObjects(1, (void*[]) { dctx });

        AfxDoSystemShutdown(0);
    }
    Sleep(3000);
    return 0;
}
