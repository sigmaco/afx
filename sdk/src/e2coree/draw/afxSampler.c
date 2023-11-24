/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                  Q W A D R O   E X E C U T I O N   E C O S Y S T E M
 *
 *                                   Public Test Build
 *                   (c) 2017 SIGMA Technology Group — Federação SIGMA
 *                                    www.sigmaco.org
 */

#define _AFX_DRAW_C
#define _AFX_SAMPLER_C
#define _AFX_DRAW_SYSTEM_C
#define _AFX_DRAW_CONTEXT_C
#include "afx/draw/afxDrawContext.h"

_AFX void AfxDescribeSampler(afxSampler samp, afxSamplerConfig* spec)
{
    afxError err = AFX_ERR_NONE;
    AfxAssert(spec);
    AfxAssertObjects(1, &samp, afxFcc_SAMP);

    spec->magFilter = samp->magFilter;
    spec->minFilter = samp->minFilter;
    spec->mipmapFilter = samp->mipmapFilter;
    spec->uvw[0] = samp->uvw[0];
    spec->uvw[1] = samp->uvw[1];
    spec->uvw[2] = samp->uvw[2];
    spec->anisotropyEnabled = samp->anisotropyEnabled;
    spec->anisotropyMaxDegree = samp->anisotropyMaxDegree;
    spec->compareEnabled = samp->compareEnabled;
    spec->compareOp = samp->compareOp;
    AfxCopyColor(spec->borderColor, samp->borderColor);
    spec->unnormalizedCoords = samp->unnormalizedCoords;
}

_AFX afxError AfxAcquireSamplers(afxDrawContext dctx, afxNat cnt, afxSamplerConfig const config[], afxSampler samp[])
{
    afxError err = AFX_ERR_NONE;
    AfxEntry("cnt=%u,samp=%p,config=%p", cnt, samp, config);
    AfxAssert(cnt);
    AfxAssert(samp);
    AfxAssertObjects(1, &dctx, afxFcc_DCTX);

    if (AfxAcquireObjects(&dctx->samplers, cnt, (afxHandle*)samp, (void*[]) { (void*)config }))
        AfxThrowError();

    return err;
}
