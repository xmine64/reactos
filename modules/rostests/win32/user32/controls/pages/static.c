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
 * | SS_NOTIFY: Click me                                  |
 * | Prefix: E&xit              No Prefix: E&xit          |
 * | [ Black Rect ]    [ Gray Rect ]    [ White Rect ]    |
 * | [ Black Frame ]   [ Gray Frame ]   [ White Frame ]   |
 * | [ Etched Frame ]  [ Etched Horz ]  [ Etched Vert ]   |
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

    HWND hPrefix;
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
                    20, 20, 400, 20,
                    Parent);
    PageData->hCenter =
        CreateChild(0,
                    L"STATIC",
                    L"Center aligned",
                    SS_CENTER,
                    20, 40, 400, 20,
                    Parent);
    PageData->hRight =
        CreateChild(0,
                    L"STATIC",
                    L"Right aligned",
                    SS_RIGHT,
                    20, 60, 400, 20,
                    Parent);

    PageData->hNotify =
        CreateChild(IDC_STATIC_NOTIFY,
                    L"STATIC",
                    L"Click me (SS_NOTIFY)",
                    SS_NOTIFY,
                    20, 80, 180, 20,
                    Parent);

    PageData->hPrefix =
        CreateChild(0,
                    L"STATIC",
                    L"Prefix: E&xit",
                    SS_SIMPLE,
                    20, 100, 180, 20,
                    Parent);
    PageData->hNoPrefix =
        CreateChild(0,
                    L"STATIC",
                    L"NoPrefix: E&xit",
                    SS_NOPREFIX,
                    220, 100, 180, 20,
                    Parent);

    PageData->hBlackRect =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_BLACKRECT,
                    20, 140, 40, 40,
                    Parent);
    PageData->hGrayRect =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_GRAYRECT,
                    80, 140, 40, 40,
                    Parent);
    PageData->hWhiteRect =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_WHITERECT,
                    140, 140, 40, 40,
                    Parent);

    PageData->hBlackFrame =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_BLACKFRAME,
                    200, 140, 40, 40,
                    Parent);
    PageData->hGrayFrame =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_GRAYFRAME,
                    260, 140, 40, 40,
                    Parent);
    PageData->hWhiteFrame =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_WHITEFRAME,
                    320, 140, 40, 40,
                    Parent);

    PageData->hEtchedFrame =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_ETCHEDFRAME,
                    20, 200, 100, 100,
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
                    140, 220, 2, 80,
                    Parent);

    PageData->hIconButton =
        CreateChild(0,
                    L"STATIC",
                    NULL,
                    SS_ICON,
                    20, 320, 32, 32,
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
                    80, 320, 32, 32,
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
