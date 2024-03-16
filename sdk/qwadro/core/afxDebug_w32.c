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
 *                       (c) 2017 SIGMA, Engineering In Technology
 *                             <https://sigmaco.org/qwadro/>
 */

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN 1
#include "../dep/stb_sprintf.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <Shlwapi.h>
//#include <Mmsystem.h>
#pragma comment (lib, "Shlwapi")

#if 0
#include <dwmapi.h>
#pragma comment (lib, "dwmapi")
#endif

#include "qwadro/core//afxSystem.h"
#include "qwadro/core/afxDebug.h"
#include "qwadro/async/afxThread.h"
#include "qwadro/async/afxCondition.h"
#include "qwadro/core/afxObject.h"
#include "qwadro/core/afxString.h"

extern afxString const qwadroSignature;

AFX_DEFINE_STRUCT(afxDebugger)
{
    afxMutex        mtx;
    AfxCondition         cond;
    afxBool         isLocked;

    afxNat          unitLockerIdx;

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

AFXINL afxResult _AfxLogFn(afxNat32 color, afxChar const* msg)
{
    WORD tattr = 0;

    if (color & 0x000000FF)
        tattr |= FOREGROUND_INTENSITY;

    if (color & 0x0000FF00)
        tattr |= FOREGROUND_BLUE;

    if (color & 0x00FF0000)
        tattr |= FOREGROUND_GREEN;

    if (color & 0xFF000000)
        tattr |= FOREGROUND_RED;

    if (debugger.output)
    {
        SetConsoleTextAttribute(debugger.conOutHnd, tattr);
        fprintf(debugger.output, "%s", msg);
        fflush(debugger.output);
    }

    if (debugger.dump)
    {
        fprintf(debugger.dump, "%s", msg);
        fflush(debugger.dump);
    }
    return 1;
}

void AfxLogCall(afxHint const hint, afxChar const* args, ...)
{
    if (AfxLockDebugger())
    {
        afxChar msg[8192];
        int len;

#ifdef AFX_PLATFORM_64
        len = stbsp_sprintf(msg, "\n %s:%lli\n  %s(", AfxFindPathTarget((char const *const)hint[0]), hint[1], (char const *const)hint[2]);
#else
        len = stbsp_sprintf(msg, "\n %s:%i\n  %s(",  AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
#endif

        va_list args2;
        va_start(args2, args);
        int len2 = stbsp_vsprintf(&msg[len], args, args2);
        va_end(args2);
        msg[len + len2] = ')';
        msg[len + len2 + 1] = '\0';

        _AfxLogFn(0x000000FF, msg);
        AfxUnlockDebugger();
    }
}

void AfxLogComment(afxHint const hint, afxChar const* msg, ...)
{
    if (AfxLockDebugger())
    {
        afxChar msg2[8192];
        int len;

        if (msg && msg[0])
        {
            len = stbsp_sprintf(msg2, "\n ");
    }
        else
        {
#ifdef AFX_PLATFORM_64
            len = stbsp_sprintf(msg2, "\n ... %s:%lli?%s\n  ", AfxFindPathTarget((char const *const)hint[0]), hint[1], (char const *const)hint[2]);
#else
            len = stbsp_sprintf(msg2, "\n ... %s:%i?%s\n  ", AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
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

        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFFFFFFFF, msg2);
        AfxUnlockDebugger();
    }
}

void AfxLogEcho(afxHint const hint, afxChar const* msg, ...)
{
    if (AfxLockDebugger())
    {
        afxChar msg2[8192];
        int len;

        if (msg && msg[0])
        {
            len = stbsp_sprintf(msg2, "\n ");
        }
        else
        {
#ifdef AFX_PLATFORM_64
            len = stbsp_sprintf(msg2, "\n SUC %s:%lli?%s\n  ", AfxFindPathTarget((char const *const)hint[0]), hint[1], (char const *const)hint[2]);
#else
            len = stbsp_sprintf(msg2, "\n SUC %s:%i?%s\n  ", AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
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

        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0x00FF0000, msg2);
        AfxUnlockDebugger();
    }
}

void AfxLogAssistence(afxHint const hint, afxChar const* msg, ...)
{
    if (AfxLockDebugger())
    {
        afxChar msg2[8192];
        int len;

#ifdef AFX_PLATFORM_64
        len = stbsp_sprintf(msg2, "\n [$] %s:%lli?%s\n  ", AfxFindPathTarget((char const *const)hint[0]), hint[1], (char const *const)hint[2]);
#else
        len = stbsp_sprintf(msg2, "\n [$] %s:%i?%s\n  ", AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
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

        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFF00FF00, msg2);
        AfxUnlockDebugger();
    }
}

void AfxLogAdvertence(afxHint const hint, afxChar const* msg, ...)
{
    if (AfxLockDebugger())
    {
        afxChar msg2[8192];
        int len;

#ifdef AFX_PLATFORM_64
        len = stbsp_sprintf(msg2, "\n [!] %s:%lli?%s\n  ", AfxFindPathTarget((char const *const)hint[0]), hint[1], (char const *const)hint[2]);
#else
        len = stbsp_sprintf(msg2, "\n [!] %s:%i?%s\n  ", AfxFindPathTarget((char const *const)hint[0]), (int)hint[1], (char const *const)hint[2]);
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

        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFFFF0000, msg2);
        AfxUnlockDebugger();
    }
}

void AfxLogError(afxHint const hint, afxChar const* msg, ...)
{
    if (AfxLockDebugger())
    {
        afxChar msg2[8192];
        int len;

#ifdef AFX_PLATFORM_64
        len = stbsp_sprintf(msg2, "\n ERR %s:%lli?%s\n  ", AfxFindPathTarget((char const * const)hint[0]), hint[1], (char const * const)hint[2]);
#else
        len = stbsp_sprintf(msg2, "\n ERR %s:%i?%s\n  ", AfxFindPathTarget((char const * const)hint[0]), (int)hint[1], (char const * const)hint[2]);
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

        msg2[len + len2 + 1] = '\0';

        _AfxLogFn(0xFF000000, msg2);
        AfxUnlockDebugger();
    }
}

afxResult AfxLogMessageFormatted(afxNat32 color, afxChar const* msg, ...)
{
    if (AfxLockDebugger())
    {
        va_list args;
        va_start(args, msg);
        afxChar msg2[4096];
        stbsp_vsprintf(msg2, msg, args);
        va_end(args);

        _AfxLogFn(color, msg2);
        AfxUnlockDebugger();
    }
    return 0;
}

afxResult AfxLogMessage(afxNat32 color, afxChar const* msg)
{
    if (AfxLockDebugger())
    {
        if (!color)
            color = 0x000000FF;

        _AfxLogFn(color, msg);
        AfxUnlockDebugger();
        return 1;
    }
    return 0;
}

_AFXEXPORT afxResult AfxDetachDebugTerminal(void)
{
    AfxEntry("");

    if (AfxLockDebugger())
    {
        if (debugger.conOutHnd != INVALID_HANDLE_VALUE)
        {
            //CloseHandle(debugger.conOutHnd);
            debugger.conOutHnd = INVALID_HANDLE_VALUE;
        }

        if (debugger.conWnd)
        {
            //CloseHandle(debugger.conWnd);
            debugger.conWnd = NIL;
            //FreeConsole();
        }

        if (debugger.output)
        {
            //fclose(debugger.output);
            debugger.output = NIL;
        }

        AfxUnlockDebugger();
    }
    return 0;
}

_AFXEXPORT afxResult AfxAttachDebugTerminal(void)
{
    AfxEntry("");
    afxResult rslt = 0;
    afxError err = AFX_ERR_NONE;

    if (AfxLockDebugger())
    {
        //FreeConsole();

        debugger.conWnd = GetConsoleWindow();

        if (INVALID_HANDLE_VALUE == debugger.conWnd || NULL == debugger.conWnd)
            if (!AllocConsole())
                AfxThrowError();

        debugger.conWnd = GetConsoleWindow();

        if (!debugger.conWnd) AfxThrowError();
        else
        {
#if 0
            DWM_BLURBEHIND bb = { 0 };
            bb.dwFlags = DWM_BB_ENABLE;
            bb.fEnable = TRUE;
            bb.hRgnBlur = NULL;
            DwmEnableBlurBehindWindow(debugger.conWnd, &(bb));
#endif
            debugger.output = freopen("CONOUT$", "w", stdout);

            if (!debugger.output) AfxThrowError();
            else
            {
                //setbuf(debugger.output);
                debugger.conOutHnd = GetStdHandle(STD_OUTPUT_HANDLE);

                afxChar buf[1024];
                stbsp_sprintf(buf, "Qwadro Execution Ecosystem (c) 2017 SIGMA Technology Group --- Public Test Build");
                SetConsoleTitleA(buf);
                rslt = 1;
            }
        }
        AfxUnlockDebugger();
    }
    return rslt;
}

afxResult AfxUnlockDebugger(void)
{
    afxResult rslt;

    if (!debugger.running) rslt = 0;
    else
    {
        //if (!(rslt = AfxLockMutex(&(debugger.mtx)))) printf("couldn't unlock mutex");
        //else
        {
            debugger.isLocked = FALSE;
            rslt = AfxUnlockMutex(&(debugger.mtx));
            AfxSignalSingleCondition(&debugger.cond);
            afxSystem sys;

            if ((sys = AfxGetSystem()))
            {
                afxNat unitUnlockerIdx;
                AfxGetThreadingId(&unitUnlockerIdx);
                //afxError err;
                //AfxAssert(unitUnlockerIdx == debugger.unitLockerIdx);
            }
        }
    }
    return rslt;
}

afxResult AfxLockDebugger(void)
{
    afxResult rslt;

    if (!debugger.running) rslt = 0;
    else
    {
        if (!(rslt = AfxLockMutex(&(debugger.mtx)))) printf("couldn't lock mutex");
        else
        {
            afxNat oldUnitIdx = debugger.unitLockerIdx;
            afxSystem sys;
            
            if ((sys = AfxGetSystem()))
            {
                AfxGetThreadingId(&debugger.unitLockerIdx);

                if (oldUnitIdx != debugger.unitLockerIdx)
                {
                    afxChar msg2[512];
                    stbsp_sprintf(msg2, "\n Entering the Thread Execution Environment %2u", debugger.unitLockerIdx);
                    _AfxLogFn(0xFFFF0000, msg2);
                }
            }

            debugger.isLocked = (afxBool)TRUE;
            //rslt = AfxUnlockMutex(&(debugger.mtx));
        }
    }
    return rslt;
}

afxBool AfxDebuggerIsAttached(void)
{
    return debugger.running;
}

afxResult AfxDetachDebugger(void)
{
    AfxEntry("");

    if (AfxLockDebugger())
    {
        AfxDetachDebugTerminal();

        if (debugger.dump)
        {
            //fclose(dbgCfg.dump);
            debugger.dump = NIL;
        }

        debugger.running = 0;
        AfxReleaseWaitCondition(&(debugger.cond));
        AfxCleanUpMutex(&(debugger.mtx));
    }
    return 0;
}

afxResult AfxAttachDebugger(afxChar const* file)
{
    AfxEntry("");
    (void)file;
    afxError err = AFX_ERR_NONE;
    if (debugger.running) AfxThrowError();
    else
    {
        AfxTakeMutex(&(debugger.mtx), AFX_MTX_RECURSIVE);
        AfxTakeWaitCondition(&(debugger.cond));
        debugger.isLocked = TRUE;

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
            
            AfxSprintf(path, "%s\\diag\\%s(%04i%02i%02i-%02i%02i%02i).log", sys, exeName, 1900 + ti->tm_year, ti->tm_mon, ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
            
            if (!(debugger.dump = fopen(path, "w+"))) AfxThrowError();
            else
            {
                //fclose(debugger.dump);

                //if (!(debugger.dump = fopen(path, "w+")))
                    //AfxThrowError();
            }
        }

        if (debugger.dump || !file[0])
        {
            //setbuf(debugger.dump);
            debugger.running = 1;

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

                for (afxNat i = 0; i < 1120; i++)
                    AfxLogMessageFormatted(0xFFFF0000, "%.*s", 1, &qwadroSignature.start[i * 1]);

                Sleep(1000);

                SetConsoleTextAttribute(debugger.conOutHnd, (WORD)(~(MAXUINT16) | FOREGROUND_INTENSITY));
                AfxUnlockDebugger();
                return TRUE;
            }
            debugger.running = 0;

            if (debugger.dump)
            {
                fclose(debugger.dump);
                debugger.dump = NIL;
            }
        }

        AfxReleaseWaitCondition(&(debugger.cond));
        AfxCleanUpMutex(&(debugger.mtx));
    }
    return FALSE;
}

_AFX void AfxCatchError_(afxError err_, afxHint const hint)
{
    afxError err = NIL;
    AfxAssert(err_);
    AfxAssert(hint);

    if (err_)
    {
        AfxLogError(hint, "");
        afxInt a = 0;
    }
}
