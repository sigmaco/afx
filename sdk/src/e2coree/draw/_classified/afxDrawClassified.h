/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *              T H E   Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_DRAW_CLASSIFIED_H
#define AFX_DRAW_CLASSIFIED_H

#define AFX_DRAW_SRC
#include "afx/draw/afxDrawSystem.h"
#include "afx/draw/pipelining/afxPipeline.h"
#include "afx/draw/afxDrawScript.h"

#define AfxDrawContextGetVmt(dctx_) ((afxDctxImpl const*)AfxObjectGetVmt((afxInstance*)(dctx_)))

// DRAW IMPLEMETATION HIDEOUT

AFX_DEFINE_STRUCT(_afxDdevVmt)
{
    afxError(*proc)(afxDrawThread,afxDrawDevice); // call their draw threads.
};

AFX_DEFINE_STRUCT(_afxDoutVmt)
{
    afxError(*link)(afxDrawOutput, afxDrawContext from, afxDrawContext to, afxNat *slotIdx); // before try to and after successful binding, dctx call this method to give a chance of dout to release/rebuild surfaces and do other stuff.
    afxError(*flush)(afxDrawOutput, afxTime timeout);
    afxError(*req)(afxDrawOutput, afxTime timeout, afxNat*bufIdx);
};

AFX_DEFINE_STRUCT(_afxDinVmt)
{
    afxError(*link)(afxDrawInput,afxDrawContext from, afxDrawContext to, afxNat *slotIdx);
    afxError(*req)(afxDrawInput, afxNat portIdx, afxNat *scrIdx);
    afxError(*subm)(afxDrawInput, afxNat, afxDrawScript[]);
    afxError(*pres)(afxDrawInput, afxNat, afxDrawOutput[], afxNat[]);
};

AFX_DEFINE_STRUCT(_afxDscrVmt)
{
    afxError(*begin)(afxDrawScript, afxBool permanent);
    afxError(*end)(afxDrawScript);
    afxError(*reset)(afxDrawScript);
};

AFX_DEFINE_STRUCT(_afxDcmdVmt)
{
    void(*execCmd)(afxDrawScript, afxNat, afxDrawScript[]);
    void(*beginCanv)(afxDrawScript, afxCanvasConfig const *state);
    void(*nextPass)(afxDrawScript, afxBool useAuxScripts);
    void(*endCanv)(afxDrawScript);
    
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
    afxError(*ext)(afxCanvas, afxWhd const from, afxWhd const to);
};

AFX_DEFINE_STRUCT(_afxTexVmt)
{
    afxError(*map)(afxTexture, afxTextureRegion const *rgn, afxTextureOpenFlags flags, afxNat *siz, void**ptr);
    afxError(*unmap)(afxTexture, afxTextureRegion const *rgn);
};

AFX_DEFINE_STRUCT(_afxBufVmt)
{
    void*(*map)(afxBuffer, afxSize offset, afxNat range, afxFlags flags);
    afxError(*unmap)(afxBuffer);
};

struct _dscrParadigm
{
    afxNat portIdx;
    afxDrawInput owner;
};

#endif//AFX_DRAW_CLASSIFIED_H