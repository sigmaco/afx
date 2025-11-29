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

#ifndef AFX_SYSTEM_DDK_H
#define AFX_SYSTEM_DDK_H

#include "afxDeviceDDK.h"
#include "afxModuleDDK.h"
#include "afxServiceDDK.h"
#include "afxThreadingDDK.h"
#include "../io/afxIoDDK.h"

#ifdef _AFX_SYSTEM_C
AFX_OBJECT(afxSystem)
{
    afxBool         isInBootUp;
    afxBool         isInShutdown;
    afxBool         operating;
    afxBool         interruptionRequested;
    afxInt          exitCode;

    afxChain        classes;
    afxClass        mmuCls;
    afxClass        strbCls;
    afxClass        mdleCls;
    afxClass        procCls;
    afxClass        thrCls;
    afxClass        thrpCls;
    afxClass        svcCls;
    afxClass        devCls;
    afxClass        cdcCls;
    afxClass        iosCls;
    afxClass        fsysCls;
    afxClass        exuCls;

    afxUnit         ptrSiz;
    afxBool         bigEndian;
    afxUnit         memPageSiz; // The page size and the granularity of page protection and commitment.
    afxUnit         allocGranularity;
    afxUnit         hwThreadingCap; // # of logical proc units (hardware threads)
    afxReal         unitsPerMeter; // the number of units in a meter.
    afxSize         maxMemUsage;
    afxUnit         ioBufSiz;
    afxIoBridge     primeExu;
    afxUnit32       primeTid;
    afxThread       primeThr;
    afxBool         strictStorage;
    afxAssertHook           assertHook; // external assertion handling function (optional)
    afxReallocatorFn        reallocatorFn;
    afxProfilerPushTimerFn  profilerPushTimer; // external (optional) function for tracking performance of the system that is called when a timer starts. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPopTimerFn   profilerPopTimer; // external (optional) function for tracking performance of the system that is called when a timer stops. (only called in Debug and Profile binaries; this is not called in Release)
    afxProfilerPostMarkerFn profilerPostMarker; // external (optional) function for tracking significant events in the system, to act as a marker or bookmark. (only called in Debug and Profile binaries; this is not called in Release)

    afxUri2048              pwd; // process working dir (usually abs/path/to/qwadro/system/$(host)/)
    afxUri2048              qwd; // root dir for Qwadro forked from pwd (usually qwadro/system/$(host)/../../)

    afxStorage              defStops[9]; // [ ., system/$(host)d, system/$(host), system, code, sound, data, art, tmp ]

    afxModule               e2coree;

    // avx
    afxBool             avxDisabled;
    afxModule           e2drawDll;
    afxClass            ddevCls;
    afxClass            dsysCls;
    afxChain            avxIcdChain;
    afxError            (*avxIoctl)(afxSystem, afxModule, afxUnit, void*);

    // amx
    afxBool             amxDisabled;
    afxModule           e2mixDll;
    afxClass            asiCls;
    afxClass            mdevCls;
    afxClass            msysCls;
    afxChain            amxIcdChain;
    afxError            (*amxIoctl)(afxSystem, afxModule, afxUnit, void*);
    
    // asx
    afxBool             asxDisabled;
    afxModule           e2simDll;
    afxClass            sdevCls;
    afxClass            simCls;
    afxChain            asxIcdChain;
    afxError            (*asxIoctl)(afxSystem, afxModule, afxUnit, void*);
    
    // aux (shell)
    afxBool             auxDisabled;
    afxModule           e2mmuxDll;
    afxChain            auxIcdChain;
    afxClass            vduCls;
    afxClass            sshCls;
    afxClass            sesCls;
    afxClass            hidCls;
    afxError            (*auxIoctl)(afxSystem, afxModule, afxUnit, void*);

    struct
    {
        struct
        {
            afxFcc          type;
            afxChain        resources;
        }                   supplyChain[1];
    }                       resourcing;
};
#endif//_AFX_SYSTEM_C

AFX afxClassConfig const _AFX_STRB_CLASS_CONFIG;
AFX afxClassConfig const _AFX_MMU_CLASS_CONFIG;
AFX afxClassConfig const _AFX_CDC_CLASS_CONFIG;
AFX afxClassConfig const _AFX_IOB_CLASS_CONFIG;
AFX afxClassConfig const _AFX_FSYS_CLASS_CONFIG;
AFX afxClassConfig const _AFX_EXU_CLASS_CONFIG;

AFX afxClass const*     _AfxSysGetIobClass(afxSystem sys);
AFX afxClass const*     _AfxSysGetFsysClass(afxSystem sys);
AFX afxClass const*     _AfxSysGetMmuClass(afxSystem sys);
AFX afxClass const*     _AfxSysGetMdleClass(afxSystem sys);
AFX afxClass const*     _AfxSysGetSvcClass(afxSystem sys);
AFX afxClass const*     _AfxSysGetStrbClass(afxSystem sys);
AFX afxClass const*     _AfxSysGetProcClass(afxSystem sys);
AFX afxClass const*     _AfxSysGetDevClass(afxSystem sys);
AFX afxClass const*     _AfxSysGetExuClass(afxSystem sys);

AFX afxError _AfxAttachCoreModule(afxModule* e2coree);
AFX afxError _AfxAttachTargaModule(afxModule* e2targa);
AFX afxError _AfxAttachMixModule(afxModule* e2mixva);
AFX afxError _AfxAttachUxModule(afxModule* e2mmux);
AFX afxError _AfxAttachSimModule(afxModule* e2synerg);

#endif//AFX_SYSTEM_DDK_H
