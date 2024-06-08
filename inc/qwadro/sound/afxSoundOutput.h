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
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef ASX_SOUND_OUTPUT_H
#define ASX_SOUND_OUTPUT_H

#include "qwadro/sound/afxSoundDefs.h"

AFX_DEFINE_STRUCT(afxSoundOutputConfig)
{
    void*               udd;
#ifdef AFX_OS_WIN
    struct
    {
        void*           hwnd;
        void*           hinstance;
    }                   w32;
#endif
};

#ifdef _ASX_SOUND_C
#ifdef _ASX_SOUND_OUTPUT_C

AFX_OBJECT(afxSoundOutput)
{
    afxLinkage          sdev;
    afxLinkage          sctx; // bound context
    void*               udd; // user-defined data
    afxBool             (*getIddCb)(afxSoundOutput,afxNat,void*);
    afxAtom32           reconnecting;

    afxClock            startClock;
    afxClock            lastClock;
    afxClock            outCntResetClock;

    struct _afxDoutIdd* idd;

#ifdef _ASX_SOUND_OUTPUT_IMPL
#ifdef AFX_OS_WIN
    struct
    {
        HINSTANCE       hInst;
        HWND            hWnd;
    }                   w32;
#endif//AFX_OS_WIN
#endif//_ASX_SOUND_OUTPUT_IMPL
};
#endif//_ASX_SOUND_OUTPUT_C
#endif//_ASX_SOUND_C

ASX afxSoundDevice  AfxGetSoundOutputDevice(afxSoundOutput sout);
ASX afxError        AfxGetSoundOutputIdd(afxSoundOutput sout, afxNat code, void* dst);

// Connection

ASX afxBool         AfxReconnectSoundOutput(afxSoundOutput sout, afxSoundContext sctx);
ASX afxError        AfxDisconnectSoundOutput(afxSoundOutput sout);
ASX afxBool         AfxGetSoundOutputContext(afxSoundOutput sout, afxSoundContext* context);

////////////////////////////////////////////////////////////////////////////////

ASX afxError        AfxAcquireSoundOutput(afxNat sdevId, afxSoundOutputConfig const* cfg, afxSoundOutput* output);

ASX afxError        AfxOpenSoundOutput(afxNat sdevId, afxUri const* endpoint, afxSoundOutputConfig const* cfg, afxSoundOutput* output);

#endif//ASX_SOUND_OUTPUT_H
