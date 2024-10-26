#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/inc/afxQwadro.h"

#include "../../../../dep_/vgl/vgl.h"
afxSimulation sim = NIL;

afxBool readyToRender = FALSE;
afxSoundContext sctx;
afxWindow window;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
afxDrawInput din = NIL;

void *vg = NIL;

afxBool DrawInputProc(afxDrawInput din, avxEvent const* ev) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    switch (ev->id)
    {
    default:
    {
        akxRenderer rnd = AfxGetDrawInputUdd(din);
        afxDrawContext dctx;
        AfxGetDrawInputContext(din, &dctx);

        if (readyToRender)
        {
            afxUnit outBufIdx = 0;

            if (!AfxRequestDrawOutputBuffer(dout, 0, &outBufIdx))
            {
                avxCmdb cmdb;
                afxUnit queIdx = AFX_INVALID_INDEX;
                afxUnit portId = 0;

                if (AfxOpenDrawBatches(dctx, portId, 1, &cmdb)) AfxThrowError();
                else
                {
                    queIdx = AfxGetCmdBufferPool(cmdb);

                    afxWhd canvWhd;
                    avxCanvas canv;
                    AfxGetDrawOutputCanvas(dout, outBufIdx, &canv);
                    AfxAssertObjects(1, &canv, afxFcc_CANV);                        
                    canvWhd = AfxGetCanvasExtent(canv);

                    avxDrawTarget dt[2] = { 0 };
                    AfxV4dSet(dt[0].clearValue.rgba, 0.3, 0.3, 0.3, 1.0);
                    dt[1].clearValue.depth = 1.0;
                    dt[1].clearValue.stencil = 0;
                    avxDrawScope sync = { 0 };
                    sync.area.w = canvWhd.w;
                    sync.area.h = canvWhd.h;
                    sync.canv = canv;
                    sync.rasterCnt = 1;
                    sync.rasters = &dt[0];
                    sync.depth = &dt[1];
                    AvxCmdCommenceDrawScope(cmdb, &sync);

                    afxViewport vp = { 0 };
                    vp.offset[0] = sync.area.x;
                    vp.offset[1] = sync.area.y;
                    vp.extent[0] = sync.area.w;
                    vp.extent[1] = sync.area.h;
                    vp.depth[1] = 1.0f;
                    AvxCmdAdjustViewports(cmdb, 0, 1, &vp);

                    TestSvg(vg, cmdb, canvWhd);

                    AvxCmdConcludeDrawScope(cmdb);

                    afxSemaphore dscrCompleteSem = NIL;

                    if (AfxCloseDrawBatches(1, &cmdb)) AfxThrowError();
                    else
                    {
                        avxSubmission subm = { 0 };

                        if (AfxSubmitDrawCommands(dctx, &subm, 1, &cmdb))
                            AfxThrowError();

                        AfxWaitForDrawQueue(dctx, subm.exuIdx, queIdx);
                    }
                }

                afxSemaphore dscrCompleteSem = NIL;

                //AfxStampDrawOutputBuffers(1, &req, AfxV2d(200, 200), &AfxString("Test"), 738);

                avxPresentation pres = { 0 };

                if (AfxPresentDrawOutputBuffers(dctx, &pres, 1, &dout, &outBufIdx))
                    AfxThrowError();

            }
        }
        break;
    }
    }
    return FALSE;
}

void UpdateFrameMovement(afxReal64 DeltaTime)
{
    afxError err = AFX_ERR_NONE;

}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;
    afxResult rslt = AFX_SUCCESS, opcode = AFX_OPCODE_CONTINUE;

    //afxUri2048 romUri;
    //AfxMakeUri2048(&romUri, NIL);
    //AfxFormatUri(&romUri.uri, "%s", argv[0]); // hardcoded name

    // Boot up the Qwadro

    afxSystemConfig sysCfg;
    AfxConfigureSystem(&sysCfg);
    AfxDoSystemBootUp(&sysCfg);
    
    afxSession ses;
    afxSessionConfig scfg = { 0 };
    AfxAcquireSession(0, &scfg, &ses);

    // Acquire hardware device contexts
#if 0
    afxSoundContextConfig sctxCfg = { 0 };
    AfxOpenSoundDevice(0, &sctxCfg, &sctx);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);
#endif
    afxUnit ddevId = 0;
    afxDrawContextConfig dctxCfg = { 0 };
    AfxOpenDrawDevice(ddevId, &dctxCfg, &dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    // Acquire a drawable surface

    afxWindowConfig wrc = { 0 };
    wrc.ddevId = ddevId;
    wrc.surface.pixelFmt = avxFormat_RGBA8;
    wrc.surface.pixelFmtDs[0] = avxFormat_D32f;
    wrc.surface.minBufCnt = 2;
    AfxAcquireWindow(ses, &wrc, &window);
    AfxAssertObjects(1, &window, afxFcc_WND);
    AfxAdjustWindowFromNdc(window, NIL, AfxV2d(0.5, 0.5));

    AfxGetWindowDrawOutput(window, NIL, &dout);
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxReconnectDrawOutput(dout, dctx);

    // Acquire a draw operator

    afxDrawInputConfig dinConfig = { 0 };
    //dinConfig.proc = DrawInputProc;
    AfxOpenDrawInput(ddevId, &dinConfig, &din);
    AFX_ASSERT(din);
    AfxReconnectDrawInput(din, dctx);

    // Acquire a simulation

    akxSimulationConfig simSpec = { 0 };
    AfxAabbSet(&simSpec.extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    simSpec.dctx = dctx;
    simSpec.din = din;
    simSpec.unitsPerMeter = 1.f;
    AfxV3dSet(simSpec.right, 1, 0, 0);
    AfxV3dSet(simSpec.up, 0, 1, 0);
    AfxV3dSet(simSpec.back, 0, 0, 1);
    AfxV3dZero(simSpec.origin);
    AfxAcquireSimulations(0, &simSpec, &sim);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    // Load startup scripts

    afxUri uri;
    AfxMakeUri(&uri, 0, "system/engine.xss", 0);
    //AfxLoadScript(NIL, &uri);

    // Run

    readyToRender = TRUE;

    vg = AvxAcquireGraphic(dctx, 1);

    while (AfxSystemIsExecuting())
    {
        AfxPollInput();

        afxReal64 ct, dt;
        AfxStepWindow(window, &ct, &dt);
        UpdateFrameMovement(dt);

        DrawInputProc(din, NIL);
    }

    AvxReleaseGraphic(vg);

    AfxReleaseObjects(1, &window);

    AfxReleaseObjects(1, &dctx);

    AfxDoSystemShutdown(0);
    Sleep(3000);
    return 0;
}
