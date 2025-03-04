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

#ifndef ASX_BUFFER_H
#define ASX_BUFFER_H

#include "qwadro/inc/math/bound/afxSphere.h"
#include "qwadro/inc/sim/afxSimDefs.h"

typedef enum asxBufferUsage
{
    asxBufferUsage_VERTEX = AFX_BIT(0), // used to hold mesh triangulation vertices
    asxBufferUsage_INDEX = AFX_BIT(1), // used to hold mesh triangulation indices
    asxBufferUsage_DRAW = AFX_BIT(2), // used to be drawn by draw system
    asxBufferUsage_CURVE = AFX_BIT(3), // used to hold geometric curves
} asxBufferUsage;

typedef enum asxBufferFlag
{
    asxBufferFlag_R = AFX_BIT(0),
    asxBufferFlag_W = AFX_BIT(1),
    asxBufferFlag_X = AFX_BIT(2),
} asxBufferFlags;

typedef enum asxFormat
{
    asxFormat_DaKeyframes32f, // Da, Keyframes32f
    asxFormat_DaK32fC32f, // Da, K32f, C32f
    asxFormat_DaIdentity, // Da, Identity
    asxFormat_DaConstant32f, // Da, Constant32f
    asxFormat_D3Constant32f, // D3, Constant32f
    asxFormat_D4Constant32f, // D4, Constant32f
    // still unsupported
    asxFormat_DaK16uC16u, // Da, K16u, C16u
    asxFormat_DaK8uC8u, // Da, K8u, C8u
    asxFormat_D4nK16uC15u, // D4n, K16u, C15u
    asxFormat_D4nK8uC7u, // D4n, K8u, C7u
    asxFormat_D3K16uC16u, // D3, K16u, C16u
    asxFormat_D3K8uC8u, // D3, K8u, C8u
    asxFormat_D9i1K16uC16u, // D9i1, K16u, C16u    
    asxFormat_D9i3K16uC16u, // D9i3, K16u, C16u
    asxFormat_D9i1K8uC8u, // D9i1, K8u, C8u
    asxFormat_D9i3K8uC8u, // D9i3, K8u, C8u
    asxFormat_D3i1K32fC32f, // D3i1, K32f, C32f
    asxFormat_D3i1K16uC16u, // D3i1, K16u, C16u
    asxFormat_D3i1K8uC8u // D3i1, K8u, C8u    

    // Let curve formats at first to be retrocompatible with old code.



} asxFormat;

AFX_DEFINE_STRUCT(asxBufferInfo)
{
    afxUnit         cap;
    asxBufferUsage  usage;
    asxBufferFlags  flags;
    asxFormat       fmt;
};

////////////////////////////////////////////////////////////////////////////////

ASX afxError    AsxAcquireBuffers(afxSimulation sim, afxUnit cnt, asxBufferInfo const infos[], asxBuffer buffers[]);

#endif//ASX_BUFFER_H
