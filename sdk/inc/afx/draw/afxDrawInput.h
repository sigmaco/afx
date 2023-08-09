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

// afxDrawInput devices operates like device submission queues grouping sets of draw streams and present their result to the connected draw output devices.

// No QWADRO, há um conceito de uma fila de submissão de trabalho para a GPU. Porém, diferentemente do Vulkan, no QWADRO esta "fila" é separada em dois.
// A primeira parte contém a demanda e a segunda parte o estado da fila. Isto porque assim podemos usar a mesma demanda para dois contextos diferentes, cada qual contendo a outra parte, e logo, seu estado particular.

#ifndef AFX_DRAW_INPUT_H
#define AFX_DRAW_INPUT_H

#include "afx/draw/afxDrawQueue.h"
#include "afx/core/io/afxUri.h"

AFX_DEFINE_STRUCT(afxDrawInputConfig)
{
    afxNat              drvIdx;
    afxUri const*       endpoint;
    afxNat              cmdPoolMemStock;
    afxNat              estimatedSubmissionCnt;
    afxError            (*prefetch)(afxDrawInput,afxDrawThread);
    void*               udd;
};

AFX_DECLARE_STRUCT(_afxDinVmt);

struct _afxDinD
{
    _AFX_DBG_FCC
    afxObject           dinObj;
    _afxDinVmt const*   vmt;
    void*               idd;
#ifdef _AFX_DRAW_INPUT_C
    afxDrawDriver       ddrv; 
    afxDrawContext      dctx; // bound context
    afxContext           mem;

    afxArray            scripts;
    afxNat              minScriptReserve;

    afxError            (*userPrefetchProc)(afxDrawInput din, afxDrawThread dthr);
    //afxSlock              prefetchSlock;
    afxBool             prefetching;
    afxBool             prefetchEnabled;
    void*               udd;
#endif
};

AFX afxError            AfxAcquireDrawInputs(afxNat cnt, afxDrawInput din[], afxDrawInputConfig const config[]);
AFX void                AfxReleaseDrawInputs(afxNat cnt, afxDrawInput din[]);

AFX afxDrawDriver       AfxGetDrawInputDriver(afxDrawInput din);

AFX afxError            AfxAcquireDrawScripts(afxDrawInput din, afxNat portIdx, afxNat cnt, afxDrawScript dscr[]);

// Connection
AFX afxBool             AfxDrawInputIsConnected(afxDrawInput din);
AFX afxBool             AfxGetConnectedDrawInputContext(afxDrawInput din, afxDrawContext *dctx);
AFX afxBool             AfxReconnectDrawInput(afxDrawInput din, afxDrawContext dctx, afxNat *slotIdx);
AFX afxError            AfxDisconnectDrawInput(afxDrawInput din, afxNat *slotIdx);

AFX afxError            AfxRequestDrawInputScript(afxDrawInput din, afxDrawQueueFlags caps, afxTime timeout, afxNat *scrIdx);
AFX afxError            AfxRecycleDrawInputScripts(afxDrawInput din, afxNat firstScrIdx, afxNat scrCnt);

AFX afxError            AfxSubmitDrawScripts(afxDrawInput din, afxNat cnt, afxDrawScript scripts[]);
AFX afxError            AfxSubmitPresentations(afxDrawInput din, afxNat cnt, afxDrawOutput outputs[], afxNat outputBufIdx[]);

AFX afxError            AfxEnableDrawInputPrefetching(afxDrawInput din, afxBool enabled);

AFX void*               AfxGetDrawInputUdd(afxDrawInput din);

#endif//AFX_DRAW_INPUT_H