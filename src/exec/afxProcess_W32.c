/*
 *           ::::::::    :::::::::::    ::::::::    ::::     ::::       :::
 *          :+:    :+:       :+:       :+:    :+:   +:+:+: :+:+:+     :+: :+:
 *          +:+              +:+       +:+          +:+ +:+:+ +:+    +:+   +:+
 *          +#++:++#++       +#+       :#:          +#+  +:+  +#+   +#++:++#++:
 *                 +#+       +#+       +#+   +#+#   +#+       +#+   +#+     +#+
 *          #+#    #+#       #+#       #+#    #+#   #+#       #+#   #+#     #+#
 *           ########    ###########    ########    ###       ###   ###     ###
 *
 *                     S I G M A   T E C H N O L O G Y   G R O U P
 *
 *                                   Public Test Build
 *                               (c) 2017 SIGMA FEDERATION
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>

#define _AFX_CORE_C
#include "afxSystemDDK.h"

AFX_OBJECT(afxProcess)
{
    PROCESS_INFORMATION pi;
    STARTUPINFO si;
};

void CreateIsolatedProcess(const char *application)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    HANDLE hJob;

    // Initialize security attributes
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = FALSE;
    sa.lpSecurityDescriptor = NULL;

    // Create a Job Object to manage process resources
    hJob = CreateJobObject(&sa, NULL);
    if (hJob == NULL) {
        AfxReport(0, "CreateJobObject failed: %d\n", GetLastError());
        return;
    }

    // Set job limits (e.g., limit the number of processes)
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_JOB_MEMORY;
    jeli.JobMemoryLimit = 50 * 1024 * 1024; // Example: limit to 50 MB

    if (!SetInformationJobObject(hJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
    {
        AfxReport(0, "SetInformationJobObject failed: %d\n", GetLastError());
        return;
    }

    // Initialize the STARTUPINFO structure
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;

    // Initialize PROCESS_INFORMATION structure
    ZeroMemory(&pi, sizeof(pi));

    // Create the isolated process
    if (!CreateProcess(NULL, (LPSTR)application, NULL, NULL, FALSE, CREATE_SUSPENDED | CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        AfxReport(0, "CreateProcess failed: %d\n", GetLastError());
        return;
    }

    // Assign the process to the job object
    if (!AssignProcessToJobObject(hJob, pi.hProcess)) {
        AfxReport(0, "AssignProcessToJobObject failed: %d\n", GetLastError());
        return;
    }

    // Resume the process
    ResumeThread(pi.hThread);

    // Wait for the process to finish
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Clean up handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hJob);
}

_AFX afxError _AfxProcCtorCb(afxProcess proc, void** args, afxUnit invokeNo)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PROC, 1, &proc);

    afxShell sh = args[0];
    AFX_ASSERT_OBJECTS(afxFcc_USYS, 1, &sh);
    AFX_ASSERT(args[1]);
    afxProcessInfo const* info = ((afxProcessInfo const*)args[1]) + invokeNo;

    return err;
}

_AFX afxError _AfxProcDtorCb(afxProcess proc)
{
    afxError err = { 0 };
    AFX_ASSERT_OBJECTS(afxFcc_PROC, 1, &proc);

    return err;
}

_AFX afxClassConfig const _AFX_PROC_CLASS_CONFIG =
{
    .fcc = afxFcc_PROC,
    .name = "Process",
    .desc = "Process",
    .fixedSiz = sizeof(AFX_OBJECT(afxProcess)),
    .ctor = (void*)_AfxProcCtorCb,
    .dtor = (void*)_AfxProcDtorCb,
    //.event = (void*)_AfxXssEvent
};

_AFX afxError AfxSpawnProcess(afxUnit reserved, afxProcessInfo const* info, afxUnit cnt, afxProcess procs[])
{
    afxError err = { 0 };
    AFX_ASSERT(info);

    afxSystem sys;
    AfxGetSystem(&sys);
    AFX_ASSERT_OBJECTS(afxFcc_SYS, 1, &sys);
    afxClass* cls = (afxClass*)_AfxSysGetProcClass(sys);
    AFX_ASSERT_CLASS(cls, afxFcc_PROC);

    if (AfxAcquireObjects(cls, cnt, (afxObject*)procs, (void const*[]) { sys, (void*)info, }))
    {
        AfxThrowError();
        return err;
    }
    else
    {
        AFX_ASSERT_OBJECTS(afxFcc_PROC, cnt, procs);
    }

    return err;
}
