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

// This code is part of SIGMA GL/2 <https://sigmaco.org/gl>

#ifndef AFX_MODULE_DDK_H
#define AFX_MODULE_DDK_H

#include "qwadro/exec/afxSystem.h"
#include "../exec/afxThreadingDDK.h"

#ifdef _AFX_MODULE_C
AFX_OBJECT(afxModule)
{
    afxChain            classes;
    afxModuleFlags      flags;
    afxUri128           path;
    void*               osHandle;
    afxBool             demangle;

    afxManifest         etc;
    afxChain            devices; // if it is a ICD, this chain interlinks each device provided by this driver.
    afxBool             attached;

    struct
    {
        afxError(*scmHookFn)(afxModule, afxUri const*); // initializes system-wide data structures and resources.
        afxError(*icdHookFn)(afxModule, afxUri const*); // initializes driver-wide data structures and resources.
        afxError(*icdUnloadFn)(afxModule); // performs any operations that are necessary before the system unloads the driver.
        afxByte icdCache[64];
        void* icdData;

        afxLink avx; // ICD attachment for AVX implementations
        afxLink amx; // ICD attachment for AMX implementations
        afxLink asx; // ICD attachment for ASX implementations
        afxLink aux; // ICD attachment for AUX implementations

        afxClass ddevCls;
        afxClass dsysCls;

        afxClass sdevCls;
        afxClass dinCls;
        afxClass simCls;

        afxClass asiCls;
        afxClass mcdcCls;
        afxClass mdevCls;
        afxClass msysCls;

        afxClass sshCls;
        afxClass vduCls;
        afxClass sesCls;
        afxClass hidCls;
        afxError(*getVduClsc)(afxModule icd, afxString const* tool, afxClassConfig* clsc);
        afxError(*getDoutClsc)(afxDrawSystem dsys, afxString const* tool, afxClassConfig* clsc);
        afxError(*getSinkClsc)(afxMixSystem msys, afxString const* tool, afxClassConfig* clsc);
    } icd;

    afxString           description;
    afxUnit             verMajor, verMinor, verPatch, verBuild;
    afxString           product;
    afxString           vendor;
    afxString           legal;
};
#endif//_AFX_MODULE_C

#if 0
#ifdef _AFX_DRIVER_C
AFX_OBJECT(afxDriver)
{
    AFX_OBJ(afxModule)  mdle;
    afxManifest         etc;
    afxChain            devices;
    afxFlags            flags;

    afxError            (*icdHookFn)(afxDriver, afxUri const*);

    afxString           description;
    afxString           version;
    afxString           product;
    afxString           vendor;
    afxString           legal;
};
#endif//_AFX_DRIVER_C
#endif

AFX afxClassConfig const _AFX_MDLE_CLASS_CONFIG;

#endif//AFX_MODULE_DDK_H
