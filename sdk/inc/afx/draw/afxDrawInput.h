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

// This section is part of SIGMA GL/2.

// afxDrawInput devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

// No QWADRO, há um conceito de uma fila de submissão de trabalho para a GPU. Porém, diferentemente do Vulkan, no QWADRO esta "fila" é separada em dois.
// A primeira parte contém a demanda e a segunda parte o estado da fila. Isto porque assim podemos usar a mesma demanda para dois contextos diferentes, cada qual contendo a outra parte, e logo, seu estado particular.

#ifndef AFX_DRAW_INPUT_H
#define AFX_DRAW_INPUT_H

#include "afx/draw/afxDrawQueue.h"
#include "afx/core/afxUri.h"

typedef afxBool(*afxDrawInputProc)(afxDrawInput din, afxNat msg, afxSize argv[][1][1]);

AFX_DEFINE_STRUCT(afxDrawInputConfig)
{
    afxUri const*       endpoint;
    afxNat              cmdPoolMemStock;
    afxNat              estimatedSubmissionCnt;
    afxError            (*prefetch)(afxDrawInput din, afxNat thrUnitIdx);
    void*               udd;
};

#ifdef _AFX_DRAW_C
#ifdef _AFX_DRAW_INPUT_C
AFX_OBJECT(afxDrawInput)
#else
struct afxBaseDrawInput
#endif
{
    afxLinkage          dctx; // bound context
    afxContext          mem;

    afxError            (*submitCb)(afxDrawInput, afxNat, afxDrawScript[]);
    afxError            (*presentCb)(afxDrawInput, afxNat, afxDrawOutput[], afxNat[]);

    afxArray            scripts;
    afxNat              minScriptReserve;

    //afxSlock              prefetchSlock;
    afxBool             prefetching;
    afxBool             prefetchEnabled;
    afxError            (*prefetchCb)(afxDrawInput din, afxNat thrUnitIdx);
    void*               udd;
};
#endif

AFX afxDrawDevice       AfxGetDrawInputDevice(afxDrawInput din);

// Connection
AFX afxBool             AfxDrawInputIsConnected(afxDrawInput din);
AFX afxBool             AfxGetDrawInputConnection(afxDrawInput din, afxDrawContext *dctx);
AFX afxBool             AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx);
AFX afxError            AfxDisconnectDrawInput(afxDrawInput din);

//AFX afxError            AfxRequestDrawInputScript(afxDrawInput din, afxDrawQueueFlags caps, afxTime timeout, afxNat *scrIdx);
AFX afxError            AfxRecycleDrawInputScripts(afxDrawInput din, afxNat firstScrIdx, afxNat scrCnt);
AFX void                AfxDiscardDrawInputScripts(afxDrawInput din, afxNat cnt, afxNat scrIdx[]);

AFX afxError            AfxRequestDrawInputBuffer(afxDrawInput din, afxNat portIdx, afxNat *scrIdx);
AFX afxBool             AfxGetDrawInputBuffer(afxDrawInput din, afxNat idx, afxDrawScript *dscr);
AFX afxError            AfxSubmitDrawInputBuffers(afxDrawInput din, afxNat cnt, afxNat inBufIdx[]);

AFX afxError            AfxSubmitDrawScripts(afxDrawInput din, afxNat cnt, afxDrawScript scripts[]);
AFX afxError            AfxPresentDrawOutputBuffers(afxDrawInput din, afxNat cnt, afxDrawOutput outputs[], afxNat outputBufIdx[]);

AFX afxError            AfxEnableDrawInputPrefetching(afxDrawInput din, afxBool enabled);

AFX void*               AfxGetDrawInputUdd(afxDrawInput din);

#endif//AFX_DRAW_INPUT_H