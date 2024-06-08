#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#include "qwadro/afxQwadro.h"
#include "qwadro/sim/rendering/akxRenderer.h"

#define ENABLE_DRAW 1

afxBool readyToRender = FALSE;
afxScript TheApp;
afxWindow window;
const afxReal CameraSpeed = 30.0f;
afxSimulation sim = NIL;
afxDrawOutput dout = NIL;
afxDrawContext dctx = NIL;
akxRenderer rnd = NIL;

afxCamera cam = NIL;

afxUri2048 uri;


afxBool DrawInputProc(afxDrawInput din, afxDrawEvent const* ev) // called by draw thread
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
            afxNat outBufIdx = 0;

            if (!AfxLockDrawOutputBuffer(dout, 0, &outBufIdx))
            {
                avxCmdb cmdb;
                afxNat queIdx = AFX_INVALID_INDEX;
                afxNat portIdx = 0;

                if (AfxAcquireCmdBuffers(din, avxCmdbUsage_ONCE, 1, &cmdb)) AfxThrowError();
                else
                {
                    queIdx = AfxGetCmdBufferPool(cmdb);

                    afxCanvas canv;
                    AfxEnumerateDrawOutputCanvases(dout, outBufIdx, 1, &canv);
                    AfxAssertObjects(1, &canv, afxFcc_CANV);

                    AkxCmdBeginSceneRendering(cmdb, rnd, rnd->activeCam, NIL, canv);

                    AfxDrawSky(cmdb, &rnd->sky);

                    AkxCmdEndSceneRendering(cmdb, rnd);

                    afxSemaphore dscrCompleteSem = NIL;

                    if (AfxCompileCmdBuffers(1, &cmdb)) AfxThrowError();
                    else
                    {
                        afxExecutionRequest execReq = { 0 };
                        execReq.cmdb = cmdb;
                        execReq.signal = dscrCompleteSem;

                        if (AFX_INVALID_INDEX == (queIdx = AfxExecuteCmdBuffers(din, 1, &execReq, NIL)))
                            AfxThrowError();
                    }

                    AfxWaitForIdleDrawQueue(dctx, portIdx, queIdx);
                }

                afxSemaphore dscrCompleteSem = NIL;

                //AfxStampDrawOutputBuffers(1, &req, AfxV2d(200, 200), &AfxString("Test"), 738);
                //AfxStampDrawOutputBuffer(dout, outBufIdx, portIdx, NIL, AfxV2d(200, 200), &AfxString("Test awgaw\nglhmawh\npanwhpinwanhawnpihnawpnwh\nipawnonhaiownoiawh\nnoiawhnawionhawoinhaow"));

                if (AFX_INVALID_INDEX == AfxPresentDrawOutputBuffer(dout, outBufIdx, portIdx, dscrCompleteSem))
                    AfxThrowError();
            }
        }
        break;
    }
    }
    return FALSE;
}

afxBool CamEventFilter(afxObject *obj, afxObject *watched, afxEvent *ev)
{
    afxError err = AFX_ERR_NONE;
    afxCamera cam = (void*)obj;
    AfxAssertObjects(1, &cam, afxFcc_CAM);
    (void)watched;
    (void)ev;

    switch (ev->id)
    {
    case afxUxEventId_AXIS:
    {
        //afxMouse mse = (void*)watched;
        //AfxAssertObjects(1, &mse, afxFcc_MSE);

        // TODO Leva isso para o usuário

        if (AfxLmbIsPressed(0))
        {
            afxV2d delta;
            afxV3d deltaEar;
            AfxGetMouseMotion(0, delta);
            deltaEar[1] = -((afxReal)(delta[0] * AFX_PI / 180.f));
            deltaEar[0] = -((afxReal)(delta[1] * AFX_PI / 180.f));
            deltaEar[2] = 0.f;

            AfxApplyCameraElevAzimRoll(cam, deltaEar);
        }

        if (AfxRmbIsPressed(0))
        {
            afxV2d delta;
            afxV3d off;
            AfxGetMouseMotion(0, delta);
            off[0] = -((afxReal)(delta[0] * AFX_PI / 180.f));
            off[1] = -((afxReal)(delta[1] * AFX_PI / 180.f));
            off[2] = 0.f;

            AfxApplyCameraOffset(cam, off);
        }
        break;
    }
    case afxUxEventId_WHEEL:
    {
        afxReal w = AfxGetMouseWheelDelta(0);
        w = w / 120.f; // WHEEL_DELTA
        AfxApplyCameraDistance(cam, w);
        break;
    }
    case afxUxEventId_KEY:
    {
        //afxKeyboard kbd = (void*)watched;
        //AfxAssertObjects(1, &kbd, afxFcc_KBD);
        break;
    }
    default:break;
    }
    return FALSE;
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

    afxDrawContextConfig dctxCfg = { 0 };
    AfxAcquireDrawContext(0, &dctxCfg, &dctx);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    // Acquire a drawable surface

    AfxAcquireWindow(0, dctx, NIL, &window);
    AfxAdjustWindowFromNdc(window, NIL, AfxSpawnV2d(0.5, 0.5));

    AfxGetSurfaceDrawOutput(window, &dout);
    AfxAssert(dout);
    AfxReconnectDrawOutput(dout, dctx);

    // Test mount Zip archive

#if 0

    afxUri uri, uri2;
    AfxMakeUri(&uri, "art", 0);
    AfxMakeUri(&uri2, "art/mnt.zip", 0);
    rslt = AfxMountStorageUnit(&uri, &uri2, afxFileFlag_R);
    AfxAssert(rslt == 1);

#endif

    // Test targa raster output

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

    // Acquire a simulation

    akxSimulationConfig simSpec = { 0 };
    AfxRecomputeAabb(simSpec.extent, 2, (afxV3d const[]) { { -1000, -1000, -1000 }, { 1000, 1000, 1000 } });
    simSpec.dctx = dctx;
    simSpec.din = NIL;
    simSpec.unitsPerMeter = 1.f;
    AfxSetV3d(simSpec.right, 1, 0, 0);
    AfxSetV3d(simSpec.up, 0, 1, 0);
    AfxSetV3d(simSpec.back, 0, 0, 1);
    AfxZeroV3d(simSpec.origin);
    AfxAcquireSimulations(1, &simSpec, &sim);
    AfxAssertObjects(1, &sim, afxFcc_SIM);

    // Acquire a simulation renderer

    akxRendererConfig rndConf = { 0 };
    rndConf.dinProc = DrawInputProc;
    AkxAcquireRenderers(sim, 1, &rnd, &rndConf);

    // Acquire a view point

    AfxAcquireCameras(rnd->din, 1, &cam);
    AfxAssert(cam);

    rnd->activeCam = cam;

    // Handle mouse-camera interaction

    afxHid hid;
    AfxGetHid(0, &hid);
    AfxInstallEventFilter(cam, (void*)CamEventFilter);
    AfxInstallWatcher(window, cam);

    // Run

    readyToRender = TRUE;

    while (AfxSystemIsExecuting())
    {
        AfxDoSystemExecution(0);
        DrawInputProc(rnd->din, NIL);

        afxReal64 ct, dt;
        AfxStepWindow(window, &ct, &dt);
        UpdateFrameMovement(dt);
    }

    Sleep(1000);

    AfxReleaseObjects(1, &window);

    AfxReleaseObjects(1, &dctx);

    AfxDoSystemShutdown(0);
    Sleep(3000);
    return 0;
}
