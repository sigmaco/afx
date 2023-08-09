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

// The Autonomous Draw System, responsible to manage resources and provide drawing capabilities.

#ifndef AFX_DRAW_SYSTEM_H
#define AFX_DRAW_SYSTEM_H

#include "afx/draw/afxDrawDefs.h"
#include "afx/draw/afxDrawDriver.h"
#include "afx/draw/afxDrawThread.h"

#include "afx/draw/afxColor.h"
#include "afx/draw/afxVertex1.h"

#include "afx/draw/pipelining/afxDrawOperation.h"
#include "afx/core/afxModule.h"
#include "afx/afxApplication.h"
#include "afx/core/mem/afxResidency.h"

AFX_DECLARE_STRUCT(afxTextureStorageRegistry);

AFX_DEFINE_STRUCT(afxTextureStorageSpecification)
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

AFX_DEFINE_STRUCT(afxDrawSystemConfig)
{
    afxContext           genrlMem;

    afxSize const*      attrs[2];
};

struct _afxDsysD
#ifdef _AFX_DRAW_SYSTEM_C
{
    _AFX_DBG_FCC
    afxObject           dsysObj;
    afxChain            provisions;
    afxClass            ddrvClass;

    afxContext           mem;
    afxArena            aren;

    afxResidency         inputs;
    afxResidency         outputs;
    afxResidency         contexts;
    afxResidency         threads;

    afxChain            texIoCodecs;

    afxModule           e2draw; // SIGMA GL is required for minimal operability since core has no more embedded fallback.
}
#endif
;

AFX void                AfxChooseDrawSystemConfiguration(afxDrawSystemConfig *config, afxNat extendedSiz);
AFX afxError            AfxEstablishDrawSystem(afxDrawSystemConfig const *config, afxDrawSystem *dsys);
AFX void                AfxAbolishDrawSystem(void);
AFX afxBool             AfxGetDrawSystem(afxDrawSystem *dsys);

AFX afxContext           AfxGetDrawMemory(void);

AFX afxBool             AfxGetDrawDriver(afxNat drvIdx, afxDrawDriver *ddrv);
AFX afxError            AfxRegisterDrawDriver(afxDrawDriverSpecification const *spec, afxDrawDriver *ddrv);

AFX afxNat              AfxEnumerateDrawThreads(afxNat first, afxNat cnt, afxDrawThread dthr[]);
AFX afxNat              AfxEnumerateDrawDrivers(afxNat first, afxNat cnt, afxDrawDriver ddrv[]);
AFX afxNat              AfxEnumerateDrawContexts(afxNat first, afxNat cnt, afxDrawContext dctx[]);
AFX afxNat              AfxEnumerateDrawOutputs(afxNat first, afxNat cnt, afxDrawOutput dout[]);
AFX afxNat              AfxEnumerateDrawInputs(afxNat first, afxNat cnt, afxDrawInput din[]);

AFX afxNat              AfxCurateDrawThreads(afxNat first, afxNat cnt, afxBool(*f)(afxDrawThread, void*), void *udd);
AFX afxNat              AfxCurateDrawDrivers(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDriver, void*), void *udd);
AFX afxNat              AfxCurateDrawContexts(afxNat first, afxNat cnt, afxBool(*f)(afxDrawContext, void*), void *udd);
AFX afxNat              AfxCurateDrawOutputs(afxNat first, afxNat cnt, afxBool(*f)(afxDrawOutput, void*), void *udd);
AFX afxNat              AfxCurateDrawInputs(afxNat first, afxNat cnt, afxBool(*f)(afxDrawInput, void*), void *udd);


AFX afxNat              AfxGetDrawThreadCount(void);
AFX afxNat              AfxGetDrawDriverCount(void);
AFX afxNat              AfxGetDrawContextCount(void);
AFX afxNat              AfxGetDrawOutputCount(void);
AFX afxNat              AfxGetDrawInputCount(void);

AFX afxClass*           AfxGetDrawDriverClass(void);

AFX afxNat              AfxChooseTextureStorage(afxTexture tex, afxTextureRegion const *rgn);
AFX afxResult           AfxDescribeTextureStorages(afxNat first, afxNat cnt, afxTextureStorageSpecification spec[]);
AFX afxResult           AfxEnumerateTextureStorages(afxNat first, afxNat cnt, afxTextureStorageRegistry *storage[]);
AFX afxResult           AfxRegisterTextureStorages(afxNat cnt, afxTextureStorageSpecification const spec[]);

#endif//AFX_DRAW_SYSTEM_H
