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
    afxError err = NIL;

    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxEnterApplication(afxApplication app)
{
    afxError err = NIL;
    AfxEntry("app=%p", app);

    afxUri uriMap;
    AfxUriMapConstData(&uriMap, "e2newton.icd", 0);
    afxSimulationSpecification simSpec = { 0 };
    simSpec.bounding = NIL;
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    sim = AfxApplicationAcquireSimulation(TheApp, &simSpec);
    AfxAssertObject(sim, AFX_FCC_SIM);

    AfxUriMapConstData(&uriMap, "window", 0);
    afxDrawOutputSpecification doutSpec = { 0 };
    doutSpec.endpoint = &uriMap;
    doutSpec.bufCnt = 3;
    doutSpec.clipped = TRUE;
    doutSpec.colorSpc = NIL;
    doutSpec.compositeAlpha = FALSE;
    doutSpec.pixelFmt = AFX_PIXEL_FMT_RGBA8;
    doutSpec.presentMode = AFX_PRESENT_MODE_FIFO;
    doutSpec.presentTransform = NIL;
    doutSpec.bufUsage = AFX_TEX_USAGE_RASTER_BUFFER;
    afxWhd extent = { 720, 480, 1 };

    dout = AfxDrawContextAcquireOutput(dctx, extent, &doutSpec);
    AfxAssert(dout);

    afxDrawInputSpecification dinSpec = { 0 };
    dinSpec.prefetch = (void*)NIL;
    dinSpec.udd = NIL;
    dinSpec.cmdPoolMemStock = 4096;
    dinSpec.estimatedSubmissionCnt = 3;
    dinSpec.enabledPresentationThreads = (afxNat[]) { 1, 0, 0, 0 };
    dinSpec.enabledStreamingThreads = (afxNat[]) { 1, 1, 1, 1 };
    dinSpec.enabledSubmissionThreads = (afxNat[]) { 1, 1, 1, 1 };
    din = AfxDrawContextAcquireInput(dctx, &dinSpec);
    AfxAssert(din);
    //AfxDrawInputAffinePrefetchThreads(din, 0, 1, (afxNat[]) { 1 });

    afxResult rslt;
    return AFX_SUCCESS;
}

_AFXEXPORT afxResult AfxLeaveApplication(afxApplication app)
{
    AfxEntry("app=%p", app);
    AfxObjectRelease(&din->obj);
    AfxEcho("aaaaaaaaaaaa");

    return AFX_SUCCESS;
}

int AfxMain(afxApplication app, int argc, char const* argv[])
{

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

        afxDrawContextSpecification dctxSpec;
        dctxSpec.driverId = 0;
        dctxSpec.queueCnt = 1;

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
