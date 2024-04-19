#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/afxQwadro.h"
#include "qwadro/sim/afxSimulation.h"

afxSimulation sim = NIL;

afxSoundContext sctx;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
afxDrawInput din = NIL;

afxBool DrawInputProc(afxDrawInput din, afxDrawEvent const* ev) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);

    switch (ev->id)
    {
    default:
    {
        awxRenderer rnd = AfxGetDrawInputUdd(din);
        afxDrawContext dctx = AfxGetDrawInputContext(din);

        afxDrawStream diob;

        if (AfxAcquireDrawStreams(din, 0, 1, &diob)) AfxThrowError();
        else
        {
            if (AfxRecordDrawStream(diob, afxDrawStreamUsage_ONCE)) AfxThrowError();
            else
            {

                afxNat outBufIdx = 0;
                AfxLockDrawOutputBuffer(dout, 0, &outBufIdx);
                afxRaster surf;
                AfxEnumerateDrawOutputBuffers(dout, outBufIdx, 1, &surf);
                afxCanvas canv;
                AfxEnumerateDrawOutputCanvases(dout, outBufIdx, 1, &canv);
                AfxAssertObjects(1, &surf, afxFcc_RAS);

                //AwxCmdBeginSceneRendering(diob, rnd, rnd->activeCam, NIL, canv);



                //AwxCmdEndSceneRendering(diob, rnd);

                afxSemaphore dscrCompleteSem = NIL;

                if (AfxCompileDrawStream(diob)) AfxThrowError();
                else
                {
                    afxExecutionRequest execReq = { 0 };
                    execReq.diob = diob;
                    execReq.signal = dscrCompleteSem;

                    if (AfxExecuteDrawStreams(din, 1, &execReq, NIL))
                        AfxThrowError();
                }

                afxPresentationRequest req = { 0 };
                req.dout = dout;
                req.bufIdx = outBufIdx;
                req.wait = dscrCompleteSem;

                //AfxStampDrawOutputBuffers(1, &req, AfxV2d(200, 200), &AfxString("Test"), 738);

                if (AfxPresentDrawOutputBuffers(1, &req))
                    AfxThrowError();
            }
        }
    }
    }
    return err;
}

void UpdateFrameMovement(afxReal64 DeltaTime)
{
    afxError err = AFX_ERR_NONE;

}

_AFXEXPORT afxResult Once(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    afxDrawOutputConfig doutConfig = { 0 };
    doutConfig.pixelFmt = afxPixelFormat_RGB8;
    AfxOpenDrawOutputs(0, 1, &doutConfig, &dout);
    AfxAssert(dout);
    AfxReconnectDrawOutput(dout, dctx);

    afxDrawInputConfig dinConfig = { 0 };
    //dinConfig.proc = DrawInputProc;
    AfxOpenDrawInputs(0, 1, &dinConfig, &din);
    AfxAssert(din);
    AfxReconnectDrawInput(din, dctx);

    awxSimulationConfig simSpec = { 0 };
    AfxRecomputeAabb(simSpec.extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    simSpec.dctx = dctx;
    simSpec.sctx = sctx;
    simSpec.din = NIL;
    simSpec.unitsPerMeter = 1.f;
    AfxSetV3d(simSpec.right, 1, 0, 0);
    AfxSetV3d(simSpec.up, 0, 1, 0);
    AfxSetV3d(simSpec.back, 0, 0, 1);
    AfxZeroV3d(simSpec.origin);
    AfxAcquireSimulations(1, &simSpec, &sim);
    AfxAssertObjects(1, &sim, afxFcc_SIM);
    
    return err;
}

_AFXEXPORT afxError AppProc(afxApplication app, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;

    switch (opcode)
    {
    case afxThreadEvent_RUN:
    {
        Once(app);
        break;
    }
    case afxThreadEvent_QUIT:
    {
        AfxEntry("app=%p", app);
        break;
    }
    default:
    {

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

    while (!AfxDoBootUp(NIL))
    {
        AfxAssertObjects(1, (void*[]) { AfxGetSystem() }, afxFcc_SYS);

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
        AfxMakeUri(&uri, "system/engine.xss", 0);
        AfxRunApplication(TheApp, &uri);

        while (AfxSystemIsExecuting())
            if (AfxDoSystemExecution(0))
                reboot = 0;

        AfxDoShutdown(0);

        if (!reboot)
            break;
    }
    Sleep(3000);
    return 0;
}
