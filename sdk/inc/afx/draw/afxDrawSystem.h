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

// The Autonomous Draw System, responsible to manage resources and provide drawing capabilities.

#ifndef AFX_DRAW_SYSTEM_H
#define AFX_DRAW_SYSTEM_H

#include "afx/draw/base/afxDrawDefs.h"
#include "afx/draw/base/afxColor.h"
#include "afx/draw/base/afxVertex.h"
#include "afxDrawContext.h"
#include "afxDrawDriver.h"
#include "afxDrawInput.h"
#include "afxDrawOutput.h"
#include "afx/core/afxModule.h"
#include "../afxApplication.h"

// Criar novo formato .tga do Qwadro.

//#define _AFX_DRAW_MULTITHREADED

#ifndef AFX_DRAW_SRC

AFX_OBJECT(afxDrawSystem) { afxObject obj; };

#endif

AFX_DECLARE_STRUCT(afxTextureCodecRegistry);

AFX_DEFINE_STRUCT(afxTextureCodecSpecification)
{
    afxString const*    name;
    afxString const*    author;
    afxString const*    website;
    afxString const*    note;
    afxNat              verMajor;
    afxNat              verMinor;
    afxNat              verPatch;
    afxNat              extCnt;
    afxString const*    exts;

    afxNat              minScale;
    afxNat              maxScale;
    afxNat              minDepth;
    afxNat              maxDepth;
    afxNat              minBpp;
    afxNat              maxBpp;
    afxBool             hasAlpha;

    afxError            (*acq)(afxDrawContext dctx, afxNat cnt, afxUri const uri[], afxTexture tex[]);
    // coacquire for batch layered textures such as DDS?
    afxError            (*upload)(afxTexture tex, afxUri const *uri);
    afxError            (*download)(afxTexture tex, afxUri const *uri);
    afxError            (*uploadRgn)(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
    afxError            (*downloadRgn)(afxTexture tex, afxNat cnt, afxTextureRegion const rgn[], afxUri const uri[]);
};

AFX void*           AfxDrawSystemGetSystem(afxDrawSystem dsys);

AFX afxDrawDriver   AfxDrawSystemRegisterDriver(afxDrawSystem dsys, afxModule mdle, afxDrawDriverSpecification const *spec);
AFX afxDrawContext  AfxDrawSystemAcquireContext(afxDrawSystem dsys, afxDrawContextSpecification const *spec);

AFX afxClass*       AfxDrawSystemGetDriverClass(afxDrawSystem dsys);

AFX afxDrawDriver   AfxDrawSystemGetDriver(afxDrawSystem dsys, afxNat idx);

AFX afxResult       AfxDrawSystemForEachDriver(afxDrawSystem dsys, void(*f)(afxIterator *iter), void *data);

AFX afxResult       AfxDrawSystemForEachTextureCodec(afxDrawSystem dsys, afxResult(*f)(afxTextureCodecRegistry*, void*), void *data);
AFX afxResult       AfxDrawSystemRegisterTextureCodec(afxDrawSystem dsys, afxNat cnt, afxTextureCodecSpecification const queries[]);

AFX afxResult       AfxDrawSystemProcess(afxDrawSystem dsys);

#endif//AFX_DRAW_SYSTEM_H
