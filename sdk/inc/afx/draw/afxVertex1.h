/*
 *          ::::::::  :::       :::     :::     :::::::::  :::::::::   ::::::::
 *         :+:    :+: :+:       :+:   :+: :+:   :+:    :+: :+:    :+: :+:    :+:
 *         +:+    +:+ +:+       +:+  +:+   +:+  +:+    +:+ +:+    +:+ +:+    +:+
 *         +#+    +:+ +#+  +:+  +#+ +#++:++#++: +#+    +:+ +#++:++#:  +#+    +:+
 *         +#+  # +#+ +#+ +#+#+ +#+ +#+     +#+ +#+    +#+ +#+    +#+ +#+    +#+
 *         #+#   +#+   #+#+# #+#+#  #+#     #+# #+#    #+# #+#    #+# #+#    #+#
 *          ###### ###  ###   ###   ###     ### #########  ###    ###  ########
 *
 *                      S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 Federação SIGMA
 *                                    www.sigmaco.org
 */

#ifndef AFX_VERTEX1_H
#define AFX_VERTEX1_H

#include "afx/draw/afxDrawDefs.h"

AFX_DEFINE_STRUCT(afxVertexLayout_)
{
    afxNat          stride;
    afxNat          attrCnt;
    afxNat          *attrSiz;
    afxNat          *attrOff;
    afxString const *attrName;
};

AFX afxVertexLayout_ const* AFX_XYZ_NRM_TAN_BTN_UV_UV2;

AFX afxVertexLayout_ const* AFX_V4dV3dV2d;

#endif//AFX_VERTEX1_H