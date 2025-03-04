#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/inc/afxQwadro.h"

afxSimulation sim = NIL;

afxBool readyToRender = FALSE;
afxMixSystem ssys;
afxWindow window;
afxSession ses = NIL;
afxDrawOutput dout = NIL;
afxDrawSystem dsys = NIL;
afxDrawInput din = NIL;

void *vg = NIL;

void UpdateFrameMovement(afxReal64 DeltaTime)
{
    afxError err = AFX_ERR_NONE;
}

int main(int argc, char const* argv[])
{
    afxError err = AFX_ERR_NONE;

    // Boot up the Qwadro (if necessary)

    afxSystemConfig sysc;
    AfxConfigureSystem(&sysc);
    AfxBootstrapSystem(&sysc);

    // Acquire hardware device contexts

    afxUnit ddevId = 0;
    afxDrawSystemConfig dsyc;
    AfxConfigureDrawSystem(ddevId, &dsyc);
    AfxEstablishDrawSystem(ddevId, &dsyc, &dsys);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Open a session

    afxSessionConfig scfg = { 0 };
    AfxAcquireSession(0, &scfg, &ses);
    AFX_ASSERT_OBJECTS(afxFcc_SES, 1, &ses);
    AfxOpenSession(ses, NIL, NIL, NIL);

    // Acquire a drawable surface

    afxWindowConfig wrc;
    wrc.dsys = dsys;
    AfxConfigureWindow(&wrc, NIL, AFX_V3D(0.5, 0.5, 1));
    AfxAcquireWindow(&wrc, &window);
    //AfxAdjustWindowFromNdc(window, NIL, AFX_V3D(0.5, 0.5, 1));
    AfxGetWindowDrawOutput(window, NIL, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    // Acquire a draw input mechanism

    afxDrawInput din;
    afxDrawInputConfig dinCfg;
    AfxConfigureDrawInput(dsys, &dinCfg);
    AfxOpenDrawInput(dsys, &dinCfg, &din);
    AFX_ASSERT_OBJECTS(afxFcc_DIN, 1, &din);

    afxWidget wid;
    afxWidgetConfig widc = { 0 };
    AfxAcquireWidgets(window, 1, &widc, &wid);
    

    // Run

    afxClock startClock, lastClock;
    AfxGetClock(&startClock);
    lastClock = startClock;

    readyToRender = TRUE;

    while (AfxSystemIsExecuting())
    {
        AfxPollInput(NIL, 0);

        afxReal64 ct, dt;
        //AfxStepWindow(window, &ct, &dt);
        {
            afxClock currClock;
            AfxGetClock(&currClock);
            ct = AfxGetSecondsElapsed(&startClock, &currClock);
            dt = AfxGetSecondsElapsed(&lastClock, &currClock);
            lastClock = currClock;
        }

        UpdateFrameMovement(dt);

        if (!AfxSystemIsExecuting())
            break;

        if (!readyToRender)
            continue;

        afxUnit outBufIdx = 0;

        if (AfxRequestDrawOutputBuffer(dout, 0, &outBufIdx))
            continue;

        afxDrawContext dctx;
        afxUnit queIdx = 0;
        afxUnit portId = 0;

        if (AfxAcquireDrawContexts(dsys, portId, queIdx, 1, &dctx))
        {
            AfxThrowError();
            AfxRecycleDrawOutputBuffer(dout, outBufIdx);
            continue;
        }

        avxCanvas canv;
        avxRange canvWhd;
        AfxGetDrawOutputCanvas(dout, outBufIdx, &canv);
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);
        canvWhd = AfxGetCanvasExtent(canv);

        {
            avxDrawTarget rdt = { 0 };
            rdt.clearValue.rgba[0] = 0.3f;
            rdt.clearValue.rgba[1] = 0.1f;
            rdt.clearValue.rgba[2] = 0.3f;
            rdt.clearValue.rgba[3] = 1;
            rdt.loadOp = avxLoadOp_CLEAR;
            rdt.storeOp = avxStoreOp_STORE;
            avxDrawTarget ddt = { 0 };
            ddt.clearValue.depth = 1.0;
            ddt.clearValue.stencil = 0;
            ddt.loadOp = avxLoadOp_CLEAR;
            ddt.storeOp = avxStoreOp_STORE;

            avxDrawScope dps = { 0 };
            dps.canv = canv;
            dps.layerCnt = 1;
            dps.targetCnt = 1;
            dps.targets = &rdt;
            //dps.depth = &ddt;
            //dps.stencil = &ddt;

            dps.area = AVX_RECT(0, 0, canvWhd.w, canvWhd.h);
            AvxCmdCommenceDrawScope(dctx, &dps);

            AfxResetWidget(wid);
            AfxDoWidgetInput(wid);
            AfxTestWidget(wid);
            AfxRedrawWidgets(window, dctx);

            //TestSvg(vg, dctx, canvWhd);
            AvxCmdConcludeDrawScope(dctx);
        }

        afxSemaphore dscrCompleteSem = NIL;

        if (AfxCompileDrawCommands(dctx))
        {
            AfxThrowError();
            AfxRecycleDrawOutputBuffer(dout, outBufIdx);
            continue;
        }

        avxSubmission subm = { 0 };

        if (AfxExecuteDrawCommands(dsys, &subm, 1, &dctx, NIL))
        {
            AfxThrowError();
            AfxRecycleDrawOutputBuffer(dout, outBufIdx);
            continue;
        }

        AfxWaitForDrawQueue(dsys, subm.exuIdx, subm.baseQueIdx, 0);

        //AfxStampDrawOutputBuffers(1, &req, AFX_V2D(200, 200), &AfxString("Test"), 738);

        avxPresentation pres = { 0 };

        if (AfxPresentDrawOutputs(dsys, &pres, NIL, 1, &dout, &outBufIdx, NIL))
        {
            AfxThrowError();
            AfxRecycleDrawOutputBuffer(dout, outBufIdx);
            continue;
        }
    }

    AfxDisposeObjects(1, &window);

    AfxDisposeObjects(1, &dsys);

    AfxDoSystemShutdown(0);
    Sleep(3000);
    return 0;
}
