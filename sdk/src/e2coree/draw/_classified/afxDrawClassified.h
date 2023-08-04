/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_DRAW_CLASSIFIED_H
#define AFX_DRAW_CLASSIFIED_H

#define AFX_DRAW_SRC
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/pipelining/afxPipeline.h"
#include "afx/draw/afxDrawScript.h"

#define AfxDrawContextGetVmt(dctx_) ((afxDctxImpl const*)AfxObjectGetVmt((afxObject*)(dctx_)))

// DRAW IMPLEMETATION HIDEOUT

AFX_DEFINE_STRUCT(_afxDdrvVmt)
{
    afxError(*dtor)(afxDrawDriver);
    afxError(*proc)(afxDrawDriver,afxDrawThread); // call their draw threads.
    afxError(*dque)(afxDrawQueue, afxDrawQueueSpecification const*);
    afxError(*dctx)(afxDrawContext,afxDrawContextSpecification const*);
    afxError(*dout)(afxDrawOutput,afxUri const*,afxDrawOutputSpecification const*);
    afxError(*din)(afxDrawInput,afxUri const*,afxDrawInputSpecification const*);
};

AFX_DEFINE_STRUCT(_afxDqueVmt)
{
    afxError(*dtor)(afxDrawQueue);
    afxError(*proc)(afxDrawQueue,afxDrawThread);
    afxError(*wait)(afxDrawQueue); // wait for queue liberation
    afxError(*submit)(afxDrawQueue, afxDrawSubmissionSpecification const *spec, afxNat *submNo); // called by din to enqueue submission
    afxError(*recycle)(afxDrawQueue);
    afxError(*open)(afxDrawQueue);
    afxError(*close)(afxDrawQueue);
};

AFX_DEFINE_STRUCT(_afxDctxVmt)
{
    afxError(*dtor)(afxDrawContext);
    afxError(*proc)(afxDrawContext,afxDrawThread);
    afxError(*cout)(afxDrawContext,afxDrawOutput,afxBool connect, afxNat *slotIdx);
    afxError(*cin)(afxDrawContext,afxDrawInput,afxBool connect, afxNat *slotIdx);
    afxError(*dscr)(afxDrawScript);
    afxError(*canv)(afxCanvas);
    afxError(*tex)(afxTexture);
    afxError(*surf)(afxSurface);
    afxError(*shd)(afxShader);
    afxError(*pip)(afxPipeline);
    afxError(*buf)(afxBuffer);
    afxError(*lego)(afxLego);
    afxError(*samp)(afxSampler);
};

AFX_DEFINE_STRUCT(_afxDoutVmt)
{
    afxError(*dtor)(afxDrawOutput);
    afxError(*proc)(afxDrawOutput,afxDrawThread);
    afxError(*cctx)(afxDrawOutput, afxDrawContext from, afxDrawContext to, afxNat *slotIdx); // before try to and after successful binding, dctx call this method to give a chance of dout to release/rebuild surfaces and do other stuff.
    afxError(*flush)(afxDrawOutput, afxTime timeout);
    afxError(*req)(afxDrawOutput, afxTime timeout, afxNat*bufIdx);
};

AFX_DEFINE_STRUCT(_afxDinVmt)
{
    afxError(*dtor)(afxDrawInput);
    afxError(*proc)(afxDrawInput,afxDrawThread);
    afxError(*cctx)(afxDrawInput,afxDrawContext from, afxDrawContext to, afxNat *slotIdx);
};

AFX_DEFINE_STRUCT(_afxDscrVmt)
{
    afxError(*dtor)(afxDrawScript);
    afxError(*begin)(afxDrawScript, afxBool permanent);
    afxError(*end)(afxDrawScript);
    afxError(*reset)(afxDrawScript);
};

AFX_DEFINE_STRUCT(_afxDcmdVmt)
{
    void(*beginPass)(afxDrawScript, afxDrawPassState const *state);
    void(*nextPass)(afxDrawScript, afxBool useAuxScripts);
    void(*endPass)(afxDrawScript);
    
    void(*bindPipeline)(afxDrawScript, afxPipeline pip);

    void(*setInputAssemblyState)(afxDrawScript, afxPipelineInputAssemblyState const *state);
    void(*setViewport)(afxDrawScript, afxNat first, afxNat cnt, afxViewport const vp[]);
    void(*setRasterizerState)(afxDrawScript, afxPipelineRasterizerState const *state);
    void(*setDepthState)(afxDrawScript, afxPipelineDepthState const *state);
    void(*setScissor)(afxDrawScript, afxNat first, afxNat cnt, afxRect const rect[]);

    void(*bindBuffers)(afxDrawScript, afxNat set, afxNat first, afxNat cnt, afxBuffer buf[], afxNat offset[], afxNat range[]);
    void(*bindVertexBuffers)(afxDrawScript, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[]);
    void(*bindVertexBuffers2)(afxDrawScript, afxNat first, afxNat cnt, afxBuffer buf[], afxSize const offset[], afxSize const stride[]);
    void(*bindManagedVertexBuffers)(afxDrawScript, afxNat first, afxNat cnt, afxVertexBuffer vbuf[], afxNat const baseVtx[], afxNat const vtxArr[]);
    void(*bindIndexBuffer)(afxDrawScript, afxBuffer buf, afxNat offset, afxNat idxSiz);
    void(*bindManagedIndexBuffer)(afxDrawScript, afxIndexBuffer ibuf, afxNat rgnIdx);
    void(*bindTextures)(afxDrawScript, afxNat set, afxNat first, afxNat cnt, afxSampler smp[], afxTexture tex[]);

    void(*draw)(afxDrawScript, afxNat vtxCnt, afxNat instCnt, afxNat firstVtx, afxNat firstInst);
    void(*drawIndexed)(afxDrawScript, afxNat idxCnt, afxNat instCnt, afxNat firstIdx, afxNat vtxOff, afxNat firstInst);
};

AFX_DEFINE_STRUCT(_afxCanvVmt)
{
    afxError(*dtor)(afxCanvas);
    afxError(*ext)(afxCanvas, afxWhd const from, afxWhd const to);
};

AFX_DEFINE_STRUCT(_afxTexVmt)
{
    afxError(*dtor)(afxTexture);
    afxError(*map)(afxTexture, afxTextureRegion const *rgn, afxTextureOpenFlags flags, afxNat *siz, void**ptr);
    afxError(*unmap)(afxTexture, afxTextureRegion const *rgn);
};

AFX_DEFINE_STRUCT(_afxSurfVmt)
{
    afxError(*dtor)(afxSurface);
};

AFX_DEFINE_STRUCT(_afxBufVmt)
{
    afxError(*dtor)(afxBuffer);
    void*(*map)(afxBuffer, afxSize offset, afxNat range, afxFlags flags);
    afxError(*unmap)(afxBuffer);
};


AFX_DEFINE_STRUCT(_afxSampVmt)
{
    afxError(*dtor)(afxSampler);
};

AFX_DEFINE_STRUCT(_afxShdVmt)
{
    afxError(*dtor)(afxShader);
};

AFX_DEFINE_STRUCT(_afxPipVmt)
{
    afxError(*dtor)(afxPipeline);
};

#endif//AFX_DRAW_CLASSIFIED_H