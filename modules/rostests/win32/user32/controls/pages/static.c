/*
 * PROJECT:     ReactOS Tests
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     User32 Control Gallery
 * COPYRIGHT:   Copyright 2026 Mohammad Amin Mollazadeh <madamin@pm.me>
 */

#include "../controls.h"

/*
 * +------------------------------------------------------+
 * | Left aligned text                                    |
 * |                Center aligned text                   |
 * |                                 Right aligned text   |
 * |                                                      |
 * | Prefix: E&xit              No Prefix: E&xit          |
 * |                                                      |
 * | +-----------------------------------------------+    |
 * | | Word-wrapped static text demonstrating        |    |
 * | | SS_LEFTNOWORDWRAP vs SS_LEFT                  |    |
 * | +-----------------------------------------------+    |
 * |                                                      |
 * | +----------------- Etched Horz -----------------+    |
 * | |  Etched Vert  |   Black Frame   | White Frame |    |
 * | |               |                 |             |    |
 * | |               |                 |             |    |
 * |                                                      |
 * | [ Black Rect ]  [ Gray Rect ]     [ White Rect ]     |
 * |                                                      |
 * | [ ICON ]            [ BITMAP ]                       |
 * +------------------------------------------------------+
 */

#define IDC_STATIC_NOTIFY 100001

typedef struct _STATIC_PAGE_DATA
{
    HWND hLeft;
    HWND hCenter;
    HWND hRight;

    HWND hNotify;
    HWND hNoPrefix;

    HWND hBlackRect;
    HWND hGrayRect;
    HWND hWhiteRect;

    HWND hBlackFrame;
    HWND hGrayFrame;
    HWND hWhiteFrame;

    HWND hEtchedFrame;
    HWND hEtchedHorz;
    HWND hEtchedVert;

    HWND hBitmapButton;
    HWND hIconButton;
} STATIC_PAGE_DATA, *PSTATIC_PAGE_DATA;

static
LRESULT
InitPage(HWND Parent, PSTATIC_PAGE_DATA PageData)
{
    PageData->hLeft =
        CreateChild(0,
                    L"STATIC",
                    L"Left aligned",
                    SS_LEFT,
                    20, 20, 180, 20,
                    Parent);

    PageData->hCenter =
        CreateChild(0,
                    L"STATIC",
                    L"Center aligned",
                    SS_CENTER,
                    220, 20, 180, 20,
                    Parent);

    PageData->hRight =
        CreateChild(0,
                    L"STATIC",
                    L"Right aligned",
                    SS_RIGHT,
                    420, 20, 180, 20,
                    Parent);

    PageData->hNotify =
        CreateChild(IDC_STATIC_NOTIFY,
                    L"STATIC",
                    L"Click me (SS_NOTIFY)",
                    SS_NOTIFY,
                    20, 60, 180, 20,
                    Parent);

    PageData->hNoPrefix =
        CreateChild(0,
                    L"STATIC",
                    L"E&xit",
                    SS_NOPREFIX,
                    220, 60, 180, 20,
                    Parent);

    PageData->hBlackRect =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_BLACKRECT,
                    20, 110, 60, 40,
                    Parent);

    PageData->hGrayRect =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_GRAYRECT,
                    100, 110, 60, 40,
                    Parent);

    PageData->hWhiteRect =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_WHITERECT,
                    180, 110, 60, 40,
                    Parent);

    PageData->hBlackFrame =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_BLACKFRAME,
                    280, 110, 60, 40,
                    Parent);

    PageData->hGrayFrame =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_GRAYFRAME,
                    360, 110, 60, 40,
                    Parent);

    PageData->hWhiteFrame =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_WHITEFRAME,
                    440, 110, 60, 40,
                    Parent);

    PageData->hEtchedFrame =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_ETCHEDFRAME,
                    20, 180, 100, 60,
                    Parent);

    PageData->hEtchedHorz =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_ETCHEDHORZ,
                    140, 200, 200, 2,
                    Parent);

    PageData->hEtchedVert =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_ETCHEDVERT,
                    370, 170, 2, 80,
                    Parent);

    PageData->hIconButton =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_ICON,
                    420, 180, 32, 32,
                    Parent);

    SendMessageW(PageData->hIconButton,
                 STM_SETICON,
                 (WPARAM)LoadIconW(NULL, IDI_INFORMATION),
                 0);

    PageData->hBitmapButton =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_BITMAP,
                    470, 180, 32, 32,
                    Parent);

    SendMessageW(PageData->hBitmapButton,
                 STM_SETIMAGE,
                 IMAGE_BITMAP,
                 (LPARAM)LoadBitmapW(NULL,
                                     MAKEINTRESOURCEW(OBM_CLOSE)));

    return TRUE;
}

LRESULT
StaticPageProc(PPAGE_HOST PageHost,
               UINT msg,
               WPARAM wParam,
               LPARAM lParam)
{
    PSTATIC_PAGE_DATA PageData;

    if (msg == WM_CREATE)
    {
        PageData = HeapAlloc(GetProcessHeap(),
                             HEAP_ZERO_MEMORY,
                             sizeof(*PageData));

        if (!PageData)
            return -1;

        PageHost->UserData = PageData;
        return InitPage(PageHost->Wnd, PageData);
    }

    PageData = (PSTATIC_PAGE_DATA)PageHost->UserData;

    switch (msg)
    {
        case WM_COMMAND:
            if (LOWORD(wParam) == IDC_STATIC_NOTIFY)
            {
                SetWindowTextW(PageData->hNotify,
                               L"Clicked!");
                return TRUE;
            }
            break;

        case WM_DESTROY:
            HeapFree(GetProcessHeap(), 0, PageData);
            PageHost->UserData = NULL;
            break;
    }

    return FALSE;
}
