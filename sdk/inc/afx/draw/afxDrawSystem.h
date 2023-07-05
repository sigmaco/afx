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
#include "afx/draw/afxColor.h"
#include "afx/draw/afxVertex.h"
#include "afx/draw/afxDrawContext.h"
#include "afx/draw/afxDrawDriver.h"
#include "afx/draw/afxDrawInput.h"
#include "afx/draw/afxDrawOutput.h"
#include "afx/draw/afxDrawOperation.h"
#include "afx/core/afxModule.h"
#include "afx/afxApplication.h"
#include "afx/core/io/afxFileSystem.h"

// Criar novo formato .tga do Qwadro.

//#define _AFX_DRAW_MULTITHREADED

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

AFX_OBJECT(afxDrawSystem)
{
    afxObject           obj;
#ifdef _AFX_DRAW_SYSTEM_C
    afxChain            provisions;
    afxClass            ddrvClass;
    afxFileSystem       fsys;
    afxMemory        genrlMem;
    afxChain            texIoCodecs;
    afxModule           e2draw; // SIGGL is required for minimal support
#endif
};

AFX_DEFINE_STRUCT(afxDrawSystemSpecification)
{
    afxMemory           genrlMem;
    afxFileSystem       fsys;
};

AFX void*               AfxDrawSystemGetSystem(afxDrawSystem dsys);

AFX afxDrawDriver       AfxDrawSystemRegisterDriver(afxDrawSystem dsys, afxModule mdle, afxDrawDriverSpecification const *spec);
AFX afxDrawContext      AfxDrawSystemAcquireContext(afxDrawSystem dsys, afxDrawContextSpecification const *spec);

AFX afxClass*           AfxDrawSystemGetDriverClass(afxDrawSystem dsys);
AFX afxClass*           AfxDrawSystemGetContextClass(afxDrawSystem dsys, afxNat driverId);
AFX afxMemory           AfxDrawSystemGetMemory(afxDrawSystem dsys);
AFX afxFileSystem       AfxDrawSystemGetFileSystem(afxDrawSystem dsys);
AFX afxDrawDriver       AfxDrawSystemGetDriver(afxDrawSystem dsys, afxNat idx);
AFX afxNat              AfxDrawSystemGetDriverCount(afxDrawSystem dsys);
AFX afxNat              AfxDrawSystemGetContextCount(afxDrawSystem dsys, afxNat driverId);

AFX afxResult           AfxDrawSystemEnumerateDrivers(afxDrawSystem dsys, afxNat base, afxNat cnt, afxDrawDriver ddrv[]);
AFX afxResult           AfxDrawSystemEnumerateContexts(afxDrawSystem dsys, afxNat driverId, afxNat base, afxNat cnt, afxDrawContext dctx[]);

AFX afxNat              AfxDrawSystemChooseTextureStorage(afxDrawSystem dsys, afxTexture tex, afxTextureRegion const *rgn);
AFX afxResult           AfxDrawSystemDescribeTextureStorages(afxDrawSystem dsys, afxNat base, afxNat cnt, afxTextureStorageSpecification spec[]);
AFX afxResult           AfxDrawSystemEnumerateTextureStorages(afxDrawSystem dsys, afxNat base, afxNat cnt, afxTextureStorageRegistry *storage[]);
AFX afxResult           AfxDrawSystemRegisterTextureStorages(afxDrawSystem dsys, afxNat cnt, afxTextureStorageSpecification const spec[]);

AFX afxError            _AfxDrawSystemProcess(afxDrawSystem dsys); // Called by core execution system. Reserved for enginners at SIGMA.

#endif//AFX_DRAW_SYSTEM_H
