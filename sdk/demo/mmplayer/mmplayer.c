#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "afx/core/afxSystem.h"
#include "afx/core/diag/afxDebug.h"

#include "afx/simul/dag/afxNode.h"

#include "afx/draw/afxDrawSystem.h"
#include "afx/afxQwadro.h"
#include "afx/mmux/afxBinkVideo.h"

#define ENABLE_DIN1 // 
#define ENABLE_DOUT1

//#define ENABLE_DIN2 // yuv
//#define ENABLE_DOUT2

//#define ENABLE_DIN3 // rgb
//#define ENABLE_DOUT3

afxSystem sys = NIL;
afxDrawSystem dsys = NIL;
afxApplication TheApp = NIL;
afxSimulation sim = NIL;
afxTexture dumpImg = NIL;

afxDrawOutput dout[3] = { NIL, NIL, NIL };
afxDrawInput din[3] = { NIL, NIL, NIL };
afxDrawContext dctx = NIL;

afxCanvas canv[3][3] = { NIL };

afxUri4096 uri, uri2;

afxVertexBuffer vbo = NIL;
afxBuffer ubo = NIL;
afxPipeline dpip[2] = { NIL, NIL };
afxTexture tex[4] = { NIL, NIL, NIL, NIL };

afxBinkVideo bnk = { 0 };

_AFXEXPORT afxError DinFetcherFn(afxDrawInput din, afxNat qid, afxBinkVideo *bnk) // called by draw context
{
    afxError err = NIL;
    
    afxDrawScript dscr = AfxDrawInputAcquireScript(din, 1);
    //return 0;

    if (!dscr) AfxThrowError();
    else
    {
        if (AfxDrawScriptBegin(dscr, NIL)) AfxThrowError();
        else
        {
            afxNat outBufIdx = 0;
            AfxDrawOutputRequestBuffer(dout[0], 0, &outBufIdx);

            //AfxBinkDoFrame(bnk, TRUE, TRUE);
            AfxBinkBlitFrame(bnk, dscr, NIL, AfxDrawOutputGetBuffer(dout[0], outBufIdx));
            //AfxBinkBlitFrame(bnk, dscr, NIL);
            //glCopyImage2D

            if (AfxDrawScriptEnd(dscr)) AfxThrowError();
            else
            {
                if (AfxDrawInputEnqueueSubmission(din, 1, &dscr)) // draw output count hardcoded to 1.
                    AfxThrowError();

                //if (qid == 0)
                    if (AfxDrawInputEnqueuePresentation(din, 1, dout, &outBufIdx)) // draw output count hardcoded to 1.
                        AfxThrowError();
            }
        }
    }
    return err;
}

_AFXEXPORT afxResult AfxUpdateApplication(afxApplication app)
{
    afxError err = NIL;
    //AfxUriFormat(&uri, "?text=timer %u, timer^2 %u", AfxGetTimer(), AfxGetTimer() * AfxGetTimer());
    
    //AfxBinkDoFrame(&bnk, TRUE, TRUE);
    //AfxBinkBlitFrame(&bnk);
    AfxBinkDoFrame(&bnk, TRUE, TRUE);
    return err;
}

_AFXEXPORT afxResult AfxEnterApplication(afxApplication app)
{
    afxError err = NIL;

    AfxEntry("app=%p", app);
    AfxUri4096(&uri, NIL);

    AfxUriFormat(&uri.uri, "art/world.tga");
    
    if (1 != AfxDrawContextFetchTextures(dctx, 1, &uri.uri, &dumpImg))
        AfxThrowError();

    AfxAssert(dumpImg);
    AfxUriFormat(&uri.uri, "system/world2.tga");
    AfxTextureDownload(dumpImg, &uri.uri);
    AfxObjectRelease(&dumpImg->res.obj);

    AfxUriFormat(&uri.uri, "window");
    afxDrawOutputSpecification doutSpec;
    doutSpec.endpoint = &uri.uri;
    doutSpec.bufCnt = 3;
    doutSpec.clipped = TRUE;
    doutSpec.colorSpc = NIL;
    doutSpec.compositeAlpha = FALSE;
    doutSpec.pixelFmt = AFX_PIXEL_FMT_RGBA8;
    doutSpec.presentMode = AFX_PRESENT_MODE_MAILBOX;
    doutSpec.presentTransform = NIL;
    doutSpec.bufUsage = AFX_TEX_USAGE_SURFACE_RASTER;
    afxWhd extent = { 1280, 720, 1 };

#ifdef ENABLE_DOUT1
    doutSpec.presentMode = AFX_PRESENT_MODE_FIFO;
    dout[0] = AfxDrawContextAcquireOutput(dctx, extent, &doutSpec);
    AfxAssert(dout[0]);

    afxSurfaceSpecification surfSpec[] =
    {
        { NIL, doutSpec.pixelFmt, doutSpec.bufUsage },
        { NIL, AFX_PIXEL_FMT_D24S8, AFX_TEX_USAGE_SURFACE_DEPTH },
    };

    for (afxNat i = 0; i < doutSpec.bufCnt; i++)
    {
        surfSpec[0].surf = AfxDrawOutputGetBuffer(dout[0], i);
        AfxDrawOutputGetExtent(dout[0], extent);
        canv[0][i] = AfxDrawContextAcquireCanvas(dctx, extent, 1, surfSpec);
    }
#endif
#ifdef ENABLE_DOUT2
    doutSpec.presentMode = AFX_PRESENT_MODE_IMMEDIATE;
    dout[1] = AfxDrawContextAcquireOutput(dctx, &doutSpec, &uri);
    AfxAssert(dout[1]);

    surfSpec.fmt = doutSpec.pixelFmt;
    surfSpec.usage = doutSpec.bufUsage;

    for (afxNat i = 0; i < doutSpec.bufCnt; i++)
    {
        surfSpec.surf = AfxDrawOutputGetBuffer(dout[1], i);
        canv[1][i] = AfxDrawContextAcquireCanvas(dctx, dout[1]->whd, 1, &surfSpec);
    }
#endif
#ifdef ENABLE_DOUT3
    doutSpec.presentMode = AFX_PRESENT_MODE_MAILBOX;
    dout[2] = AfxDrawContextAcquireOutput(dctx, &doutSpec, &uri);
    AfxAssert(dout[2]);

    surfSpec.fmt = doutSpec.pixelFmt;
    surfSpec.usage = doutSpec.bufUsage;

    for (afxNat i = 0; i < doutSpec.bufCnt; i++)
    {
        surfSpec.surf = AfxDrawOutputGetBuffer(dout[2], i);
        canv[2][i] = AfxDrawContextAcquireCanvas(dctx, dout[2]->whd, 1, &surfSpec);
    }
#endif

    afxDrawInputSpecification dinSpec;
    dinSpec.prefetch = (void*)DinFetcherFn;
    dinSpec.udd = &bnk;
    dinSpec.cmdPoolMemStock = 4096;
    dinSpec.estimatedSubmissionCnt = 2;
    dinSpec.enabledPresentationThreads = (afxNat[]) { 1, 0, 0, 0 };
    dinSpec.enabledStreamingThreads = (afxNat[]) { 1, 1, 1, 1 };
    dinSpec.enabledSubmissionThreads = (afxNat[]) { 1, 1, 1, 1 };

#ifdef ENABLE_DIN1
    AfxUriErase(&uri.uri);
    din[0] = AfxDrawContextAcquireInput(dctx, &dinSpec);
    AfxAssert(din[0]);

#endif
#ifdef ENABLE_DIN2
    din[1] = AfxDrawContextAcquireInput(dctx, &dinSpec);
    AfxAssert(din[1]);
#endif
#ifdef ENABLE_DIN3
    din[2] = AfxDrawContextAcquireInput(dctx, &dinSpec);
    AfxAssert(din[2]);
#endif

    //BinkSoundUseDirectSound(0);
    err = AfxBinkDeploy(&bnk, dctx);
    AfxAssert(!err);
#if !0
    AfxUriFormat(&uri.uri, "art/fmv/t2.bik");
#else
    AfxUriFormat(&uri.uri, "art/fmv/ubi.bik");
#endif
    
    err = AfxBinkOpen(&bnk, &uri.uri);
    AfxAssert(!err);

#ifdef ENABLE_DOUT1
    AfxDrawOutputSetExtent(dout[0], bnk.whd);
#endif
#ifdef ENABLE_DOUT2
    AfxDrawOutputSetExtent(dout[1], bnk.whd);
#endif
#ifdef ENABLE_DOUT3
    AfxDrawOutputSetExtent(dout[2], bnk.whd);
#endif

    AfxDrawInputAffinePrefetchThreads(din[0], 0, 1, (afxNat[]) { 1 });
    afxResult rslt;
    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxLeaveApplication(afxApplication app)
{
    AfxEntry("app=%p", app);

    AfxEcho("aaaaaaaaaaaa");
    
#ifdef ENABLE_DIN1
    AfxObjectRelease(&din[0]->obj);
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

int main(int argc, char const* argv[])
{
    afxError err = NIL;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    afxUri4096 romUri;
    AfxUri4096(&romUri, NIL);
    AfxUriFormat(&romUri.uri, "%s", argv[0]); // hardcoded name

    afxBool reboot = 1;
    while (reboot)
    {
        sys = AfxSystemBootUp(NIL);

        dsys = AfxSystemAcquireDrawSystem(sys);
        AfxAssertObject(dsys, AFX_FCC_DSYS);

        afxDrawContextSpecification dctxSpec = { 0 };
        dctxSpec.driverId = 0;
        dctxSpec.queueCnt = 1;
        dctxSpec.autonomousQueue = FALSE;

        dctx = AfxDrawSystemAcquireContext(dsys, &dctxSpec);
        AfxAssert(dctx);

        afxApplicationSpecification appSpec;
        appSpec.argc = argc;
        appSpec.argv = argv;
        appSpec.dctx = dctx;
        appSpec.enter = AfxEnterApplication;
        appSpec.exit = AfxLeaveApplication;
        appSpec.update = AfxUpdateApplication;
        TheApp = AfxSystemAcquireApplication(sys, &appSpec);
        AfxAssertObject(TheApp, AFX_FCC_APP);

        if (AFX_OPCODE_BREAK == AfxApplicationExecute(TheApp))
            reboot = 0;

        AfxObjectRelease(&TheApp->obj);
        
        AfxObjectRelease(&dctx->obj);

        AfxObjectRelease(&dsys->obj);

        AfxObjectRelease(&sys->obj);
    }
    Sleep(3000);
    return 0;
}
