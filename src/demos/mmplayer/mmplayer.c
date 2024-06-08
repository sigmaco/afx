#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include "qwadro/afxQwadro.h"
#include "../src/bink1/afxBinkVideo.h"

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

        //if (bnk->running)
        {
            afxNat outBufIdx = 0;
            
            if (!AfxLockDrawOutputBuffer(dout[0], 0, &outBufIdx))
            {
                afxDrawContext dctx;
                AfxGetDrawInputContext(din, &dctx);

                afxNat queIdx;
                afxNat portIdx = 0;
                avxCmdb cmdb;

                if (AfxAcquireCmdBuffers(din, avxCmdbUsage_ONCE, 1, &cmdb)) AfxThrowError();
                else
                {
                    afxCanvas canv;
                    AfxEnumerateDrawOutputCanvases(dout[0], outBufIdx, 1, &canv);
                    //afxRaster surf = AfxEnumerateDrawOutputBuffers(dout[0], outBufIdx);
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
                    AvxCmdBeginSynthesis(cmdb, &dps);

                    afxViewport vp = { 0 };
                    vp.extent[0] = extent[0];
                    vp.extent[1] = extent[1];
                    vp.depth[0] = (afxReal)0;
                    vp.depth[1] = (afxReal)1;
                    AvxCmdAdjustViewports(cmdb, 0, 1, &vp);

#if 0
                    afxPipelineRasterizerState ras = { 0 };
                    ras.cullMode = afxCullMode_BACK;
                    ras.fillMode = afxFillMode_SOLID;
                    ras.frontFace = afxFrontFace_CCW;
                    ras.lineWidth = 1.f;
                    AvxCmdSetRasterizerState(cmdb, &ras);
#endif
                    // turn off Z buffering, culling, and projection (since we are drawing orthographically)
                    //afxPipelineDepthState const depth = { 0 };
                    //AvxCmdSetDepthState(cmdb, &depth);

                    //AfxBinkDoFrame(&bnk, TRUE, TRUE, outBufIdx, cmdb, canv, NIL);
                    //AfxBinkDoFrame(bnk, TRUE, TRUE);
                    //AfxBinkBlitFrame(bnk2, cmdb, canv[0][outBufIdx], NIL);
                    //AfxBinkDoFrame(bnk, TRUE, TRUE, 0, 0, NIL);
                    AfxBinkBlitFrame(bnk, cmdb);

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

                    if (AfxCompileCmdBuffers(1, &cmdb)) AfxThrowError();
                    else
                    {
                        afxExecutionRequest execReq = { 0 };
                        execReq.cmdb = cmdb;
                        execReq.signal = dscrCompleteSem;

                        if (AFX_INVALID_INDEX == AfxExecuteCmdBuffers(din, 1, &execReq, NIL))
                            AfxThrowError();
                    }
                }

                afxSemaphore dscrCompleteSem = NIL;

                //AfxStampDrawOutputBuffers(1, &req, AfxV2d(100, 100), &AfxString("Qwadro Execution Ecosystem\nSIGMA_GL/2"), 738);

                if (AFX_INVALID_INDEX == AfxPresentDrawOutputBuffer(dout[0], outBufIdx, portIdx, dscrCompleteSem))
                    AfxThrowError();
            }
        }
        break;
    }
    }
    return FALSE;
}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxUri2048 uriB;
    AfxMakeUri2048(&uriB, NIL);
    AfxFormatUri(&uriB.uri, "%s", argv[0]); // hardcoded name

    afxSystemConfig sysCfg;
    AfxChooseSystemConfiguration(&sysCfg);
    AfxDoSystemBootUp(&sysCfg);

    afxSoundContext sctx;
    afxSoundContextConfig sctxCfg = { 0 };
    AfxAcquireSoundContext(0, &sctxCfg, &sctx);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    afxDrawContextConfig dctxCfg = { 0 };        
    AfxAcquireDrawContext(0, &dctxCfg, &dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

#if 0
    afxDrawOutputConfig doutCfg = { 0 };
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
    doutCfg.pixelFmt = afxPixelFormat_RGB8;
    //doutConfig.colorSpc = afxColorSpace_SRGB;
    AfxOpenDrawOutput(0, NIL, &doutCfg, &dout[0]);
    AfxAssertObjects(1, &dout[0], afxFcc_DOUT);
    AfxReconnectDrawOutput(dout[0], dctx);
#endif

    afxDrawInputConfig dinCfg = { 0 };
    dinCfg.proc = DrawInputProc;
    dinCfg.udd = &bnk;
    dinCfg.cmdPoolMemStock = 4096;
    dinCfg.estimatedSubmissionCnt = 2;
    AfxOpenDrawInput(0, NIL, &dinCfg, &din[0]);
    AfxAssertObjects(1, &din[0], afxFcc_DIN);
    AfxReconnectDrawInput(din[0], dctx);

    afxUri uri;
    AfxMakeUri(&uri, "system/mmplayer.xss", 0);
    AfxLoadScript(NIL, &uri);

    AfxFormatUri(&uriB.uri, "art/world.tga");

    if (AfxLoadRasters(dctx, NIL, NIL, 1, &uriB.uri, &dumpImg))
        AfxThrowError();

    AfxAssert(dumpImg);
    AfxFormatUri(&uriB.uri, "tmp/world2.tga");
    AfxPrintRaster(dumpImg, NIL, 1, &uriB.uri);
    AfxReleaseObjects(1, &dumpImg);

    AfxFormatUri(&uriB.uri, "desktop");

    AfxClearUri(&uriB.uri);

    //BinkSoundUseDirectSound(0);

    bnk.running |= TRUE;

    err = AfxSetUpBinkPlayer(&bnk, dctx);
    AfxAssert(!err);
#if 0
    AfxFormatUri(&uriB.uri, "art/fmv/t2.bik");
#else
    AfxFormatUri(&uriB.uri, "art/fmv/ubi.bik");
    //AfxFormatUri(&uri.uri, "art/fmv/disco.bik");
#endif

    err = AfxOpenVideoBink(&bnk, &uriB.uri);
    AfxAssert(!err);

    afxWindow wnd;
    afxRect rc = { 0 };
    rc.w = bnk.whd[0];
    rc.h = bnk.whd[1];
    AfxAcquireWindow(0, dctx, &rc, &wnd);
    AfxAssertObjects(1, &wnd, afxFcc_WND);
    AfxGetSurfaceDrawOutput(wnd, &dout[0]);
    AfxAssert(dout[0]);
    AfxReconnectDrawOutput(dout[0], dctx);
    AfxAdjustWindowSurface(wnd, &rc);

    //AfxAdjustDrawOutput(dout[0], bnk.whd);

    while (AfxSystemIsExecuting())
    {
        AfxBinkDoFrame(&bnk, TRUE, TRUE);
        DrawInputProc(din[0], (afxDrawEvent[]) {0});
        //AfxRedrawWindow(ovy);
        AfxDoSystemExecution(0);
    }

    AfxDropVideoBink(&bnk);

    AfxReleaseObjects(1, &dctx);

    AfxDoSystemShutdown(0);
    Sleep(3000);
    return 0;
}
