#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "qwadro/afxQwadro.h"

arxSimulation sim = NIL;

afxBool readyToRender = FALSE;
afxMixSystem ssys;
afxWindow window;
afxEnvironment env = NIL;
afxSurface dout = NIL;
afxDrawSystem dsys = NIL;
arxDrawInput din = NIL;

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
    AvxConfigureDrawSystem(ddevId, &dsyc);
    AvxEstablishDrawSystem(ddevId, &dsyc, &dsys);
    AFX_ASSERT_OBJECTS(afxFcc_DSYS, 1, &dsys);

    // Open a session

    afxEnvironmentConfig ecfg = { 0 };
    AfxConfigureEnvironment(0, &ecfg);
    AfxAcquireEnvironment(0, &ecfg, &env);
    AFX_ASSERT_OBJECTS(afxFcc_ENV, 1, &env);
    AfxOpenEnvironment(env, NIL, NIL, NIL);

    // Acquire a drawable surface

    afxWindowConfig wrc;
    wrc.dsys = dsys;
    AfxConfigureWindow(env, &wrc, NIL, AFX_V3D(0.5, 0.5, 1));
    AfxAcquireWindow(env, &wrc, &window);
    //AfxAdjustWindowFromNdc(window, NIL, AFX_V3D(0.5, 0.5, 1));
    AfxGetWindowSurface(window, &dout);
    AFX_ASSERT_OBJECTS(afxFcc_DOUT, 1, &dout);

    // Acquire a draw input mechanism

    arxDrawInput din;
    arxRenderContextConfig dinCfg;
    AfxConfigureRenderContext(dsys, &dinCfg);
    ArxOpenRenderContext(dsys, &dinCfg, &din);
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
        AfxDoUx(NIL, AFX_TIMEOUT_INFINITE);

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

        if (AvxLockSurfaceBuffer(dout, AFX_TIMEOUT_NONE, NIL, &outBufIdx, NIL))
            continue;

        afxDrawContext dctx;
        afxUnit queIdx = 0;
        afxUnit portId = 0;

        if (AvxAcquireDrawContexts(dsys, portId, queIdx, 1, &dctx))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        avxCanvas canv;
        afxLayeredRect area;
        AvxGetSurfaceCanvas(dout, outBufIdx, &canv, &area);
        AFX_ASSERT_OBJECTS(afxFcc_CANV, 1, &canv);

        {
            avxDrawScope dps = { 0 };
            dps.canv = canv;
            dps.bounds = area;
            dps.targetCnt = 1;
            dps.targets[0].clearVal.rgba[0] = 0.3f;
            dps.targets[0].clearVal.rgba[1] = 0.1f;
            dps.targets[0].clearVal.rgba[2] = 0.3f;
            dps.targets[0].clearVal.rgba[3] = 1;
            dps.targets[0].loadOp = avxLoadOp_CLEAR;
            dps.targets[0].storeOp = avxStoreOp_STORE;
            dps.ds[0].clearVal.depth = 1.0;
            dps.ds[0].clearVal.stencil = 0;
            dps.ds[0].loadOp = avxLoadOp_CLEAR;
            dps.ds[0].storeOp = avxStoreOp_STORE;
            AvxCmdCommenceDrawScope(dctx, &dps);

            AfxResetWidget(wid);
            AfxDoWidgetInput(wid);
            AfxTestWidget(wid);
            AfxRedrawWidgets(window, &area.area, dctx);

            //TestSvg(vg, dctx, canvWhd);
            AvxCmdConcludeDrawScope(dctx);
        }

        afxSemaphore dscrCompleteSem = NIL;

        if (AvxCompileDrawCommands(dctx, batchId))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        avxSubmission subm = { 0 };
        subm.dctx = dctx;
        subm.batchId = batchId;
        if (AvxExecuteDrawCommands(dsys, 1, &subm))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }

        AvxWaitForDrawBridges(dsys, AFX_TIMEOUT_INFINITE, subm.exuMask);

        //AfxStampDrawOutputBuffers(1, &req, AFX_V2D(200, 200), &AfxString("Test"), 738);

        avxPresentation pres = { 0 };
        pres.dout = dout;
        pres.bufIdx = outBufIdx;
        if (AvxPresentSurfaces(dsys, 1, &pres))
        {
            AfxThrowError();
            AvxUnlockSurfaceBuffer(dout, outBufIdx);
            continue;
        }
    }

    AfxDisposeObjects(1, &window);

    AfxDisposeObjects(1, &dsys);

    AfxDoSystemShutdown(0);
    Sleep(3000);
    return 0;
}
