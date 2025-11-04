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
#include "../../dep/stb_sprintf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <float.h>
#include <Windows.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <windows.h>
#include <intrin.h>
#include <minidumpapiset.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")
//#include <Mmsystem.h>
#pragma comment (lib, "Shlwapi")

#if 0
#include <dwmapi.h>
#pragma comment (lib, "dwmapi")
#endif

#include "src/impl/afxExecImplKit.h"

extern afxString const qwadroSignature;

AFX_DEFINE_STRUCT(afxDebugger)
{
    PVOID           hVecExcHandler;
    afxMutex        mtx;
    afxCondition cond;
    afxBool         isLocked;

    afxUnit          unitLockerIdx;

    afxBool         running;
    void*           output;
    void*           dump;
    HWND            conWnd;
    HANDLE          conOutHnd;
}
debugger =
{
    .mtx = { 0 },
    .cond = { 0 },
    .isLocked = FALSE,

    .unitLockerIdx = -1,

    .running = FALSE,
    .output = NIL,
    .dump = NIL,
    .conWnd = NIL,
    .conOutHnd = INVALID_HANDLE_VALUE
};

AFX_DEFINE_STRUCT(_afxDbgChan)
{
    afxUnit32 fgCol;
    afxUnit32 bgCol;
    afxUnit16 beepFreq;
    afxUnit16 beepDur;
    afxBool8 echo;
    afxBool8 dump;
};

static _afxDbgChan dbgChannels[32] =
{
    {
        .fgCol = 0x000000FF, // grey : comment
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFFFFFFFF, // white : highlight
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0x00ff0000, // green : regular
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFF00FF00, // blue : assistence
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0x00FFFF00, // turquish
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFF00FF00, // purple
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFFFF0000, // yellow
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFFFF00FF, // yellow : advertence (aka warning)
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFF000000, // red dark : assertion
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
    {
        .fgCol = 0xFF0000FF, // red : critical error
        .beepFreq = 440,
        .beepDur = 300,
        .echo = TRUE,
        .dump = TRUE
    },
};

_AFX void AfxResetDebugOutputChannel(afxUnit ch, afxBool echo, afxBool dump)
{
    dbgChannels[ch].echo = echo;
    dbgChannels[ch].dump = dump;
}

_AFX void AfxBeep(afxUnit freq, afxUnit dur)
{
    // Ah, the good old motherboard speaker beep.
#if 0
    // Using ASCII Bell.
    printf("\a"); // ASCII Bell character
    fflush(stdout); // Make sure it's sent immediately
#else
#ifndef _WIN32
    // A true hardware-level beep from the PC speaker.
    // Direct access via '/dev/console' (requires Root).
    int con_fd = open("/dev/console", O_WRONLY);
    if (con_fd == -1)
    {
        perror("open");
        return 1;
    }
    int duration = 1000; // in milliseconds
    int freq = 1000;     // frequency in Hz
    int tone = (1193180 / freq);
    ioctl(con_fd, KIOCSOUND, tone);
    usleep(duration * 1000);
    ioctl(con_fd, KIOCSOUND, 0); // Stop sound
    close(con_fd);
#else
    Beep(440, 300); // frequency in Hz, duration in ms
#endif
#endif
    return;
}

 _AFXINL afxChar const* _AfxDbgTrimFilename(afxChar const* path)
{
    afxChar const* start = (afxChar const*)path, *p = (afxChar const*)path;
    while (*(p)++);
    while (--(p) != start && *(p) != (afxChar)'/' && *(p) != (afxChar)'\\' && *(p) != (afxChar)':');
    return((*(p) == (afxChar)'/' || *(p) == (afxChar)'\\' || *(p) == (afxChar)':') ? (afxChar const*)++p : NIL);
}

_AFXINL afxResult _AfxReportFn(afxUnit ch, afxChar const* msg, afxUnit len)
{
    WORD tattr = 0;
    afxUnit color = 0;

    switch (ch % 10)
    {
    case 0: color = 0x000000FF; break; // grey : comment
    case 1: color = 0xFFFFFFFF; break; // white : highlight
    case 2: color = 0x00ff0000; break; // green : regular
    case 3: color = 0xFF00FF00; break; // blue : assistence
    case 4: color = 0x00FFFF00; break; // turquish
    case 5: color = 0xFF00FF00; break; // purple

    case 6: color = 0xFFFF0000; break; // yellow
    case 7: color = 0xFFFF00FF; break; // yellow : advertence (aka warning)
    case 8: color = 0xFF000000; break; // red dark : assertion
    case 9: color = 0xFF0000FF; break; // red : critical error
    default:break;
    }

    if (color & 0x000000FF)
        tattr |= FOREGROUND_INTENSITY;

    if (color & 0x0000FF00)
        tattr |= FOREGROUND_BLUE;

    if (color & 0x00FF0000)
        tattr |= FOREGROUND_GREEN;

    if (color & 0xFF000000)
        tattr |= FOREGROUND_RED;

    AfxPrint(color, msg);

    if (debugger.dump)
    {
        //fputs(msg, debugger.dump);
        fwrite(msg, 1, len, debugger.dump);
        fflush(debugger.dump);
    }
    return 1;
}

_AFXINL afxResult AfxReportf(afxUnit ch, afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len = 0;

        if ((ch % 10) >= 7)
        {
            // \a = BELL control code
#ifdef AFX_ON_X86_64
            len = stbsp_sprintf(msg2, "%s:%lli?%s\n", _AfxDbgTrimFilename((char const * const)hint[0]), hint[1], (char const * const)hint[2]);
#else
            len = stbsp_sprintf(msg2, "%s:%i?%s\n", _AfxDbgTrimFilename((char const * const)hint[0]), (int)hint[1], (char const * const)hint[2]);
#endif
            //AfxBeep(1, 1);
        }

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxReportFn(ch, msg2, len + len2);
        _AfxDbgUnlock();
    }
    return 0;
}

_AFXINL afxResult AfxReport(afxUnit ch, afxChar const* msg, afxUnit len)
{
    if (_AfxDbgLock())
    {
        _AfxReportFn(ch, msg, len);
        _AfxDbgUnlock();
    }
    return 0;
}

#if 0
void AfxLogCall_(afxHere const hint, afxChar const* args, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg[8192];
        int len;

#ifdef AFX_ON_X86_64
        len = stbsp_sprintf(msg, "%s(", (char const *const)hint[2]);
#else
        len = stbsp_sprintf(msg, "%s(", (char const *const)hint[2]);
#endif

        va_list args2;
        va_start(args2, args);
        int len2 = stbsp_vsprintf(&msg[len], args, args2);
        va_end(args2);
        msg[len + len2] = ')';
        msg[len + len2 + 1] = '\n';
        msg[len + len2 + 2] = '\0';

        _AfxLogFn(0x000000FF, msg);
        _AfxDbgUnlock();
    }
}

void AfxReportComment_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

        if (msg && msg[0])
        {
            len = 0;// stbsp_sprintf(msg2, "\n ");
    }
        else
        {
#ifdef AFX_ON_X86_64
            len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const *const)hint[0]), hint[1]);
#else
            len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1]);
#endif
        }

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFFFFFFFF, msg2);
        _AfxDbgUnlock();
    }
}

void AfxReportMessage_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

        if (msg && msg[0])
        {
            len = 0;// stbsp_sprintf(msg2, "");
        }
        else
        {
#ifdef AFX_ON_X86_64
            len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const *const)hint[0]), hint[1]);
#else
            len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1]);
#endif
        }

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0x00FF0000, msg2);
        _AfxDbgUnlock();
    }
}

void AfxReportHint_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

#ifdef AFX_ON_X86_64
        len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const *const)hint[0]), hint[1]);
#else
        len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1]);
#endif

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFF00FF00, msg2);
        _AfxDbgUnlock();
    }
}

void AfxReportWarn_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

#ifdef AFX_ON_X86_64
        len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const *const)hint[0]), hint[1]);
#else
        len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const *const)hint[0]), (int)hint[1]);
#endif

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFFFF0000, msg2);
        _AfxDbgUnlock();
    }
}

void AfxReportError_(afxHere const hint, afxChar const* msg, ...)
{
    if (_AfxDbgLock())
    {
        afxChar msg2[8192];
        int len;

#ifdef AFX_ON_X86_64
        len = stbsp_sprintf(msg2, "%s:%lli\n", _AfxDbgTrimFilename((char const * const)hint[0]), hint[1]);
#else
        len = stbsp_sprintf(msg2, "%s:%i\n", _AfxDbgTrimFilename((char const * const)hint[0]), (int)hint[1]);
#endif

        int len2;

        if (!(msg && msg[0])) msg2[len] = '\0', len2 = 0;
        else
        {
            va_list args;
            va_start(args, msg);
            len2 = stbsp_vsprintf(&msg2[len], msg, args);
            va_end(args);
        }

        msg2[len + len2] = '\n';
        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFF000000, msg2);
        _AfxDbgUnlock();
    }
}
#endif

LONG WINAPI UnhandledExceptionFilterCb(EXCEPTION_POINTERS *ExceptionInfo)
{
    printf("Unhandled exception caught! Code: 0x%X\n", ExceptionInfo->ExceptionRecord->ExceptionCode);
    
    return EXCEPTION_EXECUTE_HANDLER;
}

LONG CALLBACK VectoredDumpHandlerCb(EXCEPTION_POINTERS *ExceptionInfo)
// When an unhandled exception happens, it creates a timestamped .dmp file and calls MiniDumpWriteDump with 
// MiniDumpWithFullMemory (plus extra useful flags) to produce a full memory dump.
{
    printf("Unhandled exception caught. Code: 0x%X\n", ExceptionInfo->ExceptionRecord->ExceptionCode);

    switch (ExceptionInfo->ExceptionRecord->ExceptionCode)
    {
    case 0x80000003: // STATUS_BREAKPOINT
    case 0x80000004: // STATUS_SINGLE_STEP
    case 0x80000001: // STATUS_GUARD_PAGE_VIOLATION
    {
        // These are expected and caught internally; they don't indicate real errors unless unhandled.
        return EXCEPTION_CONTINUE_SEARCH;
    }
    case 0x40010006: // STATUS_WAS_LOCKED
    {
        /*
            STATUS_WAS_LOCKED
            Code: 0x40010006
            Meaning: "Indicates that a region specified in a Lock request is already locked."
            This code is used in Windows kernel-mode APIs (and some user-mode APIs) related to memory management, file locking, or inter-process synchronization.
        */
        printf("STATUS_WAS_LOCKED. Indicates that a region specified in a Lock request is already locked.\n", ExceptionInfo->ExceptionRecord->ExceptionCode);
        return EXCEPTION_CONTINUE_SEARCH;
    }
    case 0x406D1388:
    {
        /*
            The 0x406D1388 Exception is a trick used to inform Visual Studio of a thread's name, 
            but it tends to cause problems with other debuggers, and the .NET runtime. 
            Note that SDL 2.0.6 and later will still use the (safer) SetThreadDescription API, 
            introduced in the Windows 10 Creators Update, if available.
        */
        return EXCEPTION_CONTINUE_SEARCH;
    }
    default:break;
    }

    // We could analyze ExceptionInfo and decide what to do.

    // Build a timestamped filename
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);
    char filename[MAX_PATH];
    snprintf(filename, sizeof(filename),
        "crash_dump_%04d%02d%02d_%02d%02d%02d_pid%u.dmp",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
        tm.tm_hour, tm.tm_min, tm.tm_sec,
        (unsigned)GetCurrentProcessId());

    HANDLE hFile = CreateFileA(filename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        // can't write file; nothing more we can do here
        return EXCEPTION_CONTINUE_SEARCH;
    }

    // Fill MINIDUMP_EXCEPTION_INFORMATION
    MINIDUMP_EXCEPTION_INFORMATION mei = { 0 };
    mei.ThreadId = GetCurrentThreadId();
    mei.ExceptionPointers = ExceptionInfo;
    mei.ClientPointers = FALSE;

    // Choose flags for "full memory"
    const MINIDUMP_TYPE dumpType =
        (MINIDUMP_TYPE)(MiniDumpWithFullMemory |
            MiniDumpWithHandleData |
            MiniDumpWithFullMemoryInfo |
            MiniDumpWithThreadInfo |
            MiniDumpWithUnloadedModules);

    // Write the dump
    BOOL ok = MiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        dumpType,
        &mei,      // exception info
        NULL,      // user stream callback
        NULL       // callback
    );

    DWORD err = 0;
    if (!ok) err = GetLastError();

    // Best-effort flush and close
    FlushFileBuffers(hFile);
    CloseHandle(hFile);

    // Optionally log to stderr (may not always succeed during crash)
    if (ok) {
        fprintf(stderr, "Crash dump filed as: %s\n", filename);
    }
    else {
        fprintf(stderr, "MiniDumpWriteDump failed: 0x%08X\n", err);
    }

    // Return EXCEPTION_CONTINUE_EXECUTION if we fix the problem and want execution to continue.
    // Return EXCEPTION_CONTINUE_SEARCH to let the next handler in the chain handle the exception.
    // Return EXCEPTION_CONTINUE_SEARCH so system handlers/other handlers run.
    // We do not try to continue execution.
    return EXCEPTION_CONTINUE_SEARCH;
}

_AFXINL afxResult _AfxDbgUnlock(void)
{
    afxResult rslt;

    if (!debugger.running) rslt = 0;
    else
    {
        //if (!(rslt = AfxLockMutex(&(debugger.mtx)))) printf("couldn't unlock mutex");
        //else
        {
            debugger.isLocked = FALSE;
            AfxSignalCondition(&debugger.cond);
            rslt = AfxUnlockMutex(&(debugger.mtx));
            afxSystem sys;

            if (AfxGetSystem(&sys))
            {
                afxUnit unitUnlockerIdx = AfxGetTid();
                //afxError err;
                //AFX_ASSERT(unitUnlockerIdx == debugger.unitLockerIdx);
            }
        }
    }
    return rslt;
}

_AFXINL afxResult _AfxDbgLock(void)
{
    afxResult rslt;

    if (!debugger.running) rslt = 0;
    else
    {
        if (!(rslt = !AfxLockMutex(&(debugger.mtx)))) printf("couldn't lock mutex");
        else
        {
            afxUnit oldUnitIdx = debugger.unitLockerIdx;
            afxSystem sys;
            
            if (AfxGetSystem(&sys))
            {
                debugger.unitLockerIdx = AfxGetTid();

                if (oldUnitIdx != debugger.unitLockerIdx)
                {
                    afxChar msg2[512];
                    stbsp_sprintf(msg2, "Entering the Thread Execution Environment %2u\n", debugger.unitLockerIdx);
                    //AfxReportf(6, NIL, msg2);
                }
            }

            debugger.isLocked = (afxBool)TRUE;
            //rslt = AfxUnlockMutex(&(debugger.mtx));
        }
    }
    return rslt;
}

_AFXINL afxBool _AfxDbgIsAttached(void)
{
    return debugger.running;
}

_AFXINL afxResult _AfxDbgDetach(void)
{
    if (_AfxDbgLock())
    {
        AfxReleaseConsole();

        if (debugger.dump)
        {
            //fclose(dbgCfg.dump);
            debugger.dump = NIL;
        }

        debugger.running = 0;
        AfxDismantleCondition(&(debugger.cond));
        AfxDismantleMutex(&(debugger.mtx));

        // Remove handler
        if (debugger.hVecExcHandler)
            RemoveVectoredExceptionHandler(debugger.hVecExcHandler);
    }
    return 0;
}

void sigHandler(int s)
{
    switch (s)
    {
    case SIGTERM:
    {
        _AfxReportFn(9, "\aEXCEPTION: Termination request sent to the program.", 0);
        break;
    }
    case SIGSEGV:
    {
        _AfxReportFn(9, "\aEXCEPTION: Invalid memory access (segmentation fault).", 0);
        break;
    }
    case SIGINT:
    {
        _AfxReportFn(9, "\aEXCEPTION: External interrupt, usually initiated by the user.", 0);
        break;
    }
    case SIGILL:
    {
        _AfxReportFn(9, "\aEXCEPTION: Invalid program image, such as invalid instruction.", 0);
        break;
    }
    case SIGABRT:
    case SIGABRT_COMPAT:
    {
        _AfxReportFn(9, "\aEXCEPTION: Abnormal termination condition, as is e.g. initiated by abort().", 0);
        break;
    }
    case SIGFPE:
    {
        _AfxReportFn(9, "\aEXCEPTION: Erroneous arithmetic operation such as divide by zero.", 0);
        break;
    }
    default: break;
    }
}

_AFXINL afxResult _AfxDbgAttach(afxChar const* file)
{
    (void)file;
    afxError err = AFX_ERR_NONE;
    
    AfxBeep(0, 0);

    if (debugger.running)
    {
        AfxThrowError();
    }
    else
    {
        //SetUnhandledExceptionFilter(UnhandledExceptionFilterCb);
        // Register vectored handler (place at front)
        debugger.hVecExcHandler = AddVectoredExceptionHandler(1, VectoredDumpHandlerCb);
        if (!debugger.hVecExcHandler) {
            fprintf(stderr, "AddVectoredExceptionHandler failed\n");
        }
        else {
            fprintf(stderr, "Crash dump handler installed.\n");
        }

        AfxDeployMutex(&(debugger.mtx), AFX_MTX_RECURSIVE);
        AfxDeployCondition(&(debugger.cond));
        debugger.isLocked = TRUE;

        signal(SIGTERM, sigHandler);
        signal(SIGSEGV, sigHandler);
        signal(SIGINT, sigHandler);
        signal(SIGILL, sigHandler);
        signal(SIGABRT, sigHandler);
        signal(SIGFPE, sigHandler);

#ifdef AVX_FLOATING_POINT_VALIDATION_ENABLED
        unsigned int u;
        u = _controlfp(0, 0);
        u = u & ~(_EM_INVALID | /*_EM_DENORMAL |*/ _EM_ZERODIVIDE | _EM_OVERFLOW /*| _EM_UNDERFLOW  | _EM_INEXACT*/);
        _controlfp(u, _MCW_EM);
#endif

        afxChar path[1024];

        if (file)
        {
            if (file[0])
            {
                AfxSprintf(path, "%s", file);
                //AfxPopulateString(path, file);

                if (!(debugger.dump = fopen(path, "w+")))
                    AfxThrowError();
            }
        }
        else
        {
            afxChar sys[2048], exeName[2048];
            GetModuleFileNameA(NIL, exeName, sizeof(exeName));
            PathStripPathA(exeName);
            
            GetModuleFileNameA(GetModuleHandleA("e2coree.dll"), sys, sizeof(sys));
            PathRemoveFileSpecA(sys);
            
            time_t t;
            time(&t);
            struct tm * ti;
            ti = localtime(&t);
            
            AfxSprintf(path, "%s(%04i%02i%02i-%02i%02i%02i).log", exeName, 1900 + ti->tm_year, ti->tm_mon, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
            
            if (!(debugger.dump = fopen(path, "w+"))) AfxThrowError();
            else
            {
                //fclose(debugger.dump);

                //if (!(debugger.dump = fopen(path, "w+")))
                    //AfxThrowError();
            }
        }

        if (debugger.dump || (!file || !file[0]))
        {
            //setbuf(debugger.dump);
            debugger.running = 1;

#if 0
            if (!AfxAttachDebugTerminal()) AfxThrowError();
            else
            {
                short fontSizX = 16, fontSizY = 16;
                CONSOLE_FONT_INFOEX cfi = { .cbSize = sizeof(cfi), .nFont = 0, .dwFontSize = { fontSizX, fontSizY }, .FontFamily = FF_DONTCARE, .FontWeight = FW_NORMAL };
                wcscpy(cfi.FaceName, L"Consolas");
                SetCurrentConsoleFontEx(debugger.conOutHnd, FALSE, &cfi);
                COORD coord = GetLargestConsoleWindowSize(debugger.conOutHnd);
                
                MoveWindow(debugger.conWnd, -4, -4, 80 * fontSizX, coord.Y * fontSizY, TRUE);
                //ShowScrollBar(debugger.conWnd, SB_BOTH, FALSE);

                coord.X = 80;
                coord.Y = 5000;
                SetConsoleScreenBufferSize(debugger.conOutHnd, coord);

                SetConsoleTextAttribute(debugger.conOutHnd, (WORD)(~(MAXUINT16) | FOREGROUND_RED | FOREGROUND_GREEN));

                for (afxUnit i = 0; i < 1120; i++)
                    AfxReportf(0xFFFF0000, "%.*s", 1, &qwadroSignature.start[i * 1]);

                Sleep(1000);

                SetConsoleTextAttribute(debugger.conOutHnd, (WORD)(~(MAXUINT16) | FOREGROUND_INTENSITY));
                _AfxDbgUnlock();
                return TRUE;
            }
#else
            if (AfxReacquireConsole())
            {
                //_AfxDbgUnlock();


                {
                    int cpuInfo[4];
                    char name[49] = { 0 };

                    for (int i = 0; i < 3; i++)
                    {
                        __cpuid(cpuInfo, 0x80000002 + i);
                        memcpy(name + i * 16, cpuInfo, sizeof(cpuInfo));
                    }

                    printf("%s\n", name);
                }

                {
                    int cpuInfo[4] = { 0 };
                    char vendor[13] = { 0 };

                    __cpuid(cpuInfo, 0);
                    *((int *)&vendor[0]) = cpuInfo[1];  // EBX
                    *((int *)&vendor[4]) = cpuInfo[3];  // EDX
                    *((int *)&vendor[8]) = cpuInfo[2];  // ECX

                    int cpuInfo1[4] = { 0 };
                    __cpuid(cpuInfo1, 1); // Function 1 gives feature + model info.

                    int stepping = cpuInfo1[0] & 0xF;
                    int model = (cpuInfo1[0] >> 4) & 0xF;
                    int family = (cpuInfo1[0] >> 8) & 0xF;
                    int ext_model = (cpuInfo1[0] >> 16) & 0xF;
                    int ext_family = (cpuInfo1[0] >> 20) & 0xFF;

                    int m = model, f = family;
                    if (family == 6 || family == 15)
                        m = model + (ext_model << 4);
                    if (family == 15)
                        f = family + ext_family;

                    printf(" %s F%d/XF%d/FXF%d/M%d/XM%d/MXM%d/S%d\n", vendor, family, ext_family, f, model, ext_model, m, stepping);
                }

                {
                    int cpuInfo[4];
                    __cpuid(cpuInfo, 1);

                    printf(" CPU features:");
                    if (cpuInfo[3] & (1 << 4))   printf(" TSC,");
                    if (cpuInfo[3] & (1 << 23))  printf(" MMX,");
                    if (cpuInfo[3] & (1 << 25))  printf(" SSE,");
                    if (cpuInfo[3] & (1 << 26))  printf(" SSE2,");
                    if (cpuInfo[2] & (1 << 0))   printf(" SSE3,");
                    if (cpuInfo[2] & (1 << 9))   printf(" SSSE3,");
                    if (cpuInfo[2] & (1 << 19))  printf(" SSE4.1,");
                    if (cpuInfo[2] & (1 << 20))  printf(" SSE4.2,");
                    if (cpuInfo[2] & (1 << 28))  printf(" AVX,");
                    if (cpuInfo[2] & (1 << 3))   printf(" MONITOR/MWAIT,");

                    // Check extended features.
                    int regs[4];
                    __cpuidex(regs, 7, 0);
                    //printf("* CPU features (Extended)\n");
                    if (regs[1] & (1 << 5)) printf(" AVX2,");
                    if (regs[1] & (1 << 3)) printf(" BMI1,");
                    if (regs[1] & (1 << 8)) printf(" BMI2,");
                    if (regs[1] & (1 << 7)) printf(" SMEP,");
                    if (regs[1] & (1 << 2)) printf(" SGX,");

                    printf("\b \n"); // remove trailing comma
                }

                {
                    SYSTEM_INFO sysInfo;
                    GetSystemInfo(&sysInfo);
                    printf(" CPU cores/threads: %lu/%lu\n", sysInfo.dwNumberOfProcessors, sysInfo.dwNumberOfProcessors);

                    /*
                        RelationProcessorCore
                        0
                        Retrieves information about logical processors that share a single processor core.
                        RelationNumaNode
                        1
                        Retrieves information about logical processors that are part of the same NUMA node.
                        RelationCache
                        2
                        Retrieves information about logical processors that share a cache.
                        RelationProcessorPackage
                        3
                        Retrieves information about logical processors that share a physical package.
                        RelationGroup
                        4
                        Retrieves information about logical processors that share a processor group.
                        RelationProcessorDie
                        5
                        Retrieves information about logical processors that share a processor die.
                        RelationNumaNodeEx
                        6
                        Retrieves information about logical processors that are part of the same NUMA node (with full affinity).
                        RelationProcessorModule
                        7
                        Retrieves information about logical processors that share a processor module.
                        RelationAll
                        0xffff
                        Retrieves information about logical processors for all relationship types (cache, NUMA node, processor core, physical package, processor group, processor die, and processor module).
                    */

                    SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX slpi[1];
                    //GetLogicalProcessorInformationEx(0xffff, slpi, sizeof(slpi));

                    // Note: For physical core count, we'd need to use GetLogicalProcessorInformation().
                }

                {
                    int regs[4];
                    __cpuid(regs, 0x16);
                    printf(" CPU Base Freq %d MHz; Max Freq %d MHz; Bus (Ref) Freq %d MHz\n", regs[0], regs[1], regs[2]);
                    
                    if (regs[0] == 0)
                        printf("Not available (function 0x16 unsupported)\n\n");

                    LARGE_INTEGER freq;
                    if (QueryPerformanceFrequency(&freq))
                    {
                        printf(" CPU Approx. base freq: %.2f MHz\n", freq.QuadPart / 1e6);
                    }
                    else
                    {
                        printf(" CPU frequency not available.\n");
                    }
                }

                {
                    int regs[4];
                    for (int i = 0; ; ++i)
                    {
                        __cpuidex(regs, 4, i);
                        int cache_type = regs[0] & 0x1F;
                        if (cache_type == 0) break; // No more caches

                        const char* level_str = (regs[0] >> 5) & 0x7 ? "Unified" : "Data/Instruction";
                        int level = (regs[0] >> 5) & 0x7;
                        int line_size = (regs[1] & 0xFFF) + 1;
                        int partitions = ((regs[1] >> 12) & 0x3FF) + 1;
                        int ways = ((regs[1] >> 22) & 0x3FF) + 1;
                        int sets = regs[2] + 1;
                        int size_kb = ways * partitions * line_size * sets / 1024;

                        printf(" L%d Cache: %d KB (%s)\n", level, size_kb, level_str);
                    }
                    printf("\n");
                }

                return TRUE;
            }
#endif

            debugger.running = 0;

            if (debugger.dump)
            {
                fclose(debugger.dump);
                debugger.dump = NIL;
            }
        }

        AfxDismantleCondition(&(debugger.cond));
        AfxDismantleMutex(&(debugger.mtx));
    }
    return FALSE;
}

_AFXINL void AfxCatchError_(afxError err_, afxHere const hint)
{
    afxError err = NIL;
    AFX_ASSERT(err_);
    AFX_ASSERT(hint);

    if (err_)
    {
        AfxReportError("");
        afxInt a = 0;
    }
}

_AFX void AfxRaiseException(afxError errCode, afxHere const hint)
{
    afxError err = NIL;
    AFX_ASSERT(errCode);
    AFX_ASSERT(hint);

    if (errCode)
    {
        //AfxReportError("");
        afxInt a = 0;
    }
}
