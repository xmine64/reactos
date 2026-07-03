/*
 * PROJECT:         ReactOS api tests
 * LICENSE:         MIT (https://spdx.org/licenses/MIT)
 * PURPOSE:         Test for SetSystemVisualStyle
 * COPYRIGHT:       Copyright 2026 Mohammad Amin Mollazadeh <madamin@pm.me>
 */

#include <apitest.h>
#include <windows.h>
#include <uxtheme.h>
#include <uxundoc.h>
#include <tmschema.h>

typedef struct _SPI_ENTRY
{
    PCWSTR Name;
    int Id;
} SPI_ENTRY;

typedef struct _SPI_TMT_ENTRY
{
    PCWSTR Name;
    int Id;
    int TmtId;
} SPI_TMT_ENTRY;

static const SPI_ENTRY svsMetrics[] =
{
    { L"SM_CXVSCROLL", SM_CXVSCROLL },
    { L"SM_CYVSCROLL", SM_CYVSCROLL },
    { L"SM_CXHSCROLL", SM_CXHSCROLL },
    { L"SM_CYHSCROLL", SM_CYHSCROLL },

    { L"SM_CXBORDER", SM_CXBORDER },
    { L"SM_CYBORDER", SM_CYBORDER },

    { L"SM_CYCAPTION", SM_CYCAPTION },
    { L"SM_CXSIZE", SM_CXSIZE },
    { L"SM_CYSIZE", SM_CYSIZE },

    { L"SM_CYMENU", SM_CYMENU },

    { L"SM_CXFRAME", SM_CXFRAME },
    { L"SM_CYFRAME", SM_CYFRAME },
    { L"SM_CXSMSIZE", SM_CXSMSIZE },
    { L"SM_CYSMSIZE", SM_CYSMSIZE },
    { L"SM_CXMENUSIZE", SM_CXMENUSIZE },
    { L"SM_CYMENUSIZE", SM_CYMENUSIZE },
};

static const SPI_ENTRY svsColors[] =
{
    { L"COLOR_SCROLLBAR", COLOR_SCROLLBAR },
    { L"COLOR_BACKGROUND", COLOR_BACKGROUND },
    { L"COLOR_ACTIVECAPTION", COLOR_ACTIVECAPTION },
    { L"COLOR_INACTIVECAPTION", COLOR_INACTIVECAPTION },
    { L"COLOR_MENU", COLOR_MENU },
    { L"COLOR_WINDOW", COLOR_WINDOW },
    { L"COLOR_WINDOWFRAME", COLOR_WINDOWFRAME },
    { L"COLOR_MENUTEXT", COLOR_MENUTEXT },
    { L"COLOR_WINDOWTEXT", COLOR_WINDOWTEXT },
    { L"COLOR_CAPTIONTEXT", COLOR_CAPTIONTEXT },
    { L"COLOR_ACTIVEBORDER", COLOR_ACTIVEBORDER },
    { L"COLOR_INACTIVEBORDER", COLOR_INACTIVEBORDER },
    { L"COLOR_APPWORKSPACE", COLOR_APPWORKSPACE },
    { L"COLOR_HIGHLIGHT", COLOR_HIGHLIGHT },
    { L"COLOR_HIGHLIGHTTEXT", COLOR_HIGHLIGHTTEXT },
    { L"COLOR_BTNFACE", COLOR_BTNFACE },
    { L"COLOR_BTNSHADOW", COLOR_BTNSHADOW },
    { L"COLOR_GRAYTEXT", COLOR_GRAYTEXT },
    { L"COLOR_BTNTEXT", COLOR_BTNTEXT },
    { L"COLOR_INACTIVECAPTIONTEXT", COLOR_INACTIVECAPTIONTEXT },
    { L"COLOR_BTNHIGHLIGHT", COLOR_BTNHIGHLIGHT }
};

static const SPI_TMT_ENTRY svsFonts[] =
{
    { L"SPI_GETICONTITLELOGFONT", SPI_GETICONTITLELOGFONT, TMT_CAPTIONFONT }
};

typedef struct _ENUM_THEME_CONTEXT
{
    WCHAR ThemeFile[MAX_PATH];
    BOOL Found;
} ENUM_THEME_CONTEXT;

static BOOL
CALLBACK
SetSystemVisualStyle_EnumThemeProc(
    LPVOID lpReserved,
    LPCWSTR pszThemeFileName,
    LPCWSTR pszThemeName,
    LPCWSTR pszToolTip,
    LPVOID lpReserved2,
    LPVOID lpData)
{
    ENUM_THEME_CONTEXT *ctx = (ENUM_THEME_CONTEXT*)lpData;

    lstrcpyW(ctx->ThemeFile, pszThemeFileName);
    ctx->Found = TRUE;
    skip("Theme found: %wS (%wS)\n", pszThemeFileName, pszThemeName);
    return FALSE;
}

static BOOL SetSystemVisualStyle_FindTheme(LPWSTR pszThemeFile, LPWSTR pszColor, LPWSTR pszSize)
{
    ENUM_THEME_CONTEXT EnumThemeContext = {0};
    THEMENAMES ThemeNames;

    EnumThemes(NULL, SetSystemVisualStyle_EnumThemeProc, &EnumThemeContext);
    if (EnumThemeContext.Found == FALSE)
    {
        skip("No theme file found to test against\n");
        return FALSE;
    }

    if (EnumThemeColors(EnumThemeContext.ThemeFile, NULL, 0, &ThemeNames) != S_OK)
    {
        skip("No theme colors found to test against\n");
        return FALSE;
    }

    if (EnumThemeSizes(EnumThemeContext.ThemeFile, NULL, 0, &ThemeNames) != S_OK)
    {
        skip("No theme sizes found to test against\n");
        return FALSE;
    }

    lstrcpyW(pszThemeFile, EnumThemeContext.ThemeFile);
    lstrcpyW(pszColor, ThemeNames.szName);
    lstrcpyW(pszSize, ThemeNames.szName);

    return TRUE;
}

START_TEST(SetSystemVisualStyle)
{
    HRESULT hr;
    HTHEMEFILE hThemeFile = NULL;
    HTHEME hTheme = NULL;
    
    WCHAR ThemeName[MAX_PATH] = {0};
    WCHAR ColorScheme[MAX_PATH] = {0};
    WCHAR SizeScheme[MAX_PATH] = {0};

    WCHAR oldTheme[MAX_PATH] = {0};
    WCHAR oldColor[MAX_PATH] = {0};
    WCHAR oldSize[MAX_PATH] = {0};

    UINT Flags = UXTAPPLYFLAG_LOADSYSTEMMETRICS |
                 UXTAPPLYFLAG_APPLYSYSTEMMETRICS;

    LOGFONTW ThemeFont = {0}, SystemFont = {0};
    NONCLIENTMETRICSW NonClientMetrics = {0};

    /* Backup the current theme, to restore at the end of the test */
    GetCurrentThemeName(oldTheme, MAX_PATH,
                        oldColor, MAX_PATH,
                        oldSize, MAX_PATH);

    /* Find a theme to run the test against */
    if (!SetSystemVisualStyle_FindTheme(ThemeName, ColorScheme, SizeScheme))
    {
        skip("No theme found to test against\n");
        goto cleanup;
    }

    /* Apply Windows Classic theme first */
    SetSystemVisualStyle(NULL, NULL, NULL, Flags);

    /* Apply test theme */
    hr = SetSystemVisualStyle(ThemeName, ColorScheme, SizeScheme, Flags);
    if (hr != S_OK)
    {
        skip("SetSystemVisualStyle failed: 0x%lx\n", hr);
        goto cleanup;
    }

    /* Open theme data */
    hr = OpenThemeFile(ThemeName, ColorScheme, SizeScheme, &hThemeFile, 0);
    if (hr != S_OK)
    {
        skip("OpenThemeFile failed: 0x%lx\n", hr);
        goto cleanup;
    }
    hTheme = OpenThemeDataFromFile(hThemeFile, NULL, L"globals", 0);
    if (!hTheme)
    {
        skip("OpenThemeDataFromFile failed\n");
        goto cleanup;
    }

    /* Verify system metrics */
    for (int i = 0; i < _countof(svsMetrics); i++)
    {
        int themeVal = GetThemeSysSize(hTheme, svsMetrics[i].Id);
        int sysVal = GetSystemMetrics(svsMetrics[i].Id);

        ok(themeVal == sysVal,
           "%ls mismatch: theme=%d sys=%d\n",
           svsMetrics[i].Name, themeVal, sysVal);
    }

    /* Verify system colors */
    for (int i = 0; i < _countof(svsColors); i++)
    {
        COLORREF themeColor = GetThemeSysColor(hTheme, svsColors[i].Id);
        COLORREF sysColor = GetSysColor(svsColors[i].Id);

        ok(themeColor == sysColor,
           "%ls mismatch: theme=%08lx sys=%08lx\n",
           svsColors[i].Name, themeColor, sysColor);
    }

    /* Verify fonts */
    for (int i = 0; i < _countof(svsFonts); i++)
    {
        hr = GetThemeSysFont(hTheme, svsFonts[i].TmtId, &ThemeFont);
        ok(hr == S_OK, "GetThemeSysFont failed: 0x%lx\n", hr);

        SystemParametersInfoW(svsFonts[i].Id,
                              sizeof(SystemFont),
                              &SystemFont,
                              0);

        ok(memcmp(&ThemeFont, &SystemFont, sizeof(LOGFONTW)) == 0,
           "%ls mismatch\n", svsFonts[i].Name);
    }

    SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, sizeof(NonClientMetrics), &NonClientMetrics, 0);

    GetThemeSysFont(hTheme, TMT_CAPTIONFONT, &ThemeFont);
    ok(memcmp(&ThemeFont, &NonClientMetrics.lfCaptionFont, sizeof(LOGFONTW)) == 0,
       "Caption font mismatch\n");

    GetThemeSysFont(hTheme, TMT_SMALLCAPTIONFONT, &ThemeFont);
    ok(memcmp(&ThemeFont, &NonClientMetrics.lfSmCaptionFont, sizeof(LOGFONTW)) == 0,
       "Small caption font mismatch\n");

    GetThemeSysFont(hTheme, TMT_MENUFONT, &ThemeFont);
    ok(memcmp(&ThemeFont, &NonClientMetrics.lfMenuFont, sizeof(LOGFONTW)) == 0,
       "Menu font mismatch\n");

    GetThemeSysFont(hTheme, TMT_STATUSFONT, &ThemeFont);
    ok(memcmp(&ThemeFont, &NonClientMetrics.lfStatusFont, sizeof(LOGFONTW)) == 0,
       "Status font mismatch\n");

    GetThemeSysFont(hTheme, TMT_MSGBOXFONT, &ThemeFont);
    ok(memcmp(&ThemeFont, &NonClientMetrics.lfMessageFont, sizeof(LOGFONTW)) == 0,
       "Message Box font mismatch\n");

cleanup:
    /* Restore user theme */
    SetSystemVisualStyle(oldTheme, oldColor, oldSize, Flags);

    if (hTheme)
        CloseThemeData(hTheme);

    if (hThemeFile)
        CloseThemeFile(hThemeFile);
}
