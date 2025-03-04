/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *       Q W A D R O   S O U N D   S Y N T H E S I S   I N F R A S T R U C T U R E
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#ifndef AMX_BUFFER_H
#define AMX_BUFFER_H

#include "qwadro/inc/mix/afxMixDefs.h"

typedef enum amxBufferUsage
{
    amxBufferUsage_SAMPLED = AFX_BIT(0),
    amxBufferUsage_MIX = AFX_BIT(1),
    amxBufferUsage_SOURCE = AFX_BIT(2),
    amxBufferUsage_SINK = AFX_BIT(3),
    amxBufferUsage_AUDIO_DEC = AFX_BIT(4),
    amxBufferUsage_AUDIO_ENC = AFX_BIT(5),
    amxBufferUsage_VIDEO_DEC = AFX_BIT(6),
    amxBufferUsage_VIDEO_ENC = AFX_BIT(7),
} amxBufferUsage;

typedef enum amxBufferFlag
{
    amxBufferFlag_R = AFX_BIT(0),
    amxBufferFlag_W = AFX_BIT(1),
    amxBufferFlag_X = AFX_BIT(2),
} amxBufferFlags;

AFX_DEFINE_STRUCT(amxBufferInfo)
{
    afxUnit         cap;
    amxBufferUsage  usage;
    amxBufferFlags  flags;
    amxFormat       fmt;
    void*           udd;
};

////////////////////////////////////////////////////////////////////////////////

AMX afxError    AmxAcquireBuffers(afxMixSystem msys, afxUnit cnt, amxBufferInfo const infos[], amxBuffer buffers[]);

#endif//AMX_BUFFER_H
