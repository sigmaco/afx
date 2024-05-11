#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/afxQwadro.h"
#include "qwadro/sim/afxSimulation.h"

afxSimulation sim = NIL;

afxBool readyToRender = FALSE;
afxSoundContext sctx;
afxWindow window;
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
        afxDrawContext dctx;
        AfxGetDrawInputContext(din, &dctx);

        if (readyToRender)
        {
            afxNat outBufIdx = 0;

            if (!AfxLockDrawOutputBuffer(dout, 0, &outBufIdx))
            {
                afxDrawStream diob;
                afxNat queIdx = AFX_INVALID_INDEX;
                afxNat portIdx = 0;

                if (AfxAcquireDrawStreams(dctx, portIdx, &queIdx, 1, &diob)) AfxThrowError();
                else
                {
                    if (AfxRecordDrawStreams(afxDrawStreamUsage_ONCE, 1, &diob)) AfxThrowError();
                    else
                    {
                        afxRaster surf;
                        AfxEnumerateDrawOutputBuffers(dout, outBufIdx, 1, &surf);
                        afxCanvas canv;
                        AfxEnumerateDrawOutputCanvases(dout, outBufIdx, 1, &canv);
                        AfxAssertObjects(1, &surf, afxFcc_RAS);

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

                        void* ddge = AfxGetDrawBridge(dctx, portIdx);
                        AfxWaitForIdleDrawQueue(ddge, queIdx);
                    }
                }


                afxSemaphore dscrCompleteSem = NIL;

                //AfxStampDrawOutputBuffers(1, &req, AfxV2d(200, 200), &AfxString("Test"), 738);

                if (AFX_INVALID_INDEX == AfxPresentDrawOutputBuffer(dout, outBufIdx, portIdx, dscrCompleteSem))
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

    afxUri2048 romUri;
    AfxMakeUri2048(&romUri, NIL);
    AfxFormatUri(&romUri.uri, "%s", argv[0]); // hardcoded name

    // Boot up the Qwadro

    afxSystemConfig sysCfg;
    AfxChooseSystemConfiguration(&sysCfg);
    AfxDoSystemBootUp(&sysCfg);

    // Acquire hardware device contexts

    afxSoundContextConfig sctxCfg = { 0 };
    AfxAcquireSoundContext(0, &sctxCfg, &sctx);
    AfxAssertObjects(1, &sctx, afxFcc_SCTX);

    afxDrawContextConfig dctxCfg = { 0 };
    AfxAcquireDrawContext(0, &dctxCfg, &dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    // Acquire a drawable surface

    AfxAcquireWindow(dctx, NIL, &window);
    AfxAdjustWindowNdc(window, AfxSpawnV3d(0.5, 0.5, 1));

    AfxGetWindowDrawOutput(window, &dout);
    AfxAssert(dout);
    AfxReconnectDrawOutput(dout, dctx);

    // Acquire a draw operator

    afxDrawInputConfig dinConfig = { 0 };
    //dinConfig.proc = DrawInputProc;
    AfxAcquireDrawInput(0, &dinConfig, &din);
    AfxAssert(din);
    AfxReconnectDrawInput(din, dctx);

    // Acquire a simulation

    awxSimulationConfig simSpec = { 0 };
    AfxRecomputeAabb(simSpec.extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    simSpec.dctx = dctx;
    simSpec.sctx = sctx;
    simSpec.din = din;
    simSpec.unitsPerMeter = 1.f;
    AfxSetV3d(simSpec.right, 1, 0, 0);
    AfxSetV3d(simSpec.up, 0, 1, 0);
    AfxSetV3d(simSpec.back, 0, 0, 1);
    AfxZeroV3d(simSpec.origin);
    AfxAcquireSimulations(1, &simSpec, &sim);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    // Load startup scripts

    afxUri uri;
    AfxMakeUri(&uri, "system/engine.xss", 0);
    AfxLoadScript(NIL, &uri);

    // Run

    readyToRender = TRUE;

    while (AfxSystemIsExecuting())
    {
        AfxDoSystemExecution(0);
        DrawInputProc(din, NIL);

        afxReal64 ct, dt;
        AfxStepWindow(window, &ct, &dt);
        UpdateFrameMovement(dt);
    }

    AfxReleaseObjects(1, &window);

    AfxReleaseObjects(1, &dctx);

    AfxDoSystemShutdown(0);
    Sleep(3000);
    return 0;
}
