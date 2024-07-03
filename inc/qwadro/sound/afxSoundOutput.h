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
    union
    {
#ifdef AFX_OS_WIN
        struct
        {
            void*       hInstance; // HINSTANCE
            void*       hWnd; // HWND
        }               w32;
#endif
    };
};

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
