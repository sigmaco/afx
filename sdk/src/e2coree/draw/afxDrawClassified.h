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
#include "afx/draw/afxPipeline.h"
#include "afx/draw/afxDrawScript.h"

#define AfxDrawContextGetVmt(dctx_) ((afxDctxImpl const*)AfxObjectGetVmt((afxInstance*)(dctx_)))

// DRAW IMPLEMETATION HIDEOUT

AFX_DEFINE_STRUCT(_afxDoutVmt)
{
    afxError(*flush)(afxDrawOutput, afxTime timeout);
    afxError(*req)(afxDrawOutput, afxTime timeout, afxNat*bufIdx);
};

AFX_DEFINE_STRUCT(_afxDinVmt)
{
    afxError(*req)(afxDrawInput, afxNat portIdx, afxNat *scrIdx);
    afxError(*subm)(afxDrawInput, afxNat, afxDrawScript[]);
    afxError(*pres)(afxDrawInput, afxNat, afxDrawOutput[], afxNat[]);
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