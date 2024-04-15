#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AFX_UX_C
#define _AFX_OVERLAY_C
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

        //if (bnk->running)
        {
            afxNat outBufIdx = 0;
            
            if (!AfxLockDrawOutputBuffer(dout[0], 0, &outBufIdx))
            {
                afxDrawContext dctx;
                AfxGetDrawInputContext(din, &dctx);

                afxNat queIdx;
                afxNat portIdx = 0;
                afxDrawStream diob;

                if (AfxAcquireDrawStreams(dctx, portIdx, &queIdx, 1, &diob)) AfxThrowError();
                else
                {
                    if (AfxRecordDrawStreams(afxDrawStreamUsage_ONCE, 1, &diob)) AfxThrowError();
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
                        AfxCmdBeginSynthesis(diob, &dps);

                        afxViewport vp = { 0 };
                        vp.extent[0] = extent[0];
                        vp.extent[1] = extent[1];
                        vp.depth[0] = (afxReal)0;
                        vp.depth[1] = (afxReal)1;
                        AfxCmdResetViewports(diob, 1, &vp);

#if 0
                        afxPipelineRasterizerState ras = { 0 };
                        ras.cullMode = afxCullMode_BACK;
                        ras.fillMode = afxFillMode_SOLID;
                        ras.frontFace = afxFrontFace_CCW;
                        ras.lineWidth = 1.f;
                        AfxCmdSetRasterizerState(diob, &ras);
#endif
                        // turn off Z buffering, culling, and projection (since we are drawing orthographically)
                        //afxPipelineDepthState const depth = { 0 };
                        //AfxCmdSetDepthState(diob, &depth);

                        //AfxBinkDoFrame(&bnk, TRUE, TRUE, outBufIdx, diob, canv, NIL);
                        //AfxBinkDoFrame(bnk, TRUE, TRUE);
                        //AfxBinkBlitFrame(bnk2, diob, canv[0][outBufIdx], NIL);
                        //AfxBinkDoFrame(bnk, TRUE, TRUE, 0, 0, NIL);
                        AfxBinkBlitFrame(bnk, diob);

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

                        if (AfxCompileDrawStreams(1, &diob)) AfxThrowError();
                        else
                        {
                            afxExecutionRequest execReq = { 0 };
                            execReq.diob = diob;
                            execReq.signal = dscrCompleteSem;

                            if (AFX_INVALID_INDEX == AfxExecuteDrawStreams(din, 1, &execReq, NIL))
                                AfxThrowError();
                        }

                        

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

_AFXEXPORT afxResult Once(afxApplication app)
{
    afxError err = AFX_ERR_NONE;

    AfxMakeUri2048(&uri, NIL);

    AfxFormatUri(&uri.uri, "art/world.tga");
    
    if (AfxLoadRastersFromTarga(dctx, NIL, NIL, 1, &uri.uri, &dumpImg))
        AfxThrowError();

    AfxAssert(dumpImg);
    AfxFormatUri(&uri.uri, "tmp/world2.tga");
    AfxPrintRasterToTarga(dumpImg, NIL, &uri.uri);
    AfxReleaseObjects(1, (void*[]) { dumpImg });

    AfxFormatUri(&uri.uri, "desktop");
    
    AfxClearUri(&uri.uri);

    //BinkSoundUseDirectSound(0);
    
    bnk.running |= TRUE;
    return AFX_SUCCESS;
}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxUri2048 uriB;
    AfxMakeUri2048(&uriB, NIL);
    AfxFormatUri(&uriB.uri, "%s", argv[0]); // hardcoded name

    afxSystemConfig sysCfg;
    afxPlatformConfig winCfg;
    afxDrawSystemConfig dsysCfg;
    AfxChooseSystemConfiguration(&sysCfg, &winCfg);
    AfxChooseDrawSystemConfiguration(&dsysCfg);
    sysCfg.platform = &winCfg;
    sysCfg.draw = &dsysCfg;
    sysCfg.sound = NIL;
    AfxDoSystemBootUp(&sysCfg);

    afxSoundContext sctx;
    afxSoundContextConfig sctxCfg = { 0 };
    AfxAcquireSoundContexts(0, 1, &sctxCfg, &sctx);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    afxDrawContextConfig dctxCfg = { 0 };        
    AfxAcquireDrawContexts(0, 1, &dctxCfg, &dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

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
    AfxOpenDrawOutput(0, &doutCfg, NIL, &dout[0]);
    AfxAssertObjects(1, &dout[0], afxFcc_DOUT);
    AfxReconnectDrawOutput(dout[0], dctx);

    afxDrawInputConfig dinCfg = { 0 };
    dinCfg.proc = DrawInputProc;
    dinCfg.udd = &bnk;
    dinCfg.cmdPoolMemStock = 4096;
    dinCfg.estimatedSubmissionCnt = 2;
    AfxOpenDrawInput(0, &dinCfg, NIL, &din[0]);
    AfxAssertObjects(1, &din[0], afxFcc_DIN);
    AfxReconnectDrawInput(din[0], dctx);

    afxApplication TheApp;
    afxApplicationConfig appCfg = { 0 };
    appCfg.argc = argc;
    appCfg.argv = argv;
    appCfg.dctx = dctx;
    appCfg.sctx = sctx;
    AfxAcquireApplications(1, &appCfg, &TheApp);
    AfxAssertObjects(1, &TheApp, afxFcc_APP);

    afxUri uri;
    AfxMakeUri(&uri, "system/mmplayer.xss", 0);
    AfxRunApplication(TheApp, &uri);

    Once(TheApp);

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

    afxOverlay ovy;
    AfxAcquireOverlay(dctx, bnk.whd, &ovy);
    dout[0] = ovy->dout;

    //AfxAdjustDrawOutput(dout[0], bnk.whd);

    while (AfxSystemIsExecuting())
    {
        AfxBinkDoFrame(&bnk, TRUE, TRUE);
        DrawInputProc(din[0], (afxDrawEvent[]) {0});
        AfxDoOverlay(ovy);
        AfxDoSystemExecution(0);
    }

    AfxDropVideoBink(&bnk);

    AfxReleaseObjects(1, (void*[]) { TheApp });
        
    AfxReleaseObjects(1, (void*[]) { dctx });

    AfxDoSystemShutdown(0);
    Sleep(3000);
    return 0;
}
