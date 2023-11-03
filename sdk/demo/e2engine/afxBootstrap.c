#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "afx/afxQwadro.h"

afxSystem sys = NIL;
afxDrawSystem dsys = NIL;
afxApplication TheApp = NIL;
afxSimulation sim = NIL;

afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
afxDrawInput din = NIL;

_AFXEXPORT afxResult AfxUpdateApplication(afxApplication app)
{
    afxError err = AFX_ERR_NONE;

    return err;
}

_AFXEXPORT afxResult AfxEnterApplication(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    afxUri uriMap;
    AfxUriWrapLiteral(&uriMap, "e2newton.icd", 0);
    afxSimulationConfig simSpec = { 0 };
    simSpec.bounding = NIL;
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    sim = AfxAcquireSimulations(TheApp, &simSpec);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    AfxUriWrapLiteral(&uriMap, "window", 0);
    afxDrawOutputConfig doutConfig = { 0 };
    doutConfig.endpoint = &uriMap;
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

    dout = AfxOpenDrawOutputs(dctx, &doutConfig);
    AfxAssert(dout);

    afxDrawInputConfig dinConfig = { 0 };
    dinConfig.prefetch = (void*)NIL;
    dinConfig.udd[0] = NIL;
    dinConfig.cmdPoolMemStock = 4096;
    dinConfig.estimatedSubmissionCnt = 3;
    dinConfig.enabledPresentationThreads = (afxNat[]) { 1, 0, 0, 0 };
    dinConfig.enabledStreamingThreads = (afxNat[]) { 1, 1, 1, 1 };
    dinConfig.enabledSubmissionThreads = (afxNat[]) { 1, 1, 1, 1 };
    din = AfxOpenDrawInputs(dctx, &dinConfig);
    AfxAssert(din);
    //AfxEnableDrawInputPrefetching(din, 0, 1, (afxNat[]) { 1 });

    afxResult rslt;
    return err;
}

_AFXEXPORT afxResult AfxLeaveApplication(afxApplication app)
{
    AfxEntry("app=%p", app);
    AfxReleaseObject(&dinD->obj);
    AfxEcho("aaaaaaaaaaaa");

    return err;
}

int AfxMain(afxApplication app, int argc, char const* argv[])
{

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
        sys = AfxBootUpBasicIoSystem(NIL);

        afxDrawSystemConfig dsysSpec = { 0 };
        dsys = AfxAcquireDrawSystems(&dsysSpec);
        AfxAssertType(dsys, afxFcc_DSYS);

        afxDrawContextConfig dctxConfig = { 0 };
        dctxConfig.queueCnt = 1;

        dctx = AfxAcquireDrawContexts(&dctxConfig);
        AfxAssert(dctx);

        afxApplicationConfig appConfig;
        appConfig.argc = argc;
        appConfig.argv = argv;
        appConfig.dctx = dctx;
        appConfig.enter = AfxEnterApplication;
        appConfig.exit = AfxLeaveApplication;
        appConfig.update = AfxUpdateApplication;
        TheApp = AfxAcquireApplication(&appConfig);
        AfxAssertObject(TheApp, afxFcc_APP);

        if (AFX_OPCODE_BREAK == AfxRunApplication(TheApp))
            reboot = 0;

        AfxReleaseObject(&TheApp->obj);

        AfxReleaseObject(&dctxD->obj);

        AfxAbolishDrawSystem();
        AfxShutdownBasicIoSystem();
    }
    Sleep(3000);
    return 0;
}
