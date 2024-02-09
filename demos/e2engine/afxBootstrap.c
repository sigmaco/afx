#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/afxQwadro.h"

afxSimulation sim = NIL;

afxSoundContext sctx;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
afxDrawInput din = NIL;

afxError DrawInputProc(afxDrawInput din, afxNat thrUnitIdx) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    awxRenderer rnd = AfxGetDrawInputUdd(din);
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
            AfxRequestDrawOutputBuffer(dout, 0, &outBufIdx);
            afxRaster surf;
            AfxGetDrawOutputBuffer(dout, outBufIdx, 1, &surf);
            afxCanvas canv;
            AfxGetDrawOutputCanvas(dout, outBufIdx, 1, &canv);
            AfxAssertObjects(1, &surf, afxFcc_RAS);

            AwxCmdBeginSceneRendering(dscr, rnd, rnd->activeCam, NIL, canv);



            AwxCmdEndSceneRendering(dscr, rnd);

            if (AfxCompileDrawScript(dscr)) AfxThrowError();
            else if (AfxExecuteDrawScripts(din, 1, &dscr))
                AfxThrowError();

            if (AfxPresentDrawBuffers(din, 1, &dout, &outBufIdx))
                AfxThrowError();
        }
    }
    return err;
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
    dinConfig.prefetch = (void*)DrawInputProc;
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
    case AFX_THR_OPCODE_RUN:
    {
        Once(app);
        break;
    }
    case AFX_THR_OPCODE_QUIT:
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
