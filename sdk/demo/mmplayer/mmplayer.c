#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "afx/core/afxSystem.h"
#include "afx/core/diag/afxDebug.h"

#include "afx/sim/afxNode.h"

#include "afx/draw/afxDrawSystem.h"
#include "afx/afxQwadro.h"
#include "../src/e2bink/afxBinkVideo.h"
#include "afx/draw/afxDrawSystem.h"
#include "afx/core/mem/afxBitmap.h"

#define ENABLE_DIN1 // 
#define ENABLE_DOUT1

//#define ENABLE_DIN2 // yuv
//#define ENABLE_DOUT2

//#define ENABLE_DIN3 // rgb
//#define ENABLE_DOUT3

afxSimulation sim = NIL;
afxTexture dumpImg = NIL;

afxDrawOutput dout[3] = { NIL, NIL, NIL };
afxDrawInput din[3] = { NIL, NIL, NIL };
afxDrawContext dctx = NIL;

//afxCanvas canv[3][60] = { NIL };

afxUri2048 uri, uri2;

afxVertexBuffer vbo = NIL;
afxBuffer ubo = NIL;
afxPipeline dpip[2] = { NIL, NIL };
afxTexture tex[4] = { NIL, NIL, NIL, NIL };

afxBinkVideo bnk = { 0 };

_AFXEXPORT afxError DinFetcherFn(afxDrawInput din, afxDrawThread dthr) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    afxBinkVideo *bnk2 = &bnk;
    afxDrawContext dctx;
    AfxGetConnectedDrawInputContext(din, &dctx);
    afxDrawScript dscr;
    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);
    AfxAcquireDrawScripts(din, unitIdx, 1, &dscr);
    //return 0;

    if (!dscr) AfxThrowError();
    else
    {
        if (AfxBeginDrawScript(dscr, NIL)) AfxThrowError();
        else
        {
            afxNat outBufIdx = 0;
            AfxRequestDrawOutputBuffer(dout[0], 0, &outBufIdx);
            afxCanvas canv;
            AfxGetDrawOutputCanvas(dout[0], outBufIdx, &canv);

            //if (bnk.set.bink_buffers.FrameNum == outBufIdx)
            {
                //AfxBinkDoFrame(&bnk, TRUE, TRUE, outBufIdx, dscr, canv, NIL);
                //AfxBinkDoFrame(bnk, TRUE, TRUE);
                //AfxBinkBlitFrame(bnk2, dscr, canv[0][outBufIdx], NIL);
                AfxBinkBlitFrame(&bnk, dscr, canv, NIL);
                //glCopyImage2D
            }
            
            //AfxDoSystemThreading(0);

            if (AfxEndDrawScript(dscr)) AfxThrowError();
            else
            {
                if (AfxSubmitDrawScripts(din, 1, &dscr)) // draw output count hardcoded to 1.
                    AfxThrowError();

                //if (qid == 0)
                    if (AfxSubmitPresentations(din, 1, dout, &outBufIdx)) // draw output count hardcoded to 1.
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
    
    AfxBinkDoFrame(&bnk, TRUE, TRUE, 0, 0, 0, NIL);
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
    
    if (AfxAcquireTextures(dctx, 1, &uri.uri, &dumpImg))
        AfxThrowError();

    AfxAssert(dumpImg);
    AfxFormatUri(&uri.uri, "tmp/world2.tga");
    AfxPrintTexture(dumpImg, 0, 0, AfxGetTextureImageCount(dumpImg), &uri.uri);
    AfxReleaseObject(&dumpImg->obj);

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
    doutConfig.presentMode = AFX_PRESENT_MODE_LIFO;
    doutConfig.presentTransform = NIL;
    doutConfig.bufUsage = AFX_TEX_FLAG_SURFACE_RASTER;
#endif
    doutConfig.pixelFmt = AFX_PFD_RGBA4;
    
#ifdef ENABLE_DOUT1
    AfxAcquireDrawOutputs(1, dout, &doutConfig);
    AfxAssert(dout[0]);
    AfxReconnectDrawOutput(dout[0], dctx, NIL);

    //afxResult rslt = AfxBuildDrawOutputCanvases(dout[0], 0, AfxGetDrawOutputCapacity(dout[0]), NIL, NIL, canv[0]);
    //AfxAssert(!rslt);

#endif
#ifdef ENABLE_DOUT2
    doutConfig.presentMode = AFX_PRESENT_MODE_IMMEDIATE;
    dout[1] = AfxAcquireDrawOutputs(dctx, &doutConfig, &uri);
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
    doutConfig.presentMode = AFX_PRESENT_MODE_FIFO;
    dout[2] = AfxAcquireDrawOutputs(dctx, &doutConfig, &uri);
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
    dinConfig.prefetch = (void*)DinFetcherFn;
    dinConfig.udd = &bnk;
    dinConfig.cmdPoolMemStock = 4096;
    dinConfig.estimatedSubmissionCnt = 2;

#ifdef ENABLE_DIN1
    AfxEraseUri(&uri.uri);
    AfxAcquireDrawInputs(1, din, &dinConfig);
    AfxAssert(din[0]);
    AfxReconnectDrawInput(din[0], dctx, NIL);

#endif
#ifdef ENABLE_DIN2
    din[1] = AfxAcquireDrawInputs(&dinConfig);
    AfxAssert(din[1]);
#endif
#ifdef ENABLE_DIN3
    din[2] = AfxAcquireDrawInputs(&dinConfig);
    AfxAssert(din[2]);
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
    
    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxLeaveApplication(afxThread thr, afxApplication app)
{
    AfxEntry("app=%p", app);

    AfxEcho("aaaaaaaaaaaa");
    
#ifdef ENABLE_DIN1
    AfxReleaseDrawInputs(1, din);
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
        afxSystem sys;
        afxSystemConfigWin32 sysConfig;
        AfxChooseBasicIoSystemConfiguration(&sysConfig.base, sizeof(sysConfig));
        AfxBootUpBasicIoSystem(&sysConfig.base, &sys);
        AfxAssertObjects(1, &sys, AFX_FCC_SYS);

        afxDrawSystem dsys;
        afxDrawSystemConfig dsysConfig;
        AfxChooseDrawSystemConfiguration(&dsysConfig, sizeof(dsysConfig));
        AfxEstablishDrawSystem(&dsysConfig, &dsys);
        AfxAssertObjects(1, &dsys, AFX_FCC_DSYS);

        afxDrawContextConfig dctxConfig = { 0 };        
        AfxAcquireDrawContexts(1, &dctx, &dctxConfig);
        //AfxAssertType(dctxD, AFX_FCC_DCTX);

        afxApplication TheApp;
        afxApplicationConfig appConfig = { 0 };
        appConfig.argc = argc;
        appConfig.argv = argv;
        appConfig.dctx = dctx;
        appConfig.proc = MmplayerThrProc;
        AfxAcquireApplications(1, &TheApp, &appConfig);
        AfxAssertObjects(1, &TheApp, AFX_FCC_APP);
        AfxRunApplication(TheApp);

        while (AfxSystemIsOperating())
            AfxDoSystemThreading(0);

        AfxReleaseApplications(1, &TheApp);
        
        AfxReleaseDrawContexts(1, &dctx);

        AfxAbolishDrawSystem();
        AfxShutdownBasicIoSystem(0);
    }
    Sleep(3000);
    return 0;
}
