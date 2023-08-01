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

    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxEnterApplication(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    afxUri uriMap;
    AfxUriWrapLiteral(&uriMap, "e2newton.icd", 0);
    afxSimulationSpecification simSpec = { 0 };
    simSpec.bounding = NIL;
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    sim = AfxApplicationAcquireSimulation(TheApp, &simSpec);
    AfxAssertObject(sim, AFX_FCC_SIM);

    AfxUriWrapLiteral(&uriMap, "window", 0);
    afxDrawOutputSpecification doutSpec = { 0 };
    doutSpec.endpoint = &uriMap;
    doutSpec.whd[0] = 1280;
    doutSpec.whd[1] = 720;
    doutSpec.whd[2] = 1;
    doutSpec.bufCnt = 2;
    doutSpec.clipped = TRUE;
    doutSpec.colorSpc = NIL;
    doutSpec.presentAlpha = FALSE;
    doutSpec.pixelFmt = AFX_PIXEL_FMT_RGBA8;
    doutSpec.presentMode = AFX_PRESENT_MODE_LIFO;
    doutSpec.presentTransform = NIL;
    doutSpec.bufUsage = AFX_TEX_FLAG_SURFACE_RASTER;

    dout = AfxAcquireDrawOutputs(dctx, &doutSpec);
    AfxAssert(dout);

    afxDrawInputSpecification dinSpec = { 0 };
    dinSpec.prefetch = (void*)NIL;
    dinSpec.udd[0] = NIL;
    dinSpec.cmdPoolMemStock = 4096;
    dinSpec.estimatedSubmissionCnt = 3;
    dinSpec.enabledPresentationThreads = (afxNat[]) { 1, 0, 0, 0 };
    dinSpec.enabledStreamingThreads = (afxNat[]) { 1, 1, 1, 1 };
    dinSpec.enabledSubmissionThreads = (afxNat[]) { 1, 1, 1, 1 };
    din = AfxAcquireDrawInputs(dctx, &dinSpec);
    AfxAssert(din);
    //AfxEnableDrawInputPrefetching(din, 0, 1, (afxNat[]) { 1 });

    afxResult rslt;
    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxLeaveApplication(afxApplication app)
{
    AfxEntry("app=%p", app);
    AfxReleaseObject(&din->obj);
    AfxEcho("aaaaaaaaaaaa");

    return AFX_SUCCESS;
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
        sys = AfxBootUpSystem(NIL);

        afxDrawSystemSpecification dsysSpec = { 0 };
        dsys = AfxBootUpDrawSystem(&dsysSpec);
        AfxAssertObject(dsys, AFX_FCC_DSYS);

        afxDrawContextSpecification dctxSpec = { 0 };
        dctxSpec.queueCnt = 1;

        dctx = AfxAcquireDrawContexts(&dctxSpec);
        AfxAssert(dctx);

        afxApplicationSpecification appSpec;
        appSpec.argc = argc;
        appSpec.argv = argv;
        appSpec.dctx = dctx;
        appSpec.enter = AfxEnterApplication;
        appSpec.exit = AfxLeaveApplication;
        appSpec.update = AfxUpdateApplication;
        TheApp = AfxAcquireApplication(&appSpec);
        AfxAssertObject(TheApp, AFX_FCC_APP);

        if (AFX_OPCODE_BREAK == AfxRunApplication(TheApp))
            reboot = 0;

        AfxReleaseObject(&TheApp->obj);

        AfxReleaseObject(&dctx->obj);

        AfxShutdownDrawSystem();
        AfxShutdownSystem();
    }
    Sleep(3000);
    return 0;
}
