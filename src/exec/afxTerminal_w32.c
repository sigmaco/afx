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
#if 0
#include <dwmapi.h>
#pragma comment (lib, "dwmapi")
#endif
#include "../../dep/stb_sprintf.h"
#include <stdio.h>
#include "src/impl/afxExecImplKit.h"

extern afxString const qwadroSignature;

static HWND ttyWnd = NIL;
static HANDLE ttyOut = NIL;
static FILE* ttyOutFd = NIL;
static afxAtomic ttyRefCnt = 0;

WORD ch[] =
{
    FOREGROUND_RED,
    FOREGROUND_INTENSITY | FOREGROUND_RED,
    FOREGROUND_GREEN | FOREGROUND_RED,
    FOREGROUND_GREEN,
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_BLUE,
    FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
    FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
};

_AFX afxResult AfxPrompt(afxString const* cur, afxString* buf)
{
    afxError err = { 0 };
    AfxReport(0, cur->start, cur->len);
    int bufsize = AfxGetStringCapacity(buf, 0);
    int position = 0;
    char *buffer = AfxGetStringStorage(buf, 0);
    int c;

    while (1)
    {
        // Read a character
        c = getchar();

        // If we hit EOF, replace it with a null character and return.
        if (c == EOF || c == '\n')
        {
            buffer[position] = '\0';
            return 0;
        }
        else
        {
            buffer[position] = c;
        }
        position++;

        // If we have exceeded the buffer, reallocate.
        if (position >= bufsize)
        {
            // Readjust buffer?
            AfxThrowError();
            break;
        }
    }
    buffer[position] = '\0';
    return position;
}

_AFX afxResult AfxPrint(afxUnit32 color, afxChar const* msg)
{
    afxResult rslt = 0;

    if (ttyRefCnt)
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

        SetConsoleTextAttribute(ttyOut, tattr);
        fputs(msg, ttyOutFd);
        SetConsoleTextAttribute(ttyOut, FOREGROUND_INTENSITY); // grey
        fflush(ttyOutFd);
        rslt = 1;
    }
    return rslt;
}

_AFX afxResult AfxPrintf(afxUnit32 color, afxChar const* msg, ...)
{
    afxResult rslt = 0;

    if (ttyRefCnt)
    {
        va_list args;
        va_start(args, msg);
        afxChar msg2[4096];
        stbsp_vsprintf(msg2, msg, args);
        va_end(args);
        rslt = AfxPrint(color, msg2);
    }
    return rslt;
}

_AFX afxResult AfxReleaseConsole(void)
{
    afxError err = { 0 };
    afxInt refCnt;

    if ((refCnt = --ttyRefCnt))
    {
        if (refCnt == 0)
        {
            FreeConsole();
            ttyOut = NIL;
            free(ttyOutFd);
            ttyOutFd = NIL;
        }
        else if (refCnt == AFX_I32_MAX)
            ttyRefCnt = 0;
    }
    return 0;
}
_AFX afxResult AfxReacquireConsole(void)
{
    afxError err = { 0 };
    afxInt refCnt;

    if ((refCnt = ++ttyRefCnt))
    {
        if (refCnt == 1)
        {
            ttyWnd = GetConsoleWindow();

            if (INVALID_HANDLE_VALUE == ttyWnd || NULL == ttyWnd)
                if (!AllocConsole())
                    AfxThrowError();

            ttyWnd = GetConsoleWindow();

            if (!ttyWnd) AfxThrowError();
            else
            {
#if 0
                DWM_BLURBEHIND bb = { 0 };
                bb.dwFlags = DWM_BB_ENABLE;
                bb.fEnable = TRUE;
                bb.hRgnBlur = NULL;
                DwmEnableBlurBehindWindow(debugger.conWnd, &(bb));
#endif
                ttyOutFd = freopen("CONOUT$", "w", stdout);

                if (!ttyOutFd) AfxThrowError();
                else
                {
                    ttyOut = GetStdHandle(STD_OUTPUT_HANDLE);
					COORD coord = GetLargestConsoleWindowSize(ttyOut);

                    afxChar buf[1024];
                    stbsp_sprintf(buf, "Terminal --- Qwadro Execution Ecosystem (c) 2017 SIGMA --- Public Test Build");
                    SetConsoleTitleA(buf);
#if 0 // disabled on Win10 to test why console was deformed
                    short fontSizX = 16, fontSizY = 16;
                    CONSOLE_FONT_INFOEX cfi = { .cbSize = sizeof(cfi),.nFont = 0,.dwFontSize = { fontSizX, fontSizY },.FontFamily = FF_DONTCARE,.FontWeight = FW_NORMAL };
                    wcscpy(cfi.FaceName, L"Consolas");
                    SetCurrentConsoleFontEx(ttyOut, FALSE, &cfi);

                    MoveWindow(ttyWnd, -4, -4, 80 * fontSizX, coord.Y * fontSizY, TRUE);
                    //ShowScrollBar(debugger.conWnd, SB_BOTH, FALSE);
#endif
                    coord.X = 80;
                    coord.Y = 5000;
                    SetConsoleScreenBufferSize(ttyOut, coord);

                    SetConsoleTextAttribute(ttyOut, (WORD)(~(MAXUINT16) | FOREGROUND_RED | FOREGROUND_GREEN));

                    for (afxUnit i = 0; i < 1120; i++)
                        AfxPrintf(0xFFFF0000, "%.*s", 1, &qwadroSignature.start[i * 1]);

                    AfxPrintf(0x00000000, "\a");
                    Sleep(250);

                    SetConsoleTextAttribute(ttyOut, (WORD)(~(MAXUINT16) | FOREGROUND_INTENSITY));
                }
            }
        }

        if (err)
            --ttyRefCnt;
    }
    return refCnt;
}
