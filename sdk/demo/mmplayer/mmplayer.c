#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "afx/core/afxSystem.h"
#include "afx/core/afxDebug.h"

#include "afx/sim/afxNode.h"

#include "afx/draw/afxDrawSystem.h"
#include "afx/afxQwadro.h"
#include "../src/e2bink/afxBinkVideo.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/afxBitmap.h"

#define ENABLE_DIN1 // 
#define ENABLE_DOUT1

//#define ENABLE_DIN2 // yuv
//#define ENABLE_DOUT2

//#define ENABLE_DIN3 // rgb
//#define ENABLE_DOUT3

afxSystem sys;
afxDrawSystem dsys;

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

_AFXEXPORT afxError DrawInputProc(afxDrawInput din, afxNat thrUnitIdx) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    afxBinkVideo *bnk = AfxGetDrawInputUdd(din);

    if (bnk->running)
    {
        afxDrawContext dctx;
        AfxGetDrawInputConnection(din, &dctx);
        afxNat unitIdx;
        AfxGetThreadingUnit(&unitIdx);

        afxDrawScript dscr;

        if (AfxAcquireDrawScripts(din, 0, 1, &dscr)) AfxThrowError();
        else
        {
            if (AfxRecordDrawScript(dscr, afxDrawScriptUsage_ONCE)) AfxThrowError();
            else
            {
                afxRaster surf;
                afxNat outBufIdx = 0;
                AfxRequestDrawOutputBuffer(dout[0], 0, &outBufIdx);
                AfxGetDrawOutputBuffer(dout[0], outBufIdx, &surf);
                AfxAssertObjects(1, &surf, afxFcc_RAS);
                
                //AfxBinkDoFrame(&bnk, TRUE, TRUE, outBufIdx, dscr, canv, NIL);
                //AfxBinkDoFrame(bnk, TRUE, TRUE);
                //AfxBinkBlitFrame(bnk2, dscr, canv[0][outBufIdx], NIL);
                //AfxBinkDoFrame(bnk, TRUE, TRUE, 0, 0, NIL);
                AfxBinkBlitFrame(bnk, surf, TRUE, FALSE, dscr);

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

_AFXEXPORT afxResult AfxUpdateApplication(afxThread thr, afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    //AfxFormatUri(&uri, "?text=timer %u, timer^2 %u", AfxGetTimer(), AfxGetTimer() * AfxGetTimer());
    
    AfxBinkDoFrame(&bnk, TRUE, TRUE);
    //AfxBinkDoFrame(&bnk, TRUE, TRUE);
    //AfxBinkBlitFrame(&bnk);
    //AfxBinkDoFrame(&bnk, TRUE, TRUE);
    return err;
}

_AFXEXPORT afxResult AfxEnterApplication(afxThread thr, afxApplication app)
{
    afxError err = AFX_ERR_NONE;

    AfxEntry("app=%p", app);
    AfxUri2048(&uri);

    AfxFormatUri(&uri.uri, "art/world.tga");
    
    if (AfxLoadRastersFromTarga(dctx, NIL, 1, &uri.uri, &dumpImg))
        AfxThrowError();

    AfxAssert(dumpImg);
    AfxFormatUri(&uri.uri, "tmp/world2.tga");
    AfxPrintRaster(dumpImg, 0, 0, 1, &uri.uri);
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
    doutConfig.pixelFmt = AFX_PFD_RGBA8;
    doutConfig.presentMode = afxPresentMode_LIFO;
    doutConfig.presentTransform = NIL;
    doutConfig.bufUsage = AFX_TEX_FLAG_SURFACE_RASTER;
#endif
    doutConfig.pixelFmt = AFX_PFD_RGB8;
    //doutConfig.colorSpc = afxColorSpace_SRGB;
    
#ifdef ENABLE_DOUT1
    AfxOpenDrawOutputs(dsys, 0, 1, &doutConfig, dout);
    AfxAssert(dout[0]);
    AfxReconnectDrawOutput(dout[0], dctx);

    //afxResult rslt = AfxBuildDrawOutputCanvases(dout[0], 0, AfxGetDrawOutputCapacity(dout[0]), NIL, NIL, canv[0]);
    //AfxAssert(!rslt);

#endif
#ifdef ENABLE_DOUT2
    doutConfig.presentMode = afxPresentMode_IMMEDIATE;
    dout[1] = AfxOpenDrawOutputs(dctx, &doutConfig, &uri);
    AfxAssert(dout[1]);

    surfSpec.fmt = doutConfig.pixelFmt;
    surfSpec.usage = doutConfig.bufUsage;

    for (afxNat i = 0; i < doutConfig.bufCnt; i++)
    {
        surfSpec.surf = AfxGetDrawOutputBuffer(dout[1], i);
        canv[1][i] = AfxBuildCanvases(dctx, dout[1]->whd, 1, &surfSpec);
    }
#endif
#ifdef ENABLE_DOUT3
    doutConfig.presentMode = afxPresentMode_FIFO;
    dout[2] = AfxOpenDrawOutputs(dctx, &doutConfig, &uri);
    AfxAssert(dout[2]);

    surfSpec.fmt = doutConfig.pixelFmt;
    surfSpec.usage = doutConfig.bufUsage;

    for (afxNat i = 0; i < doutConfig.bufCnt; i++)
    {
        surfSpec.surf = AfxGetDrawOutputBuffer(dout[2], i);
        canv[2][i] = AfxBuildCanvases(dctx, dout[2]->whd, 1, &surfSpec);
    }
#endif

    afxDrawInputConfig dinConfig = { 0 };
    dinConfig.prefetch = (void*)DrawInputProc;
    dinConfig.udd = &bnk;
    dinConfig.cmdPoolMemStock = 4096;
    dinConfig.estimatedSubmissionCnt = 2;

#ifdef ENABLE_DIN1
    AfxClearUri(&uri.uri);
    AfxOpenDrawInputs(dsys, 0, 1, &dinConfig, din);
    AfxAssert(din[0]);
    AfxReconnectDrawInput(din[0], dctx);

#endif
#ifdef ENABLE_DIN2
    din[1] = AfxOpenDrawInputs(&dinConfig);
    AfxAssert(din[1]);
#endif
#ifdef ENABLE_DIN3
    din[2] = AfxOpenDrawInputs(&dinConfig);
    AfxAssert(din[2]);
#endif

    //BinkSoundUseDirectSound(0);
    err = AfxBinkDeploy(&bnk, dctx);
    AfxAssert(!err);
#if !0
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

    bnk.running |= TRUE;
    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxLeaveApplication(afxThread thr, afxApplication app)
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

    return AFX_SUCCESS;
}

_AFXEXPORT afxError MmplayerThrProc(afxThread thr, afxApplication app, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;

    switch (opcode)
    {
        //case 0: break;
    case AFX_THR_OPCODE_RUN: AfxEnterApplication(thr, app); break;
        //case AFX_THR_OPCODE_QUIT: AfxEndApplication(thr, app); break;
    default: AfxUpdateApplication(thr, app); break;
    }

    return err;
}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxUri2048 romUri;
    AfxUri2048(&romUri);
    AfxFormatUri(&romUri.uri, "%s", argv[0]); // hardcoded name

    afxBool reboot = 1;
    while (reboot)
    {
        afxSystemConfigWin32 sysConfig;
        AfxChooseBasicIoSystemConfiguration(&sysConfig.base, sizeof(sysConfig));
        AfxBootUpBasicIoSystem(&sysConfig.base, &sys);
        AfxAssertObjects(1, &sys, afxFcc_SYS);

#if 0
        afxContext thrs[10];
        AfxAcquireContexts(10, thrs, NIL, AfxHint());
        AfxReleaseObjects(10, thrs);
        AfxAcquireContexts(10, thrs, NIL, AfxHint());
        AfxReleaseObjects(10, thrs);
        AfxAcquireContexts(10, thrs, NIL, AfxHint());
        AfxReleaseObjects(10, thrs);
        AfxAcquireContexts(10, thrs, NIL, AfxHint());
        AfxReleaseObjects(10, thrs);
#endif

        afxDrawSystemConfig dsysConfig;
        AfxChooseDrawSystemConfiguration(&dsysConfig, sizeof(dsysConfig));
        AfxAcquireDrawSystems(1, &dsysConfig, &dsys);
        AfxAssertObjects(1, &dsys, afxFcc_DSYS);

        afxDrawDevice ddev;
        AfxGetDrawDevice(dsys, 0, &ddev);
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        afxDrawContextConfig dctxConfig = { 0 };        
        AfxAcquireDrawContexts(dsys, 0, 1, &dctxConfig, &dctx);
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);

        afxApplication TheApp;
        afxApplicationConfig appConfig = { 0 };
        appConfig.argc = argc;
        appConfig.argv = argv;
        appConfig.dctx = dctx;
        appConfig.proc = MmplayerThrProc;
        AfxAcquireApplications(1, &TheApp, &appConfig);
        AfxAssertObjects(1, &TheApp, afxFcc_APP);
        AfxRunApplication(TheApp);

        while (AfxSystemIsExecuting())
            AfxDoSystemThreading(0);

        AfxReleaseObjects(1, (void*[]) { TheApp });
        
        AfxReleaseObjects(1, (void*[]) { dctx });

        AfxReleaseObjects(1, (void*[]) { dsys });
        AfxShutdownBasicIoSystem(0);
    }
    Sleep(3000);
    return 0;
}
