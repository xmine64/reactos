#pragma once

/* Unimplemented */

#define GetDpiForWindow(HWND) 96

#define NtUserGetPrivateData(HWND, OFFSET, SIZE) GetWindowLongPtrW((HWND), (OFFSET)); C_ASSERT((SIZE) == sizeof(LONG))
#define NtUserSetPrivateData(HWND, OFFSET, SIZE, VALUE) SetWindowLongPtrW((HWND), (OFFSET), (VALUE)); C_ASSERT((SIZE) == sizeof(LONG))

/* NtUser to NtUserx map */

#define NtUserReleaseDC(HWND, HDC) NtUserxReleaseDC((HDC))
#define NtUserReleaseCapture() NtUserxReleaseCapture()
#define NtUserKillSystemTimer(HWND, ID) NtUserxKillSystemTimer((HWND), (ID))
#define NtUserEnableWindow(HWND, ENABLE) NtUserxEnableWindow((HWND), (ENABLE))

/* WINE calls NtUserSetSystemTimer with 3 parameters instead of 4 */
#define NtUserSetSystemTimer(HWND, ID, TIMEOUT) NtUserSetSystemTimer((HWND), (ID), (TIMEOUT), NULL)

/* NtUser to User32 map */

/* DrawIconEx has 9 parameters and NtUserDrawIconEx has 11, WINE calls NtUserDrawIconEx with 9 arguments */
#define NtUserDrawIconEx(P1, P2, P3, P4, P5, P6, P7, P8, P9) DrawIconEx((P1), (P2), (P3), (P4), (P5), (P6), (P7), (P8), (P9))

/* standard C */

#define malloc(size) HeapAlloc(GetProcessHeap(), 0, (size))
#define calloc(count, size) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (count) * (size))
#define realloc(ptr, size) ptr ? HeapReAlloc(GetProcessHeap(), 0, (ptr), (size)) : HeapAlloc(GetProcessHeap(), 0, (size))
#define free(ptr) HeapFree(GetProcessHeap(), 0, (ptr))

/* MAKE_FNID */

#define NTUSER_WNDPROC_SCROLLBAR    FNID_SCROLLBAR
#define NTUSER_WNDPROC_MENU         FNID_MENU
#define NTUSER_WNDPROC_DESKTOP      FNID_DESKTOP
#define NTUSER_WNDPROC_ICONTITLE    FNID_ICONTITLE
#define NTUSER_WNDPROC_BUTTON       FNID_BUTTON
#define NTUSER_WNDPROC_COMBO        FNID_COMBOBOX
#define NTUSER_WNDPROC_COMBOLBOX    FNID_COMBOLBOX
#define NTUSER_WNDPROC_DIALOG       FNID_DIALOG
#define NTUSER_WNDPROC_EDIT         FNID_EDIT
#define NTUSER_WNDPROC_LISTBOX      FNID_LISTBOX
#define NTUSER_WNDPROC_MDICLIENT    FNID_MDICLIENT
#define NTUSER_WNDPROC_STATIC       FNID_STATIC
#define NTUSER_WNDPROC_IME          FNID_IME
#define NTUSER_WNDPROC_GHOST        FNID_GHOST

#define MAKE_FNID(index) (index)
