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
#include "afx/core/io/afxIoSystem.h"

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

AFX_DEFINE_STRUCT(afxDrawSystemSpecification)
{
    afxMemory           genrlMem;
};

AFX_OBJECT(afxDrawSystem)
{
    afxObject           obj;
#ifdef _AFX_DRAW_SYSTEM_C
    afxChain            provisions;
    afxClass            ddrvClass;
    afxClass            dthrClass;

    afxMemory           mem;
    afxArena            aren;

    afxChain            texIoCodecs;

    afxModule           e2draw; // SIGMA GL is required for minimal operability
#endif
};

AFX afxBool             AfxGetDrawSystem(afxDrawSystem *dsys);
AFX afxError            AfxBootUpDrawSystem(afxDrawSystemSpecification const *spec, afxDrawSystem *dsys);
AFX void                AfxShutdownDrawSystem(void);

AFX afxMemory           AfxGetDrawMemory(void);

AFX afxBool             AfxGetDrawDriver(afxNat drvIdx, afxDrawDriver *ddrv);
AFX afxError            AfxRegisterDrawDriver(afxDrawDriverSpecification const *spec, afxDrawDriver *ddrv);

AFX afxError            AfxAcquireDrawContexts(afxDrawContextSpecification const *spec, afxNat cnt, afxDrawContext dctx[]);
AFX afxError            AfxAcquireDrawOutputs(afxDrawOutputSpecification const *spec, afxNat cnt, afxDrawOutput dout[]);
AFX afxError            AfxAcquireDrawInputs(afxDrawInputSpecification const *spec, afxNat cnt, afxDrawInput din[]);

AFX afxNat              AfxEnumerateDrawThreads(afxNat first, afxNat cnt, afxDrawThread dthr[]);
AFX afxNat              AfxEnumerateDrawDrivers(afxNat first, afxNat cnt, afxDrawDriver ddrv[]);

AFX afxNat              AfxCurateDrawThreads(afxNat first, afxNat cnt, afxBool(*f)(afxDrawThread, void*), void *udd);
AFX afxNat              AfxCurateDrawDrivers(afxNat first, afxNat cnt, afxBool(*f)(afxDrawDriver, void*), void *udd);

AFX afxNat              AfxGetDrawThreadCount(void);
AFX afxNat              AfxGetDrawDriverCount(void);

AFX afxClass*           AfxGetDrawThreadClass(void);
AFX afxClass*           AfxGetDrawDriverClass(void);

AFX afxNat              AfxChooseTextureStorage(afxTexture tex, afxTextureRegion const *rgn);
AFX afxResult           AfxDescribeTextureStorages(afxNat first, afxNat cnt, afxTextureStorageSpecification spec[]);
AFX afxResult           AfxEnumerateTextureStorages(afxNat first, afxNat cnt, afxTextureStorageRegistry *storage[]);
AFX afxResult           AfxRegisterTextureStorages(afxNat cnt, afxTextureStorageSpecification const spec[]);

#endif//AFX_DRAW_SYSTEM_H
