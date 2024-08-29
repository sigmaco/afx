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

// Voice
// each of the notes or sounds able to be produced simultaneously by a musical instrument (especially an electronic one) or a computer.
// (in an electronic musical instrument) each of a number of preset or programmable tones.

#ifndef ASX_SOUND_H
#define ASX_SOUND_H

#include "qwadro/inc/sound/afxSoundDefs.h"

#ifdef _ASX_SOUND_C
#ifdef _AFX_LISTENER_C
#ifndef _AFX_LISTENER_IMPL
AFX_OBJECT(afxListener)
#else
struct _afxBaseListener
#endif
{
    afxBool loaded;
};
#endif//_AFX_LISTENER_C
#endif//_ASX_SOUND_C


AFX_OBJECT(asxSound)
{
    afxReal gain; // A value of 0.0 is meaningless with respect to a logarithmic scale; it is silent.
    afxBool looping;
    afxV3d  position;
    afxV3d  velocity;
    afxV3d  direction;
    afxReal pitch; // A multiplier for the frequency(sample rate) of the source's buffer.
    afxReal innerConeAngle; // The angle covered by the inner cone, where the source will not attenuate.
    afxReal outerConeAngle; // The angle covered by the outer cone, where the source will be fully attenuated.

    afxBool playing;
    afxSphere bounding;
};



#endif//ASX_SOUND_H
