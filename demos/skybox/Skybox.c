#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include "qwadro/afxQwadro.h"
#include "qwadro/ux/afxApplication.h"
#include "qwadro/core/afxDebug.h"
#include "qwadro/math/afxMathDefs.h"

#define ENABLE_DRAW 1

#ifdef ENABLE_DRAW
#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/draw/afxCamera.h"
#include "qwadro/sim/rendering/awxRenderer.h"
#endif

const afxReal CameraSpeed = 30.0f;
afxSimulation sim = NIL;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
awxRenderer rnd = NIL;

afxCamera cam = NIL;

afxUri2048 uri;

afxError DrawInputProc(afxDrawInput din, afxNat thrUnitIdx) // called by draw thread
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &din, afxFcc_DIN);
    awxRenderer rnd = AfxGetDrawInputUdd(din);
    afxDrawContext dctx = AfxGetDrawInputContext(din);
    afxNat unitIdx;
    AfxGetThreadingUnit(&unitIdx);

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

            AwxCmdBeginSceneRendering(diob, rnd, rnd->activeCam, NIL, canv);
            
            AfxDrawSky(diob, &rnd->sky);

            AwxCmdEndSceneRendering(diob, rnd);

            if (AfxCompileDrawStream(diob)) AfxThrowError();
            else if (AfxExecuteDrawStreams(din, 1, &diob, NIL, NIL, NIL))
                AfxThrowError();

            if (AfxPresentDrawBuffers(din, 1, &dout, &outBufIdx, NIL))
                AfxThrowError();
        }
    }

    return err;
}

void UpdateFrameMovement(afxReal64 DeltaTime)
{
    afxError err = AFX_ERR_NONE;

    afxReal64 MovementThisFrame = DeltaTime * CameraSpeed;

    // Note: because the NegZ axis is forward, we have to invert the way you'd normally
    // think about the 'W' or 'S' key's action.
    afxReal64 ForwardSpeed = (AfxGetKeyPressure(0, afxKey_W) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_S) ? 1 : 0.0f);
    afxReal64 RightSpeed = (AfxGetKeyPressure(0, afxKey_A) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_D) ? 1 : 0.0f);
    afxReal64 UpSpeed = (AfxGetKeyPressure(0, afxKey_Q) ? -1 : 0.0f) + (AfxGetKeyPressure(0, afxKey_E) ? 1 : 0.0f);
    afxV3d v =
    {
        MovementThisFrame * RightSpeed,
        MovementThisFrame * UpSpeed,
        MovementThisFrame * ForwardSpeed
    };
    AfxApplyCameraMotion(cam, v);
}

_AFXEXPORT void Once(afxApplication app)
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("app=%p", app);

    afxString str, str2;
    afxUri uriMap;

    afxUri128 uri128;
    AfxMakeUri128(&uri128, NIL);
    afxStoragePointSpecification mpSpec = { 0 };
    AfxFormatUri(&uri128.uri, "art/mnt.zip");
    mpSpec.hostPath = &uri128.uri.str;
    AfxMakeUri(&uriMap, "art", 0);
    mpSpec.namespace = &uriMap;
    mpSpec.perm = afxIoFlag_R;
    afxStorage stop;
    afxResult rslt = AfxMountStorages(1, &mpSpec, &stop);
    AfxAssert(rslt == 1);
#if 0
    afxArchive arc;
    AfxAcquireArchives(1, &arc, &uri128.uri, (afxFileFlags[]) { afxFileFlag_R });
    AfxAssertObjects(1, &arc, afxFcc_ARC);
    afxUri itemNam;
    AfxMakeUri(&itemNam, "art/worldtest.tga", 0);
    afxNat itemIdx;
    AfxFindArchivedFile(arc, &itemNam, &itemIdx);
    afxStream item;
    AfxOpenArchivedFile(arc, itemIdx, &item);
    AfxReleaseStream(&item);
    AfxMakeUri(&itemNam, "tmp/worldtest.tga", 0);
    //AfxExtractArchivedFile(arc, itemIdx, &itemNam);
    AfxReleaseObjects(1, (void*[]) { arc });
#endif
    AfxMakeUri(&uriMap, "e2newton.icd", 0);
    awxSimulationConfig simSpec = { 0 };
    AfxRecomputeAabb(simSpec.extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.driver = &uriMap;
    simSpec.unitsPerMeter = 1.f;
    AfxSetV3d(simSpec.right, 1, 0, 0);
    AfxSetV3d(simSpec.up, 0, 1, 0);
    AfxSetV3d(simSpec.back, 0, 0, 1);
    AfxZeroV3d(simSpec.origin);
    AfxAcquireSimulations(1, &simSpec, &sim);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    AfxMakeUri(&uriMap, "window", 0);
    afxDrawOutputConfig doutConfig = {0};
    doutConfig.pixelFmt = afxPixelFormat_RGB8_SRGB;
    doutConfig.auxDsFmt[0] = afxPixelFormat_D32F;
    doutConfig.endpoint = &uriMap;
    AfxOpenDrawOutputs(0, 1, &doutConfig, &dout);
    AfxAssertObjects(1, &dout, afxFcc_DOUT);
    AfxReconnectDrawOutput(dout, dctx);

    awxRendererConfig rndConf = { 0 };
    rndConf.dinProc = DrawInputProc;
    AwxAcquireRenderers(sim, 1, &rnd, &rndConf);
    
    AfxAcquireCameras(rnd->din, 1, &cam);
    AfxAssert(cam);

    rnd->activeCam = cam;

    afxMouse mse = AfxGetMouse(0);
    AfxInstallWatcher(mse, cam);
    
    //AfxEnableDrawInputPrefetching(rnd->din, TRUE); // bug: sem isso não desenha
    
}

_AFXEXPORT afxError SkyboxProc(afxApplication app, afxThreadOpcode opcode)
{
    afxError err = AFX_ERR_NONE;

    switch (opcode)
    {
    //case 0: break;
    case afxThreadEvent_RUN:
    {
        Once(app);
        break;
    }
    case afxThreadEvent_QUIT:
    {
        break;
    }
    default:
    {
        afxReal64 dt;
        AfxGetThreadTime(NIL, &dt);
        UpdateFrameMovement(dt);
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
    while (reboot)
    {
        afxSystemConfig sysCfg;
        afxPlatformConfig winCfg;
        afxDrawSystemConfig dsysCfg;
        AfxChooseSystemConfiguration(&sysCfg, &winCfg);
        AfxChooseDrawSystemConfiguration(&dsysCfg);
        sysCfg.platform = &winCfg;
        sysCfg.draw = &dsysCfg;
        AfxDoBootUp(&sysCfg);
        AfxAssertObjects(1, (void*[]) { AfxGetSystem() }, afxFcc_SYS);

        afxDrawDevice ddev = AfxGetDrawDevice(0);
        AfxAssertObjects(1, &ddev, afxFcc_DDEV);

        afxDrawContextConfig dctxCfg = { 0 };
        AfxAcquireDrawContexts(0, 1, &dctxCfg, &dctx);
        AfxAssertObjects(1, &dctx, afxFcc_DCTX);


        afxApplication TheApp;
        afxApplicationConfig appConfig = { 0 };
        appConfig.argc = argc;
        appConfig.argv = argv;
        appConfig.proc = SkyboxProc;
        appConfig.dctx = dctx;
        AfxAcquireApplications(1, &appConfig, &TheApp);
        AfxAssertObjects(1, &TheApp, afxFcc_APP);
        AfxRunApplication(TheApp);

        while (AfxSystemIsExecuting())
            AfxDoSystemExecution(0);

        AfxReleaseObjects(1, (void*[]) { TheApp });

        AfxReleaseObjects(1, (void*[]) { dctx });

        AfxDoShutdown(0);
    }
    Sleep(3000);
    return 0;
}
