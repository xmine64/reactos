/*
 * PROJECT:     ReactOS Tests
 * LICENSE:     MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:     User32 Control Gallery
 * COPYRIGHT:   Copyright 2026 Mohammad Amin Mollazadeh <madamin@pm.me>
 */

#pragma once

#define OEMRESOURCE
#include <windows.h>

typedef enum _PAGES
{
    PAGE_BUTTONS,
    PAGE_COMBO,
    PAGE_EDIT,
    PAGE_ICONTITLE,
    PAGE_LISTBOX,
    PAGE_MDI,
    PAGE_MENU,
    PAGE_SCROLLBAR,
    PAGE_MAX
} PAGE_ID;

typedef struct _PAGE_HOST* PPAGE_HOST;
typedef LRESULT (*PFN_PAGE_PROC)(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct _PAGE
{
    PAGE_ID Id;
    LPCWSTR Title;
    PFN_PAGE_PROC PageProc;
} PAGE, *PPAGE;

typedef struct _PAGE_HOST
{
    HWND Wnd;
    HWND WndParent;
    PPAGE PageData;
    PVOID UserData;
} PAGE_HOST, *PPAGE_HOST;

typedef struct _NAVIGATOR
{
    HWND WndParent;
    RECT ClientRect;
    HWND WndNavigationList;
    UINT CurrentPage;
    UINT PageCount;
    PPAGE_HOST Pages[PAGE_MAX];
} NAVIGATOR, *PNAVIGATOR;

/* Helper function */
static inline
HWND
CreateChild(
    int Id,
    LPCTSTR ClassName,
    LPCTSTR Text,
    DWORD Style,
    int x,
    int y,
    int cx,
    int cy,
    HWND Parent)
{
    return CreateWindowEx(
        0,
        ClassName,
        Text,
        WS_CHILD | WS_VISIBLE | Style,
        x, y, cx, cy,
        Parent,
        (HMENU)(INT_PTR)Id,
        GetModuleHandle(NULL),
        NULL);
}

/* navigation */
#define IDC_NAVIGATION_LIST 1000
#define IDC_NAVIGATION_PAGE_HOST 1001

PNAVIGATOR CreateNavigationHost(HINSTANCE hInst, HWND Parent, PPAGE Pages, UINT PageCount);
void NavigateTo(PNAVIGATOR Nav, PAGE_ID Id);
void UpdateSize(PNAVIGATOR Nav);
void DestroyNavigationHost(PNAVIGATOR Nav);

PPAGE_HOST CreatePageHost(HINSTANCE hInst, HWND Parent, int x, int y, int cx, int cy, PPAGE PageData);
LRESULT CallPageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);
void ShowPage(PPAGE_HOST PageHost);
void HidePage(PPAGE_HOST PageHost);
void DestroyPageHost(PPAGE_HOST PageHost);

/* button page */
#define IDC_BUTTON_PAGE_CONTAINER 2000

LRESULT ButtonPageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);

/* edit page */
#define IDC_EDIT_PAGE_CONTAINER 3000

LRESULT EditPageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);

/* combo page */
#define IDC_COMBO_PAGE_CONTAINER 4000

LRESULT ComboPageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);

/* icontitle page */
#define IDC_ICONTITLE_PAGE_CONTAINER 5000

LRESULT IconTitlePageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);

/* list box page */
#define IDC_LISTBOX_PAGE_CONTAINER 6000

LRESULT ListBoxPageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);

/* MDI page */
#define IDC_MDI_PAGE_CONTAINER 7000

LRESULT MdiPageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);

/* menu page */
#define IDC_MENU_PAGE_CONTAINER 8000

LRESULT MenuPageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);

/* scrollbar page */
#define IDC_SCROLLBAR_PAGE_CONTAINER 9000

LRESULT ScrollBarPageProc(PPAGE_HOST PageHost, UINT msg, WPARAM wParam, LPARAM lParam);
