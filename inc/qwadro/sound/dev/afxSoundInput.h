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
 *                       (c) 2017 SIGMA, Engitech, Scitech, Serpro
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ASX_SOUND_INPUT_H
#define ASX_SOUND_INPUT_H

#include "qwadro/sound/afxSoundDefs.h"
#include "qwadro/core/afxManager.h"
#include "qwadro/io/afxUri.h"
#include "qwadro/sound/dev/afxSoundBridge.h"

typedef enum afxSoundEventId
{
    afxSoundEventId_FENCE,
    afxSoundEventId_EXECUTE,
    afxSoundEventId_UPLOAD,
    afxSoundEventId_DOWNLOAD,
    afxSoundEventId_PREFETCH,
} afxSoundEventId;

AFX_DEFINE_STRUCT(afxSoundEvent)
{
    afxSoundEventId  id;
    afxBool         posted, accepted;
    void*           udd[0];
};

typedef afxBool(*afxSoundInputProc)(afxSoundInput sin, afxSoundEvent const* ev);

AFX_DEFINE_STRUCT(afxSoundInputConfig)
{
    afxSoundInputProc   proc;
    afxNat              dragTxuBase;
    afxNat              dragTxuCnt;

    afxNat              cmdPoolMemStock;
    afxNat              estimatedSubmissionCnt;
    afxNat              minVtxBufCap;
    afxNat              maxVtxBufCap;
    afxNat              minIdxBufCap;
    afxNat              maxIdxBufCap;
    void*               udd;
};

#ifdef _ASX_SOUND_C
#ifdef _ASX_SOUND_INPUT_C
AFX_OBJECT(afxSoundInput)
{
    afxLinkage          sdev;
    afxLinkage          sctx; // bound context

    afxChain            classes;
    afxSoundInputProc   procCb;

    afxBool             reconnecting;

    struct _afxDinIdd*  idd;
    void*               udd; // user-defined data
};
#endif//_ASX_SOUND_INPUT_C
#endif//_ASX_SOUND_C

ASX afxSoundDevice  AfxGetSoundInputDevice(afxSoundInput sin);

ASX void*           AfxGetSoundInputUdd(afxSoundInput sin);

// Connection

ASX afxBool         AfxReconnectSoundInput(afxSoundInput sin, afxSoundContext sctx);
ASX afxError        AfxDisconnectSoundInput(afxSoundInput sin);
ASX afxBool         AfxGetSoundInputContext(afxSoundInput sin, afxSoundContext* context);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AfxAcquireSoundInput(afxNat sdevId, afxSoundInputConfig const* cfg, afxSoundInput* input);

ASX afxError        AfxOpenSoundInput(afxNat sdevId, afxUri const* endpoint, afxSoundInputConfig const* cfg, afxSoundInput* input);

#endif//ASX_SOUND_INPUT_H
