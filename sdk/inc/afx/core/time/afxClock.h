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

#ifndef AFX_CLOCK_H
#define AFX_CLOCK_H

#include "afx/core/afxCoreDefs.h"

AFX_DEFINE_STRUCT(afxClock)
{
    afxNat32 data[4];
};

AFX void        AfxGetClock(afxClock *clock);
AFX afxReal64   AfxGetSecondsElapsed(afxClock const *start, afxClock const *end);

/// Qwadro acessa ao relógio em tempo real do sistema para fins cronométricos internos. 
/// Conforme esperado que você a desejar solicitar esse relógio, Qwadro expõe as funções que ele usa. 
/// Aqui está um exemplo de como eles podem ser usados para cronometrar as iterações de um loop.

/*
    afxClock lastClock, currentClock;

    AfxGetClock(&lastClock);

    while(1)
    {
        // Find out how many seconds have elapsed for the previous iteration of the loop
        AfxGetClock(&currentClock);
        afxReal SecondsElapsed = AfxGetSecondsElapsed(&lastClock, &currentClock);
        lastClock = currentClock;

        // Do processing with SecondsElapsed here
    }
*/

#endif//AFX_CLOCK_H