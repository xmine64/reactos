/*
 * PROJECT:     ReactOS Tests
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     User32 Control Gallery
 * COPYRIGHT:   Copyright 2026 Mohammad Amin Mollazadeh <madamin@pm.me>
 */

#include "../controls.h"

/*
 * +------------------------------------------------+
 * | Single Selection                               |
 * | +--------------------------------------------+ |
 * | | Item 1                                     | |
 * | | Item 2                                     | |
 * | | Item 3                                     | |
 * | +--------------------------------------------+ |
 * |                                                |
 * | Multiple Selection                            |
 * | +--------------------------------------------+ |
 * | | [ ] Item A                                 | |
 * | | [x] Item B                                 | |
 * | | [x] Item C                                 | |
 * | +--------------------------------------------+ |
 * |                                                |
 * | Extended Selection                             |
 * | +--------------------------------------------+ |
 * | | Item 1                                     | |
 * | | Item 2                                     | |
 * | | Item 3                                     | |
 * | +--------------------------------------------+ |
 * +------------------------------------------------+
 */

typedef struct _LISTBOX_PAGE_DATA
{
    HWND hSingle;
    HWND hMultiple;
    HWND hExtended;
} LISTBOX_PAGE_DATA, *PLISTBOX_PAGE_DATA;

LRESULT
InitPage(HWND Parent, PLISTBOX_PAGE_DATA PageData)
{
    PageData->hSingle =
        CreateChild(0, L"LISTBOX", NULL, LBS_STANDARD, 20, 20, 200, 100, Parent);
    SendMessageW(PageData->hSingle, LB_ADDSTRING, 0, (LPARAM)L"Item 1");
    SendMessageW(PageData->hSingle, LB_ADDSTRING, 0, (LPARAM)L"Item 2");
    SendMessageW(PageData->hSingle, LB_ADDSTRING, 0, (LPARAM)L"Item 3");

    PageData->hMultiple =
        CreateChild(0, L"LISTBOX", NULL, LBS_STANDARD | LBS_MULTIPLESEL, 20, 140, 200, 100, Parent);
    SendMessageW(PageData->hMultiple, LB_ADDSTRING, 0, (LPARAM)L"Item A");
    SendMessageW(PageData->hMultiple, LB_ADDSTRING, 0, (LPARAM)L"Item B");
    SendMessageW(PageData->hMultiple, LB_ADDSTRING, 0, (LPARAM)L"Item C");

    PageData->hExtended =
        CreateChild(0, L"LISTBOX", NULL, LBS_STANDARD | LBS_EXTENDEDSEL, 240, 140, 200, 100, Parent);
    SendMessageW(PageData->hExtended, LB_ADDSTRING, 0, (LPARAM)L"Item 1");
    SendMessageW(PageData->hExtended, LB_ADDSTRING, 0, (LPARAM)L"Item 2");
    SendMessageW(PageData->hExtended, LB_ADDSTRING, 0, (LPARAM)L"Item 3");

    return TRUE;
}

LRESULT
ListBoxPageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
        {
            PLISTBOX_PAGE_DATA PageData;

            PageData = HeapAlloc(GetProcessHeap(),
                                 HEAP_ZERO_MEMORY,
                                 sizeof(*PageData));

            if (!PageData)
                return FALSE;

            PageHost->UserData = PageData;
            return InitPage(PageHost->Wnd, PageData);
        }

        case WM_DESTROY:
        {
            PLISTBOX_PAGE_DATA PageData = (PLISTBOX_PAGE_DATA)PageHost->UserData;
            HeapFree(GetProcessHeap(), 0, PageData);
            return TRUE;
        }
    }

    return FALSE;
}
