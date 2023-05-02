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
 *                      (c) 2017 SIGMA Co. & SIGMA Technology Group
 *                                    www.sigmaco.org
 */

#ifndef AFX_PIPELINE_MODULE_H
#define AFX_PIPELINE_MODULE_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/core/io/afxStream.h"
#include "afx/core/io/afxUri.h"

AFX_DEFINE_HANDLE(afxPipelineModule);

AFX void*               AfxPipelineModuleGetContext(afxPipelineModule pipm);
AFX void*               AfxPipelineModuleGetDriver(afxPipelineModule pipm);
AFX void*               AfxPipelineModuleGetDrawSystem(afxPipelineModule pipm);

AFX afxError            AfxPipelineModuleDownload(afxPipelineModule pipm, afxUri const *uri);
AFX afxError            AfxPipelineModuleSerialize(afxPipelineModule pipm, afxStream ios);

#ifndef AFX_DRAW_DRIVER_SRC

AFX_OBJECT(afxPipelineModule)
{
    afxObject           obj;
};

#endif

#endif//AFX_PIPELINE_MODULE_H