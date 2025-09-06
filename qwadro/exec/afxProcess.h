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

// Advanced User Experience Extensions for Qwadro

#ifndef AFX_PROCESS_H
#define AFX_PROCESS_H

#include "qwadro/exec/afxThread.h"
#include "qwadro/io/afxUri.h"

AFX_DEFINE_HANDLE(afxProcess);

AFX_DEFINE_STRUCT(afxProcessInfo)
{
    // Path pointing to the program to be executed.
    afxUri      file;
    // Environment for the new process. If NIL the parents environment is used.
    afxChar**   env;
    // Current working directory for the subprocess.
    afxUri      cwd;
    // Command line arguments.args[0] should be the path to the program.
    afxChar**   args;
    // Various flags that control how Spawn() behaves.
    afxFlags    flags;
    afxUnit     ioCnt;
    struct
    {
        afxFlags  flags;
        afxStream iob;
        afxInt    fd;
    }           ios[3];
    // The child process' user Id
    afxSize     uid;
    // The child process' group Id.
    afxSize     gid;
    // Callback called after the process exits.
    void        (*exitCb)(afxProcess, afxInt64 exitCode, afxInt termSignal);
};

AFX afxUnit AfxGetPid(afxProcess);

AFX afxError AfxSpawnProcess(afxUnit reserved, afxProcessInfo const* info, afxUnit cnt, afxProcess procs[]);

#endif//AFX_PROCESS_H
