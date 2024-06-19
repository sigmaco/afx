/*
 *             :::::::::::     :::     :::::::::   ::::::::      :::
 *                 :+:       :+: :+:   :+:    :+: :+:    :+:   :+: :+:
 *                 +:+      +:+   +:+  +:+    +:+ +:+         +:+   +:+
 *                 +#+     +#++:++#++: +#++:++#:  :#:        +#++:++#++:
 *                 +#+     +#+     +#+ +#+    +#+ +#+   +#+# +#+     +#+
 *                 #+#     #+#     #+# #+#    #+# #+#    #+# #+#     #+#
 *                 ###     ###     ### ###    ###  ########  ###     ###
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#include "qwadro/io/afxXml.h"
#include "sgl.h"

#include "qwadro/draw/pipe/avxPipeline.h"

#include "qwadro/draw/afxDrawSystem.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/exec/afxSystem.h"

_SGL afxError _DpuBindAndSyncFenc(sglDpu* dpu, afxBool syncOnly, afxFence fenc)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;

    gl->FenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    return err;
}

_SGL afxError _SglWaitFenc(afxBool waitAll, afxNat64 timeout, afxNat cnt, afxFence const fences[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, fences, afxFcc_FENC);
    afxDrawContext dctx = (void*)AfxGetFenceContext(fences[0]);
    afxDrawDevice ddev = AfxGetDrawContextDevice(dctx);
    afxNat txuIdx = 0;
    glVmt const* gl = &ddev->idd->dpus[txuIdx].gl;

    afxClock startClock, currClock;
    AfxGetClock(&startClock);

    do
    {

        for (afxNat i = 0; i < cnt; i++)
        {
            afxFence fenc = fences[i];
            AfxAssertObjects(1, fenc, afxFcc_FENC);

            // To block all CPU operations until a sync object is signaled, you call this function:
            GLenum rslt = gl->ClientWaitSync(fenc->glHandle, GL_SYNC_FLUSH_COMMANDS_BIT, /*timeout*/0); _SglThrowErrorOccuried();

            /*
                This function will not return until one of two things happens: the sync​ object parameter becomes signaled, or a number of nanoseconds greater than or equal to the timeout​ parameter passes.
                If timeout​ is zero, the function will simply check to see if the sync object is signaled and return immediately.
                Note that the fact that timeout​ is in nanoseconds does not imply that this function has true nanosecond granularity in its timeout; you are only guaranteed that at least that much time will pass.
            */

            switch (rslt)
            {
            case GL_ALREADY_SIGNALED:
            {
                // the sync object was signaled before the function was called.
                fenc->m.signaled = TRUE;
                break;
            }
            case GL_TIMEOUT_EXPIRED:
            {
                // the sync object did not signal within the given timeout period.
                fenc->m.signaled = FALSE;
                break;
            }
            case GL_CONDITION_SATISFIED:
            {
                // the sync object was signaled within the given timeout period.
                fenc->m.signaled = TRUE;
                break;
            }
            case GL_WAIT_FAILED:
            default:
            {
                // If an OpenGL Error occurred, then GL_WAIT_FAILED will be returned in addition to raising an error.
                fenc->m.signaled = FALSE;
                break;
            }
            }

            if (fenc->m.signaled && !waitAll)
                break;
        }

        /*
            There is another function for waiting on sync objects:

            void glWaitSync(GLsync sync, GLbitfield flags, GLuint64 timeout)

            Recall the discussion in the article on Synchronization about the difference between the GPU's command queue and the driver's internal command buffer.
            What glWaitSync does is prevent the driver from adding any commands to the GPU's command queue until this sync object is signaled.
            This function does not halt the CPU.

            The driver will still put commands in its internal buffer. But none of them will be seen by the GPU until this sync object is signaled.

            You need to ensure that the sync object is in the GPU's command queue.
            If you don't, then you may create an infinite loop.
            Since glWaitSync prevents the driver from adding any commands to the GPU command queue, this would include the sync object itself if it has not yet been added to the queue.
            This function does not take the GL_SYNC_FLUSH_COMMANDS_BIT, so you have to do it with a manual glFlush call.


        */

        AfxGetClock(&currClock);
    } while (timeout > AfxGetSecondsElapsed(&startClock, &currClock));
    return err;
}

_SGL afxError _SglResetFenc(afxNat cnt, afxFence const fences[])
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(cnt, fences, afxFcc_FENC);
    
    for (afxNat i = 0; i < cnt; i++)
    {
        afxFence fenc = fences[i];
        AfxAssertObjects(1, fenc, afxFcc_FENC);
        fenc->m.signaled = FALSE;
    }
    return err;
}

_SGL afxError _SglFencDtor(afxFence fenc)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fenc, afxFcc_FENC);
    afxDrawContext dctx = (void*)AfxGetFenceContext(fenc);

    if (fenc->glHandle)
    {
        _SglDctxDeleteGlRes(dctx, 9, fenc->glHandle);
        fenc->glHandle = 0;
    }

    if (_AfxFencStdImplementation.dtor(fenc))
        AfxThrowError();

    return err;
}

_SGL afxError _SglFencCtor(afxFence fenc, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &fenc, afxFcc_FENC);

    if (_AfxFencStdImplementation.ctor(fenc, cookie)) AfxThrowError();
    else
    {
        fenc->glHandle = 0;
        fenc->updFlags = SGL_UPD_FLAG_DEVICE_INST;

        if (_AfxFencStdImplementation.dtor(fenc))
            AfxThrowError();
    }
    return err;
}

_SGL afxClassConfig const _SglFencMgrCfg =
{
    .fcc = afxFcc_FENC,
    .name = "Fence",
    .desc = "Device Synchronization Fence",
    .fixedSiz = sizeof(AFX_OBJECT(afxFence)),
    .ctor = (void*)_SglFencCtor,
    .dtor = (void*)_SglFencDtor
};

_SGL afxError _SglSemDtor(afxSemaphore sem)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sem, afxFcc_SEM);

    if (_AfxSemStdImplementation.dtor(sem))
        AfxThrowError();

    return err;
}

_SGL afxError _SglSemCtor(afxSemaphore sem, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &sem, afxFcc_SEM);

    if (_AfxSemStdImplementation.ctor(sem, cookie)) AfxThrowError();
    else
    {

        if (_AfxSemStdImplementation.dtor(sem))
            AfxThrowError();
    }
    return err;
}

_SGL afxClassConfig const _SglSemMgrCfg =
{
    .fcc = afxFcc_SEM,
    .name = "Semaphore",
    .desc = "Device Synchronization Semaphore",
    .fixedSiz = sizeof(AFX_OBJECT(afxSemaphore)),
    .ctor = (void*)_SglSemCtor,
    .dtor = (void*)_SglSemDtor
};
