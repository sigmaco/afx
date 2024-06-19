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

_SGL void _DpuBeginQuery(sglDpu* dpu, avxQueryPool pool, afxNat queryIdx, afxBool precise)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->m.cap, queryIdx, 1);
    glVmt const* gl = &dpu->gl;

    gl->BeginQuery(pool->glTarget, pool->glHandle[queryIdx]);
}

_SGL void _DpuEndQuery(sglDpu* dpu, avxQueryPool pool, afxNat queryIdx)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->m.cap, queryIdx, 1);
    glVmt const* gl = &dpu->gl;

    gl->EndQuery(pool->glTarget);
}

_SGL void _DpuCopyQueryResults(sglDpu* dpu, avxQueryPool pool, afxNat baseQuery, afxNat queryCnt, afxBuffer buf, afxSize offset, afxSize stride, afxQueryResultFlags flags)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->m.cap, baseQuery, queryCnt);
    AfxAssertObjects(1, &buf, afxFcc_BUF);
    AfxAssertRange(AfxGetBufferCapacity(buf), offset, stride);
    glVmt const* gl = &dpu->gl;
    GLenum pname = (flags & afxQueryResultFlag_WAIT) ? GL_QUERY_RESULT : GL_QUERY_RESULT_NO_WAIT;

    //DpuBindAndSyncBuf(dpu, TRUE, TRUE, GL_QUERY_RESULT_BUFFER, );

    if (flags & afxQueryResultFlag_64)
    {
        for (afxNat i = 0; i < pool->m.cap; i++)
        {
            gl->GetQueryObjectui64v(pool->glHandle[i], pname, (GLuint64[]) { offset });

            if (flags & afxQueryResultFlag_WITH_AVAIL)
            {
                GLuint avail;
                gl->GetQueryObjectuiv(pool->glHandle[i], GL_QUERY_RESULT_AVAILABLE, &avail);
            }
        }
    }
    else
    {
        for (afxNat i = 0; i < pool->m.cap; i++)
        {
            gl->GetQueryObjectuiv(pool->glHandle[i], pname, (GLuint[]) { offset });

            if (flags & afxQueryResultFlag_WITH_AVAIL)
            {
                GLuint avail;
                gl->GetQueryObjectuiv(pool->glHandle[i], GL_QUERY_RESULT_AVAILABLE, &avail);
            }
        }
    }
}

_SGL void _DpuResetQueries(sglDpu* dpu, avxQueryPool pool, afxNat baseQuery, afxNat queryCnt)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->m.cap, baseQuery, queryCnt);
    glVmt const* gl = &dpu->gl;


}

_SGL void _DpuWriteTimestamp(sglDpu* dpu, avxQueryPool pool, afxNat queryIdx, avxPipelineStage stage)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &pool, afxFcc_QRYP);
    AfxAssertRange(pool->m.cap, queryIdx, 1);
    glVmt const* gl = &dpu->gl;
    AfxAssert(pool->glTarget == GL_TIMESTAMP);
    gl->QueryCounter(pool->glHandle[queryIdx], pool->glTarget);
}

_SGL afxError _DpuBindAndSyncQryp(sglDpu* dpu, afxBool syncOnly, avxQueryPool qryp)
{
    //AfxEntry("pip=%p", pip);
    afxError err = AFX_ERR_NONE;
    glVmt const* gl = &dpu->gl;


    if (!qryp)
    {
        
    }
    else
    {
        AfxAssertObjects(1, &qryp, afxFcc_QRYP);
        GLuint* glHandle = qryp->glHandle;

        if ((!glHandle) || (qryp->updFlags & SGL_UPD_FLAG_DEVICE))
        {
            if ((!glHandle) || (qryp->updFlags & SGL_UPD_FLAG_DEVICE_INST))
            {
                if (glHandle)
                {
                    gl->DeleteQueries(qryp->m.cap, glHandle); _SglThrowErrorOccuried();
                    glHandle = NIL;
                }
                gl->GenQueries(qryp->m.cap, glHandle); _SglThrowErrorOccuried();
                qryp->glHandle = glHandle;
                AfxLogEcho("Query pool inited. %u", qryp->m.cap);
            }

            qryp->updFlags |= ~SGL_UPD_FLAG_DEVICE;
        }
    }

    return err;
}

_SGL afxError _SglQrypDtor(avxQueryPool qryp)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &qryp, afxFcc_QRYP);
    afxDrawContext dctx = AfxGetQueryPoolContext(qryp);

    if (qryp->glHandle)
    {
        for (afxNat i = 0; i < qryp->m.cap; i++)
        {
            _SglDctxDeleteGlRes(dctx, 10, (void*)qryp->glHandle[i]);
        }
        qryp->glHandle = 0;
    }

    return err;
}

_SGL afxError _SglQrypCtor(avxQueryPool qryp, afxCookie const* cookie)
{
    afxError err = AFX_ERR_NONE;
    AfxAssertObjects(1, &qryp, afxFcc_QRYP);

    afxDrawContext dctx = cookie->udd[0];
    afxQueryType type = *(afxQueryType const*)cookie->udd[1];
    afxNat cap = *(afxNat const*)cookie->udd[2];

    qryp->glHandle = AfxAllocate(qryp->m.cap, sizeof(qryp->glHandle[0]), 0, AfxHere());
    qryp->updFlags = SGL_UPD_FLAG_DEVICE_INST;
    qryp->m.type = type;
    qryp->m.cap = cap;

    if (qryp->m.type == afxQueryType_OCCLUSION)
    {
        qryp->glTarget = GL_SAMPLES_PASSED;
    }
    else if (qryp->m.type == afxQueryType_PIPELINE)
    {
        qryp->glTarget = 0;
    }
    else if (qryp->m.type == afxQueryType_TIMESTAMP)
    {
        qryp->glTarget = GL_TIME_ELAPSED;
    }
    else AfxThrowError();

    return err;
}
